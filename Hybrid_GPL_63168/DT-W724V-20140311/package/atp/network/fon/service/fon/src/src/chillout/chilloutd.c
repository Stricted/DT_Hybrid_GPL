/* -*- mode: c; c-basic-offset: 8 -*- */
/*
 * Chilloutd. Xl2tpd and chillispot control daemon
 *
 * Copyright (C) 2007 FON Wireless Limited. All rights reserved
 *
 * Created: Oct 2007. Pablo Mart铆n <pablo@fon.com>
 *
 * $Rev: 85 $
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <libdaemon/daemon.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "poption.h"
#include "chillout.h"
#include <syslog.h>
#include "log.h"
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define VERSION "0.1"
#define DEFAULT_SOCKET_PATH                     "/tmp/chillout"
/*Begin:added by luokunling l00192527 ,fon用户登录成功之后.只清楚当前用户连接跟踪,不清楚所有fon用户*/
#define FON_USER_CURRENTLOGIN_LANIP     "echo %d > /proc/foncurrent_loginip 2>/dev/null"
/*End:added by luokunling l00192527,2012/2/1*/

#define MAX_USERNAME 256
#define MAX_PASSWORD 256
#define MAX_DEVICE 32		/* ppp0, ppp1, etc */
#define MAX_CID 32
#define MAX_PID 32

#define LEASE_PEND 0		/* lease pending */
#define LEASE_PPPD 1		/* before calling pppd */
#define LEASE_REGISTERED 2 	/* after pppd registers */
#define LEASE_TUNN 3		/* lease tunneled */
#define LEASE_AUTH 4		/* lease authenticated (after tunneled) */
#define LEASE_DETUNN 5		/* after notifing xl2tpd, before pppd exits */
#define LEASE_NONETWORK 6	/* pppd initiated down */
#define LEASE_DENY 7

#define TUNNEL_DOWN 0
#define TUNNEL_UP 1
#define TUNNEL_PENDING 2

#define CLIENT_UNSET 	0
#define CLIENT_CHILLI 	1
#define CLIENT_CONTROL 	2
#define CLIENT_XL2TPD 	3
#define CLIENT_PPPD 	4

/* Return values for session_create */
#define SESSION_OK 	0
#define SESSION_ERROR	1
#define SESSION_XL2TPD  2

struct client {
	chillout_t chillout;
	struct lease *lease;	/* associated lease, if it is a pppd client */
	int type;
	struct client *next;
	char buffer[256];	/* associated buffer */
};

struct lease {
	char mac[18];
	char ip[16];
	char wan_ip[16];
	time_t date;		/* date of lease/renew/update */
	time_t login_date;	/* login date, set before pppd launch */
	struct lease *next;
	char username[MAX_USERNAME + 1];
	char password[MAX_PASSWORD + 1];	/* OTP used in login */
	char device[MAX_DEVICE + 1]; /* ppp device used by this connection */
	char cid[MAX_CID + 1];	     /* call id on xl2tpd */
	char pppd_pid[MAX_PID + 1];  /* pid of associated pppd */
	int status;
	chillout_t redir; /* associated redir connection, in LEASE_PPPD */
	int release;	/* 1 only when del_lease was called but was delayed
			   because it was tunneled */
};

struct chillispot {
	chillout_t chillout;
	struct lease *leases;
	int num_leases;
	int num_auth_leases;
};

struct tunnel {
	int status;
	char user[MAX_USERNAME + 1];
	char password[MAX_PASSWORD + 1];
	struct tunnel *next;
};

struct xl2tpd {
	chillout_t chillout;
};

/* chillout_t */
struct chilloutd {
	/* chillout handler for the control socket */
	/* chillout_t chilloutd; */
	/* big file descriptor set */
	fd_set fds;
	int quit;
	int signalfd;
	int serverfd;
	struct client *clients;
	struct chillispot chillispot;
	struct xl2tpd xl2tpd;
	time_t start_time;
	time_t config_time;
	struct tunnel tunnel;
};

typedef struct chilloutd *chilloutd_t;

/* global_options */
static struct options {
	int nodaemon;
	int killdaemon;
	int reloaddaemon;
	int check;
	int version;
	int client;
	int keepAlive;
	char *echo;
	char *chillispot;
	char *xl2tpd;
	char *chillout;
	char *config_path;
	char *socket_path;
	char *rulescript;
} global_options;

/* prototipes */
static int create_socket(char *socket_path);
static int loop(chilloutd_t chilloutd);
static int daemonize(chilloutd_t chilloutd);
static int init(chilloutd_t chilloutd);
static int finish(chilloutd_t chilloutd);
static int parse_command_line(int *argc, char ***argv);
static int add_client(chilloutd_t chilloutd, int fd);
static int del_client(chilloutd_t, struct client *, int, int);
static int client(void);
static int keepAlive(void);
static int sendingEcho(char* client);
static ssize_t socket_readline(int sockd, void *vptr, size_t maxlen);
static ssize_t socket_writeline(int sockd, const void *vptr, size_t n);
static struct client *isset_client(chilloutd_t chilloutd, fd_set * fds);
static int read_and_process(struct client *client);
static struct client *retrieve_client_from_fd(chilloutd_t, int, int);
static struct client *retrieve_client_from_name(chilloutd_t, char *);
static struct client *retrieve_client_from_chillout(chilloutd_t, chillout_t);
static int parse_line_into_vector(char **vec, char *line, int *n, int max);
static int date_string_from_time_t(char *buffer, time_t date);
static int merge_chillouts(chillout_t co_read, chillout_t co_write);

static int login(struct chilloutd *ch, struct lease *l, char *user, char *pass);
static int logout(struct chilloutd *chilloutd, struct lease *lease);

#define PROTOCALLBACK(name) static int callback_##name(chillout_t, void *, \
int, char **)

PROTOCALLBACK(echo);
PROTOCALLBACK(default);
PROTOCALLBACK(control_reload);
PROTOCALLBACK(control_show);
PROTOCALLBACK(control_sleep);
PROTOCALLBACK(control_chilli);
PROTOCALLBACK(control_xl2tpd);
PROTOCALLBACK(xl2tpd_tunnel);
PROTOCALLBACK(xl2tpd_session);
PROTOCALLBACK(all_show);
PROTOCALLBACK(control_send);
PROTOCALLBACK(chilli_dhcp);
PROTOCALLBACK(unset_hello);
PROTOCALLBACK(redir_login);
PROTOCALLBACK(chilli_logout);
PROTOCALLBACK(ppp_register);
PROTOCALLBACK(ppp_network);
PROTOCALLBACK(ppp_failure);

static int add_lease(struct chillispot *ch, char *mac, char *ip);
static int del_lease(chilloutd_t chilloutd, struct chillispot *ch, char *mac);
static int reset_lease(chilloutd_t chilloutd, struct lease *l);
static int tunnel_lease(chilloutd_t c, struct lease *l, char *user, char *pass);
static int detunnel_lease(chilloutd_t c, struct lease *l);
static int free_lease_list(struct chillispot *ch);
static char *get_ip_from_mac(struct chillispot *ch, char *mac);
static char *get_mac_from_ip(struct chillispot *ch, char *ip);
static int session_create(chillout_t c, char *lac, char *user, char *password,
			  char *cid, char *mac);
#define FROM_MAC 1
#define FROM_IP 2
#define FROM_DEVICE 3
#define FROM_USERNAME 4
#define FROM_CID 5
static struct lease *get_lease_string(int what, struct chillispot *ch, char *str);
static int rule(char *local_address, char *device, char *wan_address);
static int derule(char *local_address, char *device, char *wan_address);
static int retrieve_table_from_device(char *device);
static int retrieve_wan_address(char *device, char *wan_address);
static int session_delete(chillout_t c, char *lac, char *cid);

static const char rcsid[] = "$Id: chilloutd.c 85 2008-01-23 16:17:38Z jesus.pico $";

/*
struct callback {
	char ident[MAX_NAME];
	char keyword[MAX_KEYWORD];
	int hash_keyword;
	chillout_callback *function;
};
*/

