/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��       : bftpd_message.h
  �� �� ��       : ����
  ��    ��           : y00183935
  ��������   : 2012��6��25��
  ����޸�   :
  ��������   : bftpd_message.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��        : 2012��6��25��
    ��    ��         : y00183935
    �޸����� : �����ļ�

******************************************************************************/
#ifndef __BFTPD_MESSAGE_H__
#define __BFTPD_MESSAGE_H__
#include "atptypes.h"
#include "msgapi.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


VOS_UINT32 ATP_BFTPD_CHILD_Init( );

#ifndef SUPPORT_ATP_FTP_UPGRADE
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_TYPE_EN eOperate, ATP_FTPSERVERCMS_IPTYPE_EN eIPType, VOS_UINT32 iPort );
#endif
#ifdef SUPPORT_ATP_FTP_UPGRADE
VOS_UINT32 ATP_BFTPD_CHILD_SendRecv_DOUPGRADE( );
#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */



#endif  //end #ifndef __BFTPD_MESSAGE_H__


