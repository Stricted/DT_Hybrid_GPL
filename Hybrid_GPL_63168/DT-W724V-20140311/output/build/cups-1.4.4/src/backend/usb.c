/*
 * "$Id: usb.c 7687 2008-06-24 01:28:36Z mike $"
 *
 *   USB port backend for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 2007-2008 by Apple Inc.
 *   Copyright 1997-2007 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   "LICENSE" which should have been included with this file.  If this
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   list_devices() - List all available USB devices to stdout.
 *   print_device() - Print a file to a USB device.
 *   main()         - Send a file to the specified USB port.
 */

/*
 * Include necessary headers.
 */

#ifdef __APPLE__
   /* A header order dependency requires this be first */
#  include <ApplicationServices/ApplicationServices.h>
#endif /* __APPLE__ */

#include "backend-private.h"

#ifdef WIN32
#  include <io.h>
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <termios.h>
#endif /* WIN32 */


/*
 * Local functions...
 */

void	list_devices(void);
int	print_device(const char *uri, const char *hostname,
	             const char *resource, char *options,
		     int print_fd, int copies, int argc, char *argv[]);


/*
 * Include the vendor-specific USB implementation...
 */

#ifdef HAVE_USB_H
#  include "usb-libusb.c"
#elif defined(__APPLE__)
#  include "usb-darwin.c"
#elif defined(__linux) || defined(__sun) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  include "usb-unix.c"
#else
/*
 * Use dummy functions that do nothing on unsupported platforms...
 * These can be used as templates for implementing USB printing on new
 * platforms...
 */

/*
 * 'list_devices()' - List all available USB devices to stdout.
 */

void
list_devices(void)
{
 /*
  * Don't have any devices to list... Use output of the form:
  *
  *     direct usb:/make/model?serial=foo "Make Model" "USB Printer"
  *
  * Note that "Hewlett Packard" or any other variation MUST be mapped to
  * "HP" for compatibility with the PPD and ICC specs.
  */
}


/*
 * 'print_device()' - Print a file to a USB device.
 */

int					/* O - Exit status */
print_device(const char *uri,		/* I - Device URI */
             const char *hostname,	/* I - Hostname/manufacturer */
             const char *resource,	/* I - Resource/modelname */
	     char       *options,	/* I - Device options/serial number */
	     int        print_fd,	/* I - File descriptor to print */
	     int        copies,		/* I - Copies to print */
	     int	argc,		/* I - Number of command-line arguments (6 or 7) */
	     char	*argv[])	/* I - Command-line arguments */
{
 /*
  * Can't print, so just reference the arguments to eliminate compiler
  * warnings and return and exit status of 1.  Normally you would use the
  * arguments to send a file to the printer and return 0 if everything
  * worked OK and non-zero if there was an error.
  */

  (void)uri;
  (void)hostname;
  (void)resource;
  (void)options;
  (void)print_fd;
  (void)copies;
  (void)argc;
  (void)argv;
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  return (CUPS_BACKEND_FAILED);
}
#endif /* __APPLE__ */


/*
 * 'main()' - Send a file to the specified USB port.
 *
 * Usage:
 *
 *    printer-uri job-id user title copies options [file]
 */

int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line arguments (6 or 7) */
     char *argv[])			/* I - Command-line arguments */
{
  int		print_fd;		/* Print file */
  int		copies;			/* Number of copies to print */
  int		status;			/* Exit status */
  int		port;			/* Port number (not used) */
  const char	*uri;			/* Device URI */
  char		method[255],		/* Method in URI */
		hostname[1024],		/* Hostname */
		username[255],		/* Username info (not used) */
		resource[1024],		/* Resource info (device and options) */
		*options;		/* Pointer to options */
#if defined(HAVE_SIGACTION) && !defined(HAVE_SIGSET)
  struct sigaction action;		/* Actions for POSIX signals */
#endif /* HAVE_SIGACTION && !HAVE_SIGSET */
int i = 0;
int pid=getpid();
 /*
  * Make sure status messages are not buffered...
  */
//printf("==file:%s,line:%d,func:%s=pid=%d,argc=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid,argc);
  setbuf(stderr, NULL);

 /*
  * Ignore SIGPIPE signals...
  */

#ifdef HAVE_SIGSET
  sigset(SIGPIPE, SIG_IGN);
#elif defined(HAVE_SIGACTION)
  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &action, NULL);
#else
  signal(SIGPIPE, SIG_IGN);
#endif /* HAVE_SIGSET */

 /*
  * Check command-line...
  */

  if (argc == 1)
  {
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    list_devices();
    return (CUPS_BACKEND_OK);
  }
  else if (argc < 6 || argc > 7)
  {
  #ifdef NOT_CUT
   _cupsLangPrintf(stderr,
                   _("Usage: %s job-id user title copies options [file]\n"),
                    argv[0]);
  #endif
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    return (CUPS_BACKEND_FAILED);
  }
for(i=0;i<argc;i++)
{
    //printf("\n==file:%s,line:%d,func:%s=argv[%d]=%s=\n",__FILE__,__LINE__,__FUNCTION__,i,argv[i]);
}
 /*
  * Extract the device name and options from the URI...
  */
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  uri = cupsBackendDeviceURI(argv);
//printf("==file:%s,line:%d,func:%s=pid=%d,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,uri);
  if (httpSeparateURI(HTTP_URI_CODING_ALL, uri,
                      method, sizeof(method), username, sizeof(username),
		      hostname, sizeof(hostname), &port,
		      resource, sizeof(resource)) < HTTP_URI_OK)
  {
 #ifdef NOT_CUT 
    _cupsLangPuts(stderr,
                  _("ERROR: No device URI found in argv[0] or in DEVICE_URI "
	            "environment variable!\n"));
 #endif
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);	            
    return (1);
  }
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
 /*
  * See if there are any options...
  */

  if ((options = strchr(resource, '?')) != NULL)
  {
   /*
    * Yup, terminate the device name string and move to the first
    * character of the options...
    */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    *options++ = '\0';
  }

 /*
  * If we have 7 arguments, print the file named on the command-line.
  * Otherwise, send stdin instead...
  */

  if (argc == 6)
  {
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    print_fd = 0;
    copies   = 1;
  }
  else
  {
   /*
    * Try to open the print file...
    */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    if ((print_fd = open(argv[6], O_RDONLY)) < 0)
    {
//      _cupsLangPrintf(stderr, _("ERROR: Unable to open print file %s - %s\n"),
//                      argv[6], strerror(errno));
      //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
      return (CUPS_BACKEND_FAILED);
    }

    copies = atoi(argv[4]);
  }

 /*
  * Finally, send the print file...
  */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  status = print_device(uri, hostname, resource, options, print_fd, copies,
                        argc, argv);

 /*
  * Close the input file and return...
  */
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  if (print_fd != 0)
    close(print_fd);
//printf("==file:%s,line:%d,func:%s=status=%d=\n",__FILE__,__LINE__,__FUNCTION__,status);
  return (status);
}


/*
 * End of "$Id: usb.c 7687 2008-06-24 01:28:36Z mike $".
 */
