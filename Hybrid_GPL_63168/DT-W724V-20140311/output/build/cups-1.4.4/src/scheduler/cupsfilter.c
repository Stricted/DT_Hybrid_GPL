/*
 * "$Id: cupsfilter.c 9061 2010-03-30 22:07:33Z mike $"
 *
 *   CUPS filtering program for CUPS.
 *
 *   Copyright 2007-2010 by Apple Inc.
 *   Copyright 1997-2006 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 * Contents:
 *
 *   main()            - Main entry for the test program.
 *   compare_pids()    - Compare two filter PIDs...
 *   escape_options()  - Convert an options array to a string.
 *   exec_filter()     - Execute a single filter.
 *   exec_filters()    - Execute filters for the given file and options.
 *   get_job_file()    - Get the specified job file.
 *   open_pipe()       - Create a pipe which is closed on exec.
 *   read_cupsd_conf() - Read the cupsd.conf file to get the filter settings.
 *   set_string()      - Copy and set a string.
 *   usage()           - Show program usage...
 */

/*
 * Include necessary headers...
 */

#include <cups/cups.h>
#include <cups/i18n.h>
#include <cups/string.h>
#include <errno.h>
#include "mime.h"
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#if defined(__APPLE__)
#  include <libgen.h>
#endif /* __APPLE__ */


/*
 * Local globals...
 */

static char		*DataDir = NULL;/* CUPS_DATADIR environment variable */
static char		*FontPath = NULL;
					/* CUPS_FONTPATH environment variable */
static mime_filter_t	GZIPFilter =	/* gziptoany filter */
{
  NULL,					/* Source type */
  NULL,					/* Destination type */
  0,					/* Cost */
  "gziptoany"				/* Filter program to run */
};
static char		*Path = NULL;	/* PATH environment variable */
static char		*ServerBin = NULL;
					/* CUPS_SERVERBIN environment variable */
static char		*ServerRoot = NULL;
					/* CUPS_SERVERROOT environment variable */
static char		*RIPCache = NULL;
					/* RIP_MAX_CACHE environment variable */
static char		TempFile[1024] = "";
					/* Temporary file */


/*
 * Local functions...
 */

static void		add_printer_filter(const char *command, mime_t *mime,
			                   mime_type_t *printer_type,
		                           const char  *filter);
static mime_type_t	*add_printer_filters(const char *command,
					     mime_t *mime, const char *printer,
			                     const char *ppdfile,
					     mime_type_t **prefilter_type);
static int		compare_pids(mime_filter_t *a, mime_filter_t *b);
static char		*escape_options(int num_options, cups_option_t *options);
static int		exec_filter(const char *filter, char **argv,
			            char **envp, int infd, int outfd);
static int		exec_filters(mime_type_t *srctype,
			             cups_array_t *filters, const char *infile,
		                     const char *outfile, const char *ppdfile,
			             const char *printer, const char *user,
				     const char *title, int num_options,
			             cups_option_t *options);
static void		get_job_file(const char *job);
static int		open_pipe(int *fds);
static int		read_cupsd_conf(const char *filename);
static void		set_string(char **s, const char *val);
static void		sighandler(int sig);
static void		usage(const char *command, const char *opt);


/*
 * 'main()' - Main entry for the test program.
 */

