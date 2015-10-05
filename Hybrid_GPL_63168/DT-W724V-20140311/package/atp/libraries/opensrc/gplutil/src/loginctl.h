/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : loginctl.h
  �� �� ��   : ����
  ��    ��   : qichenghong KF19818
  ��������   : 2009��12��2��
  ����޸�   :
  ��������   : loginctl.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��12��2��
    ��    ��   : qichenghong KF19818
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __LOGINCTL_H__
#define __LOGINCTL_H__
#include "atptypes.h"

#define LOGIN_OK	0
#define LOGIN_FAIL	1

#define LOGIN_VALID		1
#define LOGIN_INVALID		0

//#define LOGIN_DEBUG

#ifdef LOGIN_DEBUG
#define LoginDebugPrint(format, args...)       do{printf("FILE: %s FUNC: %s LINE: %d: ", __FILE__,__FUNCTION__, __LINE__);printf(format, ##args);}while(0)
#else
#define LoginDebugPrint(format, args...) 	
#endif

typedef enum ATP_LOGINRET_EN    //��¼���ؽ��
{
    ATP_LOGINRET_OK        	= 0,   //��ʾ���û��ɼ�����¼
    ATP_LOGINRET_LOGINED,             //��ʾ���û��Ѿ���¼��
    ATP_LOGINRET_TOOMANY_FAIL,           //��ʾ��ǰ�û���¼ʧ�ܴ����Ѿ��������ƴ���
    ATP_LOGINRET_EXISTED,            //��ʾͬһ���û�ͬһ���ն��Ѿ���¼��һ��
    
    ATP_LOGINRET_INNORMAL,         //��ʾ����������
    ATP_LOGINRET_END,
} ATP_LOGINRET_EN;

#define ATP_LOGINTYPE_REMOTE    0x8000

typedef enum ATP_LOGINTYPE_EN
{
    ATP_LOGINTYPE_NONE        	= 0,   //δ֪��¼��ʽ
    ATP_LOGINTYPE_FTP,             		//ͨ��FTP ��¼
    ATP_LOGINTYPE_SAMBA,          		//ͨ��SAMBA ��¼
    ATP_LOGINTYPE_FTPTIMEOUT,             		//ͨ��FTP ��¼��ʱ��

    ATP_LOGINTYPE_END = 0x7FFF
} ATP_LOGINTYPE_EN;
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL //y00181549 for ����ftp����ϸ��Ȩ�޿��� at 2012.6.10
typedef enum ATP_FTPTYPE_EN
{
    ATP_FTPTYPE_NONE        	= 0,   //δ֪��¼��ʽ
    ATP_FTPTYPE_FTPNORMAL,          //δ����FTP����
    ATP_FTPTYPE_FTPSEXPLICIT,       //��ʾ����FTP����
    ATP_FTPTYPE_FTPSIMPLICIT,      //��ʽ����FTP����

    ATP_FTPTYPE_END
} ATP_FTPTYPE_EN;

typedef enum ATP_FROMTYPE_EN
{
    ATP_FROMTYPE_WAN        	= 0,   //wan�������
    ATP_FROMTYPE_LAN,                    //lan�������
    ATP_FROMTYPE_END
} ATP_FROMTYPE_EN;
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
VOS_UINT32 ATP_LoginCtlInit(VOS_UINT32 ulLogType);
ATP_LOGINRET_EN ATP_LoginCtlCheck(const VOS_CHAR *pszUsername ,const VOS_CHAR *pszIpAddr ,VOS_UINT32 ulLogType , 
                                                                VOS_UINT ulLogStatus);
VOS_INT32 ATP_LoginCtlLogout(const VOS_CHAR *pszUsername , const VOS_CHAR *pszAddrInfo,VOS_UINT32 ulLogintype);
#if 0   //deleted by qichenghong 2010.01.15
VOS_INT32 ATP_LoginCtlCheckLanIP(const VOS_CHAR *pszClientAddr);
#endif
VOS_INT32 ATP_LoginCtlAllowWanAcc(const VOS_CHAR *pszUsername , VOS_UINT32 ulLogintype);
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL
VOS_INT32 ATP_LoginCtlCheckFtpAcc(const VOS_CHAR *pszUsername, VOS_UINT32 ulFtpType,VOS_UINT32 ulFromType);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */



#endif  //end #ifndef __LOGINCTL_H__


