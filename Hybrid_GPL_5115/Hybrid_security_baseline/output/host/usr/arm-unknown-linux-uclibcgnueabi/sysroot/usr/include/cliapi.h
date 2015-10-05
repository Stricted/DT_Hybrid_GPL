#ifndef __ATP_CLI_API_H__
#define __ATP_CLI_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atptypes.h"

#ifdef ATP_DEBUG
#define ATP_CLI_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: in %s[", __FILE__, __LINE__,__FUNCTION__);printf(format, ##args);printf("]\n");}
#else
#define ATP_CLI_DEBUG(x...)
#endif

#define ATP_CLI_TRACE       ATP_MSG_Printf


#ifdef SUPPORT_ENG_VER
/*add just for no debug ver, when there is debug ver, delete it*/
#undef SUPPORT_ATP_CLI_PROTECT
#endif

typedef enum tagATP_CLI_RET_CODE_EN
{
    ATP_CLI_RET_PARA_ERR        = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_CLI),
    ATP_CLI_RET_MEM_ERR,
    ATP_CLI_RET_NOT_FOUND_ERR,
    ATP_CLI_RET_PREV_ERR,
    ATP_CLI_RET_SYS_ERR,
    ATP_CLI_RET_PTY_ERR,
    ATP_CLI_RET_TIMEOUT_ERR,

} ATP_CLI_RET_CODE_EN;

typedef enum{
    CLI_CONSOLE_IN = 0,
    CLI_TELNET_IN,
    CLI_SSH_IN,
    CLI_ERR_IN
}CLI_LoginType_EN;

extern CLI_LoginType_EN     g_enAccessMode;
extern VOS_UINT32           g_ulCliPriv;
extern VOS_BOOL             g_bCliHidCmdFlag;
extern VOS_UINT32           g_ulCliTimeout;

#define CLI_CMD_PARA_LEN            128 /*the para length of setouinum could be as long as 64*/
#define CLI_CMD_LEN                 256

/*the max and min length of ssid */

#define ATP_CLI_ENDOF_LINE      "\r\n"
#define ATP_CLI_CMD_LEN_D       (256)

#define FLASHINITNUM            -1          /*内存中0xff值在char变量中的值为-1*/
#define MAX_MAC_STR_LEN         19    // mac address string 18+1 in regular format
/*
 *  Read a line from the command line, we can only delete, move forward and move backword
 */

extern const VOS_CHAR* g_pszWelcomeInfo;

VOS_UINT32 ATP_CLI_ReadLine(VOS_CHAR *acCmd, VOS_UINT ulBufLen, VOS_BOOL bEcho);
VOS_VOID ATP_CLI_SetTerm(VOS_INT32 lFd);

VOS_UINT32 ATP_CLI_AdaptAuthenticateCheck(const VOS_CHAR  *pszLoginName, const VOS_CHAR  *pszLoginPasswd);

/*
 *  Command register and parsing
 */

// Start privilege define
#define CLI_PRIV_USER_READBIT               0x01 //0x0001
#define CLI_PRIV_ADMIN_READBIT              0x08
#define CLI_PRIV_USER_WRITEBIT              0x10
#define CLI_PRIV_ADMIN_WRITEBIT             0x80

/*command hidden bit*/
#define ATP_CLI_HIDDEN_COMMAND_BIT             0x40   /*0100 0000*/

// Get privilege of command
#define CLI_PRIV_GETUSER_READ(iPriv)        ((iPriv) & CLI_PRIV_USER_READBIT)
#define CLI_PRIV_GETADMIN_READ(iPriv)       ((iPriv) & CLI_PRIV_ADMIN_READBIT)

#define CLI_PRIV_GETUSER_WRITE(iPriv)       ((iPriv) & CLI_PRIV_USER_WRITEBIT)
#define CLI_PRIV_GETADMIN_WRITE(iPriv)      ((iPriv) & CLI_PRIV_ADMIN_WRITEBIT)

// Set privilege of command
#define CLI_PRIV_SETUSER_READ(iPriv)        (VOS_UINT8)((iPriv) | CLI_PRIV_USER_READBIT)
#define CLI_PRIV_SETADMIN_READ(iPriv)       (VOS_UINT8)((iPriv) | CLI_PRIV_ADMIN_READBIT)

#define CLI_PRIV_SETUSER_WRITE(iPriv)       (VOS_UINT8)((iPriv) | CLI_PRIV_USER_WRITEBIT)
#define CLI_PRIV_SETADMIN_WRITE(iPriv)      (VOS_UINT8)((iPriv) | CLI_PRIV_ADMIN_WRITEBIT)

// Used for Register cmd
#define ATP_CLI_USERADMIN_R                 (VOS_UINT8)(CLI_PRIV_USER_READBIT | CLI_PRIV_ADMIN_READBIT)
#define ATP_CLI_USERADMIN_W                 (VOS_UINT8)(CLI_PRIV_USER_WRITEBIT | CLI_PRIV_USER_WRITEBIT)
#define ATP_CLI_USER_RW                     (VOS_UINT8)(CLI_PRIV_USER_READBIT | CLI_PRIV_USER_WRITEBIT)
#define ATP_CLI_ADMIN_RW                    (VOS_UINT8)(CLI_PRIV_ADMIN_READBIT | CLI_PRIV_ADMIN_WRITEBIT)
#define ATP_CLI_ALL_RW                      (VOS_UINT8)(ATP_CLI_USER_RW | ATP_CLI_ADMIN_RW)

/*for some hidden command privilege*/
#define ATP_CLI_USER_RW_HIDDEN           (VOS_UINT8)(ATP_CLI_USER_RW | ATP_CLI_HIDDEN_COMMAND_BIT)
#define ATP_CLI_ADMIN_RW_HIDDEN          (VOS_UINT8)(ATP_CLI_ADMIN_RW | ATP_CLI_HIDDEN_COMMAND_BIT)

/*End of privilege define*/


/******************结构体***********************************************/
/*
暂时只使用两个级别.
低16位用于表示读权限。
高16位用于表示写权限。
共32位。

READPRIV:
0-2:(3bit) User Read
3-5:(3bit) Admin Read
6-15:(10) User for Group .now not used

WRITEPRIV:
16-18:(3bit) User Write
19-21:(3bit) Admin Write
21-32:(10) User for Group .now not used
*/

typedef struct CLI_PRIV_ST{
    VOS_INT ReadPriv:15;
    VOS_INT ReadObl:  1;/*保留位*/
    VOS_INT WritePriv:15;
    VOS_INT WriteObl:  1;/*保留位*/
}ATP_CLI_RPIV_ST;

/*CallbackFunction deal with argument*/
//typedef VOS_INT (*ATP_CLI_Reg_Proc_CallBack)( VOS_UINT32 ulArgc, VOS_CHAR **ppargv);
typedef VOS_UINT32 (*ATP_CLI_Reg_Proc_CallBack)(VOS_UINT32 ulArgc, VOS_CHAR  *acLastPara[]);
typedef VOS_VOID (*ATP_CLI_Help_Proc_CallBack)();
/*enum*/

/**********************************
0 bit is command type tag
1 bit is command end tag
***********************************/
#define ATP_CLI_SUBCMD       0x0
#define ATP_CLI_PROCFUNC    0x1
#define ATP_CLI_CMDEND        0x2

#define CLI_IS_CMDEND(TAG) (TAG & ATP_CLI_CMDEND)
#define CLI_CMDTYPE(TAG) (TAG & ATP_CLI_PROCFUNC)


/*union*/
typedef union unCli_REG_Tag
{
    ATP_CLI_Reg_Proc_CallBack pfnCliProcCallback;
    struct stCLI_REG_ST *pstSubCmd;
}CLI_REGCMD_UN;

/*Struct*/
typedef struct stCLI_REG_ST{
    VOS_CHAR *pszCmdName;
    /*privilege control*/
    VOS_UINT8 ulPrivMask;
    /*we do not use this tag*/
    VOS_UINT8 ulNumOfSubs;
    VOS_UINT8 enCliSubCmdTag;
    /*we have one byte for resever*/
    CLI_REGCMD_UN unCliCmdFuncTag;    
}ATP_CLI_REGCMD_ST,CLI_REG_ST;

/*最大参数个数*/
#define ATP_CLI_MAX_PARM_COUNTS     (16)

/*CLI提供接口如下*/
extern const ATP_CLI_REGCMD_ST g_stCliCmd[];    // Cmd tree defined in cliadapt.c
extern VOS_UINT32 g_ulCliPriv;

VOS_UINT32 ATP_CLI_CmdProc(VOS_CHAR  *acCmdBuf);

VOS_UINT32 ATP_CLI_CmdShellProc(VOS_VOID);

/*****************************************************************
内部实现的命令
clisdk.c调用到的命令
******************************************************************/

#ifdef SUPPORT_ATP_VENDORCFG
VOS_UINT32 Cli_CustomizeAddFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_CustomizeSaveFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_CustomizeSetFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_CustomizeShowFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_CustomizeCleanFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_CustomizeDelFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif

VOS_UINT32 Cli_DebugTraceProcess(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 Cli_SaveCfgFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_SetDynProcIdleTime(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

/**************************************************************************
clicmd.c调用的命令
**************************************************************************/
VOS_UINT32 Cli_ClsFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_HelpFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

/* <DTS2012101501678 j81004134 2012/10/16 begin */
#define ONLINE_INTERVAL_FILE "/tmp/onlineinterval"
VOS_UINT32 Cli_OnlineCheckTimeFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
/* DTS2012101501678 j81004134 2012/10/16 end> */
extern VOS_UINT32 g_ulShTimeout;

//add by huangji 
VOS_UINT32 Cli_SendModuleDebugMsg(VOS_CHAR *pcModuleName, VOS_CHAR *pcRetDebugMsg);
VOS_UINT32 Cli_ModuleDebugProc(VOS_UINT32 lArgCnt, VOS_UINT32 ulRetVal, VOS_CHAR **ppcArgs);
//end
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif

