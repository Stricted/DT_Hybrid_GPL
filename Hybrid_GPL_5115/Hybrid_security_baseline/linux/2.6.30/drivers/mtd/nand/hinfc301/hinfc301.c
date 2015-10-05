/******************************************************************************
*    NAND Flash Controller V301 Device Driver
*    Copyright (c) 2009-2010 by Hisilicon.
*    All rights reserved.
* ***
*    written by CaiZhiYong. 2010-11-04
*    
******************************************************************************/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/errno.h>
#include <linux/platform_device.h>
#include <mach/kexport.h>
#include "atpconfig.h"
#include "atpnandparts.h"

#ifdef CONFIG_HISI_SD5115
#include "kerneltag.h"
#endif

#ifdef CONFIG_MTD_PARTITIONS
#  include <linux/mtd/partitions.h>
#  define mtd_has_partitions()  (1)
#else
#  define mtd_has_partitions()  (0)
#endif /* CONFIG_MTD_PARTITIONS */

//#define CONFIG_MTD_HISI_SD5115_NFC_DBG_PROC_FILE
//#define CONFIG_MTD_HISI_SD5115_NFC_DBG_EC_NOTICE
//#define CONFIG_MTD_HISI_SD5115_NFC_DBG_STAT_PROC_FILE

#include "hinfc301.h"
#include "hinfc301dbg.c"

#ifdef CONFIG_HISI_SD5115
extern unsigned char gac_flash_info[];

static struct hinand_info
{
    struct mtd_info mtd;
    int nr_parts;
    struct mtd_partition* parts;
};
#endif

/*****************************************************************************/

extern void hinfc301_controller_enable(struct hinfc_host *host, int enable);

extern void (*nand_base_oob_resize)(struct mtd_info *mtd, struct nand_chip *chip);

/*****************************************************************************/

static char *get_ecctype_str(enum ecc_type ecctype)
{
	static char *ecctype_str[] =
	{ "None", "1bit", "4Bytes", "8Bytes", "24bits/1K", "unknown", "unknown", "unknown"};
	return ecctype_str[ecctype];
}
/*****************************************************************************/

static char *get_pagesize_str(enum page_type pagetype)
{
	static char *pagesize_str[] = {"512", "2K", "4K", "8K", "unknown" };
	return pagesize_str[pagetype];
}
/*****************************************************************************/

static unsigned int get_pagesize(enum page_type pagetype)
{
	unsigned int pagesize[] = {_512B, _2K, _4K, _8K, 0};
	return pagesize[pagetype];
}
/*****************************************************************************/

static void hinfc301_dma_transfer(struct hinfc_host *host, int todev)
{
	unsigned long reg_val;
	hinfc_write(host, host->dma_buffer, HINFC301_DMA_ADDR_DATA);
	hinfc_write(host, host->dma_oob, HINFC301_DMA_ADDR_OOB);

	if (host->ecctype == et_ecc_none)
	{
		hinfc_write(host,
			((host->oobsize & HINFC301_DMA_LEN_OOB_MASK) << HINFC301_DMA_LEN_OOB_SHIFT),
			HINFC301_DMA_LEN);
			
		if(HI_CHIP_ID_MPW_E == hi_kernel_get_chip_id())
		{
			hinfc_write(host,
				HINFC301_DMA_PARA_DATA_RW_EN
				| HINFC301_DMA_PARA_OOB_RW_EN
				| ((host->n24bit_ext_len & HINFC301_DMA_PARA_EXT_LEN_MASK) << HINFC301_DMA_PARA_EXT_LEN_SHIFT),
				HINFC301_DMA_PARA);
		}
		else
		{
			hinfc_write(host,
				HINFC301_DMA_PARA_DATA_RW_EN
				| HINFC301_DMA_PARA_OOB_RW_EN,
				HINFC301_DMA_PARA);
		}
	}
	else
	{
		if(HI_CHIP_ID_MPW_E == hi_kernel_get_chip_id())
		{
			hinfc_write(host,
				HINFC301_DMA_PARA_DATA_RW_EN
				| HINFC301_DMA_PARA_OOB_RW_EN
				| HINFC301_DMA_PARA_DATA_EDC_EN
				| HINFC301_DMA_PARA_OOB_EDC_EN
				| HINFC301_DMA_PARA_DATA_ECC_EN
				| HINFC301_DMA_PARA_OOB_ECC_EN
				| ((host->n24bit_ext_len & HINFC301_DMA_PARA_EXT_LEN_MASK) << HINFC301_DMA_PARA_EXT_LEN_SHIFT),
				HINFC301_DMA_PARA);
		}
		else
		{
			hinfc_write(host,
				HINFC301_DMA_PARA_DATA_RW_EN
				| HINFC301_DMA_PARA_OOB_RW_EN
				| HINFC301_DMA_PARA_DATA_EDC_EN
				| HINFC301_DMA_PARA_OOB_EDC_EN
				| HINFC301_DMA_PARA_DATA_ECC_EN
				| HINFC301_DMA_PARA_OOB_ECC_EN,
				HINFC301_DMA_PARA);
		}
	}

	reg_val = (HINFC301_DMA_CTRL_DMA_START
		| HINFC301_DMA_CTRL_BURST4_EN
		| HINFC301_DMA_CTRL_BURST8_EN
		| HINFC301_DMA_CTRL_BURST16_EN
		| HINFC301_DMA_CTRL_DATA_AREA_EN
		| HINFC301_DMA_CTRL_OOB_AREA_EN
		| ((host->addr_cycle == 4 ? 1 : 0) << HINFC301_DMA_CTRL_ADDR_NUM_SHIFT)
		| ((host->chipselect & HINFC301_DMA_CTRL_CS_MASK) << HINFC301_DMA_CTRL_CS_SHIFT));

	if (todev)
		reg_val |= HINFC301_DMA_CTRL_WE;

	hinfc_write(host, reg_val, HINFC301_DMA_CTRL);

	while ((hinfc_read(host, HINFC301_DMA_CTRL)) & HINFC301_DMA_CTRL_DMA_START)
		_cond_resched();
}
/*****************************************************************************/

static void hinfc301_chip_reset(struct hinfc_host *host)
{
	hinfc_write(host, NAND_CMD_RESET, HINFC301_CMD);
	hinfc_write(host,
		(HINFC301_OP_CMD1_EN 
		| ((host->chipselect & HINFC301_OP_NF_CS_MASK) << HINFC301_OP_NF_CS_SHIFT)
		| HINFC301_OP_WAIT_READY_EN),
		HINFC301_OP);

	while ((hinfc_read(host, HINFC301_STATUS) & 0x1) == 0x0)
		;
}
static int hinfc301_chip_reset_test(struct hinfc_host *host)
{
	int i;
	hinfc_write(host, NAND_CMD_RESET, HINFC301_CMD);
	hinfc_write(host,
		(HINFC301_OP_CMD1_EN 
		| ((host->chipselect & HINFC301_OP_NF_CS_MASK) << HINFC301_OP_NF_CS_SHIFT)
		| HINFC301_OP_WAIT_READY_EN),
		HINFC301_OP);
	for(i=0;i<50;i++)
	{
		if((hinfc_read(host, HINFC301_STATUS) & 0x1) != 0x0)
		return  0;
		mdelay(1);	
	}
	return 1;
}
/*****************************************************************************/

static void hinfc301_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (ctrl & NAND_ALE)
	{
		unsigned int addr_value = 0;
		unsigned int addr_offset = 0;

		if (ctrl & NAND_CTRL_CHANGE)
		{
			host->addr_cycle = 0x0;
			host->addr_value[0] = 0x0;
			host->addr_value[1] = 0x0;
		}
		addr_offset =  host->addr_cycle << 3;

		if (host->addr_cycle >= HINFC301_ADDR_CYCLE_MASK)
		{
			addr_offset = (host->addr_cycle - HINFC301_ADDR_CYCLE_MASK) << 3;
			addr_value = 1;
		}

		host->addr_value[addr_value] |= ((dat & 0xff) << addr_offset);

		host->addr_cycle ++;
	}

	if ((ctrl & NAND_CLE) && (ctrl & NAND_CTRL_CHANGE))
	{
		host->command = dat & 0xff;
		switch (host->command)
		{
		case NAND_CMD_PAGEPROG:

			hinfc_write(host, host->NFC_CON, HINFC301_CON);
			hinfc_write(host, host->addr_value[0] & 0xffff0000, HINFC301_ADDRL);
			hinfc_write(host, host->addr_value[1], HINFC301_ADDRH);
			hinfc_write(host, NAND_CMD_PAGEPROG << 8 | NAND_CMD_SEQIN, HINFC301_CMD);

			hinfc301_dma_transfer(host, 1);

			dbg_nand_proc_save_logs(host, "W");
			dbg_nand_stat_operation(host, "W");

			break;

		case NAND_CMD_READSTART: 

			hinfc_write(host, HINFC301_INTCLR_UE, HINFC301_INTCLR);
			hinfc_write(host, host->NFC_CON, HINFC301_CON);
			hinfc_write(host, host->addr_value[0] & 0xffff0000, HINFC301_ADDRL);
			hinfc_write(host, host->addr_value[1], HINFC301_ADDRH);
			hinfc_write(host, NAND_CMD_READSTART << 8 | NAND_CMD_READ0, HINFC301_CMD);

			hinfc_write(host, 0, HINFC301_LOG_READ_ADDR);
			hinfc_write(host, (host->pagesize + host->oobsize), HINFC301_LOG_READ_LEN);

			hinfc301_dma_transfer(host, 0);
			host->uc_er = ((hinfc_read(host, HINFC301_INTS) & HINFC301_INTS_UE) ? 1 : 0);

			dbg_nand_ec_notice(host);
			dbg_nand_proc_save_logs(host, "R");
			dbg_nand_stat_operation(host, "R");
			host->uc_er = 0;

			break;

		case NAND_CMD_ERASE2:
			hinfc_write(host, host->NFC_CON_ECC_NONE, HINFC301_CON);
			hinfc_write(host, host->addr_value[0], HINFC301_ADDRL);
			hinfc_write(host, (NAND_CMD_ERASE2 << 8) | NAND_CMD_ERASE1, HINFC301_CMD);

			hinfc_write(host,
				HINFC301_OP_WAIT_READY_EN
				| HINFC301_OP_CMD2_EN
				| HINFC301_OP_CMD1_EN
				| HINFC301_OP_ADDR_EN
				| ((host->chipselect & HINFC301_OP_NF_CS_MASK) << HINFC301_OP_NF_CS_SHIFT)
				| ((host->addr_cycle & HINFC301_OP_ADDR_CYCLE_MASK) << HINFC301_OP_ADDR_CYCLE_SHIFT),
				HINFC301_OP);

			while ((hinfc_read(host, HINFC301_STATUS) & 0x1) == 0x0)
				;
			dbg_nand_proc_save_logs(host, "E");
			dbg_nand_stat_operation(host, "E");

			break;

		case NAND_CMD_READID:

			memset((unsigned char *)(chip->IO_ADDR_R), 0, 0x10);

			hinfc_write(host, host->NFC_CON_ECC_NONE, HINFC301_CON);
			hinfc_write(host, NAND_CMD_READID, HINFC301_CMD);
			hinfc_write(host, 0, HINFC301_ADDRL);

			hinfc_write(host,
				HINFC301_OP_CMD1_EN
				| HINFC301_OP_ADDR_EN
				| HINFC301_OP_READ_DATA_EN
				| HINFC301_OP_WAIT_READY_EN
				| ((host->chipselect & HINFC301_OP_NF_CS_MASK) << HINFC301_OP_NF_CS_SHIFT)
				| (1 << HINFC301_OP_ADDR_CYCLE_SHIFT),
				HINFC301_OP);

			host->addr_cycle = 0x0;

			while ((hinfc_read(host, HINFC301_STATUS) & 0x1) == 0x0)
				;

			break;

		case NAND_CMD_STATUS:

			hinfc_write(host, host->NFC_CON_ECC_NONE, HINFC301_CON);
			hinfc_write(host, NAND_CMD_STATUS, HINFC301_CMD);
			hinfc_write(host,
				HINFC301_OP_CMD1_EN
				| HINFC301_OP_READ_DATA_EN
				| HINFC301_OP_WAIT_READY_EN
				| ((host->chipselect & HINFC301_OP_NF_CS_MASK) << HINFC301_OP_NF_CS_SHIFT),
				HINFC301_OP);

			while ((hinfc_read(host, HINFC301_STATUS) & 0x1) == 0x0)
				;

			break;

		case NAND_CMD_SEQIN:
		case NAND_CMD_ERASE1:
		case NAND_CMD_READ0:
			break;
		case NAND_CMD_RESET:

			hinfc301_chip_reset(host);

			break;

		default :
			break;
		}
	}

	if ((dat == NAND_CMD_NONE) && host->addr_cycle)
	{
		if (host->command == NAND_CMD_SEQIN 
			|| host->command == NAND_CMD_READ0 
			|| host->command == NAND_CMD_READID)
		{
			host->offset = 0x0;
			host->column = (host->addr_value[0] & 0xffff);
		}
	}
}
/*****************************************************************************/

