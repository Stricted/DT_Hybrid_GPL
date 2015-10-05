/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpnandparts.c
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2012-6-5
*/
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include "atpnandparts.h"
#include "kerneltag.h"

static struct mtd_partition atp_nand_parts[] = 
{
    {name: "rootfs",        offset: 0, size: 0},//mtd0
    {name: "all",     offset: 0, size: 0},//mtd1
    {name: "config",        offset: 0, size: 0},//mtd2
    {name: "equip",        offset: 0, size: 0},    //mtd3
    {name: "upgflag",        offset: 0, size: 0},//mtd4
    {name: "blrom",        offset: 0, size: 0},//mtd5
    {name: "rootfstag",        offset: 0, size: 0},//mtd6
    {name: "reserved",        offset: 0, size: 0},//mtd7
    {name: "html",          offset: 0, size: 0},//mtd8
    {name: "wlanrf",        offset: 0, size: 0}, //mtd9
    {name: "kernel",        offset: 0, size: 0}, //mtd10
    {name: "kernelbak",     offset: 0, size: 0}, //mtd11
    {name: "configbak",     offset: 0, size: 0}, //mtd12
    {name: "module",        offset: 0, size: 0}, //mtd13
    {name: NULL,            offset: 0, size: 0}
};

#define MTD_ROOTFS      0
#define MTD_ALL         1
#define MTD_CONFIG      2
#define MTD_EQUIP       3
#define MTD_UPGFLAG     4
#define MTD_BLROM       5
#define MTD_ROOTFSTAG   6
#define MTD_RESERVED		7
#define MTD_HTML        8
#define MTD_WLANRF      9
#define MTD_KERNEL      10
#define MTD_KERNELBAK   11
#define MTD_CONFIGBAK   12
#define MTD_MODULE      13

int atp_setup_mtd_partitions(
                                    struct mtd_info *mtd,
                                    const unsigned char *pucBootParam, 
                                    unsigned int ulMainOrSlave, /* 0 main 1 slave*/
                                    struct mtd_partition **ppstParts,
                                    unsigned int *pulMtdNum)
{
    ATP_UPG_Tag_ST st_tag;
    ATP_UPG_TAG_FLASH_PAIR_ST *astFlashPairs = NULL;
    int i;

    if ((NULL == mtd) || (NULL == ppstParts) || (NULL == pulMtdNum) || (NULL == pucBootParam))
    {
        return 0;
    }

    astFlashPairs = st_tag.flashLayout.stFlashLayoutInfo.astFlashInfo;
    for(i = 0; i < ATP_TAG_FLASH_MAX_INDEX; i++)
    {
        astFlashPairs[i].ulAddress = 0;
        astFlashPairs[i].ulLen = 0;
    }

    memcpy(astFlashPairs, pucBootParam, ATP_TAG_FLASH_MAX_INDEX * sizeof(ATP_UPG_TAG_FLASH_PAIR_ST));

    for(i = 0;i<ATP_TAG_FLASH_MAX_INDEX;i++)
    {
        printk("addr:%d,len:%d \n" ,astFlashPairs[i].ulAddress, astFlashPairs[i].ulLen);
    }

    *pulMtdNum = sizeof(atp_nand_parts) / sizeof(struct mtd_partition) - 1;
    *ppstParts = atp_nand_parts;

    /* 这里的len是为了不挂载Tag，它不是jffs2格式的 */
    if (ulMainOrSlave)
    {
        //active fs
        atp_nand_parts[MTD_ROOTFS].offset = (ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_FS_INDEX].ulAddress);
        atp_nand_parts[MTD_ROOTFS].size = (ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_FS_INDEX].ulLen);
        atp_nand_parts[MTD_ROOTFS].ecclayout = mtd->ecclayout;
        
        //activetag
        atp_nand_parts[MTD_ROOTFSTAG].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_TAG_INDEX].ulAddress;
        atp_nand_parts[MTD_ROOTFSTAG].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_TAG_INDEX].ulLen;
        atp_nand_parts[MTD_ROOTFSTAG].ecclayout = mtd->ecclayout;   

        //html
#if defined(CONFIG_HTML_PARTITION)
        atp_nand_parts[MTD_HTML].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MIAN_WEB_INDEX].ulAddress;
        atp_nand_parts[MTD_HTML].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MIAN_WEB_INDEX].ulLen;
        atp_nand_parts[MTD_HTML].ecclayout = mtd->ecclayout; 
