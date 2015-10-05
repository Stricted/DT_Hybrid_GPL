#ifndef __ATP_CFM_ADAPT_H__
#define __ATP_CFM_ADAPT_H__

#ifdef SUPPORT_DESKTOP
#define ATP_CFM_CUR_CFG_FILE    "../etc/curcfg.xml"
#define ATP_CFM_CUR_PART_CFG_FILE    "../etc/curpartcfg.xml"
#define ATP_CFM_FLASH_CFG_FILE    "/config/curcfg"
#else
#define ATP_CFM_CUR_CFG_FILE    "/var/curcfg.xml"
#define ATP_CFM_CUR_PART_CFG_FILE    "/var/curpartcfg.xml"
#define ATP_CFM_FLASH_CFG_FILE    "/config/curcfg"
#endif

typedef enum tagATP_RESTORE_MODE_EN
{
    ATP_RESTORE_LOCAL_EN,
    ATP_RESTORE_REMOTE_EN,
    ATP_RESTORE_NORMAL_EN,
} ATP_RESTORE_MODE_EN;

/*!
  \brief  恢复出厂，发消息至mic执行恢复出厂
  \param[in]  enRestoreMode: 
  \return 
  \retval 
*/
VOS_VOID ATP_CFM_ExtRestoreDft(ATP_RESTORE_MODE_EN enRestoreMode);

/*!
  \brief  恢复出厂，需要root权限
  \param[in]  enRestoreMode: 
  \return 
  \retval 
*/
VOS_VOID ATP_CFM_ExtRestoreDftEx(ATP_RESTORE_MODE_EN enRestoreMode);

VOS_VOID ATP_CFM_ExtReboot(VOS_VOID);

VOS_VOID ATP_CFM_EquipRestoreDftEx();

VOS_VOID ATP_CFM_ButtonRestoreDftEx();

VOS_UINT32 ATP_CFM_ExtImportEncryptedCfgFile(const VOS_CHAR *pcFileName);

VOS_UINT32 ATP_CFM_ExtExportEncryptedCfgFile(const VOS_CHAR *pcTargetFileName );

VOS_UINT32 ATP_CFM_ExtDecryptedCfgFile(const VOS_CHAR *pcFileName, const VOS_CHAR *pcDstFile);

#endif

