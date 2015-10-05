/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名      : bhalapicli.h
  版 本 号      : 初稿
  作    者          : y00183935
  生成日期   : 2011-7-27
  最近修改   :
  功能描述   : 装备测试命令集，本文源自z00141363创建的equipsdk.h和x00104602创建的equipRegister.h
  函数列表   :
  修改历史   :
  1.日    期       : 2011-7-27
    作    者        : y00183935
    修改内容 : 创建文件


******************************************************************************/
/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z81004143    2013/05/03    DTS2013050301791    [TR069] B890-66 V100R001C994B122(ROUTER): 新增TR069功能
z00159386    2013/07/06    DTS2013070601078    70V voip特性合入B890主干
c00159151    2014/01/23    DTS2014010701340    【B882】V100R001C00B100(ROUTER)【烧片】B882烧片版本需要修改产品名以及合入之前产品遗留的装备问题 
*******************************************************************************/

#ifndef __BHALAPICLI_H__
#define  __BHALAPICLI_H__

/*****************************************************************************/
/*  本文件需要包含的其他头文件依赖                                           */
/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "atpconfig.h"
#include "atptypes.h"
#include "atputil.h"
#include "cliapi.h"

/*****************************************************************************/
/*  宏常量定义                                                               */
/*****************************************************************************/

#define MIN_TR064_PWD_LEN       1
#define MAX_TR064_PWD_LEN       64
#define SYS_CMD_LEN             256
#define EQUIP_MSG_FILE_SSID       "/var/equicmpssid"
#define EQUIP_MSG_FILE_WLPWD       "/var/equicmpwlpwd"
#define ATP_EQUIP_OK            0
#define SSIDNUM_MIN_LEN     1
#define SSIDNUM_MAX_LEN    32

#define EQUIP_RANGE_LEN     256 /*处理问题单AU4D00087输入128位单板条码死机问题*/
#define EQUIP_PAMA_LEN      128
#define EQUIP_READ_STRLEN   512
#define EQUIP_HELP_STRLEN   512
#define MIN_SERIAL_NUM_LEN   1
#define MAX_SERIAL_NUM_LEN   BHAL_BOARD_SERIAL_NUMBER_LEN
#define MIN_TR064_PWD_LEN    1
#define MAX_TR064_PWD_LEN    64
#define MIN_PRODUCT_NUM_LEN  1
#define MAX_PRODUCT_NUM_LEN  BHAL_BOARD_NUMBER_LEN
#define MIN_OUI_NUM_LEN     32
#define MAX_OUI_NUM_LEN     64
#define EQUIP_FILE_NAME     "/var/equip.result"
#define USERINFO_ACCESS_TMPFILE  "/var/userinfotmp.txt"
#define ATIMEI_FILE_NAME "/var/voice/atStatus"
#define CMDRESULT_ERROR_STR     "ERROR"
#define IMEI_LEN 15

#define EQUIP_LED_FLASH_INTERVAL_MAX 3000   //ms
#define EQUIP_LED_FLASH_INTERVAL_MIN 100    //ms

/* DTS2013050301791 z81004143 2013/4/15, delete 5 line(s),宏定义挪到atputil.h中 */

/*****************************************************************************/
/*  全局函数声明extern                                                       */
/*****************************************************************************/

extern VOS_UINT32 Cli_EQUIPCmdMainProc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 Cli_DiagLedOnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagLedOffFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 Cli_DiagLedTestModeOnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagLedTestModeOffFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagRedLedSetFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 Cli_DiagWhiteLedSetFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);

