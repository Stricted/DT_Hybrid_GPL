#include <config.h>
#include <stdio.h>
#include <pwd.h>
#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
/* add BSD support */
#include <limits.h>
#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif
#ifdef WANT_PAM
#include <security/pam_appl.h>
#endif
#ifdef HAVE_UTMP_H
# include <utmp.h>
# ifdef HAVE_PATHS_H
#  include <paths.h>
#  ifndef _PATH_WTMP
#   define _PATH_WTMP "/dev/null"
#   warning "<paths.h> doesn't set _PATH_WTMP. You can not use wtmp logging"
#   warning "with bftpd."
#  endif
# else
#  define _PATH_WTMP "/dev/null"
#  warning "<paths.h> was not found. You can not use wtmp logging with bftpd."
# endif
#endif
#include <errno.h>
#include <grp.h>
#include <stdlib.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cwd.h"
#include "dirlist.h"
#include "mystring.h"
#include "options.h"
#include "login.h"
#include "logging.h"
#include "bftpdutmp.h"
#include "main.h"

#ifdef SUPPORT_LOGIN_CONTROL  
#include "loginctl.h" 
#include "atplog.h"
pid_t  g_LoginPid = 0;  
extern unsigned char g_ucLoginCtlEnable;
extern int isAccFromWan;
extern int isAccFromlan;
#endif


#ifdef WANT_PAM
char usepam = 0;
pam_handle_t *pamh = NULL;
#endif

#ifdef HAVE_UTMP_H
FILE *wtmp;
#endif

struct passwd userinfo;
char userinfo_set = 0;

/*Start of ATP网络协议组 2010-4-1 14:34 forAU4D02426  by h00163136*/
#if 0
/*系统时间与真正UTC时间的偏差，秒数*/
#define TIME_OFFSET_TO_REAL_UTC_FILE "/var/utc_offset"
int g_utc_offset = 0;
#endif
/*End of ATP网络协议组 2010-4-1 14:34 by h00163136*/

char *mygetpwuid(int uid, FILE * file, char *name)
{
    int _uid;
    char foo[256];
    int i;
	if (file) {
		rewind(file);
        while (fscanf(file, "%255s%*[^\n]\n", foo) != EOF) {
            if ((foo[0] == '#') || (!strchr(foo, ':')) || (strchr(foo, ':') > foo + USERLEN - 1))
                continue;
            i = strchr(foo, ':') - foo;
            strncpy(name, foo, i);
            name[i] = 0;
			sscanf(strchr(foo + i + 1, ':') + 1, "%i", &_uid);
			if (_uid == uid) {
				if (name[0] == '\n')
					cutto(name, 1);
				return name;
			}
		}
	}
	sprintf(name, "%i", uid);
	return name;
}

int mygetpwnam(char *name, FILE * file)
{
	char _name[USERLEN + 1];
	char foo[256];
	int uid, i;
	if (file) {
		rewind(file);
        while (fscanf(file, "%255s%*[^\n]\n", foo) != EOF) {
            if ((foo[0] == '#') || (!strchr(foo, ':')) || (strchr(foo, ':') > foo + USERLEN - 1))
                continue;
            i = strchr(foo, ':') - foo;
            strncpy(_name, foo, i);
            _name[i] = 0;
			sscanf(strchr(foo + i + 1, ':') + 1, "%i", &uid);
			if (_name[0] == '\n')
				cutto(_name, 1);
			if (!strcmp(name, _name))
				return uid;
		}
	}
	return -1;
}

#ifdef HAVE_UTMP_H
void wtmp_init()
{
	if (strcasecmp(config_getoption("LOG_WTMP"), "no")) {
		if (!((wtmp = fopen(_PATH_WTMP, "a"))))
			bftpd_log("Warning: Unable to open %s.\n", _PATH_WTMP);
	}
}

