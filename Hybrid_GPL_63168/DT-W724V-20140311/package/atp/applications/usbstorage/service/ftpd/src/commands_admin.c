#include "config.h"
#include <stdio.h>
#ifdef HAVE_CRYPT_H
# include <crypt.h>
#endif
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#include "mystring.h"
#include "options.h"
#include "commands.h"
#include "logging.h"
#include "bftpdutmp.h"
#include "login.h"

FILE *mystatuslog;
extern int atp_ftpd_read_command( char *pBuffer, int iSize );

void loginfailed()
{
    control_printf(SL_FAILURE, "421 Login incorrect.");
    bftpd_log("Administrative login FAILED\n");
    exit(1);
}

void command_adminlogin(char *params)
{
    char adminpass[31];
    char rootpass[31];
    char buffer[256];
    char *realadminpass = config_getoption("ADMIN_PASS");
    if (sscanf(params, "%30s %30s", adminpass, rootpass) < 2)
        loginfailed();
    if (!realadminpass[0])
        loginfailed();
    if (strcmp(crypt(adminpass, realadminpass), realadminpass))
        loginfailed();
    /* Admin password is right */
	
	/*Added by lvxin00135113@20110425 安全代码整改*/
	#if 0
    strcpy(user, "root");
	#else    
    snprintf(user, sizeof(user), "%s","root");                
	#endif
	/*Added by lvxin00135113@20110425 安全代码整改*/
    init_userinfo();
    if (checkpass(rootpass))
        loginfailed();
    /* Root password is right as well */
    signal(SIGALRM, SIG_IGN);
    control_printf(SL_SUCCESS, "230 Administrative login successful.");
    bftpd_log("Administrative login SUCCESSFUL\n");

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if 0
    while (fgets(buffer, sizeof(buffer), stdin))
#else
    while( atp_ftpd_read_command( buffer, sizeof( buffer ) ) >= 0 )
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

    {
        admin_parsecmd(buffer);
    }
    exit(0);
}

void command_admingetconf(char *params)
{
    control_printf(SL_FAILURE, "500 Not implemented yet.");
}

void command_adminlog(char *params)
{
    fd_set rfds;
    struct timeval tv;
    char buffer[256];
    control_printf(SL_SUCCESS, "200 Starting logfile transmission.");
    mystatuslog = statuslogforreading;
    fseek(mystatuslog, 0, SEEK_END);
    while (mystatuslog) {
        while (fgets(buffer, sizeof(buffer), mystatuslog))
        {
            /* Don't use control_printf here, as it would generate an infinite loop */
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
            if ( pstsslcommandobj != NULL )
            {
                SSL_write(pstsslcommandobj,(const unsigned char*) buffer, strlen(buffer));
            }
            else
            {
                fprintf(stderr, "%s", buffer );
            }
#else
            fprintf(stderr, "%s", buffer);
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

        }
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if (select(1, &rfds, NULL, NULL, &tv) > 0) {

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#if 0
            if (!fgets(buffer, sizeof(buffer), stdin))
#else
            if ( atp_ftpd_read_command( buffer, sizeof( buffer ) ) < 0 )
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
            {
                exit(0);
            }
            admin_parsecmd(buffer);
        }
    }
    control_printf(SL_SUCCESS, "202 Logfile transmission stopped.");
}

void command_adminstoplog(char *params)
{
    mystatuslog = NULL;
    control_printf(SL_SUCCESS, "201 Stopping logfile transmission.");
}

