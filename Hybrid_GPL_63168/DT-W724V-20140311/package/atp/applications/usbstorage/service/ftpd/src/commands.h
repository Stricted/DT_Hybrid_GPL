#ifndef __BFTPD_COMMANDS_H
#define __BFTPD_COMMANDS_H

#include <config.h>
#include <stdio.h>
#ifdef HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#ifdef HAVE_NETINET_IN_H
  #include <netinet/in.h>
#endif

#include "commands_admin.h"

/*
 * min()/max()/clamp() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#define min(x, y) ({				\
	typeof(x) _min1 = (x);			\
	typeof(y) _min2 = (y);			\
	(void) (&_min1 == &_min2);		\
	_min1 < _min2 ? _min1 : _min2; })

#define max(x, y) ({				\
	typeof(x) _max1 = (x);			\
	typeof(y) _max2 = (y);			\
	(void) (&_max1 == &_max2);		\
	_max1 > _max2 ? _max1 : _max2; })

enum {
    STATE_CONNECTED, STATE_USER, STATE_AUTHENTICATED, STATE_RENAME, STATE_ADMIN
};

enum {
    TYPE_ASCII, TYPE_BINARY
};

#define USERLEN 256 //changed by qichenghong 2009.12.24
#define PASSWDLEN 64 //added by qichenghong 2009.12.24
#define USERLEN_S "30"
#define MAXCMD 512

#define BFTPD_FILE_CREATEMODE 00660
#define BFTPD_DIR_CREATEMODE 00770

extern int state;
extern char user[USERLEN + 1];
extern struct sockaddr_in sa;
extern char pasv;
extern int sock;
extern int transferring;
extern int pasvsock;
extern char *philename;
extern unsigned long offset;
extern int ratio_send, ratio_recv;
extern double bytes_stored, bytes_recvd;
extern int xfer_bufsize;

void control_printf(char success, char *format, ...);

void init_userinfo();
void new_umask();
int parsecmd(char *);
int setctlsocket(int );
int dataconn();
void command_user(char *);
void command_pass(char *);
void command_pwd(char *);
void command_type(char *);
void command_port(char *);
void command_stor(char *);
void command_mget(char *);
void command_mput(char *);
void command_retr(char *);
void command_list(char *);
void command_syst(char *);
void command_cwd(char *);
void command_cdup(char *);
void command_dele(char *);
void command_mkd(char *);
void command_rmd(char *);
void command_noop(char *);
void command_rnfr(char *);
void command_rnto(char *);
void command_rest(char *);
void command_abor(char *);
void command_quit(char *);
void command_help(char *);
void command_stat(char *);
void command_feat(char *);
void command_opts(char *);

struct command {
  char *name;
  char *syntax;
  void (*function)(char *);
  char state_needed;
  char showinfeat;
};

/* File the size of the transfer buffer, divided by number of connetions */
int get_buffer_size(int num_connections);

/* Function which forks and runs a script. Returns TRUE on success
and FALSE on failure. */
int run_script(char *script, char *path);

#endif

