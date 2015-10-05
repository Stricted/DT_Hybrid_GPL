/*
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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <osipparser2/osip_parser.h>

#include "siproxd.h"
#include "log.h"

static char const ident[]="$Id: register.c,v 1.4 2009/06/19 10:31:00 y42304 Exp $";

/* configuration storage */
extern struct siproxd_config configuration;

struct urlmap_s urlmap[URLMAP_SIZE];		/* URL mapping table     */

extern int errno;
/*
 * initialize the URL mapping table
 */
void register_init(void) {
   FILE *stream;
   int sts, size, i;
   char buff[128];

   memset(urlmap, 0, sizeof(urlmap));

   printsip("**** init urlmap:[%d] \r\n", 
       sizeof(urlmap));
   //初始化每个map的wan socket和port
   for (i = 0; i < URLMAP_SIZE; i++)
   {
       urlmap[i].wan_port = 0;
       urlmap[i].wan_socket_fd = -1;
   }

   if (configuration.registrationfile)
   {
      stream = fopen(configuration.registrationfile, "r");

      if (!stream)
      {
         /*
          * the file does not exist, or the size was incorrect,
          * delete it and start from scratch
          */
         unlink(configuration.registrationfile);
         WARN("registration file not found, starting with empty table");
      } else {
         /* read the url table from file */
         for (i=0;i < URLMAP_SIZE; i++) {
            fgets(buff, sizeof(buff), stream);
            sts=sscanf(buff, "***:%i:%i", &urlmap[i].active, &urlmap[i].expires);
            if (urlmap[i].active) {
               osip_uri_init(&urlmap[i].true_url);
               osip_uri_init(&urlmap[i].masq_url);
               osip_uri_init(&urlmap[i].reg_url);

	       #define R(X) {\
               fgets(buff, sizeof(buff), stream);\
               buff[sizeof(buff)-1]='\0';\
               if (strchr(buff, 10)) *strchr(buff, 10)='\0';\
               if (strchr(buff, 13)) *strchr(buff, 13)='\0';\
               if (strlen(buff) > 0) {\
                  size = strlen(buff);\
                  X    =(char*)malloc(size+1);\
                  sts=sscanf(buff,"%s",X);\
               } else {\
                  X = NULL;\
               }\
               }

               R(urlmap[i].true_url->scheme);
               R(urlmap[i].true_url->username);
               R(urlmap[i].true_url->host);
               R(urlmap[i].true_url->port);
               R(urlmap[i].masq_url->scheme);
               R(urlmap[i].masq_url->username);
               R(urlmap[i].masq_url->host);
               R(urlmap[i].masq_url->port);
               R(urlmap[i].reg_url->scheme);
               R(urlmap[i].reg_url->username);
               R(urlmap[i].reg_url->host);
               R(urlmap[i].reg_url->port);
            }
         }
         fclose(stream);
      }
   }
   return;
}


/*
 * shut down the URL mapping table
 */
void register_shut(void) {
   int i;
   FILE *stream;

   if (configuration.registrationfile) {
      /* write urlmap back to file */
      stream = fopen(configuration.registrationfile, "w+");
      if (!stream) {
         /* try to unlink it and open again */
         unlink(configuration.registrationfile);
         stream = fopen(configuration.registrationfile, "w+");

         /* open file for write failed, complain */
         if (!stream) {
            ERROR("unable to write registration file");
            return;
         }
      }

      for (i=0;i < URLMAP_SIZE; i++) {
         fprintf(stream, "***:%i:%i\n", urlmap[i].active, urlmap[i].expires);
         if (urlmap[i].active) {
            #define W(X) fprintf(stream, "%s\n", (X)? X:"");

            W(urlmap[i].true_url->scheme);
            W(urlmap[i].true_url->username);
            W(urlmap[i].true_url->host);
            W(urlmap[i].true_url->port);
            W(urlmap[i].masq_url->scheme);
            W(urlmap[i].masq_url->username);
            W(urlmap[i].masq_url->host);
            W(urlmap[i].masq_url->port);
            W(urlmap[i].reg_url->scheme);
            W(urlmap[i].reg_url->username);
            W(urlmap[i].reg_url->host);
            W(urlmap[i].reg_url->port);
         }
      }
      fclose(stream);
   }
   return;
}


