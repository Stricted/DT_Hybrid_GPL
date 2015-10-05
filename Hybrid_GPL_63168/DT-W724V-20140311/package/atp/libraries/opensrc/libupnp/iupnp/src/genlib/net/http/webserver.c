/**************************************************************************
 *
 * Copyright (c) 2000-2003 Intel Corporation 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 *
 * - Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * - Neither name of Intel Corporation nor the names of its contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************/

/*!
 * \file
 *
 * \brief Defines the Web Server and has functions to carry out
 * operations of the Web Server.
 */

#include "config.h"

#include "webserver.h"

#include "httpparser.h"
#include "httpreadwrite.h"
#include "ithread.h"
#include "membuffer.h"
#include "ssdplib.h"
#include "statcodes.h"
#include "strintmap.h"
#include "unixutil.h"
#include "upnp.h"
#include "upnpapi.h"
#include "UpnpIntTypes.h"
#include "UpnpStdInt.h"
#include "upnputil.h"
#include "VirtualDir.h"
#include "atpconfig.h"
#ifdef SUPPORT_ATP_DMS_UPLOAD
	#include "mdbapi.h"
#endif

#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "avformat.h"

#define WRITEFILE_DATA_BUFFER (256*1024)
/*!
 * Response Types.
 */
enum resp_type {
	RESP_FILEDOC,
	RESP_XMLDOC,
	RESP_HEADERS,
	RESP_WEBDOC,
	RESP_POST
};

/* mapping of file extension to content-type of document */
struct document_type_t {
	/*! . */
	const char *file_ext;
	/*! . */
	const char *content_type;
	/*! . */
	const char *content_subtype;
};

struct xml_alias_t {
       struct  xml_alias_t* pnext;
	/*! name of DOC from root; e.g.: /foo/bar/mydesc.xml */
	membuffer name;
	/*! the XML document contents */
	membuffer doc;
	/*! . */
	time_t last_modified;
	/*! . */
	int *ct;
};

static const char *gMediaTypes[] = {
	/*! 0. */
	NULL,
	/*! 1. */
	"audio",
	/*! 2. */
	"video",
	/*! 3. */
	"image",
	/*! 4. */
	"application",
	/*! 5. */
	"text"
};

/*
 * Defines.
 */

/* index into 'gMediaTypes' */
#define AUDIO_STR        "\1"
#define VIDEO_STR        "\2"
#define IMAGE_STR        "\3"
#define APPLICATION_STR  "\4"
#define TEXT_STR         "\5"

/* int index */
#define APPLICATION_INDEX 4
#define TEXT_INDEX        5

/* general */
#define NUM_MEDIA_TYPES       69


/* sorted by file extension; must have 'NUM_MEDIA_TYPES' extensions */
static const char *gEncodedMediaTypes =
	"aif\0" AUDIO_STR "aiff\0"
	"aifc\0" AUDIO_STR "aiff\0"
	"aiff\0" AUDIO_STR "aiff\0"
	"asf\0" VIDEO_STR "x-ms-asf\0"
	"asx\0" VIDEO_STR "x-ms-asf\0"
	"au\0" AUDIO_STR "basic\0"
	"avi\0" VIDEO_STR "msvideo\0"
	"bmp\0" IMAGE_STR "bmp\0"
	"dcr\0" APPLICATION_STR "x-director\0"
	"dib\0" IMAGE_STR "bmp\0"
	"dir\0" APPLICATION_STR "x-director\0"
	"dxr\0" APPLICATION_STR "x-director\0"
	"gif\0" IMAGE_STR "gif\0"
	"hta\0" TEXT_STR "hta\0"
	"htm\0" TEXT_STR "html\0"
	"html\0" TEXT_STR "html\0"
	"jar\0" APPLICATION_STR "java-archive\0"
	"jfif\0" IMAGE_STR "pjpeg\0"
	"jpe\0" IMAGE_STR "jpeg\0"
	"jpeg\0" IMAGE_STR "jpeg\0"
	"jpg\0" IMAGE_STR "jpeg\0"
	"js\0" APPLICATION_STR "x-javascript\0"
	"kar\0" AUDIO_STR "midi\0"
	"m3u\0" AUDIO_STR "mpegurl\0"
	"mid\0" AUDIO_STR "midi\0"
	"midi\0" AUDIO_STR "midi\0"
	"mov\0" VIDEO_STR "quicktime\0"
	"mp2v\0" VIDEO_STR "x-mpeg2\0"
	"mp3\0" AUDIO_STR "mpeg\0"
	"mpe\0" VIDEO_STR "mpeg\0"
	"mpeg\0" VIDEO_STR "mpeg\0"
	"mpg\0" VIDEO_STR "mpeg\0"
	"mpv\0" VIDEO_STR "mpeg\0"
	"mpv2\0" VIDEO_STR "x-mpeg2\0"
	"pdf\0" APPLICATION_STR "pdf\0"
	"pjp\0" IMAGE_STR "jpeg\0"
	"pjpeg\0" IMAGE_STR "jpeg\0"
	"plg\0" TEXT_STR "html\0"
	"pls\0" AUDIO_STR "scpls\0"
	"png\0" IMAGE_STR "png\0"
	"qt\0" VIDEO_STR "quicktime\0"
	"ram\0" AUDIO_STR "x-pn-realaudio\0"
	"rmi\0" AUDIO_STR "mid\0"
	"rmm\0" AUDIO_STR "x-pn-realaudio\0"
	"rtf\0" APPLICATION_STR "rtf\0"
	"shtml\0" TEXT_STR "html\0"
	"smf\0" AUDIO_STR "midi\0"
	"snd\0" AUDIO_STR "basic\0"
	"spl\0" APPLICATION_STR "futuresplash\0"
	"ssm\0" APPLICATION_STR "streamingmedia\0"
	"swf\0" APPLICATION_STR "x-shockwave-flash\0"
	"tar\0" APPLICATION_STR "tar\0"
	"tcl\0" APPLICATION_STR "x-tcl\0"
	"text\0" TEXT_STR "plain\0"
	"tif\0" IMAGE_STR "tiff\0"
	"tiff\0" IMAGE_STR "tiff\0"
	"txt\0" TEXT_STR "plain\0"
	"ulw\0" AUDIO_STR "basic\0"
	"wav\0" AUDIO_STR "wav\0"
	"wax\0" AUDIO_STR "x-ms-wax\0"
	"wm\0" VIDEO_STR "x-ms-wm\0"
	"wma\0" AUDIO_STR "x-ms-wma\0"
	"wmv\0" VIDEO_STR "x-ms-wmv\0"
	"wvx\0" VIDEO_STR "x-ms-wvx\0"
	"xbm\0" IMAGE_STR "x-xbitmap\0"
	"xml\0" TEXT_STR "xml\0"
	"xsl\0" TEXT_STR "xml\0"
	"z\0" APPLICATION_STR "x-compress\0"
	"zip\0" APPLICATION_STR "zip\0" "\0";
/* *** end *** */

/*!
 * module variables - Globals, static and externs.
 */

static struct document_type_t gMediaTypeList[NUM_MEDIA_TYPES];

/*! Global variable. A local dir which serves as webserver root. */
membuffer gDocumentRootDir;

/*! XML document. */
static struct xml_alias_t* gAliasDoc = NULL;
static ithread_mutex_t gWebMutex;
extern int Http_Header_Num;
extern str_int_entry Http_Header_Names[];

/*!
 * \brief Decodes list and stores it in gMediaTypeList.
 */
static UPNP_INLINE void media_list_init(void)
{
	int i;
	const char *s = gEncodedMediaTypes;
	struct document_type_t *doc_type;

	for (i = 0; *s != '\0'; i++) {
		doc_type = &gMediaTypeList[i];
		doc_type->file_ext = s;
		/* point to type. */
		s += strlen(s) + 1;
		doc_type->content_type = gMediaTypes[(int)*s];
		/* point to subtype. */
		s++;
		doc_type->content_subtype = s;
		/* next entry. */
		s += strlen(s) + 1;
	}
	assert(i == NUM_MEDIA_TYPES);
}

/*!
 * \brief Based on the extension, returns the content type and content
 * subtype.
 *
 * \return
 * \li \c 0 on success
 * \li \c -1 on error
 */
static UPNP_INLINE int search_extension(
	/*! [in] . */
	const char *extension,
	/*! [out] . */
	const char **con_type,
	/*! [out] . */
	const char **con_subtype)
{
	int top, mid, bot;
	int cmp;

	top = 0;
	bot = NUM_MEDIA_TYPES - 1;

	while (top <= bot) {
		mid = (top + bot) / 2;
		cmp = strcasecmp(extension, gMediaTypeList[mid].file_ext);
		if (cmp > 0) {
			/* look below mid. */
			top = mid + 1;
		} else if (cmp < 0) {
			/* look above mid. */
			bot = mid - 1;
		} else {
			/* cmp == 0 */
			*con_type = gMediaTypeList[mid].content_type;
			*con_subtype = gMediaTypeList[mid].content_subtype;
			return 0;
		}
	}

	return -1;
}

/*!
 * \brief Based on the extension, clones an XML string based on type and
 * content subtype. If content type and sub type are not found, unknown
 * types are used.
 *
 * \return
 * \li \c 0 on success.
 * \li \c UPNP_E_OUTOF_MEMORY - on memory allocation failures.
 */
static UPNP_INLINE int get_content_type(
	/*! [in] . */
	const char *filename,
	/*! [out] . */
	DOMString *content_type)
{
	const char *extension;
	const char *type;
	const char *subtype;
	int ctype_found = FALSE;
	char *temp = NULL;
	size_t length = 0;
	int rc = 0;

	(*content_type) = NULL;
	/* get ext */
	extension = strrchr(filename, '.');
	if (extension != NULL)
		if (search_extension(extension + 1, &type, &subtype) == 0)
			ctype_found = TRUE;
	if (!ctype_found) {
		/* unknown content type */
		type = gMediaTypes[APPLICATION_INDEX];
		subtype = "octet-stream";
	}
	length = strlen(type) + strlen("/") + strlen(subtype) + 1;
	temp = malloc(length);
	if (!temp)
		return UPNP_E_OUTOF_MEMORY;
	rc = snprintf(temp, length, "%s/%s", type, subtype);
	if (rc < 0 || (unsigned int) rc >= length) {
		free(temp);
		return UPNP_E_OUTOF_MEMORY;
	}
	(*content_type) = ixmlCloneDOMString(temp);
	free(temp);
	if (!(*content_type))
		return UPNP_E_OUTOF_MEMORY;

	return 0;
}

/*!
 * \brief Check for the validity of the XML object buffer.
 *
 * \return BOOLEAN.
 */
static UPNP_INLINE int is_valid_alias(
	/*! [in] XML alias object. */
	const struct xml_alias_t *alias)
{
	return alias->doc.buf != NULL;
}

/*!
 * \brief Release the XML document referred to by the input parameter. Free
 * the allocated buffers associated with this object.
 */
static UPNP_INLINE void alias_release(
	/*! [in] XML alias object. */
	struct xml_alias_t *alias)
{
	ithread_mutex_lock(&gWebMutex);
	/* ignore invalid alias */
	if (!is_valid_alias(alias)) {
		ithread_mutex_unlock(&gWebMutex);
		return;
	}
        assert(*alias->ct > 0);
        *alias->ct -= 1;
	if (*alias->ct <= 0) {
		membuffer_destroy(&alias->doc);
		membuffer_destroy(&alias->name);
		free(alias->ct);
	}
	ithread_mutex_unlock(&gWebMutex);
}


/*!
 * \brief Initialize the global XML document. Allocate buffers for the XML
 * document.
 */
static UPNP_INLINE void glob_alias_init(void)
{
/*	struct xml_alias_t *alias = &gAliasDoc;

	membuffer_init(&alias->doc);
	membuffer_init(&alias->name);
	alias->ct = NULL;
	alias->last_modified = 0;
*/
    ithread_mutex_lock(&gWebMutex);
    struct xml_alias_t* temp = gAliasDoc, *pnext;
    while( temp ){
        pnext = temp->pnext;
       // alias_release( temp );
       (*temp->ct)--;
       if( *temp->ct == 0 )
       {
            membuffer_destroy(&temp->doc);
            membuffer_destroy(&temp->name);
	     free(temp->ct);
        }
       free(temp);
       temp = pnext;
    }
    gAliasDoc = NULL;
    ithread_mutex_unlock(&gWebMutex);
}



/*!
 * \brief Copy the contents of the global XML document into the local output
 * parameter.
 */
static void alias_grab(
	/*! [out] XML alias object. */
	struct xml_alias_t *alias, struct xml_alias_t* aliasRc )
{
	ithread_mutex_lock(&gWebMutex);
	assert(is_valid_alias(aliasRc));
	memcpy(alias, aliasRc, sizeof(struct xml_alias_t));
	*alias->ct = *alias->ct + 1;
	ithread_mutex_unlock(&gWebMutex);
}

static struct xml_alias_t* find_alias_fromlist( const char* alias_name )
{
    ithread_mutex_lock(&gWebMutex);
    struct xml_alias_t* temp = gAliasDoc;
    while( temp ){
        if (strcmp( alias_name, temp->name.buf ) == 0 ){
            	ithread_mutex_unlock(&gWebMutex);
            return temp;
        }
        temp = temp->pnext;
    }
    ithread_mutex_unlock(&gWebMutex);
    return NULL;
}

static void set_alias_tolist( struct xml_alias_t *alias )
{
    
    struct xml_alias_t* temp = gAliasDoc, *last = NULL;
    ithread_mutex_lock(&gWebMutex);
    while( temp ){
        if (strcmp( alias->name.buf, temp->name.buf ) == 0 )
            break;
        last = temp;
        temp = last->pnext;
    }
    if (last){
        last->pnext = alias;
    }else{
        gAliasDoc = alias;
    }
    if (temp){
       alias->pnext = temp->pnext;
       (*temp->ct)--;
       if (*temp->ct==0)
       {//���ܱ���������
           membuffer_destroy(&temp->doc);
           membuffer_destroy(&temp->name);
           free(temp->ct);  
       }
        free(temp);
    }
    ithread_mutex_unlock(&gWebMutex);
}

int web_server_set_alias(const char *alias_name, 
	const char *alias_content, size_t alias_content_length,
	time_t last_modified)
{
	int ret_code;
	struct xml_alias_t* alias;
       if (alias_name == NULL) {
		/* don't serve aliased doc anymore */
		return 0;
	}

       
	assert(alias_content != NULL);
       //�����µ�
       alias = (struct xml_alias_t*)malloc( sizeof( struct xml_alias_t ) );
       if ( alias == NULL )
            return UPNP_E_OUTOF_MEMORY;
       memset( alias, 0, sizeof(struct xml_alias_t ) );
	membuffer_init(&alias->doc);
	membuffer_init(&alias->name);
	alias->ct = NULL;
	do {
		/* insert leading /, if missing */
		if (*alias_name != '/')
			if (membuffer_assign_str(&alias->name, "/") != 0)
				break;	/* error; out of mem */
		ret_code = membuffer_append_str(&alias->name, alias_name);
		if (ret_code != 0)
			break;	/* error */
		if ((alias->ct = (int *)malloc(sizeof(int))) == NULL)
			break;	/* error */
		*alias->ct = 1;
		membuffer_attach(&alias->doc, (char *)alias_content,
				 alias_content_length);
		alias->last_modified = last_modified;
              set_alias_tolist( alias );
		/* save in module var */
		return 0;
	} while (FALSE);
	/* error handler */
	/* free temp alias */
	membuffer_destroy(&alias->name);
	membuffer_destroy(&alias->doc);
	free(alias->ct);
	free(alias);

	return UPNP_E_OUTOF_MEMORY;
}

int web_server_init()
{
	int ret = 0;

	if (bWebServerState == WEB_SERVER_DISABLED) {
		/* decode media list */
		media_list_init();
		membuffer_init(&gDocumentRootDir);
		glob_alias_init();
		pVirtualDirList = NULL;

		/* Initialize callbacks */
		virtualDirCallback.get_info = NULL;
		virtualDirCallback.open = NULL;
		virtualDirCallback.read = NULL;
		virtualDirCallback.write = NULL;
		virtualDirCallback.seek = NULL;
		virtualDirCallback.close = NULL;

		if (ithread_mutex_init(&gWebMutex, NULL) == -1)
			ret = UPNP_E_OUTOF_MEMORY;
		else
			bWebServerState = WEB_SERVER_ENABLED;
	}

	return ret;
}

void web_server_destroy(void)
{
	int ret;

	if (bWebServerState == WEB_SERVER_ENABLED) {
		membuffer_destroy(&gDocumentRootDir);
	//	alias_release(&gAliasDoc);

    /*		ithread_mutex_lock(&gWebMutex);
		memset(&gAliasDoc, 0, sizeof(struct xml_alias_t));
		ithread_mutex_unlock(&gWebMutex);

		ret = ithread_mutex_destroy(&gWebMutex);
		assert(ret == 0);
	*/
	    glob_alias_init();
	    bWebServerState = WEB_SERVER_DISABLED;
	}
}

/*!
 * \brief Release memory allocated for the global web server root directory
 * and the global XML document. Resets the flag bWebServerState to
 * WEB_SERVER_DISABLED.
 *
 * \return Integer.
 */
static int get_file_info(
	/*! [in] Filename having the description document. */
	const char *filename,
	/*! [out] File information object having file attributes such as filelength,
	 * when was the file last modified, whether a file or a directory and
	 * whether the file or directory is readable. */
	struct File_Info *info)
{
	int code;
	struct stat s;
	FILE *fp;
	int rc = 0;

	ixmlFreeDOMString(info->content_type);	
	info->content_type = NULL;
	code = stat(filename, &s);
	if (code == -1)
		return -1;
	if (S_ISDIR(s.st_mode))
		info->is_directory = TRUE;
	else if (S_ISREG(s.st_mode))
		info->is_directory = FALSE;
	else
		return -1;
	/* check readable */
	fp = fopen(filename, "r");
	info->is_readable = (fp != NULL);
	if (fp)
		fclose(fp);
	info->file_length = s.st_size;
	info->last_modified = s.st_mtime;
	rc = get_content_type(filename, &info->content_type);
	UpnpPrintf(UPNP_INFO, HTTP, __FILE__, __LINE__,
		"file info: %s, length: %lld, last_mod=%s readable=%d\n",
		filename, (long long)info->file_length,
		asctime(gmtime(&info->last_modified)), info->is_readable);

	return rc;
}

int web_server_set_root_dir(const char *root_dir)
{
	size_t index;
	int ret;

	ret = membuffer_assign_str(&gDocumentRootDir, root_dir);
	if (ret != 0)
		return ret;
	/* remove trailing '/', if any */
	if (gDocumentRootDir.length > 0) {
		index = gDocumentRootDir.length - 1;	/* last char */
		if (gDocumentRootDir.buf[index] == '/')
			membuffer_delete(&gDocumentRootDir, index, 1);
	}

	return 0;
}

/*!
 * \brief Compare the files names between the one on the XML alias the one
 * passed in as the input parameter. If equal extract file information.
 *
 * \return
 * \li \c TRUE - On Success
 * \li \c FALSE if request is not an alias
 */
static UPNP_INLINE int get_alias(
	/*! [in] request file passed in to be compared with. */
	const char *request_file,
	/*! [out] xml alias object which has a file name stored. */
	struct xml_alias_t *alias,
	/*! [out] File information object which will be filled up if the file
	 * comparison succeeds. */
	struct File_Info *info)
{
	int cmp = strcmp(alias->name.buf, request_file);
	if (cmp == 0) {
		/* fill up info */
		info->file_length = (off_t)alias->doc.length;
		info->is_readable = TRUE;
		info->is_directory = FALSE;
		info->last_modified = alias->last_modified;
	}

	return cmp == 0;
}

/*!
 * \brief Compares filePath with paths from the list of virtual directory
 * lists.
 *
 * \return BOOLEAN.
 */
static int isFileInVirtualDir(
	/*! [in] Directory path to be tested for virtual directory. */
	char *filePath)
{
	virtualDirList *pCurVirtualDir;
	size_t webDirLen;

	pCurVirtualDir = pVirtualDirList;
	while (pCurVirtualDir != NULL) {
		webDirLen = strlen(pCurVirtualDir->dirName);
		if (webDirLen) {
			if (pCurVirtualDir->dirName[webDirLen - 1] == '/') {
				if (strncmp(pCurVirtualDir->dirName, filePath,
						webDirLen) == 0)
					return !0;
			} else {
				if (strncmp(pCurVirtualDir->dirName, filePath,
						webDirLen) == 0 &&
				    (filePath[webDirLen] == '/' ||
				     filePath[webDirLen] == '\0' ||
				     filePath[webDirLen] == '?'))
					return !0;
			}
		}
		pCurVirtualDir = pCurVirtualDir->next;
	}

	return 0;
}

/*!
 * \brief Converts input string to upper case.
 */
static void ToUpperCase(
	/*! Input string to be converted. */
	char *s)
{
	while (*s) {
		*s = (char)toupper(*s);
		++s;
	}
}

/*!
 * \brief Finds a substring from a string in a case insensitive way.
 *
 * \return A pointer to the first occurence of s2 in s1.
 */
static char *StrStr(
	/*! Input string. */
	char *s1,
	/*! Input sub-string. */
	const char *s2)
{
	char *Str1;
	char *Str2;
	const char *Ptr;
	char *ret = NULL;

	Str1 = strdup(s1);
	if (!Str1)
		goto error1;
	Str2 = strdup(s2);
	if (!Str2)
		goto error2;
	ToUpperCase(Str1);
	ToUpperCase(Str2);
	Ptr = strstr(Str1, Str2);
	if (!Ptr)
		ret = NULL;
	else
		ret = s1 + (Ptr - Str1);

	free(Str2);
error2:
	free(Str1);
error1:
	return ret;
}

/*!
 * \brief Finds next token in a string.
 *
 * \return Pointer to the next token.
 */
static char *StrTok(
	/*! String containing the token. */
	char **Src,
	/*! Set of delimiter characters. */
	const char *Del)
{
	char *TmpPtr;
	char *RetPtr;

	if (*Src != NULL) {
		RetPtr = *Src;
		TmpPtr = strstr(*Src, Del);
		if (TmpPtr != NULL) {
			*TmpPtr = '\0';
			*Src = TmpPtr + strlen(Del);
		} else
			*Src = NULL;

		return RetPtr;
	}

	return NULL;
}

/*!
 * \brief Returns a range of integers from a string.
 *
 * \return Always returns 1.
 */
static int GetNextRange(
	/*! string containing the token / range. */
	char **SrcRangeStr,
	/*! gets the first byte of the token. */
	off_t *FirstByte,
	/*! gets the last byte of the token. */
	off_t *LastByte)
{
    char *Tok;
    int64_t F = -1;
    int64_t L = -1;

	if (*SrcRangeStr == NULL)
		return -1;
	Tok = StrTok(SrcRangeStr, ",");
	if (strstr(Tok, "-") == NULL)
		return -1;

    sscanf( Tok, "%"SCNd64"-%"SCNd64, &F, &L );

    *FirstByte = (off_t)F;
    *LastByte = (off_t)L;

    return 1;
}

typedef struct {
	int64_t hour;
	int64_t min;
	int64_t sec;
	int64_t usec;
} NTP_ST;

int string2time(char *str, NTP_ST *ntp)
{
    int count = 0;
    int64_t num[2];
    char *p;
    char *q;
    int i,j;

    if(NULL == str)
    return -1;
    //printf("str=[%s]\n", str);

    p = str;
    //ȡСʱ�ͷ���
    while(count < 2)
    {
        q = strchr(p, ':');
        if(q !=NULL ) 
        {
            num[count] = atoll(p);
            p = q + 1;
            count++;
        }
        else
        {
            break;
        }
    }
    
    if(count == 1)
    {
        ntp->min = num[0];
    }
    else if(count == 2) 
    {
        ntp->hour = num[0];
        ntp->min = num[1];
    }

    q = strchr(p, '.');
    if(q !=NULL && strlen(q)>1) 
    {
        ntp->sec = atoll(p);
        p = q + 1;
        ntp->usec = atoll(p);
        ntp->usec *= 1000000;
        j = strlen(p);
        for(i=0; i<j; i++)
        {
            ntp->usec /= 10;
        }
    }
    else
    {
        ntp->sec = atoll(p);
    }

    return 0;
}


