/*
 * "$Id: conf.h 9120 2010-04-23 18:56:34Z mike $"
 *
 *   Configuration file definitions for CUPS.
 *
 *   Copyright 2007-2010 by Apple Inc.
 *   Copyright 1997-2007 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 */


/*
 * Log levels...
 */

typedef enum
{
  CUPSD_LOG_PPD = -4,			/* Used internally for PPD keywords */
  CUPSD_LOG_ATTR,			/* Used internally for attributes */
  CUPSD_LOG_STATE,			/* Used internally for state-reasons */
  CUPSD_LOG_PAGE,			/* Used internally for page logging */
  CUPSD_LOG_NONE,
  CUPSD_LOG_EMERG,			/* Emergency issues */
  CUPSD_LOG_ALERT,			/* Something bad happened that needs attention */
  CUPSD_LOG_CRIT,			/* Critical error but server continues */
  CUPSD_LOG_ERROR,			/* Error condition */
  CUPSD_LOG_WARN,			/* Warning */
  CUPSD_LOG_NOTICE,			/* Normal condition that needs logging */
  CUPSD_LOG_INFO,			/* General information */
  CUPSD_LOG_DEBUG,			/* General debugging */
  CUPSD_LOG_DEBUG2			/* Detailed debugging */
} cupsd_loglevel_t;

typedef enum
{
  CUPSD_ACCESSLOG_CONFIG,		/* Log config requests */
  CUPSD_ACCESSLOG_ACTIONS,		/* Log config, print, and job management requests */
  CUPSD_ACCESSLOG_ALL			/* Log everything */
} cupsd_accesslog_t;

typedef enum
{
  CUPSD_TIME_STANDARD,			/* "Standard" Apache/CLF format */
  CUPSD_TIME_USECS			/* Standard format with microseconds */
} cupsd_time_t;


/*
 * FatalErrors flags...
 */

#define CUPSD_FATAL_NONE	0	/* No errors are fatal */
#define CUPSD_FATAL_BROWSE	1	/* Browse bind errors are fatal */
#define CUPSD_FATAL_CONFIG	2	/* Config file syntax errors are fatal */
#define CUPSD_FATAL_LISTEN	4	/* Listen/Port bind errors are fatal */
#define CUPSD_FATAL_LOG		8	/* Log file errors are fatal */
#define CUPSD_FATAL_PERMISSIONS	16	/* File permission errors are fatal */
#define CUPSD_FATAL_ALL		~0	/* All errors are fatal */


/*
 * Printcap formats...
 */

#define PRINTCAP_BSD		0	/* Berkeley LPD format */
#define PRINTCAP_SOLARIS	1	/* Solaris lpsched format */
#define PRINTCAP_PLIST		2	/* Mac OS X plist format */


/*
 * SSL options (bits)...
 */

#define CUPSD_SSL_NONE		0	/* No special options */
#define CUPSD_SSL_NOEMPTY	1	/* Do not insert empty fragments */


/*
 * ServerAlias data...
 */

typedef struct
{
  size_t	namelen;		/* Length of alias name */
  char		name[1];		/* Alias name */
} cupsd_alias_t;


/*
 * Globals...
 */

VAR char		*ConfigurationFile	VALUE(NULL),
					/* Configuration file to use */
			*ServerName		VALUE(NULL),
					/* FQDN for server */
			*ServerAdmin		VALUE(NULL),
					/* Administrator's email */
			*ServerRoot		VALUE(NULL),
					/* Root directory for scheduler */
			*ServerBin		VALUE(NULL),
					/* Root directory for binaries */
			*StateDir		VALUE(NULL),
					/* Root directory for state data */
			*RequestRoot		VALUE(NULL),
					/* Directory for request files */
			*DocumentRoot		VALUE(NULL);
					/* Root directory for documents */
VAR cups_array_t	*ServerAlias		VALUE(NULL);
					/* Alias names for server */
