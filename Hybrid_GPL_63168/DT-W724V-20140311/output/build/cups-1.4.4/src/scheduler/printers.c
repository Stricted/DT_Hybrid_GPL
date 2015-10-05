/*
 * "$Id: printers.c 9124 2010-04-23 20:53:38Z mike $"
 *
 *   Printer routines for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 2007-2010 by Apple Inc.
 *   Copyright 1997-2007 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 * Contents:
 *
 *   cupsdAddPrinter()          - Add a printer to the system.
 *   cupsdAddPrinterHistory()   - Add the current printer state to the history.
 *   cupsdAddPrinterUser()      - Add a user to the ACL.
 *   cupsdCreateCommonData()    - Create the common printer data.
 *   cupsdDeleteAllPrinters()   - Delete all printers from the system.
 *   cupsdDeletePrinter()       - Delete a printer from the system.
 *   cupsdFindDest()            - Find a destination in the list.
 *   cupsdFindPrinter()         - Find a printer in the list.
 *   cupsdFreePrinterUsers()    - Free allow/deny users.
 *   cupsdLoadAllPrinters()     - Load printers from the printers.conf file.
 *   cupsdRenamePrinter()       - Rename a printer.
 *   cupsdSaveAllPrinters()     - Save all printer definitions to the
 *                                printers.conf file.
 *   cupsdSetAuthInfoRequired() - Set the required authentication info.
 *   cupsdSetDeviceURI()        - Set the device URI for a printer.
 *   cupsdSetPrinterAttr()      - Set a printer attribute.
 *   cupsdSetPrinterAttrs()     - Set printer attributes based upon the PPD
 *                                file.
 *   cupsdSetPrinterReasons()   - Set/update the reasons strings.
 *   cupsdSetPrinterState()     - Update the current state of a printer.
 *   cupsdStopPrinter()         - Stop a printer from printing any jobs...
 *   cupsdUpdatePrinterPPD()    - Update keywords in a printer's PPD file.
 *   cupsdUpdatePrinters()      - Update printers after a partial reload.
 *   cupsdValidateDest()        - Validate a printer/class destination.
 *   cupsdWritePrintcap()       - Write a pseudo-printcap file for older
 *                                applications that need it...
 *   add_printer_defaults()     - Add name-default attributes to the printer
 *                                attributes.
 *   add_printer_filter()       - Add a MIME filter for a printer.
 *   add_printer_formats()      - Add document-format-supported values for a
 *                                printer.
 *   add_string_array()         - Add a string to an array of CUPS strings.
 *   compare_printers()         - Compare two printers.
 *   delete_printer_filters()   - Delete all MIME filters for a printer.
 *   delete_string_array()      - Delete an array of CUPS strings.
 *   load_ppd()                 - Load a cached PPD file, updating the cache as
 *                                needed.
 *   new_media_col()            - Create a media-col collection value.
 *   write_irix_config()        - Update the config files used by the IRIX
 *                                desktop tools.
 *   write_irix_state()         - Update the status files used by IRIX printing
 *                                desktop tools.
 *   write_xml_string()         - Write a string with XML escaping.
 */

/*
 * Include necessary headers...
 */

#include "cupsd.h"
#include <cups/dir.h>


/*
 * Local functions...
 */

static void	add_printer_defaults(cupsd_printer_t *p);
static void	add_printer_filter(cupsd_printer_t *p, mime_type_t *type,
				   const char *filter);
static void	add_printer_formats(cupsd_printer_t *p);
static void	add_string_array(cups_array_t **a, const char *s);
static int	compare_printers(void *first, void *second, void *data);
static void	delete_printer_filters(cupsd_printer_t *p);
static void	delete_string_array(cups_array_t **a);
static void	load_ppd(cupsd_printer_t *p);
static ipp_t	*new_media_col(_pwg_size_t *size, const char *source,
		               const char *type);
#ifdef __sgi
static void	write_irix_config(cupsd_printer_t *p);
static void	write_irix_state(cupsd_printer_t *p);
#endif /* __sgi */
static void	write_xml_string(cups_file_t *fp, const char *s);


/*
 * 'cupsdAddPrinter()' - Add a printer to the system.
 */

cupsd_printer_t *			/* O - New printer */
cupsdAddPrinter(const char *name)	/* I - Name of printer */
{
  cupsd_printer_t	*p;		/* New printer */


 /*
  * Range check input...
  */
////printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,getpid());
////printf("\n=111=cupsdAddPrinter(\"%s\")", name);
  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cupsdAddPrinter(\"%s\")", name);

 /*
  * Create a new printer entity...
  */

  if ((p = calloc(1, sizeof(cupsd_printer_t))) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_CRIT, "Unable to allocate memory for printer - %s",
                    strerror(errno));
    return (NULL);
  }

  cupsdSetString(&p->name, name);
  cupsdSetString(&p->info, name);
  cupsdSetString(&p->hostname, ServerName);

  cupsdSetStringf(&p->uri, "ipp://%s:%d/printers/%s", ServerName, RemotePort,
                  name);
  cupsdSetDeviceURI(p, "file:///dev/null");

  p->state      = IPP_PRINTER_STOPPED;
  p->state_time = time(NULL);
  p->accepting  = 0;
  p->shared     = DefaultShared;
  p->filetype   = mimeAddType(MimeDatabase, "printer", name);

  cupsdSetString(&p->job_sheets[0], "none");
  cupsdSetString(&p->job_sheets[1], "none");

  cupsdSetString(&p->error_policy, ErrorPolicy);
  cupsdSetString(&p->op_policy, DefaultPolicy);

  p->op_policy_ptr = DefaultPolicyPtr;

  if (MaxPrinterHistory)
    p->history = calloc(MaxPrinterHistory, sizeof(ipp_t *));

 /*
  * Insert the printer in the printer list alphabetically...
  */
////printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,getpid());
  if (!Printers)
    {
//    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,getpid());
    Printers = cupsArrayNew(compare_printers, NULL);
//     //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,getpid());
    }
  cupsdLogMessage(CUPSD_LOG_ERROR,
                  "cupsdAddPrinter: Adding %s to Printers", p->name);
//  //printf("=222==cupsdAddPrinter: Adding %s to Printers", p->name);
  cupsArrayAdd(Printers, p);
////printf("==file:%s,line:%d,func:%s=cupsArrayCount=%d=\n",__FILE__,__LINE__,__FUNCTION__,cupsArrayCount(Printers));
  if (!ImplicitPrinters)
    {
//    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,getpid());
    ImplicitPrinters = cupsArrayNew(compare_printers, NULL);
    }

 /*
  * Return the new printer...
  */

  return (p);
}


/*
 * 'cupsdAddPrinterHistory()' - Add the current printer state to the history.
 */
#ifdef NOT_CUT

void
cupsdAddPrinterHistory(
    cupsd_printer_t *p)			/* I - Printer */
{
  ipp_t	*history;			/* History collection */


 /*
  * Stop early if we aren't keeping history data...
  */

  if (MaxPrinterHistory <= 0)
    return;

 /*
  * Retire old history data as needed...
  */

  p->sequence_number ++;

  if (p->num_history >= MaxPrinterHistory)
  {
    p->num_history --;
    ippDelete(p->history[0]);
    memmove(p->history, p->history + 1, p->num_history * sizeof(ipp_t *));
  }

 /*
  * Create a collection containing the current printer-state, printer-up-time,
  * printer-state-message, and printer-state-reasons attributes.
  */

  history = ippNew();
  ippAddInteger(history, IPP_TAG_PRINTER, IPP_TAG_ENUM, "printer-state",
                p->state);
  ippAddBoolean(history, IPP_TAG_PRINTER, "printer-is-accepting-jobs",
                p->accepting);
  ippAddBoolean(history, IPP_TAG_PRINTER, "printer-is-shared", p->shared);
  ippAddString(history, IPP_TAG_PRINTER, IPP_TAG_TEXT, "printer-state-message",
               NULL, p->state_message);
#ifdef __APPLE__
  if (p->recoverable)
    ippAddString(history, IPP_TAG_PRINTER, IPP_TAG_TEXT,
                 "com.apple.print.recoverable-message", NULL, p->recoverable);
#endif /* __APPLE__ */
  if (p->num_reasons == 0)
    ippAddString(history, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
                 "printer-state-reasons", NULL, "none");
  else
    ippAddStrings(history, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
                  "printer-state-reasons", p->num_reasons, NULL,
		  (const char * const *)p->reasons);
  //printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  ippAddInteger(history, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "printer-state-change-time", p->state_time);
  ippAddInteger(history, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "printer-state-sequence-number", p->sequence_number);

  p->history[p->num_history] = history;
  p->num_history ++;
}
#endif

/*
 * 'cupsdAddPrinterUser()' - Add a user to the ACL.
 */
#ifdef NOT_CUT

void
cupsdAddPrinterUser(
    cupsd_printer_t *p,			/* I - Printer */
    const char      *username)		/* I - User */
{
  const char	**temp;			/* Temporary array pointer */


  if (!p || !username)
    return;

  if (p->num_users == 0)
    temp = malloc(sizeof(char **));
  else
    temp = realloc(p->users, sizeof(char **) * (p->num_users + 1));

  if (!temp)
    return;

  p->users = temp;
  temp     += p->num_users;

  if ((*temp = strdup(username)) != NULL)
    p->num_users ++;
}
#endif

/*
 * 'cupsdCreateCommonData()' - Create the common printer data.
 */

void
cupsdCreateCommonData(void)
{
  int			i;		/* Looping var */
  ipp_attribute_t	*attr;		/* Attribute data */
  cups_dir_t		*dir;		/* Notifier directory */
  cups_dentry_t		*dent;		/* Notifier directory entry */
  cups_array_t		*notifiers;	/* Notifier array */
  char			filename[1024],	/* Filename */
			*notifier;	/* Current notifier */
  cupsd_policy_t	*p;		/* Current policy */
  static const int nups[] =		/* number-up-supported values */
		{ 1, 2, 4, 6, 9, 16 };
  static const int orients[4] =/* orientation-requested-supported values */
		{
		  IPP_PORTRAIT,
		  IPP_LANDSCAPE,
		  IPP_REVERSE_LANDSCAPE,
		  IPP_REVERSE_PORTRAIT
		};
  static const char * const holds[] =	/* job-hold-until-supported values */
		{
		  "no-hold",
		  "indefinite",
		  "day-time",
		  "evening",
		  "night",
		  "second-shift",
		  "third-shift",
		  "weekend"
		};
  static const char * const versions[] =/* ipp-versions-supported values */
		{
		  "1.0",
		  "1.1",
		  "2.0",
		  "2.1"
		};
  static const int	ops[] =		/* operations-supported values */
		{
		  IPP_PRINT_JOB,
		  IPP_VALIDATE_JOB,
		  IPP_CREATE_JOB,
		  IPP_SEND_DOCUMENT,
		  IPP_CANCEL_JOB,
		  IPP_GET_JOB_ATTRIBUTES,
		  IPP_GET_JOBS,
		  IPP_GET_PRINTER_ATTRIBUTES,
		  IPP_HOLD_JOB,
		  IPP_RELEASE_JOB,
		  IPP_PAUSE_PRINTER,
		  IPP_RESUME_PRINTER,
		  IPP_PURGE_JOBS,
		  IPP_SET_PRINTER_ATTRIBUTES,
		  IPP_SET_JOB_ATTRIBUTES,
		  IPP_GET_PRINTER_SUPPORTED_VALUES,
		  IPP_CREATE_PRINTER_SUBSCRIPTION,
		  IPP_CREATE_JOB_SUBSCRIPTION,
		  IPP_GET_SUBSCRIPTION_ATTRIBUTES,
		  IPP_GET_SUBSCRIPTIONS,
		  IPP_RENEW_SUBSCRIPTION,
		  IPP_CANCEL_SUBSCRIPTION,
		  IPP_GET_NOTIFICATIONS,
		  IPP_ENABLE_PRINTER,
		  IPP_DISABLE_PRINTER,
		  IPP_HOLD_NEW_JOBS,
		  IPP_RELEASE_HELD_NEW_JOBS,
		  CUPS_GET_DEFAULT,
		  CUPS_GET_PRINTERS,
		  CUPS_ADD_PRINTER,
		  CUPS_DELETE_PRINTER,
		  CUPS_GET_CLASSES,
		  CUPS_ADD_CLASS,
		  CUPS_DELETE_CLASS,
		  CUPS_ACCEPT_JOBS,
		  CUPS_REJECT_JOBS,
		  CUPS_SET_DEFAULT,
		  CUPS_GET_DEVICES,
		  CUPS_GET_PPDS,
		  CUPS_MOVE_JOB,
		  CUPS_AUTHENTICATE_JOB,
		  CUPS_GET_PPD,
		  CUPS_GET_DOCUMENT,
		  IPP_RESTART_JOB
		};
  static const char * const charsets[] =/* charset-supported values */
		{
		  "us-ascii",
		  "utf-8"
		};
  static const char * const compressions[] =
		{			/* document-compression-supported values */
		  "none"
#ifdef HAVE_LIBZ
		  ,"gzip"
#endif /* HAVE_LIBZ */
		};
  static const char * const media_col_supported[] =
		{			/* media-col-supported values */
		  "media-bottom-margin",
		  "media-left-margin",
		  "media-right-margin",
		  "media-size",
		  "media-source",
		  "media-top-margin",
		  "media-type"
		};
  static const char * const multiple_document_handling[] =
		{			/* multiple-document-handling-supported values */
		  "separate-documents-uncollated-copies",
		  "separate-documents-collated-copies"
		};
  static const char * const notify_attrs[] =
		{			/* notify-attributes-supported values */
		  "printer-state-change-time",
		  "notify-lease-expiration-time",
		  "notify-subscriber-user-name"
		};
  static const char * const notify_events[] =
		{			/* notify-events-supported values */
        	  "job-completed",
        	  "job-config-changed",
        	  "job-created",
        	  "job-progress",
        	  "job-state-changed",
        	  "job-stopped",
        	  "printer-added",
        	  "printer-changed",
        	  "printer-config-changed",
        	  "printer-deleted",
        	  "printer-finishings-changed",
        	  "printer-media-changed",
        	  "printer-modified",
        	  "printer-restarted",
        	  "printer-shutdown",
        	  "printer-state-changed",
        	  "printer-stopped",
        	  "server-audit",
        	  "server-restarted",
        	  "server-started",
        	  "server-stopped"
		};
  static const char * const job_creation[] =
		{			/* job-creation-attributes-supported */
		  "copies",
		  "finishings",
		  "ipp-attribute-fidelity",
		  "job-hold-until",
		  "job-name",
		  "job-priority",
		  "job-sheets",
		  "media",
		  "media-col",
		  "multiple-document-handling",
		  "number-up",
		  "output-bin",
		  "orientation-requested",
		  "page-ranges",
		  "print-quality",
		  "printer-resolution",
		  "sides"
		};
  static const char * const job_settable[] =
		{			/* job-settable-attributes-supported */
		  "copies",
		  "finishings",
		  "job-hold-until",
		  "job-name",
		  "job-priority",
		  "media",
		  "media-col",
		  "multiple-document-handling",
		  "number-up",
		  "output-bin",
		  "orientation-requested",
		  "page-ranges",
		  "print-quality",
		  "printer-resolution",
		  "sides"
		};
  static const char * const printer_settable[] =
		{			/* printer-settable-attributes-supported */
		  "printer-info",
		  "printer-location"
	        };

//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  if (CommonData)
    ippDelete(CommonData);

  CommonData = ippNew();
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
 /*
  * This list of attributes is sorted to improve performance when the
  * client provides a requested-attributes attribute...
  */

  /* charset-configured */
  ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_CHARSET | IPP_TAG_COPY,
               "charset-configured", NULL, "utf-8");

  /* charset-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_CHARSET | IPP_TAG_COPY,
                "charset-supported", sizeof(charsets) / sizeof(charsets[0]),
		NULL, charsets);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  /* compression-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
        	"compression-supported",
		sizeof(compressions) / sizeof(compressions[0]),
		NULL, compressions);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  /* copies-supported */
  ippAddRange(CommonData, IPP_TAG_PRINTER, "copies-supported", 1, MaxCopies);

  /* cups-version */
  ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_TEXT | IPP_TAG_COPY,
               "cups-version", NULL, CUPS_SVERSION + 6);

  /* generated-natural-language-supported (no IPP_TAG_COPY) */
  ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_LANGUAGE,
               "generated-natural-language-supported", NULL, DefaultLanguage);

  /* ipp-versions-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "ipp-versions-supported", sizeof(versions) / sizeof(versions[0]),
		NULL, versions);

  /* ippget-event-life */
  ippAddInteger(CommonData, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "ippget-event-life", 15);

  /* job-creation-attributes-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "job-creation-attributes-supported",
		sizeof(job_creation) / sizeof(job_creation[0]),
		NULL, job_creation);

  /* job-hold-until-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "job-hold-until-supported", sizeof(holds) / sizeof(holds[0]),
		NULL, holds);

  /* job-priority-supported */
  ippAddInteger(CommonData, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "job-priority-supported", 100);

  /* job-settable-attributes-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "job-settable-attributes-supported",
		sizeof(job_settable) / sizeof(job_settable[0]),
		NULL, job_settable);

  /* job-sheets-supported */
  if (cupsArrayCount(Banners) > 0)
  {
   /*
    * Setup the job-sheets-supported attribute...
    */

    if (Classification && !ClassifyOverride)
      attr = ippAddString(CommonData, IPP_TAG_PRINTER,
                          IPP_TAG_NAME | IPP_TAG_COPY,
                	  "job-sheets-supported", NULL, Classification);
    else
      attr = ippAddStrings(CommonData, IPP_TAG_PRINTER,
                           IPP_TAG_NAME | IPP_TAG_COPY,
                	   "job-sheets-supported", cupsArrayCount(Banners) + 1,
			   NULL, NULL);

    if (attr == NULL)
      cupsdLogMessage(CUPSD_LOG_EMERG,
                      "Unable to allocate memory for "
                      "job-sheets-supported attribute: %s!", strerror(errno));
    else if (!Classification || ClassifyOverride)
    {
      cupsd_banner_t	*banner;	/* Current banner */


      attr->values[0].string.text = _cupsStrAlloc("none");

      for (i = 1, banner = (cupsd_banner_t *)cupsArrayFirst(Banners);
	   banner;
	   i ++, banner = (cupsd_banner_t *)cupsArrayNext(Banners))
	attr->values[i].string.text = banner->name;
    }
  }
  else
    ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_NAME | IPP_TAG_COPY,
                 "job-sheets-supported", NULL, "none");

  /* media-col-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "media-col-supported",
                sizeof(media_col_supported) /
		    sizeof(media_col_supported[0]), NULL,
	        media_col_supported);

  /* multiple-document-handling-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "multiple-document-handling-supported",
                sizeof(multiple_document_handling) /
		    sizeof(multiple_document_handling[0]), NULL,
	        multiple_document_handling);

  /* multiple-document-jobs-supported */
  ippAddBoolean(CommonData, IPP_TAG_PRINTER,
                "multiple-document-jobs-supported", 1);

  /* multiple-operation-time-out */
  ippAddInteger(CommonData, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "multiple-operation-time-out", MultipleOperationTimeout);

  /* natural-language-configured (no IPP_TAG_COPY) */
  ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_LANGUAGE,
               "natural-language-configured", NULL, DefaultLanguage);

  /* notify-attributes-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "notify-attributes-supported",
		(int)(sizeof(notify_attrs) / sizeof(notify_attrs[0])),
		NULL, notify_attrs);

  /* notify-lease-duration-supported */
  ippAddRange(CommonData, IPP_TAG_PRINTER,
              "notify-lease-duration-supported", 0,
	      MaxLeaseDuration ? MaxLeaseDuration : 2147483647);

  /* notify-max-events-supported */
  ippAddInteger(CommonData, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
               "notify-max-events-supported", MaxEvents);

  /* notify-events-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "notify-events-supported",
		(int)(sizeof(notify_events) / sizeof(notify_events[0])),
		NULL, notify_events);

  /* notify-pull-method-supported */
  ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
               "notify-pull-method-supported", NULL, "ippget");

  /* notify-schemes-supported */
  snprintf(filename, sizeof(filename), "%s/notifier", ServerBin);
  if ((dir = cupsDirOpen(filename)) != NULL)
  {
    notifiers = cupsArrayNew((cups_array_func_t)strcmp, NULL);

    while ((dent = cupsDirRead(dir)) != NULL)
      if (S_ISREG(dent->fileinfo.st_mode) &&
          (dent->fileinfo.st_mode & S_IXOTH) != 0)
        cupsArrayAdd(notifiers, _cupsStrAlloc(dent->filename));

    if (cupsArrayCount(notifiers) > 0)
    {
      attr = ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
        	           "notify-schemes-supported",
			   cupsArrayCount(notifiers), NULL, NULL);

      for (i = 0, notifier = (char *)cupsArrayFirst(notifiers);
           notifier;
	   i ++, notifier = (char *)cupsArrayNext(notifiers))
	attr->values[i].string.text = notifier;
    }

    cupsArrayDelete(notifiers);
    cupsDirClose(dir);
  }

  /* number-up-supported */
  ippAddIntegers(CommonData, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                 "number-up-supported", sizeof(nups) / sizeof(nups[0]), nups);

  /* operations-supported */
  ippAddIntegers(CommonData, IPP_TAG_PRINTER, IPP_TAG_ENUM,
                 "operations-supported",
                 sizeof(ops) / sizeof(ops[0]) + JobFiles - 1, ops);

  /* orientation-requested-supported */
  ippAddIntegers(CommonData, IPP_TAG_PRINTER, IPP_TAG_ENUM,
                 "orientation-requested-supported", 4, orients);

  /* page-ranges-supported */
  ippAddBoolean(CommonData, IPP_TAG_PRINTER, "page-ranges-supported", 1);

  /* pdf-override-supported */
  ippAddString(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
               "pdl-override-supported", NULL, "not-attempted");

  /* printer-op-policy-supported */
  attr = ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_NAME | IPP_TAG_COPY,
                       "printer-op-policy-supported", cupsArrayCount(Policies),
		       NULL, NULL);
  for (i = 0, p = (cupsd_policy_t *)cupsArrayFirst(Policies);
       p;
       i ++, p = (cupsd_policy_t *)cupsArrayNext(Policies))
    attr->values[i].string.text = p->name;

  /* printer-settable-attributes-supported */
  ippAddStrings(CommonData, IPP_TAG_PRINTER, IPP_TAG_KEYWORD | IPP_TAG_COPY,
                "printer-settable-attributes-supported",
		sizeof(printer_settable) / sizeof(printer_settable[0]),
		NULL, printer_settable);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  /* server-is-sharing-printers */
  ippAddBoolean(CommonData, IPP_TAG_PRINTER, "server-is-sharing-printers",
                BrowseLocalProtocols != 0 && Browsing);
  //printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
}