VOS_UINT32 ATP_CLI_DisplaySysuptimeFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DisplayAtpModuleVersion(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 Cli_DiagRedLedOffFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
VOS_UINT32 Cli_DiagGreenLedOffFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
VOS_UINT32 Cli_DiagKeytestOnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagGreenLedOnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagChipselfTestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagLedOnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagLedOffFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagRedLedOnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagFlashSizeFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 Cli_DiagSystemTypeFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 Cli_DiagDisplayBootFromFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 Cli_DiagSetBootFromSlavess(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_UINT32 Cli_DiagSetBootFromMainss(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);



VOS_UINT32 ATP_CLI_DiagSetPppoeUserFunc(VOS_UINT32 ulArgc, VOS_CHAR *  acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetManuDateFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetPppoePwdFunc(VOS_UINT32 ulArgc, VOS_CHAR *  acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetUserNameFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetFONKeyFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetSIPPwd1Func(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetCWMPPwdFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetCWMPUserNameFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagSetIdentifier1Func(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayPppoePwdFunc(VOS_UINT32 ulArgc, VOS_CHAR *  acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayPppoeUserFunc(VOS_UINT32 ulArgc, VOS_CHAR *  acLastPara[]);

VOS_UINT32 ATP_CLI_DiagSetOuinumFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetOuiFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagSetBarcodeFunc(VOS_UINT32 ulArgc, VOS_CHAR   *  acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetDaytimeFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetCustomerSn1Func(VOS_UINT32 ulArgc, VOS_CHAR *  acLastPara[]);

VOS_UINT32 ATP_CLI_DiagSetSnFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagSetUserpwdFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
#if defined(SUPPORT_ATP_UPNP_TR064)
VOS_UINT32 Cli_DebugSetResetPwdFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
#endif
/*Added by lvxin00135113@20100115 for support get/set telnet state*/
VOS_UINT32 ATP_CLI_DebugSetKernelDbgFlagsFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
/*Added by lvxin00135113@20100115 for support get/set telnet state*/

#ifdef BRCM_DAYING_GASP
VOS_UINT32 Cli_DebugSetDyingGaspOnFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
VOS_UINT32 Cli_DebugSetDyingGaspOffFunc(VOS_UINT32 ulArgc, VOS_CHAR * acLastPara[]);
#endif

VOS_UINT32 ATP_CLI_DiagDisplayLanConfig(VOS_UINT32 ulArgc,VOS_CHAR   *acLastPara[]);/* DTS2014010701340 c00159151 20131231 */
VOS_UINT32 ATP_CLI_DiagDisplayActiveEthPortsFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayCfgFileVerFunc(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagSetMacaddrFunc(VOS_UINT32 ulArgc, VOS_CHAR   * acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayUserNameFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagDisplayOuinumFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayBarcodeFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
/* <DTS2013070601078  z00159386  20130706  begin */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
VOS_UINT32 ATP_CLI_DiagDisplayMobileSNFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif /* CONFIG_B880_PRODUCT */
/* DTS2013070601078  z00159386  20130706  end> */
VOS_UINT32 ATP_CLI_DiagDisplayOuiFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagDisplayImeiFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayDaytimeFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplaySnFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayCustomerSn1Func(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayIdentifier1Func(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayManuDateFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayCWMPUserNameFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayCWMPPwdFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagDisplaySIPPwd1Func(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_UINT32 ATP_CLI_DiagDisplayFONKeyFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

#if 0
#ifdef SUPPORT_ATP_XDSL
VOS_UINT32 Cli_DiagDisplayDslinfoFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagDslDownFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
#endif
#endif
VOS_UINT32 ATP_CLI_DiagDisplayUserpwdFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayVersionFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 ATP_CLI_DiagDisplayMacaddrFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagRestoreDefaultFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

/* <DTS2012071400362 z00189398 2012-07-14 begin */
/* <DTS2013050301791 z81004143 2013/4/15 begin */
VOS_UINT32 Cli_DiagGetResPDMVerFunc(VOS_UINT32 ulArgc,VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagGetAppPDMVerFunc(VOS_UINT32 ulArgc,VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagGetResVerFunc(VOS_UINT32 ulArgc,VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagGetHTMLPDMVerFunc(VOS_UINT32 ulArgc,VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagGetHTMLVerFunc(VOS_UINT32 ulArgc,VOS_CHAR   *acLastPara[]);
/* DTS2013050301791 z81004143 2013/4/15 end> */
/* DTS2012071400362 z00189398 2012-07-14 end> */


VOS_INT32 EquipcmdSwVersion(VOS_VOID);
#if 0
#ifdef SUPPORT_ATP_XDSL
VOS_INT32 EquipcmdXdslInfo(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdChiptestXdsl(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
#endif
VOS_INT32 EquipcmdChiptestLanswitch(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdUsbTest(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#if 0
#ifdef SUPPORT_ATP_WLAN    
VOS_INT32 EquipcmdWlanledOn(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdWlanledOff(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
#endif
#ifdef SUPPORT_ATP_WPS
VOS_INT32 EquipcmdWpsledOn(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdWpsledOff(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
VOS_INT32 EquipcmdSetBoardnum(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_INT32 EquipcmdSetmacAddr(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_INT32 EquipcmdSetProductnum(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_UINT32 EquipcmdCustomerSn1Set(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_INT32 EquipcmdSetOui(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);

VOS_INT32 EquipcmdSetIdentifier1(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_INT32 EquipcmdSetManuDate(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);

VOS_INT32 EquipcmdSetOuinum(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_INT32 EquipcmdUserpwdSet(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_INT32 EquipcmdUserNameSet(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
#if 0
#ifdef SUPPORT_ATP_WLAN
VOS_INT32 EquipcmdSsidSet(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdWlpwdSet(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSsidDisplay(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdWlpwdDisplay(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
#endif
VOS_INT32 EquipcmdSetAssemblyDisable(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSetAssemblyEnable(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#ifdef SUPPORT_ATP_UPNP_TR064
VOS_INT32 EquipcmdTr064pwdDisplay(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSetTr064pwd(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
VOS_INT32 EquipcmdLoadElabel(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdBackupElabel(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdRestoreDef(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#ifdef SUPPORT_ATP_MIDDLE_WARE
VOS_INT32 EquipcmdEraseMiddle(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
/*Added by p00134244 for mupg */
#ifdef SUPPORT_ATP_MUPG
VOS_INT32 EquipcmdEraseMupg(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
#ifdef SUPPORT_ATP_DOUBLE_SYSTEM  
VOS_INT32 DispalyBootFromInfo(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 SetBootFromInfo(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
VOS_INT32 EquipcmdImeiDisplay(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSdcardTest(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSetBarcode(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdReboot(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdLedTest(VOS_CHAR *acPara);
VOS_INT32 EquipcmdLedMapTest(VOS_UINT32 *enLedName,VOS_CHAR *acPara, VOS_UINT32 ulLedLen);
VOS_INT32 EquipcmdRedLedTest(VOS_CHAR *acPara);
VOS_INT32 EquipcmdGreenLedTest( VOS_CHAR *acPara,VOS_CHAR *acPrint);

VOS_INT32 EquipcmdKeyTest(VOS_VOID);
#if 0
#ifdef SUPPORT_ATP_WLAN 
VOS_INT32 EquipcmdThroughoutEnable(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSetTempSsid(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
#endif
VOS_INT32 EquipcmdSetDaytime(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
#if 0
#ifdef SUPPORT_ATP_WLAN 
VOS_INT32 EquipcmdSetSsidn(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdDisplaySsidn(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSetSsidnWpkeyn(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdDisplaySsidnWpkeyn(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
#endif
#endif
VOS_INT32 EquipcmdChiptestCpu(VOS_UINT32 ulArgc, VOS_CHAR *acLastPara[]);
VOS_INT32 EquipcmdSystem(VOS_CHAR *acArgs);

#if 0
#ifdef SUPPORT_ATP_USBMOUNT
VOS_UINT32 Cli_DiagSdcard1TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagSdcard2TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb1TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb2TestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb2SsidFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);
VOS_UINT32 Cli_DiagUsb1speedTestFunc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

#endif
#endif

VOS_UINT32 EquipcmdPppoeUserSet(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);
VOS_UINT32 EquipcmdPppoePwdSet(VOS_UINT32 ulArgc, VOS_CHAR * const acLastPara[]);


VOS_UINT32 EquipcmdOuiDisplay(VOS_VOID);

VOS_UINT32 EquipcmdCustomerSn1Display(VOS_VOID);
VOS_UINT32 EquipcmdIdentifier1Display(VOS_VOID);
VOS_UINT32 EquipcmdPppoeUserDisplay(VOS_VOID);
VOS_UINT32 EquipcmdPppoePwdDisplay(VOS_VOID);
VOS_INT32 EquipcmdDaytimeDisplay(VOS_VOID);
VOS_INT32 EquipcmdDisplayActiveEthPorts(VOS_VOID);
VOS_INT32 EquipcmdDisplayElabel(VOS_VOID);
VOS_INT32 EquipcmdUserpwdDisplay(VOS_VOID);
VOS_INT32 EquipcmdBoardNumDisplay(VOS_VOID);
/* <DTS2013070601078  z00159386  20130706  begin */
/* <DTS2013092310511 z00182709 2013/09/24 begin */
#if defined(CONFIG_B880_PRODUCT) || defined( CONFIG_B881_PRODUCT )
/* DTS2013092310511 z00182709 2013/09/24 end> */
VOS_INT32 EquipcmdMobileSNDisplay(VOS_VOID);
#endif /* CONFIG_B880_PRODUCT */
/* DTS2013070601078  z00159386  20130706  end> */
VOS_INT32 EquipcmdMacaddrDisplay(VOS_VOID);
VOS_INT32 EquipcmdProductnumDisplay(VOS_VOID);
VOS_INT32 EquipcmdOuinumDisplay(VOS_VOID);
VOS_INT32 EquipcmdDisplayMakeDate(VOS_VOID);
VOS_INT32 EquipcmdUserNameDisplay(VOS_VOID);
VOS_INT32 EquipcmdDisplayFonKey(VOS_VOID);

VOS_INT32 EquipcmdDisplaySIPPwd1(VOS_VOID);

VOS_INT32 EquipcmdDisplayCWMPPwd(VOS_VOID);

VOS_INT32 EquipcmdDisplayCWMPUserName(VOS_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif

