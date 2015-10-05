/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/*\file wlancli.h
  *\copyright 2007-2020，华为技术有限公司
  *\author z001758982
  *\date  2012-02-17
*/
#ifndef __WLANCLI_H__
#define __WLANCLI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atpconfig.h"
#include "atptypes.h"


/* WLAN接口名定义是因BRCM驱动而固定死(请勿改)*/
#if defined(SUPPORT_ATP_WLAN_INF_REVERSE)
#define ATP_WL0                     (WLAN_INFNAME_HEAD"1") /*当2.4G或者5G或者2.4G/5G时2.4G的第一个SSID接口名*/
#define ATP_WL1                     (WLAN_INFNAME_HEAD"0") /*当2.4G&5G(同时打开)时5G的第一个SSID接口名*/
#else
#define ATP_WL0                     (WLAN_INFNAME_HEAD"0") /*当2.4G或者5G或者2.4G/5G时2.4G的第一个SSID接口名*/
#define ATP_WL1                     (WLAN_INFNAME_HEAD"1") /*当2.4G&5G(同时打开)时5G的第一个SSID接口名*/
#endif
#define WLAN_RES_FILE               "/var/wlan_res_lst"

#define WLAN_WEPKEY_LEN_ASCII_64        5   /*64位密钥，密钥为字符串的长度*/
#define WLAN_WEPKEY_LEN_HEX_64          10  /*64位密钥，密钥为十六进制的长度*/
#define WLAN_WEPKEY_LEN_ASCII_128       13  /*128位密钥，密钥为字符串的长度*/    
#define WLAN_WEPKEY_LEN_HEX_128         26  /*128位密钥，密钥为十六进制的长度*/

/*define for quantenna */
#define QTM_WLAN_INTERFACE          "wifi"
#define QTM_CMD_RESILT_FILE         "/var/run/qtm.log"
#define QTM_IP_FILE                 "/var/run/qtm.ip"
#define QTM_CMD_OK                  "complete"
#define QTM_CMD_ERR                 "QCS API error"

typedef enum 
{
    WLAN_CLI_SET_THROUGHPUT = 0,
    WLAN_CLI_SET_WIRELESS,
    WLAN_CLI_SET_SSID,
    WLAN_CLI_SET_WPAKEY,
    WLAN_CLI_SET_WEPKEY,
    WLAN_CLI_SET_APPINCODE,
    WLAN_CLI_SET_ANT_TX,

    WLAN_CLI_DISP_WIRELESS,
    WLAN_CLI_DISP_SSID,
    WLAN_CLI_DISP_WPAKEY,
    WLAN_CLI_DISP_WEPKEY,
    WLAN_CLI_DISP_APPINCODE,
    WLAN_CLI_DISP_ANT_TX
}WLAN_CLI_EVENT_EN;

typedef struct
{
    WLAN_CLI_EVENT_EN enWlanCliEvt;
    VOS_UINT32        ulIndex1;
    VOS_UINT32        ulIndex2;
    VOS_CHAR         *pcValue1Set;
    VOS_CHAR         *pcValue2Set;
    VOS_CHAR          acValueGet[256];
}WLAN_CLI_INTIME_ST;

VOS_UINT32 WlanCliGetKeylistInFile(VOS_CHAR *pFile, VOS_CHAR *pcKey, VOS_UINT32* pulInstId, VOS_UINT32 ulDepth);

VOS_UINT32 RB_DiagSetWireless(VOS_UINT32 ulEnable, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagSetVssSsid(VOS_CHAR *pcSsid, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagSetVssWpakey(VOS_CHAR *pcWpakey, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagSetVssWepkey(VOS_CHAR *pcWepKey, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagSetVssAppin(VOS_CHAR *pcAppincode, VOS_UINT32 ulBand, VOS_UINT32 ulindex);

VOS_UINT32 RB_DiagDisplayWireless(VOS_UINT32 *pulEnable, VOS_UINT32 len, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagDisplayVssSsid(VOS_CHAR *pSsid, VOS_UINT32 len, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagDisplayVssWpakey(VOS_CHAR *pWpakey, VOS_UINT32 len, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagDisplayVssWepkey(VOS_CHAR *pcWepKey, VOS_UINT32 len, VOS_UINT32 ulBand, VOS_UINT32 ulindex);
VOS_UINT32 RB_DiagDisplayVssAppin(VOS_CHAR *pcPin, VOS_UINT32 len, VOS_UINT32 ulBand, VOS_UINT32 ulindex);

#if defined(SUPPORT_ATP_WLAN_MAIN_2G) || defined(SUPPORT_ATP_WLAN_COM_2G)
VOS_UINT32 ATP_CLI_WlanInTimeAction(WLAN_CLI_INTIME_ST *pstWlCliInTime);
#endif
#if defined(SUPPORT_ATP_WLAN_MAIN_5G) || defined(SUPPORT_ATP_WLAN_COM_5G)
VOS_UINT32 ATP_CLI_Wlan5GInTimeAction(WLAN_CLI_INTIME_ST *pstWlCliInTime);
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif

