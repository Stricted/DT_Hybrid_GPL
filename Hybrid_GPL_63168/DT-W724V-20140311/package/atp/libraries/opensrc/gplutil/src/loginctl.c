/******************************************************************************

                  ��Ȩ���� (C), 2009-2011, ��Ϊ�������޹�˾

  �� �� ��   : loginctl.c
  �� �� ��   : V1.0
  ��    ��   : qichenghong KF19818
  ��������   : 2009��12��2��
  ����޸�   :
  ��������   : �����ĵ�¼����ģ��
  �����б�   :
  
  �޸���ʷ   :
    ��    ��   : 2009��12��2��
    ��    ��   : qichenghong KF19818
    �޸�����   : �����ļ�

******************************************************************************/
/*include ͷ�ļ�*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>
#include <errno.h>
#include <mntent.h>
#include "atputil.h"
#include "loginctl.h"

#include "msgapi.h"

#ifdef SUPPORT_ATP_USBSTORAGE

/******************************************************************************
  ��������  : ATP_LoginCtlInit
  ��������  : ��ʼ����¼����ģ��, ��ʼ��ȫ�ֱ�����������¼ģ�����õ�Ŀ¼
  �������  : ulLogType:��¼���ƵĽ�������
  
  ���ú���  : 
  ��������  : �ṩ���ⲿ����
  �������  : ��
  �� �� ֵ  : VOS_OK�ɹ�
  			     VOS_ERRORʧ��
  ˵    ��  : ��
  
  �޸���ʷ     :
  1.��    ��   : 2009��12��2��
    ��    ��   : qichenghong KF19818
    �޸�����   : �����ɺ���
  2.��    ��   : 2010��2��3��
    ��    ��   : qichenghong KF19818
    �޸�����   : �޸ĳ�ֻ����Ϣ��ʼ��
******************************************************************************/
VOS_UINT32 ATP_LoginCtlInit(VOS_UINT32 ulLogType)
{
    VOS_UINT32 ulRet = VOS_OK;

    if(ATP_LOGINTYPE_FTP == ulLogType)
    {
        ulRet = ATP_MSG_Init(ATP_CBBID_BFTPD_NAME);
    }
    else
    {
        if(ATP_LOGINTYPE_SAMBA == ulLogType)
        {
            ulRet = ATP_MSG_Init(ATP_CBBID_SMBD_NAME);
        }
    }

    return ulRet;
}

/******************************************************************************
  ��������  : ATP_LoginCtlCheck
  ��������  : ���ݴ�FTP/SMB ������û���¼��Ϣ���жϸ��û�Ŀǰ�Ƿ�ɺϷ���¼,
                           ������Ӧ��Ϣд���¼�ļ�LOGIN_FILE ��
  �������  : pszUsername : �û���
                           pszIpAddr : ��¼�ն˵�IP ��ַ
                           pszLogType : ��¼��ʽ, FTP/SMB
                           ulLogStatus : ��¼״̬, �ɹ�LOGIN_OK / ʧ��LOGIN_FAIL
  ���ú���  : 
  ��������  : �ṩ���ⲿ����
  �������  : ��
  �� �� ֵ  :    ATP_LOGINRET_OK        	  //��ʾ���û��ɼ�����¼
    				 ATP_LOGINRET_LOGINED      //��ʾ���û��Ѿ���¼��
     				 ATP_LOGINRET_FAIL_EXD    //��ʾ��ǰ�û���¼ʧ�ܴ����Ѿ��������ƴ���
     				 ATP_LOGINRET_EXISTED,            //��ʾͬһ���û�ͬһ���ն��Ѿ���¼��һ��
     				 ATP_LOGINRET_INNORMAL   //��ʾ����������
  			     
  ˵    ��  : ��
  
  �޸���ʷ     :
  1.��    ��   : 2009��12��2��
    ��    ��   : qichenghong KF19818
    �޸�����   : �����ɺ���
******************************************************************************/
ATP_LOGINRET_EN ATP_LoginCtlCheck(const VOS_CHAR *pszUsername ,const VOS_CHAR *pszIpAddr ,VOS_UINT32 ulLogType , 
                                                                VOS_UINT ulLogStatus)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32  ulRet = VOS_OK;

    LoginDebugPrint("Begin check login status...\n");

    if((VOS_NULL_PTR == pszUsername) && (VOS_NULL_PTR == pszIpAddr))
    {
        LoginDebugPrint("The pointer parameter is NULL\n");
        return ATP_LOGINRET_INNORMAL;
    }
    
    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %s %u %u" , pszUsername , pszIpAddr , ulLogType , ulLogStatus);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_CHECK , 0);
    pstResp = NULL;
     /*modify by y00181549 for ��Ϣ�ַ����Ľ�����at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return ATP_LOGINRET_INNORMAL;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    ulRet = (ATP_LOGINRET_EN)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);

    return (ATP_LOGINRET_EN)ulRet;
}

/******************************************************************************
  ��������  : ATP_LoginCtlLogout
  ��������  : �û��ǳ�, ����û��Ѿ�������ǳ�ʱ����û���ص�������Ϣ
  �������  : pszUsername : �û���
                           ulLogintype : �û���¼�ķ�ʽFTP/SMB

  ���ú���  : 
  ��������  : �ṩ���ⲿ����
  �������  : ��
  �� �� ֵ  : VOS_OK�ɹ�
  			     VOS_ERRORʧ��
  ˵    ��  : ��
  
  �޸���ʷ     :
  1.��    ��   : 2009��12��3��
    ��    ��   : qichenghong KF19818
    �޸�����   : �����ɺ���
******************************************************************************/
VOS_INT32 ATP_LoginCtlLogout(const VOS_CHAR *pszUsername , const VOS_CHAR *pszAddrInfo,VOS_UINT32 ulLogintype)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32   ulRet = VOS_OK;
    VOS_INT32   lRet = VOS_OK;

    LoginDebugPrint("Begin check login status...\n");

    if(VOS_NULL_PTR == pszUsername) 
    {
        LoginDebugPrint("The pointer parameter is NULL\n");
        return VOS_ERROR;
    }
    
    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %s %u" , pszUsername, pszAddrInfo,ulLogintype);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_LOGOUT , 0);
    pstResp = NULL;
     /*modify by y00181549 for ��Ϣ�ַ����Ľ�����at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return VOS_ERROR;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    lRet = (VOS_INT32)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);

    return lRet;
}

/******************************************************************************
  ��������  : ATP_LoginCtlAllowWanAcc
  ��������  : �ж�һ��ָ���û��Ƿ����ͨ��WAN�����,��Ҫ�õ������ļ�/var/ftp/ftpwanacc
  �������  : pszUsername : �û���
                           ulLogintype : ��¼��ʽ,Ŀǰδ��

  ���ú���  : 
  ��������  : �ṩ���ⲿ����
  �������  : ��
  �� �� ֵ  : VOS_OK ���û�����ͨ��WAN�����
  			     VOS_ERROR �����쳣,���ļ�/var/ftp/ftpwanacc ������
  			     VOS_NOK ���û�����ͨ��WAN�����
  ˵    ��  : ��
  
  �޸���ʷ     :
  1.��    ��   : 2009��12��3��
    ��    ��   : qichenghong KF19818
    �޸�����   : �����ɺ���
  2.��    ��   : 2009��12��7��
    ��    ��   : qichenghong KF19818
    �޸�����   : ��Ӻ�����
******************************************************************************/
VOS_INT32 ATP_LoginCtlAllowWanAcc(const VOS_CHAR *pszUsername , VOS_UINT32 ulLogintype)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32  ulRet = VOS_OK;
    VOS_INT32   lRet = VOS_OK;

    LoginDebugPrint("Begin check login status...\n");

    if(VOS_NULL_PTR == pszUsername) 
    {
        LoginDebugPrint("The pointer parameter is NULL\n");
        return VOS_ERROR;
    }
    
    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %u" , pszUsername , ulLogintype);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_ALLOWWAN , 0);
    pstResp = NULL;
     /*modify by y00181549 for ��Ϣ�ַ����Ľ�����at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return VOS_ERROR;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    lRet = (VOS_INT32)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);

    return lRet;
}
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL //y00181549 for ����ftp����ϸ��Ȩ�޿��� at 2012.6.10
/******************************************************************************
  ��������  : ATP_LoginCtlCheckFtpAcc
  ��������  : ����FTP ���ͺ����������͵��жϴ������Ƿ���Խ���
  �������  :
                          

  ���ú���  : 
  ��������  : �ṩ���ⲿ����
  �������  : ��
  �� �� ֵ  : VOS_OK �����ӿ��Խ���
  			     VOS_NOK�����Ӳ����Խ���
  ˵    ��  : ��
  
  �޸���ʷ     :
  1.��    ��   : 2012.6.10
    ��    ��   : y00181549
    �޸�����   : �����ɺ���
******************************************************************************/
VOS_INT32 ATP_LoginCtlCheckFtpAcc(const VOS_CHAR *pszUsername, VOS_UINT32 ulFtpType,VOS_UINT32 ulFromType)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32  ulRet = VOS_OK;
    VOS_INT32   lRet = VOS_OK;

    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %u %u" , pszUsername, ulFtpType,ulFromType);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_ALLOWFTPTYPE , 0);
    pstResp = NULL;
    /*modify by y00181549 for ��Ϣ�ַ����Ľ�����at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    LoginDebugPrint("ulDataLen=%d\n",pstMsg->ulDataLen);
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return VOS_ERROR;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    lRet = (VOS_INT32)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);
    //printf("---------------->%d\r\n",ulRet);
    return lRet;
}
#endif
#endif

/*****************************************************************************
 �� �� ��  : ATP_UTIL_IsPathVfat
 ��������  :�жϴ���·���ļ�ϵͳ�����Ƿ�ΪVFAT
 �������  : 
 �������  : 
 �� �� ֵ     : 
                       0: ����FAT�ļ�ϵͳ��
                       1: ��FAT�ļ�ϵͳ��
              
 ���ú���  :��
 ��������  : 
 ˵    ��  �� 
 
 �޸���ʷ      :
  1.��    ��   : 2010��4��8��
    ��    ��   : lvxin 00135113
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_INT32   ATP_UTIL_IsPathVfat(const char *pcPath)
{
    FILE                *fstab;
    struct mntent       mtEnt;
    VOS_CHAR        acBuf[1024];

    if (NULL == pcPath)
    {
        return 0;
    }

    fstab = setmntent("/proc/mounts", "r");
    if (NULL == fstab)
    {
        return 0;
    }

    memset((void *)(&(mtEnt)), 0, sizeof(mtEnt));
    memset((void *)acBuf, 0, sizeof(acBuf));
    while (NULL != getmntent_r(fstab, &mtEnt, acBuf, sizeof(acBuf)))
    {
    #if 0
        printf("================================\n");
        printf("fsname: [%s].\n", mtEnt.mnt_fsname);
        printf("dir   : [%s].\n", mtEnt.mnt_dir);
        printf("type  : [%s].\n", mtEnt.mnt_type);
        printf("opts  : [%s].\n", mtEnt.mnt_opts);
    #endif
        if ((NULL != mtEnt.mnt_type) && (0 != strcmp(mtEnt.mnt_type, "vfat")))
        {
            continue;
        }

        if ((NULL != mtEnt.mnt_dir) &&
            (0 == memcmp((void *)(mtEnt.mnt_dir), (void *)pcPath, strlen(mtEnt.mnt_dir))) &&
            (('/' == pcPath[strlen(mtEnt.mnt_dir)]) || ('\0' == pcPath[strlen(mtEnt.mnt_dir)])))
        {
            endmntent(fstab);
            return 1;
        }

        memset((void *)(&(mtEnt)), 0, sizeof(mtEnt));
        memset((void *)acBuf, 0, sizeof(acBuf));
    }
    endmntent(fstab);

    return 0;
}



