/*
 *  dw_serial.c
 *
 *  hisilicon
 *
 *  This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * This is a generic driver for ARM DW-type serial ports.  They
 * have a lot of 16550-like features, but are not register compatible.
 * Note that although they do have CTS, DCD and DSR inputs, they do
 * not have an RI input, nor do they have DTR or RTS outputs.  If
 * required, these have to be supplied via some other means (eg, GPIO)
 * 
 *
 */
#include <linux/autoconf.h>

#if defined(CONFIG_SERIAL_HISI_SD5115_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)         
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/amba/bus.h>
#include <linux/amba/dw_serial.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/slab.h>

#include <asm/io.h>
#include <asm/irq.h>

#define ESS_KERNEL_DEBUG
#ifdef  ESS_KERNEL_DEBUG
#include <mach/early-debug.h>
#endif

#define DW_UART_SUPPORT_MODEM 0

#define UART_NR            14

#define SERIAL_DW_MAJOR    204
#define SERIAL_DW_MINOR    64
#define SERIAL_DW_NR       UART_NR

#define DW_ISR_PASS_LIMIT    256

#define UART_DR_ERROR		(UARTdw_DR_OE|UARTdw_DR_BE|UARTdw_DR_PE|UARTdw_DR_FE)
#define UART_DUMMY_DR_RX	(1 << 16)

/*
 * We wrap our port structure around the generic uart_port.
 */
struct uart_dw_port {
    struct uart_port    port;
    struct clk        *clk;
    unsigned int        im;    /* interrupt mask */
    unsigned int        old_status;
};

static void dw_stop_tx(struct uart_port *port)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;

    uap->im &= ~(PTIME|ETBEI);
    writel(uap->im, uap->port.membase + DW_UART_IER);

}

static void dw_start_tx(struct uart_port *port)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;

    uap->im |= ETBEI;
    writel(uap->im, uap->port.membase + DW_UART_IER);
}

static void dw_stop_rx(struct uart_port *port)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;

    uap->im &= ~(ELSI|ERBFI);         
    writel(uap->im, uap->port.membase + DW_UART_IER);
}


static void dw_enable_ms(struct uart_port *port)
{
    //TODOedb_trace(1);
}

static void dw_rx_chars(struct uart_dw_port *uap)
{
	struct tty_struct *tty = uap->port.info->port.tty;
    unsigned int status, ch, flag, rsr, max_count = 256;

    status = readl(uap->port.membase + DW_UART_LSR);
    
    /* BEGIN: Added by j00144355, 2008/12/29   PN:AI8D01656*/
    if(!(status & DW_DR))
    {
        readl(uap->port.membase + DW_UART_RBR);
    }
    /* END:   Added by j00144355, 2008/12/29 */

    while ((status & DW_DR) && max_count--) {

        ch = readl(uap->port.membase + DW_UART_RBR)|UART_DUMMY_DR_RX;
        flag = TTY_NORMAL;
        uap->port.icount.rx++;

        /*
         * Note that the error handling code is
         * out of the main execution path
         */


		if (unlikely(ch & UART_DR_ERROR)) {
			if (ch & UARTdw_DR_BE) {
				ch &= ~(UARTdw_DR_FE | UARTdw_DR_PE);
				uap->port.icount.brk++;
				if (uart_handle_break(&uap->port))
					goto ignore_char;
			} else if (ch & UARTdw_DR_PE)
				uap->port.icount.parity++;
			else if (ch & UARTdw_DR_FE)
				uap->port.icount.frame++;
			if (ch & UARTdw_DR_OE)
				uap->port.icount.overrun++;

			ch &= uap->port.read_status_mask;

			if (ch & UARTdw_DR_BE)
				flag = TTY_BREAK;
			else if (ch & UARTdw_DR_PE)
				flag = TTY_PARITY;
			else if (ch & UARTdw_DR_FE)
				flag = TTY_FRAME;
        }

		if (uart_handle_sysrq_char(&uap->port, ch & 255))
            goto ignore_char;

		uart_insert_char(&uap->port, ch, UARTdw_DR_OE, ch, flag);

ignore_char:
        status = readl(uap->port.membase + DW_UART_LSR);
    }
    tty_flip_buffer_push(tty);
    return;
}

