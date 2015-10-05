#include "config.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/* added for FreeBSD compatibility */
#include <limits.h>
#include <signal.h>
#include <memory.h>
#include <string.h>
#include <mntent.h>
#include <net/if.h>
#include "msgapi.h"

#include <sys/statvfs.h>

//#define __USE_GNU
#include <unistd.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_ASM_SOCKET_H
#include <asm/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#include <fcntl.h>
#include <string.h>
#ifdef HAVE_WAIT_H
# include <wait.h>
#else
# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif
#endif
#ifdef HAVE_SYS_SENDFILE_H
# include <sys/sendfile.h>
#endif


#include "mystring.h"
#include "login.h"
#include "logging.h"
#include "dirlist.h"
#include "options.h"
#include "main.h"
#include "targzip.h"
#include "cwd.h"
#include "bftpdutmp.h"
#include "openssl/ssl.h"
//#include "md5.h"

#ifdef HAVE_ZLIB_H
#include <zlib.h>
#else
#undef WANT_GZIP
#endif

#ifdef SUPPORT_LOGIN_CONTROL
#include "loginctl.h"

int isAccFromWan = FALSE;
int ulisFtpsExCtl = FALSE; 
int is_proted = 0;
int isAccFromlan = FALSE;
extern unsigned char g_ucLoginCtlEnable;
#endif

#ifdef SUPPORT_ATP_FTP_UPGRADE
#define MAX_IMAGE_SIZE (ATP_FTP_UPGRADE_MAX_IMAGESIZE*1024*1024)
#endif

#ifdef SUPPORT_ATP_NAS
typedef struct ATP_NAS_SEND_EMAIL_NOTIFY_MSG_ST
{
      ATP_MSG_HEADER_ST stMsgHead;
      VOS_CHAR  acLoginFailReason[128];
}ATP_NAS_SEND_EMAIL_NOTIFY_MSG_ST;
#endif

#ifndef SUPPORT_ATP_FTP_UPGRADE
#include "bftpd_message.h"
int ATPDealACLDataPortForPassiveMode( int iSocketFd, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_TYPE_EN enPortOperate );
#endif
#include "atplog.h"
int state = STATE_CONNECTED;
char user[USERLEN + 1];
struct sockaddr_in sa;
#ifdef SUPPORT_ATP_IPV6
struct sockaddr_in6 sa6;
int eaf = 1;
#endif
char pasv = 0;
int sock;
int ctlsock = 0;
int pasvsock;
char *philename = NULL;
unsigned long offset = 0;
short int xfertype = TYPE_BINARY;
int ratio_send = 1, ratio_recv = 1;
/* long unsigned bytes_sent = 0, bytes_recvd = 0; */
double bytes_sent = 0.0, bytes_recvd = 0.0;
int epsvall = 0;
int xfer_bufsize;
//extern int g_utc_offset;

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
unsigned int ssl_data_protect = UNSECURED;
extern SSL_CTX *  pstctx;
extern ssl_context *  pstsslcommandobj;
extern ssl_context *  pstssldataobj;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
extern unsigned int  g_epsv;

#ifdef SUPPORT_ATP_FTPDS
extern void atp_ftpd_ssl_context_free_obj( ssl_context ** sslContext );
#endif

extern int IsAccessFromLan(struct sockaddr_in *ipaddr);
extern int atp_ftpd_read_command( char *pBuffer, int iSize );

#if 0
#define USB_STORAGE_TAG   "usb"
static int IsUSBDirectory(const char *pcDir)
{
    const char      *pcFirstUsb;
    const char      *pc3rdSlash;
    char            pcRealpath[1024];
    char            *pEnddir;
        
    if (NULL == pcDir)
    {
        return 0;
    }
    
    snprintf(pcRealpath,sizeof(pcRealpath),"%s%s",userinfo.pw_dir,pcDir);
    pEnddir = strrchr(pcRealpath,'/');
    pEnddir++;
    *pEnddir = '\0';
        
    // 找出第一个包括usb的字符串
    pcFirstUsb = strstr(pcRealpath, USB_STORAGE_TAG);
    if (NULL == pcFirstUsb)  // 不包括usb
    {
        return 0;
    }

    pc3rdSlash = pcRealpath + 1;
    // 找出第二个/
    pc3rdSlash = strchr(pc3rdSlash, '/');
    if (NULL != pc3rdSlash)
    {
        // 找出第三个/
		pc3rdSlash++;
        pc3rdSlash = strchr(pc3rdSlash, '/');
        if (NULL == pc3rdSlash)
        {
            return 0;
        }
        if (pcFirstUsb < pc3rdSlash)
        {
            return 1;
        }
    }
    return 0;
}
#endif
/*过滤掉文件名中前后的空格，传入参数为全路径，仅仅处理最后的文件名部分。*/
static void filtrateBlankCharaterInFilename( char * pcFullPath )
{
    if( pcFullPath != NULL && strlen( pcFullPath ) > 0 )
    {
        char * pTmp = NULL;
        if( ( pTmp = strrchr( pcFullPath, '/' ) ) != NULL )
        {
            pTmp++;
        }
        else
        {
            pTmp = pcFullPath;
        }

        if( strlen( pTmp ) > 0 )
        {
            char * qTmp = pTmp;
            while ((*qTmp) && ((*qTmp == ' ') || (*qTmp == '\t')))
            {
                qTmp++;
            }
            memmove(pTmp, qTmp, strlen(pTmp) - (qTmp - pTmp) + 1);

            if( strlen( pTmp ) > 0 )
            {
                qTmp = pTmp + strlen(pTmp) - 1;
                while( qTmp > pTmp && ((*qTmp == ' ') || (*qTmp == '\t')))
                {
                    *qTmp = '\0';
                    qTmp--;
                }
            }
        }
    }
}


static int IsValidFilename(const char *pcMapped, const char *pcFilename, const unsigned int limit)
{
    static const char *pcInvalidChar="\\/:*?\"<>|";    
    char   *p;
    char   *q;
    char   *pcNopathFilename = (char*)pcFilename;

    /*Added by yehuisheng00183935@20110922 修改问题单(DTS2011083103592)*/
#if 0
    if (NULL == pcMapped || NULL == pcFilename || limit < 1)
    {
        return 0;
    }
#else
    if ( NULL == pcMapped || 0 == strlen( pcMapped ) || NULL == pcFilename || 0 == strlen( pcFilename ) || limit < 1 )
    {
        return -2;
    }
#endif
    /*Added by yehuisheng00183935@20110922 修改问题单(DTS2011083103592)*/
        
    if(strlen(pcMapped) + strlen(userinfo.pw_dir) > limit)
    {        
        return 0;
    }

    if(!strcmp(pcMapped,pcFilename))
    {
        pcNopathFilename = strrchr(pcFilename,'/');
        pcNopathFilename++;

        /*Added by yehuisheng00183935@20110920 修改问题单(DTS2011082906100)*/
        if( '\0' == *pcNopathFilename )
        {
            return -1;
        }
        /*Added by yehuisheng00183935@20110920 修改问题单(DTS2011082906100)*/
    }

    for(p = (char*)pcInvalidChar; *p != '\0'; p++)
    {
        for(q = (char*)pcNopathFilename; *q != '\0'; q++)
        {
            if(*p == *q)
            {                
                return -1;
            }
        }
    }
    
    return 1;
}


int checkdiskfree(char *pathfilename)
{
    int iRet = -1;

    if( pathfilename != NULL && strlen( pathfilename ) > 0 )
    {
        int pathbufsize = strlen( pathfilename ) + 1;

        char *pathname = malloc( pathbufsize );

        if( pathname != NULL )
        {
            char * path = NULL;

            snprintf( pathname, pathbufsize, "%s", pathfilename );

            if( strstr( pathname, "/" ) != NULL )
            {
                if( *( pathname + strlen( pathname ) -1 ) != '/' )
                {
                    char * pTmp = strrchr( pathname, '/' );

                    if( pTmp != NULL )
                    {
                        *(pTmp + 1) = '\0';
                    }
                }
                path = pathname;
            }
            else        /* Use the current working directory. */
            {
                path = bftpd_cwd_getcwd();
                free(pathname);
                pathname = NULL;
            }

            if( path != NULL )
            {
                struct statvfs statbuf;

                memset( &statbuf, 0, sizeof( statbuf ) );

                if( statvfs(path , &statbuf) >= 0 )
                {
                    if( statbuf.f_bavail > 0 && statbuf.f_blocks > 0 )
                    {
                        iRet = 0;
                        bftpd_statuslog( 1 , 1, "free avail =%ld,bfree=%ld,blocks=%ld,bsize=%ld\n", statbuf.f_bavail, statbuf.f_bfree, statbuf.f_blocks, statbuf.f_bsize );
                    }
                    else
                    {
                        bftpd_statuslog( 1 , 1, "Can't write outside the disk dirctory: %s\n", path );
                    }
                }
                else
                {
                    bftpd_statuslog( 1 , 1, "checkdiskfree failed to statvfs.\n" );
                }

                free(path);
                path = NULL;
            }
        }
        else
        {
            bftpd_statuslog(1 , 1, "checkdiskfree malloc failed.\n" );
        }
    }
    else
    {
        bftpd_statuslog(1 , 1, "checkdiskfree error parameters.\n" );
    }

    return iRet;
}


/* start of HG556b_protocal : Added by c65985, 2009.7.21  IBM笔记本无法连接网关FTP问题 HG556bV100R001C01B010 */
void control_printf(char success, char *format, ...)
{
    char buffer[256];//,buffer1[256];
    char output_buffer[256+32];
    va_list val;
    va_start(val, format);
    memset(buffer, 0x00, sizeof(buffer));
    vsnprintf(buffer, sizeof(buffer), format, val);
    va_end(val);

    memset(output_buffer, 0x00, sizeof(output_buffer));
    strncpy(output_buffer,buffer,sizeof(buffer)-1);
    strncat(output_buffer, "\r\n", sizeof(output_buffer) -strlen(output_buffer) - 1);

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
#if 0
    int ret = 0;
    if  ( pstsslcommandobj != NULL )
    {
        ret = SSL_write( pstsslcommandobj, output_buffer, strlen(output_buffer) );
        if (ret < 0)
        {
            ssl_close_notify(pstsslcommandobj);
            SSL_free(pstsslcommandobj);
            pstsslcommandobj = NULL;
            close(ctlsock);
        }
    }
    else
    {
        fprintf(stderr, "%s", output_buffer);
    }
	
   // fprintf(stderr, "%s", output_buffer);
#else
    if ( pstsslcommandobj != NULL )
    {
        SSL_write( pstsslcommandobj, (const unsigned char*)output_buffer, strlen(output_buffer) );
    }
    else
    {
        fprintf(stderr, "%s", output_buffer);
    }
#endif
#else
    fprintf(stderr, "%s", output_buffer);
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

    replace(buffer, "\r", "", MAX_STRING_LENGTH);
    bftpd_statuslog(3, success, "%s", buffer);
}
/* end of HG556b_protocal : Added by c65985, 2009.7.21  IBM笔记本无法连接网关FTP问题 HG556bV100R001C01B010 */

void new_umask()
{
    int um;
    unsigned long get_um;
    char *foo = config_getoption("UMASK");
    if (!foo[0])
        um = 022;
    else
    {
        get_um = strtoul(foo, NULL, 8);
        if (get_um <= INT_MAX)
           um = get_um;
        else
        {
           bftpd_log("Error with umask value. Setting to 022.\n", 0);
           um = 022;
        }
    }
    umask(um);
}

/*w45260 2010-03-29  start :增加wan侧读写速度*/

#include <netinet/in.h>   
#include  <netinet/tcp.h>   //   for   TCP_NODELAY   
#include <sys/types.h>
#include <sys/socket.h>

void prepare_sock(int sock)
{
    int on = 1;
	int retval = 0;
#ifdef TCP_NODELAY
	retval = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *) &on, sizeof(on));
#endif
#ifdef TCP_NOPUSH
	retval = setsockopt(sock, IPPROTO_TCP, TCP_NOPUSH, (void *) &on, sizeof(on));
#endif
#ifdef SO_REUSEADDR
	retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &on, sizeof(on));
#endif
#ifdef SO_REUSEPORT
	retval = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (void *) &on, sizeof(on));
#endif
#ifdef SO_SNDBUF
	on = 65536;   //降低大小, 解决内存分配不足问题.
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *) &on, sizeof(on));
#endif
	on = 65536;  //降低大小, 解决内存分配不足问题.
	retval = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *) &on, sizeof(on));
	if (0 != retval)
	{
        bftpd_log("prepare_sock: %s\n", retval);
	}
}

/*w45260 2010-03-29  end:增加wan侧读写速度*/

int dataconn()
{
    struct sockaddr foo;
    struct sockaddr_in local;
#ifdef SUPPORT_ATP_IPV6
    struct sockaddr_in6 local6;
#endif
    socklen_t namelen = sizeof(foo);
    int curuid = geteuid();

    memset(&foo, 0, sizeof(foo));
    memset(&local, 0, sizeof(local));

    if (pasv)
    {
		sock = accept(pasvsock, (struct sockaddr *) &foo, (socklen_t *) &namelen);
		if (sock == -1)
        {
            control_printf(SL_FAILURE, "425-Unable to accept data connection.\r\n425 %s.",
                     strerror(errno));
			return 1;
		}

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        if ( ssl_data_protect == SECURED )
        {
            int iDataProtect = UNSECURED;

            if( pstssldataobj != NULL )         /* data obj 还没有被释放，出错了。 */
            {
                bftpd_log( "Something error: pstssldataobj is freed by new connection. \r\n" );
                atp_ftpd_ssl_context_free_obj( &pstssldataobj );
            }

            if( pstssldataobj == NULL )
            {
                if ( ( pstssldataobj = SSL_new(pstctx) ) != NULL )
                {
                    SSL_set_fd( pstssldataobj, sock );

                    if ( SSL_accept( sock, pstctx, pstssldataobj ) >= 0 )
                    {
                        iDataProtect = SECURED;
                    }
                }
            }

            if( UNSECURED == iDataProtect )
            {
                if( pstssldataobj != NULL )
                {
                    SSL_free(pstssldataobj);
                    pstssldataobj = NULL;
                }

                close(sock);
                return 1;
            }
        }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifndef SUPPORT_ATP_FTP_UPGRADE
        /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
        ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
        /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
        close(pasvsock);
        prepare_sock(sock);
	}
    else
    {
#ifdef SUPPORT_ATP_IPV6
	    if (eaf == 2)
	    {
            sock = socket(AF_INET6, SOCK_STREAM, 0);
            prepare_sock(sock);
            memcpy(local6.sin6_addr.s6_addr, name6.sin6_addr.s6_addr, 16);
            local6.sin6_family = AF_INET6;
            if (!strcasecmp(config_getoption("DATAPORT20"), "yes"))
            {
                seteuid(0);
                local6.sin6_port = htons(20);
            }

            /*Added by yehuisheng00183935@20110916 修改主动模式IPv6链路地址访问问题(DTS2011083100649)--使用链路地址时，必须指定参数"sin6_scope_id"*/
            if (IN6_IS_ADDR_LINKLOCAL(&(local6.sin6_addr)))
            {
                local6.sin6_scope_id = if_nametoindex("br0");
            }
            /*Added by yehuisheng00183935@20110916 修改主动模式IPv6链路地址访问问题(DTS2011083100649)--使用链路地址时，必须指定参数"sin6_scope_id"*/

            if (bind(sock, (struct sockaddr *) &local6, sizeof(local6)) < 0)
            {
                control_printf(SL_FAILURE, "425-Unable to bind data socket.\r\n425 %s.",
                        strerror(errno));
                return 1;
            }
            if (!strcasecmp(config_getoption("DATAPORT20"), "yes"))
            {
                seteuid(curuid);
            }

            sa6.sin6_family = AF_INET6;
            if (connect(sock, (struct sockaddr *) &sa6, sizeof(sa6)) == -1)
            {
                control_printf(SL_FAILURE, "425-Unable to establish data connection.\r\n"
                        "425 %s.", strerror(errno));
                return 1;
            }
	    }
        else
#endif
        {
    		sock = socket(AF_INET, SOCK_STREAM, 0);
            prepare_sock(sock);
    		local.sin_addr.s_addr = name.sin_addr.s_addr;
    		local.sin_family = AF_INET;
            if (!strcasecmp(config_getoption("DATAPORT20"), "yes"))
            {
                seteuid(0);
                local.sin_port = htons(20);
            }
    		if (bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0)
            {
    			control_printf(SL_FAILURE, "425-Unable to bind data socket.\r\n425 %s.",
    					strerror(errno));
    			return 1;
    		}
            if (!strcasecmp(config_getoption("DATAPORT20"), "yes"))
            {
                seteuid(curuid);
            }

    		sa.sin_family = AF_INET;
    		if (connect(sock, (struct sockaddr *) &sa, sizeof(sa)) == -1)
            {
    			control_printf(SL_FAILURE, "425-Unable to establish data connection.\r\n"
                        "425 %s.", strerror(errno));
    			return 1;
    		}
        }

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        if ( ssl_data_protect == SECURED )
        {
            int iDataProtect = UNSECURED;

            if( pstssldataobj != NULL )         /* data obj 还没有被释放，出错了。 */
            {
                bftpd_log( "Something error: pstssldataobj is freed by new connection. \r\n" );
                atp_ftpd_ssl_context_free_obj( &pstssldataobj );
            }

            if( pstssldataobj == NULL )
            {
                if ( ( pstssldataobj = SSL_new(pstctx) ) != NULL )
                {
                    SSL_set_fd( pstssldataobj, sock );

                    if ( SSL_connect( sock, pstctx, pstssldataobj ) >= 0 )
                    {
                        iDataProtect = SECURED;
                    }
                }
            }

            if( UNSECURED == iDataProtect )
            {
                if( pstssldataobj != NULL )
                {
                    SSL_free(pstssldataobj);
                    pstssldataobj = NULL;
                }

                close(sock);
                return 1;
            }
        }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

	}

	control_printf(SL_SUCCESS, "150 %s data connection established.",
	               xfertype == TYPE_BINARY ? "BINARY" : "ASCII");
	return 0;
}