#endif
        //kernel
        atp_nand_parts[MTD_KERNEL].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulAddress;
        atp_nand_parts[MTD_KERNEL].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulLen;
        atp_nand_parts[MTD_KERNEL].ecclayout = mtd->ecclayout; 

        //kernelbak
        atp_nand_parts[MTD_KERNELBAK].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulAddress;
        atp_nand_parts[MTD_KERNELBAK].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulLen;
        atp_nand_parts[MTD_KERNELBAK].ecclayout = mtd->ecclayout;
    }
    else
    {
        //active fs
        atp_nand_parts[MTD_ROOTFS].offset = (ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_FS_INDEX].ulAddress);
        atp_nand_parts[MTD_ROOTFS].size = (ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_FS_INDEX].ulLen);
        atp_nand_parts[MTD_ROOTFS].ecclayout = mtd->ecclayout;

        //activetag
        atp_nand_parts[MTD_ROOTFSTAG].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_TAG_INDEX].ulAddress;
        atp_nand_parts[MTD_ROOTFSTAG].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_TAG_INDEX].ulLen;
        atp_nand_parts[MTD_ROOTFSTAG].ecclayout = mtd->ecclayout;   

        // html
#if defined(CONFIG_HTML_PARTITION)
        atp_nand_parts[MTD_HTML].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_WEB_INDEX].ulAddress;
        atp_nand_parts[MTD_HTML].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_WEB_INDEX].ulLen;
        atp_nand_parts[MTD_HTML].ecclayout = mtd->ecclayout;
#endif
        //kernel
        atp_nand_parts[MTD_KERNEL].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulAddress;
        atp_nand_parts[MTD_KERNEL].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MAIN_KERNEL_INDEX].ulLen;
        atp_nand_parts[MTD_KERNEL].ecclayout = mtd->ecclayout; 

        //kernelbak
        atp_nand_parts[MTD_KERNELBAK].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulAddress;
        atp_nand_parts[MTD_KERNELBAK].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_SLAVE_KERNEL_INDEX].ulLen;
        atp_nand_parts[MTD_KERNELBAK].ecclayout = mtd->ecclayout;
    }

    //all
    atp_nand_parts[MTD_ALL].offset = 0;
    atp_nand_parts[MTD_ALL].size = 0;
    atp_nand_parts[MTD_ALL].ecclayout = mtd->ecclayout;

    //config
    atp_nand_parts[MTD_CONFIG].offset =  ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_CURRENT_CONFIG_INDEX].ulAddress;
    atp_nand_parts[MTD_CONFIG].size = ATP_KILO_SIZE * (astFlashPairs[ATP_TAG_FLASH_CONFIG_BACK_INDEX].ulAddress
                                 - astFlashPairs[ATP_TAG_FLASH_CURRENT_CONFIG_INDEX].ulAddress);
    atp_nand_parts[MTD_CONFIG].ecclayout = mtd->ecclayout;   

    //configback
    atp_nand_parts[MTD_CONFIGBAK].offset =  ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_CONFIG_BACK_INDEX].ulAddress;
    atp_nand_parts[MTD_CONFIGBAK].size = ATP_KILO_SIZE * (astFlashPairs[ATP_TAG_FLASH_EQUIP_PARAM_INDEX].ulAddress
                                 - astFlashPairs[ATP_TAG_FLASH_CONFIG_BACK_INDEX].ulAddress);
    atp_nand_parts[MTD_CONFIGBAK].ecclayout = mtd->ecclayout;   

    //module
    atp_nand_parts[MTD_MODULE].offset =  ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MUDULE_INDEX].ulAddress;
    atp_nand_parts[MTD_MODULE].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_MUDULE_INDEX].ulLen;
    atp_nand_parts[MTD_MODULE].ecclayout = mtd->ecclayout;   

    //equip
    atp_nand_parts[MTD_EQUIP].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_EQUIP_PARAM_INDEX].ulAddress;
    atp_nand_parts[MTD_EQUIP].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_EQUIP_PARAM_INDEX].ulLen;
    atp_nand_parts[MTD_EQUIP].ecclayout = mtd->ecclayout;   

    //Flag
    atp_nand_parts[MTD_UPGFLAG].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_FLAG_INDEX].ulAddress;
    atp_nand_parts[MTD_UPGFLAG].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_FLAG_INDEX].ulLen;
    atp_nand_parts[MTD_UPGFLAG].ecclayout = mtd->ecclayout;   

    //Bootloader rom
    atp_nand_parts[MTD_BLROM].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_BOOTLOADER_INDEX].ulAddress;
    atp_nand_parts[MTD_BLROM].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_BOOTLOADER_INDEX].ulLen;
    atp_nand_parts[MTD_BLROM].ecclayout = mtd->ecclayout;   

    //reseved
    atp_nand_parts[MTD_RESERVED].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_RESERVED_INDEX].ulAddress;
    atp_nand_parts[MTD_RESERVED].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_RESERVED_INDEX].ulLen;
    atp_nand_parts[MTD_RESERVED].ecclayout = mtd->ecclayout;   

    //wlanrf
    atp_nand_parts[MTD_WLANRF].offset = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_WLANRF_INDEX].ulAddress;
    atp_nand_parts[MTD_WLANRF].size = ATP_KILO_SIZE * astFlashPairs[ATP_TAG_FLASH_WLANRF_INDEX].ulLen;
    atp_nand_parts[MTD_WLANRF].ecclayout = mtd->ecclayout;

    return 1;
}


