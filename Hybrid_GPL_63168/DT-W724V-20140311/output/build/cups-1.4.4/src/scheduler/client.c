/*
 * "$Id: client.c 9120 2010-04-23 18:56:34Z mike $"
 *
 *   Client routines for the Common UNIX Printing System (CUPS) scheduler.
 *
 *   Copyright 2007-2010 by Apple Inc.
 *   Copyright 1997-2007 by Easy Software Products, all rights reserved.
 *
 *   This file contains Kerberos support code, copyright 2006 by
 *   Jelmer Vernooij.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 * Contents:
 *
 *   cupsdAcceptClient()    - Accept a new client.
 *   cupsdCloseAllClients() - Close all remote clients immediately.
 *   cupsdCloseClient()     - Close a remote client.
 *   cupsdFlushHeader()     - Flush the header fields to the client.
 *   cupsdReadClient()      - Read data from a client.
 *   cupsdSendCommand()     - Send output from a command via HTTP.
 *   cupsdSendError()       - Send an error message via HTTP.
 *   cupsdSendHeader()      - Send an HTTP request.
 *   cupsdUpdateCGI()       - Read status messages from CGI scripts and
 *                            programs.
 *   cupsdWriteClient()     - Write data to a client as needed.
 *   check_if_modified()    - Decode an "If-Modified-Since" line.
 *   compare_clients()      - Compare two client connections.
 *   data_ready()           - Check whether data is available from a client.
 *   encrypt_client()       - Enable encryption for the client...
 *   get_cdsa_certificate() - Get a SSL/TLS certificate from the System
 *                            keychain.
 *   get_file()             - Get a filename and state info.
 *   install_conf_file()    - Install a configuration file.
 *   is_cgi()               - Is the resource a CGI script/program?
 *   is_path_absolute()     - Is a path absolute and free of relative elements
 *                            (i.e. "..").
 *   make_certificate()     - Make a self-signed SSL/TLS certificate.
 *   pipe_command()         - Pipe the output of a command to the remote client.
 *   valid_host()           - Is the Host: field valid?
 *   write_file()           - Send a file via HTTP.
 *   write_pipe()           - Flag that data is available on the CGI pipe.
 */

/*
 * Include necessary headers...
 */

#include "cupsd.h"

#ifdef HAVE_CDSASSL
#  include <Security/Security.h>
#  ifdef HAVE_SECIDENTITYSEARCHPRIV_H
#    include <Security/SecIdentitySearchPriv.h>
#  else /* Declare prototype for function in that header... */
extern OSStatus SecIdentitySearchCreateWithPolicy(SecPolicyRef policy, 
				CFStringRef idString, CSSM_KEYUSE keyUsage, 
				CFTypeRef keychainOrArray, 
				Boolean returnOnlyValidIdentities, 
				SecIdentitySearchRef* searchRef);
#  endif /* HAVE_SECIDENTITYSEARCHPRIV_H */
#  ifdef HAVE_SECPOLICYPRIV_H
#    include <Security/SecPolicyPriv.h>
#  else /* Declare prototype for function in that header... */
extern OSStatus SecPolicySetValue(SecPolicyRef policyRef, 
                                  const CSSM_DATA *value);
#  endif /* HAVE_SECPOLICYPRIV_H */
#  ifdef HAVE_SECBASEPRIV_H
#    include <Security/SecBasePriv.h>
#  else /* Declare prototype for function in that header... */
extern const char *cssmErrorString(int error);
#  endif /* HAVE_SECBASEPRIV_H */
#endif /* HAVE_CDSASSL */

#ifdef HAVE_GNUTLS
#  include <gnutls/x509.h>
#endif /* HAVE_GNUTLS */

#ifdef HAVE_TCPD_H
#  include <tcpd.h>
#endif /* HAVE_TCPD_H */


/*
 * Local functions...
 */

static int		check_if_modified(cupsd_client_t *con,
			                  struct stat *filestats);
static int		compare_clients(cupsd_client_t *a, cupsd_client_t *b,
			                void *data);
static int		data_ready(cupsd_client_t *con);
#ifdef HAVE_SSL
static int		encrypt_client(cupsd_client_t *con);
#endif /* HAVE_SSL */
#ifdef HAVE_CDSASSL
static CFArrayRef	get_cdsa_certificate(cupsd_client_t *con);
#endif /* HAVE_CDSASSL */
static char		*get_file(cupsd_client_t *con, struct stat *filestats,
			          char *filename, int len);
static http_status_t	install_conf_file(cupsd_client_t *con);
static int		is_cgi(cupsd_client_t *con, const char *filename,
		               struct stat *filestats, mime_type_t *type);
static int		is_path_absolute(const char *path);
#ifdef HAVE_SSL
static int		make_certificate(cupsd_client_t *con);
#endif /* HAVE_SSL */
static int		pipe_command(cupsd_client_t *con, int infile, int *outfile,
			             char *command, char *options, int root);
static int		valid_host(cupsd_client_t *con);
static int		write_file(cupsd_client_t *con, http_status_t code,
		        	   char *filename, char *type,
				   struct stat *filestats);
static void		write_pipe(cupsd_client_t *con);


/*
 * 'cupsdAcceptClient()' - Accept a new client.
 */

void
cupsdAcceptClient(cupsd_listener_t *lis)/* I - Listener socket */
{
  int			count;		/* Count of connections on a host */
  int			val;		/* Parameter value */
  cupsd_client_t	*con,		/* New client pointer */
			*tempcon;	/* Temporary client pointer */
  http_addrlist_t	*addrlist,	/* List of adddresses for host */
			*addr;		/* Current address */
  socklen_t		addrlen;	/* Length of address */
  char			*hostname;	/* Hostname for address */
  http_addr_t		temp;		/* Temporary address variable */
  static time_t		last_dos = 0;	/* Time of last DoS attack */
#ifdef HAVE_TCPD_H
  struct request_info	wrap_req;	/* TCP wrappers request information */
#endif /* HAVE_TCPD_H */


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cupsdAcceptClient(lis=%p(%d)) Clients=%d",
                  lis, lis->fd, cupsArrayCount(Clients));

 /*
  * Make sure we don't have a full set of clients already...
  */

  if (cupsArrayCount(Clients) == MaxClients)
    return;

 /*
  * Get a pointer to the next available client...
  */

  if (!Clients)
    Clients = cupsArrayNew(NULL, NULL);

  if (!Clients)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to allocate memory for clients array!");
    cupsdPauseListening();
    return;
  }

  if (!ActiveClients)
    ActiveClients = cupsArrayNew((cups_array_func_t)compare_clients, NULL);

  if (!ActiveClients)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to allocate memory for active clients array!");
    cupsdPauseListening();
    return;
  }

  if ((con = calloc(1, sizeof(cupsd_client_t))) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to allocate memory for client!");
    cupsdPauseListening();
    return;
  }

  con->http.activity = time(NULL);
  con->file          = -1;
  con->http.hostaddr = &(con->clientaddr);

 /*
  * Accept the client and get the remote address...
  */

  addrlen = sizeof(http_addr_t);

  if ((con->http.fd = accept(lis->fd, (struct sockaddr *)con->http.hostaddr,
                             &addrlen)) < 0)
  {
    if (errno == ENFILE || errno == EMFILE)
      cupsdPauseListening();

    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to accept client connection - %s.",
                    strerror(errno));
    free(con);

    return;
  }

#ifdef AF_INET6
  if (lis->address.addr.sa_family == AF_INET6)
  {
   /*
    * Save the connected port number...
    */

    con->http.hostaddr->ipv6.sin6_port = lis->address.ipv6.sin6_port;

   /*
    * Convert IPv4 over IPv6 addresses (::ffff:n.n.n.n) to IPv4 forms we
    * can more easily use...
    */

    if (con->http.hostaddr->ipv6.sin6_addr.s6_addr32[0] == 0 &&
        con->http.hostaddr->ipv6.sin6_addr.s6_addr32[1] == 0 &&
        ntohl(con->http.hostaddr->ipv6.sin6_addr.s6_addr32[2]) == 0xffff)
      con->http.hostaddr->ipv6.sin6_addr.s6_addr32[2] = 0;
  }
  else
#endif /* AF_INET6 */
  if (lis->address.addr.sa_family == AF_INET)
    con->http.hostaddr->ipv4.sin_port = lis->address.ipv4.sin_port;

 /*
  * Check the number of clients on the same address...
  */

  for (count = 0, tempcon = (cupsd_client_t *)cupsArrayFirst(Clients);
       tempcon;
       tempcon = (cupsd_client_t *)cupsArrayNext(Clients))
    if (httpAddrEqual(tempcon->http.hostaddr, con->http.hostaddr))
    {
      count ++;
      if (count >= MaxClientsPerHost)
	break;
    }

  if (count >= MaxClientsPerHost)
  {
    if ((time(NULL) - last_dos) >= 60)
    {
      last_dos = time(NULL);
      cupsdLogMessage(CUPSD_LOG_WARN,
                      "Possible DoS attack - more than %d clients connecting "
		      "from %s!",
	              MaxClientsPerHost,
		      httpAddrString(con->http.hostaddr, con->http.hostname,
		                     sizeof(con->http.hostname)));
    }

#ifdef WIN32
    closesocket(con->http.fd);
#else
    close(con->http.fd);
#endif /* WIN32 */

    free(con);
    return;
  }

 /*
  * Get the hostname or format the IP address as needed...
  */

  if (httpAddrLocalhost(con->http.hostaddr))
  {
   /*
    * Map accesses from the loopback interface to "localhost"...
    */

    strlcpy(con->http.hostname, "localhost", sizeof(con->http.hostname));
    hostname = con->http.hostname;
  }
  else
  {
   /*
    * Map accesses from the same host to the server name.
    */

    if (HostNameLookups)
      hostname = httpAddrLookup(con->http.hostaddr, con->http.hostname,
                                sizeof(con->http.hostname));
    else
    {
      hostname = NULL;
      httpAddrString(con->http.hostaddr, con->http.hostname,
                     sizeof(con->http.hostname));
    }
  }

  if (hostname == NULL && HostNameLookups == 2)
  {
   /*
    * Can't have an unresolved IP address with double-lookups enabled...
    */

#ifdef WIN32
    closesocket(con->http.fd);
#else
    close(con->http.fd);
#endif /* WIN32 */

    cupsdLogMessage(CUPSD_LOG_WARN,
                    "Name lookup failed - connection from %s closed!",
                    con->http.hostname);

    free(con);
    return;
  }

  if (HostNameLookups == 2)
  {
   /*
    * Do double lookups as needed...
    */

    if ((addrlist = httpAddrGetList(con->http.hostname, AF_UNSPEC, NULL))
            != NULL)
    {
     /*
      * See if the hostname maps to the same IP address...
      */

      for (addr = addrlist; addr; addr = addr->next)
        if (httpAddrEqual(con->http.hostaddr, &(addr->addr)))
          break;
    }
    else
      addr = NULL;

    httpAddrFreeList(addrlist);

    if (!addr)
    {
     /*
      * Can't have a hostname that doesn't resolve to the same IP address
      * with double-lookups enabled...
      */

#ifdef WIN32
      closesocket(con->http.fd);
#else
      close(con->http.fd);
#endif /* WIN32 */

      cupsdLogMessage(CUPSD_LOG_WARN,
                      "IP lookup failed - connection from %s closed!",
                      con->http.hostname);
      free(con);
      return;
    }
  }

#ifdef HAVE_TCPD_H
 /*
  * See if the connection is denied by TCP wrappers...
  */

  request_init(&wrap_req, RQ_DAEMON, "cupsd", RQ_FILE, con->http.fd, NULL);
  fromhost(&wrap_req);

  if (!hosts_access(&wrap_req))
  {
#ifdef WIN32
    closesocket(con->http.fd);
#else
    close(con->http.fd);
#endif /* WIN32 */

    cupsdLogMessage(CUPSD_LOG_WARN,
                    "Connection from %s refused by /etc/hosts.allow and "
		    "/etc/hosts.deny rules.", con->http.hostname);
    free(con);
    return;
  }
#endif /* HAVE_TCPD_H */

#ifdef AF_INET6
  if (con->http.hostaddr->addr.sa_family == AF_INET6)
    cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdAcceptClient: %d from %s:%d (IPv6)",
                    con->http.fd, con->http.hostname,
		    ntohs(con->http.hostaddr->ipv6.sin6_port));
  else
#endif /* AF_INET6 */
#ifdef AF_LOCAL
  if (con->http.hostaddr->addr.sa_family == AF_LOCAL)
    cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdAcceptClient: %d from %s (Domain)",
                    con->http.fd, con->http.hostname);
  else
#endif /* AF_LOCAL */
  cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdAcceptClient: %d from %s:%d (IPv4)",
                  con->http.fd, con->http.hostname,
		  ntohs(con->http.hostaddr->ipv4.sin_port));

 /*
  * Get the local address the client connected to...
  */

  addrlen = sizeof(temp);
  if (getsockname(con->http.fd, (struct sockaddr *)&temp, &addrlen))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to get local address - %s",
                    strerror(errno));

    strcpy(con->servername, "localhost");
    con->serverport = LocalPort;
  }
  else
  {
#ifdef AF_INET6
    if (temp.addr.sa_family == AF_INET6)
    {
      if (httpAddrLocalhost(&temp))
        strlcpy(con->servername, "localhost", sizeof(con->servername));
      else if (HostNameLookups || RemotePort)
        httpAddrLookup(&temp, con->servername, sizeof(con->servername));
      else
        httpAddrString(&temp, con->servername, sizeof(con->servername));

      con->serverport = ntohs(lis->address.ipv6.sin6_port);
    }
    else
#endif /* AF_INET6 */
    if (temp.addr.sa_family == AF_INET)
    {
      if (httpAddrLocalhost(&temp))
        strlcpy(con->servername, "localhost", sizeof(con->servername));
      else if (HostNameLookups || RemotePort)
        httpAddrLookup(&temp, con->servername, sizeof(con->servername));
      else
        httpAddrString(&temp, con->servername, sizeof(con->servername));

      con->serverport = ntohs(lis->address.ipv4.sin_port);
    }
    else
    {
      strcpy(con->servername, "localhost");
      con->serverport = LocalPort;
    }
  }

  cupsArrayAdd(Clients, con);

 /*
  * Using TCP_NODELAY improves responsiveness, especially on systems with a slow
  * loopback interface.  Since we write large buffers when sending print files
  * and requests there shouldn't be any performance penalty for this...
  */

  val = 1;
  setsockopt(con->http.fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));

 /*
  * Close this file on all execs...
  */

  fcntl(con->http.fd, F_SETFD, fcntl(con->http.fd, F_GETFD) | FD_CLOEXEC);

 /*
  * Add the socket to the server select.
  */

  cupsdAddSelect(con->http.fd, (cupsd_selfunc_t)cupsdReadClient, NULL, con);

 /*
  * Temporarily suspend accept()'s until we lose a client...
  */

  if (cupsArrayCount(Clients) == MaxClients)
    cupsdPauseListening();

#ifdef HAVE_SSL
 /*
  * See if we are connecting on a secure port...
  */

  if (lis->encryption == HTTP_ENCRYPT_ALWAYS)
  {
   /*
    * https connection; go secure...
    */

    con->http.encryption = HTTP_ENCRYPT_ALWAYS;

    if (!encrypt_client(con))
      cupsdCloseClient(con);
  }
  else
    con->auto_ssl = 1;
#endif /* HAVE_SSL */
}


/*
 * 'cupsdCloseAllClients()' - Close all remote clients immediately.
 */

void
cupsdCloseAllClients(void)
{
  cupsd_client_t	*con;		/* Current client */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cupsdCloseAllClients() Clients=%d",
                  cupsArrayCount(Clients));

  for (con = (cupsd_client_t *)cupsArrayFirst(Clients);
       con;
       con = (cupsd_client_t *)cupsArrayNext(Clients))
    if (cupsdCloseClient(con))
      cupsdCloseClient(con);
}


/*
 * 'cupsdCloseClient()' - Close a remote client.
 */

int					/* O - 1 if partial close, 0 if fully closed */
cupsdCloseClient(cupsd_client_t *con)	/* I - Client to close */
{
  int		partial;		/* Do partial close for SSL? */
#ifdef HAVE_LIBSSL
  SSL_CTX	*context;		/* Context for encryption */
  SSL		*conn;			/* Connection for encryption */
  unsigned long	error;			/* Error code */
#elif defined(HAVE_GNUTLS)
  http_tls_t	*conn;			/* TLS connection information */
  int		error;			/* Error code */
  gnutls_certificate_server_credentials *credentials;
					/* TLS credentials */
#  elif defined(HAVE_CDSASSL)
  http_tls_t	*conn;			/* CDSA connection information */
#endif /* HAVE_LIBSSL */


  cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdCloseClient: %d", con->http.fd);

 /*
  * Flush pending writes before closing...
  */

  httpFlushWrite(HTTP(con));

  partial = 0;

#ifdef HAVE_SSL
 /*
  * Shutdown encryption as needed...
  */

  if (con->http.tls)
  {
    partial = 1;

#  ifdef HAVE_LIBSSL
    conn    = (SSL *)(con->http.tls);
    context = SSL_get_SSL_CTX(conn);

    switch (SSL_shutdown(conn))
    {
      case 1 :
          cupsdLogMessage(CUPSD_LOG_DEBUG,
	                  "SSL shutdown successful!");
	  break;
      case -1 :
          cupsdLogMessage(CUPSD_LOG_ERROR,
	                  "Fatal error during SSL shutdown!");
      default :
	  while ((error = ERR_get_error()) != 0)
	    cupsdLogMessage(CUPSD_LOG_ERROR, "SSL shutdown failed: %s",
	                    ERR_error_string(error, NULL));
          break;
    }

    SSL_CTX_free(context);
    SSL_free(conn);

#  elif defined(HAVE_GNUTLS)
    conn        = (http_tls_t *)(con->http.tls);
    credentials = (gnutls_certificate_server_credentials *)(conn->credentials);

    error = gnutls_bye(conn->session, GNUTLS_SHUT_WR);
    switch (error)
    {
      case GNUTLS_E_SUCCESS:
	cupsdLogMessage(CUPSD_LOG_DEBUG,
	                "SSL shutdown successful!");
	break;
      default:
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "SSL shutdown failed: %s", gnutls_strerror(error));
	break;
    }

    gnutls_deinit(conn->session);
    gnutls_certificate_free_credentials(*credentials);
    free(credentials);
    free(conn);

#  elif defined(HAVE_CDSASSL)
    conn = (http_tls_t *)(con->http.tls);

    while (SSLClose(conn->session) == errSSLWouldBlock)
      usleep(1000);

    SSLDisposeContext(conn->session);

    if (conn->certsArray)
      CFRelease(conn->certsArray);

    free(conn);
#  endif /* HAVE_LIBSSL */

    con->http.tls = NULL;
  }
