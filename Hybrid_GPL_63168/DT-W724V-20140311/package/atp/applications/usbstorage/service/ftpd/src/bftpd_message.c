/******************************************************************************

                  ��Ȩ���� (C), 2009-2011, ��Ϊ�������޹�˾

  �� �� ��   : bftpd_message.c
  �� �� ��   : V1.0
  ��    ��   : y00183935
  ��������   : 2012��6��26��
  ����޸�   :
  ��������   : Bftpd��ftpservercms��ģ�鷢��Ϣ
  �����б�   :
  
  �޸���ʷ   :
    ��    ��   : 2012��6��26��
    ��    ��   : y00183935
    �޸�����   : �����ļ�

******************************************************************************/
/*include ͷ�ļ�*/
#include <stdio.h>
#include <string.h>
#include "atputil.h"
#include "bftpd_message.h"
#include "msgapi.h"


/******************************************************************************
  ��������  : ATP_BFTPD_CHILD_Init
  ��������  : ��ʼ��BFTPD �ӽ��̵�MSG ��Ϣ
  �������  : ��
  
  ���ú���  : 
  ��������  : �ṩ�� BFTPD ����
  �������  : ��
  �� �� ֵ  : VOS_OK�ɹ�
  			     VOS_ERRORʧ��
  
  �޸���ʷ     :
  1.��    ��          : 2012��6��26��
     ��    ��          : y00183935
     �޸�����  : �����ɺ���
******************************************************************************/
VOS_UINT32 ATP_BFTPD_CHILD_Init( )
{
    VOS_UINT32 ulRet = VOS_NOK;

    ATP_MSG_Destroy();
    ulRet = ATP_MSG_Init( VOS_NULL );

    return ulRet;
}


#ifndef SUPPORT_ATP_FTP_UPGRADE

/******************************************************************************
  ��������  : ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg
  ��������  : ���ʹ���ACL �����͹ر����ݶ˿ڵ���Ϣ��ftpservercms
  �������  : 
         1. eOperate : �򿪡��رղ���
         2. eIPType   : IP��ַ����
         3. iPort        : �˿ں�
  
  ���ú���  : 
  ��������  : �ṩ�� BFTPD ����
  �������  : ��
  �� �� ֵ  : VOS_OK�ɹ�
  			     VOS_ERRORʧ��
  
  �޸���ʷ     :
  1.��    ��          : 2012��6��26��
     ��    ��          : y00183935
     �޸�����  : �����ɺ���
******************************************************************************/
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_TYPE_EN eOperate, ATP_FTPSERVERCMS_IPTYPE_EN eIPType, VOS_UINT32 iPort )
{
    VOS_UINT32 ulRet = VOS_NOK;

    if( iPort > 0 )
    {
#if 0 //modify by y00181549 at 20130218 for DTS2013021800116 ��ftp�ϴ����ع����У��β�Ӳ�̣�һ��ʱ��󣬻����CMS�������   
        ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
        ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;

        VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = "";
        VOS_CHAR    *pszTmp =  VOS_NULL;

        pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
        ATP_MSG_SimpleBuilder( pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_FTPSERVERCMS_DEAL_ACL_DATAPORT, 0 );

        pszTmp = (VOS_CHAR *)( aucBuf + sizeof( ATP_MSG_HEADER_ST ) );
        snprintf( pszTmp , ATP_ACTUAL_LEN_512, "%d %d %d", eOperate, eIPType, iPort );
        pstMsg->ulDataLen = strlen( pszTmp ) + 1;

        pstResp = NULL;
        ulRet = ATP_MSG_SendAndRecv( pstMsg, &pstResp, 2*1000 );

        if( VOS_OK == ulRet && VOS_NULL != pstResp )
        {
            ulRet = (pstResp->ulMsgData);
            free((VOS_VOID *)pstResp);
        }
#else
        VOS_UINT8   aucBuf[1024] = "";

        snprintf( aucBuf , sizeof(aucBuf), "%d %d %d", eOperate, eIPType, iPort );


        ATP_MSG_SendMsgData("cms", ATP_MSG_FTPSERVERCMS_DEAL_ACL_DATAPORT, (VOS_VOID*)aucBuf, strlen( aucBuf ) + 1);
#endif
    }

    return ulRet;
}
#endif

#ifdef SUPPORT_ATP_FTP_UPGRADE

/******************************************************************************
  ��������  : ATP_BFTPD_CHILD_SendRecv_DOUPGRADE
  ��������  : 
  �������  : 
         1. eOperate : �򿪡��رղ���
         2. eIPType   : IP��ַ����
         3. iPort        : �˿ں�
  
  ���ú���  : 
  ��������  : �ṩ�� BFTPD ����
  �������  : ��
  �� �� ֵ  : VOS_OK�ɹ�
  			     VOS_ERRORʧ��
  
  �޸���ʷ     :
  1.��    ��          : 2012��6��26��
     ��    ��          : y00183935
     �޸�����  : �����ɺ���
******************************************************************************/
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_DOUPGRADE( VOS_CHAR * acImagePath )
{
    VOS_UINT32 ulRet = VOS_NOK;

    if( strlen(acImagePath) > 0 )
    {
        ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
        ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;

        VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = "";
        VOS_CHAR    *pszTmp =  VOS_NULL;

        pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
        ATP_MSG_SimpleBuilder( pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_FTPUPGCMS_DOUPGRADE, 0 );

        pszTmp = (VOS_CHAR *)( aucBuf + sizeof( ATP_MSG_HEADER_ST ) );
        snprintf( pszTmp , ATP_ACTUAL_LEN_512, "%s", acImagePath);
        pstMsg->ulDataLen = strlen( pszTmp ) + 1;

        pstResp = NULL;
        ulRet = ATP_MSG_SendAndRecv( pstMsg, &pstResp, 60*1000 );

        if( VOS_OK == ulRet && VOS_NULL != pstResp )
        {
            ulRet = (pstResp->ulMsgData);
            free((VOS_VOID *)pstResp);
        }
    }

    return ulRet;
}

#endif

