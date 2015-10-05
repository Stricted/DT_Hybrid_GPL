#ifndef __ATP_EMAILCMS_MSGTYPES_H__
#define __ATP_EMAILCMS_MSGTYPES_H__

#define ATP_CBBID_EMAIL_NAME "email"

enum ATP_EMAILCMS_MSGTYPES_EN
{
	ATP_MSG_EMAIL_GETEMAIL      = ATP_MSG_CATEGORY_DEF(ATP_MODULE_EMAILCMS), /*子机启动扫描邮件 voice-->cms*/
	ATP_MSG_EMAIL_GETEMAIL_RES,             /*更新完邮件列表后，通知cms可以读取数据 mail-->cms*/
	ATP_MSG_EMAIL_NEW_NOTIFY,               /*定时刷新后，通知cms有新邮件 mail-->cms*/
	ATP_MSG_EMAIL_REVEDLL,                  /*删除邮件 cms-->mail*/		
	ATP_MSG_EMAIL_REVESCAN,                 /*扫描邮件 cms-->mail*/
	ATP_MSG_EMAIL_READ_NOTIFY,         //unkonw     /*读取邮件后，刷新读取状态 voice-->cms*/
	ATP_MSG_EMAIL_DEL_ENTRY,                /*子机删除邮件命令 voice-->cms*/
	ATP_MSG_EMAIL_SCAN_FAILED,              /*获取失败响应 mail-->cms*/
	ATP_MSG_EMAIL_SAVEDB,                   /*将状态信息更新到DB voice-->cms*/
	ATP_MSG_EMAIL_SIGN_READ,                /*标记邮件已读*/
	
};


#endif // End of __ATP_EMAILCMS_MSGTYPES_H__

