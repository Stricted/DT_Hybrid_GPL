#if defined(CONFIG_BCM_KF_MIPS_BCM9685XX)
/*
 * lilac_spi.c - Lilac SPI controller driver.
 */
 
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/spi/spi.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/module.h>

#include "bl_lilac_spi.h"

struct lilac_spi
{
	struct workqueue_struct	*workqueue;
	struct work_struct work;
	spinlock_t lock;	/* protect 'queue' */
	struct list_head queue;
};

static int setup(struct spi_device *spi)
{
	BL_SPI_SLAVE_CFG *cfg;
	
	if(spi->bits_per_word && spi->bits_per_word != 8)
	{
		printk("Lilac SPI driver error: spi->bits_per_word != 8\n");
		return -EINVAL;
	}
	
	cfg = kmalloc(sizeof(BL_SPI_SLAVE_CFG), GFP_KERNEL);
	if (!cfg)
		return -ENOMEM;

	cfg->sleep_period = 10000;
	cfg->bus = spi->master->bus_num;
	cfg->cs = spi->chip_select;
	cfg->polarity = (spi->mode & SPI_CPOL) ? 1:0;
	cfg->lsb_msb = (spi->mode & SPI_LSB_FIRST) ? 1:0;
	cfg->edge = (spi->mode & SPI_CPHA) ? 1:0;
	cfg->clock = spi->max_speed_hz;
	cfg->handler = NULL;

	if(bl_spi_slave_init(cfg))
	{
		kfree(cfg);
		return -EINVAL;
	}
	
	spi_set_ctldata(spi, cfg);
	
	return 0;
}

static void cleanup(struct spi_device *spi)
{
	kfree(spi_get_ctldata(spi));
}

static void spi_work_one(struct lilac_spi *c, struct spi_message *m)
{
	struct spi_device *spi = m->spi;
	BL_SPI_SLAVE_CFG *cfg = (BL_SPI_SLAVE_CFG *)spi_get_ctldata(spi);
	struct spi_transfer *t;
	int status = 0;

	bl_claim_bus(cfg);
	bl_spi_cs_activate(cfg);
	
	list_for_each_entry(t, &m->transfers, transfer_list)
	{
		if(bl_spi_xfer(cfg, t->len, t->rx_buf, t->tx_buf, 0))
		{
			printk("Lilac SPI driver error: transfer timed out\n");
			status = -EIO;
			break;
		}
		m->actual_length += t->len;

		if (t->transfer_list.next == &m->transfers)
			break;
	}

	m->status = status;
	m->complete(m->context);

	bl_spi_cs_deactivate(cfg);
}

static void spi_work(struct work_struct *work)
{
	struct lilac_spi *c = container_of(work, struct lilac_spi, work);
	unsigned long flags;

	spin_lock_irqsave(&c->lock, flags);
	while(!list_empty(&c->queue))
	{
		struct spi_message *m;

		m = container_of(c->queue.next, struct spi_message, queue);
		list_del_init(&m->queue);
		spin_unlock_irqrestore(&c->lock, flags);

		spi_work_one(c, m);

		spin_lock_irqsave(&c->lock, flags);
	}
	spin_unlock_irqrestore(&c->lock, flags);
}

static int transfer(struct spi_device *spi, struct spi_message *m)
{
	struct spi_master *master = spi->master;
	struct lilac_spi *c = spi_master_get_devdata(master);
	struct spi_transfer *t;
	unsigned long flags;

	m->actual_length = 0;

	/* check each transfer's parameters */
	list_for_each_entry(t, &m->transfers, transfer_list)
	{
		if(t->speed_hz && t->speed_hz != spi->max_speed_hz)
		{
			printk("Lilac SPI driver error: t->speed_hz != spi->max_speed_hz\n");
			return -EINVAL;
		}
		if(t->bits_per_word && t->bits_per_word != 8)
		{
			printk("Lilac SPI driver error: t->bits_per_word != 8\n");
			return -EINVAL;
		}
		if(t->cs_change)
		{
			printk("Lilac SPI driver error: t->cs_change == 1\n");
			return -EINVAL;
		}

		if(!t->tx_buf && !t->rx_buf && t->len)
			return -EINVAL;
	}

	spin_lock_irqsave(&c->lock, flags);
	list_add_tail(&m->queue, &c->queue);
	queue_work(c->workqueue, &c->work);
	spin_unlock_irqrestore(&c->lock, flags);

	return 0;
}

static int probe(struct platform_device *dev)
{
	struct spi_master *master;
	struct lilac_spi *c;
	int ret = -ENODEV;

	master = spi_alloc_master(&dev->dev, sizeof(*c));
	if (!master)
		return ret;
		
	c = spi_master_get_devdata(master);
	platform_set_drvdata(dev, master);

	INIT_WORK(&c->work, spi_work);
	spin_lock_init(&c->lock);
	INIT_LIST_HEAD(&c->queue);

	c->workqueue = create_singlethread_workqueue(dev_name(master->dev.parent));
	if (!c->workqueue)
		goto exit_busy;

	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST;

	master->bus_num = dev->id;
	master->setup = setup;
	master->cleanup = cleanup;
	master->transfer = transfer;
	master->num_chipselect = (u16)UINT_MAX; /* any GPIO numbers */

	ret = spi_register_master(master);
	if (ret)
		goto exit;
	
	bl_spi_bus_init(dev->id);
	
	return 0;
	
exit_busy:
	ret = -EBUSY;
exit:
	if (c->workqueue)
		destroy_workqueue(c->workqueue);
	platform_set_drvdata(dev, NULL);
	spi_master_put(master);
	return ret;
}

static int __exit remove(struct platform_device *dev)
{
	struct spi_master *master = spi_master_get(platform_get_drvdata(dev));
	struct lilac_spi *c = spi_master_get_devdata(master);

	spi_unregister_master(master);
	platform_set_drvdata(dev, NULL);
	destroy_workqueue(c->workqueue);
	spi_master_put(master);
	return 0;
}

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:lilac_spi");

static struct platform_driver spi_driver =
{
	.probe = probe,
	.remove = __exit_p(remove),
	.driver = {
		.name = "lilac_spi",
		.owner = THIS_MODULE,
	},
};

static int __init spi_init(void)
{
	return platform_driver_register(&spi_driver);
}
module_init(spi_init);

static void __exit spi_exit(void)
{
	platform_driver_unregister(&spi_driver);
}
module_exit(spi_exit);

MODULE_DESCRIPTION("Lilac SPI Driver");
MODULE_LICENSE("GPL");

#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */

