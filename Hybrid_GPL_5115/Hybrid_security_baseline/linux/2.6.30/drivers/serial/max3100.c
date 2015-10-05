/*
 *
 *  Copyright (C) 2008 Christian Pellegrin <chripell@evolware.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *
 * Notes: the MAX3100 doesn't provide an interrupt on CTS so we have
 * to use polling for flow control. TX empty IRQ is unusable, since
 * writing conf clears FIFO buffer and we cannot have this interrupt
 * always asking us for attention.
 *
 * Example platform data:

 static struct plat_max3100 max3100_plat_data = {
 .loopback = 0,
 .crystal = 0,
 .poll_time = 100,
 };

 static struct spi_board_info spi_board_info[] = {
 {
 .modalias	= "max3100",
 .platform_data	= &max3100_plat_data,
 .irq		= IRQ_EINT12,
 .max_speed_hz	= 5*1000*1000,
 .chip_select	= 0,
 },
 };

 * The initial minor number is 209 in the low-density serial port:
 * mknod /dev/ttyMAX0 c 204 209
 */

#define MAX3100_MAJOR 204
#define MAX3100_MINOR 209
/* 4 MAX3100s should be enough for everyone */
#define MAX_MAX3100 4

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/spi/spi.h>
#include <linux/freezer.h>

#include <linux/console.h>

#include <linux/serial_max3100.h>

#define MAX3100_C    (1<<14)
#define MAX3100_D    (0<<14)
#define MAX3100_W    (1<<15)
#define MAX3100_RX   (0<<15)

#define MAX3100_WC   (MAX3100_W  | MAX3100_C)
#define MAX3100_RC   (MAX3100_RX | MAX3100_C)
#define MAX3100_WD   (MAX3100_W  | MAX3100_D)
#define MAX3100_RD   (MAX3100_RX | MAX3100_D)
#define MAX3100_CMD  (3 << 14)

#define MAX3100_T    (1<<14)
#define MAX3100_R    (1<<15)

#define MAX3100_FEN  (1<<13)
#define MAX3100_SHDN (1<<12)
#define MAX3100_TM   (1<<11)
#define MAX3100_RM   (1<<10)
#define MAX3100_PM   (1<<9)
#define MAX3100_RAM  (1<<8)
#define MAX3100_IR   (1<<7)
#define MAX3100_ST   (1<<6)
#define MAX3100_PE   (1<<5)
#define MAX3100_L    (1<<4)
#define MAX3100_BAUD (0xf)

#define MAX3100_TE   (1<<10)
#define MAX3100_RAFE (1<<10)
#define MAX3100_RTS  (1<<9)
#define MAX3100_CTS  (1<<9)
#define MAX3100_PT   (1<<8)
#define MAX3100_DATA (0xff)

#define MAX3100_RT   (MAX3100_R | MAX3100_T)
#define MAX3100_RTC  (MAX3100_RT | MAX3100_CTS | MAX3100_RAFE)

/* the following simulate a status reg for ignore_status_mask */
#define MAX3100_STATUS_PE 1
#define MAX3100_STATUS_FE 2
#define MAX3100_STATUS_OE 4

struct max3100_port {
	struct uart_port port;
	struct spi_device *spi;

	int cts;	        /* last CTS received for flow ctrl */
	int tx_empty;		/* last TX empty bit */

	spinlock_t conf_lock;	/* shared data */
	int conf_commit;	/* need to make changes */
	int conf;		/* configuration for the MAX31000
				 * (bits 0-7, bits 8-11 are irqs) */
	int rts_commit;	        /* need to change rts */
	int rts;		/* rts status */
	int baud;		/* current baud rate */

	int parity;		/* keeps track if we should send parity */
#define MAX3100_PARITY_ON 1
#define MAX3100_PARITY_ODD 2
#define MAX3100_7BIT 4
	int rx_enabled;	        /* if we should rx chars */

	int irq;		/* irq assigned to the max3100 */

	int minor;		/* minor number */
	int crystal;		/* 1 if 3.6864Mhz crystal 0 for 1.8432 */
	int loopback;		/* 1 if we are in loopback mode */

	/* for handling irqs: need workqueue since we do spi_sync */
	struct workqueue_struct *workqueue;
	struct work_struct work;
	/* set to 1 to make the workhandler exit as soon as possible */
	int  force_end_work;
	/* need to know we are suspending to avoid deadlock on workqueue */
	int suspending;

	/* hook for suspending MAX3100 via dedicated pin */
	void (*max3100_hw_suspend) (int suspend);

	/* poll time (in ms) for ctrl lines */
	int poll_time;
 	/* and its timer */
 	struct timer_list	timer;
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE
	int console_flags;
	/* is this port a console? */
#define MAX3100_IS_CONSOLE   (1 << 0)
	/* are we in suspend/resume? */
#define MAX3100_SUSPENDING   (1 << 1)
	/* console buffer */
#define CONSOLE_BUF_SIZE 4096
	char *console_buf;
	int console_head, console_tail;
	/* delayed console work because we may have to sleep */
	struct work_struct console_work;
	/* char tx timeout */
	int console_tout;
#endif
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
};

static struct max3100_port *max3100s[MAX_MAX3100]; /* the chips */
static DEFINE_MUTEX(max3100s_lock);		   /* race on probe */

static int max3100_do_parity(struct max3100_port *s, u16 c)
{
	int parity;

	if (s->parity & MAX3100_PARITY_ODD)
		parity = 1;
	else
		parity = 0;

	if (s->parity & MAX3100_7BIT)
		c &= 0x7f;
	else
		c &= 0xff;

	parity = parity ^ (hweight8(c) & 1);
	return parity;
}

static int max3100_check_parity(struct max3100_port *s, u16 c)
{
	return max3100_do_parity(s, c) == ((c >> 8) & 1);
}

static void max3100_calc_parity(struct max3100_port *s, u16 *c)
{
	if (s->parity & MAX3100_7BIT)
		*c &= 0x7f;
	else
		*c &= 0xff;

	if (s->parity & MAX3100_PARITY_ON)
		*c |= max3100_do_parity(s, *c) << 8;
}

static void max3100_work(struct work_struct *w);

static void max3100_dowork(struct max3100_port *s)
{
	if (!s->force_end_work && !work_pending(&s->work) &&
	    !freezing(current) && !s->suspending)
		queue_work(s->workqueue, &s->work);
}

static void max3100_timeout(unsigned long data)
{
	struct max3100_port *s = (struct max3100_port *)data;

	if (s->port.info) {
		max3100_dowork(s);
		mod_timer(&s->timer, jiffies + s->poll_time);
	}
}

static int max3100_sr(struct max3100_port *s, u16 tx, u16 *rx)
{
	struct spi_message message;
	u16 etx, erx;
	int status;
	struct spi_transfer tran = {
		.tx_buf = &etx,
		.rx_buf = &erx,
		.len = 2,
	};

	etx = cpu_to_be16(tx);
	spi_message_init(&message);
	spi_message_add_tail(&tran, &message);
	status = spi_sync(s->spi, &message);
	if (status) {
		dev_warn(&s->spi->dev, "error while calling spi_sync\n");
		return -EIO;
	}
	*rx = be16_to_cpu(erx);
	s->tx_empty = (*rx & MAX3100_T) > 0;
	dev_dbg(&s->spi->dev, "%04x - %04x\n", tx, *rx);
	return 0;
}

static int max3100_handlerx(struct max3100_port *s, u16 rx)
{
	unsigned int ch, flg, status = 0;
	int ret = 0, cts;

	if (rx & MAX3100_R && s->rx_enabled) {
		dev_dbg(&s->spi->dev, "%s\n", __func__);
		ch = rx & (s->parity & MAX3100_7BIT ? 0x7f : 0xff);
		if (rx & MAX3100_RAFE) {
			s->port.icount.frame++;
			flg = TTY_FRAME;
			status |= MAX3100_STATUS_FE;
		} else {
			if (s->parity & MAX3100_PARITY_ON) {
				if (max3100_check_parity(s, rx)) {
					s->port.icount.rx++;
					flg = TTY_NORMAL;
				} else {
					s->port.icount.parity++;
					flg = TTY_PARITY;
					status |= MAX3100_STATUS_PE;
				}
			} else {
				s->port.icount.rx++;
				flg = TTY_NORMAL;
			}
		}
		uart_insert_char(&s->port, status, MAX3100_STATUS_OE, ch, flg);
		ret = 1;
	}

	cts = (rx & MAX3100_CTS) > 0;
	if (s->cts != cts) {
		s->cts = cts;
		uart_handle_cts_change(&s->port, cts ? TIOCM_CTS : 0);
	}

	return ret;
}

