/**\file atsmsgtypes.h
  *�����ں˵����ݿ���Ϣ������ҵ��ģ�����Ϣ
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author w00190448
  *\date  2012-2-2
*/
/*******************************************************************************
Edit History of Huawei: 
<�޸���>     <�޸�ʱ��>    <���ⵥ��>          <�޸�����>
z00159386    2012/10/16    DTS2012102201180    TELUS�������󣬰���Voicemail�����ο��ء�SMS�ض���
*******************************************************************************/
#ifndef __ATSMSGTYPES_H__
#define __ATSMSGTYPES_H__

#define ATP_CBBID_ATSERVER_NAME "atserver"

/*��Ϣ����*/
/* <DTS2013083001384 xiazhongyue 20130830 begin */
typedef enum tagATP_ATS_MSG_TYPE_EN
{
    ATP_ATS_MSG_ATS_START       =  ATP_MSG_CATEGORY_DEF(ATP_MODULE_ATSERVER),
    ATP_ATS_MSG_UNKNOWN,        /*��ʼ����ֵ*/
    ATP_ATS_MSG_REQ,                 /*��ͨ��at cmd����*/
    ATP_ATS_MSG_ACTIVE_REPORT,          /*���������ϱ�*/
    ATP_ATS_MSG_GET_REPORT_REQ,         /*client ��ȡ�����ϱ���Ϣ*/
    ATP_ATS_MSG_VOICE_REQ,    /*�������͹�������*/
    ATP_MSG_DATACARD_START,
    ATP_MSG_DATACARD_STOP,
    ATP_MSG_WM_RST,
    //!֪ͨwanumts������Ϣ
    ATP_E_WANUMTSCMS_STATE_CHANGE,
    ATP_MSG_WM_START,
    ATP_MSG_WM_STOP,    
    ATP_MSG_NEWSMS_COMING,/* ���߲��ж���Ϣ�յ� */
    ATP_MSG_SMS_REQ,       /* ���Զ���ģ���at���� */
/* <DTS2012072100279 z00189398 2012-07-21 begin */
/*���ú�ɾ���ض�����Ϣ*/
    ATP_E_WANUMTSCMS_SET_REDIRECT,
    ATP_E_WANUMTSCMS_REMOVE_REDIRECT,
/* DTS2012072100279 z00189398 2012-07-21 end> */
/*  <DTS2013022804321 wanglong 2012-2-28 begin */
/* Get PLMN, Do plmn auto select */
    ATP_E_SYSCFGCMS_GET_PLMN,
/*  DTS2013022804321 wanglong 2012-2-28 end> */   
/* <DTS2012102201180  z00159386  20121012  begin */
    ATP_E_NOTIFICATIONCMS_SET_REDIRECT,
    ATP_E_NOTIFICATIONCMS_REMOVE_REDIRECT,
    ATP_E_NOTIFICATIONCMS_DELAY_TIMER,
    ATP_E_NOTIFICATIONCMS_CANCEL_TIMER,
/* DTS2012102201180  z00159386  20121012  end> */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
    ATP_MSG_SYNC_WIRELESS_TIME
/* DTS2013012602546 z81004485 2013/1/28 end> */
} ATP_ATS_MSG_TYPE_EN;

#endif
