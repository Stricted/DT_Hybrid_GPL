#ifndef __ATP_EMAILCMS_MSGTYPES_H__
#define __ATP_EMAILCMS_MSGTYPES_H__

#define ATP_CBBID_EMAIL_NAME "email"

enum ATP_EMAILCMS_MSGTYPES_EN
{
	ATP_MSG_EMAIL_GETEMAIL      = ATP_MSG_CATEGORY_DEF(ATP_MODULE_EMAILCMS), /*�ӻ�����ɨ���ʼ� voice-->cms*/
	ATP_MSG_EMAIL_GETEMAIL_RES,             /*�������ʼ��б��֪ͨcms���Զ�ȡ���� mail-->cms*/
	ATP_MSG_EMAIL_NEW_NOTIFY,               /*��ʱˢ�º�֪ͨcms�����ʼ� mail-->cms*/
	ATP_MSG_EMAIL_REVEDLL,                  /*ɾ���ʼ� cms-->mail*/		
	ATP_MSG_EMAIL_REVESCAN,                 /*ɨ���ʼ� cms-->mail*/
	ATP_MSG_EMAIL_READ_NOTIFY,         //unkonw     /*��ȡ�ʼ���ˢ�¶�ȡ״̬ voice-->cms*/
	ATP_MSG_EMAIL_DEL_ENTRY,                /*�ӻ�ɾ���ʼ����� voice-->cms*/
	ATP_MSG_EMAIL_SCAN_FAILED,              /*��ȡʧ����Ӧ mail-->cms*/
	ATP_MSG_EMAIL_SAVEDB,                   /*��״̬��Ϣ���µ�DB voice-->cms*/
	ATP_MSG_EMAIL_SIGN_READ,                /*����ʼ��Ѷ�*/
	
};


#endif // End of __ATP_EMAILCMS_MSGTYPES_H__

