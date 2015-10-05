#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "atptypes.h"
#ifdef SUPPORT_ATP_VOICE
#include "voicepub.h"
#endif

#if 1
VOS_UINT32 ATP_UTIL_GetSysTime(struct timeval* pstTimeVal)
{
    struct timespec ts;

    if (!pstTimeVal)
    {
        return VOS_NOK;
    }

	if (syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &ts))
	{
        pstTimeVal->tv_sec   = 0;
        pstTimeVal->tv_usec  = 0;
	    perror("syscall:__NR_clock_gettime");
        return VOS_NOK;
	}

    pstTimeVal->tv_sec   = ts.tv_sec;
    pstTimeVal->tv_usec  = ts.tv_nsec/1000; 
    return VOS_OK;
}
#else
VOS_UINT32 ATP_UTIL_GetSysTime(struct timeval* pstTimeVal)
{
    struct timespec ts;

    if (!pstTimeVal)
    {
        return VOS_NOK;
    }

    if (gettimeofday(pstTimeVal, NULL) < 0)
    {
        pstTimeVal->tv_sec   = 0;
        pstTimeVal->tv_usec  = 0;
        return VOS_NOK;
    }

    pstTimeVal->tv_sec   = ts.tv_sec;
    pstTimeVal->tv_usec  = ts.tv_nsec/1000; 
    return VOS_OK;
}
#endif

#ifdef SUPPORT_ATP_VOICE
/*****************************************************************************
 函 数 名: ATP_UTIL_VoiceGetVoipStatus
 功能描述: 获取VoIP状态，空闲还是忙
           VoIP状态由所有SIP帐号状态所反映，只要有一个SIP帐号为忙则返回忙状态的结果
 输入参数: 无
 输出参数: 无
 返 回 值: VoIP状态: VOS_FALSE为空闲;VOS_TRUE为忙
*****************************************************************************/
VOS_BOOL ATP_UTIL_VoiceGetVoipStatus(VOS_VOID)
{
    FILE        *pstFile;
    VOS_UINT32  ulStatus;
    VOS_CHAR    acSrcData[VOICE_STATUS_LEN_MAX];

    memset(acSrcData, 0, VOICE_STATUS_LEN_MAX);
    pstFile = fopen(FILE_PATH_VOIP_STATUS, "rb");
    if (VOS_NULL_PTR == pstFile)
    {
        return VOS_FALSE;
    }

    fread(acSrcData, 1, VOICE_STATUS_LEN_MAX, pstFile);
    fclose(pstFile);
    acSrcData[VOICE_STATUS_LEN_MAX - 1] = '\0';
    ulStatus = (VOS_UINT32)atoi(acSrcData);
    if (ulStatus > 0)
    {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}
#ifdef SUPPORT_ATP_VOICE_DT
VOS_INT32 ATP_UTIL_VoiceGetSipInterval(VOS_VOID)
{
        FILE *      fp = VOS_NULL_PTR;
        VOS_CHAR        acFile[VOICE_STATUS_LEN_MAX];   

        memset(&acFile, 0, sizeof(acFile));
        
        fp = fopen(FILE_PATH_VOIP_INTERVAL, "r");
        if ( VOS_NULL_PTR == fp )
        {
            return 0;
        }
        fread(acFile, 1, VOICE_STATUS_LEN_MAX, fp);
        fclose(fp);
        printf("the number is %d\n",(VOS_UINT32)atoi(acFile));
        return (VOS_UINT32)atoi(acFile);
}
#endif
/*****************************************************************************
 函 数 名: ATP_UTIL_VoiceGetVoipStatus
 功能描述: 获取VoIP状态，空闲还是忙
           VoIP状态由所有SIP帐号状态所反映，只要有一个SIP帐号为忙则返回忙状态的结果
 输入参数: 无
 输出参数: 无
 返 回 值: VoIP状态: VOS_FALSE为空闲;VOS_TRUE为忙
*****************************************************************************/
VOS_BOOL ATP_UTIL_VoiceHasUrgCall(VOS_VOID)
{
    FILE        *pstFile;
    VOS_UINT32  ulStatus;
    VOS_CHAR    acSrcData[VOICE_STATUS_LEN_MAX];

    memset(acSrcData, 0, VOICE_STATUS_LEN_MAX);
    pstFile = fopen(FILE_PATH_URGCALL_STATUS, "rb");
    if (VOS_NULL_PTR == pstFile)
    {
        return VOS_FALSE;
    }

    fread(acSrcData, 1, VOICE_STATUS_LEN_MAX, pstFile);
    fclose(pstFile);
    acSrcData[VOICE_STATUS_LEN_MAX - 1] = '\0';
    ulStatus = (VOS_UINT32)atoi(acSrcData);
    if (ulStatus > 0)
    {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}
#endif