void urlmap_clean_item(int i)
{
   if ((i>=0) && (i<URLMAP_SIZE))
   {
      if (urlmap[i].masq_url)
      {
          printsip("sip: cleaned entry:urlmap[%i].masq %s@%s \r\n", i,
             urlmap[i].masq_url->username,  urlmap[i].masq_url->host);
      }
      urlmap[i].active=0;
      osip_uri_free(urlmap[i].true_url);
      osip_uri_free(urlmap[i].masq_url);
      osip_uri_free(urlmap[i].reg_url);
      //释放WAN socket句柄，如果socket fd大于0
      if (urlmap[i].wan_socket_fd > 0)
      {
         proxy_control_wan_port(FW_SIP_CMD_TYPE_REMOVE, urlmap[i].wan_port, i);
         close(urlmap[i].wan_socket_fd);
         urlmap[i].wan_socket_fd = -1;
         urlmap[i].wan_port = 0;
      }
      
   #if UNREG_WITHOUT_REG_HG522
      if (NULL != urlmap[i].to_ip)
      {
          free(urlmap[i].to_ip);
          urlmap[i].to_ip = NULL;
      }
   #endif
      /* DT c40163 add for STUN */
      memset(&(urlmap[i].from), 0, sizeof(struct sockaddr_in));
      /* DT c40163 end */
   	 
      /* DT c40163 add for QOS */
      urlmap[i].t_online = 0;
      /* DT c40163 end */
      urlmap[i].incall = ALG_FALSE;

   }
}



void proxy_control_wan_port(SIP_FW_COMMAND_TYPE_EN EN_ACTION, int iWanPort, int idex)
{
    char acCmd[256] = {0};
    char Cmd = 0;
    char *outbound_if = 0;
    
    if (iWanPort <= 0 || iWanPort > 0xFFFF)
    {
        printsip("wrong wan port defined!\n");
        return;
    }
    
    switch (EN_ACTION)
    {
        case FW_SIP_CMD_TYPE_OPEN:
            Cmd = 'I';
            break;
        case FW_SIP_CMD_TYPE_REMOVE:
            Cmd = 'D';
            break;
        default:
            break;
    }

    //if the outbound interface is "gre1" or "gre2", set the iptable rule as "gre+". therefore the iptable rule
	//still works even when switch between gre1&gre2 happens during the registraion period.
    if (0 == strncmp(urlmap[idex].outbound_if, VOICE_TUNNNEL_PREFIX, sizeof(VOICE_TUNNNEL_PREFIX) -1))
    {
        outbound_if = VOICE_TUNNNEL_ALL;
    }
    else
    {
        outbound_if = urlmap[idex].outbound_if;
    }
    
    if (Cmd)
    {
        snprintf(acCmd, sizeof(acCmd), "iptables -t nat -%c PREROUTING -i %s -p udp --dport %d -j ACCEPT",
                 Cmd, outbound_if, iWanPort);
        ATP_UTIL_ForkProcess(acCmd, -1, NULL, NULL);
        snprintf(acCmd, sizeof(acCmd), "iptables -%c INPUT_SERVICE -i %s -p udp --dport %d -j ACCEPT",
                 Cmd, outbound_if, iWanPort);
        ATP_UTIL_ForkProcess(acCmd, -1, NULL, NULL);
    }
}

/*
 * handles r egister requests and updates the URL mapping table
 * force_lcl_masq 0   The register server
 * force_lcl_masq 1   The Proxy
 * RETURNS:
 *    STS_SUCCESS : successfully registered
 *    STS_FAILURE : registration failed
 *    STS_NEED_AUTH : authentication needed
 */
int register_client(sip_ticket_t *ticket, int force_lcl_masq) {
   int i, j, n, sts;
   int expires;
   time_t time_now;
   struct timeval stTm;

   /* 报文中 to 字段  和  contact 字段 的 URL 信息 */
   osip_uri_t *url1_to, *url1_contact=NULL;

   /* 全局 urlmap[i].reg_url 和true_url中保存的信息 ,对比注册报文的contact,dt要求每个ip的注册使用不同的wan端口
   出去，即使2个pc注册同一个sip服务器上的同一个sip账号，必须区分wan端口,要给它们分配2个url_map映射
   每个映射里面，加上wan socket句柄和端口号,方便取用*/
   osip_uri_t *url2_to, *url2_contact=NULL;
   int iWanSocket = 0;
   int iWanPort = 0;
   char *outbound_if = NULL;       /* 报文从接口转发，WAN侧 */
   struct in_addr addr_outbound = {0};
   char addr_outboundstr[HOSTNAME_SIZE] = {0};
   
   osip_header_t *expires_hdr;
   osip_uri_param_t *expires_param=NULL;
   
   /*
    * Authorization - do only if I'm not just acting as outbound proxy
    * but am ment to be the registrar
    */
   if (force_lcl_masq == 0) 
   {
      /*
       * RFC 3261, Section 16.3 step 6
       * Proxy Behavior - Request Validation - Proxy-Authorization
       */
      sts = authenticate_proxy(ticket);
      if (sts == STS_FAILURE) 
      {
         /* failed */
         printsip("proxy authentication failed for %s@%s \r\n",
              (ticket->sipmsg->to->url->username)? 
              ticket->sipmsg->to->url->username : "*NULL*",
              ticket->sipmsg->to->url->host);
         return STS_FAILURE;
      }
      else if (sts == STS_NEED_AUTH)
      {
         /* needed */
         printsip("proxy authentication needed for %s@%s \r\n",
                ticket->sipmsg->to->url->username,
                ticket->sipmsg->to->url->host);
         return STS_NEED_AUTH;
      }
   }

/*
   fetch 1st Via entry and remember this address. Incoming requests
   for the registered address have to be passed on to that host.

   To: -> address to be registered
   Contact: -> host is reachable there
               Note: in case of un-REGISTER, the contact header may
                     contain '*' only - which means "all registrations
                     made by this UA"
   
   => Mapping is
   To: <1--n> Contact
   
*/
   //time(&time_now);
   ATP_UTIL_GetSysTime(&stTm);
   time_now = stTm.tv_sec;

   DEBUGC(DBCLASS_BABBLE,"sip_register:");

   /* evaluate Expires Header field */
   osip_message_get_expires(ticket->sipmsg, 0, &expires_hdr);

  /*
   * look for an Contact expires parameter - in case of REGISTER
   * these two are equal. The Contact expires has higher priority!
   */
   if (ticket->sipmsg->contacts && ticket->sipmsg->contacts->node &&
       ticket->sipmsg->contacts->node->element)
   {
      osip_contact_param_get_byname(
              (osip_contact_t*) ticket->sipmsg->contacts->node->element,
              EXPIRES, &expires_param);
   }

   if (expires_param && expires_param->gvalue) 
   {
      /* get expires from contact Header */
      expires=atoi(expires_param->gvalue);
   }
   else if (expires_hdr && expires_hdr->hvalue)
   {
      /* get expires from expires Header */
      expires=atoi(expires_hdr->hvalue);
   } 
   else
   {
      char tmp[16];
      /* it seems, the expires field is not present everywhere... */
      printsip("no 'expires' header found - set time to %i sec \r\n",
             configuration.default_expires);
      expires=configuration.default_expires;
      sprintf(tmp,"%i",expires);
      osip_message_set_expires(ticket->sipmsg, tmp);
   }

   url1_to=ticket->sipmsg->to->url;
   /* Sip: The sipmsg to->username:[88880120], to->host[129.102.100.5] */
   printsip("Sip: Expires:[%d] The sipmsg to->username:[%s], to->host[%s] \r\n",
                   expires, (url1_to->username) ? url1_to->username : "*NULL*",
                   (url1_to->host) ? url1_to->host : "*NULL*");


   //根据telecomlist来判断需要绑定哪条wan接口
   outbound_if = get_if_by_telecomlist(utils_inet_ntoa(ticket->to.sin_addr), ticket->from.sin_port, is_t_online(ticket->sipmsg, -1), ticket->ulMark, ticket->sipmsg->req_uri->host);

   printsip("outbound_if = %s \r\n",outbound_if);
   
   memset(&addr_outbound, 0, sizeof(addr_outbound));
   if (get_ip_by_ifname(outbound_if, &addr_outbound) != STS_SUCCESS)
   {
      printsip("can't find outbound interface %s - configuration error?", outbound_if);
      return STS_FAILURE;
   }
   snprintf(addr_outboundstr, sizeof(addr_outboundstr), "%s", utils_inet_ntoa(addr_outbound));

   printsip("addr_outboundstr = %s \r\n",addr_outboundstr);



   /*
    * REGISTER
    */
   if (expires > 0)
   {
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
          ticket->sipmsg->contacts->node->element) {
         url1_contact=((osip_contact_t*)
                      (ticket->sipmsg->contacts->node->element))->url;
      }
      if ((url1_contact == NULL) || (url1_contact->host == NULL)) {
         /* Don't have required Contact fields */
         printsip("tried registration with empty Contact header \r\n");
         return STS_FAILURE;
      }
         
      printsip("register: %s@%s expires=%i seconds \r\n",
          (url1_contact->username) ? url1_contact->username : "*NULL*",
          (url1_contact->host) ? url1_contact->host : "*NULL*",
          expires);

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

         url2_to=urlmap[i].reg_url;
         url2_contact = urlmap[i].true_url;/*增加比较pc的contact*/
         
         /* 是否需要更新注册记录 */
         /* check address-of-record ("public address" of user) */
         if (compare_url(url1_to, url2_to) == STS_SUCCESS
         //增加比较pc ip,区分比较lan侧2个设备注册同一个SIP号码的情况
         && compare_url(url1_contact, url2_contact) == STS_SUCCESS)
         {
            printsip("found entry for %s@%s <-> %s@%s at globle urlmap[%i], exp=%li \r\n",
	           (url1_contact->username) ? url1_contact->username : "*NULL*",
                   (url1_contact->host) ? url1_contact->host : "*NULL*",
	           (url2_to->username) ? url2_to->username : "*NULL*",
                   (url2_to->host) ? url2_to->host : "*NULL*",
		       i, urlmap[i].expires-time_now);
            break;
         }
      }

      if ( (j < 0) && (i >= URLMAP_SIZE) )
      {
         /* oops, no free entries left... */
         printsip("URLMAP is full - registration failed \r\n");
         return STS_FAILURE;
      }

      //当根据telecomlist查到的wan接口解析的地址与原有初始注册的地址不一致时，将绑定的wan进行切换
      //同时在进行通话时不能进行wan切换
      if (((i>=0) && (i<URLMAP_SIZE)) &&// (NULL !=outbound_if) && (urlmap[i].outbound_if) && 
        (//(strncmp(urlmap[i].outbound_if, outbound_if, IFNAME_SIZE) != 0) || 
        (memcmp(&urlmap[i].outbound_addr, &addr_outbound, sizeof(urlmap[i].outbound_addr)))) &&
        (urlmap[i].incall == ALG_FALSE))
      {
         //清掉原有的urlmap项
         printsip("sip: cleaned entry:urlmap[%i].masq %s@%s \r\n", i,
           urlmap[i].masq_url->username,  urlmap[i].masq_url->host);
         
         printsip("outbound_if = %s \r\n",outbound_if);
         printsip("urlmap[i].outbound_if = %s \r\n",urlmap[i].outbound_if);
         printsip("addr_outboundstr = %s \r\n",addr_outboundstr);
         printsip("urlmap[i].masq_url->host = %s \r\n",urlmap[i].masq_url->host);
	     urlmap_clean_item(i);
         
         //将i重新赋值，新增一条urlmap指向新的outbound_if
         for (i=0; i<URLMAP_SIZE; i++)
         {
            if (urlmap[i].active == 0) 
            {
           	    if (j < 0) j=i; /* remember first hole */
                break;
            }
         }
         i = URLMAP_SIZE;
      }

      printsip("Sip:register_client(), record i[%d], cur j[%d] \r\n", i, j);
      if (i >= URLMAP_SIZE)
      {
         /* entry not existing, create new one */
         i=j;

         /* write entry */
         urlmap[i].active=1;
         /* Contact: field */
         osip_uri_clone( ((osip_contact_t*)
                         (ticket->sipmsg->contacts->node->element))->url, 
        	         &urlmap[i].true_url);
         /* To: field */
         osip_uri_clone( ticket->sipmsg->to->url, 
        	    &urlmap[i].reg_url);

        //增加wan socket fd， wan port到url_map列表中
         iWanSocket = get_wan_socket(outbound_if, &iWanPort);
         if (0 == iWanSocket)
         {
            ERROR("Allocate WAN Socket !!!FAILED!!! for %s@%s:%s",
                (url1_contact->username) ? url1_contact->username : "*NULL*",
                (url1_contact->host) ? url1_contact->host : "*NULL*",
                url1_contact->port);
            return STS_FAILURE;
         }
         //转包到WAN要用下列socket和端口, 端口号要更新到WAN的contact中,而不是原来写死的56005
         urlmap[i].wan_socket_fd = iWanSocket;
         urlmap[i].wan_port = iWanPort;
         //记录telecomlist查到的wan接口和查询到的对应的地址，用于后续使用
         snprintf(urlmap[i].outbound_if, sizeof(urlmap[i].outbound_if), "%s", outbound_if);
         memcpy(&urlmap[i].outbound_addr, &addr_outbound, sizeof(urlmap[i].outbound_addr));
         proxy_control_wan_port(FW_SIP_CMD_TYPE_REMOVE, iWanPort, i);
         proxy_control_wan_port(FW_SIP_CMD_TYPE_OPEN, iWanPort, i);

         printsip("urlmap[i].wan_socket_fd:%d, urlmap[i].wan_port:%d, urlmap[i].outbound_if:%s \r\n", urlmap[i].wan_socket_fd, urlmap[i].wan_port, urlmap[i].outbound_if);
         
         printsip("create new entry for %s@%s:%s <-> %s@%s at globle urlmap[%i] \r\n",
                (url1_contact->username) ? url1_contact->username : "*NULL*",
                (url1_contact->host) ? url1_contact->host : "*NULL*",
                url1_contact->port,/*PC源端口号*/
	        (urlmap[i].reg_url->username) ? urlmap[i].reg_url->username : "*NULL*",
                (urlmap[i].reg_url->host) ? urlmap[i].reg_url->host : "*NULL*",
                i);

         /*
          * try to figure out if we ought to do some masquerading
          */
         osip_uri_clone( ticket->sipmsg->to->url, 
        	         &urlmap[i].masq_url);

         n=configuration.mask_host.used;
         if (n != configuration.masked_host.used) {
            ERROR("# of mask_host is not equal to # of masked_host in config!");
            n=0;
         }

         DEBUG("%i entries in MASK config table", n);
         for (j=0; j<n; j++)
         {
            DEBUG("compare [%s] <-> [%s]",configuration.mask_host.string[j],
                  ticket->sipmsg->to->url->host);
            if (strcmp(configuration.mask_host.string[j],
                ticket->sipmsg->to->url->host)==0)
               break;
         }
         if (j<n)
         { 
            /* we are masquerading this UA, replace the host part of the url */
            DEBUGC(DBCLASS_REG,"masquerading UA %s@%s as %s@%s",
                   (url1_contact->username) ? url1_contact->username : "*NULL*",
                   (url1_contact->host) ? url1_contact->host : "*NULL*",
                   (url1_contact->username) ? url1_contact->username : "*NULL*",
                   configuration.masked_host.string[j]);
            urlmap[i].masq_url->host=realloc(urlmap[i].masq_url->host,
                                    strlen(configuration.masked_host.string[j])+1);
            strcpy(urlmap[i].masq_url->host, configuration.masked_host.string[j]);
         }

         /*
          * for transparent proxying: force device to be masqueraded
          * as with the outbound IP
          */
         /* proxy 需要进入此分支, 修改masq_url 字段 */
         if (force_lcl_masq)
         {
            struct in_addr addr;
            char *addrstr;
#if 0
            if (get_ip_by_ifname(configuration.outbound_if, &addr) !=
                STS_SUCCESS)
            {
               ERROR("can't find outbound interface %s - configuration error?",
                     configuration.outbound_if);
               return STS_FAILURE;
            }
#else
            memcpy(&addr, &urlmap[i].outbound_addr, sizeof(addr));
#endif

            /* host part */
            addrstr = utils_inet_ntoa(addr);
            DEBUGC(DBCLASS_REG,"masquerading UA %s@%s local %s@%s",
                   (url1_contact->username) ? url1_contact->username : "*NULL*",
                   (url1_contact->host) ? url1_contact->host : "*NULL*",
                   (url1_contact->username) ? url1_contact->username : "*NULL*",
                   addrstr);
            urlmap[i].masq_url->host=realloc(urlmap[i].masq_url->host,
                                             strlen(addrstr)+1);
            strcpy(urlmap[i].masq_url->host, addrstr);

            /* port number if required */

#if 0
            if (configuration.sip_listen_port != SIP_PORT) 
            {
               urlmap[i].masq_url->port=realloc(urlmap[i].masq_url->port, 16);
               sprintf(urlmap[i].masq_url->port, "%i",
                       configuration.sip_listen_port);
            }
#else
//动态修改masqured的contact端口号，而不是对所有lan侧设备的注册都写死

            urlmap[i].masq_url->port=realloc(urlmap[i].masq_url->port, 16);
            sprintf(urlmap[i].masq_url->port, "%i", urlmap[i].wan_port);
#endif
         }
      }
      else
      { /* if new entry,else we have a record to recove it */
      /*
       * Some phones (like BudgeTones *may* dynamically grab a SIP port
       * so we might want to update the true_url and reg_url each time
       * we get an REGISTER
       */
         /* Contact: field */
         printsip("Sip:Sip:register_client() recor exist \r\n");
         osip_uri_free(urlmap[i].true_url);
         osip_uri_clone( ((osip_contact_t*)
                         (ticket->sipmsg->contacts->node->element))->url, 
        	         &urlmap[i].true_url);
         /* To: field */
         osip_uri_free(urlmap[i].reg_url);
         osip_uri_clone( ticket->sipmsg->to->url, 
        	    &urlmap[i].reg_url);
      }
      /* give some safety margin for the next update */
      if (expires > 0) expires+=30;

      /* update registration timeout */
      urlmap[i].expires=time_now+expires;

	  /* DT c40163 add for STUN */
	  if (force_lcl_masq) {
         struct in_addr addr_wan, addr_cont;
         if ((url1_contact->host[0] == '*')) {
            osip_via_t *via = osip_list_get(ticket->sipmsg->vias, 0);
		    if (via == NULL || via->host == NULL) {
		       printf("force stun: no via\n");
               return STS_FAILURE;
		    }
		    get_ip_by_host(via->host, &addr_cont);
         }
		 else {
		    get_ip_by_host(((osip_contact_t*)(ticket->sipmsg->contacts->node->element))->url->host, &addr_cont);
		 }
         
		 //get_ip_by_ifname(urlmap[i].outbound_if, &addr_wan);
         memcpy(&addr_wan, &urlmap[i].outbound_addr, sizeof(addr_wan));
		
		/*HOMEGW-11184,通过contact字段地址和IP层源地址是否相同，判断是否强制进行stun，
		而不是通过本地WAN接口IP，因为存在多层NAT的关系。 started by g00121640*/
		 #if 0
		 if (!memcmp(&addr_wan, &addr_cont, sizeof(struct in_addr))) {
		 	printf("force stun: register, backup from, ind=%u, ip=%s\n",
				i, utils_inet_ntoa(ticket->from.sin_addr));
		 	urlmap[i].from = ticket->from;
		 }
		 #else
		 if ((memcmp(&ticket->from.sin_addr, &addr_cont, sizeof(struct in_addr)))) {
		 	printf("force stun: register, backup from, ind=%u, ip=%s\n",
				i, utils_inet_ntoa(ticket->from.sin_addr));
            /*在contact等于实际的报文的源地址不同的时候，以报文的源地址为准，后续响应发往该ip和port*/
            memcpy(&urlmap[i].from, &ticket->from, sizeof(urlmap[i].from));
		 	//urlmap[i].from = ticket->from;
		 }
		 #endif
		 	/*HOMEGW-11184,通过contact字段地址和IP层源地址是否相同，判断是否强制进行stun，
		而不是通过本地WAN接口IP，因为存在多层NAT的关系。 ended by g00121640*/
		 else {
		 	memset(&(urlmap[i].from), 0, sizeof(struct sockaddr_in));
		 }
	     /* DT c40163 add for QOS */
		 urlmap[i].t_online = is_t_online(ticket->sipmsg, -1);
	     /* DT c40163 end */
	  }
	  /* DT c40163 end */

   /*
    * un-REGISTER
    */
   }
   else
   { /* expires > 0 , else expires <= 0 */
      /*
       * Remove registration
       * Siproxd will ALWAYS remove ALL bindings for a given
       * address-of-record
       */
      
      if (ticket->sipmsg->contacts && ticket->sipmsg->contacts->node &&
          ticket->sipmsg->contacts->node->element) {
         url1_contact=((osip_contact_t*)
                      (ticket->sipmsg->contacts->node->element))->url;
      }
      if ((url1_contact == NULL) || (url1_contact->host == NULL)) {
         /* Don't have required Contact fields */
         printsip("tried registration with empty Contact header \r\n");
         return STS_FAILURE;
      }
      
      printsip("Sip: un-REGISTER packet expires <= 0 \r\n");
      for (i=0; i<URLMAP_SIZE; i++)
      {
         if (urlmap[i].active == 0) continue;

         url2_to = urlmap[i].reg_url;
         url2_contact = urlmap[i].true_url;/*增加比较pc的contact*/
         
         if ((compare_url(url1_to, url2_to) == STS_SUCCESS)
         //增加匹配PC的 contact,即IP
            && (compare_url(url1_contact, url2_contact) == STS_SUCCESS))
         {
            printsip("removing registration for %s@%s at globle urlmap[%i] \r\n",
	           (url2_to->username) ? url2_to->username : "*NULL*",
                   (url2_to->host) ? url2_to->host : "*NULL*", i);
            urlmap[i].expires=0;
            //释放对应的wan转发的socket,不能在此做，否则没wan socket发注销报文出去，
            //register_agemap后续会自动处理
            break;
         }
      }
	  /* DT c40163 add for STUN */
	  if (i < URLMAP_SIZE) {
         osip_via_t *via = osip_list_get(ticket->sipmsg->vias, 0);
		 if (via != NULL && via->host != NULL) {
            struct in_addr addr_wan, addr_cont;
            get_ip_by_host(via->host, &addr_cont);
#if 0
		    get_ip_by_ifname(configuration.outbound_if, &addr_wan);
#else
            memcpy(&addr_wan, &urlmap[i].outbound_addr, sizeof(addr_wan));
#endif
		    if (!memcmp(&addr_wan, &addr_cont, sizeof(struct in_addr))) {
			   if ((urlmap[i].from.sin_addr.s_addr != ticket->from.sin_addr.s_addr)
			   	  || (urlmap[i].from.sin_port != ticket->from.sin_port))
			   {
		          printf("force stun: unreg, stun session\n");
                  /*网关不会自行回复200ok*/
		          //register_response(ticket, STS_SUCCESS);
			   }
		    }
		 }
	     /* DT c40163 add for QOS */
		 urlmap[i].t_online = is_t_online(ticket->sipmsg, -1);
	     /* DT c40163 end */
	  }
	  else {
	  	 printf("force stun: unreg, not found session\n");
         /*网关不会自行回复200ok*/
	  	 //register_response(ticket, STS_SUCCESS);
	  }
      /* DT c40163 end */
   }
   return STS_SUCCESS;
}



/*
 * cyclically called to do the aging of the URL mapping table entries
 * and throw out expired entries.
 */
void register_agemap(void) {
   int i;
   time_t t;
   struct timeval stTm;
   struct in_addr addr_outbound = {0};
   
   //time(&t);
   ATP_UTIL_GetSysTime(&stTm);
   t = stTm.tv_sec;
   //DEBUGC(DBCLASS_BABBLE,"sip_agemap, t=%i",(int)t);
   for (i=0; i<URLMAP_SIZE; i++)
   {
      if ((urlmap[i].active == 1) && (urlmap[i].expires < t))
      {
         printsip("sip: cleaned entry:urlmap[%i].masq %s@%s \r\n", i,
           urlmap[i].masq_url->username,  urlmap[i].masq_url->host);
	     urlmap_clean_item(i);
      }
      /*当ppp或lte 切换了ip之后，在siproxd未重启的情况下需要将原先ip相关的urlmap条目清除掉，
      否则后续的注册报文无法通过旧的ip和socket转发出去*/
      if (urlmap[i].active == 1)
      {
          if (get_ip_by_ifname(urlmap[i].outbound_if, &addr_outbound) == STS_SUCCESS)
          {
             if ((memcmp(&urlmap[i].outbound_addr, &addr_outbound, sizeof(urlmap[i].outbound_addr))) && 
                (urlmap[i].incall == ALG_FALSE))
             {
                printsip("sip: cleaned entry:urlmap[%i].masq %s@%s \r\n", i,
                  urlmap[i].masq_url->username,  urlmap[i].masq_url->host);
                urlmap_clean_item(i);
             }
          }
      }
   }
   return;
}