static struct callback global_callbacks[] = {
	{"control", "echo", 0, callback_echo},
	{"control", "reload", 0, callback_control_reload},
	{"control", "show", 0, callback_control_show},
	{"control", "sleep", 0, callback_control_sleep},
	{"control", "default", 0, callback_default},
	{"control", "send", 0, callback_control_send},
	{"control", "chilli", 0, callback_control_chilli},
	{"control", "xl2tpd", 0, callback_control_xl2tpd},
	{"unset", "hello", 0, callback_unset_hello},
	{"unset", "default", 0, callback_default},
	{"xl2tpd", "default", 0, callback_default},
	{"xl2tpd", "tunnel", 0, callback_xl2tpd_tunnel},
	{"xl2tpd", "session", 0, callback_xl2tpd_session},
	{"chilli", "default", 0, callback_default},
	{"chilli", "dhcp", 0, callback_chilli_dhcp},
	{"chilli", "send", 0, callback_control_send},
	{"chilli", "logout", 0, callback_chilli_logout},
	{"redir", "default", 0, callback_default},
	{"redir", "login", 0, callback_redir_login},
	{"ppp", "default", 0, callback_default},
	{"ppp", "echo", 0, callback_echo},
	{"ppp", "register", 0, callback_ppp_register},
	{"ppp", "network", 0, callback_ppp_network},
	{"ppp", "failure", 0, callback_ppp_failure},
	{"all", "show", 0, callback_all_show},
	{"", "", 0, NULL}
};

/* 
 * main 
 */
int main(int argc, char **argv)
{
	chilloutd_t chilloutd;
	int ret;
	int signal = 0;

	if (parse_command_line(&argc, &argv)) {
		fprintf(stderr, "E: parsing commandline. %s -h\n", argv[0]);
		return -1;
	}
	if (global_options.version) {
		printf("chilloutd version %s\n", VERSION);
		printf("Copyright 2007, FON Wireless Limited\n");
		return 0;
	}

	if (global_options.keepAlive) {
		return keepAlive();
	}
	if (global_options.echo) {
		return sendingEcho(global_options.echo);
	}
	if (global_options.check) {
		if (daemon_pid_file_is_running() >= 0) {
			printf("chilloutd already running\n");
			return 0;
		} else {
			printf("chilloutd not running\n");
			return 1;
		}
	}
	if ((!global_options.rulescript) && (!global_options.client)) {
		printf("specify a rulescript with -r <rulescript>\n");
		return 0;
	}
	if (global_options.killdaemon)
		signal = SIGTERM;
	else if (global_options.reloaddaemon)
		signal = SIGHUP;

	if (signal) {
		daemon_pid_file_ident = daemon_log_ident =
		    daemon_ident_from_argv0(argv[0]);
		if ((ret = daemon_pid_file_kill_wait(signal, 5)) < 0)
			printf("Error sending signal %d to daemon", signal);
		return ret < 0 ? 1 : 0;
	}
	if (global_options.client) {
		return client();
	}
	chilloutd = (chilloutd_t) calloc(1, sizeof(struct chilloutd));
	if (chilloutd == NULL)
		return 254;
	daemon_pid_file_ident = daemon_log_ident = daemon_ident_from_argv0(argv[0]);
	if (global_options.nodaemon) {
		log_start("chilloutd", 1);
		if ((ret = init(chilloutd)) != 0) {
			log_error("Error %d initializing. Exiting...", ret);
			daemon_retval_send(ret);
			return ret;
		}
	} else {
		if ((ret = daemonize(chilloutd)) != 0)
			return ret;
	}
	if (daemon_signal_init
	    (SIGINT, SIGQUIT, SIGTERM, SIGHUP, SIGALRM, SIGUSR1, SIGUSR2, SIGPIPE,
	     0) < 0) {
		log_error("registering signal handlers: %s", strerror(errno));
		return 255;
	}
	ret = loop(chilloutd);
	if (ret == 0)
		return finish(chilloutd);
	finish(chilloutd);
	return ret;
}
/*Begin:added by luokunling l00192527 ,fon用户登录成功之后.只清楚当前用户连接跟踪,不清楚所有fon用户*/
static int fonuser_conntrack_cleanup(const char *local_address)
{
     char acLine[128] = {0};

     unsigned int ulfoncurloginip = 0x00000000;
     if(NULL == local_address)
    {
        return -1;
    }
    inet_pton(AF_INET, local_address, &ulfoncurloginip);
    snprintf(acLine,128,FON_USER_CURRENTLOGIN_LANIP,ulfoncurloginip);
    system(acLine);
    return 0;
}
/*End:added by luokunling l00192527,2012/2/1*/
/* create_socket: */
static int create_socket(char *socket_path)
{
	struct sockaddr_un local;
	int len, sock;

	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		log_error("could not allocate unix socket");
		return -1;
	}
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, socket_path);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(sock, (struct sockaddr *)&local, len) == -1) {
		log_error("could not bind unix socket");
		close(sock);
		return -1;
	}
	if (listen(sock, 5) == -1) {
		log_error("could not listen to unix socket");
		close(sock);
		return -1;
	}
	/* Upon return, the socket is ready and awaiting connections */
	return sock;
}

static void signal_dispatch(chilloutd_t chilloutd, int sig)
{
	switch (sig) {
	case SIGINT:
	case SIGQUIT:
	case SIGTERM:
		log_critical("received SIGINT or SIGQUIT or SIGTERM");
		chilloutd->quit = 1;
		break;
	case SIGHUP:
		log_message("received SIGHUP. reloading configuration");
		/*
		   SMC_free_configuration(fonsmcd);
		   SMC_reload(fonsmcd);
		 */
		break;
	case SIGALRM:
		log_message("received SIGALRM: Alarm!");
		break;
	default:
		log_message("unknown signal %d received", sig);
		break;
	}
}

/*
 * loop:
 * select on all incoming sockets, both controlling sockets
 * and on our own control sockets, and redirect things
 */
static int loop(chilloutd_t chilloutd)
{
	fd_set fds;
	int sig, new, r;
	size_t size;
	struct sockaddr_un address;
	struct client *client;

	/* prepare file descriptor set */
	FD_ZERO(&chilloutd->fds);
	chilloutd->signalfd = daemon_signal_fd();
	FD_SET(chilloutd->signalfd, &chilloutd->fds);
	FD_SET(chilloutd->serverfd, &chilloutd->fds);
	chilloutd->quit = 0;

	while (!chilloutd->quit) {
		fds = chilloutd->fds;
		if (select(FD_SETSIZE, &fds, 0, 0, 0) < 0) {
			/* If we've been interrupted by an incoming signal, 
			 * continue */
			if (errno == EINTR)
				continue;
			daemon_log(LOG_ERR, "select(): %s", strerror(errno));
			break;
		}
		if (FD_ISSET(chilloutd->signalfd, &fds)) {
			/* Get signal waiting in the signalfd descriptor, 
			 * and dispatch */
			if ((sig = daemon_signal_next()) <= 0) {
				log_error("daemon_signal_next() failed.");
				break;
			}
			signal_dispatch(chilloutd, sig);
		} else if (FD_ISSET(chilloutd->serverfd, &fds)) {
			size = sizeof(struct sockaddr_un);
			new = accept(chilloutd->serverfd,
				     (struct sockaddr *)&address, &size);
			if (new < 0) {
				log_error("accept() failed on server socket");
				break;
			}
			/* Attach client information (fd) to chilloutd 
			 * linked list */
			add_client(chilloutd, new);
		} else {
			/* Cycle through fds descriptors, until the right one 
			 * is found */
			client = isset_client(chilloutd, &fds);
			if (client == NULL)
				continue;
			if ((r = read_and_process(client))) {
				if (r == CHILLOUT_DISCONNECTED) {
					log_message("client \"%s\" disconnect",
						    client->chillout->name);
					del_client(chilloutd, client, 0, 0);
				}
				/* if there has been an error, it's the 
				 * callback duty to answer using 
				 * chillout_write
				 */
			} else {
				/* MARK */
				chillout_write(client->chillout, 0, "ok");
			}
		}
	}
	return 0;
}
/* read_and_process: */
static int read_and_process(struct client *client)
{
	char buffer[MAX_QUERY];
	int r;
	size_t length;

	r = chillout_read(client->chillout, 0, buffer);
	if (r == CHILLOUT_DISCONNECTED) {
		return CHILLOUT_DISCONNECTED;
	} else if (r != 0) {
		return CHILLOUT_ERROR;
	}
	/* Get rid of the \n ending all messages */
	length = strlen(buffer);
	if (buffer[length - 1] == '\n')
		buffer[length - 1] = '\0';
	/* log_message("%s: \"%s\"", chillout_retrieve_name(client->chillout), buffer); */
	return chillout_process(client->chillout, buffer);
}

/* daemonize process, init chilloutd, parent wait for settle */
static int daemonize(chilloutd_t chilloutd)
{
	pid_t pid;
	int ret;

	if ((pid = daemon_pid_file_is_running()) >= 0) {
		printf("Daemon already running with PID %u\n", pid);
		return 1;
	}
	daemon_retval_init();
	if ((pid = daemon_fork()) < 0) {
		printf("Error calling daemon_fork()\n");
		daemon_retval_done();
		return 1;
	} else if (pid) {	/* parent process */
		int ret;
		if ((ret = daemon_retval_wait(20)) < 0) {
			printf("E: obtaining ret value from daemon process.");
			return 255;
		}
		if (ret != 0)
			printf("Child returned %d\n", ret);
		exit(ret);
	}
	/* daemon */
	log_start("chilloutd", 0);
	if ((ret = init(chilloutd)) != 0) {
		log_error("Error %d initializing. Exiting...", ret);
		daemon_retval_send(ret);
		return ret;
	}
	daemon_retval_send(0);
	return 0;
}

/* initialize the whole thing. upon return of this function,
 * all should be already running*/
static int init(chilloutd_t chilloutd)
{
	if (!global_options.nodaemon) {
		if (daemon_pid_file_create() < 0) {
			log_error("Could not create PID file: %s", strerror(errno));
			daemon_retval_send(1);
			return 255;
		}
	}
	chilloutd->serverfd =
	    create_socket(global_options.socket_path ? global_options.
			  socket_path : DEFAULT_SOCKET_PATH);
	if (chilloutd->serverfd == -1) {
		log_error("Error creating listening socket: %s", strerror(errno));
		return 254;
	}
	chilloutd->start_time = chilloutd->config_time = time(NULL);
	log_message("chilloutd successfully started");
	return 0;
}

/* close sockets, disable signal hooks, close logs, free structures ... */
static int finish(chilloutd_t chilloutd)
{
      struct client *node;
	daemon_signal_done();
	if (!global_options.nodaemon)
		daemon_pid_file_remove();
	close(chilloutd->serverfd);
	log_message("chillout successfully finished");

       /* close all clients */
        for (node = chilloutd->clients; node != NULL; node = node->next) 
        {
            close(node->chillout->readsock);
            close(node->chillout->writesock);
            node->chillout->readsock = -1;
            node->chillout->writesock = -1;
        }

        log_message("End chillout successfully finished");            
	return 0;
}

/* reload configuration */
static int reload(chilloutd_t chilloutd)
{
	chilloutd->config_time = time(NULL);
	log_message("chillout configuration reloaded");
	return 0;
}

/* parse_command_line */
static int parse_command_line(int *argc, char ***argv)
{
	OP_ERROR p_error;

	static OP_option options[] = {
		{"title", 'X', OP_TYPE_TITLE, "fonsmcd", NULL},
		{"kill", 'k', OP_TYPE_FLAG, "Kill already running daemon",
		 &(global_options.killdaemon)},
		{"reload", 'l', OP_TYPE_FLAG, "Reload configuration",
		 &(global_options.reloaddaemon)},
		{"check", 'C', OP_TYPE_FLAG, "Check if the daemon is already running",
		 &(global_options.check)},
		{"version", 'v', OP_TYPE_FLAG, "Print program version",
		 &(global_options.version)},
		{"configuration", 'f', OP_TYPE_STRING,
		 "Configuration file path (default /etc/config/chilloutd.conf)",
		 &(global_options.config_path)},
		{"socketpath", 's', OP_TYPE_STRING,
		 "Server socket path (default /tmp/chillout)",
		 &(global_options.socket_path)},
		{"client", 't', OP_TYPE_FLAG,
		 "Client mode, to send commands through the control socket",
		 &(global_options.client)},
		{"keep_alive", 'a', OP_TYPE_FLAG, "Evaluates if chilloutd is alive",
		 &(global_options.keepAlive)}, 
		{"echo", 'e', OP_TYPE_STRING, "Sends an \"echo\" message to specified client",
                 &(global_options.echo)},
#ifndef NDEBUG
		{"nodaemon", 'n', OP_TYPE_FLAG,
		 "Do not fork in the background, and log to stdout",
		 &(global_options.nodaemon)},
		{"chillispot", 'c', OP_TYPE_STRING, "chillispot chillout socket path",
		 &(global_options.chillispot)},
		{"xl2tpd", 'x', OP_TYPE_STRING, "xl2tpd chillout socket path",
		 &(global_options.xl2tpd)},
		{"chillout", 's', OP_TYPE_STRING, "chilloutd chillout socket path",
		 &(global_options.xl2tpd)},
		{"rulescript", 'r', OP_TYPE_STRING,
		 "script used to set/unset tunneling rules",
		 &(global_options.rulescript)},
#endif
		{NULL, '\0', OP_TYPE_FLAG, NULL, NULL}
	};

	memset(&global_options, 0, sizeof(global_options));
	p_error = OP_parse(options, *argc, *argv);
	if (p_error == OP_ERROR_HELP) {
		exit(0);
	} else if (p_error != OP_ERROR_OK) {
		log_critical("Error parsing commandline");
		return -1;
	}
	return 0;
}

static int add_client(chilloutd_t chilloutd, int fd)
{
	struct client *add;
	struct client *node;

	add = (struct client *)calloc(1, sizeof(struct client));
	if (add == NULL)
		return -1;
	add->chillout = chillout_init_from_fd("unset", fd, (void *)chilloutd);
	add->type = CLIENT_UNSET;
	add->lease = NULL;
	add->buffer[0] = '\0';
	if (add->chillout == NULL) {
		free(add);
		return -1;
	}
	add->chillout->callbacks = global_callbacks;
	add->next = NULL;
	for (node = chilloutd->clients; ((node != NULL) && (node->next != NULL));
	     node = node->next) ;
	if (node == NULL) {
		chilloutd->clients = add;
		FD_SET(fd, &(chilloutd->fds));
		return 0;
	}
	node->next = add;
	FD_SET(fd, &(chilloutd->fds));
	return 0;
}

static int release(chilloutd_t chilloutd, struct lease *lease)
{
	//struct client *chilli_client;
	//int ret;

	/* static struct client *retrieve_client_from_name(chilloutd_t, char *); */
	/*
	chilli_client = retrieve_client_from_name(chilloutd, "chilli");
	if (chilli_client == NULL) {
		log_error("%s(): ARG! chilli not registered", __FUNCTION__);
		return 1;
	} 
	chillout_write(chilli_client->chillout, CHILLOUT_WRITE_SOCK, "dhcp release %s", lease->mac);
	lease->status = LEASE_DETUNN;
	*/
	return 0;
}