int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line args */
     char *argv[])			/* I - Command-line arguments */
{
  int		i;			/* Looping vars */
  const char	*command,		/* Command name */
		*opt,			/* Current option */
		*printer;		/* Printer name */
  mime_type_t	*printer_type,		/* Printer MIME type */
		*prefilter_type;	/* Printer prefilter MIME type */
  char		*srctype,		/* Source type */
		*dsttype,		/* Destination type */
		super[MIME_MAX_SUPER],	/* Super-type name */
		type[MIME_MAX_TYPE];	/* Type name */
  int		compression;		/* Compression of file */
  int		cost;			/* Cost of filters */
  mime_t	*mime;			/* MIME database */
  char		mimedir[1024];		/* MIME directory */
  char		*infile,		/* File to filter */
		*outfile;		/* File to create */
  char		cupsdconf[1024];	/* cupsd.conf file */
  const char	*server_root;		/* CUPS_SERVERROOT environment variable */
  mime_type_t	*src,			/* Source type */
		*dst;			/* Destination type */
  cups_array_t	*filters;		/* Filters for the file */
  int		num_options;		/* Number of options */
  cups_option_t	*options;		/* Options */
  const char	*ppdfile;		/* PPD file */
  const char	*title,			/* Title string */
		*user;			/* Username */
  int		all_filters,		/* Use all filters */
		removeppd,		/* Remove PPD file */
		removeinfile;		/* Remove input file */
  int		status;			/* Execution status */


 /*
  * Setup defaults...
  */

  if ((command = strrchr(argv[0], '/')) != NULL)
    command ++;
  else
    command = argv[0];

  printer      = !strcmp(command, "convert") ? "tofile" : "cupsfilter";
  mime         = NULL;
  srctype      = NULL;
  compression  = 0;
  dsttype      = "application/pdf";
  infile       = NULL;
  outfile      = NULL;
  num_options  = 0;
  options      = NULL;
  ppdfile      = NULL;
  title        = NULL;
  user         = cupsUser();
  all_filters  = 0;
  removeppd    = 0;
  removeinfile = 0;

  if ((server_root = getenv("CUPS_SERVERROOT")) == NULL)
    server_root = CUPS_SERVERROOT;

  snprintf(cupsdconf, sizeof(cupsdconf), "%s/cupsd.conf", server_root);

 /*
  * Process command-line arguments...
  */

  _cupsSetLocale(argv);

  for (i = 1; i < argc; i ++)
    if (argv[i][0] == '-')
    {
      for (opt = argv[i] + 1; *opt; opt ++)
        switch (*opt)
	{
	  case '-' : /* Next argument is a filename... */
	      i ++;
	      if (i < argc && !infile)
	        infile = argv[i];
	      else
	        usage(command, opt);
	      break;

          case 'a' : /* Specify option... */
	      i ++;
	      if (i < argc)
	        num_options = cupsParseOptions(argv[i], num_options, &options);
	      else
	        usage(command, opt);
	      break;

          case 'c' : /* Specify cupsd.conf file location... */
	      i ++;
	      if (i < argc)
	      {
	        if (!strcmp(command, "convert"))
		  num_options = cupsAddOption("copies", argv[i], num_options,
					      &options);
		else
		  strlcpy(cupsdconf, argv[i], sizeof(cupsdconf));
	      }
	      else
	        usage(command, opt);
	      break;

	  case 'D' : /* Delete input file after conversion */
	      removeinfile = 1;
	      break;

          case 'e' : /* Use every filter from the PPD file */
	      all_filters = 1;
	      break;

          case 'f' : /* Specify input file... */
	      i ++;
	      if (i < argc && !infile)
	        infile = argv[i];
	      else
	        usage(command, opt);
	      break;

          case 'i' : /* Specify source MIME type... */
	      i ++;
	      if (i < argc)
	      {
	        if (sscanf(argv[i], "%15[^/]/%255s", super, type) != 2)
		  usage(command, opt);

                srctype = argv[i];
	      }
	      else
	        usage(command, opt);
	      break;

          case 'j' : /* Get job file or specify destination MIME type... */
              if (strcmp(command, "convert"))
	      {
	        i ++;
		if (i < argc)
		{
		  get_job_file(argv[i]);
		  infile = TempFile;
		}
		else
		  usage(command, opt);

                break;
	      }

          case 'm' : /* Specify destination MIME type... */
	      i ++;
	      if (i < argc)
	      {
	        if (sscanf(argv[i], "%15[^/]/%255s", super, type) != 2)
		  usage(command, opt);

                dsttype = argv[i];
	      }
	      else
	        usage(command, opt);
	      break;

          case 'n' : /* Specify number of copies... */
	      i ++;
	      if (i < argc)
	        num_options = cupsAddOption("copies", argv[i], num_options,
		                            &options);
	      else
	        usage(command, opt);
	      break;

          case 'o' : /* Specify option(s) or output filename */
	      i ++;
	      if (i < argc)
	      {
	        if (!strcmp(command, "convert"))
		{
		  if (outfile)
		    usage(command, NULL);
		  else
		    outfile = argv[i];
		}
		else
	          num_options = cupsParseOptions(argv[i], num_options,
		                                 &options);
	      }
	      else
	        usage(command, opt);
	      break;

          case 'p' : /* Specify PPD file... */
          case 'P' : /* Specify PPD file... */
	      i ++;
	      if (i < argc)
	        ppdfile = argv[i];
	      else
	        usage(command, opt);
	      break;

          case 't' : /* Specify title... */
          case 'J' : /* Specify title... */
	      i ++;
	      if (i < argc)
	        title = argv[i];
	      else
	        usage(command, opt);
	      break;

	  case 'u' : /* Delete PPD file after conversion */
	      removeinfile = 1;
	      break;

          case 'U' : /* Specify username... */
	      i ++;
	      if (i < argc)
	        user = argv[i];
	      else
	        usage(command, opt);
	      break;

	  default : /* Something we don't understand... */
	      usage(command, opt);
	      break;
	}
    }
    else if (!infile)
    {
      if (strcmp(command, "convert"))
	infile = argv[i];
      else
      {
	_cupsLangPuts(stderr,
		      _("convert: Use the -f option to specify a file to "
		        "convert.\n"));
	usage(command, NULL);
      }
    }
    else
    {
      _cupsLangPuts(stderr,
                    _("cupsfilter: Only one filename can be specified!\n"));
      usage(command, NULL);
    }

  if (!infile && !srctype)
    usage(command, NULL);

  if (!title)
  {
    if (!infile)
      title = "(stdin)";
    else if ((title = strrchr(infile, '/')) != NULL)
      title ++;
    else
      title = infile;
  }

 /*
  * Load the cupsd.conf file and create the MIME database...
  */

  if (read_cupsd_conf(cupsdconf))
    return (1);

  snprintf(mimedir, sizeof(mimedir), "%s/mime", DataDir);

  mime = mimeLoadTypes(NULL, mimedir);
  mime = mimeLoadTypes(mime, ServerRoot);
  mime = mimeLoadFilters(mime, mimedir, Path);
  mime = mimeLoadFilters(mime, ServerRoot, Path);

  if (!mime)
  {
    _cupsLangPrintf(stderr,
                    _("%s: Unable to read MIME database from \"%s\" or "
		      "\"%s\"!\n"),
		    command, mimedir, ServerRoot);
    return (1);
  }

  if (all_filters)
  {
    printer_type = add_printer_filters(command, mime, printer, ppdfile,
				       &prefilter_type);
  }
  else
  {
    printer_type   = mimeType(mime, "application", "vnd.cups-postscript");
    prefilter_type = NULL;
  }

 /*
  * Get the source and destination types...
  */

  if (srctype)
  {
    sscanf(srctype, "%15[^/]/%255s", super, type);
    if ((src = mimeType(mime, super, type)) == NULL)
    {
      _cupsLangPrintf(stderr,
		      _("%s: Unknown source MIME type %s/%s!\n"),
		      command, super, type);
      return (1);
    }
  }
  else if ((src = mimeFileType(mime, infile, infile, &compression)) == NULL)
  {
    _cupsLangPrintf(stderr,
                    _("%s: Unable to determine MIME type of \"%s\"!\n"),
		    command, infile);
    return (1);
  }

  sscanf(dsttype, "%15[^/]/%255s", super, type);
  if (!strcasecmp(super, "printer"))
    dst = printer_type;
  else if ((dst = mimeType(mime, super, type)) == NULL)
  {
    _cupsLangPrintf(stderr,
                    _("%s: Unknown destination MIME type %s/%s!\n"),
		    command, super, type);
    return (1);
  }

 /*
  * Figure out how to filter the file...
  */

  if (src == dst)
  {
   /*
    * Special case - no filtering needed...
    */

    filters = cupsArrayNew(NULL, NULL);
    cupsArrayAdd(filters, &GZIPFilter);
    GZIPFilter.src = src;
    GZIPFilter.dst = dst;
  }
  else if ((filters = mimeFilter(mime, src, dst, &cost)) == NULL)
  {
    _cupsLangPrintf(stderr,
                    _("%s: No filter to convert from %s/%s to %s/%s!\n"),
		    command, src->super, src->type, dst->super, dst->type);
    return (1);
  }
  else if (compression)
    cupsArrayInsert(filters, &GZIPFilter);

  if (prefilter_type)
  {
   /*
    * Add pre-filters...
    */

    mime_filter_t	*filter,	/* Current filter */
			*prefilter;	/* Current pre-filter */
    cups_array_t	*prefilters = cupsArrayNew(NULL, NULL);
					/* New filters array */


    for (filter = (mime_filter_t *)cupsArrayFirst(filters);
	 filter;
	 filter = (mime_filter_t *)cupsArrayNext(filters))
    {
      if ((prefilter = mimeFilterLookup(mime, filter->src,
                                        prefilter_type)) != NULL)
	cupsArrayAdd(prefilters, prefilter);

      cupsArrayAdd(prefilters, filter);
    }

    cupsArrayDelete(filters);
    filters = prefilters;
  }

 /*
  * Do it!
  */

  status = exec_filters(src, filters, infile, outfile, ppdfile, printer, user,
                        title, num_options, options);

 /*
  * Remove files as needed, then exit...
  */

  if (TempFile[0])
    unlink(TempFile);

  if (removeppd && ppdfile)
    unlink(ppdfile);

  if (removeinfile && infile)
    unlink(infile);

  return (status);
}


