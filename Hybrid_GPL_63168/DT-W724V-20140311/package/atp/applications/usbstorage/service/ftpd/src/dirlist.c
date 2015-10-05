#include <config.h>
#include <stdio.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#else
#    define dirent direct
#    define NAMLEN(dirent) (dirent)->d_namlen
#  ifdef HAVE_SYS_NDIR_H
#    include <sys/ndir.h>
#  endif
#  ifdef HAVE_SYS_DIR_H
#    include <sys/dir.h>
#  endif
#  ifdef HAVE_NDIR_H
#    include <ndir.h>
#  endif
#endif

#include <unistd.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <mystring.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <glob.h>

#include "cwd.h"
#include "options.h"
#include "main.h"
#include "login.h"
#include "dirlist.h"

//extern int g_utc_offset;

struct hidegroup *hidegroups = NULL;

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
extern ssl_context * pstssldataobj;
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/


void add_to_hidegroups(int gid)
{
    static struct hidegroup *tmp = NULL;
    if (tmp)
        tmp = tmp->next = malloc(sizeof(struct hidegroup));
    else
        hidegroups = tmp = malloc(sizeof(struct hidegroup));

    if (tmp)
    {
      tmp->next = NULL;
      tmp->data = gid;
    }
}

void hidegroups_init()
{
    char *foo = strdup(config_getoption("HIDE_GROUP"));
    char *foo_save = foo;
    char *bar;
    struct group *tmpgrp;

    while ((bar = strtok(foo, ","))) {
        foo = NULL; /* strtok requirement */
        if ((strcmp(bar, "0")) && (!strtoul(bar, NULL, 10))) {
            /* bar is not numeric */
            if ((tmpgrp = getgrnam(bar)))
                add_to_hidegroups(tmpgrp->gr_gid);
        } else
            if (strtoul(bar, NULL, 10))
                add_to_hidegroups(strtoul(bar, NULL, 10));
    }
	free(foo_save);
}

void hidegroups_end()
{
    struct hidegroup *tmp = hidegroups;
    if (hidegroups)
        while (hidegroups->next) {
            tmp = hidegroups->next;
            free(hidegroups);
            hidegroups = tmp;
        }
}

void bftpd_stat(char *name, FILE * client)
{
    struct stat statbuf;
	char temp[MAXCMD + 3], linktarget[MAXCMD + 5], perm[11], timestr[17],
		uid[USERLEN + 1], gid[USERLEN + 1];
    struct tm filetime;
    struct tm *tea_time, *local_time;
    time_t t;
    char buffer[512] = "";
    
    if (lstat(name, (struct stat *) &statbuf) == -1)
    { // used for command_stat
#if 0
        fprintf(client, "213-Error: %s.\n", strerror(errno));
#endif
        snprintf( buffer, sizeof(buffer), "213-Error: %s.\n", strerror(errno) );

/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        if ( pstssldataobj != NULL )
        {
            SSL_write(pstssldataobj, (const unsigned char *)buffer, strlen(buffer));
        }
        else
        {
            fprintf(client, "%s", buffer );
        }
#else
        fprintf(client, "%s", buffer );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

        return;
    }
	/*Added by lvxin00135113@20110425 安全代码整改*/
	
#ifdef S_ISLNK
	if (S_ISLNK(statbuf.st_mode)) {
		#if 0
		strcpy(perm, "lrwxrwxrwx");
		temp[readlink(name, temp, sizeof(temp) - 1)] = '\0';
		sprintf(linktarget, " -> %s", temp);
		#else
        snprintf(perm, sizeof(perm), "%s","lrwxrwxrwx");
		temp[readlink(name, temp, sizeof(temp) - 1)] = '\0';
	
        snprintf(linktarget, sizeof(linktarget), " -> %s", temp);
		#endif
	} else {
#endif
		#if 0
		strcpy(perm, "----------");
		#else
        snprintf(perm, sizeof(perm), "%s","----------");
		#endif
		/*Added by lvxin00135113@20110425 安全代码整改*/
		if (S_ISDIR(statbuf.st_mode))
			perm[0] = 'd';
		if (statbuf.st_mode & S_IRUSR)
			perm[1] = 'r';
		if (statbuf.st_mode & S_IWUSR)
			perm[2] = 'w';
		if (statbuf.st_mode & S_IXUSR)
			perm[3] = 'x';
		if (statbuf.st_mode & S_IRGRP)
			perm[4] = 'r';
		if (statbuf.st_mode & S_IWGRP)
			perm[5] = 'w';
		if (statbuf.st_mode & S_IXGRP)
			perm[6] = 'x';
		if (statbuf.st_mode & S_IROTH)
			perm[7] = 'r';
		if (statbuf.st_mode & S_IWOTH)
			perm[8] = 'w';
		if (statbuf.st_mode & S_IXOTH)
			perm[9] = 'x';
		linktarget[0] = '\0';
#ifdef S_ISLNK
	}
#endif
    /* memcpy(&filetime, localtime(&(statbuf.st_mtime)), sizeof(struct tm)); */
/*Start of ATP网络协议组 2010-4-1 14:34 forAU4D02426  by h00163136*/
/*修正问题单AU4D02426后，系统时间正确，不需该调整*/
#if 0
    /*start of AU4D02390 ftp显示的时间差一年, by hh00163136, 20100208*/
    statbuf.st_mtime -= g_utc_offset;
    /*end of AU4D02390 ftp显示的时间差一年, by hh00163136, 20100208*/
#endif
/*End of ATP网络协议组 2010-4-1 14:34 by h00163136*/
    local_time = gmtime(&(statbuf.st_mtime));
    if (! local_time) return;
    memcpy(&filetime, local_time, sizeof(struct tm));
    time(&t);
    tea_time = gmtime(&t);
    if (! tea_time) return;
    /* if (filetime.tm_year == localtime(&t)->tm_year) */
    if (filetime.tm_year == tea_time->tm_year)
    	mystrncpy(timestr, ctime(&(statbuf.st_mtime)) + 4, 12);
    else
        strftime(timestr, sizeof(timestr), "%b %d  %G", &filetime);
    mygetpwuid(statbuf.st_uid, passwdfile, uid,sizeof(uid))[8] = 0;
    mygetpwuid(statbuf.st_gid, groupfile, gid,sizeof(gid))[8] = 0;

#if 0
    fprintf(client, "%s %3i %-8s %-8s %llu %s %s%s\r\n", perm,
            (int) statbuf.st_nlink, uid, gid,
            (unsigned long long) statbuf.st_size,
            timestr, name, linktarget);
#endif

    snprintf(buffer, sizeof(buffer), "%s %3i %-8s %-8s %llu %s %s%s\r\n",perm,
            (int) statbuf.st_nlink, uid, gid,
            (unsigned long long) statbuf.st_size,
            timestr, name, linktarget);
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
    if ( pstssldataobj != NULL )
    {
        SSL_write(pstssldataobj, (const unsigned char *)buffer, strlen(buffer));
    }
    else
    {
        fprintf(client, "%s", buffer );
    }
#else
    fprintf(client, "%s", buffer );
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/

}

