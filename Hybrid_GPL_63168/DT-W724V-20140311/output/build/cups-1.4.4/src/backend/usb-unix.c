/*
 * "$Id: usb-unix.c 8912 2009-12-08 02:13:42Z mike $"
 *
 *   USB port backend for the Common UNIX Printing System (CUPS).
 *
 *   This file is included from "usb.c" when compiled on UNIX/Linux.
 *
 *   Copyright 2007-2009 by Apple Inc.
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
 *   print_device() - Print a file to a USB device.
 *   list_devices() - List all USB devices.
 *   open_device()  - Open a USB device...
 *   side_cb()      - Handle side-channel requests...
 */

/*
 * Include necessary headers.
 */

#include <sys/select.h>


/*
 * Local functions...
 */

static int	open_device(const char *uri, int *use_bc);
static int	side_cb(int print_fd, int device_fd, int snmp_fd,
		        http_addr_t *addr, int use_bc);


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
  int		use_bc;			/* Use backchannel path? */
  int		device_fd;		/* USB device */
  size_t	tbytes;			/* Total number of bytes written */
  struct termios opts;			/* Parallel port options */

int curpid=getpid();
  (void)argc;
  (void)argv;
//printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
 /*
  * Open the USB port device...
  */

  fputs("STATE: +connecting-to-device\n", stderr);

  do
  {
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
   /*
    * *BSD's ulpt driver currently does not support the
    * back-channel, incorrectly returns data ready on a select(),
    * and locks up on read()...
    */

    use_bc = 0;

#elif defined(__sun)
   /*
    * CUPS STR #3028: Solaris' usbprn driver apparently does not support
    * select() or poll(), so we can't support backchannel...
    */

    use_bc = 0;

#else
   /*
    * Disable backchannel data when printing to Brother, Canon, or
    * Minolta USB printers - apparently these printers will return
    * the IEEE-1284 device ID over and over and over when they get
    * a read request...
    */
    //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
    use_bc = strcasecmp(hostname, "Brother") &&
             strcasecmp(hostname, "Canon") &&
             strncasecmp(hostname, "Konica", 6) &&
             strncasecmp(hostname, "Minolta", 7);
#endif /* __FreeBSD__ || __NetBSD__ || __OpenBSD__ || __DragonFly__ */
//printf("==file:%s,line:%d,func:%s=uri=%s,use_bc=%d=\n",__FILE__,__LINE__,__FUNCTION__,uri,use_bc);
    if ((device_fd = open_device(uri, &use_bc)) == -1)
    {
     //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
      if (getenv("CLASS") != NULL)
      {
       /*
        * If the CLASS environment variable is set, the job was submitted
	* to a class and not to a specific queue.  In this case, we want
	* to abort immediately so that the job can be requeued on the next
	* available printer in the class.
	*/
        //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
     #ifdef NOT_CUT
        _cupsLangPuts(stderr,
	              _("INFO: Unable to contact printer, queuing on next "
			"printer in class...\n"));
     #endif
       /*
        * Sleep 5 seconds to keep the job from requeuing too rapidly...
	*/

	sleep(5);

        return (CUPS_BACKEND_FAILED);
      }
//printf("==file:%s,line:%d,func:%s=pid=%d=errno=%d=\n",__FILE__,__LINE__,__FUNCTION__,getpid(),errno);
#ifdef NOT_CUT

      if (errno == EBUSY)
      {
        _cupsLangPuts(stderr,
	              _("INFO: Printer busy; will retry in 10 seconds...\n"));
	sleep(10);
      }
      else if (errno == ENXIO || errno == EIO || errno == ENOENT ||
               errno == ENODEV)
      {
        //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
       _cupsLangPuts(stderr,("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid));
        _cupsLangPuts(stderr,
	              _("INFO: Printer not connected; will retry in 30 "
		        "seconds...\n"));
	sleep(30);
      }
      else
      {
        //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
//	_cupsLangPrintf(stderr,
//	                _("ERROR: Unable to open device file \"%s\": %s\n"),
			resource, strerror(errno));
	return (CUPS_BACKEND_FAILED);
      }
#endif
    }
    //printf("==file:%s,line:%d,func:%s=uri=%s,use_bc=%d=\n",__FILE__,__LINE__,__FUNCTION__,uri,use_bc);
  }
  while (device_fd < 0);
//printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
  fputs("STATE: -connecting-to-device\n", stderr);

 /*
  * Set any options provided...
  */

  tcgetattr(device_fd, &opts);

  opts.c_lflag &= ~(ICANON | ECHO | ISIG);	/* Raw mode */

  /**** No options supported yet ****/

  tcsetattr(device_fd, TCSANOW, &opts);

 /*
  * Finally, send the print file...
  */

  tbytes = 0;

  while (copies > 0 && tbytes >= 0)
  {
    copies --;

    if (print_fd != 0)
    {
      fputs("PAGE: 1 1\n", stderr);
      lseek(print_fd, 0, SEEK_SET);
    }

#ifdef __sun
   /*
    * CUPS STR #3028: Solaris' usbprn driver apparently does not support
    * select() or poll(), so we can't support the sidechannel either...
    */
    //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
    tbytes = backendRunLoop(print_fd, device_fd, -1, NULL, use_bc, 1, NULL);

#else
    //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
    tbytes = backendRunLoop(print_fd, device_fd, -1, NULL, use_bc, 1, side_cb);
#endif /* __sun */
    //printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
 #ifdef NOT_CUT  
    if (print_fd != 0 && tbytes >= 0)
     _cupsLangPrintf(stderr,
#ifdef HAVE_LONG_LONG
		      _("INFO: Sent print file, %lld bytes...\n"),
#else
		      _("INFO: Sent print file, %ld bytes...\n"),
#endif /* HAVE_LONG_LONG */
 	      CUPS_LLCAST tbytes);
  #endif
  }

 /*
  * Close the USB port and return...
  */
//printf("==file:%s,line:%d,func:%s=curpid=%d=\n",__FILE__,__LINE__,__FUNCTION__,curpid);
  close(device_fd);

  return (tbytes < 0 ? CUPS_BACKEND_FAILED : CUPS_BACKEND_OK);
}


/*
 * 'list_devices()' - List all USB devices.
 */

void
list_devices(void)
{
#ifdef __linux
  int	i;				/* Looping var */
  int	fd;				/* File descriptor */
  char	device[255],			/* Device filename */
	device_id[1024],		/* Device ID string */
	device_uri[1024],		/* Device URI string */
	make_model[1024];		/* Make and model */


 /*
  * Try to open each USB device...
  */

  for (i = 0; i < 16; i ++)
  {
   /*
    * Linux has a long history of changing the standard filenames used
    * for USB printer devices.  We get the honor of trying them all...
    */

    sprintf(device, "/dev/usblp%d", i);

    if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
    {
      if (errno != ENOENT)
	continue;

      sprintf(device, "/dev/usb/lp%d", i);

      if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
      {
	if (errno != ENOENT)
	  continue;

	sprintf(device, "/dev/usb/usblp%d", i);

    	if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
	  continue;
      }
    }

    if (!backendGetDeviceID(fd, device_id, sizeof(device_id),
                            make_model, sizeof(make_model),
			    "usb", device_uri, sizeof(device_uri)))
      cupsBackendReport("direct", device_uri, make_model, make_model,
                        device_id, NULL);

    close(fd);
  }
#elif defined(__sgi)
#elif defined(__sun) && defined(ECPPIOC_GETDEVID)
  int	i;			/* Looping var */
  int	fd;			/* File descriptor */
  char	device[255],		/* Device filename */
	device_id[1024],	/* Device ID string */
	device_uri[1024],	/* Device URI string */
	make_model[1024];	/* Make and model */


 /*
  * Open each USB device...
  */

  for (i = 0; i < 8; i ++)
  {
    sprintf(device, "/dev/usb/printer%d", i);

    if ((fd = open(device, O_WRONLY | O_EXCL)) >= 0)
    {
      if (!backendGetDeviceID(fd, device_id, sizeof(device_id),
                              make_model, sizeof(make_model),
			      "usb", device_uri, sizeof(device_uri)))
	cupsBackendReport("direct", device_uri, make_model, make_model,
			  device_id, NULL);

      close(fd);
    }
  }
#elif defined(__hpux)
#elif defined(__osf)
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
  int   i;                      /* Looping var */
  char  device[255];            /* Device filename */


  for (i = 0; i < 8; i ++)
  {
    sprintf(device, "/dev/ulpt%d", i);
    if (!access(device, 0))
      printf("direct usb:%s \"Unknown\" \"USB Printer #%d\"\n", device, i + 1);

    sprintf(device, "/dev/unlpt%d", i);
    if (!access(device, 0))
      printf("direct usb:%s \"Unknown\" \"USB Printer #%d (no reset)\"\n", device, i + 1);
  }
#endif
}


