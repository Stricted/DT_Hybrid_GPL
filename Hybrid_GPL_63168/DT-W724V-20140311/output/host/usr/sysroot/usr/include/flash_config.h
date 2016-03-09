/*************************************************************************************** 
 * ��Ȩ     :    Copyright (C) 2006-2006, Huawei Tech. Co., Ltd.              
 *                           ALL RIGHTS RESERVED 
 * �ļ����� : 
 * ����     : ������y42304 @ HAL&OS
 *
 * �޸���ʷ :
 *	1. �����ļ�ʱ��: 2005��02��22�� 
 *	
 *
 * ˵��: 1. ���÷�Χ: 1). ֧��CFI��Intel Flash(8λ/16λ)
 *                    2). ֧��CFI��AMD Flash(8λ/16λ)
 *                    3). ��֧��CFI��AMD Flash(8λ/16λ)����Ҫ�û�����������ͺźͿ�ṹ
 *                        ����(ÿ���ɲ��������ʼƫ�ƣ����С�Ϳ���Ŀ).
 *          ע: ���������λ����ָFlash����������߿�ȣ�����ָӲ�����ʱ��֧�ֵ�Flash
 *              ����λ�����磺һ��Flash����������λ��16λ��Flash��Ӳ����ƿ��ܻὫFlash
 *              ��Ƴ�8λģʽ(����FlashоƬ�ϣ�BYTE��������,��CPU�йأ���ЩCPU������ʱ
 *              ֻ֧��Flash 8λģʽ)��
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
 * ��;: 1. ���Flash��ѭIntel��̹�����붨���INTEL_FLASH
 *          ��#define INTEL_FLASH
 *           
 *          ֧��Intel ���������У�
 *            A.  Intel
 *            B.  Micron
 *            C.  ST���ⷨ�뵼�壩��M28ϵ�е�Flash
 *
 *       2. ���Flash��ѭAMD��̹�����붨���AMD_FLASH
 *          ��#define AMD_FLASH
 *            
 *          ֧��AMD�������̡�
 *            A. AMD��Fujitsu
 *            B. Spansion��AMD��Fujitsu����˾�����沿�źϲ����ɵ�Spansion��˾��
 *            C. ATMEL
 *            D. SAMSUNG
 *            E. ST���ⷨ�뵼�壩(���ù�˾��M28ϵ�е�Flash֧��Intel��Flash���������).
 *            F. SST
 *            G. Toshiba
 * 
 * ע:   2����INTEL_FLASH �� AMD_FLASH ֻ���ұ��붨������֮һ 
 *********************************************************************************/
//#define INTEL_FLASH
#define AMD_FLASH

//#include <board.h>

/*******************************************************
 *  ���õ�һƬFlash�ڵ����еĻ���ַ
 ******************************************************/
#define FLASH_ADDR_BASE_FIRST     0xB8000000 
/* #define FLASH_ADDR_BASE_FIRST     (0xA0000000 | (MPI->cs[0].base & 0xFFFFFF00)) */
//0xBE000000 --16MB

/*******************************************************************************
 *  ���õڶ�ƬFlash�ڵ����еĻ���ַ
 * A.������2ƬFLASH�����ǲ�������Ϊ����������λ��Ļ���
 *   ֻ���һƬFLASHоƬFLASH_ADDR_BASE_FIRST��д��ϵͳ��FLASH����ʼ��ַ��
 *   ���ڶ�ƬFLASHоƬFLASH_ADDR_BASE_SECOND����Ϊ0xFFFFFFFF 
 * B.������2ƬFLASH�����Ǵ�������Ļ����趨����ȷ��FLASHоƬ2�Ļ���ַ��
 ******************************************************************************/
#define FLASH_ADDR_BASE_SECOND     0xFFFFFFFF  

/********************************************************************
 * CPU��֧�ֵķ���Flash������λ��
 * 1:  ��ʾ����λ��Ϊ8λ
 * 2:  ��ʾ����λ��Ϊ16λ
 * 4:  ��ʾ����λ��Ϊ32λ (ĿǰCBB��֧��)
 ********************************************************************/
#define CFIDEV_BUSWIDTH             2

/*********************************************************************
 * ��;: ����FlashоƬ�Ŀ��������֧��2Ƭ
 ********************************************************************/
#define CFIDEV_INTERLEAVE           1 
#define CFIDEV_DEVTYPE              2
 
#ifdef AMD_FLASH

/*
 * �������õ�AMD FLASH ֧��Unlock bypass дFLASH����
 * �����CFIDEV_UNLOCK_BYPASS������ȡ���ú�Ķ���
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
