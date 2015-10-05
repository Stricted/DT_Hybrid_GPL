/*
* <:copyright-BRCM:2011:proprietary:standard
* 
*    Copyright (c) 2011 Broadcom Corporation
*    All Rights Reserved
* 
*  This program is the proprietary software of Broadcom Corporation and/or its
*  licensors, and may only be used, duplicated, modified or distributed pursuant
*  to the terms and conditions of a separate, written license agreement executed
*  between you and Broadcom (an "Authorized License").  Except as set forth in
*  an Authorized License, Broadcom grants no license (express or implied), right
*  to use, or waiver of any kind with respect to the Software, and Broadcom
*  expressly reserves all rights in and to the Software and all intellectual
*  property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
*  NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
*  BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
* 
*  Except as expressly set forth in the Authorized License,
* 
*  1. This program, including its structure, sequence and organization,
*     constitutes the valuable trade secrets of Broadcom, and you shall use
*     all reasonable efforts to protect the confidentiality thereof, and to
*     use this information only in connection with your use of Broadcom
*     integrated circuit products.
* 
*  2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*     AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*     WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*     RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
*     ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
*     FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
*     COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
*     TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
*     PERFORMANCE OF THE SOFTWARE.
* 
*  3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
*     ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*     INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
*     WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*     IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
*     OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*     SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
*     SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
*     LIMITED REMEDY.
* :>
 
*/


#ifndef _ETHSWCTL_H_
#define _ETHSWCTL_H_

typedef unsigned char u8;
typedef unsigned int u32;

#define ETHSWCTL_VERSION "0.0.2"
#define ETHSWCTL_RELDATE "January 27, 2009"
#define ETHSWCTL_NAME    "ethswctl"
static char *version =
ETHSWCTL_NAME ".c:v" ETHSWCTL_VERSION " (" ETHSWCTL_RELDATE ")\n"
"o Pratapa Reddy Vaka (pvaka@broadcom.com). \n";

