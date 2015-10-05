#ifndef __ATP_VOICECORE_MSGTYPES_H__
#define __ATP_VOICECORE_MSGTYPES_H__

#define  ATP_CBBID_VOICE_NAME  "voice"

enum ATP_VOICECORE_MSGTYPES_EN
{
    ATP_MSG_VOICECORE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DTVOICE),
    ATP_MSG_VOICE_DECT_HS_REG,               /*Dect注册*/
    ATP_MSG_VOICE_DECT_HS_UNREG,             /*Dect注销*/  
    ATP_MSG_VOICE_DECT_REBOOT,               /*DECT 异常重启*/
    ATP_MSG_VOICE_ANIMSN_REPORT,             /*自动获取MSN结果*/
    ATP_MSG_VOICE_DECT_RESTORE,             /*dect配置和cmbs的初始化*/
};

#endif // End of __ATP_VOICECORE_MSGTYPES_H__