#endif /* HAVE_SSL */

  if (con->pipe_pid != 0)
  {
   /*
    * Stop any CGI process...
    */

    cupsdEndProcess(con->pipe_pid, 1);
    con->pipe_pid = 0;
  }

  if (con->file >= 0)
  {
    cupsdRemoveSelect(con->file);

    close(con->file);
    con->file = -1;
  }

 /*
  * Close the socket and clear the file from the input set for select()...
  */

  if (con->http.fd >= 0)
  {
    cupsArrayRemove(ActiveClients, con);
    cupsdSetBusyState();

    if (partial)
    {
     /*
      * Only do a partial close so that the encrypted client gets everything.
      */

      shutdown(con->http.fd, 0);
      cupsdAddSelect(con->http.fd, (cupsd_selfunc_t)cupsdReadClient, NULL, con);
    }
    else
    {
     /*
      * Shut the socket down fully...
      */

      cupsdRemoveSelect(con->http.fd);
      close(con->http.fd);
      con->http.fd = -1;
    }
  }

  if (!partial)
  {
   /*
    * Free memory...
    */

    if (con->http.input_set)
      free(con->http.input_set);

    httpClearCookie(HTTP(con));
    httpClearFields(HTTP(con));

    cupsdClearString(&con->filename);
    cupsdClearString(&con->command);
    cupsdClearString(&con->options);
    cupsdClearString(&con->query_string);

    if (con->request)
    {
      ippDelete(con->request);
      con->request = NULL;
    }

    if (con->response)
    {
      ippDelete(con->response);
      con->response = NULL;
    }

    if (con->language)
    {
      cupsLangFree(con->language);
      con->language = NULL;
    }

#ifdef HAVE_AUTHORIZATION_H
    if (con->authref)
    {
      AuthorizationFree(con->authref, kAuthorizationFlagDefaults);
      con->authref = NULL;
    }
#endif /* HAVE_AUTHORIZATION_H */

   /*
    * Re-enable new client connections if we are going back under the
    * limit...
    */

    if (cupsArrayCount(Clients) == MaxClients)
      cupsdResumeListening();

   /*
    * Compact the list of clients as necessary...
    */

    cupsArrayRemove(Clients, con);

    free(con);
  }

  return (partial);
}


/*
 * 'cupsdFlushHeader()' - Flush the header fields to the client.
 */

int					/* I - Bytes written or -1 on error */
cupsdFlushHeader(cupsd_client_t *con)	/* I - Client to flush to */
{
  int bytes = httpFlushWrite(HTTP(con));

  con->http.data_encoding = HTTP_ENCODE_LENGTH;

  return (bytes);
}


/*
 * 'cupsdReadClient()' - Read data from a client.
 */

void
cupsdReadClient(cupsd_client_t *con)	/* I - Client to read from */
{
  char			line[32768],	/* Line from client... 32768*/
			operation[64],	/* Operation code from socket */
			version[64],	/* HTTP version number string */
			locale[64],	/* Locale */
			*ptr;		/* Pointer into strings */
  int			major, minor;	/* HTTP version numbers */
  http_status_t		status;		/* Transfer status */
  ipp_state_t		ipp_state;	/* State of IPP transfer */
  int			bytes;		/* Number of bytes to POST */
  char			*filename;	/* Name of file for GET/HEAD */
  char			buf[1024];	/* Buffer for real filename */
  struct stat		filestats;	/* File information */
  mime_type_t		*type;		/* MIME type of file */
  cupsd_printer_t	*p;		/* Printer */
  static unsigned	request_id = 0;	/* Request ID for temp files */
#ifdef SUPPORT_ATP_SAMBA_PRINT_SIGLE_CACHFILE
  int         bfilenamefound = 0;
  char         filenamein[32] = {0};
#endif
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  status = HTTP_CONTINUE;

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
		  "cupsdReadClient(con=%p(%d)) "
		  "con->http.error=%d "
		  "con->http.used=%d, "
		  "con->http.state=%d "
		  "con->data_encoding=HTTP_ENCODE_%s, "
		  "con->data_remaining=" CUPS_LLFMT ", "
		  "con->file=%d",
		  con, con->http.fd, con->http.error, con->http.used,
		  con->http.state,
		  con->http.data_encoding == HTTP_ENCODE_CHUNKED ?
		      "CHUNKED" : "LENGTH",
		  CUPS_LLCAST con->http.data_remaining, con->file);

#ifdef HAVE_SSL
  if (con->auto_ssl)
  {
   /*
    * Automatically check for a SSL/TLS handshake...
    */

    con->auto_ssl = 0;

    if (recv(con->http.fd, buf, 1, MSG_PEEK) == 1 &&
        (!buf[0] || !strchr("DGHOPT", buf[0])))
    {
     /*
      * Encrypt this connection...
      */

      cupsdLogMessage(CUPSD_LOG_DEBUG2,
                      "cupsdReadClient: Saw first byte %02X, auto-negotiating "
		      "SSL/TLS session...", buf[0] & 255);

      if (!encrypt_client(con))
        cupsdCloseClient(con);

      return;
    }
  }
