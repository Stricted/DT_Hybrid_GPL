/*************************************************************************************** 
 * 版权     :    Copyright (C) 2006-2006, Huawei Tech. Co., Ltd.              
 *                           ALL RIGHTS RESERVED 
 * 文件功能 : 
 * 作者     : 俞佑清y42304 @ HAL&OS
 *
 * 修改历史 :
 *	1. 创建文件时间: 2005年02月22日 
 *	
 *
 * 说明: 1. 适用范围: 1). 支持CFI的Intel Flash(8位/16位)
 *                    2). 支持CFI的AMD Flash(8位/16位)
 *                    3). 不支持CFI的AMD Flash(8位/16位)，需要用户给出具体的型号和块结构
 *                        参数(每个可擦除块的起始偏移，块大小和块数目).
 *          注: 这里的数据位宽不是指Flash本身的数据线宽度，而是指硬件设计时所支持的Flash
 *              数据位宽。例如：一块Flash本身数据线位宽16位的Flash，硬件设计可能会将Flash
 *              设计成8位模式(即把Flash芯片上＃BYTE引脚拉低,与CPU有关，有些CPU在启动时
 *              只支持Flash 8位模式)。
 *
 ****************************************************************************************/
     
#ifndef __HAL_FLASH_CONFIG_H__
#define __HAL_FLASH_CONFIG_H__

#include "hal_retvalue.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#define SUPPORT_02  1

/*********************************************************************************
 * 用途: 1. 如果Flash遵循Intel编程规格，则请定义宏INTEL_FLASH
 *          即#define INTEL_FLASH
 *           
 *          支持Intel 的生产商有：
 *            A.  Intel
 *            B.  Micron
 *            C.  ST（意法半导体）的M28系列的Flash
 *
 *       2. 如果Flash遵循AMD编程规格，则请定义宏AMD_FLASH
 *          即#define AMD_FLASH
 *            
 *          支持AMD的生产商。
 *            A. AMD和Fujitsu
 *            B. Spansion（AMD和Fujitsu两公司的闪存部门合并而成的Spansion公司）
 *            C. ATMEL
 *            D. SAMSUNG
 *            E. ST（意法半导体）(但该公司的M28系列的Flash支持Intel的Flash编程命令字).
 *            F. SST
 *            G. Toshiba
 * 
 * 注:   2个宏INTEL_FLASH 和 AMD_FLASH 只能且必须定义其中之一 
 *********************************************************************************/
//#define INTEL_FLASH
#define AMD_FLASH

//#include <board.h>

/*******************************************************
 *  配置第一片Flash在单板中的基地址
 ******************************************************/
#define FLASH_ADDR_BASE_FIRST     0xB8000000 
/* #define FLASH_ADDR_BASE_FIRST     (0xA0000000 | (MPI->cs[0].base & 0xFFFFFF00)) */
//0xBE000000 --16MB

/*******************************************************************************
 *  配置第二片Flash在单板中的基地址
 * A.假如有2片FLASH并且是并联起来为了扩充数据位宽的话，
 *   只需第一片FLASH芯片FLASH_ADDR_BASE_FIRST填写成系统中FLASH的起始地址，
 *   将第二片FLASH芯片FLASH_ADDR_BASE_SECOND定义为0xFFFFFFFF 
 * B.假如有2片FLASH并且是串联联起的话，需定义正确的FLASH芯片2的基地址。
 ******************************************************************************/
#define FLASH_ADDR_BASE_SECOND     0xFFFFFFFF  

/********************************************************************
 * CPU所支持的访问Flash的数据位宽
 * 1:  表示数据位宽为8位
 * 2:  表示数据位宽为16位
 * 4:  表示数据位宽为32位 (目前CBB不支持)
 ********************************************************************/
#define CFIDEV_BUSWIDTH             2

/*********************************************************************
 * 用途: 配置Flash芯片的块数，最多支持2片
 ********************************************************************/
#define CFIDEV_INTERLEAVE           1 
#define CFIDEV_DEVTYPE              2
 
#ifdef AMD_FLASH

/*
 * 假如你用的AMD FLASH 支持Unlock bypass 写FLASH，则
 * 定义宏CFIDEV_UNLOCK_BYPASS，否则取消该宏的定义
 */
#define CFIDEV_UNLOCK_BYPASS       1

#endif

#ifdef _BOOTLOAD_                                                
#define FLASH_UDELAY(x)
#define DEBUG_PRINT     printf
#else       // OS    
#define FLASH_UDELAY(x)  udelay(x>>1) 
#define DEBUG_PRINT      printk
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