void init_userinfo()
{
#ifndef NO_GETPWNAM
    struct passwd *temp;
    if(!strcmp(user,"anonymous"))
    {
        userinfo.pw_name = strdup("anonymous");
        userinfo.pw_passwd = strdup("*");
        userinfo.pw_uid = 0;
        userinfo.pw_gid = 0;
        userinfo.pw_gecos = strdup("");
        userinfo.pw_dir = strdup("/mnt");
        userinfo.pw_shell = strdup("sh");        
        userinfo_set = 1;
    }
    else
    {
        temp = getpwnam(user);
        if (temp) 
        {
            userinfo.pw_name = strdup(temp->pw_name);
            userinfo.pw_passwd = strdup(temp->pw_passwd);
            userinfo.pw_uid = temp->pw_uid;
            userinfo.pw_gid = temp->pw_gid;
            userinfo.pw_gecos = strdup(temp->pw_gecos);
            userinfo.pw_dir = strdup(temp->pw_dir);
            userinfo.pw_shell = strdup(temp->pw_shell);
			userinfo_set = 1;
        }
    }
#endif
}

void command_user(char *username)
{
	char *alias;
#ifdef SUPPORT_LOGIN_CONTROL
      VOS_UINT32    ulAllowWan = VOS_NOK;
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL
      char acAddr[64] = {0};
#endif
#endif

	if (state) 
	{
		control_printf(SL_FAILURE, "503 Username already given.");
		return;
	}

#ifdef SUPPORT_LOGIN_CONTROL
      isAccFromlan = IsAccessFromLan(&remotename);
#ifdef SUPPORT_ATP_FTP_ACCESS_CONTROL //y00181549 for 根据ftp类型细分权限控制 at 2012.6.10
	if(VOS_TRUE == ulisFtpsExCtl)
	{
		ulAllowWan = ATP_LoginCtlCheckFtpAcc(username,ATP_FTPTYPE_FTPSEXPLICIT,isAccFromlan);
		ulisFtpsExCtl = VOS_FALSE;
	}
	else 
	{
		ulAllowWan = ATP_LoginCtlCheckFtpAcc(username,ATP_FTPTYPE_FTPNORMAL,isAccFromlan);
	}
	if(VOS_OK != ulAllowWan)
	{
              if(remotename.sin_addr.s_addr)
              {
                  snprintf(acAddr,sizeof(acAddr),"%s",inet_ntoa(remotename.sin_addr));
              }
              else
              {
                  inet_ntop(AF_INET6, &remotename6.sin6_addr, acAddr, sizeof(acAddr));
              }
		control_printf(SL_FAILURE , "331 User or password is wrong.");//modify for UDK0000285516
              ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_USER_LOGIN_FAIL_INFO,username,acAddr,"FTP");
		exit(0);
	}
	if(!strcasecmp(username,"anonymous")) //add by y00181549 for ftp anonymous禁止登陆
	{
		control_printf(SL_FAILURE , "332 The user is forbidden.");
	}
#else
      if(strcasecmp(username,"anonymous"))
      {
            ulAllowWan = ATP_LoginCtlAllowWanAcc(username , ATP_LOGINTYPE_FTP);
      }
	if((VOS_OK != ulAllowWan) && (!isAccFromlan))//changed by qichenghong 2010.01.15
	{
		control_printf(SL_FAILURE , "332 The user is forbidden.");//DTS2012071700212 ，提示用户输入用户名和密码
		return ;
	}
#endif
	if((VOS_OK == ulAllowWan) && (!isAccFromlan))
      {
            isAccFromWan = TRUE;
      }
#endif

	mystrncpy(user, username, sizeof(user) - 1);
    userinfo_set = 1; /* Dirty! */
	alias = (char *) config_getoption("ALIAS");
    userinfo_set = 0;
	if (alias[0] != '\0')
		mystrncpy(user, alias, sizeof(user) - 1);
    init_userinfo();
#ifdef DEBUG
	bftpd_log("Trying to log in as %s.\n", user);
#endif
    expand_groups();
    if((!strcasecmp(username,"anonymous")) || (!strcasecmp(username,"")))
    {
            state = STATE_USER;
                control_printf(SL_SUCCESS, "331 Password please.");
            // control_printf(SL_SUCCESS, "331 Password please.",username);
		//bftpd_login("");
    }
    else {
		state = STATE_USER;
		control_printf(SL_SUCCESS, "331 Password please.");
		//control_printf(SL_SUCCESS, "331 Password please.user=%s",username);
    }
}

#ifdef SUPPORT_ATP_NAS
static unsigned int  NasFtpLoginFailSendEmail(char *username,char *addr)
{
    if ((NULL == username) || (NULL == addr))
    {
        return 0;
    }
    ATP_NAS_SEND_EMAIL_NOTIFY_MSG_ST stUsbVolumeResult;
    
    ATP_MSG_SimpleBuilder((ATP_MSG_HEADER_ST *)&stUsbVolumeResult, "cms", ATP_MSG_NASCMS_SEND_EMAIL_NOTIFY, VOS_OK);
    stUsbVolumeResult.stMsgHead.ulDataLen = sizeof(stUsbVolumeResult.acLoginFailReason);
    stUsbVolumeResult.stMsgHead.ulMsgData = ATP_NAS_EMAIL_REASON_LOGIN_FALIED;
    snprintf(stUsbVolumeResult.acLoginFailReason, sizeof(stUsbVolumeResult.acLoginFailReason), "User %s, %s login failed with FTP", username,addr);
    ATP_MSG_Send((const ATP_MSG_HEADER_ST *)&stUsbVolumeResult);
    
    return 1;
}
#endif

void command_pass(char *password)
{
    //socklen_t my_length;
    //struct sockaddr_in new;
    int ret = 0;  //add by y00181549 for 输入错误用户名返回331，防止尝试用户名
    char acAddr[64] = {0};
#ifdef SUPPORT_LOGIN_CONTROL
    ATP_LOGINRET_EN enLoginRet = ATP_LOGINRET_OK;
#endif 
    if (state > STATE_USER) 
    {
        control_printf(SL_FAILURE, "503 Already logged in.");
        return;
    }
    if(remotename.sin_addr.s_addr)
    {
        snprintf(acAddr,sizeof(acAddr),"%s",inet_ntoa(remotename.sin_addr));
    }
    else
    {
        inet_ntop(AF_INET6, &remotename6.sin6_addr, acAddr, sizeof(acAddr));
    }
    ret = bftpd_login(password);
    /*start of add by y00181549 for W724v log at 20120718*/ 
    if(0 == ret )
    {
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_USER_LOGIN_INFO,user,acAddr,"FTP");
    }
/*    if (-2 == ret)
    {
        ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_USER_LOGIN_FAIL_INFO,user,acAddr,"FTP");
#ifdef SUPPORT_ATP_NAS
    if (0 == NasFtpLoginFailSendEmail(user,acAddr))
    {
        printf("[%s][%d]Nas send login ftp  fail email fail\r\n",__FILE__,__LINE__);
    }
#endif
        control_printf(SL_FAILURE, "331 User or password is wrong.");
    }*/
    else if ((-1 == ret)||(-2 == ret))
    {
        bftpd_log("Login as user '%s' failed.\n", user);
        //control_printf(SL_FAILURE, "421 Login incorrect. =command_pass===222==");
        /*modify for HOMEGW-14395 by y00181549 at 20121025*/
        control_printf(SL_FAILURE, "331 User or password is wrong.");
#if defined(SUPPORT_LOGIN_CONTROL) && defined (SUPPORT_ATP_FTP_ACCESS_CONTROL)// add by y00181549 for 同一帐号ftp登陆5次失败后1个小时内禁止登陆
	if((1 == g_ucLoginCtlEnable) && (strcasecmp(user , "anonymous")))
	{
		if(isAccFromWan)
		{
			enLoginRet = ATP_LoginCtlCheck(user , inet_ntoa(remotename.sin_addr) , ATP_LOGINTYPE_FTP|ATP_LOGINTYPE_REMOTE, LOGIN_FAIL);
		}
		else
		{
			enLoginRet = ATP_LoginCtlCheck(user , inet_ntoa(remotename.sin_addr) , ATP_LOGINTYPE_FTP , LOGIN_FAIL);
		}
	}
#endif
        if (ATP_LOGINRET_OK == enLoginRet)
        {
            ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_USER_LOGIN_FAIL_INFO,user,acAddr,"FTP");
            if (-1 == ret) //??óD?ú?ü??′í?óê±????
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_FTP_LOGIN_ERROR_PASSWORD,user);
            }
#ifdef SUPPORT_ATP_NAS
            if (0 == NasFtpLoginFailSendEmail(user,acAddr))
            {
                printf("[%s][%d]Nas send login ftp fail email fail\r\n",__FILE__,__LINE__);
            }
#endif
        }
        else
        {
            /*start of add by y00181549 for W724v log 5次失败后密码错误的失败也记录at 20120727*/
            if(1 == isAccFromlan)
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_LOCAL_USER_FAIL_LOGIN_5_TIMES,user);
            }
            else
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_REMOTE_USER_FAIL_LOGIN_5_TIMES,user);
            }
            /*end of add by y00181549 for W724v log 5次失败后密码错误的失败也记录at 20120727*/
        }
     /*end of add by y00181549 for W724v log at 20120718*/ 
/*Added by yehuisheng00183935@20110702 优化ftpd功能*/
#if 0
        /*added by lvxin 00135113@20081223 for firefox login ftp*/
        if((!strcasecmp(user,"anonymous")) || (!strcasecmp(user,"")))
        {
            //control_printf(SL_FAILURE, "anonymous request .state=%d,user=%s",state,user);				 
            state = STATE_CONNECTED;
        }
        else
        {
            state = STATE_CONNECTED;
        }

        while (fgets(str, sizeof(str), stdin))
        {           
            alarm(control_timeout);
            str[strlen(str) - 2] = 0;
            bftpd_statuslog(2, 0, "%s", str);

#ifdef DEBUG 
            bftpd_log("Processing command: %s\n", str);
#endif
            parsecmd(str);
        }
        /*end of added by lvxin 00135113@20081223 for firefox login ftp*/
#else
        state = STATE_CONNECTED;
#endif
/*Added by yehuisheng00183935@20110702 优化ftpd功能*/

        //control_printf(SL_FAILURE, "530 Login incorrect. =end_child===20001==");
        return;
    }
}

void command_pwd(char *params)
{
     char *my_cwd = NULL;

     my_cwd = bftpd_cwd_getcwd();
     if (my_cwd)
     {
         control_printf(SL_SUCCESS, "257 \"%s\" is the current working directory.", my_cwd);
         free(my_cwd);
     }
}


void command_type(char *params)
{
    if ((*params == 'A') || (*params == 'a')) {
        control_printf(SL_SUCCESS, "200 Transfer type changed to ASCII");
        xfertype = TYPE_ASCII;
    } else if ((*params == 'I') || (*params == 'i')) {
      	control_printf(SL_SUCCESS, "200 Transfer type changed to BINARY");
        xfertype = TYPE_BINARY;
    } else
        control_printf(SL_FAILURE, "500 Type '%c' not supported.", *params);
}

void command_port(char *params) {
  unsigned long a0, a1, a2, a3, p0, p1, addr;
  unsigned long port = 0;
  if (epsvall) {
      control_printf(SL_FAILURE, "500 EPSV ALL has been called.");
      return;
  }
  sscanf(params, "%lu,%lu,%lu,%lu,%lu,%lu", &a0, &a1, &a2, &a3, &p0, &p1);
  addr = htonl((a0 << 24) + (a1 << 16) + (a2 << 8) + a3);
  if((addr != remotename.sin_addr.s_addr) &&( strncasecmp(config_getoption("ALLOW_FXP"), "yes", 3))) {
      control_printf(SL_FAILURE, "500 The given address is not yours.");
      return;
  }
  sa.sin_addr.s_addr = addr;
  port = (p0 << 8) + p1;
  sa.sin_port = htons(port);
  if (pasv) {
    close(sock);
    pasv = 0;
  }
  if (port <= 1024) {
      control_printf(SL_FAILURE, "504 Command not implemented for that parameter.");
      return;
  }
  control_printf(SL_SUCCESS, "200 PORT %lu.%lu.%lu.%lu:%lu OK",
           a0, a1, a2, a3, (p0 << 8) + p1);
}

void command_eprt(char *params) {
    char delim;
    int af;
    char addr[51];
    char foo[20];
    int port;
    if (epsvall) {
        control_printf(SL_FAILURE, "500 EPSV ALL has been called.");
        return;
    }
    if (strlen(params) < 5) {
        control_printf(SL_FAILURE, "500 Syntax error.");
        return;
    }
    delim = params[0];
	/*Added by lvxin00135113@20110425 安全代码整改*/
	#if 0
    sprintf(foo, "%c%%i%c%%50[^%c]%c%%i%c", delim, delim, delim, delim, delim);
	#else
    snprintf(foo,sizeof(foo),"%c%%i%c%%50[^%c]%c%%i%c", delim, delim, delim, delim, delim);
	#endif
	/*Added by lvxin00135113@20110425 安全代码整改*/
    if (sscanf(params, foo, &af, addr, &port) < 3) {
        control_printf(SL_FAILURE, "500 Syntax error.");
        return;
    }

#ifdef SUPPORT_ATP_IPV6
    if ((af != 1)&&(af != 2)) 
    {
        control_printf(SL_FAILURE, "522 Protocol unsupported, use (1)");
        return;
    }
    eaf = af;
#else
    if (af != 1) {
        control_printf(SL_FAILURE, "522 Protocol unsupported, use (1)");
        return;
    }
#endif

    if(1 == af)
    {
        sa.sin_addr.s_addr = inet_addr(addr);
        if ((sa.sin_addr.s_addr != remotename.sin_addr.s_addr) && (strncasecmp(config_getoption("ALLOW_FXP"), "yes", 3))) {
            control_printf(SL_FAILURE, "500 The given address is not yours.");
            return;
        }
        sa.sin_port = htons(port);
        if (pasv) {
            close(sock);
            pasv = 0;
        }
    }
#ifdef SUPPORT_ATP_IPV6
    else
    {
//modified by @jinpeng j00196195 at 20111008 for DTS2011083100649 START
	{
		/*
		找到scope id，并从addr字符串中删除，
			这里的方法是在找到并用字符串结束符'\0' 替换
		*/
		char *psz_tmp = NULL;
		psz_tmp = strstr(addr,"%");
		if ( NULL != psz_tmp )
		{
		    addr[psz_tmp-addr] = '\0';
		}
	}
//modified by @jinpeng j00196195 at 20111008 for DTS2011083100649 END

        if (inet_pton(AF_INET6, addr, &sa6.sin6_addr) < 0)
        {
            return;
        }
        if ((0 != memcmp(sa6.sin6_addr.s6_addr, remotename6.sin6_addr.s6_addr, 16)) && (strncasecmp(config_getoption("ALLOW_FXP"), "yes", 3))) {
            control_printf(SL_FAILURE, "500 The given address is not yours.");
            return;
        }
        sa6.sin6_port = htons(port);
        sa6.sin6_family = AF_INET6;
        
        if (pasv) {
            close(sock);
            pasv = 0;
        }
    }
#endif

    control_printf(SL_FAILURE, "200 EPRT %s:%i OK", addr, port);
}

void command_pasv(char *foo)
{
	int a1, a2, a3, a4;
        socklen_t namelen;
	struct sockaddr_in localsock;
        char *my_override_ip;

    if (epsvall) {
        control_printf(SL_FAILURE, "500 EPSV ALL has been called.");
        return;
    }
	pasvsock = socket(AF_INET, SOCK_STREAM, 0);
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_family = AF_INET;

    if (!config_getoption("PASSIVE_PORTS") || !strlen(config_getoption("PASSIVE_PORTS"))) {
        /* bind to any port */
        sa.sin_port = 0;
        if (bind(pasvsock, (struct sockaddr *) &sa, sizeof(sa)) == -1) 
        {
            control_printf(SL_FAILURE, "425-Error: Unable to bind data socket.\r\n425 %s", strerror(errno));
            return;
        }
    } 

     else {
        int i = 0, success = 0, port;
        for (;;) {
            port = int_from_list(config_getoption("PASSIVE_PORTS"), i++);
            if (port < 0)
                break;
            sa.sin_port = htons(port);
            if (bind(pasvsock, (struct sockaddr *) &sa, sizeof(sa)) == 0) {
                success = 1;
#ifdef DEBUG
                bftpd_log("Passive mode: Successfully bound port %d\n", port);
#endif
                break;
            }
        }   /* end of for loop */
        if (!success) {
            control_printf(SL_FAILURE, "425 Error: Unable to bind data socket.");
            return;
        }
        prepare_sock(pasvsock);
    }    /* end of else using list of ports */
#ifndef SUPPORT_ATP_FTP_UPGRADE
    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
    ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
	if (listen(pasvsock, 1)) {
		control_printf(SL_FAILURE, "425-Error: Unable to make socket listen.\r\n425 %s",
				 strerror(errno));
		return;
	}
	namelen = sizeof(localsock);
	getsockname(pasvsock, (struct sockaddr *) &localsock, (socklen_t *) &namelen);

        /* see if we should over-ride the IP address sent to the client */
        my_override_ip = config_getoption("OVERRIDE_IP");
        if (my_override_ip[0])
        {
            sscanf( my_override_ip, "%i.%i.%i.%i",
                    &a1, &a2, &a3, &a4);
        }
        else   /* noraml, no over-ride */
        {
  	    sscanf((char *) inet_ntoa(name.sin_addr), "%i.%i.%i.%i",
		   &a1, &a2, &a3, &a4);
        }

	control_printf(SL_SUCCESS, "227 Entering Passive Mode (%i,%i,%i,%i,%i,%i)", a1, a2, a3, a4,
			 ntohs(localsock.sin_port) >> 8, ntohs(localsock.sin_port) & 0xFF);
	pasv = 1;
}

void command_epsv(char *params)
{
    struct sockaddr_in localsock;
#ifdef SUPPORT_ATP_IPV6
    struct sockaddr_in6 localsock6;
#endif
    socklen_t namelen;
    int af = 0 ; 
    if (params[0]) 
    {
        if (!strncasecmp(params, "ALL", 3))
        {
            epsvall = 1;
            /* epsv 请求参数为ALL时的处理add by hkf39743 2011-3-31 start  */
            if  ( 0 == g_epsv )
            {
                pasvsock = socket(AF_INET, SOCK_STREAM, 0);
                sa.sin_addr.s_addr = INADDR_ANY;
                sa.sin_port = 0;
                sa.sin_family = AF_INET;
                if (bind(pasvsock, (struct sockaddr *) &sa, sizeof(sa)) == -1)
                {
                    control_printf(SL_FAILURE, "500-Error: Unable to bind data socket.\r\n425 %s",
                    strerror(errno));
                    return;
                }
#ifndef SUPPORT_ATP_FTP_UPGRADE
                /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
                ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
                /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
                if (listen(pasvsock, 1))
                {
                    control_printf(SL_FAILURE, "500-Error: Unable to make socket listen.\r\n425 %s",
                    strerror(errno));
                    return;
                }
                namelen = sizeof(localsock);
                getsockname(pasvsock, (struct sockaddr *) &localsock, (socklen_t *) &namelen);
                control_printf(SL_SUCCESS, "229 Entering extended passive mode (|||%i|)",
                ntohs(localsock.sin_port));
                pasv = 1;
            }
#ifdef SUPPORT_ATP_IPV6
            else
            {
                pasvsock = socket(AF_INET6, SOCK_STREAM, 0);
                sa6.sin6_addr = in6addr_any;
                sa6.sin6_port = 0;
                sa6.sin6_family = AF_INET6;
                if (bind(pasvsock, (struct sockaddr *) &sa6, sizeof(sa6)) == -1)
                {
                    control_printf(SL_FAILURE, "500-Error: Unable to bind data socket.\r\n425 %s",
                    strerror(errno));
                    return;
                }
#ifndef SUPPORT_ATP_FTP_UPGRADE
                /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
                ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
                /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
                if (listen(pasvsock, 1))
                {
                    control_printf(SL_FAILURE, "500-Error: Unable to make socket listen.\r\n425 %s",
                    strerror(errno));
                    return;
                }
                namelen = sizeof(localsock6);
                getsockname(pasvsock, (struct sockaddr *) &localsock6, (socklen_t *) &namelen);
                control_printf(SL_SUCCESS, "229 Entering extended passive mode (|||%i|)",
                ntohs(localsock6.sin6_port));
                pasv = 1;
            }
#endif
            /* epsv 请求参数为 ALL 时处理add by hkf39743 2011-3-31 end  */
        }
        else 
        {
            if (sscanf(params, "%i", &af) < 1) 
            {
                control_printf(SL_FAILURE, "500 Syntax error.");
                return;
            } 
            else 
            {
#ifdef SUPPORT_ATP_IPV6
                if ((af != 1) && (af !=2))
                {
                    control_printf(SL_FAILURE, "Network protocol not supported, use (1,2)");
                    return ;
                }
#else
                if (af != 1) 
                {
                    control_printf(SL_FAILURE, "522 Protocol unsupported, use (1)");
                    return;
                }
#endif

                /*epsv 请求参数为1，2时的处理add by hkf39743 2011-3-31 start */   
                if  ( 1 == af )
                {
                    pasvsock = socket(AF_INET, SOCK_STREAM, 0);
                    sa.sin_addr.s_addr = INADDR_ANY;
                    sa.sin_port = 0;
                    sa.sin_family = AF_INET;
                    if (bind(pasvsock, (struct sockaddr *) &sa, sizeof(sa)) == -1)
                    {
                        control_printf(SL_FAILURE, "500-Error: Unable to bind data socket.\r\n425 %s",
                        strerror(errno));
                        return;
                    }
#ifndef SUPPORT_ATP_FTP_UPGRADE
                    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
                    ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
                    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
                    if (listen(pasvsock, 1))
                    {
                        control_printf(SL_FAILURE, "500-Error: Unable to make socket listen.\r\n425 %s",
                        strerror(errno));
                        return;
                    }
                    namelen = sizeof(localsock);
                    getsockname(pasvsock, (struct sockaddr *) &localsock, (socklen_t *) &namelen);
                    control_printf(SL_SUCCESS, "229 Entering extended passive mode (|||%i|)",
                    ntohs(localsock.sin_port));
                    pasv = 1;
                }
#ifdef SUPPORT_ATP_IPV6
                else if ( 2 == af )
                {
                    pasvsock = socket(AF_INET6, SOCK_STREAM, 0);
                    sa6.sin6_addr = in6addr_any;
                    sa6.sin6_port = 0;
                    sa6.sin6_family = AF_INET6;
                    if (bind(pasvsock, (struct sockaddr *) &sa6, sizeof(sa6)) == -1)
                    {
                        control_printf(SL_FAILURE, "500-Error: Unable to bind data socket.\r\n425 %s",
                        strerror(errno));
                        return;
                    }
#ifndef SUPPORT_ATP_FTP_UPGRADE
                    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
                    ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
                    /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
                    if (listen(pasvsock, 1))
                    {
                        control_printf(SL_FAILURE, "500-Error: Unable to make socket listen.\r\n425 %s",
                        strerror(errno));
                        return;
                    }
                    namelen = sizeof(localsock6);
                    getsockname(pasvsock, (struct sockaddr *) &localsock6, (socklen_t *) &namelen);
                    control_printf(SL_SUCCESS, "229 Entering extended passive mode (|||%i|)",
                    ntohs(localsock6.sin6_port));
                    pasv = 1;
                }
#endif
                /*epsv 请求参数为1，2时的处理add by hkf39743 2011-3-31 end */  
            }
        }
    }
    /*epsv 请求参数为空时处理add by hkf39743 2011-3-31 start  */
    else
    {
        if  ( 0 == g_epsv )
        {
            pasvsock = socket(AF_INET, SOCK_STREAM, 0);
            sa.sin_addr.s_addr = INADDR_ANY;
            sa.sin_port = 0;
            sa.sin_family = AF_INET;
            if (bind(pasvsock, (struct sockaddr *) &sa, sizeof(sa)) == -1)
            {
                control_printf(SL_FAILURE, "500-Error: Unable to bind data socket.\r\n425 %s",
                strerror(errno));
                return;
            }
#ifndef SUPPORT_ATP_FTP_UPGRADE
            /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
            ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
            /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
            if (listen(pasvsock, 1))
            {
                control_printf(SL_FAILURE, "500-Error: Unable to make socket listen.\r\n425 %s",
                strerror(errno));
                return;
            }
            namelen = sizeof(localsock);
            getsockname(pasvsock, (struct sockaddr *) &localsock, (socklen_t *) &namelen);
            control_printf(SL_SUCCESS, "229 Entering extended passive mode (|||%i|)",
            ntohs(localsock.sin_port));
            pasv = 1;
        }
#ifdef SUPPORT_ATP_IPV6
        else
        {
            pasvsock = socket(AF_INET6, SOCK_STREAM, 0);
            sa6.sin6_addr = in6addr_any;
            sa6.sin6_port = 0;
            sa6.sin6_family = AF_INET6;
            if (bind(pasvsock, (struct sockaddr *) &sa6, sizeof(sa6)) == -1)
            {
                control_printf(SL_FAILURE, "500-Error: Unable to bind data socket.\r\n425 %s",
                strerror(errno));
                return;
            }
#ifndef SUPPORT_ATP_FTP_UPGRADE
            /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
            ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN );
            /* Added by y00183935@20120623 for opening ACL function about passive mode data port. */
#endif
            if (listen(pasvsock, 1))
            {
                control_printf(SL_FAILURE, "500-Error: Unable to make socket listen.\r\n425 %s",
                strerror(errno));
                return;
            }
            namelen = sizeof(localsock6);
            getsockname(pasvsock, (struct sockaddr *) &localsock6, (socklen_t *) &namelen);
            control_printf(SL_SUCCESS, "229 Entering extended passive mode (|||%i|)",
            ntohs(localsock6.sin6_port));
            pasv = 1;
        }
#endif
    }
    /*epsv 请求参数为空时的处理add by hkf39743 2011-3-31 end  */
}


char test_abort(char selectbefore, int file, int sock)
{
    char str[512];
    fd_set rfds;
    struct timeval tv;
    if (selectbefore) {
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&rfds);
        FD_SET(fileno(stdin), &rfds);
        if (!select(fileno(stdin) + 1, &rfds, NULL, NULL, &tv))
            return 0;
    }

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if 0
	fgets(str, sizeof(str), stdin);
#else
    if ( atp_ftpd_read_command( str, sizeof( str ) ) < 0 )
    {
        return 0;
    }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
	
    if (strstr(str, "ABOR")) {
        control_printf(SL_SUCCESS, "426 Transfer aborted.");

    	close(file);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
		close(sock);
   		control_printf(SL_SUCCESS, "226 Aborted.");
		bftpd_log("Client aborted file transmission.\n");
        alarm(control_timeout);
        return 1;
	}
    return 0;
}

void command_allo(char *foo)
{
    command_noop(foo);
}


/* This function allows the storage of multiple files on the server. */
void command_mput(char *filenames)
{
   char filename[MAXCMD];  /* single filename */
   int from_index, to_index;      /* position in "filenames" and "filename" */
                                                                                                                             
   from_index = 0;     /* start at begining of filenames */
   memset(filename, 0, MAXCMD);       /* clear filename */
   to_index = 0;

   /* go until we find a NULL character */
   while ( filenames[from_index] > 0)
   {
       /* copy filename until we hit a space */
       if (filenames[from_index] == ' ')
       {
          /* got a full filename */
          command_stor(filename);
          /* clear filename and reset to_index */
          to_index = 0;
          memset(filename, 0, MAXCMD);

          while (filenames[from_index] == ' ')
            from_index++;    /* goto next position */
       }
                                                                                                                             
       /* if we haven't hit a space, then copy the letter */
       else
       {
          filename[to_index] = filenames[from_index];
          to_index++;
          from_index++;
          /* if the next character is a NULL, then this is the end of the filename */
          if (! filenames[from_index])
          {
             command_stor(filename);   /* get the file */
             to_index = 0;             /* reset filename index */
             memset(filename, 0, MAXCMD);    /* clear filename buffer */
             from_index++;                /* goto next character */
          }
       }
                                                                                                                             
       /* if the buffer is getting too big, then stop */
       if (to_index > (MAXCMD - 2) )
       {
          bftpd_log("Error: Filename in '%s' too long.\n", filenames);
          return;
       }
                                                                                                                             
    }   /* end of while */
                                                                                                                             
}

//!添加对文件加锁 解锁 测试锁的方法by w00190448@20120504 start
#include <fcntl.h>

int lock_file(int fd, int type)
{
    struct flock st_lock;

    st_lock.l_type = type;
    st_lock.l_start = 0;
    st_lock.l_whence = SEEK_SET;
    st_lock.l_len = 0;
    return (fcntl(fd, F_SETLK, &st_lock));
}

int unlock_file(int fd)
{
    struct flock st_lock;

    st_lock.l_type = F_UNLCK;
    st_lock.l_start = 0;
    st_lock.l_whence = SEEK_SET;
    st_lock.l_len = 0;
    return (fcntl(fd, F_SETLK, &st_lock));
}

int checklock_file(int fd, int type)
{
    struct flock st_lock;

    st_lock.l_type = type;
    st_lock.l_start = 0;
    st_lock.l_whence = SEEK_SET;
    st_lock.l_len = 0;
    st_lock.l_pid = -1;
    fcntl(fd, F_GETLK, &st_lock);

    if(st_lock.l_type != F_UNLCK)
    {        
        if(st_lock.l_type == F_WRLCK)
        {        
            bftpd_statuslog(2, 0, "Error: Write lock already set by %d\n", st_lock.l_pid);
        } 
        else if(st_lock.l_type == F_RDLCK)
        {
            bftpd_statuslog(2, 0, "Error: Read lock already set by %d\n", st_lock.l_pid);
        }

        return -1;
    }

    return 0;
}

//!添加对文件加锁 解锁 测试锁的方法by w00190448@20120504 end

