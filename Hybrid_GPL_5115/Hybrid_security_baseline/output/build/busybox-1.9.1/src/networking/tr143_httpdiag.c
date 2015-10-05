#include "tr143_httpdiag.h"

int GetDefaultRouteInterfaceName(char *pcIfcName) 
{ 
    int bRet = 0;
    char acLine[1024];
    int  lCount = 0;
    char acCol[11][32];
    FILE* fsRoute = NULL;

    if ( pcIfcName == NULL ) 
    {
        return  -1;
    }

    memset(acCol, 0, sizeof(acCol));
    memset(acLine, 0, sizeof(acLine));

    pcIfcName[0] = '\0';
  
    if ( (fsRoute = fopen("/proc/net/route", "r")) == NULL ) 
    {
        printf("Can't Open File:/proc/net/route\n");
        return  -1;
    }
    while ( fgets(acLine, sizeof(acLine), fsRoute) ) 
    {
        if ( lCount == 0 ) 
        {
            lCount = 1;
            continue;
        }
        sscanf(acLine, "%s %s %s %s %s %s %s %s %s %s %s",
            acCol[0], acCol[1], 
            acCol[2], acCol[3], 
            acCol[4],  acCol[5],
            acCol[6], acCol[7], 
            acCol[8],  acCol[9], 
            acCol[10]);
        if ( strcmp(acCol[1], "00000000") == 0)
        {
            if(strcmp(acCol[7], "00000000") == 0) 
            {
                strcpy(pcIfcName, acCol[0]);
                break;
            }
        }
    }
    fclose(fsRoute);

    return bRet;
}

int read_routeshowcmd( char * s)
{
	char line[LINE_SIZE]={0};
	FILE *fp;
	int size;
	char inter[32];
	char *p;
	char shcmd[50]={0};
	char ifcmd[50]={0};
	int ret, ret_if ;

	memset(inter, 0, 32);
	GetDefaultRouteInterfaceName(inter);
	if (inter[0]=='\0')
	{
		printf("get default wan failed.\n");
		return -1;
	}
	sprintf(ifcmd,"ifconfig %s > /var/ifcfgcmd", inter);
	system(ifcmd);
	
	 ret_if = access("/var/ifcfgcmd", F_OK);
	if ( -1 == ret_if )
	{
		printf(" sorry, the ifcfgcmd is not existed.\n");
		return -1;
	} else {
		fp= fopen("/var/ifcfgcmd", "r" );	
		if (fp ==NULL)
		{
			printf("(/var/ifcfgcmd) is not exsited.\n");
			return -1;
		}
	}
	memset(line, 0, LINE_SIZE);
	while((fgets(line, LINE_SIZE, fp)) !=NULL)
	{
		p = strstr(line, s);
		if (p){
			p += strlen(s);
			size = atoi(p);
		}
	}	
	fclose(fp);
	
	return size;
}


int read_ifcfgcmd(char *inter, char *s)
{
	char line[LINE_SIZE]={0};
	//char *s ="RX packets:";
	//char *s2 ="TX packets:";
	FILE *fp;
	int size = 0;
	int ret = 0;
	char *p;
	char ifcmd[50]={0};
	
	
	sprintf(ifcmd,"ifconfig %s > /var/ifcfgcmd", inter);
	system(ifcmd);
	
	ret = access("/var/ifcfgcmd", F_OK);
	if ( -1 == ret )
	{
		printf(" sorry, the ifcfgcmd is not existed.\n");
		return -1;
	}
	else 
	{
		fp= fopen("/var/ifcfgcmd", "r" );	
	}
	if(NULL != fp)
	{
	    while((fgets(line, LINE_SIZE, fp)) !=NULL)
           {
       		p = strstr(line, s);
       		if (p)
       		{
       			p += strlen(s);
       			size = atoi(p);
       		}
            }	
            fclose(fp);        	
            return size;
	}
}

//使用的是UTC 时间
int write_utc_downTime( char *s )
{
	struct timeval tv;
	char tv_buf[50];
        FILE *fp = NULL;
  	char acTmeTmp[100];   
    	struct tm *pstTmeType;
    	
        gettimeofday(&tv, NULL);
        
	fp = fopen("/var/http_down.txt", "a+" );	// r+
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
    
    	pstTmeType = gmtime(&tv.tv_sec);
    	strftime(acTmeTmp , 100 , "%Y-%m-%dT%H:%M:%S" ,pstTmeType);
	fprintf(fp,"%s:",s);
	fprintf(fp,"%s",acTmeTmp);
	fprintf(fp,".%d\n",tv.tv_usec);
 	fclose(fp);
	return 0;
}




#if 0
int write_tr143cfg(char *s)
{	
	char line[LINE_SIZE];

	system("mv -f /var/http.txt /var/http_copy.txt");
	FILE *fp1 = fopen("/var/http_copy.txt", "r");
	FILE *fp2 = fopen("/var/http.txt", "w");

	if(( fp1 == NULL) || ( fp2 == NULL))
	{	
		printf("fopen failed.\n");
		exit(0);
	}
	while((fgets(line, LINE_SIZE, fp1)) !=NULL)
	{
		if ( strncmp(line, s, strlen(s)) == 0)
		{
		//printf("strncmp:%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
			fprintf(fp2, "%s", s);
			//fprintf(fp2, ":%s\n", time);
			write_utc_downTime(fp2);
		}
		else{
			fprintf(fp2, "%s", line);
			printf("fprintf fp2:%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
		}
	}
	fclose(fp1);
	fclose(fp2);
	//system("rm -rf /var/http_copy.txt");
	printf("system: %s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
	return 0;
}
#endif


int Http_downROMTime( void )
{
	write_utc_downTime( "Http_downROMTime") ;
	return 0;
}

int Http_downBOMTime( void )
{
	write_utc_downTime( "Http_downBOMTime") ;
	return 0;
}

int Http_downEOMTime( void )
{
	write_utc_downTime( "Http_downEOMTime") ;
	return 0;
}

int Http_downRequestTime( void )
{
	write_utc_downTime("Http_downRequestTime");
	return 0;
}

int Http_downResponseTime( void )
{
	write_utc_downTime("Http_downResponseTime");
	return 0;
}


int Http_downTotalReceived( int totalReceived)
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return  -1;
	}
	fprintf(fp,"%s:","Http_downTotalReceived");
	fprintf(fp,"%d\n",totalReceived);
	fclose(fp);
	return 0;
}

int Http_downTestReceived( int testReceived)
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp,"%s:","Http_downTestReceived");
	fprintf(fp, "%d\n", testReceived);

	fclose(fp);
	return 0;
}

int Http_wgetDownEnd()
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	// r+
	if ( fp ==NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
	fprintf(fp,"Http_wgetDownEnd:%s\n:","Completed");
	fclose(fp);
	return 0;
}

