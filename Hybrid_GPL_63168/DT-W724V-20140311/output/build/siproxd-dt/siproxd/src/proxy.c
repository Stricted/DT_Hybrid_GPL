/* -*- Mode: C; c-basic-offset: 3 -*-
    Copyright (C) 2002-2005  Thomas Ries <tries@gmx.net>

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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <osipparser2/osip_parser.h>
#include <osipparser2/sdp_message.h>
#include "atpconfig.h"

#include "siproxd.h"
#include "log.h"

static char const ident[]="$Id: proxy.c,v 1.10 2009/06/22 09:48:16 y42304 Exp $";

/* configuration storage */
extern struct siproxd_config configuration;    /* defined in siproxd.c */

extern struct urlmap_s urlmap[];        /* URL mapping table     */
extern struct lcl_if_s local_addresses;
extern int sip_udp_lan_socket;

#ifdef SUPPORT_ATP_BRIDGE_BIND
extern int g_lOutIFaceIsProxy;
extern char g_acPPPoEIpAddr[SIP_IPADDR];
extern char g_acPPPoEMask[SIP_IPADDR];
extern char g_acBrIpAddr[SIP_IPADDR];
extern char g_acBrMask[SIP_IPADDR];
#endif

#define MAX_SIPVIA_RECORD  50  //viaȫ���������󳤶�
#define MAX_SIPVIA_NUM  5     //ÿ��sip������via��������
typedef struct tagsip_via_t
{
    char *callid_tmp;  //��sip�������call id
    osip_via_t *via[MAX_SIPVIA_NUM];//��sip�������via 
    int via_num;  //ÿ��sip������via�ĸ���
	int sip_csep; //�洢sip���ĵ����к�
}sip_via_t;

typedef struct tagsip_via_node
{
    struct tagsip_via_node *pstNext; 
    sip_via_t   stSaveSipVia;
}sip_via_node;


sip_via_node  *g_pstSipViaList = NULL;  //���ڴ洢via��ȫ������
int g_SipViaNum = 0;   //��¼ȫ������ǰ�ĳ���


/* �ͷŴ洢��via*/
void FreeSipVia(sip_via_t *pstSipVia)
{
    int i = 0;
	
    if (NULL == pstSipVia)
    {
	    return;
	}
	
	for (i = 0; i < pstSipVia->via_num; i++)
    {
	    if (NULL != pstSipVia->via[i])
	    {
		    osip_via_free(pstSipVia->via[i]);
		}
	}
}

/*
 * PROXY_REQUEST
 *
 * RETURNS
 *    STS_SUCCESS on success
 *    STS_FAILURE on error
 *
 * RFC3261
 *    Section 16.3: Proxy Behavior - Request Validation
 *    1. Reasonable Syntax
 *    2. URI scheme
 *    3. Max-Forwards
 *    4. (Optional) Loop Detection
 *    5. Proxy-Require
 *    6. Proxy-Authorization
 *
 *    Section 16.6: Proxy Behavior - Request Forwarding
 *    1.  Make a copy of the received request
 *    2.  Update the Request-URI
 *    3.  Update the Max-Forwards header field
 *    4.  Optionally add a Record-route header field value
 *    5.  Optionally add additional header fields
 *    6.  Postprocess routing information
 *    7.  Determine the next-hop address, port, and transport
 *    8.  Add a Via header field value
 *    9.  Add a Content-Length header field if necessary
 *    10. Forward the new request
 *    11. Set timer C
 */
int proxy_request (sip_ticket_t *ticket) 
{
   int Hg522 = -1;
   int i;
   int sts;
   int type;
   struct in_addr sendto_addr;
   osip_uri_t *url;
   int port;
   char *buffer;
   osip_message_t *request;      //  �յ��ı���
   struct sockaddr_in *from;     //  �յ����ĵ�Դ��ַ
   struct in_addr local_ip;
   struct in_addr temp_ip;
   //struct in_addr *temp_ip;

  /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
   int index = -1;
   /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
   int iWanSocket = 0;
   int iWanPort = 0;
   char *outbound_if = NULL;       /* ���Ĵӽӿ�ת����WAN�� */
   struct in_addr addr_outbound = {0};
   
   osip_via_t *via = NULL;
   osip_via_t *via_temp = NULL;
   struct in_addr addr_via;
   int port_via = 0;
   sip_via_node  *pstTmpNode = NULL;
   sip_via_node *pstCurNode = NULL;
   sip_via_node *pstPreNode = NULL;
   char *tmp = NULL;
   int via_i = 0;  
   int pos = 0;
   osip_contact_t *contact = NULL;

   DEBUGC(DBCLASS_PROXY,"proxy_request");

   if (ticket==NULL)
   {
      printsip("proxy_request: called with NULL ticket \r\n");
      return STS_FAILURE;
   }

   request=ticket->sipmsg;         //  �յ��ı���
   from=&ticket->from;             //  �յ����ĵ�Դ��ַ

//��ʼ�����͵�ַ��������������֪��IP
	memset(&sendto_addr,0,sizeof(struct in_addr));
   /*
    * RFC 3261, Section 16.4
    * Proxy Behavior - Route Information Preprocessing
    * (process Route header)
    */
   route_preprocess(ticket);

   /*
    * figure out whether this is an incoming or outgoing request
    * by doing a lookup in the registration table.
    */
#define _OLD_DIRECTION_EVALUATION 0
#if 0 //_OLD_DIRECTION_EVALUATION
   type = 0;
   for (i=0; i<URLMAP_SIZE; i++)
   {
      if (urlmap[i].active == 0) continue;

      /* incoming request ('to' == 'masq') || (('to' == 'reg') && !REGISTER)*/
      if ((compare_url(request->to->url, urlmap[i].masq_url)==STS_SUCCESS) ||
          (!MSG_IS_REGISTER(request) &&
           (compare_url(request->to->url, urlmap[i].reg_url)==STS_SUCCESS)))
      {
         type=REQTYP_INCOMING;   //define siproxd.h = 1
         DEBUGC(DBCLASS_PROXY,"incoming request from %s@%s from outbound",
         request->from->url->username? request->from->url->username:"*NULL*",
           request->from->url->host? request->from->url->host: "*NULL*");
         break;
      }

      /* outgoing request ('from' == 'reg') */
      if (compare_url(request->from->url, urlmap[i].reg_url)==STS_SUCCESS)
      {
         type=REQTYP_OUTGOING;
         DEBUGC(DBCLASS_PROXY,"outgoing request from %s@%s from inbound",
            request->from->url->username? request->from->url->username:"*NULL*",
            request->from->url->host? request->from->url->host: "*NULL*");
         break;
      }
   }
#else
   type = 0;
   /*��forking������»��ж��contactͷ�������Ҫ���Ҷ�Ӧ��contact���и�д*/
   pos = 0;
   while (!osip_list_eol (ticket->sipmsg->contacts, pos)) {
      contact = (osip_contact_t *) osip_list_get (ticket->sipmsg->contacts, pos);

      if (contact == NULL)
      {
          printsip("sip: proxy_request contact is NULL \r\n");
          continue;
      }

      for (i=0;i<URLMAP_SIZE;i++)
      {
        if (urlmap[i].active == 0) continue;
        if (((ticket->direction == REQTYP_OUTGOING) || (ticket->direction == RESTYP_OUTGOING)) &&
            (compare_url(contact->url, urlmap[i].true_url)==STS_SUCCESS)) {printsip("sip: proxy_request urlmap[%d] match \r\n", i);index = i;type=REQTYP_OUTGOING;break;}
        if (((ticket->direction == REQTYP_INCOMING) || (ticket->direction == RESTYP_INCOMING)) &&
            (compare_url(contact->url, urlmap[i].masq_url)==STS_SUCCESS)) {printsip("sip: proxy_request urlmap[%d] match \r\n", i);index = i;type=REQTYP_INCOMING;break;}
      }
      pos++;
   }

   /*
    * did I receive the telegram from a REGISTERED host?
    * -> it must be an OUTGOING request
    */
   if (type == 0) 
   {
      for (i=0; i<URLMAP_SIZE; i++) 
      {
         struct in_addr tmp_addr;
   
         if (urlmap[i].active == 0) 
         {
           continue;
         }
         printsip("Sip:Proxy_request(), Find Dir urlmap[%d] is active. \r\n", i);
         
         if (get_ip_by_host(urlmap[i].true_url->host, &tmp_addr) == STS_FAILURE) 
         {
            printsip("proxy_request(): cannot resolve host [%s] \r\n",
                urlmap[i].true_url->host);
         }
         else
         {
            /* ���ڵ�һ��ע�ᱨ��, �� sts = register_client(&ticket, 1) ���Ѿ�����urlmap[i]
               from == tmp_addr(urlmap[i].ture_url->host) */
            printsip("proxy_request: urlmap[%d].true_url:[%s] src.ip:[%s] \r\n",
                   i, urlmap[i].true_url->host, utils_inet_ntoa(from->sin_addr));
            printsip("proxy_request: urlmap[%d].true_url:[%s] from->sin_port:[%d] \r\n",
                   i, urlmap[i].true_url->host, from->sin_port);
   
            /* ������ݰ������� conntact �ߣ�Ҳ������ע���߷�������� */
            if ((memcmp(&tmp_addr, &from->sin_addr, sizeof(tmp_addr)) == 0) 
               && ((urlmap[i].true_url->port && atoi(urlmap[i].true_url->port) == from->sin_port)
                  || (!urlmap[i].true_url->port && 5060 == from->sin_port)))
            {
               // ���urlmap[i].true_url����ip �� �յ����ĵ�Դip ��ͬ ��Ϊout����
               type=REQTYP_OUTGOING;
   #if UNREG_WITHOUT_REG_HG522
               Hg522 = i;
               printsip("proxy_request: type = OUT, urlmap:[%d].regurl:[%s] \r\n", Hg522, urlmap[Hg522].reg_url->host);
   #endif
              /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
               index = i;
               /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
   
               break;
            }
         }
   
   	  /* DT c40163 add for STUN */
         if (urlmap[i].from.sin_addr.s_addr != 0
   	  	&& memcmp(&(urlmap[i].from.sin_addr), &from->sin_addr, sizeof(struct in_addr)) == 0
   	  	&& (urlmap[i].from.sin_port == from->sin_port))
         {
            type = REQTYP_OUTGOING;
            index = i;
            break;
         }
   	  /* DT c40163 end */
      }
   }


   /*
    * is the telegram directed to an internally registered host?
    * -> it must be an INCOMING request
    */
   if (type == 0) 
   {
      for (i=0; i<URLMAP_SIZE; i++) 
      {
         if (urlmap[i].active == 0) continue;
         /* RFC3261:
          * 'To' contains a display name (Bob) and a SIP or SIPS URI
          * (sip:bob@biloxi.com) towards which the request was originally
          * directed.  Display names are described in RFC 2822 [3].
          */

         /* So this means, that we must check the SIP URI supplied with the
          * INVITE method, as this points to the real wanted target.
          * Q: does there exist a situation where the SIP URI itself does
          *    point to "somewhere" but the To: points to the correct UA?
          * So for now, we just look at both of them (SIP URI and To: header)
          */

         /* incoming request (SIP URI == 'masq') || ((SIP URI == 'reg') && !REGISTER)*/
         if ((compare_url(request->req_uri, urlmap[i].masq_url)==STS_SUCCESS) ||
             (!MSG_IS_REGISTER(request) &&
              (compare_url(request->req_uri, urlmap[i].reg_url)==STS_SUCCESS))) 
         {
            type=REQTYP_INCOMING;
#if UNREG_WITHOUT_REG_HG522
            Hg522 = i;
            printsip("proxy_request: type = IN, urlmap:[%d].regurl:[%s] \r\n", Hg522, urlmap[Hg522].reg_url->host);
#endif
            /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            index = i;
            /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */

            break;
         }
         /*���ȱȽ�request url*/
#if 0
         /* incoming request ('to' == 'masq') || (('to' == 'reg') && !REGISTER)*/
         if ((compare_url(request->to->url, urlmap[i].masq_url)==STS_SUCCESS) ||
             (!MSG_IS_REGISTER(request) &&
              (compare_url(request->to->url, urlmap[i].reg_url)==STS_SUCCESS))) 
         {
            type=REQTYP_INCOMING;
#if UNREG_WITHOUT_REG_HG522
            Hg522 = i;
            printsip("proxy_request: type = IN, urlmap:[%d].regurl:[%s] \r\n", Hg522, urlmap[Hg522].reg_url->host);
#endif
            /* DT c40163 for STUN */
            index = i;
            /* DT c40163 end */

            break;
         }
#endif
      }

      /*Ϊ����W724v ��sip server(������to url�����˿�),���ȱȽ�request url,request urlû��ƥ���ϣ��ٱȽ�to url*/
      /* �����ͬpcע��ͬһ�����룬���ⲿ����˺��룬����ת��lan������pc,û�и���wan��˿�ƥ����ת��*/
      if (i >= URLMAP_SIZE)
      {
          for (i=0; i<URLMAP_SIZE; i++) 
          {
             if (urlmap[i].active == 0) continue;
             /* RFC3261:
              * 'To' contains a display name (Bob) and a SIP or SIPS URI
              * (sip:bob@biloxi.com) towards which the request was originally
              * directed.  Display names are described in RFC 2822 [3].
              */

             /* So this means, that we must check the SIP URI supplied with the
              * INVITE method, as this points to the real wanted target.
              * Q: does there exist a situation where the SIP URI itself does
              *    point to "somewhere" but the To: points to the correct UA?
              * So for now, we just look at both of them (SIP URI and To: header)
              */
             /* incoming request ('to' == 'masq') || (('to' == 'reg') && !REGISTER)*/
             if ((compare_url(request->to->url, urlmap[i].masq_url)==STS_SUCCESS) ||
                 (!MSG_IS_REGISTER(request) &&
                  (compare_url(request->to->url, urlmap[i].reg_url)==STS_SUCCESS))) 
             {
                type=REQTYP_INCOMING;
#if UNREG_WITHOUT_REG_HG522
                Hg522 = i;
                printsip("proxy_request: type = IN, urlmap:[%d].regurl:[%s] \r\n", Hg522, urlmap[Hg522].reg_url->host);
#endif
                /* DT c40163 for STUN */
                index = i;
                /* DT c40163 end */

                break;
             }
          }
      }
   }

#if UNREG_WITHOUT_REG_HG522
/* Start of sipalg by c47036 20070429 */
   if (type == 0)
   {
      int j;
      osip_uri_t *url1_contact=NULL;
      struct in_addr lan_ip;
      
      get_ip_by_ifname(configuration.inbound_if, &lan_ip);

      if ((from->sin_addr.s_addr & htonl(0xffffff00)) == (lan_ip.s_addr & htonl(0xffffff00)))
      {
        printsip("\r\n ******* HG522(OUT): ticket from:%s from:%s lan:%s  \r\n", inet_ntoa(ticket->from.sin_addr), inet_ntoa(from->sin_addr), utils_inet_ntoa(lan_ip));
        type = REQTYP_OUTGOING;
      }
      else
      {
        printsip("\r\n ******* HG522(IN): from:%s lan:%s  \r\n", utils_inet_ntoa(from->sin_addr), utils_inet_ntoa(lan_ip));
        type = REQTYP_INCOMING;
      }

      /* 2009-06-18 start of d00107688  AU4D01899  ��SIP�绰ͨ�������У�ȥʹ��/ʹ��sip alg��sip�绰�һ����ٴν���sip���У����յ��Է�200OKSDPʱ���ֽ����˳� */
        if (((type == REQTYP_INCOMING) || (type == REQTYP_OUTGOING)) && !(MSG_IS_REGISTER(request)))
        {
            DEBUGPC("please register first......");
            return STS_FAILURE;
        }
#if 0
        else if ((type == REQTYP_OUTGOING) && !(MSG_IS_REGISTER(request)))
        {
            DEBUGPC("msg is outgoing, but not register msg and can not found urlmap");
            return STS_FAILURE;
        }
#endif
      /* 2009-06-18 end of d00107688  AU4D01899  ��SIP�绰ͨ�������У�ȥʹ��/ʹ��sip alg��sip�绰�һ����ٴν���sip���У����յ��Է�200OKSDPʱ���ֽ����˳� */
      
      /*
       * First make sure, we have a prober Contact header:
       *  - url
       *  - url -> hostname
       *
       * Libosip parses an:
       * "Contact: *"
       * the following way (Note: Display name!! and URL is NULL)
       * (gdb) p *((osip_contact_t*)(sip->contacts->node->element))
       * $5 = {displayname = 0x8af8848 "*", url = 0x0, gen_params = 0x8af8838}
       */
      if (ticket->sipmsg->contacts && ticket->sipmsg->contacts->node &&
          ticket->sipmsg->contacts->node->element)
      {
         printsip("HG522: ticket->sipmsg->contacts->node->element not NULL to URL \r\n");
         url1_contact=((osip_contact_t*)
                      (ticket->sipmsg->contacts->node->element))->url;
      }
      if ((url1_contact == NULL) || (url1_contact->host == NULL))
      {
         /* Don't have required Contact fields */
         printsip("HG522: tried registration(I) with empty Contact header, URL is NULL \r\n");
/* start of by d00107688 2009-05-25 AU4D01736 �¹�SIP�绰ͨ�������У�ȥʹ��/ʹ��sip alg��sip�绰�һ��ᵼ��sip alg�����˳� */
         return STS_FAILURE;
/* end of by d00107688 2009-05-25 AU4D01736 �¹�SIP�绰ͨ�������У�ȥʹ��/ʹ��sip alg��sip�绰�һ��ᵼ��sip alg�����˳� */
      }
         
      /*
       * Update registration. There are two possibilities:
       * - already registered, then update the existing record
       * - not registered, then create a new record
       */

      j=-1;
      for (i=0; i<URLMAP_SIZE; i++)
      {
         if (urlmap[i].active == 0)
         {
            if (j < 0) j=i; /* remember first hole */
            continue;
         }
      }

      if ( (j < 0) && (i >= URLMAP_SIZE) )
      {
         /* oops, no free entries left... */
         printsip("HG522: URLMAP is full - registration failed \r\n");
         return STS_FAILURE;
      }

      /* ������� */
      if (i >= URLMAP_SIZE)
      {
         //����telecomlist����ѯ��Ӧ��wan�ӿ�
         outbound_if = get_if_by_telecomlist(utils_inet_ntoa(ticket->to.sin_addr), ticket->from.sin_port, is_t_online(ticket->sipmsg, -1), ticket->ulMark, ticket->sipmsg->req_uri->host);
         if (get_ip_by_ifname(outbound_if, &addr_outbound) != STS_SUCCESS)
         {
            DEBUGPC("can't find outbound interface %s - configuration error?", outbound_if);
            return STS_FAILURE;
         }
         /* entry not existing, create new one */
         i=j;
         Hg522 = i;

         /* write entry */
         urlmap[i].active=1;  
         urlmap[i].expires = 30; 
         if (type == REQTYP_OUTGOING)
         {
            char port[HOSTNAME_SIZE] = {0};
            DEBUGPC("type (OUT) so will create a url map");
#if 0
             /* Contact: field */
             osip_uri_clone( ((osip_contact_t*)
                             (ticket->sipmsg->contacts->node->element))->url, 
                         &urlmap[i].true_url);

#else             
             /* Contact: field */
             osip_uri_clone(ticket->sipmsg->from->url, 
                         &urlmap[i].true_url);

             printsip("HG522(OUT): urlmap[%d].true_url->host:[%s], srcip:[%s], \r\n", i , urlmap[i].true_url->host, inet_ntoa(from->sin_addr));
             /*��Ҫfree��ԭ�ȵ��ڴ棬Ȼ������malloc�����µ��ڴ������host��port������ռ���ܲ���*/
             snprintf(port, sizeof(port), "%i", from->sin_port);
             osip_free (urlmap[i].true_url->host);
             osip_free (urlmap[i].true_url->port);
             urlmap[i].true_url->host = osip_strdup (inet_ntoa(from->sin_addr));
             urlmap[i].true_url->port = osip_strdup (port);
#endif            
             
             /* To: field */
             osip_uri_clone( ticket->sipmsg->to->url, 
                    &urlmap[i].reg_url);
             /*���Ѿ�ע����ϵ�����£�����ٴ��յ�ע�����ģ���ֹ����to_ip�ĸ�ֵ���º������sendto_addrʱ����HG522�ķ�֧
             �Ӷ�����ֱ�ӻ�ȡ��Ŀ��ip���������½���dns�����õ�sendto_addr��ip*/
#ifndef SUPPORT_ATP_SIPPXY_RECORDDSTIP
#if UNREG_WITHOUT_REG_HG522
             urlmap[i].to_ip = (char*)malloc(256);
             if (NULL != urlmap[i].to_ip)
             {
                  strcpy(urlmap[i].to_ip, ticket->sipmsg->to->url->host);
                  printsip("**************** %d reg_url:[%s],  to_ip:[%s] \r\n", i, urlmap[i].reg_url->host, urlmap[i].to_ip);
             }
             else
             {
                  printsip("sip: serious err, no mem\r\n");
             }
#endif
#endif
             iWanSocket = get_wan_socket(outbound_if, &iWanPort);
             if (0 == iWanSocket)
             {
                ERROR("Allocate WAN Socket !!!FAILED!!! ");
                return STS_FAILURE;
             }

             /*hometalk app��ע��ok������£����ʲô�����޸��ٴ�ע���ʱ�򣬻���ԭ���Ķ˿�(��1880)ע��һ�飬Ȼ��������һ���˿�(��36519)��ע��һ�飬
             ���ǵڶ���ע����via(1880)��contact(36519)�ͱ��ĵ�Դ��ַ(36519)�еĶ˿ڲ�һ�£������õ�ԭ���Ķ˿�(1880)����������Ҫ��ס������ĵ�Դport��ip��
             ������ת����Ӧ��ʱ�����ticket->from��ip��portת��*/
             memcpy(&urlmap[i].from, &ticket->from, sizeof(urlmap[i].from));
             //ת����WANҪ������socket�Ͷ˿�, �˿ں�Ҫ���µ�WAN��contact��,������ԭ��д����56005
             urlmap[i].wan_socket_fd = iWanSocket;
             urlmap[i].wan_port = iWanPort;
             snprintf(urlmap[i].outbound_if, sizeof(urlmap[i].outbound_if), "%s", outbound_if);
             memcpy(&urlmap[i].outbound_addr, &addr_outbound, sizeof(urlmap[i].outbound_addr));
             proxy_control_wan_port(FW_SIP_CMD_TYPE_REMOVE, iWanPort, i);
             proxy_control_wan_port(FW_SIP_CMD_TYPE_OPEN, iWanPort, i);
             index = i;
         }
         else
         {
             printsip("HG522(IN): urlmap[%d].true_url->host:[%s], srcip:[%s], \r\n", 
                i , urlmap[i].true_url->host, utils_inet_ntoa(from->sin_addr));
             /* Contact: field */
             osip_uri_clone(((osip_contact_t*)
                             (ticket->sipmsg->contacts->node->element))->url, 
                         &urlmap[i].reg_url);
             /* To: field */
             osip_uri_clone( ticket->sipmsg->to->url, 
                    &urlmap[i].true_url);     
             
             printsip("HG522(IN): urlmap[%d].true_url->host:[%s], srcip:[%s], \r\n", i , urlmap[i].true_url->host, utils_inet_ntoa(from->sin_addr));
         }

         /*
          * try to figure out if we ought to do some masquerading
          */
         osip_uri_clone( ticket->sipmsg->to->url, 
                     &urlmap[i].masq_url);


         /*
          * for transparent proxying: force device to be masqueraded
          * as with the outbound IP
          */
         {
            struct in_addr addr;
            char *addrstr;
#if 0
            if (get_ip_by_ifname(configuration.outbound_if, &addr) !=
                STS_SUCCESS) {
               ERROR("can't find outbound interface %s - configuration error?",
                     configuration.outbound_if);
               return STS_FAILURE;
            }
#else
            memcpy(&addr, &urlmap[i].outbound_addr, sizeof(addr));
#endif

            /* host part */
            addrstr = utils_inet_ntoa(addr);

            urlmap[i].masq_url->host=realloc(urlmap[i].masq_url->host,
                                             strlen(addrstr)+1);
            strcpy(urlmap[i].masq_url->host, addrstr);
#if 0
            /* port number if required */
            if (configuration.sip_listen_port != SIP_PORT) 
            {
               urlmap[i].masq_url->port=realloc(urlmap[i].masq_url->port, 16);
               sprintf(urlmap[i].masq_url->port, "%i",
                       configuration.sip_listen_port);
            }
#else
           urlmap[i].masq_url->port=realloc(urlmap[i].masq_url->port, 16);
           sprintf(urlmap[i].masq_url->port, "%i",urlmap[i].wan_port);
           DEBUGC(DBCLASS_PROXY,"proxy_request, masq_url->port=%d", urlmap[i].wan_port);
#endif

         }
      }
   }

   DEBUGPC("macro UNREG_WITHOUT_REG_HG522 over");
/* End of sipalg by c47036 20070429 */
#endif

#endif
   //ticket->direction = type;
   type = ticket->direction;

   if ((index < 0) || (index >= URLMAP_SIZE))
   {
      printsip("in proxy_request urlmap index error\n");
      return STS_FAILURE;
   }

   /*
    * logging of passing calls
    */
   if (configuration.log_calls) 
   {
      osip_uri_t *cont_url = NULL;
      if (!osip_list_eol(request->contacts, 0))
         cont_url = ((osip_contact_t*)(request->contacts->node->element))->url;
      
      /* INVITE */
      if (MSG_IS_INVITE(request)) 
      {
         if (cont_url) 
         {
            printsip("%s Call from: %s@%s \r\n",
                 (type==REQTYP_INCOMING) ? "Incoming":"Outgoing",
                 cont_url->username ? cont_url->username:"*NULL*",
                 cont_url->host ? cont_url->host : "*NULL*");
         } 
         else
         {
            printsip("%s Call (w/o contact header) from: %s@%s \r\n",
                 (type==REQTYP_INCOMING) ? "Incoming":"Outgoing",
             request->from->url->username ? 
                    request->from->url->username:"*NULL*",
             request->from->url->host ? 
                    request->from->url->host : "*NULL*");
         }
      /* BYE / CANCEL */
      } 
      else if (MSG_IS_BYE(request) || MSG_IS_CANCEL(request)) 
      {
         if (cont_url)
         {
            printsip("Ending Call from: %s@%s \r\n",
                 cont_url->username ? cont_url->username:"*NULL*",
                 cont_url->host ? cont_url->host : "*NULL*");
         }
         else
         {
            printsip("Ending Call (w/o contact header) from: %s@%s \r\n",
             request->from->url->username ? 
                    request->from->url->username:"*NULL*",
             request->from->url->host ? 
                    request->from->url->host : "*NULL*");
         }
      }
   } /* log_calls */


   /*
    * RFC 3261, Section 16.6 step 1
    * Proxy Behavior - Request Forwarding - Make a copy
    */
   /* nothing to do here, copy is ready in 'request'*/

   /* get destination address ��ȡ���ͱ��ĵ� Ŀ�ĵ�ַ(request line) */
   url=osip_message_get_uri(request);

   /* Determine local address to use based on the To: address */
   DEBUGPC("request->to->url->host: %s\n", request->to->url->host);

   /* ���to�ֶ��д���Ĳ���һ��IP��ַ������һ����������ô��ʱ��Ȼ���������� 0  */
   if (utils_inet_aton(request->to->url->host, &temp_ip) != 0)
   {
       get_local_ip(request->to->url->host, &local_ip, i);
       
       DEBUGPC("Sip:request->to->url->host:[%s]  local IP(route): %s \r\n", 
           request->to->url->host,inet_ntoa(local_ip));
   }
   else
   {
      //get_ip_by_ifname(configuration.outbound_if, &local_ip);
      memcpy(&local_ip, &urlmap[i].outbound_addr, sizeof(local_ip));
      
      DEBUGPC("Sip:local IP(outbound): local IP: %s \r\n", inet_ntoa(local_ip));
   }
   
   DEBUGC(DBCLASS_PROXY, "local IP: %s", inet_ntoa(local_ip));

   switch (type)
   {
  /*
   * from an external host to the internal masqueraded host
   */
   case REQTYP_INCOMING:
      DEBUGC(DBCLASS_PROXY,"incoming request from %s@%s from outbound",
    request->from->url->username? request->from->url->username:"*NULL*",
        request->from->url->host? request->from->url->host: "*NULL*");

      /*
       * RFC 3261, Section 16.6 step 2
       * Proxy Behavior - Request Forwarding - Request-URI
       * (rewrite request URI to point to the real host)
       */
      /* 'i' still holds the valid index into the URLMAP table */
      if (check_rewrite_rq_uri(request) == STS_TRUE) 
      {
         proxy_rewrite_request_uri(request, i);
      }

      /* if this is CANCEL/BYE request, stop RTP proxying */
      if (MSG_IS_BYE(request) || MSG_IS_CANCEL(request)) 
      {
         //��¼�Ƿ��ڽ���ͨ���������ж�ͨ��ʱ���ܽ���wan�л�
         urlmap[index].incall = ALG_FALSE;
         printsip("incoming cancel to rtp_stop_fwd \r\n");
         /* stop the RTP proxying stream(s) */
         rtp_stop_fwd(osip_message_get_call_id(request), DIR_INCOMING, urlmap[index].wan_port);
         rtp_stop_fwd(osip_message_get_call_id(request), DIR_OUTGOING, urlmap[index].wan_port);

      /* check for incoming request */
      }
      else if (MSG_IS_INVITE(request))
      {
         printsip("Sip: proxy_request: urlmap[i].wan_port %d \r\n", urlmap[index].wan_port);

         /* rewrite the body */
         if (configuration.rtp_proxy_enable == 1)
         {
            //��¼�Ƿ��ڽ���ͨ���������ж�ͨ��ʱ���ܽ���wan�л�
            urlmap[index].incall = ALG_TRUE;
             /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            #if 0
            sts = proxy_rewrite_invitation_body(request, DIR_INCOMING, NULL);
            #else
			/* DT c40163 modify for STUN
            sts = proxy_rewrite_invitation_body(index, request, DIR_INCOMING, NULL);
            */
            sts = proxy_rewrite_invitation_body(index, request, DIR_INCOMING, NULL, 0, urlmap[index].wan_port);
			/* DT c40163 end */
            #endif
            /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
         }
      } 
      else if (MSG_IS_ACK(request))
      {
         printsip("Sip: proxy_request: urlmap[i].wan_port %d \r\n", urlmap[index].wan_port);
         urlmap[index].incall = ALG_TRUE;

         /* rewrite the body */
          /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
         #if 0
         sts = proxy_rewrite_invitation_body(request, DIR_INCOMING, NULL);
         #else
         /* DT c40163 modify for STUN
         sts = proxy_rewrite_invitation_body(index, request, DIR_INCOMING, NULL);
         */
         sts = proxy_rewrite_invitation_body(index, request, DIR_INCOMING, NULL, 0, urlmap[index].wan_port);
		 /* DT c40163 end */
         #endif
         /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
      }
      break;
   
  /*
   * from the internal masqueraded host to an external host
   */
   case REQTYP_OUTGOING:
    
      DEBUGPC("outgoing request from %s@%s from inbound",
    request->from->url->username? request->from->url->username:"*NULL*",
        request->from->url->host? request->from->url->host: "*NULL*");

      /*
       * RFC 3261, Section 16.6 step 2
       * Proxy Behavior - Request Forwarding - Request-URI
       */
      /* nothing to do for an outgoing request */


      /* if it is addressed to myself, then it must be some request
       * method that I as a proxy do not support. Reject */
#if 0
/* careful - an internal UA might send an request to another internal UA.
   This would be caught here, so don't do this. This situation should be
   caught in the default part of the CASE statement below */
      if (is_sipuri_local(ticket) == STS_TRUE) {
         WARN("unsupported request [%s] directed to proxy from %s@%s -> %s@%s",
        request->sip_method? request->sip_method:"*NULL*",
        request->from->url->username? request->from->url->username:"*NULL*",
        request->from->url->host? request->from->url->host : "*NULL*",
        url->username? url->username : "*NULL*",
        url->host? url->host : "*NULL*");

         sip_gen_response(ticket, 403 /*forbidden*/);

         return STS_FAILURE;
      }
#endif

      /* rewrite Contact header to represent the masqued address */
      sip_rewrite_contact(ticket, DIR_OUTGOING, &local_ip, index);

      /* if an INVITE, rewrite body */
      if (MSG_IS_INVITE(request))
      {
        urlmap[index].incall = ALG_TRUE;
        printsip("Sip: proxy_request: urlmap[i].wan_port %d \r\n", urlmap[index].wan_port);

        DEBUGPC("invite msg, so start rtp proxy");
        /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
#if 0
         sts = proxy_rewrite_invitation_body(request, DIR_OUTGOING, &local_ip);
#else
         /* DT c40163 modify for STUN
         sts = proxy_rewrite_invitation_body(index, request, DIR_OUTGOING, &local_ip);
         */
         sts = proxy_rewrite_invitation_body(index, request, DIR_OUTGOING, &local_ip, &(from->sin_addr), urlmap[index].wan_port);
         /* DT c40163 end */
 #endif
         /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
      } 
      else if (MSG_IS_ACK(request))
      {
         urlmap[index].incall = ALG_TRUE;
         printsip("Sip: proxy_request: urlmap[i].wan_port %d \r\n", urlmap[index].wan_port);

          /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
#if 0
         sts = proxy_rewrite_invitation_body(request, DIR_OUTGOING, &local_ip);
#else
        /* DT c40163 modify for STUN
        sts = proxy_rewrite_invitation_body(index, request, DIR_OUTGOING, &local_ip);
        */
        sts = proxy_rewrite_invitation_body(index, request, DIR_OUTGOING, &local_ip, &(from->sin_addr), urlmap[index].wan_port);
        /* DT c40163 end */
#endif
        /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
      }

      /* if this is CANCEL/BYE request, stop RTP proxying */
      if (MSG_IS_BYE(request) || MSG_IS_CANCEL(request))
      {
         urlmap[index].incall = ALG_FALSE;
         DEBUGPC("MSG_IS_BYE(request) || MSG_IS_CANCEL(request) rtp_stop_fwd");
         /* stop the RTP proxying stream(s) */
         rtp_stop_fwd(osip_message_get_call_id(request), DIR_INCOMING, urlmap[index].wan_port);
         rtp_stop_fwd(osip_message_get_call_id(request), DIR_OUTGOING, urlmap[index].wan_port);
      }

      break;
   
   default:
      DEBUGC(DBCLASS_PROXY, "request [%s] from/to unregistered UA "
           "(RQ: %s@%s -> %s@%s)",
           request->sip_method? request->sip_method:"*NULL*",
       request->from->url->username? request->from->url->username:"*NULL*",
       request->from->url->host? request->from->url->host : "*NULL*",
       url->username? url->username : "*NULL*",
       url->host? url->host : "*NULL*");

/*
 * we may end up here for two reasons:
 *  1) An incomming request (from outbound) that is directed to
 *     an unknown (not registered) local UA
 *  2) an outgoing request from a local UA that is not registered.
 *
 * Case 1) we should probably answer with "404 Not Found",
 * case 2) more likely a "403 Forbidden"
 * 
 * How about "408 Request Timeout" ?
 *
 */
      printsip("Sip:proxy_request(), sip_gen_response(couldn't find dir).\r\n");
      sip_gen_response(ticket, 408 /* Request Timeout */);

      return STS_FAILURE;
   }


   /*
    * RFC 3261, Section 16.6 step 3
    * Proxy Behavior - Request Forwarding - Max-Forwards
    * (if Max-Forwards header exists, decrement by one, if it does not
    * exist, add a new one with value SHOULD be 70)
    */
   {
   osip_header_t *max_forwards;
   int forwards_count = DEFAULT_MAXFWD;
   char mfwd[8];

   osip_message_get_max_forwards(request, 0, &max_forwards);
   if (max_forwards == NULL) 
   {
      sprintf(mfwd, "%i", forwards_count);
      osip_message_set_max_forwards(request, mfwd);
   }
   else
   {
      if (max_forwards->hvalue) 
      {
         forwards_count = atoi(max_forwards->hvalue);
         forwards_count -=1;
         osip_free (max_forwards->hvalue);
      }

      sprintf(mfwd, "%i", forwards_count);
      max_forwards->hvalue = osip_strdup(mfwd);
   }

   DEBUGC(DBCLASS_PROXY,"setting Max-Forwards=%s",mfwd);
   }

   /*
    * RFC 3261, Section 16.6 step 4
    * Proxy Behavior - Request Forwarding - Add a Record-route header
    */

   /*
    * for ALL incoming requests, include my Record-Route header.
    * The local UA will probably send its answer to the topmost 
    * Route Header (8.1.2 of RFC3261)
    */
   if (type == REQTYP_INCOMING) 
   {
      DEBUGC(DBCLASS_PROXY,"Adding my Record-Route");
      /*w724v�Խӹ����з���hometalk app�ԽӲ��ɹ�����ΪRecord-Route�е�port�����ˣ�lan��Ӧ��Ϊ5060��wan��Ӧ��Ϊʵʱ����Ķ˿ڣ�56005-56nnn*/
      //route_add_recordroute(ticket);
   } 
   else 
   {
      /*
       * outgoing packets must not have my record route header, as
       * this likely will contain a private IP address (my inbound).
       */
      DEBUGC(DBCLASS_PROXY,"Purging Record-Routes (outgoing packet)");
      route_purge_recordroute(ticket);
   }

   /*
    * RFC 3261, Section 16.6 step 5
    * Proxy Behavior - Request Forwarding - Add Additional Header Fields
    */
   /* NOT IMPLEMENTED (optional) */


   /*
    * RFC 3261, Section 16.6 step 6
    * Proxy Behavior - Request Forwarding - Postprocess routing information
    *
    * If the copy contains a Route header field, the proxy MUST
    * inspect the URI in its first value.  If that URI does not
    * contain an lr parameter, the proxy MUST modify the copy as
    * follows:
    *
    * -  The proxy MUST place the Request-URI into the Route header
    *    field as the last value.
    *
    * -  The proxy MUST then place the first Route header field value
    *    into the Request-URI and remove that value from the Route
    *    header field.
    */
#if 0
   route_postprocess(ticket);
#endif

   /*
    * RFC 3261, Section 16.6 step 7
    * Proxy Behavior - Determine Next-Hop Address
    */
/*&&&& priority probably should be:
 * 1) Route header
 * 2) fixed outbound proxy
 * 3) SIP URI
 */
   /*
    * fixed or domain outbound proxy defined ?
    */
   if ((type == REQTYP_OUTGOING) &&
       (sip_find_outbound_proxy(ticket, &sendto_addr, &port) == STS_SUCCESS))
   {
      printsip("Sip: proxy_request: have outbound proxy %s:%i \r\n",
             utils_inet_ntoa(sendto_addr), port);
   /*
    * Route present?
    * If so, fetch address from topmost Route: header and remove it.
    */
   }
   else if ((type == REQTYP_OUTGOING) && 
              (request->routes && !osip_list_eol(request->routes, 0)))
   {
      //sts=route_determine_nexthop(ticket, &sendto_addr, &port);
      //Modify by z00182249:һ�߷���sip����·����ʧ���·��������Ͽɣ�ע��ɹ� 2013/12/24
      sts=route_get_nexthop(ticket, &sendto_addr, &port);
      if (sts == STS_FAILURE)
      {
         printsip("proxy_request: route_determine_nexthop failed \r\n");
         return STS_FAILURE;
      }
      printsip("Sip: proxy_request: have Route header to %s:%i \r\n",
             utils_inet_ntoa(sendto_addr), port);
   /*
    * destination from SIP URI
    */
   }
   else
   {
      /* get the destination from the SIP URI */
      printsip("Sip: Func:proxy_request(), pkt:dst url->host:[%s] \r\n", 
          url->host);
      if ((Hg522 != -1) 
#if UNREG_WITHOUT_REG_HG522
      && (NULL != urlmap[Hg522].to_ip) 
#endif
        && (type == REQTYP_OUTGOING))
      {
//          sts = get_ip_by_host(urlmap[Hg522].reg_url->host, &sendto_addr);
#if UNREG_WITHOUT_REG_HG522
          //inet_aton(urlmap[Hg522].to_ip, &sendto_addr);
          get_ip_by_host(urlmap[Hg522].to_ip, &sendto_addr);
          printsip("**** Sip: Func:proxy_request(hg522 OUT), urlmap[%d].to_ip:[%s]  reg_url:[%s]  pkt:dst url->host:[%s] \r\n", 
              Hg522, urlmap[Hg522].to_ip, urlmap[Hg522].reg_url->host, url->host);
#endif
      }
      else
      {
#if defined(SUPPORT_ATP_SIPPXY_RECORDDSTIP)
      int tmp = 0;
      tmp = !ValidateIpAddress(url->host);
      if ((!ValidateIpAddress(url->host)) && (0 != ticket->to.sin_addr.s_addr))//(type != REQTYP_INCOMING)
      {
         /* Request from SBC. */
         memcpy(&sendto_addr, &(ticket->to.sin_addr), sizeof(sendto_addr));             
         printsip("**** Sip: Func:proxy_request ValidateIpAddress[%s] is not ip-address. \r\n", url->host);
         sts = STS_SUCCESS;
      }
      else
      {
         printsip("**** Sip: Func:proxy_request ValidateIpAddress[%s] is an ip-address. \r\n", url->host);
         sts = get_ip_by_host(url->host, &sendto_addr);
      }
#else
      sts = get_ip_by_host(url->host, &sendto_addr);
#endif
          if (sts == STS_FAILURE)
          {
             printsip("Sip: proxy_request: cannot resolve URI [%s] \r\n",
                    url->host);
             return STS_FAILURE;
          }
          printsip("**** Sip: Func:proxy_request(OUT or hg522 IN), ip[%s], dir:[%d]", url->host, type);
      }

#if defined(SUPPORT_ATP_SIPPXY_RECORDDSTIP)
      if (REQTYP_INCOMING == type)
      {
         if (url->port)
         {
            port=atoi(url->port);
         }
         else
         {
            port=SIP_PORT;
         }
      }
      else
      {
            port=SIP_PORT;
      }
#else
      if (url->port)
      {
         port=atoi(url->port);
      }
      else
      {
        /* start of AU4D01412 �޸Ķ˿ں��ֱ�ת���� 5060 by d00107688 2009-02-18 */
		/* QGT: HOMEGW-8509 modify
        if (configuration.sip_listen_port)
        {
            port = configuration.sip_listen_port;
        }
        else
        {
            port=SIP_PORT;
        }
        */
        port=SIP_PORT;
		/* QGT: HOMEGW-8509 end */
        /* end of AU4D01412 �޸Ķ˿ں��ֱ�ת���� 5060 by d00107688 2009-02-18 */
      }
#endif
      printsip("Sip: proxy_request: have SIP URI sent to %s:%i \r\n",
             url->host, port);

      /* DT c40163 add for STUN */
      if (type == REQTYP_INCOMING && index >= 0) {
         if (urlmap[index].from.sin_addr.s_addr != 0) {
            sendto_addr = urlmap[index].from.sin_addr;
            port = urlmap[index].from.sin_port;
            printf("force stun: incoming request, ip=%s, port=%u\n",
				utils_inet_ntoa(sendto_addr), port);
         }
      }
      /* DT c40163 end */
   }

   /*
    * RFC 3261, Section 16.6 step 8
    * Proxy Behavior - Add a Via header field value
    */
   /* add my Via header line (outbound interface)*/
   if (type == REQTYP_INCOMING)
   {
      /*w724�Խӹ����з���hometalk app�����ܴ�lan��ip��viaͷ��*/
#if 0
      sts = sip_add_myvia(ticket, IF_INBOUND, NULL, 0);
      if (sts == STS_FAILURE)
      {
         ERROR("adding my inbound via failed!");
      }
#endif
   }
   else
   {
		sts = sip_add_myvia(ticket, IF_OUTBOUND, &local_ip, urlmap[index].wan_port, index);
        if (sts == STS_FAILURE)
        {
             ERROR("adding my outbound via failed!");
             return STS_FAILURE;
        }
		

    	pstTmpNode = (sip_via_node*)malloc(sizeof(sip_via_node));
        if (pstTmpNode == NULL)
        {
            return STS_FAILURE;    
        }

        memset(pstTmpNode, 0, sizeof(sip_via_node));
		//��call idת��Ϊ�ַ���
        via_i = osip_call_id_to_str (ticket->sipmsg->call_id, &tmp);
        if (via_i != 0)
        {
		    osip_free(pstTmpNode);
            return STS_FAILURE;
        }

        pstTmpNode->stSaveSipVia.callid_tmp= (char *)malloc(strlen(tmp)*sizeof(char));
        if (NULL == pstTmpNode->stSaveSipVia.callid_tmp)
        {
            osip_free (tmp);
		    osip_free(pstTmpNode);
            return STS_FAILURE;
        }
        memset(pstTmpNode->stSaveSipVia.callid_tmp, 0, strlen(tmp)*sizeof(char));
		//�洢���ĵ�call id
        snprintf(pstTmpNode->stSaveSipVia.callid_tmp, strlen(tmp)*sizeof(char), "%s", tmp);
        osip_free (tmp);
		//�洢sip���ĵ����к�
		pstTmpNode->stSaveSipVia.sip_csep = osip_atoi(ticket->sipmsg->cseq->number);
        
		//��Ϊǰ�潫����wan��via���˽������Ҽӵ����׸�λ��0������ɾ��lan���viaҪ��1��λ�ÿ�ʼɾ
		pos = 1;
		//��lan�������via��ɾ����ֻ�������ؼӵ�wan��via
        while ((!osip_list_eol (request->vias, pos)) && (pos < MAX_SIPVIA_NUM)) 
        {
		    //��ȡ�������via
            via = (osip_via_t *) osip_list_get (request->vias, pos);
			if (NULL == via)
			{
			    pos++;
				continue;
			}

            //��ȡvia���ip��ַ
            sts=get_ip_by_host(via->host, &addr_via);
            if (sts == STS_FAILURE)
            {
                printf("proxy_request: cannot resolve VIA [%s]\r\n",
                            via->host);
			    pos++;
			    continue;
            }
            else
            {
			    //��ȡvia���port
                if (via->port) 
                {
                    port_via=atoi(via->port);
                }

                if (port_via <= 0) 
                {
                    port_via=SIP_PORT;
                }
            }
			
			//��via����һ��
			if (0 != osip_via_clone(via, &via_temp))
			{
			    printf("clone via for %s fail\n", via->host);
				pos++;
				continue;
		    }
			
			if (NULL == via_temp)
			{
				pos++;
				continue;
			}
			
            /* remove my Via header line */
            sts = sip_del_via(ticket, &addr_via, port_via, pos);
            if (sts == STS_FAILURE) {
               printf("Delete VIA fail for %s\n", via->host);
            }
         
            pstTmpNode->stSaveSipVia.via[pos-1] = via_temp;
            pstTmpNode->stSaveSipVia.via_num++;
            pos++;
        }
        
		
		char *_via1;
		if (0 == osip_via_to_str (pstTmpNode->stSaveSipVia.via[0], &_via1))
		{
			    printsip("proxy_request: add via record to list:callid:[%s], sip_csep:[%d], via[%s], vianum[%d]\r\n", pstTmpNode->stSaveSipVia.callid_tmp, pstTmpNode->stSaveSipVia.sip_csep, _via1, pstTmpNode->stSaveSipVia.via_num);
			    osip_free (_via1);
        }

        /* ���ӽ����ӵ�����ͷ */
        pstTmpNode->pstNext = g_pstSipViaList;
        g_pstSipViaList = pstTmpNode;
        g_SipViaNum++;
        

        /* �������Ƶ�������ɾ�����ϵļ�¼�������ͷ������ֹ�������� */
        if (g_SipViaNum > MAX_SIPVIA_RECORD)
        {
            
            //��ȡviaȫ������ı�β��ɾ������ı�β
            pstCurNode = g_pstSipViaList;
            pstPreNode = g_pstSipViaList;
            
            while(NULL != pstCurNode)
            {
                pstPreNode = pstCurNode;
                pstCurNode = pstCurNode->pstNext;
                if (NULL != pstCurNode && NULL == pstCurNode->pstNext)
                {
                    break;
                }
            }
            if ((NULL != pstCurNode) && (NULL != pstPreNode))
        	{
			    char *_via;
			    if (0 == osip_via_to_str (pstCurNode->stSaveSipVia.via[0], &_via))
				{
			        printsip("delete the first via record:callid:[%s], sip_csep:[%d], via[%s], vianum[%d]\r\n", pstCurNode->stSaveSipVia.callid_tmp, pstCurNode->stSaveSipVia.sip_csep, _via, pstCurNode->stSaveSipVia.via_num);
					osip_free (_via);
        	    }
				if (NULL != pstCurNode->stSaveSipVia.callid_tmp) //��call id
                {
                    osip_free(pstCurNode->stSaveSipVia.callid_tmp);
                    pstCurNode->stSaveSipVia.callid_tmp = NULL;
                }
				
				FreeSipVia(&(pstCurNode->stSaveSipVia)); //���via
        		osip_free(pstCurNode);
                pstCurNode = NULL;
                pstPreNode->pstNext = NULL;
        	}

            g_SipViaNum--;
        }  
   }
  /*
   * RFC 3261, Section 16.6 step 9
   * Proxy Behavior - Add a Content-Length header field if necessary
   */
  /* not necessary, already in message and we do not support TCP */

  /*
   * RFC 3261, Section 16.6 step 10
   * Proxy Behavior - Forward the new request
   */
   sts = osip_message_to_str(request, &buffer);
   if (sts != 0)
   {
      ERROR("proxy_request: osip_message_to_str failed");
      return STS_FAILURE;
   }

   /* DT c40163 modify for QOS
   sipsock_send(sendto_addr, port, ticket->protocol,
                buffer, strlen(buffer)); 
   */
   sipsock_send(sendto_addr, port, ticket->protocol, buffer,
      strlen(buffer), is_t_online(request, index), type, ticket->tos, index); 
   /* DT c40163 end */

   printsip("\r\n Sip: Sent REQ Buff:[%s] \r\n", buffer);
   osip_free (buffer);

  /*
   * RFC 3261, Section 16.6 step 11
   * Proxy Behavior - Set timer C
   */
  /* NOT IMPLEMENTED - does this really apply for stateless proxies? */

   return STS_SUCCESS;
}


/*
 * PROXY_RESPONSE
 *
 * RETURNS
 *    STS_SUCCESS on success
 *    STS_FAILURE on error
 * RFC3261
 *    Section 16.7: Proxy Behavior - Response Processing
 *    1.  Find the appropriate response context
 *    2.  Update timer C for provisional responses
 *    3.  Remove the topmost Via
 *    4.  Add the response to the response context
 *    5.  Check to see if this response should be forwarded immediately
 *    6.  When necessary, choose the best final response from the
 *        response context
 *    7.  Aggregate authorization header field values if necessary
 *    8.  Optionally rewrite Record-Route header field values
 *    9.  Forward the response
 *    10. Generate any necessary CANCEL requests 
 *
 */
int proxy_response (sip_ticket_t *ticket, int fd) {
   int i = 0;
   int sts;
   int type = 0;
   struct in_addr sendto_addr, local_ip;
   osip_via_t *via;
   osip_via_t *via_tmp;
   int port;
   char *buffer;
   osip_message_t *response;
   struct sockaddr_in *from;          //  ����Դip��ַ
   int tmp_port = 0;
   int status_code = 0;
   struct timeval stTm;
   osip_header_t *expires_hdr;
   osip_uri_param_t *expires_param=NULL;
   int expires = 0;

   /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
   int index = -1;
    /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */

   sip_via_node *pstTmp = NULL;
   sip_via_node *pstTmp1 = NULL;
   int ViaNum = 0;
   int ViaNumTmp = 0;
   char *tmp = NULL;
   char *callid_tmp = NULL;
   int i_tmp = 0; 
   int sip_cseq_tmp = 0;
   osip_contact_t *contact = NULL;
   int pos = 0;

    
   DEBUGC(DBCLASS_PROXY,"proxy_response");

   if (ticket==NULL) {
      ERROR("proxy_response: called with NULL ticket");
      return STS_FAILURE;
   }

#ifdef SUPPORT_ATP_BRIDGE_BIND
   char acIpFrom[SIP_IPADDR] = {0};
   strcpy(acIpFrom, inet_ntoa(ticket->from.sin_addr));
   printsip("Sip: Response Src:[%s]\r\n", acIpFrom);
#endif

   response=ticket->sipmsg;
   from=&ticket->from;


   /*����socket�ҵ�urlmap���wan���ȥ�Ķ˿ں�*/
   if (fd != sip_udp_lan_socket)
   {
       for (i = 0; i < URLMAP_SIZE; i++)
       {
           if ((fd == urlmap[i].wan_socket_fd) && (1 == urlmap[i].active))
           {
               tmp_port = urlmap[i].wan_port;
               /*�����ж�type = RESTYP_INCOMING�е��Ǹ���response->from->url��urlmap����Ƚϣ���forking������»��Ҵ�
               ���ֱ��ͨ��socket���ж�index*/
               index = i;
               type=RESTYP_INCOMING;
               break;
           }
       }
   }
   else
   {
       /*LAN��ֻ����һ���˿�56005(�����ļ�),PC���е�WAN��5060���ض����˵��˶˿�*/
       tmp_port = configuration.sip_listen_port;
       DEBUGC(DBCLASS_PROXY,"response is from LAN");
   }

   memset(&local_ip, 0, sizeof(local_ip));
   if (i < URLMAP_SIZE)
   {
       get_local_ip(response->to->url->host, &local_ip, i);
   }
   
   /* Determine local address to use based on the To: address */
   printsip("Sip:response->to->url->host:[%s]  local IP(route): %s\n", 
       response->to->url->host,inet_ntoa(local_ip));
   DEBUGC(DBCLASS_PROXY, "local IP: %s", inet_ntoa(local_ip));

   /*
    * RFC 3261, Section 16.7 step 3
    * Proxy Behavior - Response Processing - Remove my Via header field value
    */

    if (ticket->direction == RESTYP_INCOMING)
	{
        //��call idת��Ϊ�ַ���
        i_tmp = osip_call_id_to_str (ticket->sipmsg->call_id, &tmp);
        if (i_tmp != 0 || NULL == tmp)
        {
    	    printf("osip_call_id_to_str fail\n");
            return STS_FAILURE;
        }
    	
        callid_tmp = (char *)malloc(strlen(tmp)*sizeof(char));
        if (NULL == callid_tmp)
        {
            osip_free (tmp);
            return STS_FAILURE;
        }
        memset(callid_tmp, 0, strlen(tmp)*sizeof(char));
    	//����call id
        snprintf(callid_tmp, strlen(tmp)*sizeof(char), "%s", tmp);
        osip_free (tmp); 
    	
    	sip_cseq_tmp = osip_atoi(ticket->sipmsg->cseq->number);

        pstTmp  = g_pstSipViaList;
    	pstTmp1 = pstTmp;
    	//ƥ��洢��call id�����кţ��Ա��ҵ���Ӧ�洢��via
        while (NULL != pstTmp)
    	{
		    char *_via;
			if (0 == osip_via_to_str (pstTmp->stSaveSipVia.via[0], &_via))
			{
			    printsip("proxy_response: find via record:callid:[%s], sip_csep:[%d], via[%s], vianum[%d]\r\n", pstTmp->stSaveSipVia.callid_tmp, pstTmp->stSaveSipVia.sip_csep, _via, pstTmp->stSaveSipVia.via_num);
			    osip_free (_via);
        	}
    	    if (0 == strncmp(callid_tmp, pstTmp->stSaveSipVia.callid_tmp, strlen(callid_tmp)))
        	{
    		    pstTmp1 = pstTmp;
    		    if (sip_cseq_tmp == pstTmp->stSaveSipVia.sip_csep)
    			{
				    printsip("proxy_response: have find the needed via record\r\n");
    			    break;
    			}
        	}

    		pstTmp = pstTmp->pstNext;
    	} 
    	
    	//���û��ƥ���ϣ�ȡ���һ��call idƥ���ϵ�
    	if (NULL == pstTmp)
        {
		    printsip("proxy_response: can't find via record,get the last via that callid is same\r\n");
    	    pstTmp = pstTmp1;
    	}
    	
    	osip_free(callid_tmp);

        if (NULL != pstTmp)
        {
        	ViaNum = pstTmp->stSaveSipVia.via_num;
    		ViaNumTmp = ViaNum;
        }
        
    	//��֮ǰ�洢��via�ӵ��ظ���lan��ı�����
        while(ViaNum)
        {	
    	    if (NULL == pstTmp->stSaveSipVia.via[ViaNum-1])
    		{
    			ViaNum--;
    			continue;
    		}
    		
    		//��via����һ��
    		if (0 != osip_via_clone(pstTmp->stSaveSipVia.via[ViaNum-1],  &via_tmp))
    		{
    		    printf("clone via for %s fail\n", pstTmp->stSaveSipVia.via[ViaNum-1]->host);
    			ViaNum--;
    			continue;
    		}
    		
			char *_via;
			if (0 == osip_via_to_str (via_tmp, &_via))
			{
			    printsip("proxy_response: add via record to packet:callid:[%s], sip_csep:[%d], clonevia[%s], vianum[%d]\r\n", pstTmp->stSaveSipVia.callid_tmp, pstTmp->stSaveSipVia.sip_csep, _via, pstTmp->stSaveSipVia.via_num);
			    osip_free (_via);
        	}
						
            osip_list_add(ticket->sipmsg->vias, via_tmp, 0);
    		ViaNum--;
        }
    }
       /* remove my Via header line */
   sts = sip_del_via(ticket, &local_ip, tmp_port, ViaNumTmp);
   if (sts == STS_FAILURE) {
      DEBUGC(DBCLASS_PROXY,"not addressed to my VIA, ignoring response");
      /*w724�Խӹ����з�������hometalk app�������ܴ�lan���ַ��viaͷ�����ȥ��local via��ʱ��ʧ��ҲҪ��������*/
      //return STS_FAILURE;
   }

   /*
    * figure out if this is an request coming from the outside
    * world to one of our registered clients
    */

   /* Ahhrghh...... a response seems to have NO contact information... 
    * so let's take FROM instead...
    * the TO and FROM headers are EQUAL to the request - that means 
    * they are swapped in their meaning for a response...
    */

#if 0 //_OLD_DIRECTION_EVALUATION
   type = 0;
   for (i=0; i<URLMAP_SIZE; i++) {
      if (urlmap[i].active == 0) continue;

      /* incoming response ('from' == 'masq') || ('from' == 'reg') */
      if ((compare_url(response->from->url, urlmap[i].reg_url)==STS_SUCCESS) ||
          (compare_url(response->from->url, urlmap[i].masq_url)==STS_SUCCESS)) {
         type=RESTYP_INCOMING;
         DEBUGC(DBCLASS_PROXY,"incoming response for %s@%s from outbound",
       response->from->url->username? response->from->url->username:"*NULL*",
       response->from->url->host? response->from->url->host : "*NULL*");
     break;
      }

      /* outgoing response ('to' == 'reg') || ('to' == 'masq' ) */
      if ((compare_url(response->to->url, urlmap[i].masq_url)==STS_SUCCESS) ||
          (compare_url(response->to->url, urlmap[i].reg_url)==STS_SUCCESS)){
         type=RESTYP_OUTGOING;
         DEBUGC(DBCLASS_PROXY,"outgoing response for %s@%s from inbound",
            response->from->url->username ?
                   response->from->url->username : "*NULL*",
            response->from->url->host ? 
                   response->from->url->host : "*NULL*");
     break;
      }
   }
#else
   /*��forking������»��ж��contactͷ�������Ҫ���Ҷ�Ӧ��contact���и�д*/
   pos = 0;
   while (!osip_list_eol (ticket->sipmsg->contacts, pos)) {
      contact = (osip_contact_t *) osip_list_get (ticket->sipmsg->contacts, pos);

      if (contact == NULL)
      {
          printsip("sip: proxy_request contact is NULL \r\n");
          continue;
      }

      for (i=0;i<URLMAP_SIZE;i++)
      {
        if (urlmap[i].active == 0) continue;
        if (((ticket->direction == REQTYP_OUTGOING) || (ticket->direction == RESTYP_OUTGOING)) &&
            (compare_url(contact->url, urlmap[i].true_url)==STS_SUCCESS)) {printsip("sip: proxy_request urlmap[%d] match \r\n", i);index = i;type=REQTYP_OUTGOING;break;}
        if (((ticket->direction == REQTYP_INCOMING) || (ticket->direction == RESTYP_INCOMING)) &&
            (compare_url(contact->url, urlmap[i].masq_url)==STS_SUCCESS)) {printsip("sip: proxy_request urlmap[%d] match \r\n", i);index = i;type=REQTYP_INCOMING;break;}
      }
      pos++;
   }


   if (type == 0)
   {
   /*
    * did I receive the telegram from a REGISTERED host?
    * -> it must be an OUTGOING response
    */
   for (i=0; i<URLMAP_SIZE; i++) 
   {
      struct in_addr tmp_addr;
      if (urlmap[i].active == 0) continue;

      if (get_ip_by_host(urlmap[i].true_url->host, &tmp_addr) == STS_FAILURE)
      {
         printsip("proxy_response: cannot resolve  urlmap[%d].true_url->host:[%s] \r\n",
             i, urlmap[i].true_url->host);
      } 
      else
      {
         DEBUGC(DBCLASS_PROXY, "proxy_response: reghost:%s ip:%s",
                urlmap[i].true_url->host, utils_inet_ntoa(from->sin_addr));
         if (memcmp(&tmp_addr, &from->sin_addr, sizeof(tmp_addr)) == 0
            && ((urlmap[i].true_url->port && atoi(urlmap[i].true_url->port) == from->sin_port)
               || (!urlmap[i].true_url->port && 5060 == from->sin_port)))
         {
            type=RESTYP_OUTGOING;
          /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            index = i;
           /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            break;
         }
      }

	  /* DT c40163 add for STUN */
      if (urlmap[i].from.sin_addr.s_addr != 0
	  	&& memcmp(&(urlmap[i].from.sin_addr), &from->sin_addr, sizeof(struct in_addr)) == 0
	  	&& (urlmap[i].from.sin_port == from->sin_port))
      {
         type = RESTYP_OUTGOING;
         index = i;
         break;
      }
	  /* DT c40163 end */
   }
   }
   /*
    * is the telegram directed to an internal registered host?
    * -> it must be an INCOMING response
    */
   if (type == 0)
   {
      for (i=0; i<URLMAP_SIZE; i++)
      {
         if (urlmap[i].active == 0) continue;
         /* incoming response ('from' == 'masq') || ('from' == 'reg') */
         if ((compare_url(response->from->url, urlmap[i].reg_url)==STS_SUCCESS) ||
             (compare_url(response->from->url, urlmap[i].masq_url)==STS_SUCCESS)) 
         {
            type=RESTYP_INCOMING;
            /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            index = i;
            /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            break;
         }
      }
   }

   printsip("Sip: proxy_response(), type:[%d] 3 in, 4 out\r\n", type);
  // printsip("Sip: respons urlmap:[%d].trueip:%s, masqip:%s, regip:%s \r\n", 
       //i, urlmap[i].true_url->host, urlmap[i].masq_url->host, urlmap[i].reg_url->host);
   
/* &&&& Open Issue &&&&
   it has been seen with cross-provider calls that the FROM may be 'garbled'
   (e.g 1393xxx@proxy01.sipphone.com for calls made sipphone -> FWD)
   How can we deal with this? Should I take into consideration the 'Via'
   headers? This is the only clue I have, pointing to the *real* UA.
   Maybe I should put in a 'siproxd' ftag value to recognize it a header
   put in by myself
*/
   if ((type == 0) && (!osip_list_eol(response->vias, 0)))
   {
      osip_via_t *via;
      struct in_addr addr_via, addr_myself;
      int port_via, port_ua;

      /* get the via address */
      via = (osip_via_t *) osip_list_get (response->vias, 0);
      DEBUGC(DBCLASS_PROXY, "proxy_response: check via [%s] for "
             "registered UA",via->host);
      sts=get_ip_by_host(via->host, &addr_via);
      if (sts == STS_FAILURE)
      {
         printsip("proxy_response: cannot resolve VIA [%s]",
                via->host);
      } 
      else
      {

         for (i=0; i<URLMAP_SIZE; i++)
         {
            if (urlmap[i].active == 0) continue;
            /* incoming response (1st via in list points to a registered UA) */
            sts=get_ip_by_host(urlmap[i].true_url->host, &addr_myself);
            if (sts == STS_FAILURE)
            {
               DEBUGC(DBCLASS_DNS, "proxy_response: cannot resolve "
                      "true_url [%s]", via->host);
               continue;
            }

            port_via=0;
            if (via->port) port_via=atoi(via->port);
            if (port_via <= 0) port_via=SIP_PORT;

            port_ua=0;
            if (urlmap[i].true_url->port)
               port_ua=atoi(urlmap[i].true_url->port);
            if (port_ua <= 0) port_ua=SIP_PORT;

            DEBUGC(DBCLASS_BABBLE, "proxy_response: checking for registered "
                   "host [%s:%i] <-> [%s:%i]",
                   urlmap[i].true_url->host, port_ua,
                   via->host, port_via);

            if ((memcmp(&addr_myself, &addr_via, sizeof(addr_myself))==0) &&
                (port_via == port_ua))
            {
               type=RESTYP_INCOMING;
               /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
               index = i;
               /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
               break;
            }
         }
      }
   }
    
#endif

   //ticket->direction=type;
   type = ticket->direction;

   if ((index < 0) || (index >= URLMAP_SIZE))
   {
      printsip("in proxy_request urlmap index error\n");
      return STS_FAILURE;
   }

/*
 * ok, we got a response that we are allowed to process.
 */
   switch (type)
   {
  /*
   * from an external host to the internal masqueraded host
   */
   case RESTYP_INCOMING:
    
      //printsip("incoming response for %s@%s from outbound",
    //response->from->url->username? response->from->url->username:"*NULL*",
    //response->from->url->host? response->from->url->host : "*NULL*");

    DEBUGPC("come here, incoming packet......");
      /*
       * Response for INVITE - deal with RTP data in body and
       *                       start RTP proxy stream(s). In case
       *                       of a negative answer, stop RTP stream
       */
      if (MSG_IS_RESPONSE_FOR(response,"INVITE"))
      {
        DEBUGPC("response for invite...... code: %d", response ? response->status_code : -1);
         /* positive response, start RTP stream */
         if ((MSG_IS_STATUS_1XX(response)) || 
              (MSG_IS_STATUS_2XX(response)))
         {
            if (configuration.rtp_proxy_enable == 1)
            {
               urlmap[index].incall = ALG_TRUE;
               printsip("Sip: proxy_request: urlmap[i].wan_port %d \r\n", urlmap[index].wan_port);

               DEBUGC(DBCLASS_PROXY,"here");
               /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
#if 0
               sts = proxy_rewrite_invitation_body(response, DIR_INCOMING, NULL);
#else
                /* DT c40163 modify for STUN
                sts = proxy_rewrite_invitation_body(index, response, DIR_INCOMING, NULL);
                */
                sts = proxy_rewrite_invitation_body(index, response, DIR_INCOMING, NULL, 0, urlmap[index].wan_port);
                /* DT c40163 end */
#endif
                /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
            }
         /* negative - stop a possibly started RTP stream */
         } 
         else if ((MSG_IS_STATUS_4XX(response))  ||
                     (MSG_IS_STATUS_5XX(response)) ||
                     (MSG_IS_STATUS_6XX(response)))
         {
            urlmap[index].incall = ALG_FALSE;
            DEBUGPC("MSG_IS_STATUS_4XX(response)) rtp_stop_fwd");
            rtp_stop_fwd(osip_message_get_call_id(response), DIR_INCOMING, urlmap[index].wan_port);
            rtp_stop_fwd(osip_message_get_call_id(response), DIR_OUTGOING, urlmap[index].wan_port);
         }
      } /* if INVITE */

      /*
       * Response for REGISTER - special handling of Contact header
       */
      if (MSG_IS_RESPONSE_FOR(response,"REGISTER")) 
      {
        DEBUGPC("response for register ");
         /*
          * REGISTER returns *my* Contact header information.
          * Rewrite Contact header back to represent the true address.
          * Other responses do return the Contact header of the sender.
          */
         //����200 OK��expire����url_map��expire
         if (MSG_IS_STATUS_2XX(response))
         {
             //contact header��expire������ expire header���ȼ���
             osip_message_get_expires(ticket->sipmsg, 0, &expires_hdr);
             if (ticket->sipmsg->contacts && ticket->sipmsg->contacts->node &&
                   ticket->sipmsg->contacts->node->element)
             {
                 osip_contact_param_get_byname(
                      (osip_contact_t*) ticket->sipmsg->contacts->node->element,
                      EXPIRES, &expires_param);
                 /*ims�п�����200ok��Ӧ��contactû�н���Ӧ��register�����е�contact�ŵ���һ����
                 ����������forking�Ŀͻ��˵�contact�ŵ���һ���ˣ������Ҫ���в��Ҷ�Ӧ��contact*/
                 int pos = 0;
                 while (!osip_list_eol (ticket->sipmsg->contacts, pos)) {
                    osip_contact_t* contact_tmp = NULL;
                    contact_tmp = (osip_contact_t *) osip_list_get (ticket->sipmsg->contacts, pos);
                    if (compare_url(contact_tmp->url, urlmap[index].masq_url)==STS_SUCCESS)
                    {
                        DEBUGPC("proxy_response: UPDATE urlmap[%d] pos=[%d]", index, pos);
                        osip_contact_param_get_byname(contact_tmp, EXPIRES, &expires_param);
                        break;
                    }
                    pos++;
                 }
             }
             if (expires_param && expires_param->gvalue) 
             {
                /* get expires from contact Header */
                expires = atoi(expires_param->gvalue);
             }
             else if (expires_hdr && expires_hdr->hvalue)
             {
                /* get expires from expires Header */
                expires = atoi(expires_hdr->hvalue);
             }
             DEBUGPC("proxy_response: UPDATE urlmap[%d] expires=[%d]", index, expires);
             ATP_UTIL_GetSysTime(&stTm);
             urlmap[index].expires = stTm.tv_sec + expires;
         }
         /*��forking������£���Ҫ����û�и�ip��wan���contact���ж���һ��urlmap��Ҫˢ�£�
         ���sip_rewrite_contact�ŵ�ˢ��expires�����*/
         sip_rewrite_contact(ticket, DIR_INCOMING, NULL, index);
      }

      /* 
       * Response for SUBSCRIBE
       *
       * HACK for Grandstream SIP phones (with newer firmware like 1.0.4.40):
       *   They send a SUBSCRIBE request to the registration server. In
       *   case of beeing registering directly to siproxd, this request of
       *   course will eventually be forwarded back to the same UA.
       *   Grandstream then does reply with an '202' response (A 202
       *   response merely indicates that the subscription has been
       *   understood, and that authorization may or may not have been
       *   granted), which then of course is forwarded back to the phone.
       *   Ans it seems that the Grandstream can *not* *handle* this
       *   response, as it immediately sends another SUBSCRIBE request.
       *   And this games goes on and on and on...
       *
       *   As a workaround we will transform any 202 response to a
       *   '404 unknown destination'
       *   
       */
{
      osip_header_t *ua_hdr=NULL;
      osip_message_get_user_agent(response, 0, &ua_hdr);
      if (ua_hdr && ua_hdr->hvalue &&
          (osip_strncasecmp(ua_hdr->hvalue,"grandstream", 11)==0) &&
          (MSG_IS_RESPONSE_FOR(response,"SUBSCRIBE")) &&
          (MSG_TEST_CODE(response, 202)))
      {
         DEBUGC(DBCLASS_PROXY, "proxy_response: Grandstream hack 202->404");
         response->status_code=404;
      }
}
      break;
   
  /*
   * from the internal masqueraded host to an external host
   */
   case RESTYP_OUTGOING:
      printsip("outgoing response for %s@%s from inbound",
         response->from->url->username ?
                response->from->url->username : "*NULL*",
         response->from->url->host ? 
                response->from->url->host : "*NULL*");

      /* rewrite Contact header to represent the masqued address */
      sip_rewrite_contact(ticket, DIR_OUTGOING, NULL, index);

      /*
       * If an 2xx OK or 1xx response, answer to an INVITE request,
       * rewrite body
       *
       * In case of a negative answer, stop RTP stream
       */
      if (MSG_IS_RESPONSE_FOR(response,"INVITE")) {
         /* positive response, start RTP stream */
         if ((MSG_IS_STATUS_1XX(response)) || 
              (MSG_IS_STATUS_2XX(response))) {
             /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
#if 0
            /* This is an outgoing response, therefore an outgoing stream */
            sts = proxy_rewrite_invitation_body(response, DIR_OUTGOING, NULL);
#else
            printsip("Sip: proxy_request: urlmap[i].wan_port %d \r\n", urlmap[index].wan_port);
            urlmap[index].incall = ALG_TRUE;

            /* DT c40163 modify for STUN
            sts = proxy_rewrite_invitation_body(index, response, DIR_OUTGOING, NULL);
            */
            sts = proxy_rewrite_invitation_body(index, response, DIR_OUTGOING, NULL, &(from->sin_addr), urlmap[index].wan_port);
            /* DT c40163 end */
#endif
            /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
         /* megative - stop a possibly started RTP stream */
         } else if ((MSG_IS_STATUS_4XX(response))  ||
                     (MSG_IS_STATUS_5XX(response)) ||
                     (MSG_IS_STATUS_6XX(response))) {
                     
                     urlmap[index].incall = ALG_FALSE;
                     DEBUGPC("MSG_IS_STATUS_4XX(response)) rtp_stop_fwd");
            rtp_stop_fwd(osip_message_get_call_id(response), DIR_INCOMING, urlmap[index].wan_port);
            rtp_stop_fwd(osip_message_get_call_id(response), DIR_OUTGOING, urlmap[index].wan_port);
         }
      } /* if INVITE */

      break;
   
   default:
      DEBUGC(DBCLASS_PROXY, "response from/to unregistered UA (%s@%s)",
       response->from->url->username? response->from->url->username:"*NULL*",
       response->from->url->host? response->from->url->host : "*NULL*");
      return STS_FAILURE;
   }

   /*
    * for ALL incoming response include my Record-Route header.
    * The local UA will probably send its answer to the topmost 
    * Route Header (8.1.2 of RFC3261)
    */
    if (type == RESTYP_INCOMING) {
       DEBUGC(DBCLASS_PROXY,"Adding my Record-Route");
       printsip("Sip: incoming not add route. %s, %d \r\n", __FILE__, __LINE__);
       /*w724v�Խӹ����з���hometalk app�ԽӲ��ɹ�����ΪRecord-Route�е�port�����ˣ�lan��Ӧ��Ϊ5060��wan��Ӧ��Ϊʵʱ����Ķ˿ڣ�56005-56nnn*/
       //route_add_recordroute(ticket);
    } else {
       /*
        * outgoing packets must not have my record route header, as
        * this likely will contain a private IP address (my inbound).
        */
       DEBUGC(DBCLASS_PROXY,"Purging Record-Routes (outgoing packet)");
       route_purge_recordroute(ticket);
    }

   /*
    * Determine Next-Hop Address
    */
/*&&&& priority probably should be:
 * 1) Route header
 * 2) fixed outbound proxy
 * 3) SIP URI
 */
   /*
    * check if we need to send to an outbound proxy
    */
   if ((type == RESTYP_OUTGOING) &&
       (sip_find_outbound_proxy(ticket, &sendto_addr, &port) == STS_SUCCESS)) 
   {
      DEBUGC(DBCLASS_PROXY, "proxy_response: have outbound proxy %s:%i",
             utils_inet_ntoa(sendto_addr), port);
   /*
    * Route present?
    * If so, fetch address from topmost Route: header and remove it.
    */
   }
   else if ((type == RESTYP_OUTGOING) && 
              (response->routes && !osip_list_eol(response->routes, 0)))
   {
      //Modify by z00182249:һ�߷���sip����·����ʧ���·��������Ͽɣ�ע��ɹ� 2013/12/24
      sts=route_get_nexthop(ticket, &sendto_addr, &port);
      if (sts == STS_FAILURE) {
         DEBUGC(DBCLASS_PROXY, "proxy_response: route_determine_nexthop failed");
         return STS_FAILURE;
      }
      DEBUGC(DBCLASS_PROXY, "proxy_response: have Route header to %s:%i",
             utils_inet_ntoa(sendto_addr), port);
   } 
   else
   {
      /* get target address and port from VIA header */
      via = (osip_via_t *) osip_list_get (response->vias, 0);
      if (via == NULL)
      {
         ERROR("proxy_response: list_get via failed");
         return STS_FAILURE;
      }

      sts = get_ip_by_host(via->host, &sendto_addr);
      if (sts == STS_FAILURE)
      {
         DEBUGC(DBCLASS_PROXY, "proxy_response: cannot resolve VIA [%s]",
                via->host);
         return STS_FAILURE;
      }

      if (via->port)
      {
         port=atoi(via->port);
      }
      else
      {
         port=SIP_PORT;
      }

      /* DT c40163 add for STUN */
	  if (type == RESTYP_INCOMING && index >= 0) {
         if (urlmap[index].from.sin_addr.s_addr != 0) {
		    sendto_addr = urlmap[index].from.sin_addr;
			port = urlmap[index].from.sin_port;
			printf("force stun: incoming response, ip=%s, port=%u\n",
				utils_inet_ntoa(sendto_addr), port);
         }
      }
      /* DT c40163 end */
   }

   sts = osip_message_to_str(response, &buffer);
   if (sts != 0) {
      ERROR("proxy_response: osip_message_to_str failed");
      return STS_FAILURE;
   }

   /* DT c40163 modify for QOS
   sipsock_send(sendto_addr, port, ticket->protocol,
                buffer, strlen(buffer)); 
   */
   type = ticket->direction;
   sipsock_send(sendto_addr, port, ticket->protocol, buffer,
      strlen(buffer), is_t_online(response, index), type, ticket->tos, index); 
   /* DT c40163 end */
   
   //�ж���WAN�������status code 400~699,�����Ƕ�REGISTER��response,ɾurlmap
   //˵��PCע��ʧ��,��map������
   //status_code = osip_message_get_status_code(response);
   //printsip("proxy RESPONSE CODE=[%d]\r\n", status_code);
   if ((RESTYP_INCOMING == type) && (index >= 0) && (MSG_IS_RESPONSE_FOR(response,"REGISTER")))
   {
       status_code = osip_message_get_status_code(response);
       if ((400 <= status_code) && (status_code <= 699))
       {
           /*urlmap[i].expires������������Ϊ0�������յ�401��Ҫ��Ȩʱ��
           �������Ȩ��Ϣ�ĵڶ���registerû�м�ʱ�յ�����û���������ĵ�ʱ��urlmap�ᱻregister_agemap�����
           �Ӷ��ڶ��ε�register�����wan�˿�56nnn�����·�������Ϊ����ˢ�µ�ע����Ƕ�Ӧ��Ȩ����ע�ᣬ�ظ�403*/
           time_t time_now;
           ATP_UTIL_GetSysTime(&stTm);
           time_now = stTm.tv_sec;
           /* update registration timeout */
           urlmap[i].expires=time_now+32;
       }
   }
   
   printsip("\r\n Sip: Sent RES Buff:[%s] \r\n", buffer);
   osip_free (buffer);
   return STS_SUCCESS;
}


/*
 * PROXY_REWRITE_INVITATION_BODY
 *
 * rewrites the outgoing INVITATION request or response packet
 * 
 * RETURNS
 *    STS_SUCCESS on success
 *    STS_FAILURE on error
 */
/* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
#if 0
int proxy_rewrite_invitation_body(osip_message_t *mymsg, int direction,
                  struct in_addr *local_ip)
#else
/* DT c40163 modify
int proxy_rewrite_invitation_body(int idex, osip_message_t *mymsg, int direction,
                  struct in_addr *local_ip)
*/
int proxy_rewrite_invitation_body(int idex, osip_message_t *mymsg, int direction,
                  struct in_addr *local_ip, struct in_addr *from_ip, int wan_port)
/* DT c40163 end */
#endif
/* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
{
   osip_body_t *body;
   sdp_message_t  *sdp;
   struct in_addr map_addr, addr_sess, addr_media, outside_addr, inside_addr;
   int sts;
   char *bodybuff;
   char clen[8]; /* content length: probably never more than 7 digits !*/
   int map_port, msg_port;
   int media_stream_no;
   sdp_connection_t *sdp_conn;
   sdp_media_t *sdp_med;
   int rtp_direction=0;
   int have_c_media=0;

   DEBUGPC("use to rewirte the invite body: %s \r\n", local_ip == NULL ? "local_ip is NULL":inet_ntoa(*local_ip));

   /* start of AU4D00864 by d00107688 2008-10-17 ���HG323��SIP�绰���÷ǿ����޷�ͨ������ */
    char acSdpCaddr[HOSTNAME_SIZE] = {0};
    /* end of AU4D00864 by d00107688 2008-10-17 ���HG323��SIP�绰���÷ǿ����޷�ͨ������ */

   if (configuration.rtp_proxy_enable == 0) return STS_SUCCESS;

   /*
    * get SDP structure
    */
   sts = osip_message_get_body(mymsg, 0, &body);
   if (sts != 0)
   {
      if ((MSG_IS_RESPONSE_FOR(mymsg,"INVITE")) &&
          (MSG_IS_STATUS_1XX(mymsg))) {
         /* 1xx responses *MAY* contain SDP data */
         DEBUGC(DBCLASS_PROXY, "rewrite_invitation_body: "
                "no body found in message");
         return STS_SUCCESS;
      } else {
         /* INVITE request and 200 response *MUST* contain SDP data */
         ERROR("rewrite_invitation_body: no body found in message");
         return STS_FAILURE;
      }
   }

   sts = osip_body_to_str(body, &bodybuff);
   if (sts != 0) {
      ERROR("rewrite_invitation_body: unable to sip_body_to_str");
   }

   //  sdp ָ������ڴ棬����invite��sdp ��
   sts = sdp_message_init(&sdp);
   sts = sdp_message_parse (sdp, bodybuff);
   osip_free(bodybuff);
   if (sts != 0) {
      ERROR("rewrite_invitation_body: unable to sdp_message_parse body");
      sdp_message_free(sdp);
      return STS_FAILURE;
   }


if (configuration.debuglevel)
{ /* just dump the buffer */
   char *tmp, *tmp2;
   sts = osip_message_get_body(mymsg, 0, &body);
   sts = osip_body_to_str(body, &tmp);
   osip_content_length_to_str(mymsg->content_length, &tmp2);
   DEBUG("Body before rewrite (clen=%s, strlen=%i):\n%s\n----",
         tmp2, strlen(tmp), tmp);
   osip_free(tmp);
   osip_free(tmp2);
}

   /*
    * RTP proxy: get ready and start forwarding
    * start forwarding for each media stream ('m=' item in SIP message)
    */

   /* get outbound address */
#if 0
   if (get_ip_by_ifname(configuration.outbound_if, &outside_addr) != 
       STS_SUCCESS) {
      ERROR("can't find outbound interface %s - configuration error?",
            configuration.outbound_if);
      sdp_message_free(sdp);
      return STS_FAILURE;
   }
#else
   memcpy(&outside_addr, &urlmap[idex].outbound_addr, sizeof(outside_addr));
#endif

   if (local_ip == NULL) {
      /* get inbound address */
      if (get_ip_by_ifname(configuration.inbound_if, &inside_addr) !=
          STS_SUCCESS) {
         ERROR("can't find inbound interface %s - configuration error?",
               configuration.inbound_if);
         sdp_message_free(sdp);
         return STS_FAILURE;
      }
      /* start of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */
      DEBUGPC("@@@@@@@@@@@@@@@@@@@ get inside_addr: %s", inet_ntoa(inside_addr));

     /* add by d00107688 ����Ǵ�����ô��Ҫѡ����ʵ�ppp�ӿ� */
     if ((1 == g_lOutIFaceIsProxy) && (strcmp(configuration.inbound_if, "br0") == 0)
        && (-1 != idex) && (NULL != urlmap[idex].true_url) 
        && (NULL != urlmap[idex].true_url->host) && (direction == DIR_INCOMING))
     {
         FILE *fp = NULL;
         char string[256] = {0};
         char tmpstr[9][32] = {{0}};
         int arrlen = 9 * 32;
         char *host = NULL;
         struct in_addr hostip;
         system("ip route ls > /var/tmproute");
         fp = fopen("/var/tmproute", "r");
         if (fp == NULL)
         {
             DEBUGPC("open file tmproute error");
         }
         else
         {
             while (NULL != fgets(string, 256, fp))
             {
                DEBUGPC("%s", string);
                 memset(tmpstr, 0, arrlen);
                 /* 192.168.239.12 dev ppp3  proto kernel  scope link  src 192.168.239.4  */
                 sscanf(string, "%s %s %s  %s %s  %s %s  %s %s ", 
                     tmpstr[0], tmpstr[1], tmpstr[2], tmpstr[3], tmpstr[4],
                     tmpstr[5], tmpstr[6], tmpstr[7], tmpstr[8]);
                 
                 get_ip_by_host(urlmap[idex].true_url->host, &hostip);
                 host = inet_ntoa(hostip);
                 if (0 == strncmp(tmpstr[0], host, strlen(host)))
                 {
                     DEBUGPC("\r\n\r\n notice: get the interface: %s true: %s\r\n\r\n\r\n", 
                        tmpstr[8], urlmap[idex].true_url->host);
                     inet_aton(tmpstr[8], &inside_addr);
                 }
                 memset(string, 0, 256);
             }
             fclose(fp);
             unlink("/var/tmproute");
         }
     }
    /* end of AU4D01936 by d00107688 2009-06-19 sip�绰ͨ��pppoe proxy�������ػ�ȡ��ַ��sip alg������ͨ�������У����ؿ��˿ڳ�������RTP�����ܻ�ͨ */

      
   } else {
      outside_addr = *local_ip;
   }

   DEBUGPC("@@@@@@@@@@@@@@@@@@@ get inside_addr: %s", inet_ntoa(inside_addr));

   /* figure out what address to use for RTP masquerading */
   if (MSG_IS_REQUEST(mymsg)) {
      if (direction == DIR_INCOMING) {
         memcpy(&map_addr, &inside_addr, sizeof (map_addr));
         rtp_direction = DIR_OUTGOING;
      } else {
         memcpy(&map_addr, &outside_addr, sizeof (map_addr));
         rtp_direction = DIR_INCOMING;
      }
   } else /* MSG_IS_REPONSE(mymsg) */ {
      if (direction == DIR_INCOMING) {
         memcpy(&map_addr, &inside_addr, sizeof (map_addr));
         rtp_direction = DIR_OUTGOING;
      } else {
         memcpy(&map_addr, &outside_addr, sizeof (map_addr));
         rtp_direction = DIR_INCOMING;
      }
   }

   DEBUGPC("proxy_rewrite_invitation_body: SIP[%s %s] RTP[%s %s]",
          MSG_IS_REQUEST(mymsg)? "RQ" : "RS",
          (direction==DIR_INCOMING)? "IN" : "OUT",
          (rtp_direction==DIR_INCOMING)? "IN" : "OUT",
          utils_inet_ntoa(map_addr));

   /*
    * first, check presence of a 'c=' item on session level
    */
   if (sdp->c_connection==NULL || sdp->c_connection->c_addr==NULL) {
      /*
       * No 'c=' on session level, search on media level now
       *
       * According to RFC2327, ALL media description must
       * include a 'c=' item now:
       */
      media_stream_no=0;
      while (!sdp_message_endof_media(sdp, media_stream_no))
      {
         /* check if n'th media stream is present */
         if (sdp_message_c_addr_get(sdp, media_stream_no, 0) == NULL)
         {
            ERROR("SDP: have no 'c=' on session level and neither "
                  "on media level (media=%i)",media_stream_no);
            sdp_message_free(sdp);
            return STS_FAILURE;
         }
         media_stream_no++;             /* ͳ�Ƴ����忪�˼��� ý�� �˿� */
      } /* while */
   }

   /* Required 'c=' items ARE present */


   /*
    * rewrite 'c=' item on session level if present and not yet done.
    * remember the original address in addr_sess
    */
   memset(&addr_sess, 0, sizeof(addr_sess));
   if (sdp->c_connection && sdp->c_connection->c_addr) {
      sts = get_ip_by_host(sdp->c_connection->c_addr, &addr_sess);
      if (sts == STS_FAILURE) {
         ERROR("SDP: cannot resolve session 'c=' host [%s]",
               sdp->c_connection->c_addr);
         sdp_message_free(sdp);
         return STS_FAILURE;
      }
      /*
       * Rewrite
       * an IP address of 0.0.0.0 means *MUTE*, don't rewrite such
       */
      /*&&&& should use gethostbyname here */
      if (strcmp(sdp->c_connection->c_addr, "0.0.0.0") != 0) {

        /* start of AU4D00864 by d00107688 2008-10-17 ���HG323��SIP�绰���÷ǿ����޷�ͨ������ */
        {
            memcpy(acSdpCaddr, sdp->c_connection->c_addr, HOSTNAME_SIZE);
            acSdpCaddr[15] = '\0';            
        }
        /* end of AU4D00864 by d00107688 2008-10-17 ���HG323��SIP�绰���÷ǿ����޷�ͨ������ */
        
         osip_free(sdp->c_connection->c_addr);
         sdp->c_connection->c_addr=osip_malloc(HOSTNAME_SIZE);
         sprintf(sdp->c_connection->c_addr, "%s", utils_inet_ntoa(map_addr));
      } else {
         /* 0.0.0.0 - don't rewrite */
         DEBUGC(DBCLASS_PROXY, "proxy_rewrite_invitation_body: "
                "got a MUTE c= record (on session level - legal?)");
      }
   }


   /*
    * rewrite 'o=' item (originator) on session level if present.
    */
   if (sdp->o_addrtype && sdp->o_addr) {
      if (strcmp(sdp->o_addrtype, "IP4") != 0) {
         ERROR("got IP6 in SDP originator - not yet suported by siproxd");
         sdp_message_free(sdp);
         return STS_FAILURE;
      }

      osip_free(sdp->o_addr);
      sdp->o_addr=osip_malloc(HOSTNAME_SIZE);
      sprintf(sdp->o_addr, "%s", utils_inet_ntoa(map_addr));
   }


   /* �ڴ˴��������ڶ˿�
    * loop through all media descritions,
    * start RTP proxy and rewrite them
    */
   for (media_stream_no=0;;media_stream_no++)
   {
      /* check if n'th media stream is present */
      if (sdp_message_m_port_get(sdp, media_stream_no) == NULL) break;

      /*
       * check if a 'c=' item is present in this media description,
       * if so -> rewrite it
       */
      memset(&addr_media, 0, sizeof(addr_media));
      have_c_media=0;
      sdp_conn=sdp_message_connection_get(sdp, media_stream_no, 0);

      //  ��media & audio �绰��ͨ���˴� sdp_conn = NULL
      if (sdp_conn && sdp_conn->c_addr)
      {
         /*&&&& should use gethostbyname here as well */
         if (strcmp(sdp_conn->c_addr, "0.0.0.0") != 0)
         {
            sts = get_ip_by_host(sdp_conn->c_addr, &addr_media);
            have_c_media=1;
            /* have a valid address */
            osip_free(sdp_conn->c_addr);
            sdp_conn->c_addr=osip_malloc(HOSTNAME_SIZE);
            sprintf(sdp_conn->c_addr, "%s", utils_inet_ntoa(map_addr));
         } 
         else
         {
            /* 0.0.0.0 - don't rewrite */
            DEBUGC(DBCLASS_PROXY, "proxy_rewrite_invitation_body: got a "
                   "MUTE c= record (media level)");
         }
      }

      /* start an RTP proxying stream */
      if (sdp_message_m_port_get(sdp, media_stream_no))
      {
         msg_port=atoi(sdp_message_m_port_get(sdp, media_stream_no));
         if (msg_port > 0)
         {
            osip_uri_t *cont_url = NULL;
            char *client_id=NULL;
            /* try to get some additional UA specific unique ID.
             * Try:
             * 1) User part of Contact header
             * 2) Host part of Contact header (will be different
             *    between internal UA and external UA)
             */
            if (!osip_list_eol(mymsg->contacts, 0))
               cont_url = ((osip_contact_t*)(mymsg->contacts->node->element))->url;
            if (cont_url)
            {
               client_id=cont_url->username;

               /* start of AU4D00864 by d00107688 2008-10-17 ���HG323��SIP�绰���÷ǿ����޷�ͨ������ */
               if (client_id == NULL)
                {
                    client_id=cont_url->host;

                    if (client_id == NULL)
                    {
                        client_id = acSdpCaddr;
                    }
                }
            }
            else
            {
                 client_id = acSdpCaddr;
            }
            /* end of AU4D00864 by d00107688 2008-10-17 ���HG323��SIP�绰���÷ǿ����޷�ͨ������ */


            /*
             * do we have a 'c=' item on media level?
             * if not, use the same as on session level
             */
            if (have_c_media == 0)
            {
               //  ��� m= ѡ������ c_connection���ȡ��ֵ
               //  ����ȡ  c= ѡ���е� connection addressֵ
               memcpy(&addr_media, &addr_sess, sizeof(addr_sess));
            }

			/* DT c40163 add for STUN */
			if (direction == DIR_OUTGOING && from_ip != 0) {
				if (!memcmp(&addr_media, &outside_addr, sizeof(struct in_addr))) {
					memcpy(&addr_media, from_ip, sizeof(struct in_addr));
					printf("force stun: modify media addr, ip=%s, port=%u\n",
						utils_inet_ntoa(*from_ip), msg_port);
				}
			}
			/* DT c40163 end */

			/* DT c40163 modify for QOS
            sts = rtp_start_fwd(osip_message_get_call_id(mymsg),  //  call id ֵ
                                client_id,         //  Ӧ����  contact �е�usernameֵ
                                                   //  ��� usernameΪ�գ���Ϊ hostֵ
                                rtp_direction,     //  Ӧ����  in
                                media_stream_no,   //  ��0��ʼ����
                                map_addr,          //  ����dir=out ��Ϊwan���ַ
                                &map_port,         //  �������Ĳ���
                                addr_media,    // ���� c= ѡ���е�connection addressֵ
                                msg_port           //  m= ѡ���е�portֵ
                                );
            */
            sts = rtp_start_fwd(osip_message_get_call_id(mymsg),  //  call id ֵ
                                client_id,         //  Ӧ����  contact �е�usernameֵ
                                                   //  ��� usernameΪ�գ���Ϊ hostֵ
                                rtp_direction,     //  Ӧ����  in
                                media_stream_no,   //  ��0��ʼ����
                                map_addr,          //  ����dir=out ��Ϊwan���ַ
                                &map_port,         //  �������Ĳ���
                                addr_media,    // ���� c= ѡ���е�connection addressֵ
                                msg_port,           //  m= ѡ���е�portֵ
                                is_t_online(mymsg, idex),
                                wan_port
                                );
			/* DT c40163 end */

            if (sts == STS_SUCCESS)
            {
               /* and rewrite the port */
               //  sdp_med ָ��ǰ media_stream_no ��Ӧ m= ָ��
               sdp_med=osip_list_get(sdp->m_medias, media_stream_no);
               if (sdp_med && sdp_med->m_port) 
               {
                  osip_free(sdp_med->m_port);
                  sdp_med->m_port=osip_malloc(8); /* 5 digits, \0 + align */
                  sprintf(sdp_med->m_port, "%i", map_port);
                  DEBUGC(DBCLASS_PROXY, "proxy_rewrite_invitation_body: "
                         "m= rewrote port to [%i]",map_port);
               }
               else
               {
                  ERROR("rewriting port in m= failed sdp_med=%p, "
                        "m_number_of_port=%p", sdp_med, sdp_med->m_port);
               }
            } /* sts == success */
         } /* if msg_port > 0 */
      }
      else
      {
         /* no port defined - skip entry */
         WARN("no port defined in m=(media) stream_no=%i", media_stream_no);
         continue;
      }
   } /* for media_stream_no */

   /* remove old body */
   sts = osip_list_remove(mymsg->bodies, 0);
   osip_body_free(body);

   /* dump new body */
   sdp_message_to_str(sdp, &bodybuff);

   /* free sdp structure */
   sdp_message_free(sdp);

   /* include new body */
   osip_message_set_body(mymsg, bodybuff);

   /* free content length resource and include new one*/
   osip_content_length_free(mymsg->content_length);
   mymsg->content_length=NULL;
   sprintf(clen,"%i",strlen(bodybuff));
   sts = osip_message_set_content_length(mymsg, clen);

   /* free old body */
   osip_free(bodybuff);

if (configuration.debuglevel)
{ /* just dump the buffer */
   char *tmp, *tmp2;
   sts = osip_message_get_body(mymsg, 0, &body);
   sts = osip_body_to_str(body, &tmp);
   osip_content_length_to_str(mymsg->content_length, &tmp2);
   DEBUG("Body after rewrite (clen=%s, strlen=%i):\n%s\n----",
         tmp2, strlen(tmp), tmp);
   osip_free(tmp);
   osip_free(tmp2);
}
   return STS_SUCCESS;
}


/*
 * PROXY_REWRITE_REQUEST_URI
 *
 * rewrites the incoming Request URI
 * 
 * RETURNS
 *    STS_SUCCESS on success
 */
int proxy_rewrite_request_uri(osip_message_t *mymsg, int idx){
   char *host;
   char *port;
   osip_uri_t *url;

   if ((idx >= URLMAP_SIZE) || (idx < 0)) {
      WARN("proxy_rewrite_request_uri: called with invalid index");
      return STS_FAILURE;
   }

   DEBUGC(DBCLASS_PROXY,"rewriting incoming Request URI");
   url=osip_message_get_uri(mymsg);

   /* set the true host */
   if (url->host) osip_free(url->host);url->host=NULL;
   if (urlmap[idx].true_url->host) {
      DEBUGC(DBCLASS_BABBLE,"proxy_rewrite_request_uri: host=%s",
             urlmap[idx].true_url->host);
      host = (char *)malloc(strlen(urlmap[idx].true_url->host)+1);
      memcpy(host, urlmap[idx].true_url->host, strlen(urlmap[idx].true_url->host));
      host[strlen(urlmap[idx].true_url->host)]='\0';
      osip_uri_set_host(url, host);
   }

   /* set the true port */
   if (url->port) osip_free(url->port);url->port=NULL;
   if (urlmap[idx].true_url->port) {
      DEBUGC(DBCLASS_BABBLE,"proxy_rewrite_request_uri: port=%s",
             urlmap[idx].true_url->port);
      port = (char *)malloc(strlen(urlmap[idx].true_url->port)+1);
      memcpy(port, urlmap[idx].true_url->port, strlen(urlmap[idx].true_url->port));
      port[strlen(urlmap[idx].true_url->port)]='\0';
      osip_uri_set_port(url, port);
   }
   return STS_SUCCESS;
}