/*
 * 'add_printer_filter()' - Add a single filters from a PPD file.
 */

static void
add_printer_filter(
    const char  *command,		/* I - Command name */
    mime_t      *mime,			/* I - MIME database */
    mime_type_t *filtertype,		/* I - Printer or prefilter MIME type */
    const char  *filter)		/* I - Filter to add */
{
  char		super[MIME_MAX_SUPER],	/* Super-type for filter */
		type[MIME_MAX_TYPE],	/* Type for filter */
		program[1024];		/* Program/filter name */
  int		cost;			/* Cost of filter */
  mime_type_t	*temptype;		/* MIME type looping var */
  char		filename[1024];		/* Full filter filename */


 /*
  * Parse the filter string; it should be in the following format:
  *
  *     super/type cost program
  */

  if (sscanf(filter, "%15[^/]/%31s%d%*[ \t]%1023[^\n]", super, type, &cost,
             program) != 4)
  {
    _cupsLangPrintf(stderr, _("%s: Invalid filter string \"%s\"\n"), command,
                    filter);
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

    if (access(filename, X_OK))
    {
      _cupsLangPrintf(stderr, _("%s: Filter \"%s\" not available: %s\n"),
                      command, program, strerror(errno));
      return;
    }
  }

 /*
  * Add the filter to the MIME database, supporting wildcards as needed...
  */

  for (temptype = mimeFirstType(mime);
       temptype;
       temptype = mimeNextType(mime))
    if (((super[0] == '*' && strcasecmp(temptype->super, "printer")) ||
         !strcasecmp(temptype->super, super)) &&
        (type[0] == '*' || !strcasecmp(temptype->type, type)))
      mimeAddFilter(mime, temptype, filtertype, cost, program);
}