static void max3100_work(struct work_struct *w)
{
	struct max3100_port *s = container_of(w, struct max3100_port, work);
	int rxchars;
	u16 tx, rx;
	int conf, cconf, rts, crts;
	struct circ_buf *xmit = &s->port.info->xmit;

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	rxchars = 0;
	do {
		spin_lock(&s->conf_lock);
		conf = s->conf;
		cconf = s->conf_commit;
		s->conf_commit = 0;
		rts = s->rts;
		crts = s->rts_commit;
		s->rts_commit = 0;
		spin_unlock(&s->conf_lock);
		if (cconf)
			max3100_sr(s, MAX3100_WC | conf, &rx);
		if (crts) {
			max3100_sr(s, MAX3100_WD | MAX3100_TE |
				   (s->rts ? MAX3100_RTS : 0), &rx);
			rxchars += max3100_handlerx(s, rx);
		}

		max3100_sr(s, MAX3100_RD, &rx);
		rxchars += max3100_handlerx(s, rx);

		if (rx & MAX3100_T) {
			tx = 0xffff;
			if (s->port.x_char) {
				tx = s->port.x_char;
				s->port.icount.tx++;
				s->port.x_char = 0;
			} else if (!uart_circ_empty(xmit) &&
				   !uart_tx_stopped(&s->port)) {
				tx = xmit->buf[xmit->tail];
				xmit->tail = (xmit->tail + 1) &
					(UART_XMIT_SIZE - 1);
				s->port.icount.tx++;
			}
			if (tx != 0xffff) {
				max3100_calc_parity(s, &tx);
				tx |= MAX3100_WD | (s->rts ? MAX3100_RTS : 0);
				max3100_sr(s, tx, &rx);
				rxchars += max3100_handlerx(s, rx);
			}
		}

		if (rxchars > 16 && s->port.info->port.tty != NULL) {
			tty_flip_buffer_push(s->port.info->port.tty);
			rxchars = 0;
		}
		if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
			uart_write_wakeup(&s->port);

	} while (!s->force_end_work &&
		 !freezing(current) &&
		 ((rx & MAX3100_R) ||
		  (!uart_circ_empty(xmit) &&
		   !uart_tx_stopped(&s->port))));

	if (rxchars > 0 && s->port.info->port.tty != NULL)
		tty_flip_buffer_push(s->port.info->port.tty);
}

static irqreturn_t max3100_irq(int irqno, void *dev_id)
{
	struct max3100_port *s = dev_id;

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	max3100_dowork(s);
	return IRQ_HANDLED;
}

static void max3100_enable_ms(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	if (s->poll_time > 0)
		mod_timer(&s->timer, jiffies);
	dev_dbg(&s->spi->dev, "%s\n", __func__);
}

static void max3100_start_tx(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	max3100_dowork(s);
}

static void max3100_stop_rx(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	s->rx_enabled = 0;
	spin_lock(&s->conf_lock);
	s->conf &= ~MAX3100_RM;
	s->conf_commit = 1;
	spin_unlock(&s->conf_lock);
	max3100_dowork(s);
}

static unsigned int max3100_tx_empty(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	/* may not be truly up-to-date */
	max3100_dowork(s);
	return s->tx_empty;
}

static unsigned int max3100_get_mctrl(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	/* may not be truly up-to-date */
	max3100_dowork(s);
	/* always assert DCD and DSR since these lines are not wired */
	return (s->cts ? TIOCM_CTS : 0) | TIOCM_DSR | TIOCM_CAR;
}