/*
 * 'open_device()' - Open a USB device...
 */

static int				/* O - File descriptor or -1 on error */
open_device(const char *uri,		/* I - Device URI */
            int        *use_bc)		/* O - Set to 0 for unidirectional */
{
  int	fd;				/* File descriptor */
    int pid =getpid();

 /*
  * The generic implementation just treats the URI as a device filename...
  * Specific operating systems may also support using the device serial
  * number and/or make/model.
  */
//printf("==file:%s,line:%d,func:%s=uri=%s,pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,uri,pid);
  if (!strncmp(uri, "usb:/dev/", 9))
    return (open(uri + 4, O_RDWR | O_EXCL));
#ifdef __linux
#if 0
  {
   /*
    * Do not allow direct devices anymore...
    */
    printf("==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    errno = ENODEV;
    return (-1);
  }
#endif
  else if (!strncmp(uri, "usb://", 6))
  {
   /*
    * For Linux, try looking up the device serial number or model...
    */

    int		i;			/* Looping var */
    int		busy;			/* Are any ports busy? */
    char	device[255],		/* Device filename */
		device_id[1024],	/* Device ID string */
		make_model[1024],	/* Make and model */
		device_uri[1024];	/* Device URI string */


   /*
    * Find the correct USB device...
    */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    for (;;)
    {
      for (busy = 0, i = 0; i < 16; i ++)
      {
       /*
	* Linux has a long history of changing the standard filenames used
	* for USB printer devices.  We get the honor of trying them all...
	*/

	sprintf(device, "/dev/usblp%d", i);

        //printf("==file:%s,line:%d,func:%s=pid=%d,errno=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid,errno);
	if ((fd = open(device, O_RDWR | O_EXCL)) < 0 && errno == ENOENT)
	{
	  sprintf(device, "/dev/usb/lp%d", i);
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
	  if ((fd = open(device, O_RDWR | O_EXCL)) < 0 && errno == ENOENT)
	  {
	    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
	    sprintf(device, "/dev/usb/usblp%d", i);

    	    if ((fd = open(device, O_RDWR | O_EXCL)) < 0 && errno == ENOENT)
	      continue;
	  }
	}
        //printf("==file:%s,line:%d,func:%s=pid=%d,errno=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid,errno);
	if (fd >= 0)
	{
	    //printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,make_model=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,make_model);
	  backendGetDeviceID(fd, device_id, sizeof(device_id),
                             make_model, sizeof(make_model),
			     "usb", device_uri, sizeof(device_uri));
      //printf("==file:%s,line:%d,func:%s=device_uri=%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,device_uri,pid);
	}
	else
	{
	 /*
	  * If the open failed because it was busy, flag it so we retry
	  * as needed...
	  */
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
	  if (errno == EBUSY)
	    busy = 1;

	  device_uri[0] = '\0';
        }
        //printf("==file:%s,line:%d,func:%s=device_uri=%s,uri=%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,device_uri,uri,pid);
        if (!strcmp(uri, device_uri))
	{
	 /*
	  * Yes, return this file descriptor...
	  */
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
	  fprintf(stderr, "DEBUG: Printer using device file \"%s\"...\n",
		  device);

	  return (fd);
	}

       /*
	* This wasn't the one...
	*/
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
        if (fd >= 0)
	  close(fd);
      }

     /*
      * If we get here and at least one of the printer ports showed up
      * as "busy", then sleep for a bit and retry...
      */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
      if (busy)
   #ifdef NOT_CUT     
	_cupsLangPuts(stderr,
	              _("INFO: Printer busy; will retry in 5 seconds...\n"));
   #endif
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
      sleep(5);
    }
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  }
#elif defined(__sun) && defined(ECPPIOC_GETDEVID)
  {
   /*
    * Do not allow direct devices anymore...
    */
    //printf("==file:%s,line:%d,func:%s==\n",__FILE__,__LINE__,__FUNCTION__);
    errno = ENODEV;
    return (-1);
  }
  else if (!strncmp(uri, "usb://", 6))
  {
   /*
    * For Solaris, try looking up the device serial number or model...
    */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    int		i;			/* Looping var */
    int		busy;			/* Are any ports busy? */
    char	device[255],		/* Device filename */
		device_id[1024],	/* Device ID string */
		make_model[1024],	/* Make and model */
		device_uri[1024];	/* Device URI string */


   /*
    * Find the correct USB device...
    */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    do
    {
      for (i = 0, busy = 0; i < 8; i ++)
      {
	sprintf(device, "/dev/usb/printer%d", i);

	if ((fd = open(device, O_WRONLY | O_EXCL)) >= 0)
	  backendGetDeviceID(fd, device_id, sizeof(device_id),
                             make_model, sizeof(make_model),
			     "usb", device_uri, sizeof(device_uri));
	else
	{
	 /*
	  * If the open failed because it was busy, flag it so we retry
	  * as needed...
	  */

	  if (errno == EBUSY)
	    busy = 1;

	  device_uri[0] = '\0';
        }
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
        if (!strcmp(uri, device_uri))
	{
	 /*
	  * Yes, return this file descriptor...
	  */

          fputs("DEBUG: Setting use_bc to 0!\n", stderr);

          *use_bc = 0;

	  return (fd);
	}

       /*
	* This wasn't the one...
	*/

        if (fd >= 0)
	  close(fd);
      }
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
     /*
      * If we get here and at least one of the printer ports showed up
      * as "busy", then sleep for a bit and retry...
      */

      if (busy)
      {
    #ifdef NOT_CUT
	_cupsLangPuts(stderr,
	              _("INFO: Printer is busy; will retry in 5 seconds...\n"));
    #endif
	sleep(5);
      }
    }
    while (busy);
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
   /*
    * Couldn't find the printer, return "no such device or address"...
    */

    errno = ENODEV;

    return (-1);
  }
#else
  {
   //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    if (*use_bc)
      fd = open(uri + 4, O_RDWR | O_EXCL);
    else
      fd = -1;

    if (fd < 0)
    {
      fd      = open(uri + 4, O_WRONLY | O_EXCL);
      *use_bc = 0;
    }

    return (fd);
  }
#endif /* __linux */
  else
  {
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    errno = ENODEV;
    return (-1);
  }
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
}


/*
 * 'side_cb()' - Handle side-channel requests...
 */

static int				/* O - 0 on success, -1 on error */
side_cb(int         print_fd,		/* I - Print file */
        int         device_fd,		/* I - Device file */
        int         snmp_fd,		/* I - SNMP socket (unused) */
	http_addr_t *addr,		/* I - Device address (unused) */
	int         use_bc)		/* I - Using back-channel? */
{
  cups_sc_command_t	command;	/* Request command */
  cups_sc_status_t	status;		/* Request/response status */
  char			data[2048];	/* Request/response data */
  int			datalen;	/* Request/response data size */


  (void)snmp_fd;
  (void)addr;

  datalen = sizeof(data);

  if (cupsSideChannelRead(&command, &status, data, &datalen, 1.0))
    return (-1);

  switch (command)
  {
    case CUPS_SC_CMD_DRAIN_OUTPUT :
#ifdef NOT_CUT
        if (backendDrainOutput(print_fd, device_fd))
	  status = CUPS_SC_STATUS_IO_ERROR;
	else if (tcdrain(device_fd))
	  status = CUPS_SC_STATUS_IO_ERROR;
	else
	  status = CUPS_SC_STATUS_OK;
#else
    status = CUPS_SC_STATUS_IO_ERROR;
#endif
	datalen = 0;
        break;

    case CUPS_SC_CMD_GET_BIDI :
	status  = CUPS_SC_STATUS_OK;
        data[0] = use_bc;
        datalen = 1;
        break;

    case CUPS_SC_CMD_GET_DEVICE_ID :
        memset(data, 0, sizeof(data));

        if (backendGetDeviceID(device_fd, data, sizeof(data) - 1,
	                       NULL, 0, NULL, NULL, 0))
        {
	  status  = CUPS_SC_STATUS_NOT_IMPLEMENTED;
	  datalen = 0;
	}
	else
        {
	  status  = CUPS_SC_STATUS_OK;
	  datalen = strlen(data);
	}
        break;

    default :
        status  = CUPS_SC_STATUS_NOT_IMPLEMENTED;
	datalen = 0;
	break;
  }

  return (cupsSideChannelWrite(command, status, data, datalen, 1.0));
}


/*
 * End of "$Id: usb-unix.c 8912 2009-12-08 02:13:42Z mike $".
 */
