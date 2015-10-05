#ifndef __ATP_ABUSE_MSGTYPES_H__
#define __ATP_ABUSE_MSGTYPES_H__


#define ATP_CBBID_ABUSECMD_NAME "abusecmd"

enum ATP_ABUSE_MSGTYPES_EN
{
    ATP_MSG_ABUSE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_ABUSECMD),
    ATP_MSG_ABUSECMS_FINISH,                          // finish update
} ;

#endif // End of __ATP_ABUSE_MSGTYPES_H__
