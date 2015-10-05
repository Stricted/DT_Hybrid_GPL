/*
 * Chillout. A lightweight RPC mechanism using POSIX Local IPC Sockets
 *
 * Copyright (C) 2007 FON Wireless Limited.
 *
 * Created: Oct 2007 Pablo Martín <pablo@fon.com>
 *
 * $Rev: 84 $
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include "chillout.h"
#include <syslog.h>
#include "log.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

static int create_and_connect_socket(char *socket_path);
static ssize_t socket_readline(int sockd, void *vptr, size_t maxlen);
static ssize_t socket_writeline(int sockd, const void *vptr, size_t n);
static struct callback *chillout_get_callback(chillout_t co, char *token);

static int create_and_connect_socket(char *socket_path)
{
	int sock, len;
	struct sockaddr_un local;

	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		log_error("could not allocate unix socket:%d",  errno);
		return -1;
	}
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, socket_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (connect(sock, (struct sockaddr *)&local, len) < 0) {
		log_error("could not connect to server:%d, path:%s", errno, socket_path);
		close(sock);
		return -1;
	}
	return sock;
}

int create_and_connect_socket_for_test(char *socket_path, long timeOutSeconds)
{
	fd_set myfds;
        struct timeval myTimeOut;

	int sock, len;
	struct sockaddr_un local;
	int flags;
	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		log_error("could not allocate unix socket");
		return -1;
	}
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, socket_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	flags = fcntl(sock, F_GETFL);
	fcntl(sock, F_SETFL, flags |= O_NONBLOCK);
	if (connect(sock, (struct sockaddr *)&local, len) < 0) {
		if (errno == EINPROGRESS) {
		/* we need to wait for connection process... It's ready when it's ready to write.*/
			FD_ZERO(&myfds);
        		FD_SET(sock, &myfds);
			myTimeOut.tv_sec = timeOutSeconds;
			myTimeOut.tv_usec = 0;
			if (select(FD_SETSIZE, 0, &myfds, 0, &myTimeOut) <= 0) {
			/*TimeOut */
				log_error("could not connect to server. TimeOut");
				FD_CLR(sock, &myfds);			
				close(sock);
				return -1;
			}
			FD_CLR(sock, &myfds);
		}
		else {
			log_error("could not connect to server, %d  :%s", errno, strerror(errno));
			close(sock);
                        return -1;
		}
	}
	fcntl(sock, F_SETFL, flags);
	return sock;
}

chillout_t chillout_init(char *socket_path, char *ident, void *data)
{
	struct chillout *ret;
	int err, i, flags;
	char query[MAX_QUERY];
	char answer[MAX_ANSWER];

	ret = (struct chillout *)malloc(sizeof(struct chillout));
	if (ret == NULL)
		return NULL;

       for(i = 0; i< 5; i++)
       {
            ret->readsock = create_and_connect_socket(socket_path);
            ret->writesock = create_and_connect_socket(socket_path);

            if(ret->readsock >=0 && ret->writesock >= 0)
            {
                log_error(" create socket succfully");
                break;

            }

            sleep(2);
       } 
       
        if ((ret->readsock < 0) || (ret->writesock < 0)) {
            log_error(" create socket failed");
            free(ret);
            return NULL;
        }
        
	ret->data = data;
	ret->date = time(NULL);
	ret->callbacks = NULL;
	if (ident == NULL)
		return ret;
	strcpy(ret->name, ident);
	/* connect, read response (id) */
	for (i = 0; i < 2; i++) {
		err = 0;
		flags = i ? CHILLOUT_READ_SOCK : CHILLOUT_WRITE_SOCK;
		sprintf(query, "hello %s %s", i ? "read" : "write", ident);
		if (chillout_write(ret, flags, query))
			err = 1;
		if ((!err) && (chillout_read(ret, flags, answer)))
			err = 1;
		if (err) {
			log_error
			    ("error calling chillout_write or chillout_read");
			close(ret->readsock);
			close(ret->writesock);
                     ret->readsock = -1;
                     ret->writesock = -1;
			free(ret);
			return NULL;
		}
		if (strncmp(query, answer, strlen(query))) {
			log_error("error in answer from chilloutd");
			close(ret->readsock);
			close(ret->writesock);
                     ret->readsock = -1;
                     ret->writesock = -1;   
			free(ret);
			return NULL;
		}
		chillout_read(ret, flags, answer);	/* "ok" */
	}
	return ret;
}

