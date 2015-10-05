/*
 * "$Id: ieee1284.c 9098 2010-04-09 22:42:09Z mike $"
 *
 *   IEEE-1284 support functions for the Common UNIX Printing System (CUPS).
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
 *   backendGetDeviceID()  - Get the IEEE-1284 device ID string and
 *                           corresponding URI.
 *   backendGetMakeModel() - Get the make and model string from the device ID.
 */

/*
 * Include necessary headers.
 */

#include "backend-private.h"


/*
 * 'backendGetDeviceID()' - Get the IEEE-1284 device ID string and
 *                          corresponding URI.
 */

int					/* O - 0 on success, -1 on failure */
backendGetDeviceID(
    int        fd,			/* I - File descriptor */
    char       *device_id,		/* O - 1284 device ID */
    int        device_id_size,		/* I - Size of buffer */
    char       *make_model,		/* O - Make/model */
    int        make_model_size,		/* I - Size of buffer */
    const char *scheme,			/* I - URI scheme */
    char       *uri,			/* O - Device URI */
    int        uri_size)		/* I - Size of buffer */
{

int pid =getpid();
#ifdef __APPLE__ /* This function is a no-op */
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  return (-1);
    
#else /* Get the device ID from the specified file descriptor... */
#  ifdef __linux
  int	length;				/* Length of device ID info */
  int   got_id = 0;
#  endif /* __linux */
#  if defined(__sun) && defined(ECPPIOC_GETDEVID)
  struct ecpp_device_id did;		/* Device ID buffer */
#  endif /* __sun && ECPPIOC_GETDEVID */

//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
printf("backendGetDeviceID(fd=%d, device_id=%s, device_id_size=%d, "
                "make_model=%s, make_model_size=%d, scheme=\"%s\", "
		"uri=%s, uri_size=%d)\n", fd, device_id, device_id_size,
		make_model, make_model_size, scheme ? scheme : "(null)",
		uri, uri_size);
  DEBUG_printf(("backendGetDeviceID(fd=%d, device_id=%p, device_id_size=%d, "
                "make_model=%p, make_model_size=%d, scheme=\"%s\", "
		"uri=%p, uri_size=%d)\n", fd, device_id, device_id_size,
		make_model, make_model_size, scheme ? scheme : "(null)",
		uri, uri_size));

 /*
  * Range check input...
  */

  if (!device_id || device_id_size < 32)
  {
    DEBUG_puts("backendGetDeviceID: Bad args!");
    return (-1);
  }

  if (make_model)
    *make_model = '\0';
//printf("==file:%s,line:%d,func:%s=fd=%d,pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,fd,pid);
  if (fd >= 0)
  {
   /*
    * Get the device ID string...
    */

    *device_id = '\0';

#  ifdef __linux
    //printf("==file:%s,line:%d,func:%s=pid=%d,errno=%d,device_id=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,errno,device_id);
    if (ioctl(fd, LPIOC_GET_DEVICE_ID(device_id_size), device_id))
    {
     /*
      * Linux has to implement things differently for every device it seems.
      * Since the standard parallel port driver does not provide a simple
      * ioctl() to get the 1284 device ID, we have to open the "raw" parallel
      * device corresponding to this port and do some negotiation trickery
      * to get the current device ID.
      */
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
      if (uri && !strncmp(uri, "parallel:/dev/", 14))
      {
	char	devparport[16];		/* /dev/parportN */
	int	devparportfd,		/* File descriptor for raw device */
		  mode;			/* Port mode */


       /*
	* Since the Linux parallel backend only supports 4 parallel port
	* devices, just grab the trailing digit and use it to construct a
	* /dev/parportN filename...
	*/

	snprintf(devparport, sizeof(devparport), "/dev/parport%s",
		 uri + strlen(uri) - 1);

	if ((devparportfd = open(devparport, O_RDWR | O_NOCTTY)) != -1)
	{
	 /*
	  * Claim the device...
	  */

	  if (!ioctl(devparportfd, PPCLAIM))
	  {
	    fcntl(devparportfd, F_SETFL, fcntl(devparportfd, F_GETFL) | O_NONBLOCK);

	    mode = IEEE1284_MODE_COMPAT;

	    if (!ioctl(devparportfd, PPNEGOT, &mode))
	    {
	     /*
	      * Put the device into Device ID mode...
	      */

	      mode = IEEE1284_MODE_NIBBLE | IEEE1284_DEVICEID;

	      if (!ioctl(devparportfd, PPNEGOT, &mode))
	      {
	       /*
		* Read the 1284 device ID...
		*/

		if ((length = read(devparportfd, device_id,
				   device_id_size - 1)) >= 2)
		{
		  device_id[length] = '\0';
		  got_id = 1;
		}
	      }
	    }

	   /*
	    * Release the device...
	    */

	    ioctl(devparportfd, PPRELEASE);
	  }

	  close(devparportfd);
	}
      }
    }
    else
        {
        //printf("==file:%s,line:%d,func:%s=pid=%d,errno=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid,errno);
      got_id = 1;
        }
//printf("==file:%s,line:%d,func:%s=pid=%d,errno=%d,device_id=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,errno,device_id);
    if (got_id)
    {
     /*
      * Extract the length of the device ID string from the first two
      * bytes.  The 1284 spec says the length is stored MSB first...
      */
        //printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
      length = (((unsigned)device_id[0] & 255) << 8) +
	       ((unsigned)device_id[1] & 255);

     /*
      * Check to see if the length is larger than our buffer; first
      * assume that the vendor incorrectly implemented the 1284 spec,
      * and then limit the length to the size of our buffer...
      */

      if (length > device_id_size)
	length = (((unsigned)device_id[1] & 255) << 8) +
		 ((unsigned)device_id[0] & 255);

      if (length > device_id_size)
	length = device_id_size;

     /*
      * The length field counts the number of bytes in the string
      * including the length field itself (2 bytes).  The minimum
      * length for a valid/usable device ID is 14 bytes:
      *
      *     <LENGTH> MFG: <MFG> ;MDL: <MDL> ;
      *        2  +   4  +  1  +  5 +  1 +  1
      */
        //printf("==file:%s,line:%d,func:%s=pid=%d=length=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid,length);
      if (length < 14)
      {
       /*
	* Can't use this device ID, so don't try to copy it...
	*/

	device_id[0] = '\0';
	got_id       = 0;
      }
      else
      {
       /*
	* Copy the device ID text to the beginning of the buffer and
	* nul-terminate.
	*/
        //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
	length -= 2;

	memmove(device_id, device_id + 2, length);
	device_id[length] = '\0';
    //printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
      }
      //printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
    }
#    ifdef DEBUG
    else
      DEBUG_printf(("backendGetDeviceID: ioctl failed - %s\n",
                    strerror(errno)));
#    endif /* DEBUG */
#  endif /* __linux */

#   if defined(__sun) && defined(ECPPIOC_GETDEVID)
    did.mode = ECPP_CENTRONICS;
    did.len  = device_id_size - 1;
    did.rlen = 0;
    did.addr = device_id;

    if (!ioctl(fd, ECPPIOC_GETDEVID, &did))
    {
     /*
      * Nul-terminate the device ID text.
      */

      if (did.rlen < (device_id_size - 1))
	device_id[did.rlen] = '\0';
      else
	device_id[device_id_size - 1] = '\0';
    }
#    ifdef DEBUG
    else
      DEBUG_printf(("backendGetDeviceID: ioctl failed - %s\n",
                    strerror(errno)));
#    endif /* DEBUG */
#  endif /* __sun && ECPPIOC_GETDEVID */
  }
//printf("\n==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
//printf("backendGetDeviceID: device_id=\"%s\"\n", device_id);
  DEBUG_printf(("\nbackendGetDeviceID: device_id=\"%s\"\n", device_id));
//printf("==file:%s,line:%d,func:%s=scheme=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,scheme,uri);
  if (scheme && uri)
    *uri = '\0';
//printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
  if (!*device_id)
    {
    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
    return (-1);
    }

 /*
  * Get the make and model...
  */
//printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
  if (make_model)
    backendGetMakeModel(device_id, make_model, make_model_size);
//printf("==file:%s,line:%d,func:%s=pid=%d,make_model=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,make_model);
 /*
  * Then generate a device URI...
  */
//printf("==file:%s,line:%d,func:%s=scheme=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,scheme,uri);
  if (scheme && uri && uri_size > 32)
  {
    int			num_values;	/* Number of keys and values */
    cups_option_t	*values;	/* Keys and values in device ID */
    const char		*mfg,		/* Manufacturer */
			*mdl,		/* Model */
			*sern;		/* Serial number */
    char		temp[256],	/* Temporary manufacturer string */
			*tempptr;	/* Pointer into temp string */

    //printf("==file:%s,line:%d,func:%s=pid=%d=\n",__FILE__,__LINE__,__FUNCTION__,pid);
   /*
    * Get the make, model, and serial numbers...
    */

    num_values = _ppdGet1284Values(device_id, &values);
//printf("==file:%s,line:%d,func:%s=scheme=%s,num_values=%d=\n",__FILE__,__LINE__,__FUNCTION__,scheme,num_values);
    if ((sern = cupsGetOption("SERIALNUMBER", num_values, values)) == NULL)
      if ((sern = cupsGetOption("SERN", num_values, values)) == NULL)
        sern = cupsGetOption("SN", num_values, values);
//printf("==file:%s,line:%d,func:%s=scheme=%s,sern=%s=\n",__FILE__,__LINE__,__FUNCTION__,scheme,sern);
    if ((mfg = cupsGetOption("MANUFACTURER", num_values, values)) == NULL)
      mfg = cupsGetOption("MFG", num_values, values);
//printf("==file:%s,line:%d,func:%s=scheme=%s,mfg=%s=\n",__FILE__,__LINE__,__FUNCTION__,scheme,mfg);
    if ((mdl = cupsGetOption("MODEL", num_values, values)) == NULL)
      mdl = cupsGetOption("MDL", num_values, values);
//printf("==file:%s,line:%d,func:%s=scheme=%s,mdl=%s=\n",__FILE__,__LINE__,__FUNCTION__,scheme,mdl);
    if (mfg)
    {
      if (!strcasecmp(mfg, "Hewlett-Packard"))
        mfg = "HP";
      else if (!strcasecmp(mfg, "Lexmark International"))
        mfg = "Lexmark";
    }
    else
    {
      strlcpy(temp, make_model, sizeof(temp));
        //printf("==file:%s,line:%d,func:%s=pid=%s,mdl=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,mdl);
      if ((tempptr = strchr(temp, ' ')) != NULL)
        *tempptr = '\0';

      mfg = temp;
    }

    if (!mdl)
      mdl = "";

    if (!strncasecmp(mdl, mfg, strlen(mfg)))
    {
      mdl += strlen(mfg);

      while (isspace(*mdl & 255))
        mdl ++;
    }
    //printf("==file:%s,line:%d,func:%s=pid=%d=mdl=%s,mfg=%s,sern=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,mdl,mfg,sern);
   /*
    * Generate the device URI from the manufacturer, make_model, and
    * serial number strings.
    */
//printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
    httpAssembleURIf(HTTP_URI_CODING_ALL, uri, uri_size, scheme, NULL, mfg, 0,
                     "/%s%s%s", mdl, sern ? "?serial=" : "", sern ? sern : "");
    //printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
    cupsFreeOptions(num_values, values);
  }
//printf("==file:%s,line:%d,func:%s=pid=%d,device_id=%s,uri=%s=\n",__FILE__,__LINE__,__FUNCTION__,pid,device_id,uri);
  return (0);
#endif /* __APPLE__ */
}


