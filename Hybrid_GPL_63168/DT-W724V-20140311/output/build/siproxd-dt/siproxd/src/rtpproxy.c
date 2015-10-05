/*
    Copyright (C) 2003-2005  Thomas Ries <tries@gmx.net>

    This file is part of Siproxd.
    
    Siproxd is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    Siproxd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with Siproxd; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include "config.h"

#include <sys/types.h>
#include <netinet/in.h>

#include <osipparser2/osip_parser.h>

#include "siproxd.h"
#include "rtpproxy.h"
#include "log.h"

static char const ident[]="$Id: rtpproxy.c,v 1.2 2009/02/11 10:10:24 y42304 Exp $";

/* configuration storage */
extern struct siproxd_config configuration;

/*
 * initialize and create rtp_proxy
 *
 * RETURNS
 *    STS_SUCCESS on success
 */
int rtpproxy_init( void ) {
  int sts=STS_FAILURE;

   if (configuration.rtp_proxy_enable == 0)
   {
      sts = STS_SUCCESS;
   } 
   else if (configuration.rtp_proxy_enable == 1)
   { // Relay
      sts = rtp_relay_init ();
   }
   else
   {
      ERROR("CONFIG: rtp_proxy_enable has invalid value",
            configuration.rtp_proxy_enable);
   }

   return sts;
}

/*
 * start an rtp stream on the proxy
 *
 * RETURNS
 *    STS_SUCCESS on success
 *    STS_FAILURE on error
 */
/* DT c40163 modify for QOS
int rtp_start_fwd (osip_call_id_t *callid,      //  call id 值
                   char *client_id,             //  应该是  contact 中的username值
                   int direction,               //  应该是  in
                   int media_stream_no,         //  从0开始递增
                   struct in_addr local_ipaddr, //  对于dir=out 则为wan侧地址
                   int *local_port,             //  传出来的参数
                   struct in_addr remote_ipaddr, // 如上 c= 选项中的connection address值
                   int remote_port)             //  m= 选项中的port值
*/
int rtp_start_fwd (osip_call_id_t *callid,      //  call id 值
                   char *client_id,             //  应该是  contact 中的username值
                   int direction,               //  应该是  in
                   int media_stream_no,         //  从0开始递增
                   struct in_addr local_ipaddr, //  对于dir=out 则为wan侧地址
                   int *local_port,             //  传出来的参数
                   struct in_addr remote_ipaddr, // 如上 c= 选项中的connection address值
                   int remote_port,             //  m= 选项中的port值
                   int t_online,
                   int wan_port)
/* DT c50163 end */
{
   int sts=STS_FAILURE;
      printsip("Sip: enter rtp_start_fwd \r\n");


   if (configuration.rtp_proxy_enable == 0)
   {
      sts = STS_SUCCESS;
   }
   else if (configuration.rtp_proxy_enable == 1)
   { // Relay
      /* DT c40163 modify for QOS
      sts = rtp_relay_start_fwd (callid, client_id,
                                 direction, media_stream_no,
                                 local_ipaddr, local_port,
                                 remote_ipaddr, remote_port);
      */
      sts = rtp_relay_start_fwd (callid, client_id,
                                 direction, media_stream_no,
                                 local_ipaddr, local_port,
                                 remote_ipaddr, remote_port,
                                 t_online, wan_port);
	  /* DT c40163 end */
   }
   else
   {
      ERROR("CONFIG: rtp_proxy_enable has invalid value",
            configuration.rtp_proxy_enable);
   }

   return sts;
}


/*
 * stop a rtp stream on the proxy
 *
 * RETURNS
 *    STS_SUCCESS on success
 *    STS_FAILURE on error
 */
int rtp_stop_fwd (osip_call_id_t *callid, int direction, int wan_port) {
   int sts = STS_FAILURE;

   
   printsip("rtp_stop_fwd wan_port = %d\r\n", wan_port);

   if (configuration.rtp_proxy_enable == 0) {
      sts = STS_SUCCESS;
   } else if (configuration.rtp_proxy_enable == 1) { // Relay
      sts = rtp_relay_stop_fwd(0, callid, direction, 0, wan_port);
   } else {
      ERROR("CONFIG: rtp_proxy_enable has invalid value",
            configuration.rtp_proxy_enable);
   }

   return sts;
}