#endif /* HAVE_SSL */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

  switch (con->http.state)
  {
    case HTTP_WAITING :
       /*
        * See if we've received a request line...
	*/
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

        if (httpGets(line, sizeof(line) - 1, HTTP(con)) == NULL)
	{
	  if (con->http.error && con->http.error != EPIPE)
	  	{
	    cupsdLogMessage(CUPSD_LOG_DEBUG,
			    "cupsdReadClient: %d WAITING Closing for error %d "
			    "(%s)", con->http.fd, con->http.error,
			    strerror(con->http.error));
//	printf("pail=child cups==file:%s,line:%d=ERRRRRRRRRRRRRRRRRR--httpGets---http.error \n",__FILE__,__LINE__);

	  	}
	  else
	  	{
	    cupsdLogMessage(CUPSD_LOG_DEBUG,
	                    "cupsdReadClient: %d WAITING Closing on EOF",
			    con->http.fd);
//printf("pail=child cups==file:%s,line:%d=ERRRRRRRRRRRRRRRRRR--httpGets---EOF\n",__FILE__,__LINE__);
	  	}
	  cupsdCloseClient(con);
	  return;
	}
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

       /*
        * Ignore blank request lines...
	*/

        if (line[0] == '\0')
	  break;
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

       /*
        * Clear other state variables...
	*/

        httpClearFields(HTTP(con));

        con->http.activity        = time(NULL);
        con->http.version         = HTTP_1_0;
	con->http.keep_alive      = HTTP_KEEPALIVE_OFF;
	con->http.data_encoding   = HTTP_ENCODE_LENGTH;
	con->http.data_remaining  = 0;
	con->http._data_remaining = 0;
	con->operation            = HTTP_WAITING;
	con->bytes                = 0;
	con->file                 = -1;
	con->file_ready           = 0;
	con->pipe_pid             = 0;
	con->username[0]          = '\0';
	con->password[0]          = '\0';
	con->uri[0]               = '\0';

	cupsdClearString(&con->command);
	cupsdClearString(&con->options);
	cupsdClearString(&con->query_string);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

	if (con->request)
	{
	  ippDelete(con->request);
	  con->request = NULL;
	}

	if (con->response)
	{
	  ippDelete(con->response);
	  con->response = NULL;
	}

	if (con->language)
	{
	  cupsLangFree(con->language);
	  con->language = NULL;
	}

#ifdef HAVE_GSSAPI
        con->have_gss = 0;

	if (con->gss_creds)
	{
	  OM_uint32 minor_status;
	  gss_release_cred(&minor_status, &con->gss_creds);
	}
#endif /* HAVE_GSSAPI */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

       /*
        * Grab the request line...
	*/

        switch (sscanf(line, "%63s%1023s%63s", operation, con->uri, version))
	{
	  case 1 :
	      if (line[0])
	      {
		cupsdLogMessage(CUPSD_LOG_ERROR,
				"Bad request line \"%s\" from %s!", line,
				con->http.hostname);
		cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE);
		cupsdCloseClient(con);
              }
	      return;
	  case 2 :
	      con->http.version = HTTP_0_9;
	      break;
	  case 3 :
	      if (sscanf(version, "HTTP/%d.%d", &major, &minor) != 2)
	      {
		cupsdLogMessage(CUPSD_LOG_ERROR,
		                "Bad request line \"%s\" from %s!", line,
	                        con->http.hostname);
		cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE);
		cupsdCloseClient(con);
		return;
	      }

	      if (major < 2)
	      {
	        con->http.version = (http_version_t)(major * 100 + minor);
		if (con->http.version == HTTP_1_1 && KeepAlive)
		  con->http.keep_alive = HTTP_KEEPALIVE_ON;
		else
		  con->http.keep_alive = HTTP_KEEPALIVE_OFF;
	      }
	      else
	      {
		cupsdLogMessage(CUPSD_LOG_ERROR,
		                "Unsupported request line \"%s\" from %s!",
				line, con->http.hostname);
	        cupsdSendError(con, HTTP_NOT_SUPPORTED, CUPSD_AUTH_NONE);
		cupsdCloseClient(con);
		return;
	      }
	      break;
	}
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

       /*
        * Handle full URLs in the request line...
	*/

        if (strcmp(con->uri, "*"))
	{
	  char	scheme[HTTP_MAX_URI],	/* Method/scheme */
		userpass[HTTP_MAX_URI],	/* Username:password */
		hostname[HTTP_MAX_URI],	/* Hostname */
		resource[HTTP_MAX_URI];	/* Resource path */
          int	port;			/* Port number */


         /*
	  * Separate the URI into its components...
	  */

          httpSeparateURI(HTTP_URI_CODING_MOST, con->uri,
	                  scheme, sizeof(scheme),
	                  userpass, sizeof(userpass),
			  hostname, sizeof(hostname), &port,
			  resource, sizeof(resource));

         /*
	  * Only allow URIs with the servername, localhost, or an IP
	  * address...
	  */

	  if (strcmp(scheme, "file") &&
	      strcasecmp(hostname, ServerName) &&
	      strcasecmp(hostname, "localhost") &&
	      !isdigit(hostname[0]) && hostname[0] != '[')
	  {
	   /*
	    * Nope, we don't do proxies...
	    */

	    cupsdLogMessage(CUPSD_LOG_ERROR, "Bad URI \"%s\" in request!",
	                    con->uri);
	    cupsdSendError(con, HTTP_METHOD_NOT_ALLOWED, CUPSD_AUTH_NONE);
	    cupsdCloseClient(con);
	    return;
	  }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);

         /*
	  * Copy the resource portion back into the URI; both resource and
	  * con->uri are HTTP_MAX_URI bytes in size...
	  */

          strcpy(con->uri, resource);
	}

       /*
        * Process the request...
	*/

        if (!strcmp(operation, "GET"))
	  con->http.state = HTTP_GET;
        else if (!strcmp(operation, "PUT"))
	  con->http.state = HTTP_PUT;
        else if (!strcmp(operation, "POST"))
	  con->http.state = HTTP_POST;
        else if (!strcmp(operation, "DELETE"))
	  con->http.state = HTTP_DELETE;
        else if (!strcmp(operation, "TRACE"))
	  con->http.state = HTTP_TRACE;
        else if (!strcmp(operation, "OPTIONS"))
	  con->http.state = HTTP_OPTIONS;
        else if (!strcmp(operation, "HEAD"))
	  con->http.state = HTTP_HEAD;
	else
	{
	  cupsdLogMessage(CUPSD_LOG_ERROR, "Bad operation \"%s\"!", operation);
	  cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE);
	  cupsdCloseClient(con);
	  return;
	}
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        gettimeofday(&(con->start), NULL);
        con->operation = con->http.state;

        cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdReadClient: %d %s %s HTTP/%d.%d",
	                con->http.fd, operation, con->uri,
		        con->http.version / 100, con->http.version % 100);

	con->http.status = HTTP_OK;

        if (!cupsArrayFind(ActiveClients, con))
	{
	  cupsArrayAdd(ActiveClients, con);
          cupsdSetBusyState();
        }

    case HTTP_OPTIONS :
    case HTTP_DELETE :
    case HTTP_GET :
    case HTTP_HEAD :
    case HTTP_POST :
    case HTTP_PUT :
    case HTTP_TRACE :
       /*
        * Parse incoming parameters until the status changes...
	*/
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        while ((status = httpUpdate(HTTP(con))) == HTTP_CONTINUE)
	  if (!data_ready(con))
	    break;

	if (status != HTTP_OK && status != HTTP_CONTINUE)
	{
	  if (con->http.error && con->http.error != EPIPE)
	    cupsdLogMessage(CUPSD_LOG_DEBUG,
			    "cupsdReadClient: %d FIELDS Closing for error %d "
			    "(%s)", con->http.fd, con->http.error,
			    strerror(con->http.error));
	  else
	    cupsdLogMessage(CUPSD_LOG_DEBUG,
	                    "cupsdReadClient: %d FIELDS Closing on EOF",
			    con->http.fd);

	  cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE);
	  cupsdCloseClient(con);
	  return;
	}
	break;
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
    default :
        if (!data_ready(con) && recv(con->http.fd, buf, 1, MSG_PEEK) < 1)
	{
	 /*
	  * Connection closed...
	  */

	  cupsdLogMessage(CUPSD_LOG_DEBUG,
			  "cupsdReadClient: %d Closing on EOF", con->http.fd);
          cupsdCloseClient(con);
	  return;
	}
        break; /* Anti-compiler-warning-code */
  }

 /*
  * Handle new transfers...
  */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  if (status == HTTP_OK)
  {
    if (con->http.fields[HTTP_FIELD_ACCEPT_LANGUAGE][0])
    {
     /*
      * Figure out the locale from the Accept-Language and Content-Type
      * fields...
      */

      if ((ptr = strchr(con->http.fields[HTTP_FIELD_ACCEPT_LANGUAGE],
                        ',')) != NULL)
        *ptr = '\0';

      if ((ptr = strchr(con->http.fields[HTTP_FIELD_ACCEPT_LANGUAGE],
                        ';')) != NULL)
        *ptr = '\0';

      if ((ptr = strstr(con->http.fields[HTTP_FIELD_CONTENT_TYPE],
                        "charset=")) != NULL)
      {
       /*
        * Combine language and charset, and trim any extra params in the
	* content-type.
	*/

        snprintf(locale, sizeof(locale), "%s.%s",
	         con->http.fields[HTTP_FIELD_ACCEPT_LANGUAGE], ptr + 8);

	if ((ptr = strchr(locale, ',')) != NULL)
	  *ptr = '\0';
      }
      else
        snprintf(locale, sizeof(locale), "%s.UTF-8",
	         con->http.fields[HTTP_FIELD_ACCEPT_LANGUAGE]);

      con->language = cupsLangGet(locale);
    }
    else
      con->language = cupsLangGet(DefaultLocale);
    #ifdef NOT_CUT

    cupsdAuthorize(con);

    #endif
    if (!strncasecmp(con->http.fields[HTTP_FIELD_CONNECTION], "Keep-Alive",
	             10) && KeepAlive)
      con->http.keep_alive = HTTP_KEEPALIVE_ON;
    else if (!strncasecmp(con->http.fields[HTTP_FIELD_CONNECTION], "close", 5))
      con->http.keep_alive = HTTP_KEEPALIVE_OFF;

    if (!con->http.fields[HTTP_FIELD_HOST][0] &&
        con->http.version >= HTTP_1_1)
    {
     /*
      * HTTP/1.1 and higher require the "Host:" field...
      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
      if (!cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE))
      {
        cupsdLogMessage(CUPSD_LOG_ERROR, "Missing Host: field in request!");
	cupsdCloseClient(con);
	return;
      }
    }
    else if (!valid_host(con))
    {
     /*
      * Access to localhost must use "localhost" or the corresponding IPv4
      * or IPv6 values in the Host: field.
      */

      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Request from \"%s\" using invalid Host: field \"%s\"",
		      con->http.hostname, con->http.fields[HTTP_FIELD_HOST]);

      if (!cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE))
      {
	cupsdCloseClient(con);
	return;
      }
    }
    else if (con->operation == HTTP_OPTIONS)
    {
     /*
      * Do OPTIONS command...
      */

      if (con->best && con->best->type != CUPSD_AUTH_NONE)
      {
	if (!cupsdSendHeader(con, HTTP_UNAUTHORIZED, NULL, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}
      }

      if (!strcasecmp(con->http.fields[HTTP_FIELD_CONNECTION], "Upgrade") &&
	  con->http.tls == NULL)
      {
#ifdef HAVE_SSL
       /*
        * Do encryption stuff...
	*/

	if (!cupsdSendHeader(con, HTTP_SWITCHING_PROTOCOLS, NULL, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}

	httpPrintf(HTTP(con), "Connection: Upgrade\r\n");
	httpPrintf(HTTP(con), "Upgrade: TLS/1.0,HTTP/1.1\r\n");
	httpPrintf(HTTP(con), "Content-Length: 0\r\n");
	httpPrintf(HTTP(con), "\r\n");

	if (cupsdFlushHeader(con) < 0)
        {
	  cupsdCloseClient(con);
	  return;
	}

        if (!encrypt_client(con))
        {
	  cupsdCloseClient(con);
	  return;
	}
#else
	if (!cupsdSendError(con, HTTP_NOT_IMPLEMENTED, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}
#endif /* HAVE_SSL */
      }

      if (!cupsdSendHeader(con, HTTP_OK, NULL, CUPSD_AUTH_NONE))
      {
	cupsdCloseClient(con);
	return;
      }

      httpPrintf(HTTP(con), "Allow: GET, HEAD, OPTIONS, POST, PUT\r\n");
      httpPrintf(HTTP(con), "Content-Length: 0\r\n");
      httpPrintf(HTTP(con), "\r\n");
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
      if (cupsdFlushHeader(con) < 0)
      {
	cupsdCloseClient(con);
	return;
      }
    }
    else if (!is_path_absolute(con->uri))
    {
     /*
      * Protect against malicious users!
      */

      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Request for non-absolute resource \"%s\"!", con->uri);

      if (!cupsdSendError(con, HTTP_FORBIDDEN, CUPSD_AUTH_NONE))
      {
	cupsdCloseClient(con);
	return;
      }
    }
    else
    {
      if (!strcasecmp(con->http.fields[HTTP_FIELD_CONNECTION], "Upgrade") &&
	  con->http.tls == NULL)
      {
#ifdef HAVE_SSL
       /*
        * Do encryption stuff...
	*/

	if (!cupsdSendHeader(con, HTTP_SWITCHING_PROTOCOLS, NULL, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}

	httpPrintf(HTTP(con), "Connection: Upgrade\r\n");
	httpPrintf(HTTP(con), "Upgrade: TLS/1.0,HTTP/1.1\r\n");
	httpPrintf(HTTP(con), "Content-Length: 0\r\n");
	httpPrintf(HTTP(con), "\r\n");
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	if (cupsdFlushHeader(con) < 0)
        {
	  cupsdCloseClient(con);
	  return;
	}
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        if (!encrypt_client(con))
        {
	  cupsdCloseClient(con);
	  return;
	}
#else
	if (!cupsdSendError(con, HTTP_NOT_IMPLEMENTED, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}
#endif /* HAVE_SSL */
      }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
#ifdef NOT_CUT
      if ((status = cupsdIsAuthorized(con, NULL)) != HTTP_OK)
      {
	cupsdSendError(con, status, CUPSD_AUTH_NONE);
	cupsdCloseClient(con);
	return;
      }
#endif
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
      if (con->http.expect &&
          (con->operation == HTTP_POST || con->operation == HTTP_PUT))
      {
        if (con->http.expect == HTTP_CONTINUE)
	{
	 /*
	  * Send 100-continue header...
	  */

	  if (!cupsdSendHeader(con, HTTP_CONTINUE, NULL, CUPSD_AUTH_NONE))
	  {
	    cupsdCloseClient(con);
	    return;
	  }
	}
	else
	{
	 /*
	  * Send 417-expectation-failed header...
	  */

	  if (!cupsdSendHeader(con, HTTP_EXPECTATION_FAILED, NULL,
	                       CUPSD_AUTH_NONE))
	  {
	    cupsdCloseClient(con);
	    return;
	  }

	  httpPrintf(HTTP(con), "Content-Length: 0\r\n");
	  httpPrintf(HTTP(con), "\r\n");

	  if (cupsdFlushHeader(con) < 0)
          {
	    cupsdCloseClient(con);
	    return;
	  }
	}
      }
//printf("pail=child cups==file:%s,line:%d=[%d]\n",__FILE__,__LINE__,con->http.state);
      switch (con->http.state)
      {
	case HTTP_GET_SEND :
            if (!strncmp(con->uri, "/printers/", 10) &&
		!strcmp(con->uri + strlen(con->uri) - 4, ".ppd"))
	    {
	     /*
	      * Send PPD file - get the real printer name since printer
	      * names are not case sensitive but filenames can be...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              con->uri[strlen(con->uri) - 4] = '\0';	/* Drop ".ppd" */

              if ((p = cupsdFindPrinter(con->uri + 10)) != NULL)
		snprintf(con->uri, sizeof(con->uri), "/ppd/%s.ppd", p->name);
	      else
	      {
		if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }
	    }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	    if ((!strncmp(con->uri, "/admin", 6) &&
	         strncmp(con->uri, "/admin/conf/", 12) &&
	         strncmp(con->uri, "/admin/log/", 11)) ||
		!strncmp(con->uri, "/printers", 9) ||
		!strncmp(con->uri, "/classes", 8) ||
		!strncmp(con->uri, "/help", 5) ||
		!strncmp(con->uri, "/jobs", 5))
	    {
	     /*
	      * Send CGI output...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if (!strncmp(con->uri, "/admin", 6))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/admin.cgi",
		                ServerBin);

		cupsdSetString(&con->options, strchr(con->uri + 6, '?'));
	      }
              else if (!strncmp(con->uri, "/printers", 9))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/printers.cgi",
		                ServerBin);

                if (con->uri[9] && con->uri[10])
		  cupsdSetString(&con->options, con->uri + 9);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/classes", 8))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/classes.cgi",
		                ServerBin);

                if (con->uri[8] && con->uri[9])
		  cupsdSetString(&con->options, con->uri + 8);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/jobs", 5))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/jobs.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/help.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if (!cupsdSendCommand(con, con->command, con->options, 0))
	      {
		if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
              }
	      else
 #ifdef OPEN_DEBUG           
        	cupsdLogRequest(con, HTTP_OK);
 #endif
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      if (con->http.version <= HTTP_1_0)
		con->http.keep_alive = HTTP_KEEPALIVE_OFF;
	    }
            else if ((!strncmp(con->uri, "/admin/conf/", 12) &&
	              (strchr(con->uri + 12, '/') ||
		       strlen(con->uri) == 12)) ||
		     (!strncmp(con->uri, "/admin/log/", 11) &&
	              (strchr(con->uri + 11, '/') ||
		       strlen(con->uri) == 11)))
	    {
	     /*
	      * GET can only be done to configuration files directly under
	      * /admin/conf...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      cupsdLogMessage(CUPSD_LOG_ERROR,
			      "Request for subdirectory \"%s\"!", con->uri);

	      if (!cupsdSendError(con, HTTP_FORBIDDEN, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }
	    else
	    {
	     /*
	      * Serve a file...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if ((filename = get_file(con, &filestats, buf,
	                               sizeof(buf))) == NULL)
	      {
		if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }

	      type = mimeFileType(MimeDatabase, filename, NULL, NULL);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if (is_cgi(con, filename, &filestats, type))
	      {
	       /*
	        * Note: con->command and con->options were set by
		* is_cgi()...
		*/
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        	if (!cupsdSendCommand(con, con->command, con->options, 0))
		{
		  if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		  {
		    cupsdCloseClient(con);
		    return;
		  }
        	}
		else
       #ifdef OPEN_DEBUG
        	  cupsdLogRequest(con, HTTP_OK);
       #endif
		if (con->http.version <= HTTP_1_0)
		  con->http.keep_alive = HTTP_KEEPALIVE_OFF;
	        break;
	      }

	      if (!check_if_modified(con, &filestats))
              {
        	if (!cupsdSendError(con, HTTP_NOT_MODIFIED, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	      else
              {
		if (type == NULL)
	          strcpy(line, "text/plain");
		else
	          snprintf(line, sizeof(line), "%s/%s", type->super, type->type);

        	if (!write_file(con, HTTP_OK, filename, line, &filestats))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	    }
            break;

	case HTTP_POST_RECV :
           /*
	    * See if the POST request includes a Content-Length field, and if
	    * so check the length against any limits that are set...
	    */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
            if (con->http.fields[HTTP_FIELD_CONTENT_LENGTH][0] &&
		MaxRequestSize > 0 &&
		con->http.data_remaining > MaxRequestSize)
	    {
	     /*
	      * Request too large...
	      */

              if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
            }
	    else if (con->http.data_remaining < 0 ||
	             (!con->http.fields[HTTP_FIELD_CONTENT_LENGTH][0] &&
		      con->http.data_encoding == HTTP_ENCODE_LENGTH))
	    {
	     /*
	      * Negative content lengths are invalid!
	      */

              if (!cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }

           /*
	    * See what kind of POST request this is; for IPP requests the
	    * content-type field will be "application/ipp"...
	    */

	    if (!strcmp(con->http.fields[HTTP_FIELD_CONTENT_TYPE],
	                "application/ipp"))
              con->request = ippNew();
	    else if ((!strncmp(con->uri, "/admin", 6) &&
	              strncmp(con->uri, "/admin/conf/", 12) &&
	              strncmp(con->uri, "/admin/log/", 11)) ||
	             !strncmp(con->uri, "/printers", 9) ||
	             !strncmp(con->uri, "/classes", 8) ||
	             !strncmp(con->uri, "/help", 5) ||
	             !strncmp(con->uri, "/jobs", 5))
	    {
	     /*
	      * CGI request...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if (!strncmp(con->uri, "/admin", 6))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/admin.cgi",
		                ServerBin);

		cupsdSetString(&con->options, strchr(con->uri + 6, '?'));
	      }
              else if (!strncmp(con->uri, "/printers", 9))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/printers.cgi",
		                ServerBin);

                if (con->uri[9] && con->uri[10])
		  cupsdSetString(&con->options, con->uri + 9);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/classes", 8))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/classes.cgi",
		                ServerBin);

                if (con->uri[8] && con->uri[9])
		  cupsdSetString(&con->options, con->uri + 8);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/jobs", 5))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/jobs.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/help.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }

	      if (con->http.version <= HTTP_1_0)
		con->http.keep_alive = HTTP_KEEPALIVE_OFF;
	    }
	    else
	    {
	     /*
	      * POST to a file...
	      */

              if ((filename = get_file(con, &filestats, buf,
	                               sizeof(buf))) == NULL)
	      {
		if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }

	      type = mimeFileType(MimeDatabase, filename, NULL, NULL);

              if (!is_cgi(con, filename, &filestats, type))
	      {
	       /*
	        * Only POST to CGI's...
		*/

		if (!cupsdSendError(con, HTTP_UNAUTHORIZED, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	    }
	    break;

	case HTTP_PUT_RECV :
	   /*
	    * Validate the resource name...
	    */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
            if (strncmp(con->uri, "/admin/conf/", 12) ||
	        strchr(con->uri + 12, '/') ||
		strlen(con->uri) == 12)
	    {
	     /*
	      * PUT can only be done to configuration files under
	      * /admin/conf...
	      */

	      cupsdLogMessage(CUPSD_LOG_ERROR,
			      "Request for subdirectory \"%s\"!", con->uri);

	      if (!cupsdSendError(con, HTTP_FORBIDDEN, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }

           /*
	    * See if the PUT request includes a Content-Length field, and if
	    * so check the length against any limits that are set...
	    */

            if (con->http.fields[HTTP_FIELD_CONTENT_LENGTH][0] &&
		MaxRequestSize > 0 &&
		con->http.data_remaining > MaxRequestSize)
	    {
	     /*
	      * Request too large...
	      */

              if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
            }
	    else if (con->http.data_remaining < 0)
	    {
	     /*
	      * Negative content lengths are invalid!
	      */

              if (!cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
           /*
	    * Open a temporary file to hold the request...
	    */

            cupsdSetStringf(&con->filename, "%s/%08x", RequestRoot,
	                    request_id ++);
	    con->file = open(con->filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);

	    if (con->file < 0)
	    {
	      cupsdLogMessage(CUPSD_LOG_ERROR,
	                      "Unable to create request file %s: %s",
	                      con->filename, strerror(errno));
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }

	    fchmod(con->file, 0640);
	    fchown(con->file, RunUser, Group);
	    fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);
	    break;

	case HTTP_DELETE :
	case HTTP_TRACE :
            cupsdSendError(con, HTTP_NOT_IMPLEMENTED, CUPSD_AUTH_NONE);
	    cupsdCloseClient(con);
	    return;

	case HTTP_HEAD :
            if (!strncmp(con->uri, "/printers/", 10) &&
		!strcmp(con->uri + strlen(con->uri) - 4, ".ppd"))
	    {
	     /*
	      * Send PPD file - get the real printer name since printer
	      * names are not case sensitive but filenames can be...
	      */

              con->uri[strlen(con->uri) - 4] = '\0';	/* Drop ".ppd" */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if ((p = cupsdFindPrinter(con->uri + 10)) != NULL)
		snprintf(con->uri, sizeof(con->uri), "/ppd/%s.ppd", p->name);
	      else
	      {
		if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }
	    }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	    if ((!strncmp(con->uri, "/admin", 6) &&
	         strncmp(con->uri, "/admin/conf/", 12) &&
	         strncmp(con->uri, "/admin/log/", 11)) ||
		!strncmp(con->uri, "/printers", 9) ||
		!strncmp(con->uri, "/classes", 8) ||
		!strncmp(con->uri, "/help", 5) ||
		!strncmp(con->uri, "/jobs", 5))
	    {
	     /*
	      * CGI output...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if (!cupsdSendHeader(con, HTTP_OK, "text/html", CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      if (httpPrintf(HTTP(con), "\r\n") < 0)
	      {
		cupsdCloseClient(con);
		return;
	      }

	      if (cupsdFlushHeader(con) < 0)
	      {
		cupsdCloseClient(con);
		return;
	      }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
#ifdef OPEN_DEBUG
              cupsdLogRequest(con, HTTP_OK);
#endif
	    }
            else if ((!strncmp(con->uri, "/admin/conf/", 12) &&
	              (strchr(con->uri + 12, '/') ||
		       strlen(con->uri) == 12)) ||
		     (!strncmp(con->uri, "/admin/log/", 11) &&
	              (strchr(con->uri + 11, '/') ||
		       strlen(con->uri) == 11)))
	    {
	     /*
	      * HEAD can only be done to configuration files under
	      * /admin/conf...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      cupsdLogMessage(CUPSD_LOG_ERROR,
			      "Request for subdirectory \"%s\"!", con->uri);

	      if (!cupsdSendError(con, HTTP_FORBIDDEN, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }
	    else if ((filename = get_file(con, &filestats, buf,
	                                  sizeof(buf))) == NULL)
	    {
	      if (!cupsdSendHeader(con, HTTP_NOT_FOUND, "text/html",
	                           CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
#ifdef OPEN_DEBUG

              cupsdLogRequest(con, HTTP_NOT_FOUND);
#endif
	    }
	    else if (!check_if_modified(con, &filestats))
            {
              if (!cupsdSendError(con, HTTP_NOT_MODIFIED, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
#ifdef OPEN_DEBUG

              cupsdLogRequest(con, HTTP_NOT_MODIFIED);
#endif
	    }
	    else
	    {
	     /*
	      * Serve a file...
	      */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      type = mimeFileType(MimeDatabase, filename, NULL, NULL);
	      if (type == NULL)
		strcpy(line, "text/plain");
	      else
		snprintf(line, sizeof(line), "%s/%s", type->super, type->type);

              if (!cupsdSendHeader(con, HTTP_OK, line, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      if (httpPrintf(HTTP(con), "Last-Modified: %s\r\n",
	                     httpGetDateString(filestats.st_mtime)) < 0)
	      {
		cupsdCloseClient(con);
		return;
	      }

	      if (httpPrintf(HTTP(con), "Content-Length: %lu\r\n",
	                     (unsigned long)filestats.st_size) < 0)
	      {
		cupsdCloseClient(con);
		return;
	      }
#ifdef OPEN_DEBUG

              cupsdLogRequest(con, HTTP_OK);
#endif
	    }

            if (httpPrintf(HTTP(con), "\r\n") < 0)
	    {
	      cupsdCloseClient(con);
	      return;
	    }

	    if (cupsdFlushHeader(con) < 0)
            {
	      cupsdCloseClient(con);
	      return;
	    }

            con->http.state = HTTP_WAITING;
            break;

	default :
            break; /* Anti-compiler-warning-code */
      }
    }
  }

 /*
  * Handle any incoming data...
  */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  switch (con->http.state)
  {
    case HTTP_PUT_RECV :
        do
	{
          if ((bytes = httpRead2(HTTP(con), line, sizeof(line))) < 0)
	  {
	    if (con->http.error && con->http.error != EPIPE)
	      cupsdLogMessage(CUPSD_LOG_DEBUG,
			      "cupsdReadClient: %d PUT_RECV Closing for error "
			      "%d (%s)", con->http.fd, con->http.error,
			      strerror(con->http.error));
	    else
	      cupsdLogMessage(CUPSD_LOG_DEBUG,
			      "cupsdReadClient: %d PUT_RECV Closing on EOF",
			      con->http.fd);

	    cupsdCloseClient(con);
	    return;
	  }
	  else if (bytes > 0)
	  {
	    con->bytes += bytes;

            if (write(con->file, line, bytes) < bytes)
	    {
              cupsdLogMessage(CUPSD_LOG_ERROR,
	                      "cupsdReadClient: Unable to write %d bytes to %s: %s",
	                      bytes, con->filename, strerror(errno));

	      close(con->file);
	      con->file = -1;
	      unlink(con->filename);
	      cupsdClearString(&con->filename);

              if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }
	  }
        }
	while (con->http.state == HTTP_PUT_RECV && data_ready(con));
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        if (con->http.state == HTTP_WAITING)
	{
	 /*
	  * End of file, see how big it is...
	  */

	  fstat(con->file, &filestats);

	  close(con->file);
	  con->file = -1;

          if (filestats.st_size > MaxRequestSize &&
	      MaxRequestSize > 0)
	  {
	   /*
	    * Request is too big; remove it and send an error...
	    */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	    unlink(con->filename);
	    cupsdClearString(&con->filename);

            if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	    {
	      cupsdCloseClient(con);
	      return;
	    }
	  }

         /*
	  * Install the configuration file...
	  */

          status = install_conf_file(con);

         /*
	  * Return the status to the client...
	  */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
          if (!cupsdSendError(con, status, CUPSD_AUTH_NONE))
	  {
	    cupsdCloseClient(con);
	    return;
	  }
	}
        break;

    case HTTP_POST_RECV :
        do
	{
          if (con->request && con->file < 0
#ifdef SUPPORT_ATP_SAMBA_PRINT_SIGLE_CACHFILE
           && con->request->filename[0] == '\0'
#endif
           )
	  {
	   /*
	    * Grab any request data from the connection...
	    */
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	    if ((ipp_state = ippRead(&(con->http), con->request)) == IPP_ERROR)
	    {
              cupsdLogMessage(CUPSD_LOG_ERROR,
	                      "cupsdReadClient: %d IPP Read Error!",
			      con->http.fd);

	      cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE);
	      cupsdCloseClient(con);
	      return;
	    }
	    else if (ipp_state != IPP_DATA)
	    {
              if (con->http.state == HTTP_POST_SEND)
	      {
		cupsdSendError(con, HTTP_BAD_REQUEST, CUPSD_AUTH_NONE);
		cupsdCloseClient(con);
		return;
	      }

	      break;
            }
	    else
	    {
	      cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdReadClient: %d %d.%d %s %d",
			      con->http.fd, con->request->request.op.version[0],
			      con->request->request.op.version[1],
			      ippOpString(con->request->request.op.operation_id),
			      con->request->request.op.request_id);
	      con->bytes += ippLength(con->request);
	    }
	  }
#ifndef SUPPORT_ATP_SAMBA_PRINT_SIGLE_CACHFILE
          if (con->file < 0 && con->http.state != HTTP_POST_SEND)
	  {
           /*
	    * Create a file as needed for the request data...
	    */

            cupsdSetStringf(&con->filename, "%s/%08x", RequestRoot,
	                    request_id ++);
	    con->file = open(con->filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);

	    if (con->file < 0)
	    {
	      cupsdLogMessage(CUPSD_LOG_ERROR,
	                      "Unable to create request file %s: %s",
	                      con->filename, strerror(errno));

	      if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	    fchmod(con->file, 0640);
	    fchown(con->file, RunUser, Group);
            fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);
	  }
#endif
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	  if (con->http.state != HTTP_POST_SEND)
	  {
//	  printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
            if ((bytes = httpRead2(HTTP(con), line, sizeof(line))) < 0)
	    {
	      if (con->http.error && con->http.error != EPIPE)
		cupsdLogMessage(CUPSD_LOG_DEBUG,
				"cupsdReadClient: %d POST_SEND Closing for "
				"error %d (%s)", con->http.fd, con->http.error,
				strerror(con->http.error));
	      else
		cupsdLogMessage(CUPSD_LOG_DEBUG,
				"cupsdReadClient: %d POST_SEND Closing on EOF",
				con->http.fd);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      cupsdCloseClient(con);

	      return;
	    }
	    else if (bytes > 0)
	    {
	      con->bytes += bytes;
#ifdef SUPPORT_ATP_SAMBA_PRINT_SIGLE_CACHFILE
            DBGYJ("con->bytes= %llu,bytes=%d",con->bytes,bytes);
            if(con->request->filename[0] == '\0') 
            {
                if (bytes >= 48)
                {
                    DBGYJ("line= %s",line);
                    snprintf(con->request->filename,48,"%s",line);
                    DBGYJ("con->request->filename= %s",con->request->filename);
                }
                else
                {
                    snprintf(con->request->filename,48,"%s","+");
                }
                DBGYJ("con->request->filename = %s",con->request->filename);
            }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
            if ( con->request->filename[0] == '+'  && con->file < 0 && con->http.state != HTTP_POST_SEND)
            {
               /*
            * Create a file as needed for the request data...
            */

                cupsdSetStringf(&con->filename, "%s/%08x", RequestRoot,
                            request_id ++);
            con->file = open(con->filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);

            if (con->file < 0)
            {
              cupsdLogMessage(CUPSD_LOG_ERROR,
                              "Unable to create request file %s: %s",
                              con->filename, strerror(errno));

              if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
              {
            cupsdCloseClient(con);
            return;
              }
            }
            //printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
            fchmod(con->file, 0640);
            fchown(con->file, RunUser, Group);
                fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);
            }
            if(con->file >= 0)
            {
#endif
              if (write(con->file, line, bytes) < bytes)
	      {
        	cupsdLogMessage(CUPSD_LOG_ERROR,
	                	"cupsdReadClient: Unable to write %d bytes to "
				"%s: %s", bytes, con->filename,
				strerror(errno));

		close(con->file);
		con->file = -1;
		unlink(con->filename);
		cupsdClearString(&con->filename);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        	if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE,
		                    CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
            }
#ifdef SUPPORT_ATP_SAMBA_PRINT_SIGLE_CACHFILE
	    }
#endif
	    else if (con->http.state == HTTP_POST_RECV)
              return;
	    else if (con->http.state != HTTP_POST_SEND)
	    {
	      cupsdLogMessage(CUPSD_LOG_DEBUG,
	                      "cupsdReadClient: %d Closing on unknown HTTP "
			      "state %d", con->http.fd, con->http.state);
	      cupsdCloseClient(con);
	      return;
	    }
	  }
        }
	while (con->http.state == HTTP_POST_RECV && data_ready(con));
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	if (con->http.state == HTTP_POST_SEND)
	{
#ifdef SUPPORT_ATP_SAMBA_PRINT_SIGLE_CACHFILE
	  if ((con->file >= 0) || (con->request->filename[0] != '\0'))
	  {
              if (( con->request->filename[0] != '\0' ) && ( con->request->filename[0] != '+' ))
        	  {
                    DBGYJ("Rename....................");
                    cupsdSetStringf(&con->filename, "%s/%08x", RequestRoot,
        	                    request_id ++);
                    DBGYJ("con->request->filename=%s",con->request->filename);
                   rename(con->request->filename,con->filename);
                   
        	    con->file = open(con->filename, O_RDWR);
        	    if (con->file < 0)
        	    {
        	      cupsdLogMessage(CUPSD_LOG_ERROR,
        	                      "Unable to create request file %s: %s",
        	                      con->filename, strerror(errno));

        	      if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
        	      {
        		cupsdCloseClient(con);
        		return;
        	      }
        	    }
        //printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        	    fchmod(con->file, 0640);
        	    fchown(con->file, RunUser, Group);
                    fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);
        	  }
#else
	  if (con->file >= 0)
	  {
#endif
	    fstat(con->file, &filestats);

	    close(con->file);
	    con->file = -1;

            if (filestats.st_size > MaxRequestSize &&
	        MaxRequestSize > 0)
	    {
	     /*
	      * Request is too big; remove it and send an error...
	      */

	      unlink(con->filename);
	      cupsdClearString(&con->filename);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	      if (con->request)
	      {
	       /*
	        * Delete any IPP request data...
		*/
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	        ippDelete(con->request);
		con->request = NULL;
              }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
              if (!cupsdSendError(con, HTTP_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }
	    else if (filestats.st_size == 0)
	    {
	     /*
	      * Don't allow empty file...
	      */

	      unlink(con->filename);
	      cupsdClearString(&con->filename);
	    }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
        printf("==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);

	    if (con->command)
	    {
	      if (!cupsdSendCommand(con, con->command, con->options, 0))
	      {
		if (!cupsdSendError(con, HTTP_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
              }
            #ifdef OPEN_DEBUG
            else
        	cupsdLogRequest(con, HTTP_OK);
            #endif
            }
	  }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
          if (con->request)
	  {

	    cupsdProcessIPPRequest(con);

	    if (con->filename)
	    {
	      unlink(con->filename);
	      cupsdClearString(&con->filename);
	    }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
	    return;
	  }
	}
        break;

    default :
        break; /* Anti-compiler-warning-code */
  }
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  if (con->http.state == HTTP_WAITING)
  {
    if (!con->http.keep_alive)
    {
    #ifdef OPEN_DEBUG
      cupsdLogMessage(CUPSD_LOG_DEBUG,
		      "cupsdReadClient: %d Closing because Keep-Alive disabled",
		      con->http.fd);
    #endif
      cupsdCloseClient(con);
    }
    else
    {
      cupsArrayRemove(ActiveClients, con);
      cupsdSetBusyState();
    }
  }
}


/*
 * 'cupsdSendCommand()' - Send output from a command via HTTP.
 */

int					/* O - 1 on success, 0 on failure */
cupsdSendCommand(
    cupsd_client_t *con,		/* I - Client connection */
    char           *command,		/* I - Command to run */
    char           *options,		/* I - Command-line options */
    int            root)		/* I - Run as root? */
{
  int	fd;				/* Standard input file descriptor */

//printf("pail=child cups==file:%s,line:%d=[%s]\n",__FILE__,__LINE__,con->filename);
  if (con->filename)
  {
    fd = open(con->filename, O_RDONLY);

    if (fd < 0)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "cupsdSendCommand: %d Unable to open \"%s\" for reading: %s",
                      con->http.fd, con->filename ? con->filename : "/dev/null",
	              strerror(errno));
      return (0);
    }

    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
  }
  else
    fd = -1;
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  con->pipe_pid = pipe_command(con, fd, &(con->file), command, options, root);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  if (fd >= 0)
    close(fd);

  cupsdLogMessage(CUPSD_LOG_INFO, "Started \"%s\" (pid=%d)", command,
                  con->pipe_pid);

  cupsdLogMessage(CUPSD_LOG_DEBUG, "cupsdSendCommand: %d file=%d",
                  con->http.fd, con->file);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  if (con->pipe_pid == 0)
    return (0);

  fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);

  cupsdAddSelect(con->file, (cupsd_selfunc_t)write_pipe, NULL, con);
//printf("pail=child cups==file:%s,line:%d=\n",__FILE__,__LINE__);
  con->sent_header = 0;
  con->file_ready  = 0;
  con->got_fields  = 0;
  con->header_used = 0;

  return (1);
}


/*
 * 'cupsdSendError()' - Send an error message via HTTP.
 */

int					/* O - 1 if successful, 0 otherwise */
cupsdSendError(cupsd_client_t *con,	/* I - Connection */
               http_status_t  code,	/* I - Error code */
	       int            auth_type)/* I - Authentication type */
{
  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cupsdSendError(con=%p(%d), code=%d, auth_type=%d", con,
		  con->http.fd, code, auth_type);

#ifdef HAVE_SSL
 /*
  * Force client to upgrade for authentication if that is how the
  * server is configured...
  */

  if (code == HTTP_UNAUTHORIZED &&
      DefaultEncryption == HTTP_ENCRYPT_REQUIRED &&
      strcasecmp(con->http.hostname, "localhost") &&
      !con->http.tls)
  {
    code = HTTP_UPGRADE_REQUIRED;
  }
#endif /* HAVE_SSL */

 /*
  * Put the request in the access_log file...
  */
#ifdef OPEN_DEBUG

  cupsdLogRequest(con, code);
#endif
 /*
  * To work around bugs in some proxies, don't use Keep-Alive for some
  * error messages...
  *
  * Kerberos authentication doesn't work without Keep-Alive, so
  * never disable it in that case.
  */

  if (code >= HTTP_BAD_REQUEST && con->http.auth_type != CUPSD_AUTH_NEGOTIATE)
    con->http.keep_alive = HTTP_KEEPALIVE_OFF;

 /*
  * Send an error message back to the client.  If the error code is a
  * 400 or 500 series, make sure the message contains some text, too!
  */

  if (!cupsdSendHeader(con, code, NULL, auth_type))
    return (0);

#ifdef HAVE_SSL
  if (code == HTTP_UPGRADE_REQUIRED)
    if (httpPrintf(HTTP(con), "Connection: Upgrade\r\n") < 0)
      return (0);

  if (httpPrintf(HTTP(con), "Upgrade: TLS/1.0,HTTP/1.1\r\n") < 0)
    return (0);
#endif /* HAVE_SSL */

  if (con->http.version >= HTTP_1_1 &&
      con->http.keep_alive == HTTP_KEEPALIVE_OFF)
  {
    if (httpPrintf(HTTP(con), "Connection: close\r\n") < 0)
      return (0);
  }

  if (code >= HTTP_BAD_REQUEST)
  {
   /*
    * Send a human-readable error message.
    */

    char	message[4096],		/* Message for user */
		urltext[1024],		/* URL redirection text */
		redirect[1024];		/* Redirection link */
    const char	*text;			/* Status-specific text */


    redirect[0] = '\0';

    if (code == HTTP_UNAUTHORIZED)
      text = _cupsLangString(con->language,
                             _("Enter your username and password or the "
			       "root username and password to access this "
			       "page. If you are using Kerberos authentication, "
			       "make sure you have a valid Kerberos ticket."));
    else if (code == HTTP_UPGRADE_REQUIRED)
    {
      text = urltext;

      snprintf(urltext, sizeof(urltext),
               _cupsLangString(con->language,
                               _("You must access this page using the URL "
			         "<A HREF=\"https://%s:%d%s\">"
				 "https://%s:%d%s</A>.")),
               con->servername, con->serverport, con->uri,
	       con->servername, con->serverport, con->uri);

      snprintf(redirect, sizeof(redirect),
               "<META HTTP-EQUIV=\"Refresh\" "
	       "CONTENT=\"3;URL=https://%s:%d%s\">\n",
	       con->servername, con->serverport, con->uri);
    }
    else
      text = "";

    snprintf(message, sizeof(message),
             "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" "
	     "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
	     "<HTML>\n"
	     "<HEAD>\n"
             "\t<META HTTP-EQUIV=\"Content-Type\" "
	     "CONTENT=\"text/html; charset=utf-8\">\n"
	     "\t<TITLE>%d %s</TITLE>\n"
	     "\t<LINK REL=\"STYLESHEET\" TYPE=\"text/css\" "
	     "HREF=\"/cups.css\">\n"
	     "%s"
	     "</HEAD>\n"
             "<BODY>\n"
	     "<H1>%d %s</H1>\n"
	     "<P>%s</P>\n"
	     "</BODY>\n"
	     "</HTML>\n",
	     code, httpStatus(code), redirect, code, httpStatus(code), text);

    if (httpPrintf(HTTP(con), "Content-Type: text/html; charset=utf-8\r\n") < 0)
      return (0);
    if (httpPrintf(HTTP(con), "Content-Length: %d\r\n",
                   (int)strlen(message)) < 0)
      return (0);
    if (httpPrintf(HTTP(con), "\r\n") < 0)
      return (0);
    if (httpPrintf(HTTP(con), "%s", message) < 0)
      return (0);
  }
  else if (httpPrintf(HTTP(con), "\r\n") < 0)
    return (0);

  if (cupsdFlushHeader(con) < 0)
    return (0);

  con->http.state = HTTP_WAITING;

  return (1);
}


/*
 * 'cupsdSendHeader()' - Send an HTTP request.
 */

int					/* O - 1 on success, 0 on failure */
cupsdSendHeader(
    cupsd_client_t *con,		/* I - Client to send to */
    http_status_t  code,		/* I - HTTP status code */
    char           *type,		/* I - MIME type of document */
    int            auth_type)		/* I - Type of authentication */
{
  char		auth_str[1024];		/* Authorization string */
#ifdef HAVE_GSSAPI
  static char	*gss_buf = NULL;	/* Kerberos auth data buffer */
  static int	gss_bufsize = 0;	/* Size of Kerberos auth data buffer */
#endif /* HAVE_GSSAPI */


 /*
  * Send the HTTP status header...
  */

  if (code == HTTP_CONTINUE)
  {
   /*
    * 100-continue doesn't send any headers...
    */

    return (httpPrintf(HTTP(con), "HTTP/%d.%d 100 Continue\r\n\r\n",
		       con->http.version / 100, con->http.version % 100) > 0);
  }

  httpFlushWrite(HTTP(con));

  con->http.data_encoding = HTTP_ENCODE_FIELDS;

  if (httpPrintf(HTTP(con), "HTTP/%d.%d %d %s\r\n", con->http.version / 100,
                 con->http.version % 100, code, httpStatus(code)) < 0)
    return (0);
  if (httpPrintf(HTTP(con), "Date: %s\r\n", httpGetDateString(time(NULL))) < 0)
    return (0);
  if (ServerHeader)
    if (httpPrintf(HTTP(con), "Server: %s\r\n", ServerHeader) < 0)
      return (0);
  if (con->http.keep_alive && con->http.version >= HTTP_1_0)
  {
    if (httpPrintf(HTTP(con), "Connection: Keep-Alive\r\n") < 0)
      return (0);
    if (httpPrintf(HTTP(con), "Keep-Alive: timeout=%d\r\n",
                   KeepAliveTimeout) < 0)
      return (0);
  }
  if (code == HTTP_METHOD_NOT_ALLOWED)
    if (httpPrintf(HTTP(con), "Allow: GET, HEAD, OPTIONS, POST, PUT\r\n") < 0)
      return (0);

  if (code == HTTP_UNAUTHORIZED)
  {
    if (auth_type == CUPSD_AUTH_NONE)
    {
      if (!con->best || con->best->type <= CUPSD_AUTH_NONE)
	auth_type = DefaultAuthType;
      else
	auth_type = con->best->type;
    }

    auth_str[0] = '\0';

    if (auth_type == CUPSD_AUTH_BASIC || auth_type == CUPSD_AUTH_BASICDIGEST)
      strlcpy(auth_str, "Basic realm=\"CUPS\"", sizeof(auth_str));
    else if (auth_type == CUPSD_AUTH_DIGEST)
      snprintf(auth_str, sizeof(auth_str), "Digest realm=\"CUPS\", nonce=\"%s\"",
	       con->http.hostname);
#ifdef HAVE_GSSAPI
    else if (auth_type == CUPSD_AUTH_NEGOTIATE && con->gss_output_token.length == 0)
      strlcpy(auth_str, "Negotiate", sizeof(auth_str));
#endif /* HAVE_GSSAPI */

    if (con->best && auth_type != CUPSD_AUTH_NEGOTIATE &&
        !strcasecmp(con->http.hostname, "localhost"))
    {
     /*
      * Add a "trc" (try root certification) parameter for local non-Kerberos
      * requests when the request requires system group membership - then the
      * client knows the root certificate can/should be used.
      *
      * Also, for Mac OS X we also look for @AUTHKEY and add an "authkey"
      * parameter as needed...
      */

      int 	i;			/* Looping var */
      char	*auth_key;		/* Auth key buffer */
      size_t	auth_size;		/* Size of remaining buffer */

      auth_key  = auth_str + strlen(auth_str);
      auth_size = sizeof(auth_str) - (auth_key - auth_str);

      for (i = 0; i < con->best->num_names; i ++)
      {
#ifdef HAVE_AUTHORIZATION_H
	if (!strncasecmp(con->best->names[i], "@AUTHKEY(", 9))
	{
	  snprintf(auth_key, auth_size, ", authkey=\"%s\"",
	           con->best->names[i] + 9);
	  /* end parenthesis is stripped in conf.c */
	  break;
        }
	else
#endif /* HAVE_AUTHORIZATION_H */
	if (!strcasecmp(con->best->names[i], "@SYSTEM"))
	{
#ifdef HAVE_AUTHORIZATION_H
	  if (SystemGroupAuthKey)
	    snprintf(auth_key, auth_size,
	             ", authkey=\"%s\", trc=\"y\"",
		     SystemGroupAuthKey);
          else
#else
	  strlcpy(auth_key, ", trc=\"y\"", auth_size);
#endif /* HAVE_AUTHORIZATION_H */
	  break;
	}
      }
    }

    if (auth_str[0])
    {
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "cupsdSendHeader: %d WWW-Authenticate: %s", con->http.fd,
                      auth_str);

      if (httpPrintf(HTTP(con), "WWW-Authenticate: %s\r\n", auth_str) < 0)
        return (0);
    }
  }

#ifdef HAVE_GSSAPI
 /*
  * WWW-Authenticate: Negotiate can be included even for
  * non-401 replies...
  */

  if (con->gss_output_token.length > 0 && con->gss_output_token.length <= 65536)
  {
    OM_uint32	minor_status;		/* Minor status code */
    int		bufsize;		/* Size of output token buffer */


    bufsize = con->gss_output_token.length * 4 / 3 + 2;

    if (bufsize > gss_bufsize)
    {
      char	*buf;			/* New buffer */


      bufsize = (bufsize + 1023) & 1023;/* Round up */

      if (gss_buf)
        buf = realloc(gss_buf, bufsize);
      else
        buf = malloc(bufsize);

      if (!buf)
      {
	cupsdLogMessage(CUPSD_LOG_ERROR,
			"Unable to allocate %d bytes for Kerberos credentials!",
			bufsize);
	return (0);
      }

      gss_buf     = buf;
      gss_bufsize = bufsize;
    }

    httpEncode64_2(gss_buf, gss_bufsize,
		   con->gss_output_token.value,
		   con->gss_output_token.length);
    gss_release_buffer(&minor_status, &con->gss_output_token);

    cupsdLogMessage(CUPSD_LOG_DEBUG,
		    "cupsdSendHeader: WWW-Authenticate: Negotiate %s", gss_buf);

    if (httpPrintf(HTTP(con), "WWW-Authenticate: Negotiate %s\r\n",
                   gss_buf) < 0)
      return (0);
  }
  else if (con->gss_output_token.length > 65536)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Kerberos credentials larger than 64k (%d)!",
		    (int)con->gss_output_token.length);
    return (0);
  }
#endif /* HAVE_GSSAPI */

  if (con->language && strcmp(con->language->language, "C"))
  {
    if (httpPrintf(HTTP(con), "Content-Language: %s\r\n",
                   con->language->language) < 0)
      return (0);
  }

  if (type)
  {
    if (!strcmp(type, "text/html"))
    {
      if (httpPrintf(HTTP(con),
                     "Content-Type: text/html; charset=utf-8\r\n") < 0)
        return (0);
    }
    else if (httpPrintf(HTTP(con), "Content-Type: %s\r\n", type) < 0)
      return (0);
  }

  return (1);
}


/*
 * 'cupsdUpdateCGI()' - Read status messages from CGI scripts and programs.
 */

void
cupsdUpdateCGI(void)
{
  char		*ptr,			/* Pointer to end of line in buffer */
		message[1024];		/* Pointer to message text */
  int		loglevel;		/* Log level for message */


  while ((ptr = cupsdStatBufUpdate(CGIStatusBuffer, &loglevel,
                                   message, sizeof(message))) != NULL)
  {
    if (loglevel == CUPSD_LOG_INFO)
      cupsdLogMessage(CUPSD_LOG_INFO, "%s", message);

    if (!strchr(CGIStatusBuffer->buffer, '\n'))
      break;
  }

  if (ptr == NULL && !CGIStatusBuffer->bufused)
  {
   /*
    * Fatal error on pipe - should never happen!
    */

    cupsdLogMessage(CUPSD_LOG_CRIT,
                    "cupsdUpdateCGI: error reading from CGI error pipe - %s",
                    strerror(errno));
  }
}


/*
 * 'cupsdWriteClient()' - Write data to a client as needed.
 */

void
cupsdWriteClient(cupsd_client_t *con)	/* I - Client connection */
{
  int		bytes,			/* Number of bytes written */
		field_col;		/* Current column */
  char		*bufptr,		/* Pointer into buffer */
		*bufend;		/* Pointer to end of buffer */
  ipp_state_t	ipp_state;		/* IPP state value */


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cupsdWriteClient(con=%p(%d)) response=%p(%d), file=%d "
		  "pipe_pid=%d state=%d",
                  con, con->http.fd, con->response,
		  con->response ? con->response->state : -1,
		  con->file, con->pipe_pid, con->http.state);

  if (con->http.state != HTTP_GET_SEND &&
      con->http.state != HTTP_POST_SEND)
  {
   /*
    * If we get called in the wrong state, then something went wrong with the
    * connection and we need to shut it down...
    */

    cupsdLogMessage(CUPSD_LOG_DEBUG,
		    "cupsdWriteClient: %d Closing on unknown HTTP state %d",
		    con->http.fd, con->http.state);
    cupsdCloseClient(con);
    return;
  }

  if (con->pipe_pid)
  {
   /*
    * Make sure we select on the CGI output...
    */

    cupsdAddSelect(con->file, (cupsd_selfunc_t)write_pipe, NULL, con);

    if (!con->file_ready)
    {
     /*
      * Try again later when there is CGI output available...
      */

      cupsdRemoveSelect(con->http.fd);
      return;
    }

    con->file_ready = 0;
  }

  if (con->response && con->response->state != IPP_DATA)
  {
    ipp_state = ippWrite(HTTP(con), con->response);
    bytes     = ipp_state != IPP_ERROR &&
                (con->file >= 0 || ipp_state != IPP_DATA);
  }
  else if ((bytes = read(con->file, con->header,
			 sizeof(con->header) - con->header_used)) > 0)
  {
    con->header_used += bytes;

    if (con->pipe_pid && !con->got_fields)
    {
     /*
      * Inspect the data for Content-Type and other fields.
      */

      for (bufptr = con->header, bufend = con->header + con->header_used,
               field_col = 0;
           !con->got_fields && bufptr < bufend;
	   bufptr ++)
      {
        if (*bufptr == '\n')
	{
	 /*
	  * Send line to client...
	  */

	  if (bufptr > con->header && bufptr[-1] == '\r')
	    bufptr[-1] = '\0';
	  *bufptr++ = '\0';

          cupsdLogMessage(CUPSD_LOG_DEBUG, "Script header: %s", con->header);

          if (!con->sent_header)
	  {
	   /*
	    * Handle redirection and CGI status codes...
	    */

            if (!strncasecmp(con->header, "Location:", 9))
	    {
  	      cupsdSendHeader(con, HTTP_SEE_OTHER, NULL, CUPSD_AUTH_NONE);
	      con->sent_header = 2;

	      if (httpPrintf(HTTP(con), "Content-Length: 0\r\n") < 0)
		return;
	    }
	    else if (!strncasecmp(con->header, "Status:", 7))
	    {
  	      cupsdSendError(con, (http_status_t)atoi(con->header + 7),
	                     CUPSD_AUTH_NONE);
	      con->sent_header = 2;
	    }
	    else
	    {
  	      cupsdSendHeader(con, HTTP_OK, NULL, CUPSD_AUTH_NONE);
	      con->sent_header = 1;

	      if (con->http.version == HTTP_1_1)
	      {
		if (httpPrintf(HTTP(con), "Transfer-Encoding: chunked\r\n") < 0)
		  return;
	      }
            }
	  }

	  if (strncasecmp(con->header, "Status:", 7))
	    httpPrintf(HTTP(con), "%s\r\n", con->header);

         /*
	  * Update buffer...
	  */

	  con->header_used -= bufptr - con->header;

	  if (con->header_used > 0)
	    memmove(con->header, bufptr, con->header_used);

	  bufptr = con->header - 1;

         /*
	  * See if the line was empty...
	  */

	  if (field_col == 0)
	  {
	    con->got_fields = 1;

            if (cupsdFlushHeader(con) < 0)
	    {
	      cupsdCloseClient(con);
	      return;
	    }

	    if (con->http.version == HTTP_1_1)
	      con->http.data_encoding = HTTP_ENCODE_CHUNKED;
          }
	  else
	    field_col = 0;
	}
	else if (*bufptr != '\r')
	  field_col ++;
      }

      if (!con->got_fields)
      {
        con->http.activity = time(NULL);
        return;
      }
    }

    if (con->header_used > 0)
    {
      if (httpWrite2(HTTP(con), con->header, con->header_used) < 0)
      {
	cupsdLogMessage(CUPSD_LOG_DEBUG,
			"cupsdWriteClient: %d Closing for error %d (%s)",
			con->http.fd, con->http.error,
			strerror(con->http.error));
	cupsdCloseClient(con);
	return;
      }

      if (con->http.data_encoding == HTTP_ENCODE_CHUNKED)
        httpFlushWrite(HTTP(con));

      con->bytes += con->header_used;

      if (con->http.state == HTTP_WAITING)
	bytes = 0;
      else
        bytes = con->header_used;

      con->header_used = 0;
    }
  }

  if (bytes <= 0 ||
      (con->http.state != HTTP_GET_SEND && con->http.state != HTTP_POST_SEND))
  {
    if (!con->sent_header && con->pipe_pid)
      cupsdSendError(con, HTTP_SERVER_ERROR, CUPSD_AUTH_NONE);
    else
    {
    #ifdef OPEN_DEBUG
      cupsdLogRequest(con, HTTP_OK);
    #endif
      httpFlushWrite(HTTP(con));

      if (con->http.data_encoding == HTTP_ENCODE_CHUNKED && con->sent_header == 1)
      {
	if (httpWrite2(HTTP(con), "", 0) < 0)
	{
	  cupsdLogMessage(CUPSD_LOG_DEBUG,
			  "cupsdWriteClient: %d Closing for error %d (%s)",
			  con->http.fd, con->http.error,
			  strerror(con->http.error));
	  cupsdCloseClient(con);
	  return;
	}
      }
    }

    con->http.state = HTTP_WAITING;

    cupsdAddSelect(con->http.fd, (cupsd_selfunc_t)cupsdReadClient, NULL, con);

    if (con->file >= 0)
    {
      cupsdRemoveSelect(con->file);

      if (con->pipe_pid)
	cupsdEndProcess(con->pipe_pid, 0);

      close(con->file);
      con->file     = -1;
      con->pipe_pid = 0;
    }

    if (con->filename)
    {
      unlink(con->filename);
      cupsdClearString(&con->filename);
    }

    if (con->request)
    {
      ippDelete(con->request);
      con->request = NULL;
    }

    if (con->response)
    {
      ippDelete(con->response);
      con->response = NULL;
    }

    cupsdClearString(&con->command);
    cupsdClearString(&con->options);
    cupsdClearString(&con->query_string);

    if (!con->http.keep_alive)
    {
      cupsdLogMessage(CUPSD_LOG_DEBUG,
		      "cupsdWriteClient: %d Closing because Keep-Alive disabled",
		      con->http.fd);
      cupsdCloseClient(con);
      return;
    }
    else
    {
      cupsArrayRemove(ActiveClients, con);
      cupsdSetBusyState();
    }
  }

  con->http.activity = time(NULL);
}


/*
 * 'check_if_modified()' - Decode an "If-Modified-Since" line.
 */

static int				/* O - 1 if modified since */
check_if_modified(
    cupsd_client_t *con,		/* I - Client connection */
    struct stat    *filestats)		/* I - File information */
{
  char		*ptr;			/* Pointer into field */
  time_t	date;			/* Time/date value */
  off_t		size;			/* Size/length value */


  size = 0;
  date = 0;
  ptr  = con->http.fields[HTTP_FIELD_IF_MODIFIED_SINCE];

  if (*ptr == '\0')
    return (1);

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "check_if_modified(con=%p(%d), "
		  "filestats=%p(" CUPS_LLFMT ", %d)) If-Modified-Since=\"%s\"",
                  con, con->http.fd, filestats, CUPS_LLCAST filestats->st_size,
		  (int)filestats->st_mtime, ptr);

  while (*ptr != '\0')
  {
    while (isspace(*ptr) || *ptr == ';')
      ptr ++;

    if (strncasecmp(ptr, "length=", 7) == 0)
    {
      ptr += 7;
      size = strtoll(ptr, NULL, 10);

      while (isdigit(*ptr))
        ptr ++;
    }
    else if (isalpha(*ptr))
    {
      date = httpGetDateTime(ptr);
      while (*ptr != '\0' && *ptr != ';')
        ptr ++;
    }
    else
      ptr ++;
  }

  return ((size != filestats->st_size && size != 0) ||
          (date < filestats->st_mtime && date != 0) ||
	  (size == 0 && date == 0));
}


/*
 * 'compare_clients()' - Compare two client connections.
 */

static int				/* O - Result of comparison */
compare_clients(cupsd_client_t *a,	/* I - First client */
                cupsd_client_t *b,	/* I - Second client */
                void           *data)	/* I - User data (not used) */
{
  (void)data;

  if (a == b)
    return (0);
  else if (a < b)
    return (-1);
  else
    return (1);
}


/*
 * 'data_ready()' - Check whether data is available from a client.
 */

static int				/* O - 1 if data is ready, 0 otherwise */
data_ready(cupsd_client_t *con)		/* I - Client */
{
  if (con->http.used > 0)
    return (1);
#ifdef HAVE_SSL
  else if (con->http.tls)
  {
#  ifdef HAVE_LIBSSL
    if (SSL_pending((SSL *)(con->http.tls)))
      return (1);
#  elif defined(HAVE_GNUTLS)
    if (gnutls_record_check_pending(((http_tls_t *)(con->http.tls))->session))
      return (1);
#  elif defined(HAVE_CDSASSL)
    size_t bytes;			/* Bytes that are available */

    if (!SSLGetBufferedReadSize(((http_tls_t *)(con->http.tls))->session,
                                &bytes) && bytes > 0)
      return (1);
#  endif /* HAVE_LIBSSL */
  }
#endif /* HAVE_SSL */

  return (0);
}


#ifdef HAVE_SSL
/*
 * 'encrypt_client()' - Enable encryption for the client...
 */

static int				/* O - 1 on success, 0 on error */
encrypt_client(cupsd_client_t *con)	/* I - Client to encrypt */
{
#  ifdef HAVE_LIBSSL
  SSL_CTX	*context;		/* Context for encryption */
  SSL		*conn;			/* Connection for encryption */
  BIO		*bio;			/* BIO data */
  unsigned long	error;			/* Error code */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "encrypt_client(con=%p(%d))", con,
                  con->http.fd);

 /*
  * Verify that we have a certificate...
  */

  if (access(ServerKey, 0) || access(ServerCertificate, 0))
  {
   /*
    * Nope, make a self-signed certificate...
    */

    if (!make_certificate(con))
      return (0);
  }

 /*
  * Create the SSL context and accept the connection...
  */

  context = SSL_CTX_new(SSLv23_server_method());

  SSL_CTX_set_options(context, SSL_OP_NO_SSLv2); /* Only use SSLv3 or TLS */
  if (SSLOptions & CUPSD_SSL_NOEMPTY)
    SSL_CTX_set_options(context, SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS);
  SSL_CTX_use_PrivateKey_file(context, ServerKey, SSL_FILETYPE_PEM);
  SSL_CTX_use_certificate_chain_file(context, ServerCertificate);

  bio = BIO_new(_httpBIOMethods());
  BIO_ctrl(bio, BIO_C_SET_FILE_PTR, 0, (char *)HTTP(con));

  conn = SSL_new(context);
  SSL_set_bio(conn, bio, bio);

  if (SSL_accept(conn) != 1)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to encrypt connection from %s!",
                    con->http.hostname);

    while ((error = ERR_get_error()) != 0)
      cupsdLogMessage(CUPSD_LOG_ERROR, "%s", ERR_error_string(error, NULL));

    SSL_CTX_free(context);
    SSL_free(conn);
    return (0);
  }

  cupsdLogMessage(CUPSD_LOG_DEBUG, "Connection from %s now encrypted.",
                  con->http.hostname);

  con->http.tls = conn;
  return (1);

#  elif defined(HAVE_GNUTLS)
  http_tls_t	*conn;			/* TLS session object */
  int		error;			/* Error code */
  gnutls_certificate_server_credentials *credentials;
					/* TLS credentials */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "encrypt_client(con=%p(%d))", con,
                  con->http.fd);

 /*
  * Verify that we have a certificate...
  */

  if (access(ServerKey, 0) || access(ServerCertificate, 0))
  {
   /*
    * Nope, make a self-signed certificate...
    */

    if (!make_certificate(con))
      return (0);
  }

 /*
  * Create the SSL object and perform the SSL handshake...
  */

  conn = (http_tls_t *)malloc(sizeof(http_tls_t));

  if (conn == NULL)
    return (0);

  credentials = (gnutls_certificate_server_credentials *)
                    malloc(sizeof(gnutls_certificate_server_credentials));
  if (credentials == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to encrypt connection from %s - %s",
                    con->http.hostname, strerror(errno));

    free(conn);
    return (0);
  }

  gnutls_certificate_allocate_credentials(credentials);
  gnutls_certificate_set_x509_key_file(*credentials, ServerCertificate,
				       ServerKey, GNUTLS_X509_FMT_PEM);

  gnutls_init(&(conn->session), GNUTLS_SERVER);
  gnutls_set_default_priority(conn->session);
  gnutls_credentials_set(conn->session, GNUTLS_CRD_CERTIFICATE, *credentials);
  gnutls_transport_set_ptr(conn->session, (gnutls_transport_ptr)HTTP(con));
  gnutls_transport_set_pull_function(conn->session, _httpReadGNUTLS);
  gnutls_transport_set_push_function(conn->session, _httpWriteGNUTLS);

  error = gnutls_handshake(conn->session);

  if (error != GNUTLS_E_SUCCESS)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to encrypt connection from %s - %s",
                    con->http.hostname, gnutls_strerror(error));

    gnutls_deinit(conn->session);
    gnutls_certificate_free_credentials(*credentials);
    free(conn);
    free(credentials);
    return (0);
  }

  cupsdLogMessage(CUPSD_LOG_DEBUG, "Connection from %s now encrypted.",
                  con->http.hostname);

  conn->credentials = credentials;
  con->http.tls = conn;
  return (1);

#  elif defined(HAVE_CDSASSL)
  OSStatus	error;			/* Error code */
  http_tls_t	*conn;			/* CDSA connection information */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "encrypt_client(con=%p(%d))", con,
                  con->http.fd);

  if ((conn = (http_tls_t *)malloc(sizeof(http_tls_t))) == NULL)
    return (0);

  error            = 0;
  conn->session    = NULL;
  conn->certsArray = get_cdsa_certificate(con);

  if (!conn->certsArray)
  {
   /*
    * No keychain (yet), make a self-signed certificate...
    */

    if (make_certificate(con))
      conn->certsArray = get_cdsa_certificate(con);
  }

  if (!conn->certsArray)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
        	    "Could not find signing key in keychain \"%s\"",
		    ServerCertificate);
    error = errSSLBadCert; /* errSSLBadConfiguration is a better choice, but not available on 10.2.x */
  }

  if (!error)
    error = SSLNewContext(true, &conn->session);

  if (!error)
    error = SSLSetIOFuncs(conn->session, _httpReadCDSA, _httpWriteCDSA);

  if (!error)
    error = SSLSetProtocolVersionEnabled(conn->session, kSSLProtocol2, false);

  if (!error)
    error = SSLSetConnection(conn->session, HTTP(con));

  if (!error)
    error = SSLSetAllowsExpiredCerts(conn->session, true);

  if (!error)
    error = SSLSetAllowsAnyRoot(conn->session, true);

  if (!error)
    error = SSLSetCertificate(conn->session, conn->certsArray);

  if (!error)
  {
   /*
    * Perform SSL/TLS handshake
    */

    while ((error = SSLHandshake(conn->session)) == errSSLWouldBlock)
      usleep(1000);
  }

  if (error)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to encrypt connection from %s - %s (%d)",
                    con->http.hostname, cssmErrorString(error), (int)error);

    con->http.error  = error;
    con->http.status = HTTP_ERROR;

    if (conn->session)
      SSLDisposeContext(conn->session);

    if (conn->certsArray)
      CFRelease(conn->certsArray);

    free(conn);

    return (0);
  }

  cupsdLogMessage(CUPSD_LOG_DEBUG, "Connection from %s now encrypted.",
                  con->http.hostname);

  con->http.tls = conn;
  return (1);

#  endif /* HAVE_LIBSSL */
}
#endif /* HAVE_SSL */


#ifdef HAVE_CDSASSL
/*
 * 'get_cdsa_certificate()' - Get a SSL/TLS certificate from the System keychain.
 */

static CFArrayRef			/* O - Array of certificates */
get_cdsa_certificate(
    cupsd_client_t *con)		/* I - Client connection */
{
  OSStatus		err;		/* Error info */
  SecKeychainRef	keychain;	/* Keychain reference */
  SecIdentitySearchRef	search;		/* Search reference */
  SecIdentityRef	identity;	/* Identity */
  CFArrayRef		certificates = NULL;
					/* Certificate array */


  if ((err = SecKeychainOpen(ServerCertificate, &keychain)))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Cannot open keychain \"%s\" - %s (%d)",
	            ServerCertificate, cssmErrorString(err), (int)err);
    return (NULL);
  }

#  if HAVE_SECIDENTITYSEARCHCREATEWITHPOLICY
 /*
  * Use a policy to search for valid certificates who's common name matches the
  * servername...
  */

  SecPolicySearchRef	policy_search;	/* Policy search ref */
  SecPolicyRef		policy;		/* Policy ref */
  CSSM_DATA		options;	/* Policy options */
  CSSM_APPLE_TP_SSL_OPTIONS
			ssl_options;	/* SSL Option for hostname */
  char			localname[1024];/* Local hostname */


  if (SecPolicySearchCreate(CSSM_CERT_X_509v3, &CSSMOID_APPLE_TP_SSL, 
			    NULL, &policy_search))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Cannot create a policy search reference");
    CFRelease(keychain);
    return (NULL);
  }

  if (SecPolicySearchCopyNext(policy_search, &policy))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
		    "Cannot find a policy to use for searching");
    CFRelease(keychain);
    CFRelease(policy_search);
    return (NULL);
  }

  memset(&ssl_options, 0, sizeof(ssl_options));
  ssl_options.Version = CSSM_APPLE_TP_SSL_OPTS_VERSION;
  ssl_options.ServerName = con->servername;
  ssl_options.ServerNameLen = strlen(con->servername);

  cupsdLogMessage(CUPSD_LOG_DEBUG,
                  "get_cdsa_certificate: Looking for certs for \"%s\"...",
		  con->servername);

  options.Data = (uint8 *)&ssl_options;
  options.Length = sizeof(ssl_options);

  if (SecPolicySetValue(policy, &options))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
		    "Cannot set policy value to use for searching");
    CFRelease(keychain);
    CFRelease(policy_search);
    return (NULL);
  }

  err = SecIdentitySearchCreateWithPolicy(policy, NULL, CSSM_KEYUSE_SIGN,
					  keychain, FALSE, &search);
  if (err && DNSSDHostName)
  {
   /*
    * Search for the connection server name failed; try the DNS-SD .local
    * hostname instead...
    */

    snprintf(localname, sizeof(localname), "%s.local", DNSSDHostName);

    ssl_options.ServerName    = localname;
    ssl_options.ServerNameLen = strlen(localname);

    cupsdLogMessage(CUPSD_LOG_DEBUG,
		    "get_cdsa_certificate: Looking for certs for \"%s\"...",
		    localname);

    if (SecPolicySetValue(policy, &options))
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
		      "Cannot set policy value to use for searching");
      CFRelease(keychain);
      CFRelease(policy_search);
      return (NULL);
    }

    err = SecIdentitySearchCreateWithPolicy(policy, NULL, CSSM_KEYUSE_SIGN,
					    keychain, FALSE, &search);
  }