void do_stor(char *filename, int flags)
{
    char *buffer;
    int fd, i, max;
    fd_set rfds;
    struct timeval tv;
    char *p, *pp;
    char *mapped = bftpd_cwd_mappath(filename);

    int my_buffer_size;    /* total transfer buffer size divided by number of clients */
    int num_clients = 1;       /* number of clients connected to the server */
    int new_num_clients = 1;
    int xfer_delay;
    int attempt_gzip = FALSE;
    unsigned long get_value;
    int change_buffer_size = FALSE;
    int stdin_fileno;
    int  nIsValidFilename;
	struct stat stbuf;
	int lFileCreateMode = BFTPD_FILE_CREATEMODE;

    /*Added by yehuisheng00183935@20111104 修改问题单(DTS2011092702151) */
    filtrateBlankCharaterInFilename( mapped );
    /*Added by yehuisheng00183935@20111104 修改问题单(DTS2011092702151) */

    #ifdef HAVE_ZLIB_H
    gzFile my_zip_file = NULL;
    #endif
      
    if ( 1 == iCheckMemCreate)
    {
         //文件不能保存在根目录底下
         if ( checkdiskfree(mapped) < 0 )
         {
             control_printf(SL_FAILURE, "452 Error: memory or disk space is not enough or disk is full.", mapped);

             if(mapped)
                free(mapped);

/*数据未连接返回时关闭数据连接start*/
             if(pasvsock)
             {
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifndef SUPPORT_ATP_FTP_UPGRADE
                ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                close(pasvsock);
             }
/*数据未连接返回时关闭数据连接end*/
             
            return;
         }
    }

    nIsValidFilename =  IsValidFilename(mapped,filename,512);

    if(1  != nIsValidFilename)
    {
      if(0 == nIsValidFilename)
      {
          control_printf(SL_FAILURE, "451 Error: Can not longer than 512 characters.");
      }
      else if(-1 == nIsValidFilename)
      {
          control_printf(SL_FAILURE, "451 Error: Can not include these characters:\\/:*?\"<>|.");        
      }
      else if(-2 == nIsValidFilename)   /*Added by yehuisheng00183935@20110922 修改问题单(DTS2011083103592)*/
      {
          control_printf(SL_FAILURE, "450 Error: Requested file action not taken.");        
      }

      if(mapped)
         free(mapped);

/*数据未连接返回时关闭数据连接start*/
             if(pasvsock)
             {
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifndef SUPPORT_ATP_FTP_UPGRADE
                ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                close(pasvsock);
             }
/*数据未连接返回时关闭数据连接end*/
      
      return;
    }

    if (pre_write_script)
       run_script(pre_write_script, mapped);

    #ifdef HAVE_ZLIB_H
    if (! strcmp( config_getoption("GZ_UPLOAD"), "yes") )
    {
       attempt_gzip = TRUE;
       strncat(mapped, ".gz", strlen(path) + strlen(cwd) + 16 - strlen(mapped) - 1);
    }
    else
       attempt_gzip = FALSE;
    #endif
 
        /* See if we should delay between data transfers */
        get_value = strtoul( config_getoption("XFER_DELAY"), NULL, 0);
        if (get_value <= INT_MAX)
           xfer_delay = get_value;
        else
        {
           bftpd_log("Error getting xfer_delay in do_stor().\n", 0);
           xfer_delay = 0;
           /* restrict upload speed. <2008.12.25 tanyin> */
           xfer_delay = 500*1000; /* 500ms */
        }

        /* Check to see if the file exists and if we can over-write
           it, if it does. -- Jesse */
        fd = open(mapped, O_RDONLY);
        if (fd >= 0)  /* file exists */
        {
           /* close the file */
           close(fd);
           /* check if we can over-write it */
           if ( !strcasecmp( config_getoption("ALLOWCOMMAND_DELE"), "no") )
           {
              bftpd_log("Not allowed to over-write '%s'.\n", filename);
              control_printf(SL_FAILURE, 
                     "553 Error: Remote file is write protected.");

              if(mapped)
                 free(mapped);
              
/*Added by yehuisheng00183935@20110702 优化ftpd功能*/
#if 1 
             if(pasvsock)
             {
#ifndef SUPPORT_ATP_FTP_UPGRADE
                ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                close(pasvsock);
             }

            exit(0);
#endif
/*Added by yehuisheng00183935@20110702 优化ftpd功能*/

           }
        }

        if (! attempt_gzip)
        { 
	  fd = open(mapped, flags, lFileCreateMode);
	  /*
             do this below
             if (mapped)
	 	free(mapped);
          */
	  if (fd == -1) {
		bftpd_statuslog(2, 0, "Error: '%s' while trying to store file '%s'.\n",
				  strerror(errno), filename);
		control_printf(SL_FAILURE, "553 Error: %s.", strerror(errno));

                close(fd);     /* make sure it is not open */
                if (post_write_script)
                   run_script(post_write_script, mapped);

                if(mapped)
                   free(mapped);

/*数据未连接返回时关闭数据连接start*/
#if 1
             if(pasvsock)
             {
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifndef SUPPORT_ATP_FTP_UPGRADE
                ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                close(pasvsock);
             }

             return;
#endif
                
      }

//!检测锁存在和加锁by w00190448@20120504 start
   if( -1 == checklock_file(fd, F_WRLCK))
    {
        bftpd_statuslog(2, 0, "Error while trying to check file [%s] lock.\n",filename);
        
        close(fd);

        if (post_write_script)
           run_script(post_write_script, mapped);

        if(mapped)
            free(mapped);

        control_printf(SL_FAILURE, "451 Error:File [%s] is in use.", filename);

/*数据未连接返回时关闭数据连接start*/
#if 1
             if(pasvsock)
             {
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifndef SUPPORT_ATP_FTP_UPGRADE
                 ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                 close(pasvsock);
             }


            return;
#endif
/*数据未连接返回时关闭数据连接end*/
        
    }
    else
    {
        if(-1 == lock_file(fd, F_WRLCK))
        {
            bftpd_statuslog(2, 0,"Error while trying to lock file [%s]\n",filename);

            close(fd);

            if (post_write_script)
               run_script(post_write_script, mapped);
            
            if(mapped)
                free(mapped);

            control_printf(SL_FAILURE, "451 Error while trying to lock file [%s]", filename);

/*数据未连接返回时关闭数据连接start*/
#if 1
             if(pasvsock)
             {
#ifndef SUPPORT_ATP_FTP_UPGRADE
                 ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                 close(pasvsock);
             }
            return;
#endif
/*数据未连接返回时关闭数据连接end*/
        
        }
    }
//!检测锁存在和加锁by w00190448@20120504 end
      
	}

      //added by qichenghong 20100227 for 解决FAT/NTFS文件系统创建文件和文件夹时无法设置mode
      if(-1 != fstat(fd , &stbuf))
      {
        if((stbuf.st_mode & 00777) != lFileCreateMode)
        {
            fchmod(fd , (stbuf.st_mode & ~(00777)) | lFileCreateMode);
        }
      }
      //added by qichenghong 20100227 for 解决FAT/NTFS文件系统创建文件和文件夹时无法设置mode end
        #ifdef HAVE_ZLIB_H
        if ( attempt_gzip )
        {
           my_zip_file = gzopen(mapped, "wb+");
           if (mapped)
           {
               free(mapped);
               mapped = NULL;
           }
           if (! my_zip_file)
           {
              control_printf(SL_FAILURE, "553 Error: An error occured creating compressed file.");
/*Added by yehuisheng00183935@20110702 优化ftpd功能*/
#if 1  
             if(pasvsock)
             {
#ifndef SUPPORT_ATP_FTP_UPGRADE
                 ATPDealACLDataPortForPassiveMode( pasvsock, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE );
#endif
                 close(pasvsock);
             }
#endif
/*Added by yehuisheng00183935@20110702 优化ftpd功能*/
              close(fd);

              if(mapped)
                 free(mapped);
              
              exit(0);
           }
        }
        #endif

	bftpd_log("Client is storing file '%s'.\n", filename);
	if (dataconn())
    {
//!解锁by w00190448@20120505 start
        
        if(-1 == unlock_file(fd))
        {
            bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
        }
//!解锁by w00190448@20120505 end

        close(fd);

        if (post_write_script)
           run_script(post_write_script, mapped);
        
        if(mapped)
           free(mapped);

        return;
    }

    /* decide if the transfer buffer size should change. */
    if (! strcasecmp( config_getoption("CHANGE_BUFSIZE"), "yes") )
       change_buffer_size = TRUE;
    /* Figure out how big the transfer buffer should be.
       This will be the total size divided by the number of clients connected.
       -- Jesse
    */
    if (change_buffer_size)
    {
       num_clients = bftpdutmp_usercount("*");
       my_buffer_size = get_buffer_size(num_clients);
    }
    else
       my_buffer_size = xfer_bufsize;

    alarm(0);
    buffer = malloc(my_buffer_size+1);
    /* Check to see if we are out of memory. -- Jesse */
    if (! buffer)
    {
       bftpd_log("Unable to create buffer to receive file.\n", 0);
       
       control_printf(SL_FAILURE, "452 Error: low memory.");
       if (fd >= 0)
       {
    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(fd))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end        
          close(fd);
       }
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
       atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
       close(sock);

       if(mapped)
          free(mapped);
       
       return;
    }

	lseek(fd, offset, SEEK_SET);
	offset = 0;
    /* Do not use the whole buffer, because a null byte has to be
     * written after the string in ASCII mode. */
    stdin_fileno = fileno(stdin);
    max = (sock > stdin_fileno ? sock : stdin_fileno) + 1;
	for (;;)       /* start receiving loop */ 
        {
        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);
        FD_SET( stdin_fileno, &rfds);
        
        tv.tv_sec = data_timeout;
        tv.tv_usec = 0;
        if (!select(max, &rfds, NULL, NULL, &tv)) {
	         if (buffer)
		        free(buffer);

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
            atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
            close(sock);

    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(fd))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end        

            close(fd);
            control_printf(SL_FAILURE, "426 Kicked due to data transmission timeout.");
            bftpd_log("Kicked due to data transmission timeout.\n");
            /* Before we exit, let's remove our entry in the log file. -- Jesse */
            if (post_write_script)
               run_script(post_write_script, mapped);

            bftpdutmp_end();
            exit(0);
        }
        if (FD_ISSET(stdin_fileno, &rfds)) {
            test_abort(0, fd, sock);
	    if (buffer)
		free(buffer);

    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(fd))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end      
        
            close(fd);
            if (post_write_script)
               run_script(post_write_script, mapped);

            if(mapped)
               free(mapped); 

            close(sock);

            return;
        }

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        if ( pstssldataobj != NULL )
        {
            if ( ( i = SSL_read( pstssldataobj, (unsigned char *)buffer, ( my_buffer_size -1 ) ) ) <= 0 )
            {
                break;
            }
        }
        else
        {
            if ( ( i = recv(sock, buffer, my_buffer_size - 1, 0 ) ) <= 0 )
            {
                break;
            }
        }
#else
        if (!((i = recv(sock, buffer, my_buffer_size - 1, 0))))
        {
            break;
        }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

        bytes_recvd += i;
#ifdef SUPPORT_ATP_FTP_UPGRADE
        /* 限制文件大小，超过4M的中断传输，重启 */
        if(bytes_recvd > MAX_IMAGE_SIZE)
        {	       	    
	       control_printf(SL_FAILURE, "701 Error: Image file is too big!The system will reboot!");
             bftpd_log("failed because file size is too big.\n");
	       if (fd >= 0) 
             {
                    if(-1 == unlock_file(fd))
                    {
                        bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
                    }
    		       close(fd);
	       }
               free(buffer);
#ifdef SUPPORT_ATP_FTPDS
               atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
	       close(sock);
	       unlink(mapped);

               if(mapped)
                  free(mapped);
                    
       	  return;	     
       }
#endif

        if (xfertype == TYPE_ASCII) {
            buffer[i] = '\0';
            /* on ASCII stransfer, strip character 13 */
            p = pp = buffer;
    	    while (*p) {
        		if ((unsigned char) *p == 13)
        			p++;
        		else
        			*pp++ = *p++;
            }   
        	*pp++ = 0;
           // i = strlen(buffer);
           i = pp - buffer - 1;
        }     // end of if ASCII type transfer

        #ifdef HAVE_ZLIB_H
           if (my_zip_file)
               gzwrite( my_zip_file, buffer, i );    
        #endif
           if(! attempt_gzip)
       	{
       	      int nWRBytes = 0;
       
              nWRBytes = write(fd, buffer, i);
	       if (nWRBytes == -1)
	       	{	       	    
		       control_printf(SL_FAILURE, "452 Error: memory or disk space is not enough.");
		       if (fd >= 0) {
                    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(fd))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end      
    		       close(fd);
		       }
               free(buffer);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
               atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
		       close(sock);
		       unlink(mapped);

               if(mapped)
                  free(mapped);
                    
       	       return;	     
	       	}
       	}
        /* Check to see if our bandwidth usage should change. -- Jesse */
        if (change_buffer_size)
        {
           new_num_clients = bftpdutmp_usercount("*");
           if (new_num_clients != num_clients)
           {
               num_clients = new_num_clients;
               my_buffer_size = get_buffer_size(num_clients);
           }
        }

        /* check for transfer delay */
        if ( xfer_delay )
        {
            struct timeval wait_time;

            wait_time.tv_sec = 0;
            wait_time.tv_usec = xfer_delay;
            select( 0, NULL, NULL, NULL, &wait_time);
        }


	}     // end of for loop, reading

    /* flush disk. <2008.12.25 tanyin> */
    fsync(fd);
	free(buffer);
#ifdef HAVE_ZLIB_H
    gzclose(my_zip_file);
#else

    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(fd))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end      

    close(fd);
#endif

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
	close(sock);
        alarm(control_timeout);
    offset = 0;
	control_printf(SL_SUCCESS, "226 File transmission successful.");
	bftpd_log("File transmission successful.\n");
    if (post_write_script)
      run_script(post_write_script, mapped);

#ifdef SUPPORT_ATP_FTP_UPGRADE   
        if(ATP_BFTPD_CHILD_SendRecv_DOUPGRADE(mapped))//error
        {               
               control_printf(SL_FAILURE, "702 Error: Image file or config file is invalid");
                 bftpd_log("failed because Image file or config file is invalid\n");
               unlink(mapped);

               if(mapped)
                  free(mapped);
                        
              exit(0);  
       }

#endif

    if(mapped)
       free(mapped); 


}

void command_stor(char *filename)
{
    do_stor(filename, O_CREAT | O_WRONLY | O_TRUNC);
}

void command_appe(char *filename)
{
    do_stor(filename, O_CREAT | O_WRONLY | O_APPEND);
}




/* Send multpile files to the client. */
void command_mget(char *filenames)
{
   char filename[MAXCMD];  /* single filename */
   int from_index, to_index;      /* position in "filenames" and "filename" */

   from_index = 0;     /* start at begining of filenames */
   memset(filename, 0, MAXCMD);       /* clear filename */
   to_index = 0;   

   /* go until we find a NULL character */
   while ( filenames[from_index] > 0)
   {
       /* copy filename until we hit a space */
       if (filenames[from_index] == ' ') 
       {
          /* got a full filename */
          command_retr(filename);
          /* clear filename and reset to_index */
          to_index = 0;
          memset(filename, 0, MAXCMD);

          while (filenames[from_index] == ' ')
            from_index++;    /* goto next position */
       }
       
       /* if we haven't hit a space, then copy the letter */
       else
       {
          filename[to_index] = filenames[from_index];
          to_index++;
          from_index++;
          /* if the next character is a NULL, then this is the end of the filename */
          if (! filenames[from_index])
          {
             command_retr(filename);   /* send the file */
             to_index = 0;             /* reset filename index */
             memset(filename, 0, MAXCMD);    /* clear filename buffer */
             from_index++;                /* goto next character */
          }
       }

       /* if the buffer is getting too big, then stop */
       if (to_index > (MAXCMD - 2) )
       {
	  bftpd_log("Error: Filename in '%s' too long.\n", filenames);
          return;
       }

    }   /* end of while */
   
}



/*w45260 2010-03-29  start :采用双线程增加wan侧读写速度。对LAN侧速率也有提升*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <errno.h> 
#include <sys/ipc.h> 
#include <semaphore.h> 


int gSock  = 0;
int gFilefd = 0;
int gBufSize = 0;
int gXfer_delay = 0;
int gSendIdx = 0;
int RdFileFinished= 0;
int RdFileThreadPid= 0;
sem_t sem; //读文件允许信号量，

struct FileData{
    int      datalen;
    int      datavalid ;
    char * databuf  ;
};

#define MAXCOUNT 10
struct FileData filedatas[MAXCOUNT];

int   RdDataBufInit()
{
      int i = 0;
      char * buffer = NULL;
	  
      for( i = 0;i<MAXCOUNT;i++ )
      	{
      		  filedatas[i].databuf = NULL;
                buffer = calloc((gBufSize* 2 + 1),1);
                if (! buffer)
                {
                    goto freemem;
                }
		  filedatas[i].databuf = buffer;
		  filedatas[i].datavalid=0;
		  filedatas[i].datalen= 0;
       }
       return 0;

freemem:

	for( i = 0;i<MAXCOUNT;i++ )
      	{    	       
      		 if( NULL != filedatas[i].databuf )
		 {
		     free(filedatas[i].databuf );
      		 }	
		 filedatas[i].databuf = NULL	;
		 filedatas[i].datavalid = 0;
       }

	return -1;    
}

void  RdDataBufUnInit()
{
      int i = 0;
	  
       for( i = 0;i<MAXCOUNT;i++ )
      	{    	       
      		 if( NULL != filedatas[i].databuf )
		 {
		     free(filedatas[i].databuf );
      		 }	
		 filedatas[i].databuf = NULL	;
		  filedatas[i].datavalid = 0;
       }
}
 
int  RdDataBufIsEmpty()
{
      int i = 0;
      for( i = 0;i<MAXCOUNT;i++ )
      	{
      		 if( 1 == filedatas[i].datavalid)
		 {
		     return 0;
      		 }		 
       }

	return 1;  
}


/******************************************************************************
  函数名称  : ATP_Senddata
  功能描述  : 发送数据给客户端
  输入参数  : 无
             
  输出参数  :  
  返 回 值  : int 型变量，0 成功-1 失败结束

  修改历史     :
  1.日    期   : 2011-3-10
    作    者   : hkf39743
    修改内容   : 新创建函数
******************************************************************************/
int  ATP_Senddata()                                                    
{
    int   lBytesSent = 0;
    int   lLength = 0;
    int   lBytesOffset;
    lBytesOffset = 0;

    lLength = filedatas[gSendIdx].datalen;
    while (1)
    {
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        if ( pstssldataobj != NULL )
        {
            lBytesSent = SSL_write(pstssldataobj, (const unsigned char *)(filedatas[gSendIdx].databuf +  lBytesOffset) , filedatas[gSendIdx].datalen );
        }
        else
        {
            lBytesSent =  send(sock, filedatas[gSendIdx].databuf +  lBytesOffset , filedatas[gSendIdx].datalen, 0);
        }
#else
        lBytesSent =  send(sock, filedatas[gSendIdx].databuf +  lBytesOffset , filedatas[gSendIdx].datalen, 0);
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

        if (lBytesSent <= 0)
        {
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
            if ( POLARSSL_ERR_NET_TRY_AGAIN == lBytesSent )
            {
                continue;
            }
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

            return -1 ;
        }

        bytes_sent += lBytesSent;       /* 目前尚不明确该全局变量的作用，为了保持与原始函数的匹配，暂时保留。--y00183935 */

        lLength -= lBytesSent;
        lBytesOffset += lBytesSent;
        if (lLength <= 0)
        {
            break;
        }
    }

    return  0;
}

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if 0
int SendData()
{
       int send_status = 0;
        int xfer_delay;
        struct timeval wait_time;

       while ( (0 == RdFileFinished)  || (0 == RdDataBufIsEmpty())) 
      {//send to networik
      
             if (test_abort(1, gFilefd, gSock)) {
                     control_printf(SL_SUCCESS, "send  thread  test_abort !");						   	
               	goto senderrout;
              }
                   
      
            if(1 == filedatas[gSendIdx].datavalid )
            {
                  alarm(10);//send 超时检查
                  send_status =  send(sock, filedatas[gSendIdx].databuf, filedatas[gSendIdx].datalen, 0);
                  //// check for dropped connection
                  if (send_status < 0)
                  {
                       //control_printf(SL_SUCCESS, "426 Transfer aborted.");
                       //control_printf(SL_SUCCESS, "226 Aborted.");
                       //bftpd_log("File transmission interrupted. Send failed.\n");
                      goto senderrout;
					   
                  }else if( send_status != filedatas[gSendIdx].datalen)
                  {
                       control_printf(SL_SUCCESS, "Need send  %d bytes. But only send %d.",filedatas[gSendIdx].datalen,send_status);
                  }

		    bytes_sent += send_status;										   
                  filedatas[gSendIdx].datavalid = 0;
                  gSendIdx++;
                  if(gSendIdx == MAXCOUNT)
                  {
                       gSendIdx = 0;
                  }	

                    /*信号量加一，V操作:通知文件读取线程继续读取*/ 
                   sem_post(&sem); 	            	 
                             
               }else
               { 
                    //休眠等待.似乎没有避免死锁的更好的等待方式
      		      usleep(10000); // the time vlaue is very important。debug it.
               }

             /*支持降速配置项: pause between transfers */
             if (gXfer_delay)
             {
                   wait_time.tv_sec = 0;
                   wait_time.tv_usec = gXfer_delay;
                   select( 0, NULL, NULL, NULL, &wait_time);
             }      
      
      }
  				
      return 0 ;

senderrout:
      control_printf(SL_SUCCESS, "Send  error !");
      return -1;

 }
#else
int SendData()
{
    struct timeval wait_time;

    while ( (0 == RdFileFinished)  || (0 == RdDataBufIsEmpty()))
    {//send to networik
        if ( test_abort(1, gFilefd, gSock) )
        {
            control_printf(SL_SUCCESS, "send  thread  test_abort !");
        /*Added by yehuisheng00183935@20110702 优化ftpd功能*/
        #if 0       /* 客户端取消，属于正常退出。 */
            goto senderrout;
        #else
            return 0;
        #endif
        /*Added by yehuisheng00183935@20110702 优化ftpd功能*/
        }
        if ( 1 == filedatas[gSendIdx].datavalid )
        {
            alarm(10);//send 超时检查
            if (ATP_Senddata() < 0 )
            {
                goto senderrout;
            }

            filedatas[gSendIdx].datavalid = 0;
            gSendIdx++;
            if(gSendIdx == MAXCOUNT)
            {
                gSendIdx = 0;
            }

            /*信号量加一，V操作:通知文件读取线程继续读取*/ 
            sem_post(&sem);
        }
        else
        {
            //休眠等待.似乎没有避免死锁的更好的等待方式
            usleep(10000); // the time vlaue is very important。debug it.
        }

        /*支持降速配置项: pause between transfers */
        if (gXfer_delay)
        {
            wait_time.tv_sec = 0;
            wait_time.tv_usec = gXfer_delay;
            select( 0, NULL, NULL, NULL, &wait_time);
        }
    }

    return 0 ;

senderrout:

    control_printf(SL_SUCCESS, "Send  error !");
    return -1;

}
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/


void ReadThread()
{
      int i = 0;
      off_t size = 0; 
      off_t filesize = 0;
      off_t readsize = 0; 

	RdFileThreadPid = getpid();
	
	lseek(gFilefd, 0, SEEK_SET);
	filesize = lseek(gFilefd, 0, SEEK_END);
       if(-1 == filesize)
       {
       	    control_printf(SL_SUCCESS, "Read thread  lseek error!" );
       }

      lseek(gFilefd, 0, SEEK_SET);		


       while(readsize < filesize)
       {

              for( i = 0;i<MAXCOUNT;i++ )
          	{	
                     if(0 == filedatas[i].datavalid )
                 	{
                 	
                         size = read(gFilefd, filedatas[i].databuf, gBufSize); 
			     if (-1 == size)
                         {
                               if( (EINTR == errno) || (EAGAIN == errno))
                               {
                                    break;//to do it again later
                               } else
                               {
                                    control_printf(SL_SUCCESS, "Read thread read err ! errno %d ", errno);
                                    goto freemem;
                               }				   
                         }else if (0 == size)
                         {
                               //control_printf(SL_SUCCESS, "Read thread read len: %d  ", size);
                               break;//to do it again later
                         }
                         readsize += size;  //update it before  TYPE_ASCII  repalce.This is real len.
                 				
						 
                         if (xfertype == TYPE_ASCII) {
                             filedatas[i].databuf[size] = '\0';
                             size += replace(filedatas[i].databuf, "\n", "\r\n", gBufSize);
                         }
     
              	    filedatas[i].datalen = size;
                         filedatas[i].datavalid =1; // set this value at last of read op.


			    /*信号量减一，P操作:等待数据存储空间空闲*/ 
                         sem_wait(&sem); 
                 	}

           }
		  
       }
	 
      RdFileFinished = 1;
      return ;
	 
freemem:
	
      control_printf(SL_SUCCESS, "Thread  read  fail !");
      RdFileFinished = 1;//主发送进程需要这个标志结束发送
      return ; 
}

void command_retr_2thread(char *filename)
{
        int num_clients = 1;
        int my_buffer_size;       /* size of the transfer buffer to use */
	char *mapped = NULL;
        int xfer_delay;
        unsigned long get_value;
        int change_buffer_size = FALSE;

	
    	 /*设置高优先级可能导致语音异常且对FTP速率提升几乎没有，暂时取消*/	
#if 0		 
	 {
            struct sched_param param = { .sched_priority = 50}; //change the schedule priority here
            sched_setscheduler(getpid(), SCHED_RR, &param);
	 }
#endif
		 
#if (defined(WANT_GZIP) || defined(HAVE_ZLIB_H))
    gzFile gzfile;
#endif
	int phile;
	int whattodo = DO_NORMAL;
	struct stat statbuf;
#if (defined(WANT_TAR) && defined(WANT_GZIP))
    int filedes[2];
#endif
#if (defined(WANT_TAR) || defined(WANT_GZIP))
    char *foo;
#endif
#ifdef WANT_TAR
    char *argv[4];
#endif

        get_value = strtoul( config_getoption("XFER_DELAY"), NULL, 0);
        if (get_value <= INT_MAX)
           xfer_delay = get_value;
        else
        {
           bftpd_log("Error getting XFER_DELAY in command_retr().\n", 0);
           xfer_delay = 0;
        }

	mapped = bftpd_cwd_mappath(filename);
        if (! mapped)
        {
           bftpd_log("Memory error in sending file.\n", 0);
           control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 9);
           return;
        }

        if (! strcasecmp( config_getoption("CHANGE_BUFSIZE"), "yes") )
           change_buffer_size = TRUE;

	phile = open(mapped, O_RDONLY);
	if (phile == -1) {       // failed to open a file
#if (defined(WANT_TAR) && defined(WANT_GZIP))
		if ((foo = strstr(filename, ".tar.gz")))
			if (strlen(foo) == 7) {
				whattodo = DO_TARGZ;
				*foo = '\0';
			}
#endif
#ifdef WANT_TAR
		if ((foo = strstr(filename, ".tar")))
			if (strlen(foo) == 4) {
				whattodo = DO_TARONLY;
				*foo = '\0';
			}
#endif
#ifdef WANT_GZIP
		if ((foo = strstr(filename, ".gz")))
			if (strlen(foo) == 3) {
				whattodo = DO_GZONLY;
				*foo = '\0';
			}
#endif
		if (whattodo == DO_NORMAL) 
        {
			bftpd_log("Error: '%s' while trying to receive file '%s'.\n",
					  strerror(errno), filename);
			control_printf(SL_FAILURE, "553 Error: %s.", strerror(errno));
			if (mapped)
			{
				free(mapped);
				mapped = NULL;
			}
			return;
		}
	}

        #ifdef HAVE_ZLIB_H
        else  // we did open a file
        {
            char *my_temp;
            char *zip_option;

            my_temp = strstr(filename, ".gz");
            zip_option = config_getoption("GZ_DOWNLOAD");
            if (my_temp)
            {
               if ( ( strlen(my_temp) == 3) && (! strcasecmp(zip_option, "yes") ) )
                  whattodo = DO_GZUNZIP;
            }
        }
        #endif

	stat(mapped, (struct stat *) &statbuf);
	if (S_ISDIR(statbuf.st_mode)) 
    {
		control_printf(SL_FAILURE, "550 Error: Is a directory.");
		if (mapped)
		{
			free(mapped);
			mapped = NULL;
		}
        close(phile);
		return;
	}

/*
	if ((((statbuf.st_size - offset) * ratio_send) / ratio_recv > bytes_recvd
		 - bytes_sent) && (strcmp((char *) config_getoption("RATIO"), "none"))) {
		bftpd_log("Error: 'File too big (ratio)' while trying to receive file "
				  "'%s'.\n", filename);
		control_printf(SL_FAILURE, "553 File too big. Send at least %lf bytes first.",
				(double) (((statbuf.st_size - offset) * ratio_send) / ratio_recv)
				- bytes_recvd);
		if (mapped)
			free(mapped);
		return;
	}
	*/

//!检测锁存在和加锁by w00190448@20120504 start
    if( -1 == checklock_file(phile, F_RDLCK))
    {
        bftpd_statuslog(2, 0, "Error while trying to check file [%s] lock.\n",filename);
        
        close(phile);

	if (mapped)
	{
		free(mapped);
		mapped = NULL;
	}

        control_printf(SL_FAILURE, "451 Error:File [%s] is in use.", filename);
        
        return;
        
    }
    else
    {
        if(-1 == lock_file(phile, F_RDLCK))
        {
            bftpd_statuslog(2, 0,"Error while trying to lock file [%s]\n",filename);

            close(phile);

        	if (mapped)
        	{
        		free(mapped);
        		mapped = NULL;
        	}

            control_printf(SL_FAILURE, "451 Error while trying to lock file [%s]", filename);

            return;            
        }
    }
//!检测锁存在和加锁by w00190448@20120504 end
      

	bftpd_log("Client is receiving file '%s'.\n", filename);
	switch (whattodo)
    {
#if (defined(WANT_TAR) && defined(WANT_GZIP))
        case DO_TARGZ:
            close(phile);
            if (dataconn())
            {
				if (mapped)
				{
					free(mapped);
					mapped = NULL;
				}
                return;
			}
            alarm(0);
            pipe(filedes);
            if (fork())
            {
                buffer = malloc(xfer_bufsize);
                /* check to make sure alloc worked */
                if (! buffer)
                {
                   if (mapped)
                   {
                      free(mapped);
                      mapped = NULL;
                   }
                   bftpd_log("Memory error in sending file.\n", 0);
                   control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 9);
                   return;
                }

                /* find the size of the transfer buffer divided by number of connections */
                if (change_buffer_size)
                {
                   num_clients =  bftpdutmp_usercount("*");
                   my_buffer_size = get_buffer_size(num_clients);
                }
                else 
                  my_buffer_size = xfer_bufsize;
                
                close(filedes[1]);
                gzfile = gzdopen(sock, "wb");
                while ((i = read(filedes[0], buffer, my_buffer_size))) {
                    gzwrite(gzfile, buffer, i);
                    test_abort(1, phile, sock);

                    /* check for a change in number of connections */
                    if (change_buffer_size)
                    {
                       new_num_clients = bftpdutmp_usercount("*");
                       if (new_num_clients != num_clients)
                       {
                          num_clients = new_num_clients;
                          my_buffer_size = get_buffer_size(num_clients);
                       }
                    }
                    /* pause between transfers */
                    if (xfer_delay)
                    {
                        wait_time.tv_sec = 0;
                        wait_time.tv_usec = xfer_delay;
                        select( 0, NULL, NULL, NULL, &wait_time);
                    }
                }     // end of while 
                free(buffer);
                gzclose(gzfile);
                wait(NULL); /* Kill the zombie */
            }
            else
            {
                stderr = devnull;
                close(filedes[0]);
                close(fileno(stdout));
                dup2(filedes[1], fileno(stdout));
                setvbuf(stdout, NULL, _IONBF, 0);
                argv[0] = "tar";
                argv[1] = "cf";
                argv[2] = "-";
                argv[3] = mapped;
                exit(pax_main(4, argv));
            }
            break;
#endif
#ifdef WANT_TAR
		case DO_TARONLY:
            if (dataconn())
            {
				if (mapped)
					free(mapped);
                return;
			}
            alarm(0);
            if (fork())
            {
                wait(NULL);
            }
            else
            {
                stderr = devnull;
                dup2(sock, fileno(stdout));
                argv[0] = "tar";
                argv[1] = "cf";
                argv[2] = "-";
                argv[3] = mapped;
                exit(pax_main(4, argv));
            }
            break;
#endif
#ifdef WANT_GZIP
		case DO_GZONLY:
			if (mapped)
            {
                free(mapped);
                mapped = NULL;
            }
            if ((phile = open(mapped, O_RDONLY)) < 0) {
				control_printf(SL_FAILURE, "553 Error: %s.", strerror(errno));
				return;
			}
			if (dataconn()) {
				if (mapped)
					free(mapped);
				return;
			}
            alarm(0);
            buffer = malloc(xfer_bufsize);
            /* check for alloc error */
            if (! buffer)
            {
               bftpd_log("Memory error while sending file.", 0);
               control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 0);
               if (phile) close(phile);
               return;
            }

            /* check buffer size based on number of connections */
            if (change_buffer_size)
            {
              num_clients = bftpdutmp_usercount("*");
              my_buffer_size = get_buffer_size(num_clients);
            }
            else
              my_buffer_size = xfer_bufsize;

            /* Use "wb9" for maximum compression, uses more CPU time... */
            gzfile = gzdopen(sock, "wb");
            while ((i = read(phile, buffer, my_buffer_size))) {
                gzwrite(gzfile, buffer, i);
                test_abort(1, phile, sock);
                if (change_buffer_size)
                {
                  new_num_clients = bftpdutmp_usercount("*");
                  if ( new_num_clients != num_clients )
                  {
                      num_clients = new_num_clients;
                      my_buffer_size = get_buffer_size(num_clients);
                  }
                }
                /* pause between transfers */
                if (xfer_delay)
                {
                    wait_time.tv_sec = 0;
                    wait_time.tv_usec = xfer_delay;
                    select( 0, NULL, NULL, NULL, &wait_time);
                }
            }
            free(buffer);
            close(phile);
            gzclose(gzfile);
            break;
#endif

#ifdef HAVE_ZLIB_H
            case DO_GZUNZIP:
               if ( dataconn() )
                  return;

               gzfile = gzdopen(phile, "rb");
               if (! gzfile)
               {
                   close(phile);
                   bftpd_log("Memory error while sending file.", 0);
                   control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 0);
                   return;
               }

               alarm(0);
               buffer = malloc(xfer_bufsize);
               if (! buffer)
               {
                  close(phile);
                  gzclose(gzfile);
                  bftpd_log("Memory error while sending file.", 0);
                  control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 0);
                  return;
               }

               /* check buffer size based on number of connections */
               if (change_buffer_size)
               {
                 num_clients = bftpdutmp_usercount("*");
                 my_buffer_size = get_buffer_size(num_clients);
               }
               else
                   my_buffer_size = xfer_bufsize;

               i = gzread(gzfile, buffer, my_buffer_size);
               while ( i )
               {
                   write(sock, buffer, i);
                   // test_abort(1, phile, sock);

                   if (change_buffer_size)
                   {
                     new_num_clients = bftpdutmp_usercount("*");
                     if ( new_num_clients != num_clients )
                     {
                        num_clients = new_num_clients;
                        my_buffer_size = get_buffer_size(num_clients);
                     }
                   }

                   /* pause between transfers */
                   if (xfer_delay)
                   {
                      wait_time.tv_sec = 0;
                      wait_time.tv_usec = xfer_delay;
                      select( 0, NULL, NULL, NULL, &wait_time);
                   }

                   i = gzread(gzfile, buffer, my_buffer_size);
               }   // end of while not end of file

               free(buffer);
               close(phile);
               gzclose(gzfile);
            break;    // send file and unzip on the fly
#endif

		case DO_NORMAL:
            /* used to be commented out */
			if (mapped)
            {
                free(mapped);
                mapped = NULL;
            }
			if (dataconn())
            {

    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(phile))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
            close(phile);
    //!解锁by w00190448@20120505 end    
                
                if (mapped)
                {
                    free(mapped);
                    mapped = NULL;
                }
                return;
            }

            alarm(0);
            lseek(phile, offset, SEEK_SET);
            offset = 0;

            if (change_buffer_size)
            {
                num_clients = bftpdutmp_usercount("*");
                my_buffer_size = get_buffer_size(num_clients);
            }
            else
            {
                my_buffer_size = xfer_bufsize;
            }

            //开始双线程操作:独立线程负责数据读取 ，主进程等待数据并发送 
            {
                int ret = 0; 
                pthread_t pid;
                pthread_attr_t thread_attr;

                /*初始化每次retr 操作需要的全局变量*/
                gFilefd = phile;
                gSock  = sock;
                gXfer_delay = xfer_delay;
                gBufSize = my_buffer_size;
                gSendIdx = 0;
                RdFileFinished= 0;
                RdFileThreadPid = -1;

                /*初始化信号量为为数据存储空间的数目-1*/ 
                ret=sem_init(&sem,0,MAXCOUNT-1);
                if(ret !=0)
                {
                    control_printf(SL_SUCCESS,"sem_init fail");
                    goto errout;
                }

                ret = RdDataBufInit();
                if(ret !=0)
                {
                    control_printf(SL_SUCCESS,"RdDataBufInit() fail");
                    goto errout;
                }

                /*创建文件读线程*/
                pthread_attr_init(&thread_attr);
                pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

                /*设置高优先级可能导致语音异常且对FTP速率提升几乎没有，暂时取消*/			  
                /*param.sched_priority = 50;
                pthread_attr_setschedpolicy(&thread_attr,SCHED_RR);		  
                pthread_attr_setschedparam(&thread_attr,&param);	
                */
                if( 0 != pthread_create(&pid, &thread_attr, (void *)ReadThread, 0))
                {
                    control_printf(SL_SUCCESS, "Create read file thread error !");                   	   
                    goto errout;
                }
                pthread_attr_destroy(&thread_attr);

                /*读取内存数据并发送*/
                //ret = SendData_new();
                ret = SendData();
                if(ret !=0)
                {
                    control_printf(SL_SUCCESS,"DoSend() fail");
                    goto errout;
                }

                /*释放局部资源*/
                RdDataBufUnInit() ;
                sem_destroy(&sem);
            }
            //双线程处理结束

            break;


        default:
            break;
    }

    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(phile))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end    

	   close(phile);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
           atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
           close(sock);
           offset = 0;
           alarm(control_timeout);
           control_printf(SL_SUCCESS, "226 File transmission successful.");
           bftpd_log("File transmission of '%s' successful.\n", filename);
           if (mapped)
           {
               free(mapped);
               mapped = NULL;
           }	
           return ;
		   
errout:
	    sem_destroy(&sem);
	    if( -1 != RdFileThreadPid)
	    {
	        kill(RdFileThreadPid,SIGKILL);
	    }	    	
           RdDataBufUnInit();

    //!解锁by w00190448@20120505 start
            
            if(-1 == unlock_file(phile))
            {
                bftpd_statuslog(2, 0,"Error while trying to unlock file [%s]\n",filename);
            }
    //!解锁by w00190448@20120505 end    

           close(phile);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
           atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
           close(sock);
            offset = 0;		    
           control_printf(SL_SUCCESS, "426 Transfer aborted.");
           control_printf(SL_SUCCESS, "226 Aborted.");
           bftpd_log("File transmission interrupted. Send failed.\n");
           //changed by qichenghong 20100211 for AU8D04022
           exit(0);//return;			
}



#if 0
void command_retr_original(char *filename)
{
        int num_clients = 1;
        int new_num_clients = 1;   /* number of connectiosn to the server */
        int my_buffer_size;       /* size of the transfer buffer to use */
	char *mapped = NULL;
	char *buffer;
        int xfer_delay;
        struct timeval wait_time;
        unsigned long get_value;
        ssize_t send_status;
        int change_buffer_size = FALSE;

#if (defined(WANT_GZIP) || defined(HAVE_ZLIB_H))
    gzFile gzfile;
#endif
	int phile;
	int i, whattodo = DO_NORMAL;
	struct stat statbuf;
#if (defined(WANT_TAR) && defined(WANT_GZIP))
    int filedes[2];
#endif
#if (defined(WANT_TAR) || defined(WANT_GZIP))
    char *foo;
#endif
#ifdef WANT_TAR
    char *argv[4];
#endif

        get_value = strtoul( config_getoption("XFER_DELAY"), NULL, 0);
        if (get_value <= INT_MAX)
           xfer_delay = get_value;
        else
        {
           bftpd_log("Error getting XFER_DELAY in command_retr().\n", 0);
           xfer_delay = 0;
        }

	mapped = bftpd_cwd_mappath(filename);
        if (! mapped)
        {
           bftpd_log("Memory error in sending file.\n", 0);
           control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 9);
           return;
        }

        if (! strcasecmp( config_getoption("CHANGE_BUFSIZE"), "yes") )
           change_buffer_size = TRUE;

	phile = open(mapped, O_RDONLY);
	if (phile == -1) {       // failed to open a file
#if (defined(WANT_TAR) && defined(WANT_GZIP))
		if ((foo = strstr(filename, ".tar.gz")))
			if (strlen(foo) == 7) {
				whattodo = DO_TARGZ;
				*foo = '\0';
			}
#endif
#ifdef WANT_TAR
		if ((foo = strstr(filename, ".tar")))
			if (strlen(foo) == 4) {
				whattodo = DO_TARONLY;
				*foo = '\0';
			}
#endif
#ifdef WANT_GZIP
		if ((foo = strstr(filename, ".gz")))
			if (strlen(foo) == 3) {
				whattodo = DO_GZONLY;
				*foo = '\0';
			}
#endif
		if (whattodo == DO_NORMAL) 
        {
			bftpd_log("Error: '%s' while trying to receive file '%s'.\n",
					  strerror(errno), filename);
			control_printf(SL_FAILURE, "553 Error: %s.", strerror(errno));
			if (mapped)
			{
				free(mapped);
				mapped = NULL;
			}
			return;
		}
	}

        #ifdef HAVE_ZLIB_H
        else  // we did open a file
        {
            char *my_temp;
            char *zip_option;

            my_temp = strstr(filename, ".gz");
            zip_option = config_getoption("GZ_DOWNLOAD");
            if (my_temp)
            {
               if ( ( strlen(my_temp) == 3) && (! strcasecmp(zip_option, "yes") ) )
                  whattodo = DO_GZUNZIP;
            }
        }
        #endif

	stat(mapped, (struct stat *) &statbuf);
	if (S_ISDIR(statbuf.st_mode)) 
    {
		control_printf(SL_FAILURE, "550 Error: Is a directory.");
		if (mapped)
		{
			free(mapped);
			mapped = NULL;
		}
		return;
	}
/*
	if ((((statbuf.st_size - offset) * ratio_send) / ratio_recv > bytes_recvd
		 - bytes_sent) && (strcmp((char *) config_getoption("RATIO"), "none"))) {
		bftpd_log("Error: 'File too big (ratio)' while trying to receive file "
				  "'%s'.\n", filename);
		control_printf(SL_FAILURE, "553 File too big. Send at least %lf bytes first.",
				(double) (((statbuf.st_size - offset) * ratio_send) / ratio_recv)
				- bytes_recvd);
		if (mapped)
			free(mapped);
		return;
	}
	*/
	bftpd_log("Client is receiving file '%s'.\n", filename);
	switch (whattodo) {
#if (defined(WANT_TAR) && defined(WANT_GZIP))
        case DO_TARGZ:
            close(phile);
            if (dataconn()) {
				if (mapped)
				{
					free(mapped);
					mapped = NULL;
				}
                return;
			}
            alarm(0);
            pipe(filedes);
            if (fork()) {
                buffer = malloc(xfer_bufsize);
                /* check to make sure alloc worked */
                if (! buffer)
                {
                   if (mapped)
                   {
                      free(mapped);
                      mapped = NULL;
                   }
                   bftpd_log("Memory error in sending file.\n", 0);
                   control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 9);
                   return;
                }

                /* find the size of the transfer buffer divided by number of connections */
                if (change_buffer_size)
                {
                   num_clients =  bftpdutmp_usercount("*");
                   my_buffer_size = get_buffer_size(num_clients);
                }
                else 
                  my_buffer_size = xfer_bufsize;
                
                close(filedes[1]);
                gzfile = gzdopen(sock, "wb");
                while ((i = read(filedes[0], buffer, my_buffer_size))) {
                    gzwrite(gzfile, buffer, i);
                    test_abort(1, phile, sock);

                    /* check for a change in number of connections */
                    if (change_buffer_size)
                    {
                       new_num_clients = bftpdutmp_usercount("*");
                       if (new_num_clients != num_clients)
                       {
                          num_clients = new_num_clients;
                          my_buffer_size = get_buffer_size(num_clients);
                       }
                    }
                    /* pause between transfers */
                    if (xfer_delay)
                    {
                        wait_time.tv_sec = 0;
                        wait_time.tv_usec = xfer_delay;
                        select( 0, NULL, NULL, NULL, &wait_time);
                    }
                }     // end of while 
                free(buffer);
                gzclose(gzfile);
                wait(NULL); /* Kill the zombie */
            } else {
                stderr = devnull;
                close(filedes[0]);
                close(fileno(stdout));
                dup2(filedes[1], fileno(stdout));
                setvbuf(stdout, NULL, _IONBF, 0);
                argv[0] = "tar";
                argv[1] = "cf";
                argv[2] = "-";
                argv[3] = mapped;
                exit(pax_main(4, argv));
            }
            break;
#endif
#ifdef WANT_TAR
		case DO_TARONLY:
            if (dataconn()) {
				if (mapped)
					free(mapped);
                return;
			}
            alarm(0);
            if (fork())
                wait(NULL);
            else {
                stderr = devnull;
                dup2(sock, fileno(stdout));
                argv[0] = "tar";
                argv[1] = "cf";
                argv[2] = "-";
                argv[3] = mapped;
                exit(pax_main(4, argv));
            }
            break;
#endif
#ifdef WANT_GZIP
		case DO_GZONLY:
			if (mapped)
                        {
				free(mapped);
                                mapped = NULL;
                        }
            if ((phile = open(mapped, O_RDONLY)) < 0) {
				control_printf(SL_FAILURE, "553 Error: %s.", strerror(errno));
				return;
			}
			if (dataconn()) {
				if (mapped)
					free(mapped);
				return;
			}
            alarm(0);
            buffer = malloc(xfer_bufsize);
            /* check for alloc error */
            if (! buffer)
            {
               bftpd_log("Memory error while sending file.", 0);
               control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 0);
               if (phile) close(phile);
               return;
            }

            /* check buffer size based on number of connections */
            if (change_buffer_size)
            {
              num_clients = bftpdutmp_usercount("*");
              my_buffer_size = get_buffer_size(num_clients);
            }
            else
              my_buffer_size = xfer_bufsize;

            /* Use "wb9" for maximum compression, uses more CPU time... */
            gzfile = gzdopen(sock, "wb");
            while ((i = read(phile, buffer, my_buffer_size))) {
                gzwrite(gzfile, buffer, i);
                test_abort(1, phile, sock);
                if (change_buffer_size)
                {
                  new_num_clients = bftpdutmp_usercount("*");
                  if ( new_num_clients != num_clients )
                  {
                      num_clients = new_num_clients;
                      my_buffer_size = get_buffer_size(num_clients);
                  }
                }
                /* pause between transfers */
                if (xfer_delay)
                {
                    wait_time.tv_sec = 0;
                    wait_time.tv_usec = xfer_delay;
                    select( 0, NULL, NULL, NULL, &wait_time);
                }
            }
            free(buffer);
            close(phile);
            gzclose(gzfile);
            break;
#endif

#ifdef HAVE_ZLIB_H
            case DO_GZUNZIP:
               if ( dataconn() )
                  return;

               gzfile = gzdopen(phile, "rb");
               if (! gzfile)
               {
                   close(phile);
                   bftpd_log("Memory error while sending file.", 0);
                   control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 0);
                   return;
               }

               alarm(0);
               buffer = malloc(xfer_bufsize);
               if (! buffer)
               {
                  close(phile);
                  gzclose(gzfile);
                  bftpd_log("Memory error while sending file.", 0);
                  control_printf(SL_FAILURE, "553 An unknown error occured on the server.", 0);
                  return;
               }

               /* check buffer size based on number of connections */
               if (change_buffer_size)
               {
                 num_clients = bftpdutmp_usercount("*");
                 my_buffer_size = get_buffer_size(num_clients);
               }
               else
                   my_buffer_size = xfer_bufsize;

               i = gzread(gzfile, buffer, my_buffer_size);
               while ( i )
               {
                   write(sock, buffer, i);
                   // test_abort(1, phile, sock);

                   if (change_buffer_size)
                   {
                     new_num_clients = bftpdutmp_usercount("*");
                     if ( new_num_clients != num_clients )
                     {
                        num_clients = new_num_clients;
                        my_buffer_size = get_buffer_size(num_clients);
                     }
                   }

                   /* pause between transfers */
                   if (xfer_delay)
                   {
                      wait_time.tv_sec = 0;
                      wait_time.tv_usec = xfer_delay;
                      select( 0, NULL, NULL, NULL, &wait_time);
                   }

                   i = gzread(gzfile, buffer, my_buffer_size);
               }   // end of while not end of file

               free(buffer);
               close(phile);
               gzclose(gzfile);
            break;    // send file and unzip on the fly
#endif

		case DO_NORMAL:
                       /* used to be commented out */			
			if (dataconn())
            {
                if (mapped)
                {
                    free(mapped);
                    mapped = NULL;
                }
                return;
            }
            
            if (mapped)
                        {
				free(mapped);
                                mapped = NULL;
                        }
                        alarm(0);
			lseek(phile, offset, SEEK_SET);
			offset = 0;
			buffer = malloc(xfer_bufsize * 2 + 1);
                        /* make sure buffer was created */
                        if (! buffer)
                        {
                            control_printf(SL_FAILURE, "553 An unknown error occured.");
                            bftpd_log("Memory error while trying to send file.", 0);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
                            atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
                            close(sock);
                            close(phile);
                            return;
                        }
                        if (change_buffer_size)
                        {
                          num_clients = bftpdutmp_usercount("*");
                          my_buffer_size = get_buffer_size(num_clients);
                        }
                        else
                            my_buffer_size = xfer_bufsize;

			while (1) {
			    i = read(phile, buffer, my_buffer_size);
			    if (i <= 0)
			    {
			        bftpd_statuslog(0, 0, "FS read err %d:%d.\n", i, errno);
			        break;
		        }
				if (test_abort(1, phile, sock)) {
					free(buffer);
					//changed by qichenghong 20100211 for AU8D04022 
					exit(0);//return;
					//changed by qichenghong 20100211 for AU8D04022 end
				}

                if (xfertype == TYPE_ASCII) {
                    buffer[i] = '\0';
                    i += replace(buffer, "\n", "\r\n", xfer_bufsize);
                }
				alarm(10);
				send_status = send(sock, buffer, i, 0);
                                // check for dropped connection
                                if (send_status < 0)
                                {
                                   free(buffer);
                                   close(phile);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
                                   atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
                                   close(sock);
                                   alarm(control_timeout);
                                   control_printf(SL_SUCCESS, "426 command_retr Transfer aborted.");
								   /*start l00163811 2010-8-4 用IE从服务器copy文件，然后取消，再copy的时候连不上。不返回226 */
//                                   			     control_printf(SL_SUCCESS, "226 Aborted.");								   
								   /*end l00163811 2010-8-4 用IE从服务器copy文件，然后取消，再copy的时候连不上。不返回226 */
                                   bftpd_log("File transmission interrupted. Send failed.\n");
                                   return;
                                }

				bytes_sent += i;
              
                                if (change_buffer_size)
                                {
                                  new_num_clients = bftpdutmp_usercount("*");
                                  my_buffer_size = get_buffer_size(num_clients);
                                }

                                /* pause between transfers */
                                if (xfer_delay)
                                {
                                   wait_time.tv_sec = 0;
                                   wait_time.tv_usec = xfer_delay;
                                   select( 0, NULL, NULL, NULL, &wait_time);
                                }

			}       // end of while
            free(buffer);
            break;
            
        default:
            break;
            }

	close(phile);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
	close(sock);
        offset = 0;
        alarm(control_timeout);
	control_printf(SL_SUCCESS, "226 File transmission successful.");
	bftpd_log("File transmission of '%s' successful.\n", filename);
		    if (mapped)
		    {
		        free(mapped);
                mapped = NULL;
		    }
}
#endif

void command_retr(char *filename)
{
    command_retr_2thread(filename);
	//command_retr_original(filename);   
}

void do_dirlist(char *dirname, char verbose)
{
	FILE *datastream;
       int bshow_hiden_filese=FALSE;
       
       /*解决问题单AU4D02503，在此获得-a参数*/
        /******************filter parameter start************************/
        if (dirname[0] != '\0') {
        	/* skip arguments */
                if (dirname[0] == '-') {
                    while ((dirname[0] != ' ') && (dirname[0] != '\0'))
                    {         
                        dirname++;
                        if(('a' == *dirname) && ((dirname[0] != ' ') || (dirname[0] != '\0')))
                        {
                            bshow_hiden_filese=TRUE;
                        }
                     }
                    if (dirname[0] != '\0')
                    {
                        dirname++;
                    }
        	}
        }
        /******************filter parameter end************************/
        
        if (dataconn())
        {   
		return;
        }
        alarm(0);
        datastream = fdopen(sock, "w");
        if (dirname[0] == '\0')
        {   
        	dirlist("*", datastream, verbose,bshow_hiden_filese);
        }
        else
        {
        	char *mapped = bftpd_cwd_mappath(dirname);
        	dirlist(mapped, datastream, verbose,bshow_hiden_filese);
        	free(mapped);
        }
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        atp_ftpd_ssl_context_free_obj( &pstssldataobj );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
        fclose(datastream);
        alarm(control_timeout);
        control_printf(SL_SUCCESS, "226 Directory list has been submitted.");
}

void command_list(char *dirname)
{
	do_dirlist(dirname, 1);
}

void command_nlst(char *dirname)
{
	do_dirlist(dirname, 0);
}

void command_syst(char *params)
{
	control_printf(SL_SUCCESS, "215 UNIX Type: L8");
}

void command_mdtm(char *filename)
{
	struct stat statbuf;
	struct tm *filetime = NULL;
	char *fullfilename = bftpd_cwd_mappath(filename);
	if (fullfilename)
	{
		if (!stat(fullfilename, (struct stat *) &statbuf)) {
	/*Start of ATP网络协议组 2010-4-1 14:34 forAU4D02426  by h00163136*/
	/*修正问题单AU4D02426后,系统时间正确后，不需该调整*/
#if 0
		         /*start of AU4D02390 ftp显示的时间差一年, by hh00163136, 20100208*/
		         statbuf.st_mtime -= g_utc_offset;
		         /*end of AU4D02390 ftp显示的时间差一年, by hh00163136, 20100208*/
#endif
	/*End of ATP网络协议组 2010-4-1 14:34 by h00163136*/
			filetime = gmtime((time_t *) & statbuf.st_mtime);
			if (filetime)
			{      
				control_printf(SL_SUCCESS, "213 %04i%02i%02i%02i%02i%02i",
						filetime->tm_year + 1900, filetime->tm_mon + 1,
						filetime->tm_mday, filetime->tm_hour, filetime->tm_min,
						filetime->tm_sec);
			}
		} else {
			control_printf(SL_FAILURE, "550 Error while determining the modification time: %s",
					strerror(errno));
		}
		free(fullfilename);
	}
}

void command_cwd(char *dir)
{
    if (bftpd_cwd_chdir(dir)) {
		bftpd_log("Error: '%s' while changing directory to '%s'.\n",
				  strerror(errno), dir);
		control_printf(SL_FAILURE, "451 Error: %s.", strerror(errno));
	} else {
		bftpd_log("Changed directory to '%s'.\n", dir);
		control_printf(SL_SUCCESS, "250 OK");
	}
}

void command_cdup(char *params)
{
	bftpd_log("Changed directory to '..'.\n");
	bftpd_cwd_chdir("..");
	control_printf(SL_SUCCESS, "250 OK");
}