/*
 * 'add_printer_filters()' - Add filters from a PPD file.
 */

static mime_type_t *			/* O - Printer type or NULL on error */
add_printer_filters(
    const char  *command,		/* I - Command name */
    mime_t      *mime,			/* I - MIME database */
    const char  *printer,		/* I - Printer name */
    const char  *ppdfile,		/* I - PPD file */
    mime_type_t **prefilter_type)	/* O - Prefilter type */
{
  int	      i;			/* Looping var */
  mime_type_t *printer_type;		/* Printer MIME type */
  ppd_file_t  *ppd;			/* PPD file data */
  ppd_attr_t  *ppdattr;			/* Current prefilter */


  *prefilter_type = NULL;

  if ((ppd = ppdOpenFile(ppdfile)) == NULL)
  {
    ppd_status_t  status;		/* PPD load status */

    status = ppdLastError(&i);
    _cupsLangPrintf(stderr, _("%s: Unable to open PPD file: %s on line %d\n"),
                    command, ppdErrorString(status), i);
    return (NULL);
  }

  printer_type = mimeAddType(mime, "printer", printer);

  if (ppd->num_filters > 0)
  {
    for (i = 0; i < ppd->num_filters; i ++)
      add_printer_filter(command, mime, printer_type, ppd->filters[i]);
  }
  else
  {
    add_printer_filter(command, mime, printer_type,
                       "application/vnd.cups-command 0 commandtops");
    add_printer_filter(command, mime, printer_type,
                       "application/vnd.cups-postscript 0 -");
  }

  if ((ppdattr = ppdFindAttr(ppd, "cupsPreFilter", NULL)) != NULL)
  {
    *prefilter_type = mimeAddType(mime, "prefilter", printer);

    for (; ppdattr; ppdattr = ppdFindNextAttr(ppd, "cupsPreFilter", NULL))
      if (ppdattr->value)
	add_printer_filter(command, mime, *prefilter_type, ppdattr->value);
  }
  else
    *prefilter_type = NULL;

  return (printer_type);
}


/*
 * 'compare_pids()' - Compare two filter PIDs...
 */

static int				/* O - Result of comparison */
compare_pids(mime_filter_t *a,		/* I - First filter */
             mime_filter_t *b)		/* I - Second filter */
{
 /*
  * Because we're particularly lazy, we store the process ID in the "cost"
  * variable...
  */

  return (a->cost - b->cost);
}


/*
 * 'escape_options()' - Convert an options array to a string.
 */

static char *				/* O - Option string */
escape_options(
    int           num_options,		/* I - Number of options */
    cups_option_t *options)		/* I - Options */
{
  int		i;			/* Looping var */
  cups_option_t	*option;		/* Current option */
  int		bytes;			/* Number of bytes needed */
  char		*s,			/* Option string */
		*sptr,			/* Pointer into string */
		*vptr;			/* Pointer into value */


 /*
  * Figure out the worst-case number of bytes we need for the option string.
  */

  for (i = num_options, option = options, bytes = 1; i > 0; i --, option ++)
    bytes += 2 * (strlen(option->name) + strlen(option->value)) + 2;

  if ((s = malloc(bytes)) == NULL)
    return (NULL);

 /*
  * Copy the options to the string...
  */

  for (i = num_options, option = options, sptr = s; i > 0; i --, option ++)
  {
    if (!strcmp(option->name, "copies"))
      continue;

    if (sptr > s)
      *sptr++ = ' ';

    strcpy(sptr, option->name);
    sptr += strlen(sptr);
    *sptr++ = '=';

    for (vptr = option->value; *vptr;)
    {
      if (strchr("\\ \t\n", *vptr))
        *sptr++ = '\\';

      *sptr++ = *vptr++;
    }
  }

  *sptr = '\0';

  return (s);
}


/*
 * 'exec_filter()' - Execute a single filter.
 */

