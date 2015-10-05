/*****************************************************************************
 *               Copyright (C) 2006, Huawei Tech. Co., Ltd.               
 *                           ALL RIGHTS RESERVED                               
 * FileName: xml_interface.h                                                          
 * Version: 1.0                                                                
 * Description:  xml interface      
 *                                                                             
 * History:                                                                    
 * 1. 2006-12-25,z42422 Create this file.                                  
******************************************************************************/

#ifndef __ATP_LGPLUTIL_H__
#define __ATP_LGPLUTIL_H__

#include "atptypes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define ATP_SYS_DOMAIN_LEN                  64
/* BEGIN: Added by z67728, 2010/3/1 for DNS SRV */
/*dns_srv support*/
typedef struct TagATP_DNS_SRV_ANSWER_ST
{
    unsigned int ulGetFlag; 
    char acQuestion[ATP_SYS_DOMAIN_LEN];  /*64字节*/
    unsigned short usType;
    unsigned short usClass;
    unsigned int ulTtl; 
    unsigned short usLen; 
    unsigned short usPriority;
    unsigned short usWeight;
    unsigned short usPort;
    char acTarget[ATP_SYS_DOMAIN_LEN]; 
    //ip地址非特殊说明使用网络序存储
    //unsigned int ulTargetAddr;
    char acIpAddress[ATP_SYS_DOMAIN_LEN];
}ATP_DNS_SRV_ANSWER_ST;

//extern ATP_DNS_SRV_ANSWER_ST *ATP_UTIL_DnsSrvQuery(char* pszDomain, char* pszinterface);
//extern ATP_DNS_SRV_ANSWER_ST *ATP_UTIL_DnsSrvQueryEx(char* pszDomain, char* pszinterface, int bProxy);

extern struct sockaddr_storage * ATP_DNS_SRV_Query(const char * pszDomain, int af);

/* END:   Added by z67728, 2010/3/1 */

/*Start of 维护管理组 2008-3-28 10:2 for 实现外部绑定DNS特性 by handy*/
struct hostent *atp_gethostbyname(const char *name, unsigned int localAddr);
/*End of 维护管理组 2008-3-28 10:2 for 实现外部绑定DNS特性 by handy*/
struct hostent *ATP_UTIL_Gethostbyname(const char *name, int family, char* bindaddr);

struct addrinfo *ATP_UTIL_getaddrinfo(const char *name, int family, unsigned int port, char *bindaddr);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __ATP_LGPLUTIL_H__ */

