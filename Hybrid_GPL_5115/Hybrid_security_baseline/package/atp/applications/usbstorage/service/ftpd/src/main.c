/*

bftpd Copyright (C) 1999-2003 Max-Wilhelm Bruker

This program is is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2 of the
License as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <config.h>
#include <stdio.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_ASM_SOCKET_H
#include <asm/socket.h>
#endif
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <netdb.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_WAIT_H
# include <wait.h>
#else
# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif
#endif

#include<net/if.h>
#include<sys/ioctl.h>

#ifdef SUPPORT_ATP_FTPDS
#include "ssl.h"
#endif

#include "main.h"
#include "cwd.h"
#include "mystring.h"
#include "logging.h"
#include "dirlist.h"
#include "bftpdutmp.h"
#include "options.h"
#include "login.h"
#include "list.h"
//#include "atp_version.h"

#ifdef SUPPORT_ATP_MIC_INETD
#include "atpmiclisten.h"
#endif

#include "bftpd_message.h"
#include "msgapi.h"

#ifdef SUPPORT_LOGIN_CONTROL
#include "loginctl.h"
#include "atplog.h"
extern pid_t  g_LoginPid;
unsigned char g_ucLoginCtlEnable = 0;
#endif

int global_argc;
char **global_argv;
char **my_argv_list;   // jesse
struct sockaddr_in name;
int isparent = 1;
int listensocket = -1, sock;

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
int listensocket_implicit = -1;
SSL_CTX *  pstctx = NULL;
ssl_context *  pstsslcommandobj = NULL;
ssl_context *  pstssldataobj = NULL;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
unsigned int  g_epsv = 0;

#ifdef SUPPORT_ATP_IPV6
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
int listensocket6_implicit = -1;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
int listensocket6 = -1;
struct sockaddr_in6 remotename6;
struct sockaddr_in6 name6;
int socktype = AF_INET;
#endif

FILE *passwdfile = NULL, *groupfile = NULL, *devnull;

struct sockaddr_in remotename;
char *remotehostname;
int control_timeout, data_timeout;
int alarm_type = 0;
int ftpdflag = 0;

struct bftpd_list_element *child_list;

/* Command line parameters */
char *configpath = PATH_BFTPD_CONF;
int daemonmode = 0;

/*add by l63336 是否支持内存中创建目录*/
int iCheckMemCreate = 0;
/*add by l63336 是否支持内存中创建目录*/

#ifdef SUPPORT_ATP_MIC_INETD
static unsigned int iFtpdMicListenFds[4] = { 0, 0, 0, 0 };
void static ATP_MIC_Close_Ftpd_Readable_Connection()
{
    ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection( iFtpdMicListenFds, ( sizeof( iFtpdMicListenFds ) / sizeof(int) ) );
}
#endif

#ifdef SUPPORT_ATP_FTP_CONNECTION_NUM //add by y00181549 for ftp最大连接数的控制宏
int g_iMaxLinkNum = SUPPORT_ATP_FTP_CONNECTION_NUM;
#else
int g_iMaxLinkNum = 15;
#endif
#if 0
#ifndef SUPPORT_ACL
#define SUPPORT_ACL
#endif
#endif
/*****************************************************/
#ifdef SUPPORT_ATP_FTPDS
void atp_ftpd_ssl_context_free_obj( ssl_context ** sslContext );
#endif

void print_file(int number, char *filename)
{
	FILE *phile;
	char foo[MAX_STRING_LENGTH];
	phile = fopen(filename, "r");
	if (phile) {
		while (fgets(foo, MAX_STRING_LENGTH, phile)) {
			foo[strlen(foo) - 1] = '\0';
			control_printf(SL_SUCCESS, "%i-%s", number, foo);
		}
		fclose(phile);
	}
}

int lock_whole_file_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len)
{
	struct flock lock;
	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;
	return(fcntl(fd,cmd,&lock));
}

void process_bftpd_user_pid_add_del(char *user,int userpid,int type)
{
    bftpd_userpid_st stUserPid;
    memset(&stUserPid, 0, sizeof(stUserPid));

    stUserPid.lpid = userpid;
    stUserPid.type = type;
    snprintf(stUserPid.acUserName,sizeof(stUserPid.acUserName),"%s",user);
    ATP_MSG_Init(NULL);
    ATP_MSG_SendMsgData("cms", ATP_MSG_CMS_MODIFYBFTPDUSERPID_FILE, (void *)(&stUserPid), sizeof(stUserPid));
}

void end_child()
{
#ifdef SUPPORT_LOGIN_CONTROL
    char acAddr[64] = {0};
#endif
	if (passwdfile)
		fclose(passwdfile);
	if (groupfile)
		fclose(groupfile);
	config_end();
	bftpd_log("Quitting.\n");
    bftpd_statuslog(1, 0, "quit");
    #if 0
	process_bftpd_user_pid_del(getpid());
    #else
    process_bftpd_user_pid_add_del("",getpid(),0);
    #endif
    bftpdutmp_end();
	log_end();
	login_end();
	bftpd_cwd_end();

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    atp_ftpd_ssl_context_free_obj( &pstsslcommandobj );    /* 子进程退出时，保证监听ssl 的socket正常关闭。 */ 
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

#ifdef SUPPORT_LOGIN_CONTROL  //added by qichenghong 2009.12.04
    if(remotename.sin_addr.s_addr)
    {
        snprintf(acAddr,sizeof(acAddr),"%s",inet_ntoa(remotename.sin_addr));
    }
    else
    {
        inet_ntop(AF_INET6, &remotename6.sin6_addr, acAddr, sizeof(acAddr));
    }
        if((g_LoginPid == getpid()) && (1 == g_ucLoginCtlEnable))//added by qichenghong 2009.12.04
        {
            ATP_LoginCtlLogout(user ,acAddr,ATP_LOGINTYPE_FTP);//added by qichenghong 2009.12.03
            /*add by y00181549 fot W724v log at 20120718*/
           ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_USER_LOGOUT_INFO,user,acAddr);
        }
#endif
	if (daemonmode) {
		close(sock);
		close(0);
		close(1);
		close(2);
	}
    //printf("\n child quit");
	
}



/*
This function causes the program to 
re-read parts of the config file.

-- Jesse
*/
void handler_sighup(int sig)
{
   bftpd_log("Caught HUP signal. Re-reading config file.\n");
   Reread_Config_File();
   signal(sig, handler_sighup);
}


int IsAccessFromLan(struct sockaddr_in *ipaddr)
{
    struct ifreq ifr1, ifr2;
    int lSockfd;
    struct sockaddr_in *addr;
    struct sockaddr_in *mask;

    if (ipaddr == NULL)
    {
        return TRUE;
    }


#ifdef SUPPORT_ATP_IPV6
    if (!ipaddr->sin_addr.s_addr)
    {
        /*add by hkf39743 2011-4-20 start */
       FILE* pstFile = NULL;
       char acLine[256] = {0};
       char acFinalAddr[64] = {0};
       char acTempAddr[64] = {0};
       char acIfc[64] = {0};
       char ulLen;
       unsigned int  i, j;
       struct in6_addr stAddr; 
       pstFile = fopen("/proc/net/if_inet6", "r"); //从这个文件中取得br0 lan 侧地址
       if (pstFile == NULL)
       {
           return -1;
       }
       while (fgets(acLine, 256, pstFile)) 
       {
     	    acIfc[0] = '\0';
            sscanf(acLine, "%s %s %s %s %s %s\n", acTempAddr, acIfc, acIfc, acIfc, acIfc, acIfc);
            if (0 == strcmp(acIfc, "br0"))    //匹配读出lan 侧地址
            {
                ulLen = strlen(acTempAddr);
                for (i = 0, j = 0; i < ulLen; i++, j++)
                {
                    if ((i != 0)
                        && (i % 4 == 0))
                    {
                        acFinalAddr[j] = ':';
                        j++;
                    }
                    acFinalAddr[j] = acTempAddr[i];
                }
                 memset(&stAddr, 0, sizeof(struct in6_addr)); //将地址转换成ipv6地址格式
                if (inet_pton(AF_INET6, acFinalAddr, &stAddr) > 0)
                {
                    if ( memcmp( &remotename6.sin6_addr, &stAddr, 8) == 0) //与登录地址比较，相等说明是lan侧登录
                    {
                        fclose(pstFile);
                        return TRUE;
                    }
                }
             }
         }
	fclose(pstFile);
	return FALSE;
     }
    /*add by hkf39743 2011-4-21 end */ 
#endif

    lSockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (lSockfd < 0)
    {
        perror("socket");
        return (TRUE);
    }

    
    //printf("Now, Coming IsAccessFromLan\n");

    memset(&ifr1, 0, sizeof(struct ifreq));
	/*Added by lvxin00135113@20110425 安全代码整改*/
	#if 0
    strcpy(ifr1.ifr_name, "br0");
    memset(&ifr2, 0, sizeof(struct ifreq));
    strcpy(ifr2.ifr_name, "br0");
	#else
    snprintf(ifr1.ifr_name, sizeof(ifr1.ifr_name), "%s","br0");
    memset(&ifr2, 0, sizeof(struct ifreq));

    snprintf(ifr2.ifr_name, sizeof(ifr2.ifr_name), "%s","br0");
	#endif
	/*Added by lvxin00135113@20110425 安全代码整改*/
    
    if (ioctl(lSockfd, SIOCGIFADDR, &ifr1) < 0)
    {
        perror("ioctl");
        close(lSockfd);
        return (TRUE);
    }
    addr = (struct sockaddr_in *)&ifr1.ifr_addr;    
    
    if (ioctl(lSockfd, SIOCGIFNETMASK, &ifr2) < 0)
    {
        perror("ioctl");
        close(lSockfd);
        return (TRUE);
    }
    mask = (struct sockaddr_in *)&ifr2.ifr_netmask;

    if (((addr->sin_addr.s_addr) & (mask->sin_addr.s_addr)) == ((ipaddr->sin_addr.s_addr) & (mask->sin_addr.s_addr)))
    {
        //printf("host address: %s",  inet_ntoa(addr->sin_addr));
        //printf("  mask: %s\n", inet_ntoa(mask->sin_addr));
        //printf("local ip: %s\n", inet_ntoa(ipaddr->sin_addr));

        close(lSockfd);
        return TRUE;
    }
    else
    {
        //printf("host address: %s",  inet_ntoa(addr->sin_addr));
        //printf("  mask: %s\n", inet_ntoa(mask->sin_addr));
        //printf("remote address: %s\n", inet_ntoa(ipaddr->sin_addr));
        close(lSockfd);
        return FALSE;
    }
        
}

#if 0
typedef struct sockaddr_in IPADDR_ST;
typedef struct in_addr IN_ADDR;
int FTP_AclFunc(IPADDR_ST *pstAddr)
{

    IN_ADDR stIp;
    char *pcIP;
    unsigned int ulRemoteIP;
    FILE *fp;
    int bHttpLanEn;
    int bHttpWanEn;
    int bFtpLanEn;
    int bFtpWanEn;
    int bTelnetLanEn;
    int bTelnetWanEn;
    int bSshLanEn;
    int bSshWanEn;
    
    //printf("begin to do FTP ACL_func\n");
    ulRemoteIP = pstAddr->sin_addr.s_addr;
    stIp.s_addr = ulRemoteIP;
    pcIP = inet_ntoa(stIp);
    //printf("pcip = %s\n", pcIP);
    
    if (NULL == (fp = fopen("/var/AclServices.conf", "r")))
    {
        //printf("open AclServices.conf fail!\n");
        return TRUE;//没有ACL，则保持正常行为
    }
    
    fscanf(fp, "%d %d %d %d %d %d %d %d", &bHttpLanEn, &bHttpWanEn, &bFtpLanEn, &bFtpWanEn, &bTelnetLanEn, &bTelnetWanEn, &bSshLanEn, &bSshWanEn);
    fclose(fp);
    
    if (TRUE == IsAccessFromLan(pstAddr))
    {
        if (TRUE == bFtpLanEn)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if (TRUE == bFtpWanEn)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

#endif




void handler_sigchld(int sig)
{
	pid_t pid;
	int i;
	struct bftpd_childpid *childpid; 

        /* Get the child's return code so that the zombie dies */
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        //printf("wait return pid %d.\n", pid);
    	for (i = 0; i < bftpd_list_count(child_list); i++) {
    		childpid = bftpd_list_get(child_list, i);
            //printf("Check %dth child %x.\n", i, childpid);
    		if ( (childpid) && (childpid->pid == pid) ) {
                //printf("Found child with sock %d.\n", childpid->sock);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
                atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
    			close(childpid->sock);
    			bftpd_list_del(&child_list, i);
    			free(childpid);
                            /* make sure the child is removed from the log */
                            bftpdutmp_remove_pid(pid);
							
    		}
    	}
    }
    signal(SIGCHLD, handler_sigchld);
}

void handler_sigterm(int signum)
{
        bftpdutmp_end();
	exit(0);	/* Force normal termination so that end_child() is called */
}

void handler_sigalrm(int signum)
{
    /* Log user out. -- Jesse <slicer69@hotmail.com> */
    bftpdutmp_end();

    if (alarm_type)
    {
        close(alarm_type);
        bftpd_log("Kicked from the server due to data connection timeout.\n");
        control_printf(SL_FAILURE, "421 Kicked from the server due to data connection timeout.");
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        atp_ftpd_ssl_context_free_obj( &pstssldataobj );
        atp_ftpd_ssl_context_free_obj( &pstsslcommandobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
        exit(0);
    }
    else
    {
        bftpd_log("Kicked from the server due to control connection timeout.\n");
        control_printf(SL_FAILURE, "421 Kicked from the server due to control connection timeout.");
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        atp_ftpd_ssl_context_free_obj( &pstssldataobj );
        atp_ftpd_ssl_context_free_obj( &pstsslcommandobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
        exit(0);
    }
}

void init_everything()
{
	if (!daemonmode) {
        hidegroups_init();
    }
	config_init();
	log_init();
    bftpdutmp_init();
	login_init();
    
	
}
 
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS

/******************************************************************************
  函数名称  : ATP_SSLCreateCtx
  功能描述  : 创建SSL 上下文，并且导入证书
  输入参数  : char *buffer,   int len
             
  输出参数  :  
  返 回 值  : SSL * 指针
  修改历史     :
  1.日    期   : 2011-3-10
    作    者   : hkf39743
    修改内容   : 新创建函数
******************************************************************************/
SSL_CTX *ATP_SSLCreateCtx(
                            ATP_FTPDS_SSL_VERSION    enSslVersion,
                            ATP_FTPDS_SSL_MODE       enSslMode,
                            const VOS_CHAR          *pcCAPath,
                            const VOS_CHAR          *pcPubCertPath,
                            const VOS_CHAR          *pcKeyPath,
                            const VOS_CHAR          *pcPassPhrase,
                            VOS_BOOL                bIsServer)
{
    VOS_INT32       lVerifyMode;
    SSL_CTX         *pstRet;
#ifdef SUPPORT_ATP_CYASSL
    InitCyaSSL();
#endif

#ifdef SUPPORT_ATP_OPENSSL
    if (VOS_FALSE == m_bSslLibInited)
    {
        (VOS_VOID)SSL_library_init();
        m_bSslLibInited = VOS_TRUE;
    }
#endif

    /*
        *   SSL Version
        */
#ifdef SUPPORT_ATP_POLARSSL
    lVerifyMode = SSL_MINOR_VERSION_1;  // Default TLSv1.0
    if (ATP_FTPDS_SSL_V3 == enSslVersion)
    {
        lVerifyMode = SSL_MINOR_VERSION_0;
    }
    pstRet = SSL_CTX_new(lVerifyMode, (int)bIsServer);

#else
    pstRet = NULL;
    if ((ATP_FTPDS_SSL_ANY            == enSslVersion) ||
        (ATP_FTPDS_SSL_ANY_EXCLUDE_V2 == enSslVersion))
    {
    #ifdef SUPPORT_ATP_CYASSL
        // Cyassl donot support SSLv2, so default to TLSv1
        if (VOS_TRUE == bIsServer)
        {
            pstRet = SSL_CTX_new(TLSv1_server_method());
        }
        else
        {
            pstRet = SSL_CTX_new(TLSv1_client_method());
        }
    #endif

    #ifdef SUPPORT_ATP_OPENSSL
        pstRet = SSL_CTX_new(SSLv23_method());
    #endif
    }
    else if (ATP_FTPDS_SSL_V3 == enSslVersion)
    {
        if (VOS_TRUE == bIsServer)
        {
            pstRet = SSL_CTX_new(SSLv3_server_method());
        }
        else
        {
            pstRet = SSL_CTX_new(SSLv3_client_method());
        }
    }
    else if (ATP_FTPDS_SSL_TLS == enSslVersion)
    {
        if (VOS_TRUE == bIsServer)
        {
            pstRet = SSL_CTX_new(TLSv1_server_method());
        }
        else
        {
            pstRet = SSL_CTX_new(TLSv1_client_method());
        }
    }
#endif
    if (NULL == pstRet)
    {
        return NULL;
    }

      /*
        *   Load CA cert if necessary
        */
    lVerifyMode = SSL_VERIFY_NONE;
    if ((ATP_FTPDS_SSL_CERT_AUTH_PEER == enSslMode) ||
        (ATP_FTPDS_SSL_CERT_AUTH_BOTH == enSslMode))
    {
    #ifdef SUPPORT_ATP_POLARSSL
        lVerifyMode = SSL_VERIFY_OPTIONAL;
    #else
        lVerifyMode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
    #endif
    }
    if ((SSL_VERIFY_NONE != lVerifyMode) || (NULL != pcCAPath))
    {
        if ((NULL == pcCAPath) ||
            (1 != SSL_CTX_load_verify_locations(pstRet, pcCAPath, NULL)))
        {
            goto err_create_ctx;
        }
    }

      /*
        *   Load Public cert if necessary
        */
    if ((ATP_FTPDS_SSL_CERT_AUTH_BY_PEER == enSslMode) ||
        (ATP_FTPDS_SSL_CERT_AUTH_BOTH    == enSslMode))
    {
        // Check if the key file is encrypted
    #ifdef SUPPORT_ATP_CYASSL
        if (NULL != pcPassPhrase)
        {
            // Cyassl do not support encrypted key file
            fprintf(stderr, "Fatal: CYASSL do not support encrypted private key.\n");
            SSL_CTX_free(pstRet);
            return NULL;
        }
    #endif
    #ifdef SUPPORT_ATP_OPENSSL
        if (NULL != pcPassPhrase)
        {
            SSL_CTX_set_default_passwd_cb_userdata(pstRet, (void *)strdup(pcPassPhrase));
            SSL_CTX_set_default_passwd_cb(pstRet, passwd_cb);
        }
    #endif

        // Load server cert
    #ifdef SUPPORT_ATP_CYASSL
        if ((NULL == pcPubCertPath) ||
            (1 != SSL_CTX_use_certificate_file(pstRet, pcPubCertPath, SSL_FILETYPE_PEM)))
    #endif
    #ifdef SUPPORT_ATP_OPENSSL
        if ((NULL == pcPubCertPath) ||
            (1 != SSL_CTX_use_certificate_chain_file(pstRet, pcPubCertPath)))
    #endif
    #ifdef SUPPORT_ATP_POLARSSL
        if ((NULL == pcPubCertPath) || (NULL == (pstRet->server_crt = calloc(1, sizeof(x509_cert)))) ||
            (0 != x509parse_crtfile(pstRet->server_crt, pcPubCertPath)))
    #endif
        {
            goto err_create_ctx;
        }
        // Load server key file
    #ifdef SUPPORT_ATP_POLARSSL

        if ((NULL == pcKeyPath) || (NULL == (pstRet->server_key = calloc(1, sizeof(rsa_context)))) ||
            (0 != x509parse_keyfile(pstRet->server_key, pcKeyPath, pcPassPhrase)))
    #else
        if ((NULL == pcKeyPath) ||
            (1 != SSL_CTX_use_PrivateKey_file(pstRet, pcKeyPath, SSL_FILETYPE_PEM)))
    #endif
        {
            goto err_create_ctx;
        }
    }

  /*
     *  CPE must support the following cipher suites:
     *      1. RSA_WITH_3DES_EDE_CBC_SHA
     *      2. RSA_WITH_RC4_128_SHA
     */
	/* Start of 2008-12-06 by z65940 for 使用小型化的cyassl，没有这些接口 */
#ifdef SUPPORT_ATP_OPENSSL
    #if 0
    if (1 != SSL_CTX_set_cipher_list(pstModule->pstCtxObj,
                                          "RC4-SHA:EXP-DES-CBC-SHA"))
    #else
    if (1 != SSL_CTX_set_cipher_list(pstRet, "ALL"))
    #endif
    {
        goto err_create_ctx;
    }

    if ((ATP_FTPDS_SSL_ANY_EXCLUDE_V2 == enSslVersion))
    {
        SSL_CTX_set_options(pstRet, SSL_OP_ALL|SSL_OP_NO_SSLv2);
    }
#endif

#ifdef SUPPORT_ATP_CYASSL
    SSL_CTX_set_verify(pstRet, lVerifyMode, NULL);
#endif

#ifdef SUPPORT_ATP_OPENSSL
    SSL_CTX_set_verify(pstRet, lVerifyMode, SslVerifyFilter);
#endif

#ifdef SUPPORT_ATP_POLARSSL
    SSL_CTX_set_verify(pstRet, lVerifyMode);
    pstRet->ciphers = ssl_default_ciphers;
#endif

    return pstRet;
err_create_ctx:
    SSL_CTX_free(pstRet);
    return NULL;
}


void atp_ftpd_ssl_context_free_obj( ssl_context ** sslContext )
{
    if ( sslContext != NULL && *sslContext != NULL )
    {
        /*告诉client 数据发送完成关闭数据连接，如果不关，客户端会出错。*/
        ssl_close_notify(*sslContext);
        SSL_free( *sslContext );
        *sslContext = NULL;
    }
}

#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/


int atp_ftpd_read_command( char *pBuffer, int iSize )
{
    int iRet = -1;

    if( pBuffer != NULL && iSize > 0 )
    {
#ifdef SUPPORT_ATP_FTPDS
        if( pstsslcommandobj != NULL )
        {
            int iLength = SSL_read( pstsslcommandobj, (unsigned char*)pBuffer, ( iSize - 1 ) );

            if( iLength > 0 && iLength < iSize )
            {
                pBuffer[iLength] = '\0';
                iRet = 0;
            }
        }
        else
        {
            if ( fgets ( pBuffer, ( iSize - 1 ), stdin ) != NULL )
            {
                iRet = 0;
            }
        }
#else
        if ( fgets ( pBuffer, ( iSize - 1 ), stdin ) != NULL )
        {
            iRet = 0;
        }
#endif
    }

    return iRet;
}

#ifndef SUPPORT_ATP_FTP_UPGRADE

int ACLCloseAllDataPortForPassiveMode( )
{
    int iRet = -1;
    char acTmp[512] = "";

    snprintf( acTmp, sizeof( acTmp ), "iptables -t nat -F PRE_FTP" );
    ATP_UTIL_ForkProcess( acTmp, 0, NULL, NULL );
    
    snprintf( acTmp, sizeof( acTmp ), "iptables -t filter -F INPUT_FTP" );
    ATP_UTIL_ForkProcess( acTmp, 0, NULL, NULL );

    snprintf( acTmp, sizeof( acTmp ), "ip6tables -t filter -F INPUT_FTP" );
    ATP_UTIL_ForkProcess( acTmp, 0, NULL, NULL );

    iRet = 0;

    return iRet;
}
#endif
int main(int argc, char **argv)
{
	char str[MAX_STRING_LENGTH + 1];
	static struct hostent *he;
	int i = 1, port;
	int retval;
    unsigned long get_value;
    socklen_t my_length;
    char *pszCport=NULL; /*add by l63336 for config port*/
    char *temp_string = NULL;
    int iInetdFlag = 0;   //是否用MIC动态启动标志
    char * pTmp = NULL;
    int lSockValue = 0;
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
    unsigned int implicit_mode = 0;         //ftps implicit mode 标志位
#endif

#ifdef SUPPORT_ATP_MIC_INETD
   ATP_UTIL_MIC_ResetBackupAppFds( iFtpdMicListenFds, ( sizeof( iFtpdMicListenFds ) / sizeof(int) ) );
   atexit(ATP_MIC_Close_Ftpd_Readable_Connection);
#endif

#ifndef SUPPORT_ATP_FTP_UPGRADE

    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
    ACLCloseAllDataPortForPassiveMode( );
    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

    /*start: 用于查询组件版本号(ftpd atpv),请不要修改或删除 h00189019 2012-06-22*/
    if ((argc == 2) && (NULL != argv[1]) && (0 == strcmp(argv[1],ATP_VERSION_CMD_KEY)))
    {
       printf("\r\n%s.\n", MODULE_VERSION);    	    		
       exit(0);
    }
    /*end */        
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    /*建立SSL 上下文，并导入证书*/
    pstctx = ATP_SSLCreateCtx(ATP_FTPDS_SSL_ANY_EXCLUDE_V2, ATP_FTPDS_SSL_CERT_AUTH_BY_PEER, 
                                "/etc/root_ftp.pem", "/etc/servercert_ftp.pem", "/etc/serverkey_ftp.pem", 
                                NULL, VOS_TRUE);
    if ( NULL == pstctx )
    {
        printf("SSL creatctx faild \n");
    }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

    my_argv_list = argv;
    signal(SIGHUP, handler_sighup);

#ifdef SUPPORT_LOGIN_CONTROL
        while (((retval = getopt(argc, argv, "c:s:t:p:khdDinL"))) > -1)
#else
        while (((retval = getopt(argc, argv, "c:s:t:p:khdDin"))) > -1)
#endif

	{
		switch (retval) {
			case 'h':
				printf(
					"Usage: %s [-h] [-i|-d|-D] [-c <filename>|-n] [-p <portnum>] [-k]\n"
					"-h print this help\n"
					"-i (default) run from inetd\n"
					"-d daemon mode: fork() and run in TCP listen mode\n"
					"-D run in TCP listen mode, but don't pre-fork()\n"
					"-c read the config file named \"filename\" instead of " PATH_BFTPD_CONF "\n"
					"-n no config file, use defaults\n"
					"-p set the listened port of ftp server"
					"-k check directory.\n"
					"-l max children link number", argv[0]);
				return 0;
			case 'i': daemonmode = 0; break;
			case 'd': daemonmode = 1; break;
			case 'D': daemonmode = 2; break;
			case 'c': 
				  configpath = strdup(optarg);
				  break;
			case 'n': configpath = NULL; break;
			case 'k': 
				iCheckMemCreate = 1; /*是否支持内存中创建目录 */
				break;
			case 'p': 
				pszCport = strdup(optarg); 
				break;

/* 对于每个文件描述符，均使用s*t* 格式来传输参数，因此可以用临时变量传递参数值--y00183935 */
#if 0
			case 's': 
            #ifdef SUPPORT_ATP_IPV6
                if (-1 == listensocket)
                {
                    listensocket = atoi(optarg);  //支持通过mic动态启动.
                }
                else
                {
                    listensocket6 = atoi(optarg);  //支持通过mic动态启动.
                }
            #else
				listensocket = atoi(optarg);  //支持通过mic动态启动.
			#endif
                iInetdFlag = 1;
				break;
            case 't':
                break;
#else
			case 's':
				lSockValue = atoi(optarg);  //支持通过mic动态启动.
#ifdef SUPPORT_ATP_MIC_INETD
                if( lSockValue > 0 )
                {
                    ATP_UTIL_MIC_AddBackupAppFds( iFtpdMicListenFds, ( sizeof(iFtpdMicListenFds) / sizeof(int) ), lSockValue );
                }
#endif
                iInetdFlag = 1;
				break;
            case 't':   /* 通过t参数得到传进 来的套接字的类型 */
                switch ((VOS_UINT32)atoi(optarg))
                {
                     case ATP_SOCK_DFT:    //监听21端口ftp/ftpes 模式ipv4 套接字
                        listensocket = lSockValue;
                        break;
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
                     case ATP_SOCK_TCP4_SSL: //监听990 端口的ftps implicit 模式ipv4 套接字
                        listensocket_implicit =  lSockValue;
                        break;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_IPV6
                     case ATP_SOCK_IP_VER:  //监听21端口ftp/ftpes 模式ipv6套接字
                        listensocket6 = lSockValue;
                        break;
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
                     case ATP_SOCK_TCP6_SSL: //监听990端口ftps implicit 模式ipv6套接字
                        listensocket6_implicit = lSockValue;
                        break;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#endif
                     default:
                        break;
                }
                break;
#endif
/* 对于每个文件描述符，均使用s*t* 格式来传输参数，因此可以用临时变量传递参数值--y00183935 */
            /*start  l00163811 从命令参数传入最大子进程连接数*/
            case 'l':
                g_iMaxLinkNum = atoi(optarg);
                //printf("g_iMaxLinkNum is <%d>\n", g_iMaxLinkNum);
                break;
            /*end  l00163811 从命令参数传入最大子进程连接数*/
                    
#ifdef SUPPORT_LOGIN_CONTROL
			case 'L':
				if(ATP_LoginCtlInit(ATP_LOGINTYPE_FTP) == VOS_OK) //changed by qichenghong 20100204
				{
					g_ucLoginCtlEnable = 1;
				}
				break;
#endif				
		}		
	}

	if (daemonmode) 
	{
		struct sockaddr_in myaddr, new;
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
        struct sockaddr_in myaddr_implicit;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

#ifdef SUPPORT_ATP_IPV6
        struct sockaddr_in6 myaddr6;
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
        struct sockaddr_in6 myaddr6_implicit;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#endif

		if (daemonmode == 1) 
		{
			if (fork())
				exit(0);  /* Exit from parent process */
			setsid();
			if (fork())
				return 0;
		}
		signal(SIGCHLD, handler_sigchld);
		config_init();
		chdir("/");
                hidegroups_init();

        if (-1 == listensocket)  //判断是否通过MIC动态启动   
        {
            listensocket = socket(AF_INET, SOCK_STREAM, 0);
            if ( listensocket >= 0 )
            {
#ifdef SO_REUSEADDR
                setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, (void *) &i,
                sizeof(i));
#endif
#ifdef SO_REUSEPORT
                setsockopt(listensocket, SOL_SOCKET, SO_REUSEPORT, (void *) &i,
                sizeof(i));
#endif

                //revised by l65130 for set port, we read port no. from file "ftp.conf"
                memset((void *) &myaddr, 0, sizeof(myaddr));
                if (!((port = strtoul(config_getoption("PORT"), NULL, 10))))
                {
                    if(NULL != pszCport)
                    {
                        port = atoi(pszCport);
                    }
                    else
                    {
                        port = DEFAULT_PORT;
                    }
                }
                myaddr.sin_port = htons(port);
                if (!strcasecmp(config_getoption("BIND_TO_ADDR"), "any")
                    || !config_getoption("BIND_TO_ADDR")[0])
                {
                    myaddr.sin_addr.s_addr = INADDR_ANY;
                }
                else
                {
                    myaddr.sin_addr.s_addr = inet_addr(config_getoption("BIND_TO_ADDR"));
                }
                if (bind(listensocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0)
                {
                    fprintf(stderr, "Bind failed: %s\n", strerror(errno));
#if 0   /* 由于使用IPv6 和FTPDS 所以其中一个失败以后，不应该退出进程。--y00183935 */
                    exit(1);
#else
                    close(listensocket);
                    listensocket = -1;
#endif
                }
                if (listen(listensocket, 5))
                {
                    fprintf(stderr, "Listen failed: %s\n", strerror(errno));
#if 0   /* 由于使用IPv6 和FTPDS 所以其中一个失败以后，不应该退出进程。--y00183935 */
                    exit(1);
#else
                    close(listensocket);
                    listensocket = -1;
#endif
                }
            }
            else
            {
                fprintf(stderr, "Creat socket failed: %s\n", strerror(errno));
                listensocket = -1;
            }
        }

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
        if (-1 == listensocket_implicit)  //判断是否通过MIC动态启动   
        {
            listensocket_implicit= socket(AF_INET, SOCK_STREAM, 0);
            if( listensocket_implicit >= 0 )
            {
#ifdef SO_REUSEADDR
                setsockopt(listensocket_implicit, SOL_SOCKET, SO_REUSEADDR, (void *) &i,
                sizeof(i));
#endif
#ifdef SO_REUSEPORT
                setsockopt(listensocket_implicit, SOL_SOCKET, SO_REUSEPORT, (void *) &i,
                sizeof(i));
#endif
                //revised by l65130 for set port, we read port no. from file "ftp.conf"
                memset((void *) &myaddr_implicit, 0, sizeof(myaddr_implicit));
                if (!((port = strtoul(config_getoption("FTPDSPORT"), NULL, 10))))
                {
                    if(NULL != pszCport)
                    {
                        port = atoi(pszCport);
                    }
                    else
                    {
                        port = FTPDS_DEFAULT_PORT;
                    }
                }
                myaddr_implicit.sin_port = htons(port);
                if (!strcasecmp(config_getoption("BIND_TO_ADDR"), "any")
                    || !config_getoption("BIND_TO_ADDR")[0])
                {
                    myaddr_implicit.sin_addr.s_addr = INADDR_ANY;
                }
                else
                {
                    myaddr_implicit.sin_addr.s_addr = inet_addr(config_getoption("BIND_TO_ADDR"));
                }
                if (bind(listensocket_implicit, (struct sockaddr *) &myaddr_implicit, sizeof(myaddr_implicit)) < 0)
                {
                    fprintf(stderr, "Bind failed: %s\n", strerror(errno));
                    close(listensocket_implicit);
                    listensocket_implicit = -1;
                }
                if (listen(listensocket_implicit, 5))
                {
                    fprintf(stderr, "Listen failed: %s\n", strerror(errno));
                    close(listensocket_implicit);
                    listensocket_implicit = -1;
                }
            }
            else
            {
                fprintf(stderr, "Creat socket failed: %s\n", strerror(errno));
                listensocket_implicit = -1;
            }
        }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

#ifdef SUPPORT_ATP_IPV6
         if (-1 == listensocket6)  //判断是否通过MIC动态启动   
         {
             listensocket6 = socket(AF_INET6, SOCK_STREAM, 0);
             if (listensocket6 >= 0)
             {
                 setsockopt(listensocket6, SOL_SOCKET, SO_REUSEADDR, (void *) &i, sizeof(i));
                 setsockopt(listensocket6, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &i, sizeof(i));
                       
                //revised by l65130 for set port, we read port no. from file "ftp.conf"
                 memset((void *) &myaddr6, 0, sizeof(myaddr6));
                 if (!((port = strtoul(config_getoption("PORT"), NULL, 10))))
                 {
                     if(NULL != pszCport)
                     {
                        port = atoi(pszCport);
                     }
                     else
                     {
                        port = DEFAULT_PORT;
                     }
                 }
                 myaddr6.sin6_family = AF_INET6;
                 myaddr6.sin6_port = htons(port);
                 myaddr6.sin6_addr = in6addr_any;
                 if (bind(listensocket6, (struct sockaddr *) &myaddr6, sizeof(myaddr6)) < 0) 
                 {
                     fprintf(stderr, "Bind failed: %s\n", strerror(errno));
                     close(listensocket6);
                     listensocket6 = -1;
                     //exit(1);
                 }
                 else if (listen(listensocket6, 5))
                 {
                     fprintf(stderr, "Listen failed: %s\n", strerror(errno));
                     close(listensocket6);
                     listensocket6 = -1;
                     //exit(1);
                 }
             }
             else
             {
                fprintf(stderr, "Creat socket failed: %s\n", strerror(errno));
                listensocket6 = -1;
                //exit(1);
             }
         }

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
         if (-1 == listensocket6_implicit)  //判断是否通过MIC动态启动   
         {
             listensocket6_implicit= socket(AF_INET6, SOCK_STREAM, 0);
             if (listensocket6_implicit >= 0)
             { 
                 setsockopt(listensocket6_implicit, SOL_SOCKET, SO_REUSEADDR, (void *) &i, sizeof(i));
                 setsockopt(listensocket6_implicit, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &i, sizeof(i));
 
                 //revised by l65130 for set port, we read port no. from file "ftp.conf"
                 memset((void *) &myaddr6_implicit, 0, sizeof(myaddr6_implicit));
                 if (!((port = strtoul(config_getoption("FTPDSPORT"), NULL, 10))))
                 {
                     if(NULL != pszCport)
                     {
                         port = atoi(pszCport);
                     }
                     else
                     {
                         port = FTPDS_DEFAULT_PORT;
                     }
                 }
                 myaddr6_implicit.sin6_family = AF_INET6;
                 myaddr6_implicit.sin6_port = htons(port);
                 myaddr6_implicit.sin6_addr = in6addr_any;
                 if (bind(listensocket6_implicit, (struct sockaddr *) &myaddr6_implicit, sizeof(myaddr6_implicit)) < 0) 
                 {
                     fprintf(stderr, "Bind failed: %s\n", strerror(errno));
                     close(listensocket6_implicit);
                     listensocket6_implicit= -1;
                 }
                 else if (listen(listensocket6_implicit, 5))
                 {
                     fprintf(stderr, "Listen failed: %s\n", strerror(errno));
                     close(listensocket6_implicit);
                     listensocket6_implicit = -1;
                 }
             }
             else
             {
                 fprintf(stderr, "Creat socket failed: %s\n", strerror(errno));
                 listensocket6_implicit= -1;
             }
         }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

#endif

        /* check for open stdin, stdout, stderr */
    #if 1
        if (listensocket >= 3)
        {
            for (i = 0; i < 3; i++)
            {
                close(i);		/* Remove fd pointing to the console */
			    open("/dev/null", O_RDWR);	/* Create fd pointing nowhere */
		    }
        }
    #endif
        
		my_length = sizeof(new);
		while (1)
        {
			pid_t pid;
			fd_set rdfdset;
			int selret;
            int maxFd = 0;
            struct timeval  stTimeVal;

            /* 重置参数--y00183935 */
            g_epsv = 0;

#ifdef SUPPORT_ATP_IPV6
            socktype = AF_INET;
#endif

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
            implicit_mode = 0;          /* 表示是正常ftp模式*/
            pstsslcommandobj = NULL;    /* 每次在while循环中创建的对象，将会在子进程中释放，因此，可以在这里直接重置父进程中的变量。 */
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

            if (1 == iInetdFlag) //y42304 added: 用mic动态启动
            {
                FD_ZERO(&rdfdset);
        		if ( listensocket >= 0 )
        		{
                    FD_SET(listensocket, &rdfdset);
                    maxFd = listensocket;
        		}

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
                if (listensocket_implicit >= 0 )
                {
                    FD_SET(listensocket_implicit, &rdfdset);
                    maxFd = (maxFd < listensocket_implicit) ? listensocket_implicit : maxFd;
                }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

#ifdef SUPPORT_ATP_IPV6
                if (listensocket6 >= 0)
                {
                    FD_SET(listensocket6, &rdfdset);
                    maxFd = (maxFd < listensocket6) ? listensocket6 : maxFd;
                }

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
        		if (listensocket6_implicit >= 0 )
                {
                    FD_SET(listensocket6_implicit, &rdfdset);
                    maxFd = (maxFd < listensocket6_implicit) ? listensocket6_implicit : maxFd;
        		}
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#endif

#ifndef SUPPORT_ATP_FTP_UPGRADE

                stTimeVal.tv_sec  = 60;
#else
                stTimeVal.tv_sec  = 60*3;
#endif
                stTimeVal.tv_usec = 0;
        		selret = select(maxFd + 1, &rdfdset, NULL, NULL, &stTimeVal);
        		if (selret < 0)
        		{
        			//printf("Select error: %d %d.\n", selret, errno);
        			continue;
        		}
                else if (0 == selret)
                {
                    if (bftpd_list_count(child_list) > 0)
                    {
                        continue;
                    }
                    //printf("bftpd timeout.\n");

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS    /* 在退出ftpd 进程以前，一定要释放ftpds的动态空间内容，否则内存泄露，尽管进程退出时，动态空间会释放，最好手动处理。--y00183935 */
                    if ( pstctx != NULL )
                    {
                        SSL_CTX_free ( pstctx );
                    }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifndef SUPPORT_ATP_FTP_UPGRADE

                    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
                    ACLCloseAllDataPortForPassiveMode( );
                    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
                    exit(0);
                }
            }

#if 0
#ifdef SUPPORT_ATP_IPV6
            if (FD_ISSET(listensocket, &rdfdset))
            {
                sock = accept(listensocket, (struct sockaddr *)&new, &my_length);
                socktype = AF_INET;
            }
            else if ((listensocket6 >= 0) && (FD_ISSET(listensocket6, &rdfdset)))
            {
                sock = accept(listensocket6, (struct sockaddr *)&new, &my_length);
                socktype = AF_INET6;
            }
#else
			sock = accept(listensocket, (struct sockaddr *) &new, &my_length);
#endif
#else

            if (listensocket >= 0 && FD_ISSET(listensocket, &rdfdset))
            {
                sock = accept(listensocket, (struct sockaddr *)&new, &my_length);
            }
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
            else if (listensocket_implicit >= 0 && FD_ISSET(listensocket_implicit, &rdfdset))
            {
                sock = accept(listensocket_implicit, (struct sockaddr *)&new, &my_length);
                implicit_mode = 1;
            }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_IPV6
            else if ((listensocket6 >= 0) && (FD_ISSET(listensocket6, &rdfdset)))
            {
                sock = accept(listensocket6, (struct sockaddr *)&new, &my_length);
                socktype = AF_INET6;
                /*add 2011-3-30 for epsv commands by hkf39743 start */
                g_epsv = 1; //g_epsv = 1时表示控制连接使用的是ipv6地址为0表示使用的是ipv4地址
                /*add 2011-3-30 end*/
            }
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
            else if ((listensocket6_implicit >= 0) && (FD_ISSET(listensocket6_implicit,  &rdfdset)))
            {
                sock = accept(listensocket6_implicit, (struct sockaddr *)&new, &my_length);
                socktype = AF_INET6;
                /*add 2011-3-30 for epsv commands by hkf39743 start */
                g_epsv = 1; //g_epsv = 1时表示控制连接使用的是ipv6地址为0表示使用的是ipv4地址
                /*add 2011-3-30 end*/
                implicit_mode = 1;
            }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#endif

#endif

#ifdef SUPPORT_ATP_MIC_INETD
            ATP_UTIL_MIC_ResetBackupAppFds( iFtpdMicListenFds, ( sizeof( iFtpdMicListenFds ) / sizeof(int) ) );
#endif

			/* If accept() becomes interrupted by SIGCHLD, it will return -1.
			 * So in order not to create a child process when that happens,
			 * we have to check if accept() returned an error.
			 */
			if (sock > 0)
            {
			    /*start of HG_Protocol 2007.06.29 HG522V100R001C01B011 A36D03658 */
                retval = bftpd_list_count(child_list);
                //printf("Current count %d.\n", retval);
                if (retval >= g_iMaxLinkNum)
                {
                    //printf("Only allow %d ftp session.\n",g_iMaxLinkNum);
                    close(sock);
                    continue;
                }
                /*end of HG_Protocol 2007.01.08 HG522V100R001C01B011 A36D03658 */

#ifdef SUPPORT_ACL
#if 0
                // add by d00107688 to debug
                //printf("\nbegin to do FTP ACL\n");
                // end add by d00107688 to debug
                if (FALSE == FTP_AclFunc(&new))
                {
                    close(sock);
                    continue;
                }
                //printf("after FTP ACL\n");
#endif
#endif

#if defined (SUPPORT_ATP_FTPDS) && defined (SUPPORT_ATP_FTPDS_IMPLICIT) //add by y00181549 for 粗隔离出隐式ftps功能 at 2012.6.9
                if ( 1 == implicit_mode )       /* 客户端使用ftpds 隐式规则连接。 */
                {
                    int iImplicitFlag = UNSECURED;

                    if ( pstctx != NULL && NULL == pstsslcommandobj )
                    {
                        pstsslcommandobj = SSL_new( pstctx );      /* 对于每一个accept 的tcp 连接，都要一个ssl_new 对象去处理，处理完以后，在子进程中释放。 */

                        if ( pstsslcommandobj != NULL )
                        {
                            SSL_set_fd( pstsslcommandobj, sock );

                            if ( SSL_accept ( sock, pstctx, pstsslcommandobj ) >= 0 )
                            {
                                iImplicitFlag = SECURED;
                            }
                        }
                    }

                    if( UNSECURED == iImplicitFlag )
                    {
                        if ( pstsslcommandobj != NULL )
                        {
                            SSL_free( pstsslcommandobj );
                            pstsslcommandobj = NULL;
                        }

                        close( sock );

                        continue;
                    }
                }
#endif

				pid = fork();
				if (!pid)
                {       /* child */
					close(0);
					close(1);
					close(2);
					isparent = 0;
					dup2(sock, fileno(stdin));
					dup2(sock, fileno(stderr));
					setctlsocket(sock);
					break;
				}
                else
                {          /* parent */
					struct bftpd_childpid *tmp_pid = malloc(sizeof(struct bftpd_childpid));
					tmp_pid->pid = pid;
					tmp_pid->sock = sock;
					bftpd_list_add(&child_list, tmp_pid);
                    //printf("Add a client with pid %d sock %d.\n", pid, sock);
				}
			}
		}
	}
        
        /* Child only. From here on... */

	devnull = fopen("/dev/null", "w");
	global_argc = argc;
	global_argv = argv;
	init_everything();
	atexit(end_child);
	signal(SIGTERM, handler_sigterm);
    	signal(SIGALRM, handler_sigalrm);

    ATP_BFTPD_CHILD_Init( );


    /* If we do not have getpwnam() for some reason, then
       we must use FILE_AUTH or exit. */
    #ifdef NO_GETPWNAM
    {
        char *file_auth_option;

        file_auth_option = config_getoption("FILE_AUTH");
        if (! file_auth_option[0] )
        {
           bftpd_log("Exiting, becasue we have no way to authorize clients.\n");
           exit(0);
        }
    }
    #endif

    get_value = strtoul(config_getoption("CONTROL_TIMEOUT"), NULL, 0);
    if (get_value <= INT_MAX)
       control_timeout = get_value;
    else
       control_timeout = 0;
    if (!control_timeout)
        control_timeout = CONTROL_TIMEOUT;

    get_value = strtoul(config_getoption("DATA_TIMEOUT"), NULL, 0);
    if (get_value <= INT_MAX)
       data_timeout = get_value;
    else
       data_timeout = 0;
    if (!data_timeout)
        data_timeout = DATA_TIMEOUT;
    get_value = strtoul(config_getoption("XFER_BUFSIZE"), NULL, 0);
    if (get_value <= INT_MAX)
       xfer_bufsize = get_value;
    else
       xfer_bufsize = 9;

    /* handled below
    if (!xfer_bufsize)
        xfer_bufsize = XFER_BUFSIZE;
    */
    /* do range check on xfer_bufsize, which is an int */
    if ( (xfer_bufsize <= 0) || (xfer_bufsize > 1000000) )
       xfer_bufsize = XFER_BUFSIZE;

    /* get scripts to run pre and post write */
    pre_write_script = config_getoption("PRE_WRITE_SCRIPT");
    if (! pre_write_script[0])
       pre_write_script = NULL;
    post_write_script = config_getoption("POST_WRITE_SCRIPT");
    if (! post_write_script[0])
       post_write_script = NULL;

#ifdef SUPPORT_ATP_IPV6
    if (socktype == AF_INET6)
    {
        char tmpbuff[64];
        
    	my_length = sizeof(remotename6);
        if (getpeername(fileno(stderr), (struct sockaddr *) &remotename6, &my_length)) {
    		control_printf(SL_FAILURE, "421-Could not get peer IP address.\r\n421 %s.",
    		               strerror(errno));
    		return 0;
    	}
    	i = 1;
    	setsockopt(fileno(stdin), SOL_SOCKET, SO_OOBINLINE, (void *) &i,
    			   sizeof(i));
    	setsockopt(fileno(stdin), SOL_SOCKET, SO_KEEPALIVE, (void *) &i,
    			   sizeof(i));
    	/* If option is set, determine the client FQDN */
    	if (!strcasecmp((char *) config_getoption("RESOLVE_CLIENT_IP"), "yes")) {
    		if ((he = gethostbyaddr((char *) &remotename6.sin6_addr, sizeof(struct in_addr), AF_INET6)))
            {
               if (he->h_name)
		            remotehostname = strdup(he->h_name);
               else
                    remotehostname = "";
            }
    		else
            {
                temp_string =(char*)inet_ntop(AF_INET6, (const void*)&(remotename6.sin6_addr), tmpbuff, INET6_ADDRSTRLEN);
                if (temp_string)
                   remotehostname = strdup(temp_string);
                else
                   remotehostname = "";
		        /* remotehostname = strdup(inet_ntoa(remotename.sin_addr)); */
            }
    	}
        else
        {
              temp_string = (char*)inet_ntop(AF_INET6, &remotename6.sin6_addr, tmpbuff, INET6_ADDRSTRLEN);
              if (temp_string)
                 remotehostname = strdup(temp_string);
              else
                 remotehostname = "";
		      /* remotehostname = strdup(inet_ntoa(remotename.sin_addr)); */
        }
    	bftpd_log("Incoming connection from %s.\n", remotehostname);
        bftpd_statuslog(1, 0, "connect %s", remotehostname); 
        
            
        my_length = sizeof(name6);
        getsockname(fileno(stdin), (struct sockaddr *) &name6, &my_length);
        print_file(220, config_getoption("MOTD_GLOBAL"));
        /* Parse hello message */
        memset(str,0x00,sizeof(str));
		/*Added by lvxin00135113@20110425 安全代码整改*/
		#if 0
        strcpy(str, (char *) config_getoption("HELLO_STRING"));
		#else
        snprintf(str, sizeof(str), "%s",(char *) config_getoption("HELLO_STRING"));        
		#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
        //replace(str, "%v", VERSION, MAX_STRING_LENGTH);
        if (strstr(str, "%h")) {
            if ((he = gethostbyaddr((char *) &name6.sin6_addr, sizeof(struct in6_addr), AF_INET6)))
                replace(str, "%h", he->h_name, MAX_STRING_LENGTH);
            else
                replace(str, "%h", (char *) inet_ntop(AF_INET6, &name6.sin6_addr, tmpbuff, INET6_ADDRSTRLEN), MAX_STRING_LENGTH);
        }
        replace(str, "%i", (char *) inet_ntop(AF_INET6, &name6.sin6_addr, tmpbuff, INET6_ADDRSTRLEN), MAX_STRING_LENGTH);
        control_printf(SL_SUCCESS, "220 %s", str);
    }
    else
#endif
    {
	my_length = sizeof(remotename);
    if (getpeername(fileno(stderr), (struct sockaddr *) &remotename, &my_length)) {
		control_printf(SL_FAILURE, "421-Could not get peer IP address.\r\n421 %s.",
		               strerror(errno));
		return 0;
	}
	i = 1;
	setsockopt(fileno(stdin), SOL_SOCKET, SO_OOBINLINE, (void *) &i,
			   sizeof(i));
	setsockopt(fileno(stdin), SOL_SOCKET, SO_KEEPALIVE, (void *) &i,
			   sizeof(i));
	/* If option is set, determine the client FQDN */
	if (!strcasecmp((char *) config_getoption("RESOLVE_CLIENT_IP"), "yes")) {
		if ((he = gethostbyaddr((char *) &remotename.sin_addr, sizeof(struct in_addr), AF_INET)))
                {
                   if (he->h_name)
			remotehostname = strdup(he->h_name);
                   else
                        remotehostname = "";
                }
		else
                {
                        temp_string = inet_ntoa(remotename.sin_addr);
                        if (temp_string)
                           remotehostname = strdup(temp_string);
                        else
                           remotehostname = "";
			/* remotehostname = strdup(inet_ntoa(remotename.sin_addr)); */
                }
	}
        else
        {
              temp_string = inet_ntoa(remotename.sin_addr);
              if (temp_string)
                 remotehostname = strdup(temp_string);
              else
                 remotehostname = "";
		/* remotehostname = strdup(inet_ntoa(remotename.sin_addr)); */
        }
	bftpd_log("Incoming connection from %s.\n", remotehostname);
       bftpd_statuslog(1, 0, "connect %s", remotehostname);
	my_length = sizeof(name);
	getsockname(fileno(stdin), (struct sockaddr *) &name, &my_length);
	print_file(220, config_getoption("MOTD_GLOBAL"));
	/* Parse hello message */
	memset(str,0x00,sizeof(str));
	/*Added by lvxin00135113@20110425 安全代码整改*/
	#if 0
	strcpy(str, (char *) config_getoption("HELLO_STRING"));
	#else
    snprintf(str, sizeof(str), "%s",(char *) config_getoption("HELLO_STRING"));            
	#endif
	/*Added by lvxin00135113@20110425 安全代码整改*/
	//replace(str, "%v", VERSION, MAX_STRING_LENGTH);
	if (strstr(str, "%h")) {
		if ((he = gethostbyaddr((char *) &name.sin_addr, sizeof(struct in_addr), AF_INET)))
			replace(str, "%h", he->h_name, MAX_STRING_LENGTH);
		else
			replace(str, "%h", (char *) inet_ntoa(name.sin_addr), MAX_STRING_LENGTH);
	}
	replace(str, "%i", (char *) inet_ntoa(name.sin_addr), MAX_STRING_LENGTH);
	control_printf(SL_SUCCESS, "220 %s", str);
    }	

        /* We might not get any data, so let's set an alarm before the
           first read. -- Jesse <slicer69@hotmail.com> */
        alarm(control_timeout);
        

#if 0
    /* Read lines from client and execute appropriate commands */
    while (fgets(str, MAXCMD, stdin)) {
        alarm(control_timeout);
        str[strlen(str) - 2] = 0;
#else
	/* Read lines from client and execute appropriate commands */
    while( atp_ftpd_read_command( str, sizeof( str ) ) >= 0 )
    {
        alarm(control_timeout);

        if( ( pTmp = strstr( str, "\r\n" ) ) != NULL )
        {
            *pTmp = '\0';
        }

        if( strlen( str ) <= 0 )        /* 防止读取的内容被处理\r\n 时清空了。 */
        {
            continue;
        }
#endif

        bftpd_statuslog(2, 0, "%s\r\n", str);
#ifdef DEBUG
		bftpd_log("Processing command: %s\n", str);
#endif

		parsecmd(str);
		if(NULL != user && 0 == ftpdflag)
		{
			if(0 != strcasecmp(user,"anonymous"))
			{
                #if 0
				process_bftpd_user_pid_add(user,getpid());
                #else
                process_bftpd_user_pid_add_del(user,getpid(),1);
                #endif
				ftpdflag = 1;
			}
		}
	}

	usleep(10); // We should wait for parent to add me to the list
        if (remotehostname)
        {
            free(remotehostname);
            remotehostname = NULL;
        }
	return 0;
}