static void dw_tx_chars(struct uart_dw_port *uap)
{
	struct circ_buf *xmit = &uap->port.info->xmit;
    int count;
    
    if (uap->port.x_char) {
        while ((readl(uap->port.membase + DW_UART_LSR) & 0x20)!=0x20);
        writel(uap->port.x_char, uap->port.membase + DW_UART_THR);
        uap->port.icount.tx++;
        uap->port.x_char = 0;
        return;
    }

    if (uart_circ_empty(xmit) || uart_tx_stopped(&uap->port)) {
        dw_stop_tx(&uap->port);
        return;
    }

    count = uap->port.fifosize >> 1;

    do {
        while ((readl(uap->port.membase + DW_UART_LSR) & 0x20)!=0x20);
        writel(xmit->buf[xmit->tail], uap->port.membase + DW_UART_THR);
        xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
        uap->port.icount.tx++;
        if (uart_circ_empty(xmit))
            break;
    } while (--count > 0);

    if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
        uart_write_wakeup(&uap->port);

    if (uart_circ_empty(xmit))
        dw_stop_tx(&uap->port);
}


static void dw_modem_status(struct uart_dw_port *uap)
{

    return ;
#if DW_UART_SUPPORT_MODEM
    unsigned int status, delta;

    status = readl(uap->port.membase + DW_UART_MSR) & DW_MSR_ANY;

    delta = status ^ uap->old_status;
    uap->old_status = status;

    if (!delta)
        return;

    if (delta & DW_DCD)
        uart_handle_dcd_change(&uap->port, status & DW_DCD);

    if (delta & DW_DSR)
        uap->port.icount.dsr++;

    if (delta & DW_CTS)
        uart_handle_cts_change(&uap->port, status & DW_CTS);

    wake_up_interruptible(&uap->port.info->delta_msr_wait);
#endif
}

static irqreturn_t dw_uart_intr(int irq, void *dev_id)
{
    struct uart_dw_port *uap = dev_id;
    unsigned int status, pass_counter = DW_ISR_PASS_LIMIT;
    unsigned int interruptID;
    int handled = 0;

    /* BEGIN: Added by j00144355, 2008/12/29   PN:AI8D01656*/
    unsigned int status_usr = 0;
    /* END:   Added by j00144355, 2008/12/29 */

    spin_lock(&uap->port.lock);

    status = readl(uap->port.membase + DW_UART_IIR);
    interruptID = 0x0F&status;                   

    while (interruptID != DW_NOINTERRUPT) {
        if ((DW_RECEIVEAVA == interruptID) || (DW_RECTIMEOUT == interruptID) || (DW_RECEIVERR == interruptID)){
            dw_rx_chars(uap);
        }else if (DW_MODEMSTA == interruptID){
            dw_modem_status(uap);
        }else if (DW_TRANSEMP == interruptID){
            dw_tx_chars(uap);
        }
        /* BEGIN: Added by j00144355, 2008/12/29   PN:AI8D01656*/
        else
        {
            //printk("\n Err interruptID = 0x%x\n",interruptID);
            status_usr = readl(uap->port.membase + DW_UART_USR);            
            //printk("\n status_usr = 0x%x\n",status_usr);
        }
        /* END:   Added by j00144355, 2008/12/29 */
        
        if (pass_counter-- == 0)
            break;

        status = readl(uap->port.membase + DW_UART_IIR);
        interruptID = 0x0F&status;                   
        handled = 1;
    } 

    spin_unlock(&uap->port.lock);

    return IRQ_RETVAL(handled);
}

static unsigned int dw_tx_empty(struct uart_port *port)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;
    unsigned int status = readl(uap->port.membase + DW_UART_LSR);

    if(status & DW_TEMT)
    {
        return TIOCSER_TEMT;
    }

    return 0;            
}

static unsigned int dw_get_mctrl(struct uart_port *port)
{

    unsigned int result = 0;
#if DW_UART_SUPPORT_MODEM
    struct uart_dw_port *uap = (struct uart_dw_port *)port;
    unsigned int result = 0;
    unsigned int status = readl(uap->port.membase + DW_UART_MSR);

#define BIT(uartbit, tiocmbit)        \
    if (status & uartbit)        \
        result |= tiocmbit

            BIT(DW_DCD, TIOCM_CAR);
    BIT(DW_DSR, TIOCM_DSR);
    BIT(DW_CTS, TIOCM_CTS);
    BIT(DW_RI, TIOCM_RNG);
#undef BIT

#endif

    return result;
}

static void dw_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
    return ;
#if DW_UART_SUPPORT_MODEM
    struct uart_dw_port *uap = (struct uart_dw_port *)port;
    unsigned int cr;

    cr = readl(uap->port.membase + DW_UART_MCR);