/*
 * 'cupsdDeleteAllPrinters()' - Delete all printers from the system.
 */

void
cupsdDeleteAllPrinters(void)
{
  cupsd_printer_t	*p;		/* Pointer to current printer/class */


  for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
       p;
       p = (cupsd_printer_t *)cupsArrayNext(Printers))
  {
    p->op_policy_ptr = DefaultPolicyPtr;
    cupsdDeletePrinter(p, 0);
  }
}


/*
 * 'cupsdDeletePrinter()' - Delete a printer from the system.
 */

int					/* O - 1 if classes affected, 0 otherwise */
cupsdDeletePrinter(
    cupsd_printer_t *p,			/* I - Printer to delete */
    int             update)		/* I - Update printers.conf? */
{
  int	i,				/* Looping var */
	changed = 0;			/* Class changed? */
#ifdef __sgi
  char	filename[1024];			/* Interface script filename */
#endif /* __sgi */


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cupsdDeletePrinter(p=%p(%s), update=%d)",
                  p, p->name, update);

 /*
  * Save the current position in the Printers array...
  */

  cupsArraySave(Printers);

 /*
  * Stop printing on this printer...
  */

  cupsdSetPrinterState(p, IPP_PRINTER_STOPPED, update);

  if (p->job)
    cupsdSetJobState(p->job, IPP_JOB_PENDING, CUPSD_JOB_FORCE,
                     update ? "Job stopped due to printer being deleted." :
		              "Job stopped.");

 /*
  * If this printer is the next for browsing, point to the next one...
  */

  if (p == BrowseNext)
  {
    cupsArrayFind(Printers, p);
    BrowseNext = (cupsd_printer_t *)cupsArrayNext(Printers);
  }

 /*
  * Remove the printer from the list...
  */

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cupsdDeletePrinter: Removing %s from Printers", p->name);
  cupsArrayRemove(Printers, p);

  if (p->type & CUPS_PRINTER_IMPLICIT)
  {
    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "cupsdDeletePrinter: Removing %s from ImplicitPrinters",
		    p->name);
    cupsArrayRemove(ImplicitPrinters, p);
  }

 /*
  * Remove the dummy interface/icon/option files under IRIX...
  */

#ifdef __sgi
  snprintf(filename, sizeof(filename), "/var/spool/lp/interface/%s", p->name);
  unlink(filename);

  snprintf(filename, sizeof(filename), "/var/spool/lp/gui_interface/ELF/%s.gui",
           p->name);
  unlink(filename);

  snprintf(filename, sizeof(filename), "/var/spool/lp/activeicons/%s", p->name);
  unlink(filename);

  snprintf(filename, sizeof(filename), "/var/spool/lp/pod/%s.config", p->name);
  unlink(filename);

  snprintf(filename, sizeof(filename), "/var/spool/lp/pod/%s.status", p->name);
  unlink(filename);

  snprintf(filename, sizeof(filename), "/var/spool/lp/member/%s", p->name);
  unlink(filename);
#endif /* __sgi */

 /*
  * If p is the default printer, assign a different one...
  */

  if (p == DefaultPrinter)
  {
    DefaultPrinter = NULL;

    if (UseNetworkDefault)
    {
     /*
      * Find the first network default printer and use it...
      */

      cupsd_printer_t	*dp;		/* New default printer */


      for (dp = (cupsd_printer_t *)cupsArrayFirst(Printers);
	   dp;
	   dp = (cupsd_printer_t *)cupsArrayNext(Printers))
	if (dp != p && (dp->type & CUPS_PRINTER_DEFAULT))
	{
	  DefaultPrinter = dp;
	  break;
	}
    }
  }

 /*
  * Remove this printer from any classes...
  */

  if (!(p->type & CUPS_PRINTER_IMPLICIT))
  {
//    changed = cupsdDeletePrinterFromClasses(p);
     changed=1;
   /*
    * Deregister from any browse protocols...
    */

    cupsdDeregisterPrinter(p, 1);
  }

 /*
  * Free all memory used by the printer...
  */

  if (p->printers != NULL)
    free(p->printers);

  if (MaxPrinterHistory)
  {
    for (i = 0; i < p->num_history; i ++)
      ippDelete(p->history[i]);

    free(p->history);
  }

  delete_printer_filters(p);

  for (i = 0; i < p->num_reasons; i ++)
    _cupsStrFree(p->reasons[i]);

  ippDelete(p->attrs);
  ippDelete(p->ppd_attrs);

  mimeDeleteType(MimeDatabase, p->filetype);
  mimeDeleteType(MimeDatabase, p->prefiltertype);

  delete_string_array(&(p->filters));
  delete_string_array(&(p->pre_filters));
#ifdef NOT_CUT
  cupsdFreePrinterUsers(p);

  cupsdFreeQuotas(p);
#endif

  cupsdClearString(&p->uri);
  cupsdClearString(&p->hostname);
  cupsdClearString(&p->name);
  cupsdClearString(&p->location);
  cupsdClearString(&p->make_model);
  cupsdClearString(&p->info);
  cupsdClearString(&p->job_sheets[0]);
  cupsdClearString(&p->job_sheets[1]);
  cupsdClearString(&p->device_uri);
  cupsdClearString(&p->sanitized_device_uri);
  cupsdClearString(&p->port_monitor);
  cupsdClearString(&p->op_policy);
  cupsdClearString(&p->error_policy);

  cupsdClearString(&p->alert);
  cupsdClearString(&p->alert_description);

#ifdef HAVE_DNSSD
  cupsdClearString(&p->product);
  cupsdClearString(&p->pdl);
#endif /* HAVE_DNSSD */

  cupsArrayDelete(p->filetypes);

  if (p->browse_attrs)
    free(p->browse_attrs);

#ifdef __APPLE__
  cupsdClearString(&p->recoverable);
#endif /* __APPLE__ */

  cupsFreeOptions(p->num_options, p->options);

  free(p);

 /*
  * Restore the previous position in the Printers array...
  */

  cupsArrayRestore(Printers);

  return (changed);
}


/*
 * 'cupsdFindDest()' - Find a destination in the list.
 */

cupsd_printer_t *			/* O - Destination in list */
cupsdFindDest(const char *name)		/* I - Name of printer or class to find */
{
  cupsd_printer_t	key;		/* Search key */


  key.name = (char *)name;
  //printf("\n==file:%s,line:%d,func:%s=name=%s,=\n",__FILE__,__LINE__,__FUNCTION__,name);
  return ((cupsd_printer_t *)cupsArrayFind(Printers, &key));
}


/*
 * 'cupsdFindPrinter()' - Find a printer in the list.
 */

cupsd_printer_t *			/* O - Printer in list */
cupsdFindPrinter(const char *name)	/* I - Name of printer to find */
{
  cupsd_printer_t	*p;		/* Printer in list */


  if ((p = cupsdFindDest(name)) != NULL && (p->type & CUPS_PRINTER_CLASS))
    return (NULL);
  else
    return (p);
}


/*
 * 'cupsdFreePrinterUsers()' - Free allow/deny users.
 */
#ifdef NOT_CUT
void
cupsdFreePrinterUsers(
    cupsd_printer_t *p)			/* I - Printer */
{
  int	i;				/* Looping var */


  if (!p || !p->num_users)
    return;

  for (i = 0; i < p->num_users; i ++)
    free((void *)p->users[i]);

  free(p->users);

  p->num_users = 0;
  p->users     = NULL;
}
#endif

/*
 * 'cupsdLoadAllPrinters()' - Load printers from the printers.conf file.
 */

void
cupsdLoadAllPrinters(void)
{
  int			i;		/* Looping var */
  cups_file_t		*fp;		/* printers.conf file */
  int			linenum;	/* Current line number */
  char			line[4096],	/* Line from file */
			*value,		/* Pointer to value */
			*valueptr;	/* Pointer into value */
  cupsd_printer_t	*p;		/* Current printer */


 /*
  * Open the printers.conf file...
  */

  snprintf(line, sizeof(line), "%s/printers.conf", ServerRoot);
  if ((fp = cupsFileOpen(line, "r")) == NULL)
  {
    if (errno != ENOENT)
      cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to open %s - %s", line,
		      strerror(errno));
    return;
  }

 /*
  * Read printer configurations until we hit EOF...
  */

  linenum = 0;
  p       = NULL;

  while (cupsFileGetConf(fp, line, sizeof(line), &value, &linenum))
  {
   /*
    * Decode the directive...
    */

    if (!strcasecmp(line, "<Printer") ||
        !strcasecmp(line, "<DefaultPrinter"))
    {
     /*
      * <Printer name> or <DefaultPrinter name>
      */

      if (p == NULL && value)
      {
       /*
        * Add the printer and a base file type...
	*/

        cupsdLogMessage(CUPSD_LOG_DEBUG, "Loading printer %s...", value);
        p = cupsdAddPrinter(value);
	p->accepting = 1;
	p->state     = IPP_PRINTER_IDLE;

       /*
        * Set the default printer as needed...
	*/

        if (!strcasecmp(line, "<DefaultPrinter"))
	  DefaultPrinter = p;
      }
      else
        cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "</Printer>"))
    {
      if (p != NULL)
      {
       /*
        * Close out the current printer...
	*/

        cupsdSetPrinterAttrs(p);
  #ifdef NOT_CUT      
	cupsdAddPrinterHistory(p);
  #endif

        if (strncmp(p->device_uri, "file:", 5) &&
	    p->state != IPP_PRINTER_STOPPED)
	{
	 /*
          * See if the backend exists...
	  */
 

	  snprintf(line, sizeof(line), "%s/backend/%s", ServerBin,
	           p->device_uri);

          if ((valueptr = strchr(line + strlen(ServerBin), ':')) != NULL)
	    *valueptr = '\0';		/* Chop everything but URI scheme */

          if (access(line, 0))
	  {
	   /*
	    * Backend does not exist, stop printer...
	    */

	    p->state = IPP_PRINTER_STOPPED;
	    snprintf(p->state_message, sizeof(p->state_message),
	             "Backend %s does not exist!", line);
	  }
        }

        p = NULL;
      }
      else
        cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!p)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Syntax error on line %d of printers.conf.", linenum);
    }
    #ifdef NOT_CUT
    else if (!strcasecmp(line, "AuthInfoRequired"))
    {
      if (!cupsdSetAuthInfoRequired(p, value, NULL))
	cupsdLogMessage(CUPSD_LOG_ERROR,
			"Bad AuthInfoRequired on line %d of printers.conf.",
			linenum);
    }
    #endif
    else if (!strcasecmp(line, "Info"))
    {
      if (value)
	cupsdSetString(&p->info, value);
    }
    else if (!strcasecmp(line, "MakeModel"))
    {
      if (value)
	cupsdSetString(&p->make_model, value);
    }
    else if (!strcasecmp(line, "Location"))
    {
      if (value)
	cupsdSetString(&p->location, value);
    }
    else if (!strcasecmp(line, "DeviceURI"))
    {
      if (value)
	cupsdSetDeviceURI(p, value);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Option") && value)
    {
     /*
      * Option name value
      */

      for (valueptr = value; *valueptr && !isspace(*valueptr & 255); valueptr ++);

      if (!*valueptr)
        cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
      else
      {
        for (; *valueptr && isspace(*valueptr & 255); *valueptr++ = '\0');

        p->num_options = cupsAddOption(value, valueptr, p->num_options,
	                               &(p->options));
      }
    }
    else if (!strcasecmp(line, "PortMonitor"))
    {
      if (value && strcmp(value, "none"))
	cupsdSetString(&p->port_monitor, value);
      else if (value)
        cupsdClearString(&p->port_monitor);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Reason"))
    {
      if (value &&
          strcmp(value, "com.apple.print.recoverable-warning") &&
          strcmp(value, "connecting-to-device") &&
          strcmp(value, "cups-insecure-filter-warning") &&
          strcmp(value, "cups-missing-filter-warning"))
      {
        for (i = 0 ; i < p->num_reasons; i ++)
	  if (!strcmp(value, p->reasons[i]))
	    break;

        if (i >= p->num_reasons &&
	    p->num_reasons < (int)(sizeof(p->reasons) / sizeof(p->reasons[0])))
	{
	  p->reasons[p->num_reasons] = _cupsStrAlloc(value);
	  p->num_reasons ++;
	}
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "State"))
    {
     /*
      * Set the initial queue state...
      */

      if (value && !strcasecmp(value, "idle"))
        p->state = IPP_PRINTER_IDLE;
      else if (value && !strcasecmp(value, "stopped"))
      {
        p->state = IPP_PRINTER_STOPPED;

        for (i = 0 ; i < p->num_reasons; i ++)
	  if (!strcmp("paused", p->reasons[i]))
	    break;

        if (i >= p->num_reasons &&
	    p->num_reasons < (int)(sizeof(p->reasons) / sizeof(p->reasons[0])))
	{
	  p->reasons[p->num_reasons] = _cupsStrAlloc("paused");
	  p->num_reasons ++;
	}
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "StateMessage"))
    {
     /*
      * Set the initial queue state message...
      */

      if (value)
	strlcpy(p->state_message, value, sizeof(p->state_message));
    }
    else if (!strcasecmp(line, "StateTime"))
    {
     /*
      * Set the state time...
      */

      if (value)
        p->state_time = atoi(value);
    }
    else if (!strcasecmp(line, "Accepting"))
    {
     /*
      * Set the initial accepting state...
      */

      if (value &&
          (!strcasecmp(value, "yes") ||
           !strcasecmp(value, "on") ||
           !strcasecmp(value, "true")))
        p->accepting = 1;
      else if (value &&
               (!strcasecmp(value, "no") ||
        	!strcasecmp(value, "off") ||
        	!strcasecmp(value, "false")))
        p->accepting = 0;
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Type"))
    {
      if (value)
        p->type = atoi(value);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Product"))
    {
      if (value)
      {
#ifdef HAVE_DNSSD
        p->product = _cupsStrAlloc(value);
#endif /* HAVE_DNSSD */
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Filter"))
    {
      if (value)
      {
        if (!p->filters)
	  p->filters = cupsArrayNew(NULL, NULL);

	cupsArrayAdd(p->filters, _cupsStrAlloc(value));
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "PreFilter"))
    {
      if (value)
      {
        if (!p->pre_filters)
	  p->pre_filters = cupsArrayNew(NULL, NULL);

	cupsArrayAdd(p->pre_filters, _cupsStrAlloc(value));
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Shared"))
    {
     /*
      * Set the initial shared state...
      */

      if (value &&
          (!strcasecmp(value, "yes") ||
           !strcasecmp(value, "on") ||
           !strcasecmp(value, "true")))
        p->shared = 1;
      else if (value &&
               (!strcasecmp(value, "no") ||
        	!strcasecmp(value, "off") ||
        	!strcasecmp(value, "false")))
        p->shared = 0;
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "JobSheets"))
    {
     /*
      * Set the initial job sheets...
      */

      if (value)
      {
	for (valueptr = value; *valueptr && !isspace(*valueptr & 255); valueptr ++);

	if (*valueptr)
          *valueptr++ = '\0';

	cupsdSetString(&p->job_sheets[0], value);

	while (isspace(*valueptr & 255))
          valueptr ++;

	if (*valueptr)
	{
          for (value = valueptr; *valueptr && !isspace(*valueptr & 255); valueptr ++);

	  if (*valueptr)
            *valueptr = '\0';

	  cupsdSetString(&p->job_sheets[1], value);
	}
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "AllowUser"))
    {
      if (value)
      {
        p->deny_users = 0;
        #ifdef NOT_CUT
        cupsdAddPrinterUser(p, value);
        #endif
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "DenyUser"))
    {
      if (value)
      {
        p->deny_users = 1;
        #ifdef NOT_CUT
        cupsdAddPrinterUser(p, value);
        #endif
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "QuotaPeriod"))
    {
      if (value)
        p->quota_period = atoi(value);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "PageLimit"))
    {
      if (value)
        p->page_limit = atoi(value);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "KLimit"))
    {
      if (value)
        p->k_limit = atoi(value);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "OpPolicy"))
    {
      if (value)
      {
        cupsd_policy_t *pol;		/* Policy */

#ifdef NOT_CUT

        if ((pol = cupsdFindPolicy(value)) != NULL)
	{
          cupsdSetString(&p->op_policy, value);
	  p->op_policy_ptr = pol;
	}
	else
	  cupsdLogMessage(CUPSD_LOG_ERROR,
	                  "Bad policy \"%s\" on line %d of printers.conf",
			  value, linenum);
#endif
      }
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "ErrorPolicy"))
    {
      if (value)
        cupsdSetString(&p->error_policy, value);
      else
	cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
    }
    else if (!strcasecmp(line, "Attribute") && value)
    {
      for (valueptr = value; *valueptr && !isspace(*valueptr & 255); valueptr ++);

      if (!*valueptr)
        cupsdLogMessage(CUPSD_LOG_ERROR,
	                "Syntax error on line %d of printers.conf.", linenum);
      else
      {
        for (; *valueptr && isspace(*valueptr & 255); *valueptr++ = '\0');

        if (!p->attrs)
            printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);
	  cupsdSetPrinterAttrs(p);

        if (!strcmp(value, "marker-change-time"))
	  p->marker_time = atoi(valueptr);
	else
          cupsdSetPrinterAttr(p, value, valueptr);
      }
    }
    else
    {
     /*
      * Something else we don't understand...
      */

      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unknown configuration directive %s on line %d of printers.conf.",
	              line, linenum);
    }
  }

  cupsFileClose(fp);
}


/*
 * 'cupsdRenamePrinter()' - Rename a printer.
 */
#ifdef NOT_CUT
void
cupsdRenamePrinter(
    cupsd_printer_t *p,			/* I - Printer */
    const char      *name)		/* I - New name */
{
 /*
  * Remove the printer from the array(s) first...
  */

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cupsdRenamePrinter: Removing %s from Printers", p->name);
  cupsArrayRemove(Printers, p);

  if (p->type & CUPS_PRINTER_IMPLICIT)
  {
    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "cupsdRenamePrinter: Removing %s from ImplicitPrinters",
		    p->name);
    cupsArrayRemove(ImplicitPrinters, p);
  }

 /*
  * Rename the printer type...
  */

  mimeDeleteType(MimeDatabase, p->filetype);
  p->filetype = mimeAddType(MimeDatabase, "printer", name);

  mimeDeleteType(MimeDatabase, p->prefiltertype);
  p->prefiltertype = mimeAddType(MimeDatabase, "prefilter", name);

 /*
  * Rename the printer...
  */

  cupsdSetString(&p->name, name);

 /*
  * Reset printer attributes...
  */

  cupsdSetPrinterAttrs(p);

 /*
  * Add the printer back to the printer array(s)...
  */

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cupsdRenamePrinter: Adding %s to Printers", p->name);
  cupsArrayAdd(Printers, p);

  if (p->type & CUPS_PRINTER_IMPLICIT)
  {
    cupsdLogMessage(CUPSD_LOG_DEBUG2,
		    "cupsdRenamePrinter: Adding %s to ImplicitPrinters",
		    p->name);
    cupsArrayAdd(ImplicitPrinters, p);
  }
}
#endif

/*
 * 'cupsdSaveAllPrinters()' - Save all printer definitions to the printers.conf
 *                            file.
 */
#ifdef NOT_CUT
void
cupsdSaveAllPrinters(void)
{
  int			i;		/* Looping var */
  cups_file_t		*fp;		/* printers.conf file */
  char			temp[1024],	/* Temporary string */
			backup[1024],	/* printers.conf.O file */
			value[2048],	/* Value string */
			*ptr;		/* Pointer into value */
  cupsd_printer_t	*printer;	/* Current printer class */
  time_t		curtime;	/* Current time */
  struct tm		*curdate;	/* Current date */
  cups_option_t		*option;	/* Current option */
  ipp_attribute_t	*marker;	/* Current marker attribute */


 /*
  * Create the printers.conf file...
  */

  snprintf(temp, sizeof(temp), "%s/printers.conf", ServerRoot);
  snprintf(backup, sizeof(backup), "%s/printers.conf.O", ServerRoot);

  if (rename(temp, backup))
  {
    if (errno != ENOENT)
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to backup printers.conf - %s", strerror(errno));
  }

  if ((fp = cupsFileOpen(temp, "w")) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to save printers.conf - %s", strerror(errno));

    if (rename(backup, temp))
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to restore printers.conf - %s", strerror(errno));
    return;
  }
  else
    cupsdLogMessage(CUPSD_LOG_INFO, "Saving printers.conf...");

 /*
  * Restrict access to the file...
  */

  fchown(cupsFileNumber(fp), getuid(), Group);
  fchmod(cupsFileNumber(fp), 0600);

 /*
  * Write a small header to the file...
  */

  curtime = time(NULL);
  curdate = localtime(&curtime);
  strftime(temp, sizeof(temp) - 1, "%Y-%m-%d %H:%M", curdate);

  cupsFilePuts(fp, "# Printer configuration file for " CUPS_SVERSION "\n");
  cupsFilePrintf(fp, "# Written by cupsd on %s\n", temp);
  cupsFilePuts(fp, "# DO NOT EDIT THIS FILE WHEN CUPSD IS RUNNING\n");

 /*
  * Write each local printer known to the system...
  */

  for (printer = (cupsd_printer_t *)cupsArrayFirst(Printers);
       printer;
       printer = (cupsd_printer_t *)cupsArrayNext(Printers))
  {
   /*
    * Skip remote destinations and printer classes...
    */

    if ((printer->type & CUPS_PRINTER_DISCOVERED) ||
        (printer->type & CUPS_PRINTER_CLASS) ||
	(printer->type & CUPS_PRINTER_IMPLICIT))
      continue;

   /*
    * Write printers as needed...
    */

    if (printer == DefaultPrinter)
      cupsFilePrintf(fp, "<DefaultPrinter %s>\n", printer->name);
    else
      cupsFilePrintf(fp, "<Printer %s>\n", printer->name);

    if (printer->num_auth_info_required > 0)
    {
      switch (printer->num_auth_info_required)
      {
        case 1 :
            strlcpy(value, printer->auth_info_required[0], sizeof(value));
	    break;

        case 2 :
            snprintf(value, sizeof(value), "%s,%s",
	             printer->auth_info_required[0],
		     printer->auth_info_required[1]);
	    break;

        case 3 :
	default :
            snprintf(value, sizeof(value), "%s,%s,%s",
	             printer->auth_info_required[0],
		     printer->auth_info_required[1],
		     printer->auth_info_required[2]);
	    break;
      }

      cupsFilePutConf(fp, "AuthInfoRequired", value);
    }

    if (printer->info)
      cupsFilePutConf(fp, "Info", printer->info);

    if (printer->location)
      cupsFilePutConf(fp, "Location", printer->location);

    if (printer->make_model)
      cupsFilePutConf(fp, "MakeModel", printer->make_model);

    cupsFilePutConf(fp, "DeviceURI", printer->device_uri);

    if (printer->port_monitor)
      cupsFilePutConf(fp, "PortMonitor", printer->port_monitor);

    if (printer->state == IPP_PRINTER_STOPPED)
    {
      cupsFilePuts(fp, "State Stopped\n");

      if (printer->state_message)
        cupsFilePutConf(fp, "StateMessage", printer->state_message);
    }
    else
      cupsFilePuts(fp, "State Idle\n");

    cupsFilePrintf(fp, "StateTime %d\n", (int)printer->state_time);

    for (i = 0; i < printer->num_reasons; i ++)
      if (strcmp(printer->reasons[i], "com.apple.print.recoverable-warning") &&
          strcmp(printer->reasons[i], "connecting-to-device") &&
          strcmp(printer->reasons[i], "cups-insecure-filter-warning") &&
          strcmp(printer->reasons[i], "cups-missing-filter-warning"))
        cupsFilePutConf(fp, "Reason", printer->reasons[i]);

    cupsFilePrintf(fp, "Type %d\n", printer->type);

#ifdef HAVE_DNSSD
    if (printer->product)
      cupsFilePutConf(fp, "Product", printer->product);
#endif /* HAVE_DNSSD */

    for (ptr = (char *)cupsArrayFirst(printer->filters);
         ptr;
	 ptr = (char *)cupsArrayNext(printer->filters))
      cupsFilePutConf(fp, "Filter", ptr);

    for (ptr = (char *)cupsArrayFirst(printer->pre_filters);
         ptr;
	 ptr = (char *)cupsArrayNext(printer->pre_filters))
      cupsFilePutConf(fp, "PreFilter", ptr);

    if (printer->accepting)
      cupsFilePuts(fp, "Accepting Yes\n");
    else
      cupsFilePuts(fp, "Accepting No\n");

    if (printer->shared)
      cupsFilePuts(fp, "Shared Yes\n");
    else
      cupsFilePuts(fp, "Shared No\n");

    snprintf(value, sizeof(value), "%s %s", printer->job_sheets[0],
             printer->job_sheets[1]);
    cupsFilePutConf(fp, "JobSheets", value);

    cupsFilePrintf(fp, "QuotaPeriod %d\n", printer->quota_period);
    cupsFilePrintf(fp, "PageLimit %d\n", printer->page_limit);
    cupsFilePrintf(fp, "KLimit %d\n", printer->k_limit);

    for (i = 0; i < printer->num_users; i ++)
      cupsFilePutConf(fp, printer->deny_users ? "DenyUser" : "AllowUser",
                      printer->users[i]);

    if (printer->op_policy)
      cupsFilePutConf(fp, "OpPolicy", printer->op_policy);
    if (printer->error_policy)
      cupsFilePutConf(fp, "ErrorPolicy", printer->error_policy);

    for (i = printer->num_options, option = printer->options;
         i > 0;
	 i --, option ++)
    {
      snprintf(value, sizeof(value), "%s %s", option->name, option->value);
      cupsFilePutConf(fp, "Option", value);
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-colors",
                                   IPP_TAG_NAME)) != NULL)
    {
      snprintf(value, sizeof(value), "%s ", marker->name);

      for (i = 0, ptr = value + strlen(value);
           i < marker->num_values && ptr < (value + sizeof(value) - 1);
	   i ++)
      {
        if (i)
	  *ptr++ = ',';

        strlcpy(ptr, marker->values[i].string.text,
	        value + sizeof(value) - ptr);
        ptr += strlen(ptr);
      }

      *ptr = '\0';
      cupsFilePutConf(fp, "Attribute", value);
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-levels",
                                   IPP_TAG_INTEGER)) != NULL)
    {
      cupsFilePrintf(fp, "Attribute %s %d", marker->name,
                     marker->values[0].integer);
      for (i = 1; i < marker->num_values; i ++)
        cupsFilePrintf(fp, ",%d", marker->values[i].integer);
      cupsFilePuts(fp, "\n");
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-low-levels",
                                   IPP_TAG_INTEGER)) != NULL)
    {
      cupsFilePrintf(fp, "Attribute %s %d", marker->name,
                     marker->values[0].integer);
      for (i = 1; i < marker->num_values; i ++)
        cupsFilePrintf(fp, ",%d", marker->values[i].integer);
      cupsFilePuts(fp, "\n");
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-high-levels",
                                   IPP_TAG_INTEGER)) != NULL)
    {
      cupsFilePrintf(fp, "Attribute %s %d", marker->name,
                     marker->values[0].integer);
      for (i = 1; i < marker->num_values; i ++)
        cupsFilePrintf(fp, ",%d", marker->values[i].integer);
      cupsFilePuts(fp, "\n");
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-message",
                                   IPP_TAG_TEXT)) != NULL)
    {
      snprintf(value, sizeof(value), "%s %s", marker->name,
               marker->values[0].string.text);

      cupsFilePutConf(fp, "Attribute", value);
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-names",
                                   IPP_TAG_NAME)) != NULL)
    {
      snprintf(value, sizeof(value), "%s ", marker->name);

      for (i = 0, ptr = value + strlen(value);
           i < marker->num_values && ptr < (value + sizeof(value) - 1);
	   i ++)
      {
        if (i)
	  *ptr++ = ',';

        strlcpy(ptr, marker->values[i].string.text,
	        value + sizeof(value) - ptr);
        ptr += strlen(ptr);
      }

      *ptr = '\0';
      cupsFilePutConf(fp, "Attribute", value);
    }

    if ((marker = ippFindAttribute(printer->attrs, "marker-types",
                                   IPP_TAG_KEYWORD)) != NULL)
    {
      snprintf(value, sizeof(value), "%s ", marker->name);

      for (i = 0, ptr = value + strlen(value);
           i < marker->num_values && ptr < (value + sizeof(value) - 1);
	   i ++)
      {
        if (i)
	  *ptr++ = ',';

        strlcpy(ptr, marker->values[i].string.text,
	        value + sizeof(value) - ptr);
        ptr += strlen(ptr);
      }

      *ptr = '\0';
      cupsFilePutConf(fp, "Attribute", value);
    }

    if (printer->marker_time)
      cupsFilePrintf(fp, "Attribute marker-change-time %ld\n",
                     (long)printer->marker_time);

    cupsFilePuts(fp, "</Printer>\n");

#ifdef __sgi
    /*
     * Make IRIX desktop & printer status happy
     */

    write_irix_state(printer);
#endif /* __sgi */
  }

  cupsFileClose(fp);
}
#endif

/*
 * 'cupsdSetAuthInfoRequired()' - Set the required authentication info.
 */
#ifdef NOT_CUT
int					/* O - 1 if value OK, 0 otherwise */
cupsdSetAuthInfoRequired(
    cupsd_printer_t *p,			/* I - Printer */
    const char      *values,		/* I - Plain text value (or NULL) */
    ipp_attribute_t *attr)		/* I - IPP attribute value (or NULL) */
{
  int	i;				/* Looping var */


  p->num_auth_info_required = 0;

 /*
  * Do we have a plain text value?
  */

  if (values)
  {
   /*
    * Yes, grab the keywords...
    */

    const char	*end;			/* End of current value */


    while (*values && p->num_auth_info_required < 4)
    {
      if ((end = strchr(values, ',')) == NULL)
        end = values + strlen(values);

      if ((end - values) == 4 && !strncmp(values, "none", 4))
      {
        if (p->num_auth_info_required != 0 || *end)
	  return (0);

        p->auth_info_required[p->num_auth_info_required] = "none";
	p->num_auth_info_required ++;

	return (1);
      }
      else if ((end - values) == 9 && !strncmp(values, "negotiate", 9))
      {
        if (p->num_auth_info_required != 0 || *end)
	  return (0);

        p->auth_info_required[p->num_auth_info_required] = "negotiate";
	p->num_auth_info_required ++;
      }
      else if ((end - values) == 6 && !strncmp(values, "domain", 6))
      {
        p->auth_info_required[p->num_auth_info_required] = "domain";
	p->num_auth_info_required ++;
      }
      else if ((end - values) == 8 && !strncmp(values, "password", 8))
      {
        p->auth_info_required[p->num_auth_info_required] = "password";
	p->num_auth_info_required ++;
      }
      else if ((end - values) == 8 && !strncmp(values, "username", 8))
      {
        p->auth_info_required[p->num_auth_info_required] = "username";
	p->num_auth_info_required ++;
      }
      else
        return (0);

      values = (*end) ? end + 1 : end;
    }

    if (p->num_auth_info_required == 0)
    {
      p->auth_info_required[0]  = "none";
      p->num_auth_info_required = 1;
    }

   /*
    * Update the printer-type value as needed...
    */

    if (p->num_auth_info_required > 1 ||
        strcmp(p->auth_info_required[0], "none"))
      p->type |= CUPS_PRINTER_AUTHENTICATED;
    else
      p->type &= ~CUPS_PRINTER_AUTHENTICATED;

    return (1);
  }

 /*
  * Grab values from an attribute instead...
  */

  if (!attr || attr->num_values > 4)
    return (0);

 /*
  * Update the printer-type value as needed...
  */

  if (attr->num_values > 1 ||
      strcmp(attr->values[0].string.text, "none"))
    p->type |= CUPS_PRINTER_AUTHENTICATED;
  else
    p->type &= ~CUPS_PRINTER_AUTHENTICATED;

  for (i = 0; i < attr->num_values; i ++)
  {
    if (!strcmp(attr->values[i].string.text, "none"))
    {
      if (p->num_auth_info_required != 0 || attr->num_values != 1)
	return (0);

      p->auth_info_required[p->num_auth_info_required] = "none";
      p->num_auth_info_required ++;

      return (1);
    }
    else if (!strcmp(attr->values[i].string.text, "negotiate"))
    {
      if (p->num_auth_info_required != 0 || attr->num_values != 1)
	return (0);

      p->auth_info_required[p->num_auth_info_required] = "negotiate";
      p->num_auth_info_required ++;

      return (1);
    }
    else if (!strcmp(attr->values[i].string.text, "domain"))
    {
      p->auth_info_required[p->num_auth_info_required] = "domain";
      p->num_auth_info_required ++;
    }
    else if (!strcmp(attr->values[i].string.text, "password"))
    {
      p->auth_info_required[p->num_auth_info_required] = "password";
      p->num_auth_info_required ++;
    }
    else if (!strcmp(attr->values[i].string.text, "username"))
    {
      p->auth_info_required[p->num_auth_info_required] = "username";
      p->num_auth_info_required ++;
    }
    else
      return (0);
  }

  return (1);
}
#endif

/*
 * 'cupsdSetDeviceURI()' - Set the device URI for a printer.
 */

void
cupsdSetDeviceURI(cupsd_printer_t *p,	/* I - Printer */
                  const char      *uri)	/* I - Device URI */
{
  char	buffer[1024],			/* URI buffer */
	*start,				/* Start of data after scheme */
	*slash,				/* First slash after scheme:// */
	*ptr;				/* Pointer into user@host:port part */


 /*
  * Set the full device URI..
  */

  cupsdSetString(&(p->device_uri), uri);

 /*
  * Copy the device URI to a temporary buffer so we can sanitize any auth
  * info in it...
  */

  strlcpy(buffer, uri, sizeof(buffer));

 /*
  * Find the end of the scheme:// part...
  */

  if ((ptr = strchr(buffer, ':')) != NULL)
  {
    for (start = ptr + 1; *start; start ++)
      if (*start != '/')
        break;

   /*
    * Find the next slash (/) in the URI...
    */

    if ((slash = strchr(start, '/')) == NULL)
      slash = start + strlen(start);	/* No slash, point to the end */

   /*
    * Check for an @ sign before the slash...
    */

    if ((ptr = strchr(start, '@')) != NULL && ptr < slash)
    {
     /*
      * Found an @ sign and it is before the resource part, so we have
      * an authentication string.  Copy the remaining URI over the
      * authentication string...
      */

      _cups_strcpy(start, ptr + 1);
    }
  }

 /*
  * Save the sanitized URI...
  */

  cupsdSetString(&(p->sanitized_device_uri), buffer);
}


/*
 * 'cupsdSetPrinterAttr()' - Set a printer attribute.
 */

void
cupsdSetPrinterAttr(
    cupsd_printer_t *p,			/* I - Printer */
    const char      *name,		/* I - Attribute name */
    char            *value)		/* I - Attribute value string */
{
  ipp_attribute_t	*attr;		/* Attribute */
  int			i,		/* Looping var */
			count;		/* Number of values */
  char			*ptr;		/* Pointer into value */
  ipp_tag_t		value_tag;	/* Value tag for this attribute */


 /*
  * Don't allow empty values...
  */

  if (!*value && strcmp(name, "marker-message"))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Ignoring empty \"%s\" attribute", name);
    return;
  }

 /*
  * Count the number of values...
  */

  for (count = 1, ptr = value;
       (ptr = strchr(ptr, ',')) != NULL;
       ptr ++, count ++);

 /*
  * Then add or update the attribute as needed...
  */

  if (!strcmp(name, "marker-levels") || !strcmp(name, "marker-low-levels") ||
      !strcmp(name, "marker-high-levels"))
  {
   /*
    * Integer values...
    */

    if ((attr = ippFindAttribute(p->attrs, name, IPP_TAG_INTEGER)) != NULL &&
        attr->num_values < count)
    {
      ippDeleteAttribute(p->attrs, attr);
      attr = NULL;
    }

    if (attr)
      attr->num_values = count;
    else
      attr = ippAddIntegers(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER, name,
                            count, NULL);

    if (!attr)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to allocate memory for printer attribute "
		      "(%d values)", count);
      return;
    }

    for (i = 0; i < count; i ++)
    {
      if ((ptr = strchr(value, ',')) != NULL)
        *ptr++ = '\0';

      attr->values[i].integer = strtol(value, NULL, 10);

      if (ptr)
        value = ptr;
    }
  }
  else
  {
   /*
    * Name or keyword values...
    */

    if (!strcmp(name, "marker-types"))
      value_tag = IPP_TAG_KEYWORD;
    else if (!strcmp(name, "marker-message"))
      value_tag = IPP_TAG_TEXT;
    else
      value_tag = IPP_TAG_NAME;

    if ((attr = ippFindAttribute(p->attrs, name, value_tag)) != NULL &&
        attr->num_values < count)
    {
      ippDeleteAttribute(p->attrs, attr);
      attr = NULL;
    }
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
    if (attr)
    {
      for (i = 0; i < attr->num_values; i ++)
	_cupsStrFree(attr->values[i].string.text);

      attr->num_values = count;
    }
    else
      attr = ippAddStrings(p->attrs, IPP_TAG_PRINTER, value_tag, name,
                           count, NULL, NULL);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
    if (!attr)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to allocate memory for printer attribute "
		      "(%d values)", count);
      return;
    }

    for (i = 0; i < count; i ++)
    {
      if ((ptr = strchr(value, ',')) != NULL)
        *ptr++ = '\0';

      attr->values[i].string.text = _cupsStrAlloc(value);

      if (ptr)
        value = ptr;
    }
  }
  //printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
}


/*
 * 'cupsdSetPrinterAttrs()' - Set printer attributes based upon the PPD file.
 */

void
cupsdSetPrinterAttrs(cupsd_printer_t *p)/* I - Printer to setup */
{
  int		i,			/* Looping var */
		length;			/* Length of browse attributes */
  char		resource[HTTP_MAX_URI];	/* Resource portion of URI */
  int		num_air;		/* Number of auth-info-required values */
  const char	* const *air;		/* auth-info-required values */
  cupsd_location_t *auth=NULL;		/* Pointer to authentication element */
  const char	*auth_supported;	/* Authentication supported */
  ipp_t		*oldattrs;		/* Old printer attributes */
  ipp_attribute_t *attr;		/* Attribute data */
  cups_option_t	*option;		/* Current printer option */
  char		*filter;		/* Current filter */
  static const char * const air_none[] =
		{			/* No authentication */
		  "none"
		};
  static const char * const air_userpass[] =
		{			/* Basic/Digest authentication */
		  "username",
		  "password"
		};

//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  DEBUG_printf(("cupsdSetPrinterAttrs: entering name = %s, type = %x\n", p->name,
                p->type));

 /*
  * Make sure that we have the common attributes defined...
  */
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  if (!CommonData)
    cupsdCreateCommonData();
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
 /*
  * Clear out old filters, if any...
  */

  delete_printer_filters(p);

 /*
  * Figure out the authentication that is required for the printer.
  */

  auth_supported = "requesting-user-name";
  num_air        = 1;
  air            = air_none;

  if (p->num_auth_info_required > 0 && strcmp(p->auth_info_required[0], "none"))
  {
    num_air = p->num_auth_info_required;
    air     = p->auth_info_required;
  }
  else if ((p->type & CUPS_PRINTER_AUTHENTICATED) &&
           (p->type & CUPS_PRINTER_DISCOVERED))
  {
    num_air = 2;
    air     = air_userpass;
  }

  if (p->type & CUPS_PRINTER_CLASS)
    snprintf(resource, sizeof(resource), "/classes/%s", p->name);
  else
    snprintf(resource, sizeof(resource), "/printers/%s", p->name);
#ifdef NOT_CUT
  if ((auth = cupsdFindBest(resource, HTTP_POST)) == NULL ||
      auth->type == CUPSD_AUTH_NONE)
    auth = cupsdFindPolicyOp(p->op_policy_ptr, IPP_PRINT_JOB);
#endif
  if (auth)
  {
    int	auth_type;		/* Authentication type */


    if ((auth_type = auth->type) == CUPSD_AUTH_DEFAULT)
      auth_type = DefaultAuthType;

    if (auth_type == CUPSD_AUTH_BASIC || auth_type == CUPSD_AUTH_BASICDIGEST)
      auth_supported = "basic";
    else if (auth_type == CUPSD_AUTH_DIGEST)
      auth_supported = "digest";
#ifdef HAVE_GSSAPI
    else if (auth_type == CUPSD_AUTH_NEGOTIATE)
      auth_supported = "negotiate";
#endif /* HAVE_GSSAPI */

    if (!(p->type & CUPS_PRINTER_DISCOVERED))
    {
      if (auth_type != CUPSD_AUTH_NONE)
	p->type |= CUPS_PRINTER_AUTHENTICATED;
      else
	p->type &= ~CUPS_PRINTER_AUTHENTICATED;
    }
  }
  else if (!(p->type & CUPS_PRINTER_DISCOVERED))
    p->type &= ~CUPS_PRINTER_AUTHENTICATED;

 /*
  * Create the required IPP attributes for a printer...
  */

  oldattrs = p->attrs;
  p->attrs = ippNew();

  ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
               "uri-authentication-supported", NULL, auth_supported);
  ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
               "uri-security-supported", NULL, "none");
  ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME, "printer-name", NULL,
               p->name);
  ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT, "printer-location",
               NULL, p->location ? p->location : "");
  ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT, "printer-info",
               NULL, p->info ? p->info : "");
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  if (p->num_users)
  {
    if (p->deny_users)
      ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
                    "requesting-user-name-denied", p->num_users, NULL,
		    p->users);
    else
      ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
                    "requesting-user-name-allowed", p->num_users, NULL,
		    p->users);
  }

  ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "job-quota-period", p->quota_period);
  ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "job-k-limit", p->k_limit);
  ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                "job-page-limit", p->page_limit);
  ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		"auth-info-required", num_air, NULL, air);

  if (cupsArrayCount(Banners) > 0 && !(p->type & CUPS_PRINTER_DISCOVERED))
  {
   /*
    * Setup the job-sheets-default attribute...
    */

    attr = ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
                	 "job-sheets-default", 2, NULL, NULL);

    if (attr != NULL)
    {
      attr->values[0].string.text = _cupsStrAlloc(Classification ?
	                                   Classification : p->job_sheets[0]);
      attr->values[1].string.text = _cupsStrAlloc(Classification ?
	                                   Classification : p->job_sheets[1]);
    }
  }

  p->raw    = 0;
  p->remote = 0;

  if (p->type & CUPS_PRINTER_DISCOVERED)
  {
   /*
    * Tell the client this is a remote printer of some type...
    */

    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_URI,
	         "printer-uri-supported", NULL, p->uri);
   printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_URI, "printer-more-info",
		 NULL, p->uri);

    if (p->make_model)
      ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
                   "printer-make-and-model", NULL, p->make_model);

    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_URI, "device-uri", NULL,
        	 p->uri);

    p->raw    = 1;
    p->remote = 1;
  }
  else
  {
   /*
    * Assign additional attributes depending on whether this is a printer
    * or class...
    */

    if (p->type & (CUPS_PRINTER_CLASS | CUPS_PRINTER_IMPLICIT))
    {
      p->raw = 1;
      p->type &= ~CUPS_PRINTER_OPTIONS;

     /*
      * Add class-specific attributes...
      */

      if ((p->type & CUPS_PRINTER_IMPLICIT) && p->num_printers > 0 &&
          p->printers[0]->make_model)
	ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
                     "printer-make-and-model", NULL, p->printers[0]->make_model);
      else
	ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
                     "printer-make-and-model", NULL, "Local Printer Class");

      ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_URI, "device-uri", NULL,
        	   "file:///dev/null");

      if (p->num_printers > 0)
      {
       /*
	* Add a list of member names; URIs are added in copy_printer_attrs...
	*/

	attr    = ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
                                "member-names", p->num_printers, NULL, NULL);
        p->type |= CUPS_PRINTER_OPTIONS;
	for (i = 0; i < p->num_printers; i ++)
	{
          if (attr != NULL)
            attr->values[i].string.text = _cupsStrRetain(p->printers[i]->name);

	  p->type &= ~CUPS_PRINTER_OPTIONS | p->printers[i]->type;
        }
      }
    }
    else
    {
     /*
      * Add printer-specific attributes...
      */

      ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_URI, "device-uri", NULL,
		   p->sanitized_device_uri);

     /*
      * Assign additional attributes from the PPD file (if any)...
      */

      load_ppd(p);

     /*
      * Add filters for printer...
      */
#ifdef NOT_CUT
      cupsdSetPrinterReasons(p, "-cups-missing-filter-warning,"
                                "cups-insecure-filter-warning");
#endif
      for (filter = (char *)cupsArrayFirst(p->filters);
	   filter;
	   filter = (char *)cupsArrayNext(p->filters))
	add_printer_filter(p, p->filetype, filter);

      if (p->pre_filters)
      {
        p->prefiltertype = mimeAddType(MimeDatabase, "prefilter", p->name);

        for (filter = (char *)cupsArrayFirst(p->pre_filters);
	     filter;
	     filter = (char *)cupsArrayNext(p->pre_filters))
	  add_printer_filter(p, p->prefiltertype, filter);
      }
    }
  }

 /*
  * Copy marker attributes as needed...
  */

  if (oldattrs)
  {
    ipp_attribute_t *oldattr;		/* Old attribute */


    if ((oldattr = ippFindAttribute(oldattrs, "marker-colors",
                                    IPP_TAG_NAME)) != NULL)
    {
      if ((attr = ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
                                "marker-colors", oldattr->num_values, NULL,
				NULL)) != NULL)
      {
	for (i = 0; i < oldattr->num_values; i ++)
	  attr->values[i].string.text =
	      _cupsStrRetain(oldattr->values[i].string.text);
      }
    }

    if ((oldattr = ippFindAttribute(oldattrs, "marker-levels",
                                    IPP_TAG_INTEGER)) != NULL)
    {
      if ((attr = ippAddIntegers(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                                 "marker-levels", oldattr->num_values,
				 NULL)) != NULL)
      {
	for (i = 0; i < oldattr->num_values; i ++)
	  attr->values[i].integer = oldattr->values[i].integer;
      }
    }

    if ((oldattr = ippFindAttribute(oldattrs, "marker-message",
                                    IPP_TAG_TEXT)) != NULL)
      ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT, "marker-message",
                   NULL, oldattr->values[0].string.text);

    if ((oldattr = ippFindAttribute(oldattrs, "marker-low-levels",
                                    IPP_TAG_INTEGER)) != NULL)
    {
      if ((attr = ippAddIntegers(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                                 "marker-low-levels", oldattr->num_values,
				 NULL)) != NULL)
      {
	for (i = 0; i < oldattr->num_values; i ++)
	  attr->values[i].integer = oldattr->values[i].integer;
      }
    }

    if ((oldattr = ippFindAttribute(oldattrs, "marker-high-levels",
                                    IPP_TAG_INTEGER)) != NULL)
    {
      if ((attr = ippAddIntegers(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                                 "marker-high-levels", oldattr->num_values,
				 NULL)) != NULL)
      {
	for (i = 0; i < oldattr->num_values; i ++)
	  attr->values[i].integer = oldattr->values[i].integer;
      }
    }

    if ((oldattr = ippFindAttribute(oldattrs, "marker-names",
                                    IPP_TAG_NAME)) != NULL)
    {
      if ((attr = ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
                                "marker-names", oldattr->num_values, NULL,
				NULL)) != NULL)
      {
	for (i = 0; i < oldattr->num_values; i ++)
	  attr->values[i].string.text =
	      _cupsStrRetain(oldattr->values[i].string.text);
      }
    }

    if ((oldattr = ippFindAttribute(oldattrs, "marker-types",
                                    IPP_TAG_KEYWORD)) != NULL)
    {
      if ((attr = ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
                                "marker-types", oldattr->num_values, NULL,
				NULL)) != NULL)
      {
	for (i = 0; i < oldattr->num_values; i ++)
	  attr->values[i].string.text =
	      _cupsStrRetain(oldattr->values[i].string.text);
      }
    }

    ippDelete(oldattrs);
  }

 /*
  * Force sharing off for remote queues...
  */
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  if (p->type & (CUPS_PRINTER_REMOTE | CUPS_PRINTER_IMPLICIT))
    p->shared = 0;
  else
  {
   /*
    * Copy the printer options into a browse attributes string we can re-use.
    */

    const char	*valptr;		/* Pointer into value */
    char	*attrptr;		/* Pointer into attribute string */


   /*
    * Free the old browse attributes as needed...
    */

    if (p->browse_attrs)
      free(p->browse_attrs);

   /*
    * Compute the length of all attributes + job-sheets, lease-duration,
    * and BrowseLocalOptions.
    */

    for (length = 1, i = p->num_options, option = p->options;
         i > 0;
	 i --, option ++)
    {
      length += strlen(option->name) + 2;

      if (option->value)
      {
        for (valptr = option->value; *valptr; valptr ++)
	  if (strchr(" \"\'\\", *valptr))
	    length += 2;
	  else
	    length ++;
      }
    }

    length += 13 + strlen(p->job_sheets[0]) + strlen(p->job_sheets[1]);
    length += 32;
    if (BrowseLocalOptions)
      length += 12 + strlen(BrowseLocalOptions);

   /*
    * Allocate the new string...
    */

    if ((p->browse_attrs = calloc(1, length)) == NULL)
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to allocate %d bytes for browse data!",
		      length);
    else
    {
     /*
      * Got the allocated string, now copy the options and attributes over...
      */

      sprintf(p->browse_attrs, "job-sheets=%s,%s lease-duration=%d",
              p->job_sheets[0], p->job_sheets[1], BrowseTimeout);
      attrptr = p->browse_attrs + strlen(p->browse_attrs);

      if (BrowseLocalOptions)
      {
        sprintf(attrptr, " ipp-options=%s", BrowseLocalOptions);
        attrptr += strlen(attrptr);
      }

      for (i = p->num_options, option = p->options;
           i > 0;
	   i --, option ++)
      {
        *attrptr++ = ' ';
	strcpy(attrptr, option->name);
	attrptr += strlen(attrptr);

	if (option->value)
	{
	  *attrptr++ = '=';

          for (valptr = option->value; *valptr; valptr ++)
	  {
	    if (strchr(" \"\'\\", *valptr))
	      *attrptr++ = '\\';

	    *attrptr++ = *valptr;
	  }
	}
      }
    }
  }

 /*
  * Populate the document-format-supported attribute...
  */

  add_printer_formats(p);

  DEBUG_printf(("cupsdSetPrinterAttrs: leaving name = %s, type = %x\n", p->name,
                p->type));

 /*
  * Add name-default attributes...
  */

  add_printer_defaults(p);

#ifdef __sgi
 /*
  * Write the IRIX printer config and status files...
  */

  write_irix_config(p);
  write_irix_state(p);
#endif /* __sgi */

 /*
  * Let the browse protocols reflect the change
  */
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  cupsdRegisterPrinter(p);
  //printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
}


/*
 * 'cupsdSetPrinterReasons()' - Set/update the reasons strings.
 */
#ifdef NOT_CUT
int					/* O - 1 if something changed, 0 otherwise */
cupsdSetPrinterReasons(
    cupsd_printer_t  *p,		/* I - Printer */
    const char *s)			/* I - Reasons strings */
{
  int		i,			/* Looping var */
		changed = 0;		/* Did something change? */
  const char	*sptr;			/* Pointer into reasons */
  char		reason[255],		/* Reason string */
		*rptr;			/* Pointer into reason */


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
		  "cupsdSetPrinterReasons(p=%p(%s),s=\"%s\"", p, p->name, s);

  if (s[0] == '-' || s[0] == '+')
  {
   /*
    * Add/remove reasons...
    */

    sptr = s + 1;
  }
  else
  {
   /*
    * Replace reasons...
    */

    sptr = s;

    for (i = 0; i < p->num_reasons; i ++)
      _cupsStrFree(p->reasons[i]);

    p->num_reasons = 0;
    changed        = 1;

    cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

    if (PrintcapFormat == PRINTCAP_PLIST)
      cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);
  }

  if (!strcmp(s, "none"))
    return (changed);

 /*
  * Loop through all of the reasons...
  */

  while (*sptr)
  {
   /*
    * Skip leading whitespace and commas...
    */

    while (isspace(*sptr & 255) || *sptr == ',')
      sptr ++;

    for (rptr = reason; *sptr && !isspace(*sptr & 255) && *sptr != ','; sptr ++)
      if (rptr < (reason + sizeof(reason) - 1))
        *rptr++ = *sptr;

    if (rptr == reason)
      break;

    *rptr = '\0';

    if (s[0] == '-')
    {
     /*
      * Remove reason...
      */

      for (i = 0; i < p->num_reasons; i ++)
        if (!strcmp(reason, p->reasons[i]))
	{
	 /*
	  * Found a match, so remove it...
	  */

	  p->num_reasons --;
          changed = 1;
	  _cupsStrFree(p->reasons[i]);

	  if (i < p->num_reasons)
	    memmove(p->reasons + i, p->reasons + i + 1,
	            (p->num_reasons - i) * sizeof(char *));

          if (!strcmp(reason, "paused") && p->state == IPP_PRINTER_STOPPED)
	    cupsdSetPrinterState(p, IPP_PRINTER_IDLE, 1);

          if (strcmp(reason, "connecting-to-device"))
	  {
	    cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

	    if (PrintcapFormat == PRINTCAP_PLIST)
	      cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);
          }
	  break;
	}
    }
    else if (p->num_reasons < (int)(sizeof(p->reasons) / sizeof(p->reasons[0])))
    {
     /*
      * Add reason...
      */

      for (i = 0; i < p->num_reasons; i ++)
        if (!strcmp(reason, p->reasons[i]))
	  break;

      if (i >= p->num_reasons)
      {
        if (i >= (int)(sizeof(p->reasons) / sizeof(p->reasons[0])))
	{
	  cupsdLogMessage(CUPSD_LOG_ALERT,
	                  "Too many printer-state-reasons values for %s (%d)",
			  p->name, i + 1);
          return (changed);
        }

        p->reasons[i] = _cupsStrAlloc(reason);
	p->num_reasons ++;
        changed = 1;

	if (!strcmp(reason, "paused") && p->state != IPP_PRINTER_STOPPED)
	  cupsdSetPrinterState(p, IPP_PRINTER_STOPPED, 1);

	if (strcmp(reason, "connecting-to-device"))
	{
	  cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

	  if (PrintcapFormat == PRINTCAP_PLIST)
	    cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);
	}
      }
    }
  }

  return (changed);
}
#endif

