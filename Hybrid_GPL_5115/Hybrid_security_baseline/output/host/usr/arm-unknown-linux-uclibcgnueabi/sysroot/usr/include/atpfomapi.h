/******************************************************************************
  ��Ȩ����  : 2012-2020����Ϊ�������޹�˾
  �� �� ��  : atpfomapi.h
  ��    ��  : l00135113
  ��    ��  : 1.0
  ��������  : 2012-1-11
  ��    ��  :
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2012-1-11
     ��    ��   : l00135113
     �޸�����   : ��ɳ���

*********************************************************************************/
#ifndef _ATPFOMAPI_H_
#define _ATPFOMAPI_H_

#include "atptypes.h"
#include "atpflashapi.h"
#include <mtd/mtd-user.h>

#if defined(__cplusplus)
extern "C" {
#endif


/*FOM:Flash Operation Master*/

/*�������ĳһflash�������͵Ľڵ�*/
typedef struct stFOM_AREA_INFO
{
    FOM_E_FLASH_AREA enFlashType;/*��Ӧflash��������*/
    VOS_UINT32 ulFlashAreaIndx;/*�������������������������������Һͼ���*/
    VOS_UINT32 ulFlashAreaSize;
    VOS_UINT32 ulFlashAreaOffset;
    struct stFOM_AREA_INFO*   pstRChildNode; /*ָ��ͬһ����flash��������һ�ڵ�*/    
} FOM_AREA_INFO_ST;


/*ָ�����ĳһflash�������������ͷ�ڵ�*/
typedef struct stFOM_AREA_INFO_HEAD
{
    FOM_E_FLASH_AREA enFlashType;/*��Ӧflash��������*/
    VOS_UINT32 ulAreaCount;/*�������ܸ���*/    
    VOS_UINT32 ulAreaAllSize;/*�������͵��ܳ��ȣ�*/
    
    struct stFOM_AREA_INFO_HEAD *pstLSiblingHead; /*ָ����һflash�������͵�����ͷ*/
    struct stFOM_AREA_INFO*   pstRChildNode; /*ָ��ͬһ����flash��������һ�ڵ�*/   
} FOM_AREA_INFO_HEAD_ST;

/******************Globe_Mac*********************/
typedef struct Globe_Mac
{
    VOS_INT16 sWanMacNum;
    VOS_INT16 sWlanMacNum;
    VOS_INT32 lmacInUse;
    char szBaseMacAddr[BHAL_OBLI_MAC_ADDRESS_LEN];  //8           /* ��MAC��ַ */
}FOM_GlobeMac_ST;


/****************************Function Prototype********************************/
/* Flash */
extern VOS_INT32 ATP_FomFlashInit();
VOS_UINT32 FOMGetAllFlashSize(VOS_VOID);

VOS_INT32 FOMResetShemInit(VOS_VOID);
VOS_UINT32 FomReadFlashAreaByHead(FOM_AREA_INFO_HEAD_ST *pstNodeHead,
VOS_INT8 *pcDstBuf, VOS_UINT32 ulOffset, VOS_UINT32 ulLen);
VOS_UINT32 FomWriteFlashAreaByHead(FOM_AREA_INFO_HEAD_ST *pstNodeHead,
VOS_INT8 *pcSrcBuf, VOS_UINT32 ulOffset, VOS_UINT32 ulLen);
void displayAllList();
FOM_AREA_INFO_HEAD_ST * FomSearchAreaHeadByType(FOM_E_FLASH_AREA enAreaType);
VOS_INT32 bhalIoctl(VOS_UINT32 ulBhalioctl, VOS_UINT32 ulType, VOS_INT8 *pcString,
                        VOS_UINT32 ulStrLen, VOS_UINT32 ulOffset, VOS_INT8 *pcBuf);
VOS_INT32 FOM_FIX(VOS_VOID);
VOS_INT32 FOM_FLASHFIX_ONCE(VOS_VOID);
VOS_VOID UPG_FlashPartWrite(VOS_CHAR *SrcStr,int length);

VOS_UINT32 FOMGetBlockSize(VOS_UINT32 pulOffset);
FOM_AREA_INFO_HEAD_ST * FOMGetListHead(VOS_VOID);
FOM_GlobeMac_ST * FOMGlobeMacHead(VOS_VOID);


void FOM_GetFileName(VOS_INT8 *pfilename,FOM_E_FLASH_AREA enFlashArea);

int mtd_write_fd_to_offset(int fd, mtd_info_t *mtd_info, unsigned char *pucBuf, ssize_t len, off_t offset);
int mtd_read_fd_from_offset(int fd,unsigned char *pucBuf, ssize_t len, off_t offset);
VOS_INT32 ATP_Norflash_WriteConfigAreaOffset(VOS_CHAR *pucBuf, VOS_UINT32 ulLen, VOS_UINT32 ulOffset);
VOS_UINT32 ATP_MTD_GetSectorSize(VOS_INT32 lFd,VOS_UINT32 ulOffset);

/********************MTD ��������*******************************/
#define MTD_ACTIVE_PARTITIONS 0
#define MTD_UPG_PARTITIONS 1
#define MTD_CONFIG_PARTITIONS 2
#define MTD_EQUIP_PARTITIONS 3
#define MTD_UPGFLAG_PARTITIONS 4
#define MTD_BLROM_PARTITIONS 5
#define MTD_ACTIVETAG_PARTITIONS 6
#define MTD_RESERVED_PARTITIONS 7
#define MTD_TAG_PARTITIONS 8
#define MTD_WLANRF_PARTITIONS 9
#define MTD_KERNEL_PARTITIONS 10
#define MTD_KERNELBAK_PARTITIONS 11
#define MTD_FOTA_PARTITIONS 12
#define MTD_COREDUMP_PARTITIONS 13


VOS_INT32 ATP_flash_EraseAll(VOS_INT32 lFd);
VOS_INT32 ATP_MTD_Getfd(VOS_INT32 lmtdpartitions,VOS_UINT32* ulMtdSize);
VOS_UINT32 ATP_MTD_Erase(VOS_INT32 lFd,VOS_UINT32 ulLen,VOS_UINT32 ulOffset);
VOS_UINT32 ATP_MTD_WriteBufToOffset(VOS_INT32 lFd,VOS_UINT8 *pucBuf, VOS_UINT32 ulLen, VOS_UINT32 ulOffset);
VOS_UINT32 ATP_MTD_WriteBufToOffset_Ex(VOS_INT32 lFd,VOS_UINT8 *pucBuf, VOS_UINT32 ulLen, VOS_UINT32 ulOffset,VOS_UINT8 ucPrintPoint);
VOS_UINT32 ATP_MTD_ReadBuffromOffset(VOS_INT32 lFd,VOS_UINT8 *pucBuf, VOS_UINT32 ulLen, VOS_UINT32 ulOffset);
#ifdef SUPPORT_ATP_NORFLASH
VOS_INT32 get_config_area_offset();
VOS_INT32 norflash_erase(VOS_INT32 lFd, erase_info_t steraseinfo);
#endif


#if defined(__cplusplus)
}
#endif

#endif /* _ATPFOMAPI_H_ */