#define    BIT(tiocmbit, uartbit)        \
    if (mctrl & tiocmbit)        \
        cr |= uartbit;        \
    else                \
        cr &= ~uartbit


			BIT(TIOCM_RTS, DW_MC_RTS);
    BIT(TIOCM_DTR, DW_MC_DTR);
    BIT(TIOCM_OUT1, DW_MC_OUT1);
    BIT(TIOCM_OUT2, DW_MC_OUT2);
    BIT(TIOCM_LOOP, DW_MC_LOOP);
#undef BIT

    writel(cr, uap->port.membase + DW_UART_MCR);
#endif
}

static void dw_break_ctl(struct uart_port *port, int break_state)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;
    unsigned long flags;
    unsigned int lcr_h;

    spin_lock_irqsave(&uap->port.lock, flags);

    while(readl(uap->port.membase + DW_UART_USR) & DW_UART_BUSY);
    
    lcr_h = readl(uap->port.membase + DW_UART_LCR);
    if (break_state)
        lcr_h |= DW_UART_BREAK;
    else
        lcr_h &= ~DW_UART_BREAK;
    writel(lcr_h, uap->port.membase + DW_UART_LCR);

    spin_unlock_irqrestore(&uap->port.lock, flags);
}

static int dw_startup(struct uart_port *port)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;
    int retval;
	unsigned int tmp;

    /*
     * Try to enable the clock producer.
     */
    retval = clk_enable(uap->clk);
    if (retval)
        goto out;
    uap->port.uartclk = clk_get_rate(uap->clk);

	/* disable FIFO and interrupt */
    writel(0, uap->port.membase + DW_UART_IER);
	writel(0, uap->port.membase + DW_UART_FCR);

    /* BEGIN: Modified by j00144355, 2008/12/29   PN:AI8D01656*/
    #if 0
	do{
		tmp = readl(uap->port.membase + DW_UART_USR);
	}while(tmp & 0x01);
    #else
    do{
        tmp = readl(uap->port.membase + DW_UART_USR);
    }while(0);
    #endif
    /* END:   Modified by j00144355, 2008/12/29 */

#if DW_UART_SUPPORT_MODEM
    /*
     * initialise the old status of the modem signals
     */
    uap->old_status = readl(uap->port.membase + DW_UART_MSR) & DW_MSR_ANY;

#endif    

    /*
     * Allocate the IRQ
     */
    retval = request_irq(uap->port.irq, dw_uart_intr, 0, "uart-dw", uap);
    if (retval)
        goto clk_dis;

	/* enable FIFO */
    writel(0xb1,uap->port.membase + DW_UART_FCR);

	/* clear interrupt status */
    readl(uap->port.membase + DW_UART_LSR);
    
	/*
     * Finally, enable interrupts
     */
    spin_lock_irq(&uap->port.lock);

    uap->im = (ERBFI|ELSI);
    writel(uap->im, uap->port.membase + DW_UART_IER);

    spin_unlock_irq(&uap->port.lock);

	return 0;

clk_dis:
    clk_disable(uap->clk);
out:
    return retval;
}

static void dw_shutdown(struct uart_port *port)
{
    struct uart_dw_port *uap = (struct uart_dw_port *)port;
    unsigned long val;

    spin_lock_irq(&uap->port.lock);

    /*
     * disable/clear all interrupts
     */
    uap->im = 0;
    writel(uap->im, uap->port.membase + DW_UART_IER);
    readl( uap->port.membase + DW_UART_LSR);

    spin_unlock_irq(&uap->port.lock);

    /*
     * Free the interrupt
     */
    free_irq(uap->port.irq, uap);

    /*
     * disable break condition and fifos
     */
    while(readl(uap->port.membase + DW_UART_USR) & DW_UART_BUSY);
    val = readl(uap->port.membase + DW_UART_LCR);
    val &= ~(DW_UART_BREAK);
    writel(val, uap->port.membase + DW_UART_LCR);

    /* disable fifo*/
    writel(0,uap->port.membase + DW_UART_FCR);

    /*
     * Shut down the clock producer
     */
    clk_disable(uap->clk);
}