void command_dele(char *filename)
{
	char *mapped = bftpd_cwd_mappath(filename);

//!检查锁added by w00190448@20120505 start
        int fd;
        fd = open(mapped, O_RDONLY);
        if (fd >= 0)  /* file exists */
        {
            if( -1 == checklock_file(fd, F_WRLCK))
            {
                bftpd_statuslog(2, 0, "Error: file [%s] is locked by other progress.\n", filename);
                close(fd);

                control_printf(SL_FAILURE, "451 Error: file [%s] is in use", filename);
                return;
                
            }
            close(fd);
        }        
//!检查锁added by w00190448@20120505 end

    if (pre_write_script)
       run_script(pre_write_script, mapped);       

	if (unlink(mapped)) {
		bftpd_log("Error: '%s' while trying to delete file '%s'.\n",
				  strerror(errno), filename);
		control_printf(SL_FAILURE, "451 Error: %s.", strerror(errno));
	} else {
		bftpd_log("Deleted file '%s'.\n", filename);
		control_printf(SL_SUCCESS, "200 OK");
	}

        if (post_write_script)
           run_script(post_write_script, mapped);

	free(mapped);
}

void command_mkd(char *dirname)
{
	char *mapped = bftpd_cwd_mappath(dirname);
    int   nIsValidFilename = 0;
	int lDirCreateMode = BFTPD_DIR_CREATEMODE;
	struct stat stbuf;

    /*Added by yehuisheng00183935@20111104 修改问题单(DTS2011092702151) */
    filtrateBlankCharaterInFilename( mapped );
    /*Added by yehuisheng00183935@20111104 修改问题单(DTS2011092702151) */

    if ( 1 == iCheckMemCreate)
    {
         if ( checkdiskfree(mapped) < 0 )
         {
             control_printf(SL_FAILURE, "452 Error: memory or disk space is not enough.");

             if(mapped)
                free(mapped);
             
             return;
         }
    }

    nIsValidFilename =  IsValidFilename(mapped,dirname,248);
    
    if(1  != nIsValidFilename)
    {
        if(0 == nIsValidFilename)
        {
            control_printf(SL_FAILURE, "451 Error: Can not create directory longer than 248 characters.");
        }
        else if(-1 == nIsValidFilename)
        {
            control_printf(SL_FAILURE, "451 Error: Directory can not include these characters:\\/:*?\"<>|.");        
        }
        else if(-2 == nIsValidFilename)     /*Added by yehuisheng00183935@20110922 修改问题单(DTS2011083103592)*/
        {
            control_printf(SL_FAILURE, "450 Error: Requested file action not taken.");        
        }

        if(mapped)
           free(mapped);
        
        return;
    }

    if (pre_write_script)
       run_script(pre_write_script, mapped);

    if (mkdir(mapped, lDirCreateMode)) {
		bftpd_log("Error: '%s' while trying to create directory '%s'.\n",
				  strerror(errno), dirname);
		control_printf(SL_FAILURE, "451 Error: %s.", strerror(errno));
	} else {
            //added by qichenghong 20100227 for 解决FAT/NTFS文件系统创建文件和文件夹时无法设置mode
            if(-1 != stat(mapped , &stbuf))
            {
                if((stbuf.st_mode & 00777) != lDirCreateMode)
                {
                    chmod(mapped , (stbuf.st_mode & ~(00777)) | lDirCreateMode);
                }
            }
            //added by qichenghong 20100227 for 解决FAT/NTFS文件系统创建文件和文件夹时无法设置mode end
		bftpd_log("Created directory '%s'.\n", dirname);
		control_printf(SL_SUCCESS, "257 \"%s\" has been created.", dirname);
	}

    if (post_write_script)
       run_script(post_write_script, mapped);

    if(mapped)
	    free(mapped);
}

void command_rmd(char *dirname)
{
	char *mapped = bftpd_cwd_mappath(dirname);

    if (pre_write_script)
       run_script(pre_write_script, mapped);

    if ( 1 == iCheckMemCreate)
    {
         if ( checkdiskfree(mapped) < 0 )
         {
             control_printf(SL_FAILURE, "451 Error: Forbid removing file(or directory) outside the root and disk directory.");
    
             if(mapped)
                free(mapped);
    
             return;
         }
    }

 
	if (rmdir(mapped)) {
		bftpd_log("Error: '%s' while trying to remove directory '%s'.\n", strerror(errno), dirname);
		control_printf(SL_FAILURE, "451 Error: %s.", strerror(errno));
	} else {
		bftpd_log("Removed directory '%s'.\n", dirname);
		control_printf(SL_SUCCESS, "250 OK");
	}
        
        if (post_write_script)
           run_script(post_write_script, mapped);
	free(mapped);
}

void command_noop(char *params)
{
	control_printf(SL_SUCCESS, "200 OK");
}

void command_rnfr(char *oldname)
{
    FILE *file;
    char *mapped = bftpd_cwd_mappath(oldname);
    int fd;
    
    if ( 1 == iCheckMemCreate)
    {
         if ( checkdiskfree(mapped) < 0 )
         {
             control_printf(SL_FAILURE, "451 Error: Forbid changing the file(or directory) name outside the root and disk directory.");
    
             if(mapped)
                free(mapped);
    
             return;
         }
    }

//!DTS2012062105095:检查锁added by w00190448@20120505 start
        fd = open(mapped, O_RDONLY);
        if (fd >= 0)  /* file exists */
        {
            if( -1 == checklock_file(fd, F_WRLCK))
            {
                bftpd_statuslog(2, 0, "Error: file [%s] is locked by other progress.\n", oldname);
                close(fd);

                control_printf(SL_FAILURE, "451 Error: file [%s] is in use", oldname);
                return;
                
            }
            close(fd);
        }        
//!DTS2012062105095:检查锁added by w00190448@20120505 end


        
	if ((file = fopen(mapped, "r"))) {
		fclose(file);
		if (philename)
			free(philename);
		philename = mapped;
		state = STATE_RENAME;
		control_printf(SL_SUCCESS, "350 File exists, ready for destination name");
	} else {
		free(mapped);
		control_printf(SL_FAILURE, "451 Error: %s.", strerror(errno));
	}
}

void command_rnto(char *newname)
{
    FILE *file;
    char *mapped = bftpd_cwd_mappath(newname);
    int  nIsValidFilename;
    char *pcwd = NULL;

    /*Added by yehuisheng00183935@20111104 修改问题单(DTS2011092702151) */
    filtrateBlankCharaterInFilename( mapped );
    /*Added by yehuisheng00183935@20111104 修改问题单(DTS2011092702151) */

    nIsValidFilename =  IsValidFilename(mapped,newname,512);
    
    if(1  != nIsValidFilename)
    {
        if(0 == nIsValidFilename)
        {
            control_printf(SL_FAILURE, "451 Error: Can not longer than 512 characters.");
        }
        else if(-1 == nIsValidFilename)
        {
            control_printf(SL_FAILURE, "451 Error: Can not include these characters:\\/:*?\"<>|.");        
        }
        else if(-2 == nIsValidFilename)     /*Added by yehuisheng00183935@20110922 修改问题单(DTS2011083103592)*/
        {
            control_printf(SL_FAILURE, "450 Error: Requested file action not taken.");        
        }

        if(mapped)
           free(mapped);
        
        return;
    }

    pcwd = bftpd_cwd_getcwd();
    /*Added by yehuisheng00183935@20110927 修改问题单(DTS2011082906100)*/
    if( pcwd != NULL && strlen( pcwd ) > 0 && strlen( newname ) > 0 && 0 == strcmp( mapped, newname ) )
    {
        if( 0 == strncmp( newname, pcwd, strlen( pcwd ) ) )     /* 在当前目录下进行处理，全路径名 */
        {
            char * pTmp = strstr( newname, pcwd );
            char qTmp = pcwd[strlen(pcwd)-1];     /* 获得最后一个字符 */
            int iCwdLen = strlen( pcwd );

            if( pTmp != NULL )
            {
                if( '/' == qTmp )
                {
                    pTmp = pTmp + iCwdLen;
                }
                else
                {
                    pTmp = pTmp + iCwdLen + 1;
                }

                if( pTmp < ( newname + strlen( newname ) ) )
                {
                    if( strchr( pTmp, '/' ) != NULL )   /* 新文件名中包含'/' 非法字符 */
                    {
                        control_printf(SL_FAILURE, "451 Error: Can not include these characters:\\/:*?\"<>|.");

                        if(mapped)
                        {
                            free(mapped);
                        }
                        free(pcwd);
                        pcwd = NULL;
                        return;
                    }
                }
            }
        }
        else        /* 非当前目录下处理，相对路径名 */
        {
            if( strchr( newname, '/' ) != NULL )   /* 新文件名中包含'/' 非法字符 */
            {
                control_printf(SL_FAILURE, "451 Error: Can not include these characters:\\/:*?\"<>|.");
            
                if(mapped)
                {
                    free(mapped);
                }
                free(pcwd);
                pcwd = NULL;
                return;
            }
        }
    }
    /*Added by yehuisheng00183935@20110927 修改问题单(DTS2011082906100)*/
    
    if (pcwd)
    {
        free(pcwd);
        pcwd = NULL;
    }

/*Added by yehuisheng00183935@20111105 修改问题单(DTS2011092702151)*/
#if 0
    if ((file = fopen(mapped, "r"))) 
#else
    if ( strcmp( philename, mapped ) != 0 && ( file = fopen( mapped, "r" ) ) )
#endif
/*Added by yehuisheng00183935@20111105 修改问题单(DTS2011092702151)*/
    {
        control_printf(SL_FAILURE, "550 Error: file exists.");
        fclose(file);
        free(mapped);
        if (philename)
        free(philename);
         philename = NULL;        
         state = STATE_AUTHENTICATED;
         return;
    }
    else
    {
    }
        if (pre_write_script)
           run_script(pre_write_script, mapped);

	if (rename(philename, mapped)) {
		bftpd_log("Error: '%s' while trying to rename '%s' to '%s'.\n",
				  strerror(errno), philename, bftpd_cwd_mappath(newname));
		control_printf(SL_FAILURE, "451 Error: %s.", strerror(errno));
	} else {
		bftpd_log("Successfully renamed '%s' to '%s'.\n", philename, bftpd_cwd_mappath(newname));
		control_printf(SL_SUCCESS, "250 OK");
		state = STATE_AUTHENTICATED;
	}

        if (post_write_script)
           run_script(post_write_script, mapped);

	free(philename);
	free(mapped);
	philename = NULL;
}

void command_rest(char *params)
{
    offset = strtoul(params, NULL, 10);
	control_printf(SL_SUCCESS, "350 Restarting at offset %i.", offset);
}

void command_size(char *filename)
{
	struct stat statbuf;
	char *mapped = bftpd_cwd_mappath(filename);
	if (!stat(mapped, &statbuf)) {
        if(S_ISDIR(statbuf.st_mode))
        {
            control_printf(SL_FAILURE, "550 Error: Is a directory.");
        }
        else
        {
            control_printf(SL_SUCCESS, "213 %i", (int) statbuf.st_size);
        }
	} else {
		control_printf(SL_FAILURE, "550 Error: %s.", strerror(errno));
	}
	free(mapped);
}

void command_quit(char *params)
{
	control_printf(SL_SUCCESS, "221 %s", config_getoption("QUIT_MSG"));
        /* Make sure we log user out. -- Jesse <slicer69@hotmail.com> */
        bftpdutmp_end();
	exit(0);
}

void command_stat(char *filename)
{
	char *mapped = bftpd_cwd_mappath(filename);
    control_printf(SL_SUCCESS, "213-Status of %s:", filename);
    bftpd_stat(mapped, stderr);
    control_printf(SL_SUCCESS, "213 End of Status.");
	free(mapped);
}

/* SITE commands */

void command_chmod(char *params)
{
	int permissions;
	char *mapped;
        char *my_string;

	if (!strchr(params, ' ')) {
		control_printf(SL_FAILURE, "550 Usage: SITE CHMOD <permissions> <filename>");
		return;
	}
        my_string = strdup(strchr(params, ' ') + 1);
        if (! my_string)
        {
            control_printf(SL_FAILURE, "550: An error occured on the server trying to CHMOD.");
            return;
        }
	/* mapped = bftpd_cwd_mappath(strdup(strchr(params, ' ') + 1)); */
        mapped = bftpd_cwd_mappath(my_string);
        free(my_string);
       
        if (pre_write_script)
           run_script(pre_write_script, mapped);

	*strchr(params, ' ') = '\0';
	sscanf(params, "%o", &permissions);
	if (chmod(mapped, permissions))
		control_printf(SL_FAILURE, "Error: %s.", strerror(errno));
	else {
		bftpd_log("Changed permissions of '%s' to '%o'.\n", mapped,
				  permissions);
		control_printf(SL_SUCCESS, "200 CHMOD successful.");
	}
        if (post_write_script)
           run_script(post_write_script, mapped);

	free(mapped);
}

void command_chown(char *params)
{
	char foo[MAXCMD + 1], owner[MAXCMD + 1], group[MAXCMD + 1],
		filename[MAXCMD + 1], *mapped;
	int uid, gid;
	if (!strchr(params, ' ')) {
		control_printf(SL_FAILURE, "550 Usage: SITE CHOWN <owner>[.<group>] <filename>");
		return;
	}
	sscanf(params, "%[^ ] %s", foo, filename);
	if (strchr(foo, '.'))
		sscanf(foo, "%[^.].%s", owner, group);
	else {	
		/*Added by lvxin00135113@20110425 安全代码整改*/
		#if 0
		strcpy(owner, foo);
		#else
        snprintf(owner, sizeof(owner), "%s",foo);
		#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
		group[0] = '\0';
	}
	if (!sscanf(owner, "%i", &uid))	/* Is it a number? */
		if (((uid = mygetpwnam(owner, passwdfile))) < 0) {
			control_printf(SL_FAILURE, "550 User '%s' not found.", owner);
			return;
		}
	if (!sscanf(group, "%i", &gid))
		if (((gid = mygetpwnam(group, groupfile))) < 0) {
			control_printf(SL_FAILURE, "550 Group '%s' not found.", group);
			return;
		}
	mapped = bftpd_cwd_mappath(filename);
        if (pre_write_script)
           run_script(pre_write_script, mapped);

	if (chown(mapped, uid, gid))
		control_printf(SL_FAILURE, "550 Error: %s.", strerror(errno));
	else {
		bftpd_log("Changed owner of '%s' to UID %i GID %i.\n", filename, uid,
				  gid);
		control_printf(SL_SUCCESS, "200 CHOWN successful.");
	}
        if (post_write_script)
           run_script(post_write_script, mapped);
	free(mapped);
}

#if 0
/*
Send the md5sum check of a given file to the
client.
*/
void command_md5(char *philename)
{
    FILE *myphile;
    md5_t my_md5;
    char buffer[1024];
    int bytes_read = 0;
    char md5_result[16];
    char output_string[64];

    if (! philename)
    {
       control_printf(SL_FAILURE, "550 File not found.");
       return;
    }

    philename++;
    myphile = fopen(philename, "r");
    if (! myphile)
    {
        control_printf(SL_FAILURE, "550 Unable to open file `%s'.", philename);
        return;
    }

    md5_init(&my_md5);
    bytes_read = fread(buffer, sizeof(char), 1024, myphile);
    while (bytes_read > 0)
    {
        md5_process(&my_md5, buffer, bytes_read);
        bytes_read = fread(buffer, sizeof(char), 1024, myphile);        
    }

    md5_finish(&my_md5, md5_result);
    md5_sig_to_string(md5_result, output_string, sizeof(output_string) );
    control_printf(SL_SUCCESS, "200 MD5 finger print: %s.", output_string);
    fclose(myphile);
}
#endif

void command_site(char *str)
{
	const struct command subcmds[] = {
		{"chmod ", NULL, command_chmod, STATE_AUTHENTICATED},
		{"chown ", NULL, command_chown, STATE_AUTHENTICATED},
               // {"md5", NULL, command_md5, STATE_AUTHENTICATED},
		{NULL, NULL, 0}
	};
	int i;
	if (!strcasecmp(config_getoption("ENABLE_SITE"), "no")) {
		control_printf(SL_FAILURE, "550 SITE commands are disabled.");
		return;
	}
	for (i = 0; subcmds[i].name; i++) {
		if (!strncasecmp(str, subcmds[i].name, strlen(subcmds[i].name))) {
			cutto(str, strlen(subcmds[i].name));
			subcmds[i].function(str);
			return;
		}
	}
        /* see if the user needs help */
        if (! strcasecmp(str, "help") )
        {
            control_printf(SL_SUCCESS, "211-Possible usages for SITE command:");
            control_printf(SL_SUCCESS, "211-site chmod <mask> <filename>");
            control_printf(SL_SUCCESS, "211-site chown <owner> <filename>");
            control_printf(SL_SUCCESS, "211 site md5 <filename>");
            return;
        }
	control_printf(SL_FAILURE, "550 Unknown command: 'SITE %s'.", str);
}


/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS

/******************************************************************************
  函数名称  : auth_prot
  功能描述  : 执行prot命令响应客户端请求，用于切换到保护通道模式
  输入参数  : char *cmd
             
  输出参数  :  
  返 回 值  :无

  修改历史     :
  1.日    期   : 2011-7-2
    作    者   : y00183935
    修改内容   : 新创建函数
******************************************************************************/
void command_prot(char *cmd)
{
    if ( cmd != NULL )
    {
        if ( pstsslcommandobj != NULL )
        {
            if ( strcasecmp( cmd, "C" ) == 0 )
            {
                ssl_data_protect = UNSECURED;
				is_proted =1;
                control_printf( SL_SUCCESS, "200 Protection level set to C" );
            }
            else if ( strcasecmp( cmd, "P" ) == 0 )
            {
                ssl_data_protect = SECURED;
				is_proted =1;
                control_printf( SL_SUCCESS, "200 Protection level set to P" );
            }
            else
            {
                control_printf( SL_FAILURE, "504 Command not implemented for that parameter." );
            }
        }
        else
        {
            control_printf( SL_FAILURE, "503 Bad sequence of commands." );
        }
    }
    else
    {
        control_printf( SL_FAILURE, "501 Invalid number of parameters." );
    }
}