static int del_client(chilloutd_t chilloutd, struct client *client, int readfd,
		      int writefd)
{
	struct client *node;
	int flags, fd;

	if (client == NULL) {
		if (readfd == 0) {
			if (writefd == 0)
				return -1;
			flags = CHILLOUT_WRITE_SOCK;
			fd = readfd;
		} else {
			flags = CHILLOUT_READ_SOCK;
			fd = writefd;
		}
		client = retrieve_client_from_fd(chilloutd, flags, fd);
		if (client == NULL) {
			log_error("%s: requesting to delete a client that doesn't exist",
				  __FUNCTION__);
			return -1;
		}
	}
	/* The descriptor to keep out from the select collection
	 * is the reading one */
	fd = client->chillout->readsock;
	if (fd != -1)
		FD_CLR(fd, &(chilloutd->fds));
	/* When del_client is called for a merge, the file descriptor of the read socket
	 * must have ben resseted to -1, else it is closed (twice)
	 */
	chillout_finish(client->chillout);
	if (chilloutd->clients == client) {
		chilloutd->clients = client->next;
	} else {
		for (node = chilloutd->clients; node != NULL; node = node->next) 
	    {
            if (node->next == client)
            {
                break;
            }
	    }
		if (node == NULL) {
			log_error("%s: client not found in list", __FUNCTION__);
			return -1;
		}
		node->next = client->next;
	}
	/*
	 * cleaning things up upon disconnect of PPPD's
	 */
	switch (client->type) {
	case CLIENT_PPPD:
		if (client->lease != NULL) {
			log_message("%s: about to deregister ppp", __FUNCTION__);
			if (client->lease->status != LEASE_DETUNN) {
				log_critical("%s: lease was not in detunn state, releasing lease in chillispot", __FUNCTION__);
				release(chilloutd, client->lease);
			} else {
				if (derule
				    (client->lease->ip, client->lease->device,
				     client->lease->wan_ip)) {
					log_critical("%s: error calling derule");
				}
				/* lease is orphan by now */
				if (client->lease->redir == NULL) {
					log_message("%s(): There is no redir (baad)", __FUNCTION__);
				} else {
					log_message("%(): Answering waiting redir process...", __FUNCTION__);
					chillout_write(client->lease->redir, 0, "ok");
					client->lease->redir = NULL;
				}
				if(client->lease->release)
					free(client->lease);
			}
			client->lease = NULL;
			client->type = CLIENT_UNSET;
		}
		break;
	default:
		if (client->lease != NULL) {
			log_error("%s: Upon deletion, client->lease != NULL",
				  __FUNCTION__);
		}
		break;
	}
	free(client);
	return 0;
}

static struct client *retrieve_client_from_fd(chilloutd_t chilloutd, int flags, int fd)
{
	struct client *client;
	int csocket;
	
	for (client = chilloutd->clients; (client); client = client->next) {
		if (flags & CHILLOUT_WRITE_SOCK)
			csocket = client->chillout->writesock;
		else if (flags & CHILLOUT_READ_SOCK)
			csocket = client->chillout->readsock;
		else
			return NULL;
		if (csocket == fd)
			return client;
	}
	return NULL;
}

static struct client *retrieve_client_from_name(chilloutd_t chilloutd, char *name)
{
	struct client *client;

	for (client = chilloutd->clients; client != NULL; client = client->next) {
		if (!strcmp(client->chillout->name, name))
			return client;
	}
	return NULL;
}

static struct client *retrieve_client_from_chillout(chilloutd_t chilloutd,
						    chillout_t chillout)
{
	struct client *client;

	for (client = chilloutd->clients; client != NULL; client = client->next)
		if (client->chillout == chillout)
			return client;
	return NULL;
}

static struct client *isset_client(chilloutd_t chilloutd, fd_set * fds)
{
	struct client *client;

	for (client = chilloutd->clients; client; client = client->next) {
		if (FD_ISSET(client->chillout->readsock, fds))
			return client;
	}
	return NULL;
}

/* setup client loop, registering as control ... hum
 * better use socat */
static int client(void)
{
	chillout_t chillout;
	char buffer[MAX_QUERY];
	int ret;

	chillout =
	    chillout_init(global_options.socket_path ? global_options.
			  socket_path : DEFAULT_SOCKET_PATH, "control", NULL);
	if (chillout == NULL)
		return -1;
	do {
		if (fgets(buffer, MAX_QUERY, stdin) == NULL) {
			printf("EOF received\n");
			break;
		}
		ret = chillout_write(chillout, CHILLOUT_WRITE_SOCK, buffer);
		if (ret == -1) {
			printf("chillout_write()\n");
			break;
		}
		while (1) {
			if (ret = chillout_read(chillout, CHILLOUT_WRITE_SOCK, buffer)) {
				printf("chillout_read()\n");
				break;
			}
			if (!strcmp(buffer, "ok\n")) {
				break;
			}
			printf("%s", buffer);
			if (!strncmp(buffer, "error", 5))
				break;
		}
	} while (1);

	return chillout_finish(chillout);
}

static int keepAlive(void)
{
	chillout_t chillout;
	char Message[] = "hello write control";
	char buffer[MAX_QUERY];
	int ret;

	chillout =
	    chillout_init_for_test(global_options.socket_path ? global_options.
			  socket_path : DEFAULT_SOCKET_PATH, "control", NULL);
	if (chillout == NULL)
		return 1;

	ret = chillout_write_for_test(chillout, CHILLOUT_WRITE_SOCK, Message);
	if (ret == -1) {
		return 1;
	}
	if (ret = chillout_read_for_test(chillout, CHILLOUT_WRITE_SOCK, buffer, TESTING_TIMEOUT_IN_SECONDS)) {
		return 1;
	}
	if (!strcmp(buffer, Message)) {
		return 1;
	}
	return chillout_finish(chillout);
}

static int sendingEcho(char* client)
{
	chillout_t chillout;
	char Message[MAX_QUERY];
	char buffer[MAX_ANSWER];
	int ret;

	chillout =
	    chillout_init_for_test(global_options.socket_path ? global_options.
			  socket_path : DEFAULT_SOCKET_PATH, "control", NULL);
	if (chillout == NULL)
		return 1;
	sprintf(Message, "hello write control");
	ret = chillout_write_for_test(chillout, CHILLOUT_WRITE_SOCK, Message);
	if (ret == -1) {
		return 1;
	}
	if (ret = chillout_read_for_test(chillout, CHILLOUT_WRITE_SOCK, buffer, TESTING_TIMEOUT_IN_SECONDS)) {
		return 1;
	}
	if (!strcmp(buffer, Message)) {
		return 1;
	}
	sprintf(Message, "send %s echo invalid", client);
	ret = chillout_write_for_test(chillout, CHILLOUT_WRITE_SOCK, Message);
        if (ret == -1) {
                return 1;
        }
	if (ret = chillout_read_for_test(chillout, CHILLOUT_WRITE_SOCK, buffer, TESTING_TIMEOUT_IN_SECONDS)) {
                return 1;
        }
	/*response must be: ok\n response, so we want the second line.*/
	if (ret = chillout_read_for_test(chillout, CHILLOUT_WRITE_SOCK, buffer, TESTING_TIMEOUT_IN_SECONDS)) {
                return 1;
        }
        if (strstr(buffer, "not connected")) {
                return 1;
        }

	return chillout_finish(chillout);
}

static int date_string_from_time_t(char *buffer, time_t date)
{
	struct tm stm;
	time_t t = date;

	gmtime_r(&t, &stm);
	strftime(buffer, 255, "%Y-%m-%d %H:%M:%S", &stm);

	return 0;
}

/***************************************************************************
 CONTROL SOCKET CALLBACKS 
 ***************************************************************************/

static int callback_echo(chillout_t co, void *data, int argc, char **argv)
{
	int i;

	for (i = 0; i < argc; i++)
		chillout_write(co, 0, argv[i]);
	return 0;
}

static int callback_default(chillout_t co, void *data, int argc, char **argv)
{
	chillout_write(co, 0, "error unknown token %s", argv[0]);
	return 1;
}