static void dw_set_termios(struct uart_port *port, struct ktermios *termios, struct ktermios *old)
{
    unsigned int lcr_h, old_cr;
    unsigned long flags;
    unsigned int baud, quot;

    /*
     * Ask the core to calculate the divisor for us.
     */
    baud = uart_get_baud_rate(port, termios, old, 0, port->uartclk/16);
    quot = port->uartclk / (16 * baud);

    switch (termios->c_cflag & CSIZE) {
        case CS5:
            lcr_h = DW_UART_5bit;
            break;
        case CS6:
            lcr_h = DW_UART_6bit;
            break;
        case CS7:
            lcr_h = DW_UART_7bit;
            break;
        case CS8:
            lcr_h = DW_UART_8bit;
            break;
        default: // CS8
            lcr_h = DW_UART_8bit;
            break;
    }

    if (termios->c_cflag & CSTOPB)
        lcr_h |= DW_UART_STOP;

    if (termios->c_cflag & PARENB) {
        lcr_h |= DW_UART_PEN;

        if (!(termios->c_cflag & PARODD))
            lcr_h |= DW_UART_EPS;
    }

    spin_lock_irqsave(&port->lock, flags);

    /*
     * Update the per-port timeout.
     */
    uart_update_timeout(port, termios->c_cflag, baud);

	port->read_status_mask = UARTdw_DR_OE | 255;

    if (termios->c_iflag & INPCK)
		port->read_status_mask |= UARTdw_DR_FE | UARTdw_DR_PE;
	if (termios->c_iflag & (BRKINT | PARMRK))
		port->read_status_mask |= UARTdw_DR_BE;

	/*
	 * Characters to ignore
	 */
	port->ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= UARTdw_DR_FE | UARTdw_DR_PE;
	if (termios->c_iflag & IGNBRK) {
		port->ignore_status_mask |= UARTdw_DR_BE;
        /*
         * If we're ignoring parity and break indicators,
         * ignore overruns too (for real raw support).
         */
        if (termios->c_iflag & IGNPAR)
			port->ignore_status_mask |= UARTdw_DR_OE;
    }

    /*
     * Ignore all characters if CREAD is not set.
     */
    if ((termios->c_cflag & CREAD) == 0)
		port->ignore_status_mask |= UART_DUMMY_DR_RX;
    //port->ignore_status_mask |= UART_DUMMY_RSR_RX; !!todo


    if (UART_ENABLE_MS(port, termios->c_cflag))
        dw_enable_ms(port);

    /* AI8D01366, dubo, 20080711, 串口输入导致串口输出错误，系统挂死 */
    //do{
        //old_cr = readl(port->membase + DW_UART_USR);
    //} while(old_cr & DW_UART_BUSY);    /*wait for idle*/

	/* Enable DLL and DLH */
    old_cr = readl(port->membase + DW_UART_LCR);    
    old_cr |= DW_UART_DLAB;     
    writel(old_cr,port->membase + DW_UART_LCR);

    /* Set baud rate */
    writel(((quot&0xFF00)>>8), port->membase + DW_UART_DLH);
    writel((quot & 0xFF), port->membase + DW_UART_DLL);

    old_cr &= ~DW_UART_DLAB;
    writel(old_cr,port->membase + DW_UART_LCR);

    writel(lcr_h, port->membase + DW_UART_LCR);

    spin_unlock_irqrestore(&port->lock, flags);
}

static const char *dw_type(struct uart_port *port)
{
    return port->type == PORT_AMBA ? "AMBA/DW" : NULL;
}

/*
 * Release the memory region(s) being used by 'port'
 */
static void dw_release_port(struct uart_port *port)
{

    release_mem_region(port->mapbase, SZ_4K);
}

/*
 * Request the memory region(s) being used by 'port'
 */
static int dw_request_port(struct uart_port *port)
{
    return request_mem_region(port->mapbase, SZ_4K, "uart-dw")
        != NULL ? 0 : -EBUSY;
}

/*
 * Configure/autoconfigure the port.
 */
static void dw_config_port(struct uart_port *port, int flags)
{
    if (flags & UART_CONFIG_TYPE) {
        port->type = PORT_AMBA;
        dw_request_port(port);
    }
}

/*
 * verify the new serial_struct (for TIOCSSERIAL).
 */
static int dw_verify_port(struct uart_port *port, struct serial_struct *ser)
{
    int ret = 0;
    if (ser->type != PORT_UNKNOWN && ser->type != PORT_AMBA)
        ret = -EINVAL;
    if (ser->irq < 0 || ser->irq >= NR_IRQS)
        ret = -EINVAL;
    if (ser->baud_base < 9600)
        ret = -EINVAL;
    return ret;
}

static struct uart_ops dw_serail_pops = {
    .tx_empty    = dw_tx_empty,
    .set_mctrl    = dw_set_mctrl,
    .get_mctrl    = dw_get_mctrl,
    .stop_tx    = dw_stop_tx,
    .start_tx    = dw_start_tx,
    .stop_rx    = dw_stop_rx,
    .enable_ms    = dw_enable_ms,
    .break_ctl    = dw_break_ctl,
    .startup    = dw_startup,
    .shutdown    = dw_shutdown,
    .set_termios    = dw_set_termios,
    .type        = dw_type,
    .release_port    = dw_release_port,
    .request_port    = dw_request_port,
    .config_port    = dw_config_port,
    .verify_port    = dw_verify_port,
};

static struct uart_dw_port *dw_ports[UART_NR];



#ifdef CONFIG_SERIAL_HISI_SD5115_CONSOLE
static void dw_console_putchar(struct uart_port *port, int ch)
{
	struct uart_dw_port *uap = (struct uart_dw_port *)port;
	//while ((readw(uap->port.membase + DW_UART_LSR) & 0X20) != 0X20);
	while((*(unsigned long *)IO_ADDRESS(BOOTUP_UART_BASE + 0x14)& 0X20) != 0X20)
		barrier();
    //writel(ch, uap->port.membase + DW_UART_THR);
    *(unsigned long *)IO_ADDRESS(BOOTUP_UART_BASE) = ch;
}

static void dw_console_write(struct console *co, const char *s, unsigned int count)
{
    struct uart_dw_port *uap = dw_ports[co->index];
    unsigned int status;

    clk_enable(uap->clk);

    /*
     *    Now, do each character
     */
	uart_console_write(&uap->port, s, count, dw_console_putchar);

    /*
     *    Finally, wait for transmitter to become empty
     *    and restore the TCR
     */
    /* AI8D01366, dubo, 20080711, 串口输入导致串口输出错误，系统挂死 */
    //do {
        //status = readl(uap->port.membase + DW_UART_USR);
    //} while (status & DW_UART_BUSY);

    //    writel(old_cr, uap->port.membase + DW_UART_MCR);

    clk_disable(uap->clk);
}

static void __init 
dw_console_get_options(struct uart_dw_port *uap, int *baud, int *parity, int *bits)
{

    unsigned int lcr_h, brd,status;
    unsigned int flags;
    unsigned char lbrd,hbrd;

    lcr_h = readl(uap->port.membase + DW_UART_LCR);

    *parity = 'n';
    if (lcr_h & DW_UART_PEN) {
        if (lcr_h & DW_UART_EPS)
            *parity = 'e';
        else
            *parity = 'o';
    }

    if ((lcr_h & DW_UART_DATALEN_MASK) == DW_UART_5bit)
        *bits = 5;
    else if((lcr_h & DW_UART_DATALEN_MASK) == DW_UART_6bit)
        *bits = 6;
    else if((lcr_h & DW_UART_DATALEN_MASK) == DW_UART_7bit)        
        *bits = 7;
    else
        *bits = 8;

    do{
        status = readl(uap->port.membase + DW_UART_USR);
    }while(status & DW_UART_BUSY);//wait for idle

    spin_lock_irqsave(&uap->port.lock, flags);

    lbrd = readl(uap->port.membase + DW_UART_DLL);
    hbrd = readl(uap->port.membase + DW_UART_DLH);

    spin_unlock_irqrestore(&uap->port.lock, flags);

    brd = (hbrd<<8) + lbrd;

    *baud = uap->port.uartclk / (16*brd);
}

static int __init dw_console_setup(struct console *co, char *options)
{
    struct uart_dw_port *uap;
    int baud = 115200;
    int bits = 8;
    int parity = 'n';
    int flow = 'n';

    if (co->index >= UART_NR)
        co->index = 0;
    
    uap = dw_ports[co->index];
	if (!uap)
		return -ENODEV;

#ifdef CONFIG_HISI_SD5115
//    uap->port.uartclk = HISILICON_AHBCLK_DEFAULT;
    uap->port.uartclk = CONFIG_DEFAULT_BUSCLK;  
#endif	
    uap = dw_ports[co->index];
/*
    if (options)
        uart_parse_options(options, &baud, &parity, &bits, &flow);
    else
        dw_console_get_options(uap, &baud, &parity, &bits);
*/

    return uart_set_options(&(uap->port), co, baud, parity, bits, flow);
}
static struct uart_driver dw_reg;
static struct console dw_console = {
    .name   = "ttyAMA",
    .write  = dw_console_write,
    .device = uart_console_device,
    .setup  = dw_console_setup,
    .flags  = CON_PRINTBUFFER,    //1
    .index  = -1,
    .data   = &dw_reg,
};

