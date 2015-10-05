#if defined(CONFIG_BCM_KF_MIPS_BCM9685XX)
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <../drivers/mtd/mtdcore.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/platform_device.h>
#include <asm/bootinfo.h>
#include "bl_lilac_cr.h"
#include <bcm_hwdefs.h>
#include <board.h>
#include "bl_lilac_mips_blocks.h"

extern int board_nand_init(struct nand_chip *nand);
extern void board_nand_clock_down(void);

//static struct platform_device spi_controller = {
//	.name		= "lilac_spi",
//	.id		= 0,	/* bus number */
//};

//static struct platform_device *platform_devs[] __initdata = {
//	&spi_controller,
//};

//static struct mtd_partition spi_primary_no_nand_parts[] = {
//	{
//		.name	= "rootfs0",
//		.offset	= SPIF_ROOTFS_PARTITION_OFFSET(0),
//		.size	= 0, /* calculated later on according to the flash size*/
//	},
//	{
//		.name	= "boot",
//		.offset	= SPIF_STARTER_PARTITION_OFFSET,
//		.size	= SPIF_STARTER_PARTITION_SIZE + SPIF_CFG_PARTITION_SIZE + SPIF_UBOOT_PARTITION_SIZE*2
//	},
//};

//static struct flash_platform_data spiflash_data = {
//	.name		= "SPI Flash",
//	.type		= "m25p128",
//};

//static struct spi_board_info spi_devs[] __initdata = {
//	{
//		.modalias	= "m25p80",
//		.max_speed_hz	= 50000000,
//		.bus_num	= 0,
//		.chip_select	= 6,
//		.mode		= 3,
//		.platform_data	= &spiflash_data,
//	},
//};

static struct mtd_partition bcm63XX_nand_parts[] = 
{
	{name: "rootfs",		offset: 0, size: 0},
	{name: "rootfs_update",	offset: 0, size: 0},
	{name: "data",			offset: 0, size: 0},
	{name: "nvram",			offset: 0, size: 0},
	{name: "image",			offset: 0, size: 0},
	{name: "image_update",	offset: 0, size: 0},
	{name: NULL,			offset: 0, size: 0},
	{name: NULL,			offset: 0, size: 0},
	{name: NULL,			offset: 0, size: 0}
};

static int __init 
is_split_partition (struct mtd_info* mtd, unsigned long offset, unsigned long size, unsigned long *split_offset)
{
	uint8_t buf[0x100];
	size_t retlen;
	int split_found = 0;

	/* Search RootFS partion for split marker.
	 * Marker is located in the last 0x100 bytes of the last BootFS Erase Block
	 * If marker is found, we have separate Boot and Root Partitions.
	 */
	for (*split_offset = offset + mtd->erasesize; *split_offset <= offset + size; *split_offset += mtd->erasesize)
	{
		if (mtd->_block_isbad(mtd, *split_offset - mtd->erasesize)) {
			continue;
		}
		mtd->_read(mtd, *split_offset - 0x100, 0x100, &retlen, buf);

		if (!strncmp (BCM_BCMFS_TAG, buf, strlen (BCM_BCMFS_TAG))) {
			if (!strncmp (BCM_BCMFS_TYPE_UBIFS, &buf[strlen (BCM_BCMFS_TAG)], strlen (BCM_BCMFS_TYPE_UBIFS)))
			{
				printk("***** Found UBIFS Marker at 0x%08lx\n", *split_offset - 0x100); 
				split_found = 1;
				break;
			}
		}
	}

	return split_found;
}