VAR int			ServerNameIsIP		VALUE(0);
					/* Is the ServerName an IP address? */
VAR int			NumSystemGroups		VALUE(0);
					/* Number of system group names */
VAR char		*SystemGroups[MAX_SYSTEM_GROUPS]
						VALUE({0});
					/* System group names */
VAR gid_t		SystemGroupIDs[MAX_SYSTEM_GROUPS]
						VALUE({0});
					/* System group IDs */
VAR char		*AccessLog		VALUE(NULL),
					/* Access log filename */
			*ErrorLog		VALUE(NULL),
					/* Error log filename */
			*PageLog		VALUE(NULL),
					/* Page log filename */
			*CacheDir		VALUE(NULL),
					/* Cache file directory */
			*DataDir		VALUE(NULL),
					/* Data file directory */
			*DefaultLanguage	VALUE(NULL),
					/* Default language encoding */
			*DefaultLocale		VALUE(NULL),
					/* Default locale */
			*DefaultPaperSize	VALUE(NULL),
					/* Default paper size */
			*ErrorPolicy		VALUE(NULL),
					/* Default printer-error-policy */
			*RIPCache		VALUE(NULL),
					/* Amount of memory for RIPs */
			*TempDir		VALUE(NULL),
					/* Temporary directory */
			*Printcap		VALUE(NULL),
					/* Printcap file */
			*PrintcapGUI		VALUE(NULL),
					/* GUI program to use for IRIX */
			*FontPath		VALUE(NULL),
					/* Font search path */
			*RemoteRoot		VALUE(NULL),
					/* Remote root user */
			*Classification		VALUE(NULL);
					/* Classification of system */
VAR uid_t		User			VALUE(1);
					/* User ID for server */
VAR gid_t		Group			VALUE(0);
					/* Group ID for server */
VAR cupsd_accesslog_t	AccessLogLevel		VALUE(CUPSD_ACCESSLOG_ACTIONS);
					/* Access log level */
VAR int			ClassifyOverride	VALUE(0),
					/* Allow overrides? */
			ConfigFilePerm		VALUE(0640),
					/* Permissions for config files */
			LogDebugHistory		VALUE(200),
					/* Amount of automatic debug history */
			FatalErrors		VALUE(CUPSD_FATAL_CONFIG),
					/* Which errors are fatal? */
			LogFilePerm		VALUE(0644);
					/* Permissions for log files */
VAR cupsd_loglevel_t	LogLevel		VALUE(CUPSD_LOG_WARN);
					/* Error log level */
VAR cupsd_time_t	LogTimeFormat		VALUE(CUPSD_TIME_STANDARD);
					/* Log file time format */
VAR int			MaxClients		VALUE(100),
					/* Maximum number of clients */
			MaxClientsPerHost	VALUE(0),
					/* Maximum number of clients per host */
			MaxCopies		VALUE(CUPS_DEFAULT_MAX_COPIES),
					/* Maximum number of copies per job */
			MaxLogSize		VALUE(1024 * 1024),
					/* Maximum size of log files */
			MaxPrinterHistory	VALUE(10),
					/* Maximum printer state history */
			MaxRequestSize		VALUE(0),
					/* Maximum size of IPP requests */
			HostNameLookups		VALUE(FALSE),
					/* Do we do reverse lookups? */
			Timeout			VALUE(DEFAULT_TIMEOUT),
					/* Timeout during requests */
			KeepAlive		VALUE(TRUE),
					/* Support the Keep-Alive option? */
			KeepAliveTimeout	VALUE(DEFAULT_KEEPALIVE),
					/* Timeout between requests */
			ImplicitClasses		VALUE(TRUE),
					/* Are classes implicitly created? */
			ImplicitAnyClasses	VALUE(FALSE),
					/* Create AnyPrinter classes? */
			HideImplicitMembers	VALUE(TRUE),
					/* Hide implicit class members? */
			FileDevice		VALUE(FALSE),
					/* Allow file: devices? */
			FilterLimit		VALUE(0),
					/* Max filter cost at any time */
			FilterLevel		VALUE(0),
					/* Current filter level */
			FilterNice		VALUE(0),
					/* Nice value for filters */
			ReloadTimeout		VALUE(DEFAULT_KEEPALIVE),
					/* Timeout before reload from SIGHUP */
			RootCertDuration	VALUE(300),
					/* Root certificate update interval */
			RunUser			VALUE(0),
					/* User to run as, used for files */
			PrintcapFormat		VALUE(PRINTCAP_BSD),
					/* Format of printcap file? */
			DefaultShared		VALUE(TRUE),
					/* Share printers by default? */
			MultipleOperationTimeout VALUE(DEFAULT_TIMEOUT);
					/* multiple-operation-time-out value */
VAR cups_file_t		*AccessFile		VALUE(NULL),
					/* Access log file */
			*ErrorFile		VALUE(NULL),
					/* Error log file */
			*PageFile		VALUE(NULL);
					/* Page log file */
VAR char		*PageLogFormat		VALUE(NULL);
					/* Page log format */
VAR mime_t		*MimeDatabase		VALUE(NULL);
					/* MIME type database */
VAR int			NumMimeTypes		VALUE(0);
					/* Number of MIME types */
VAR const char		**MimeTypes		VALUE(NULL);
					/* Array of MIME types */

#ifdef HAVE_SSL
VAR char		*ServerCertificate	VALUE(NULL);
					/* Server certificate file */
#  if defined(HAVE_LIBSSL) || defined(HAVE_GNUTLS)
VAR char		*ServerKey		VALUE(NULL);
					/* Server key file */
#  endif /* HAVE_LIBSSL || HAVE_GNUTLS */
VAR int			SSLOptions		VALUE(CUPSD_SSL_NONE);
					/* SSL/TLS options */
#endif /* HAVE_SSL */

#ifdef HAVE_LAUNCHD
VAR int			LaunchdTimeout		VALUE(DEFAULT_KEEPALIVE);
					/* Time after which an idle cupsd will exit */
#endif /* HAVE_LAUNCHD */

#ifdef __APPLE__
VAR int			AppleQuotas		VALUE(TRUE);
					/* Use Apple PrintService Quotas instead of CUPS quotas */
#endif  /* __APPLE__ */

#ifdef HAVE_AUTHORIZATION_H
VAR char		*SystemGroupAuthKey	VALUE(NULL);
					/* System group auth key */
#endif /* HAVE_AUTHORIZATION_H */


/*
 * Prototypes...
 */

extern void	cupsdAddAlias(cups_array_t *aliases, const char *name);
extern int	cupsdCheckPermissions(const char *filename,
		                      const char *suffix, int mode,
	 			      int user, int group, int is_dir,
				      int create_dir);
extern void	cupsdFreeAliases(cups_array_t *aliases);
extern char	*cupsdGetDateTime(struct timeval *t, cupsd_time_t format);
#ifdef HAVE_GSSAPI
extern int	cupsdLogGSSMessage(int level, int major_status,
		                   int minor_status,
		                   const char *message, ...);
#endif /* HAVE_GSSAPI */
extern int	cupsdLogJob(cupsd_job_t *job, int level, const char *message, ...)
#ifdef __GNUC__
__attribute__ ((__format__ (__printf__, 3, 4)))
#endif /* __GNUC__ */
;
extern int	cupsdLogMessage(int level, const char *message, ...)
#ifdef __GNUC__
__attribute__ ((__format__ (__printf__, 2, 3)))
#endif /* __GNUC__ */
;
extern int	cupsdLogPage(cupsd_job_t *job, const char *page);
extern int	cupsdLogRequest(cupsd_client_t *con, http_status_t code);
extern int	cupsdReadConfiguration(void);
extern int	cupsdWriteErrorLog(int level, const char *message);


/*
 * End of "$Id: conf.h 9120 2010-04-23 18:56:34Z mike $".
 */