void bftpd_logwtmp(char type)
{
	struct utmp ut;
	if (!wtmp)
		return;
	memset((void *) &ut, 0, sizeof(ut));
#ifdef _HAVE_UT_PID
	ut.ut_pid = getpid();
#endif
	/*Added by lvxin00135113@20110425 安全代码整改*/
	#if 0
	sprintf(ut.ut_line, "ftp%i", (int) getpid());
	#else
    snprintf(ut.ut_line,sizeof(ut.ut_line), "ftp%i",(int) getpid());
	#endif
	if (type) {
#ifdef _HAVE_UT_TYPE
		ut.ut_type = USER_PROCESS;
#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
		#if 0
		strncpy(ut.ut_name, user, sizeof(ut.ut_name));
		#else
        snprintf(ut.ut_name,sizeof(ut.ut_name), "%s",user);
		#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
#ifdef _HAVE_UT_HOST   
		/*Added by lvxin00135113@20110425 安全代码整改*/
		#if 0
		strncpy(ut.ut_host, remotehostname, sizeof(ut.ut_host));
		#else
        snprintf(ut.ut_host,sizeof(ut.ut_host), "%s",remotehostname);
		#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
#endif    
	} else {
#ifdef _HAVE_UT_TYPE
		ut.ut_type = DEAD_PROCESS;
#endif
	}
	time(&(ut.ut_time));
	fseek(wtmp, 0, SEEK_END);
	fwrite((void *) &ut, sizeof(ut), 1, wtmp);
	fflush(wtmp);
}

void wtmp_end()
{
	if (wtmp) {
		if (state >= STATE_AUTHENTICATED)
			bftpd_logwtmp(0);
		fclose(wtmp);
	}
}
#endif

void login_init()
{
//    FILE *pstFile;
    char *foo = config_getoption("INITIAL_CHROOT");
    
/*Start of ATP网络协议组 2010-4-1 14:34 forAU4D02426  by h00163136*/
 /*问题单AU4D02426:系统时间设置成真正GMT时间,无偏差,不作该调整*/
#if 0
    /*start of AU4D02390 ftp显示的时间差一年, by hh00163136, 20100208*/
    /*在还没有chroot之前，把var里面的偏差值读出来*/
    pstFile = fopen(TIME_OFFSET_TO_REAL_UTC_FILE, "rb");
    if (NULL != pstFile)
    {
        fread(&g_utc_offset, sizeof(g_utc_offset), 1, pstFile);
        fclose(pstFile);
    }
    /*end of AU4D02390 ftp显示的时间差一年, by hh00163136, 20100208*/
#endif
/*End of ATP网络协议组 2010-4-1 14:34 by h00163136*/
#ifdef HAVE_UTMP_H
	wtmp_init();
#endif
    if (foo[0]) { /* Initial chroot */
        if (chroot(foo) == -1) {
            control_printf(SL_FAILURE, "421 Initial chroot failed.");
            exit(1);
        }
    }
}

int bftpd_setuid(uid_t uid)
{
    /* If we must open the data connections from port 20,
     * we have to keep the possibility to regain root privileges */
    if (!strcasecmp(config_getoption("DATAPORT20"), "yes"))
        return seteuid(uid);
    else
        return setuid(uid);
}