/*
 * 'cupsdSetPrinterState()' - Update the current state of a printer.
 */

void
cupsdSetPrinterState(
    cupsd_printer_t *p,			/* I - Printer to change */
    ipp_pstate_t    s,			/* I - New state */
    int             update)		/* I - Update printers.conf? */
{
  ipp_pstate_t	old_state;		/* Old printer state */
  static const char * const printer_states[] =
  {					/* State strings */
    "idle",
    "processing",
    "stopped"
  };


 /*
  * Can't set status of remote printers...
  */

  if (p->type & CUPS_PRINTER_DISCOVERED)
    return;

 /*
  * Set the new state...
  */

  if (PrintcapFormat == PRINTCAP_PLIST)
    cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);

  old_state = p->state;
  p->state  = s;

  if (old_state != s)
  {
    cupsdAddEvent(s == IPP_PRINTER_STOPPED ? CUPSD_EVENT_PRINTER_STOPPED :
                      CUPSD_EVENT_PRINTER_STATE, p, NULL,
		  "%s \"%s\" state changed to %s.",
		  (p->type & CUPS_PRINTER_CLASS) ? "Class" : "Printer",
		  p->name, printer_states[p->state]);

   /*
    * Let the browse code know this needs to be updated...
    */

    BrowseNext     = p;
    p->state_time  = time(NULL);
    p->browse_time = 0;

#ifdef __sgi
    write_irix_state(p);
#endif /* __sgi */
  }

 /*
  * Set/clear the paused reason as needed...
  */
#ifdef NOT_CUT

  if (s == IPP_PRINTER_STOPPED)
    cupsdSetPrinterReasons(p, "+paused");
  else
    cupsdSetPrinterReasons(p, "-paused");
#endif
 /*
  * Clear the message for the queue when going to processing...
  */

  if (s == IPP_PRINTER_PROCESSING)
    p->state_message[0] = '\0';

 /*
  * Update the printer history...
  */
#ifdef NOT_CUT

  cupsdAddPrinterHistory(p);
#endif
 /*
  * Let the browse protocols reflect the change...
  */

  if (update)
    cupsdRegisterPrinter(p);

 /*
  * Save the printer configuration if a printer goes from idle or processing
  * to stopped (or visa-versa)...
  */

  if (update &&
      (old_state == IPP_PRINTER_STOPPED) != (s == IPP_PRINTER_STOPPED))
  {
    if (p->type & CUPS_PRINTER_CLASS)
      cupsdMarkDirty(CUPSD_DIRTY_CLASSES);
    else
      cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);
  }
}


/*
 * 'cupsdStopPrinter()' - Stop a printer from printing any jobs...
 */

void
cupsdStopPrinter(cupsd_printer_t *p,	/* I - Printer to stop */
                 int             update)/* I - Update printers.conf? */
{
 /*
  * Set the printer state...
  */

  cupsdSetPrinterState(p, IPP_PRINTER_STOPPED, update);

 /*
  * See if we have a job printing on this printer...
  */

  if (p->job && p->job->state_value == IPP_JOB_PROCESSING)
    cupsdSetJobState(p->job, IPP_JOB_PENDING, CUPSD_JOB_DEFAULT,
                     "Job stopped due to printer being paused.");
}


/*
 * 'cupsdUpdatePrinterPPD()' - Update keywords in a printer's PPD file.
 */
#ifdef NOT_CUT
int					/* O - 1 if successful, 0 otherwise */
cupsdUpdatePrinterPPD(
    cupsd_printer_t *p,			/* I - Printer */
    int             num_keywords,	/* I - Number of keywords */
    cups_option_t   *keywords)		/* I - Keywords */
{
  int		i;			/* Looping var */
  cups_file_t	*src,			/* Original file */
		*dst;			/* New file */
  char		srcfile[1024],		/* Original filename */
		dstfile[1024],		/* New filename */
		line[1024],		/* Line from file */
		keystring[41];		/* Keyword from line */
  cups_option_t	*keyword;		/* Current keyword */


  cupsdLogMessage(CUPSD_LOG_INFO, "Updating keywords in PPD file for %s...",
                  p->name);

 /*
  * Get the old and new PPD filenames...
  */

  snprintf(srcfile, sizeof(srcfile), "%s/ppd/%s.ppd.O", ServerRoot, p->name);
  snprintf(dstfile, sizeof(srcfile), "%s/ppd/%s.ppd", ServerRoot, p->name);

 /*
  * Rename the old file and open the old and new...
  */

  if (rename(dstfile, srcfile))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to backup PPD file for %s: %s",
                    p->name, strerror(errno));
    return (0);
  }

  if ((src = cupsFileOpen(srcfile, "r")) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to open PPD file \"%s\": %s",
                    srcfile, strerror(errno));
    rename(srcfile, dstfile);
    return (0);
  }

  if ((dst = cupsFileOpen(dstfile, "w")) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to create PPD file \"%s\": %s",
                    dstfile, strerror(errno));
    cupsFileClose(src);
    rename(srcfile, dstfile);
    return (0);
  }

 /*
  * Copy the first line and then write out all of the keywords...
  */

  if (!cupsFileGets(src, line, sizeof(line)))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to read PPD file \"%s\": %s",
                    srcfile, strerror(errno));
    cupsFileClose(src);
    cupsFileClose(dst);
    rename(srcfile, dstfile);
    return (0);
  }

  cupsFilePrintf(dst, "%s\n", line);

  for (i = num_keywords, keyword = keywords; i > 0; i --, keyword ++)
  {
    cupsdLogMessage(CUPSD_LOG_DEBUG, "*%s: %s", keyword->name, keyword->value);
    cupsFilePrintf(dst, "*%s: %s\n", keyword->name, keyword->value);
  }

 /*
  * Then copy the rest of the PPD file, dropping any keywords we changed.
  */

  while (cupsFileGets(src, line, sizeof(line)))
  {
   /*
    * Skip keywords we've already set...
    */

    if (sscanf(line, "*%40[^:]:", keystring) == 1 &&
        cupsGetOption(keystring, num_keywords, keywords))
      continue;

   /*
    * Otherwise write the line...
    */

    cupsFilePrintf(dst, "%s\n", line);
  }

 /*
  * Close files and return...
  */

  cupsFileClose(src);
  cupsFileClose(dst);

  return (1);
}
#endif
/*
 * 'cupsdUpdatePrinters()' - Update printers after a partial reload.
 */

void
cupsdUpdatePrinters(void)
{
  cupsd_printer_t	*p;		/* Current printer */


 /*
  * Loop through the printers and recreate the printer attributes
  * for any local printers since the policy and/or access control
  * stuff may have changed.  Also, if browsing is disabled, remove
  * any remote printers...
  */

  for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
       p;
       p = (cupsd_printer_t *)cupsArrayNext(Printers))
  {
   /*
    * Remove remote printers if we are no longer browsing...
    */

    if (!Browsing &&
        (p->type & (CUPS_PRINTER_IMPLICIT | CUPS_PRINTER_DISCOVERED)))
    {
      if (p->type & CUPS_PRINTER_IMPLICIT)
        cupsArrayRemove(ImplicitPrinters, p);

      cupsArraySave(Printers);
      cupsdDeletePrinter(p, 0);
      cupsArrayRestore(Printers);
      continue;
    }

   /*
    * Update the operation policy pointer...
    */
#ifdef NOT_CUT

    if ((p->op_policy_ptr = cupsdFindPolicy(p->op_policy)) == NULL)
      p->op_policy_ptr = DefaultPolicyPtr;
#endif
   /*
    * Update printer attributes as needed...
    */

    if (!(p->type & CUPS_PRINTER_DISCOVERED))
      cupsdSetPrinterAttrs(p);
  }
}


/*
 * 'cupsdValidateDest()' - Validate a printer/class destination.
 */

const char *				/* O - Printer or class name */
cupsdValidateDest(
    const char      *uri,		/* I - Printer URI */
    cups_ptype_t    *dtype,		/* O - Type (printer or class) */
    cupsd_printer_t **printer)		/* O - Printer pointer */
{
  cupsd_printer_t	*p;		/* Current printer */
  char			localname[1024],/* Localized hostname */
			*lptr,		/* Pointer into localized hostname */
			*sptr,		/* Pointer into server name */
			*rptr,		/* Pointer into resource */
			scheme[32],	/* Scheme portion of URI */
			username[64],	/* Username portion of URI */
			hostname[HTTP_MAX_HOST],
					/* Host portion of URI */
			resource[HTTP_MAX_URI];
					/* Resource portion of URI */
  int			port;		/* Port portion of URI */


  DEBUG_printf(("cupsdValidateDest(uri=\"%s\", dtype=%p, printer=%p)\n", uri,
                dtype, printer));

 /*
  * Initialize return values...
  */

  if (printer)
    *printer = NULL;
 

  if (dtype)
    *dtype = (cups_ptype_t)0;

 /*
  * Pull the hostname and resource from the URI...
  */
//printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
  httpSeparateURI(HTTP_URI_CODING_ALL, uri, scheme, sizeof(scheme),
                  username, sizeof(username), hostname, sizeof(hostname),
		  &port, resource, sizeof(resource));
//printf("\n==file:%s,line:%d,func:%s=resource=%s=\n",__FILE__,__LINE__,__FUNCTION__,resource);
 /*
  * See if the resource is a class or printer...
  */

  if (!strncmp(resource, "/classes/", 9))
  {
   /*
    * Class...
    */
        //printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    rptr = resource + 9;
  }
  else if (!strncmp(resource, "/printers/", 10))
  {
   /*
    * Printer...
    */
 
    rptr = resource + 10;
   // printf("\n==file:%s,line:%d,func:%s=rptr=%s=\n",__FILE__,__LINE__,__FUNCTION__,rptr);
  }
  else
  {
   /*
    * Bad resource name...
    */
    printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    return (NULL);
  }

 /*
  * See if the printer or class name exists...
  */
//printf("\n==file:%s,line:%d,func:%s=rptr=%s=\n",__FILE__,__LINE__,__FUNCTION__,rptr);
 // p = cupsdFindDest(rptr);
 p = (cupsd_printer_t *)cupsArrayFirst(Printers);

  if (p == NULL && strchr(rptr, '@') == NULL)
    {
    //printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    return (NULL);
    }
  else if (p != NULL)
  {
    if (printer)
      *printer = p;
    //printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    if (dtype)
      *dtype = p->type & (CUPS_PRINTER_CLASS | CUPS_PRINTER_IMPLICIT |
                          CUPS_PRINTER_REMOTE | CUPS_PRINTER_DISCOVERED);
//printf("\n==file:%s,line:%d,func:%s=p->name=%s=\n",__FILE__,__LINE__,__FUNCTION__,p->name);
    return (p->name);
  }

 /*
  * Change localhost to the server name...
  */
//printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
  if (!strcasecmp(hostname, "localhost"))
    strlcpy(hostname, ServerName, sizeof(hostname));

  strlcpy(localname, hostname, sizeof(localname));

  if (!strcasecmp(hostname, ServerName))
  {
   /*
    * Localize the hostname...
    */
    //printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    lptr = strchr(localname, '.');
    sptr = strchr(ServerName, '.');

    if (sptr != NULL && lptr != NULL)
    {
     /*
      * Strip the common domain name components...
      */
        //printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
      while (lptr != NULL)
      {
	if (!strcasecmp(lptr, sptr))
	{
          *lptr = '\0';
	  break;
	}
	else
          lptr = strchr(lptr + 1, '.');
      }
    }
  }

  DEBUG_printf(("localized hostname is \"%s\"...\n", localname));
printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
//printf("localized hostname is \"%s\"...\n", localname);
 /*
  * Find a matching printer or class...
  */

  for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
       p;
       p = (cupsd_printer_t *)cupsArrayNext(Printers))
    if (!strcasecmp(p->hostname, localname) &&
        !strcasecmp(p->name, rptr))
    {
      if (printer)
        *printer = p;

      if (dtype)
	*dtype = p->type & (CUPS_PRINTER_CLASS | CUPS_PRINTER_IMPLICIT |
                            CUPS_PRINTER_REMOTE | CUPS_PRINTER_DISCOVERED);
     printf("\n==file:%s,line:%d,func:%s=p->name=%s=\n",__FILE__,__LINE__,__FUNCTION__,p->name);
      return (p->name);
    }
//printf("\n==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
  return (NULL);
}


/*
 * 'cupsdWritePrintcap()' - Write a pseudo-printcap file for older applications
 *                          that need it...
 */

void
cupsdWritePrintcap(void)
{
  int			i;		/* Looping var */
  cups_file_t		*fp;		/* Printcap file */
  cupsd_printer_t	*p;		/* Current printer */


#ifdef __sgi
 /*
  * Update the IRIX printer state for the default printer; if
  * no printers remain, then the default printer file will be
  * removed...
  */

  write_irix_state(DefaultPrinter);
#endif /* __sgi */

 /*
  * See if we have a printcap file; if not, don't bother writing it.
  */

  if (!Printcap || !*Printcap)
    return;

  cupsdLogMessage(CUPSD_LOG_INFO, "Generating printcap %s...", Printcap);

 /*
  * Open the printcap file...
  */

  if ((fp = cupsFileOpen(Printcap, "w")) == NULL)
    return;

 /*
  * Put a comment header at the top so that users will know where the
  * data has come from...
  */

  if (PrintcapFormat != PRINTCAP_PLIST)
    cupsFilePrintf(fp, "# This file was automatically generated by cupsd(8) "
                       "from the\n"
                       "# %s/printers.conf file.  All changes to this file\n"
		       "# will be lost.\n", ServerRoot);

 /*
  * Write a new printcap with the current list of printers.
  */

  switch (PrintcapFormat)
  {
    case PRINTCAP_BSD :
       /*
	* Each printer is put in the file as:
	*
	*    Printer1:
	*    Printer2:
	*    Printer3:
	*    ...
	*    PrinterN:
	*/

	if (DefaultPrinter)
	  cupsFilePrintf(fp, "%s|%s:rm=%s:rp=%s:\n", DefaultPrinter->name,
			 DefaultPrinter->info, ServerName,
			 DefaultPrinter->name);

	for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
	     p;
	     p = (cupsd_printer_t *)cupsArrayNext(Printers))
	  if (p != DefaultPrinter)
	    cupsFilePrintf(fp, "%s|%s:rm=%s:rp=%s:\n", p->name, p->info,
			   ServerName, p->name);
	break;

    case PRINTCAP_PLIST :
       /*
	* Each printer is written as a dictionary in a plist file.
	* Currently the printer-name, printer-info, printer-is-accepting-jobs,
	* printer-location, printer-make-and-model, printer-state,
	* printer-state-reasons, printer-type, and (sanitized) device-uri.
	*/

	cupsFilePuts(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			 "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD "
			 "PLIST 1.0//EN\" \"http://www.apple.com/DTDs/"
			 "PropertyList-1.0.dtd\">\n"
			 "<plist version=\"1.0\">\n"
			 "<array>\n");

	for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
	     p;
	     p = (cupsd_printer_t *)cupsArrayNext(Printers))
	{
	  cupsFilePuts(fp, "\t<dict>\n"
			   "\t\t<key>printer-name</key>\n"
			   "\t\t<string>");
	  write_xml_string(fp, p->name);
	  cupsFilePuts(fp, "</string>\n"
			   "\t\t<key>printer-info</key>\n"
			   "\t\t<string>");
	  write_xml_string(fp, p->info);
	  cupsFilePrintf(fp, "</string>\n"
			     "\t\t<key>printer-is-accepting-jobs</key>\n"
			     "\t\t<%s/>\n"
			     "\t\t<key>printer-location</key>\n"
			     "\t\t<string>", p->accepting ? "true" : "false");
	  write_xml_string(fp, p->location);
	  cupsFilePuts(fp, "</string>\n"
			   "\t\t<key>printer-make-and-model</key>\n"
			   "\t\t<string>");
	  write_xml_string(fp, p->make_model);
	  cupsFilePrintf(fp, "</string>\n"
			     "\t\t<key>printer-state</key>\n"
			     "\t\t<integer>%d</integer>\n"
			     "\t\t<key>printer-state-reasons</key>\n"
			     "\t\t<array>\n", p->state);
	  for (i = 0; i < p->num_reasons; i ++)
	  {
	    cupsFilePuts(fp, "\t\t\t<string>");
	    write_xml_string(fp, p->reasons[i]);
	    cupsFilePuts(fp, "</string>\n");
	  }
	  cupsFilePrintf(fp, "\t\t</array>\n"
			     "\t\t<key>printer-type</key>\n"
			     "\t\t<integer>%d</integer>\n"
			     "\t\t<key>device-uri</key>\n"
			     "\t\t<string>", p->type);
	  write_xml_string(fp, p->sanitized_device_uri);
	  cupsFilePuts(fp, "</string>\n"
			   "\t</dict>\n");
	}
	cupsFilePuts(fp, "</array>\n"
			 "</plist>\n");
	break;

    case PRINTCAP_SOLARIS :
       /*
	* Each printer is put in the file as:
	*
	*    _all:all=Printer1,Printer2,Printer3,...,PrinterN
	*    _default:use=DefaultPrinter
	*    Printer1:\
	*            :bsdaddr=ServerName,Printer1:\
	*            :description=Description:
	*    Printer2:
	*            :bsdaddr=ServerName,Printer2:\
	*            :description=Description:
	*    Printer3:
	*            :bsdaddr=ServerName,Printer3:\
	*            :description=Description:
	*    ...
	*    PrinterN:
	*            :bsdaddr=ServerName,PrinterN:\
	*            :description=Description:
	*/

	cupsFilePuts(fp, "_all:all=");
	for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
	     p;
	     p = (cupsd_printer_t *)cupsArrayCurrent(Printers))
	  cupsFilePrintf(fp, "%s%c", p->name,
			 cupsArrayNext(Printers) ? ',' : '\n');

	if (DefaultPrinter)
	  cupsFilePrintf(fp, "_default:use=%s\n", DefaultPrinter->name);

	for (p = (cupsd_printer_t *)cupsArrayFirst(Printers);
	     p;
	     p = (cupsd_printer_t *)cupsArrayNext(Printers))
	  cupsFilePrintf(fp, "%s:\\\n"
			     "\t:bsdaddr=%s,%s:\\\n"
			     "\t:description=%s:\n",
			 p->name, ServerName, p->name,
			 p->info ? p->info : "");
	break;
  }

 /*
  * Close the file...
  */

  cupsFileClose(fp);
}


/*
 * 'add_printer_defaults()' - Add name-default attributes to the printer attributes.
 */