#  else
 /*
  * Assume there is exactly one SecIdentity in the keychain...
  */

  err = SecIdentitySearchCreate(keychain, CSSM_KEYUSE_SIGN, &search);
#  endif /* HAVE_SECIDENTITYSEARCHCREATEWITHPOLICY */

  if (err)
    cupsdLogMessage(CUPSD_LOG_DEBUG,
		    "Cannot create keychain search reference: %s (%d)", 
		    cssmErrorString(err), (int)err);
  else
  {
    if ((err = SecIdentitySearchCopyNext(search, &identity)))
    {
      cupsdLogMessage(CUPSD_LOG_DEBUG,
		      "Cannot find signing key in keychain \"%s\": %s (%d)",
		      ServerCertificate, cssmErrorString(err), (int)err);
    }
    else
    {
      if (CFGetTypeID(identity) != SecIdentityGetTypeID())
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "SecIdentitySearchCopyNext CFTypeID failure!");
      else
      {
      if ((certificates = CFArrayCreate(NULL, (const void **)&identity, 
				      1, &kCFTypeArrayCallBacks)) == NULL)
	cupsdLogMessage(CUPSD_LOG_ERROR, "Cannot create certificate array");
      }

      CFRelease(identity);
    }

    CFRelease(search);
  }

#  if HAVE_SECIDENTITYSEARCHCREATEWITHPOLICY
  CFRelease(policy);
  CFRelease(policy_search);