static int				/* O - Process ID or -1 on error */
exec_filter(const char *filter,		/* I - Filter to execute */
            char       **argv,		/* I - Argument list */
	    char       **envp,		/* I - Environment list */
	    int        infd,		/* I - Stdin file descriptor */
	    int        outfd)		/* I - Stdout file descriptor */
{
  int		pid,			/* Process ID */
		fd;			/* Temporary file descriptor */
#if defined(__APPLE__)
  char		processPath[1024],	/* CFProcessPath environment variable */
		linkpath[1024];		/* Link path for symlinks... */
  int		linkbytes;		/* Bytes for link path */


 /*
  * Add special voodoo magic for MacOS X - this allows MacOS X
  * programs to access their bundle resources properly...
  */

  if ((linkbytes = readlink(filter, linkpath, sizeof(linkpath) - 1)) > 0)
  {
   /*
    * Yes, this is a symlink to the actual program, nul-terminate and
    * use it...
    */

    linkpath[linkbytes] = '\0';

    if (linkpath[0] == '/')
      snprintf(processPath, sizeof(processPath), "CFProcessPath=%s",
	       linkpath);
    else
      snprintf(processPath, sizeof(processPath), "CFProcessPath=%s/%s",
	       dirname((char *)filter), linkpath);
  }
  else
    snprintf(processPath, sizeof(processPath), "CFProcessPath=%s", filter);

  envp[0] = processPath;		/* Replace <CFProcessPath> string */
#endif	/* __APPLE__ */

  if ((pid = fork()) == 0)
  {
   /*
    * Child process goes here...
    *
    * Update stdin/stdout/stderr as needed...
    */

    if (infd != 0)
    {
      if (infd < 0)
        infd = open("/dev/null", O_RDONLY);

      if (infd > 0)
      {
        dup2(infd, 0);
	close(infd);
      }
    }

    if (outfd != 1)
    {
      if (outfd < 0)
        outfd = open("/dev/null", O_WRONLY);

      if (outfd > 1)
      {
	dup2(outfd, 1);
	close(outfd);
      }
    }

    if ((fd = open("/dev/null", O_RDWR)) > 3)
    {
      dup2(fd, 3);
      close(fd);
    }
    fcntl(3, F_SETFL, O_NDELAY);

    if ((fd = open("/dev/null", O_RDWR)) > 4)
    {
      dup2(fd, 4);
      close(fd);
    }
    fcntl(4, F_SETFL, O_NDELAY);

   /*
    * Execute command...
    */

    execve(filter, argv, envp);

    perror(filter);

    exit(errno);
  }

  return (pid);
}


/*
 * 'exec_filters()' - Execute filters for the given file and options.
 */