static const char *usage_msg =
"Usage: \n"
"       ethswctl {-c|--command} hw-switching\n"
"\n"
"       ethswctl {-c|--command} hw-switching {-o|--operation} <enable|disable>\n"
"\n"
"       ethswctl {-c|--command} mibdump {-p|--port} <port> [{-s}]\n"
"\n"
"       ethswctl {-c|--command} mibdump {-p|--port} <port> {-a|--all} [{-s}]\n"
"\n"
"       ethswctl {-c|--command} pagedump {-P|--Page} <page> [{-s}]\n"
"\n"
"       ethswctl {-c|--command} iudmadump \n"
"\n"
"       ethswctl {-c|--command} iudmasplit -p <port> {-q <iudma>}\n"
"\n"
"       ethswctl {-c|--command} swctrl {-t|--type} <type> [{-v|--value} <val>] \n"
"         type = 0(BUFF_CTRL); val = 0x1(TXQ_PAUSE_EN), 0x2(TXQ_DROP_EN), \n"
"         0x4(TOT_PAUSE_EN), 0x8(TOT_DROP_EN)\n"
"         type = 1(8021Q_CTRL); val = 0x1(ENABLE), 0(DISABLE), \n"
"\n"
"       ethswctl {-c|--command} swprioctrl {-q|--priority} <prio> \n"
"                {-t|--type} <type> {-v|--value} [val]\n"
"         type = 0(TOT_DROP), 1(TOT_PAUSE), 2(TOT_HYST), 3(TXQ_HI_DROP),\n"
"         TXQ_HI_PAUSE(4), TXQ_HI_HYST(5), TXQ_LO_DROP\n"
"\n"
"       ethswctl {-c|--command} rxscheduling {-v|--value} [val]\n"
"         val = 0 (SP), 1(WRR) \n"
"\n"
"       ethswctl {-c|--command} wrrparams {-t|--type} <type> {-v|--value} [val]\n"
"         type = 0 (interval_in_pkts), 1 (weight1), 2 (weight2), 3 (weight3), 4 (weight4)\n"
"         val: interval_in_pkts: <1-1280> ;weight: <1-16> \n"
"\n"
"       ethswctl {-c|--command} vlan {-v|--value} vlan_id {-f|--fwdmap} \n"
"                fwd_map {-u|-untagmap} untag_map\n"
"\n"
"       ethswctl {-c|--command} replacetagval {-p|--port} <port> \n"
"                {-t|--type} <type> [{-v|--value} <val>] \n"
"         type = 0(TAG), 1(TPID), 2(TCI), 3(VID), 4(8021P), 5(CFI), 6(MATCH_VID) \n"
"\n"
"       ethswctl {-c|--command} replacetagop {-p|--port} <port> \n"
"                {-t|--type} <type> [{-v|--value} <val>] \n"
"         type = EN_REPLACE_8021P(0), EN_REPLACE_CFI(1), EN_REPLACE_VID(2), \n"
"         EN_REPLACE_TPID(3), EN_TAG_STRIP (4) \n"
"         val = 1 (Enable), 0 (Disable) \n"
"\n"
"       ethswctl {-c|--command} pbvlanmap {-p|--port} <port> [{-v|--value} <val>] \n"
"         val = port_map; Bit8: MIPS, Bit7: GPON; Bit5: MoCA: Bits3-0: UNI \n"
"\n"
"       ethswctl {-c|--command} getstat -p port -t type \n"
"\n"
"       ethswctl {-c|--command} getstat32 -p port -t type \n"
"\n"
"       ethswctl {-c|--command} pause -p port -v val \n"
"\n"
"       ethswctl {-c|--command} cosq -v val \n"
"\n"
"       ethswctl {-c|--command} cosq -p port -q priority -v val \n"
"\n"
"       ethswctl {-c|--command} cosq -q priority -v val \n"
"\n"
"       ethswctl {-c|--command} costxq -v channel -q queue \n"
"\n"
"       ethswctl {-c|--command} costxq -t method \n"
"         method = 0(USE_BD_PRIO), 1(USE_TX_IUDMA), \n"
"\n"
"       ethswctl {-c|--command} byteratecfg -v channel -x 0(disable)|1(enable) \n"
"\n"
"       ethswctl {-c|--command} byterate -v channel -x rate \n"
"\n"
"       ethswctl {-c|--command} pktratecfg -v channel -x 0(disable)|1(enable) \n"
"\n"
"       ethswctl {-c|--command} pktrate -v channel -x rate \n"
"\n"
"       ethswctl {-c|--command} deftxq -i if_name -v val \n"
"\n"
"       ethswctl {-c|--command} deftxqen -i if_name -o enable|disable \n"
"\n"
"       ethswctl {-c|--command} getrxcntrs \n"
"\n"
"       ethswctl {-c|--command} resetrxcntrs \n"
"\n"
"       ethswctl {-c|--command} pad [{-l|--length} <len> \n"
"                {-v|--value} <0(disable)|1(enable)>]\n"
"\n"
"       ethswctl {-c|--command} arl -m mac -v vid [-x data] \n"
"\n"
"       ethswctl {-c|--command} arldump \n"
"\n"
"       ethswctl {-c|--command} arlflush \n"
"\n"
"       ethswctl -c untaggedprio -p port [-v val] \n"
"\n"
"       ethswctl -c cosqpriomethod [-v val]\n"
"         val = 0(PORT_QOS), 1(MAC_QOS), 2(8021P_QOS), 3(DIFFSERV_QOS) \n"
"\n"
"       ethswctl -c cosqsched -v [val] -t sp_endq -x weight1 \n"
"        -y weight2 -z weight3 -w weight4 \n"
"         val = 0 (SP), 1(WRR), 2(COMBO) \n"
"         sp_endq: queue where SP scheduling ends. Valid only for COMBO \n"
"\n"
"       ethswctl {-c|--command} portctrl {-p|--port} <port> [-v val] \n"
"         val bit-0:  1 (Rx Disable), 0 (Rx Enable) \n"
"         val bit-1:  1 (Tx Disable), 0 (Tx Enable) \n"
"\n"
"       ethswctl {-c|--command} portloopback {-p|--port} <port> [-v val] \n"
"         val:  1 (Enable), 0 (Disable) \n"
"\n"
"       ethswctl {-c|--command} phymode {-p|--port} <port> [-y speed] [-z duplex]\n"
"         speed:  0 (auto), 1000, 100, 10 \n"
"         duplex: 0 (halfduplex), 1 (fullduplex)\n"
"\n"
"       ethswctl {-c|--command} test -t type [-x param] [-v val] [-s]\n"
"         type = 0(ISR config); param=channel; val=1|0 \n"
"         type = 1(Enable Rx Dump); val=1|0 \n"
"         type = 2(Reset MIB) \n"
"         type = 3(Reset Switch); -s for SPI interface \n"
"\n"
"       ethswctl {-c|--command} jumbo {-p|--port} <port> \n"  // bill
"                [{-v|--value} <0(disable)|1(enable)>]\n"
"         port = 9(ALL), 8(MIPS), 7(GPON), 6(USB), 5(MOCA),\n"
"                4(GPON_SERDES), 3(GMII_2), 2(GMII_1), 1(GPHY_1), 0(GPHY_0)\n"
"\n"
"       ethswctl {-c|--command} dscp2prio -v dscpVal [-q prio] > \n"
"                dscpVal is the DSCP value\n"
"                prio is the switch priority\n"
"         channel = dma channel number. needed when type = 0(ISRCFG)\n"
"\n"
"       ethswctl {-c|--command} regaccess -v offset -l len [-d data]\n"
"\n"
"       ethswctl {-c|--command} spiaccess -x addr -l len [-d data]\n"
"\n"
"       ethswctl {-c|--command} pmdioaccess -x addr -l len [-d data]\n"
"\n"
"       ethswctl {-c|--command} switchinfo -v switch_number\n"
"\n"
"       ethswctl {-c|--command} setlinkstatus -n unit -p port -x linkstatus -y speed -z duplex\n"
"\n"
"       ethswctl {-c|--command} mocasoftswitch -x <queueDepth> \n"
"                {-c|--command} mocasoftswitch -w <port map> [-o] <enable|disable>\n"
"\n"
"       ethswctl {-c|--command} softswitchstartq [-q queue]\n"
"\n"
"       ethswctl {-c|--command} mocaxmitbudget [-v val]\n"
"\n"
"       ethswctl {-c|--command} mocaportcheck {-w wan/lan} <1/0>\n" 
"                [-o] <enable/disable> [-x] <period_ns> [-y] <threshold>\n"
"\n"
"       ethswctl {-c|--command} rxratectrl -p port [-x limit-kbps -y burst-kbps]\n"
"\n"
"       ethswctl {-c|--command} txratectrl -p port [-x limit-kbps -y burst-kbps]\n";