void dirlist_one_file(char *name, FILE *client, char verbose)
{
    struct stat statbuf;
    struct hidegroup *tmp = hidegroups;
    char *filename_index;      /* place where filename starts in path */
    char szBuffer[512] = "";

    if (!stat(name, (struct stat *) &statbuf)) {
        if (tmp)
            do {
                if (statbuf.st_gid == tmp->data)
                    return;
            } while ((tmp = tmp->next));
    }

    /* find start of filename after path */
    filename_index = strrchr(name, '/');
    if (filename_index)
       filename_index++;   /* goto first character after '/' */
    else
       filename_index = name;    

    if (verbose)
    {
        bftpd_stat(name, client);
    }
    else
    {
#if 0
        fprintf(client, "%s\r\n", filename_index);
#endif

        snprintf( szBuffer,sizeof(szBuffer), "%s\r\n", filename_index );
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
#ifdef SUPPORT_ATP_FTPDS
        if ( pstssldataobj != NULL )
        {
            SSL_write(pstssldataobj,(const unsigned char*)szBuffer, strlen(szBuffer) );
        }
        else
        {
            fprintf(client, "%s", szBuffer);
        }
#else
        fprintf(client, "%s", szBuffer);
#endif
/*Added by yehuisheng00183935@20110702 添加ftpds功能*/
    }
}

void dirlist(char *name, FILE * client, char verbose,int bshow_hiden_files)
{
	DIR *directory;
    FILE *can_see_file;
    int show_nonreadable_files = FALSE;
    char *local_cwd = NULL;
    int i;
    int show_hidden_files = FALSE;
    glob_t globbuf;

     // 0.不支持配置文件配置，直接设置成显示所有文件包括隐藏文件
#if 0       
    if (! strcasecmp( config_getoption("SHOW_HIDDEN_FILES"), "yes") )
#endif
       show_hidden_files = bshow_hiden_files;

    if (! strcasecmp( config_getoption("SHOW_NONREADABLE_FILES"), "yes") )
       show_nonreadable_files = TRUE;

    if ((strstr(name, "/.")) && strchr(name, '*'))
        return; /* DoS protection */

	if ((directory = opendir(name))) 
        {
	     closedir(directory);
             local_cwd = bftpd_cwd_getcwd();
             chdir(name);
             glob("*", 0, NULL, &globbuf);
             if (show_hidden_files)
                 glob(".*", GLOB_APPEND, NULL, &globbuf);
	 } 
        else
        {
    	     if ( (name[0] == '*') && (show_hidden_files) )
             {
                glob(name, 0, NULL, &globbuf);
                glob(".*", GLOB_APPEND, NULL, &globbuf);
             }
             else
                glob(name, 0, NULL, &globbuf);
        }

	for (i = 0; i < globbuf.gl_pathc; i++)
        {
            if (! show_nonreadable_files) 
            {
               if ( (can_see_file = fopen(globbuf.gl_pathv[i], "r") ) == NULL)
                   continue;
               else
                   fclose(can_see_file);
            }

            dirlist_one_file(globbuf.gl_pathv[i], client, verbose);
        }

	globfree(&globbuf);
	if (local_cwd) {
		chdir(local_cwd);
		free(local_cwd);
	}
}