chillout_t chillout_init_for_test(char *socket_path, char *ident, void *data)
{
	struct chillout *ret;

	ret = (struct chillout *)malloc(sizeof(struct chillout));
	if (ret == NULL)
		return NULL;
	ret->readsock = create_and_connect_socket_for_test(socket_path, TESTING_TIMEOUT_IN_SECONDS);
	ret->writesock = create_and_connect_socket_for_test(socket_path, TESTING_TIMEOUT_IN_SECONDS);
	if ((ret->readsock < 0) || (ret->writesock < 0)) {
		free(ret);
		return NULL;
	}
	ret->data = data;
	ret->date = time(NULL);
	ret->callbacks = NULL;
	if (ident == NULL)
		return ret;
	strcpy(ret->name, ident);
	return ret;
}

/* Create chillout from server socket */
chillout_t chillout_init_from_fd(char *ident, int fd, void *data)
{
	struct chillout *ret;

	ret = (struct chillout *)malloc(sizeof(struct chillout));
	if (ret == NULL)
		return NULL;
	ret->readsock = fd;
	ret->writesock = -1;
	strcpy(ret->name, ident);
	ret->data = data;
	ret->date = time(NULL);
	return ret;
}

int chillout_finish(chillout_t co)
{
	if (co->readsock != -1)
		close(co->readsock);
	if (co->writesock != -1)
		close(co->writesock);
	free(co);
	return 0;
}

/*
 * This function is called by the chillout clients when
 * they detect that there is data pending to read in the
 * chillout socket. The fd they must FD_SET() before their
 * main select() loop, and the one to check if ISSET is
 * the one in chillout->sock
 */
int chillout_accept(chillout_t chillout)
{
	int ret;
	char buffer[MAX_QUERY];

	if ((ret = chillout_read(chillout, 0, buffer)))
		return ret;
	if ((ret = chillout_process(chillout, buffer)))
		return ret;
	chillout_write(chillout, 0, "ok");
	return 0;
}

int chillout_read(chillout_t co, int flags, char *buffer)
{
	ssize_t size_read;
	int socket;

	if (flags & CHILLOUT_WRITE_SOCK)
		socket = co->writesock;
	else
		socket = co->readsock;
	size_read = socket_readline(socket, (void *)buffer, (size_t) MAX_QUERY);
	if (size_read < ((ssize_t) 0))
		return CHILLOUT_ERROR;
	if (size_read == 0) {
		return CHILLOUT_DISCONNECTED;
	}
	return 0;
}

int chillout_read_for_test(chillout_t co, int flags, char *buffer, long timeOutSeconds)
{
	fd_set myfds;
	struct timeval myTimeOut;		

	ssize_t size_read;
	int socket;

	if (flags & CHILLOUT_WRITE_SOCK)
		socket = co->writesock;
	else
		socket = co->readsock;
	
	FD_ZERO(&myfds);
        FD_SET(socket, &myfds);
        myTimeOut.tv_sec = timeOutSeconds;
        myTimeOut.tv_usec = 0;
	
	size_read = 0;
	if (select(FD_SETSIZE, &myfds, 0, 0, &myTimeOut) > 0) {
		size_read = socket_readline(socket, (void *)buffer, (size_t) MAX_QUERY);
	}
	FD_CLR(socket, &myfds);
	if (size_read < ((ssize_t) 0))
		return CHILLOUT_ERROR;
	if (size_read == 0) {
		return CHILLOUT_DISCONNECTED;
	}
	return 0;
}

int chillout_write_for_test(chillout_t co, int flags, char *format, ...)
{
	ssize_t size_written;
	size_t length;
	char buffer[MAX_ANSWER];
	va_list ap;
	int sock, waitsock;

	if (co == NULL)		/* No initialized chillout */
		return 0;
	va_start(ap, format);
	vsnprintf(buffer, MAX_ANSWER - 1, format, ap);
	va_end(ap);

	length = strlen(buffer);
	sock = (flags & CHILLOUT_WRITE_SOCK) ? co->writesock : co->readsock;
	size_written = socket_writeline(sock, buffer, length);
	if (size_written != (ssize_t) length)
		return -1;
	if (flags & CHILLOUT_WAIT_FOR_ANSWER) {
                waitsock =
                    (flags & CHILLOUT_READ_SOCK) ? 0 : CHILLOUT_WRITE_SOCK;
                if (chillout_read_for_test(co, waitsock, buffer, TESTING_TIMEOUT_IN_SECONDS))
                        return -1;
                if (!strncmp(buffer, "ok", 2))
                        return 0;
                else if (!strncmp(buffer, "error", 5))
                        return 1;
                else {
                        if (buffer[strlen(buffer) - 1] == '\n')
                                buffer[strlen(buffer) - 1] = '\0';
                        log_critical("%s: answer?: %s", __FUNCTION__, buffer);
                        return -1;
                }
        }

	return 0;
}

/* 
 * write asyncronously to the chillout.
 * if wait_for_answer is 1, it waits until either 'ok' or 'error'
 * is returned
 */
int chillout_write(chillout_t co, int flags, char *format, ...)
{
	ssize_t size_written;
	size_t length;
	char buffer[MAX_ANSWER];
	va_list ap;
	int sock, waitsock;

	if (co == NULL)		/* No initialized chillout */
		return 0;

	va_start(ap, format);
	vsnprintf(buffer, MAX_ANSWER - 1, format, ap);
	va_end(ap);

	length = strlen(buffer);
	/* 
	 * Write is done by default on OUR read socket (the client write socket)
	 * There is a need to do it in the write socket, tough, while responding
	 * to the original hello read <client> call
	 */
	sock = (flags & CHILLOUT_WRITE_SOCK) ? co->writesock : co->readsock;
	size_written = socket_writeline(sock, buffer, length);
	if (size_written != (ssize_t) length)
		return -1;
	if (flags & CHILLOUT_WAIT_FOR_ANSWER) {
		waitsock =
		    (flags & CHILLOUT_READ_SOCK) ? 0 : CHILLOUT_WRITE_SOCK;
		if (chillout_read(co, waitsock, buffer))
			return -1;
		if (!strncmp(buffer, "ok", 2))
			return 0;
		else if (!strncmp(buffer, "error", 5))
			return 1;
		else {
			if (buffer[strlen(buffer) - 1] == '\n')
				buffer[strlen(buffer) - 1] = '\0';
			log_critical("%s: answer?: %s", __FUNCTION__, buffer);
			return -1;
		}
	}
	return 0;
}

/* So plagiarised from the "printed library" Stevens */
static ssize_t socket_readline(int sockd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *buffer;

	buffer = vptr;

	for (n = 1; n < maxlen; n++) {
		if ((rc = read(sockd, &c, 1)) == 1) {
			*buffer++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return 0;
			else
				break;
		} else {
			if (errno == EINTR)
				continue;
			return -1;
		}
	}
	*buffer = 0;
	return n;
}

static ssize_t socket_writeline(int sockd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *buffer;
	int append_eol = 0;

	buffer = vptr;
	nleft = n;
	if (buffer[n - 1] != '\n')
		append_eol = 1;
	while (nleft > 0) {
		if ((nwritten = write(sockd, buffer, nleft)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		buffer += nwritten;
	}
	if (append_eol) {
		while (1) {
			if (write(sockd, "\n", 1) <= 0) {
				if (errno == EINTR)
					continue;
				else
					return -1;
			} else {
				break;
			}
		}
	}
	return n;
}

/* it all boils down to a function that receives both the
 * query and does some chillout_write()'s */
int chillout_process(chillout_t co, char *query)
{
	struct callback *c;
	char *vector[MAX_TOKENS];
	int num_tokens;

	if (query[strlen(query) - 1] == '\n')
		query[strlen(query) - 1] = '\0';
	log_message("%s(): [%s]", __FUNCTION__, query);
	if (chillout_string_to_vector(vector, query, &num_tokens, MAX_TOKENS)) {
		log_error("%s(): error parsing string", __FUNCTION__);
		chillout_write(co, 0, "error parsing string");
		return CHILLOUT_ERROR;
	}
	if ((c = chillout_get_callback(co, vector[0])) == NULL) {
		log_error("%s(): no defaultcb for \"%s\"", __FUNCTION__,
			  co->name);
		chillout_write(co, 0, "error retrieving callback");
		return CHILLOUT_UNKNOWN;
	}
	return c->function(co, co->data, num_tokens, vector);
}

int chillout_string_to_vector(char **vector, char *line, int *num_tokens,
			      int max_tokens)
{
	char *p;
	char **v;

	(*num_tokens) = 0;
	for (p = line, v = vector, *v = p; *p; p++) {
		if (*p == ' ') {
			if (*v == p) {
				p++;
				continue;
			}
			if ((*num_tokens) == max_tokens)
				return 1;
			(*num_tokens)++;
			v++;
			*v = (p + 1);
			*p = '\0';
		}
	}
	(*num_tokens)++;
	return 0;
}

static struct callback *chillout_get_callback(chillout_t chillout, char *token)
{
	int i;
	struct callback *node;
	struct callback *defnode;
	struct callback *allnode;

	defnode = allnode = NULL;
	if (chillout->callbacks == NULL) {
		log_message("%s:%d:%s: chillout->callbacks is NULL", __FILE__,
			    __LINE__, __FUNCTION__);
		return NULL;
	}
	for (i = 0; chillout->callbacks[i].keyword[0] != '\0'; i++) {
		node = &(chillout->callbacks[i]);
		if (strcmp(node->ident, chillout->name)) {
			if (strcmp(node->ident, "all") &&
			    !strcmp(node->keyword, token)) {
				allnode = node;
			} else {
				continue;
			}
		}
		if (!strcmp(node->keyword, "default")) {
			defnode = node;
		} else if (!strcmp(node->keyword, token)) {
			return node;
		}
		if (allnode)
			return allnode;
	}
	return defnode;
}