static void
add_printer_defaults(cupsd_printer_t *p)/* I - Printer */
{
  int		i;			/* Looping var */
  int		num_options;		/* Number of default options */
  cups_option_t	*options,		/* Default options */
		*option;		/* Current option */
  char		name[256];		/* name-default */


 /*
  * Maintain a common array of default attribute names...
  */

  if (!CommonDefaults)
  {
    CommonDefaults = cupsArrayNew((cups_array_func_t)strcmp, NULL);

    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("copies-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("document-format-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("finishings-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("job-hold-until-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("job-priority-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("job-sheets-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("media-col-default"));
    cupsArrayAdd(CommonDefaults, _cupsStrAlloc("number-up-default"));
    cupsArrayAdd(CommonDefaults,
                 _cupsStrAlloc("orientation-requested-default"));
  }

 /*
  * Add all of the default options from the .conf files...
  */

  for (num_options = 0, options = NULL, i = p->num_options, option = p->options;
       i > 0;
       i --, option ++)
  {
    if (strcmp(option->name, "ipp-options") &&
	strcmp(option->name, "job-sheets") &&
        strcmp(option->name, "lease-duration"))
    {
      snprintf(name, sizeof(name), "%s-default", option->name);
      num_options = cupsAddOption(name, option->value, num_options, &options);

      if (!cupsArrayFind(CommonDefaults, name))
        cupsArrayAdd(CommonDefaults, _cupsStrAlloc(name));
    }
  }

 /*
  * Convert options to IPP attributes...
  */

  cupsEncodeOptions2(p->attrs, num_options, options, IPP_TAG_PRINTER);
  cupsFreeOptions(num_options, options);

 /*
  * Add standard -default attributes as needed...
  */

  if (!cupsGetOption("copies", p->num_options, p->options))
    ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER, "copies-default",
                  1);

  if (!cupsGetOption("document-format", p->num_options, p->options))
    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_MIMETYPE,
        	 "document-format-default", NULL, "application/octet-stream");

  if (!cupsGetOption("job-hold-until", p->num_options, p->options))
    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
                 "job-hold-until-default", NULL, "no-hold");

  if (!cupsGetOption("job-priority", p->num_options, p->options))
    ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                  "job-priority-default", 50);

  if (!cupsGetOption("number-up", p->num_options, p->options))
    ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
                  "number-up-default", 1);

  if (!cupsGetOption("notify-lease-duration", p->num_options, p->options))
    ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
        	  "notify-lease-duration-default", DefaultLeaseDuration);

  if (!cupsGetOption("notify-events", p->num_options, p->options))
    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
        	 "notify-events-default", NULL, "job-completed");

  if (!cupsGetOption("orientation-requested", p->num_options, p->options))
    ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_NOVALUE,
                 "orientation-requested-default", NULL, NULL);

  if (!cupsGetOption("print-quality", p->num_options, p->options))
    ippAddInteger(p->attrs, IPP_TAG_PRINTER, IPP_TAG_ENUM,
                  "print-quality-default", IPP_QUALITY_NORMAL);
}


/*
 * 'add_printer_filter()' - Add a MIME filter for a printer.
 */

static void
add_printer_filter(
    cupsd_printer_t  *p,		/* I - Printer to add to */
    mime_type_t	     *filtertype,	/* I - Filter or prefilter MIME type */
    const char       *filter)		/* I - Filter to add */
{
  char		super[MIME_MAX_SUPER],	/* Super-type for filter */
		type[MIME_MAX_TYPE],	/* Type for filter */
		program[1024];		/* Program/filter name */
  int		cost;			/* Cost of filter */
  mime_type_t	*temptype;		/* MIME type looping var */
  char		filename[1024],		/* Full filter filename */
		*dirsep;		/* Pointer to directory separator */
  struct stat	fileinfo;		/* File information */


 /*
  * Parse the filter string; it should be in the following format:
  *
  *     super/type cost program
  */

  if (sscanf(filter, "%15[^/]/%31s%d%*[ \t]%1023[^\n]", super, type, &cost,
             program) != 4)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "%s: invalid filter string \"%s\"!",
                    p->name, filter);
    return;
  }

 /*
  * See if the filter program exists; if not, stop the printer and flag
  * the error!
  */

  if (strcmp(program, "-"))
  {
    if (program[0] == '/')
      strlcpy(filename, program, sizeof(filename));
    else
      snprintf(filename, sizeof(filename), "%s/filter/%s", ServerBin, program);

    if (stat(filename, &fileinfo))
    {
      memset(&fileinfo, 0, sizeof(fileinfo));

      snprintf(p->state_message, sizeof(p->state_message),
               "Filter \"%s\" for printer \"%s\" not available: %s",
	       filename, p->name, strerror(errno));
   #ifdef NOT_CUT   
      cupsdSetPrinterReasons(p, "+cups-missing-filter-warning");

      cupsdLogMessage(CUPSD_LOG_ERROR, "%s", p->state_message);
   #endif
    }

   /*
    * When running as root, do additional security checks...
    */

    if (!RunUser)
    {
     /*
      * Only use filters that are owned by root and do not have world write
      * permissions.
      */

      if (fileinfo.st_uid || (fileinfo.st_mode & (S_ISUID | S_IWOTH)) != 0)
      {
	if (fileinfo.st_uid)
	  snprintf(p->state_message, sizeof(p->state_message),
		   "Filter \"%s\" for printer \"%s\" not owned by root",
		   filename, p->name);
	else
	  snprintf(p->state_message, sizeof(p->state_message),
		   "Filter \"%s\" for printer \"%s\" has insecure permissions "
		   "(0%o)", filename, p->name, fileinfo.st_mode);
#ifdef NOT_CUT

	cupsdSetPrinterReasons(p, "+cups-insecure-filter-warning");

	cupsdLogMessage(CUPSD_LOG_ERROR, "%s", p->state_message);
#endif
      }
      else if (fileinfo.st_mode)
      {
       /*
	* Similarly, check that the parent directory is also owned by root and
	* does not have world write permissions.
	*/

	if ((dirsep = strrchr(filename, '/')) != NULL)
	  *dirsep = '\0';

	if (!stat(filename, &fileinfo) &&
	    (fileinfo.st_uid ||
	     (fileinfo.st_mode & (S_ISUID | S_IWOTH)) != 0))
	{
	  if (fileinfo.st_uid)
	    snprintf(p->state_message, sizeof(p->state_message),
		     "Filter directory \"%s\" for printer \"%s\" not owned by "
		     "root", filename, p->name);
	  else
	    snprintf(p->state_message, sizeof(p->state_message),
		     "Filter directory \"%s\" for printer \"%s\" has insecure "
		     "permissions (0%o)", filename, p->name, fileinfo.st_mode);
#ifdef NOT_CUT

	  cupsdSetPrinterReasons(p, "+cups-insecure-filter-warning");

	  cupsdLogMessage(CUPSD_LOG_ERROR, "%s", p->state_message);
#endif
	}
      }
    }
  }

 /*
  * Add the filter to the MIME database, supporting wildcards as needed...
  */

  for (temptype = mimeFirstType(MimeDatabase);
       temptype;
       temptype = mimeNextType(MimeDatabase))
    if (((super[0] == '*' && strcasecmp(temptype->super, "printer")) ||
         !strcasecmp(temptype->super, super)) &&
        (type[0] == '*' || !strcasecmp(temptype->type, type)))
    {
      cupsdLogMessage(CUPSD_LOG_DEBUG2,
                      "add_printer_filter: %s: adding filter %s/%s %s/%s %d %s",
                      p->name, temptype->super, temptype->type,
		      filtertype->super, filtertype->type,
                      cost, program);
      mimeAddFilter(MimeDatabase, temptype, filtertype, cost, program);
    }
}


/*
 * 'add_printer_formats()' - Add document-format-supported values for a printer.
 */

static void
add_printer_formats(cupsd_printer_t *p)	/* I - Printer */
{
  int		i;			/* Looping var */
  mime_type_t	*type;			/* Current MIME type */
  cups_array_t	*filters;		/* Filters */
  ipp_attribute_t *attr;		/* document-format-supported attribute */
  char		mimetype[MIME_MAX_SUPER + MIME_MAX_TYPE + 2];
					/* MIME type name */


 /*
  * Raw (and remote) queues advertise all of the supported MIME
  * types...
  */
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  cupsArrayDelete(p->filetypes);
  p->filetypes = NULL;

  if (p->raw)
  {
    ippAddStrings(p->attrs, IPP_TAG_PRINTER,
                  (ipp_tag_t)(IPP_TAG_MIMETYPE | IPP_TAG_COPY),
                  "document-format-supported", NumMimeTypes, NULL, MimeTypes);
    //printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
    return;
  }
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
 /*
  * Otherwise, loop through the supported MIME types and see if there
  * are filters for them...
  */

  cupsdLogMessage(CUPSD_LOG_DEBUG2, "add_printer_formats: %d types, %d filters",
                  mimeNumTypes(MimeDatabase), mimeNumFilters(MimeDatabase));

  p->filetypes = cupsArrayNew(NULL, NULL);

  for (type = mimeFirstType(MimeDatabase);
       type;
       type = mimeNextType(MimeDatabase))
  {
    snprintf(mimetype, sizeof(mimetype), "%s/%s", type->super, type->type);

    if ((filters = mimeFilter(MimeDatabase, type, p->filetype, NULL)) != NULL)
    {
      cupsdLogMessage(CUPSD_LOG_DEBUG2,
                      "add_printer_formats: %s: %s needs %d filters",
                      p->name, mimetype, cupsArrayCount(filters));

      cupsArrayDelete(filters);
      cupsArrayAdd(p->filetypes, type);
    }
    else
      cupsdLogMessage(CUPSD_LOG_DEBUG2,
                      "add_printer_formats: %s: %s not supported",
                      p->name, mimetype);
  }

 /*
  * Add the file formats that can be filtered...
  */

  if ((type = mimeType(MimeDatabase, "application", "octet-stream")) == NULL ||
      !cupsArrayFind(p->filetypes, type))
    i = 1;
  else
    i = 0;

  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "add_printer_formats: %s: %d supported types",
                  p->name, cupsArrayCount(p->filetypes) + i);

  attr = ippAddStrings(p->attrs, IPP_TAG_PRINTER, IPP_TAG_MIMETYPE,
                       "document-format-supported",
                       cupsArrayCount(p->filetypes) + i, NULL, NULL);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
  if (i)
    attr->values[0].string.text = _cupsStrAlloc("application/octet-stream");

  for (type = (mime_type_t *)cupsArrayFirst(p->filetypes);
       type;
       i ++, type = (mime_type_t *)cupsArrayNext(p->filetypes))
  {
    snprintf(mimetype, sizeof(mimetype), "%s/%s", type->super, type->type);

    attr->values[i].string.text = _cupsStrAlloc(mimetype);
  }

#ifdef HAVE_DNSSD
  {
    char		pdl[1024];	/* Buffer to build pdl list */
    mime_filter_t	*filter;	/* MIME filter looping var */


   /*
    * We only support raw printing if this is not a Tioga PrintJobMgr based
    * queue and if application/octet-stream is a known type...
    */

    for (filter = (mime_filter_t *)cupsArrayFirst(MimeDatabase->filters);
	 filter;
	 filter = (mime_filter_t *)cupsArrayNext(MimeDatabase->filters))
    {
      if (filter->dst == p->filetype && filter->filter &&
	  strstr(filter->filter, "PrintJobMgr"))
	break;
    }

    pdl[0] = '\0';

    if (!filter && mimeType(MimeDatabase, "application", "octet-stream"))
      strlcat(pdl, "application/octet-stream,", sizeof(pdl));

   /*
    * Then list a bunch of formats that are supported by the printer...
    */

    for (type = (mime_type_t *)cupsArrayFirst(p->filetypes);
	 type;
	 type = (mime_type_t *)cupsArrayNext(p->filetypes))
    {
      if (!strcasecmp(type->super, "application"))
      {
        if (!strcasecmp(type->type, "pdf"))
	  strlcat(pdl, "application/pdf,", sizeof(pdl));
        else if (!strcasecmp(type->type, "postscript"))
	  strlcat(pdl, "application/postscript,", sizeof(pdl));
      }
      else if (!strcasecmp(type->super, "image"))
      {
        if (!strcasecmp(type->type, "jpeg"))
	  strlcat(pdl, "image/jpeg,", sizeof(pdl));
	else if (!strcasecmp(type->type, "png"))
	  strlcat(pdl, "image/png,", sizeof(pdl));
      }
    }

    if (pdl[0])
      pdl[strlen(pdl) - 1] = '\0';	/* Remove trailing comma */

    cupsdSetString(&p->pdl, pdl);
  }
#endif /* HAVE_DNSSD */
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
}


/*
 * 'add_string_array()' - Add a string to an array of CUPS strings.
 */
#if 0
static void
add_string_array(cups_array_t **a,	/* I - Array */
		 const char   *s)	/* I - String */
{
  if (!*a)
    *a = cupsArrayNew(NULL, NULL);

  cupsArrayAdd(*a, _cupsStrAlloc(s));
}
#endif

/*
 * 'compare_printers()' - Compare two printers.
 */

static int				/* O - Result of comparison */
compare_printers(void *first,		/* I - First printer */
                 void *second,		/* I - Second printer */
		 void *data)		/* I - App data (not used) */
{
  return (strcasecmp(((cupsd_printer_t *)first)->name,
                     ((cupsd_printer_t *)second)->name));
}


/*
 * 'delete_printer_filters()' - Delete all MIME filters for a printer.
 */

static void
delete_printer_filters(
    cupsd_printer_t *p)			/* I - Printer to remove from */
{
  mime_filter_t	*filter;		/* MIME filter looping var */


 /*
  * Range check input...
  */

  if (p == NULL)
    return;

 /*
  * Remove all filters from the MIME database that have a destination
  * type == printer...
  */

  for (filter = mimeFirstFilter(MimeDatabase);
       filter;
       filter = mimeNextFilter(MimeDatabase))
    if (filter->dst == p->filetype || filter->dst == p->prefiltertype)
    {
     /*
      * Delete the current filter...
      */

      mimeDeleteFilter(MimeDatabase, filter);
    }
#ifdef NOT_CUT

  cupsdSetPrinterReasons(p, "-cups-insecure-filter-warning"
                            ",cups-missing-filter-warning");
#endif
}


/*
 * 'delete_string_array()' - Delete an array of CUPS strings.
 */

static void
delete_string_array(cups_array_t **a)	/* I - Array */
{
  char	*ptr;				/* Current string */


  for (ptr = (char *)cupsArrayFirst(*a);
       ptr;
       ptr = (char *)cupsArrayNext(*a))
    _cupsStrFree(ptr);

  cupsArrayDelete(*a);
  *a = NULL;
}


/*
 * 'load_ppd()' - Load a cached PPD file, updating the cache as needed.
 */

static void
load_ppd(cupsd_printer_t *p)		/* I - Printer */
{
  //int		i, j, k;		/* Looping vars */
  //cups_file_t	*cache;			/* IPP cache file */
  char		cache_name[1024];	/* IPP cache filename */
  struct stat	cache_info;		/* IPP cache file info */
  char		pwg_name[1024];		/* PWG cache filename */
  struct stat	pwg_info;		/* PWG cache file info */
  //ppd_file_t	*ppd;			/* PPD file */
  char		ppd_name[1024];		/* PPD filename */
  struct stat	ppd_info;		/* PPD file info */
  #if 0
  int		num_media;		/* Number of media options */
  ppd_size_t	*size;			/* Current PPD size */
  ppd_option_t	*duplex,		/* Duplex option */
		*output_bin,		/* OutputBin option */
		*output_mode,		/* OutputMode option */
		*resolution;		/* (Set|JCL|)Resolution option */
  ppd_choice_t	*choice,		/* Current PPD choice */
		*input_slot,		/* Current input slot */
		*media_type;		/* Current media type */
  ppd_attr_t	*ppd_attr;		/* PPD attribute */
  int		xdpi,			/* Horizontal resolution */
		ydpi;			/* Vertical resolution */
  const char	*resptr;		/* Pointer into resolution keyword */
  _pwg_size_t	*pwgsize;		/* Current PWG size */
  _pwg_map_t	*pwgsource,		/* Current PWG source */
		*pwgtype;		/* Current PWG type */
  ipp_attribute_t *attr;		/* Attribute data */
  #endif
  ipp_value_t	*val;			/* Attribute value */
  int		num_finishings,		/* Number of finishings */
		finishings[5];		/* finishings-supported values */
 // int		num_qualities,		/* Number of print-quality values */
//		qualities[3];		/* print-quality values */
//  int		num_margins,		/* Number of media-*-margin-supported values */
//		margins[16];		/* media-*-margin-supported values */
  static const char * const sides[3] =	/* sides-supported values */
		{
		  "one-sided",
		  "two-sided-long-edge",
		  "two-sided-short-edge"
		};
 #if 0
  static const char * const standard_commands[] =
		{			/* Standard CUPS commands */
		  "AutoConfigure",
		  "Clean",
		  "PrintSelfTestPage"
		};

    printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);
 #endif
 /*
  * Check to see if the cache is up-to-date...
  */

  snprintf(cache_name, sizeof(cache_name), "%s/%s.ipp2", CacheDir, p->name);
  if (stat(cache_name, &cache_info))
    cache_info.st_mtime = 0;

  snprintf(pwg_name, sizeof(pwg_name), "%s/%s.pwg", CacheDir, p->name);
  if (stat(pwg_name, &pwg_info))
    pwg_info.st_mtime = 0;

  snprintf(ppd_name, sizeof(ppd_name), "%s/ppd/%s.ppd", ServerRoot, p->name);
  if (stat(ppd_name, &ppd_info))
    ppd_info.st_mtime = 1;

  ippDelete(p->ppd_attrs);
  p->ppd_attrs = ippNew();
#if 0
  _pwgDestroy(p->pwg);
  p->pwg = NULL;

  if (pwg_info.st_mtime >= ppd_info.st_mtime)
    p->pwg = _pwgCreateWithFile(pwg_name);

  if (cache_info.st_mtime >= ppd_info.st_mtime && p->pwg &&
      (cache = cupsFileOpen(cache_name, "r")) != NULL)
  {
   /*
    * Load cached information and return...
    */

    cupsdLogMessage(CUPSD_LOG_DEBUG, "load_ppd: Loading %s...", cache_name);

    if (ippReadIO(cache, (ipp_iocb_t)cupsFileRead, 1, NULL,
                  p->ppd_attrs) == IPP_DATA)
    {
      cupsFileClose(cache);
      return;
    }
    printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    cupsFileClose(cache);
  }
#endif
 /*
  * Reload PPD attributes from disk...
  */
 

  cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

#if 0

  _pwgDestroy(p->pwg);
  p->pwg = NULL;
#endif
  cupsdLogMessage(CUPSD_LOG_DEBUG, "load_ppd: Loading %s...", ppd_name);

  delete_string_array(&(p->filters));
  delete_string_array(&(p->pre_filters));

  p->type &= ~CUPS_PRINTER_OPTIONS;
  p->type |= CUPS_PRINTER_BW;

  finishings[0]  = IPP_FINISHINGS_NONE;
  num_finishings = 1;