int bftpd_login(char *password)
{
	char str[MAX_STRING_LENGTH + 1];
	char *foo;
	int maxusers;
    char *file_auth;   /* if used, points to file used to auth users */
    char *home_directory = NULL;   /* retrieved from auth_file */
    char *anonymous = NULL;
#ifdef SUPPORT_LOGIN_CONTROL
    ATP_LOGINRET_EN enLoginRet = ATP_LOGINRET_OK;
#endif 
	int ret = -1 ;  //add by y00181549 for 输入错误用户名返回331，防止尝试用户名
        //unsigned long get_maxusers;
        //printf("\n l63336 login");
        str[0] = '\0';     /* avoid garbage in str */
        file_auth = config_getoption("FILE_AUTH");

        if (! file_auth[0] )    /* not using auth file */
        {
           // check to see if regular authentication is avail

           
           #ifndef NO_GETPWNAM
        if(strcasecmp(user,"") && strcasecmp(user,"anonymous") )
        {
            if (!getpwnam(user)) 
            {
                control_printf(SL_FAILURE, "421 Login incorrect.");
          //      control_printf(SL_FAILURE, "530 not allow anonymous.=bftpd_login==user=%s=posswd=%s==5555=",user,password);
                //            control_printf(SL_SUCCESS, "220");
    	     #if 0
           if(strstr(password,"mozilla") && (!strcasecmp(user,"") ||!strcasecmp(user,"anonymous") ))
               {
                    control_printf(SL_FAILURE, "this is firefox browser.=bftpd_login==user=%s=posswd=%s==5555=",user,password);
    	
                       //        control_printf(SL_SUCCESS, "530 not allow anonymous this is anonymous user.=bftpd_login==user==555=");
                }
              else
                {
                     exit(0);
                 }
             #endif 
              exit(0);
            }
         }
           #endif
        }
        /* we are using auth_file */
        else
        {
    		ret = check_file_password(file_auth, user, password, &home_directory);
    		if(-1 == ret)
    		{
    			return -1;
    		}

           anonymous = config_getoption("ANONYMOUS_USER");
           if (! home_directory || '\0' == home_directory[0])
           {
               if (! strcasecmp(anonymous, "yes") )
			   	exit(-1);
               else
               {
#ifdef SUPPORT_LOGIN_CONTROL  //added by qichenghong 2009.12.04
                    if((1 == g_ucLoginCtlEnable) && (strcasecmp(user , "anonymous")))
                    {
                        if(isAccFromWan)
                          {
                              ATP_LoginCtlCheck(user , inet_ntoa(remotename.sin_addr) , ATP_LOGINTYPE_FTP|ATP_LOGINTYPE_REMOTE, LOGIN_FAIL);
                          }
                        else
                          {
                                ATP_LoginCtlCheck(user , inet_ntoa(remotename.sin_addr) , ATP_LOGINTYPE_FTP , LOGIN_FAIL);//added by qichenghong 2009.12.03
                          }
                    }
#endif
                  //control_printf(SL_FAILURE, "421 Authentication incorrect.");
                  exit(-1);
               }
           }
        }

	if (strncasecmp(foo = config_getoption("DENY_LOGIN"), "no", 2)) {
		if (foo[0] != '\0') {
			if (strncasecmp(foo, "yes", 3))
				control_printf(SL_FAILURE, "421-Server disabled.\r\n421 Reason: %s", foo);
			else
				control_printf(SL_FAILURE, "421 Login incorrect.");
			bftpd_log("Login as user '%s' failed: Server disabled.\n", user);
			exit(0);
		}
	}
	maxusers = strtoul(config_getoption("USERLIMIT_GLOBAL"), NULL, 10);
    /*--start---  maxusers now set to 15*/
    maxusers = 0; //to do,  will support 15 latter, when use this feature,bftpd have a serious bug when transfer file with chinese name.090509
    /*---end --- add by w145036*/
	if ((maxusers) && (maxusers == bftpdutmp_usercount("*"))) {
		control_printf(SL_FAILURE, "421 There are already %i users logged in.", maxusers);
        bftpd_log("Login as user '%s' failed. Too many users on server.\n", user);
		exit(0);
	}
	maxusers = strtoul(config_getoption("USERLIMIT_SINGLEUSER"), NULL, 10);
    /*--start---  maxusers now set to 0*/
    maxusers = 0;
    /*---end --- add by w145036*/
	if ((maxusers) && (maxusers == bftpdutmp_usercount(user))) {
		control_printf(SL_FAILURE, "421 User %s is already logged in %i times.", user, maxusers);
        bftpd_log("Login as user '%s' failed. Already logged in %d times.",user, maxusers);
		exit(0);
	}

        /* Check to see if we should block mulitple logins from the same machine.
           -- Jesse <slicer69@hotmail.com>
        */
        maxusers = strtoul( config_getoption("USERLIMIT_HOST"), NULL, 10);
        /*--start---  maxusers now set to 0*/
        maxusers = 0;
        /*---end --- add by w145036*/
        if ( (maxusers) && (maxusers == bftpdutmp_dup_ip_count(remotehostname) ) )
        {
            control_printf(SL_FAILURE, "421 Too many connections from your IP address.");
            bftpd_log("Login as user '%s' failed. Already %d connections from %s.\n", user, maxusers, remotehostname);
            exit(0);
        }
        
		//--- start ---  add by w00145036
        /* fobidden the other user except ftp user*/
        #if 0
        if (! file_auth[0] )
        {
	        bftpd_log("Login as user '%s' failed. userinfino = '%s'.\n", user, userinfo.pw_gecos);
            if(strcasecmp(userinfo.pw_gecos,"ftp user") != 0)
            {
                control_printf(SL_FAILURE, "421 the login name is not ftp user name.");  
				return -1;
            }
        }
        #endif
        //---end---


        /* disable these checks when logging in via auth file */
        if (! file_auth[0] ) 
        {
            #ifndef NO_GETPWNAM
	    if(checkuser() || checkshell()) {
		control_printf(SL_FAILURE, "421 Login incorrect.");
                return -1;
	    }
            #endif
        }

        /* do not do this check when we are using auth_file */
        if (! file_auth[0] )
        {
            #ifndef NO_GETPWNAM
	    if (checkpass(password))
		return 1;
            #endif
        }

        //--- start ---  add by w00145036
        /* fobidden the other user except ftp user*/
        if (! file_auth[0] )
        {
            bftpd_log("Login as user '%s' failed. userinfino = '%s'.\n", user, userinfo.pw_gecos);
            if(strcasecmp(userinfo.pw_gecos,"ftp user") != 0)
            {
                control_printf(SL_FAILURE, "421 the login name is not ftp user name.");  
                return -1;
            }
        }
        //---end---

#ifdef SUPPORT_LOGIN_CONTROL
    if((1 == g_ucLoginCtlEnable) && 
        (strcasecmp(user , "anonymous")))  //anonymous 不参与用户登录控制
    {
          if(isAccFromWan)
          {
              enLoginRet = ATP_LoginCtlCheck(user , inet_ntoa(remotename.sin_addr) , ATP_LOGINTYPE_FTP|ATP_LOGINTYPE_REMOTE, LOGIN_OK);
          }
          else
          {
              enLoginRet = ATP_LoginCtlCheck(user , inet_ntoa(remotename.sin_addr) , ATP_LOGINTYPE_FTP, LOGIN_OK);
          }                
        if(ATP_LOGINRET_OK != enLoginRet)
        {
            control_printf(SL_FAILURE, "421 Login incorrect.");
            bftpd_statuslog(1, 0, "%s", "xxxxxxxxxx\n");
            /*start of add by y00181549 for W724v at 20120718*/
            if(1 == isAccFromlan)
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_LOCAL_USER_FAIL_LOGIN_5_TIMES,user);
            }
            else
            {
                ATP_LOG_LogItemPrint(ATP_SYS_LOG_UMOUNT_REMOTE_USER_FAIL_LOGIN_5_TIMES,user);
            }
            /*end of add by y00181549 for W724v at 20120718*/
            return -1;
        }
        g_LoginPid = getpid();
    }
