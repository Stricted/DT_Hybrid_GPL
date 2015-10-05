#ifndef __BFTPD_MAIN_H
#define __BFTPD_MAIN_H

#include <sys/types.h>
#include "atpconfig.h"

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
#include <openssl/ssl.h>
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/


#define TRUE 1
#define FALSE 0

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
#define   UNSECURED     0L
#define   SECURED       1L
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

#define BFTPD_MAX_BUFFER_LEN 1024
#define BFTPD_CMD_LINE_LEN 32

/*start of HG_Protocol 2007.06.29 HG522V100R001C01B011 A36D03658 */
#define FTP_MAX_LINK_NUM 15
/*end of HG_Protocol 2007.06.29 HG522V100R001C01B011 A36D03658 */

struct bftpd_childpid {
    pid_t pid;
    int sock;
};


typedef struct bftpd_userpid {
    int type;
    int lpid;
    char acUserName[256];
}bftpd_userpid_st;

/*
 *  Socket Layer
 *      This layer abastract TCP socket and SSL socket
 *      All http client and http server should use this layer to do data trans-recv
 */
/** Socket type */
typedef enum tagATP_SOCK_FLAG_EN
{
    ATP_SOCK_DFT        = 0,        // Default: TCP, no SSL, IPv4
    ATP_SOCK_UDP        = 0x01,     // Socket type, 0: TCP; 1: UDP
    ATP_SOCK_SSL        = 0x02,     // Whether SSL, 0: No SSL; 1: SSL
    ATP_SOCK_IP_VER     = 0x04,     // Version, 0: IPv4; 1: IPv6
    ATP_SOCK_UNIX       = 0x08,

    // Allowed combinations
    ATP_SOCK_TCP4_SSL   = ATP_SOCK_SSL,
    ATP_SOCK_TCP6_SSL   = ATP_SOCK_SSL | ATP_SOCK_IP_VER,
    ATP_SOCK_UDP6       = ATP_SOCK_UDP | ATP_SOCK_IP_VER,
} ATP_SOCK_FLAG_EN;

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS

// Authorization mode
typedef enum tagATP_FTPDS_SSL_MODE
{
    ATP_FTPDS_SSL_CERT_AUTH_NONE,        // No authorization
    ATP_FTPDS_SSL_CERT_AUTH_PEER,        // Authorization peer, need to provide root cert
    ATP_FTPDS_SSL_CERT_AUTH_BY_PEER,     // To be authorized by peer, need to provide public cert and key
    ATP_FTPDS_SSL_CERT_AUTH_BOTH,        // Need to do full authorization
    ATP_FTPDS_SSL_CERT_AUTH_MAX
} ATP_FTPDS_SSL_MODE;

/* SSL版本 */
typedef enum tagATP_FTPDS_SSL_VERSION
{
    ATP_FTPDS_SSL_ANY_EXCLUDE_V2,        // Any version except SSLv2
    ATP_FTPDS_SSL_ANY,                   // SSLv2, SSLv3 or TLSv1
    ATP_FTPDS_SSL_TLS,                   // TLSv1 only
    ATP_FTPDS_SSL_V3,                    // SSLv3 only
    ATP_FTPDS_SSL_MAX
} ATP_FTPDS_SSL_VERSION;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

extern int global_argc;
extern char **global_argv;
extern struct sockaddr_in name;
extern FILE *passwdfile, *groupfile, *devnull;
extern char *remotehostname;
extern struct sockaddr_in remotename;
extern int control_timeout, data_timeout;
extern int alarm_type;
#ifdef SUPPORT_ATP_IPV6
extern struct sockaddr_in6 name6;
extern struct sockaddr_in6 remotename6;
#endif

/* Command line options */
char *configpath;
int daemonmode;
/*add by l63336 是否支持内存中创建目录*/
extern int iCheckMemCreate;
/*add by l63336 是否支持内存中创建目录*/

/* scripts to run before and after writing to the file system */
char *pre_write_script;
char *post_write_script;


void print_file(int number, char *filename);
void process_bftpd_user_pid_add(char *user,int pid);
void process_bftpd_user_pid_del(int pid);

#endif
