/*****************************************************************************
 *               Copyright (C) 2006, Huawei Tech. Co., Ltd.               
 *                           ALL RIGHTS RESERVED                               
 * FileName: proclockutil.h                                                          
 * Version: 1.0                                                                
 * Description:  proc lock interface      
 *                                                                             
 * History:                                                                    
 * 1. 2013-09-04,l00180987 Create this file.                                  
******************************************************************************/

#ifndef __ATP_PROCLOCKUTIL_H__
#define __ATP_PROCLOCKUTIL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*需要进行互斥执行命令的lock文件*/
#define IPTABLES_LOCK_FILE "/var/iptables_lock_file"
#define IP6TABLES_LOCK_FILE "/var/ip6tables_lock_file"

typedef struct TagATP_PROC_LOCK_INFO_ST
{
    pid_t ulProcPid;        /*进程pid*/
    FILE  *pfLockFile;      /*进程lock流*/
}ATP_PROC_LOCK_INFO_ST;

FILE *ATP_UTIL_LockProc(const char *psLockFile);

int ATP_UTIL_UnLockProc(FILE * fpLockFile);

void ATP_UTIL_RegUnLock(FILE * fpLockFile);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __ATP_PROCLOCKUTIL__ */