int64_t 
GetNextTimeRange(char *FullPathName, off_t FileLength,char **SrcTimeRangeStr,
              off_t *FirstByte, off_t *LastByte )
{
    char *Tok = NULL;
    AVFormatContext       *pstFormatCtx = NULL;
    int64_t duration = 0;
    char time1[512]={0}, time2[512]={0};
    NTP_ST npt1={0}, npt2={0};
    int64_t  F = 0, L = 0;
    
    if( *SrcTimeRangeStr == NULL ) 
    {
        return -1;
    }

    Tok = StrTok( SrcTimeRangeStr, "," );
    
    if (strstr(Tok, "-") == NULL ) 
    {
        return -1;
    }

    if(strlen(Tok) > sizeof(time1))
    {
        return -1;
    }

    sscanf(Tok, "%[^-]-%s", time1, time2);

    if(0 == strlen(time1))
    {
        return -2;
    }
        
    //����"-"ǰ���ʱ��
    string2time(time1, &npt1);

    F = (npt1.hour*3600 + npt1.min*60 + npt1.sec)*1000000 + npt1.usec;
    
    //����"-"�����ʱ��
    if(strlen(time2) > 0)
    {
        string2time(time2, &npt2);        
    }
    
    L = (npt2.hour*3600 + npt2.min*60 + npt2.sec)*1000000 + npt2.usec;

    if(npt2.hour >=9999 || npt1.hour >= 9999)
    {
        return -1;
    }
    
    if(av_open_input_file(&pstFormatCtx, FullPathName, NULL, 0, NULL) == 0)
    {
        if(av_find_stream_info(pstFormatCtx) >= 0 )
        {
            duration = pstFormatCtx->duration;
            if(strlen(time2) == 0)
            {
                L = duration;        
            }
            
            if(0 == F)
            {
                *FirstByte = 0;
            }
            else
            {
                av_seek_frame(pstFormatCtx, -1, F, AVSEEK_FLAG_ANY);
                *FirstByte = (off_t)(pstFormatCtx->pb->pos);
                if(*FirstByte >= FileLength)
                {
                    *FirstByte = FileLength-1024;
                }
            }

            if(L/1000 == duration/1000)
            {
                *LastByte = FileLength-1;
            }
            else
            {
                av_seek_frame(pstFormatCtx, -1, L, AVSEEK_FLAG_ANY);
                *LastByte = (off_t)(pstFormatCtx->pb->pos);
            }
        }
        av_close_input_file(pstFormatCtx);
    }

    if(F < L && F <= duration && L <= duration)
    {
        return duration/1000;
    }
    else
    {
        return -1;
    }
    
}

/************************************************************************
 * Function: CreateHTTPRangeResponseHeader
 *
 * Parameters:
 *	char * ByteRangeSpecifier ; String containing the range
 *	long FileLength ; Length of the file
 *	OUT struct SendInstruction * Instr ; SendInstruction object
 *		where the range operations will be stored
 *
 * Description: Fills in the Offset, read size and contents to send out
 *	as an HTTP Range Response
 *
 * \return
 * \li \c HTTP_BAD_REQUEST
 * \li \c UPNP_E_OUTOF_MEMORY
 * \li \c HTTP_REQUEST_RANGE_NOT_SATISFIABLE
 * \li \c HTTP_OK
 */
static int CreateHTTPRangeResponseHeader(
	/*! String containing the range. */
	char *ByteRangeSpecifier,
	/*! Length of the file. */
	off_t FileLength,
	/*! [out] SendInstruction object where the range operations will be stored. */
	struct SendInstruction *Instr)
{
	off_t FirstByte, LastByte;
	char *RangeInput;
	char *Ptr;
	int rc = 0;

	Instr->IsRangeActive = 1;
	Instr->IsTimeSeek = 0;
	Instr->ReadSendSize = FileLength;
	if (!ByteRangeSpecifier)
		return HTTP_BAD_REQUEST;
	RangeInput = malloc(strlen(ByteRangeSpecifier) + 1);
	if (!RangeInput)
		return UPNP_E_OUTOF_MEMORY;
	memset(RangeInput, 0, strlen(ByteRangeSpecifier) + 1);
	strncpy(RangeInput, ByteRangeSpecifier, strlen(ByteRangeSpecifier));
	/* CONTENT-RANGE: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT */
	if (StrStr(RangeInput, "bytes") == NULL ||
		(Ptr = StrStr(RangeInput, "=")) == NULL) {
		free(RangeInput);
		Instr->IsRangeActive = 0;
		return HTTP_BAD_REQUEST;
	}
	/* Jump = */
	Ptr = Ptr + 1;
	if (FileLength < 0) {
		free(RangeInput);
		return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
	}
	if (GetNextRange(&Ptr, &FirstByte, &LastByte) != -1) {
		if(FirstByte > 0 && LastByte > 0 && LastByte < FirstByte) {
			free( RangeInput );
			return HTTP_BAD_REQUEST;
		}

		if (FileLength < FirstByte) {
			free(RangeInput);
			return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
		}

		if (FirstByte >= 0 && LastByte >= 0 && LastByte >= FirstByte) {
			if (LastByte >= FileLength)
				LastByte = FileLength - 1;
			Instr->RangeOffset = FirstByte;
			Instr->ReadSendSize = LastByte - FirstByte + 1;
			/* Data between two range. */
			rc = snprintf( Instr->RangeHeader,
				sizeof(Instr->RangeHeader),
				"CONTENT-RANGE: bytes %"PRId64"-%"PRId64"/%"PRId64"\r\n",
				(int64_t)FirstByte,
				(int64_t)LastByte,
				(int64_t)FileLength);
			if (rc < 0 || (unsigned int) rc >= sizeof(Instr->RangeHeader)) {
				free(RangeInput);
				return UPNP_E_OUTOF_MEMORY;
			}
		} else if (FirstByte >= 0 && LastByte == -1
			   && FirstByte < FileLength) {
			Instr->RangeOffset = FirstByte;
			Instr->ReadSendSize = FileLength - FirstByte;
			rc = snprintf(Instr->RangeHeader,
				sizeof(Instr->RangeHeader),
				"CONTENT-RANGE: bytes %"PRId64"-%"PRId64"/%"PRId64"\r\n",
				(int64_t)FirstByte,
				(int64_t)(FileLength - 1),
				(int64_t)FileLength);
			if (rc < 0 || (unsigned int) rc >= sizeof(Instr->RangeHeader)) {
				free(RangeInput);
				return UPNP_E_OUTOF_MEMORY;
			}
		} else if (FirstByte == -1 && LastByte > 0) {
			if (LastByte >= FileLength) {
				Instr->RangeOffset = 0;
				Instr->ReadSendSize = FileLength;
				rc = snprintf(Instr->RangeHeader,
					sizeof(Instr->RangeHeader),
					"CONTENT-RANGE: bytes 0-%"PRId64"/%"PRId64"\r\n",
					(int64_t)(FileLength - 1),
					(int64_t)FileLength);
			} else {
				Instr->RangeOffset = 0;
				Instr->ReadSendSize = LastByte+1;
				rc = snprintf( Instr->RangeHeader,
					sizeof(Instr->RangeHeader),
					"CONTENT-RANGE: bytes %"PRId64"-%"PRId64"/%"PRId64"\r\n",
					(int64_t)0,
					(int64_t)LastByte,
					(int64_t)FileLength);
			}
			if (rc < 0 || (unsigned int) rc >= sizeof(Instr->RangeHeader)) {
				free(RangeInput);
				return UPNP_E_OUTOF_MEMORY;
			}
		} else {
			free(RangeInput);
			return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
		}
	} else {
		free(RangeInput);
		return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
	}

	free(RangeInput);
	return HTTP_OK;
}

int
CreateHTTPTimeSeekResponseHeader(char *TimeSeekSpecifier,
                               off_t FileLength, char *FullPathName,
                               OUT struct SendInstruction *Instr )
{

    off_t FirstByte, LastByte;
    char *RangeInput, *Ptr;
    int64_t duration = 0;
    Instr->IsRangeActive = 1;
    Instr->ReadSendSize = FileLength;

    if( !TimeSeekSpecifier )
        return HTTP_BAD_REQUEST;

    RangeInput = malloc( strlen( TimeSeekSpecifier ) + 1 );
    if( !RangeInput )
        return UPNP_E_OUTOF_MEMORY;
    strcpy( RangeInput, TimeSeekSpecifier );

    //CONTENT-RANGE: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT
    if( StrStr( RangeInput, "npt" ) == NULL ||
        ( Ptr = StrStr( RangeInput, "=" ) ) == NULL ) {
        free( RangeInput );
        Instr->IsRangeActive = 0;
        return HTTP_BAD_REQUEST;
    }
    //Jump =
    Ptr = Ptr + 1;

    if( FileLength < 0 ) {
        free( RangeInput );
        return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
    }

    if( (duration = GetNextTimeRange(FullPathName, FileLength, &Ptr, &FirstByte, &LastByte )) > 0 ) {
        if(FirstByte > 0 && LastByte > 0 && LastByte < FirstByte)
        {
            free( RangeInput );
            return HTTP_BAD_REQUEST;
        }

        if( FileLength < FirstByte ) {
            free( RangeInput );
            return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
        }

        if( FirstByte >= 0 && LastByte >= 0 && LastByte >= FirstByte ) {
            if( LastByte >= FileLength )
                LastByte = FileLength - 1;

            Instr->RangeOffset = FirstByte;
            Instr->ReadSendSize = LastByte - FirstByte + 1;
            snprintf( Instr->RangeHeader, sizeof(Instr->RangeHeader),
                "TimeSeekRange.dlna.org: %s/%lld.%lld bytes=%"PRId64"-%"PRId64"/%"PRId64"\r\n",
                TimeSeekSpecifier,
                duration/1000, duration%1000,
                (int64_t)FirstByte,
                (int64_t)LastByte,
                (int64_t)FileLength );   //Data between two range.
        } else if( FirstByte >= 0 && LastByte == -1
                   && FirstByte < FileLength ) {
            Instr->RangeOffset = FirstByte;
            Instr->ReadSendSize = FileLength - FirstByte;
            snprintf( Instr->RangeHeader, sizeof(Instr->RangeHeader),
                     "TimeSeekRange.dlna.org: %s/%lld.%lld bytes=%"PRId64"-%"PRId64"/%"PRId64"\r\n",
                    TimeSeekSpecifier,
                     duration/1000, duration%1000,
                     (int64_t)FirstByte,
                     (int64_t)(FileLength - 1),
                     (int64_t)FileLength );
        } else if( FirstByte == -1 && LastByte > 0 ) {
            if( LastByte >= FileLength ) {
                Instr->RangeOffset = 0;
                Instr->ReadSendSize = FileLength;
                snprintf( Instr->RangeHeader, sizeof(Instr->RangeHeader),
                         "TimeSeekRange.dlna.org: %s/%lld.%lld bytes=0-%"PRId64"/%"PRId64"\r\n",
                         TimeSeekSpecifier,
                        duration/1000, duration%1000,
                         (int64_t)(FileLength - 1),
                         (int64_t)FileLength );
            } else {
                Instr->RangeOffset = 0;
                Instr->ReadSendSize = LastByte+1;
                snprintf( Instr->RangeHeader, sizeof(Instr->RangeHeader),
                         "TimeSeekRange.dlna.org: %s/%lld.%lld bytes=%"PRId64"-%"PRId64"/%"PRId64"\r\n",
                         TimeSeekSpecifier,
                         duration/1000, duration%1000,
                         (int64_t)0,
                         (int64_t)LastByte,
                         (int64_t)FileLength );
            }
        } else {
            free( RangeInput );
            return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
        }
    } 
    else if(-1 == duration)
    {
        free( RangeInput );
        return HTTP_REQUEST_RANGE_NOT_SATISFIABLE;
    }
    else if(-2 == duration)
    {
        free( RangeInput );
        return HTTP_BAD_REQUEST;        
    }
    
    free( RangeInput );
    Instr->IsTimeSeek =1;
    Instr->IsRangeActive =1;
    return HTTP_OK;
}

/************************************************************************
 * Function: CheckOtherHTTPHeaders
 *
 * Parameters:
 *	IN http_message_t * Req ;  HTTP Request message
 *	OUT struct SendInstruction * RespInstr ; Send Instruction object to
 *		data for the response
 *	int FileSize ;	Size of the file containing the request document
 *
 * Description: Get header id from the request parameter and take
 *	appropriate action based on the ids.
 *	as an HTTP Range Response
 *
 * \return
 * \li \c HTTP_BAD_REQUEST
 * \li \c UPNP_E_OUTOF_MEMORY
 * \li \c HTTP_REQUEST_RANGE_NOT_SATISFIABLE
 * \li \c HTTP_OK
 */
static int CheckOtherHTTPHeaders(
	/*! [in] HTTP Request message. */
	http_message_t *Req,
	/*! [out] Send Instruction object to data for the response. */
	struct SendInstruction *RespInstr,
	/*! Size of the file containing the request document. */
                       struct File_Info *finfo)
{
	http_header_t *header;
	ListNode *node;
    BOOL      bBad = FALSE;
    BOOL      bNotAccept = FALSE;
    BOOL      bInteractive = FALSE;
    BOOL      bTimeSeek = FALSE;
    BOOL      bPlaySpeed = FALSE;
    BOOL      bRealTime = FALSE;
    BOOL      bHost = FALSE;
	/*NNS: dlist_node* node; */
	int index, RetCode = HTTP_OK;
	char *TmpBuf;

	TmpBuf = (char *)malloc(LINE_SIZE);
	if (!TmpBuf)
		return UPNP_E_OUTOF_MEMORY;
	node = ListHead(&Req->headers);
	while (node != NULL) {
		header = (http_header_t *) node->item;
		/* find header type. */
		index = map_str_to_int((const char *)header->name.buf,
				header->name.length, Http_Header_Names,
                                Http_Header_Num, FALSE );
		if (header->value.length >= LINE_SIZE) {
			free(TmpBuf);
			TmpBuf = (char *)malloc(header->value.length + 1);
			if (!TmpBuf)
				return UPNP_E_OUTOF_MEMORY;
		}
		memcpy(TmpBuf, header->value.buf, header->value.length);
		TmpBuf[header->value.length] = '\0';
		if (index >= 0) {
			switch (Http_Header_Names[index].id) {
			case HDR_TE: {
				/* Request */
				RespInstr->IsChunkActive = 1;

				if (strlen(TmpBuf) > strlen("gzip")) {
					/* means client will accept trailer. */
					if (StrStr(TmpBuf, "trailers") != NULL) {
						RespInstr->IsTrailers = 1;
					}
				}
				break;
			}
            case HDR_HOST:
                bHost = TRUE;
                break;
			
			case HDR_CONTENT_LENGTH:
				RespInstr->RecvWriteSize = atoi(TmpBuf);
				break;
                case HDR_RANGE:
                    RetCode = CreateHTTPRangeResponseHeader( TmpBuf,
                                                                   finfo->file_length,
                                                                   RespInstr ); 
                    break;
			case HDR_ACCEPT_LANGUAGE:
				if (header->value.length + 1 > sizeof(RespInstr->AcceptLanguageHeader)) {
					size_t length = sizeof(RespInstr->AcceptLanguageHeader) - 1;
					memcpy(RespInstr->AcceptLanguageHeader, TmpBuf, length);
					RespInstr->AcceptLanguageHeader[length] = '\0';
				} else {
					memcpy(RespInstr->AcceptLanguageHeader, TmpBuf,
						header->value.length + 1);
				}
				break;
                case HDR_CONTENT_FEATURE_DLNA:
                    if(!((header->value.length == 1) &&(*(header->value.buf) == '1')))
                    {
                        bBad = TRUE;
                    }
                    break;
                case HDR_TIMESEEK_RANGE_DLNA:
                    if(strstr(finfo->protocolinfo,"DLNA.ORG_OP=1"))
                    {
                        /*֧��Time-based Seek*/
                        RetCode = CreateHTTPTimeSeekResponseHeader( TmpBuf,
                                                               finfo->file_length, finfo->fullpathname,
                                                               RespInstr );                             
                    }
                    else
                    {
                        bNotAccept = TRUE;
                    }

                    bTimeSeek = TRUE;
                    break;
                case HDR_PLAYSPEED_DLNA:
                    bPlaySpeed = TRUE;
                    break;
                case HDR_REALTIMEINFO_DLNA:
                    bRealTime = TRUE;
                    break;
                case HDR_TRANSFER_MODE_DLNA:
                    if(!memcmp(header->value.buf, "Streaming", strlen("Streaming")))
                    {
                        if(1 != finfo->nTransMode)
                        {
                            bNotAccept = TRUE;
                        }
                    }else  if(!memcmp(header->value.buf, "Interactive", strlen("Interactive")))
                    {
                        bInteractive = TRUE;
                        if(0 != finfo->nTransMode)
                        {
                            bNotAccept = TRUE;
                        }
                    }else  if(!memcmp(header->value.buf, "Background", strlen("Background")))
                    {
                        finfo->nTransMode = 2;
                    }
                    break;
			default:
				/*
				   TODO
				 */
				/*
				   header.value is the value.
				 */
				/*
				   case HDR_CONTENT_TYPE: return 1;
				   case HDR_CONTENT_LANGUAGE:return 1;
				   case HDR_LOCATION: return 1;
				   case HDR_CONTENT_LOCATION:return 1;
				   case HDR_ACCEPT: return 1;
				   case HDR_ACCEPT_CHARSET: return 1;
				   case HDR_USER_AGENT: return 1;
				 */

				/*Header check for encoding */
				/*
				   case HDR_ACCEPT_RANGE:
				   case HDR_CONTENT_RANGE:
				   case HDR_IF_RANGE:
				 */

				/*Header check for encoding */
				/*
				   case HDR_ACCEPT_ENCODING:
				   if(StrStr(TmpBuf, "identity"))
				   {
				   break;
				   }
				   else return -1;
				   case HDR_CONTENT_ENCODING:
				   case HDR_TRANSFER_ENCODING:
				 */
				break;
			}
		}
		node = ListNext(&Req->headers, node);
	}
	free(TmpBuf);

    if(!bHost && (1 == Req->major_version) && (1 == Req->minor_version))
        return HTTP_BAD_REQUEST;

    if(bInteractive && (bPlaySpeed || bTimeSeek || bRealTime))
        return HTTP_BAD_REQUEST;
    
    if(bBad)
        return HTTP_BAD_REQUEST;

    if(bNotAccept)
        return HTTP_NOT_ACCEPTABLE;
	return RetCode;
}

/*!
 * \brief Processes the request and returns the result in the output parameters.
 *
 * \return
 * \li \c HTTP_BAD_REQUEST
 * \li \c UPNP_E_OUTOF_MEMORY
 * \li \c HTTP_REQUEST_RANGE_NOT_SATISFIABLE
 * \li \c HTTP_OK
 */
static int process_request(
	/*! [in] HTTP Request message. */
	http_message_t *req,
	/*! [out] Tpye of response. */
	enum resp_type *rtype,
	/*! [out] Headers. */
	membuffer *headers,
	/*! [out] Get filename from request document. */
	membuffer *filename,
	/*! [out] Xml alias document from the request document. */
	struct xml_alias_t *alias,
	/*! [out] Send Instruction object where the response is set up. */
	struct SendInstruction *RespInstr)
{
	int code;
	int err_code;

	char *request_doc;
    struct File_Info finfo={0};
	int using_alias;
	int using_virtual_dir;
	uri_type *url;
	const char *temp_str;
	int resp_major;
	int resp_minor;
	int alias_grabbed;
	size_t dummy;
	const char *extra_headers = NULL;
    char dlnaStream[1024];
    char *pszTransMode = NULL;
    struct xml_alias_t * aliastemp = NULL;

	print_http_headers(req);
	url = &req->uri;
	assert(req->method == HTTPMETHOD_GET ||
	       req->method == HTTPMETHOD_HEAD ||
	       req->method == HTTPMETHOD_POST ||
	       req->method == HTTPMETHOD_SIMPLEGET);
	/* init */
	request_doc = NULL;
	finfo.content_type = NULL;
	alias_grabbed = FALSE;
	err_code = HTTP_INTERNAL_SERVER_ERROR;	/* default error */
	using_virtual_dir = FALSE;
	using_alias = FALSE;

	http_CalcResponseVersion(req->major_version, req->minor_version,
				 &resp_major, &resp_minor);
	/* */
	/* remove dots */
	/* */
	request_doc = malloc(url->pathquery.size + 1);
	if (request_doc == NULL) {
		goto error_handler;	/* out of mem */
	}
	memcpy(request_doc, url->pathquery.buff, url->pathquery.size);
	request_doc[url->pathquery.size] = '\0';
	dummy = url->pathquery.size;
	remove_escaped_chars(request_doc, &dummy);
	code = remove_dots(request_doc, url->pathquery.size);
	if (code != 0) {
		err_code = HTTP_FORBIDDEN;
		goto error_handler;
	}
	if (*request_doc != '/') {
		/* no slash */
		err_code = HTTP_BAD_REQUEST;
		goto error_handler;
	}
	if (isFileInVirtualDir(request_doc)) {
		using_virtual_dir = TRUE;
		RespInstr->IsVirtualFile = 1;
		if (membuffer_assign_str(filename, request_doc) != 0) {
			goto error_handler;
		}
	} else {
		/* try using alias */
              aliastemp = find_alias_fromlist(request_doc);
              if (aliastemp ){
    		    if (is_valid_alias(aliastemp)) {
        			alias_grab(alias, aliastemp);
        			alias_grabbed = TRUE;
        			using_alias = get_alias(request_doc, alias, &finfo);
        			if (using_alias == TRUE) {
                                    finfo.content_type = ixmlCloneDOMString( "text/xml; charset=\"utf-8\"" );
        				if (finfo.content_type == NULL) {
        					goto error_handler;
        				}
        			}
        		}
             }
	}
	if (using_virtual_dir) {
		if (req->method != HTTPMETHOD_POST) {
			/* get file info */
			if (virtualDirCallback.
			    get_info(filename->buf, &finfo) != 0) {
				err_code = HTTP_NOT_FOUND;
				goto error_handler;
			}
			/* try index.html if req is a dir */
			if (finfo.is_directory) {
				if (filename->buf[filename->length - 1] == '/') {
					temp_str = "index.html";
				} else {
					temp_str = "/index.html";
				}
				if (membuffer_append_str(filename, temp_str) !=
				    0) {
					goto error_handler;
				}
				/* get info */
				if ((virtualDirCallback.
				     get_info(filename->buf,
					      &finfo) != UPNP_E_SUCCESS)
				    || finfo.is_directory) {
					err_code = HTTP_NOT_FOUND;
					goto error_handler;
				}
			}
			/* not readable */
			if (!finfo.is_readable) {
				err_code = HTTP_FORBIDDEN;
				goto error_handler;
			}
			/* finally, get content type */
			/* if ( get_content_type(filename->buf, &content_type) != 0 ) */
			/*{ */
			/*  goto error_handler; */
			/* } */
		}
	} else if (!using_alias) {
		if (gDocumentRootDir.length == 0) {
			goto error_handler;
		}
		/* */
		/* get file name */
		/* */

		/* filename str */
		if (membuffer_assign_str(filename, gDocumentRootDir.buf) != 0 ||
		    membuffer_append_str(filename, request_doc) != 0) {
			goto error_handler;	/* out of mem */
		}
		/* remove trailing slashes */
		while (filename->length > 0 &&
		       filename->buf[filename->length - 1] == '/') {
			membuffer_delete(filename, filename->length - 1, 1);
		}
		if (req->method != HTTPMETHOD_POST) {
			/* get info on file */
			if (get_file_info(filename->buf, &finfo) != 0) {
				err_code = HTTP_NOT_FOUND;
				goto error_handler;
			}
			/* try index.html if req is a dir */
			if (finfo.is_directory) {
				if (filename->buf[filename->length - 1] == '/') {
					temp_str = "index.html";
				} else {
					temp_str = "/index.html";
				}
				if (membuffer_append_str(filename, temp_str) !=
				    0) {
					goto error_handler;
				}
				/* get info */
				if (get_file_info(filename->buf, &finfo) != 0 ||
				    finfo.is_directory) {
					err_code = HTTP_NOT_FOUND;
					goto error_handler;
				}
			}
			/* not readable */
			if (!finfo.is_readable) {
				err_code = HTTP_FORBIDDEN;
				goto error_handler;
			}
		}
		/* finally, get content type */
		/*      if ( get_content_type(filename->buf, &content_type) != 0 ) */
		/*      { */
		/*          goto error_handler; */
		/*      } */
	}
	RespInstr->ReadSendSize = finfo.file_length;
	/* Check other header field. */
	if ((err_code =
	     CheckOtherHTTPHeaders(req, RespInstr,
                                 &finfo) ) != HTTP_OK ) {
		goto error_handler;
	}
    if(!strncmp("/web/mdb", req->urlbuf, 8))
    {
        switch(finfo.nTransMode)
        {
            case 0:
                pszTransMode = "Interactive";
                break;
            case 1:
                pszTransMode = "Streaming";
                break;
            case 2:
                pszTransMode = "Background";
                break;
            default:
                break;
        }
        snprintf(dlnaStream, sizeof(dlnaStream), 
            "ContentFeatures.DLNA.ORG: %s\r\nTransferMode.DLNA.ORG:%s\r\nCONNECTION: close\r\n\r\n",
            finfo.protocolinfo,pszTransMode);
    }
    else
    {
        snprintf(dlnaStream, sizeof(dlnaStream), "CONNECTION: close\r\n\r\n");
    }
	if (req->method == HTTPMETHOD_POST) {
		*rtype = RESP_POST;
		err_code = UPNP_E_SUCCESS;
		goto error_handler;
	}
	/*extra_headers = UpnpFileInfo_get_ExtraHeaders(finfo); */
	if (!extra_headers) {
		extra_headers = "";
	}

	/* Check if chunked encoding should be used. */
	if (using_virtual_dir && finfo.file_length == UPNP_USING_CHUNKED) {
		/* Chunked encoding is only supported by HTTP 1.1 clients */
		if (resp_major == 1 && resp_minor == 1) {
			RespInstr->IsChunkActive = 1;
		} else {
			/* The virtual callback indicates that we should use
			 * chunked encoding however the client doesn't support
			 * it. Return with an internal server error. */
			err_code = HTTP_NOT_ACCEPTABLE;
			goto error_handler;
		}
	}

     if( RespInstr->IsRangeActive && RespInstr->IsTimeSeek)
    {
        if (http_MakeMessage(
            headers, resp_major, resp_minor,
            "R" "N" "T" "GD" "s" "tcS" "s",
            HTTP_OK,     // status code
            RespInstr->ReadSendSize,  // content length
            finfo.content_type,       // content type
            RespInstr,                // range info
            "LAST-MODIFIED: ",
	        &finfo.last_modified,
	        dlnaStream) != 0 ) {
            goto error_handler;
        }        
    }
    else if( RespInstr->IsRangeActive && RespInstr->IsChunkActive ) {
		/* Content-Range: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT */
		/* Transfer-Encoding: chunked */
        if (http_MakeMessage(
            headers, resp_major, resp_minor,
            "R" "T" "GKLD" "s" "tcS" "s",
		    HTTP_PARTIAL_CONTENT,	/* status code */
		    finfo.content_type,	/* content type */
		    RespInstr,	/* range info */
		    RespInstr,	/* language info */
		    "LAST-MODIFIED: ",
		    &finfo.last_modified,
	        dlnaStream) != 0 ) {
			goto error_handler;
		}
	} else if (RespInstr->IsRangeActive && !RespInstr->IsChunkActive) {
		/* Content-Range: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT */
		/* Transfer-Encoding: chunked */
		if (http_MakeMessage(headers, resp_major, resp_minor,
		    "R" "N" "T" "GLD" "s" "tcS" "s",
		    HTTP_PARTIAL_CONTENT,	/* status code */
		    RespInstr->ReadSendSize,	/* content length */
		    finfo.content_type,	/* content type */
		    RespInstr,	/* range info */
		    RespInstr,	/* language info */
		    "LAST-MODIFIED: ",
		    &finfo.last_modified,
	        dlnaStream) != 0 ) {
			goto error_handler;
		}
	} else if (!RespInstr->IsRangeActive && RespInstr->IsChunkActive) {
		/* Content-Range: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT */
		/* Transfer-Encoding: chunked */
		if (http_MakeMessage(headers, resp_major, resp_minor,
		    "RK" "TLD" "s" "tcS" "s",
		    HTTP_OK,	/* status code */
		    finfo.content_type,	/* content type */
		    RespInstr,	/* language info */
		    "LAST-MODIFIED: ",
		    &finfo.last_modified,
	        dlnaStream) != 0 ) {
			goto error_handler;
		}
	} else {
		/* !RespInstr->IsRangeActive && !RespInstr->IsChunkActive */
		if (RespInstr->ReadSendSize >= 0) {
			/* Content-Range: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT */
			/* Transfer-Encoding: chunked */
			if (http_MakeMessage(headers, resp_major, resp_minor,
			    "R" "N" "TLD" "s" "tcS" "s",
			    HTTP_OK,	/* status code */
			    RespInstr->ReadSendSize,	/* content length */
			    finfo.content_type,	/* content type */
			    RespInstr,	/* language info */
			    "LAST-MODIFIED: ",
			    &finfo.last_modified,
		        dlnaStream) != 0 ) 
            {
				goto error_handler;
			}
		} else {
			/* Content-Range: bytes 222-3333/4000  HTTP_PARTIAL_CONTENT */
			/* Transfer-Encoding: chunked */
			if (http_MakeMessage(headers, resp_major, resp_minor,
			    "R" "TLD" "s" "tcS" "s",
			    HTTP_OK,	/* status code */
			    finfo.content_type,	/* content type */
			    RespInstr,	/* language info */
			    "LAST-MODIFIED: ",
		        &finfo.last_modified,
		        dlnaStream) != 0 ) {
				goto error_handler;
			}
		}
	}
	if (req->method == HTTPMETHOD_HEAD) {
		*rtype = RESP_HEADERS;
	} else if (using_alias) {
		/* GET xml */
		*rtype = RESP_XMLDOC;
	} else if (using_virtual_dir) {
		*rtype = RESP_WEBDOC;
	} else {
		/* GET filename */
		*rtype = RESP_FILEDOC;
	}
	/* simple get http 0.9 as specified in http 1.0 */
	/* don't send headers */
	if (req->method == HTTPMETHOD_SIMPLEGET) {
		membuffer_destroy(headers);
	}
	err_code = UPNP_E_SUCCESS;

 error_handler:
	free(request_doc);
	ixmlFreeDOMString(finfo.content_type);
    ixmlFreeDOMString( finfo.protocolinfo);
    ixmlFreeDOMString( finfo.fullpathname);
	if (err_code != UPNP_E_SUCCESS && alias_grabbed) {
		alias_release(alias);
	}

	return err_code;
}

/*!
 * \brief Receives the HTTP post message.
 *
 * \return
 * \li \c HTTP_INTERNAL_SERVER_ERROR
 * \li \c HTTP_UNAUTHORIZED
 * \li \c HTTP_REQUEST_RANGE_NOT_SATISFIABLE
 * \li \c HTTP_OK
 */
static int http_RecvPostMessage(
	/*! HTTP Parser object. */
	http_parser_t *parser,
	/*! [in] Socket Information object. */
	SOCKINFO *info,
	/*! File where received data is copied to. */
	char *filename,
	/*! Send Instruction object which gives information whether the file
	 * is a virtual file or not. */
	struct SendInstruction *Instr)
{
	size_t Data_Buf_Size;
	char* Buf;
	int Timeout = 0;
	FILE *Fp;
	parse_status_t status = PARSE_OK;
	int ok_on_close = FALSE;
	size_t entity_offset = 0;
	int num_read = 0;
	int ret_code = HTTP_OK;
	size_t lastSize;

	if (Instr && Instr->IsVirtualFile) {
		Fp = (virtualDirCallback.open) (filename, UPNP_WRITE);
		if (Fp == NULL)
			return HTTP_INTERNAL_SERVER_ERROR;
	} else {
		Fp = fopen(filename, "wb");
		if (Fp == NULL)
			return HTTP_UNAUTHORIZED;
	}
	parser->position = POS_ENTITY;
	Buf = malloc(WRITEFILE_DATA_BUFFER);
	if (NULL == Buf) {
		ret_code = HTTP_INTERNAL_SERVER_ERROR;
		goto ExitFunction;
	}
	do {
		Data_Buf_Size = WRITEFILE_DATA_BUFFER;
		/* first parse what has already been gotten */
		if (parser->position != POS_COMPLETE) {
			status = parser_parse_entity(parser);
		}
		if (status == PARSE_INCOMPLETE_ENTITY) {
			/* read until close */
			ok_on_close = TRUE;
		} else if ((status != PARSE_SUCCESS)
				&& (status != PARSE_CONTINUE_1)
				&& (status != PARSE_INCOMPLETE)) {
			/* error */
			ret_code = HTTP_BAD_REQUEST;
			goto ExitFunction;
		}
		/* read more if necessary entity */
		while (entity_offset + Data_Buf_Size > parser->msg.entity.length &&
				parser->position != POS_COMPLETE) {
			num_read = sock_read(info, Buf, Data_Buf_Size, &Timeout);
			if (num_read > 0) {
				/* append data to buffer */
				ret_code = membuffer_append(&parser->msg.msg,
						Buf, (size_t)num_read);
				if (ret_code != 0) {
					/* set failure status */
					parser->http_error_code =
						HTTP_INTERNAL_SERVER_ERROR;
					ret_code = HTTP_INTERNAL_SERVER_ERROR;
					goto ExitFunction;
				}
				status = parser_parse_entity(parser);
				if (status == PARSE_INCOMPLETE_ENTITY) {
					/* read until close */
					ok_on_close = TRUE;
				} else if ((status != PARSE_SUCCESS)
						&& (status != PARSE_CONTINUE_1)
						&& (status != PARSE_INCOMPLETE)) {
					ret_code = HTTP_BAD_REQUEST;
					goto ExitFunction;
				}
			}
			else if (num_read == 0) {
				if (ok_on_close) {
					UpnpPrintf(UPNP_INFO, HTTP, __FILE__, __LINE__,
							"<<< (RECVD) <<<\n%s\n-----------------\n",
							parser->msg.msg.buf);
					print_http_headers(&parser->msg);
					parser->position = POS_COMPLETE;
				}
				else {
					/* partial msg or response */
					parser->http_error_code = HTTP_BAD_REQUEST;
					ret_code = HTTP_BAD_REQUEST;
					goto ExitFunction;
				}
			}
			else {
				ret_code = num_read;
				goto ExitFunction;
			}
		}
		while (parser->msg.entity.length >= Data_Buf_Size) {
			memcpy(Buf,
					&parser->msg.msg.buf[parser->entity_start_position + entity_offset],
					Data_Buf_Size);
			if (Instr->IsVirtualFile) {
				int n = virtualDirCallback.write(Fp, Buf, Data_Buf_Size);
				if (n < 0) {
					ret_code = HTTP_INTERNAL_SERVER_ERROR;
					goto ExitFunction;
				}
			}
			else {
				size_t n = fwrite(Buf, 1, Data_Buf_Size, Fp);
				if (n != Data_Buf_Size) {
					ret_code = HTTP_INTERNAL_SERVER_ERROR;
					goto ExitFunction;
				}
			}

			//�ͷ��ڴ�//by 161677
			membuffer_delete(&parser->msg.msg, parser->entity_start_position, Data_Buf_Size);
			//����chunkģʽ��chunksize�ĸ��¼�scanner�ĸ�����Ҫ�����
			//��Ϊд���˽��յ����ݵ��ļ���Ϊ�˲�Ӱ�쵱ǰɨ��λ�ü�����chunk���ȵ��жϣ�Ҫ��Ӧ����
			if ( parser->scanner.cursor != parser->entity_start_position ) {
				//��ǰchunk������֮ǰ��������û�б��棬��ʱ����chunksize��2�����
				lastSize = parser->scanner.cursor - parser->entity_start_position;
				if (Data_Buf_Size > lastSize) {
					//д���˲��ֵ�ǰchunk�����ݵ��ļ������chunksizeҪ����д��Ĳ���
					parser->scanner.cursor = parser->entity_start_position;
					parser->chunk_size -= Data_Buf_Size - lastSize;
				}
				else {
					//д���ļ������ݲ��ǵ�ǰchunk�ģ���ǰ�������ģ�����Ҫ����chunksize��ֻ��Ҫ����ɨ��ָ��
					parser->scanner.cursor -= Data_Buf_Size;
				}
			}
			else {
				//д���ȫ���ǵ�ǰchunk�����ݡ�
				parser->chunk_size -= Data_Buf_Size;
			}
			parser->content_length -= Data_Buf_Size;
			parser->msg.entity.length -= Data_Buf_Size;
		}
	} while ((parser->position != POS_COMPLETE));
	if ( parser->msg.entity.length > 0 ) {
		Data_Buf_Size = parser->msg.entity.length;
		memcpy(Buf,
				&parser->msg.msg.buf[parser->entity_start_position + entity_offset],
				Data_Buf_Size);
		if (Instr && Instr->IsVirtualFile) {
			int n = virtualDirCallback.write(Fp, Buf, Data_Buf_Size);
			if (n < 0) {
				free( Buf );
				virtualDirCallback.close(Fp);
				return HTTP_INTERNAL_SERVER_ERROR;
			}
		} else {
			size_t n = fwrite(Buf, 1, Data_Buf_Size, Fp);
			if (n != Data_Buf_Size) {
				ret_code = HTTP_INTERNAL_SERVER_ERROR;
				goto ExitFunction;
			}
		}
		membuffer_delete(&parser->msg.msg, parser->entity_start_position, Data_Buf_Size);
	}

ExitFunction:
	if (Instr && Instr->IsVirtualFile) {
		virtualDirCallback.close(Fp);
	} else {
		fclose(Fp);
	}

	free(Buf);
	return ret_code;
}

void web_server_callback(http_parser_t *parser, INOUT http_message_t *req,
	SOCKINFO *info)
{
	int ret;
	int timeout = 0;
	enum resp_type rtype = 0;
	membuffer headers;
	membuffer filename;
	struct xml_alias_t xmldoc;
	struct SendInstruction RespInstr;
#ifdef SUPPORT_ATP_DMS_UPLOAD
	char *pStr;
	long long llObjectId;
#endif

	/*Initialize instruction header. */
	RespInstr.IsVirtualFile = 0;
	RespInstr.IsChunkActive = 0;
	RespInstr.IsRangeActive = 0;
	RespInstr.IsTrailers = 0;
	memset(RespInstr.AcceptLanguageHeader, 0,
	       sizeof(RespInstr.AcceptLanguageHeader));
	/* init */
	membuffer_init(&headers);
	membuffer_init(&filename);

	/*Process request should create the different kind of header depending on the */
	/*the type of request. */
	ret = process_request(req, &rtype, &headers, &filename, &xmldoc,
		&RespInstr);
	if (ret != UPNP_E_SUCCESS) {
		/* send error code */
		http_SendStatusResponse(info, ret, req->major_version,
			req->minor_version);
	} else {
		/* send response */
		switch (rtype) {
		case RESP_FILEDOC:
			http_SendMessage(info, &timeout, "Ibf",
					 &RespInstr,
					 headers.buf, headers.length,
					 filename.buf);
			break;
		case RESP_XMLDOC:
			http_SendMessage(info, &timeout, "Ibb",
				&RespInstr,
				headers.buf, headers.length,
				xmldoc.doc.buf, xmldoc.doc.length);
			alias_release(&xmldoc);
			break;
		case RESP_WEBDOC:
			http_SendMessage(info, &timeout, "Ibf",
				&RespInstr,
				headers.buf, headers.length,
				filename.buf);
			break;
		case RESP_HEADERS:
			/* headers only */
			http_SendMessage(info, &timeout, "b",
				headers.buf, headers.length);
			break;
		case RESP_POST:
#ifdef SUPPORT_ATP_DMS_UPLOAD
				//Send HTTP_CONTINUE
				if (0 != http_MakeMessage(
					&headers, 1, 1,
					"RNc",
					HTTP_CONTINUE,
					RespInstr.ReadSendSize)) {
					// Make message error.
					UpnpPrintf(UPNP_CRITICAL, HTTP, __FILE__, __LINE__,
						"<<< (Make Message error.)\n");
				}
				http_SendMessage( info, &timeout, "b", headers.buf,
					headers.length );
				membuffer_destroy( &headers );

				//Disable auto destroy object
				pStr = strrchr(filename.buf, '/');
				if (NULL != pStr)
				{
					pStr++;
					llObjectId = strtoll(pStr, NULL, 10);
				}
				else
				{
					llObjectId = strtoll(filename.buf, NULL, 10);
				}
				ATP_MDB_DisableUploadNote(llObjectId);

				//Receive message
				ret =
                    http_RecvPostMessage( parser, info, filename.buf,
                                          &RespInstr );
				
				//Send response.
				membuffer_init( &headers );
				if ((ret < 0) || (HTTP_INTERNAL_SERVER_ERROR == ret) || (HTTP_BAD_REQUEST == ret))		//�ϴ�ʧ��
				{
					ATP_MDB_DestroyObject(llObjectId);
					if (0 != http_MakeMessage(
	                    &headers, 1, 1,
	                    "RTDSXcCc",
	                    HTTP_REQUEST_TIMEOUT,
	                    "text/html",
	                    X_USER_AGENT)) {
						UpnpPrintf(UPNP_CRITICAL, HTTP, __FILE__, __LINE__,
							"<<< (Make Message error.)\n");
					}
				}
				else if(ret >= 0)	//�ϴ��ɹ�
				{
					//�ȴ�scannerɨ�����(���10��)
					timeout = 50;
					while (timeout >= 0)
					{
						if (VOS_TRUE == ATP_MDB_CheckUploadComplete(llObjectId))
						{
							break;
						}
						timeout--;
						usleep(200000);
					}
					timeout = 0;
					ATP_MDB_DelUploadNote(llObjectId);
					if (0 != http_MakeMessage(
						&headers, 1, 1,
						"RTDSXcCc",
						ret,
						"text/html",
						X_USER_AGENT)) {
						UpnpPrintf(UPNP_CRITICAL, HTTP, __FILE__, __LINE__,
							"<<< (Make Message error.)\n");	
					}
				}
#else
				ret =
                    http_RecvPostMessage( parser, info, filename.buf,
                                          &RespInstr );
                //Send response.

                if (0 != http_MakeMessage(
                    &headers, 1, 1,
                    "RTDSXcCc",
                    ret,
                    "text/html",
                    X_USER_AGENT)) {
					UpnpPrintf(UPNP_CRITICAL, HTTP, __FILE__, __LINE__,
						"<<< (Make Message error.)\n");	
				}
#endif
                http_SendMessage( info, &timeout, "b", headers.buf,
                                  headers.length );
                break;
		default:
			UpnpPrintf(UPNP_INFO, HTTP, __FILE__, __LINE__,
				"webserver: Invalid response type received.\n");
			assert(0);
		}
	}
	UpnpPrintf(UPNP_INFO, HTTP, __FILE__, __LINE__,
		   "webserver: request processed...\n");
	membuffer_destroy(&headers);
	membuffer_destroy(&filename);
}