#  endif /* HAVE_SECIDENTITYSEARCHCREATEWITHPOLICY */

  return (certificates);
}
#endif /* HAVE_CDSASSL */


/*
 * 'get_file()' - Get a filename and state info.
 */

static char *				/* O  - Real filename */
get_file(cupsd_client_t *con,		/* I  - Client connection */
         struct stat    *filestats,	/* O  - File information */
         char           *filename,	/* IO - Filename buffer */
         int            len)		/* I  - Buffer length */
{
  int		status;			/* Status of filesystem calls */
  char		*ptr;			/* Pointer info filename */
  int		plen;			/* Remaining length after pointer */
  char		language[7];		/* Language subdirectory, if any */


 /*
  * Figure out the real filename...
  */

  language[0] = '\0';

  if (!strncmp(con->uri, "/ppd/", 5))
    snprintf(filename, len, "%s%s", ServerRoot, con->uri);
  else if (!strncmp(con->uri, "/rss/", 5) && !strchr(con->uri + 5, '/'))
    snprintf(filename, len, "%s/rss/%s", CacheDir, con->uri + 5);
  else if (!strncmp(con->uri, "/admin/conf/", 12))
    snprintf(filename, len, "%s%s", ServerRoot, con->uri + 11);
  else if (!strncmp(con->uri, "/admin/log/", 11))
  {
    if (!strncmp(con->uri + 11, "access_log", 10) && AccessLog[0] == '/')
      strlcpy(filename, AccessLog, len);
    else if (!strncmp(con->uri + 11, "error_log", 9) && ErrorLog[0] == '/')
      strlcpy(filename, ErrorLog, len);
    else if (!strncmp(con->uri + 11, "page_log", 8) && PageLog[0] == '/')
      strlcpy(filename, PageLog, len);
    else
      return (NULL);
  }
  else if (con->language)
  {
    snprintf(language, sizeof(language), "/%s", con->language->language);
    snprintf(filename, len, "%s%s%s", DocumentRoot, language, con->uri);
  }
  else
    snprintf(filename, len, "%s%s", DocumentRoot, con->uri);

  if ((ptr = strchr(filename, '?')) != NULL)
    *ptr = '\0';

 /*
  * Grab the status for this language; if there isn't a language-specific file
  * then fallback to the default one...
  */

  if ((status = stat(filename, filestats)) != 0 && language[0] &&
      strncmp(con->uri, "/ppd/", 5) &&
      strncmp(con->uri, "/admin/conf/", 12) &&
      strncmp(con->uri, "/admin/log/", 11))
  {
   /*
    * Drop the country code...
    */

    language[3] = '\0';
    snprintf(filename, len, "%s%s%s", DocumentRoot, language, con->uri);

    if ((ptr = strchr(filename, '?')) != NULL)
      *ptr = '\0';

    if ((status = stat(filename, filestats)) != 0)
    {
     /*
      * Drop the language prefix and try the root directory...
      */

      language[0] = '\0';
      snprintf(filename, len, "%s%s", DocumentRoot, con->uri);

      if ((ptr = strchr(filename, '?')) != NULL)
	*ptr = '\0';

      status = stat(filename, filestats);
    }
  }

 /*
  * If we're found a directory, get the index.html file instead...
  */

  if (!status && S_ISDIR(filestats->st_mode))
  {
   /*
    * Make sure the URI ends with a slash...
    */

    if (con->uri[strlen(con->uri) - 1] != '/')
      strlcat(con->uri, "/", sizeof(con->uri));

   /*
    * Find the directory index file, trying every language...
    */

    do
    {
      if (status && language[0])
      {
       /*
        * Try a different language subset...
	*/

	if (language[3])
	  language[0] = '\0';		/* Strip country code */
	else
	  language[0] = '\0';		/* Strip language */
      }

     /*
      * Look for the index file...
      */

      snprintf(filename, len, "%s%s%s", DocumentRoot, language, con->uri);

      if ((ptr = strchr(filename, '?')) != NULL)
	*ptr = '\0';

      ptr  = filename + strlen(filename);
      plen = len - (ptr - filename);

      strlcpy(ptr, "index.html", plen);
      status = stat(filename, filestats);

#ifdef HAVE_JAVA
      if (status)
      {
	strlcpy(ptr, "index.class", plen);
	status = stat(filename, filestats);
      }
#endif /* HAVE_JAVA */

#ifdef HAVE_PERL
      if (status)
      {
	strlcpy(ptr, "index.pl", plen);
	status = stat(filename, filestats);
      }
#endif /* HAVE_PERL */

#ifdef HAVE_PHP
      if (status)
      {
	strlcpy(ptr, "index.php", plen);
	status = stat(filename, filestats);
      }
#endif /* HAVE_PHP */

#ifdef HAVE_PYTHON
      if (status)
      {
	strlcpy(ptr, "index.pyc", plen);
	status = stat(filename, filestats);
      }

      if (status)
      {
	strlcpy(ptr, "index.py", plen);
	status = stat(filename, filestats);
      }
#endif /* HAVE_PYTHON */

    }
    while (status && language[0]);
  }

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "get_file(con=%p(%d), filestats=%p, filename=%p, len=%d) = "
		  "%s", con, con->http.fd, filestats, filename, len,
		  status ? "(null)" : filename);

  if (status)
    return (NULL);
  else
    return (filename);
}


/*
 * 'install_conf_file()' - Install a configuration file.
 */

static http_status_t			/* O - Status */
install_conf_file(cupsd_client_t *con)	/* I - Connection */
{
  cups_file_t	*in,			/* Input file */
		*out;			/* Output file */
  char		buffer[1024];		/* Copy buffer */
  int		bytes;			/* Number of bytes */
  char		conffile[1024],		/* Configuration filename */
		newfile[1024],		/* New config filename */
		oldfile[1024];		/* Old config filename */
  struct stat	confinfo;		/* Config file info */


 /*
  * First construct the filenames...
  */

  snprintf(conffile, sizeof(conffile), "%s%s", ServerRoot, con->uri + 11);
  snprintf(newfile, sizeof(newfile), "%s%s.N", ServerRoot, con->uri + 11);
  snprintf(oldfile, sizeof(oldfile), "%s%s.O", ServerRoot, con->uri + 11);

  cupsdLogMessage(CUPSD_LOG_INFO, "Installing config file \"%s\"...", conffile);

 /*
  * Get the owner, group, and permissions of the configuration file.
  * If it doesn't exist, assign it to the User and Group in the
  * cupsd.conf file with mode 0640 permissions.
  */

  if (stat(conffile, &confinfo))
  {
    confinfo.st_uid  = User;
    confinfo.st_gid  = Group;
    confinfo.st_mode = ConfigFilePerm;
  }

 /*
  * Open the request file and new config file...
  */

  if ((in = cupsFileOpen(con->filename, "rb")) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to open request file \"%s\" - %s",
                    con->filename, strerror(errno));
    return (HTTP_SERVER_ERROR);
  }

  if ((out = cupsFileOpen(newfile, "wb")) == NULL)
  {
    cupsFileClose(in);
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to open config file \"%s\" - %s",
                    newfile, strerror(errno));
    return (HTTP_SERVER_ERROR);
  }

  fchmod(cupsFileNumber(out), confinfo.st_mode);
  fchown(cupsFileNumber(out), confinfo.st_uid, confinfo.st_gid);

 /*
  * Copy from the request to the new config file...
  */

  while ((bytes = cupsFileRead(in, buffer, sizeof(buffer))) > 0)
    if (cupsFileWrite(out, buffer, bytes) < bytes)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to copy to config file \"%s\" - %s",
        	      newfile, strerror(errno));

      cupsFileClose(in);
      cupsFileClose(out);
      unlink(newfile);

      return (HTTP_SERVER_ERROR);
    }

 /*
  * Close the files...
  */

  cupsFileClose(in);
  if (cupsFileClose(out))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Error file closing config file \"%s\" - %s",
                    newfile, strerror(errno));

    unlink(newfile);

    return (HTTP_SERVER_ERROR);
  }

 /*
  * Remove the request file...
  */

  unlink(con->filename);
  cupsdClearString(&con->filename);

 /*
  * Unlink the old backup, rename the current config file to the backup
  * filename, and rename the new config file to the config file name...
  */

  if (unlink(oldfile))
    if (errno != ENOENT)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to remove backup config file \"%s\" - %s",
        	      oldfile, strerror(errno));

      unlink(newfile);

      return (HTTP_SERVER_ERROR);
    }

  if (rename(conffile, oldfile))
    if (errno != ENOENT)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to rename old config file \"%s\" - %s",
        	      conffile, strerror(errno));

      unlink(newfile);

      return (HTTP_SERVER_ERROR);
    }

  if (rename(newfile, conffile))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to rename new config file \"%s\" - %s",
                    newfile, strerror(errno));

    rename(oldfile, conffile);
    unlink(newfile);

    return (HTTP_SERVER_ERROR);
  }

 /*
  * If the cupsd.conf file was updated, set the NeedReload flag...
  */

  if (!strcmp(con->uri, "/admin/conf/cupsd.conf"))
    NeedReload = RELOAD_CUPSD;
  else
    NeedReload = RELOAD_ALL;

  ReloadTime = time(NULL);

 /*
  * Return that the file was created successfully...
  */

  return (HTTP_CREATED);
}


/*
 * 'is_cgi()' - Is the resource a CGI script/program?
 */