#endif


	if (strcasecmp((char *) config_getoption("RATIO"), "none")) {
		sscanf((char *) config_getoption("RATIO"), "%i/%i",
			   &ratio_send, &ratio_recv);
	}

        /* do these checks if logging in via normal methods */
        if (! file_auth[0])
        {
		 /*Added by lvxin00135113@20110425 安全代码整改*/
		 #if 0
	     strcpy(str, config_getoption("ROOTDIR"));
	     if (!str[0])
		strcpy(str, "%h");
		 #else	
         snprintf(str, sizeof(str), "%s",config_getoption("ROOTDIR"));
	     if (!str[0])

         snprintf(str, sizeof(str), "%s","%h");
		 #endif
		 /*Added by lvxin00135113@20110425 安全代码整改*/
	     replace(str, "%u", userinfo.pw_name, MAX_STRING_LENGTH);
	     replace(str, "%h", userinfo.pw_dir, MAX_STRING_LENGTH);
	     if (!strcasecmp(config_getoption("RESOLVE_UIDS"), "yes")) 
             {
		passwdfile = fopen("/etc/passwd", "r");
		groupfile = fopen("/etc/group", "r");
	     } 

	setgid(userinfo.pw_gid);
	initgroups(userinfo.pw_name, userinfo.pw_gid);
	if (strcasecmp(config_getoption("DO_CHROOT"), "no")) {
		if (chroot(str)) {
			control_printf(SL_FAILURE, "421 Unable to change root directory.\r\n%s.",
					strerror(errno));
			exit(0);
		}

        if(userinfo.pw_dir)
            free(userinfo.pw_dir);
        userinfo.pw_dir = strdup(str);

		if (bftpd_setuid(userinfo.pw_uid)) {
			control_printf(SL_FAILURE, "421 Unable to change uid.");
			exit(0);
		}
		if (chdir("/")) {
			control_printf(SL_FAILURE, "421 Unable to change working directory.\r\n%s.",
					 strerror(errno));
			exit(0);
		}
	} else {
		if (bftpd_setuid(userinfo.pw_uid)) {
			control_printf(SL_FAILURE, "421 Unable to change uid.");
			exit(0);
		}
		if (chdir(str)) {
			control_printf(SL_FAILURE, "230 Couldn't change cwd to '%s': %s.", str,
					 strerror(errno));
			chdir("/");
		}
	}

        }   /* end of if we are using regular authentication methods */

        else     /* we are using file authentication */
        {
            /* get home directory */
			/*Added by lvxin00135113@20110425 安全代码整改*/
			#if 0
	    	strcpy(str, config_getoption("ROOTDIR"));
            if (! str[0])
                strcpy(str, "%h");
			#else
            snprintf(str, sizeof(str), "%s",config_getoption("ROOTDIR"));
            if (! str[0])
			{

                snprintf(str, sizeof(str), "%s","%h");
			}
			#endif
			/*Added by lvxin00135113@20110425 安全代码整改*/
		    replace(str, "%h", home_directory, MAX_STRING_LENGTH);
            replace(str, "%u", user, MAX_STRING_LENGTH);

            /* see if we should change root */
            if (! strcasecmp(config_getoption("DO_CHROOT"), "yes"))
            {
                if ( chroot(home_directory) )
                {
                    control_printf(SL_FAILURE, "421 Unable to change root directory.");
                    exit(0);
                }
                
                if(userinfo.pw_dir)
                    free(userinfo.pw_dir);
                userinfo.pw_dir = strdup(home_directory);

                if ( chdir("/") )
                {
                    control_printf(SL_FAILURE, "421 Unable to change working directory.");
                    exit(0);
                }
            }
          //start add by qichenghong 2009.11.30 
            else
            {
                if ( chdir(home_directory) )
                {
                    control_printf(SL_FAILURE, "421 Unable to change working directory.\r\n%s",home_directory);
                    exit(0);
                }
            }
          //end add by qichenghong 2009.11.30      
        }      /* end of using file auth */

        new_umask();
        /* print_file(230, config_getoption("MOTD_USER")); */
		/*Added by lvxin00135113@20110425 安全代码整改*/
		#if 0
        strcpy(str, config_getoption("MOTD_USER"));
		#else
        snprintf(str, sizeof(str), "%s",config_getoption("MOTD_USER"));
		#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
        /* Allow user specific path to MOTD file. */
        replace(str, "%h", home_directory, MAX_STRING_LENGTH);
        replace(str, "%u", user, MAX_STRING_LENGTH);
        print_file(230, str);
        control_printf(SL_SUCCESS, "230 User logged in.");
#ifdef HAVE_UTMP_H
        bftpd_logwtmp(1);
#endif
       bftpdutmp_log(1);
       bftpd_log("Successfully logged in as user '%s'.\n", user);
       if (config_getoption("AUTO_CHDIR")[0])
            chdir(config_getoption("AUTO_CHDIR"));

        state = STATE_AUTHENTICATED;
        bftpd_cwd_init();

        /* a little clean up before we go*/
        if ( (home_directory) && ( strcmp(home_directory, "/" ) ) )
            free(home_directory);
        return 0;
}


/* Return 1 on failure and 0 on success. */
int checkpass(char *password)
{
    #ifndef NO_GETPWNAM
    if (!getpwnam(user))
		return 1;
    #endif

	if (!strcasecmp(config_getoption("ANONYMOUS_USER"), "yes"))
		return 0;

#ifdef WANT_PAM
	if (!strcasecmp(config_getoption("AUTH"), "pam"))
		return checkpass_pam(password);
	else
#endif
		return checkpass_pwd(password);
}



void login_end()
{
#ifdef WANT_PAM
	if (usepam)
		return end_pam();
#endif
#ifdef HAVE_UTMP_H
	wtmp_end();
#endif
}

int checkpass_pwd(char *password)
{
#ifdef HAVE_SHADOW_H
	struct spwd *shd;
#endif
	if (strcmp(userinfo.pw_passwd, (char *) crypt(password, userinfo.pw_passwd))) {
#ifdef HAVE_SHADOW_H
		if (!(shd = getspnam(user)))
			return 1;
		if (strcmp(shd->sp_pwdp, (char *) crypt(password, shd->sp_pwdp)))
#endif
			return 1;
	}
	return 0;
}

#ifdef WANT_PAM
int conv_func(int num_msg, const struct pam_message **msgm,
			  struct pam_response **resp, void *appdata_ptr)
{
	struct pam_response *response;
	int i;
	response = (struct pam_response *) malloc(sizeof(struct pam_response) * num_msg);
	for (i = 0; i < num_msg; i++) {
		response[i].resp = (char *) strdup(appdata_ptr);
		response[i].resp_retcode = 0;
	}
	*resp = response;
	return 0;
}

int checkpass_pam(char *password)
{
	struct pam_conv conv = { conv_func, password };
	int retval = pam_start("bftpd", user, (struct pam_conv *) &conv,
						   (pam_handle_t **) & pamh);
	if (retval != PAM_SUCCESS) {
		printf("Error while initializing PAM: %s\n",
			   pam_strerror(pamh, retval));
		return 1;
	}
        /* 
        Allow Bftpd to build with OpenPAM
	pam_fail_delay(pamh, 0);
        */
	retval = pam_authenticate(pamh, 0);
	if (retval == PAM_SUCCESS)
		retval = pam_acct_mgmt(pamh, 0);
	if (retval == PAM_SUCCESS)
		pam_open_session(pamh, 0);
	if (retval != PAM_SUCCESS)
		return 1;
	else
		return 0;
}

void end_pam()
{
	if (pamh) {
		pam_close_session(pamh, 0);
		pam_end(pamh, 0);
	}
}
#endif

int checkuser()
{

	FILE *fd;
	char *p;
	char line[256];

	if ((fd = fopen(config_getoption("PATH_FTPUSERS"), "r"))) {
		while (fgets(line, sizeof(line), fd))
			if ((p = strchr(line, '\n'))) {
				*p = '\0';
				if (line[0] == '#')
					continue;
				if (!strcasecmp(line, user)) {
					fclose(fd);
					return 1;
				}
			}
		fclose(fd);
	}
	return 0;
}

int checkshell()
{
#ifdef HAVE_GETUSERSHELL
	char *cp;
	struct passwd *pwd;

    if (!strcasecmp(config_getoption("AUTH_ETCSHELLS"), "no"))
        return 0;
    /*added by lvxin*/
    if(strcmp(user,""))
    {
        return 0;
    }
	pwd = getpwnam(user);
	while ((cp = getusershell()))
		if (!strcmp(cp, pwd->pw_shell))
			break;
	endusershell();

	if (!cp)
		return 1;
	else
		return 0;
#else
    return 0;
#   warning "Your system doesn't have getusershell(). You can not"
#   warning "use /etc/shells authentication with bftpd."
#endif
}




/*
This function searches through a text file for a matching
username. If a match is found, the password in the
text file is compared to the password passed in to
the function. If the password matches, the function
returns the fourth field (home directory). On failure,
it returns NULL.
-- Jesse
*/
int check_file_password(char *my_filename, char *my_username, char *my_password, char **my_home_dir)
{
   FILE *my_file;
   int found_user = 0;
   char user[USERLEN + 1], password[PASSWDLEN + 1], group[33];//changed by qichenghong 2009.12.24
   char line[512];
   char* pchomedir = NULL;
   char* pchomedir_end;
   

   bftpd_statuslog(1, 0, "check passwd user:%s password:%s,my_filename:%s\n",my_username,my_password,my_filename);
   my_file = fopen(my_filename, "r");
   if (! my_file)
      return -1;

   //by laopeifeng 00161677,可获得带空格的配置路径
    line[0] = '\0';
    fgets(line,512,my_file);
    while(strlen(line)>7)
    {
        sscanf(line, "%256s %64s %32s", user, password, group);

        pchomedir = strchr(line,'\"');
        pchomedir++;
        pchomedir_end = strchr(pchomedir,'\"');
        *pchomedir_end = '\0';
		
        bftpd_statuslog(1, 0, "%s %d check passwd founded user:%s password:%s home_dir:%s\n",__FILE__, __LINE__, user,password, pchomedir);

        if(!strcmp(my_username,user))
        {
            found_user = 1;
            break;
        }

        line[0] = '\0';
        fgets(line,512,my_file);

    }
    //by laopeifeng 00161677
    


   fclose(my_file);
   if (found_user)
   {
#ifdef SUPPORT_ATP_FTP_GUEST //add by y00181549 for ftp guest 帐号，密码为至少一个字符
	if( (!strcmp(user,"guest")) && ( 0 < strlen(my_password)) )
	{
	}
	else
#endif
	{
	      /* check password */
	      if (! strcmp(password, "*") )
	      {
	      }
	      else if ( strcmp(password, my_password) )
			 return -1;
	}
      *my_home_dir = calloc( strlen(pchomedir) + 1, sizeof(char) );
      if (! *my_home_dir)
          return -1;
      strcpy(*my_home_dir, pchomedir);
   }
   else
   	{
		return -1;
   	}
  
   return 0;
}