/* more than in all_show */
static int callback_control_show(chillout_t co, void *data, int argc, char **argv)
{
	time_t time_now;
	int i;
	struct client *node;
	char bufdate[256];
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	if (argc < 2) {
		chillout_write(co, 0, "error argc count %d", argc);
		return 1;
	}
	if (!strcmp(argv[1], "uptime")) {
		time_now = time(NULL);
		chillout_write(co, 0, "%lu", time_now - chilloutd->start_time);
	} else if (!strcmp(argv[1], "configtime")) {
		time_now = time(NULL);
		chillout_write(co, 0, "%lu", time_now - chilloutd->config_time);
	} else if (!strcmp(argv[1], "clients")) {
		for (node = chilloutd->clients; node != NULL; node = node->next) {
			date_string_from_time_t(bufdate, node->chillout->date);
			chillout_write(co, 0, "%s %s", node->chillout->name, bufdate);
		}
	} else if (!strcmp(argv[1], "numbers")) {
		chillout_write(co, 0, "num_leases %d num_auth_leases %d",
			       chilloutd->chillispot.num_leases,
			       chilloutd->chillispot.num_auth_leases);
	} else if (!strcmp(argv[1], "version")) {
		chillout_write(co, 0, "version %s", VERSION);
	} else if (!strcmp(argv[1], "callbacks")) {
		if (argc == 2) {
			if (co->callbacks == NULL)
				return 1;
			for (i = 0; co->callbacks[i].ident[0] != '\0'; i++) {
				chillout_write(co, 0, "%s %s", co->callbacks[i].ident,
					       co->callbacks[i].keyword);
			}
		} else if (argc == 3) {
			for (i = 0; co->callbacks[i].ident[0] != '\0'; i++) {
				if (!strcmp(argv[2], co->callbacks[i].ident)) {
					chillout_write(co, 0, "%s",
						       co->callbacks[i].keyword);
				}
			}
		} else {
			chillout_write(co, 0, "error argc %d?", argc);
			return 1;
		}
	} else {
		chillout_write(co, 0, "error %s?", argv[1]);
	}
	return 0;
}

static char *strstatus(int status)
{
	switch (status) {
	case LEASE_PEND:
		return "pend";
	case LEASE_PPPD:
		return "pppd";
	case LEASE_TUNN:
		return "tunn";
	case LEASE_AUTH:
		return "auth";
	case LEASE_DENY:
		return "deny";
	case LEASE_DETUNN:
		return "detunn";
	case LEASE_REGISTERED:
		return "register";
	default:
		return "unknown";
	}
}

/* Information and commands for chillispot */
static int callback_control_chilli(chillout_t co, void *data, int argc, char **argv)
{
	char bufdate[256];
	char bufdatelogin[256];
	struct lease *l;
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	if (argc < 2) {
		chillout_write(co, 0, "error argc count %d", argc);
		return 1;
	}
	if (!strcmp(argv[1], "leases")) {
		for (l = chilloutd->chillispot.leases; l != NULL; l = l->next) {
			date_string_from_time_t(bufdate, l->date);
			date_string_from_time_t(bufdatelogin, l->login_date);
			chillout_write(co, 0,
				       "[%s]i [%s] [%s] [%s] [%s] [%s] [%s] [%s] [%s]",
				       l->mac, l->ip, bufdate, bufdatelogin, l->username,
				       l->password, l->pppd_pid, l->device,
				       strstatus(l->status));
		}
	} else {
		chillout_write(co, 0, "error %s?", argv[1]);
	}
	return 0;
}

static int callback_control_xl2tpd(chillout_t co, void *data, int argc, char **argv)
{
	if (argc < 3) {
		chillout_write(co, 0, "error argc count %d", argc);
		return 1;
	}
	if (!strcmp(argv[1], "tunnels")) {
		/* TODO: list tunnels and status for them */
	} else if (!strcmp(argv[1], "connections")) {
		/* TODO: list authenticated connections trough the tunnel */
	} else {
		chillout_write(co, 0, "error %s?", argv[1]);
	}
	return 0;
}

static int callback_control_sleep(chillout_t co, void *data, int argc, char **argv)
{
	int seconds;

	seconds = atoi(argv[1]);
	if (seconds)
		sleep(seconds);
	return 0;
}

static int callback_control_reload(chillout_t co, void *data, int argc, char **argv)
{
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	chillout_write(co, 0, "reloading configuration...");
	if (reload(chilloutd)) {
		chillout_write(co, 0, "error reloading configuration");
		return 1;
	}
	return 0;
}

/* forward a message to a client connected */
static int callback_control_send(chillout_t co, void *data, int argc, char **argv)
{
	int i, reading, ret;
	char buffer[MAX_QUERY];
	struct client *cl;
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	if (argc <= 2) {
		chillout_write(co, 0, "E: argc");
		return 1;
	}
	cl = retrieve_client_from_name(chilloutd, argv[1]);
	if (cl == NULL) {
		chillout_write(co, 0, "E: client %s not connected", argv[1]);
		return 1;
	}
	buffer[0] = '\0';
	for (i = 2; i < argc; i++) {
		strcat(buffer, argv[i]);
		if (i < argc - 1)
			strcat(buffer, " ");
	}
	log_message("%s: str [%s] client [%s]", __FUNCTION__, buffer, argv[1]);
	chillout_write(cl->chillout, CHILLOUT_WRITE_SOCK, buffer);
	/* receive and forward answers */
	reading = 1;
	ret = 0;
	while (reading) {
		/* FIXME */
		if (chillout_read(cl->chillout, CHILLOUT_WRITE_SOCK, buffer)) {
			printf("chillout_read()\n");
			break;
		}
		if (!strncmp(buffer, "ok", 2)) {
			reading = 0;
			ret = 0;
		}
		if (!strncmp(buffer, "error", 5)) {
			reading = 0;
			ret = 1;
		}
		chillout_write(co, 0, buffer);
	}
	if (!ret)
		ret = CHILLOUT_NOOK;
	return ret;
}

/***************************************************************************
                            XL2TP CALLBACKS 
 ***************************************************************************/
/*
 * tunnel abort from xl2tpd
 */
static int callback_xl2tpd_tunnel(chillout_t co, void *data, int argc, char **argv)
{
	chillout_write(co, 0, "echo tunnel");
	return 0;
}

/*
 * session abort from xl2tpd
 */
static int callback_xl2tpd_session(chillout_t co, void *data, int argc, char **argv)
{
	chillout_write(co, 0, "echo tunnel");
	return 0;
}

/***************************************************************************
                            PPP CALLBACKS 
 ***************************************************************************/
/*
 * register success <name> <ppp device> <pid> <mac>
 * register failure <name> <ppp device> <pid> <mac>
 */
static int callback_ppp_register(chillout_t co, void *data, int argc, char **argv)
{
	struct lease *l;
	struct chilloutd *cd = (struct chilloutd *)data;
	struct client *client;

	log_message("%s()", __FUNCTION__);
	if (argc != 6) {
		chillout_write(co, 0, "error incorrect argc %d", argc);
		log_error("%s(): error incorrect argc %d", __FUNCTION__, argc);
		return -1;
	}
	log_message("%s(): received login", __FUNCTION__);
	l = get_lease_string(FROM_MAC, &(cd->chillispot), argv[5]);
	if (l == NULL) {
		log_error("%s(): login %s not registered!", __FUNCTION__, argv[2]);
		chillout_write(co, 0, "error user %s (mac %s) not registered", argv[2], argv[5]);
		return 1;
	}
	if (!strncmp(argv[1], "fail", 4)) {
		log_error("%s(): failure reported from pppd authentication", __FUNCTION__);
		client = retrieve_client_from_chillout(cd, co);
		if (client == NULL) {
			log_error("%s(): %d: Error retrieving chillout client",
			__FUNCTION__, __LINE__);
			l->status = LEASE_PEND;
			return 0;
		}
		log_message("%s() client handler %x", __FUNCTION__, (void *)client);
		if (client->buffer[0] == '\0') {
			chillout_write(l->redir, 0, "error unknown");
			log_message("%s() error unknown", __FUNCTION__);
		} else {
			chillout_write(l->redir, 0, "error %s", client->buffer);
			log_message("%s() error %s written in redir l->redir %x", __FUNCTION__, client->buffer,
			l->redir);
			client->buffer[0] = '\0';
		}
		l->status =  LEASE_PEND;
		return 0;
	}
	if (l->status != LEASE_PPPD) {
		log_error
		    ("%s(): received pppd register for a lease in status != LEASE_PPPD",
		     __FUNCTION__);
		chillout_write(co, 0, "bad status on lease");
		if (l->redir != NULL) {
			chillout_write(l->redir, 0, "error bad status");
			l->redir = NULL;
		}
		return 1;
	}
	log_message("%s(): received register for user %s, updating lease",
		    __FUNCTION__, argv[2]);
	strcpy(l->device, argv[3]);
	strcpy(l->pppd_pid, argv[4]);
	client = retrieve_client_from_chillout(cd, co);
	if (client == NULL) {
		log_error("%(): AAAAARG!!!", __FUNCTION__);
		if (l->redir != NULL) {
			chillout_write(l->redir, 0, "error aaarg");
			l->redir = NULL;
		}
		return 1;
	}
	client->lease = l;
	client->type = CLIENT_PPPD;
	client->lease->status = LEASE_REGISTERED;
	client->lease->date = time(NULL);
	client->buffer[0] = '\0';
	return 0;
}