static void max3100_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);
	int rts;

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	rts = (mctrl & TIOCM_RTS) > 0;

	spin_lock(&s->conf_lock);
	if (s->rts != rts) {
		s->rts = rts;
		s->rts_commit = 1;
		max3100_dowork(s);
	}
	spin_unlock(&s->conf_lock);
}

static void
max3100_set_termios(struct uart_port *port, struct ktermios *termios,
		    struct ktermios *old)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);
	int baud = 0;
	unsigned cflag;
	u32 param_new, param_mask, parity = 0;
	struct tty_struct *tty = s->port.info->port.tty;

	dev_dbg(&s->spi->dev, "%s\n", __func__);
	if (!tty)
		return;

	cflag = termios->c_cflag;
	param_new = 0;
	param_mask = 0;

	baud = tty_get_baud_rate(tty);
	param_new = s->conf & MAX3100_BAUD;
	switch (baud) {
	case 300:
		if (s->crystal)
			baud = s->baud;
		else
			param_new = 15;
		break;
	case 600:
		param_new = 14 + s->crystal;
		break;
	case 1200:
		param_new = 13 + s->crystal;
		break;
	case 2400:
		param_new = 12 + s->crystal;
		break;
	case 4800:
		param_new = 11 + s->crystal;
		break;
	case 9600:
		param_new = 10 + s->crystal;
		break;
	case 19200:
		param_new = 9 + s->crystal;
		break;
	case 38400:
		param_new = 8 + s->crystal;
		break;
	case 57600:
		param_new = 1 + s->crystal;
		break;
	case 115200:
		param_new = 0 + s->crystal;
		break;
	case 230400:
		if (s->crystal)
			param_new = 0;
		else
			baud = s->baud;
		break;
	default:
		baud = s->baud;
	}
	tty_encode_baud_rate(tty, baud, baud);
	s->baud = baud;
	param_mask |= MAX3100_BAUD;

	if ((cflag & CSIZE) == CS8) {
		param_new &= ~MAX3100_L;
		parity &= ~MAX3100_7BIT;
	} else {
		param_new |= MAX3100_L;
		parity |= MAX3100_7BIT;
		cflag = (cflag & ~CSIZE) | CS7;
	}
	param_mask |= MAX3100_L;

	if (cflag & CSTOPB)
		param_new |= MAX3100_ST;
	else
		param_new &= ~MAX3100_ST;
	param_mask |= MAX3100_ST;

	if (cflag & PARENB) {
		param_new |= MAX3100_PE;
		parity |= MAX3100_PARITY_ON;
	} else {
		param_new &= ~MAX3100_PE;
		parity &= ~MAX3100_PARITY_ON;
	}
	param_mask |= MAX3100_PE;

	if (cflag & PARODD)
		parity |= MAX3100_PARITY_ODD;
	else
		parity &= ~MAX3100_PARITY_ODD;

	/* mask termios capabilities we don't support */
	cflag &= ~CMSPAR;
	termios->c_cflag = cflag;

	s->port.ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		s->port.ignore_status_mask |=
			MAX3100_STATUS_PE | MAX3100_STATUS_FE |
			MAX3100_STATUS_OE;

	/* we are sending char from a workqueue so enable */
	s->port.info->port.tty->low_latency = 1;

	if (s->poll_time > 0)
		del_timer_sync(&s->timer);

	uart_update_timeout(port, termios->c_cflag, baud);

	spin_lock(&s->conf_lock);
	s->conf = (s->conf & ~param_mask) | (param_new & param_mask);
	s->conf_commit = 1;
	s->parity = parity;
	spin_unlock(&s->conf_lock);
	max3100_dowork(s);

	if (UART_ENABLE_MS(&s->port, termios->c_cflag))
		max3100_enable_ms(&s->port);
}

static void max3100_shutdown(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	if (s->suspending)
		return;

	s->force_end_work = 1;

	if (s->poll_time > 0)
		del_timer_sync(&s->timer);

	if (s->workqueue) {
		flush_workqueue(s->workqueue);
		destroy_workqueue(s->workqueue);
		s->workqueue = NULL;
	}
	if (s->irq)
		free_irq(s->irq, s);

	/* set shutdown mode to save power */
	if (s->max3100_hw_suspend)
		s->max3100_hw_suspend(1);
	else  {
		u16 tx, rx;

		tx = MAX3100_WC | MAX3100_SHDN;
		max3100_sr(s, tx, &rx);
	}
}

static int max3100_startup(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);
	char b[12];

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	s->conf = MAX3100_RM;
	s->baud = s->crystal ? 230400 : 115200;
	s->rx_enabled = 1;

	if (s->suspending)
		return 0;

	s->force_end_work = 0;
	s->parity = 0;
	s->rts = 0;

	sprintf(b, "max3100-%d", s->minor);
	s->workqueue = create_freezeable_workqueue(b);
	if (!s->workqueue) {
		dev_warn(&s->spi->dev, "cannot create workqueue\n");
		return -EBUSY;
	}
	INIT_WORK(&s->work, max3100_work);

	if (request_irq(s->irq, max3100_irq,
			IRQF_TRIGGER_FALLING, "max3100", s) < 0) {
		dev_warn(&s->spi->dev, "cannot allocate irq %d\n", s->irq);
		s->irq = 0;
		destroy_workqueue(s->workqueue);
		s->workqueue = NULL;
		return -EBUSY;
	}

	if (s->loopback) {
		u16 tx, rx;
		tx = 0x4001;
		max3100_sr(s, tx, &rx);
	}

	if (s->max3100_hw_suspend)
		s->max3100_hw_suspend(0);
	s->conf_commit = 1;
	max3100_dowork(s);
	/* wait for clock to settle */
	msleep(50);

	max3100_enable_ms(&s->port);

	return 0;
}

static const char *max3100_type(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	return s->port.type == PORT_MAX3100 ? "MAX3100" : NULL;
}

static void max3100_release_port(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);
}

static void max3100_config_port(struct uart_port *port, int flags)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	if (flags & UART_CONFIG_TYPE)
		s->port.type = PORT_MAX3100;
}

static int max3100_verify_port(struct uart_port *port,
			       struct serial_struct *ser)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);
	int ret = -EINVAL;

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	if (ser->type == PORT_UNKNOWN || ser->type == PORT_MAX3100)
		ret = 0;
	return ret;
}

static void max3100_stop_tx(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);
}

static int max3100_request_port(struct uart_port *port)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);
	return 0;
}

static void max3100_break_ctl(struct uart_port *port, int break_state)
{
	struct max3100_port *s = container_of(port,
					      struct max3100_port,
					      port);

	dev_dbg(&s->spi->dev, "%s\n", __func__);
}

