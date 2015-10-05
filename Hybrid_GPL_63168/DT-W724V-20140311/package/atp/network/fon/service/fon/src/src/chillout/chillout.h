/*
 * Chillout. A lightweight RPC mechanism using POSIX Local IPC Sockets
 *
 * Copyright (C) 2007 FON Wireless Limited.
 *
 * Created: Oct. 2007. Pablo Martín <pablo@fon.com>
 *
 * $Rev: 84 $
 */
#ifndef __CHILLOUT_H__
#define __CHILLOUT_H__

/* hardcoded protocol limits */
#define MAX_NAME 32		/* size of a chillout name */
#define MAX_TOKENS 32		/* maximum number of tokens in a line */
#define MAX_KEYWORD 32		/* size of a keyword */
#define MAX_QUERY 1024		/* max size of a query line */
#define MAX_ANSWER 1024		/* max size of an answer line */

/* return values for chillout_write */
#define CHILLOUT_ERROR (-1)
#define CHILLOUT_DISCONNECTED (-2)
#define CHILLOUT_UNKNOWN (-3)
#define CHILLOUT_NOOK (-4)

/* flags for chillout_read and chillout_write */
#define CHILLOUT_NONE 0
#define CHILLOUT_WAIT_FOR_ANSWER 1
#define CHILLOUT_READ_SOCK 2
#define CHILLOUT_WRITE_SOCK 4

/* Testing */
#define TESTING_TIMEOUT_IN_SECONDS 15

struct chillout {
	/* int sock;                    file descriptor for the socket */
	int readsock;
	int writesock;
	/* each chillout comprises two sockets, so as to not receive
	 * interspersed ack from written commands mixed with asyncronous
	 * queries
	 */
	void *data;		/* data passed to callbacks */
	struct callback *callbacks;	/* callbacks array */
	char name[MAX_NAME];	/* chillout name */
	time_t date;		/* connection time */
};

typedef struct chillout *chillout_t;

typedef int (chillout_callback) (chillout_t co, void *data, int argc, char **argv);

struct callback {
	char ident[MAX_NAME];	/* name of the client */
	char keyword[MAX_KEYWORD];	/* first word on the line */
	int hash_keyword;	/* hash of the first word on the line */
	chillout_callback *function;	/* callback function */
};

chillout_t chillout_init(char *path, char *indent, void *data);
chillout_t chillout_init_from_fd(char *ident, int fd, void *data);
int chillout_register_callbacks(chillout_t co, struct callback *callbacks_array);
int chillout_finish(chillout_t co);
int chillout_accept(chillout_t co);
int chillout_write(chillout_t co, int flags, char *format, ...);
int chillout_read(chillout_t co, int flags, char *buffer);
int chillout_process(chillout_t co, char *query);
int chillout_string_to_vector(char **vector, char *line, int *num_tokens, int max_tokens);
chillout_t chillout_init_for_test(char *path, char *indent, void *data);
int chillout_write_for_test(chillout_t co, int flags, char *format, ...);
int chillout_read_for_test(chillout_t co, int flags, char *buffer, long timeOutSeconds);
int create_and_connect_socket_for_test(char *socket_path, long timeOutSeconds);
/* the "hello callback" should be appended at the end of callbacks lists */
#endif