static int				/* O - 1 = CGI, 0 = file */
is_cgi(cupsd_client_t *con,		/* I - Client connection */
       const char     *filename,	/* I - Real filename */
       struct stat    *filestats,	/* I - File information */
       mime_type_t    *type)		/* I - MIME type */
{
  const char	*options;		/* Options on URL */


 /*
  * Get the options, if any...
  */

  if ((options = strchr(con->uri, '?')) != NULL)
  {
    options ++;
    cupsdSetStringf(&(con->query_string), "QUERY_STRING=%s", options);
  }

 /*
  * Check for known types...
  */

  if (!type || strcasecmp(type->super, "application"))
  {
    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		    "type=%s/%s) = 0", con, con->http.fd, filename, filestats,
		    type ? type->super : "unknown",
		    type ? type->type : "unknown");
    return (0);
  }

  if (!strcasecmp(type->type, "x-httpd-cgi") &&
      (filestats->st_mode & 0111))
  {
   /*
    * "application/x-httpd-cgi" is a CGI script.
    */

    cupsdSetString(&con->command, filename);

    if (options)
      cupsdSetStringf(&con->options, " %s", options);

    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		    "type=%s/%s) = 1", con, con->http.fd, filename, filestats,
		    type ? type->super : "unknown",
		    type ? type->type : "unknown");
    return (1);
  }
#ifdef HAVE_JAVA
  else if (!strcasecmp(type->type, "x-httpd-java"))
  {
   /*
    * "application/x-httpd-java" is a Java servlet.
    */

    cupsdSetString(&con->command, CUPS_JAVA);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		    "type=%s/%s) = 1", con, con->http.fd, filename, filestats,
		    type ? type->super : "unknown",
		    type ? type->type : "unknown");
    return (1);
  }
#endif /* HAVE_JAVA */
#ifdef HAVE_PERL
  else if (!strcasecmp(type->type, "x-httpd-perl"))
  {
   /*
    * "application/x-httpd-perl" is a Perl page.
    */

    cupsdSetString(&con->command, CUPS_PERL);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		    "type=%s/%s) = 1", con, con->http.fd, filename, filestats,
		    type ? type->super : "unknown",
		    type ? type->type : "unknown");
    return (1);
  }
#endif /* HAVE_PERL */
#ifdef HAVE_PHP
  else if (!strcasecmp(type->type, "x-httpd-php"))
  {
   /*
    * "application/x-httpd-php" is a PHP page.
    */

    cupsdSetString(&con->command, CUPS_PHP);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		    "type=%s/%s) = 1", con, con->http.fd, filename, filestats,
		    type ? type->super : "unknown",
		    type ? type->type : "unknown");
    return (1);
  }
#endif /* HAVE_PHP */
#ifdef HAVE_PYTHON
  else if (!strcasecmp(type->type, "x-httpd-python"))
  {
   /*
    * "application/x-httpd-python" is a Python page.
    */

    cupsdSetString(&con->command, CUPS_PYTHON);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		    "type=%s/%s) = 1", con, con->http.fd, filename, filestats,
		    type ? type->super : "unknown",
		    type ? type->type : "unknown");
    return (1);
  }
#endif /* HAVE_PYTHON */

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
		  "is_cgi(con=%p(%d), filename=\"%s\", filestats=%p, "
		  "type=%s/%s) = 0", con, con->http.fd, filename, filestats,
		  type ? type->super : "unknown",
		  type ? type->type : "unknown");
  return (0);
}


/*
 * 'is_path_absolute()' - Is a path absolute and free of relative elements (i.e. "..").
 */

static int				/* O - 0 if relative, 1 if absolute */
is_path_absolute(const char *path)	/* I - Input path */
{
 /*
  * Check for a leading slash...
  */

  if (path[0] != '/')
    return (0);

  /* Add by y00183935@2014.4.26 for CVE-2014-2856. http://www.cups.org/str.php?L4356 */
  /*
  * Check for "<" or quotes in the path and reject since this is probably
  * someone trying to inject HTML...
  */

  if (strchr(path, '<') != NULL || strchr(path, '\"') != NULL || strchr(path, '\'') != NULL)
    return (0);
  /* Add by y00183935@2014.4.26 for CVE-2014-2856. http://www.cups.org/str.php?L4356 */

 /*
  * Check for "/.." in the path...
  */

  while ((path = strstr(path, "/..")) != NULL)
  {
    if (!path[3] || path[3] == '/')
      return (0);

    path ++;
  }

 /*
  * If we haven't found any relative paths, return 1 indicating an
  * absolute path...
  */

  return (1);
}


#ifdef HAVE_SSL
/*
 * 'make_certificate()' - Make a self-signed SSL/TLS certificate.
 */

static int				/* O - 1 on success, 0 on failure */
make_certificate(cupsd_client_t *con)	/* I - Client connection */
{
#if defined(HAVE_LIBSSL) && defined(HAVE_WAITPID)
  int		pid,			/* Process ID of command */
		status;			/* Status of command */
  char		command[1024],		/* Command */
		*argv[12],		/* Command-line arguments */
		*envp[MAX_ENV + 1],	/* Environment variables */
		infofile[1024],		/* Type-in information for cert */
		seedfile[1024];		/* Random number seed file */
  int		envc,			/* Number of environment variables */
		bytes;			/* Bytes written */
  cups_file_t	*fp;			/* Seed/info file */
  int		infofd;			/* Info file descriptor */


 /*
  * Run the "openssl" command to seed the random number generator and
  * generate a self-signed certificate that is good for 10 years:
  *
  *     openssl rand -rand seedfile 1
  *
  *     openssl req -new -x509 -keyout ServerKey \
  *             -out ServerCertificate -days 3650 -nodes
  *
  * The seeding step is crucial in ensuring that the openssl command
  * does not block on systems without sufficient entropy...
  */

  if (!cupsFileFind("openssl", getenv("PATH"), 1, command, sizeof(command)))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "No SSL certificate and openssl command not found!");
    return (0);
  }

  if (access("/dev/urandom", 0))
  {
   /*
    * If the system doesn't provide /dev/urandom, then any random source
    * will probably be blocking-style, so generate some random data to
    * use as a seed for the certificate.  Note that we have already
    * seeded the random number generator in cupsdInitCerts()...
    */

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Seeding the random number generator...");

   /*
    * Write the seed file...
    */

    if ((fp = cupsTempFile2(seedfile, sizeof(seedfile))) == NULL)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to create seed file %s - %s",
                      seedfile, strerror(errno));
      return (0);
    }

    for (bytes = 0; bytes < 262144; bytes ++)
      cupsFilePutChar(fp, random());

    cupsFileClose(fp);

   /*
    * Run the openssl command to seed its random number generator...
    */

    argv[0] = "openssl";
    argv[1] = "rand";
    argv[2] = "-rand";
    argv[3] = seedfile;
    argv[4] = "1";
    argv[5] = NULL;

    envc = cupsdLoadEnv(envp, MAX_ENV);
    envp[envc] = NULL;
//printf("==file:%s,line:%d,func:%s=job->status=%d=\n",__FILE__,__LINE__,__FUNCTION__,job->status);
    if (!cupsdStartProcess(command, argv, envp, -1, -1, -1, -1, -1, 1, NULL,
                           NULL, &pid))
    {
      unlink(seedfile);
      return (0);
    }

    while (waitpid(pid, &status, 0) < 0)
      if (errno != EINTR)
      {
	status = 1;
	break;
      }

    cupsdFinishProcess(pid, command, sizeof(command), NULL);

   /*
    * Remove the seed file, as it is no longer needed...
    */

    unlink(seedfile);

    if (status)
    {
      if (WIFEXITED(status))
	cupsdLogMessage(CUPSD_LOG_ERROR,
                	"Unable to seed random number generator - "
			"the openssl command stopped with status %d!",
	        	WEXITSTATUS(status));
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
                	"Unable to seed random number generator - "
			"the openssl command crashed on signal %d!",
	        	WTERMSIG(status));

      return (0);
    }
  }

 /*
  * Create a file with the certificate information fields...
  *
  * Note: This assumes that the default questions are asked by the openssl
  * command...
  */

  if ((fp = cupsTempFile2(infofile, sizeof(infofile))) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to create certificate information file %s - %s",
                    infofile, strerror(errno));
    return (0);
  }

  cupsFilePrintf(fp, ".\n.\n.\n%s\n.\n%s\n%s\n",
                 ServerName, ServerName, ServerAdmin);
  cupsFileClose(fp);

  cupsdLogMessage(CUPSD_LOG_INFO,
                  "Generating SSL server key and certificate...");

  argv[0]  = "openssl";
  argv[1]  = "req";
  argv[2]  = "-new";
  argv[3]  = "-x509";
  argv[4]  = "-keyout";
  argv[5]  = ServerKey;
  argv[6]  = "-out";
  argv[7]  = ServerCertificate;
  argv[8]  = "-days";
  argv[9]  = "3650";
  argv[10] = "-nodes";
  argv[11] = NULL;

  cupsdLoadEnv(envp, MAX_ENV);

  infofd = open(infofile, O_RDONLY);
//printf("==file:%s,line:%d,func:%s=job->status=%d=\n",__FILE__,__LINE__,__FUNCTION__,job->status);
  if (!cupsdStartProcess(command, argv, envp, infofd, -1, -1, -1, -1, 1, NULL,
                         NULL, &pid))
  {
    close(infofd);
    unlink(infofile);
    return (0);
  }

  close(infofd);
  unlink(infofile);

  while (waitpid(pid, &status, 0) < 0)
    if (errno != EINTR)
    {
      status = 1;
      break;
    }

  cupsdFinishProcess(pid, command, sizeof(command), NULL);

  if (status)
  {
    if (WIFEXITED(status))
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to create SSL server key and certificate - "
		      "the openssl command stopped with status %d!",
	              WEXITSTATUS(status));
    else
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to create SSL server key and certificate - "
		      "the openssl command crashed on signal %d!",
	              WTERMSIG(status));
  }
  else
  {
    cupsdLogMessage(CUPSD_LOG_INFO, "Created SSL server key file \"%s\"...",
		    ServerKey);
    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Created SSL server certificate file \"%s\"...",
		    ServerCertificate);
  }

  return (!status);

#elif defined(HAVE_GNUTLS)
  gnutls_x509_crt	crt;		/* Self-signed certificate */
  gnutls_x509_privkey	key;		/* Encryption key */
  cups_lang_t		*language;	/* Default language info */
  cups_file_t		*fp;		/* Key/cert file */
  unsigned char		buffer[8192];	/* Buffer for x509 data */
  size_t		bytes;		/* Number of bytes of data */
  unsigned char		serial[4];	/* Serial number buffer */
  time_t		curtime;	/* Current time */
  int			result;		/* Result of GNU TLS calls */


 /*
  * Create the encryption key...
  */

  cupsdLogMessage(CUPSD_LOG_INFO, "Generating SSL server key...");

  gnutls_x509_privkey_init(&key);
  gnutls_x509_privkey_generate(key, GNUTLS_PK_RSA, 2048, 0);

 /*
  * Save it...
  */

  bytes = sizeof(buffer);

  if ((result = gnutls_x509_privkey_export(key, GNUTLS_X509_FMT_PEM,
                                           buffer, &bytes)) < 0)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to export SSL server key - %s",
                    gnutls_strerror(result));
    gnutls_x509_privkey_deinit(key);
    return (0);
  }
  else if ((fp = cupsFileOpen(ServerKey, "w")) != NULL)
  {
    cupsFileWrite(fp, (char *)buffer, bytes);
    cupsFileClose(fp);

    cupsdLogMessage(CUPSD_LOG_INFO, "Created SSL server key file \"%s\"...",
		    ServerKey);
  }
  else
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to create SSL server key file \"%s\" - %s",
		    ServerKey, strerror(errno));
    gnutls_x509_privkey_deinit(key);
    return (0);
  }

 /*
  * Create the self-signed certificate...
  */

  cupsdLogMessage(CUPSD_LOG_INFO, "Generating self-signed SSL certificate...");

  language  = cupsLangDefault();
  curtime   = time(NULL);
  serial[0] = curtime >> 24;
  serial[1] = curtime >> 16;
  serial[2] = curtime >> 8;
  serial[3] = curtime;

  gnutls_x509_crt_init(&crt);
  if (strlen(language->language) == 5)
    gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_COUNTRY_NAME, 0,
                                  language->language + 3, 2);
  else
    gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_COUNTRY_NAME, 0,
                                  "US", 2);
  gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_COMMON_NAME, 0,
                                ServerName, strlen(ServerName));
  gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_ORGANIZATION_NAME, 0,
                                ServerName, strlen(ServerName));
  gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_ORGANIZATIONAL_UNIT_NAME,
                                0, "Unknown", 7);
  gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_STATE_OR_PROVINCE_NAME, 0,
                                "Unknown", 7);
  gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_X520_LOCALITY_NAME, 0,
                                "Unknown", 7);
  gnutls_x509_crt_set_dn_by_oid(crt, GNUTLS_OID_PKCS9_EMAIL, 0,
                                ServerAdmin, strlen(ServerAdmin));
  gnutls_x509_crt_set_key(crt, key);
  gnutls_x509_crt_set_serial(crt, serial, sizeof(serial));
  gnutls_x509_crt_set_activation_time(crt, curtime);
  gnutls_x509_crt_set_expiration_time(crt, curtime + 10 * 365 * 86400);
  gnutls_x509_crt_set_ca_status(crt, 0);
  gnutls_x509_crt_set_subject_alternative_name(crt, GNUTLS_SAN_DNSNAME,
                                               ServerName);
  gnutls_x509_crt_set_key_purpose_oid(crt, GNUTLS_KP_TLS_WWW_SERVER, 0);
  gnutls_x509_crt_set_key_usage(crt, GNUTLS_KEY_KEY_ENCIPHERMENT);
  gnutls_x509_crt_set_version(crt, 3);

  bytes = sizeof(buffer);
  if (gnutls_x509_crt_get_key_id(crt, 0, buffer, &bytes) >= 0)
    gnutls_x509_crt_set_subject_key_id(crt, buffer, bytes);

  gnutls_x509_crt_sign(crt, crt, key);

 /*
  * Save it...
  */

  bytes = sizeof(buffer);
  if ((result = gnutls_x509_crt_export(crt, GNUTLS_X509_FMT_PEM,
                                       buffer, &bytes)) < 0)
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to export SSL server certificate - %s",
		    gnutls_strerror(result));
  else if ((fp = cupsFileOpen(ServerCertificate, "w")) != NULL)
  {
    cupsFileWrite(fp, (char *)buffer, bytes);
    cupsFileClose(fp);

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Created SSL server certificate file \"%s\"...",
		    ServerCertificate);
  }
  else
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to create SSL server certificate file \"%s\" - %s",
		    ServerCertificate, strerror(errno));

 /*
  * Cleanup...
  */

  gnutls_x509_crt_deinit(crt);
  gnutls_x509_privkey_deinit(key);

  return (1);

#elif defined(HAVE_CDSASSL) && defined(HAVE_WAITPID)
  int		pid,			/* Process ID of command */
		status;			/* Status of command */
  char		command[1024],		/* Command */
		*argv[4],		/* Command-line arguments */
		*envp[MAX_ENV + 1],	/* Environment variables */
		keychain[1024],		/* Keychain argument */
		infofile[1024];		/* Type-in information for cert */
  cups_file_t	*fp;			/* Seed/info file */
  int		infofd;			/* Info file descriptor */


 /*
  * Run the "certtool" command to generate a self-signed certificate...
  */

  if (!cupsFileFind("certtool", getenv("PATH"), 1, command, sizeof(command)))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "No SSL certificate and certtool command not found!");
    return (0);
  }

 /*
  * Create a file with the certificate information fields...
  *
  * Note: This assumes that the default questions are asked by the certtool
  * command...
  */

  if ((fp = cupsTempFile2(infofile, sizeof(infofile))) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to create certificate information file %s - %s",
                    infofile, strerror(errno));
    return (0);
  }

  cupsFilePrintf(fp, "%s\nr\n\ny\nb\ns\ny\n%s\n\n\n\n\n%s\ny\n", 
        	 con->servername, con->servername, ServerAdmin);
  cupsFileClose(fp);

  cupsdLogMessage(CUPSD_LOG_INFO,
                  "Generating SSL server key and certificate...");

  snprintf(keychain, sizeof(keychain), "k=%s", ServerCertificate);

  argv[0] = "certtool";
  argv[1] = "c";
  argv[2] = keychain;
  argv[3] = NULL;

  cupsdLoadEnv(envp, MAX_ENV);

  infofd = open(infofile, O_RDONLY);
//printf("==file:%s,line:%d,func:%s=job->status=%d=\n",__FILE__,__LINE__,__FUNCTION__,job->status);
  if (!cupsdStartProcess(command, argv, envp, infofd, -1, -1, -1, -1, 1, NULL,
                         NULL, &pid))
  {
    close(infofd);
    unlink(infofile);
    return (0);
  }

  close(infofd);
  unlink(infofile);

  while (waitpid(pid, &status, 0) < 0)
    if (errno != EINTR)
    {
      status = 1;
      break;
    }

  cupsdFinishProcess(pid, command, sizeof(command), NULL);

  if (status)
  {
    if (WIFEXITED(status))
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to create SSL server key and certificate - "
		      "the certtool command stopped with status %d!",
	              WEXITSTATUS(status));
    else
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to create SSL server key and certificate - "
		      "the certtool command crashed on signal %d!",
	              WTERMSIG(status));
  }
  else
  {
    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Created SSL server certificate file \"%s\"...",
		    ServerCertificate);
  }

  return (!status);

#else
  return (0);
#endif /* HAVE_LIBSSL && HAVE_WAITPID */
}
#endif /* HAVE_SSL */


/*
 * 'pipe_command()' - Pipe the output of a command to the remote client.
 */

