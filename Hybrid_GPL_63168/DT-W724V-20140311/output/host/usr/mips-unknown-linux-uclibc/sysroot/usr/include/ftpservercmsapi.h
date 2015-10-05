/**\file ftpservercmsapi.h
  *ftpservercms����ӿ�ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author w00190448
  *\date  2012-2-24
*/
#ifndef __FTPSERVERCMSAPI_H__
#define __FTPSERVERCMSAPI_H__

#define BFTPD_MAX_BUFFER_LEN 1024
#define BFTPD_CMD_LINE_LEN 32

/*FTP Server��״̬*/
#define  FTPS_STATUS_ENABLED              "Enabled"
#define  FTPS_STATUS_DISABLED              "Disabled"

#define BFTPD_USER_FILE "/var/bftpd_user_pid"
#define  FTPS_CFGDIR        "/var/ftp"

/* �û��ķ���ģʽ */
#define  FTPS_USR_ACCESSMODE_READONLY         0
#define  FTPS_USR_ACCESSMODE_FULLCONTROL      1

#ifdef SUPPORT_ATP_FTP_SERVER_ANONYUSER
typedef struct StorageServiceFTPAnonyUser
{
    VOS_BOOL     bEnable;
    VOS_BOOL     bReadOnly;
    VOS_BOOL     bAllPath;
    VOS_CHAR     acStartingFolder[256];    //the unique reference of the folder instance
}StorageServiceFTPAnonyUser_ST;
#endif

typedef struct StorageServiceFTPServer
{
    VOS_BOOL     bEnable;
    VOS_UINT      ulMaxUserNum;
    VOS_UINT      ulIdleTime;          //FTP����������ʱ, Socket�ȴ����ӵ�ʱ��, 0��ʾ�����Ƶȴ�
    VOS_CHAR     acStatus[15];         //"Enabled" or "Disabled" or "Error"
#ifdef SUPPORT_ATP_FTP_SERVER_ANONYUSER
    struct StorageServiceFTPAnonyUser *pstAnonyUser; 
#endif
}StorageServiceFTPServer_ST;

typedef struct BFTPD_USERPID_INFO_ST{
    VOS_INT32   lType;
    VOS_INT32   lPid;
    VOS_CHAR    acUserName[256];
}BFTPD_USERPID_INFO;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief ��������FTP server�������
  \param[in]  bEnable: �Ƿ����������
  \param[in]  ulPort: FTP Server �˿ںţ�ĿǰĬ��Ϊ21
  \return 
  \retval
*/
 VOS_INT32 ATP_UsbStorageFTPReset(VOS_BOOL bEnable,VOS_UINT32 ulPort);

 /*!
  \brief ����FTPserverĿǰ��״̬
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_BOOL ATP_FtpserverGetState(VOS_VOID);

 /*!
  \brief ����FtpserverĿǰ��״̬
  \param[in]  bstate: 
  \return 
  \retval
*/
VOS_UINT32 ATP_FtpserverSetState(VOS_BOOL bstate);

 
/*!
  \brief ��ȡ�������ݿ���FTP Server�������Ϣ
  \param[in]  *pstStoSerFTPServer: 
  \return VOS_OK:   �ɹ� VOS_NOK : ʧ�� 
  \retval
*/
 VOS_INT32 ATP_StoSerFTPServerGetFromDB(StorageServiceFTPServer_ST *pstStoSerFTPServer);

/*!
  \brief ɾ��һ��FTP �û�,������Ϣ��/var/ftpĿ¼
                          �����ftpdpassword ,ftpusers ,bftpd.conf�ļ���ȥ��
  \param[in]  *pstUserInfo: �û���Ϣ
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPDelUser(const StorageServiceUserInfo_ST *pstUserInfo);

/*!
  \brief  ���һ��FTP �û�,������Ϣ��ӵ�/var/ftpĿ¼
                          �����ftpdpassword ,ftpusers ,bftpd.conf�ļ���
  \param[in]  *pszUsername: �û���
  \param[in]  *pszPasswd: �û����� 
  \param[in]  ulAllPath: 
  \param[in]  *pszPath: �û���Ŀ¼
  \param[in]  ulPermissions: �Ƿ����ͨ��WAN�����
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPAddUser(VOS_CHAR *pszUsername ,  const VOS_CHAR *pszPasswd,
                                   VOS_UINT32 ulAllPath,    const VOS_CHAR *pszPath,
                                   VOS_UINT ulPermissions);


/*!
  \brief ж��FTP server ģ��ĺ���
  \param[in]  VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageFTPUnInit( VOS_VOID );

/*!
  \brief ��ʼ��FTP serverģ��
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageFTPInit(VOS_VOID);

/*!
  \brief ɾ�����е�ftp �����ļ�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPDelAllCfgFile(VOS_VOID);

/*!
  \brief ΪFtp server ���������ļ�,������û�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPCreatLogin(VOS_VOID);

int lock_whole_file_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len);

/*!
  \brief  �������ò���CONTROL_TIMEOUT,USERLIMIT_GLOBAL
  \param[in]  ulMaxUserNum: ����û�����������USERLIMIT_GLOBAL��
  \param[in]  ulIdleTime: ��������CONTROL_TIMEOUT�Ĳ���
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPResetPara(VOS_UINT ulMaxUserNum , VOS_UINT ulIdleTime);

/*!
  \brief ����ԭʼ��ftp �����ļ�,������������д���ļ�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPCreateCfgFile(VOS_VOID);

/*!
  \brief ���û�����Ȩ��д�������ļ�/var/ftp/bftp.conf ��
  \param[in]  *pszUsername: 
  \param[in]  ulAccessmode: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPAppendUserAccessMode(VOS_CHAR *pszUsername , VOS_UINT ulAccessmode );

/*!
  \brief ���û������û�������ӵ������ļ�/var/ftp/ftpdpassword��
  \param[in]  *pszUsername: 
  \param[in]  *pszPasswd: 
  \param[in]  *pszPath: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPAppendUsersPwdFile(const VOS_CHAR *pszUsername , const VOS_CHAR *pszPasswd ,const VOS_CHAR *pszPath);

#ifdef SUPPORT_ATP_FTP_SERVER_ANONYUSER
/*!
  \brief  ��ȡ�������ݿ���FTP �����û�r�������Ϣ
  \param[in]  *pstStoSerFTPAnonyUser: 
  \return 
  \retval
*/
VOS_UINT32 ATP_StoSerFTPAnonyUserGetFromDB(StorageServiceFTPAnonyUser_ST *pstStoSerFTPAnonyUser);

/*!
  \brief �ж������û��Ƿ����ҹ���·����ȷ
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_BOOL  ATP_StoSerCheckAnonyUserEnable(StorageServiceFTPAnonyUser_ST *pstStoSerFTPAnonyUser);

/*!
  \brief ��������û�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPAddAnonyUser(VOS_VOID);


/*!
  \brief �������û���Ϣ���뻺��
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_StoSerFTPAnonyUserSaveDB(StorageServiceFTPAnonyUser_ST *pstStoSerFTPAnonyUser);
#endif

/*!
  \brief kill�û�pid
  \param[in]  *UserName: 
  \param[in]  *pszUserPidPath: 
  \return 
  \retval
*/
void ATP_FTP_KillUserPid(const VOS_CHAR *UserName,const VOS_CHAR *pszUserPidPath);

/*!
  \brief �����������ݿ���FTP Server�������Ϣ
  \param[in]  *pstStoSerFTPServer: 
  \return VOS_OK:   �ɹ� VOS_NOK : ʧ�� 
  \retval
*/
 VOS_INT32 ATP_StoSerFTPServerSaveDB(StorageServiceFTPServer_ST *pstStoSerFTPServer);

VOS_INT32 ATP_UsbStorageFTPSetEnable(VOS_BOOL bEnable,VOS_BOOL bInit);

#if defined(__cplusplus)
}
#endif

 #endif