/*
 * send answer to a registration request.
 *  flag = STS_SUCCESS    -> positive answer (200)
 *  flag = STS_FAILURE    -> negative answer (503)
 *  flag = STS_NEED_AUTH  -> proxy authentication needed (407)
 *
 * RETURNS
 *	STS_SUCCESS on success
 *	STS_FAILURE on error
 */
int register_response(sip_ticket_t *ticket, int flag) {
   osip_message_t *response;
   int code;
   int sts;
   osip_via_t *via;
   int port;
   char *buffer;
   struct in_addr addr;
   osip_header_t *expires_hdr;

   /* ok -> 200, fail -> 503 */
   switch (flag) {
   case STS_SUCCESS:
      code = 200;	/* OK */
      break;
   case STS_FAILURE:
      code = 503;	/* failed */
      break;
   case STS_NEED_AUTH:
      code = 407;	/* proxy authentication needed */
      break;
   default:
      code = 503;	/* failed */
      break;
   }

   /* create the response template */
   if ((response=msg_make_template_reply(ticket, code))==NULL) {
      ERROR("register_response: error in msg_make_template_reply");
      return STS_FAILURE;
   }

   /* insert the expiration header */
   osip_message_get_expires(ticket->sipmsg, 0, &expires_hdr);
   if (expires_hdr) {
      osip_message_set_expires(response, expires_hdr->hvalue);
   }

   /* if we send back an proxy authentication needed, 
      include the Proxy-Authenticate field */
   if (code == 407) {
      auth_include_authrq(ticket);
   }

   /* get the IP address from existing VIA header */
   osip_message_get_via (response, 0, &via);
   if (via == NULL) {
      ERROR("register_response: Cannot send response - no via field");
      return STS_FAILURE;
   }

   /* name resolution needed? */
   if (utils_inet_aton(via->host,&addr) == 0) {
      /* yes, get IP address */
      sts = get_ip_by_host(via->host, &addr);
      if (sts == STS_FAILURE) {
         DEBUGC(DBCLASS_REG, "register_response: cannot resolve VIA [%s]",
                via->host);
         return STS_FAILURE;
      }
   }   

   sts = osip_message_to_str(response, &buffer);
   if (sts != 0) {
      ERROR("register_response: msg_2char failed");
      return STS_FAILURE;
   }

   /* send answer back */
   if (via->port) {
      port=atoi(via->port);
   } else {
      port=configuration.sip_listen_port;
   }

   /* DT c40163 add for STUN */
   {
   	  struct in_addr addr_wan;
      get_ip_by_ifname(VOICE_DSL_INTF_NAME, &addr_wan);
      if (!memcmp(&addr_wan, &addr, sizeof(struct in_addr))) {
         printf("force stun: register responce, use from ip\n");
         addr = ticket->from.sin_addr;
		 port = ticket->from.sin_port;
      }
      get_ip_by_ifname(VOICE_LTE_TUNNEL_NAME, &addr_wan);
      if (!memcmp(&addr_wan, &addr, sizeof(struct in_addr))) {
         printf("force stun: register responce, use from ip\n");
         addr = ticket->from.sin_addr;
		 port = ticket->from.sin_port;
      }
   }
   /* DT c40163 end */

   /* DT c40163 modify for QOS
   sipsock_send(addr, port, ticket->protocol, buffer, strlen(buffer));
   */
   sipsock_send(addr, port, ticket->protocol, buffer, strlen(buffer), 0, 0, 0, -1);
   /* DT c40163 end */

   /* free the resources */
   osip_message_free(response);
   free(buffer);
   return STS_SUCCESS;
}

