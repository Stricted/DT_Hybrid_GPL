/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpdebugapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2012-8-10
*/
#ifndef __ATP_DEBUG_API_H__
#define __ATP_DEBUG_API_H__

#define ATP_DEBUG_MOD_ALL       "all"

#define ATP_DEBUG_HELP          "help"      /* debug help */
#define ATP_DEBUG_SWITCH        "switch"    /* debug 级别设置 */
#define ATP_DEBUG_INFO          "info"      /* 打印关键变量值 */
#define ATP_DEBUG_TRACE         "trace"     /* trace级别设置*/

typedef VOS_UINT32 (*ATP_UTIL_Debug_Proc_CallBack)(VOS_UINT32 ulArgc, VOS_CHAR  *acLastPara[]);
typedef struct tagATP_UTIL_Debug_CMD_ST
{
    const VOS_CHAR              *pcCmd;
    ATP_UTIL_Debug_Proc_CallBack pfDebugProc;
}ATP_UTIL_Debug_CMD_ST;

enum EN_ATP_TRACE_LEVEL
{
    ATP_DEBUG_TRACE_LASTWORD,
    ATP_DEBUG_TRACE_PRINTERROR,
    ATP_DEBUG_TRACE_PRINTALL
};

enum EN_ATP_DEBUG_LEVEL
{
    ATP_DEBUG_LEVEL_OFF = 0,
    ATP_DEBUG_LEVEL_CRITICAL,
    ATP_DEBUG_LEVEL_ERROR,
    ATP_DEBUG_LEVEL_WARNING,
    ATP_DEBUG_LEVEL_NOTICE,
    ATP_DEBUG_LEVEL_MAX
};

void ATP_Debug_SetDefaultCoredumpSize();


/*!
  \brief reg module debug cmds
  \param[in]  *pcMod: 
  \param[in]  *astDebugCmds: 
  \param[in]  ulCmdNum: 
  \return 
*/
VOS_UINT32 ATP_Debug_RegModCmds(
                const VOS_CHAR                  *pcModName, 
                const ATP_UTIL_Debug_CMD_ST     *astDebugCmds, 
                VOS_UINT32                      ulCmdNum);

/*!
  \brief Debug Msg proc
  \param[in]  *pstMsg: 
  \param[in]  *pstDebugCmds: 
  \param[in]  ulArraySize: 
  \return 
*/
VOS_UINT32 ATP_Debug_MsgProc(const ATP_MSG_HEADER_ST *pstMsg);

/*!
  \brief Module Debug 
  \param[in]  lArgCnt: 
  \param[in]  **ppcArgs: 
  \return 
*/
VOS_UINT32 ATP_Debug_CliProc(VOS_UINT32 lArgCnt, VOS_CHAR **ppcArgs);


/*!
  \brief debug switch api
  \param[in]  *pulDebugLevel: 
  \param[in]  ulArgc: 
  \param[in]  *acLastPara[]: 
  \return 
*/
VOS_UINT32 ATP_Debug_SwitchCmd(VOS_UINT32 *pulDebugLevel, VOS_UINT32 ulArgc, VOS_CHAR  *acLastPara[]);

/*!
  \brief default help cmd 
  \param[in]  ulArgc: 
  \param[in]  *acLastPara[]: 
  \return 
*/
VOS_UINT32 ATP_Debug_DefaultHelpCmd(VOS_UINT32 ulArgc, VOS_CHAR  *acLastPara[]);


/*********************************************************************
    define debug/trace api

**********************************************************************/
VOS_UINT32 ATP_Debug_RecordDebugItem (VOS_UINT32 ulDebugLevel, VOS_UINT32 ulLogItemIdx, ...);

/*
#define ATP_Debug_AppModDebugPrint(ulDebugLevel, ulLogItemIdx, …) \
{ \
	if (g_ulAppModDebugLevel >= ulDebugLevel) \
	{ \
		ATP_Debug_RecordDebugItem (ulDebugLevel, ulLogItemIdx ,##args, VOS_NULL); \
    } \
}
*/


VOS_UINT32 ATP_Debug_TracePrint(
                                    VOS_CHAR    *pcFileName, 
                    		        VOS_CHAR	*pcFunction,
                                    VOS_UINT32  ulLine, 
                                    VOS_UINT32  ulRetVal, 
                                    VOS_UINT32  ulArg1,
                                    VOS_UINT32  ulArg2,
                                    VOS_UINT32  ulArg3,
                                    VOS_UINT32  ulArg4);

VOS_UINT32 ATP_Debug_RecordLastword(
                                    VOS_CHAR    *pcFileName, 
                                    VOS_UINT32  ulLine, 
                                    VOS_UINT32  ulRetVal, 
                                    VOS_UINT32  ulArg1,
                                    VOS_UINT32  ulArg2,
                                    VOS_UINT32  ulArg3,
                                    VOS_UINT32  ulArg4);

/*
#define ATP_Debug_AppModTracePrint(ulRetVal, ulArg1, ulArg2, ulArg3, ulArg4)\
{\
    if (g_ulAppModTraceLevel == ATP_DEBUG_TRACE_PRINTALL)\
    {
        ATP_Debug_TracePrint(__FILE__, __FUNCTION__, __LINE__, ulRetVal, ulArg1, ulArg2, ulArg3, ulArg4);\
    }\
    else if (VOS_OK != ulRetVal)\
    {\
        if (g_ulAppModTraceLevel == ATP_DEBUG_TRACE_PRINTERROR)\
        {\
            ATP_Debug_TracePrint(__FILE__, __FUNCTION__, __LINE__, ulRetVal, ulArg1, ulArg2, ulArg3, ulArg4);\
        }\
        else\
        {\
            ATP_Debug_RecordLastword(__FILE__, __FUNCTION__, __LINE__, ulRetVal, ulArg1, ulArg2, ulArg3, ulArg4);
        }\
    }\
}
*/

#endif
