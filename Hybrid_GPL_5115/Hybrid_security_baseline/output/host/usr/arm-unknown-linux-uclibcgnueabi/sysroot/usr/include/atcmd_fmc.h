/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : atcmd.h
  ��    ��  : x00104602
  ��    ��  : 1.0
  ��������  : 2008-5-3
  ��    ��  : ������Ʒ��AT������صײ�ӿ�ͷ�ļ�
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2008-5-3
     ��    ��   : x00104602
     �޸�����   : ��ɳ���

*********************************************************************************/

#ifndef __ATCMD_H__
#define __ATCMD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atcmdadapter_fmc.h"

typedef struct cmd 
{
    char *name;
    cmd_func_t *func;
    char *help;
}at_cmd_t;

/*Start of added for support STC hspa by lvxin00135113@20090812*/
#define  ATCMD_HSPA_VENDOR_PROC                           "/proc/UsbModem"
#define  ATCMD_HSPA_VENDOR_STR                             "Vendor"
#define  ATCMD_HSPA_PRODUCT_VENDOR_HW             "Huawei"
#define  ATCMD_HSPA_PRODUCT_VENDOR_BANDLUXE   "Bandluxe"
#define  ATCMD_HSPA_PRODUCT_VENDOR_ZTE             "ZTE"

typedef enum
{
    ATCMD_HSPA_VENDOR_HW = 0,
    ATCMD_HSPA_VENDOR_BANDLUXE,
    ATCMD_HSPA_VENDOR_ZTE,

    ATCMD_HSPA_VENDOR_MAX
}ATCMD_HSPA_VENDOR_EN;


/*End of added for support STC hspa by lvxin00135113@20090812*/
#if __cplusplus
#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __ATCMD_H__ */