static int				/* O - 0 on success, 1 on error */
exec_filters(mime_type_t   *srctype,	/* I - Source type */
             cups_array_t  *filters,	/* I - Array of filters to run */
             const char    *infile,	/* I - File to filter */
	     const char    *outfile,	/* I - File to create */
	     const char    *ppdfile,	/* I - PPD file, if any */
	     const char    *printer,	/* I - Printer name */
	     const char    *user,	/* I - Username */
	     const char    *title,	/* I - Job title */
             int           num_options,	/* I - Number of filter options */
	     cups_option_t *options)	/* I - Filter options */
{
  int		i;			/* Looping var */
  const char	*argv[8],		/* Command-line arguments */
		*envp[12],		/* Environment variables */
		*temp;			/* Temporary string */
  char		*optstr,		/* Filter options */
		content_type[1024],	/* CONTENT_TYPE */
		cups_datadir[1024],	/* CUPS_DATADIR */
		cups_fontpath[1024],	/* CUPS_FONTPATH */
		cups_serverbin[1024],	/* CUPS_SERVERBIN */
		cups_serverroot[1024],	/* CUPS_SERVERROOT */
		lang[1024],		/* LANG */
		path[1024],		/* PATH */
		ppd[1024],		/* PPD */
		rip_max_cache[1024],	/* RIP_MAX_CACHE */
		userenv[1024],		/* USER */
		program[1024];		/* Program to run */
  mime_filter_t	*filter,		/* Current filter */
		*next;			/* Next filter */
  int		current,		/* Current filter */
		filterfds[2][2],	/* Pipes for filters */
		pid,			/* Process ID of filter */
		status,			/* Exit status */
		retval;			/* Return value */
  cups_array_t	*pids;			/* Executed filters array */
  mime_filter_t	key;			/* Search key for filters */
  cups_lang_t	*language;		/* Current language */


 /*
  * Setup the filter environment and command-line...
  */

  optstr = escape_options(num_options, options);

  snprintf(content_type, sizeof(content_type), "CONTENT_TYPE=%s/%s",
           srctype->super, srctype->type);
  snprintf(cups_datadir, sizeof(cups_datadir), "CUPS_DATADIR=%s", DataDir);
  snprintf(cups_fontpath, sizeof(cups_fontpath), "CUPS_FONTPATH=%s", FontPath);
  snprintf(cups_serverbin, sizeof(cups_serverbin), "CUPS_SERVERBIN=%s",
           ServerBin);
  snprintf(cups_serverroot, sizeof(cups_serverroot), "CUPS_SERVERROOT=%s",
           ServerRoot);
  language = cupsLangDefault();
  snprintf(lang, sizeof(lang), "LANG=%s.UTF8", language->language);
  snprintf(path, sizeof(path), "PATH=%s", Path);
  if (ppdfile)
    snprintf(ppd, sizeof(ppd), "PPD=%s", ppdfile);
  else if ((temp = getenv("PPD")) != NULL)
    snprintf(ppd, sizeof(ppd), "PPD=%s", temp);
  else
#ifdef __APPLE__
  if (!access("/System/Library/Frameworks/ApplicationServices.framework/"
	      "Versions/A/Frameworks/PrintCore.framework/Versions/A/"
	      "Resources/English.lproj/Generic.ppd", 0))
    strlcpy(ppd, "PPD=/System/Library/Frameworks/ApplicationServices.framework/"
                 "Versions/A/Frameworks/PrintCore.framework/Versions/A/"
		 "Resources/English.lproj/Generic.ppd", sizeof(ppd));
  else
    strlcpy(ppd, "PPD=/System/Library/Frameworks/ApplicationServices.framework/"
                 "Versions/A/Frameworks/PrintCore.framework/Versions/A/"
		 "Resources/Generic.ppd", sizeof(ppd));
#else
    snprintf(ppd, sizeof(ppd), "PPD=%s/model/laserjet.ppd", DataDir);
#endif /* __APPLE__ */
  snprintf(rip_max_cache, sizeof(rip_max_cache), "RIP_MAX_CACHE=%s", RIPCache);
  snprintf(userenv, sizeof(userenv), "USER=%s", user);

  argv[0] = (char *)printer;
  argv[1] = "1";
  argv[2] = user;
  argv[3] = title;
  argv[4] = cupsGetOption("copies", num_options, options);
  argv[5] = optstr;
  argv[6] = infile;
  argv[7] = NULL;

  if (!argv[4])
    argv[4] = "1";

  envp[0]  = "<CFProcessPath>";
  envp[1]  = content_type;
  envp[2]  = cups_datadir;
  envp[3]  = cups_fontpath;
  envp[4]  = cups_serverbin;
  envp[5]  = cups_serverroot;
  envp[6]  = lang;
  envp[7]  = path;
  envp[8]  = ppd;
  envp[9]  = rip_max_cache;
  envp[10] = userenv;
  envp[11] = NULL;

  for (i = 0; argv[i]; i ++)
    fprintf(stderr, "DEBUG: argv[%d]=\"%s\"\n", i, argv[i]);

  for (i = 0; envp[i]; i ++)
    fprintf(stderr, "DEBUG: envp[%d]=\"%s\"\n", i, envp[i]);

 /*
  * Execute all of the filters...
  */

  pids            = cupsArrayNew((cups_array_func_t)compare_pids, NULL);
  current         = 0;
  filterfds[0][0] = -1;
  filterfds[0][1] = -1;
  filterfds[1][0] = -1;
  filterfds[1][1] = -1;

  if (!infile)
    filterfds[0][0] = 0;

  for (filter = (mime_filter_t *)cupsArrayFirst(filters);
       filter;
       filter = next, current = 1 - current)
  {
    next = (mime_filter_t *)cupsArrayNext(filters);

    if (filter->filter[0] == '/')
      strlcpy(program, filter->filter, sizeof(program));
    else
      snprintf(program, sizeof(program), "%s/filter/%s", ServerBin,
	       filter->filter);

    if (filterfds[!current][1] > 1)
    {
      close(filterfds[1 - current][0]);
      close(filterfds[1 - current][1]);

      filterfds[1 - current][0] = -1;
      filterfds[1 - current][0] = -1;
    }

    if (next)
      open_pipe(filterfds[1 - current]);
    else if (outfile)
    {
      filterfds[1 - current][1] = open(outfile, O_CREAT | O_TRUNC | O_WRONLY,
                                       0666);

      if (filterfds[1 - current][1] < 0)
        fprintf(stderr, "ERROR: Unable to create \"%s\" - %s\n", outfile,
	        strerror(errno));
    }
    else
      filterfds[1 - current][1] = 1;

    pid = exec_filter(program, (char **)argv, (char **)envp,
                      filterfds[current][0], filterfds[1 - current][1]);

    if (pid > 0)
    {
      fprintf(stderr, "INFO: %s (PID %d) started.\n", filter->filter, pid);

      filter->cost = pid;
      cupsArrayAdd(pids, filter);
    }
    else
      break;

    argv[6] = NULL;
  }

 /*
  * Close remaining pipes...
  */

  if (filterfds[0][1] > 1)
  {
    close(filterfds[0][0]);
    close(filterfds[0][1]);
  }

  if (filterfds[1][1] > 1)
  {
    close(filterfds[1][0]);
    close(filterfds[1][1]);
  }

 /*
  * Wait for the children to exit...
  */

  retval = 0;

  while (cupsArrayCount(pids) > 0)
  {
    if ((pid = wait(&status)) < 0)
      continue;

    key.cost = pid;
    if ((filter = (mime_filter_t *)cupsArrayFind(pids, &key)) != NULL)
    {
      cupsArrayRemove(pids, filter);

      if (status)
      {
	if (WIFEXITED(status))
	  fprintf(stderr, "ERROR: %s (PID %d) stopped with status %d!\n",
		  filter->filter, pid, WEXITSTATUS(status));
	else
	  fprintf(stderr, "ERROR: %s (PID %d) crashed on signal %d!\n",
		  filter->filter, pid, WTERMSIG(status));

        retval = 1;
      }
      else
        fprintf(stderr, "INFO: %s (PID %d) exited with no errors.\n",
	        filter->filter, pid);
    }
  }

  cupsArrayDelete(pids);

  return (retval);
}