#define DW_CONSOLE    (&dw_console)
#else
#define DW_CONSOLE    NULL
#endif

static struct uart_driver dw_reg = {
    .owner            = THIS_MODULE,
    .driver_name    = "ttyAMA",
    .dev_name        = "ttyAMA",
    .major            = SERIAL_DW_MAJOR,  //204
    .minor            = SERIAL_DW_MINOR,  //64
    .nr              = UART_NR,              //2
    .cons            = DW_CONSOLE,
};

static int dw_probe(struct amba_device *dev, void *id)
{
    struct uart_dw_port *uap;
    void __iomem *base;
    int i, ret;

    for (i = 0; i < ARRAY_SIZE(dw_ports); i++)
        if (dw_ports[i] == NULL)
            break;

    if (i == ARRAY_SIZE(dw_ports)) {
        ret = -EBUSY;
        goto out;
    }

    uap = kmalloc(sizeof(struct uart_dw_port), GFP_KERNEL);
    if (uap == NULL) {
        ret = -ENOMEM;
        goto out;
    }

    /* I've mapped all io-space yet */
//    base = ioremap(dev->res.start, PAGE_SIZE); /* do map again  */
    base = (void*)IO_ADDRESS(dev->res.start);
    if (!base) {
        ret = -ENOMEM;
        goto free;
    }

    memset(uap, 0, sizeof(struct uart_dw_port));
    uap->clk = clk_get(&dev->dev, NULL);
    
    if (IS_ERR(uap->clk)) {
        ret = PTR_ERR(uap->clk);
        goto unmap;
    }


    uap->port.dev = &dev->dev;
    uap->port.mapbase = dev->res.start;
    uap->port.membase = base;
    uap->port.iotype = UPIO_MEM;
    uap->port.irq = dev->irq[0];
    uap->port.fifosize = 16;  //16*8bits
    uap->port.ops = &dw_serail_pops;
    uap->port.flags = UPF_BOOT_AUTOCONF;
    uap->port.line = i;

    dw_ports[i] = uap;

    amba_set_drvdata(dev, uap);
    ret = uart_add_one_port(&dw_reg, &uap->port);
    if (ret) {
        amba_set_drvdata(dev, NULL);
        dw_ports[i] = NULL;
        clk_put(uap->clk);
unmap:
        iounmap(base);
free:
        kfree(uap);
    }
out:
    return ret;
}

static int dw_remove(struct amba_device *dev)
{
    struct uart_dw_port *uap = amba_get_drvdata(dev);
    int i;

    amba_set_drvdata(dev, NULL);

    uart_remove_one_port(&dw_reg, &uap->port);

    for (i = 0; i < ARRAY_SIZE(dw_ports); i++)
        if (dw_ports[i] == uap)
            dw_ports[i] = NULL;

    iounmap(uap->port.membase);
    clk_put(uap->clk);
    kfree(uap);
    return 0;
}

static struct amba_id dw_ids[] __initdata = {
    {
        .id    = 0x00041011,
        .mask    = 0x000fffff,
    },
    { 0, 0 },
};

static struct amba_driver dw_driver = {
    .drv = {
        .name    = "uart-dw",
    },
    .id_table    = dw_ids,
    .probe        = dw_probe,
    .remove        = dw_remove,
};



static int __init dw_init(void)
{
    int ret;
    printk(KERN_INFO "Serial: dw  uart driver\n");

    ret = uart_register_driver(&dw_reg);
    if (ret == 0) {
        ret = amba_driver_register(&dw_driver);
        if (ret){
            uart_unregister_driver(&dw_reg);
        }
       
    }


    return ret;
}

static void __exit dw_exit(void)
{
    amba_driver_unregister(&dw_driver);
    uart_unregister_driver(&dw_reg);
}

//module_init(dw_init);
arch_initcall(dw_init);
module_exit(dw_exit);

MODULE_AUTHOR("Hisilicon Ltd/Deep Blue Solutions Ltd");
MODULE_DESCRIPTION("Hisilicon  DW serial port driver");
MODULE_LICENSE("GPL");