void command_adminwho(char *params)
{
    struct bftpdutmp tmp;

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    if ( pstsslcommandobj != NULL )
    {
        char buffer[256] = "";

        snprintf( buffer, sizeof( buffer ), "%s", "200-User listing follows.\n" );
        SSL_write(pstsslcommandobj, (const unsigned char*)buffer, strlen(buffer));

        snprintf( buffer, sizeof( buffer ), "%s", "200-PID       User           Host                Login time\n" );
        SSL_write(pstsslcommandobj, (const unsigned char*)buffer, strlen(buffer));

        rewind(bftpdutmp);
        while (fread((void *) &tmp, sizeof(tmp), 1, bftpdutmp)) {
            if (!tmp.bu_type)
                continue;
            snprintf( buffer, sizeof(buffer), "200-%-10i%-15s%-20s%s",  tmp.bu_pid, tmp.bu_name, tmp.bu_host,
                     ctime(&(tmp.bu_time)) );
            SSL_write(pstsslcommandobj, (const unsigned char*)buffer, strlen(buffer));
        }

        snprintf( buffer, sizeof( buffer ), "%s", "200 User listing finished.\n" );
        SSL_write(pstsslcommandobj,(const unsigned char*) buffer, strlen(buffer));
    }
    else
    {
        fprintf(stderr, "200-User listing follows.\n");
        fprintf(stderr, "200-PID       User           Host                Login time\n");
        rewind(bftpdutmp);
        while (fread((void *) &tmp, sizeof(tmp), 1, bftpdutmp)) {
            if (!tmp.bu_type)
                continue;
            fprintf(stderr, "200-%-10i%-15s%-20s%s", tmp.bu_pid, tmp.bu_name, tmp.bu_host,
                     ctime(&(tmp.bu_time)));
        }
        fprintf(stderr, "200 User listing finished.\n");
    }
#else

    fprintf(stderr, "200-User listing follows.\n");
    fprintf(stderr, "200-PID       User           Host                Login time\n");
    rewind(bftpdutmp);
    while (fread((void *) &tmp, sizeof(tmp), 1, bftpdutmp)) {
        if (!tmp.bu_type)
            continue;
        fprintf(stderr, "200-%-10i%-15s%-20s%s", tmp.bu_pid, tmp.bu_name, tmp.bu_host,
                 ctime(&(tmp.bu_time)));
    }
    fprintf(stderr, "200 User listing finished.\n");

#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/


}

void command_adminkick(char *strpid)
{
    unsigned long int get_pid = strtoul(strpid, NULL, 10);
    int pid;
    
    if (get_pid <= INT_MAX)
       pid = get_pid;
    else
       pid = 0;
    if (!pid)
        control_printf(SL_FAILURE, "500 Error: Given PID is not valid.");
    else if (bftpdutmp_pidexists(pid)) {
        if (kill(pid, SIGTERM))
            control_printf(SL_FAILURE, "500 Error: %s.", strerror(errno));
        else
            control_printf(SL_FAILURE, "200 OK");
    } else
        control_printf(SL_FAILURE, "500 Error: The given PID does not belong to bftpd.");
}

void command_adminquit(char *params)
{
    control_printf(SL_SUCCESS, "221 See you later...");
    exit(0);
}

const struct admin_command admin_commands[] = {
    {"ADMIN_GETCONF", command_admingetconf},
    {"ADMIN_LOG", command_adminlog},
    {"ADMIN_STOPLOG", command_adminstoplog},
    {"ADMIN_WHO", command_adminwho},
    {"ADMIN_KICK", command_adminkick},
    {"ADMIN_QUIT", command_adminquit},
	{NULL, NULL}
};

int admin_parsecmd(char *str)
{
	int i;
	char *p, *pp;
	str[strlen(str) - 2] = '\0';	/* Remove \r\n */
	p = pp = str;			/* Remove garbage in the string */
	while (*p)
		if ((unsigned char) *p < 32)
			p++;
		else
			*pp++ = *p++;
	*pp++ = 0;
	for (i = 0; admin_commands[i].name; i++) {	/* Parse command */
		if (!strncasecmp(str, admin_commands[i].name, strlen(admin_commands[i].name))) {
			cutto(str, strlen(admin_commands[i].name));
			p = str;
			while ((*p) && ((*p == ' ') || (*p == '\t')))
				p++;
			memmove(str, p, strlen(str) - (p - str) + 1);
			admin_commands[i].function(str);
			return 0;
		}
	}
	control_printf(SL_FAILURE, "500 Unknown command: \"%s\"", str);
	return 1;
}