/******************************************************************************
  函数名称  : auth_pbsz
  功能描述  : 执行pbsz 命令响应客户端请求
  输入参数  : char *cmd
             
  输出参数  :  
  返 回 值  :无

  修改历史     :
  1.日    期   : 2011-7-2
    作    者   : y00183935
    修改内容   : 新创建函数
******************************************************************************/
void command_pbsz(char *cmd)
{
    if ( cmd != NULL )
    {
        if ( pstsslcommandobj != NULL )
        {
            control_printf( SL_SUCCESS, "200 Command okay." );
        }
        else
        {
            control_printf( SL_FAILURE, "503 Bad sequence of commands." );
        }
    }
    else
    {
        control_printf( SL_FAILURE, "501 Invalid number of parameters." );
    }
}


/******************************************************************************
  函数名称  : command_auth
  功能描述  : 响应客户端AUTH 命令进行SSL 握手协商
  输入参数  : 无
             
  输出参数  :  
  返 回 值  :无
  修改历史     :
  1.日    期   : 2011-7-2
    作    者   : y00183935
    修改内容   : 新创建函数
******************************************************************************/
void command_auth(char *cmd)
{
    if (cmd != NULL)
    {
        if ( strcasecmp( cmd, "TLS" ) == 0 )        /* 使用显示模式的ftps 传输 */
        {
            int iExplicitFlag = UNSECURED;

            control_printf( SL_FAILURE, "234 Using authentication type TLS" );      /* 先回包，如果加密不成功，后面会close连接。 */

            if ( pstctx != NULL && NULL == pstsslcommandobj )      /* pstsslcommandobj 应该尚未分配内容，如果已经分配，说明出错了，很可能内容泄露。 */
            {
                pstsslcommandobj = SSL_new( pstctx );

                if( pstsslcommandobj != NULL )
                {
                    SSL_set_fd( pstsslcommandobj, ctlsock );

                    if ( SSL_accept( ctlsock, pstctx, pstsslcommandobj ) >= 0 )
                    {
                        iExplicitFlag = SECURED;
                    }
                }
            }

            if ( UNSECURED == iExplicitFlag )
            {
                if( pstsslcommandobj != NULL )
                {
                    SSL_free( pstsslcommandobj );
                    pstsslcommandobj = NULL;
                }

                close( ctlsock );
            }
        }
        else
        {
            control_printf( SL_FAILURE, "504 Command not implemented for that parameter." );
        }
    }
    else
    {
        control_printf( SL_FAILURE, "501 Invalid number of parameters." );
    }

}

#else

void command_auth(char *type)
{
    control_printf(SL_FAILURE, "550 Not implemented yet");
}

#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/


/* Command parsing */

const struct command commands[] = {
	{"USER", "<sp> username", command_user, STATE_CONNECTED, 0},
	{"PASS", "<sp> password", command_pass, STATE_USER, 0},
    {"XPWD", "(returns cwd)", command_pwd, STATE_AUTHENTICATED, 1},
	{"PWD", "(returns cwd)", command_pwd, STATE_AUTHENTICATED, 0},
	{"TYPE", "<sp> type-code (A or I)", command_type, STATE_AUTHENTICATED, 0},
	{"PORT", "<sp> h1,h2,h3,h4,p1,p2", command_port, STATE_AUTHENTICATED, 0},
    {"EPRT", "<sp><d><net-prt><d><ip><d><tcp-prt><d>", command_eprt, STATE_AUTHENTICATED, 1},
	{"PASV", "(returns address/port)", command_pasv, STATE_AUTHENTICATED, 0},
    {"EPSV", "(returns address/post)", command_epsv, STATE_AUTHENTICATED, 1},
    {"ALLO", "<sp> size", command_allo, STATE_AUTHENTICATED, 1},
	{"STOR", "<sp> pathname", command_stor, STATE_AUTHENTICATED, 0},
    {"APPE", "<sp> pathname", command_appe, STATE_AUTHENTICATED, 1},
	{"RETR", "<sp> pathname", command_retr, STATE_AUTHENTICATED, 0},
	{"LIST", "[<sp> pathname]", command_list, STATE_AUTHENTICATED, 0},
	{"NLST", "[<sp> pathname]", command_nlst, STATE_AUTHENTICATED, 0},
	{"SYST", "(returns system type)", command_syst, STATE_CONNECTED, 0},
	{"MDTM", "<sp> pathname", command_mdtm, STATE_AUTHENTICATED, 1},
    {"XCWD", "<sp> pathname", command_cwd, STATE_AUTHENTICATED, 1},
	{"CWD", "<sp> pathname", command_cwd, STATE_AUTHENTICATED, 0},
    {"XCUP", "(up one directory)", command_cdup, STATE_AUTHENTICATED, 1},
	{"CDUP", "(up one directory)", command_cdup, STATE_AUTHENTICATED, 0},
	{"DELE", "<sp> pathname", command_dele, STATE_AUTHENTICATED, 0},
    {"XMKD", "<sp> pathname", command_mkd, STATE_AUTHENTICATED, 1},
	{"MKD", "<sp> pathname", command_mkd, STATE_AUTHENTICATED, 0},
    {"XRMD", "<sp> pathname", command_rmd, STATE_AUTHENTICATED, 1},
	{"RMD", "<sp> pathname", command_rmd, STATE_AUTHENTICATED, 0},
	{"NOOP", "(no operation)", command_noop, STATE_AUTHENTICATED, 0},
	{"RNFR", "<sp> pathname", command_rnfr, STATE_AUTHENTICATED, 0},
	{"RNTO", "<sp> pathname", command_rnto, STATE_RENAME, 0},
	{"REST", "<sp> byte-count", command_rest, STATE_AUTHENTICATED, 1},
	{"SIZE", "<sp> pathname", command_size, STATE_AUTHENTICATED, 1},
	{"QUIT", "(close control connection)", command_quit, STATE_CONNECTED, 0},
	{"HELP", "[<sp> command]", command_help, STATE_AUTHENTICATED, 0},
    {"STAT", "<sp> pathname", command_stat, STATE_AUTHENTICATED, 0},
#ifndef SUPPORT_ATP_FTP_NO_SITE
	{"SITE", "<sp> string", command_site, STATE_AUTHENTICATED, 0},
#endif
    {"FEAT", "(returns list of extensions)", command_feat, STATE_AUTHENTICATED, 0},
    {"OPTS", "<sp> string <sp> val", command_opts, STATE_AUTHENTICATED, 0},
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    {"AUTH", "<sp> authtype", command_auth, STATE_CONNECTED, 1},
    {"PBSZ","<sp> command PBSZ", command_pbsz, STATE_CONNECTED, 1},
    {"PROT","<sp> auth port ", command_prot,STATE_CONNECTED, 1},
#else
/*    {"AUTH", "<sp> authtype", command_auth, STATE_CONNECTED, 0},*/
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
    {"ADMIN_LOGIN", "(admin)", command_adminlogin, STATE_CONNECTED, 0},
      {"MGET", "<sp> pathname", command_mget, STATE_AUTHENTICATED, 0},
      {"MPUT", "<sp> pathname", command_mput, STATE_AUTHENTICATED, 0},
	{NULL, NULL, NULL, 0, 0}
};

void command_feat(char *params)
{
    int i;
    control_printf(SL_SUCCESS, "211-Extensions supported:");
    for (i = 0; commands[i].name; i++)
        if (commands[i].showinfeat)
            control_printf(SL_SUCCESS, " %s", commands[i].name);

#ifndef SUPPORT_ATP_USBMOUNT_CHARACTER
    {
        control_printf(SL_SUCCESS, " UTF8");
    }
#endif

    control_printf(SL_SUCCESS, "211 End");
}

void command_opts(char *params)
{
    if (! strcasecmp(params, "utf8 on") )
    {
#ifdef SUPPORT_ATP_USBMOUNT_CHARACTER
        control_printf(SL_SUCCESS, "550 %s not implemented", params);
#else
        control_printf(SL_SUCCESS, "200 %s", params );
#endif
    }
    else
    {
        control_printf(SL_SUCCESS, "550 %s not implemented", params);
    }
}


void command_help(char *params)
{
	int i;
	if (params[0] == '\0') {
		control_printf(SL_SUCCESS, "214-The following commands are recognized.");
		for (i = 0; commands[i].name; i++)
			control_printf(SL_SUCCESS, "214-%s", commands[i].name);
        control_printf(SL_SUCCESS, "214 End of help");
	} else {
		for (i = 0; commands[i].name; i++)
			if (!strcasecmp(params, commands[i].name))
				control_printf(SL_SUCCESS, "214 Syntax: %s", commands[i].syntax);
	}
}

int setctlsocket(int socket)
{
	ctlsock = socket;
       return 0;
}

int parsecmd(char *str)
{
	int i;
	char *p, *pp, confstr[64]; /* strlen("ALLOWCOMMAND_XXXX") + 1 == 18 */
	p = pp = str;			/* Remove garbage in the string */
	while (*p)
		if ((unsigned char) *p < 32)
			p++;
		else
			*pp++ = *p++;
	*pp++ = 0;
	for (i = 0; commands[i].name; i++) {	/* Parse command */
		if (!strncasecmp(str, commands[i].name, strlen(commands[i].name))) {
				/*Added by lvxin00135113@20110425 安全代码整改*/
				#if 0
            	sprintf(confstr, "ALLOWCOMMAND_%s", commands[i].name);
				#else
                snprintf(confstr,sizeof(confstr),"ALLOWCOMMAND_%s", commands[i].name);
				#endif
				/*Added by lvxin00135113@20110425 安全代码整改*/
            if (!strcasecmp(config_getoption(confstr), "no")) {
                control_printf(SL_FAILURE, "550 The command '%s' is disabled.",
                        commands[i].name);
                return 1;
            }
#if defined(SUPPORT_LOGIN_CONTROL)&&defined (SUPPORT_ATP_FTP_ACCESS_CONTROL)//y00181549 for 根据ftp类型细分权限控制 at 2012.6.10
	if((!strncasecmp("PORT", commands[i].name, strlen(commands[i].name)))
		||(!strncasecmp("EPRT", commands[i].name, strlen(commands[i].name)))
		||(!strncasecmp("PASV", commands[i].name, strlen(commands[i].name)))
		||(!strncasecmp("EPSV", commands[i].name, strlen(commands[i].name))))
	{
              // isAccFromlan = IsAccessFromLan(&remotename);
		if((1 == is_proted)
		   &&(VOS_OK != ATP_LoginCtlCheckFtpAcc(user,ATP_FTPTYPE_FTPSEXPLICIT,isAccFromlan)))
		{
			control_printf(SL_FAILURE, "550 The command '%s' is forbidden, send 'PROT' first.",
				                commands[i].name);
			return 1;
		}
	}
	if(!strncasecmp("AUTH", commands[i].name, strlen(commands[i].name)))
	{
		ulisFtpsExCtl = VOS_TRUE;
	}
#endif
			cutto(str, strlen(commands[i].name));
			p = str;
			while ((*p) && ((*p == ' ') || (*p == '\t')))
				p++;
			memmove(str, p, strlen(str) - (p - str) + 1);
			if (state >= commands[i].state_needed) {
				commands[i].function(str);
				return 0;
			} else {
				switch (state) {
					case STATE_CONNECTED: {
						control_printf(SL_FAILURE, "503 USER expected.");
						return 1;
					}
					case STATE_USER: {
						control_printf(SL_FAILURE, "503 PASS expected.");
						return 1;
					}
					case STATE_AUTHENTICATED: {
						control_printf(SL_FAILURE, "503 RNFR before RNTO expected.");
						return 1;
					}
				}
			}
		}
	}
	control_printf(SL_FAILURE, "500 Unknown command: \"%s\"", str);
	return 0;
}


int get_buffer_size(int num_connections)
{
   int buffer_size;

   if (num_connections < 1)
      num_connections = 1;

   buffer_size = xfer_bufsize / num_connections;
   if ( buffer_size < 2)
      buffer_size = 2;

   return buffer_size;
}



/*
This function forks and runs a script. On success it
returns TRUE, if an error occures, it returns FALSE.
*/
int run_script(char *script, char *path)
{
   pid_t process_id;
   char *command_args[] = { script, path, NULL } ;
  // sighandler_t save_quit, save_int, save_chld;
   sig_t save_quit, save_int, save_chld;

   /* save original signal handler values */
   save_quit = signal(SIGQUIT, SIG_IGN);
   save_int = signal(SIGINT, SIG_IGN);
   save_chld = signal(SIGCHLD, SIG_DFL);

   process_id = fork();
   /* check for failure */
   if (process_id < 0)
   {
      signal(SIGQUIT, save_quit);
      signal(SIGINT, save_int);
      signal(SIGCHLD, save_chld);
      return FALSE;
   }

   /* child process */
   if (process_id == 0)
   {
      signal(SIGQUIT, SIG_DFL);
      signal(SIGINT, SIG_DFL);
      signal(SIGCHLD, SIG_DFL);

      execv(script, command_args);
      bftpd_log("Error trying to run script: %s\n", script);
      exit(127);
   }

   /* parent process */
   do
   {
       process_id = wait4(process_id, NULL, 0, NULL);
   } while ( (process_id == -1) && (errno == EINTR) );

   signal(SIGQUIT, save_quit);
   signal(SIGINT, save_int);
   signal(SIGCHLD, save_chld);

   return TRUE;
}

#ifndef SUPPORT_ATP_FTP_UPGRADE
int ATPDealACLDataPortForPassiveMode( int iSocketFd, ATP_FTPSERVERCMS_ACL_PORT_OPERATE_TYPE_EN enPortOperate )
{
    int iRet = -1;

    /* These static variable are used to make sure any time, just one port would be open at most. */
    static ATP_FTPSERVERCMS_IPTYPE_EN eLastOpenIPType = ATP_FTPSERVERCMS_IPTYPE_IPV4;
    static int iLastOpenDataPort = 0;
    /* These static variable are used to make sure any time, just one port would be open at most. */
#if 1 //modify by y00181549 at 20130218 for DTS2013021800116 在ftp上传下载工程中，拔插硬盘，一段时间后，会出现CMS堵塞情况
    if (VOS_TRUE == isAccFromlan)
    {
        iRet = 0;
        return iRet;
    }
#endif
    if( iSocketFd > 0 )
    {
        struct sockaddr saLocal;
        socklen_t socklenLocal;

        memset( &saLocal, 0, sizeof( saLocal ) );
        socklenLocal = sizeof(saLocal);

        getsockname( iSocketFd, (struct sockaddr *) &saLocal, (socklen_t *) &socklenLocal);

        if( AF_INET == saLocal.sa_family )
        {
            struct sockaddr_in sa4Local;

            memset( &sa4Local, 0, sizeof( sa4Local ) );
            memcpy( &sa4Local, &saLocal, min( sizeof( saLocal ), sizeof( sa4Local ) ) );

            if( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN == enPortOperate )
            {
                /* Close the Last opening data port first. */
                if( iLastOpenDataPort != 0 )
                {
                    ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE, eLastOpenIPType, iLastOpenDataPort );
                }

                /* Store the open data port here. */
                eLastOpenIPType = ATP_FTPSERVERCMS_IPTYPE_IPV4;
                iLastOpenDataPort = sa4Local.sin_port;
            }
            else if( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE == enPortOperate )
            {
                /* If the last open data port would be closed, reset the static variables, else, still keep them. */
                if( iLastOpenDataPort == sa4Local.sin_port && eLastOpenIPType == ATP_FTPSERVERCMS_IPTYPE_IPV4 )
                {
                    eLastOpenIPType = ATP_FTPSERVERCMS_IPTYPE_IPV4;
                    iLastOpenDataPort = 0;
                }
            }

            ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( enPortOperate, ATP_FTPSERVERCMS_IPTYPE_IPV4, sa4Local.sin_port );

            iRet = 0;
        }
        else if( AF_INET6 == saLocal.sa_family )
        {
            struct sockaddr_in6 sa6Local;

            memset( &sa6Local, 0, sizeof( sa6Local ) );
            memcpy( &sa6Local, &saLocal, min( sizeof( saLocal ), sizeof( sa6Local ) ) );

            if( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN == enPortOperate )
            {
                /* Close the Last opening data port first. */
                if( iLastOpenDataPort != 0 )
                {
                    ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE, eLastOpenIPType, iLastOpenDataPort );
                }

                /* Store the open data port here. */
                eLastOpenIPType = ATP_FTPSERVERCMS_IPTYPE_IPV6;
                iLastOpenDataPort = sa6Local.sin6_port;
            }
            else if( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_CLOSE == enPortOperate )
            {
                /* If the last open data port would be closed, reset the static variables, else, still keep them. */
                if( iLastOpenDataPort == sa6Local.sin6_port && eLastOpenIPType == ATP_FTPSERVERCMS_IPTYPE_IPV6 )
                {
                    eLastOpenIPType = ATP_FTPSERVERCMS_IPTYPE_IPV4;
                    iLastOpenDataPort = 0;
                }
            }

            ATP_BFTPD_CHILD_SendRecv_Deal_ACL_Msg( enPortOperate, ATP_FTPSERVERCMS_IPTYPE_IPV6, sa6Local.sin6_port );

            iRet = 0;
        }
#if 1//modify by y00181549 at 20130218 for DTS2013021800116 在ftp上传下载工程中，拔插硬盘，一段时间后，会出现CMS堵塞情况
        if ( ATP_FTPSERVERCMS_ACL_PORT_OPERATE_OPEN == enPortOperate )
        {
            usleep(100000);
        }
#endif
    }

    return iRet;
}
#endif