/*
 * 'backendGetMakeModel()' - Get the make and model string from the device ID.
 */

int					/* O - 0 on success, -1 on failure */
backendGetMakeModel(
    const char *device_id,		/* O - 1284 device ID */
    char       *make_model,		/* O - Make/model */
    int        make_model_size)		/* I - Size of buffer */
{
  int		num_values;		/* Number of keys and values */
  cups_option_t	*values;		/* Keys and values */
  const char	*mfg,			/* Manufacturer string */
		*mdl,			/* Model string */
		*des;			/* Description string */


  DEBUG_printf(("backendGetMakeModel(device_id=\"%s\", "
                "make_model=%p, make_model_size=%d)\n", device_id,
		make_model, make_model_size));

 /*
  * Range check input...
  */

  if (!device_id || !*device_id || !make_model || make_model_size < 32)
  {
    DEBUG_puts("backendGetMakeModel: Bad args!");
    return (-1);
  }

  *make_model = '\0';

 /*
  * Look for the description field...
  */
#ifdef NOT_CUT
  num_values = _ppdGet1284Values(device_id, &values);

  if ((mdl = cupsGetOption("MODEL", num_values, values)) == NULL)
    mdl = cupsGetOption("MDL", num_values, values);

  if (mdl)
  {
   /*
    * Build a make-model string from the manufacturer and model attributes...
    */

    if ((mfg = cupsGetOption("MANUFACTURER", num_values, values)) == NULL)
      mfg = cupsGetOption("MFG", num_values, values);

    if (!mfg || !strncasecmp(mdl, mfg, strlen(mfg)))
    {
     /*
      * Just copy the model string, since it has the manufacturer...
      */

      _ppdNormalizeMakeAndModel(mdl, make_model, make_model_size);
    }
    else
    {
     /*
      * Concatenate the make and model...
      */

      char	temp[1024];		/* Temporary make and model */

      if (mfg)
	snprintf(temp, sizeof(temp), "%s %s", mfg, mdl);
      else
	snprintf(temp, sizeof(temp), "%s", mdl);

      _ppdNormalizeMakeAndModel(temp, make_model, make_model_size);
    }
  }
  else if ((des = cupsGetOption("DESCRIPTION", num_values, values)) != NULL ||
           (des = cupsGetOption("DES", num_values, values)) != NULL)
  {
   /*
    * Make sure the description contains something useful, since some
    * printer manufacturers (HP) apparently don't follow the standards
    * they helped to define...
    *
    * Here we require the description to be 8 or more characters in length,
    * containing at least one space and one letter.
    */

    if (strlen(des) >= 8)
    {
      const char	*ptr;		/* Pointer into description */
      int		letters,	/* Number of letters seen */
			spaces;		/* Number of spaces seen */


      for (ptr = des, letters = 0, spaces = 0; *ptr; ptr ++)
      {
	if (isspace(*ptr & 255))
	  spaces ++;
	else if (isalpha(*ptr & 255))
	  letters ++;

	if (spaces && letters)
	  break;
      }

      if (spaces && letters)
        _ppdNormalizeMakeAndModel(des, make_model, make_model_size);
    }
  }
#endif
  if (!make_model[0])
  {
   /*
    * Use "Unknown" as the printer make and model...
    */

    strlcpy(make_model, "Unknown", make_model_size);
  }

  cupsFreeOptions(num_values, values);

  return (0);
}


/*
 * End of "$Id: ieee1284.c 9098 2010-04-09 22:42:09Z mike $".
 */
