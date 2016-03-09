#ifndef __SAMBACMSAPI_H__
#define __SAMBACMSAPI_H__

#define SMBD_USER_PID_FILE  		"/var/smbd_user_pid"
/*begin of add by y00181549 for DTS2012092408418在使能guest用户时需要将主smbd进程重启以加载新的smb.conf文件 at 20120925*/ 
#ifdef SUPPORT_ATP_GUEST_ACCESS
#define SMBD_PROCESS_PID_FILE         "/var/pid.smbd"
#endif
/*end of add by y00181549 for DTS2012092408418在使能guest用户时需要将主smbd进程重启以加载新的smb.conf文件 at 20120925*/ 

typedef struct StorageServiceNetServer
{
    VOS_BOOL     bSmbEnable;          //是否启用samba服务器
    VOS_BOOL     bNetProAuthReq;    //是否需要登录认证
}StorageServiceNetServer_ST;

//!...........................................外部api接口start.................................................
#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief 初始化SAMBA模块
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageSambaInit(VOS_VOID);

/*!
  \brief 去初始化函数，删除/var/samba目录下面的smb.conf,smbpasswd,smbusers文件
  \param[in]  VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageSambaUnInit( VOS_VOID );

/*!
  \brief 设置samba 的域名Domain name/Netbios name
  \param[in]  *pszDomain: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageSambaSetDomain(const VOS_CHAR *pszDomain);

/*!
  \brief  获取配置数据库中Net Server的相关信息
  \param[in]  *pstStoSerNetServer: 
  \return 
  \retval
*/
VOS_INT32 ATP_StoSerNetServerGetFromDB(StorageServiceNetServer_ST *pstStoSerNetServer);

/*!
  \brief 杀死 samba user 进程
  \param[in]  UserName: 用户名,pszUserPidPath Samba用户pid文件
  \param[in]  *pszUserPidPath: 
  \return 
  \retval
*/
VOS_VOID ATP_SAMBA_KillUserPid(const VOS_CHAR* UserName,const VOS_CHAR *pszUserPidPath);

/*!
  \brief 返回Samba目前的状态
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_BOOL ATP_SambaGetState(VOS_VOID);

/*!
  \brief 设置Samba目前的状态
  \param[in]  bstate: 
  \return 
  \retval
*/
VOS_UINT32 ATP_SambaSetState(VOS_BOOL bstate);

/*!
  \brief 添加一个Samba用户,将其信息添加到/etc/samba目录
                          下面的smbpasswd,smbusers,smb.conf文件中
  \param[in]  *pszUsername: 用户名
  \param[in]  *pszPasswd: 用户密码   
  \param[in]  ulAllPath: 
  \param[in]  *pszPath: 用户根目录
  \param[in]  ulPermissions: 用户访问权限
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageSambaAddUser(VOS_CHAR *pszUsername,VOS_CHAR *pszPasswd,VOS_UINT32 ulAllPath,VOS_CHAR *pszPath,VOS_UINT ulPermissions);

/*!
  \brief 删除/var/samba/目录下面的smb.conf,smbpasswd和smbusers文件
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageSambaDeleteConfigFile(VOS_VOID);

/*!
  \brief 创建samba的配置文件/var/samba/smb.conf
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageSambaCreateConfigFile(VOS_VOID);




#if defined(__cplusplus)
}
#endif
//!...........................................外部api接口end.................................................

#endif