static int hinfc301_dev_ready(struct mtd_info *mtd)
{
	return 0x1;
}
/*****************************************************************************/

static void hinfc301_select_chip(struct mtd_info *mtd, int chipselect)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (chipselect < 0)
		return;

	if (chipselect > CONFIG_MTD_HISI_SD5115_NFC_MAX_CHIP)
	{
		DBG_BUG("invalid chipselect: %d\n", chipselect);
	}

	host->chipselect = chipselect;
}
/*****************************************************************************/

static uint8_t hinfc301_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (host->command == NAND_CMD_STATUS)
	{
		return readb(chip->IO_ADDR_R);
	}

	host->offset++;

	if (host->command == NAND_CMD_READID)
	{
		return readb(chip->IO_ADDR_R + host->offset - 1);
	}

	return readb(host->buffer + host->column + host->offset - 1);
}
/*****************************************************************************/

static u16 hinfc301_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	host->offset += 2;
	return readw(host->buffer + host->column + host->offset - 2);
}
/*****************************************************************************/

static void hinfc301_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	memcpy(host->buffer + host->column + host->offset, buf, len);
	host->offset += len;
}
/*****************************************************************************/

static void hinfc301_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	memcpy(buf, host->buffer + host->column + host->offset, len);
	host->offset += len;
}
/*****************************************************************************/

static char *int_to_size(unsigned long long size)
{
	int ix;
	static char buffer[20];
	char *fmt[] = {"%u", "%uK", "%uM", "%uG", "%uT", "%uT"};

	for (ix = 0; (ix < 5) && !(size & 0x3FF) && size; ix++)
	{
		size = (size >> 10);
	}
	sprintf(buffer, fmt[ix], size);
	return buffer;
}
/*****************************************************************************/

static struct nand_ecclayout nand_ecc_default =
{
	.oobfree = {{2, 30}}
};

static struct nand_ecclayout nand_ecc_2K_1bit =
{
	.oobfree = {{22, 30}}
};
#ifdef CONFIG_MTD_HISI_SD5115_NFC_HARDWARE_PAGESIZE_ECC 
static struct nand_ecclayout nand_ecc_2K_4bytes =
{
	.oobfree = {{2, 34}}    /* <DTS2012102001555 g68080 2012-10-20 mod */
};
#endif /* CONFIG_MTD_HISI_SD5115_NFC_HARDWARE_PAGESIZE_ECC */
/*****************************************************************************/

static struct page_page_ecc_info page_page_ecc_info[] =
{
	{pt_pagesize_8K, et_ecc_24bit1k, 368, &nand_ecc_default},
	{pt_pagesize_8K, et_ecc_none,    32,  &nand_ecc_default},

	{pt_pagesize_4K, et_ecc_24bit1k, 200, &nand_ecc_default},
	{pt_pagesize_4K, et_ecc_4bytes,  128, &nand_ecc_default},
	{pt_pagesize_4K, et_ecc_1bit,    128, &nand_ecc_default},
	{pt_pagesize_4K, et_ecc_none,    32,  &nand_ecc_default},

	{pt_pagesize_2K, et_ecc_1bit,    64,  &nand_ecc_2K_1bit},
	/* 
	 * 2k4bytes not support yaffs2 file system, it's not compatibility.
	 * only hardward config mode allow support 2k4Bytes,
	 */
#ifdef CONFIG_MTD_HISI_SD5115_NFC_HARDWARE_PAGESIZE_ECC
	{pt_pagesize_2K, et_ecc_4bytes,  64,  &nand_ecc_2K_4bytes},
#endif /* CONFIG_MTD_HISI_SD5115_NFC_HARDWARE_PAGESIZE_ECC */
	{pt_pagesize_2K, et_ecc_none,    32,  &nand_ecc_default},

	{0,0,0,NULL},
};
/*****************************************************************************/
/* used the best correct arithmetic. */
struct page_page_ecc_info *hinfc301_get_best_ecc(struct mtd_info *mtd)
{
	struct page_page_ecc_info *best = NULL;
	struct page_page_ecc_info *info = page_page_ecc_info;

	for (; info->layout; info++)
	{
		if (get_pagesize(info->pagetype) != mtd->writesize)
			continue;

		if (mtd->oobsize < info->oobsize)
			continue;

		if (!best || (best->ecctype < info->ecctype))
		{
			best = info;
		}
	}

	if (!best)
	{
		DBG_BUG("not support this pagesize(%d) and oobsize(%d).\n",
			mtd->writesize, mtd->oobsize);
	}

	return best;
}
/*****************************************************************************/
/* force the pagesize and ecctype */
struct page_page_ecc_info *hinfc301_force_ecc
(
	struct mtd_info *mtd,
	enum page_type pagetype,
	enum ecc_type ecctype,
	char *cfgmsg,
	int allow_pagediv
)
{
	int pagesize;
	struct page_page_ecc_info *fit = NULL;
	struct page_page_ecc_info *info = page_page_ecc_info;

	for (; info->layout; info++)
	{
		if (info->pagetype == pagetype
			&& info->ecctype == ecctype)
		{
			fit = info;
			break;
		}
	}

	if (!fit)
	{
		DBG_BUG("system not support this nand %s: pagesize:%s, ecctype:%s\n",
			cfgmsg,
			get_pagesize_str(pagetype),
			get_ecctype_str(ecctype));
		return NULL;
	}

	pagesize = get_pagesize(pagetype);
	if ((pagesize != mtd->writesize)
		&& (pagesize > mtd->writesize || !allow_pagediv))
	{
		DBG_BUG("%s is inconsistent, config pagesize %d, "
			"but the nand chip pageszie is %d\n",
			cfgmsg,
			pagesize,
			mtd->writesize);
		return NULL;
	}

	if (fit->oobsize > mtd->oobsize)
	{
		DBG_BUG("%s is inconsistent, config %s require oobsize(%d), "
			"but the nand chip oobsize is %d, littler than config require.\n",
			cfgmsg, get_ecctype_str(ecctype),
			fit->oobsize,
			mtd->oobsize);
		return NULL;
	}

	return fit;
}
/*****************************************************************************/