static int __init mtd_init(void)
{
	unsigned long straps;
	struct mtd_info *nand_info;
	int nand_present;

	nand_info = kmalloc(sizeof(struct mtd_info) + sizeof(struct nand_chip), GFP_KERNEL);
	if(!nand_info) 
	{
		printk("Unable to allocate NAND MTD dev structure.\n");
		return -ENOMEM;
	}
	memset(nand_info, 0, sizeof(struct mtd_info) + sizeof(struct nand_chip));
	nand_info->priv = (void *)nand_info + sizeof(struct mtd_info);
	
	board_nand_init(nand_info->priv);

	/* Scan to check existence of the nand device */
	nand_present = nand_scan(nand_info, 1) == 0 ? 1:0;
	
	if(!nand_present)
		board_nand_clock_down();
	
	BL_CR_CONTROL_REGS_STRPL_READ(straps);
	if(!(straps & (1<<16)) || nand_present) 
	{
		static NVRAM_DATA nvram;
		unsigned long rootfs_ofs;
		int nr_parts;
		int rootfs, rootfs_update;
		unsigned long split_offset;

		// boot from nand, or boot from spif with nand
		if(!(straps & (1<<16)))
		{
			printk("Boot from NAND FLASH\n");
		}
		else
		{
			printk("Boot from SPI FLASH, Root FS is on NAND\n");
		}

		/* Root FS.  The CFE RAM boot loader saved the rootfs offset that the
		 * Linux image was loaded from.
		 */
		kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs);
		kerSysNvRamGet((char *)&nvram, sizeof(nvram), 0);
		nr_parts = 6;

		if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
		{
			rootfs = NP_ROOTFS_1;
			rootfs_update = NP_ROOTFS_2;
		}
		else
		{
			rootfs = NP_ROOTFS_2;
			rootfs_update = NP_ROOTFS_1;
		}

		/* RootFS partition */
		bcm63XX_nand_parts[0].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
		bcm63XX_nand_parts[0].size = nvram.ulNandPartSizeKb[rootfs]*1024;
		bcm63XX_nand_parts[0].ecclayout = nand_info->ecclayout;

		/* This partition is used for flashing images */
		bcm63XX_nand_parts[4].offset = bcm63XX_nand_parts[0].offset;
		bcm63XX_nand_parts[4].size = bcm63XX_nand_parts[0].size;
		bcm63XX_nand_parts[4].ecclayout = nand_info->ecclayout;

		if (is_split_partition (nand_info, bcm63XX_nand_parts[0].offset, bcm63XX_nand_parts[0].size, &split_offset))
		{
			/* RootFS partition */
			bcm63XX_nand_parts[0].offset = split_offset;
			bcm63XX_nand_parts[0].size -= (split_offset - nvram.ulNandPartOfsKb[rootfs]*1024);

			/* BootFS partition */
			bcm63XX_nand_parts[nr_parts].name = "bootfs";
			bcm63XX_nand_parts[nr_parts].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
			bcm63XX_nand_parts[nr_parts].size = split_offset - nvram.ulNandPartOfsKb[rootfs]*1024;
			bcm63XX_nand_parts[nr_parts].ecclayout = nand_info->ecclayout;
			nr_parts++;

			if (strncmp(arcs_cmdline, "root=", 5)) {
				kerSysSetBootParm("ubi.mtd", "0");
				kerSysSetBootParm("root=", "ubi:rootfs_ubifs");
				kerSysSetBootParm("rootfstype=", "ubifs");
			}
		}
		else {
			if (strncmp(arcs_cmdline, "root=", 5)) {
				kerSysSetBootParm("root=", "mtd:rootfs");
				kerSysSetBootParm("rootfstype=", "jffs2");
			}
		}

		/* RootFS_update partition */
		bcm63XX_nand_parts[1].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
		bcm63XX_nand_parts[1].size = nvram.ulNandPartSizeKb[rootfs_update]*1024;
		bcm63XX_nand_parts[1].ecclayout = nand_info->ecclayout;

		/* This partition is used for flashing images */
		bcm63XX_nand_parts[5].offset = bcm63XX_nand_parts[1].offset;
		bcm63XX_nand_parts[5].size = bcm63XX_nand_parts[1].size;
		bcm63XX_nand_parts[5].ecclayout = nand_info->ecclayout;

		if (is_split_partition (nand_info, bcm63XX_nand_parts[1].offset, bcm63XX_nand_parts[1].size, &split_offset))
		{
			/* rootfs_update partition */
			bcm63XX_nand_parts[1].offset = split_offset;
			bcm63XX_nand_parts[1].size -= (split_offset - nvram.ulNandPartOfsKb[rootfs_update]*1024);

			/* bootfs_update partition */
			bcm63XX_nand_parts[nr_parts].name = "bootfs_update";
			bcm63XX_nand_parts[nr_parts].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
			bcm63XX_nand_parts[nr_parts].size = split_offset - nvram.ulNandPartOfsKb[rootfs_update]*1024;
			bcm63XX_nand_parts[nr_parts].ecclayout = nand_info->ecclayout;
			nr_parts++;
		}

		/* Data (psi, scratch pad) */
		bcm63XX_nand_parts[2].offset = nvram.ulNandPartOfsKb[NP_DATA] * 1024;
		bcm63XX_nand_parts[2].size = nvram.ulNandPartSizeKb[NP_DATA] * 1024;
		bcm63XX_nand_parts[2].ecclayout = nand_info->ecclayout;

		/* Boot and NVRAM data */
		bcm63XX_nand_parts[3].offset = nvram.ulNandPartOfsKb[NP_BOOT] * 1024;
		bcm63XX_nand_parts[3].size = nvram.ulNandPartSizeKb[NP_BOOT] * 1024;
		bcm63XX_nand_parts[3].ecclayout = nand_info->ecclayout;

		add_mtd_partitions(nand_info, bcm63XX_nand_parts, nr_parts);
	}
//	else
//	{
//		kfree(nand_info);
//
//		printk("Boot from SPI FLASH, no NAND present\n");
//		spiflash_data.parts = spi_primary_no_nand_parts;
//		spiflash_data.nr_parts = ARRAY_SIZE(spi_primary_no_nand_parts);
//	}
	
//	spi_register_board_info(spi_devs, ARRAY_SIZE(spi_devs));
//	platform_add_devices(platform_devs, ARRAY_SIZE(platform_devs));
	
	return 0;
}

module_init(mtd_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD map and partitions for Lilac boards");

#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */
