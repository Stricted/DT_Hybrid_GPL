/******************************************************************************

                  版权所有 (C), 2009-2011, 华为技术有限公司

  文 件 名   : loginctl.c
  版 本 号   : V1.0
  作    者   : qichenghong KF19818
  生成日期   : 2009年12月2日
  最近修改   :
  功能描述   : 公共的登录控制模块
  函数列表   :
  
  修改历史   :
    日    期   : 2009年12月2日
    作    者   : qichenghong KF19818
    修改内容   : 创建文件

******************************************************************************/
/*include 头文件*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>
#include <errno.h>
#include <mntent.h>
#include "atputil.h"
#include "loginctl.h"

#include "msgapi.h"

#ifdef SUPPORT_ATP_USBSTORAGE

/******************************************************************************
  函数名称  : ATP_LoginCtlInit
  功能描述  : 初始化登录控制模块, 初始化全局变量及创建登录模块所用的目录
  输入参数  : ulLogType:登录控制的进程类型
  
  调用函数  : 
  被调函数  : 提供给外部调用
  输出参数  : 无
  返 回 值  : VOS_OK成功
  			     VOS_ERROR失败
  说    明  : 无
  
  修改历史     :
  1.日    期   : 2009年12月2日
    作    者   : qichenghong KF19818
    修改内容   : 新生成函数
  2.日    期   : 2010年2月3日
    作    者   : qichenghong KF19818
    修改内容   : 修改成只做消息初始化
******************************************************************************/
VOS_UINT32 ATP_LoginCtlInit(VOS_UINT32 ulLogType)
{
    VOS_UINT32 ulRet = VOS_OK;

    if(ATP_LOGINTYPE_FTP == ulLogType)
    {
        ulRet = ATP_MSG_Init(ATP_CBBID_BFTPD_NAME);
    }
    else
    {
        if(ATP_LOGINTYPE_SAMBA == ulLogType)
        {
            ulRet = ATP_MSG_Init(ATP_CBBID_SMBD_NAME);
        }
    }

    return ulRet;
}

/******************************************************************************
  函数名称  : ATP_LoginCtlCheck
  功能描述  : 根据从FTP/SMB 输入的用户登录信息来判断该用户目前是否可合法登录,
                           并将相应信息写入登录文件LOGIN_FILE 中
  输入参数  : pszUsername : 用户名
                           pszIpAddr : 登录终端的IP 地址
                           pszLogType : 登录方式, FTP/SMB
                           ulLogStatus : 登录状态, 成功LOGIN_OK / 失败LOGIN_FAIL
  调用函数  : 
  被调函数  : 提供给外部调用
  输出参数  : 无
  返 回 值  :    ATP_LOGINRET_OK        	  //表示该用户可继续登录
    				 ATP_LOGINRET_LOGINED      //表示该用户已经登录过
     				 ATP_LOGINRET_FAIL_EXD    //表示当前用户登录失败次数已经超过限制次数
     				 ATP_LOGINRET_EXISTED,            //表示同一个用户同一个终端已经登录过一次
     				 ATP_LOGINRET_INNORMAL   //表示不正常返回
  			     
  说    明  : 无
  
  修改历史     :
  1.日    期   : 2009年12月2日
    作    者   : qichenghong KF19818
    修改内容   : 新生成函数
******************************************************************************/
ATP_LOGINRET_EN ATP_LoginCtlCheck(const VOS_CHAR *pszUsername ,const VOS_CHAR *pszIpAddr ,VOS_UINT32 ulLogType , 
                                                                VOS_UINT ulLogStatus)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32  ulRet = VOS_OK;

    LoginDebugPrint("Begin check login status...\n");

    if((VOS_NULL_PTR == pszUsername) && (VOS_NULL_PTR == pszIpAddr))
    {
        LoginDebugPrint("The pointer parameter is NULL\n");
        return ATP_LOGINRET_INNORMAL;
    }
    
    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %s %u %u" , pszUsername , pszIpAddr , ulLogType , ulLogStatus);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_CHECK , 0);
    pstResp = NULL;
     /*modify by y00181549 for 消息字符串的结束符at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return ATP_LOGINRET_INNORMAL;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    ulRet = (ATP_LOGINRET_EN)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);

    return (ATP_LOGINRET_EN)ulRet;
}

/******************************************************************************
  函数名称  : ATP_LoginCtlLogout
  功能描述  : 用户登出, 如果用户已经登入则登出时清除用户相关的所有信息
  输入参数  : pszUsername : 用户名
                           ulLogintype : 用户登录的方式FTP/SMB

  调用函数  : 
  被调函数  : 提供给外部调用
  输出参数  : 无
  返 回 值  : VOS_OK成功
  			     VOS_ERROR失败
  说    明  : 无
  
  修改历史     :
  1.日    期   : 2009年12月3日
    作    者   : qichenghong KF19818
    修改内容   : 新生成函数
******************************************************************************/
VOS_INT32 ATP_LoginCtlLogout(const VOS_CHAR *pszUsername , const VOS_CHAR *pszAddrInfo,VOS_UINT32 ulLogintype)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32   ulRet = VOS_OK;
    VOS_INT32   lRet = VOS_OK;

    LoginDebugPrint("Begin check login status...\n");

    if(VOS_NULL_PTR == pszUsername) 
    {
        LoginDebugPrint("The pointer parameter is NULL\n");
        return VOS_ERROR;
    }
    
    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %s %u" , pszUsername, pszAddrInfo,ulLogintype);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_LOGOUT , 0);
    pstResp = NULL;
     /*modify by y00181549 for 消息字符串的结束符at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return VOS_ERROR;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    lRet = (VOS_INT32)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);

    return lRet;
}

/******************************************************************************
  函数名称  : ATP_LoginCtlAllowWanAcc
  功能描述  : 判断一个指定用户是否可以通过WAN侧访问,主要用到配置文件/var/ftp/ftpwanacc
  输入参数  : pszUsername : 用户名
                           ulLogintype : 登录方式,目前未用

  调用函数  : 
  被调函数  : 提供给外部调用
  输出参数  : 无
  返 回 值  : VOS_OK 该用户可以通过WAN侧访问
  			     VOS_ERROR 出现异常,如文件/var/ftp/ftpwanacc 不存在
  			     VOS_NOK 该用户不能通过WAN侧访问
  说    明  : 无
  
  修改历史     :
  1.日    期   : 2009年12月3日
    作    者   : qichenghong KF19818
    修改内容   : 新生成函数
  2.日    期   : 2009年12月7日
    作    者   : qichenghong KF19818
    修改内容   : 添加函数体
******************************************************************************/
VOS_INT32 ATP_LoginCtlAllowWanAcc(const VOS_CHAR *pszUsername , VOS_UINT32 ulLogintype)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32  ulRet = VOS_OK;
    VOS_INT32   lRet = VOS_OK;

    LoginDebugPrint("Begin check login status...\n");

    if(VOS_NULL_PTR == pszUsername) 
    {
        LoginDebugPrint("The pointer parameter is NULL\n");
        return VOS_ERROR;
    }
    
    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %u" , pszUsername , ulLogintype);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_ALLOWWAN , 0);
    pstResp = NULL;
     /*modify by y00181549 for 消息字符串的结束符at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return VOS_ERROR;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    lRet = (VOS_INT32)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);

    return lRet;
}
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL //y00181549 for 根据ftp类型细分权限控制 at 2012.6.10
/******************************************************************************
  函数名称  : ATP_LoginCtlCheckFtpAcc
  功能描述  : 根据FTP 类型和用名和类型的判断此连接是否可以建立
  输入参数  :
                          

  调用函数  : 
  被调函数  : 提供给外部调用
  输出参数  : 无
  返 回 值  : VOS_OK 该连接可以建立
  			     VOS_NOK该连接不可以建立
  说    明  : 无
  
  修改历史     :
  1.日    期   : 2012.6.10
    作    者   : y00181549
    修改内容   : 新生成函数
******************************************************************************/
VOS_INT32 ATP_LoginCtlCheckFtpAcc(const VOS_CHAR *pszUsername, VOS_UINT32 ulFtpType,VOS_UINT32 ulFromType)
{
    ATP_MSG_HEADER_ST   * pstMsg = VOS_NULL;
    ATP_MSG_HEADER_ST   * pstResp = VOS_NULL;
    VOS_UINT8   aucBuf[sizeof(ATP_MSG_HEADER_ST) + ATP_ACTUAL_LEN_512] = {0};
    VOS_CHAR    *pszTmp =  VOS_NULL;
    VOS_UINT32  ulRet = VOS_OK;
    VOS_INT32   lRet = VOS_OK;

    pszTmp = (VOS_CHAR *)(aucBuf + sizeof(ATP_MSG_HEADER_ST));
    snprintf(pszTmp , ATP_ACTUAL_LEN_512 - 1 , "%s %u %u" , pszUsername, ulFtpType,ulFromType);
    LoginDebugPrint("pszTmp=%s\n",pszTmp);
    pstMsg = (ATP_MSG_HEADER_ST *)aucBuf;
    ATP_MSG_SimpleBuilder(pstMsg , ATP_CBBID_CMS_NAME, ATP_MSG_CMS_LOGINCTL_ALLOWFTPTYPE , 0);
    pstResp = NULL;
    /*modify by y00181549 for 消息字符串的结束符at 2012.06.12*/
    pstMsg->ulDataLen = strlen(pszTmp)+1;
    LoginDebugPrint("ulDataLen=%d\n",pstMsg->ulDataLen);
    ulRet = ATP_MSG_SendAndRecv(pstMsg , &pstResp , 2*1000);
    if(VOS_OK != ulRet)
    {
        LoginDebugPrint("ATP_MSG_SendAndRecv failed, ulRet:%x\n", ulRet);
        return VOS_ERROR;
    }
    LoginDebugPrint("Ret=%u\n",pstResp->ulMsgData);
    lRet = (VOS_INT32)(pstResp->ulMsgData);
    free((VOS_VOID *)pstResp);
    //printf("---------------->%d\r\n",ulRet);
    return lRet;
}
#endif
#endif

/*****************************************************************************
 函 数 名  : ATP_UTIL_IsPathVfat
 功能描述  :判断传入路径文件系统类型是否为VFAT
 输入参数  : 
 输出参数  : 
 返 回 值     : 
                       0: 不在FAT文件系统中
                       1: 在FAT文件系统中
              
 调用函数  :无
 被调函数  : 
 说    明  ： 
 
 修改历史      :
  1.日    期   : 2010年4月8日
    作    者   : lvxin 00135113
    修改内容   : 新生成函数

*****************************************************************************/
VOS_INT32   ATP_UTIL_IsPathVfat(const char *pcPath)
{
    FILE                *fstab;
    struct mntent       mtEnt;
    VOS_CHAR        acBuf[1024];

    if (NULL == pcPath)
    {
        return 0;
    }

    fstab = setmntent("/proc/mounts", "r");
    if (NULL == fstab)
    {
        return 0;
    }

    memset((void *)(&(mtEnt)), 0, sizeof(mtEnt));
    memset((void *)acBuf, 0, sizeof(acBuf));
    while (NULL != getmntent_r(fstab, &mtEnt, acBuf, sizeof(acBuf)))
    {
    #if 0
        printf("================================\n");
        printf("fsname: [%s].\n", mtEnt.mnt_fsname);
        printf("dir   : [%s].\n", mtEnt.mnt_dir);
        printf("type  : [%s].\n", mtEnt.mnt_type);
        printf("opts  : [%s].\n", mtEnt.mnt_opts);
    #endif
        if ((NULL != mtEnt.mnt_type) && (0 != strcmp(mtEnt.mnt_type, "vfat")))
        {
            continue;
        }

        if ((NULL != mtEnt.mnt_dir) &&
            (0 == memcmp((void *)(mtEnt.mnt_dir), (void *)pcPath, strlen(mtEnt.mnt_dir))) &&
            (('/' == pcPath[strlen(mtEnt.mnt_dir)]) || ('\0' == pcPath[strlen(mtEnt.mnt_dir)])))
        {
            endmntent(fstab);
            return 1;
        }

        memset((void *)(&(mtEnt)), 0, sizeof(mtEnt));
        memset((void *)acBuf, 0, sizeof(acBuf));
    }
    endmntent(fstab);

    return 0;
}