#if 0
  if ((ppd = ppdOpenFile(ppd_name)) != NULL)
  {
   /*
    * Add make/model and other various attributes...
    */
       printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    p->pwg = _pwgCreateWithPPD(ppd);

    ppdMarkDefaults(ppd);

    if (ppd->color_device)
      p->type |= CUPS_PRINTER_COLOR;
    if (ppd->variable_sizes)
      p->type |= CUPS_PRINTER_VARIABLE;
    if (!ppd->manual_copies)
      p->type |= CUPS_PRINTER_COPIES;
    if ((ppd_attr = ppdFindAttr(ppd, "cupsFax", NULL)) != NULL)
      if (ppd_attr->value && !strcasecmp(ppd_attr->value, "true"))
	p->type |= CUPS_PRINTER_FAX;

    ippAddBoolean(p->ppd_attrs, IPP_TAG_PRINTER, "color-supported",
		  ppd->color_device);
    if (ppd->throughput)
    {
      ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		    "pages-per-minute", ppd->throughput);
      if (ppd->color_device)
	ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		      "pages-per-minute-color", ppd->throughput);
    }

    num_qualities = 0;

    if ((output_mode = ppdFindOption(ppd, "OutputMode")) != NULL)
    {
      if (ppdFindChoice(output_mode, "draft") ||
          ppdFindChoice(output_mode, "fast"))
        qualities[num_qualities ++] = IPP_QUALITY_DRAFT;
      if (ppdFindChoice(output_mode, "normal") ||
          ppdFindChoice(output_mode, "good"))
        qualities[num_qualities ++] = IPP_QUALITY_NORMAL;
      if (ppdFindChoice(output_mode, "best") ||
          ppdFindChoice(output_mode, "high"))
        qualities[num_qualities ++] = IPP_QUALITY_HIGH;
    }
    else if ((ppd_attr = ppdFindAttr(ppd, "APPrinterPreset", NULL)) != NULL)
    {
      do
      {
        if (strstr(ppd_attr->spec, "draft") ||
	    strstr(ppd_attr->spec, "Draft"))
	{
	  qualities[num_qualities ++] = IPP_QUALITY_DRAFT;
	  break;
	}
      }
      while ((ppd_attr = ppdFindNextAttr(ppd, "APPrinterPreset",
                                         NULL)) != NULL);

      qualities[num_qualities ++] = IPP_QUALITY_NORMAL;
      qualities[num_qualities ++] = IPP_QUALITY_HIGH;
    }

    if (num_qualities == 0)
      qualities[num_qualities ++] = IPP_QUALITY_NORMAL;

    ippAddIntegers(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_ENUM,
                   "print-quality-supported", num_qualities, qualities);

    if (ppd->nickname)
    {
     /*
      * The NickName can be localized in the character set specified
      * by the LanugageEncoding attribute.  However, ppdOpen2() has
      * already converted the ppd->nickname member to UTF-8 for us
      * (the original attribute value is available separately)
      */

      cupsdSetString(&p->make_model, ppd->nickname);
    }
    else if (ppd->modelname)
    {
     /*
      * Model name can only contain specific characters...
      */

      cupsdSetString(&p->make_model, ppd->modelname);
    }
    else
      cupsdSetString(&p->make_model, "Bad PPD File");

    ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
		 "printer-make-and-model", NULL, p->make_model);

   /*
    * Add media options from the PPD file...
    */

    if (ppd->num_sizes == 0 || !p->pwg)
    {
      if (!ppdFindAttr(ppd, "APScannerOnly", NULL))
	cupsdLogMessage(CUPSD_LOG_CRIT,
			"The PPD file for printer %s contains no media "
			"options and is therefore invalid!", p->name);

      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "media-default", NULL, "unknown");
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "media-supported", NULL, "unknown");
    }
    else
    {
     /*
      * media-default
      */

      if ((size = ppdPageSize(ppd, NULL)) != NULL)
        pwgsize = _pwgGetSize(p->pwg, size->name);
      else
        pwgsize = NULL;

      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "media-default", NULL,
		   pwgsize ? pwgsize->map.pwg : "unknown");

     /*
      * media-col-default
      */
         printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

      if (pwgsize)
      {
        ipp_t	*col;			/* Collection value */

	input_slot = ppdFindMarkedChoice(ppd, "InputSlot");
	media_type = ppdFindMarkedChoice(ppd, "MediaType");
	col        = new_media_col(pwgsize,
			           input_slot ?
				       _pwgGetSource(p->pwg,
				                     input_slot->choice) :
				       NULL,
				   media_type ?
				       _pwgGetType(p->pwg,
				                   media_type->choice) :
				       NULL);

	ippAddCollection(p->ppd_attrs, IPP_TAG_PRINTER, "media-col-default",
	                 col);
        ippDelete(col);
      }

     /*
      * media-supported
      */

      num_media = p->pwg->num_sizes;
      if (p->pwg->custom_min_keyword)
	num_media += 2;

      if ((attr = ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
			        "media-supported", num_media, NULL,
				NULL)) != NULL)
      {

	val = attr->values;

        for (i = p->pwg->num_sizes, pwgsize = p->pwg->sizes;
	     i > 0;
	     i --, pwgsize ++, val ++)
	  val->string.text = _cupsStrRetain(pwgsize->map.pwg);

        if (p->pwg->custom_min_keyword)
	{
	  val->string.text = _cupsStrRetain(p->pwg->custom_min_keyword);
	  val ++;
	  val->string.text = _cupsStrRetain(p->pwg->custom_max_keyword);
        }
      }
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
     /*
      * media-source-supported
      */

      if (p->pwg->num_sources > 0 &&
          (attr = ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
	                        "media-source-supported", p->pwg->num_sources,
			        NULL, NULL)) != NULL)
      {
	for (i = p->pwg->num_sources, pwgsource = p->pwg->sources,
	         val = attr->values;
	     i > 0;
	     i --, pwgsource ++, val ++)
	  val->string.text = _cupsStrRetain(pwgsource->pwg);
      }

     /*
      * media-type-supported
      */
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
      if (p->pwg->num_types > 0 &&
          (attr = ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
	                        "media-type-supported", p->pwg->num_types,
			        NULL, NULL)) != NULL)
      {
	for (i = p->pwg->num_types, pwgtype = p->pwg->types,
	         val = attr->values;
	     i > 0;
	     i --, pwgtype ++, val ++)
	  val->string.text = _cupsStrRetain(pwgtype->pwg);
      }

     /*
      * media-*-margin-supported
      */

      for (i = p->pwg->num_sizes, pwgsize = p->pwg->sizes, num_margins = 0;
	   i > 0 && num_margins < (int)(sizeof(margins) / sizeof(margins[0]));
	   i --, pwgsize ++)
      {
        for (j = 0; j < num_margins; j ++)
	  if (pwgsize->bottom == margins[j])
	    break;

	if (j >= num_margins)
	{
	  margins[num_margins] = pwgsize->bottom;
	  num_margins ++;
	}
      }
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
      if (num_margins > 0)
        ippAddIntegers(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		       "media-bottom-margin-supported", num_margins, margins);
      else
        ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		      "media-bottom-margin-supported", 0);

      for (i = p->pwg->num_sizes, pwgsize = p->pwg->sizes, num_margins = 0;
	   i > 0 && num_margins < (int)(sizeof(margins) / sizeof(margins[0]));
	   i --, pwgsize ++)
      {
        for (j = 0; j < num_margins; j ++)
	  if (pwgsize->left == margins[j])
	    break;

	if (j >= num_margins)
	{
	  margins[num_margins] = pwgsize->left;
	  num_margins ++;
	}
      }

      if (num_margins > 0)
        ippAddIntegers(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		       "media-left-margin-supported", num_margins, margins);
      else
        ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		      "media-left-margin-supported", 0);

      for (i = p->pwg->num_sizes, pwgsize = p->pwg->sizes, num_margins = 0;
	   i > 0 && num_margins < (int)(sizeof(margins) / sizeof(margins[0]));
	   i --, pwgsize ++)
      {
        for (j = 0; j < num_margins; j ++)
	  if (pwgsize->right == margins[j])
	    break;

	if (j >= num_margins)
	{
	  margins[num_margins] = pwgsize->right;
	  num_margins ++;
	}
      }

      if (num_margins > 0)
        ippAddIntegers(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		       "media-right-margin-supported", num_margins, margins);
      else
        ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		      "media-right-margin-supported", 0);

      for (i = p->pwg->num_sizes, pwgsize = p->pwg->sizes, num_margins = 0;
	   i > 0 && num_margins < (int)(sizeof(margins) / sizeof(margins[0]));
	   i --, pwgsize ++)
      {
        for (j = 0; j < num_margins; j ++)
	  if (pwgsize->top == margins[j])
	    break;

	if (j >= num_margins)
	{
	  margins[num_margins] = pwgsize->top;
	  num_margins ++;
	}
      }

      if (num_margins > 0)
        ippAddIntegers(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		       "media-top-margin-supported", num_margins, margins);
      else
        ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		      "media-top-margin-supported", 0);

     /*
      * media-col-database
      */

      num_media = p->pwg->num_sizes;
      if (p->pwg->num_sources)
      {
        if (p->pwg->num_types > 0)
	  num_media += p->pwg->num_sizes * p->pwg->num_sources *
	               p->pwg->num_types;
	else
          num_media += p->pwg->num_sizes * p->pwg->num_sources;
      }
      else if (p->pwg->num_types)
        num_media += p->pwg->num_sizes * p->pwg->num_types;

      if ((attr = ippAddCollections(p->ppd_attrs, IPP_TAG_PRINTER,
                                    "media-col-database", num_media,
				    NULL)) != NULL)
      {
        for (i = p->pwg->num_sizes, pwgsize = p->pwg->sizes, val = attr->values;
	     i > 0;
	     i --, pwgsize ++)
	{
	 /*
	  * Start by adding the page size without source or type...
	  */

	  ppdMarkOption(ppd, "PageSize", pwgsize->map.ppd);

          val->collection = new_media_col(pwgsize, NULL, NULL);
	  val ++;

         /*
	  * Then add the specific, supported combinations of size, source, and
	  * type...
	  */

	  if (p->pwg->num_sources > 0)
	  {
	    for (j = p->pwg->num_sources, pwgsource = p->pwg->sources;
	         j > 0;
		 j --, pwgsource ++)
	    {
	      ppdMarkOption(ppd, "InputSlot", pwgsource->ppd);

	      if (p->pwg->num_types > 0)
	      {
	        for (k = p->pwg->num_types, pwgtype = p->pwg->types;
		     k > 0;
		     k --, pwgtype ++)
		{
		  if (!ppdMarkOption(ppd, "MediaType", pwgtype->ppd))
		  {
		    val->collection = new_media_col(pwgsize, pwgsource->pwg,
		                                    pwgtype->pwg);
		    val ++;
		  }
		}
	      }
      #ifdef NOT_CUT
	      else if (!ppdConflicts(ppd))
	      {
	        val->collection = new_media_col(pwgsize, pwgsource->pwg, NULL);
		val ++;
	      }
      #endif
	    }
	  }
	  else if (p->pwg->num_types > 0)
	  {
	    for (j = p->pwg->num_types, pwgtype = p->pwg->types;
	         j > 0;
		 j --, pwgtype ++)
	    {
	      if (!ppdMarkOption(ppd, "MediaType", pwgtype->ppd))
	      {
	        val->collection = new_media_col(pwgsize, NULL, pwgtype->pwg);
		val ++;
	      }
	    }
	  }
	}

       /*
        * Update the number of media-col-database values...
	*/

	attr->num_values = val - attr->values;
      }
    }

   /*
    * Output bin...
    */

    if (p->pwg && p->pwg->num_bins > 0)
    {
      attr = ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
			   "output-bin-supported", p->pwg->num_bins,
			   NULL, NULL);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
      if (attr != NULL)
      {
	for (i = 0, val = attr->values;
	     i < p->pwg->num_bins;
	     i ++, val ++)
	  val->string.text = _cupsStrAlloc(p->pwg->bins[i].pwg);
      }

      if ((output_bin = ppdFindOption(ppd, "OutputBin")) != NULL)
      {
	for (i = 0; i < p->pwg->num_bins; i ++)
	  if (!strcmp(p->pwg->bins[i].ppd, output_bin->defchoice))
	    break;

        if (i >= p->pwg->num_bins)
	  i = 0;

	ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		     "output-bin-default", NULL, p->pwg->bins[i].pwg);
      }
      else
        ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
	             "output-bin-default", NULL, p->pwg->bins[0].pwg);
    }
    else if ((ppd_attr = ppdFindAttr(ppd, "DefaultOutputOrder",
                                     NULL)) != NULL &&
	     !strcasecmp(ppd_attr->value, "Reverse"))
    {
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "output-bin-supported", NULL, "face-up");
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "output-bin-default", NULL, "face-up");
    }
    else
    {
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "output-bin-supported", NULL, "face-down");
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "output-bin-default", NULL, "face-down");
    }

   /*
    * Printer resolutions...
    */

    if ((resolution = ppdFindOption(ppd, "Resolution")) == NULL)
      if ((resolution = ppdFindOption(ppd, "JCLResolution")) == NULL)
        if ((resolution = ppdFindOption(ppd, "SetResolution")) == NULL)
	  resolution = ppdFindOption(ppd, "CNRes_PGP");

    if (resolution)
    {
     /*
      * Report all supported resolutions...
      */

      attr = ippAddResolutions(p->ppd_attrs, IPP_TAG_PRINTER,
                               "printer-resolution-supported",
                               resolution->num_choices, IPP_RES_PER_INCH,
			       NULL, NULL);

      for (i = 0, choice = resolution->choices;
           i < resolution->num_choices;
	   i ++, choice ++)
      {
        xdpi = ydpi = (int)strtol(choice->choice, (char **)&resptr, 10);
	if (resptr > choice->choice && xdpi > 0 && *resptr == 'x')
	  ydpi = (int)strtol(resptr + 1, (char **)&resptr, 10);

	if (xdpi <= 0 || ydpi <= 0)
	{
	  cupsdLogMessage(CUPSD_LOG_WARN,
	                  "Bad resolution \"%s\" for printer %s.",
			  choice->choice, p->name);
	  xdpi = ydpi = 72;
	}

        attr->values[i].resolution.xres  = xdpi;
        attr->values[i].resolution.yres  = ydpi;
        attr->values[i].resolution.units = IPP_RES_PER_INCH;

        if (choice->marked)
	  ippAddResolution(p->ppd_attrs, IPP_TAG_PRINTER,
	                   "printer-resolution-default", IPP_RES_PER_INCH,
			   xdpi, ydpi);
      }
    }
    else if ((ppd_attr = ppdFindAttr(ppd, "DefaultResolution", NULL)) != NULL &&
             ppd_attr->value)
    {
     /*
      * Just the DefaultResolution to report...
      */

      xdpi = ydpi = (int)strtol(ppd_attr->value, (char **)&resptr, 10);
      if (resptr > ppd_attr->value && xdpi > 0)
      {
	if (*resptr == 'x')
	  ydpi = (int)strtol(resptr + 1, (char **)&resptr, 10);
	else
	  ydpi = xdpi;
      }

      if (xdpi <= 0 || ydpi <= 0)
      {
	cupsdLogMessage(CUPSD_LOG_WARN,
			"Bad default resolution \"%s\" for printer %s.",
			ppd_attr->value, p->name);
	xdpi = ydpi = 72;
      }

      ippAddResolution(p->ppd_attrs, IPP_TAG_PRINTER,
		       "printer-resolution-default", IPP_RES_PER_INCH,
		       xdpi, ydpi);
      ippAddResolution(p->ppd_attrs, IPP_TAG_PRINTER,
		       "printer-resolution-supported", IPP_RES_PER_INCH,
		       xdpi, ydpi);
    }
    else
    {
     /*
      * No resolutions in PPD - make one up...
      */

      ippAddResolution(p->ppd_attrs, IPP_TAG_PRINTER,
		       "printer-resolution-default", IPP_RES_PER_INCH,
		       72, 72);
      ippAddResolution(p->ppd_attrs, IPP_TAG_PRINTER,
		       "printer-resolution-supported", IPP_RES_PER_INCH,
		       72, 72);
    }

   /*
    * Duplexing, etc...
    */

    if ((duplex = ppdFindOption(ppd, "Duplex")) == NULL)
      if ((duplex = ppdFindOption(ppd, "EFDuplex")) == NULL)
	if ((duplex = ppdFindOption(ppd, "EFDuplexing")) == NULL)
	  if ((duplex = ppdFindOption(ppd, "KD03Duplex")) == NULL)
	    duplex = ppdFindOption(ppd, "JCLDuplex");
#ifdef NOT_CUT
    if (duplex && duplex->num_choices > 1 &&
	!ppdInstallableConflict(ppd, duplex->keyword, "DuplexTumble"))
#else
    if (duplex && duplex->num_choices > 1)
#endif
    {
      p->type |= CUPS_PRINTER_DUPLEX;

      ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		    "sides-supported", 3, NULL, sides);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
      if (!strcasecmp(duplex->defchoice, "DuplexTumble"))
	ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		     "sides-default", NULL, "two-sided-short-edge");
      else if (!strcasecmp(duplex->defchoice, "DuplexNoTumble"))
	ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		     "sides-default", NULL, "two-sided-long-edge");
      else
	ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		     "sides-default", NULL, "one-sided");
    }
    else
    {
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "sides-supported", NULL, "one-sided");
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "sides-default", NULL, "one-sided");
    }

    if (ppdFindOption(ppd, "Collate") != NULL)
      p->type |= CUPS_PRINTER_COLLATE;

    if (ppdFindOption(ppd, "StapleLocation") != NULL)
    {
      p->type |= CUPS_PRINTER_STAPLE;
      finishings[num_finishings++] = IPP_FINISHINGS_STAPLE;
    }

    if (ppdFindOption(ppd, "BindEdge") != NULL)
    {
      p->type |= CUPS_PRINTER_BIND;
      finishings[num_finishings++] = IPP_FINISHINGS_BIND;
    }

    for (i = 0; i < ppd->num_sizes; i ++)
      if (ppd->sizes[i].length > 1728)
	p->type |= CUPS_PRINTER_LARGE;
      else if (ppd->sizes[i].length > 1008)
	p->type |= CUPS_PRINTER_MEDIUM;
      else
	p->type |= CUPS_PRINTER_SMALL;

    if ((ppd_attr = ppdFindAttr(ppd, "APICADriver", NULL)) != NULL &&
        ppd_attr->value && !strcasecmp(ppd_attr->value, "true"))
    {
      if ((ppd_attr = ppdFindAttr(ppd, "APScannerOnly", NULL)) != NULL &&
	  ppd_attr->value && !strcasecmp(ppd_attr->value, "true"))
        p->type |= CUPS_PRINTER_SCANNER;
      else
        p->type |= CUPS_PRINTER_MFP;
    }

   /*
    * Add a filter from application/vnd.cups-raw to printer/name to
    * handle "raw" printing by users.
    */

    add_string_array(&(p->filters), "application/vnd.cups-raw 0 -");

   /*
    * Add any pre-filters in the PPD file...
    */

    if ((ppd_attr = ppdFindAttr(ppd, "cupsPreFilter", NULL)) != NULL)
    {
      for (; ppd_attr; ppd_attr = ppdFindNextAttr(ppd, "cupsPreFilter", NULL))
	if (ppd_attr->value)
	  add_string_array(&(p->pre_filters), ppd_attr->value);
    }

   /*
    * Add any filters in the PPD file...
    */

    DEBUG_printf(("ppd->num_filters = %d\n", ppd->num_filters));
    for (i = 0; i < ppd->num_filters; i ++)
    {
      DEBUG_printf(("ppd->filters[%d] = \"%s\"\n", i, ppd->filters[i]));
      add_string_array(&(p->filters), ppd->filters[i]);

      if (!strncasecmp(ppd->filters[i], "application/vnd.cups-command", 28) &&
          isspace(ppd->filters[i][28] & 255))
        p->type |= CUPS_PRINTER_COMMANDS;
    }

    if (ppd->num_filters == 0)
    {
     /*
      * If there are no filters, add PostScript printing filters.
      */

      add_string_array(&(p->filters),
                       "application/vnd.cups-command 0 commandtops");
      add_string_array(&(p->filters),
                       "application/vnd.cups-postscript 0 -");

      p->type |= CUPS_PRINTER_COMMANDS;
    }
    else if (!(p->type & CUPS_PRINTER_COMMANDS))
    {
     /*
      * See if this is a PostScript device without a command filter...
      */

      for (i = 0; i < ppd->num_filters; i ++)
	if (!strncasecmp(ppd->filters[i],
			 "application/vnd.cups-postscript", 31) &&
            isspace(ppd->filters[i][31] & 255))
	  break;

      if (i < ppd->num_filters)
      {
       /*
	* Add the generic PostScript command filter...
	*/

	add_string_array(&(p->filters),
	                 "application/vnd.cups-command 0 commandtops");
	p->type |= CUPS_PRINTER_COMMANDS;
      }
    }

    if (p->type & CUPS_PRINTER_COMMANDS)
    {
      char	*commands,		/* Copy of commands */
		*start,			/* Start of name */
		*end;			/* End of name */
      int	count;			/* Number of commands */


      if ((ppd_attr = ppdFindAttr(ppd, "cupsCommands", NULL)) != NULL &&
	  ppd_attr->value && ppd_attr->value[0])
      {
	for (count = 0, start = ppd_attr->value; *start; count ++)
	{
	  while (isspace(*start & 255))
	    start ++;

	  if (!*start)
	    break;

	  while (*start && !isspace(*start & 255))
	    start ++;
	}
      }
      else
	count = 0;

      if (count > 0)
      {
       /*
	* Make a copy of the commands string and count how many ...
	*/

	attr = ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
			     "printer-commands", count, NULL, NULL);

	commands = strdup(ppd_attr->value);

	for (count = 0, start = commands; *start; count ++)
	{
	  while (isspace(*start & 255))
	    start ++;

	  if (!*start)
	    break;

	  end = start;
	  while (*end && !isspace(*end & 255))
	    end ++;

	  if (*end)
	    *end++ = '\0';

	  attr->values[count].string.text = _cupsStrAlloc(start);

	  start = end;
	}

	free(commands);
      }
      else
      {
       /*
	* Add the standard list of commands...
	*/

	ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		      "printer-commands",
		      (int)(sizeof(standard_commands) /
			    sizeof(standard_commands[0])), NULL,
		      standard_commands);
       //printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
      }
    }
    else
    {
     /*
      * No commands supported...
      */

      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_KEYWORD,
		   "printer-commands", NULL, "none");
    }

   /*
    * Show current and available port monitors for this printer...
    */

    ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_NAME, "port-monitor",
		 NULL, p->port_monitor ? p->port_monitor : "none");

    for (i = 1, ppd_attr = ppdFindAttr(ppd, "cupsPortMonitor", NULL);
	 ppd_attr;
	 i ++, ppd_attr = ppdFindNextAttr(ppd, "cupsPortMonitor", NULL));

    if (ppd->protocols)
    {
      if (strstr(ppd->protocols, "TBCP"))
	i ++;
      else if (strstr(ppd->protocols, "BCP"))
	i ++;
    }

    attr = ippAddStrings(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_NAME,
			 "port-monitor-supported", i, NULL, NULL);
