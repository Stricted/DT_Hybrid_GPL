/**\file atsmsgtypes.h
  *接收内核的数据卡消息，接收业务模块的消息
  *\copyright 2007-2020，华为技术有限公司
  *\author w00190448
  *\date  2012-2-2
*/
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z00159386    2012/10/16    DTS2012102201180    TELUS定制需求，包含Voicemail、漫游开关、SMS重定向
*******************************************************************************/
#ifndef __ATSMSGTYPES_H__
#define __ATSMSGTYPES_H__

#define ATP_CBBID_ATSERVER_NAME "atserver"

/*消息类型*/
typedef enum tagATP_ATS_MSG_TYPE_EN
{
    ATP_ATS_MSG_ATS_START       =  ATP_MSG_CATEGORY_DEF(ATP_MODULE_ATSERVER),
    ATP_ATS_MSG_UNKNOWN,        /*初始化赋值*/
    ATP_ATS_MSG_REQ,                 /*普通的at cmd请求*/
    ATP_ATS_MSG_ACTIVE_REPORT,          /*激活主动上报*/
    ATP_ATS_MSG_GET_REPORT_REQ,         /*client 获取主动上报消息*/
    ATP_ATS_MSG_VOICE_REQ,    /*语音发送过来请求*/
    ATP_MSG_DATACARD_START,
    ATP_MSG_DATACARD_STOP,
/* <DTS2013083001384 xiazhongyue 20130830 begin */
    ATP_MSG_WM_RST,
    //!通知wanumts拨号消息
    ATP_E_WANUMTSCMS_STATE_CHANGE,
    ATP_MSG_WM_START,
    ATP_MSG_WM_STOP,    
    ATP_MSG_NEWSMS_COMING,/* 无线侧有短消息收到 */
    ATP_MSG_SMS_REQ,       /* 来自短信模块的at命令 */
/*  DTS2013083001384 xiazhongyue 20130830 end > */
/* <DTS2012072100279 z00189398 2012-07-21 begin */
/*设置和删除重定向消息*/
#if defined(SUPPORT_ATP_DNS_REDIRECT)
    ATP_E_WANUMTSCMS_SET_REDIRECT,
    ATP_E_WANUMTSCMS_REMOVE_REDIRECT,
#endif
/* DTS2012072100279 z00189398 2012-07-21 end> */
/*  <DTS2013022804321 wanglong 2012-2-28 begin */
/* Get PLMN, Do plmn auto select */
#ifdef SUPPORT_ATP_PLMN_AUTO_MATCH
    ATP_E_SYSCFGCMS_GET_PLMN,
#endif
/*  DTS2013022804321 wanglong 2012-2-28 end> */   
/* <DTS2012102201180  z00159386  20121012  begin */
#ifdef SUPPORT_ATP_NOTIFICATION
    ATP_E_NOTIFICATIONCMS_SET_REDIRECT,
    ATP_E_NOTIFICATIONCMS_REMOVE_REDIRECT,
    ATP_E_NOTIFICATIONCMS_DELAY_TIMER,
    ATP_E_NOTIFICATIONCMS_CANCEL_TIMER,
#endif /* SUPPORT_ATP_NOTIFICATION */
/* DTS2012102201180  z00159386  20121012  end> */
/* <DTS2013072304658 z81004143 2013/7/25 begin */
#ifdef SUPPORT_ATP_B880_CUBE
    ATP_E_NOTIFICATIONCMS_ROAMING,
    ATP_MSG_UPDATE_WIFI_ASSOC_NUM, /* DTS2013072309716 z81004143 2013/8/5 */
#endif
/* DTS2013072304658 z81004143 2013/7/25 end> */
/* <DTS2013012602546 z81004485 2013/1/28 begin  */
#ifdef SUPPORT_ATP_TIMEREBOOT
    ATP_MSG_SYNC_WIRELESS_TIME
#endif  /* SUPPORT_ATP_TIMEREBOOT */
/* DTS2013012602546 z81004485 2013/1/28 end> */
} ATP_ATS_MSG_TYPE_EN;

#endif
