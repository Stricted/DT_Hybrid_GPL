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
    char acQuestion[ATP_SYS_DOMAIN_LEN];  /*64�ֽ�*/
    unsigned short usType;
    unsigned short usClass;
    unsigned int ulTtl; 
    unsigned short usLen; 
    unsigned short usPriority;
    unsigned short usWeight;
    unsigned short usPort;
    char acTarget[ATP_SYS_DOMAIN_LEN]; 
    //ip��ַ������˵��ʹ��������洢
    //unsigned int ulTargetAddr;
    char acIpAddress[ATP_SYS_DOMAIN_LEN];
}ATP_DNS_SRV_ANSWER_ST;

//extern ATP_DNS_SRV_ANSWER_ST *ATP_UTIL_DnsSrvQuery(char* pszDomain, char* pszinterface);
//extern ATP_DNS_SRV_ANSWER_ST *ATP_UTIL_DnsSrvQueryEx(char* pszDomain, char* pszinterface, int bProxy);

extern struct sockaddr_storage * ATP_DNS_SRV_Query(const char * pszDomain, int af);

/* END:   Added by z67728, 2010/3/1 */

/*Start of ά�������� 2008-3-28 10:2 for ʵ���ⲿ��DNS���� by handy*/
struct hostent *atp_gethostbyname(const char *name, unsigned int localAddr);
/*End of ά�������� 2008-3-28 10:2 for ʵ���ⲿ��DNS���� by handy*/
struct hostent *ATP_UTIL_Gethostbyname(const char *name, int family, char* bindaddr);

struct addrinfo *ATP_UTIL_getaddrinfo(const char *name, int family, unsigned int port, char *bindaddr);

/*!
  \brief ͬʱ��ѯA��AAAA�ĺ���
        ��������struct hostent**��
        ��ѯ��Aʱ��*ppstV4Result��ΪNULL������ΪNULL
        ��ѯ��AAAAʱ��*ppstV6Result��ΪNULL������ΪNULL
  \param[in]  *pcName: Ҫ��ѯ������
  \param[out]  **ppstV4Result: A��ѯ�ķ���ֵ
  \param[out]  **ppstV6Result: AAAA��ѯ�ķ���ֵ
  \return ʧ��ʱ����ֵС��0���ɹ����ڵ���0
*/
int ATP_UTIL_Lookup_A_and_AAAA(const char *pcName, struct hostent **ppstV4Result, struct hostent **ppstV6Result);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __ATP_LGPLUTIL_H__ */