/*
 * 'get_job_file()' - Get the specified job file.
 */

static void
get_job_file(const char *job)		/* I - Job ID */
{
  long		jobid,			/* Job ID */
		docnum;			/* Document number */
  const char	*jobptr;		/* Pointer into job ID string */
  char		uri[1024];		/* job-uri */
  http_t	*http;			/* Connection to server */
  ipp_t		*request;		/* Request data */
  int		tempfd;			/* Temporary file */


 /*
  * Get the job ID and document number, if any...
  */

  if ((jobptr = strrchr(job, '-')) != NULL)
    jobptr ++;
  else
    jobptr = job;

  jobid = strtol(jobptr, (char **)&jobptr, 10);

  if (*jobptr == ',')
    docnum = strtol(jobptr + 1, NULL, 10);
  else
    docnum = 1;

  if (jobid < 1 || jobid > INT_MAX)
  {
    _cupsLangPrintf(stderr, _("cupsfilter: Invalid job ID %d!\n"), (int)jobid);
    exit(1);
  }

  if (docnum < 1 || docnum > INT_MAX)
  {
    _cupsLangPrintf(stderr, _("cupsfilter: Invalid document number %d!\n"),
                    (int)docnum);
    exit(1);
  }

 /*
  * Ask the server for the document file...
  */

  if ((http = httpConnectEncrypt(cupsServer(), ippPort(),
                                 cupsEncryption())) == NULL)
  {
    _cupsLangPrintf(stderr, _("%s: Unable to connect to server\n"),
                    "cupsfilter");
    exit(1);
  }

  request = ippNewRequest(CUPS_GET_DOCUMENT);

  snprintf(uri, sizeof(uri), "ipp://localhost/jobs/%d", (int)jobid);

  ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "job-uri", NULL, uri);
  ippAddInteger(request, IPP_TAG_OPERATION, IPP_TAG_INTEGER, "document-number",
                (int)docnum);

  if ((tempfd = cupsTempFd(TempFile, sizeof(TempFile))) == -1)
  {
    _cupsLangPrintError(_("ERROR: Unable to create temporary file"));
    httpClose(http);
    exit(1);
  }

  signal(SIGTERM, sighandler);

  ippDelete(cupsDoIORequest(http, request, "/", -1, tempfd));

  close(tempfd);

  httpClose(http);

  if (cupsLastError() != IPP_OK)
  {
    _cupsLangPrintf(stderr, _("cupsfilter: Unable to get job file - %s\n"),
                    cupsLastErrorString());
    unlink(TempFile);
    exit(1);
  }
}


/*
 * 'open_pipe()' - Create a pipe which is closed on exec.
 */

static int				/* O - 0 on success, -1 on error */
open_pipe(int *fds)			/* O - Pipe file descriptors (2) */
{
 /*
  * Create the pipe...
  */

  if (pipe(fds))
  {
    fds[0] = -1;
    fds[1] = -1;

    return (-1);
  }

 /*
  * Set the "close on exec" flag on each end of the pipe...
  */

  if (fcntl(fds[0], F_SETFD, fcntl(fds[0], F_GETFD) | FD_CLOEXEC))
  {
    close(fds[0]);
    close(fds[1]);

    fds[0] = -1;
    fds[1] = -1;

    return (-1);
  }

  if (fcntl(fds[1], F_SETFD, fcntl(fds[1], F_GETFD) | FD_CLOEXEC))
  {
    close(fds[0]);
    close(fds[1]);

    fds[0] = -1;
    fds[1] = -1;

    return (-1);
  }

 /*
  * Return 0 indicating success...
  */

  return (0);
}


