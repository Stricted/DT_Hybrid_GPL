#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "commands.h"
#include "cwd.h"
#include "logging.h"
#include "mystring.h"

char *cwd = NULL;

int bftpd_cwd_chdir(char *dir)
{
    struct stat statDir;
    unsigned int ulLen = 0;
	char *tmp = bftpd_cwd_mappath(dir);

    /*Added by yehuisheng00183935@20110916 修改问题单(DTS2011083001151 DTS2011082905960)*/
    if( ! ( stat( tmp, &statDir ) >= 0 && S_ISDIR( statDir.st_mode ) ) )
    {
        free(tmp);
        return errno;
    }
    /*Added by yehuisheng00183935@20110916 修改问题单(DTS2011083001151 DTS2011082905960)*/

	if (chdir(tmp)) {
/*add by l63336 for 解决windows登陆后处理中文路径不正确问题*/
#if 0
            free(tmp);
            return errno;
#else
        //add by w145036 for resolving the directory name such as /?\234z ,\202\303 
 	    //if ( NULL != strstr(tmp,"?\/") )
 	     // control_printf(SL_SUCCESS, "lvxin 000");
      /*Start of modified by lvxin 00135113@20090616 for AU4D01746 bftpd 在ntfs大文件拷贝问题*/

        #if 0
            if ( NULL != memcmp(tmp,'?',1) ||  NULL != memcmp(tmp,'\\',1) || NULL != strstr(tmp,"?\/") )
        #else
            if ((NULL != strchr(tmp,'\\')) ||  (NULL != strchr(tmp,'/')) || (NULL != strchr(tmp,':')) 
            ||(NULL != strchr(tmp,'*')) ||  (NULL != strchr(tmp,'?')) || (NULL != strchr(tmp,'"'))
            ||(NULL != strchr(tmp,'<')) ||  (NULL != strchr(tmp,'>')) || (NULL != strchr(tmp,'|')))
        #endif
            {
               // control_printf(SL_SUCCESS, "lvxin 11111");
                bftpd_log("%d directory to '%s'.\n",__LINE__,tmp);
                goto Right;
            }
            else
            {
               // control_printf(SL_SUCCESS, "lvxin 222");
                free(tmp);
                return errno;
            }
      /*End of modified by lvxin 00135113@20090616 for AU4D01746 bftpd 在ntfs大文件拷贝问题*/

#endif            
/*add by l63336 for 解决windows登陆后处理中文路径不正确问题*/
	}
Right:	
    ulLen = strlen(tmp) + 1;
	cwd = realloc(cwd, ulLen);
    snprintf(cwd, ulLen, "%s", tmp);
	new_umask();
	free(tmp);
	return 0;
}

char *bftpd_cwd_getcwd()
{
      if (cwd)
	 return strdup(cwd);
      else
         return NULL;
}

void appendpath(char *result, unsigned int bufsize, char *tmp)
{
	if (!strcmp(tmp, "."))
		return;
	if (!strcmp(tmp, "..")) {
        if (strcmp(result, "/")) {
            if (result[strlen(result) - 1] == '/')
                result[strlen(result) - 1] = '\0';
            tmp = result;
            while (strchr(tmp, '/'))
                tmp = strchr(tmp, '/') + 1;
            *tmp = '\0';
            if ((result[strlen(result) - 1] == '/') && (strlen(result) > 1))
                result[strlen(result) - 1] = '\0';
        }
	} else {
		if (result[strlen(result) - 1] != '/')
		{      
			strncat(result, "/", bufsize - strlen(result) - 1);
		}
		strncat(result, tmp, bufsize - strlen(result) - 1);
	}
}



char *bftpd_cwd_mappath(char *path)
{
    int iBufferSize = strlen(path) + strlen(cwd) + 16;
	char *result = malloc( iBufferSize );
	char *path2;
	char *tmp;

    if (! result)
    {
        return NULL;
    }

    path2 = strdup(path);
    if (! path2)
    {
        free(result);
        return NULL;
    }

    if (path[0] == '/')
    {
        snprintf(result, iBufferSize, "%s","/"); 
    }
    else
    {
        snprintf(result, iBufferSize, "%s",cwd); 
    }

	while (strchr(path2, '/')) {
		tmp = strdup(path2);
        if (tmp)
        {
            *strchr(tmp, '/') = '\0';
            cutto(path2, strlen(tmp) + 1);
            appendpath(result, iBufferSize, tmp);
            free(tmp);
        }
	}
	appendpath(result, iBufferSize, path2);
	free(path2);
	return result;
}

void bftpd_cwd_init()
{
	cwd = malloc(256);
	getcwd(cwd, 255);
}

void bftpd_cwd_end()
{
	if (cwd) {
		free(cwd);
		cwd = NULL;
	}
}
