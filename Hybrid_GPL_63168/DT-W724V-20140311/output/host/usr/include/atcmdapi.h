#ifndef _ATCMDAPI_H_
#define _ATCMDAPI_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "atptypes.h"
#include "atpconfig.h"
/* <DTS2013083001384 xiazhongyue 20130830 delete */


#define  ATCMD_OK          0
#define  ATCMD_ERROR       -1

#define ATCMD_MODEMDOWN '2'
#define ATCMD_CYCLE            256
#define ATCMD_SIZE              32
#define ATCMD_STATEFILE      "/var/at/umts_state"
#define ATCMD_COUNT           "/var/at/atcount"

/*
 *  struct
 */
 /*Start of added for support STC hspa by lvxin00135113@20090812*/
#define  ATCMD_HSPA_VENDOR_PROC                           "/proc/UsbModem"
#define  ATCMD_HSPA_VENDOR_STR                             "Vendor"
#define  ATCMD_HSPA_PRODUCT_VENDOR_HW             "Huawei"
#define  ATCMD_HSPA_PRODUCT_VENDOR_BANDLUXE   "Bandluxe"
#define  ATCMD_HSPA_PRODUCT_VENDOR_ZTE             "ZTE"

typedef int (cmd_func_t)(char **argv);

typedef enum
{
    ATCMD_HSPA_VENDOR_HW = 0,
    ATCMD_HSPA_VENDOR_BANDLUXE,
    ATCMD_HSPA_VENDOR_ZTE,

    ATCMD_HSPA_VENDOR_MAX
}ATCMD_HSPA_VENDOR_EN;

typedef struct atcmd 
{
    char *name;
    cmd_func_t *func;
    char *help;
}at_cmd_t;


/*End of added for support STC hspa by lvxin00135113@20090812*/

/*
 *  global variables
 */

/* <DTS2013083001384 xiazhongyue 20130830 delete */
extern at_cmd_t at_cmds[];

/****************************************************************
ATCMD Function
可由产品实现的命令
*****************************************************************/
int at_cmdhwversion(char **argv);
int atcmdHelp(char **argv);

extern int at_cmdhwversion(char **argv);
extern int at_cmdcimi(char **argv);
extern int at_cmdboard(char **argv);
extern int at_cmdfactory(char **argv);
extern int at_cmdswver(char **argv);
extern int at_cmdcgdcont(char **argv);
extern int at_cmdsysinfo(char **argv);
extern int at_cmdsyscfg(char **argv);
extern int at_cmdcops(char **argv);
extern int at_cmdcpwd(char **argv);
extern int at_cmdclck(char **argv);
extern int at_cmdbsn(char **argv);
extern int at_cmdcpin(char **argv);
extern int at_cmdnewcpin(char **argv);
extern int at_cmdcmee(char **argv);
extern int at_cmdstsf(char **argv);/* <DTS2013083001384 xiazhongyue 20130830 modify> */
extern int at_cmdcfun(char **argv);
extern int at_cmdcardmode(char **argv);
extern int at_cmdcsq(char **argv);
extern int at_cmdsn(char **argv);
extern int at_cmdhsdpa(char **argv);
extern int at_cmdcpas(char **argv);
extern int at_cmdata(char **argv);
extern int at_cmdatd(char **argv);
extern int at_cmdchup(char **argv);
extern int at_cmdclip(char **argv);

extern int at_cmddtmf(char **argv);

extern int at_cmdddsetex(char **argv);

extern int at_cmdscid(char **argv);

/*Added by lvxin00135113@20100120 for record the IMEI of UMTS stick */  
extern int at_cmdati(char **argv);
/*Added by lvxin00135113@20100120 for record the IMEI of UMTS stick */  

/*Added by lvxin00135113@20110722 to support require SIM card ICCID without pin code */
extern int at_cmdiccid(char **argv);
/*Added by lvxin00135113@20110722 to support require SIM card ICCID without pin code */

/*cs_20080828  chenkeyun          begin*/
extern int at_cmdccfc(char **argv);

extern int at_cmdccwa(char **argv);

extern int at_cmdchld(char **argv);

extern int at_cmdcssn(char **argv);
/*cs_20080828  chenkeyun          end*/

/*Start of added for support STC hspa by lvxin00135113@20090812*/
extern int at_cmdsysmode(char **argv);
extern int at_cmdqcsysmode(char **argv);
extern int at_cmdqcpinstat(char **argv);
extern int at_cmdcgclass(char **argv);
extern int at_cmdcpinc(char **argv);
extern int at_cmdcreg(char **argv);
/*End of added for support STC hspa by lvxin00135113@20090812*/

/*Start of added by z00166803 for support ZTE datacard 2010-07-23*/
extern int at_cmdssnd(char **argv);
extern int at_cmdzpas(char **argv);
extern int at_cmdcgmm(char **argv);
extern int at_cmdpnnm(char **argv);
extern int at_cmdzpinpuk(char **argv);
extern int at_cmdzsnt(char **argv);
extern int at_cmdsyssel(char **argv);
/*End of added by z00166803 for support ZTE datacard 2010-07-23*/
/* <DTS2013083001384 xiazhongyue 20130830 begin */
extern VOS_INT32 at_cmddsflowqry(VOS_CHAR **argv);

extern VOS_INT32 at_cmddsflowclr(VOS_CHAR **argv);

#ifdef SUPPORT_ATP_NDIS
//!added by w00190448 for datacard mode :NDIS 20120927 start
extern int at_cmddialmode(char **argv);
extern int at_cmddhcp(char **argv);

//!added by w00190448 for datacard mode:NDIS 20120927 end
#endif

extern int at_cmdsysinfoex(char **argv);

extern VOS_INT32 at_cmdsyscfgex(VOS_CHAR **argv);

VOS_UINT32 Cli_ATCmdMainProc(VOS_UINT32 ulArgc, VOS_CHAR   *acLastPara[]);

VOS_INT32 at_cmdNdisdial(VOS_CHAR **argv);
extern VOS_INT32 at_cmdResetAtserver(VOS_CHAR **argv);

VOS_INT32 at_cmdDhcp(VOS_CHAR **argv);
extern VOS_INT32 at_cmdRestoreAMSS(VOS_CHAR **argv);
extern VOS_INT32 at_cmdNVBackup(VOS_CHAR **argv);
/*  DTS2013083001384 xiazhongyue 20130830 end > */
#ifdef  __cplusplus
}
#endif

#endif