/*
 * 'read_cupsd_conf()' - Read the cupsd.conf file to get the filter settings.
 */

static int				/* O - 0 on success, 1 on error */
read_cupsd_conf(const char *filename)	/* I - File to read */
{
  cups_file_t	*fp;			/* cupsd.conf file */
  const char	*temp;			/* Temporary string */
  char		line[1024],		/* Line from file */
		*ptr;			/* Pointer into line */
  int		linenum;		/* Current line number */


  if ((temp = getenv("CUPS_DATADIR")) != NULL)
    set_string(&DataDir, temp);
  else
    set_string(&DataDir, CUPS_DATADIR);

  if ((temp = getenv("CUPS_FONTPATH")) != NULL)
    set_string(&FontPath, temp);
  else
    set_string(&FontPath, CUPS_FONTPATH);

  set_string(&RIPCache, "8m");

  if ((temp = getenv("CUPS_SERVERBIN")) != NULL)
    set_string(&ServerBin, temp);
  else
    set_string(&ServerBin, CUPS_SERVERBIN);

  strlcpy(line, filename, sizeof(line));
  if ((ptr = strrchr(line, '/')) != NULL)
    *ptr = '\0';
  else
    getcwd(line, sizeof(line));

  set_string(&ServerRoot, line);

  if ((fp = cupsFileOpen(filename, "r")) != NULL)
  {
    linenum = 0;

    while (cupsFileGetConf(fp, line, sizeof(line), &ptr, &linenum))
    {
      if (!strcasecmp(line, "DataDir"))
        set_string(&DataDir, ptr);
      else if (!strcasecmp(line, "FontPath"))
        set_string(&FontPath, ptr);
      else if (!strcasecmp(line, "RIPCache"))
        set_string(&RIPCache, ptr);
      else if (!strcasecmp(line, "ServerBin"))
        set_string(&ServerBin, ptr);
      else if (!strcasecmp(line, "ServerRoot"))
        set_string(&ServerRoot, ptr);
    }

    cupsFileClose(fp);
  }

  snprintf(line, sizeof(line),
           "%s/filter:" CUPS_BINDIR ":" CUPS_SBINDIR ":/bin:/usr/bin",
	   ServerBin);
  set_string(&Path, line);

  return (0);
}


/*
 * 'set_string()' - Copy and set a string.
 */

static void
set_string(char       **s,		/* O - Copy of string */
           const char *val)		/* I - String to copy */
{
  if (*s)
    free(*s);

  *s = strdup(val);
}


/*
 * 'sighandler()' - Signal catcher for when we print from stdin...
 */

static void
sighandler(int s)			/* I - Signal number */
{
 /*
  * Remove the temporary file we're using to print a job file...
  */

  if (TempFile[0])
    unlink(TempFile);

 /*
  * Exit...
  */

  exit(s);
}


/*
 * 'usage()' - Show program usage...
 */

static void
usage(const char *command,		/* I - Command name */
      const char *opt)			/* I - Incorrect option, if any */
{
  if (opt)
    _cupsLangPrintf(stderr, _("%s: Unknown option '%c'!\n"), command, *opt);

  if (!strcmp(command, "cupsfilter"))
    _cupsLangPuts(stdout,
		  _("Usage: cupsfilter -m mime/type [ options ] filename\n"
		    "\n"
		    "Options:\n"
		    "\n"
		    "  -c cupsd.conf    Set cupsd.conf file to use\n"
		    "  -e               Use every filter from the PPD file\n"
		    "  -j job-id[,N]    Filter file N from the specified job (default is file 1)\n"
		    "  -n copies        Set number of copies\n"
		    "  -o name=value    Set option(s)\n"
		    "  -p filename.ppd  Set PPD file\n"
		    "  -t title         Set title\n"));
  else
    _cupsLangPuts(stdout,
		  _("Usage: convert [ options ]\n"
		    "\n"
		    "Options:\n"
		    "\n"
		    "  -e                   Use every filter from the PPD file\n"
		    "  -f filename          Set file to be converted (otherwise stdin)\n"
		    "  -o filename          Set file to be generated (otherwise stdout)\n"
		    "  -i mime/type         Set input MIME type (otherwise auto-typed)\n"
		    "  -j mime/type         Set output MIME type (otherwise application/pdf)\n"
		    "  -P filename.ppd      Set PPD file\n"
		    "  -a 'name=value ...'  Set option(s)\n"
		    "  -U username          Set username for job\n"
		    "  -J title             Set title\n"
		    "  -c copies            Set number of copies\n"
		    "  -u                   Remove the PPD file when finished\n"
		    "  -D                   Remove the input file when finished\n"));

  exit(1);
}


/*
 * End of "$Id: cupsfilter.c 9061 2010-03-30 22:07:33Z mike $".
 */
