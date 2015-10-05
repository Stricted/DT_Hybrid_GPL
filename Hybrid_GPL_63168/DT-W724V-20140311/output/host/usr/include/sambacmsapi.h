#ifndef __SAMBACMSAPI_H__
#define __SAMBACMSAPI_H__

#define SMBD_USER_PID_FILE  		"/var/smbd_user_pid"
/*begin of add by y00181549 for DTS2012092408418��ʹ��guest�û�ʱ��Ҫ����smbd���������Լ����µ�smb.conf�ļ� at 20120925*/ 
#ifdef SUPPORT_ATP_GUEST_ACCESS
#define SMBD_PROCESS_PID_FILE         "/var/pid.smbd"
#endif
/*end of add by y00181549 for DTS2012092408418��ʹ��guest�û�ʱ��Ҫ����smbd���������Լ����µ�smb.conf�ļ� at 20120925*/ 

typedef struct StorageServiceNetServer
{
    VOS_BOOL     bSmbEnable;          //�Ƿ�����samba������
    VOS_BOOL     bNetProAuthReq;    //�Ƿ���Ҫ��¼��֤
}StorageServiceNetServer_ST;

//!...........................................�ⲿapi�ӿ�start.................................................
#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief ��ʼ��SAMBAģ��
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageSambaInit(VOS_VOID);

/*!
  \brief ȥ��ʼ��������ɾ��/var/sambaĿ¼�����smb.conf,smbpasswd,smbusers�ļ�
  \param[in]  VOS_VOID: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageSambaUnInit( VOS_VOID );

/*!
  \brief ����samba ������Domain name/Netbios name
  \param[in]  *pszDomain: 
  \return 
  \retval
*/
VOS_UINT32 ATP_UsbStorageSambaSetDomain(const VOS_CHAR *pszDomain);

/*!
  \brief  ��ȡ�������ݿ���Net Server�������Ϣ
  \param[in]  *pstStoSerNetServer: 
  \return 
  \retval
*/
VOS_INT32 ATP_StoSerNetServerGetFromDB(StorageServiceNetServer_ST *pstStoSerNetServer);

/*!
  \brief ɱ�� samba user ����
  \param[in]  UserName: �û���,pszUserPidPath Samba�û�pid�ļ�
  \param[in]  *pszUserPidPath: 
  \return 
  \retval
*/
VOS_VOID ATP_SAMBA_KillUserPid(const VOS_CHAR* UserName,const VOS_CHAR *pszUserPidPath);

/*!
  \brief ����SambaĿǰ��״̬
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_BOOL ATP_SambaGetState(VOS_VOID);

/*!
  \brief ����SambaĿǰ��״̬
  \param[in]  bstate: 
  \return 
  \retval
*/
VOS_UINT32 ATP_SambaSetState(VOS_BOOL bstate);

/*!
  \brief ���һ��Samba�û�,������Ϣ��ӵ�/etc/sambaĿ¼
                          �����smbpasswd,smbusers,smb.conf�ļ���
  \param[in]  *pszUsername: �û���
  \param[in]  *pszPasswd: �û�����   
  \param[in]  ulAllPath: 
  \param[in]  *pszPath: �û���Ŀ¼
  \param[in]  ulPermissions: �û�����Ȩ��
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageSambaAddUser(VOS_CHAR *pszUsername,VOS_CHAR *pszPasswd,VOS_UINT32 ulAllPath,VOS_CHAR *pszPath,VOS_UINT ulPermissions);

/*!
  \brief ɾ��/var/samba/Ŀ¼�����smb.conf,smbpasswd��smbusers�ļ�
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageSambaDeleteConfigFile(VOS_VOID);

/*!
  \brief ����samba�������ļ�/var/samba/smb.conf
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_INT32 ATP_UsbStorageSambaCreateConfigFile(VOS_VOID);




#if defined(__cplusplus)
}
#endif
//!...........................................�ⲿapi�ӿ�end.................................................

#endif