#define MAX_NUM_COMMANDS  100

typedef enum msgs_e {
  HWSWITCHING_MSG = 0,
  MIBDUMP_MSG,
  PAGEDUMP_MSG,
  IUDMADUMP_MSG,
  IUDMASPLIT_MSG,
  SWCTRL_MSG,
  SWPRIOCTRL_MSG,
  RXSCHED_MSG,
  WRRPARAMS_MSG,
  VLAN_MSG,
  REPLACETAGVAL_MSG,
  REPLACETAGOP_MSG,
  PBVLANMAP_MSG,
  GETSTAT_MSG,
  GETSTAT32_MSG,
  PAUSE_MSG,
  COSQ_MSG,
  COSTXQ_MSG,
  BYTERATECFG_MSG,
  BYTERATE_MSG,
  PKTRATECFG_MSG,
  PKTRATE_MSG,
  DEFTXQ_MSG,
  DEFTXQEN_MSG,
  GETRXCNTRS_MSG,
  RESETRXCNTRS_MSG,
  ARL_MSG,
  ARLDUMP_MSG,
  ARLFLUSH_MSG,
  UNTAGGEDPRIO_MSG,
  COSQPRIOMETHOD_MSG,
  COSQSCHED_MSG,
  PORTCTRL_MSG,
  PORTLOOPBACK_MSG,
  PHYMODE_MSG,
  TEST_MSG,
  JUMBO_MSG,
  DSCP2PRIO_MSG,
  REGACCESS_MSG,
  SPIACCESS_MSG,
  PSEUDOMDIOACCESS_MSG,
  SWITCHINFO_MSG,
  SETLINKSTATUS_MSG,
  MOCASOFTSWITCH_MSG,
  SOFTSWITCHSTARTQ_MSG,
  MOCAXMITBUDGET_MSG,
  MOCAPORTCHECK_MSG,
  PORTRXRATECTRL_MSG,
  PORTTXRATECTRL_MSG,
}msgs_t;

static const char *usage_msgs[MAX_NUM_COMMANDS] =  {
"Syntax: ethswctl {-c|--command} hw-switching {-o|--operation} <enable|disable>\n",
"Syntax: ethswctl {-c|--command} mibdump {-p|--port} <port> {-a|--all}\n",
"Syntax: ethswctl {-c|--command} pagedump {-P|--Page} <page>\n",
"Syntax: ethswctl {-c|--command} iudmadump \n",
"Syntax: ethswctl {-c|--command} iudmasplit -p <port> {-q <iudma>}\n",
"Syntax: ethswctl {-c|--command} swctrl {-t|--type} <type> [{-v|--value} <val>] \n"
"          type = 0(BUFF_CTRL); val = 0x1(TXQ_PAUSE_EN), 0x2(TXQ_DROP_EN), \n"
"           0x4(TOT_PAUSE_EN), 0x8(TOT_DROP_EN)\n"
"          type = 1(8021Q_CTRL); val = 0x1(ENABLE), 0(DISABLE), \n",
"Syntax: ethswctl {-c|--command} swprioctrl {-q|--priority} <prio> \n"
"                 {-t|--type} <type> {-v|--value} [val]\n"
"          type = 0(TOT_DROP), 1(TOT_PAUSE), 2(TOT_HYST), 3(TXQ_HI_DROP),\n"
"           TXQ_HI_PAUSE(4), TXQ_HI_HYST(5), TXQ_LO_DROP\n",
"Syntax: ethswctl {-c|--command} rxscheduling {-v|--value} [val]\n"
"          val = 0 (SP), 1(WRR) \n",
"Syntax: ethswctl {-c|--command} wrrparams {-t|--type} <type> {-v|--value} [val]\n"
"          type = 0 (interval_in_pkts), 1 (weight1), 2 (weight2), 3 (weight3), 4 (weight4)\n"
"          val: interval_in_pkts: <1-1280> ;weight: <1-16> \n",
"Syntax: ethswctl {-c|--command} vlan {-v|--value} vlan_id {-f|--fwdmap} \n"
"                 fwd_map {-u|-untagmap} untag_map\n",
"Syntax: ethswctl {-c|--command} replacetagval {-p|--port} <port> \n"
"                 {-t|--type} <type> [{-v|--value} <val>] \n"
"          type = 0(TAG), 1(TPID), 2(TCI), 3(VID), 4(8021P), 5(CFI), 6(MATCH_VID) \n",
"Syntax: ethswctl {-c|--command} replacetagop {-p|--port} <port> \n"
"                 {-t|--type} <type> [{-v|--value} <val>] \n"
"          type = EN_REPLACE_8021P(0), EN_REPLACE_CFI(1), EN_REPLACE_VID(2), \n"
"           EN_REPLACE_TPID(3), EN_TAG_STRIP (4) \n"
"          val = 1 (Enable), 0 (Disable) \n",
"Syntax: ethswctl {-c|--command} pbvlanmap {-p|--port} <port> [{-v|--value} <val>] \n"
"          val = port_map; Bit8: MIPS, Bit7: GPON; Bit5: MoCA: Bits3-0: UNI \n",
"Syntax: ethswctl {-c|--command} getstat -p port -t type \n",
"Syntax: ethswctl {-c|--command} getstat32 -p port -t type \n",
"Syntax: ethswctl {-c|--command} pause -p port -v val \n",
"Syntax: ethswctl {-c|--command} cosq -v val \n"
"        ethswctl {-c|--command} cosq -p port -q priority -v val \n"
"        ethswctl {-c|--command} cosq -q priority -v val \n",
"Syntax: ethswctl {-c|--command} costxq -v channel -q queue \n"
"        ethswctl {-c|--command} costxq -t method \n"
"          method = 0(USE_BD_PRIO), 1(USE_TX_IUDMA) \n",
"Syntax: ethswctl {-c|--command} byteratecfg -v channel -x 0(disable)|1(enable) \n" ,
"Syntax: ethswctl {-c|--command} byterate -v channel -x rate \n",
"Syntax: ethswctl {-c|--command} pktratecfg -v channel -x 0(disable)|1(enable) \n",
"Syntax: ethswctl {-c|--command} pktrate -v channel -x rate \n",
"Syntax: ethswctl {-c|--command} deftxq -i if_name -v val \n",
"Syntax: ethswctl {-c|--command} deftxqen -i if_name -o enable|disable \n",
"Syntax: ethswctl {-c|--command} getrxcntrs \n",
"Syntax: ethswctl {-c|--command} resetrxcntrs \n",
"Syntax: ethswctl {-c|--command} arl -m mac -v vid [-x data] \n",
"Syntax: ethswctl {-c|--command} arldump \n",
"Syntax: ethswctl {-c|--command} arlflush \n",
"Syntax: ethswctl -c untaggedprio -p port [-v val] \n",
"Syntax: ethswctl -c cosqpriomethod [-v val]\n"
"          val = 0(PORT_QOS), 1(MAC_QOS), 2(8021P_QOS), 3(DIFFSERV_QOS) \n",
"Syntax: ethswctl -c cosqsched -v [val] -t sp_endq -w weight1 -x weight2 "
"                 -y weight3 -z weight4 \n"
"          val = 0 (SP), 1(WRR), 2(COMBO) \n"
"          sp_endq: queue where SP scheduling ends. Valid only for COMBO \n",
"Syntax: ethswctl {-c|--command} portctrl {-p|--port} <port> [-v val] \n"
"         val bit-0:  1 (Rx Disable), 0 (Rx Enable) \n"
"         val bit-1:  1 (Tx Disable), 0 (Tx Enable) \n",
"Syntax: ethswctl {-c|--command} portloopback {-p|--port} <port> [-v val] \n"
"         val:  1 (Enable), 0 (Disable) \n",
"Syntax: ethswctl {-c|--command} phymode {-p|--port} <port> [-y speed] [-z duplex]\n"
"         speed:  0 (auto), 1000, 100, 10 \n"
"         duplex: 0 (halfduplex), 1 (fullduplex)\n",
"Syntax: ethswctl {-c|--command} test -t type [-x param] [-v val] [-s]\n"
"         type = 0(ISR config); param=channel; val=1|0 \n"
"         type = 1(Enable Rx Dump); val=1|0 \n"
"         type = 2(Reset MIB) \n"
"         type = 3(Reset Switch); -s for SPI interface  \n",
"Syntax: ethswctl {-c|--command} jumbo {-p|--port} <port> \n"
"          [{-v|--value} <0(disable)|1(enable)>]\n"
"          port = 9(ALL), 8(MIPS), 7(GPON), 6(USB), 5(MOCA),\n"
"                 4(GPON_SERDES), 3(GMII_2), 2(GMII_1), 1(GPHY_1), 0(GPHY_0)\n",
"Syntax: ethswctl -c dscp2prio -v dscpVal [-q prio] > \n"
"          dscpVal is the DSCP value\n"
"          prio is the switch priority\n",
"Syntax: ethswctl {-c|--command} regaccess -v offset -l len [-d data] [-n unit]\n",
"Syntax: ethswctl {-c|--command} spiaccess -x addr -l len [-d data]\n",
"Syntax: ethswctl {-c|--command} pmdioaccess -x addr -l len [-d data]\n",
"Syntax: ethswctl {-c|--command} switchinfo -v switch_number\n",
"Syntax: ethswctl {-c|--command} setlinkstatus -p port -x linkstatus -y speed -z duplex\n"
"          speed = 10 (for 10Mbps), 100(for 100Mbps), and 1000(1000Mbps) \n",
"Syntax: ethswctl {-c|--command} mocasoftswitch -x <queueDepth>\n"
"                                               -w <port map> [-o] <enable|disable>\n"
"          wan/lan   = 0(MOCA LAN), 1(MOCA WAN)\n"
"          operation = 'enable' or 'disable' moca port check feature\n",
"Syntax: ethswctl {-c|--command} softswitchstartq [-q queue_number]\n",
"Syntax: ethswctl {-c|--command} mocaxmitbudget [-v budget_value]\n",
"        ethswctl {-c|--command} mocaportcheck {-w wan/lan} <1/0>\n" 
"                 [-o] <enable/disable> [-x] <period_ns> [-y] <threshold>\n"
"          wan/lan   = 0(MOCA LAN), 1(MOCA WAN)\n"
"          operation = 'enable' or 'disable' moca port check feature\n"
"          period_ns = check interval in nanoseconds\n"
"          threshold = number of consecutive failed checks needed to execute watchdog\n",
"Syntax: ethswctl {-c|--command} rxratectrl -p port [-x limit-kbps -y burst-kbps]\n",
"Syntax: ethswctl {-c|--command} txratectrl -p port [-x limit-kbps -y burst-kbps]\n\n",
};

/* Command-line flags. */
unsigned int
opt_a = 0,    /* Indicates all or subset for mibdump */
opt_c = 0,    /* Command name */
opt_d = 0,    /* data */
opt_o = 0,    /* Enable/Disable operation */
opt_p = 0,    /* Port number */
opt_P = 0,    /* Page number */
opt_v = 0,    /* Value*/
opt_h = 0,    /* Help*/
opt_V = 0,    /* Version flag */
opt_q = 0,    /* Priority */
opt_f = 0,    /* forward_map */
opt_u = 0,    /* untag_map */
opt_x = 0,    /* Generic param1 */
opt_y = 0,    /* Generic param2 */
opt_z = 0,    /* Generic param3 */
opt_w = 0,    /* Generic param4 */
opt_t = 0,    /* Type parameter */
opt_l = 0,    /* Length */
opt_i = 0,    /* Interface name */
opt_m = 0,    /* MAC address */
opt_n = 0,    /* MAC address */
opt_s = 0;    /* indicates access to external spi slave device */

struct option longopts[] = {
    /* { name  has_arg, *flag  val } */
    {"all",       0, 0, 'a'}, /* mibdump parameter */
    {"command",   1, 0, 'c'}, /* Command Name */
    {"operation", 1, 0, 'o'}, /* Enable/Disable Operation for hw-switching */
    {"port",    1, 0, 'p'}, /* Port Number for mibdump */
    {"Page",    1, 0, 'P'}, /* Page Number for pagedump */
    {"version",   0, 0, 'V'}, /* Emit version information.*/
    {"help",      0, 0, 'h'}, /* Emit help message */
    {"value",     1, 0, 'v'}, /* Value parameter*/
    {"priority",  1, 0, 'q'}, /* Priority */
    {"type",      1, 0, 't'}, /* Type */
    {"length",      1, 0, 'l'},	/* Type */
    {"untagmap",  1, 0, 'u'}, /* VLAN untag map */
    {"fwdmap",    1, 0, 'f'}, /* VLAN fwd map */
    {"param1",    1, 0, 'x'}, /* Generic parameter1 */
    {"param2",    1, 0, 'y'}, /* generic parameter2 */
    {"param3",    1, 0, 'z'}, /* generic parameter3 */
    {"param4",    1, 0, 'w'}, /* Generic parameter4 */
    {"interface", 1, 0, 'i'}, /* Interface name */
    {"mac",       1, 0, 'm'}, /* MAC address */
    {"unit",      1, 0, 'n'},	/* unit */        
    {"length",    1, 0, 'l'},	/* Length */
    {0, 0, 0, 0}
};

/* Defines for swctrl value parameter */
#define SWITCH_BUFFER_CONTROL   0

/* Defines for swprioctrl type parameter */
#define TOTAL_DROP_THRESHOLD    0
#define TOTAL_PAUSE_THRESHOLD   1
#define TOTAL_HYST_THRESHOLD    2
#define TXQ_HI_DROP_THRESHOLD   3
#define TXQ_HI_PAUSE_THRESHOLD  4
#define TXQ_HI_HYST_THRESHOLD   5
#define TXQ_LO_DROP_THRESHOLD   6

/* Local functions */
static int ethswctl_control_priority_set(int unit, int priority, int type,
    unsigned int val);
static int ethswctl_control_priority_get(int unit, int priority, int type,
    unsigned int *val);
static int ethswctl_control_get(int unit, int type, unsigned int *val);

static int ethswctl_wrr_param_get(int unit);
static int ethswctl_wrr_param_set(int unit, int type, unsigned int val);

/* Defines for WRR parameter type */
#define WRR_MAX_PKTS_PER_RND    0
#define WRR_CH0_WEIGHT          1
#define WRR_CH1_WEIGHT          2
#define WRR_CH2_WEIGHT          3
#define WRR_CH3_WEIGHT          4

/* Defines for tagreplaceval type parameter */
#define REPLACE_VLAN_MATCH_VID  6

