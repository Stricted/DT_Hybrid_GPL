
/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : br_igmp_statistic.c
  �� �� ��   : ����
  ��    ��   : z67728
  ��������   : 2010��7��20��
  ����޸�   :
  ��������   : IGMPͳ�ƹ���
  �����б�   :
              br_igmp_statistic_addnode
              br_igmp_statistic_clearallchain
              br_igmp_statistic_clearcurnode
              br_igmp_statistic_delnode
              br_igmp_statistic_findnode
              br_igmp_statistic_inc
              br_igmp_statistic_incparameter
              br_igmp_statistic_print
              br_igmp_statistic_setenable
              ppp_igmp_statistic_inc
  �޸���ʷ   :
  1.��    ��   : 2010��7��20��
    ��    ��   : z67728
    �޸�����   : �����ļ�

******************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/if_vlan.h>
#include <asm/atomic.h>
#include <linux/igmp.h>
#include <linux/inetdevice.h>
#include <linux/igmp_statistic.h>
#include <linux/skbuff.h>
#include "br_private.h"

#include "atp_interface.h"
#include "br_igmp_snooping.h"

struct tag_igmp_statistics_interface_st *g_pstIgmpStatistics = NULL;

unsigned long  g_ulIgmpStatisticEnableSwitch = 0;
unsigned long  g_ulIgmpStatisticPauseSwitch  = 0;

//#define IGMP_STATISTIC_DEBUG_SWITCH
#ifdef IGMP_STATISTIC_DEBUG_SWITCH
#define IGMP_STATISTIC_DEBUG(format,args...) printk("\r\nFILE[%s] FUNC[%s] LINE[%d] "format"\r\n",__FILE__,__func__,__LINE__,## args)
#else
#define IGMP_STATISTIC_DEBUG(format,args...)
#endif

#define IGMP_STATISTIC_PORT_SRC                   (1)
#define IGMP_STATISTIC_PORT_DST                   (2)

#define IGMP_STATISTIC_PORT_ADD                   (1)
#define IGMP_STATISTIC_PORT_DEL                   (2)

#define IGMP_MULTI_SESSION_STATISTIC_INTERVAL     (10 * HZ)
#define IGMP_MULTI_SESSION_STATISTIC_TIME         ((5 * 60 - 10) * HZ)
#define IGMP_MULTI_SESSION_STATISTIC_TIME_MAX     ((5 * 60) * HZ)
#define IGMP_MULTI_SESSION_STATISTIC_NODE_NUM_MAX (2)

#define IGMP_STATISTIC_PAUSE_SWITCH_ON            (1)
#define IGMP_STATISTIC_PAUSE_SWITCH_OFF           (0)

#define IP_PROTOCOL_UDP                           (0x11)

/******************* address filter **********************/
#define IGMP_STATISTIC_GROUP_STATS_JOIN           (0x1010)
#define IGMP_STATISTIC_GROUP_STATS_LEAVE          (0x0101)
#define IGMP_UPNP_ADDRESS                         (0xEFFFFFFa)
#define IGMP_LOCAL_ADDRESS                        (0xE0000000)
#define IGMP_MDNS_ADDRESS                         (0xEF0000Fb)
/************************* end ***************************/

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_addmultisession
 ��������  : ����鲥�Ự��¼
 �������  : struct tag_igmp_statistics_interface_st **pstInterface  
             unsigned char *pucMultiMacAddr                          
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��8��12��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static int br_igmp_statistic_addmultisession(struct tag_igmp_statistics_interface_st **pstInterface,const unsigned char *pucMultiMacAddr)
{
    int                              lLoopi             = 0;
    int                              lNodeNum           = 0;
    struct tag_igmp_multicast_union *pstTemMultiSession = NULL;
    
    if ( NULL == pstInterface || NULL == *pstInterface || NULL == pucMultiMacAddr )
    {
        IGMP_STATISTIC_DEBUG("%s","pstInterface or pstInterface or pucMultiMacAddr is NULL.");
        return -1;
    }

    IGMP_STATISTIC_DEBUG("Add interface[%s] multicast session id[%02x:%02x:%02x:%02x:%02x:%02x]", (*pstInterface)->czInterfaceName,
                                                                                                  pucMultiMacAddr[-3],
                                                                                                  pucMultiMacAddr[-2],
                                                                                                  pucMultiMacAddr[-1],
                                                                                                  pucMultiMacAddr[0],
                                                                                                  pucMultiMacAddr[1],
                                                                                                  pucMultiMacAddr[2]);
    
    if ( NULL == (*pstInterface)->stIgmpStatistics.pstMultiSession )
    {
        (*pstInterface)->stIgmpStatistics.pstMultiSession = (struct tag_igmp_multicast_union *)kmalloc(sizeof(struct tag_igmp_multicast_union),GFP_KERNEL);
        if ( NULL == (*pstInterface)->stIgmpStatistics.pstMultiSession )
        {
            IGMP_STATISTIC_DEBUG("%s","Create memory meet error.");
            return -1;
        }
        else
        {
            memset((*pstInterface)->stIgmpStatistics.pstMultiSession,0,sizeof(struct tag_igmp_multicast_union));
            (*pstInterface)->stIgmpStatistics.pstMultiSession->next = NULL;
        }
    }

    pstTemMultiSession = (*pstInterface)->stIgmpStatistics.pstMultiSession;
    while ( NULL != pstTemMultiSession && IGMP_MULTI_SESSION_STATISTIC_NODE_NUM_MAX > lNodeNum )
    {
        lLoopi = 0;
        while ( IGMP_SESSION_MAX_NUM > lLoopi)
        {
            if ( 0 == pstTemMultiSession->ucMultiMacAddr[lLoopi][0] + 
                      pstTemMultiSession->ucMultiMacAddr[lLoopi][1] +
                      pstTemMultiSession->ucMultiMacAddr[lLoopi][2] )
            {
                pstTemMultiSession->ucMultiMacAddr[lLoopi][0] = pucMultiMacAddr[0];
                pstTemMultiSession->ucMultiMacAddr[lLoopi][1] = pucMultiMacAddr[1];
                pstTemMultiSession->ucMultiMacAddr[lLoopi][2] = pucMultiMacAddr[2];

                IGMP_STATISTIC_DEBUG("%s","Add multicast session successful.");
                return 0;
            }

            lLoopi++;
        }
        
        lNodeNum++;
        pstTemMultiSession = pstTemMultiSession->next;
    }

    if ( NULL == pstTemMultiSession && IGMP_MULTI_SESSION_STATISTIC_NODE_NUM_MAX > lNodeNum )
    {
        /* ѭ������,��δ�ҵ� */
        pstTemMultiSession = (struct tag_igmp_multicast_union *)kmalloc(sizeof(struct tag_igmp_multicast_union),GFP_KERNEL);
        if ( NULL == pstTemMultiSession )
        {
            IGMP_STATISTIC_DEBUG("%s","Create memory meet error.");
            return -1;
        }
        else
        {
            memset(pstTemMultiSession,0,sizeof(struct tag_igmp_multicast_union));
        }

        pstTemMultiSession->ucMultiMacAddr[0][0] = pucMultiMacAddr[0];
        pstTemMultiSession->ucMultiMacAddr[0][1] = pucMultiMacAddr[1];
        pstTemMultiSession->ucMultiMacAddr[0][2] = pucMultiMacAddr[2];

        pstTemMultiSession->next = (*pstInterface)->stIgmpStatistics.pstMultiSession;
        (*pstInterface)->stIgmpStatistics.pstMultiSession = pstTemMultiSession;

        IGMP_STATISTIC_DEBUG("%s","Create and add new multicast session successful.");

        return 0;
    }

    return -1;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_Findmultisession
 ��������  : ���Ҷ�Ӧ���鲥�ỰMAC��ַ
 �������  : struct tag_igmp_statistics_interface_st **pstInterface  
             unsigned char *pucMultiMacAddr                          
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��8��12��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static int br_igmp_statistic_findmultisession(struct tag_igmp_statistics_interface_st **pstInterface,const unsigned char *pucMultiMacAddr)
{
    int                              lLoopi             = 0;
    int                              lNodeNum           = 0;
    struct tag_igmp_multicast_union *pstTemMultiSession = NULL;

    if ( NULL == pstInterface || NULL == *pstInterface || NULL == pucMultiMacAddr )
    {
        IGMP_STATISTIC_DEBUG("%s","pstInterface or pstInterface or pucMultiMacAddr is NULL.");
        return -1;
    }

    IGMP_STATISTIC_DEBUG("Find interface[%s] multicast session id[%02x:%02x:%02x:%02x:%02x:%02x]",(*pstInterface)->czInterfaceName,
                                                                                                  pucMultiMacAddr[-3],
                                                                                                  pucMultiMacAddr[-2],
                                                                                                  pucMultiMacAddr[-1],
                                                                                                  pucMultiMacAddr[0],
                                                                                                  pucMultiMacAddr[1],
                                                                                                  pucMultiMacAddr[2]);
    
    pstTemMultiSession = (*pstInterface)->stIgmpStatistics.pstMultiSession;
    while ( NULL != pstTemMultiSession && IGMP_MULTI_SESSION_STATISTIC_NODE_NUM_MAX > lNodeNum )
    {
        lLoopi = 0;
        while ( IGMP_SESSION_MAX_NUM > lLoopi )
        {
            IGMP_STATISTIC_DEBUG("Current multicast session[01:00:5e:%02x:%02x:%02x] , finding[%02x:%02x:%02x:%02x:%02x:%02x]",
                                                                                        pstTemMultiSession->ucMultiMacAddr[lLoopi][0],
                                                                                        pstTemMultiSession->ucMultiMacAddr[lLoopi][1],
                                                                                        pstTemMultiSession->ucMultiMacAddr[lLoopi][2],
                                                                                        pucMultiMacAddr[-3],
                                                                                        pucMultiMacAddr[-2],
                                                                                        pucMultiMacAddr[-1],
                                                                                        pucMultiMacAddr[0],
                                                                                        pucMultiMacAddr[1],
                                                                                        pucMultiMacAddr[2]);
            if ( 0 != pstTemMultiSession->ucMultiMacAddr[lLoopi][0] + 
                      pstTemMultiSession->ucMultiMacAddr[lLoopi][1] +
                      pstTemMultiSession->ucMultiMacAddr[lLoopi][2] )
            {
                if ( pstTemMultiSession->ucMultiMacAddr[lLoopi][0] == pucMultiMacAddr[0] 
                  && pstTemMultiSession->ucMultiMacAddr[lLoopi][1] == pucMultiMacAddr[1]
                  && pstTemMultiSession->ucMultiMacAddr[lLoopi][2] == pucMultiMacAddr[2])
                {
                    IGMP_STATISTIC_DEBUG("%s","Find multicast session.");
                    return 0;
                }
            }
            else
            {
                IGMP_STATISTIC_DEBUG("%s","Multicast session list no context.");
                return -1;
            }

            lLoopi++;
        }
        
        lNodeNum++;
        pstTemMultiSession = pstTemMultiSession->next;
    }
    
    IGMP_STATISTIC_DEBUG("%s","Don't find multicast session.");
    return -1;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_cleanmultisession
 ��������  : ����鲥�Ựͳ�ƽ��
 �������  : struct tag_igmp_statistics_interface_st **pstInterface  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��8��12��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static void br_igmp_statistic_cleanmultisession(struct tag_igmp_statistics_interface_st **pstInterface)
{
    int                              lLoopi             = 0;
    struct tag_igmp_multicast_union *pstTemMultiSession = NULL;
    
    if ( NULL == pstInterface || NULL == *pstInterface || NULL == (*pstInterface)->stIgmpStatistics.pstMultiSession )
    {
        return;
    }

    pstTemMultiSession = (*pstInterface)->stIgmpStatistics.pstMultiSession;
    while ( NULL != pstTemMultiSession && IGMP_MULTI_SESSION_STATISTIC_NODE_NUM_MAX > lLoopi )
    {
        memset(pstTemMultiSession->ucMultiMacAddr,0,sizeof(pstTemMultiSession->ucMultiMacAddr));
        lLoopi++;
        pstTemMultiSession = pstTemMultiSession->next;
    }

    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_freemultisession
 ��������  : �ͷ��鲥�Ự����
 �������  : struct tag_igmp_statistics_interface_st **pstInterface  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��8��16��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static void br_igmp_statistic_freemultisession(struct tag_igmp_statistics_interface_st **pstInterface)
{
    int                              lLoopi             = 0;
    struct tag_igmp_multicast_union *pstTemMultiSession = NULL;

    if ( NULL == pstInterface || NULL == *pstInterface || NULL == (*pstInterface)->stIgmpStatistics.pstMultiSession )
    {
        return;
    }

    while ( NULL != (*pstInterface)->stIgmpStatistics.pstMultiSession && IGMP_MULTI_SESSION_STATISTIC_NODE_NUM_MAX > lLoopi )
    {
        pstTemMultiSession = (*pstInterface)->stIgmpStatistics.pstMultiSession;
        (*pstInterface)->stIgmpStatistics.pstMultiSession = (*pstInterface)->stIgmpStatistics.pstMultiSession->next;
        kfree(pstTemMultiSession);
        lLoopi++;
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_addportnode
 ��������  : ���IGMP�˿�ͳ�ƽڵ�
 �������  : struct tag_igmp_port_statistic **pstheader  
             unsigned long ulindex                       
             unsigned long ulport                        
 �������  : ��
 �� �� ֵ  : static struct tag_igmp_port_statistic *
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static struct tag_igmp_port_statistic *br_igmp_statistic_addportnode(struct tag_igmp_port_statistic **pstheader,unsigned long ulindex,unsigned long ulport)
{
    struct tag_igmp_port_statistic *pstCurPortNode = NULL;

    pstCurPortNode = (struct tag_igmp_port_statistic *)kmalloc(sizeof(struct tag_igmp_port_statistic),GFP_KERNEL);
    if ( NULL != pstCurPortNode )
    {
        memset(pstCurPortNode,0,sizeof(struct tag_igmp_port_statistic));
        pstCurPortNode->ulIndex   = ulindex;
        pstCurPortNode->ulPortNum = ulport;

        pstCurPortNode->next  = *pstheader;
        *pstheader            = pstCurPortNode;
        
        return *pstheader;
    }

    return NULL;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_delportnode
 ��������  : ɾ��IGMP�˿�ͳ�ƽڵ�
 �������  : struct tag_igmp_port_statistic **pstheader  
             unsigned long ulindex                       
             unsigned long ulport                        
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static int br_igmp_statistic_delportnode(struct tag_igmp_port_statistic **pstheader,unsigned long ulindex,unsigned long ulport)
{
    struct tag_igmp_port_statistic *pstCurPortNode = NULL;
    struct tag_igmp_port_statistic *pstPrePortNode = NULL;
    
    if ( NULL == *pstheader )
    {
        return -1;
    }

    pstCurPortNode = *pstheader;
    while ( pstCurPortNode )
    {
        if ( ulindex == pstCurPortNode->ulIndex && ulport == pstCurPortNode->ulPortNum )
        {
            if ( NULL == pstPrePortNode )
            {
                *pstheader = pstCurPortNode->next;
            }
            else
            {
                pstPrePortNode->next = pstCurPortNode->next;
            }

            kfree(pstCurPortNode);
            return 0;
        }
        
        pstPrePortNode = pstCurPortNode;
        pstCurPortNode = pstCurPortNode->next;
    }
    
    return -1;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_freeportchain
 ��������  : �ͷ�IGMP�˿�ͳ������
 �������  : struct tag_igmp_port_statistic **pstheader  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static void br_igmp_statistic_freeportchain(struct tag_igmp_port_statistic **pstheader)
{
    struct tag_igmp_port_statistic *pstCurPortNode = NULL;

    if ( NULL == *pstheader )
    {
        return;
    }

    pstCurPortNode = *pstheader;
    while ( *pstheader )
    {
        *pstheader = (*pstheader)->next;

        kfree(pstCurPortNode);
        
        pstCurPortNode = *pstheader;
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_cleanportchain
 ��������  : ���IGMP�˿�ͳ����Ϣ
 �������  : struct tag_igmp_port_statistic **pstheader  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static void br_igmp_statistic_cleanportchain(struct tag_igmp_port_statistic **pstheader)
{
    struct tag_igmp_port_statistic *pstCurPortNode = NULL;
    
    if ( NULL == *pstheader )
    {
        return;
    }

    pstCurPortNode = *pstheader;
    while ( pstCurPortNode )
    {
        pstCurPortNode->ulStatistic = 0;
        pstCurPortNode = pstCurPortNode->next;
    }
    
    return;
}
/*****************************************************************************
 �� �� ��  : br_igmp_statistic_clearcurnode
 ��������  : ��յ�ǰ�ڵ��е�ͳ����Ϣ
 �������  : struct tag_igmp_statistics_interface_st *pstCurNode  
 �������  : ��
 �� �� ֵ  : statistic void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��16��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static void br_igmp_statistic_clearcurnode(struct tag_igmp_statistics_interface_st *pstCurNode)
{
#if 0
    /* BT people need total number */
    pstCurNode->stIgmpStatistics.ulOwnJoinPacketsNum        = 0;
    pstCurNode->stIgmpStatistics.ulOwnLeavePacketsNum       = 0;
    pstCurNode->stIgmpStatistics.ulUpRequestPacketsNum      = 0;
    pstCurNode->stIgmpStatistics.ulDownRetransmitPacketsNum = 0;
    pstCurNode->stIgmpStatistics.ulMultiGroupsNum           = 0;
    pstCurNode->stIgmpStatistics.ulMultiDataPacketsNum      = 0;

    br_igmp_statistic_cleanportchain(&pstCurNode->stIgmpStatistics.pstDownstream);
    br_igmp_statistic_cleanportchain(&pstCurNode->stIgmpStatistics.pstUpstream);
    br_igmp_statistic_cleanmultisession(&pstCurNode);
#else
    pstCurNode->stIgmpStatistics.ulMultiGroupsNum = 0;
    br_igmp_statistic_cleanmultisession(&pstCurNode);
#endif
    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_clearallchain
 ��������  : ��ͳ�����е��������
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��16��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
void br_igmp_statistic_clearallchain(void)
{
    struct tag_igmp_statistics_interface_st *pstCurNode = NULL;

    g_ulIgmpStatisticPauseSwitch = IGMP_STATISTIC_PAUSE_SWITCH_ON;
    
    pstCurNode = g_pstIgmpStatistics;
    while ( pstCurNode )
    {
        /* ����ǰ�ڵ���� */
        br_igmp_statistic_clearcurnode(pstCurNode);
        pstCurNode = pstCurNode->next;
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_findnode
 ��������  : �����豸���Ʋ����豸.��������,������������Ϸ�ʱ����ΪNULL.
 �������  : const char *pcName  
 �������  : ��
 �� �� ֵ  : static struct tag_igmp_statistics_interface_st *
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��7��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
struct tag_igmp_statistics_interface_st * br_igmp_statistic_findnode(const char *pcName)
{
    struct tag_igmp_statistics_interface_st *pstCurNode = NULL;
    
    if ( NULL == pcName )
    {
        IGMP_STATISTIC_DEBUG("%s","pcName is null .");
        return NULL;
    }

    pstCurNode = g_pstIgmpStatistics;
    while ( pstCurNode )
    {
        if ( 0 == strcmp(pstCurNode->czInterfaceName,pcName) )
        {
            /* find */
            return pstCurNode;
        }
        
        pstCurNode = pstCurNode->next;
    }
    
    return NULL;
}
/*****************************************************************************
 �� �� ��  : br_igmp_statistic_addnode
 ��������  : ���ӽڵ�
 �������  : struct tag_igmp_statistics_interface_st *pstAddNode  
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��7��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
struct tag_igmp_statistics_interface_st * br_igmp_statistic_addnode(struct tag_igmp_statistics_interface_st *pstAddNode)
{
    struct tag_igmp_statistics_interface_st *pstNewNode = NULL;
    
    if ( NULL == pstAddNode )
    {
        IGMP_STATISTIC_DEBUG("%s","pstAddNode is null .");
        return NULL;
    }

    pstNewNode = (struct tag_igmp_statistics_interface_st *)kmalloc(sizeof(struct tag_igmp_statistics_interface_st), GFP_KERNEL);
    if ( NULL == pstNewNode )
    {
        IGMP_STATISTIC_DEBUG("%s","kmalloc meet error.");
        return NULL;
    }

    memset(pstNewNode,0,sizeof(struct tag_igmp_statistics_interface_st));
    memcpy(pstNewNode,pstAddNode,sizeof(struct tag_igmp_statistics_interface_st));
    
    if ( NULL == g_pstIgmpStatistics )
    {
        /* first node */
        g_pstIgmpStatistics = pstNewNode;
        g_pstIgmpStatistics->next = NULL;
    }
    else
    {
        pstNewNode->next = g_pstIgmpStatistics;
        g_pstIgmpStatistics = pstNewNode;
    }
    
    return pstNewNode;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_delnode
 ��������  : �����豸��ɾ���ڵ�
 �������  : const char *pcName  
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��7��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static int br_igmp_statistic_delnode(const char *pcName)
{
    struct tag_igmp_statistics_interface_st *pstCurNode = NULL;
    struct tag_igmp_statistics_interface_st *pstPreNode = NULL;
    
    if ( NULL == pcName )
    {
        IGMP_STATISTIC_DEBUG("%s","pcName is null.");
        return -1;
    }

    pstCurNode = g_pstIgmpStatistics;
    while ( pstCurNode )
    {
        if ( 0 == strcmp(pstCurNode->czInterfaceName,pcName) )
        {
            /* find */
            if ( NULL == pstPreNode )
            {
                g_pstIgmpStatistics = pstCurNode->next;
            }
            else
            {
                pstPreNode->next = pstCurNode->next;                
            }

            br_igmp_statistic_freeportchain(&pstCurNode->stIgmpStatistics.pstDownstream);
            br_igmp_statistic_freeportchain(&pstCurNode->stIgmpStatistics.pstUpstream);

            br_igmp_statistic_freemultisession(&pstCurNode);
            
            kfree(pstCurNode);
            pstCurNode = NULL;

            return 0;
        }
        
        pstPreNode = pstCurNode;
        pstCurNode = pstCurNode->next;
    }
    
    return 0;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_add_multigroup
 ��������  : ����鲥���¼
 �������  : struct tag_igmp_statistics_interface_st *pstCurNode  
             unsigned int                             address     
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��12��16��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static void br_igmp_statistic_add_multigroup
(
    struct tag_igmp_statistics_interface_st *pstCurNode,
    unsigned int                             address
)
{
    unsigned int ulLoopi = 0;

    for ( ulLoopi = 0 ; ulLoopi < IGMP_SESSION_MAX_NUM ; ulLoopi++ )
    {
        if ( 0 == pstCurNode->stIgmpGroup[ulLoopi].address 
          || IGMP_STATISTIC_GROUP_STATS_LEAVE == pstCurNode->stIgmpGroup[ulLoopi].stats)
        {
            IGMP_STATISTIC_DEBUG("Add dev[%s] address[%x].",pstCurNode->czInterfaceName,address);
            pstCurNode->stIgmpGroup[ulLoopi].address = address;
            pstCurNode->stIgmpGroup[ulLoopi].stats   = IGMP_STATISTIC_GROUP_STATS_JOIN;
            break;
        }
    }

    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_find_multigroup
 ��������  : ��ѯ�鲥���¼
 �������  : struct tag_igmp_statistics_interface_st *pstCurNode  
             unsigned int                             address     
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��12��16��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static struct tag_igmp_statistics_interface_group_record_st * 
br_igmp_statistic_find_multigroup
(
    struct tag_igmp_statistics_interface_st *pstCurNode,
    unsigned int                             address
)
{
    unsigned int ulLoopi = 0;
    
    for ( ulLoopi = 0 ; ulLoopi < IGMP_SESSION_MAX_NUM ; ulLoopi++ )
    {
        /* ʹ�ý��սṹ,addressΪ0�����û�м�¼ */
        if ( 0 == pstCurNode->stIgmpGroup[ulLoopi].address )
        {
            return NULL;
        }

        IGMP_STATISTIC_DEBUG("dev[%s] CurAddress[%x] FindingAddress[%x].",
            pstCurNode->czInterfaceName,pstCurNode->stIgmpGroup[ulLoopi].address,address);
        if ( address == pstCurNode->stIgmpGroup[ulLoopi].address )
        {
            return &pstCurNode->stIgmpGroup[ulLoopi];
        }

    }

    return NULL;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_joinorleave_judgement
 ��������  : �жϼ�����뿪���Ƿ�����ͳ����������������ʱ�����ǰ��û�м�
             ����ͳ�ƣ��뿪ʱ�����ǰ��û���뿪��ͳ�ơ�
 �������  : struct tag_igmp_statistics_interface_st *pstCurNode  
             int                                      stats       
             unsigned int                             address     
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��12��16��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
static int br_igmp_statistic_joinorleave_judgement
(
    struct tag_igmp_statistics_interface_st *pstCurNode,
    int                                      stats,
    unsigned int                             address
)
{
    struct tag_igmp_statistics_interface_group_record_st *pstmultirecord = NULL;
    if ( IGMP_UPNP_ADDRESS == address 
      || IGMP_MDNS_ADDRESS == address
      || IGMP_LOCAL_ADDRESS == (address & 0xff000000))
    {
        return 0;
    }
    
    pstmultirecord = br_igmp_statistic_find_multigroup(pstCurNode,address);
    switch ( stats )
    {
        case IGMP_JOIN_GROUP_PASKET :
            if ( NULL != pstmultirecord )
            {
                if ( IGMP_STATISTIC_GROUP_STATS_JOIN != pstmultirecord->stats )
                {
                    pstmultirecord->stats = IGMP_STATISTIC_GROUP_STATS_JOIN;
                    IGMP_STATISTIC_DEBUG("Stats change : dev[%s] address[%x].",pstCurNode->czInterfaceName,address);
                    return 1;
                }
            }
            else
            {
                /* Don't include this group. */
                br_igmp_statistic_add_multigroup(pstCurNode,address);
                IGMP_STATISTIC_DEBUG("Add dev[%s] address[%x].",pstCurNode->czInterfaceName,address);
                return 1;
            }
            break;
        case IGMP_LEAVE_GROUP_PASKET :
            if ( NULL != pstmultirecord )
            {
                if ( IGMP_STATISTIC_GROUP_STATS_JOIN == pstmultirecord->stats )
                {
                    pstmultirecord->stats = IGMP_STATISTIC_GROUP_STATS_LEAVE;
                    IGMP_STATISTIC_DEBUG("Leave dev[%s] address[%x].",pstCurNode->czInterfaceName,address);
                    return 1;
                }
            }
            break;
        default:
            return 0;
    }
    return 0;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_incparameter
 ��������  : ������������ͳ�ƣ�д����Ӧ�ļ�
 �������  : struct sk_buff *skb
             int type                   
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��7��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
int br_igmp_statistic_incparameter
(
    struct sk_buff *skb,
    int             type,
    void           *parameter
)
{
    struct tag_igmp_statistics_interface_st *pstCurNode     = NULL;
    struct tag_igmp_port_statistic          *pstCurPortNode = NULL;
    struct tag_igmp_statistics_interface_st  stTempNode;
    struct in_device     *in_dev        = NULL;
    const unsigned char  *dest          = NULL;
    const unsigned char  *tmpdest       = NULL;
    char                 *pcTempDevName = NULL;
    unsigned short        usDstPort     = 0;
    unsigned short        usSrcPort     = 0;
    unsigned char        *data          = skb->data;
    unsigned short        usIpHeadLen   = 0;
    
    if ( NULL != skb->dev )
    {
        in_dev        = __in_dev_get_rtnl(skb->dev);
        pcTempDevName = skb->dev->name;
    }
    else if  ( NULL != skb->dst->dev )
    {
        in_dev        = __in_dev_get_rtnl(skb->dst->dev);
        pcTempDevName = skb->dst->dev->name;
    }
    else
    {
        IGMP_STATISTIC_DEBUG("%s","skb->dev is null.");
        return -1;
    }

    pstCurNode = br_igmp_statistic_findnode(pcTempDevName);
    if ( NULL == pstCurNode )
    {
        /* ֻ����������ӵĽӿڲŽ���ͳ�� */
#if 0
        /* add node */
        memset(&stTempNode,0,sizeof(struct tag_igmp_statistics_interface_st));
        strcpy(stTempNode.czInterfaceName,skb->dev->name);
        if ( NULL == (pstCurNode = br_igmp_statistic_addnode(&stTempNode)) )
        {
            IGMP_STATISTIC_DEBUG("%s","br_igmp_statistic_addnode meet error.");
            return -1;
        }
#else
        return 0;
#endif
    }
    
    switch ( type )
    {
        case IGMP_JOIN_GROUP_PASKET :
            if ( NULL == parameter || 0 == *(unsigned int*)parameter )
            {
                return;
            }
            else if ( br_igmp_statistic_joinorleave_judgement(pstCurNode,IGMP_JOIN_GROUP_PASKET,ntohl(*(unsigned int*)parameter)) )
            {
                pstCurNode->stIgmpStatistics.ulOwnJoinPacketsNum++;
            }
            break;
        case IGMP_LEAVE_GROUP_PASKET :
            if ( NULL == parameter || 0 == *(unsigned int*)parameter )
            {
                return;
            }
            else if ( br_igmp_statistic_joinorleave_judgement(pstCurNode,IGMP_LEAVE_GROUP_PASKET,ntohl(*(unsigned int*)parameter)) )
            {
                pstCurNode->stIgmpStatistics.ulOwnLeavePacketsNum++;
            }
            break;
        case IGMP_UP_REQUEST_PACKET :
        {
            /* ͳ�Ƶ�������retransmit������ */
            if ( !is_multicast_ether_addr(eth_hdr(skb)->h_dest) )
            {
                if ( NULL != pstCurNode->stIgmpStatistics.pstUpstream )
                {
                    usIpHeadLen = (skb->data[0] & 0x0f) * 4;
                    usDstPort   = data[usIpHeadLen + 2];
                    usDstPort   = ((usDstPort<<8)&0xFF00) | data[usIpHeadLen + 3];
                    
                    /* ����Ŀ�Ķ˿�ͳ�� */
                    pstCurPortNode = pstCurNode->stIgmpStatistics.pstUpstream;
                    while ( pstCurPortNode )
                    {
                        if ( pstCurPortNode->ulPortNum == usDstPort )
                        {
                            /* �ñ��Ĳ���Ҫ���� */
                            blog_skip(skb);
                            pstCurPortNode->ulStatistic++;
                            IGMP_STATISTIC_DEBUG("type[UP_REQUEST] dev[%s] port[%u]",
                            __FILE__,__func__,__LINE__,skb->dev->name,pstCurPortNode->ulPortNum);
                            break;
                        }
                        
                        pstCurPortNode = pstCurPortNode->next;
                    }
                }

                //pstCurNode->stIgmpStatistics.ulUpRequestPacketsNum++;
            }
            break;
        }
        case IGMP_DOWN_RETRANSMIT_PACKET :
        {
            /* ͳ�Ƶ�������retransmit������ */
            if ( !is_multicast_ether_addr(eth_hdr(skb)->h_dest) )
            {
                if ( NULL != pstCurNode->stIgmpStatistics.pstDownstream )
                {
                    usIpHeadLen = (skb->data[0] & 0x0f) * 4;
                    usSrcPort   = data[usIpHeadLen];
                    usSrcPort   = ((usSrcPort<<8)&0xFF00) | data[usIpHeadLen + 1];
                    
                    /* ����Ŀ�Ķ˿�ͳ�� */
                    pstCurPortNode = pstCurNode->stIgmpStatistics.pstDownstream;
                    while ( pstCurPortNode )
                    {
                        if ( pstCurPortNode->ulPortNum == usSrcPort )
                        {
                            /* �ñ��Ĳ���Ҫ���� */
                            blog_skip(skb);
                            pstCurPortNode->ulStatistic++;
                            IGMP_STATISTIC_DEBUG("type[DOWN_RETRANSMIT] dev[%s] port[%u]",
                            __FILE__,__func__,__LINE__,skb->dev->name,pstCurPortNode->ulPortNum);
                            break;
                        }
                        
                        pstCurPortNode = pstCurPortNode->next;
                    }
                }
                
                //pstCurNode->stIgmpStatistics.ulDownRetransmitPacketsNum++;
            }
            break;
        }
        case IGMP_MULTI_GROUP :
            if ( NULL == in_dev )
            {
                IGMP_STATISTIC_DEBUG("%s","in_dev is null.");
                return -1;
            }
            
            if ( is_multicast_ether_addr(eth_hdr(skb)->h_dest) 
             && (IS_LAN_DEV(in_dev->dev->name) || IS_IP_WAN_DEVICE(in_dev->dev->name)) )
            {
#if 0
                /* ʹ��igmp snoopingʵ���鲥�Ựͳ�� */
                pstCurNode->stIgmpStatistics.ulMultiGroupsNum = br_igmp_snooping_find_intrface_group_num(in_dev->dev->name);
#else
                /* ʹ��ͳ��ʱ�����10���UDP������Ϊ�鲥�Ự���� */
                dest = eth_hdr(skb)->h_dest;
                if ( NULL == dest )
                {
                    IGMP_STATISTIC_DEBUG("%s","dest is null.");
                    return -1;
                }

                tmpdest = &dest[3];
                if ( 0 != br_igmp_statistic_findmultisession(&pstCurNode,tmpdest) )
                {
                    if ( 0 == br_igmp_statistic_addmultisession(&pstCurNode,tmpdest) )
                    {
                        pstCurNode->stIgmpStatistics.ulMultiGroupsNum++;
                    }
                    
                }
#endif
            }
            else if ( IS_PPP_WAN_DEVICE(in_dev->dev->name) )
            {
                pstCurNode->stIgmpStatistics.ulMultiGroupsNum = in_dev->mc_count;
            }
            break;
        case IGMP_MULTI_DATA_PACKET :
            if( is_multicast_ether_addr(eth_hdr(skb)->h_dest) )
            {
                pstCurNode->stIgmpStatistics.ulMultiDataPacketsNum++;
            }
            break;
        default:
            IGMP_STATISTIC_DEBUG("Type %d is invalid.",type);
            return -1;
            
    }
    
    return 0;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_inc
 ��������  : ���ݴ�������ݰ��ı��Ӧ�豸��ͳ��ֵ
 �������  : struct sk_buff *skb  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��7��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
int br_igmp_statistic_inc(struct sk_buff *skb,int type)
{
    int                  lRet        = 0;
    int                  lLoop       = 0;
    unsigned char       *data        = NULL;
    unsigned int        *address     = NULL;
    unsigned short       usIpHeadLen = 0;
    unsigned short       protocol    = 0;
    /* BEGIN: Added by z67728, 2010/8/12 Add for multicast section statistic .*/
    static unsigned long ulOldTime   = 0;
    static unsigned long ulCurTime   = 0;
    /* END:   Added by z67728, 2010/8/12 */
    struct igmpv3_report *pstRepV3   = NULL;
    struct igmpv3_grec   *pstGrec    = NULL;

    if ( 1 != g_ulIgmpStatisticEnableSwitch )
    {
        /* BEGIN: Added by z67728, 2010/8/12 Add for multicast section statistic .*/
        if ( ulOldTime | ulCurTime )
        {            
            ulOldTime = 0;
            ulCurTime = 0;
        }
        /* END:   Added by z67728, 2010/8/12 */
        return 0;
    }

    if ( NULL == skb || NULL == skb->dev )
    {
        IGMP_STATISTIC_DEBUG("%s","Skb is NULL.");
        return -1;
    }

    data = skb->data;
    if ( NULL == data )
    {
        IGMP_STATISTIC_DEBUG("%s","data is NULL.");
        return -1;
    }
    
    /* BEGIN: Added by z67728, 2010/8/12 Add for multicast section statistic .*/    
    if ( 0 == ulOldTime )
    {
        ulOldTime = jiffies;
    }

    ulCurTime = jiffies;
    /* END:   Added by z67728, 2010/8/12 */
    
    protocol = ((u16 *) data)[-1];
    switch ( data[9] )
    {
        case IPPROTO_IGMP :
        {
            if ( IGMP_BUFF == type )
            {
                usIpHeadLen = (skb->data[0] & 0x0f) * 4;
                if ((data[usIpHeadLen] == IGMPV2_HOST_MEMBERSHIP_REPORT 
                    || data[usIpHeadLen] == IGMP_HOST_MEMBERSHIP_REPORT)
                     && protocol == __constant_htons(ETH_P_IP))
                {
                    address = (unsigned int *)&data[usIpHeadLen + 4];
                    IGMP_STATISTIC_DEBUG("Join address %x.",ntohl(*address));
                    br_igmp_statistic_incparameter(skb,IGMP_JOIN_GROUP_PASKET,address);
                }
                else if (data[usIpHeadLen] == IGMPV3_HOST_MEMBERSHIP_REPORT && protocol == __constant_htons(ETH_P_IP)) 
                {
                    pstRepV3 = (struct igmpv3_report *)&data[usIpHeadLen];
                    pstGrec = &pstRepV3->grec[0];
                    for (lLoop = 0; lLoop < pstRepV3->ngrec; lLoop++) 
                    {
                        if ((IGMPV3_MODE_IS_EXCLUDE == pstGrec->grec_type) || 
                            (IGMPV3_CHANGE_TO_EXCLUDE == pstGrec->grec_type) || 
                            (pstGrec->grec_nsrcs != 0 && IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type) || 
                            (pstGrec->grec_nsrcs != 0 && IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type) ||
                            (IGMPV3_ALLOW_NEW_SOURCES == pstGrec->grec_type)) 
                        {
                            /* V3 join */
                            IGMP_STATISTIC_DEBUG("Join address %x.",ntohl(pstGrec->grec_mca));
                            br_igmp_statistic_incparameter(skb,IGMP_JOIN_GROUP_PASKET,&pstGrec->grec_mca);
                		}
                		else if ((IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type && pstGrec->grec_nsrcs == 0) || 
                                 (pstGrec->grec_nsrcs == 0 && IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type) || 
                                 (IGMPV3_BLOCK_OLD_SOURCES == pstGrec->grec_type)) 
                        {
                		    /* V3 Leave */
                            IGMP_STATISTIC_DEBUG("Leave address %x.",ntohl(pstGrec->grec_mca));
                            br_igmp_statistic_incparameter(skb,IGMP_LEAVE_GROUP_PASKET,&pstGrec->grec_mca);
                        }
                        pstGrec = (struct igmpv3_grec *)((char *)pstGrec + sizeof(struct igmpv3_grec) + pstGrec->grec_nsrcs * sizeof(struct in_addr)); 
                    }
                }
                else if (data[usIpHeadLen] == IGMP_HOST_LEAVE_MESSAGE) 
                {
                    address = (unsigned int *)&data[usIpHeadLen + 4];
                    IGMP_STATISTIC_DEBUG("Leave address %x.",ntohl(*address));
                    br_igmp_statistic_incparameter(skb,IGMP_LEAVE_GROUP_PASKET,address);
                }
            }
            else
            {
                /* �÷�֧�����ظ�ͳ�ƿ�����Ľ��յ����鲥�� */
                return 0;
            }
            break;
        }
        case IPPROTO_UDP :
        {
            if ( IGMP_STATISTIC_PAUSE_SWITCH_ON == g_ulIgmpStatisticPauseSwitch )
            {
                ulOldTime = jiffies;
                ulCurTime = jiffies;
                g_ulIgmpStatisticPauseSwitch = IGMP_STATISTIC_PAUSE_SWITCH_OFF;
            }

            if ( NULL != skb->dev )
            {            
                /* �鲥������ͳ�� */
                switch ( type )
                {
                    //case IGMP_BUFF :
                    case IGMP_DOWN_RETRANSMIT_PACKET :
                        if ( 1 == time_in_range(ulCurTime,ulOldTime,(ulOldTime + IGMP_MULTI_SESSION_STATISTIC_INTERVAL)) )
                        {
                            if ( (IGMP_BUFF == type && IS_IP_WAN_DEVICE(skb->dev->name)) 
                              || (IGMP_DOWN_RETRANSMIT_PACKET == type) )
                            {
                                br_igmp_statistic_incparameter(skb,IGMP_MULTI_GROUP,NULL);
                            }
                        }
                        else if ( 1 == time_after(ulCurTime,(ulOldTime + IGMP_MULTI_SESSION_STATISTIC_TIME_MAX)) )
                        {
                            ulOldTime = 0;
                        }
                    case IGMP_UP_REQUEST_PACKET :
                        br_igmp_statistic_incparameter(skb,type,NULL);
                        br_igmp_statistic_incparameter(skb,IGMP_MULTI_DATA_PACKET,NULL);
                        break;
                    default:
                        break;
                }
            }
            break;
        }    
        default:
            return -1;
    }

    return 0;
}


/*****************************************************************************
 �� �� ��  : ppp_igmp_statistic_inc
 ��������  : ·��WAN�е�IGMPͳ�ƹ���
 �������  : struct sk_buff *skb  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
int ppp_igmp_statistic_inc(struct sk_buff *skb)
{
    struct igmphdr *igmpheader = NULL;
    struct igmpv3_report *pig = NULL;
    int    lGroupNum = 0;

    if ( 1 != g_ulIgmpStatisticEnableSwitch )
    {
        return 0;
    }
    
    if ( NULL == skb || (NULL == skb->dev && NULL == skb->dst->dev) )
    {
        IGMP_STATISTIC_DEBUG("%s","Skb or dev header is NULL.");
        return -1;
    }

    igmpheader = igmp_hdr(skb);
    if ( NULL == igmpheader )
    {
        IGMP_STATISTIC_DEBUG("%s","Igmp header is NULL.");
        return -1;
    }

    switch ( igmpheader->type )
    {
        case IGMP_HOST_LEAVE_MESSAGE :
            IGMP_STATISTIC_DEBUG("%s","PPP leave massage.");
            IGMP_STATISTIC_DEBUG("Leave address %x.",ntohl(igmpheader->group));
            br_igmp_statistic_incparameter(skb,IGMP_LEAVE_GROUP_PASKET,&igmpheader->group);
            break;
        case IGMPV2_HOST_MEMBERSHIP_REPORT :
            IGMP_STATISTIC_DEBUG("%s","PPP igmpv2 join massage.");
            IGMP_STATISTIC_DEBUG("Join address %x.",ntohl(igmpheader->group));
            br_igmp_statistic_incparameter(skb,IGMP_JOIN_GROUP_PASKET,&igmpheader->group);
            break;
        case IGMPV3_HOST_MEMBERSHIP_REPORT :
            IGMP_STATISTIC_DEBUG("%s","PPP igmpv3 massage.");
            pig = igmpv3_report_hdr(skb);
            if ( NULL != pig )
            {                
                for ( lGroupNum = 0 ; lGroupNum < pig->ngrec; lGroupNum++ )
                {
                    if ((IGMPV3_MODE_IS_EXCLUDE == pig->grec[lGroupNum].grec_type) || 
                        (IGMPV3_CHANGE_TO_EXCLUDE == pig->grec[lGroupNum].grec_type) || 
                        (pig->grec[lGroupNum].grec_nsrcs != 0 && IGMPV3_MODE_IS_INCLUDE   == pig->grec[lGroupNum].grec_type) || 
                        (pig->grec[lGroupNum].grec_nsrcs != 0 && IGMPV3_CHANGE_TO_INCLUDE == pig->grec[lGroupNum].grec_type) ||
                        (IGMPV3_ALLOW_NEW_SOURCES == pig->grec[lGroupNum].grec_type))
                    {
                        IGMP_STATISTIC_DEBUG("%s","PPP igmpv3 join massage.");
                        IGMP_STATISTIC_DEBUG("Join address %x.",ntohl(pig->grec[lGroupNum].grec_mca));
                        br_igmp_statistic_incparameter(skb,IGMP_JOIN_GROUP_PASKET,&pig->grec[lGroupNum].grec_mca);
                    }
                    else if ((IGMPV3_CHANGE_TO_INCLUDE == pig->grec[lGroupNum].grec_type && pig->grec[lGroupNum].grec_nsrcs == 0) || 
                             (pig->grec[lGroupNum].grec_nsrcs == 0 && IGMPV3_MODE_IS_INCLUDE == pig->grec[lGroupNum].grec_type) || 
                             (IGMPV3_BLOCK_OLD_SOURCES == pig->grec[lGroupNum].grec_type))
                    {
                        IGMP_STATISTIC_DEBUG("%s","PPP igmpv3 leave massage.");
                        IGMP_STATISTIC_DEBUG("Leave address %x.",ntohl(pig->grec[lGroupNum].grec_mca));
                        br_igmp_statistic_incparameter(skb,IGMP_LEAVE_GROUP_PASKET,&pig->grec[lGroupNum].grec_mca);
                    }
                }
            }
            break;
        default:
            IGMP_STATISTIC_DEBUG("PPP error type message %d .",igmpheader->type);
            return -1;
    }

    //br_igmp_statistic_incparameter(skb,IGMP_MULTI_DATA_PACKET,NULL);
    return 0;
}

/*****************************************************************************
 �� �� ��  : ppp_ip_statistic_inc
 ��������  : IP��UDP����ͳ��
 �������  : struct sk_buff *skb  
             int type             
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��11��26��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
int ppp_ip_statistic_inc(struct sk_buff *skb,int type)
{
    struct iphdr                            *iph            = NULL;
    char                                    *pcTemp         = NULL;
    unsigned short                          *psSport        = NULL;
    unsigned short                          *psDport        = NULL;
    struct tag_igmp_statistics_interface_st *pstCurNode     = NULL;
    struct tag_igmp_port_statistic          *pstCurPortNode = NULL;
    unsigned short                           usSport        = 0;
    unsigned short                           usDport        = 0;

    if ( 1 != g_ulIgmpStatisticEnableSwitch )
    {
        return 0;
    }
    
    if ( NULL == skb || NULL == skb->dev )
    {
        return -1;
    }

    pcTemp = iph = ip_hdr(skb);
    switch ( type )
    {
        case IGMP_UP_REQUEST_PACKET :
            if ( NULL == skb->dst || NULL == skb->dst->dev )
            {
                return -1;
            }
            pstCurNode = br_igmp_statistic_findnode(skb->dst->dev->name);
            IGMP_STATISTIC_DEBUG("IGMP_UP_REQUEST_PACKET IP packets transmit to %s, protocol is %x.",skb->dst->dev->name,iph->protocol);
            break;
        case IGMP_DOWN_RETRANSMIT_PACKET :            
            pstCurNode = br_igmp_statistic_findnode(skb->dev->name);
            IGMP_STATISTIC_DEBUG("IGMP_DOWN_RETRANSMIT_PACKET IP packets come from %s, protocol is %x.",skb->dev->name,iph->protocol);
            break;
        default:
            return -1;
    }

    if ( NULL == pstCurNode )
    {
        return 0;
    }
    
    if ( IP_PROTOCOL_UDP == iph->protocol )
    {
        pcTemp  = pcTemp + iph->ihl * 4;
        psSport = pcTemp;
        psDport = pcTemp + 2;

        if ( NULL == psSport || NULL == psDport )
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    usSport = ntohs(*psSport);
    usDport = ntohs(*psDport);
    IGMP_STATISTIC_DEBUG("Sport %x[%u].",usSport,usSport);
    IGMP_STATISTIC_DEBUG("Dport %x[%u].",usDport,usDport);
    switch ( type )
    {
        case IGMP_UP_REQUEST_PACKET :
            if ( NULL != pstCurNode->stIgmpStatistics.pstUpstream )
            {
                /* ����Ŀ�Ķ˿�ͳ�� */
                pstCurPortNode = pstCurNode->stIgmpStatistics.pstUpstream;
                while ( pstCurPortNode )
                {
                    if ( pstCurPortNode->ulPortNum == usDport )
                    {
                        /* �ñ��Ĳ���Ҫ���� */
                        blog_skip(skb);
                        pstCurPortNode->ulStatistic++;
                        IGMP_STATISTIC_DEBUG("type[UP_REQUEST] dev[%s] port[%u] num[%u].",
                        skb->dst->dev->name,pstCurPortNode->ulPortNum,pstCurPortNode->ulStatistic);
                        break;
                    }
                    
                    pstCurPortNode = pstCurPortNode->next;
                }
            }
            break;
        case IGMP_DOWN_RETRANSMIT_PACKET :
            if ( NULL != pstCurNode->stIgmpStatistics.pstDownstream )
            {
                /* ����Ŀ�Ķ˿�ͳ�� */
                pstCurPortNode = pstCurNode->stIgmpStatistics.pstDownstream;
                while ( pstCurPortNode )
                {
                    if ( pstCurPortNode->ulPortNum == usSport )
                    {
                        /* �ñ��Ĳ���Ҫ���� */
                        blog_skip(skb);
                        pstCurPortNode->ulStatistic++;
                        IGMP_STATISTIC_DEBUG("type[DOWN_RETRANSMIT] dev[%s] port[%u] num[%u]",
                        skb->dev->name,pstCurPortNode->ulPortNum,pstCurPortNode->ulStatistic);
                        break;
                    }
                    
                    pstCurPortNode = pstCurPortNode->next;
                }
            }
            break;
        default:
            return -1;
    }
    return 0;
}
    
/*****************************************************************************
 �� �� ��  : br_igmp_statistic_print
 ��������  : ��ӡIGMPͳ����Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
void br_igmp_statistic_print()
{
    int                                      lIndex             = 1;
    int                                      lLoopi             = 0;
    struct tag_igmp_multicast_union         *pstTemMultiSession = NULL;
    struct tag_igmp_statistics_interface_st *pstCurNode         = NULL;
    struct tag_igmp_port_statistic          *pstCurPortNode     = NULL;
    
    printk("\r\n\r\n");
    printk("-------------------------------------------------------------------------------------------------------\r\n");
    if ( 0 == g_ulIgmpStatisticEnableSwitch )
    {
        printk("IGMP STATISTIC SWITCH is : Disable . \r\n");
    }
    else
    {        
        printk("IGMP STATISTIC SWITCH is : Enable . \r\n");
    }

    printk("-------------------------------------------------------------------------------------------------------\r\n");
    
    printk("\r\n\r\n");
    printk("-------------------------------------------------------------------------------------------------------\r\n");
    printk("| Index | Interface | JoinNum | Leave Num | Up_Re_Num | Down_Re_Num | MultiSession | Multi_Packet_Num |\r\n");

    pstCurNode = g_pstIgmpStatistics;
    while ( NULL != pstCurNode )
    {
        printk("    %-4d   %-10s     %-8u  %-10u     %-8u     %-9u     %-10u     %-13u \r\n",lIndex,
                                                                                        pstCurNode->czInterfaceName,
                                                                                        pstCurNode->stIgmpStatistics.ulOwnJoinPacketsNum,
                                                                                        pstCurNode->stIgmpStatistics.ulOwnLeavePacketsNum,
                                                                                        pstCurNode->stIgmpStatistics.ulUpRequestPacketsNum,
                                                                                        pstCurNode->stIgmpStatistics.ulDownRetransmitPacketsNum,
                                                                                        pstCurNode->stIgmpStatistics.ulMultiGroupsNum,
                                                                                        pstCurNode->stIgmpStatistics.ulMultiDataPacketsNum);

        pstCurPortNode = pstCurNode->stIgmpStatistics.pstDownstream;
        while ( NULL != pstCurPortNode )
        {
            printk("                    Down Stream Port statistic : Num[%u] Port[%u] Statistic[%u]\r\n",pstCurPortNode->ulIndex,pstCurPortNode->ulPortNum,pstCurPortNode->ulStatistic);
            pstCurPortNode = pstCurPortNode->next;
        }

        pstCurPortNode = pstCurNode->stIgmpStatistics.pstUpstream;
        while ( pstCurPortNode )
        {
            printk("                      Up Stream Port statistic : Num[%u] Port[%u] Statistic[%u]\r\n",pstCurPortNode->ulIndex,pstCurPortNode->ulPortNum,pstCurPortNode->ulStatistic);
            pstCurPortNode = pstCurPortNode->next;
        }

        pstTemMultiSession = pstCurNode->stIgmpStatistics.pstMultiSession;
        while ( NULL != pstTemMultiSession )
        {
            lLoopi = 0;
            while ( IGMP_SESSION_MAX_NUM > lLoopi )
            {
#if 1
                if ( 0 != pstTemMultiSession->ucMultiMacAddr[lLoopi][0] + 
                          pstTemMultiSession->ucMultiMacAddr[lLoopi][1] +
                          pstTemMultiSession->ucMultiMacAddr[lLoopi][2] )
                {
                    printk("                      Current multicast session[01:00:5e:%02x:%02x:%02x]\r\n",pstTemMultiSession->ucMultiMacAddr[lLoopi][0],
                                                                                                          pstTemMultiSession->ucMultiMacAddr[lLoopi][1],
                                                                                                          pstTemMultiSession->ucMultiMacAddr[lLoopi][2]);
                }
                else
                {
                    break;
                }
#else
                printk("                      Current multicast session[01:00:5e:%02x:%02x:%02x]\r\n",pstTemMultiSession->ucMultiMacAddr[lLoopi][0],
                                                                                                      pstTemMultiSession->ucMultiMacAddr[lLoopi][1],
                                                                                                      pstTemMultiSession->ucMultiMacAddr[lLoopi][2]);
#endif
                lLoopi++;
            }
            pstTemMultiSession = pstTemMultiSession->next;
        }
        
        lIndex++;
        pstCurNode = pstCurNode->next;
    }
    
    printk("-------------------------------------------------------------------------------------------------------\r\n");

    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_setenable
 ��������  : ʹ�ܺ�ȥʹ��IGMPͳ�ƹ���
 �������  : int enable  
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
void br_igmp_statistic_setenable(int enable)
{
    if ( 0 != enable && 1 != enable )
    {
        return;
    }
    
    g_ulIgmpStatisticEnableSwitch = enable;
    return;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_getchainheader
 ��������  : ��ȡIGMPͳ��ͷ���
 �������  : void  
 �������  : ��
 �� �� ֵ  : struct tag_igmp_statistics_interface_st *
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��21��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
struct tag_igmp_statistics_interface_st * br_igmp_statistic_getchainheader(void)
{
    return g_pstIgmpStatistics;
}

/*****************************************************************************
 �� �� ��  : br_igmp_statistic_setinterfaceport
 ��������  : ����ָ���ӿڰ���һ���˿ڹ���ͳ����Ϣ
 �������  : char *pcinterface    : �ӿ����� 
             int lacction         : ��ӻ�ɾ�� 1 : add, 2 : del
             int lindex           : ʵ���� 
             char *pcportattr     : Դ�˿ڻ�Ŀ�Ķ˿� 
             unsigned long ulport : �˿ں� 
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2010��7��20��
    ��    ��   : z67728
    �޸�����   : �����ɺ���

*****************************************************************************/
void br_igmp_statistic_setinterfaceport(char *pcinterface, unsigned long ulacction, unsigned long ulindex, unsigned long ulportattr, unsigned long ulport)
{
    struct tag_igmp_statistics_interface_st *pstCurInterface = NULL;
    struct tag_igmp_port_statistic         **pstCurPortNode  = NULL;
    struct tag_igmp_statistics_interface_st  stTempNode;
    
    if ( NULL == pcinterface )
    {
        return;
    }

    IGMP_STATISTIC_DEBUG("br_igmp_statistic_setinterfaceport : Interface[%s] Acction[%u] Index[%u] PortAttr[%u] Port[%u] .",pcinterface,
                                                                                                                            ulacction,
                                                                                                                            ulindex,
                                                                                                                            ulportattr,
                                                                                                                            ulport);
    
    pstCurInterface = br_igmp_statistic_findnode(pcinterface);
    if ( NULL == pstCurInterface )
    {
        if ( IGMP_STATISTIC_PORT_ADD == ulacction )
        {
            memset(&stTempNode,0,sizeof(struct tag_igmp_statistics_interface_st));
            strcpy(stTempNode.czInterfaceName,pcinterface);
            if ( NULL == (pstCurInterface = br_igmp_statistic_addnode(&stTempNode)) )
            {
                IGMP_STATISTIC_DEBUG("%s","br_igmp_statistic_addnode meet error.");
                return;
            }
        }
        else
        {
            return;
        }
    }

    if ( ulportattr == IGMP_STATISTIC_PORT_SRC )
    {
        IGMP_STATISTIC_DEBUG("%s","Choice down stream chain .");
        pstCurPortNode = &pstCurInterface->stIgmpStatistics.pstDownstream;
    }
    else if ( ulportattr == IGMP_STATISTIC_PORT_DST )
    {
        IGMP_STATISTIC_DEBUG("%s","Choice up stream chain .");
        pstCurPortNode = &pstCurInterface->stIgmpStatistics.pstUpstream;
    }

    switch ( ulacction )
    {
        case IGMP_STATISTIC_PORT_ADD :
            IGMP_STATISTIC_DEBUG("%s","Add port node .");
            br_igmp_statistic_addportnode(pstCurPortNode,ulindex,ulport);
            break;
        case IGMP_STATISTIC_PORT_DEL :
            IGMP_STATISTIC_DEBUG("%s","Delete port node .");
            br_igmp_statistic_delportnode(pstCurPortNode,ulindex,ulport);
            break;
        default:
            break;
    }
    
    return;
}