/* ppp network up 
 * ppp network down
 * 
 * The associated lease is extracted from the omnipresent list
 */
static int callback_ppp_network(chillout_t co, void *data, int argc, char **argv)
{
	struct lease *l;
	struct client *c;
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	if (argc != 2) {
		chillout_write(co, 0, "error incorrect argc %d", argc);
		log_error("%s(): error incorrect argc %d", __FUNCTION__, argc);
		return -1;
	}
	/* retrieve lease */
	c = retrieve_client_from_chillout(chilloutd, co);
	if ((c == NULL) || (c->lease == NULL)) {
		chillout_write(co, 0, "error lease not found");
		log_error("%s(): lease not found", __FUNCTION__);
		return -1;
	}
	l = c->lease;

	if (!strcmp(argv[1], "up")) {
		if (l->redir == NULL) {
			log_error("%(): There is no redir?", __FUNCTION__);
			return 1;
		}
		if (l->status != LEASE_REGISTERED) {
			log_error("%(): Lease not registered (assert)", __FUNCTION__);
			chillout_write(l->redir, 0, "error assert");
			return 1;
		}
              /*Begin:added by luokunling l00192527 ,fon用户登录成功之后.只清楚当前用户连接跟踪,不清楚所有fon用户*/
              if(fonuser_conntrack_cleanup(l->ip))
             {
                    log_error("%s(): onntrack_cleanup", __FUNCTION__);
			chillout_write(l->redir, 0, "error conntrack_cleanup");
			return 1;
             }
              /*End:added by luokunling l00192527,2012/2/1*/
		if (retrieve_wan_address(l->device, l->wan_ip)) {
			log_error("%s(): Error obtaining WAN ip address", __FUNCTION__);
			chillout_write(l->redir, 0, "error device");
			return 1;
		}
		if (rule(l->ip, l->device, l->wan_ip)) {
			log_error("%s(): Error inserting iptables rule", __FUNCTION__);
			chillout_write(l->redir, 0, "error iptables");
			return 1;
		}
		chillout_write(l->redir, 0, "ok");

		/*
		{
			struct client *chilli_client;
			chilli_client = retrieve_client_from_name(chilloutd, "chilli");
			if (chilli_client != NULL) {
				chillout_write(chilli_client->chillout, CHILLOUT_WRITE_SOCK, "dhcp authorize %s %s", l->mac, l->device);
			} else {
				log_error("%s(): ARG! chilli not registered", __FUNCTION__);
			}
		}
		*/

		l->status = LEASE_TUNN;
		l->redir = NULL;
	} else if (!strcmp(argv[1], "down")) {
	       /*Begin:added by luokunling l00192527 ,fon用户登录成功之后.只清楚当前用户连接跟踪,不清楚所有fon用户*/
	       fonuser_conntrack_cleanup(l->ip);
              /*End:added by luokunling l00192527,2012/2/1*/
		log_message("%s(): Doing nothing on network down (waiting for pppd to finish)", __FUNCTION__);
	} else {
		chillout_write(co, 0, "unknown argument %s", argv[1]);
		log_error("%s(): unknown argument %s", __FUNCTION__, argv[1]); 
		return -1;
	}
	return 0;
}

/*
 * received before a "register fail". Used to store the reason that is
 * sent afterwards to chillispot
 *
 * failure reason <reason>
 */
static int callback_ppp_failure(chillout_t co, void *data, int argc, char **argv)
{
	struct client *c;
	struct lease *l;
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	if (argc != 3) {
		chillout_write(co, 0, "error incorrect argc %d", argc);
		log_error("%s(): error incorrect argc %d", __FUNCTION__, argc);
		return -1;
	}
	/* retrieve lease */
	c = retrieve_client_from_chillout(chilloutd, co);
	if (c == NULL) {
		chillout_write(co, 0, "error client not found");
		log_error("%s(): client not found", __FUNCTION__);
		return -1;
	}
	log_message("%s() reason %s", __FUNCTION__, argv[2]);
	strcpy(c->buffer, argv[2]);
	log_message("%s() client handler %x", __FUNCTION__, (void
	*)c);
	
	 /* start IPCP fail, notify chillout-client */ 
	l = c->lease;
	if (l && l->redir)
	{
		chillout_write(l->redir, 0, "error %s", c->buffer);
	}
    /* end IPCP fail, notify chillout-client */
	
	return 0;
}

/***************************************************************************
                            CHILLI CALLBACKS 
 ***************************************************************************/
/* chilli show callback is used by clients to retrieve some information */
static int callback_all_show(chillout_t co, void *data, int argc, char **argv)
{
	if (argc < 2) {
		chillout_write(co, 0, "error argc count %d", argc);
		return 1;
	}
	if (!strcmp(argv[1], "version")) {
		chillout_write(co, 0, "version %s", VERSION);
	} else {
		chillout_write(co, 0, "error %s?", argv[1]);
		return 1;
	}
	return 0;
}

/* 
 * callback_chilli_dhcp
 *
 * Used by chillispot to notify  dhcp leases and releases
 */
static int callback_chilli_dhcp(chillout_t co, void *data, int argc, char **argv)
{
	struct chilloutd *chilloutd = (struct chilloutd *)data;

	if (argc < 4) {
		chillout_write(co, 0, "E: in client. argc count %d", argc);
		return 1;
	}
	if (!strcmp(argv[1], "lease")) {
		log_message("%s: new lease notified. mac address %s, ip %s",
			    __FUNCTION__, argv[2], argv[3]);
		if (add_lease(&(chilloutd->chillispot), argv[2], argv[3])) {
			log_error("%s: error adding lease mac %s, ip %s",
				  argv[2], argv[3]);
			chillout_write(co, 0, "error");
			return 1;
		}
		return 0;
	} else if (!strcmp(argv[1], "release")) {
		log_message("%s: new release notified. mac address %s, ip %s",
			    __FUNCTION__, argv[2], argv[3]);
		if (del_lease(chilloutd, &(chilloutd->chillispot), argv[2])) {
			log_error("%s: error deleting lease mac %s, ip %s",
				  __FUNCTION__, argv[2], argv[3]);
			chillout_write(co, 0, "error");
			return 1;
		}
		return 0;
	} else {
		chillout_write(co, 0, "error %s?", argv[1]);
		return 1;
	}
	return 0;
}

/* chillout_hello_callback */
static int callback_unset_hello(chillout_t co, void *data, int argc, char **argv)
{
	struct callback *node;
	int i;
	struct client *client;
	chillout_t newco;
	struct chilloutd *cd = (struct chilloutd *)data;

	if (argc != 3) {
		chillout_write(co, 0, "error incorrect argc %d", argc);
		return -1;
	}

	for (i = 0; co->callbacks[i].keyword[0] != '\0'; i++) {
		node = &(co->callbacks[i]);
		if (strcmp(node->ident, argv[2]))
			continue;
		if (!strcmp(argv[1], "read")) {
			client = retrieve_client_from_name(cd, argv[2]);
			if (client == NULL) {
				log_error("%s(): read sock before write socket [%s]",
					  __FUNCTION__, argv[2]);
				chillout_write(co, CHILLOUT_WRITE_SOCK,
					       "no write socket");
				return 1;
			}
			client->chillout->writesock = co->readsock;
			FD_CLR(client->chillout->writesock, &(cd->fds));
			co->readsock = -1;
			newco = client->chillout;
			client = retrieve_client_from_chillout(cd, co);
			co = newco;
			if (del_client(cd, client, 0, 0)) {
				chillout_write(co, CHILLOUT_WRITE_SOCK,
					       "error deleting write client");
				return 1;
			}
			/* We write the hello and the ok. This is the only
			 * time we are sending something in our receiving 
			 * socket */
			chillout_write(co, CHILLOUT_WRITE_SOCK, "hello read %s",
				       co->name);
			chillout_write(co, CHILLOUT_WRITE_SOCK, "ok");
			log_message("%s(): read connection established from client %s",
				    __FUNCTION__, co->name);
			return CHILLOUT_NOOK;
		} else {
			log_message("%s(): write connection established from client %s",
				    __FUNCTION__, co->name);
			strcpy(co->name, argv[2]);
			chillout_write(co, 0, "hello write %s", co->name);
		}
		log_message("client %s registered", co->name);
		return 0;
	}
	chillout_write(co, 0, "error unknown ident %s", argv[2]);
	return 1;
}

/*
 * login <ip> <login> <password>
 * login <ip> <login> <chap> <chapppass> 
 */
static int callback_redir_login(chillout_t co, void *data, int argc, char **argv)
{
	struct lease *l;
	int ret;
	struct chilloutd *cd = (struct chilloutd *)data;

	log_message("%s()", __FUNCTION__);
	if ((argc < 4) || (argc > 5)) {
		chillout_write(co, 0, "error incorrect argc %d", argc);
		return -1;
	}
	log_message("%s(): received login", __FUNCTION__);
	l = get_lease_string(FROM_IP, &(cd->chillispot), argv[1]);
	if (l == NULL) {
		log_error("%s(): address %s not registered!", __FUNCTION__, argv[1]);
		return 1;
	}
	switch(l->status) {
	case LEASE_PPPD: /* waiting for pppd */		
	case LEASE_TUNN:
	case LEASE_AUTH:
	case LEASE_REGISTERED:
		log_message("%s(): skipping pppd for mac %s",
			    __FUNCTION__, l->mac);
		return CHILLOUT_NOOK;
	}
	log_message("%s(): user %s ip %s mac %s password %s redir %x",
		    __FUNCTION__, argv[2], argv[1], l->mac, argv[3],
		    co);
	l->redir = co;
	ret = login(cd, l, argv[2], argv[3]);
	/*Modified by luokunling l00192527,2012/12/21 当返回值是1 表示建立隧道失败,此处应该告诉client 释放进程资源*/
	if(1 == ret)
	{
	    chillout_write(co, 0,"error session xl2tp");
	}
	/*Ended by luokunling*/
	if (ret != 0)
		return ret;
	else
		return CHILLOUT_NOOK;
}

/* logout <ip> <login> <reason> */
static int callback_chilli_logout(chillout_t co, void *data, int argc, char **argv)
{
	struct lease *l;
	struct chilloutd *cd = (struct chilloutd *)data;
	int ret;

	if (argc != 4) {
		chillout_write(co, 0, "error incorrect argc %d", argc);
		log_error("%s(): received wrong %d parameters", __FUNCTION__, argc);
		return -1;
	}
	log_message("%s(): received logout ", __FUNCTION__);
	l = get_lease_string(FROM_IP, &(cd->chillispot), argv[1]);
	if (l == NULL) {
		log_error("%s(): address %s not registered!", __FUNCTION__, argv[1]);
		return 1;
	}
	ret = logout(cd, l);
	/*Modified by luokunling l00192527,2012/12/21 当返回值是1 表示许logout 失败,此处应该告诉client 释放进程资源*/
	if(1 == ret)
	{
	    chillout_write(co, 0, "error delete session failed");
	}
	/*Ended by luokunling*/
	if (ret != 0)
		return ret;
	l->redir = co;
	return CHILLOUT_NOOK;
}



/**************************************
 * LEASE LIST HANDLERS
 ***************************************/
static int add_lease(struct chillispot *ch, char *mac, char *ip)
{
	struct lease *l, *n, *p;

	if (strlen(mac) > 17)
		return -1;
	if (strlen(ip) > 15)
		return -1;
	if ((l = (struct lease *)calloc(1, sizeof(struct lease))) == NULL)
		return -1;
	strncpy(l->mac, mac, 17);
	strncpy(l->ip, ip, 15);
	l->date = time(NULL);
	p = NULL;
	for (n = ch->leases; n != NULL; n = n->next) {
		if (strcmp(n->mac, mac)) {
			p = n;
			continue;
		}
		if (!strcmp(n->ip, ip)) {
			log_message("%s: renewing date for mac %s", __FUNCTION__, mac);
			n->date = time(NULL);
			free(l);
			return 0;
		} else {
			log_message("%s: overwriting ip mac %s", __FUNCTION__, mac);
			strncpy(n->ip, ip, 15);
			n->date = time(NULL);
			free(l);
			return 0;
		}
	}
	if (p == NULL) {
		log_message("%s: first lease registered %s %s", __FUNCTION__, mac, ip);
		ch->leases = l;
		ch->num_leases = 1;
	} else {
		log_message("%s: lease appended %s %s", __FUNCTION__, mac, ip);
		p->next = l;
		(ch->num_leases)++;
	}
	return 0;
}

static int del_lease(chilloutd_t chilloutd, struct chillispot *ch, char *mac)
{
	struct lease *n, *p;

	p = NULL;
	for (n = ch->leases; n != NULL; n = n->next) {
		if (!strcmp(n->mac, mac)) {
			if (p == NULL)
			{   
			    //ch->leases = NULL; //修改之前
			    //modified by luokunling l00192527 删除链表头结点时，头指针需移到下一个节点
			    ch->leases = ch->leases->next;
			}
			else
				p->next = n->next;
			(ch->num_leases)--;
			if ((n->status == LEASE_AUTH) || (n->status == LEASE_TUNN) || (n->status == LEASE_REGISTERED)) {
				log_critical
				    ("%s(): about to delete an authenticated lease (it will be deleted when ppp disconnects)",
				     __FUNCTION__);
				n->release = 1;
				detunnel_lease(chilloutd, n);
				return 0;
			}
			free(n);
			return 0;
		}
		p = n;
	}
	return 1;
}

static int free_lease_list(struct chillispot *ch)
{
	struct lease *l, *pstTmp;

    l = ch->leases;
    //修改coverity:原来的链表释放方式有漏洞
    while (l)
    {
        pstTmp = l->next;
        free(l);
        l = pstTmp;
    }
	ch->leases = NULL;
	return 0;
}

static char *get_ip_from_mac(struct chillispot *ch, char *mac)
{
	struct lease *n;

	n = get_lease_string(FROM_MAC, ch, mac);
	if (n == NULL)
		return NULL;
	return n->ip;
}

static char *get_mac_from_ip(struct chillispot *ch, char *ip)
{
	struct lease *n;

	n = get_lease_string(FROM_IP, ch, ip);
	if (n == NULL)
		return NULL;
	return n->mac;
}

static struct lease *get_lease_string(int what, struct chillispot *ch, char *str)
{
	struct lease *n;

	if (str[0] == '\0')
		return NULL;
	for (n = ch->leases; n != NULL; n = n->next) {
		switch (what) {
		case FROM_MAC:
			if (!strcmp(str, n->mac))
				return n;
			break;
		case FROM_IP:
			if (!strcmp(str, n->ip))
				return n;
			break;
		case FROM_USERNAME:
			if (!strcmp(str, n->username))
				return n;
			break;
		case FROM_DEVICE:
			if (!strcmp(str, n->device))
				return n;
			break;
		case FROM_CID:
			if (!strcmp(str, n->cid))
				return n;
			break;
		default:
			return NULL;
		}
	}
	return NULL;
}

/**************************************
 * Authentication list handlers
 ***************************************/