/* Defines for tagreplaceop type parameter */
#define EN_REPLACE_8021P  0
#define EN_REPLACE_CFI    1
#define EN_REPLACE_VID    2
#define EN_REPLACE_TPID   3
#define EN_TAG_STRIP      4

#define ETHSWCTL_DISABLE 0
#define ETHSWCTL_ENABLE  1

// Defines for JUMBO register control
#define ETHSWCTL_JUMBO_PORT_ALL                       9   // bill
#define ETHSWCTL_JUMBO_PORT_MIPS                      8
#define ETHSWCTL_JUMBO_PORT_GPON                      7
#define ETHSWCTL_JUMBO_PORT_USB                       6
#define ETHSWCTL_JUMBO_PORT_MOCA                      5
#define ETHSWCTL_JUMBO_PORT_GPON_SERDES               4
#define ETHSWCTL_JUMBO_PORT_GMII_2                    3
#define ETHSWCTL_JUMBO_PORT_GMII_1                    2
#define ETHSWCTL_JUMBO_PORT_GPHY_1                    1
#define ETHSWCTL_JUMBO_PORT_GPHY_0                    0

#define ETHSWCTL_JUMBO_PORT_MIPS_MASK                 0x0100
#define ETHSWCTL_JUMBO_PORT_GPON_MASK                 0x0080
#define ETHSWCTL_JUMBO_PORT_USB_MASK                  0x0040
#define ETHSWCTL_JUMBO_PORT_MOCA_MASK                 0x0020
#define ETHSWCTL_JUMBO_PORT_GPON_SERDES_MASK          0x0010
#define ETHSWCTL_JUMBO_PORT_GMII_2_MASK               0x0008
#define ETHSWCTL_JUMBO_PORT_GMII_1_MASK               0x0004
#define ETHSWCTL_JUMBO_PORT_GPHY_1_MASK               0x0002
#define ETHSWCTL_JUMBO_PORT_GPHY_0_MASK               0x0001
#define ETHSWCTL_JUMBO_PORT_MASK_VAL                  0x01FF


/* Defines for tagreplaceop val parameter */


static int ethswctl_replace_tag_val_set(int unit, int port, int type,
    unsigned int val);
static int ethswctl_replace_tag_val_get(int unit, int port, int type);
static int ethswctl_replace_tag_op_set(int unit, int port, int type,
    unsigned int val);
static int ethswctl_replace_tag_op_get(int unit, int port, int type);

typedef struct command cmd_t;
typedef int (cmd_func_t)(cmd_t *cmd, char** argv);

struct command
{
    int         nargs;
    const char  *name;
    cmd_func_t  *func;
    const char  *help;
};

cmd_t *command_lookup(const char *cmd);
void command_help(const cmd_t *);
void command_helpall(void);

#endif  /* _ETHSWCTL_H_ */
