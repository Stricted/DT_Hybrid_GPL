/**\file ftpservercmsapi.h
  *ftpservercms对外接口头文件
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2012-2-24
*/
#ifndef __FTPSERVERCMSAPI_H__
#define __FTPSERVERCMSAPI_H__

#define BFTPD_MAX_BUFFER_LEN 1024
#define BFTPD_CMD_LINE_LEN 32

/*FTP Server的状态*/
#define  FTPS_STATUS_ENABLED              "Enabled"
#define  FTPS_STATUS_DISABLED              "Disabled"

#define BFTPD_USER_FILE "/var/bftpd_user_pid"
#define  FTPS_CFGDIR        "/var/ftp"

/* 用户的访问模式 */
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
    VOS_UINT      ulIdleTime;          //FTP服务器闲置时, Socket等待连接的时间, 0表示无限制等待
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
  \brief 重新启动FTP server服务进程
  \param[in]  bEnable: 是否开启服务进程
  \param[in]  ulPort: FTP Server 端口号，目前默认为21
  \return 
  \retval
*/
 VOS_INT32 ATP_UsbStorageFTPReset(VOS_BOOL bEnable,VOS_UINT32 ulPort);

 /*!
  \brief 返回FTPserver目前的状态
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_BOOL ATP_FtpserverGetState(VOS_VOID);

 /*!
  \brief 设置Ftpserver目前的状态
  \param[in]  bstate: 
  \return 
  \retval
*/
VOS_UINT32 ATP_FtpserverSetState(VOS_BOOL bstate);

 
/*!
  \brief 获取配置数据库中FTP Server的相关信息
  \param[in]  *pstStoSerFTPServer: 
  \return VOS_OK:   成功 VOS_NOK : 失败 
  \retval
*/
 VOS_INT32 ATP_StoSerFTPServerGetFromDB(StorageServiceFTPServer_ST *pstStoSerFTPServer);

/*!
  \brief 删除一个FTP 用户,将其信息从/var/ftp目录
                          下面的ftpdpassword ,ftpusers ,bftpd.conf文件中去掉
  \param[in]  *pstUserInfo: 用户信息
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPDelUser(const StorageServiceUserInfo_ST *pstUserInfo);

/*!
  \brief  添加一个FTP 用户,将其信息添加到/var/ftp目录
                          下面的ftpdpassword ,ftpusers ,bftpd.conf文件中
  \param[in]  *pszUsername: 用户名
  \param[in]  *pszPasswd: 用户密码 
  \param[in]  ulAllPath: 
  \param[in]  *pszPath: 用户根目录
  \param[in]  ulPermissions: 是否可以通过WAN侧访问
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPAddUser(VOS_CHAR *pszUsername ,  const VOS_CHAR *pszPasswd,
                                   VOS_UINT32 ulAllPath,    const VOS_CHAR *pszPath,
                                   VOS_UINT ulPermissions);


/*!
  \brief 卸载FTP server 模块的函数
  \param[in]  VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageFTPUnInit( VOS_VOID );

/*!
  \brief 初始化FTP server模块
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageFTPInit(VOS_VOID);

/*!
  \brief 删除所有的ftp 配置文件
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPDelAllCfgFile(VOS_VOID);

/*!
  \brief 为Ftp server 创建配置文件,并添加用户
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPCreatLogin(VOS_VOID);

int lock_whole_file_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len);

/*!
  \brief  重新设置参数CONTROL_TIMEOUT,USERLIMIT_GLOBAL
  \param[in]  ulMaxUserNum: 最大用户数用来设置USERLIMIT_GLOBAL的
  \param[in]  ulIdleTime: 用来设置CONTROL_TIMEOUT的参数
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPResetPara(VOS_UINT ulMaxUserNum , VOS_UINT ulIdleTime);

/*!
  \brief 创建原始的ftp 配置文件,并将总体配置写入文件
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPCreateCfgFile(VOS_VOID);

/*!
  \brief 将用户访问权限写入配置文件/var/ftp/bftp.conf 中
  \param[in]  *pszUsername: 
  \param[in]  ulAccessmode: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPAppendUserAccessMode(VOS_CHAR *pszUsername , VOS_UINT ulAccessmode );

/*!
  \brief 将用户名和用户密码添加到配置文件/var/ftp/ftpdpassword中
  \param[in]  *pszUsername: 
  \param[in]  *pszPasswd: 
  \param[in]  *pszPath: 
  \return 
  \retval
*/
VOS_INT32 ATP_FTPAppendUsersPwdFile(const VOS_CHAR *pszUsername , const VOS_CHAR *pszPasswd ,const VOS_CHAR *pszPath);

#ifdef SUPPORT_ATP_FTP_SERVER_ANONYUSER
/*!
  \brief  获取配置数据库中FTP 匿名用户r的相关信息
  \param[in]  *pstStoSerFTPAnonyUser: 
  \return 
  \retval
*/
VOS_UINT32 ATP_StoSerFTPAnonyUserGetFromDB(StorageServiceFTPAnonyUser_ST *pstStoSerFTPAnonyUser);

/*!
  \brief 判断匿名用户是否开启且共享路径正确
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_BOOL  ATP_StoSerCheckAnonyUserEnable(StorageServiceFTPAnonyUser_ST *pstStoSerFTPAnonyUser);

/*!
  \brief 添加匿名用户
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageFTPAddAnonyUser(VOS_VOID);


/*!
  \brief 将匿名用户信息传入缓存
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_StoSerFTPAnonyUserSaveDB(StorageServiceFTPAnonyUser_ST *pstStoSerFTPAnonyUser);
#endif

/*!
  \brief kill用户pid
  \param[in]  *UserName: 
  \param[in]  *pszUserPidPath: 
  \return 
  \retval
*/
void ATP_FTP_KillUserPid(const VOS_CHAR *UserName,const VOS_CHAR *pszUserPidPath);

/*!
  \brief 设置配置数据库中FTP Server的相关信息
  \param[in]  *pstStoSerFTPServer: 
  \return VOS_OK:   成功 VOS_NOK : 失败 
  \retval
*/
 VOS_INT32 ATP_StoSerFTPServerSaveDB(StorageServiceFTPServer_ST *pstStoSerFTPServer);

VOS_INT32 ATP_UsbStorageFTPSetEnable(VOS_BOOL bEnable,VOS_BOOL bInit);

#if defined(__cplusplus)
}
#endif

 #endif