#include <linux/init.h>
#include <linux/types.h>
#include <linux/root_dev.h>
#include <linux/kernel.h>
#include <linux/zconf.h>
#include <linux/mtd/map.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#include "atpconfig.h"
#include "kerneltag.h"

#define BOOTPARAMS_LENGTH 1024
extern char g_blparms_buf[BOOTPARAMS_LENGTH];

#define MTD_ROOTFS			0
#define MTD_ROOTFSBAK		1
#define MTD_CONFIG			2
#define MTD_EQUIP			3
#define MTD_UPGFLAG			4
#define MTD_BLROM			5
#define MTD_ROOTFSTAG		6
#define MTD_RESERVED		7
#define MTD_ROOTFSBAKTAG	8	
#define MTD_WLANRF			9
#define MTD_KERNEL			10
#define MTD_KERNELBAK		11	
#define MTD_ALL				12	

#define IMAGE_TAG_SIZE           16
#define IMAGE_MAIN_F             "MAINFF"
#define IMAGE_SLAVE_F            "SLAVEFF"
#define IMAGE_MAIN_S             "MAINSS"
#define IMAGE_SLAVE_S            "SLAVESS"

#define ACTIVE_MAIN  1
#define ACTIVE_SLAVE  2


/******************************************************************************/
        
static int __init bcm963xx_mtd_init(void);
static struct mtd_info *bcm963xx_mtd_info;

static struct mtd_partition bcm963xx_parts[] = {
	{ name: "mainfs",       offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "slavefs",      offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "config",       offset: 0,	size: 0,	mask_flags: 0 },
	{ name: "equip",        offset: 0,	size: 0,	mask_flags: 0 },
	{ name: "upgflag",      offset: 0,	size: 0,	mask_flags: 0 },
	{ name: "blrom",        offset: 0,	size: 0,	mask_flags: 0 },
	{ name: "rootfstag",    offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "reserved",     offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "rootfsbaktag", offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "wlanrf",       offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "kernel",       offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "kernelbak",    offset: 0,  size: 0,    mask_flags: 0 },
	{ name: "all",          offset: 0,  size: 0,    mask_flags: 0 },
    { name: NULL,           offset: 0,  size: 0}
};

static int bcm963xx_parts_size = sizeof(bcm963xx_parts) / sizeof(bcm963xx_parts[0]);

static struct mtd_info *get_mtd_named(char *name)
{
	int i;
	struct mtd_info *mtd;

	for (i = 0; i < MAX_MTD_DEVICES; i++) {
		mtd = get_mtd_device(NULL, i);
		if (mtd) {
			if (strcmp(mtd->name, name) == 0)
				return(mtd);
			put_mtd_device(mtd);
		}
	}

	return(NULL);
}

#define MTD_PART_DEBUG_SELF 1

