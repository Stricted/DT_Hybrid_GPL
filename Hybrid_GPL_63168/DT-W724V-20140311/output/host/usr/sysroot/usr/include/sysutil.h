#ifndef __ATP_SYSUTIL_H__
#define __ATP_SYSUTIL_H__

VOS_UINT32 ATP_UTIL_GetSysTime(struct timeval* pstTimeVal);

VOS_INT32 ATP_UTIL_Base64Encode(const VOS_UINT8* pucInput, const VOS_INT32 lInputlen, VOS_UINT8** ppucOutput);
VOS_INT32 ATP_UTIL_Base64Decode(const VOS_UINT8* pucInput, VOS_INT32 lInputlen, VOS_UINT8** ppucOutput);

/*
 *  Added for starting new process.
 *      lWaitMiliSecs: -1 means wait forever; 0 means no wait; >0 means wait for that miliseconds
 *  Usage:
 *      system("test args"); can be replaced with ATP_UTIL_ForkProcess("test args", -1, NULL, NULL);
 *      With system("test args"), you can find two processes in ps: "sh -si" and "test args"
 *      But with ATP_UTIL_ForkProcess, you will only find "test args" in ps.
 */
VOS_VOID ATP_UTIL_FreeArgs(VOS_CHAR **argv);

typedef struct tagATP_PARSE_SH_SYMBOL_ST
{
    VOS_CHAR        *pcStdoutFile;
    VOS_CHAR        *pcStderrFile;
    VOS_BOOL        bStdoutAppend;
    VOS_BOOL        bStderrAppend;
    VOS_BOOL        bIsBackground;
} ATP_PARSE_SH_SYMBOL_ST;

VOS_UINT32 ATP_UTIL_ParseArgs(const VOS_CHAR *pcStr, VOS_CHAR *** pppcArgv,
                VOS_INT32 *plArgc, ATP_PARSE_SH_SYMBOL_ST *pstSymbols);

// lWaitMiliSecs: -1 means wait forever; 0 means no wait; >0 means wait for that miliseconds
VOS_INT32 ATP_UTIL_WaitForPid(VOS_INT32 lPid, VOS_INT32 lWaitMiliSecs,
                                            VOS_INT32 *plExitSignal, VOS_INT32 *plExitCode);

extern const VOS_CHAR *g_apcBinSearchPath[];
VOS_INT32 ATP_UTIL_GetExeFullPath(const VOS_CHAR *pcExeName, VOS_CHAR *pcFullPath, VOS_UINT32 ulLength);

/*
����: �ȴ����̸ı���״̬
 ���� 
                lPid
                ulWaitMiliSecs: 0 ����, -1 һֱ��,�����ȴ���ms��
 ���    
                *plExitSignal
                *plExitCode    
 ����ֵ-1:����
                  >0  �ȴ���ʱ,����lpid
                        ��������״̬�ı���ӽ��̱�ʶ
                
*/
VOS_INT32 ATP_UTIL_WaitForPid(VOS_INT32 lPid, VOS_INT32 lWaitMiliSecs,
                                            VOS_INT32 *plExitSignal, VOS_INT32 *plExitCode);
/*
����: �ȴ����̸ı���״̬
 ���� 
                lPid
                ulWaitMiliSecs: 0 ����, -1 һֱ��,�����ȴ���ms��
 ���    
                *plStatus: �������ֵ >0  ���� *pbWaitTimeout �� VOS_FALSE ,*plStatus �� waitpid����� *status
                *pbWaitTimeout: VOS_TRUE ulWaitMiliSecs �� 0 or �ȴ���ʱ
                                          ���� VOS_FALSE       
 ����ֵ-1:����
                  >0  �ȴ���ʱ,����lpid
                        ��������״̬�ı���ӽ��̱�ʶ
                
*/
VOS_INT32 ATP_UTIL_WaitForPidEx(VOS_INT32 lPid, VOS_INT32 lWaitMiliSecs, VOS_INT32 *plStatus, VOS_BOOL  *pbWaitTimeout);
// lWaitMiliSecs: -1 means wait forever; 0 means no wait; >0 means wait for that miliseconds
VOS_INT32 ATP_UTIL_ForkProcessEx(const VOS_CHAR *argv[], VOS_INT32 lWaitMiliSecs,
                                            VOS_INT32 *plExitSignal, VOS_INT32 *plExitCode);

// lWaitMiliSecs: -1 means wait forever; 0 means no wait; >0 means wait for that miliseconds
VOS_INT32 ATP_UTIL_ForkProcessFull(const VOS_CHAR *argv[], VOS_INT32 lWaitMiliSecs,
                                            VOS_INT32 *plExitSignal, VOS_INT32 *plExitCode,
                                            ATP_PARSE_SH_SYMBOL_ST *pstSymbols);
/* 
����
    fork�ӽ���ִ��argv������
����
    argv
    lWaitMiliSecs:0 ����, -1 һֱ��,�����ȴ���ms��
    pstSymbols
���
   *plStatus: �������ֵ >0  ���� *pbWaitTimeout �� VOS_FALSE ,*plStatus �� waitpid����� *status
   *pbWaitTimeout: VOS_TRUE ulWaitMiliSecs �� 0 or �ȴ���ʱ
                            VOS_FALSE ����    
����ֵ
    -1:����
    >0:fork�ӽ���pid
*/
VOS_INT32 ATP_UTIL_ForkProcessFullEx(const VOS_CHAR *argv[], VOS_INT32 lWaitMiliSecs,
                                            VOS_INT32 *plStatus, VOS_BOOL  *pbWaitTimeout,
                                            ATP_PARSE_SH_SYMBOL_ST *pstSymbols);
// lWaitMiliSecs: -1 means wait forever; 0 means no wait; >0 means wait for that miliseconds
VOS_INT32 ATP_UTIL_ForkProcess(const VOS_CHAR *pcCmd, VOS_INT32 lWaitMiliSecs,
                                            VOS_INT32 *plExitSignal, VOS_INT32 *plExitCode);
/* 
����
    fork�ӽ���ִ��pcCmd������
����
    pcCmd
    lWaitMiliSecs:0 ����, -1 һֱ��,�����ȴ���ms��
���
   *plStatus: �������ֵ >0  ���� *pbWaitTimeout �� VOS_FALSE ,*plStatus �� waitpid����� *status
   *pbWaitTimeout: VOS_TRUE ulWaitMiliSecs �� 0 or �ȴ���ʱ
                            VOS_FALSE ����    
����ֵ
    -1:����
    >0:fork�ӽ���pid
*/
VOS_INT32 ATP_UTIL_ForkProcessSys(const VOS_CHAR *pcCmd, VOS_INT32 lWaitMiliSecs,
                                                                VOS_INT32 *plStatus, VOS_BOOL  *pbWaitTimeout);


VOS_INT32 ATP_UTIL_Base64Encode(const VOS_UINT8* pucInput, VOS_INT32 lInputlen, VOS_UINT8** ppucOutput);
VOS_INT32 ATP_UTIL_Base64Decode(const VOS_UINT8* pucInput, VOS_INT32 lInputlen, VOS_UINT8** ppucOutput);

VOS_UINT32 ATP_UTIL_Base64EncodeEx(const VOS_UINT8* pucInput, VOS_UINT32 ulInputlen, VOS_UINT8** ppucOutput, VOS_UINT32 *pulOutputLen);
VOS_UINT32 ATP_UTIL_Base64DecodeEx(const VOS_UINT8* pucInput, VOS_UINT32 ulInputlen, VOS_UINT8** ppucOutput, VOS_UINT32 *pulOutputLen);


#endif