static void hinfc301_ecc_probe(struct mtd_info *mtd, struct nand_chip *chip)
{
	char *start_type = "unknown";
	struct page_page_ecc_info *best = NULL;
	struct hinfc_host *host = chip->priv;
    uint con_pagesize_mask;
    uint con_ready_busy_sel;
    uint con_ecctype_shift;

    if(HI_CHIP_ID_MPW_E != hi_kernel_get_chip_id())
    {
        con_pagesize_mask=HINFC301_CON_PAGESIZE_MASK;
        con_ready_busy_sel=HINFC301_CON_READY_BUSY_SEL;
        con_ecctype_shift=HINFC301_CON_ECCTYPE_SHIFT;
    }
    else
    {
        con_pagesize_mask=HINFC301_MPW_CON_PAGESIZE_MASK;
        con_ready_busy_sel=HINFC301_MPW_CON_READY_BUSY_SEL;
        con_ecctype_shift=HINFC301_MPW_CON_ECCTYPE_SHIFT;
    }

#ifdef CONFIG_MTD_HISI_SD5115_NFC_AUTO_PAGESIZE_ECC
	best = hinfc301_get_best_ecc(mtd);
	start_type = "Auto";
#endif /* CONFIG_MTD_HISI_SD5115_NFC_AUTO_PAGESIZE_ECC */

#ifdef CONFIG_MTD_HISI_SD5115_NFC_HARDWARE_PAGESIZE_ECC
#if 0
	best = hinfc301_force_ecc(mtd,
		((host->NFC_CON >> HINFC301_CON_PAGEISZE_SHIFT) & con_pagesize_mask),
		((host->NFC_CON >> con_ecctype_shift) & HINFC301_CON_ECCTYPE_MASK),
#else
	best = hinfc301_force_ecc(mtd,
			pt_pagesize_2K,
			et_ecc_4bytes,      /* <DTS2012102001555 g68080 2012-10-20 mod */
#endif
		"hardware config", 0);
	start_type = "Hardware";
#endif /* CONFIG_MTD_HISI_SD5115_NFC_HARDWARE_PAGESIZE_ECC */

#ifdef CONFIG_MTD_HISI_SD5115_NFC_PAGESIZE_AUTO_ECC_NONE
	{
		int pagetype;

		switch (mtd->writesize)
		{
		case _512B: pagetype = pt_pagesize_512; break;
		case _2K:   pagetype = pt_pagesize_2K;  break;
		case _4K:   pagetype = pt_pagesize_4K;  break;
		case _8K:   pagetype = pt_pagesize_8K;  break;
		default:    pagetype = pt_pagesize_2K;  break;
		}
		best = hinfc301_force_ecc(mtd, pagetype, et_ecc_none, "force config", 0);
		start_type = "AutoForce";
	}
#endif /* CONFIG_MTD_HISI_SD5115_NFC_PAGESIZE_AUTO_ECC_NONE */

	if (!best)
	{
		DBG_BUG("Please select nand flash pagesize and ecctype!\n");
	}

	if (best->ecctype != et_ecc_none)
		mtd->oobsize = best->oobsize;
	chip->ecc.layout = best->layout;

	host->ecctype  = best->ecctype;
	host->pagesize = get_pagesize(best->pagetype);
	host->oobsize  = mtd->oobsize;

	if (host->ecctype == et_ecc_24bit1k)
	{
		if (host->pagesize == _4K)
		{
			host->n24bit_ext_len = 0x03; /* 8bytes; */
		}
		else if (host->pagesize == _8K)
		{
			host->n24bit_ext_len = 0x01; /* 4bytes; */
		}
	}
	host->dma_oob = host->dma_buffer + host->pagesize;

	host->NFC_CON  = (HINFC301_CON_OP_MODE_NORMAL
		| ((best->pagetype & con_pagesize_mask) << HINFC301_CON_PAGEISZE_SHIFT)
		| con_ready_busy_sel
		| ((best->ecctype & HINFC301_CON_ECCTYPE_MASK) << con_ecctype_shift));

	host->NFC_CON_ECC_NONE  = (HINFC301_CON_OP_MODE_NORMAL
		| ((best->pagetype & con_pagesize_mask) << HINFC301_CON_PAGEISZE_SHIFT)
		| con_ready_busy_sel);

	if (mtd->writesize > NAND_MAX_PAGESIZE
		|| mtd->oobsize > NAND_MAX_OOBSIZE)
	{
		DBG_BUG("kernel not support this NAND. "
			"please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
	}

	if (mtd->writesize != host->pagesize)
	{
		unsigned int shift = 0;
		unsigned int writesize = mtd->writesize;
		while (writesize > host->pagesize)
		{
			writesize >>= 1;
			shift++;
		}
		chip->chipsize = chip->chipsize >> shift;
		mtd->erasesize = mtd->erasesize >> shift;
		mtd->writesize = host->pagesize;
		printk("Nand divide into 1/%u\n", (1 << shift));
	}

	dbg_nand_stat_proc_init(mtd->writesize, mtd->erasesize);

	printk("Nand(%s): ", start_type);
	printk("Block:%sB ", int_to_size(mtd->erasesize));
	printk("Page:%sB ",  int_to_size(mtd->writesize));
	printk("Chip:%sB ",  int_to_size(chip->chipsize));
	printk("OOB:%sB ",   int_to_size(mtd->oobsize));
	printk("ECC:%s ",   get_ecctype_str(host->ecctype));
	printk("\n");
}
/*****************************************************************************/

static int hinfc301_nand_init(struct hinfc_host *host, struct nand_chip *chip)
{
    uint con_ready_busy_sel;
    uint con_ecctype_shift;

    if(HI_CHIP_ID_MPW_E != hi_kernel_get_chip_id())
    {
        con_ready_busy_sel=HINFC301_CON_READY_BUSY_SEL;
        con_ecctype_shift=HINFC301_CON_ECCTYPE_SHIFT;
    }
    else
    {
        con_ready_busy_sel=HINFC301_MPW_CON_READY_BUSY_SEL;
        con_ecctype_shift=HINFC301_MPW_CON_ECCTYPE_SHIFT;
    }

	chip->priv        = host;
	chip->cmd_ctrl    = hinfc301_cmd_ctrl;
	chip->dev_ready   = hinfc301_dev_ready;
	chip->select_chip = hinfc301_select_chip;
	chip->read_byte   = hinfc301_read_byte;
	chip->read_word   = hinfc301_read_word;
	chip->write_buf   = hinfc301_write_buf;
	chip->read_buf    = hinfc301_read_buf;

	chip->chip_delay = HINFC301_CHIP_DELAY;
	//chip->options    = NAND_NO_AUTOINCR |  NAND_SKIP_BBTSCAN;
	chip->options    = NAND_NO_AUTOINCR;

	chip->ecc.layout = NULL;
	chip->ecc.mode   = NAND_ECC_NONE;

	host->chip          = chip;
	host->addr_cycle    = 0;
	host->addr_value[0] = 0;
	host->addr_value[1] = 0;
	host->chipselect    = 0;

	host->NFC_CON = (hinfc_read(host, HINFC301_CON)
		| HINFC301_CON_OP_MODE_NORMAL | con_ready_busy_sel);
	host->NFC_CON_ECC_NONE = (host->NFC_CON
		& (~(HINFC301_CON_ECCTYPE_MASK << con_ecctype_shift)));

	host->buffer = dma_alloc_coherent(host->dev, (NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE),
		&host->dma_buffer, GFP_KERNEL);
	if (!host->buffer)
		return 1;

	memset((char *)chip->IO_ADDR_R, 0xff, HINFC301_BUFFER_BASE_ADDRESS_LEN);
	memset(host->buffer, 0xff, (NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE));

	hinfc_write(host,
		SET_HINFC301_PWIDTH(CONFIG_HINFC301_W_LATCH, CONFIG_HINFC301_R_LATCH, CONFIG_HINFC301_RW_LATCH),
		HINFC301_PWIDTH);

	nand_base_oob_resize = hinfc301_ecc_probe;

	dbg_nand_ec_init();
	dbg_nand_proc_init();

	return 0;
}
/*****************************************************************************/
#define MAX_MTD_PARTITIONS         (32)

struct partition_entry
{
	char name[16];
	unsigned long long start;
	unsigned long long length;
	unsigned int flags;
};

struct partition_info
{
	int parts_num;
	struct partition_entry entry[MAX_MTD_PARTITIONS];
	struct mtd_partition parts[MAX_MTD_PARTITIONS];
};
/*****************************************************************************/
static struct partition_info ptn_info;

static int __init parse_nand_partitions(const struct tag *tag)
{
	int i;

	if (tag->hdr.size <= 2)
	{
		printk("%s(%d): tag->hdr.size <= 2\n", __FUNCTION__, __LINE__);
		return 0;
	}
	ptn_info.parts_num = (tag->hdr.size - 2) / (sizeof(struct partition_entry)/sizeof(int));
	memcpy(ptn_info.entry, &tag->u, ptn_info.parts_num * sizeof(struct partition_entry));

	for (i = 0; i < ptn_info.parts_num; i++)
	{
		ptn_info.parts[i].name   = ptn_info.entry[i].name;
		ptn_info.parts[i].size   = (ptn_info.entry[i].length);
		ptn_info.parts[i].offset = (ptn_info.entry[i].start);
		ptn_info.parts[i].mask_flags = 0;
		ptn_info.parts[i].ecclayout  = 0;
	}

	return 0;
}

/* turn to ascii is "HiNp" */
__tagtable(0x48694E70, parse_nand_partitions);
/*****************************************************************************/

#ifdef CONFIG_HISI_SD5115

#define	MAIN_ACTIVE			0
#define	SLAVE_ACTIVE		1

#define HI_TYPE_DOUBLE		1
#define HI_TYPE_SINGLE		2
#define HI_TYPE_BIG_SMALL		3

static void hi_nanddrv_setup_mtd_partitions(struct mtd_info *mtd, struct mtd_partition **parts, int *nr_parts)
{
    unsigned int ulMainOrSlave;
    unsigned int ui_upg_flag = 0;	/* 0:main	1:slave */
    unsigned int ui_sys_type = 0;	/* 1:double	2:single 3:big small endian */


    //内核态不支持flash直接读写，参数都由bootloader传递过来
    ui_sys_type = gac_flash_info[0];
	ui_upg_flag = gac_flash_info[1];

	if((HI_TYPE_DOUBLE==ui_sys_type) && (SLAVE_ACTIVE==ui_upg_flag))	/* 双系统,备系统激活 */
	{ 
        ulMainOrSlave = 1;
    }
    else	/* 单系统只有主系统，或双系统主启动 */
    {
        ulMainOrSlave = 0;
    }

    atp_setup_mtd_partitions(mtd, gac_flash_info+2, ulMainOrSlave, parts, nr_parts);

}
#endif

extern int g_flash_total_size;
extern int g_flash_sector_size;

static int hinfc301_probe(struct platform_device * pltdev)
{
	int result = 0;

	struct hinfc_host *host;
	struct nand_chip  *chip;
	struct mtd_info   *mtd;

	int size = sizeof(struct hinfc_host) + sizeof(struct nand_chip)
		+ sizeof(struct mtd_info);

	host = kmalloc(size, GFP_KERNEL);
	if (!host)
	{
		dev_err(&pltdev->dev, "failed to allocate device structure.\n");
		return -ENOMEM;
	}
	memset((char *)host, 0, size);
	platform_set_drvdata(pltdev, host);

	host->dev  = &pltdev->dev;
	host->chip = chip = (struct nand_chip *)&host[1];
	host->mtd  = mtd  = (struct mtd_info *)&chip[1];

	host->iobase = ioremap(pltdev->resource[0].start,
		pltdev->resource[0].end - pltdev->resource[0].start + 1);
	if (!host->iobase)
	{
		dev_err(&pltdev->dev, "ioremap failed\n");
		kfree(host);
		return -EIO;
	}

	mtd->priv  = chip;
	mtd->owner = THIS_MODULE;
	mtd->name  = (char*)(pltdev->name);

	chip->IO_ADDR_R = chip->IO_ADDR_W = ioremap_nocache(
		pltdev->resource[1].start,
		pltdev->resource[1].end - pltdev->resource[1].start + 1);
	if (!chip->IO_ADDR_R)
	{
		dev_err(&pltdev->dev, "ioremap failed\n");
		iounmap(host->iobase);
		kfree(host);
		return -EIO;
	}
		
		host->sysreg = ioremap_nocache(
		pltdev->resource[2].start,
		pltdev->resource[2].end - pltdev->resource[2].start + 1);
	if (!host->sysreg)
	{
		dev_err(&pltdev->dev, "ioremap failed\n");
		iounmap(chip->IO_ADDR_W);
		iounmap(host->iobase);
		kfree(host);
		return -EIO;
	}

	//hinfc301_controller_enable(host, 1);

	if (hinfc301_nand_init(host, chip))
	{
		dev_err(&pltdev->dev, "failed to allocate device buffer.\n");
		result = -ENOMEM;
		goto err;
	}

	if(hinfc301_chip_reset_test(host))
	{
		result = -ENODEV;
		goto err;
	}
	if (nand_scan(mtd, CONFIG_MTD_HISI_SD5115_NFC_MAX_CHIP)) 
	{
		result = -ENXIO;
		goto err;
	}

    g_flash_total_size = mtd->size;
    g_flash_sector_size = mtd->erasesize;
    printk("g_flash_total_size:%x \n", g_flash_total_size);
	printk("g_flash_sector_size:%x \n", g_flash_sector_size);

	if (mtd_has_partitions()) 
	{
		int i;
		int nr_parts = 0;
		struct mtd_partition *parts = NULL;

#ifdef CONFIG_MTD_CMDLINE_PARTS
		static const char *part_probes[] = { "cmdlinepart", NULL, };
		nr_parts = parse_mtd_partitions(mtd,
			part_probes, &parts, 0);
#else
#ifdef CONFIG_HISI_SD5115
		/* add by zhouyu00204772 2012-04-30 */
		hi_nanddrv_setup_mtd_partitions(mtd, &parts, &nr_parts);
#endif
#endif
		if (!nr_parts)
		{
			nr_parts = ptn_info.parts_num;
			parts    = ptn_info.parts;
		}

		if (nr_parts > 0) 
		{
			for (i = 0; i < nr_parts; i++) 
			{
				DEBUG(MTD_DEBUG_LEVEL2, "partitions[%d] = "
					"{.name = %s, .offset = 0x%.8lld, "
					".size = 0x%.8llx (%lluKiB) }\n",
					i, parts[i].name,
					parts[i].offset,
					parts[i].size,
					parts[i].size / 1024);
			}
			return add_mtd_partitions(mtd, parts, nr_parts);
		}
	}

	if (!add_mtd_device(mtd))
		return 0;

	result = -ENODEV;
	nand_release(mtd);

err:
	if (host->buffer)
	{
		dma_free_coherent(host->dev, (NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE),
			host->buffer, host->dma_buffer);
		host->buffer = NULL;
	}
	iounmap(chip->IO_ADDR_W);
	iounmap(host->iobase);
	iounmap(host->sysreg);
	kfree(host);
	platform_set_drvdata(pltdev, NULL);

	return result;
}
/*****************************************************************************/

int hinfc301_remove(struct platform_device *pltdev)
{
	struct hinfc_host *host = platform_get_drvdata(pltdev);

	hinfc301_controller_enable(host, 0);

	nand_release(host->mtd);
	dma_free_coherent(host->dev, (NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE),
		host->buffer, host->dma_buffer);
	iounmap(host->sysreg);
	iounmap(host->chip->IO_ADDR_W);
	iounmap(host->iobase);
	kfree(host);
	platform_set_drvdata(pltdev, NULL);

	return 0;
}
/*****************************************************************************/

static void hinfc301_pltdev_release(struct device *dev)
{
}
/*****************************************************************************/
#ifdef CONFIG_PM
static int hinfc301_suspend(struct platform_device *pltdev, pm_message_t state)
{
	struct hinfc_host *host = platform_get_drvdata(pltdev);

	while ((hinfc_read(host, HINFC301_STATUS ) & 0x1) == 0x0)
		;

	while ((hinfc_read(host, HINFC301_DMA_CTRL)) & HINFC301_DMA_CTRL_DMA_START)
		_cond_resched();

	hinfc301_controller_enable(host, 0);

	return 0;
}
/*****************************************************************************/

static int hinfc301_resume(struct platform_device *pltdev)
{
	struct hinfc_host *host = platform_get_drvdata(pltdev);

	hinfc301_controller_enable(host, 1);
	hinfc301_chip_reset(host);
	hinfc_write(host,
		SET_HINFC301_PWIDTH(CONFIG_HINFC301_W_LATCH,
			CONFIG_HINFC301_R_LATCH, CONFIG_HINFC301_RW_LATCH),
		HINFC301_PWIDTH);
	return 0;
}
#endif /* CONFIG_PM */
/*****************************************************************************/
static struct platform_driver hinfc301_pltdrv =
{
	.driver.name   = "hi_nfc",
	.probe  = hinfc301_probe,
	.remove = hinfc301_remove,
#ifdef CONFIG_PM
	.suspend  = hinfc301_suspend,
	.resume   = hinfc301_resume,
#endif /* CONFIG_PM */
};
/*****************************************************************************/

static struct resource hinfc301_resources[] = 
{
	{
		.start  = CONFIG_MTD_HISI_SD5115_NFC_REG_BASE_ADDRESS,
		.end    = CONFIG_MTD_HISI_SD5115_NFC_REG_BASE_ADDRESS + HINFC301_REG_BASE_ADDRESS_LEN,
		.flags  = IORESOURCE_MEM,
	},

	{
		.start  = CONFIG_MTD_HISI_SD5115_NFC_BUFFER_BASE_ADDRESS,
		.end    = CONFIG_MTD_HISI_SD5115_NFC_BUFFER_BASE_ADDRESS + HINFC301_BUFFER_BASE_ADDRESS_LEN,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start = CONFIG_MTD_HISI_SD5115_NFC_PERIPHERY_REGBASE,
		.end   = CONFIG_MTD_HISI_SD5115_NFC_PERIPHERY_REGBASE + 100,
		.flags = IORESOURCE_MEM,
	},
};
/*****************************************************************************/

static struct platform_device hinfc301_pltdev =
{
	.name           = "hi_nfc",
	.id             = -1,

	.dev.platform_data     = NULL,
	.dev.dma_mask          = (u64*)~0,
	.dev.coherent_dma_mask = (u64) ~0,
	.dev.release           = hinfc301_pltdev_release,

	.num_resources  = ARRAY_SIZE(hinfc301_resources),
	.resource       = hinfc301_resources,
};
/*****************************************************************************/

static int __init hinfc301_module_init(void)
{
	int result = 0;
	
	printk("Hisilicon Nand Flash Controller V301 Device Driver, Version 1.10\n");

	result = platform_driver_register(&hinfc301_pltdrv);
	if (result < 0)
	{
		return result;
	}

	result = platform_device_register(&hinfc301_pltdev);
	if (result < 0)
	{
		platform_driver_unregister(&hinfc301_pltdrv);
		return result;
	}

	return result;
}
/*****************************************************************************/

static void __exit hinfc301_module_exit (void)
{
	platform_driver_unregister(&hinfc301_pltdrv);
	platform_device_unregister(&hinfc301_pltdev);
}
/*****************************************************************************/

module_init(hinfc301_module_init);
module_exit(hinfc301_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cai ZhiYong");
MODULE_DESCRIPTION("Hisilicon Nand Flash Controller V301 Device Driver, Version 1.10");

/*****************************************************************************/
