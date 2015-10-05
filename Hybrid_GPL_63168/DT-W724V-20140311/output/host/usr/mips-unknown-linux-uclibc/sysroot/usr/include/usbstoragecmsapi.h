#ifndef __USBSTORAGECMSAPI_H__
#define __USBSTORAGECMSAPI_H__

#include "atpconfig.h"
#define STOSER_USER_SMALL_LEN 64
#define STOSER_USER_LARGE_LEN 256

#define STOSER_CMD_LEN                                256
#define STOSER_GIANT_LEN                              1024

#define STOSER_LEN_10          10
#define STOSER_LEN_32          32
#define STOSER_LEN_64          64
#define STOSER_LEN_128          128
#define STOSER_LEN_256          256
#define STOSER_LEN_400          400

//!������start
#define STORAGESERVICE_MAXUSERACCNUM       1000
//!������end

#define SYSTEM_PASSWD_FILE  	"/etc/passwd"
#define SYSTEM_GROUP_FILE       	"/etc/group"
#define GUESTUSER                         "GUEST" //for UDK0000276404
#define ADMINUSER                         "ADMIN"
#define PRINTER_CONNECT_FLIE               "/var/proc_user_printer"
/* BEGIN: mod by l00203771 2013/9/9 for UDK0000286484 */
#define USBSTORAGE_ONLINE         "Online"
#define USBSTORAGE_OFFLINE        "Offline"
/* END: mod by l00203771 2013/9/9 for UDK0000286484 */

#define USB_REMB_LOGICVOL_NUM         128   /*�������ؿ����Զ���¼֮ǰ���ӹ���128��logicvol*/

typedef enum StorageServiceSerType
{
    StorageServiceSerType_FTP = 1,
    StorageServiceSerType_SAMBA
}StorageServiceSerType_EN;

typedef enum StorageServiceUserErrInfo
{
    StorageServiceSerType_UserName_Error = 5,
    StorageServiceSerType_Password_Error = 10
}StorageServiceUserErrInfo_EN;

typedef struct StorageServiceUserInfo
{
    VOS_UINT32    ulInstId;     /*Ϊ-1��ʱ���ʾʵ������*/
    VOS_CHAR      acUserName[STOSER_USER_LARGE_LEN + 1];
    VOS_CHAR      acPasswd[STOSER_USER_SMALL_LEN + 1];
    VOS_BOOL      bEnable;
    VOS_BOOL      bAddInCount;    ////add by sKF19819 20100510 for add usercount when no an usb has been mounted
    VOS_BOOL      bAllPath;
#ifdef  SUPPORT_ATP_USER_FOLDER
    VOS_CHAR      acUserFolder[STOSER_LEN_400];
#endif
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL
    VOS_UINT     AllowFTPAccess;
    VOS_UINT     AllowFTPSAccess;
    VOS_UINT     AllowFTPInternetAccess;
    VOS_UINT     AllowFTPSInternetAccess;
#endif  
}StorageServiceUserInfo_ST;

#if 1//def SUPPORT_ATP_USER_FOLDER
typedef struct StorageServiceUserAddSetTemp
{
    VOS_UINT32    ulInstId;    
    VOS_CHAR      acUserName[STOSER_USER_LARGE_LEN + 1];
    VOS_CHAR      acPasswd[STOSER_USER_SMALL_LEN + 1];
    VOS_BOOL      bEnable;
    VOS_CHAR      acPath[STOSER_LEN_400];
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL
    VOS_UINT     AllowFTPInternetAccess;
    VOS_UINT     AllowFTPSInternetAccess;
#endif  
}StorageServiceUserAddSetTemp_ST;
#endif

typedef struct StorageServiceFolderAccess
{
    VOS_UINT32    ulInstId;     /*Ϊ-1��ʱ���ʾʵ������*/
    VOS_CHAR      acUserReference[STOSER_USER_LARGE_LEN + 1];//the unique reference of the user access this folder,format as: Username.Foldername , Ex:User1./mnt/usb1_1
    VOS_UINT       ulPermissions;    //the read/write permissions of this folder , 100:read field , 010:write field , 001:execute field , default 111
    struct StorageServiceFolderAccess * pstNext;
}StorageServiceFolderAccess_ST;

typedef struct StoSerLogicVolFolderInfo
{
    VOS_CHAR    acName[STOSER_USER_LARGE_LEN];
    VOS_BOOL    bEnable;
    VOS_UINT32  ulUsrAccountAccess; //�û���֤��ʽ
    VOS_UINT32  ulUsrAccNum;


    VOS_UINT32  ulInstID;
    
    struct StoSerLogicVolFolderInfo  * pstNext;
    struct StorageServiceFolderAccess * pstFolderAccList;
}StoSerLogicVolFolderInfo_ST;

typedef struct StoSerAllUserFolder
{
    VOS_CHAR    path[STOSER_LEN_400];
    VOS_CHAR    name[STOSER_LEN_256];
    VOS_CHAR    writelist[STOSER_LEN_256];
    VOS_CHAR    readlist[STOSER_LEN_256];
    VOS_UINT     ulPermissions; //the read/write permissions of this folder , 100:read field , 010:write field , 001:execute field , default 111
    VOS_UINT     AllowFTPAccess;
    VOS_UINT     AllowFTPSAccess;
    VOS_UINT     AllowFTPInternetAccess;
    VOS_UINT     AllowFTPSInternetAccess;
    
    struct StoSerAllUserFolder       *  pstNext;
}StoSerAllUserFolder_ST;

typedef struct StoSerUserPath
{
    VOS_CHAR    path[STOSER_LEN_400];
    VOS_CHAR    writelist[STOSER_LEN_256];
    VOS_CHAR    readlist[STOSER_LEN_256];
    //VOS_UINT     ulPermissions; //the read/write permissions of this folder , 100:read field , 010:write field , 001:execute field , default 111    
    struct StoSerUserPath   * pstPathNext;
}StoSerUserPath_ST;

typedef struct StoSerUser
{
    VOS_CHAR    name[STOSER_LEN_256];
    struct StoSerUserPath * pstUserPath;
    struct StoSerUser  * pstUserNext;
}StoSerUser_ST;
//!...........................................�ⲿapi�ӿ�start.................................................
#if defined(__cplusplus)
extern "C" {
#endif


/*!
  \brief ��ȡȥ���ַ���ǰ��Ŀո�
  \param[in]   * pTmp
  \return       ��
  \retval 
*/
VOS_VOID filtrateBlankCharaterInFilename(VOS_CHAR * pTmp);

/*!
  \brief ��ʼ��storage serviceģ�飬��ע����ϵͳģ���ʼ�������С�
  \param[in] VOS_VOID: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH ��ʼ�������ڴ�ʧ��
*/
VOS_UINT32 ATP_StorageService_Init(VOS_VOID);

/*!
  \brief ȥ��ʼ��storage serviceģ�飬��ע����ϵͳģ���ʼ�������С�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_StorageService_UnInit(VOS_BOOL bByMount);

VOS_INT32 ATP_StorageServiceGetUserInfoFromDB(VOS_VOID);

/*!
  \brief ����StartingFolder���������û��Ĺ���·��
  \param[in]  *pcStartingFolder: 
  \return 
  \retval
*/
StoSerLogicVolFolderInfo_ST *ATP_StoSerFindAnonyStartFolder(const VOS_CHAR *pcStartingFolder);


/*!
  \brief ͨ���������ֲ�ѯ����ID��
  \param[in]  *pszPName: 
  \return 
  \retval
*/
VOS_INT32 ATP_StorageServiceGetPidByName(const VOS_CHAR *pszPName);

/*!
  \brief ����/�رս��̲�ִ����ӦACL����---��Ϊ��Ҫ�漰��ftp��samba����
  \param[in]  enSerType: ������
  \param[in]  bActionServerEnable: ʹ�ܱ�־
  \return 
  \retval 
*/
VOS_VOID ATP_StoSetSerActionAndAcl(StorageServiceSerType_EN enSerType, VOS_BOOL bActionServerEnable);

#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL //UDK0000130346 �ڹر�ftp wan����ʺ�ر� wan��˿�
VOS_UINT32 ATP_StoSerGetWanAccessStatus(VOS_BOOL *bFtpWanAccEnable,VOS_CHAR * pacUserName);
VOS_VOID ATP_StoSetSerFtpWanActionAndAcl(VOS_BOOL bFtpWanAccEnable);
#ifdef SUPPORT_ATP_IPV6
VOS_VOID ATP_StoSetSerFtpIpv6WanActionAndAcl(VOS_BOOL bFtpWanAccEnable);
#endif
#endif

#ifdef SUPPORT_ATP_SAMBA
VOS_INT32 ATP_UsbStorageSambaDelUser(const StorageServiceUserInfo_ST *pstUserInfo);
VOS_INT32 ATP_UsbStorageSambaAddAllUser(VOS_VOID);
/*Start of ATP 2012-9-1 for none by z00192937: ���samb�����û�����������*/
VOS_BOOL ATP_StorageServiceValidateSambaName(VOS_CHAR* pValue);
/*End of ATP 2012-9-1 for none by z00192937*/
#endif

/*!
  \brief �жϵ�ǰ�Ƿ��������û�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32  ATP_StoSerCheckUserExist(VOS_VOID);

/*!
  \brief ��ȡ�����û��ĵ�¼��ַ·����Ȩ��
  \param[in] **pstAllUserFolder: 
  \return 
  \retval
*/
VOS_UINT ATP_StoSerGetAllUserDirAndPermiss(StoSerAllUserFolder_ST **pstAllUserFolder);
#ifdef SUPPORT_ATP_ADDITION_FOLDER
/*!
  \brief �Ի�ȡ���������û��ĵ�¼��ַ·����Ȩ�޽�������
  \param[in] **pstAllUserFolder: 
  \return 
  \retval
*/
VOS_INT32 ATP_StoSerGetAllUserDirAndPermissSort(StoSerUser_ST **pstAllUserSort, StoSerAllUserFolder_ST *pstAllUserFolder);
#endif

/*!
  \brief �鿴ĳ���û��Ƿ�ʹ��
  \param[in]  *pszUsername: �û���
  \return 
  \retval
*/
VOS_INT32 ATP_StoStorageUserIsEnabled(VOS_CHAR *pazUserName);

#ifdef SUPPORT_ATP_GUEST_ACCESS
/*!
  \brief �鿴guest�û��Ƿ���Ա�ʹ��
  \return 
  \retval
*/
VOS_INT32 ATP_StoStorageGuestCouldBeEnabled(VOS_VOID);

/*!
  \brief ��g_astStoSerFolderAcc�л�ȡGuest�û�����Ϣ
  \param[in]  **pstAllUserFolderSort
  \return 
  \retval
*/
VOS_INT32 ATP_StoSerGetGuestInfo(StoSerAllUserFolder_ST **pstGuestInfo, StoSerAllUserFolder_ST *pstAllUserFolder);
#endif

#ifdef SUPPORT_ATP_USER_FOLDER
/*!
  \brief��ȡ�û���user folder
  \param[in]  ppacPath: 
  \param[in]  pacUserName: 
  \return VOS_OK�ɹ�VOS_NOK ʧ��
  \retval
*/
VOS_INT32 ATP_StorageServiceGetUserFolder(VOS_CHAR *pacPath,VOS_CHAR *pacUserName);
VOS_UINT32 ATP_StorageServiceUserFolderCheck(VOS_CHAR *pacUserName, VOS_CHAR *pacPath);
#endif

#ifdef SUPPORT_ATP_STORAGESERVICE_ADMIN_ACCESS
VOS_UINT32 ATP_UsbstoragecmsAdminSetProcess(VOS_BOOL bSetPasswd);
#endif
#ifdef SUPPORT_ATP_DT_FIREWALL
VOS_VOID HybridFtpcmsGetEffectIntf(VOS_CHAR *pcIntfName, VOS_UINT usLen);
#endif
/*!
  \brief �����߼���������ȡ�߼������������������ÿռ�,������MBΪ��λ
  \param[in]  pszLogicVolName: 
  \param[in]  *pulCap: 
  \param[in]  *pulUsedSpace: 
  \return 
  \retval
*/
VOS_UINT32 ATP_StoSerGetCapacity(const VOS_CHAR * pszLogicVolName , VOS_UINT32 *pulCap , VOS_UINT32 *pulUsedSpace);

#if defined(__cplusplus)
}
#endif
//!...........................................�ⲿapi�ӿ�end.................................................

#endif