static int				/* O - Process ID */
pipe_command(cupsd_client_t *con,	/* I - Client connection */
             int            infile,	/* I - Standard input for command */
             int            *outfile,	/* O - Standard output for command */
	     char           *command,	/* I - Command to run */
	     char           *options,	/* I - Options for command */
	     int            root)	/* I - Run as root? */
{
  int		i;			/* Looping var */
  int		pid;			/* Process ID */
  char		*commptr,		/* Command string pointer */
		commch;			/* Command string character */
  char		*uriptr;		/* URI string pointer */
  int		fds[2];			/* Pipe FDs */
  int		argc;			/* Number of arguments */
  int		envc;			/* Number of environment variables */
  char		argbuf[10240],		/* Argument buffer */
		*argv[100],		/* Argument strings */
		*envp[MAX_ENV + 20];	/* Environment variables */
  char		auth_type[256],		/* CUPSD_AUTH_TYPE environment variable */
		content_length[1024],	/* CONTENT_LENGTH environment variable */
		content_type[1024],	/* CONTENT_TYPE environment variable */
		http_cookie[32768],	/* HTTP_COOKIE environment variable */
		http_referer[1024],	/* HTTP_REFERER environment variable */
		http_user_agent[1024],	/* HTTP_USER_AGENT environment variable */
		lang[1024],		/* LANG environment variable */
		path_info[1024],	/* PATH_INFO environment variable */
		remote_addr[1024],	/* REMOTE_ADDR environment variable */
		remote_host[1024],	/* REMOTE_HOST environment variable */
		remote_user[1024],	/* REMOTE_USER environment variable */
		script_filename[1024],	/* SCRIPT_FILENAME environment variable */
		script_name[1024],	/* SCRIPT_NAME environment variable */
		server_name[1024],	/* SERVER_NAME environment variable */
		server_port[1024];	/* SERVER_PORT environment variable */
  ipp_attribute_t *attr;		/* attributes-natural-language attribute */
  void		*ccache = NULL;		/* Kerberos credentials */


 /*
  * Parse a copy of the options string, which is of the form:
  *
  *     argument+argument+argument
  *     ?argument+argument+argument
  *     param=value&param=value
  *     ?param=value&param=value
  *     /name?argument+argument+argument
  *     /name?param=value&param=value
  *
  * If the string contains an "=" character after the initial name,
  * then we treat it as a HTTP GET form request and make a copy of
  * the remaining string for the environment variable.
  *
  * The string is always parsed out as command-line arguments, to
  * be consistent with Apache...
  */

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "pipe_command(con=%p(%d), infile=%d, outfile=%p, "
		  "command=\"%s\", options=\"%s\", root=%d)",
                  con, con->http.fd, infile, outfile, command,
		  options ? options : "(null)", root);

  argv[0] = command;

  if (options)
    strlcpy(argbuf, options, sizeof(argbuf));
  else
    argbuf[0] = '\0';

  if (argbuf[0] == '/')
  {
   /*
    * Found some trailing path information, set PATH_INFO...
    */

    if ((commptr = strchr(argbuf, '?')) == NULL)
      commptr = argbuf + strlen(argbuf);

    commch   = *commptr;
    *commptr = '\0';
    snprintf(path_info, sizeof(path_info), "PATH_INFO=%s", argbuf);
    *commptr = commch;
  }
  else
  {
    commptr      = argbuf;
    path_info[0] = '\0';

    if (*commptr == ' ')
      commptr ++;
  }

  if (*commptr == '?' && con->operation == HTTP_GET && !con->query_string)
  {
    commptr ++;
    cupsdSetStringf(&(con->query_string), "QUERY_STRING=%s", commptr);
  }

  argc = 1;

  if (*commptr)
  {
    argv[argc ++] = commptr;

    for (; *commptr && argc < 99; commptr ++)
    {
     /*
      * Break arguments whenever we see a + or space...
      */

      if (*commptr == ' ' || *commptr == '+')
      {
	while (*commptr == ' ' || *commptr == '+')
	  *commptr++ = '\0';

       /*
	* If we don't have a blank string, save it as another argument...
	*/

	if (*commptr)
	{
	  argv[argc] = commptr;
	  argc ++;
	}
	else
	  break;
      }
      else if (*commptr == '%' && isxdigit(commptr[1] & 255) &&
               isxdigit(commptr[2] & 255))
      {
       /*
	* Convert the %xx notation to the individual character.
	*/

	if (commptr[1] >= '0' && commptr[1] <= '9')
          *commptr = (commptr[1] - '0') << 4;
	else
          *commptr = (tolower(commptr[1]) - 'a' + 10) << 4;

	if (commptr[2] >= '0' && commptr[2] <= '9')
          *commptr |= commptr[2] - '0';
	else
          *commptr |= tolower(commptr[2]) - 'a' + 10;

	_cups_strcpy(commptr + 1, commptr + 3);

       /*
	* Check for a %00 and break if that is the case...
	*/

	if (!*commptr)
          break;
      }
    }
  }

  argv[argc] = NULL;

 /*
  * Setup the environment variables as needed...
  */

  if (con->username[0])
  {
    snprintf(auth_type, sizeof(auth_type), "CUPSD_AUTH_TYPE=%s",
             httpGetField(HTTP(con), HTTP_FIELD_AUTHORIZATION));

    if ((uriptr = strchr(auth_type + 10, ' ')) != NULL)
      *uriptr = '\0';
  }
  else
    auth_type[0] = '\0';

  if (con->request &&
      (attr = ippFindAttribute(con->request, "attributes-natural-language",
                               IPP_TAG_LANGUAGE)) != NULL)
  {
    switch (strlen(attr->values[0].string.text))
    {
      default :
	 /*
	  * This is an unknown or badly formatted language code; use
	  * the POSIX locale...
	  */

	  strcpy(lang, "LANG=C");
	  break;

      case 2 :
	 /*
	  * Just the language code (ll)...
	  */

	  snprintf(lang, sizeof(lang), "LANG=%s.UTF8",
		   attr->values[0].string.text);
	  break;

      case 5 :
	 /*
	  * Language and country code (ll-cc)...
	  */

	  snprintf(lang, sizeof(lang), "LANG=%c%c_%c%c.UTF8",
		   attr->values[0].string.text[0],
		   attr->values[0].string.text[1],
		   toupper(attr->values[0].string.text[3] & 255),
		   toupper(attr->values[0].string.text[4] & 255));
	  break;
    }
  }
  else if (con->language)
    snprintf(lang, sizeof(lang), "LANG=%s.UTF8", con->language->language);
  else
    strcpy(lang, "LANG=C");

  strcpy(remote_addr, "REMOTE_ADDR=");
  httpAddrString(con->http.hostaddr, remote_addr + 12,
                 sizeof(remote_addr) - 12);

  snprintf(remote_host, sizeof(remote_host), "REMOTE_HOST=%s",
           con->http.hostname);

  snprintf(script_name, sizeof(script_name), "SCRIPT_NAME=%s", con->uri);
  if ((uriptr = strchr(script_name, '?')) != NULL)
    *uriptr = '\0';

  snprintf(script_filename, sizeof(script_filename), "SCRIPT_FILENAME=%s%s",
           DocumentRoot, script_name + 12);

  sprintf(server_port, "SERVER_PORT=%d", con->serverport);

  snprintf(server_name, sizeof(server_name), "SERVER_NAME=%s",
           con->servername);

  envc = cupsdLoadEnv(envp, (int)(sizeof(envp) / sizeof(envp[0])));

  if (auth_type[0])
    envp[envc ++] = auth_type;

  envp[envc ++] = lang;
  envp[envc ++] = "REDIRECT_STATUS=1";
  envp[envc ++] = "GATEWAY_INTERFACE=CGI/1.1";
  envp[envc ++] = server_name;
  envp[envc ++] = server_port;
  envp[envc ++] = remote_addr;
  envp[envc ++] = remote_host;
  envp[envc ++] = script_name;
  envp[envc ++] = script_filename;

  if (path_info[0])
    envp[envc ++] = path_info;

  if (con->username[0])
  {
    snprintf(remote_user, sizeof(remote_user), "REMOTE_USER=%s", con->username);

    envp[envc ++] = remote_user;

   /*
    * Save Kerberos credentials, if any...
    */

#ifdef HAVE_GSSAPI
    if (con->gss_creds)
      ccache = cupsdCopyKrb5Creds(con);
#endif /* HAVE_GSSAPI */
  }

  if (con->http.version == HTTP_1_1)
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/1.1";
  else if (con->http.version == HTTP_1_0)
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/1.0";
  else
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/0.9";

  if (con->http.cookie)
  {
    snprintf(http_cookie, sizeof(http_cookie), "HTTP_COOKIE=%s",
             con->http.cookie);
    envp[envc ++] = http_cookie;
  }

  if (con->http.fields[HTTP_FIELD_USER_AGENT][0])
  {
    snprintf(http_user_agent, sizeof(http_user_agent), "HTTP_USER_AGENT=%s",
             con->http.fields[HTTP_FIELD_USER_AGENT]);
    envp[envc ++] = http_user_agent;
  }

  if (con->http.fields[HTTP_FIELD_REFERER][0])
  {
    snprintf(http_referer, sizeof(http_referer), "HTTP_REFERER=%s",
             con->http.fields[HTTP_FIELD_REFERER]);
    envp[envc ++] = http_referer;
  }

  if (con->operation == HTTP_GET)
  {
    envp[envc ++] = "REQUEST_METHOD=GET";

    if (con->query_string)
    {
     /*
      * Add GET form variables after ?...
      */

      envp[envc ++] = con->query_string;
    }
    else
      envp[envc ++] = "QUERY_STRING=";
  }
  else
  {
    sprintf(content_length, "CONTENT_LENGTH=" CUPS_LLFMT,
            CUPS_LLCAST con->bytes);
    snprintf(content_type, sizeof(content_type), "CONTENT_TYPE=%s",
             con->http.fields[HTTP_FIELD_CONTENT_TYPE]);

    envp[envc ++] = "REQUEST_METHOD=POST";
    envp[envc ++] = content_length;
    envp[envc ++] = content_type;
  }

 /*
  * Tell the CGI if we are using encryption...
  */

  if (con->http.tls)
    envp[envc ++] = "HTTPS=ON";

 /*
  * Terminate the environment array...
  */

  envp[envc] = NULL;

  if (LogLevel >= CUPSD_LOG_DEBUG)
  {
    for (i = 0; i < argc; i ++)
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "[CGI] argv[%d] = \"%s\"", i, argv[i]);
    for (i = 0; i < envc; i ++)
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "[CGI] envp[%d] = \"%s\"", i, envp[i]);
  }

 /*
  * Create a pipe for the output...
  */

  if (cupsdOpenPipe(fds))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "[CGI] Unable to create pipe for %s - %s",
                    argv[0], strerror(errno));
    return (0);
  }

 /*
  * Then execute the command...
  */
//printf("==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
  if (cupsdStartProcess(command, argv, envp, infile, fds[1], CGIPipes[1],
			-1, -1, root, DefaultProfile, NULL, &pid) < 0)
  {
   /*
    * Error - can't fork!
    */

    cupsdLogMessage(CUPSD_LOG_ERROR, "[CGI] Unable to start %s - %s", argv[0],
                    strerror(errno));

    cupsdClosePipe(fds);
    pid = 0;
  }
  else
  {
   /*
    * Fork successful - return the PID...
    */
#ifdef NOT_CUT
    if (con->username[0])
      cupsdAddCert(pid, con->username, ccache);
#endif
    cupsdLogMessage(CUPSD_LOG_DEBUG, "[CGI] Started %s (PID %d)", command, pid);

    *outfile = fds[0];
    close(fds[1]);
  }

  return (pid);
}


/*
 * 'valid_host()' - Is the Host: field valid?
 */

static int				/* O - 1 if valid, 0 if not */
valid_host(cupsd_client_t *con)		/* I - Client connection */
{
  cupsd_alias_t	*a;			/* Current alias */
  cupsd_netif_t	*netif;			/* Current network interface */
  const char	*host,			/* Host field */
		*end;			/* End character */


  host = con->http.fields[HTTP_FIELD_HOST];

  if (httpAddrLocalhost(con->http.hostaddr))
  {
   /*
    * Only allow "localhost" or the equivalent IPv4 or IPv6 numerical
    * addresses when accessing CUPS via the loopback interface...
    */

    return (!strcasecmp(host, "localhost") ||
            !strncasecmp(host, "localhost:", 10) ||
	    !strcasecmp(host, "localhost.") ||
            !strncasecmp(host, "localhost.:", 11) ||
#ifdef __linux
	    !strcasecmp(host, "localhost.localdomain") ||
            !strncasecmp(host, "localhost.localdomain:", 22) ||
#endif /* __linux */
            !strcmp(host, "127.0.0.1") ||
	    !strncmp(host, "127.0.0.1:", 10) ||
	    !strcmp(host, "[::1]") ||
	    !strncmp(host, "[::1]:", 6));
  }

#ifdef HAVE_DNSSD
 /*
  * Check if the hostname is something.local (Bonjour); if so, allow it.
  */

  if ((end = strrchr(host, '.')) != NULL &&
      (!strcasecmp(end, ".local") || !strncasecmp(end, ".local:", 7) ||
       !strcasecmp(end, ".local.") || !strncasecmp(end, ".local.:", 8)))
    return (1);
#endif /* HAVE_DNSSD */

 /*
  * Check if the hostname is an IP address...
  */

  if (isdigit(*host & 255) || *host == '[')
  {
   /*
    * Possible IPv4/IPv6 address...
    */

    char	temp[1024],		/* Temporary string */
		*ptr;			/* Pointer into temporary string */
    http_addrlist_t *addrlist;		/* List of addresses */


    strlcpy(temp, host, sizeof(temp));
    if ((ptr = strrchr(temp, ':')) != NULL && !strchr(ptr, ']'))
      *ptr = '\0';			/* Strip :port from host value */

    if ((addrlist = httpAddrGetList(temp, AF_UNSPEC, NULL)) != NULL)
    {
     /*
      * Good IPv4/IPv6 address...
      */

      httpAddrFreeList(addrlist);
      return (1);
    }
  }

 /*
  * Check for (alias) name matches...
  */

  for (a = (cupsd_alias_t *)cupsArrayFirst(ServerAlias);
       a;
       a = (cupsd_alias_t *)cupsArrayNext(ServerAlias))
  {
   /*
    * "ServerAlias *" allows all host values through...
    */

    if (!strcmp(a->name, "*"))
      return (1);

    if (!strncasecmp(host, a->name, a->namelen))
    {
     /*
      * Prefix matches; check the character at the end - it must be ":", ".",
      * ".:", or nul...
      */

      end = host + a->namelen;

      if (!*end || *end == ':' || (*end == '.' && (!end[1] || end[1] == ':')))
        return (1);
    }
  }

#ifdef HAVE_DNSSD
  for (a = (cupsd_alias_t *)cupsArrayFirst(DNSSDAlias);
       a;
       a = (cupsd_alias_t *)cupsArrayNext(DNSSDAlias))
  {
   /*
    * "ServerAlias *" allows all host values through...
    */

    if (!strcmp(a->name, "*"))
      return (1);

    if (!strncasecmp(host, a->name, a->namelen))
    {
     /*
      * Prefix matches; check the character at the end - it must be ":", ".",
      * ".:", or nul...
      */

      end = host + a->namelen;

      if (!*end || *end == ':' || (*end == '.' && (!end[1] || end[1] == ':')))
        return (1);
    }
  }
#endif /* HAVE_DNSSD */

 /*
  * Check for interface hostname matches...
  */

  for (netif = (cupsd_netif_t *)cupsArrayFirst(NetIFList);
       netif;
       netif = (cupsd_netif_t *)cupsArrayNext(NetIFList))
  {
    if (!strncasecmp(host, netif->hostname, netif->hostlen))
    {
     /*
      * Prefix matches; check the character at the end - it must be ":", ".",
      * ".:", or nul...
      */

      end = host + netif->hostlen;

      if (!*end || *end == ':' || (*end == '.' && (!end[1] || end[1] == ':')))
        return (1);
    }
  }

  return (0);
}


/*
 * 'write_file()' - Send a file via HTTP.
 */

static int				/* O - 0 on failure, 1 on success */
write_file(cupsd_client_t *con,		/* I - Client connection */
           http_status_t  code,		/* I - HTTP status */
	   char           *filename,	/* I - Filename */
	   char           *type,	/* I - File type */
	   struct stat    *filestats)	/* O - File information */
{
  con->file = open(filename, O_RDONLY);

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "write_file(con=%p(%d), code=%d, filename=\"%s\" (%d), "
		  "type=\"%s\", filestats=%p)", con, con->http.fd,
		  code, filename, con->file, type ? type : "(null)", filestats);

  if (con->file < 0)
    return (0);

  fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);

  con->pipe_pid = 0;

  if (!cupsdSendHeader(con, code, type, CUPSD_AUTH_NONE))
    return (0);

  if (httpPrintf(HTTP(con), "Last-Modified: %s\r\n",
                 httpGetDateString(filestats->st_mtime)) < 0)
    return (0);
  if (httpPrintf(HTTP(con), "Content-Length: " CUPS_LLFMT "\r\n",
                 CUPS_LLCAST filestats->st_size) < 0)
    return (0);
  if (httpPrintf(HTTP(con), "\r\n") < 0)
    return (0);

  if (cupsdFlushHeader(con) < 0)
    return (0);

  con->http.data_encoding  = HTTP_ENCODE_LENGTH;
  con->http.data_remaining = filestats->st_size;

  if (con->http.data_remaining <= INT_MAX)
    con->http._data_remaining = con->http.data_remaining;
  else
    con->http._data_remaining = INT_MAX;

  cupsdAddSelect(con->http.fd, (cupsd_selfunc_t)cupsdReadClient,
                 (cupsd_selfunc_t)cupsdWriteClient, con);

  return (1);
}


/*
 * 'write_pipe()' - Flag that data is available on the CGI pipe.
 */

static void
write_pipe(cupsd_client_t *con)		/* I - Client connection */
{
  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "write_pipe(con=%p(%d)) CGI output on fd %d",
                  con, con->http.fd, con->file);

  con->file_ready = 1;

  cupsdRemoveSelect(con->file);
  cupsdAddSelect(con->http.fd, NULL, (cupsd_selfunc_t)cupsdWriteClient, con);
}


/*
 * End of "$Id: client.c 9120 2010-04-23 18:56:34Z mike $".
 */