static struct uart_ops max3100_ops = {
	.tx_empty	= max3100_tx_empty,
	.set_mctrl	= max3100_set_mctrl,
	.get_mctrl	= max3100_get_mctrl,
	.stop_tx        = max3100_stop_tx,
	.start_tx	= max3100_start_tx,
	.stop_rx	= max3100_stop_rx,
	.enable_ms      = max3100_enable_ms,
	.break_ctl      = max3100_break_ctl,
	.startup	= max3100_startup,
	.shutdown	= max3100_shutdown,
	.set_termios	= max3100_set_termios,
	.type		= max3100_type,
	.release_port   = max3100_release_port,
	.request_port   = max3100_request_port,
	.config_port	= max3100_config_port,
	.verify_port	= max3100_verify_port,
};
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE
static void max3100_console_work(struct work_struct *w)
{
	struct max3100_port *s = container_of(w, struct max3100_port,
					      console_work);
	unsigned long start;
	u16 tx, rx;

	while (s->console_head != s->console_tail &&
	       (s->console_flags & MAX3100_SUSPENDING) == 0) {
		start = jiffies;
		do {
			tx = MAX3100_RC;
			max3100_sr(s, tx, &rx);
		} while ((rx & MAX3100_T) == 0 &&
			 !time_after(jiffies, start + s->console_tout));
		tx = s->console_buf[s->console_tail];
		max3100_calc_parity(s, &tx);
#if 1
              tx |= MAX3100_WD | MAX3100_RTS;
		max3100_sr(s, tx, &rx);
#else
              tx |= MAX3100_WD;
		max3100_sr(s, tx, &rx);
#endif
		s->console_tail = (s->console_tail + 1) % CONSOLE_BUF_SIZE;
	}
}
static void max3100_console_putchar(struct uart_port *port, int ch)
{
//	struct max3100_port *s = to_max3100_port(port);
  	struct max3100_port *s = container_of(port, struct max3100_port, port);
	int next = (s->console_head + 1) % CONSOLE_BUF_SIZE;

	if (next != s->console_tail) {
		s->console_buf[next] = ch;
		s->console_head = next;
	}
}
static void max3100_console_write(struct console *co,
				  const char *str, unsigned int count)
{
	struct max3100_port *s = max3100s[co->index];;

	uart_console_write(&s->port, str, count, max3100_console_putchar);
	schedule_work(&s->console_work);
}
static int __init hwmax3100_console_setup(struct console *co, char *options)
{
	/*
	 * Check whether an invalid uart number has been specified, and
	 * if so, set it to port 0
	 */
	struct max3100_port *s;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';
	int ret;
	u16 tx, rx;

	if (co->index == -1 || co->index >= MAX_MAX3100)
		co->index = 0;
	s = max3100s[co->index];
	if (!s)
		return -ENOENT;

	s->console_buf = kmalloc(CONSOLE_BUF_SIZE, GFP_KERNEL);
	if (!s->console_buf)
		return -ENOMEM;
	INIT_WORK(&s->console_work, max3100_console_work);
	s->console_flags |= MAX3100_IS_CONSOLE;

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	ret = uart_set_options(&s->port, co, baud, parity, bits, flow);

	tx = 0;
	switch (baud) {
	case 300:
		tx = 15;
		break;
	case 600:
		tx = 14 + s->crystal;
		break;
	case 1200:
		tx = 13 + s->crystal;
		break;
	case 2400:
		tx = 12 + s->crystal;
		break;
	case 4800:
		tx = 11 + s->crystal;
		break;
	case 9600:
		tx = 10 + s->crystal;
		break;
	case 19200:
		tx = 9 + s->crystal;
		break;
	case 38400:
		tx = 8 + s->crystal;
		break;
	case 57600:
		tx = 1 + s->crystal;
		break;
	case 115200:
		tx = 0 + s->crystal;
		break;
	case 230400:
		tx = 0;
		break;
	}
	if (bits == 7) {
		tx |= MAX3100_L;
		s->parity |= MAX3100_7BIT;
	}
	if (parity == 'o' || parity == 'e') {
		tx |= MAX3100_PE;
		s->parity |= MAX3100_PARITY_ON;
	}
	if (parity == 'o')
		s->parity |= MAX3100_PARITY_ODD;
	s->console_tout = 1 + (20 * HZ) / baud; /* jiffies to send 20 bits */

	tx |= MAX3100_WC;
	max3100_sr(s, tx, &rx);

	/* wait for oscilator to stabilize. Data sheet says 25ms,
	 * apply "multiply by two" engineer's rule */
	msleep(50);
	return ret;
}

static struct uart_driver max3100_uart_driver;

static struct console hwmax3100_console = {
	.name		= "ttyMAX",
	.write		= max3100_console_write,
	.device		= uart_console_device,
	.setup		= hwmax3100_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &max3100_uart_driver,
};
static int max3100_console_registered;
#endif
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
static struct uart_driver max3100_uart_driver = {
	.owner          = THIS_MODULE,
	.driver_name    = "ttyMAX",
	.dev_name       = "ttyMAX",
	.major          = MAX3100_MAJOR,
	.minor          = MAX3100_MINOR,
	.nr             = MAX_MAX3100,
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE	
	.cons           = &hwmax3100_console,
#endif	
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
};
static int uart_driver_registered;

static int __devinit max3100_probe(struct spi_device *spi)
{
	int i, retval;
	struct plat_max3100 *pdata;
	u16 tx, rx;

	mutex_lock(&max3100s_lock);

	if (!uart_driver_registered) {
		uart_driver_registered = 1;
		retval = uart_register_driver(&max3100_uart_driver);
		if (retval) {
			printk(KERN_ERR "Couldn't register max3100 uart driver\n");
			mutex_unlock(&max3100s_lock);
			return retval;
		}
	}

	for (i = 0; i < MAX_MAX3100; i++)
		if (!max3100s[i])
			break;
	if (i == MAX_MAX3100) {
		dev_warn(&spi->dev, "too many MAX3100 chips\n");
		mutex_unlock(&max3100s_lock);
		return -ENOMEM;
	}

	max3100s[i] = kzalloc(sizeof(struct max3100_port), GFP_KERNEL);
	if (!max3100s[i]) {
		dev_warn(&spi->dev,
			 "kmalloc for max3100 structure %d failed!\n", i);
		mutex_unlock(&max3100s_lock);
		return -ENOMEM;
	}
	max3100s[i]->spi = spi;
	max3100s[i]->irq = spi->irq;
	spin_lock_init(&max3100s[i]->conf_lock);
	dev_set_drvdata(&spi->dev, max3100s[i]);
	pdata = spi->dev.platform_data;
	max3100s[i]->crystal = pdata->crystal;
	max3100s[i]->loopback = pdata->loopback;
	max3100s[i]->poll_time = pdata->poll_time * HZ / 1000;
	if (pdata->poll_time > 0 && max3100s[i]->poll_time == 0)
		max3100s[i]->poll_time = 1;
	max3100s[i]->max3100_hw_suspend = pdata->max3100_hw_suspend;
	max3100s[i]->minor = i;
	init_timer(&max3100s[i]->timer);
	max3100s[i]->timer.function = max3100_timeout;
	max3100s[i]->timer.data = (unsigned long) max3100s[i];

	dev_dbg(&spi->dev, "%s: adding port %d\n", __func__, i);
	max3100s[i]->port.irq = max3100s[i]->irq;
	max3100s[i]->port.uartclk = max3100s[i]->crystal ? 3686400 : 1843200;
	max3100s[i]->port.fifosize = 16;
	max3100s[i]->port.ops = &max3100_ops;
	max3100s[i]->port.flags = UPF_SKIP_TEST | UPF_BOOT_AUTOCONF;
	max3100s[i]->port.line = i;
	max3100s[i]->port.type = PORT_MAX3100;
	max3100s[i]->port.dev = &spi->dev;
	retval = uart_add_one_port(&max3100_uart_driver, &max3100s[i]->port);
	if (retval < 0)
		dev_warn(&spi->dev,
			 "uart_add_one_port failed for line %d with error %d\n",
			 i, retval);
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifndef CONFIG_SPI_MAX3100_CONSOLE
	/* set shutdown mode to save power. Will be woken-up on open */
	if (max3100s[i]->max3100_hw_suspend)
		max3100s[i]->max3100_hw_suspend(1);
	else {
		tx = MAX3100_WC | MAX3100_SHDN;
		max3100_sr(max3100s[i], tx, &rx);
	}
#endif
#ifdef CONFIG_SPI_MAX3100_CONSOLE
	if(!max3100_console_registered)
	{
		register_console(&hwmax3100_console);
		max3100_console_registered = 1;
	}   
#endif
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
	mutex_unlock(&max3100s_lock);
	return 0;
}

static int __devexit max3100_remove(struct spi_device *spi)
{
	struct max3100_port *s = dev_get_drvdata(&spi->dev);
	int i;

	mutex_lock(&max3100s_lock);
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE
	if (max3100_console_registered) {
		unregister_console(&hwmax3100_console);
		max3100_console_registered = 0;
	}
#endif
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
	/* find out the index for the chip we are removing */
	for (i = 0; i < MAX_MAX3100; i++)
		if (max3100s[i] == s)
			break;

	dev_dbg(&spi->dev, "%s: removing port %d\n", __func__, i);
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE
	if (s->console_buf) {
		flush_work(&s->console_work);
		kfree(s->console_buf);
	}
#endif    
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
	uart_remove_one_port(&max3100_uart_driver, &max3100s[i]->port);
	kfree(max3100s[i]);
	max3100s[i] = NULL;

	/* check if this is the last chip we have */
	for (i = 0; i < MAX_MAX3100; i++)
		if (max3100s[i]) {
			mutex_unlock(&max3100s_lock);
			return 0;
		}
	pr_debug("removing max3100 driver\n");
	uart_unregister_driver(&max3100_uart_driver);

	mutex_unlock(&max3100s_lock);
	return 0;
}