static int tunnel_lease(chilloutd_t c, struct lease *l, char *user, char *pass)
{
	struct client *xl2tp_client;
	char *lns;
	int ret, retry = 0;
	const char *acLACs[2]={"primary", "secondary"};
	static int index = 0;//init to primary

	strcpy(l->username, user);
	strcpy(l->password, pass);
	if (l->status == LEASE_AUTH) {
		log_message("%s(): reauthenticating lease", __FUNCTION__);
		l->status = LEASE_PEND;
		(c->chillispot.num_auth_leases)--;
	}
	/* static struct client *retrieve_client_from_name(chilloutd_t, char *); */
	xl2tp_client = retrieve_client_from_name(c, "xl2tpd");
	if (xl2tp_client == NULL) {
		log_error("%s(): ARG! xl2tpd not registered", __FUNCTION__);
		return 1;
	}
	//lns = "primary";
	lns = acLACs[index];
	do {
		ret = session_create(xl2tp_client->chillout, lns, user, pass, l->cid, l->mac);
		if (ret == SESSION_OK) {
			break;
		} else if ((ret != SESSION_XL2TPD) || (retry)) {
			reset_lease(c, l);
			return 1;
		}
		retry = 1;
		//如果切换了服务器，记住，后续的session也在上面建
		//lns = "secondary";
		index = (index + 1) % 2;
		lns = acLACs[index];
	} while(1);
	l->status = LEASE_PPPD;	/* waiting for pppd */
	l->login_date = time(NULL);
	return 0;
}

static int detunnel_lease(chilloutd_t c, struct lease *l)
{
	struct client *xl2tp_client;

	if (l->status != LEASE_TUNN) {
		log_critical("about to detunnel a lease not tunneled (cleaning up?)");
	}
	if (l->status == LEASE_PEND) {
		log_critical("Refusing to detunnel a pending lease");
		log_critical("%s(): about to death a pending lease!", __FUNCTION__);
		return 1;
	}
	l->status = LEASE_PEND;
	(c->chillispot.num_auth_leases)--;
	xl2tp_client = retrieve_client_from_name(c, "xl2tpd");
	if (xl2tp_client == NULL) {
		log_error("%s(): ARG! xl2tpd not registered", __FUNCTION__);
		return 1;
	}
	/* Notify XL2TPD. Upon exit of the pppd client, rules are
	 * taken out 
	 */
	l->status = LEASE_DETUNN;
	l->login_date = time(NULL);
	return session_delete(xl2tp_client->chillout, "fon", l->cid);
	/*
	   l->status = LEASE_PPPD;
	 */
	/*
	   if (ret != 0) {
	   reset_lease(l);
	   return ret;
	   }
	   reset_lease(l);
	   return 0;
	 */
}

static int session_delete(chillout_t c, char *lac, char *cid)
{
	return chillout_write(c, CHILLOUT_WRITE_SOCK | CHILLOUT_WAIT_FOR_ANSWER,
			      "session delete %s", cid);
}

static int session_create(chillout_t c, char *lac, char *user, char *password, char *cid, char *mac)
{
	char buffer[256];
	char *vector[MAX_TOKENS];
	int num_tokens;

	chillout_write(c, CHILLOUT_WRITE_SOCK, "session create %s %s %s %s", lac, user,
		       password, mac);
	cid[0] = '\0';

	while (1) {
		if (chillout_read(c, CHILLOUT_WRITE_SOCK, buffer)) {
			log_error("%s(): chillout_read", __FUNCTION__);
			break;
		}
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		/* log_message("%s(): [%s]", __FUNCTION__, query); */
		if (chillout_string_to_vector(vector, buffer, &num_tokens, MAX_TOKENS)) {
			log_error("%s(): error parsing answer from xl2tpd",
				  __FUNCTION__);
			return SESSION_ERROR;
		}
		if (!(strcmp(vector[0], "error"))) {
			log_error("%s(): xl2tpd reported an error", __FUNCTION__);
			return SESSION_XL2TPD;
		}
		if (!(strcmp(vector[0], "cid"))) {
			strcpy(cid, vector[1]);
		}
		if (!(strncmp(vector[0], "ok", 2))) {
			if (cid[0] != '\0') {
				return SESSION_OK;
			} else {
				log_error("%s: %d: assert", __FUNCTION__, __LINE__);
				return SESSION_ERROR;
			}
		}
	}
	return SESSION_ERROR;
}

static int retrieve_wan_address(char *device, char *wan_address)
{
	char buffer[1024];
	FILE *p;

	log_message("%s(): about to retrieve wan address of device %s", __FUNCTION__,
		    device);
	sprintf(buffer, "ifconfig %s | grep addr: | awk '{ print $2 }' | cut -d: -f 2",
		device);
	p = (FILE *) popen(buffer, "r");
	if (p == NULL) {
		return -1;
	}
	if (fgets(wan_address, 1024, p) == NULL) {
		pclose(p);
		return -1;
	}
	if (wan_address[strlen(wan_address) - 1] == '\n')
		wan_address[strlen(wan_address) - 1] = '\0';
	pclose(p);
	log_message("%s(): wan_address for device %s is %s", __FUNCTION__, device,
		    wan_address);
	return 0;
}

static int rule(char *local_address, char *device, char *wan_address)
{
	char command_buffer[4096];
	int table;

	command_buffer[0] = '\0';
	table = retrieve_table_from_device(device);
	if (table < 0)
		return 1;
	sprintf(command_buffer, "%s set %s %s %d %s",
		global_options.rulescript, local_address, device, table, wan_address);
	log_message("%s() about to call [%s]", __FUNCTION__, command_buffer);
	return system(command_buffer);
}

static int retrieve_table_from_device(char *device)
{
	return atoi(device + 3) + 1;
}

static int derule(char *local_address, char *device, char *wan_address)
{
	char command_buffer[4096];
	int table;
	table = retrieve_table_from_device(device);
	if (table < 0)
		return 1;
	sprintf(command_buffer, "%s unset %s %s %d %s",
		global_options.rulescript, local_address, device, table, wan_address);
	log_message("%s() about to call [%s]", __FUNCTION__, command_buffer);
	return system(command_buffer);
	/*
	   char command_buffer[4096];
	   sprintf(command_buffer, "iptables -t nat -D POSTROUTING -o %s -j SNAT --to-source=%s",
	   device, wan_address);
	 */
}

static int reset_lease(chilloutd_t chillout, struct lease *l)
{
	l->username[0] = '\0';
	l->password[0] = '\0';
	l->device[0] = '\0';
	l->login_date = (time_t) 0;
	release(chillout, l);
	l->status = LEASE_PEND;
	return 0;
}

/* chillouts are created on the server for both connections, calling
 * chillout_init_from_fd upon accept
 * but, sooner or later one of the connections identifies itself
 * as the read socket of the same chillout. This function merges
 * the former 'incomplete' chillout with the one with the write
 * socket
 *
 * Here 'read' and 'write' are so from the point of view of the
 * client. The server writes in the 'read' socket.
 *
 * chillout_t and associated client destruction are the responsability
 * of the caller
 */
static int merge_chillouts(chillout_t co_read, chillout_t co_write)
{
	co_write->writesock = co_read->readsock;
	co_read->readsock = -1;
	return 0;
}

/********************************************************************
 * LOGIN/LOGOUT
 *******************************************************************/
static int login(struct chilloutd *ch, struct lease *l, char *user, char *pass)
{
	/*
	if (ch->tunnel.status == TUNNEL_DOWN) {
		log_message("%s(): Tunnel is down, reopening on first login",
			    __FUNCTION__);
	}
	*/
	if (tunnel_lease(ch, l, user, pass)) {
		log_critical("%s(): tunnel_lease", __FUNCTION__);
		return 1;
	}
	return 0;
}

static int logout(struct chilloutd *ch, struct lease *l)
{
	if (detunnel_lease(ch, l)) {
		log_critical("%s(): detunnel_lease", __FUNCTION__);
		return 1;
	}
	/*
	   if (ch->chillispot.num_auth_leases == 0) {
	   log_message("%s(): no auth leases, shutting down tunnel", __FUNCTION__);
	   if (destroy_tunnel(ch)) {
	   log_critical("%s(): error destroying tunnel", __FUNCTION__);
	   return 1;
	   }
	   }
	 */
	return 0;
}