//printf("\n=file:%s,line:%d,func:%s=\n",__FILE__,__LINE__,__FUNCTION__);
    attr->values[0].string.text = _cupsStrAlloc("none");

    for (i = 1, ppd_attr = ppdFindAttr(ppd, "cupsPortMonitor", NULL);
	 ppd_attr;
	 i ++, ppd_attr = ppdFindNextAttr(ppd, "cupsPortMonitor", NULL))
      attr->values[i].string.text = _cupsStrAlloc(ppd_attr->value);

    if (ppd->protocols)
    {
      if (strstr(ppd->protocols, "TBCP"))
	attr->values[i].string.text = _cupsStrAlloc("tbcp");
      else if (strstr(ppd->protocols, "BCP"))
	attr->values[i].string.text = _cupsStrAlloc("bcp");
    }

#ifdef HAVE_DNSSD
    cupsdSetString(&p->product, ppd->product);
#endif /* HAVE_DNSSD */

    if (ppdFindAttr(ppd, "APRemoteQueueID", NULL))
      p->type |= CUPS_PRINTER_REMOTE;

   /*
    * Close the PPD and set the type...
    */

    ppdClose(ppd);
  }
  else if (!access(ppd_name, 0))
  {
    int			pline;		/* PPD line number */
    ppd_status_t	pstatus;	/* PPD load status */

    printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    pstatus = ppdLastError(&pline);

    cupsdLogMessage(CUPSD_LOG_ERROR, "PPD file for %s cannot be loaded!",
		    p->name);

    if (pstatus <= PPD_ALLOC_ERROR)
      cupsdLogMessage(CUPSD_LOG_ERROR, "%s", strerror(errno));
    else
      cupsdLogMessage(CUPSD_LOG_ERROR, "%s on line %d.",
		      ppdErrorString(pstatus), pline);

    cupsdLogMessage(CUPSD_LOG_INFO,
		    "Hint: Run \"cupstestppd %s\" and fix any errors.",
		    ppd_name);

   /*
    * Add a filter from application/vnd.cups-raw to printer/name to
    * handle "raw" printing by users.
    */

    add_string_array(&(p->filters), "application/vnd.cups-raw 0 -");

   /*
    * Add a PostScript filter, since this is still possibly PS printer.
    */

    add_string_array(&(p->filters), "application/vnd.cups-postscript 0 -");
  }
  else
  {
   /*
    * If we have an interface script, add a filter entry for it...
    */
       printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    char	interface[1024];	/* Interface script */


    snprintf(interface, sizeof(interface), "%s/interfaces/%s", ServerRoot,
	     p->name);
    if (!access(interface, X_OK))
    {
     /*
      * Yes, we have a System V style interface script; use it!
      */
         printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
		   "printer-make-and-model", NULL,
		   "Local System V Printer");

      snprintf(interface, sizeof(interface), "*/* 0 %s/interfaces/%s",
	       ServerRoot, p->name);
      add_string_array(&(p->filters), interface);
    }
    else if (!strncmp(p->device_uri, "ipp://", 6) &&
	     (strstr(p->device_uri, "/printers/") != NULL ||
	      strstr(p->device_uri, "/classes/") != NULL ||
	      (strstr(p->device_uri, "._ipp.") != NULL &&
	       !strcmp(p->device_uri + strlen(p->device_uri) - 5,
		       "/cups"))))
    {
     /*
      * Tell the client this is really a hard-wired remote printer.
      */
         printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

      p->type |= CUPS_PRINTER_REMOTE;

     /*
      * Point the printer-uri-supported attribute to the
      * remote printer...
      */

      ippAddString(p->attrs, IPP_TAG_PRINTER, IPP_TAG_URI,
		   "printer-uri-supported", NULL, p->device_uri);

     /*
      * Then set the make-and-model accordingly...
      */

      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
		   "printer-make-and-model", NULL, "Remote Printer");

     /*
      * Print all files directly...
      */

      p->raw    = 1;
      p->remote = 1;
    }
    else
    {
    #endif
     /*
      * Otherwise we have neither - treat this as a "dumb" printer
      * with no PPD file...
      */
      ippAddString(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_TEXT,
		   "printer-make-and-model", NULL, "Local Raw Printer");

      p->raw = 1;
 #if 0     
    }
  }
#endif
  ippAddIntegers(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_ENUM,
		 "finishings-supported", num_finishings, finishings);
  ippAddInteger(p->ppd_attrs, IPP_TAG_PRINTER, IPP_TAG_ENUM,
		"finishings-default", IPP_FINISHINGS_NONE);
#if 0
  if (ppd && (cache = cupsFileOpen(cache_name, "w9")) != NULL)
  {
   /*
    * Save cached PPD attributes to disk...
    */
       printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    cupsdLogMessage(CUPSD_LOG_DEBUG, "load_ppd: Saving %s...", cache_name);

    p->ppd_attrs->state = IPP_IDLE;

    if (ippWriteIO(cache, (ipp_iocb_t)cupsFileWrite, 1, NULL,
                   p->ppd_attrs) != IPP_DATA)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to save PPD cache file \"%s\" - %s", cache_name,
		      strerror(errno));
      unlink(cache_name);
    }

    cupsFileClose(cache);

    _pwgWriteFile(p->pwg, pwg_name);
  }
  else
  {
 
   /*
    * Remove cache files...
    */
       printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);

    if (cache_info.st_mtime)
      unlink(cache_name);

    if (pwg_info.st_mtime)
      unlink(pwg_name);
  }
  
  printf("\n file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__FUNCTION__);
 #endif
}


/*
 * 'new_media_col()' - Create a media-col collection value.
 */
#if 0
static ipp_t *				/* O - Collection value */
new_media_col(_pwg_size_t *size,	/* I - media-size/margin values */
              const char  *source,	/* I - media-source value */
              const char  *type)	/* I - media-type value */
{
  ipp_t	*media_col,			/* Collection value */
	*media_size;			/* media-size value */


  media_col = ippNew();

  media_size = ippNew();
  ippAddInteger(media_size, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		"x-dimension", size->width);
  ippAddInteger(media_size, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		"y-dimension", size->length);
  ippAddCollection(media_col, IPP_TAG_PRINTER, "media-size", media_size);
  ippDelete(media_size);

  ippAddInteger(media_col, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		"media-bottom-margin", size->bottom);
  ippAddInteger(media_col, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		"media-left-margin", size->left);
  ippAddInteger(media_col, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		"media-right-margin", size->right);
  ippAddInteger(media_col, IPP_TAG_PRINTER, IPP_TAG_INTEGER,
		"media-top-margin", size->top);

  if (source)
    ippAddString(media_col, IPP_TAG_PRINTER, IPP_TAG_KEYWORD, "media-source",
		 NULL, source);

  if (type)
    ippAddString(media_col, IPP_TAG_PRINTER, IPP_TAG_KEYWORD, "media-type",
		 NULL, type);

  return (media_col);
}
#endif

#ifdef __sgi
/*
 * 'write_irix_config()' - Update the config files used by the IRIX
 *                         desktop tools.
 */

static void
write_irix_config(cupsd_printer_t *p)	/* I - Printer to update */
{
  char		filename[1024];		/* Interface script filename */
  cups_file_t	*fp;			/* Interface script file */
  ipp_attribute_t *attr;		/* Attribute data */


 /*
  * Add dummy interface and GUI scripts to fool SGI's "challenged" printing
  * tools.  First the interface script that tells the tools what kind of
  * printer we have...
  */

  snprintf(filename, sizeof(filename), "/var/spool/lp/interface/%s", p->name);

  if (p->type & CUPS_PRINTER_CLASS)
    unlink(filename);
  else if ((fp = cupsFileOpen(filename, "w")) != NULL)
  {
    cupsFilePuts(fp, "#!/bin/sh\n");

    if ((attr = ippFindAttribute(p->attrs, "printer-make-and-model",
                                 IPP_TAG_TEXT)) != NULL)
      cupsFilePrintf(fp, "NAME=\"%s\"\n", attr->values[0].string.text);
    else if (p->type & CUPS_PRINTER_CLASS)
      cupsFilePuts(fp, "NAME=\"Printer Class\"\n");
    else
      cupsFilePuts(fp, "NAME=\"Remote Destination\"\n");

    if (p->type & CUPS_PRINTER_COLOR)
      cupsFilePuts(fp, "TYPE=ColorPostScript\n");
    else
      cupsFilePuts(fp, "TYPE=MonoPostScript\n");

    cupsFilePrintf(fp, "HOSTNAME=%s\n", ServerName);
    cupsFilePrintf(fp, "HOSTPRINTER=%s\n", p->name);

    cupsFileClose(fp);

    chmod(filename, 0755);
    chown(filename, User, Group);
  }

 /*
  * Then the member file that tells which device file the queue is connected
  * to...  Networked printers use "/dev/null" in this file, so that's what
  * we use (the actual device URI can confuse some apps...)
  */

  snprintf(filename, sizeof(filename), "/var/spool/lp/member/%s", p->name);

  if (p->type & CUPS_PRINTER_CLASS)
    unlink(filename);
  else if ((fp = cupsFileOpen(filename, "w")) != NULL)
  {
    cupsFilePuts(fp, "/dev/null\n");

    cupsFileClose(fp);

    chmod(filename, 0644);
    chown(filename, User, Group);
  }

 /*
  * The gui_interface file is a script or program that launches a GUI
  * option panel for the printer, using options specified on the
  * command-line in the third argument.  The option panel must send
  * any printing options to stdout on a single line when the user
  * accepts them, or nothing if the user cancels the dialog.
  *
  * The default options panel program is /usr/bin/glpoptions, from
  * the ESP Print Pro software.  You can select another using the
  * PrintcapGUI option.
  */

  snprintf(filename, sizeof(filename), "/var/spool/lp/gui_interface/ELF/%s.gui", p->name);

  if (p->type & CUPS_PRINTER_CLASS)
    unlink(filename);
  else if ((fp = cupsFileOpen(filename, "w")) != NULL)
  {
    cupsFilePuts(fp, "#!/bin/sh\n");
    cupsFilePrintf(fp, "%s -d %s -o \"$3\"\n", PrintcapGUI, p->name);

    cupsFileClose(fp);

    chmod(filename, 0755);
    chown(filename, User, Group);
  }

 /*
  * The POD config file is needed by the printstatus command to show
  * the printer location and device.
  */

  snprintf(filename, sizeof(filename), "/var/spool/lp/pod/%s.config", p->name);

  if (p->type & CUPS_PRINTER_CLASS)
    unlink(filename);
  else if ((fp = cupsFileOpen(filename, "w")) != NULL)
  {
    cupsFilePrintf(fp, "Printer Class      | %s\n",
            (p->type & CUPS_PRINTER_COLOR) ? "ColorPostScript" : "MonoPostScript");
    cupsFilePrintf(fp, "Printer Model      | %s\n", p->make_model ? p->make_model : "");
    cupsFilePrintf(fp, "Location Code      | %s\n", p->location ? p->location : "");
    cupsFilePrintf(fp, "Physical Location  | %s\n", p->info ? p->info : "");
    cupsFilePrintf(fp, "Port Path          | %s\n", p->device_uri);
    cupsFilePrintf(fp, "Config Path        | /var/spool/lp/pod/%s.config\n", p->name);
    cupsFilePrintf(fp, "Active Status Path | /var/spool/lp/pod/%s.status\n", p->name);
    cupsFilePuts(fp, "Status Update Wait | 10 seconds\n");

    cupsFileClose(fp);

    chmod(filename, 0664);
    chown(filename, User, Group);
  }
}


/*
 * 'write_irix_state()' - Update the status files used by IRIX printing
 *                        desktop tools.
 */

static void
write_irix_state(cupsd_printer_t *p)	/* I - Printer to update */
{
  char		filename[1024];		/* Interface script filename */
  cups_file_t	*fp;			/* Interface script file */
  int		tag;			/* Status tag value */


  if (p)
  {
   /*
    * The POD status file is needed for the printstatus window to
    * provide the current status of the printer.
    */

    snprintf(filename, sizeof(filename), "/var/spool/lp/pod/%s.status", p->name);

    if (p->type & CUPS_PRINTER_CLASS)
      unlink(filename);
    else if ((fp = cupsFileOpen(filename, "w")) != NULL)
    {
      cupsFilePrintf(fp, "Operational Status | %s\n",
              (p->state == IPP_PRINTER_IDLE)       ? "Idle" :
              (p->state == IPP_PRINTER_PROCESSING) ? "Busy" :
                                                     "Faulted");
      cupsFilePrintf(fp, "Information        | 01 00 00 | %s\n", CUPS_SVERSION);
      cupsFilePrintf(fp, "Information        | 02 00 00 | Device URI: %s\n",
              p->device_uri);
      cupsFilePrintf(fp, "Information        | 03 00 00 | %s jobs\n",
              p->accepting ? "Accepting" : "Not accepting");
      cupsFilePrintf(fp, "Information        | 04 00 00 | %s\n", p->state_message);

      cupsFileClose(fp);

      chmod(filename, 0664);
      chown(filename, User, Group);
    }

   /*
    * The activeicons file is needed to provide desktop icons for printers:
    *
    * [ quoted from /usr/lib/print/tagit ]
    *
    * --- Type of printer tags (base values)
    *
    * Dumb=66048			# 0x10200
    * DumbColor=66080		# 0x10220
    * Raster=66112		# 0x10240
    * ColorRaster=66144		# 0x10260
    * Plotter=66176		# 0x10280
    * PostScript=66208		# 0x102A0
    * ColorPostScript=66240	# 0x102C0
    * MonoPostScript=66272	# 0x102E0
    *
    * --- Printer state modifiers for local printers
    *
    * Idle=0			# 0x0
    * Busy=1			# 0x1
    * Faulted=2			# 0x2
    * Unknown=3			# 0x3 (Faulted due to unknown reason)
    *
    * --- Printer state modifiers for network printers
    *
    * NetIdle=8			# 0x8
    * NetBusy=9			# 0x9
    * NetFaulted=10		# 0xA
    * NetUnknown=11		# 0xB (Faulted due to unknown reason)
    */

    snprintf(filename, sizeof(filename), "/var/spool/lp/activeicons/%s", p->name);

    if (p->type & CUPS_PRINTER_CLASS)
      unlink(filename);
    else if ((fp = cupsFileOpen(filename, "w")) != NULL)
    {
      if (p->type & CUPS_PRINTER_COLOR)
	tag = 66240;
      else
	tag = 66272;

      if (p->type & CUPS_PRINTER_REMOTE)
	tag |= 8;

      if (p->state == IPP_PRINTER_PROCESSING)
	tag |= 1;

      else if (p->state == IPP_PRINTER_STOPPED)
	tag |= 2;

      cupsFilePuts(fp, "#!/bin/sh\n");
      cupsFilePrintf(fp, "#Tag %d\n", tag);

      cupsFileClose(fp);

      chmod(filename, 0755);
      chown(filename, User, Group);
    }
  }

 /*
  * The default file is needed by the printers window to show
  * the default printer.
  */

  snprintf(filename, sizeof(filename), "/var/spool/lp/default");

  if (DefaultPrinter != NULL)
  {
    if ((fp = cupsFileOpen(filename, "w")) != NULL)
    {
      cupsFilePrintf(fp, "%s\n", DefaultPrinter->name);

      cupsFileClose(fp);

      chmod(filename, 0644);
      chown(filename, User, Group);
    }
  }
  else
    unlink(filename);
}
#endif /* __sgi */


/*
 * 'write_xml_string()' - Write a string with XML escaping.
 */

static void
write_xml_string(cups_file_t *fp,	/* I - File to write to */
                 const char  *s)	/* I - String to write */
{
  const char	*start;			/* Start of current sequence */


  if (!s)
    return;

  for (start = s; *s; s ++)
  {
    if (*s == '&')
    {
      if (s > start)
        cupsFileWrite(fp, start, s - start);

      cupsFilePuts(fp, "&amp;");
      start = s + 1;
    }
    else if (*s == '<')
    {
      if (s > start)
        cupsFileWrite(fp, start, s - start);

      cupsFilePuts(fp, "&lt;");
      start = s + 1;
    }
  }

  if (s > start)
    cupsFilePuts(fp, start);
}


/*
 * End of "$Id: printers.c 9124 2010-04-23 20:53:38Z mike $".
 */
