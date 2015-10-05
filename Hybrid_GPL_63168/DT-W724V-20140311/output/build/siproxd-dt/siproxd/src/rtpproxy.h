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

/* $Id: rtpproxy.h,v 1.2 2009/02/11 10:10:24 y42304 Exp $ */

#define CALLIDNUM_SIZE	256
#define CALLIDHOST_SIZE	32
#define CLIENT_ID_SIZE	128

#define MAX_BRANCH_SIZE	10


typedef struct {
   int wan_port;
   struct in_addr remote_ipaddr;                /* remote IP */    //  ת��ǰ��Ϣ�� c= �е�ipֵ
   int remote_port;                             /* remote port */  //  ת��ǰ��port,m= �е�ֵ
} rtp_remote_info_t;

typedef struct {
   int rtp_rx_sock;				/* rx socket (0 -> free slot)*/
   int rtp_tx_sock;				/* tx socket */
   char callid_number[CALLIDNUM_SIZE];          /* call ID */
   char callid_host[CALLIDHOST_SIZE];           /*  --"--  */
   char client_id[CLIENT_ID_SIZE];
   int direction;                               /* Direction of RTP stream */
   int media_stream_no;
   struct in_addr local_ipaddr;                 /* local IP */     //  ��Ӧlocal_port���ip��ַ����Ҫ�����Ǵ򿪷���ǽ
   int local_port;                              /* local allocated port */
   struct in_addr remote_ipaddr;                /* remote IP */    //  ת��ǰ��Ϣ�� c= �е�ipֵ
   int remote_port;                             /* remote port */  //  ת��ǰ��port,m= �е�ֵ
   time_t timestamp;                            /* last 'stream alive' TS */
   int rtcp_rx_sock;			/* rx socket (0 -> free slot)*/
   int rtcp_tx_sock;			/* tx socket */

   /* DT c40163 add for QOS */
   int t_online;
   /* DT c40163 end */
   rtp_remote_info_t remote_info[MAX_BRANCH_SIZE];          /*��invite fork������£����ж��invite�������صĲ�ͬ�Ķ˿ڣ�
                                                                �������е�sdp�п��ܲ�����ȫһ���������Ҫ����Ӧ��sdp��Զ����Ϣ����ס�����Һ�wan_port���ж�Ӧ*/
   
} rtp_proxytable_t;

/*
 * RTP relay
 */
int  rtp_relay_init(void);
/* DT c40163 modify for QOS
int  rtp_relay_start_fwd (osip_call_id_t *callid, char *client_id,
                          int rtp_direction, int media_stream_no,
		          struct in_addr local_ipaddr, int *local_port,
                          struct in_addr remote_ipaddr, int remote_port);
*/
int  rtp_relay_start_fwd (osip_call_id_t *callid, char *client_id,
                          int rtp_direction, int media_stream_no,
		                  struct in_addr local_ipaddr, int *local_port,
                          struct in_addr remote_ipaddr, int remote_port,
                          int t_online, int wan_port);
/* DT c40163 end */
int  rtp_relay_stop_fwd (int rtp_rx_sock, osip_call_id_t *callid, int rtp_direction, int nolock, int wan_port);