static int __init bcm963xx_mtd_init(void)
{
	struct mtd_info *mtd;
	char mtd_ativeflag = ACTIVE_MAIN;
    ATP_UPG_Tag_ST st_tag;
    char id[ATP_BOARD_ID_LEN];
    char upgflag[ATP_BOARD_ID_LEN];
	int i = 0;

	memset(id, 0, ATP_BOARD_ID_LEN);
    memcpy(id, g_blparms_buf, ATP_BOARD_ID_LEN);
    printk("board id:%s \n",id);

    memset(upgflag, 0, ATP_BOARD_ID_LEN);
    memcpy(upgflag, g_blparms_buf + ATP_BOARD_ID_LEN + ATP_BOARD_ID_LEN, ATP_BOARD_ID_LEN);
    printk("upgflag:%s \n",upgflag);

    memcpy(st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo,
            g_blparms_buf + ATP_BOARD_ID_LEN + ATP_BOARD_ID_LEN + ATP_BOARD_ID_LEN, 
            ATP_TAG_FLASH_MAX_INDEX * 8);

#ifdef MTD_PART_DEBUG_SELF
    for(i=0; i<ATP_TAG_FLASH_MAX_INDEX; i++)
    {
        printk("flash in config offset:%lx, len:%lx\n",
            st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[i].ulAddress,
            st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[i].ulLen);
    }
#endif

	if((0 == memcmp(upgflag, IMAGE_SLAVE_S, strlen(IMAGE_SLAVE_S)))
	   || (0 == memcmp(upgflag, IMAGE_MAIN_F, strlen(IMAGE_MAIN_F))))
	{
		mtd_ativeflag = ACTIVE_SLAVE;
		printk("<<<<<<%s,%d,get slave rootfs tag>>>>\n",__FILE__,__LINE__);
		
		bcm963xx_parts[MTD_ROOTFS].offset = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_FS_INDEX].ulAddress);
		bcm963xx_parts[MTD_ROOTFS].size = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_FS_INDEX].ulLen);

		bcm963xx_parts[MTD_ROOTFSBAK].offset = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_FS_INDEX].ulAddress);
		bcm963xx_parts[MTD_ROOTFSBAK].size = 
			    (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_FS_INDEX].ulLen);

		//activetag
		bcm963xx_parts[MTD_ROOTFSTAG].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_TAG_INDEX].ulAddress;
		bcm963xx_parts[MTD_ROOTFSTAG].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_TAG_INDEX].ulLen;

		//tag
		bcm963xx_parts[MTD_ROOTFSBAKTAG].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_TAG_INDEX].ulAddress;
		bcm963xx_parts[MTD_ROOTFSBAKTAG].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_TAG_INDEX].ulLen;
		
		//kernel
		bcm963xx_parts[MTD_KERNEL].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulAddress;
		bcm963xx_parts[MTD_KERNEL].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulLen;

		//kernelbak
		bcm963xx_parts[MTD_KERNELBAK].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulAddress;
		bcm963xx_parts[MTD_KERNELBAK].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulLen;
	}
	else
	{
		mtd_ativeflag = ACTIVE_MAIN;
		printk("<<<<<<%s,%d,get Main rootfs tag >>>>\n",__FILE__,__LINE__);
		bcm963xx_parts[MTD_ROOTFS].offset = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_FS_INDEX].ulAddress);
		bcm963xx_parts[MTD_ROOTFS].size = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_FS_INDEX].ulLen);

		bcm963xx_parts[MTD_ROOTFSBAK].offset = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_FS_INDEX].ulAddress);
		bcm963xx_parts[MTD_ROOTFSBAK].size = 
		        (ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_FS_INDEX].ulLen);
			
		//activetag
		bcm963xx_parts[MTD_ROOTFSTAG].offset = 
		        ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_TAG_INDEX].ulAddress;
		bcm963xx_parts[MTD_ROOTFSTAG].size = 
		        ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_TAG_INDEX].ulLen;

		//bcm963xx_parts[MTD_ROOTFSTAG].ecclayout = mtd->ecclayout;	

		//tag
		bcm963xx_parts[MTD_ROOTFSBAKTAG].offset = 
		        ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_TAG_INDEX].ulAddress;
		bcm963xx_parts[MTD_ROOTFSBAKTAG].size = 
		        ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_TAG_INDEX].ulLen;

		//kernel
		bcm963xx_parts[MTD_KERNEL].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulAddress;
		bcm963xx_parts[MTD_KERNEL].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulLen;

		//kernelbak
		bcm963xx_parts[MTD_KERNELBAK].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulAddress;
		bcm963xx_parts[MTD_KERNELBAK].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulLen;
	}


	//config
	bcm963xx_parts[MTD_CONFIG].offset =	ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_CURRENT_CONFIG_INDEX].ulAddress;
	bcm963xx_parts[MTD_CONFIG].size = ATP_KILO_SIZE * (st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_EQUIP_PARAM_INDEX].ulAddress
								 - st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_CURRENT_CONFIG_INDEX].ulAddress);

	//equip
	bcm963xx_parts[MTD_EQUIP].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_EQUIP_PARAM_INDEX].ulAddress;
	bcm963xx_parts[MTD_EQUIP].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_EQUIP_PARAM_INDEX].ulLen;


	//Flag
	bcm963xx_parts[MTD_UPGFLAG].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_FLAG_INDEX].ulAddress;
	bcm963xx_parts[MTD_UPGFLAG].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_FLAG_INDEX].ulLen;


	//Bootloader rom
	bcm963xx_parts[MTD_BLROM].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_BOOTLOADER_INDEX].ulAddress;
	bcm963xx_parts[MTD_BLROM].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_BOOTLOADER_INDEX].ulLen;


	//reseved
	bcm963xx_parts[MTD_RESERVED].offset = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_RESERVED_INDEX].ulAddress;
	bcm963xx_parts[MTD_RESERVED].size = ATP_KILO_SIZE * st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo[ATP_TAG_FLASH_RESERVED_INDEX].ulLen;

#ifdef MTD_PART_DEBUG_SELF
	printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_ROOTFS].name,
	        (uLong)(bcm963xx_parts[MTD_ROOTFS].size), (uLong)(bcm963xx_parts[MTD_ROOTFS].offset));
	printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_ROOTFSBAK].name,
	        (uLong)(bcm963xx_parts[MTD_ROOTFSBAK].size), (uLong)(bcm963xx_parts[MTD_ROOTFSBAK].offset));
	printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_ROOTFSTAG].name,
	        (uLong)(bcm963xx_parts[MTD_ROOTFSTAG].size), (uLong)(bcm963xx_parts[MTD_ROOTFSTAG].offset));
	printk("===Part name=%s, size=lx, ofs=%lx\n", bcm963xx_parts[MTD_ROOTFSBAKTAG].name,
	        (uLong)(bcm963xx_parts[MTD_ROOTFSBAKTAG].size), (uLong)(bcm963xx_parts[MTD_ROOTFSBAKTAG].offset));
	printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_KERNEL].name,
	        (uLong)(bcm963xx_parts[MTD_KERNEL].size), (uLong)(bcm963xx_parts[MTD_KERNEL].offset));
	printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_KERNELBAK].name,
	        (uLong)(bcm963xx_parts[MTD_KERNELBAK].size), (uLong)(bcm963xx_parts[MTD_KERNELBAK].offset));
    printk("===Part name=%s, size=%lx,ofs=%lx\n", bcm963xx_parts[MTD_CONFIG].name,
	        (uLong)(bcm963xx_parts[MTD_CONFIG].size), (uLong)(bcm963xx_parts[MTD_CONFIG].offset));
    printk("===Part name=%s, size=%lx,ofs=%lx\n", bcm963xx_parts[MTD_EQUIP].name,
	        (uLong)(bcm963xx_parts[MTD_EQUIP].size), (uLong)(bcm963xx_parts[MTD_EQUIP].offset));
    printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_UPGFLAG].name,
	        (uLong)(bcm963xx_parts[MTD_UPGFLAG].size), (uLong)(bcm963xx_parts[MTD_UPGFLAG].offset));
    printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_BLROM].name,
	        (uLong)(bcm963xx_parts[MTD_BLROM].size), (uLong)(bcm963xx_parts[MTD_BLROM].offset));
	printk("===Part name=%s, size=%lx, ofs=%lx\n", bcm963xx_parts[MTD_RESERVED].name,
	        (uLong)(bcm963xx_parts[MTD_RESERVED].size), (uLong)(bcm963xx_parts[MTD_RESERVED].offset));
 #endif   

	/* check if boot from SPI flash */
	bcm963xx_mtd_info = do_map_probe("spiflash_probe", NULL);
  	bcm963xx_mtd_info->owner = THIS_MODULE;

	printk("ATP_MTD_SYSTEM\n");
	add_mtd_partitions(bcm963xx_mtd_info, bcm963xx_parts, bcm963xx_parts_size);

	if(ACTIVE_MAIN == mtd_ativeflag)
	{
		printk("mount mainfs----\n");
	}
	else
	{
		printk("mount slavefs----\n");
	}
	
	mtd = get_mtd_named("mainfs");

	printk("roorfs mtd->index :%d\n",mtd->index);

	if (mtd) 
	{
		ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
		put_mtd_device(mtd);
	}
	else
	{
		printk("MKDEV NOK\n");
	}

	return 0;
}

static void __exit bcm963xx_mtd_cleanup(void)
{
	if (bcm963xx_mtd_info) {
    	del_mtd_partitions(bcm963xx_mtd_info);
       	map_destroy(bcm963xx_mtd_info);
	}
}

module_init(bcm963xx_mtd_init);
module_exit(bcm963xx_mtd_cleanup);