#ifdef CONFIG_PM

static int max3100_suspend(struct spi_device *spi, pm_message_t state)
{
	struct max3100_port *s = dev_get_drvdata(&spi->dev);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	disable_irq(s->irq);

	s->suspending = 1;
	uart_suspend_port(&max3100_uart_driver, &s->port);

	if (s->max3100_hw_suspend)
		s->max3100_hw_suspend(1);
	else {
		/* no HW suspend, so do SW one */
		u16 tx, rx;

		tx = MAX3100_WC | MAX3100_SHDN;
		max3100_sr(s, tx, &rx);
	}
	return 0;
}

static int max3100_resume(struct spi_device *spi)
{
	struct max3100_port *s = dev_get_drvdata(&spi->dev);

	dev_dbg(&s->spi->dev, "%s\n", __func__);

	if (s->max3100_hw_suspend)
		s->max3100_hw_suspend(0);
	uart_resume_port(&max3100_uart_driver, &s->port);
	s->suspending = 0;

	enable_irq(s->irq);

	s->conf_commit = 1;
	if (s->workqueue)
		max3100_dowork(s);

	return 0;
}

#else
#define max3100_suspend NULL
#define max3100_resume  NULL
#endif

static struct spi_driver max3100_driver = {
	.driver = {
		.name		= "max3100",
		.bus		= &spi_bus_type,
		.owner		= THIS_MODULE,
	},

	.probe		= max3100_probe,
	.remove		= __devexit_p(max3100_remove),
	.suspend	= max3100_suspend,
	.resume		= max3100_resume,
};

/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE
static struct spi_device * spiDeviceMax3100;

static struct plat_max3100 max3100_plat_data = {
	.loopback = 0,
	.crystal = 1,
	.poll_time = 100,
};

static struct spi_board_info spiBoardInfoMax3100 =
{
	.modalias        = "max3100",
	.platform_data   = &max3100_plat_data,
	.irq             = 10, //IRQ_EINT12,
	.controller_data = (void *)(1<<29), /* BCM - GATE Clock when SS off */
	.chip_select     = 1,               /* slave select 1 */
	.max_speed_hz    = 5*1000*1000,
//	.max_speed_hz    = 781000,
	.bus_num         = 1,               /* BCM HS SPI Controller */
	.mode            = SPI_MODE_0,
};

static int bcmHspi_init(void)
{
    struct spi_master * pSpiMaster;

    pSpiMaster       = spi_busnum_to_master( 1 ); /* BCM HS SPI controller */
    spiDeviceMax3100 = spi_new_device(pSpiMaster, &spiBoardInfoMax3100);
    return 0;
}
#endif
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
static int __init max3100_init(void)
{
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE
    int iRet;
    bcmHspi_init();
    iRet = spi_register_driver(&max3100_driver);
    //hwmax3100_console_init();
    return iRet;
#else	
	return spi_register_driver(&max3100_driver);
#endif	
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
}
module_init(max3100_init);

static void __exit max3100_exit(void)
{
	spi_unregister_driver(&max3100_driver);
/*Start Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
#ifdef CONFIG_SPI_MAX3100_CONSOLE	
	spi_unregister_device(spiDeviceMax3100);
#endif	
/*End Add For SUPPORT MAX3100 SPI TO UART By lKF33113 2011-12-06*/
}
module_exit(max3100_exit);

MODULE_DESCRIPTION("MAX3100 driver");
MODULE_AUTHOR("Christian Pellegrin <chripell@evolware.org>");
MODULE_LICENSE("GPL");
