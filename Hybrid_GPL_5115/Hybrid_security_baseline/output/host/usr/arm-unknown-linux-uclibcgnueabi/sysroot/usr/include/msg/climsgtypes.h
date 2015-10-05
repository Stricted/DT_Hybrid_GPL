#ifndef __ATP_CLI_MSGTYPES_H__
#define __ATP_CLI_MSGTYPES_H__

#define ATP_CBBID_CLI_NAME                  "cli"
#define ATP_CBBID_CONSOLE_NAME              "console"

enum ATP_CLI_MSGTYPES_EN
{
    ATP_MSG_CLI_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CLI),
        
    ATP_MSG_MODULE_DEBUG = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CLI) + 0x100,
} ;
#endif // End of __ATP_CLI_MSGTYPES_H__
