/******************************************************************************
  版权所有  : 2008-2020，华为技术有限公司
  文 件 名  : atp_interface.h
  作    者  : c47036
  版    本  : 1.0
  创建日期  : 2008-10-31
  描    述  : 
  函数列表  :

  历史记录      :
   1.日    期   : 2008-10-31
     作    者   : c47036
     修改内容   : 完成初稿

*********************************************************************************/
#if !defined(_ATP_INTERFACE_H_)
#define _ATP_INTERFACE_H_

#ifdef MTK_CHIP_NAME
#define ATP_LAN1                            "eth0.2"
#define ATP_LAN2                            "eth0.4"
#define ATP_LAN3                            "eth0.3"
#define ATP_LAN4                            "eth0.5"
#else 

#ifdef SUPPORT_ATP_ETH_PORT_ORDER_REVERSED     
//LAN接口宏定义
#define ATP_LAN1                            "eth0.5"
#define ATP_LAN2                            "eth0.4"
#define ATP_LAN3                            "eth0.3"
#define ATP_LAN4                            "eth0.2"
#else
#define ATP_LAN1                            "eth0.2"
#define ATP_LAN2                            "eth0.3"
#define ATP_LAN3                            "eth0.4"
#define ATP_LAN4                            "eth0.5"
#endif

#endif

#define ATP_WLAN1                           "wl0"
#define ATP_WLAN2                           "wl0.1"
#define ATP_WLAN3                           "wl0.2"
#define ATP_WLAN4                           "wl0.3"
#define ATP_WLAN5                           "wl0.4"
#define ATP_WLAN6                           "wl0.5"
#define ATP_WLAN7                           "wl0.6"
#define ATP_WLAN8                           "wl0.7"
#define ETHUP_WAN                           ATP_WANETH_INTERFACE_NAME
#define ATP_DEFAULT_BRIDGE_NAME             "br0"

/*Start of added by f00120964 for openwifi qos function 2012-03-22*/
#define ATP_OPENWIFI_O_BRIDGE_NAME          "br1"
#define ATP_OPENWIFI_X_BRIDGE_NAME          "br2"
/*End of added by f00120964 for openwifi qos function 2012-03-22*/


#define PPPOX_MAX_ID                        (256)

#ifndef ATP_PCLINT

/* 注意，本函数默认传递进来的数字字符串不会超过unsigned int的范围 */
static inline unsigned int str_to_uint(char *pszNum, int base)
{
    unsigned int uiRlt = 0;
    
    if ((!pszNum) || ('\0' == pszNum[0]))
    {
        return 0;
    }
    
    while ('\0' != *pszNum)
    {
        uiRlt = (uiRlt * base) + (*pszNum - '0');
        pszNum++;
    }

    return uiRlt;
}

/* 判断给定接口是否PPPoE代理的LAN接口 */
#define IS_PPPOX_LAN_DEVICE(dev_name) \
(('p' == dev_name[0]) &&('p' == dev_name[1]) && (PPPOX_MAX_ID > str_to_uint(&dev_name[3], 10)))

/* 判断给定的接口是否ppp WAN接口而不是ppp代理接口 */
#define IS_PPP_WAN_DEVICE(dev_name) \
(('p' == dev_name[0]) && (PPPOX_MAX_ID <= str_to_uint(&dev_name[3], 10)))


/* 判断给定的接口是否IPoE WAN接口 */
#define IS_IP_WAN_DEVICE(dev_name) \
('n' == dev_name[0])

/* <DTS2012112905392 w00211169 2012-12-10 begin */
/* 判断给定的接口是否UMTS接口 */
#define IS_UMTS_WAN_DEVICE(dev_name) \
(('r' == dev_name[0]) && ('m' == dev_name[1]))

/* 判断给定的接口是否WAN接口 */
#define IS_WAN_DEVICE(dev_name) \
(IS_IP_WAN_DEVICE(dev_name) || (IS_PPP_WAN_DEVICE(dev_name)) || (IS_UMTS_WAN_DEVICE(dev_name)))
/* DTS2012112905392 w00211169 2012-12-10 end> */


#define IS_LAN_ETH_DEVICE(dev_name) \
    (('e' == dev_name[0]) && ('t' == dev_name[1]) && ('h' == dev_name[2]) \
    && ('0' == dev_name[3]))

#define IS_LAN_WLAN_DEVICE(dev_name) \
(('w' == dev_name[0]))

#define IS_LAN_DEV(dev_name) \
(IS_LAN_ETH_DEVICE(dev_name) || IS_LAN_WLAN_DEVICE(dev_name))

#define IS_BRIDGE_DEV(dev_name) \
    ('b' == dev_name[0])


#if 0
#define IS_WAN_DEVICE(dev_name)\
(('b' != dev_name[0]) && ('w' != dev_name[0])&& ('e' != dev_name[0]) && (!IS_PPPOX_LAN_DEVICE(dev_name)))
#endif

#endif


#endif /* _ATP_INTERFACE_H_ */

