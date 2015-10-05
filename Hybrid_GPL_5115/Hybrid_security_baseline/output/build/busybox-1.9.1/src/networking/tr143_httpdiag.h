#ifndef _TR143_WGET_H
#define  _TR143_WGET_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LINE_SIZE 512

#ifndef EXIT_FAILURE		/* define exit() codes if not provided */
#define EXIT_FAILURE  1
#endif

int write_utc_downTime(char *s );
int write_utc_upTime(char *s );
int write_tr143cfg(char *s);

int Http_downROMTime( void );
int Http_downBOMTime( void );
int Http_downEOMTime( void );
int Http_downRequestTime( void );
int Http_downResponseTime( void );
int Http_downTotalReceived( int totalReceived);
int Http_downTestReceived( int testReceived);
int Http_upROMTime( void );
int Http_upBOMTime( void );
int Http_upEOMTime( void );
int Http_upRequestTime( void );
int Http_upResponseTime( void );
int Http_upTestReceived(int testReceived);
int Http_upTotalReceived(int totalReceived);
int Http_upTestLength(int testLength);
int Http_upErrorMsg(char *s);
int Http_downErrorMsg(char *s);
int read_ifcfgcmd(char *inter, char *s);
int read_routeshowcmd( char * s);

#endif
