#ifndef __ATP_VOICECORE_MSGTYPES_H__
#define __ATP_VOICECORE_MSGTYPES_H__

#define  ATP_CBBID_VOICE_NAME  "voice"

enum ATP_VOICECORE_MSGTYPES_EN
{
    ATP_MSG_VOICECORE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_DTVOICE),
    ATP_MSG_VOICE_DECT_HS_REG,               /*Dectע��*/
    ATP_MSG_VOICE_DECT_HS_UNREG,             /*Dectע��*/  
    ATP_MSG_VOICE_DECT_REBOOT,               /*DECT �쳣����*/
    ATP_MSG_VOICE_ANIMSN_REPORT,             /*�Զ���ȡMSN���*/
    ATP_MSG_VOICE_DECT_RESTORE,             /*dect���ú�cmbs�ĳ�ʼ��*/
};

#endif // End of __ATP_VOICECORE_MSGTYPES_H__