int write_utc_upTime( char *s )
{
	struct timeval tv;
	char tv_buf[50];
        FILE *fp = NULL;
  	char acTmeTmp[100];   
    	struct tm *pstTmeType;

    	gettimeofday(&tv, NULL);

	fp = fopen("/var/http_up.txt", "a+" );	// r+
	if ( fp==NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
    
    	pstTmeType = gmtime(&tv.tv_sec);
    	strftime(acTmeTmp , 100 , "%Y-%m-%dT%H:%M:%S" ,pstTmeType);
	fprintf(fp,"%s:",s);
	fprintf(fp,"%s",acTmeTmp);
	fprintf(fp,".%d\n",tv.tv_usec);
 	fclose(fp);
	return 0;
}


int Http_upROMTime( void )
{
	write_utc_upTime( "Http_upROMTime") ;
	return 0;
}

int Http_upBOMTime( void )
{
	write_utc_upTime( "Http_upBOMTime") ;
	return 0;
}

int Http_upEOMTime( void )
{
	write_utc_upTime( "Http_upEOMTime") ;
	return 0;
}

int Http_upRequestTime( void )
{
	write_utc_upTime("Http_upRequestTime");
	return 0;
}

int Http_upResponseTime( void )
{
	write_utc_upTime("Http_upResponseTime");
	return 0;
}

int Http_upTestLength(int testLength)
{
	FILE *fp = fopen("/var/http_up.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp, "Http_upTestLength:%d\n", testLength);

	fclose(fp);
	return 0;
}

int Http_upTotalSent(int totalReceived)
{
	FILE *fp = fopen("/var/http_up.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp, "%s:", "Http_upTotalSent");
	fprintf(fp, "%d\n", totalReceived);
	fclose(fp);
	
	return 0;
}

int Http_upErrorMsg(char *s)
{
	FILE *fp = fopen("/var/http_up.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
	fprintf(fp,"Http_upErrorMsg:%s\n:",s);
	fclose(fp);
	return 0;
}

int Http_downErrorMsg(char *s)
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
	fprintf(fp,"Http_downErrorMsg:%s\n:",s);
	fclose(fp);
	return 0;
}


#if 0
int write_config(const char * str1)
{
      int m=0;
	int i =0;
	char s1[200];
	char *s2;
	//需要增加的配置文件内容
	//char *str1 = "<shellowerg>:2009-11-12 21:20:52.155685";	
	char str2[30];
	char *str3 = str1;
	char *line =NULL;
	ssize_t len =0;
	ssize_t read;
	int flag =0;

	while( *str1!= ':' && *str1 !='\0' )
	{
		m ++;
		str1 ++;
	}
	printf("input: m = %d\n",  m);
	strncpy( str2, str3, m );
	printf("str2:%s\n", str2);
	
	system( " mv t.c t_copy.c");
	FILE * fp1 = fopen( "t_copy.c", "r");
	FILE * fp2 = fopen( "t.c", "a+");

		while((( read = getline( &line, &len, fp1) ) >0 ) && (!feof(fp1)) &&(flag ==0))
		{
			printf("%d\n", i++);
			if ((( strncasecmp(line, str3,m)) ==0) && (flag ==0))
			{
				flag =1;
				fprintf(fp2, "%s\n", str3);		
			}
			else 
			{
				flag =0;
				fprintf(fp2, "%s\n", line);
			}
		}
		
	if (line)
			free(line);
	//free(s1);
	fclose( fp1 );
	fclose( fp2 );
	printf("main is ended.\n");
	//printf("str3: %s\n", str3);

	return 0;
}
#endif

/********************* ftp **********************/

int Ftp_recvROMTime( void )
{
	write_utc_downTime( "Ftp_recvROMTime") ;
	return 0;
}

int Ftp_recvBOMTime( void )
{
	write_utc_downTime( "Ftp_recvBOMTime") ;
	return 0;
}

int Ftp_recvEOMTime( void )
{
	write_utc_downTime( "Ftp_recvEOMTime") ;
	return 0;
}

int Ftp_recvRequestTime( void )
{
	write_utc_downTime("Ftp_recvRequestTime");
	return 0;
}

int Ftp_recvResponseTime( void )
{
	write_utc_downTime("Ftp_recvResponseTime");
	return 0;
}


int Ftp_recvTotalReceived( int totalReceived)
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp,"%s:","Ftp_recvTotalReceived");
	fprintf(fp,"%d\n",totalReceived);
	fclose(fp);
	return 0;
}

int Ftp_recvTestReceived( int testReceived)
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp,"%s:","Ftp_recvTestReceived");
	fprintf(fp, "%d\n", testReceived);
	fclose(fp);
	return 0;
}

int Ftp_recvEnd()
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp,"%s:","Ftp:Completed");
	fclose(fp);
	
	return 0;
}


int Ftp_sendROMTime( void )
{
	write_utc_upTime( "Ftp_sendROMTime") ;
	return 0;
}

int Ftp_sendBOMTime( void )
{
	write_utc_upTime( "Ftp_sendBOMTime") ;
	return 0;
}

int Ftp_sendEOMTime( void )
{
	write_utc_upTime( "Ftp_sendEOMTime") ;
	return 0;
}

int Ftp_sendRequestTime( void )
{
	write_utc_upTime("Ftp_sendRequestTime");
	return 0;
}

int Ftp_sendResponseTime( void )
{
	write_utc_upTime("Ftp_sendResponseTime");
	return 0;
}


int Ftp_sendTotalReceived( int totalReceived)
{
	FILE *fp = fopen("/var/http_up.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return;
	}
	fprintf(fp,"%s:","Ftp_sendTotalReceived");
	fprintf(fp,"%d\n",totalReceived);
	fclose(fp);
	return 0;
}

int Ftp_sendTestReceived( int testReceived)
{
	FILE *fp = fopen("/var/http_up.txt", "a+" );
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
	fprintf(fp,"%s:","Ftp_sendTestReceived");
	fprintf(fp, "%d\n", testReceived);

	fclose(fp);
	return 0;
}

int Ftp_sendErroMsg(char *s)
{
	FILE *fp = fopen("/var/http_up.txt", "a+" );	
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
	fprintf(fp,"Ftp_sendErroMsg:%s\n:",s);
	fclose(fp);
	return 0;
}

int Ftp_recvErroMsg(char *s)
{
	FILE *fp = fopen("/var/http_down.txt", "a+" );
	if ( fp == NULL )
	{
		printf("open_file failed.\n");
		return -1;
	}
	fprintf(fp,"Ftp_recvErroMsg:%s\n:",s);
	fclose(fp);
	return 0;
}

