/*
<:copyright-gpl 

 Copyright 2003 Broadcom Corp. All Rights Reserved. 
 
 This program is free software; you can distribute it and/or modify it 
 under the terms of the GNU General Public License (Version 2) as 
 published by the Free Software Foundation. 
 
 This program is distributed in the hope it will be useful, but WITHOUT 
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 for more details. 
 
 You should have received a copy of the GNU General Public License along 
 with this program; if not, write to the Free Software Foundation, Inc., 
 59 Temple Place - Suite 330, Boston MA 02111-1307, USA. 

:>
*/
/**************************************************************************
 * File Name  : boardparms.c
 *
 * Description: This file contains the implementation for the BCM63xx board
 *              parameter access functions.
 * 
 * Updates    : 07/14/2003  Created.
 ***************************************************************************/

/* Includes. */
#include <linux/module.h>
#include "boardparms.h"
#include <board.h>
#include "atpconfig.h"

#ifdef BROADCOM_RELEASE_VERSION_4_14_L02
#include "63268_intr.h"
#endif
/* Defines. */

/* Default psi size in K bytes */
#define BP_PSI_DEFAULT_SIZE                     64 

#define SW_INFO_DEFAULT(n) { \
  (0x00000001 << (n)) - 1,      /* port_map */ \
  {0, 0, 0, 0, 0, 0, 0, 0}      /* phy_id */ \
}


enum bp_id {
  bp_boardId,
  bp_usGpioOverlay,
  bp_ulSdramSize,
  
  //enet start:
  bp_ucPhyType0,
  bp_ucPhyType1,
  bp_ucPhyAddress,
  bp_usConfigType,
  bp_ulPortMap,
  bp_ulPhyId0,
  bp_ulPhyId1,
  bp_ulPhyId2,
  bp_ulPhyId3,
  bp_ulPhyId4,
  bp_ulPhyId5,
  bp_ulPhyId6,
  bp_ulPhyId7,
  /*
   bp_usDuplexLed,
   bp_usSpeedLed100,
   bp_usSpeedLed1000,
   */
  //enet end:

  //voip start:
  bp_ucDspType0,
  bp_ucDspType1,
  bp_ucDspAddress,
  bp_usGpioLedVoip,
  bp_usGpioLedVoip1,
  bp_usGpioLedVoip1Fail,
  bp_usGpioLedVoip2,
  bp_usGpioLedVoip2Fail,
  //voip end:
  
  //led start:
  bp_usGpioLedPower,
  bp_usGpioLedPowerFail,
  bp_usEphyBaseAddress,
  bp_ulNonPeriphGpioCtrlMap,
  bp_usGphyBaseAddress,
  bp_usGpioLedAdsl,
  bp_usGpioLedAdslFail,
  bp_usGpioLedSecAdsl,
  bp_usGpioLedSecAdslFail,
  bp_usGpioLed3G,
  bp_usGpioLed3GFail,
  bp_usGpioLedSesWireless,
  bp_usGpioLedWanData,
  bp_usGpioLedWanError,
  bp_usGpioLedBlPowerOn,
  bp_usGpioLedInternet,
  bp_usGpioLedInternetFail,
  bp_usGpioLedWps,
  bp_usGpioLedLan,
  bp_usGpioLedWan,
  bp_usGpioLedUsb,
  bp_usGpioLedBlRedPowerOn,
  bp_usGpioLedBlStop,
  bp_usGpioLedWireless,
  bp_usGpioLedWirelessEnable,
  bp_usGpioLedWirelessFail,
  bp_usGpioLedHpna,
  bp_usGpioLedWanDataFail,
  bp_usGpioLedHspa,
  bp_usGpioLedHspaFail,
  bp_usGpioLedPpp,
  bp_usGpioLedPppFail,
  bp_usGpioLedUsbFail,
  bp_usGpioLedLan1,
  bp_usGpioLedLan2,
  bp_usGpioLedLan3,
  bp_usGpioLedLan4,
  bp_usGpioLedPots,
  bp_usGpioLedDect,
  //led end

  //btn start:
  bp_usGpioPressWps,
  bp_usGpioPressAndHoldReset,
  bp_usGpioBtnWirelessSes,
  bp_usGpioPassDyingGasp,
  //btn end

  //ver start:
  bp_usHardwareCfg,
  bp_usPcbVer_Low,        
  bp_usPcbVer_High,      
  bp_usBoardVer_Low,      
  bp_usBoardVer_Mid,       
  bp_usBoardVer_High,
  //ver end
  
  //other params start:
  bp_usSlicType,
  bp_usDaaType,
  bp_ulAfeId0,
  bp_ulAfeId1,
  bp_ulExtIntrAdslDyingGasp,
  bp_ulExtIntrHpna,
  bp_ulHpnaChipSelect,
  bp_usGpioVcopeReset,
  bp_usGpioVcopeRelay,
  bp_usCMTThead,
  //other params

  //not used
  bp_usGpioFemtoReset,
  bp_usGpioFpgaReset,
  bp_usGpioLedGpon,
  bp_usGpioLedGponFail,
  bp_usGpioLedMoCA,
  bp_usGpioLedMoCAFail,
  bp_usCsHpi,
  bp_usVcopeBoardRev,
  bp_usGpioExtAFEReset,
  bp_usGpioExtAFELDPwr,
  bp_usGpioExtAFELDMode,
  bp_usGpioLaserDis,
  bp_usGpioLaserTxPwrEn,
  bp_usVregSel1P2,
  bp_usExtIntrResetToDefault,
  bp_usExtIntrSesBtnWireless,
  bp_usAntInUseWireless,
  bp_usWirelessFlags,
  bp_usGpioWirelessPowerDown,
  bp_usVcopeResetGpio,
  bp_usVcopeRelayGpio,
  bp_usGpioUart2Sdin,
  bp_usGpioUart2Sdout,
  bp_usGpioIntAFELDMode,
  bp_usGpioAFELDRelay,

  bp_usGpioIntAFELDPwr,
  bp_usGpioExtAFELDData,
  bp_usGpioExtAFELDClk,
  /*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
  bp_ulDeviceOptions,
  /*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
  bp_usGpioDectRest,
#ifdef SUPPORT_ATP_HYBRID
  bp_usGpioLteRest,
#endif
  bp_ucVreg1P8,
  /*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
  bp_usGpioI2cResetPin,
  bp_usGpioI2cScl,        
  bp_usGpioI2cSda,  
  bp_usGpioI2cDeviceId,
  bp_usTouchBoardIntrNum,
  /*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
  //not used end
  bp_last  
};

typedef struct bp_elem {
  enum bp_id id;
  union {
    char * cp;
    unsigned char * ucp;
    unsigned char uc;
    unsigned short us;
    unsigned long ul;
  } u;
} bp_elem_t;


#if defined(_BCM963268_) || defined(CONFIG_BCM963268)

static bp_elem_t g_bcm963268_hg658b[] = {
  {bp_boardId,                 .u.cp = "963268_hg658b"},
  {bp_usGpioOverlay,           .u.us =(BP_OVERLAY_PHY |
                                       BP_OVERLAY_SERIAL_LEDS |
                                       BP_OVERLAY_USB_DEVICE) },                                       
  {bp_ulSdramSize,             .u.ul = BP_MEMORY_16MB_2_CHIP},
  {bp_usGpioPressAndHoldReset, .u.us = BP_GPIO_32_AL},
  {bp_usGpioPressWps,          .u.us = BP_GPIO_20_AL},
  {bp_usGpioLedPower,          .u.us = BP_SERIAL_GPIO_0_AL},  
  {bp_usGpioLedPowerFail,      .u.us = BP_SERIAL_GPIO_1_AL},
  {bp_usGpioLedAdsl,           .u.us = BP_SERIAL_GPIO_3_AL},  
  {bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0},
  {bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_1},
  {bp_usAntInUseWireless,      .u.us = BP_WLAN_ANT_MAIN},
  {bp_usWirelessFlags,         .u.us = 0},
  {bp_usHardwareCfg,           .u.us = 0},
  {bp_usGpioLedWirelessEnable, .u.us = BP_GPIO_37_AL},
  {bp_usGpioLedWireless,       .u.us = BP_GPIO_36_AL},
  {bp_usSlicType,               .u.us = Legerity_89116},
  {bp_usDaaType,                .u.us = Legerity_89x16_daa},
  {bp_usGpioLedLan,             .u.us = BP_SERIAL_GPIO_9_AL},
  {bp_usGpioLedInternet,        .u.us = BP_SERIAL_GPIO_8_AL},
  {bp_usGpioLedInternetFail,    .u.us = BP_SERIAL_GPIO_2_AL},
  {bp_usGpioLedBlPowerOn,       .u.us = BP_GPIO_22_AL},
  {bp_ulExtIntrAdslDyingGasp,   .u.ul = BP_HW_DEFINED},
  {bp_usExtIntrResetToDefault,  .u.us = BP_EXT_INTR_0},
  {bp_usAntInUseWireless,       .u.us = BP_WLAN_ANT_MAIN},
  {bp_usCMTThead,       .u.us = 1},
  
#ifdef CONFIG_ADSL_ANNEX_B
  {bp_ulAfeId0,       .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXBJ | BP_AFE_FE_REV_6302_REV_7_2_30},
  {bp_ulAfeId1,       .u.ul = BP_AFE_DEFAULT},
#else
  {bp_ulAfeId0,       .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV_7_2_30},
  {bp_ulAfeId1,       .u.ul = BP_AFE_DEFAULT},
#endif

  //voip 
  {bp_ucDspType0,       .u.us = BP_VOIP_MIPS},
  {bp_ucDspAddress,       .u.us = 0},
  {bp_ucDspType1,       .u.us = BP_VOIP_NO_DSP},
  
  //enet 1
  {bp_ucPhyType0,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_ucPhyAddress,            .u.uc = 0x0},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ulPortMap,				 .u.ul = 0x1f},
  {bp_ulPhyId0,				 .u.ul = 0x01},
  {bp_ulPhyId1,				 .u.ul = 0x02},
  {bp_ulPhyId2,				 .u.ul = 0x03},
  {bp_ulPhyId3,				 .u.ul = BP_PHY_ID_4},
  {bp_ulPhyId4,				 .u.ul = 0x18| MAC_IFACE_VALID | MAC_IF_RGMII},
  {bp_ulPhyId5,				 .u.ul = 0x00},
  {bp_ulPhyId6,				 .u.ul = 0x00},
  {bp_ulPhyId7,				 .u.ul = 0x00},


  {bp_last}
};


static bp_elem_t g_bcm963268_hg2xx[] = {
  {bp_boardId,                 .u.cp = "963268_hg2xx"},
  {bp_usGpioOverlay,           .u.us =(BP_OVERLAY_PHY |
                                       BP_OVERLAY_SERIAL_LEDS |
                                       BP_OVERLAY_USB_DEVICE) },                                       
  {bp_ulSdramSize,             .u.ul = BP_MEMORY_16MB_2_CHIP},
  {bp_usGpioPressAndHoldReset, .u.us = BP_GPIO_32_AL},
  {bp_usGpioPressWps,          .u.us = BP_GPIO_20_AL},
  {bp_usGpioLedPower,          .u.us = BP_SERIAL_GPIO_0_AL},  
  {bp_usGpioLedPowerFail,      .u.us = BP_SERIAL_GPIO_1_AL},
  {bp_usGpioLedAdsl,           .u.us = BP_SERIAL_GPIO_3_AL},  
  {bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0},
  {bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_1},
  {bp_usAntInUseWireless,      .u.us = BP_WLAN_ANT_MAIN},
  {bp_usWirelessFlags,         .u.us = 0},
  {bp_usHardwareCfg,           .u.us = 0},
  {bp_usGpioLedWirelessEnable, .u.us = BP_GPIO_37_AL},
  {bp_usGpioLedWireless,       .u.us = BP_GPIO_36_AL},
  {bp_usSlicType,               .u.us = Legerity_89116},
  {bp_usDaaType,                .u.us = Legerity_89x16_daa},
  {bp_usGpioLedLan,             .u.us = BP_SERIAL_GPIO_9_AL},
  {bp_usGpioLedInternet,        .u.us = BP_SERIAL_GPIO_8_AL},
  {bp_usGpioLedInternetFail,    .u.us = BP_SERIAL_GPIO_2_AL},
  {bp_usGpioLedBlPowerOn,       .u.us = BP_GPIO_22_AL},
  {bp_ulExtIntrAdslDyingGasp,   .u.ul = BP_HW_DEFINED},
  {bp_usExtIntrResetToDefault,  .u.us = BP_EXT_INTR_0},
  {bp_usAntInUseWireless,       .u.us = BP_WLAN_ANT_MAIN},
  {bp_usCMTThead,       .u.us = 1},
  
#ifdef CONFIG_ADSL_ANNEX_B
  {bp_ulAfeId0,       .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXBJ | BP_AFE_FE_REV_6302_REV_7_2_30},
  {bp_ulAfeId1,       .u.ul = BP_AFE_DEFAULT},
#else
  {bp_ulAfeId0,       .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV_7_2_30},
  {bp_ulAfeId1,       .u.ul = BP_AFE_DEFAULT},
#endif

  //voip 
  {bp_ucDspType0,       .u.us = BP_VOIP_MIPS},
  {bp_ucDspAddress,       .u.us = 0},
  {bp_ucDspType1,       .u.us = BP_VOIP_NO_DSP},
  
  //enet 1
  {bp_ucPhyType0,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_ucPhyAddress,            .u.uc = 0x0},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ulPortMap,               .u.ul = 0x1f},
  {bp_ulPhyId0,                .u.ul = 0x01},
  {bp_ulPhyId1,                .u.ul = 0x02},
  {bp_ulPhyId2,                .u.ul = 0x03},
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_4},
  {bp_ulPhyId4,                .u.ul = 0x18| MAC_IFACE_VALID | MAC_IF_RGMII},
  {bp_ulPhyId5,                .u.ul = 0x00},
  {bp_ulPhyId6,                .u.ul = 0x00},
  {bp_ulPhyId7,                .u.ul = 0x00},

  {bp_last}
};


static bp_elem_t g_bcm963268_hg257[] = {
  {bp_boardId,                 .u.cp = "963268_hg257"},
  {bp_usGpioOverlay,           .u.us =(BP_OVERLAY_PHY |
                                       BP_OVERLAY_SERIAL_LEDS |
                                       BP_OVERLAY_USB_DEVICE) },
                                       
  {bp_ulSdramSize,             .u.ul = BP_MEMORY_16MB_2_CHIP},
  {bp_usGpioLedBlPowerOn,      .u.us = BP_SERIAL_GPIO_0_AL},
  {bp_usGpioLedWps,            .u.us = BP_GPIO_36_AL},
  {bp_usGpioLed3G,             .u.us = BP_SERIAL_GPIO_2_AL},
  {bp_usGpioLed3GFail,         .u.us = BP_SERIAL_GPIO_3_AL},
  {bp_usGpioLedInternet,       .u.us = BP_SERIAL_GPIO_4_AL},
  {bp_usGpioLedInternetFail,   .u.us = BP_GPIO_8_AL},
  {bp_usGpioLedWan,            .u.us = BP_GPIO_34_AL},
  {bp_usGpioLedVoip1,          .u.us = BP_SERIAL_GPIO_6_AL},
  {bp_usGpioLedVoip1Fail,      .u.us = BP_SERIAL_GPIO_7_AL},
  {bp_usGpioLedUsb,            .u.us = BP_SERIAL_GPIO_5_AL},
  
  {bp_usGpioPressAndHoldReset, .u.us = BP_GPIO_32_AL},
  {bp_usGpioPressWps,          .u.us = BP_GPIO_20_AL},
  {bp_usGpioLedPower,          .u.us = BP_SERIAL_GPIO_0_AL},  
  {bp_usGpioLedAdsl,           .u.us = BP_SERIAL_GPIO_3_AL},  
  {bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0},
  {bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_1},
  {bp_usAntInUseWireless,      .u.us = BP_WLAN_ANT_MAIN},
  {bp_usWirelessFlags,         .u.us = 0},
  {bp_usHardwareCfg,           .u.us = 0},
  {bp_usGpioLedWirelessEnable, .u.us = BP_GPIO_37_AL},
  {bp_usGpioLedWireless,       .u.us = BP_GPIO_36_AL},
  {bp_usSlicType,               .u.us = Legerity_89116},
  {bp_usDaaType,                .u.us = Legerity_89x16_daa},
  {bp_usGpioLedLan,             .u.us = BP_SERIAL_GPIO_9_AL},
  {bp_usGpioLedInternet,        .u.us = BP_SERIAL_GPIO_8_AL},
  {bp_usGpioLedInternetFail,    .u.us = BP_SERIAL_GPIO_2_AL},
  {bp_usGpioLedBlPowerOn,       .u.us = BP_GPIO_22_AL},
  {bp_ulExtIntrAdslDyingGasp,   .u.ul = BP_HW_DEFINED},
  {bp_usExtIntrResetToDefault,  .u.us = BP_EXT_INTR_0},
  {bp_usAntInUseWireless,       .u.us = BP_WLAN_ANT_MAIN},
  {bp_usCMTThead,       .u.us = 1},

  //enet 1
  {bp_ucPhyType0,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_ucPhyAddress,            .u.uc = 0x0},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ulPortMap,               .u.ul = 0x58},
  {bp_ulPhyId0,                .u.ul = 0x00},
  {bp_ulPhyId1,                .u.ul = 0x00},
  {bp_ulPhyId2,                .u.ul = 0x00},
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_4},
  {bp_ulPhyId4,                .u.ul = RGMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPhyId5,                .u.ul = 0x00},
  {bp_ulPhyId6,                .u.ul = RGMII_DIRECT | BP_PHY_ID_25},
  {bp_ulPhyId7,                .u.ul = 0x00},

  //enet2
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_ucPhyAddress,            .u.uc = 0x30},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MDIO},
  {bp_ulPortMap,               .u.ul = 0x1e},
#if 1
  {bp_ulPhyId0,                .u.ul = 0x00},
  {bp_ulPhyId1,                .u.ul = 0x01},
  {bp_ulPhyId2,                .u.ul = 0x02},
  {bp_ulPhyId3,                .u.ul = 0x03},
  {bp_ulPhyId4,                .u.ul = 0x04},
  {bp_ulPhyId5,                .u.ul = 0x00},
  {bp_ulPhyId6,                .u.ul = 0x00},
  {bp_ulPhyId7,                .u.ul = 0x00},
#else
  {bp_ulPhyId0,                .u.ul = 0x01},
  {bp_ulPhyId1,                .u.ul = 0x02},
  {bp_ulPhyId2,                .u.ul = 0x03},
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_4},
  {bp_ulPhyId4,                .u.ul = RGMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPhyId5,                .u.ul = 0x11},
  {bp_ulPhyId6,                .u.ul = BP_PHY_ID_25},
  {bp_ulPhyId7,                .u.ul = 0x00},
#endif

#ifdef CONFIG_ADSL_ANNEX_B
    {bp_ulAfeId0,       .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXBJ | BP_AFE_FE_REV_6302_REV_7_2_30},
    {bp_ulAfeId1,       .u.ul = BP_AFE_DEFAULT},
#else
    {bp_ulAfeId0,       .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV_7_2_30},
    {bp_ulAfeId1,       .u.ul = BP_AFE_DEFAULT},
#endif
  
    //voip 
    {bp_ucDspType0,       .u.us = BP_VOIP_MIPS},
    {bp_ucDspAddress,       .u.us = 0},
    {bp_ucDspType1,       .u.us = BP_VOIP_NO_DSP},

  {bp_usHardwareCfg,            .u.us = 0},
  {bp_last}
};


static bp_elem_t g_bcm963268_W724v[] = {
    {bp_boardId,                 .u.cp = "W724v"},
	/*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
    {bp_ulDeviceOptions,         .u.ul = BP_DEVICE_OPTION_ENABLE_GMAC },
	/*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
    {bp_usGpioOverlay,           .u.us =(BP_OVERLAY_PHY |
                                       BP_OVERLAY_SERIAL_LEDS |
                                       BP_OVERLAY_USB_DEVICE) },
                                       
  {bp_ulSdramSize,             .u.ul = BP_MEMORY_16MB_2_CHIP},
  {bp_usGpioLedBlPowerOn,      .u.us = BP_SERIAL_GPIO_0_AL},
  {bp_usGpioPressAndHoldReset, .u.us = BP_GPIO_32_AL},
  {bp_usGpioPressWps,          .u.us = BP_GPIO_20_AL},
  {bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0},
  {bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_1},
  {bp_usAntInUseWireless,      .u.us = BP_WLAN_ANT_MAIN},
  {bp_usWirelessFlags,         .u.us = 0},
  {bp_usHardwareCfg,           .u.us = 0},
  {bp_usSlicType,               .u.us = Legerity_89116},
  {bp_usDaaType,                .u.us = Legerity_89x16_daa},
  {bp_ulExtIntrAdslDyingGasp,   .u.ul = BP_HW_DEFINED},
  {bp_usExtIntrResetToDefault,  .u.us = BP_EXT_INTR_0},
  {bp_usAntInUseWireless,       .u.us = BP_WLAN_ANT_MAIN},
  {bp_usCMTThead,       .u.us = 1},

  //enet 1
  {bp_ucPhyType0,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_ucPhyAddress,            .u.uc = 0x0},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ulPortMap,               .u.ul = 0x18},
  {bp_ulPhyId0,                .u.ul = 0x00},
  {bp_ulPhyId1,                .u.ul = 0x00},
  {bp_ulPhyId2,                .u.ul = 0x00},
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_4},
  {bp_ulPhyId4,                .u.ul = RGMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPhyId5,                .u.ul = 0x00},
  {bp_ulPhyId6,                .u.ul = 0x00},
  {bp_ulPhyId7,                .u.ul = 0x00},

  //enet2
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_ucPhyAddress,            .u.uc = 0x30},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_HS_SPI_SSB_0},
  {bp_ulPortMap,               .u.ul = 0x1e},
  {bp_ulPhyId0,                .u.ul = 0x00},
  {bp_ulPhyId1,                .u.ul = 0x01},
  {bp_ulPhyId2,                .u.ul = 0x02},
  {bp_ulPhyId3,                .u.ul = 0x03},
  {bp_ulPhyId4,                .u.ul = 0x04},
  {bp_ulPhyId5,                .u.ul = 0x00},
  {bp_ulPhyId6,                .u.ul = 0x00},
  {bp_ulPhyId7,                .u.ul = 0x00},

#ifdef CONFIG_ADSL_ANNEX_B
  {bp_ulAfeId0,                .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXBJ | BP_AFE_FE_REV_6302_REV_7_2_30},
  {bp_ulAfeId1,                .u.ul = BP_AFE_DEFAULT},
#else
  {bp_ulAfeId0,                .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV_7_2_30},
  {bp_ulAfeId1,                .u.ul = BP_AFE_DEFAULT},
#endif
  // UART2
#if BROADCOM_RELEASE_VERSION_4_12 //Luther debug for DSL
  {bp_usGpioIntAFELDMode,      .u.us = BP_GPIO_10_AH},
  {bp_usGpioIntAFELDPwr,       .u.us = BP_GPIO_11_AH},
#else
  {bp_usGpioIntAFELDMode,      .u.us = BP_PIN_DSL_CTRL_4},
  {bp_usGpioIntAFELDPwr,       .u.us = BP_PIN_DSL_CTRL_5},
#endif
  {bp_usGpioUart2Sdin,         .u.us = BP_GPIO_12_AH},
  {bp_usGpioUart2Sdout,        .u.us = BP_GPIO_13_AH},    
  
  //voip 
  {bp_ucDspType0,              .u.us = BP_VOIP_MIPS},
  {bp_ucDspAddress,            .u.us = 0},
  {bp_ucDspType1,              .u.us = BP_VOIP_NO_DSP},

  {bp_usHardwareCfg,           .u.us = 0},

  {bp_usGpioDectRest,          .u.us = BP_GPIO_39_AL},  
#ifdef SUPPORT_ATP_HYBRID 
  {bp_usGpioLteRest,          .u.us = BP_GPIO_33_AH},  
#endif

  {bp_last}
};
/* START ADD: l81004871 FOR Adapter HYBRID 2013-06-17*/
static bp_elem_t g_bcm963268_Hybrid[] = {
    {bp_boardId,                 .u.cp = "Hybrid"},
    {bp_ulDeviceOptions,         .u.ul = BP_DEVICE_OPTION_ENABLE_GMAC },
    {bp_usGpioOverlay,           .u.us =(BP_OVERLAY_PHY |
                                         BP_OVERLAY_SERIAL_LEDS |
                                         BP_OVERLAY_USB_DEVICE |
                                         BP_OVERLAY_PCIE_CLKREQ) },
									   
    {bp_ulSdramSize,             .u.ul = BP_MEMORY_16MB_2_CHIP},
    {bp_usGpioLedBlPowerOn,      .u.us = BP_SERIAL_GPIO_0_AL},
    {bp_usGpioPressAndHoldReset, .u.us = BP_GPIO_32_AL},
    {bp_usGpioPressWps,          .u.us = BP_GPIO_20_AL},
    {bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0},
    {bp_usExtIntrSesBtnWireless, .u.us = BP_EXT_INTR_1},
    {bp_usAntInUseWireless,      .u.us = BP_WLAN_ANT_MAIN},
    {bp_usWirelessFlags,         .u.us = 0},
    {bp_usHardwareCfg,           .u.us = 0},
    {bp_usSlicType,              .u.us = Legerity_89116},
    {bp_usDaaType,               .u.us = Legerity_89x16_daa},
    {bp_ulExtIntrAdslDyingGasp,  .u.ul = BP_HW_DEFINED},
    {bp_usExtIntrResetToDefault, .u.us = BP_EXT_INTR_0},
    {bp_usAntInUseWireless,      .u.us = BP_WLAN_ANT_MAIN},
    {bp_usCMTThead,              .u.us = 1},

    /* START ADD: l81004871 修改回BCM63168+SD5115方案 RGMII3与SD5115的RGMII1相连 2013-07-26*/
    //internal switch enet 1
    {bp_ucPhyType0,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
    {bp_ucPhyAddress,            .u.uc = 0x0},
    {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
    {bp_ulPortMap,               .u.ul = 0x50},
    {bp_ulPhyId0,                .u.ul = 0x00},
    {bp_ulPhyId1,                .u.ul = 0x00},
    {bp_ulPhyId2,                .u.ul = 0x00},
    {bp_ulPhyId3,                .u.ul = 0x00},
    {bp_ulPhyId4,                .u.ul = RGMII_DIRECT | EXTSW_CONNECTED},
    {bp_ulPhyId5,                .u.ul = 0x00},
    {bp_ulPhyId6,                .u.ul = RGMII_DIRECT},
    {bp_ulPhyId7,                .u.ul = 0x00},

    //external switch enet2
    {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
    {bp_ucPhyAddress,            .u.uc = 0x30},
    {bp_usConfigType,            .u.us = BP_ENET_CONFIG_HS_SPI_SSB_0},
    {bp_ulPortMap,               .u.ul = 0x1e},
    {bp_ulPhyId0,                .u.ul = 0x00},
    {bp_ulPhyId1,                .u.ul = 0x01 | EXTSW_CONNECTED},
    {bp_ulPhyId2,                .u.ul = 0x02 | EXTSW_CONNECTED},
    {bp_ulPhyId3,                .u.ul = 0x03 | EXTSW_CONNECTED},
    {bp_ulPhyId4,                .u.ul = 0x04 | EXTSW_CONNECTED},
    {bp_ulPhyId5,                .u.ul = 0x00},
    {bp_ulPhyId6,                .u.ul = 0x00},
    {bp_ulPhyId7,                .u.ul = 0x00},
    /* END ADD: l81004871 修改回BCM63168+SD5115方案 RGMII3与SD5115的RGMII1相连 2013-07-26*/
#ifdef CONFIG_ADSL_ANNEX_B
    {bp_ulAfeId0,                .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXBJ | BP_AFE_FE_REV_6302_REV_7_2_30},
    {bp_ulAfeId1,                .u.ul = BP_AFE_DEFAULT},
#else
    {bp_ulAfeId0,                .u.ul = BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV_7_2_30},
    {bp_ulAfeId1,                .u.ul = BP_AFE_DEFAULT},
#endif
    // UART2
#if BROADCOM_RELEASE_VERSION_4_12
    {bp_usGpioIntAFELDMode,      .u.us = BP_GPIO_10_AH},
    {bp_usGpioIntAFELDPwr,       .u.us = BP_GPIO_11_AH},
#else
    {bp_usGpioIntAFELDMode,      .u.us = BP_PIN_DSL_CTRL_4},
    {bp_usGpioIntAFELDPwr,       .u.us = BP_PIN_DSL_CTRL_5},
#endif
    {bp_usGpioUart2Sdin,         .u.us = BP_GPIO_12_AH},
    {bp_usGpioUart2Sdout,        .u.us = BP_GPIO_13_AH},    
  
    //voip 
    {bp_ucDspType0,              .u.us = BP_VOIP_MIPS},
    {bp_ucDspAddress,            .u.us = 0},
    {bp_ucDspType1,              .u.us = BP_VOIP_NO_DSP},

    {bp_usHardwareCfg,           .u.us = 0},
    {bp_usGpioDectRest,          .u.us = BP_GPIO_39_AL},  
    {bp_usGpioLteRest,           .u.us = BP_GPIO_33_AH},  
    {bp_usGpioI2cResetPin,       .u.us = BP_GPIO_22_AH},   // for gpio i2c V3 board
    {bp_usGpioI2cScl,            .u.us = BP_GPIO_8_AH},
    {bp_usGpioI2cSda,            .u.us = BP_GPIO_9_AH},  
    {bp_usGpioI2cDeviceId,       .u.us = 0x8},
    {bp_usTouchBoardIntrNum,     .u.us = BP_EXT_INTR_3},   // for i2c intr
    {bp_last}
};
/* END ADD: l81004871 FOR Adapter HYBRID 2013-06-17*/

static bp_elem_t * g_BoardParms[] = {g_bcm963268_hg658b,g_bcm963268_hg2xx,g_bcm963268_hg257,g_bcm963268_W724v, g_bcm963268_Hybrid, 0};
#endif
static bp_elem_t * g_pCurrentBp = 0;

#if 0

static BOARD_PARAMETERS g_huawei963268gw_HG658B =
{
    "963268_hg658b",                        /* szBoardId */
  (
    BP_OVERLAY_PHY |
    BP_OVERLAY_SERIAL_LEDS |
    BP_OVERLAY_USB_DEVICE ),                /* usGPIOOverlay */
    BP_MEMORY_16MB_2_CHIP,                  /* SDRAM size and type */
     
    BP_NOT_DEFINED,                         /* usGpioRj11InnerPair */
    BP_NOT_DEFINED,                         /* usGpioRj11OuterPair */
    BP_GPIO_32_AL,                         /* usGpioPressAndHoldReset */
    BP_GPIO_20_AL,                          /* usGpioPressWps */
    BP_NOT_DEFINED,                         /* usGpioUartRts */
    BP_NOT_DEFINED,                         /* usGpioUartCts */    
    
    BP_NOT_DEFINED,                         /* usVcopeResetGpio */
    BP_NOT_DEFINED,                         /* usVcopeRelayGpio */

    BP_SERIAL_GPIO_0_AL,                    /* usGpioLedPower */
    BP_SERIAL_GPIO_1_AL,                    /* usGpioLedPowerFail */ 
    BP_SERIAL_GPIO_3_AL,                    /* usGpioLedAdsl */
    BP_NOT_DEFINED,                         /* usGpioLedAdslFail */
    BP_NOT_DEFINED,                         /* usGpioSecLedAdsl */
    BP_NOT_DEFINED,                         /* usGpioSecLedAdslFail */
    BP_GPIO_37_AL,                          /* usGpioLedWirelessEnable */
    BP_GPIO_36_AL,                          /* usGpioLedWireless */
    BP_NOT_DEFINED,                         /* usGpioLedWirelessFail */
    BP_NOT_DEFINED,                         /* usGpioLedWPS */
    BP_NOT_DEFINED,                         /* usGpioLedWPSFail */
    BP_NOT_DEFINED,                         /* usGpioLedUsb */
    BP_NOT_DEFINED,                         /* usGpioLedUsbFail */
    BP_SERIAL_GPIO_9_AL,                    /* usGpioLedLan */
    BP_NOT_DEFINED,                         /* usGpioLedLan1 */
    BP_NOT_DEFINED,                         /* usGpioLedLan2 */
    BP_NOT_DEFINED,                         /* usGpioLedLan3 */
    BP_NOT_DEFINED,                         /* usGpioLedLan4 */
    BP_NOT_DEFINED,                         /* usGpioLedSesWireless */
    BP_NOT_DEFINED,                         /* usGpioLedHpna */
    BP_NOT_DEFINED,                         /* usGpioLedWanData */
    BP_NOT_DEFINED,                         /* usGpioLedWanDataFail */
    BP_NOT_DEFINED,                         /* usGpioLedHspa */
    
    BP_NOT_DEFINED,                         /* usGpioLedHspaFail */
    BP_SERIAL_GPIO_8_AL,                    /* usGpioLedInternet */
    BP_SERIAL_GPIO_2_AL,                    /* usGpioLedInternetFail*/
    
    BP_NOT_DEFINED,                         /* usGpioLedPpp */
    BP_NOT_DEFINED,                         /* usGpioLedPppFail */
    BP_GPIO_22_AL,                          /* usGpioLedBlPowerOn */
    BP_NOT_DEFINED,                         /* usGpioLedBlResetCfg */
    BP_NOT_DEFINED,                         /* usGpioLedBlStop */
    BP_NOT_DEFINED,                         /* usGpioBondingSecondaryReset */

    BP_HW_DEFINED,                          /* usExtIntrAdslDyingGasp */
    BP_EXT_INTR_0,                          /* usExtIntrResetToDefault */
    BP_NOT_DEFINED,                          /* usExtIntrSesBtnWireless */
    BP_NOT_DEFINED,                         /* usExtIntrSesBtnWireless */
    
    BP_NOT_DEFINED,                         /* usExtIntrHpna */

    BP_NOT_DEFINED,                         /* usCsHpna */
    BP_NOT_DEFINED,                         /* usCsHpi */

    BP_WLAN_ANT_MAIN,                       /* usAntInUseWireless */
    BP_NOT_DEFINED,                         /* WLAN flags */
    BP_NOT_DEFINED,                         /* usVcopeBoardRev */
    1,                                      /* usCMTThead */    
    0,                                      /* Hardware config */
    {BP_NOT_DEFINED,                        /* PCB version low bit GPIO */
     BP_NOT_DEFINED},                       /* PCB version high bit GPIO */
    {BP_NOT_DEFINED,                        /* Board version low bit GPIO */
     BP_NOT_DEFINED,                        /* Board version middle bit GPIO */                   
     BP_NOT_DEFINED,                        /* Board version high bit GPIO */       
     BP_NOT_DEFINED},                       /* usReserve */

    {{BP_ENET_EXTERNAL_SWITCH,              /* ucPhyType */
      0x0,                                  /* ucPhyAddress */
      BP_ENET_CONFIG_MMAP,                  /* usConfigType */
      {0x0f, {0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00}}},  /* sw */
      {BP_ENET_NO_PHY}},                    /* sw */

    {{BP_VOIP_MIPS,                         /* ucDspType */
    0,                                      /* ucDspAddress */
    BP_NOT_DEFINED,                         /* usGpioLedVoip */
    BP_SERIAL_GPIO_4_AL,                   /* usGpioVoip1Led */
    BP_SERIAL_GPIO_5_AL,                   /* usGpioVoip1LedFail */
    BP_NOT_DEFINED,                         /* usGpioVoip2Led */
    BP_NOT_DEFINED,                         /* usGpioVoip2LedFail */
    BP_NOT_DEFINED,                         /* usGpioPotsLed */
    BP_NOT_DEFINED},                        /* usGpioDectLed */
     {BP_VOIP_NO_DSP}}, 
	Legerity_89116, 						/* SLIC type */
	Legerity_89x16_daa,                     /* DAA type */
    BP_NOT_DEFINED,                         /* usGpioWirelessPowerDown */
#ifdef CONFIG_ATP_SUPPORT_ANNEX_B
    {BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXB | BP_AFE_FE_REV_6302_REV1,
    BP_AFE_DEFAULT},                        /* ulAfeIds */
#else    
    {BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV_7_2_30,
      BP_AFE_DEFAULT},                      /* ulAfeIds */
#endif
     BP_NOT_DEFINED,
	 BP_NOT_DEFINED,
	 BP_NOT_DEFINED,
};


static BOARD_PARAMETERS g_huawei963268gw_HG2XX=
{
    "963268_hg2xx",                        /* szBoardId */
  (
    BP_OVERLAY_PHY |
    BP_OVERLAY_SERIAL_LEDS |
    BP_OVERLAY_USB_DEVICE ),                /* usGPIOOverlay */
    BP_MEMORY_16MB_2_CHIP,                  /* SDRAM size and type */
     
    BP_NOT_DEFINED,                         /* usGpioRj11InnerPair */
    BP_NOT_DEFINED,                         /* usGpioRj11OuterPair */
    BP_GPIO_32_AL,                         /* usGpioPressAndHoldReset */
    BP_GPIO_20_AL,                          /* usGpioPressWps */
    BP_NOT_DEFINED,                         /* usGpioUartRts */
    BP_NOT_DEFINED,                         /* usGpioUartCts */    
    
    BP_NOT_DEFINED,                         /* usVcopeResetGpio */
    BP_NOT_DEFINED,                         /* usVcopeRelayGpio */

    BP_SERIAL_GPIO_0_AL,                    /* usGpioLedPower */
    BP_SERIAL_GPIO_1_AL,                    /* usGpioLedPowerFail */ 
    BP_SERIAL_GPIO_3_AL,                    /* usGpioLedAdsl */
    BP_NOT_DEFINED,                         /* usGpioLedAdslFail */
    BP_NOT_DEFINED,                         /* usGpioSecLedAdsl */
    BP_NOT_DEFINED,                         /* usGpioSecLedAdslFail */
    BP_GPIO_37_AL,                          /* usGpioLedWirelessEnable */
    BP_GPIO_36_AL,                          /* usGpioLedWireless */
    BP_NOT_DEFINED,                         /* usGpioLedWirelessFail */
    BP_NOT_DEFINED,                         /* usGpioLedWPS */
    BP_NOT_DEFINED,                         /* usGpioLedWPSFail */
    BP_NOT_DEFINED,                         /* usGpioLedUsb */
    BP_NOT_DEFINED,                         /* usGpioLedUsbFail */
    BP_SERIAL_GPIO_9_AL,                    /* usGpioLedLan */
    BP_NOT_DEFINED,                         /* usGpioLedLan1 */
    BP_NOT_DEFINED,                         /* usGpioLedLan2 */
    BP_NOT_DEFINED,                         /* usGpioLedLan3 */
    BP_NOT_DEFINED,                         /* usGpioLedLan4 */
    BP_NOT_DEFINED,                         /* usGpioLedSesWireless */
    BP_NOT_DEFINED,                         /* usGpioLedHpna */
    BP_NOT_DEFINED,                         /* usGpioLedWanData */
    BP_NOT_DEFINED,                         /* usGpioLedWanDataFail */
    BP_NOT_DEFINED,                         /* usGpioLedHspa */
    
    BP_NOT_DEFINED,                         /* usGpioLedHspaFail */
    BP_SERIAL_GPIO_8_AL,                    /* usGpioLedInternet */
    BP_SERIAL_GPIO_2_AL,                    /* usGpioLedInternetFail*/
    
    BP_NOT_DEFINED,                         /* usGpioLedPpp */
    BP_NOT_DEFINED,                         /* usGpioLedPppFail */
    BP_GPIO_22_AL,                          /* usGpioLedBlPowerOn */
    BP_NOT_DEFINED,                         /* usGpioLedBlResetCfg */
    BP_NOT_DEFINED,                         /* usGpioLedBlStop */
    BP_NOT_DEFINED,                         /* usGpioBondingSecondaryReset */

    BP_HW_DEFINED,                          /* usExtIntrAdslDyingGasp */
    BP_EXT_INTR_0,                          /* usExtIntrResetToDefault */
    BP_NOT_DEFINED,                          /* usExtIntrSesBtnWireless */
    BP_NOT_DEFINED,                         /* usExtIntrSesBtnWireless */
    
    BP_NOT_DEFINED,                         /* usExtIntrHpna */

    BP_NOT_DEFINED,                         /* usCsHpna */
    BP_NOT_DEFINED,                         /* usCsHpi */

    BP_WLAN_ANT_MAIN,                       /* usAntInUseWireless */
    BP_NOT_DEFINED,                         /* WLAN flags */
    BP_NOT_DEFINED,                         /* usVcopeBoardRev */
    1,                                      /* usCMTThead */    
    0,                                      /* Hardware config */
    {BP_NOT_DEFINED,                        /* PCB version low bit GPIO */
     BP_NOT_DEFINED},                       /* PCB version high bit GPIO */
    {BP_NOT_DEFINED,                        /* Board version low bit GPIO */
     BP_NOT_DEFINED,                        /* Board version middle bit GPIO */                   
     BP_NOT_DEFINED,                        /* Board version high bit GPIO */       
     BP_NOT_DEFINED},                       /* usReserve */
#if 1 /*暂时回退最新lanswitch配置，确保658b单板上正常*/
    {{BP_ENET_EXTERNAL_SWITCH,              /* ucPhyType */
      0x0,                                  /* ucPhyAddress */
      BP_ENET_CONFIG_MMAP,                  /* usConfigType */
      {0x1f, {0x01, 0x02, 0x03, 0x04, 0x18| MAC_IFACE_VALID | MAC_IF_RGMII, 0x00, 0x00, 0x00}}},  /* sw */
      {BP_ENET_NO_PHY}},                    /* sw */
#else
    {
        {
            BP_ENET_EXTERNAL_SWITCH,              /* ucPhyType */    
            30,                                     /* ucPhyAddress */    
            BP_NOT_DEFINED,                         /* usGpioLedPhyLinkSpeed */    
            BP_ENET_CONFIG_MDIO,                    /* usConfigType *//*Add by l63336 for 适配802.3az的GPHY,规避硬件问题*/

            {
                0x3f, 
                {0x01, 0x02, 0x03, 0x04, 0x18, 0x11, 0x00, 0x00}
            }
        },  /* sw */                
        {
            BP_ENET_EXTERNAL_SWITCH,                  /* ucPhyType */    
            30,                                      /* ucPhyAddress */    
            BP_NOT_DEFINED,                         /* usGpioLedPhyLinkSpeed */    
            BP_ENET_CONFIG_MDIO,              /* usConfigType *//*Add by l63336 for 适配802.3az的GPHY,规避硬件问题*/
            {
                0x3f, 
                {0x01, 0x02, 0x03, 0x04, 0x10, 0x11, 0x00, 0x00}
            }
        }  /* sw */
    }, 


#endif

    {{BP_VOIP_MIPS,                         /* ucDspType */
    0,                                      /* ucDspAddress */
    BP_NOT_DEFINED,                         /* usGpioLedVoip */
    BP_SERIAL_GPIO_10_AL,                   /* usGpioVoip1Led */
    BP_SERIAL_GPIO_11_AL,                   /* usGpioVoip1LedFail */
    BP_NOT_DEFINED,                         /* usGpioVoip2Led */
    BP_NOT_DEFINED,                         /* usGpioVoip2LedFail */
    BP_NOT_DEFINED,                         /* usGpioPotsLed */
    BP_NOT_DEFINED},                        /* usGpioDectLed */
     {BP_VOIP_NO_DSP}}, 
	Legerity_89116, 						/* SLIC type */
	Legerity_89x16_daa,                     /* DAA type */
    BP_NOT_DEFINED,                         /* usGpioWirelessPowerDown */
#ifdef CONFIG_ATP_SUPPORT_ANNEX_B
    {BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXB | BP_AFE_FE_REV_6302_REV1,
    BP_AFE_DEFAULT},                        /* ulAfeIds */
#else    
    {BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV1,
      BP_AFE_DEFAULT},                      /* ulAfeIds */
#endif
     BP_NOT_DEFINED,
	 BP_NOT_DEFINED,
	 BP_NOT_DEFINED,
};


static BOARD_PARAMETERS g_huawei963268gw_HG257=
{
    "963268_hg257",                        /* szBoardId */
  (
    BP_OVERLAY_PHY |
    BP_OVERLAY_SERIAL_LEDS |
    BP_OVERLAY_USB_DEVICE ),                /* usGPIOOverlay */
    BP_MEMORY_16MB_2_CHIP,                  /* SDRAM size and type */
     
    BP_NOT_DEFINED,                         /* usGpioRj11InnerPair */
    BP_NOT_DEFINED,                         /* usGpioRj11OuterPair */
    BP_GPIO_32_AL,                         /* usGpioPressAndHoldReset */
    BP_GPIO_20_AL,                          /* usGpioPressWps */
    BP_NOT_DEFINED,                         /* usGpioUartRts */
    BP_NOT_DEFINED,                         /* usGpioUartCts */    
    
    BP_NOT_DEFINED,                         /* usVcopeResetGpio */
    BP_NOT_DEFINED,                         /* usVcopeRelayGpio */

    BP_SERIAL_GPIO_0_AL,                    /* usGpioLedPower */
    BP_NOT_DEFINED,                    /* usGpioLedPowerFail */ 
    BP_NOT_DEFINED,                    /* usGpioLedAdsl */
    BP_NOT_DEFINED,                         /* usGpioLedAdslFail */
    BP_NOT_DEFINED,                         /* usGpioSecLedAdsl */
    BP_NOT_DEFINED,                         /* usGpioSecLedAdslFail */
    BP_GPIO_37_AL,                          /* usGpioLedWirelessEnable */
    BP_GPIO_36_AL,                          /* usGpioLedWireless */
    BP_NOT_DEFINED,                         /* usGpioLedWirelessFail */
    BP_NOT_DEFINED,                         /* usGpioLedWPS */
    BP_NOT_DEFINED,                         /* usGpioLedWPSFail */
    BP_SERIAL_GPIO_5_AL,                         /* usGpioLedUsb */
    BP_NOT_DEFINED,                         /* usGpioLedUsbFail */
    BP_GPIO_38_AL,                    /* usGpioLedLan */
    BP_NOT_DEFINED,                         /* usGpioLedLan1 */
    BP_NOT_DEFINED,                         /* usGpioLedLan2 */
    BP_NOT_DEFINED,                         /* usGpioLedLan3 */
    BP_NOT_DEFINED,                         /* usGpioLedLan4 */
    BP_NOT_DEFINED,                         /* usGpioLedSesWireless */
    BP_NOT_DEFINED,                         /* usGpioLedHpna */
    BP_GPIO_34_AL,                         /* usGpioLedWanData */
    BP_NOT_DEFINED,                         /* usGpioLedWanDataFail */
    BP_SERIAL_GPIO_2_AL,                         /* usGpioLedHspa */
    BP_SERIAL_GPIO_3_AL,                         /* usGpioLedHspaFail */
    BP_GPIO_8_AL,                    /* usGpioLedInternet */
    BP_SERIAL_GPIO_4_AL,                    /* usGpioLedInternetFail*/
    
    BP_NOT_DEFINED,                         /* usGpioLedPpp */
    BP_NOT_DEFINED,                         /* usGpioLedPppFail */
    BP_GPIO_22_AL,                          /* usGpioLedBlPowerOn */
    BP_NOT_DEFINED,                         /* usGpioLedBlResetCfg */
    BP_NOT_DEFINED,                         /* usGpioLedBlStop */
    BP_NOT_DEFINED,                         /* usGpioBondingSecondaryReset */

    BP_HW_DEFINED,                          /* usExtIntrAdslDyingGasp */
    BP_EXT_INTR_0,                          /* usExtIntrResetToDefault */
    BP_NOT_DEFINED,                          /* usExtIntrSesBtnWireless */
    BP_NOT_DEFINED,                         /* usExtIntrSesBtnWireless */
    
    BP_NOT_DEFINED,                         /* usExtIntrHpna */

    BP_NOT_DEFINED,                         /* usCsHpna */
    BP_NOT_DEFINED,                         /* usCsHpi */

    BP_WLAN_ANT_MAIN,                       /* usAntInUseWireless */
    BP_NOT_DEFINED,                         /* WLAN flags */
    BP_NOT_DEFINED,                         /* usVcopeBoardRev */
    1,                                      /* usCMTThead */    
    0,                                      /* Hardware config */
    {BP_NOT_DEFINED,                        /* PCB version low bit GPIO */
     BP_NOT_DEFINED},                       /* PCB version high bit GPIO */
    {BP_NOT_DEFINED,                        /* Board version low bit GPIO */
     BP_NOT_DEFINED,                        /* Board version middle bit GPIO */                   
     BP_NOT_DEFINED,                        /* Board version high bit GPIO */       
     BP_NOT_DEFINED},                       /* usReserve */
#if 0
    {{BP_ENET_EXTERNAL_SWITCH,              /* ucPhyType */
      0x0,                                  /* ucPhyAddress */
      BP_ENET_CONFIG_MMAP,                  /* usConfigType */
      {0x1f, {0x01, 0x02, 0x03, 0x04, 0x18| MAC_IFACE_VALID | MAC_IF_RGMII, 0x00, 0x00, 0x00}}},  /* sw */
      {BP_ENET_NO_PHY}},                    /* sw */
#else
    {
        {
            BP_ENET_EXTERNAL_SWITCH,              /* ucPhyType */    
            0x0,                                     /* ucPhyAddress */      
            BP_ENET_CONFIG_MMAP,                    /* usConfigType *//*Add by l63336 for 适配802.3az的GPHY,规避硬件问题*/

            {
                0x58, 
                {0x00, 0x00, 0x00, BP_PHY_ID_4, RGMII_DIRECT | EXTSW_CONNECTED, 0x00,RGMII_DIRECT | BP_PHY_ID_25, 0x00}
            }
        },  /* sw */                
        {
            BP_ENET_EXTERNAL_SWITCH,              /* ucPhyType */    
            0x30,                                     /* ucPhyAddress */    
            BP_ENET_CONFIG_MDIO,                    /* usConfigType *//*Add by l63336 for 适配802.3az的GPHY,规避硬件问题*/

            {
                0x1e, 
#if 0
                {0x01, 0x02, 0x03, BP_PHY_ID_4, RGMII_DIRECT | EXTSW_CONNECTED, 0x11, BP_PHY_ID_25, 0x00}
#else                
                {0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00}
#endif
            }
        }  /* sw */
    }, 

#endif

    {{BP_VOIP_MIPS,                         /* ucDspType */
    0,                                      /* ucDspAddress */
    BP_NOT_DEFINED,                         /* usGpioLedVoip */
    BP_SERIAL_GPIO_7_AL,                   /* usGpioVoip1Led */
    BP_SERIAL_GPIO_6_AL,                   /* usGpioVoip1LedFail */
    BP_NOT_DEFINED,                         /* usGpioVoip2Led */
    BP_NOT_DEFINED,                         /* usGpioVoip2LedFail */
    BP_NOT_DEFINED,                         /* usGpioPotsLed */
    BP_NOT_DEFINED},                        /* usGpioDectLed */
     {BP_VOIP_NO_DSP}}, 
	Legerity_89116, 						/* SLIC type */
	Legerity_89x16_daa,                     /* DAA type */
    BP_NOT_DEFINED,                         /* usGpioWirelessPowerDown */
#ifdef CONFIG_ATP_SUPPORT_ANNEX_B
    {BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXB | BP_AFE_FE_REV_6302_REV1,
    BP_AFE_DEFAULT},                        /* ulAfeIds */
#else    
    {BP_AFE_CHIP_INT | BP_AFE_LD_6302 | BP_AFE_FE_ANNEXA | BP_AFE_FE_REV_6302_REV1,
      BP_AFE_DEFAULT},                      /* ulAfeIds */
#endif
     BP_NOT_DEFINED,
	 BP_NOT_DEFINED,
	 BP_NOT_DEFINED,
};

static PBOARD_PARAMETERS g_BoardParms[] =
    {&g_huawei963268gw_HG658B,&g_huawei963268gw_HG257,&g_huawei963268gw_HG2XX,0};


static PBOARD_PARAMETERS g_pCurrentBp = 0;

#endif

extern LED_CTRLLIST g_ledctrllist[kLedfailstart];

static int GetLed( unsigned short *pusValue ,BOARD_LED_NAME ename)
{
    int nRet = BP_VALUE_NOT_DEFINED;
    *pusValue = BP_NOT_DEFINED;
    
    if(NULL != g_ledctrllist[ename].phyled)
    {
        if(g_ledctrllist[ename].phyled->failshift == 0)
        {
            if( GPIO_NOEFFECT != g_ledctrllist[ename].phyled->ledGpio)
            {
                *pusValue = g_ledctrllist[ename].phyled->ledGpio;
            }
            else
            {
               nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            if(GPIO_NOEFFECT != g_ledctrllist[ename].phyled->ledfailGpio)
            {
                *pusValue = g_ledctrllist[ename].phyled->ledfailGpio;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return nRet;
}

static int GetfailLed( unsigned short *pusValue ,BOARD_LED_NAME ename)
{
    int nRet = BP_VALUE_NOT_DEFINED;
    *pusValue = BP_NOT_DEFINED;
    if(NULL != g_ledctrllist[ename].phyled)
    {
        if(g_ledctrllist[ename].phyled->failshift == 0)
        {
            if( GPIO_NOEFFECT != g_ledctrllist[ename].phyled->ledfailGpio)
            {
                *pusValue = g_ledctrllist[ename].phyled->ledfailGpio;
            }
            else
            {   
               nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            if(GPIO_NOEFFECT != g_ledctrllist[ename].phyled->ledGpio)
            {
                *pusValue = g_ledctrllist[ename].phyled->ledGpio;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return nRet;
}

#if 1
/* Private function prototypes */
bp_elem_t * BpGetElem(enum bp_id id, int *pindex, enum bp_id stopAtId);
unsigned char BpGetSubUc(enum bp_id id, int index, enum bp_id stopAtId );
unsigned short BpGetSubUs(enum bp_id id, int index, enum bp_id stopAtId );
unsigned long BpGetSubUl(enum bp_id id, int index, enum bp_id stopAtId );
int BpGetUc(enum bp_id id, unsigned char *pucValue );
int BpGetUs(enum bp_id id, unsigned short *pusValue );
int BpGetUl(enum bp_id id, unsigned long *pulValue );

/**************************************************************************
* Name       : bpstrcmp
*
* Description: String compare for this file so it does not depend on an OS.
*              (Linux kernel and CFE share this source file.)
*
* Parameters : [IN] dest - destination string
*              [IN] src - source string
*
* Returns    : -1 - dest < src, 1 - dest > src, 0 dest == src
***************************************************************************/
int bpstrcmp(const char *dest,const char *src)
{
    while (*src && *dest) {
        if (*dest < *src) return -1;
        if (*dest > *src) return 1;
        dest++;
        src++;
    }

    if (*dest && !*src) return 1;
    if (!*dest && *src) return -1;
    return 0;
} /* bpstrcmp */


/**************************************************************************
* Name       : BpGetElem
*
* Description: Private function to walk through the profile
*              and find the desired entry
*
* Parameters : [IN] id         - id to search for
*              [IN/OUT] pindex - where to start and where it was found
*              [IN] stopAtId   - id to stop at if the searched id is not found
*                                (allows grouping and repeated ids)
*
* Returns    : ptr to entry found or to last entry otherwise
***************************************************************************/
bp_elem_t * BpGetElem(enum bp_id id, int *pindex, enum bp_id stopAtId)
{
    bp_elem_t * pelem;

    for (pelem = &g_pCurrentBp[*pindex]; pelem->id != bp_last && pelem->id != id && pelem->id != stopAtId; pelem++, (*pindex)++);

    return pelem;    
}

/**************************************************************************
* Name       : BpGetSubUc
*
* Description: Private function to get an unsigned char entry from the profile
*              can be used to search an id within a group by specifying stop id
*
* Parameters : [IN] id       - id to search for
*              [IN] index    - where to start from
*              [IN] stopAtId - id to stop at if the searched id is not found
*                                (allows grouping and repeated ids)
*
* Returns    : the unsigned char from the entry
***************************************************************************/
unsigned char BpGetSubUc(enum bp_id id, int index, enum bp_id stopAtId )
{
    bp_elem_t *pelem;

    pelem = BpGetElem(id, &index, stopAtId);
    if (id == pelem->id) {
        return pelem->u.uc;
    } else {
        return (unsigned char)BP_NOT_DEFINED;
    }
}

/**************************************************************************
* Name       : BpGetSubUs
*
* Description: Private function to get an unsigned short entry from the profile
*              can be used to search an id within a group by specifying stop id
*
* Parameters : [IN] id       - id to search for
*              [IN] index    - where to start from
*              [IN] stopAtId - id to stop at if the searched id is not found
*                                (allows grouping and repeated ids)
*
* Returns    : the unsigned short from the entry
***************************************************************************/
unsigned short BpGetSubUs(enum bp_id id, int index, enum bp_id stopAtId )
{
    bp_elem_t *pelem;

    pelem = BpGetElem(id, &index, stopAtId);
    if (id == pelem->id) {
        return pelem->u.us;
    } else {
        return BP_NOT_DEFINED;
    }
}

/**************************************************************************
* Name       : BpGetSubUl
*
* Description: Private function to get an unsigned long entry from the profile
*              can be used to search an id within a group by specifying stop id
*
* Parameters : [IN] id       - id to search for
*              [IN] index    - where to start from
*              [IN] stopAtId - id to stop at if the searched id is not found
*                                (allows grouping and repeated ids)
*
* Returns    : the unsigned long from the entry
***************************************************************************/
unsigned long BpGetSubUl(enum bp_id id, int index, enum bp_id stopAtId )
{
    bp_elem_t *pelem;

    pelem = BpGetElem(id, &index, stopAtId);
    if (id == pelem->id)
        return pelem->u.ul;
    else
        return BP_NOT_DEFINED;
}

/**************************************************************************
* Name       : BpGetUc
*
* Description: Private function to get an unsigned char entry from the profile
*              can only be used to search an id which appears once in the profile
*
* Parameters : [IN] id       - id to search for
*              [IN] pucValue - unsigned char found
*
* Returns    : BP_SUCCESS or appropriate error
***************************************************************************/
int BpGetUc(enum bp_id id, unsigned char *pucValue )
{
    int nRet;

    if( g_pCurrentBp ) {
        *pucValue = BpGetSubUc(id, 0, bp_last);
        nRet = ((unsigned char)BP_NOT_DEFINED != *pucValue ? BP_SUCCESS : BP_VALUE_NOT_DEFINED);
    } else {
        *pucValue = (unsigned char)BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
}

/**************************************************************************
* Name       : BpGetUs
*
* Description: Private function to get an unsigned short entry from the profile
*              can only be used to search an id which appears once in the profile
*
* Parameters : [IN] id       - id to search for
*              [IN] pusValue - unsigned short found
*
* Returns    : BP_SUCCESS or appropriate error
***************************************************************************/
int BpGetUs(enum bp_id id, unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp ) 
    {
        *pusValue = BpGetSubUs(id, 0, bp_last);
        nRet = (BP_NOT_DEFINED != *pusValue ? BP_SUCCESS : BP_VALUE_NOT_DEFINED);
    } 
    else 
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
}

/**************************************************************************
* Name       : BpGetUl
*
* Description: Private function to get an unsigned long entry from the profile
*              can only be used to search an id which appears once in the profile
*
* Parameters : [IN] id       - id to search for
*              [IN] pulValue - unsigned long found
*
* Returns    : BP_SUCCESS or appropriate error
***************************************************************************/
int BpGetUl(enum bp_id id, unsigned long *pulValue )
{
    int nRet;

    if( g_pCurrentBp ) {
        *pulValue = BpGetSubUl(id, 0, bp_last);
        nRet = (BP_NOT_DEFINED != *pulValue ? BP_SUCCESS : BP_VALUE_NOT_DEFINED);
    } else {
        *pulValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
}

/**************************************************************************
* Name       : BpGetVoipDspConfig
*
* Description: Gets the DSP configuration from the board parameter
*              structure for a given DSP index.
*
* Parameters : [IN] dspNum - DSP index (number)
*
***************************************************************************/
VOIP_DSP_INFO g_VoIPDspInfo[BP_MAX_VOIP_DSP] = {{0}};
#if 1
VOIP_DSP_INFO *BpGetVoipDspConfig( unsigned char dspNum )
{
    VOIP_DSP_INFO *pDspConfig = 0;
    int i;
    bp_elem_t *pelem;
    int iDspType;
    enum bp_id bp_aucDspType[BP_MAX_VOIP_DSP+1] = {bp_ucDspType0, bp_ucDspType1, bp_last};
    enum bp_id bp_current, bp_next;

    if( g_pCurrentBp ) {
        /* First initialize the structure to known values */
        for( i = 0 ; i < BP_MAX_VOIP_DSP ; i++ ) {
            g_VoIPDspInfo[i].ucDspType = BP_VOIP_NO_DSP;
        }

        /* Now populate it with what we have in the element array */
        for( i = 0 ; i < BP_MAX_VOIP_DSP ; i++ ) {
            iDspType = 0;
            bp_current = bp_aucDspType[i];
            bp_next    = bp_aucDspType[i+1];
            pelem = BpGetElem(bp_current, &iDspType, bp_next);
            if (bp_current != pelem->id) {
                continue;
            }
            g_VoIPDspInfo[i].ucDspType = pelem->u.uc;

            ++iDspType;
            g_VoIPDspInfo[i].ucDspAddress       = BpGetSubUc(bp_ucDspAddress, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioLedVoip      = BpGetSubUs(bp_usGpioLedVoip, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioVoip1Led     = BpGetSubUs(bp_usGpioLedVoip1, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioVoip1LedFail = BpGetSubUs(bp_usGpioLedVoip1Fail, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioVoip2Led     = BpGetSubUs(bp_usGpioLedVoip2, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioVoip2LedFail = BpGetSubUs(bp_usGpioLedVoip2Fail, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioPotsLed      = BpGetSubUs(bp_usGpioLedPots, iDspType, bp_next);
            g_VoIPDspInfo[i].usGpioDectLed      = BpGetSubUs(bp_usGpioLedDect, iDspType, bp_next);
        }

        /* Transfer the requested results */
        for( i = 0 ; i < BP_MAX_VOIP_DSP ; i++ ) {
            if( g_VoIPDspInfo[i].ucDspType != BP_VOIP_NO_DSP &&
                g_VoIPDspInfo[i].ucDspAddress == dspNum ) {
                pDspConfig = &g_VoIPDspInfo[i];
                break;
            }
        }
    }

    return pDspConfig;
} /* BpGetVoipDspConfig */
#endif

/**************************************************************************
* Name       : BpSetBoardId
*
* Description: This function find the BOARD_PARAMETERS structure for the
*              specified board id string and assigns it to a global, static
*              variable.
*
* Parameters : [IN] pszBoardId - Board id string that is saved into NVRAM.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_FOUND - Error, board id input string does not
*                  have a board parameters configuration record.
***************************************************************************/
int BpSetBoardId( char *pszBoardId )
{
    int nRet = BP_BOARD_ID_NOT_FOUND;
    bp_elem_t **ppcBp;

    for( ppcBp = g_BoardParms; *ppcBp; ppcBp++ ) {
        if( !bpstrcmp((*ppcBp)[0].u.cp, pszBoardId) ) {
            g_pCurrentBp = *ppcBp;
            nRet = BP_SUCCESS;
            break;
        }
    }

    return( nRet );
} /* BpSetBoardId */

/**************************************************************************
* Name       : BpGetBoardId
*
* Description: This function returns the current board id strings.
*
* Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
*                  string is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
***************************************************************************/
int BpGetBoardId( char *pszBoardId )
{
    int i;

    if (g_pCurrentBp == 0) {
        return -1;
    }

    for (i = 0; i < BP_BOARD_ID_LEN; i++) {
        pszBoardId[i] = g_pCurrentBp[0].u.cp[i];
    }

    return 0;
}

/**************************************************************************
* Name       : BpGetBoardIds
*
* Description: This function returns all of the supported board id strings.
*
* Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
*                  strings are returned in.  Each id starts at BP_BOARD_ID_LEN
*                  boundary.
*              [IN] nBoardIdsSize - Number of BP_BOARD_ID_LEN elements that
*                  were allocated in pszBoardIds.
*
* Returns    : Number of board id strings returned.
***************************************************************************/
int BpGetBoardIds( char *pszBoardIds, int nBoardIdsSize )
{
    int i;
    char *src;
    char *dest;
    bp_elem_t **ppcBp;

    for( i = 0, ppcBp = g_BoardParms; *ppcBp && nBoardIdsSize;
        i++, ppcBp++, nBoardIdsSize--, pszBoardIds += BP_BOARD_ID_LEN ) {
        dest = pszBoardIds;
        src = (*ppcBp)[0].u.cp;
        while( *src ) {
            *dest++ = *src++;
        }
        *dest = '\0';
    }

    return( i );
} /* BpGetBoardIds */

/**************************************************************************
* Name       : BpGetGPIOverlays
*
* Description: This function GPIO overlay configuration
*
* Parameters : [OUT] pusValue - Address of short word that interfaces in use.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
//int BpGetGPIOverlays( unsigned long *pulValue )
int BpGetGPIOverlays( unsigned short *pulValue )
{
    return( BpGetUs(bp_usGpioOverlay, pulValue ) );
} /* BpGetGPIOverlays */


/**************************************************************************
* Name       : BpGetEthernetMacInfo
*
* Description: This function returns all of the supported board id strings.
*
* Parameters : [OUT] pEnetInfos - Address of an array of ETHERNET_MAC_INFO
*                  buffers.
*              [IN] nNumEnetInfos - Number of ETHERNET_MAC_INFO elements that
*                  are pointed to by pEnetInfos.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
***************************************************************************/
int BpGetEthernetMacInfo( PETHERNET_MAC_INFO pEnetInfos, int nNumEnetInfos )
{
    int i, j;
    bp_elem_t *pelem;
    int iPhyType;
    int iPhyId;
    int nRet = BP_BOARD_ID_NOT_SET;
    PETHERNET_MAC_INFO pE;
    enum bp_id bp_aucPhyType[BP_MAX_ENET_MACS+1] = {bp_ucPhyType0, bp_ucPhyType1, bp_last};
    enum bp_id bp_current, bp_next;
    enum bp_id bp_aulPhyId[BP_MAX_SWITCH_PORTS+1] = {bp_ulPhyId0, bp_ulPhyId1, bp_ulPhyId2,
                bp_ulPhyId3, bp_ulPhyId4, bp_ulPhyId5, bp_ulPhyId6, bp_ulPhyId7, bp_last};
    enum bp_id bp_current_phyid;

#if 0
    /* First initialize the structure to known values */
    for( i = 0, pE = pEnetInfos; i < nNumEnetInfos; i++, pE++ ) {
        pE->ucPhyType = BP_ENET_NO_PHY;
        /* The old code only initialized the first set, let's tdo the same so the 2 compare without error */
        if (0 == i) {
            for( j = 0; j < BP_MAX_ENET_INTERNAL; j++ ) {
                pE->sw.ledInfo[j].duplexLed = BP_NOT_DEFINED;
                pE->sw.ledInfo[j].speedLed100 = BP_NOT_DEFINED;
                pE->sw.ledInfo[j].speedLed1000 = BP_NOT_DEFINED;
            }
        }
    }
#endif

    if( g_pCurrentBp ) {
        /* Populate it with what we have in the element array */
        for( i = 0, pE = pEnetInfos; i < BP_MAX_ENET_MACS; i++, pE++ ) {
            iPhyType = 0;
            bp_current = bp_aucPhyType[i];
            bp_next    = bp_aucPhyType[i+1];
            pelem = BpGetElem(bp_current, &iPhyType, bp_next);
            if (bp_current != pelem->id)
                continue;
            pE->ucPhyType = pelem->u.uc;

            ++iPhyType;
            pE->ucPhyAddress  = BpGetSubUc(bp_ucPhyAddress, iPhyType, bp_next);
            pE->usConfigType  = BpGetSubUs(bp_usConfigType, iPhyType, bp_next);
            pE->sw.port_map   = BpGetSubUl(bp_ulPortMap, iPhyType, bp_next);

            for( j = 0; j < BP_MAX_SWITCH_PORTS; j++ ) {
                iPhyId = iPhyType;
                bp_current_phyid = bp_aulPhyId[j];
                pelem = BpGetElem(bp_current_phyid, &iPhyId, bp_next);
                if (bp_current_phyid == pelem->id) {
                    pE->sw.phy_id[j] = pelem->u.ul & ~MII_OVER_GPIO_VALID;
                    ++iPhyId;
                    #if 0
                    if (j < BP_MAX_ENET_INTERNAL) {
                        while (iPhyId) {
                            switch (g_pCurrentBp[iPhyId].id) {
                            case bp_usDuplexLed:
                                pE->sw.ledInfo[j].duplexLed = g_pCurrentBp[iPhyId].u.us;
                                ++iPhyId;
                                break;
                            case bp_usSpeedLed100:
                                pE->sw.ledInfo[j].speedLed100 = g_pCurrentBp[iPhyId].u.us;
                                ++iPhyId;
                                break;
                            case bp_usSpeedLed1000:
                                pE->sw.ledInfo[j].speedLed1000 = g_pCurrentBp[iPhyId].u.us;
                                ++iPhyId;
                                break;
                            default:
                                iPhyId = 0;
                                break;
                            }
                        }
                    }
                    #endif
                } 
                else
                {
                    pE->sw.phy_id[j] = 0;
                }
            }
        }
        nRet = BP_SUCCESS;
    }
    return( nRet );

} /* BpGetEthernetMacInfo */

#if defined(CONFIG_BCM963268) 
int BpGetPortConnectedToExtSwitch(void)
{
    unsigned long phy_id, port_map;
    int i, extSwConnPort = -1;

    BpGetUl(bp_ulPortMap, &port_map);

    for (i = 0; i < BP_MAX_SWITCH_PORTS; i++) {
        if ( port_map &  (1 << i)) {
            BpGetUl(bp_ulPhyId0 + i, &phy_id);
            if (phy_id & EXTSW_CONNECTED)
            {
                extSwConnPort = i;
            }
        }
    }
    return extSwConnPort;
}
#endif

/**************************************************************************
* Name       : BpGetMiiOverGpioFlag
*
* Description: This function returns logical disjunction of MII over GPIO
*              flag over all PHY IDs.
*
* Parameters : [OUT] pMiiOverGpioFlag - MII over GPIO flag
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
***************************************************************************/
int BpGetMiiOverGpioFlag( unsigned long* pMiiOverGpioFlag )
{
    int i, j;
    bp_elem_t *pelem;
    int iPhyType;
    int iPhyId;
    int nRet;
    enum bp_id bp_aucPhyType[BP_MAX_ENET_MACS+1] = {bp_ucPhyType0, bp_ucPhyType1, bp_last};
    enum bp_id bp_current, bp_next;
    enum bp_id bp_aulPhyId[BP_MAX_SWITCH_PORTS+1] = {bp_ulPhyId0, bp_ulPhyId1, bp_ulPhyId2,
                bp_ulPhyId3, bp_ulPhyId4, bp_ulPhyId5, bp_ulPhyId6, bp_ulPhyId7, bp_last};
    enum bp_id bp_current_phyid;

    *pMiiOverGpioFlag = 0;

    if( g_pCurrentBp ) {
        for( i = 0; i < BP_MAX_ENET_MACS; i++ ) {
            iPhyType = 0;
            bp_current = bp_aucPhyType[i];
            bp_next    = bp_aucPhyType[i+1];
            pelem = BpGetElem(bp_current, &iPhyType, bp_next);
            if (bp_current != pelem->id)
                continue;

            ++iPhyType;
            for( j = 0; j < BP_MAX_SWITCH_PORTS; j++ ) {
                iPhyId = iPhyType;
                bp_current_phyid = bp_aulPhyId[j];
                pelem = BpGetElem(bp_current_phyid, &iPhyId, bp_next);
                if (bp_current_phyid == pelem->id) {
                    *pMiiOverGpioFlag |= pelem->u.ul & MII_OVER_GPIO_VALID;                
                    ++iPhyId;
                }
            }
        }
        // Normalize flag value by positioning in lsb position        
        *pMiiOverGpioFlag >>= MII_OVER_GPIO_S;
        nRet = BP_SUCCESS;
    }
    else
    {
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );

} /* BpGetMiiOverGpioFlag */

/**************************************************************************
* Name       : BpGetRj11InnerOuterPairGpios
*
* Description: This function returns the GPIO pin assignments for changing
*              between the RJ11 inner pair and RJ11 outer pair.
*
* Parameters : [OUT] pusInner - Address of short word that the RJ11 inner pair
*                  GPIO pin is returned in.
*              [OUT] pusOuter - Address of short word that the RJ11 outer pair
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, values are returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetRj11InnerOuterPairGpios( unsigned short *pusInner,
                                 unsigned short *pusOuter )
{
    *pusInner = BP_NOT_DEFINED;
    *pusOuter = BP_NOT_DEFINED;

    return( BP_VALUE_NOT_DEFINED );
} /* BpGetRj11InnerOuterPairGpios */

/**************************************************************************
* Name       : BpGetUartRtsCtsGpios
*
* Description: This function returns the GPIO pin assignments for RTS and CTS
*              UART signals.
*
* Parameters : [OUT] pusRts - Address of short word that the UART RTS GPIO
*                  pin is returned in.
*              [OUT] pusCts - Address of short word that the UART CTS GPIO
*                  pin is returned in.
*
* Returns    : BP_SUCCESS - Success, values are returned.
*              BP_BOARD_ID_NOT_SET - Error, board id input string does not
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetRtsCtsUartGpios( unsigned short *pusRts, unsigned short *pusCts )
{
    *pusRts = BP_NOT_DEFINED;
    *pusCts = BP_NOT_DEFINED;

    return( BP_VALUE_NOT_DEFINED );
} /* BpGetUartRtsCtsGpios */

/**************************************************************************
* Name       : BpGetAdslLedGpio
*
* Description: This function returns the GPIO pin assignment for the ADSL
*              LED.
*
* Parameters : [OUT] pusValue - Address of short word that the ADSL LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetAdslLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedAdsl);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedAdsl, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetAdslLedGpio */

/**************************************************************************
* Name       : BpGetAdslFailLedGpio
*
* Description: This function returns the GPIO pin assignment for the ADSL
*              LED that is used when there is a DSL connection failure.
*
* Parameters : [OUT] pusValue - Address of short word that the ADSL LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetAdslFailLedGpio( unsigned short *pusValue )
{
    int nRet = BP_VALUE_NOT_DEFINED;
    nRet =  GetfailLed( pusValue ,kLedAdsl);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedAdslFail, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetAdslFailLedGpio */

/**************************************************************************
* Name       : BpGetSecAdslLedGpio
*
* Description: This function returns the GPIO pin assignment for the ADSL
*              LED of the Secondary line, applicable more for bonding.
*
* Parameters : [OUT] pusValue - Address of short word that the ADSL LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetSecAdslLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedSecAdsl, pusValue ) );
} /* BpGetSecAdslLedGpio */

/**************************************************************************
* Name       : BpGetSecAdslFailLedGpio
*
* Description: This function returns the GPIO pin assignment for the ADSL
*              LED of the Secondary ADSL line, that is used when there is
*              a DSL connection failure.
*
* Parameters : [OUT] pusValue - Address of short word that the ADSL LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetSecAdslFailLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedSecAdslFail, pusValue ) );
} /* BpGetSecAdslFailLedGpio */

/**************************************************************************
* Name       : BpGetWirelessAntInUse
*
* Description: This function returns the antennas in use for wireless
*
* Parameters : [OUT] pusValue - Address of short word that the Wireless Antenna
*                  is in use.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWirelessAntInUse( unsigned short *pusValue )
{
    return( BpGetUs(bp_usAntInUseWireless, pusValue ) );
} /* BpGetWirelessAntInUse */

#if 0
/**************************************************************************
* Name       : BpGetWirelessFlags
*
* Description: This function returns optional control flags for wireless
*
* Parameters : [OUT] pusValue - Address of short word control flags
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWirelessFlags( unsigned short *pusValue )
{
    return( BpGetUs(bp_usWirelessFlags, pusValue ) );
} /* BpGetWirelessAntInUse */
#endif

/**************************************************************************
* Name       : BpGetWirelessSesExtIntr
*
* Description: This function returns the external interrupt number for the
*              Wireless Ses Button.
*
* Parameters : [OUT] pusValue - Address of short word that the Wireless Ses
*                  external interrup is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWirelessSesExtIntr( unsigned short *pusValue )
{
    return( BpGetUs(bp_usExtIntrSesBtnWireless, pusValue ) );
} /* BpGetWirelessSesExtIntr */

/**************************************************************************
* Name       : BpGetWirelessSesLedGpio
*
* Description: This function returns the GPIO pin assignment for the Wireless
*              Ses Led.
*
* Parameters : [OUT] pusValue - Address of short word that the Wireless Ses
*                  Led GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWirelessSesLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedWps);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedSesWireless, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetWirelessSesLedGpio */

/* this data structure could be moved to boardparams structure in the future */
/* does not require to rebuild cfe here if more srom entries are needed */
static WLAN_SROM_PATCH_INFO wlanPaInfo[]={   
    {"", 0, 0, {{0,0}}}, /* last entry*/
};

#if 0
/**************************************************************************
* Name       : BpUpdateWirelessSromMap
*
* Description: This function patch wireless PA values
*
* Parameters : [IN] unsigned short chipID
*              [IN/OUT] unsigned short* pBase - base of srom map
*              [IN/OUT] int size - size of srom map
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpUpdateWirelessSromMap(unsigned short chipID, unsigned short* pBase, int sizeInWords)
{
    int nRet = BP_BOARD_ID_NOT_FOUND;
    int i = 0;
    int j = 0;

    if(chipID == 0 || pBase == 0 || sizeInWords <= 0 )
        return nRet;

    i = 0;
    while ( wlanPaInfo[i].szboardId[0] != 0 ) {
        /* check boardId */
        if ( !bpstrcmp(g_pCurrentBp[0].u.cp, wlanPaInfo[i].szboardId) ) {
            /* check chipId */
            if ( (wlanPaInfo[i].usWirelessChipId == chipID) && (wlanPaInfo[i].usNeededSize <= sizeInWords) ){
                /* valid , patch entry */
                while ( wlanPaInfo[i].entries[j].wordOffset != 0) {
                    pBase[wlanPaInfo[i].entries[j].wordOffset] = wlanPaInfo[i].entries[j].value;
                    j++;
                }
                nRet = BP_SUCCESS;
                goto srom_update_done;
            }
        }
        i++;
    }

srom_update_done:

    return( nRet );

} /* BpUpdateWirelessSromMap */
#endif

static WLAN_PCI_PATCH_INFO wlanPciInfo[]={
    {"",                 0, 0, {{"",       0,      0}}}, /* last entry*/
};

/**************************************************************************
* Name       : BpUpdateWirelessPciConfig
*
* Description: This function patch wireless PCI Config Header
*              This is not functional critial/necessary but for dvt database maintenance
*
* Parameters : [IN] unsigned int pciID
*              [IN/OUT] unsigned int* pBase - base of pci config header
*              [IN/OUT] int sizeInDWords - size of pci config header
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpUpdateWirelessPciConfig (unsigned long pciID, unsigned long* pBase, int sizeInDWords)
{
    int nRet = BP_BOARD_ID_NOT_FOUND;
    int i = 0;
    int j = 0;

    if(pciID == 0 || pBase == 0 || sizeInDWords <= 0 )
        return nRet;

    i = 0;
    while ( wlanPciInfo[i].szboardId[0] != 0 ) {
        /* check boardId */
        if ( !bpstrcmp(g_pCurrentBp[0].u.cp, wlanPciInfo[i].szboardId) ) {
            /* check pciId */
            if ( (wlanPciInfo[i].usWirelessPciId == pciID) && (wlanPciInfo[i].usNeededSize <= sizeInDWords) ){
                /* valid , patch entry */
                while ( wlanPciInfo[i].entries[j].name[0] ) {
                    pBase[wlanPciInfo[i].entries[j].dwordOffset] = wlanPciInfo[i].entries[j].value;
                    j++;
                }
                nRet = BP_SUCCESS;
                goto pciconfig_update_done;
            }
        }
        i++;
    }

pciconfig_update_done:

    return( nRet );

}

/**************************************************************************
* Name       : BpGetWanDataLedGpio
*
* Description: This function returns the GPIO pin assignment for the WAN Data
*              LED.
*
* Parameters : [OUT] pusValue - Address of short word that the WAN Data LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWanDataLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedWan);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedWanData, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetWanDataLedGpio */

/**************************************************************************
* Name       : BpGetWanErrorLedGpio
*
* Description: This function returns the GPIO pin assignment for the WAN
*              LED that is used when there is a WAN connection failure.
*
* Parameters : [OUT] pusValue - Address of short word that the WAN LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWanErrorLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedWanError, pusValue ) );
} /* BpGetWanErrorLedGpio */

/**************************************************************************
* Name       : BpGetBootloaderPowerOnLedGpio
*
* Description: This function returns the GPIO pin assignment for the power
*              on LED that is set by the bootloader.
*
* Parameters : [OUT] pusValue - Address of short word that the alarm LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/


int BpGetPowerFailLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedAdsl);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedBlPowerOn, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetBootloaderPowerOn */
/**************************************************************************
* Name       : BpGetBootloaderPowerOnLedGpio
*
* Description: This function returns the GPIO pin assignment for the power
*              on LED that is set by the bootloader.
*
* Parameters : [OUT] pusValue - Address of short word that the alarm LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetBootloaderPowerOnLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedBlPower);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedBlPowerOn, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetBootloaderPowerOnLedGpio */

int BpGetWpsLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedWps);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedWps, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetWpsLedGpio */

int BpGet3GLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLed3G, pusValue ) );
} /* BpGet3GLedGpio */

int BpGet3GFailLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLed3GFail, pusValue ) );
} /* BpGet3GFailLedGpio */

/**************************************************************************
* Name       : BpGetBootloaderRedPowerOnLedGpio
*
* Description: This function returns the GPIO pin assignment for the red power
*              on LED that is set by the bootloader.
*
* Parameters : [OUT] pusValue - Address of short word that the alarm LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetBootloaderRedPowerOnLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedBlRedPowerOn, pusValue ) );
} /* BpGetBootloaderRedPowerOnLedGpio */


/**************************************************************************
* Name       : BpGetBootloaderStopLedGpio
*
* Description: This function returns the GPIO pin assignment for the break
*              into bootloader LED that is set by the bootloader.
*
* Parameters : [OUT] pusValue - Address of short word that the break into
*                  bootloader LED GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetBootloaderStopLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedBlReStop);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedBlStop, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetBootloaderStopLedGpio */

/**************************************************************************
* Name       : BpGetVoipLedGpio
*
* Description: This function returns the GPIO pin assignment for the VOIP
*              LED.
*
* Parameters : [OUT] pusValue - Address of short word that the VOIP LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
*
* Note       : The VoIP structure would allow for having one LED per DSP
*              however, the board initialization function assumes only one
*              LED per functionality (ie one LED for VoIP).  Therefore in
*              order to keep this tidy and simple we do not make usage of the
*              one-LED-per-DSP function.  Instead, we assume that the LED for
*              VoIP is unique and associated with DSP 0 (always present on
*              any VoIP platform).  If changing this to a LED-per-DSP function
*              then one need to update the board initialization driver in
*              bcmdrivers\opensource\char\board\bcm963xx\impl1
***************************************************************************/
int BpGetVoipLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedVoip1);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedVoip, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetVoipLedGpio */

/**************************************************************************
* Name       : BpGetVoip1LedGpio
*
* Description: This function returns the GPIO pin assignment for the VoIP1.
*              LED which is used when FXS0 is active
* Parameters : [OUT] pusValue - Address of short word that the VoIP1
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetVoip1LedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedVoip1);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedVoip1, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetVoip1LedGpio */

/**************************************************************************
* Name       : BpGetVoip1FailLedGpio
*
* Description: This function returns the GPIO pin assignment for the VoIP1
*              Fail LED which is used when there's an error with FXS0
* Parameters : [OUT] pusValue - Address of short word that the VoIP1
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetVoip1FailLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedVoip1);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedVoip1Fail, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetVoip1FailLedGpio */

/**************************************************************************
* Name       : BpGetVoip2LedGpio
*
* Description: This function returns the GPIO pin assignment for the VoIP2.
*              LED which is used when FXS1 is active
* Parameters : [OUT] pusValue - Address of short word that the VoIP2
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetVoip2LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedVoip2);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedVoip2, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetVoip2LedGpio */

/**************************************************************************
* Name       : BpGetVoip2FailLedGpio
*
* Description: This function returns the GPIO pin assignment for the VoIP2
*              Fail LED which is used when there's an error with FXS1
* Parameters : [OUT] pusValue - Address of short word that the VoIP2
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetVoip2FailLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedVoip2);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedVoip2Fail, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetVoip2FailLedGpio */

/**************************************************************************
* Name       : BpGetPotsLedGpio
*
* Description: This function returns the GPIO pin assignment for the POTS1.
*              LED which is used when DAA is active
* Parameters : [OUT] pusValue - Address of short word that the POTS11
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetPotsLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedPots);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedPots, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetPotsLedGpio */

/**************************************************************************
* Name       : BpGetDectLedGpio
*
* Description: This function returns the GPIO pin assignment for the DECT.
*              LED which is used when DECT is active
* Parameters : [OUT] pusValue - Address of short word that the DECT
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetDectLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedDect);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedDect, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetDectLedGpio */

/**************************************************************************
* Name       : BpGetFpgaResetGpio
*
* Description: This function returns the GPIO pin assignment for the FPGA
*              Reset signal.
*
* Parameters : [OUT] pusValue - Address of short word that the FPGA Reset
*                  signal GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetFpgaResetGpio( unsigned short *pusValue ) {
    return( BpGetUs(bp_usGpioFpgaReset, pusValue ) );
} /*BpGetFpgaResetGpio*/

/**************************************************************************
* Name       : BpGetGponLedGpio
*
* Description: This function returns the GPIO pin assignment for the GPON
*              LED.
*
* Parameters : [OUT] pusValue - Address of short word that the GPON LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetGponLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedGpon, pusValue ) );
} /* BpGetGponLedGpio */

/**************************************************************************
* Name       : BpGetGponFailLedGpio
*
* Description: This function returns the GPIO pin assignment for the GPON
*              LED that is used when there is a GPON connection failure.
*
* Parameters : [OUT] pusValue - Address of short word that the GPON LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetGponFailLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedGponFail, pusValue ) );
} /* BpGetGponFailLedGpio */

/**************************************************************************
* Name       : BpGetMoCALedGpio
*
* Description: This function returns the GPIO pin assignment for the MoCA
*              LED.
*
* Parameters : [OUT] pusValue - Address of short word that the MoCA LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetMoCALedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedMoCA, pusValue ) );
} /* BpGetMoCALedGpio */

/**************************************************************************
* Name       : BpGetMoCAFailLedGpio
*
* Description: This function returns the GPIO pin assignment for the MoCA
*              LED that is used when there is a MoCA connection failure.
*
* Parameters : [OUT] pusValue - Address of short word that the MoCA LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetMoCAFailLedGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLedMoCAFail, pusValue ) );
} /* BpGetMoCAFailLedGpio */

/**************************************************************************
* Name       : BpGetResetToDefaultExtIntr
*
* Description: This function returns the external interrupt number for the
*              reset to default button.
*
* Parameters : [OUT] pusValue - Address of short word that reset to default
*                  external interrupt is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetResetToDefaultExtIntr( unsigned short *pusValue )
{
    return( BpGetUs(bp_usExtIntrResetToDefault, pusValue ) );
} /* BpGetResetToDefaultExtIntr */

/**************************************************************************
* Name       : BpGetWirelessPowerDownGpio
*
* Description: This function returns the GPIO pin assignment for WLAN_PD
*
*
* Parameters : [OUT] pusValue - Address of short word that the WLAN_PD
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetWirelessPowerDownGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioWirelessPowerDown, pusValue ) );
} /* usGpioWirelessPowerDown */

/**************************************************************************
* Name       : BpGetDslPhyAfeIds
*
* Description: This function returns the DSL PHY AFE ids for primary and
*              secondary PHYs.
*
* Parameters : [OUT] pulValues-Address of an array of two long words where
*              AFE Id for the primary and secondary PHYs are returned.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET-Error, BpSetBoardId hasn't been called.
*              BP_VALUE_NOT_DEFINED - No defined AFE Ids.
**************************************************************************/
int BpGetDslPhyAfeIds( unsigned long *pulValues )
{
    int nRet;

    if( g_pCurrentBp )
    {
        if (BpGetUl(bp_ulAfeId0, &pulValues[0]) != BP_SUCCESS) {
          pulValues[0] = BP_AFE_DEFAULT;
        }
        if (BpGetUl(bp_ulAfeId1, &pulValues[1]) != BP_SUCCESS) {
          pulValues[1] = BP_AFE_DEFAULT;
        }
        nRet = BP_SUCCESS;
    }
    else
    {
        pulValues[0] = BP_AFE_DEFAULT;
        pulValues[1] = BP_AFE_DEFAULT;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetDslPhyAfeIds */

/**************************************************************************
* Name       : BpGetExtAFEResetGpio
*
* Description: This function returns the GPIO pin assignment for resetting the external AFE chip
*
*
* Parameters : [OUT] pusValue - Address of short word that the ExtAFEReset
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFEResetGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioExtAFEReset, pusValue ) );
} /* BpGetExtAFEResetGpio */

/**************************************************************************
* Name       : BpGetExtAFELDModeGpio
*
* Description: This function returns the GPIO pin assignment for setting LD Mode to ADSL/VDSL
*
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioExtAFELDMode
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFELDModeGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioExtAFELDMode, pusValue ) );
} /* BpGetExtAFELDModeGpio */

/**************************************************************************
* Name       : BpGetExtAFELDPwrGpio
*
* Description: This function returns the GPIO pin assignment for turning on/off the external AFE LD
*
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioExtAFELDPwr
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFELDPwrGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioExtAFELDPwr, pusValue ) );
} /* BpGetExtAFELDPwrGpio */


/**************************************************************************
* Name       : BpGet6829PortInfo
*
* Description: This function checks the ENET MAC info to see if a 6829
*              is connected
*
* Parameters : [OUT] portInfo6829 - 0 if 6829 is not present
*                                 - 6829 port information otherwise
*
* Returns    : BP_SUCCESS           - Success, value is returned.
*              BP_BOARD_ID_NOT_SET  - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGet6829PortInfo( unsigned char *portInfo6829 )
{
   ETHERNET_MAC_INFO enetMacInfo;
   ETHERNET_SW_INFO *pSwInfo;
   int               retVal;
   int               i;

   *portInfo6829 = 0;
   retVal = BpGetEthernetMacInfo( &enetMacInfo, 1 );
   if ( BP_SUCCESS == retVal ) {
      pSwInfo = &enetMacInfo.sw;
      for (i = 0; i < BP_MAX_SWITCH_PORTS; i++) {
         if ( ((pSwInfo->phy_id[i] & PHYID_LSBYTE_M) != 0xFF) &&
              ((pSwInfo->phy_id[i] & PHYID_LSBYTE_M) &  0x80) ) {
            *portInfo6829 = pSwInfo->phy_id[i] & PHYID_LSBYTE_M;
            retVal        = BP_SUCCESS;
            break;
         }
      }
   }

   return retVal;

}

/**************************************************************************
* Name       : BpGetFemtoResetGpio
*
* Description: This function returns the GPIO that needs to be toggled high
*              for 2 msec at least to reset the FEMTO chip
*
* Parameters : [OUT] pusValue - Address of short word that the GPIO for
*                  resetting FEMTO chip is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetFemtoResetGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioFemtoReset, pusValue ) );
} /*  BpGetFemtoResetGpio */


#if 0
/**************************************************************************
* Name       : BpGetEthSpdLedGpio
*
* Description: This function returns the GPIO pin assignment for the
*              specified port and link speed 
*
* Parameters : [IN] port - Internal phy number
*              [IN] enetIdx - index for Ethernet MAC info
*              [IN] ledIdx - 0 -> duplex GPIO
*                          - 1 -> spd 100 GPIO
*                          - 2 -> spd 1000 GPIO
*              [OUT] pusValue - Address of a short word to store the GPIO
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetEthSpdLedGpio( unsigned short port, unsigned short enetIdx,
                        unsigned short ledIdx, unsigned short *pusValue )
{
    ETHERNET_MAC_INFO enetMacInfos[BP_MAX_ENET_MACS];
    unsigned short *pShort;
    int nRet;

    if( g_pCurrentBp ) {
        nRet = BpGetEthernetMacInfo( enetMacInfos, BP_MAX_ENET_MACS );

        if ((enetIdx >= BP_MAX_ENET_MACS) ||
            (port >= BP_MAX_ENET_INTERNAL) ||
            (enetMacInfos[enetIdx].ucPhyType == BP_ENET_NO_PHY)) {
           *pusValue = BP_NOT_DEFINED;
           nRet = BP_VALUE_NOT_DEFINED;
        } else {
           pShort   = &enetMacInfos[enetIdx].sw.ledInfo[port].duplexLed;
           pShort   += ledIdx;
           *pusValue = *pShort;
           if( *pShort == BP_NOT_DEFINED ) {
               nRet = BP_VALUE_NOT_DEFINED;
           } else {
               nRet = BP_SUCCESS;
           }
        }
    } else {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetEthSpdLedGpio */
#endif

/**************************************************************************
* Name       : BpGetLaserDisGpio
*
* Description: This function returns the GPIO pin assignment for disabling
*              the laser
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioLaserDis
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetLaserDisGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLaserDis, pusValue ) );
} /* BpGetLaserDisGpio */


/**************************************************************************
* Name       : BpGetLaserTxPwrEnGpio
*
* Description: This function returns the GPIO pin assignment for enabling
*              the transmit power of the laser
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioLaserTxPwrEn
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetLaserTxPwrEnGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLaserTxPwrEn, pusValue ) );
} /* BpGetLaserTxPwrEnGpio */


/**************************************************************************
* Name       : BpGetVregSel1P2
*
* Description: This function returns the desired voltage level for 1V2
*
* Parameters : [OUT] pusValue - Address of short word that the 1V2 level
*                  is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetVregSel1P2( unsigned short *pusValue )
{
    return( BpGetUs(bp_usVregSel1P2, pusValue ) );
} /* BpGetVregSel1P2 */

int BpGetPowerLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedPower);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedPower, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetPowerLedGpio */  
int BpGetInternetLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedInternet);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedInternet, pusValue ) );
    }
    else
    {
        return BP_SUCCESS;
    }
} /* BpGetInternetLedGpio */
int BpGetLanLedGpio( unsigned short *pusValue )
{   
    int nRet;
    nRet =  GetLed( pusValue ,kLedLan1);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedLan, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetLanLedGpio */
int BpGetWanLedGpio( unsigned short *pusValue )
{
    return( BpGetUs( bp_usGpioLedWan, pusValue ) );
} /* BpGetWanLedGpio */
int  BpGetWirelessLedGpio( unsigned short *pusValue )
{    
    int nRet;
    nRet =  GetLed( pusValue ,kLedWlan);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedWireless, pusValue ) );
    }else
        return BP_SUCCESS;
} /* BpGetWlanLedGpio */


int BpGetInternetFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedInternet);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs(bp_usGpioLedInternetFail, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetInternetFailLedGpio */
int  BpGetUsbLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedUsb);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedUsb, pusValue ) );
    }
    else
        return BP_SUCCESS;
} /* BpGetUsbLedGpio */

#endif


#if 1

#if 0
/**************************************************************************
 * Name       : BpGetVoipDspConfig
 *
 * Description: Gets the DSP configuration from the board parameter
 *              structure for a given DSP index.
 *
 * Parameters : [IN] dspNum - DSP index (number)
 *
 * Returns    : Pointer to DSP configuration block if found/valid, NULL
 *              otherwise.
 ***************************************************************************/
VOIP_DSP_INFO *BpGetVoipDspConfig( unsigned char dspNum )
{
    VOIP_DSP_INFO *pDspConfig = 0;
    int i;

    if( g_pCurrentBp )
    {
        for( i = 0 ; i < BP_MAX_VOIP_DSP ; i++ )
        {
            if( g_pCurrentBp->VoIPDspInfo[i].ucDspType != BP_VOIP_NO_DSP &&
                g_pCurrentBp->VoIPDspInfo[i].ucDspAddress == dspNum )
            {
                 pDspConfig = &g_pCurrentBp->VoIPDspInfo[i];
                 break;
            }
        }
    }

    return pDspConfig;
}
#endif
#if 0
/**************************************************************************
 * Name       : BpSetBoardId
 *
 * Description: This function find the BOARD_PARAMETERS structure for the
 *              specified board id string and assigns it to a global, static
 *              variable.
 *
 * Parameters : [IN] pszBoardId - Board id string that is saved into NVRAM.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_FOUND - Error, board id input string does not
 *                  have a board parameters configuration record.
 ***************************************************************************/
int BpSetBoardId( char *pszBoardId )
{
    int nRet = BP_BOARD_ID_NOT_FOUND;
    PBOARD_PARAMETERS *ppBp;

    for( ppBp = g_BoardParms; *ppBp; ppBp++ )
    {
        if( !bpstrcmp((*ppBp)->szBoardId, pszBoardId) )
        {
            g_pCurrentBp = *ppBp;
            nRet = BP_SUCCESS;
            break;
        }
    }

    return( nRet );
} /* BpSetBoardId */

/**************************************************************************
 * Name       : BpGetBoardId
 *
 * Description: This function returns the current board id strings.
 *
 * Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
 *                  string is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 ***************************************************************************/

int BpGetBoardId( char *pszBoardId )
{
    int i;

    if (g_pCurrentBp == 0)
        return -1;

    for (i = 0; i < BP_BOARD_ID_LEN; i++)
      pszBoardId[i] = g_pCurrentBp->szBoardId[i];

    return 0;
}

/**************************************************************************
 * Name       : BpGetBoardIds
 *
 * Description: This function returns all of the supported board id strings.
 *
 * Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
 *                  strings are returned in.  Each id starts at BP_BOARD_ID_LEN
 *                  boundary.
 *              [IN] nBoardIdsSize - Number of BP_BOARD_ID_LEN elements that
 *                  were allocated in pszBoardIds.
 *
 * Returns    : Number of board id strings returned.
 ***************************************************************************/
int BpGetBoardIds( char *pszBoardIds, int nBoardIdsSize )
{
    PBOARD_PARAMETERS *ppBp;
    int i;
    char *src;
    char *dest;

    for( i = 0, ppBp = g_BoardParms; *ppBp && nBoardIdsSize;
        i++, ppBp++, nBoardIdsSize--, pszBoardIds += BP_BOARD_ID_LEN )
    {
        dest = pszBoardIds;
        src = (*ppBp)->szBoardId;
        while( *src )
            *dest++ = *src++;
        *dest = '\0';
    }

    return( i );
} /* BpGetBoardIds */


/**************************************************************************
 * Name       : BpGetGPIOverlays
 *
 * Description: This function GPIO overlay configuration
 *
 * Parameters : [OUT] pusValue - Address of short word that interfaces in use.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetGPIOverlays( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGPIOOverlay;

        if( g_pCurrentBp->usGPIOOverlay != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetGPIOverlays */

/**************************************************************************
 * Name       : BpGetEthernetMacInfo
 *
 * Description: This function returns all of the supported board id strings.
 *
 * Parameters : [OUT] pEnetInfos - Address of an array of ETHERNET_MAC_INFO
 *                  buffers.
 *              [IN] nNumEnetInfos - Number of ETHERNET_MAC_INFO elements that
 *                  are pointed to by pEnetInfos.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 ***************************************************************************/
int BpGetEthernetMacInfo( PETHERNET_MAC_INFO pEnetInfos, int nNumEnetInfos )
{
    int i, nRet;
    
    if( g_pCurrentBp )
    {
        for( i = 0; i < nNumEnetInfos; i++, pEnetInfos++ )
        {
            if( i < BP_MAX_ENET_MACS )
            {
                unsigned char *src = (unsigned char *)
                    &g_pCurrentBp->EnetMacInfos[i];
                unsigned char *dest = (unsigned char *) pEnetInfos;
                int len = sizeof(ETHERNET_MAC_INFO);
                while( len-- )
                    *dest++ = *src++;
            }
            else
                pEnetInfos->ucPhyType = BP_ENET_NO_PHY;
        }

        nRet = BP_SUCCESS;
    }
    else
    {
        for( i = 0; i < nNumEnetInfos; i++, pEnetInfos++ )
            pEnetInfos->ucPhyType = BP_ENET_NO_PHY;

        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetEthernetMacInfo */
#endif

/**************************************************************************
 * Name       : BpGetSdramSize
 *
 * Description: This function returns a constant that describees the board's
 *              SDRAM type and size.
 *
 * Parameters : [OUT] pulSdramSize - Address of short word that the SDRAM size
 *                  is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 ***************************************************************************/
int BpGetSdramSize( unsigned long *pulSdramSize )
{
    return (BpGetUl(bp_ulSdramSize, pulSdramSize));

    #if 0
    if( g_pCurrentBp )
    {
        *pulSdramSize = g_pCurrentBp->usSdramSize;
        nRet = BP_SUCCESS;
    }
    else
    {
        *pulSdramSize = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
    #endif
} /* BpGetSdramSize */


#if 0
/**************************************************************************
 * Name       : BpGetRj11InnerOuterPairGpios
 *
 * Description: This function returns the GPIO pin assignments for changing
 *              between the RJ11 inner pair and RJ11 outer pair.
 *
 * Parameters : [OUT] pusInner - Address of short word that the RJ11 inner pair
 *                  GPIO pin is returned in.
 *              [OUT] pusOuter - Address of short word that the RJ11 outer pair
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, values are returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetRj11InnerOuterPairGpios( unsigned short *pusInner,
    unsigned short *pusOuter )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusInner = g_pCurrentBp->usGpioRj11InnerPair;
        *pusOuter = g_pCurrentBp->usGpioRj11OuterPair;

        if( g_pCurrentBp->usGpioRj11InnerPair != BP_NOT_DEFINED &&
            g_pCurrentBp->usGpioRj11OuterPair != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusInner = *pusOuter = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetRj11InnerOuterPairGpios */
#endif


/**************************************************************************
 * Name       : BpGetPressAndHoldResetGpio
 *
 * Description: This function returns the GPIO pin assignment for the press
 *              and hold reset button.
 *
 * Parameters : [OUT] pusValue - Address of short word that the press and hold
 *                  reset button GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPressAndHoldResetGpio( unsigned short *pusValue )
{
    return (BpGetUs(bp_usGpioPressAndHoldReset, pusValue));
#if 0
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioPressAndHoldReset;

        if( g_pCurrentBp->usGpioPressAndHoldReset != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
    #endif
} /* BpGetPressAndHoldResetGpio */

#if 0
/**************************************************************************
 * Name       : BpGetUartRtsCtsGpios
 *
 * Description: This function returns the GPIO pin assignments for RTS and CTS
 *              UART signals.
 *
 * Parameters : [OUT] pusRts - Address of short word that the UART RTS GPIO
 *                  pin is returned in.
 *              [OUT] pusCts - Address of short word that the UART CTS GPIO
 *                  pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, values are returned.
 *              BP_BOARD_ID_NOT_SET - Error, board id input string does not
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetRtsCtsUartGpios( unsigned short *pusRts, unsigned short *pusCts )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusRts = g_pCurrentBp->usGpioUartRts;
        *pusCts = g_pCurrentBp->usGpioUartCts;

        if( g_pCurrentBp->usGpioUartRts != BP_NOT_DEFINED &&
            g_pCurrentBp->usGpioUartCts != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusRts = *pusCts = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetUartRtsCtsGpios */

/**************************************************************************
 * Name       : BpGetAdslLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the ADSL
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the ADSL LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetAdslLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedAdsl);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedAdsl;

        if( g_pCurrentBp->usGpioLedAdsl != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetAdslLedGpio */

/**************************************************************************
 * Name       : BpGetAdslFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the ADSL
 *              LED that is used when there is a DSL connection failure.
 *
 * Parameters : [OUT] pusValue - Address of short word that the ADSL LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetAdslFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedAdsl);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedAdslFail;

        if( g_pCurrentBp->usGpioLedAdslFail != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetAdslFailLedGpio */


/**************************************************************************
 * Name       : BpGetWirelessLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Wireless
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Wireless LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedWlan);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedWireless;

        if( g_pCurrentBp->usGpioLedWireless != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetWirelessLedGpio */
#endif

/* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add begin:*/
/**************************************************************************
 * Name       : BpGetWirelessFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the WirelessFail
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the WirelessFail LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessFailLedGpio( unsigned short *pusValue )
{
    int nRet = BP_VALUE_NOT_DEFINED;
    nRet =  GetfailLed( pusValue ,kLedWlan);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedWirelessFail, pusValue ) );

#if 0
    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedWirelessFail;

        if( g_pCurrentBp->usGpioLedWirelessFail!= BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
#endif
    }
    else
      return( nRet );
} /* BpGetWirelessLedGpio */
/* HUAWEI HGW s48571 2008年1月18日 Hardware Porting add end.*/

#if 0
/**************************************************************************
 * Name       : BpGetWirelessAntInUse
 *
 * Description: This function returns the antennas in use for wireless
 *
 * Parameters : [OUT] pusValue - Address of short word that the Wireless Antenna
 *                  is in use.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessAntInUse( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usAntInUseWireless;

        if( g_pCurrentBp->usAntInUseWireless != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );	
} /* BpGetWirelessAntInUse */
#endif

/**************************************************************************
 * Name       : BpGetWirelessSesBtnGpio
 *
 * Description: This function returns the GPIO pin assignment for the Wireless
 *              Ses Button.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Wireless LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessSesBtnGpio( unsigned short *pusValue )
{
    return( BpGetUs( bp_usGpioBtnWirelessSes, pusValue ) );

#if 0
    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioSesBtnWireless;

        if( g_pCurrentBp->usGpioSesBtnWireless != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
#endif
} /* BpGetWirelessSesBtnGpio */

#if 0
/**************************************************************************
 * Name       : BpGetWirelessSesExtIntr
 *
 * Description: This function returns the external interrupt number for the 
 *              Wireless Ses Button.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Wireless Ses
 *                  external interrup is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessSesExtIntr( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usExtIntrSesBtnWireless;

        if( g_pCurrentBp->usExtIntrSesBtnWireless != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );

} /* BpGetWirelessSesExtIntr */


/**************************************************************************
 * Name       : BpGetWirelessSesLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Wireless
 *              Ses Led.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Wireless Ses
 *                  Led GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessSesLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedsesWlan);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedSesWireless;

        if( g_pCurrentBp->usGpioLedSesWireless != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
	
} /* BpGetWirelessSesLedGpio */

/**************************************************************************
 * Name       : BpGetUsbLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the USB
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the USB LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetUsbLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedUsb);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedUsb;

        if( g_pCurrentBp->usGpioLedUsb != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetUsbLedGpio */
#endif

/**************************************************************************
 * Name       : BpGetHpnaLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the HPNA
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the HPNA LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetHpnaLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedHpna);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedHpna, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedHpna;

            if( g_pCurrentBp->usGpioLedHpna != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    return( nRet );
} /* BpGetHpnaLedGpio */

 /**************************************************************************
 * 函数名  :   BpGetWanDataFailLedGpio
 * 功能    :   获取为WAN fail data LED所安排的GPIO号
 *
 * 输入参数:   无           
 * 输出参数:   pusValue: 表示WAN fail data LED的16位地址值
 *
 * 返回值  :    BP_SUCCESS           :  成功 
 *              BP_VALUE_NOT_DEFINED : 不支持
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在 
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/
int BpGetWanDataFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedWan);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedWanDataFail, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedWanDataFail;

            if( g_pCurrentBp->usGpioLedWanDataFail != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    return( nRet );
} /* BpGetWanDataFailLedGpio */

#if 0
/**************************************************************************
 * Name       : BpGetWanDataLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the WAN Data
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the WAN Data LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWanDataLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedWan);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedWanData;

        if( g_pCurrentBp->usGpioLedWanData != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetWanDataLedGpio */
#endif

/**************************************************************************
 * Name       : BpGetHspaLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the HSPA
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the HSPA LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetHspaLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedHspa);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedHspa, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedHspa;

            if( g_pCurrentBp->usGpioLedHspa != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    return( nRet );
} /* BpGetHspaLedGpio */

/**************************************************************************
 * Name       : BpGetHspaFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the HSPA
 *              LED that is used when there is a HSPA connection failure.
 *
 * Parameters : [OUT] pusValue - Address of short word that the HSPA LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetHspaFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedHspa);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedHspaFail, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedHspaFail;

            if( g_pCurrentBp->usGpioLedHspaFail != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
        }
    return( nRet );
} /* BpGetHspaFailLedGpio */

#if 0
/**************************************************************************
 * Name       : BpGetInternetLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Internet
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Internet LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetInternetLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedInternet);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedInternet;

        if( g_pCurrentBp->usGpioLedInternet != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetPppLedGpio */


/**************************************************************************
 * Name       : BpGetInternetFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Internet
 *              LED that is used when there is a PPP connection failure.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Internet LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetInternetFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedInternet);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedInternetFail;

        if( g_pCurrentBp->usGpioLedInternetFail != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetPppFailLedGpio */

/**************************************************************************
* Name       : BpUpdateWirelessPciConfig
*
* Description: This function patch wireless PCI Config Header
*              This is not functional critial/necessary but for dvt database maintenance
*
* Parameters : [IN] unsigned int pciID
*              [IN/OUT] unsigned int* pBase - base of pci config header
*              [IN/OUT] int sizeInDWords - size of pci config header
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpUpdateWirelessPciConfig (unsigned long pciID, unsigned long* pBase, int sizeInDWords)
{

}
#endif

/**************************************************************************
 * Name       : BpGetPppLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the PPP
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the PPP LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPppLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedPpp);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedPpp, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedPpp;

            if( g_pCurrentBp->usGpioLedPpp != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    return( nRet );
} /* BpGetPppLedGpio */

/**************************************************************************
 * Name       : BpGetPppFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the PPP
 *              LED that is used when there is a PPP connection failure.
 *
 * Parameters : [OUT] pusValue - Address of short word that the PPP LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPppFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedPpp);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedPppFail, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedPppFail;

            if( g_pCurrentBp->usGpioLedPppFail != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    return( nRet );
} /* BpGetPppFailLedGpio */

#if 0
/**************************************************************************
 * Name       : BpGetBootloaderPowerOnLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the power
 *              on LED that is set by the bootloader.
 *
 * Parameters : [OUT] pusValue - Address of short word that the alarm LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetBootloaderPowerOnLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedBlPower);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedBlPowerOn;

        if( g_pCurrentBp->usGpioLedBlPowerOn != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetBootloaderPowerOn */
#endif

/**************************************************************************
 * Name       : BpGetPowerRedFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the power
 *              on LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the alarm LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPowerRedFailLedGpio( unsigned short *pusValue )
{
    return BP_GPIO_24_AL;
}

#if 0
/**************************************************************************
 * Name       : BpGetBootloaderStopLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the break
 *              into bootloader LED that is set by the bootloader.
 *
 * Parameters : [OUT] pusValue - Address of short word that the break into
 *                  bootloader LED GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetBootloaderStopLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedBlReStop);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedBlStop;

        if( g_pCurrentBp->usGpioLedBlStop != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetBootloaderStopLedGpio */


/**************************************************************************
 * Name       : BpGetVoipLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the VOIP
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the VOIP LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 *
 * Note       : The VoIP structure would allow for having one LED per DSP
 *              however, the board initialization function assumes only one
 *              LED per functionality (ie one LED for VoIP).  Therefore in
 *              order to keep this tidy and simple we do not make usage of the
 *              one-LED-per-DSP function.  Instead, we assume that the LED for
 *              VoIP is unique and associated with DSP 0 (always present on
 *              any VoIP platform).  If changing this to a LED-per-DSP function
 *              then one need to update the board initialization driver in
 *              bcmdrivers\opensource\char\board\bcm963xx\impl1
 ***************************************************************************/
int BpGetVoipLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedVoip1);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = BpGetVoipDspConfig( 0 );

        if( pDspInfo )
        {
           *pusValue = pDspInfo->usGpioLedVoip;

           if( *pusValue != BP_NOT_DEFINED )
           {
              nRet = BP_SUCCESS;
           }
           else
           {
              nRet = BP_VALUE_NOT_DEFINED;
           }
        }
        else
        {
           *pusValue = BP_NOT_DEFINED;
           nRet = BP_BOARD_ID_NOT_FOUND;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetVoipLedGpio */

/**************************************************************************
 * Name       : BpGetVoip1LedGpio
 *
 * Description: This function returns the GPIO pin assignment for the VoIP1.
 *              LED which is used when FXS0 is active
 * Parameters : [OUT] pusValue - Address of short word that the VoIP1
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetVoip1LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedVoip1);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = &g_pCurrentBp->VoIPDspInfo[0];

        if( pDspInfo->ucDspType != BP_VOIP_NO_DSP)
        {
            *pusValue = pDspInfo->usGpioVoip1Led;

            if( pDspInfo->usGpioVoip1Led != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetVoip1LedGpio */

/**************************************************************************
 * Name       : BpGetVoip1FailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the VoIP1
 *              Fail LED which is used when there's an error with FXS0
 * Parameters : [OUT] pusValue - Address of short word that the VoIP1
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetVoip1FailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedVoip1);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = &g_pCurrentBp->VoIPDspInfo[0];

        if( pDspInfo->ucDspType != BP_VOIP_NO_DSP)
        {
            *pusValue = pDspInfo->usGpioVoip1LedFail;

            if( pDspInfo->usGpioVoip1LedFail != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetVoip1FailLedGpio */


/**************************************************************************
 * Name       : BpGetVoip2LedGpio
 *
 * Description: This function returns the GPIO pin assignment for the VoIP2.
 *              LED which is used when FXS1 is active
 * Parameters : [OUT] pusValue - Address of short word that the VoIP2
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetVoip2LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedVoip2);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = &g_pCurrentBp->VoIPDspInfo[0];

        if( pDspInfo->ucDspType != BP_VOIP_NO_DSP)
        {
            *pusValue = pDspInfo->usGpioVoip2Led;

            if( pDspInfo->usGpioVoip2Led != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetVoip2LedGpio */

/**************************************************************************
 * Name       : BpGetVoip2FailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the VoIP2
 *              Fail LED which is used when there's an error with FXS1
 * Parameters : [OUT] pusValue - Address of short word that the VoIP2
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetVoip2FailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedVoip2);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = &g_pCurrentBp->VoIPDspInfo[0];

        if( pDspInfo->ucDspType != BP_VOIP_NO_DSP)
        {
            *pusValue = pDspInfo->usGpioVoip2LedFail;

            if( pDspInfo->usGpioVoip2LedFail != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetVoip2FailLedGpio */

/**************************************************************************
 * Name       : BpGetPotsLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the POTS1.
 *              LED which is used when DAA is active
 * Parameters : [OUT] pusValue - Address of short word that the POTS11
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPotsLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedPots);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = &g_pCurrentBp->VoIPDspInfo[0];

        if( pDspInfo->ucDspType != BP_VOIP_NO_DSP)
        {
            *pusValue = pDspInfo->usGpioPotsLed;

            if( pDspInfo->usGpioPotsLed != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetPotsLedGpio */

/**************************************************************************
 * Name       : BpGetDectLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the DECT.
 *              LED which is used when DECT is active
 * Parameters : [OUT] pusValue - Address of short word that the DECT
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetDectLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedDect);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        VOIP_DSP_INFO *pDspInfo = &g_pCurrentBp->VoIPDspInfo[0];

        if( pDspInfo->ucDspType != BP_VOIP_NO_DSP)
        {
            *pusValue = pDspInfo->usGpioDectLed;

            if( pDspInfo->usGpioDectLed != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetDectLedGpio */
#endif

/**************************************************************************
 * Name       : BpGetAdslDyingGaspExtIntr
 *
 * Description: This function returns the ADSL Dying Gasp external interrupt
 *              number.
 *
 * Parameters : [OUT] pulValue - Address of short word that the ADSL Dying Gasp
 *                  external interrupt number is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetAdslDyingGaspExtIntr( unsigned long *pulValue )
{
    return( BpGetUl( bp_ulExtIntrAdslDyingGasp, pulValue ) );
#if 0
    int nRet;

    if( g_pCurrentBp )
    {
        *pulValue = g_pCurrentBp->usExtIntrAdslDyingGasp;

        if( g_pCurrentBp->usExtIntrAdslDyingGasp != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pulValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
#endif
} /* BpGetAdslDyingGaspExtIntr */


/**************************************************************************
 * Name       : BpGetHpnaExtIntr
 *
 * Description: This function returns the HPNA external interrupt number.
 *
 * Parameters : [OUT] pulValue - Address of short word that the HPNA
 *                  external interrupt number is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetHpnaExtIntr( unsigned long *pulValue )
{
    return( BpGetUl( bp_ulExtIntrHpna, pulValue ) );
#if 0
    int nRet;

    if( g_pCurrentBp )
    {
        *pulValue = g_pCurrentBp->usExtIntrHpna;

        if( g_pCurrentBp->usExtIntrHpna != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pulValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
#endif
} /* BpGetHpnaExtIntr */

/*jsut for test*/

#if 0
#if defined(CONFIG_BCM963268) && (CONFIG_BCM_EXT_SWITCH)
int BpGetPortConnectedToExtSwitch(void)
{
#if 0
    unsigned long phy_id, port_map;
    int i, extSwConnPort = -1;

    BpGetUl(bp_ulPortMap, &port_map);

    for (i = 0; i < BP_MAX_SWITCH_PORTS; i++) {
        if ( port_map &  (1 << i)) {
            BpGetUl(bp_ulPhyId0 + i, &phy_id);
            if (phy_id & EXTSW_CONNECTED)
            {
                extSwConnPort = i;
            }
        }
    }

return extSwConnPort;
#else
    return 4;/*hg257's external switch sit is 4*/
#endif    
}
#endif
#endif
/**************************************************************************
 * Name       : BpGetHpnaChipSelect
 *
 * Description: This function returns the HPNA chip select number.
 *
 * Parameters : [OUT] pulValue - Address of short word that the HPNA
 *                  chip select number is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetHpnaChipSelect( unsigned long *pulValue )
{
    return( BpGetUl( bp_ulHpnaChipSelect, pulValue ) );
#if 0
    int nRet;

    if( g_pCurrentBp )
    {
        *pulValue = g_pCurrentBp->usCsHpna;

        if( g_pCurrentBp->usCsHpna != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pulValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
#endif
} /* BpGetHpnaChipSelect */

#if 0
/**************************************************************************
 * Name       : BpGetResetToDefaultExtIntr
 *
 * Description: This function returns the external interrupt number for the
 *              reset to default button.
 *
 * Parameters : [OUT] pusValue - Address of short word that reset to default
 *                  external interrupt is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetResetToDefaultExtIntr( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usExtIntrResetToDefault;

        if( g_pCurrentBp->usExtIntrResetToDefault != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetCMTThread */
#endif

/**************************************************************************
 * Name       : BpGetVcopeGpio 
 *
 * Description: This function returns the GPIO number of VCOPE board
 *
 * Parameters : [OUT] - GPIO index (name)
 *
 * Returns    : pin number      - Success
 *              BP_NOT_DEFINED  - any kind of Error
 ***************************************************************************/
int BpGetVcopeGpio (int pio_idx)
{
    unsigned short nRet = BP_SUCCESS;

    if( pio_idx == BCM6505_RESET_GPIO )
        //nRet = g_pCurrentBp->usVcopeResetGpio;
        return( BpGetUs( bp_usVcopeResetGpio, &nRet ) );
    else if( pio_idx == VCOPE_RELAY_GPIO)
        //nRet = g_pCurrentBp->usVcopeRelayGpio;
        return( BpGetUs( bp_usVcopeRelayGpio, &nRet ) );
    else if( pio_idx == HPI_CS )
        //nRet = g_pCurrentBp->usCsHpi;
        return( BpGetUs( bp_usCsHpi, &nRet ) );
    else if( pio_idx == VCOPE_BOARD_REV )
        //nRet = g_pCurrentBp->usVcopeBoardRev;
        return( BpGetUs( bp_usVcopeBoardRev, &nRet ) );
    else
        nRet = BP_NOT_DEFINED;

   return((int)nRet);
}


/**************************************************************************
 * 函数名  :   BpGetPCBVesionGpio
 * 功能    :   通过给定参数ucPcbVerBit，得到表示位ucPcbVerBit的GPIO并
 *             存入到输出参数pusValue   
 * 输入参数:   ucPcbVerBit  :  表示PCB版本信息某位bit0－bit2。             
 * 输出参数:   pusValue     :  存放GPIO              
 *
 * 返回值  :    BP_SUCCESS:  成功
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/

int BpGetPCBVesionGpio(unsigned char ucPcbVerBit, unsigned short *pusValue )
{
    int nRet;

    if (!g_pCurrentBp)
    {        
        *pusValue = BP_NOT_DEFINED;
        return BP_BOARD_ID_NOT_SET;        
    }

    switch(ucPcbVerBit)
    {
    case PCB_VERSION_BIT0:
        //*pusValue = g_pCurrentBp->stPcbVersion.usPcbVer_Low; 
        return( BpGetUs( bp_usPcbVer_Low, pusValue ) );
        break;            
        
    case PCB_VERSION_BIT1:
        //*pusValue = g_pCurrentBp->stPcbVersion.usPcbVer_High; 
        return( BpGetUs( bp_usPcbVer_High, pusValue ) );
        break;            

    default:
        return BP_VALUE_NOT_DEFINED;
    }
  

    if( *pusValue != BP_NOT_DEFINED )
    {
        nRet = BP_SUCCESS;
    }
    else
    {
        nRet = BP_VALUE_NOT_DEFINED;
    }
 
    return( nRet );
} 

/**************************************************************************
 * 函数名  :   BpGetBoardVesionGpio
 * 功能    :   通过给定参数ucBoardVerBit，得到表示位ucBoardVerBit的GPIO并
 *             存入到输出参数pusValue   
 * 输入参数:   ucBoardVerBit:  表示Board版本信息某位bit0－bit2。             
 * 输出参数:   pusValue     :  存放GPIO              
 *
 * 返回值  :    BP_SUCCESS:  成功
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在 
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/
int BpGetBoardVesionGpio(unsigned char ucBoardVerBit, unsigned short *pusValue )
{
    int nRet;

    *pusValue = 0;
    if (!g_pCurrentBp)
    {        
        *pusValue = BP_NOT_DEFINED;
        return BP_BOARD_ID_NOT_SET;        
    }

    switch(ucBoardVerBit)
    {
    case BOARD_VERSION_BIT0:
        //*pusValue = g_pCurrentBp->stBoardVersion.usBoardVer_Low;        
        return( BpGetUs( bp_usBoardVer_Low, pusValue ) );
        break;            
        
    case BOARD_VERSION_BIT1:
        //*pusValue = g_pCurrentBp->stBoardVersion.usBoardVer_Mid;  
        return( BpGetUs( bp_usBoardVer_Mid, pusValue ) );
        break; 

    case BOARD_VERSION_BIT2:
        //*pusValue = g_pCurrentBp->stBoardVersion.usBoardVer_High;       
        return( BpGetUs( bp_usBoardVer_High, pusValue ) );
        break;          

    default:
        return BP_VALUE_NOT_DEFINED;
        
    }
  
    if( *pusValue != BP_NOT_DEFINED )
    {
        nRet = BP_SUCCESS;
    }
    else
    {
        nRet = BP_VALUE_NOT_DEFINED;
    }
      
    return( nRet );
} 


/**************************************************************************
 * 函数名  :   BpIsSupportSerialGpioMode
 * 功能    :   判断硬件是否支持serial GPIO模式
 *
 * 输入参数:   无           
 * 输出参数:   无
 *
 * 返回值  :    BP_SUCCESS           :  成功 
 *              BP_VALUE_NOT_DEFINED : 不支持
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在 
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/
int BpIsSupportSerialGpioMode(void)
{  
    short usHardwareCfg = 0;
    BpGetUs( bp_usHardwareCfg, &usHardwareCfg );
    
    if ((usHardwareCfg) & GPIO_SERIAL_MODE_MASK )
    {
        return BP_SUCCESS;            
    }
    else
    {
        return BP_VALUE_NOT_DEFINED;
    }

    return BP_BOARD_ID_NOT_SET;
}

/**************************************************************************
 * 函数名  :   BpIsSupportLedGpioMode
 * 功能    :   判断硬件是否支持LED GPIO模式
 *
 * 输入参数:   无           
 * 输出参数:   无
 *
 * 返回值  :    BP_SUCCESS           :  成功 
 *              BP_VALUE_NOT_DEFINED : 不支持
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在 
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/
int BpIsSupportLedGpioMode(void)
{    
    short usHardwareCfg = 0;
    BpGetUs( bp_usHardwareCfg, &usHardwareCfg );

    if ((usHardwareCfg) & GPIO_LED_MODE_MMASK )
    {
        return BP_SUCCESS;            
    }
    else
    {
        return BP_VALUE_NOT_DEFINED;
    }
    return BP_BOARD_ID_NOT_SET;    
}

/**************************************************************************
 * 函数名  :   BpIsSupportPcbVersion
 * 功能    :   判断硬件是否支持读GPIO获取PCB版本号
 *
 * 输入参数:   无           
 * 输出参数:   无
 *
 * 返回值  :    BP_SUCCESS           :  成功 
 *              BP_VALUE_NOT_DEFINED : 不支持
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在 
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/
int BpIsSupportPcbVersion(void)
{        
    short usHardwareCfg = 0;
    BpGetUs( bp_usHardwareCfg, &usHardwareCfg );

    if ((usHardwareCfg) & GPIO_GET_PCB_VER_MASK )
    {
        return BP_SUCCESS;            
    }
    else
    {
        return BP_VALUE_NOT_DEFINED;
    }

    return BP_BOARD_ID_NOT_SET;    
}

/**************************************************************************
 * 函数名  :   BpIsSupportBoardVersion
 * 功能    :   判断硬件是否支持读GPIO获取Board版本号
 *
 * 输入参数:   无           
 * 输出参数:   无
 *
 * 返回值  :    BP_SUCCESS           :  成功 
 *              BP_VALUE_NOT_DEFINED : 不支持
 *              BP_BOARD_ID_NOT_FOUND: 错误, 给定的board ID不存在 
 * 作者    :    yuyouqing42304
 * 修改历史:    2006-05-16创建  
 ***************************************************************************/
int BpIsSupportBoardVersion(void)
{    
    short usHardwareCfg = 0;
    BpGetUs( bp_usHardwareCfg, &usHardwareCfg );
    if ((usHardwareCfg) & GPIO_GET_BOARD_VER_MASK )
    {
        return BP_SUCCESS;            
    }
    else
    {
        return BP_VALUE_NOT_DEFINED;
    }
    
    return BP_BOARD_ID_NOT_SET;        
}

/**************************************************************************
 * Name       : BpGetSlicType
 *
 * Description: This function returns the type (manufacturer & model) of
 *              SLIC being used by the board.
 *
 * Parameters : [OUT] slicType - The slicType will be copied from the bp
 *              structure to this address. Please note that the value copied
 *              is an enum typecasted to a short. Hence, it should be cast
 *              back to an enum before being used.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetSlicType( unsigned short *slicType )
{    
    return(BpGetUs( bp_usSlicType, slicType ));
} /* BpGetSlicType */

/**************************************************************************
 * Name       : BpGetDAAType
 *
 * Description: This function returns the type (manufacturer & model) of
 *              DAA being used by the board.
 *
 * Parameters : [OUT] daaType - The slicType will be copied from the bp
 *              structure to this address. Please note that the value copied
 *              is an enum typecasted to a short. Hence, it should be cast
 *              back to an enum before being used.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetDAAType( unsigned short *daaType )
{
    return(BpGetUs( bp_usDaaType, daaType ));
} /* BpGetDAAType */
/*START ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/
#ifdef SUPPORT_ATP_GPIO_I2C
/**************************************************************************
* Name       : BpGetI2cResetGpio
*
* Description: This function returns the desired voltage level for 1V2
*
* Parameters : [OUT] pusValue - Address of short word that the 1V2 level
*                  is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetI2cResetGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioI2cResetPin, pusValue ) );
} /* BpGetI2cResetGpio */

/**************************************************************************
* Name       : BpGetI2cSclGpio
*
* Description: This function returns the desired voltage level for 1V2
*
* Parameters : [OUT] pusValue - Address of short word that the 1V2 level
*                  is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetI2cSclGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioI2cScl, pusValue ) );
} /* BpGetI2cSclGpio */

/**************************************************************************
* Name       : BpGetI2cSdaGpio
*
* Description: This function returns the desired voltage level for 1V2
*
* Parameters : [OUT] pusValue - Address of short word that the 1V2 level
*                  is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetI2cSdaGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioI2cSda, pusValue ) );
} /* BpGetI2cSdaGpio */


/**************************************************************************
* Name       : BpGetI2cDeviceId
*
* Description: This function returns the desired voltage level for 1V2
*
* Parameters : [OUT] pusValue - Address of short word that the 1V2 level
*                  is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetI2cDeviceId( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioI2cDeviceId, pusValue ) );
} /* BpGetI2cDeviceId */

/**************************************************************************
* Name       : BpGetTouchBoardIntrNum
*
* Description: This function returns the desired voltage level for 1V2
*
* Parameters : [OUT] pusValue - Address of short word that the 1V2 level
*                  is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetTouchBoardIntrNum(unsigned short *pusValue)
{
    return( BpGetUs(bp_usTouchBoardIntrNum, pusValue ) );
} /* BpGetTouchBoardIntrNum */

#endif
/*END ADD: l00252599 FOR SUPPORT GPIO I2C 2013-09-05*/

#if 0
/**************************************************************************
 * Name       : BpGetWirelessPowerDownGpio
 *
 * Description: This function returns the GPIO pin assignment for WLAN_PD
 *
 *
 * Parameters : [OUT] pusValue - Address of short word that the WLAN_PD
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessPowerDownGpio( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioWirelessPowerDown;

        if( g_pCurrentBp->usGpioWirelessPowerDown != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* usGpioWirelessPowerDown */


/**************************************************************************
 * Name       : BpGetDslPhyAfeIds
 *
 * Description: This function returns the DSL PHY AFE ids for primary and
 *              secondary PHYs.
 *
 * Parameters : [OUT] pulValues-Address of an array of two long words where
 *              AFE Id for the primary and secondary PHYs are returned.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET-Error, BpSetBoardId hasn't been called.
 *              BP_VALUE_NOT_DEFINED - No defined AFE Ids.
 **************************************************************************/
int BpGetDslPhyAfeIds( unsigned long *pulValues )
{
    int nRet;

    if( g_pCurrentBp )
    {
        pulValues[0] = g_pCurrentBp->ulAfeIds[0];
        pulValues[1] = g_pCurrentBp->ulAfeIds[1];
        nRet = BP_SUCCESS;
    }
    else
    {
        pulValues[0] = BP_AFE_DEFAULT;
        pulValues[1] = BP_AFE_DEFAULT;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetDslPhyAfeIds */



/**************************************************************************
* Name       : BpGetExtAFEResetGpio
*
* Description: This function returns the GPIO pin assignment for resetting the external AFE chip
*
*
* Parameters : [OUT] pusValue - Address of short word that the ExtAFEReset
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFEResetGpio( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioExtAFEReset;

        if( g_pCurrentBp->usGpioExtAFEReset != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetExtAFEResetGpio */


/**************************************************************************
* Name       : BpGetExtAFELDModeGpio
*
* Description: This function returns the GPIO pin assignment for setting LD Mode to ADSL/VDSL
*
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioExtAFELDMode
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFELDModeGpio( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioExtAFELDMode;

        if( g_pCurrentBp->usGpioExtAFELDMode != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetExtAFELDModeGpio */

/**************************************************************************
* Name       : BpGetExtAFELDPwrGpio
*
* Description: This function returns the GPIO pin assignment for turning on/off the external AFE LD
*
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioExtAFELDPwr
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFELDPwrGpio( unsigned short *pusValue )
{
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioExtAFELDPwr;

        if( g_pCurrentBp->usGpioExtAFELDPwr != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
} /* BpGetExtAFELDPwrGpio */
#endif

/**************************************************************************
 * Name       : BpGetPressWpsGpio
 *
 * Description: This function returns the GPIO pin assignment for the press
 *              WPS button.
 *
 * Parameters : [OUT] pusValue - Address of short word that the press WPS
                 button GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET  -  Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined for the board.
 ***************************************************************************/
int BpGetPressWpsGpio( unsigned short *pusValue )
{
    return( BpGetUs( bp_usGpioPressWps, pusValue ) );
#if 0
    int nRet;

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioPressWps;

        if( g_pCurrentBp->usGpioPressWps != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }

    return( nRet );
#endif
} /* BpGetPressWpsGpio */

#if 0
/**************************************************************************
 * Name       : BpGetAdslLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the power
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the power LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPowerLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedPower);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedPower;

        if( g_pCurrentBp->usGpioLedPower != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetAdslLedGpio */

/**************************************************************************
 * Name       : BpGetPowerFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the power fail
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the power fail LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetPowerFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedAdsl);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedPowerFail;

        if( g_pCurrentBp->usGpioLedPowerFail != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetPowerFailLedGpio */


/**************************************************************************
 * Name       : BpGetWpsLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the WPS
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the WPS LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWpsLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedWps);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedWPS;

        if( g_pCurrentBp->usGpioLedWPS != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetWpsLedGpio */
#endif

/**************************************************************************
 * Name       : BpGetUsbFailLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the USB
 *              Fail LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the USB Fail LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetUsbFailLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetfailLed( pusValue ,kLedUsb);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedUsbFail, pusValue ) );
#if 0
    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedUsbFail;

        if( g_pCurrentBp->usGpioLedUsbFail != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
#endif
    }
    return( nRet );
} /* BpGetUsbLedGpio */

#if 0
/**************************************************************************
 * Name       : BpGetLanLedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Lan
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Lan LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetLanLedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedLan1);
    if(BP_SUCCESS != nRet)
    {

    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedLan;

        if( g_pCurrentBp->usGpioLedLan != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
        }
    return( nRet );
} /* BpGetPppLedGpio */
#endif

/**************************************************************************
 * Name       : BpGetLan1LedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Lan1
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Lan1 LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetLan1LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedLan2);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedLan1, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedLan1;

            if( g_pCurrentBp->usGpioLedLan1 != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    else
        return( nRet );
} /* BpGetPppLedGpio */

/**************************************************************************
 * Name       : BpGetLan2LedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Lan2
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Lan2 LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetLan2LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedLan3);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedLan2, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedLan2;

            if( g_pCurrentBp->usGpioLedLan2!= BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    else
        return( nRet );
} /* BpGetPppLedGpio */

/**************************************************************************
 * Name       : BpGetLan3LedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Lan3
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Lan3 LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetLan3LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedLan4);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedLan3, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedLan3;

            if( g_pCurrentBp->usGpioLedLan3 != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    else
        return( nRet );
} /* BpGetPppLedGpio */

/**************************************************************************
 * Name       : BpGetLan4LedGpio
 *
 * Description: This function returns the GPIO pin assignment for the Lan4
 *              LED.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Lan4 LED
 *                  GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetLan4LedGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedLan5);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedLan4, pusValue ) );
#if 0
        if( g_pCurrentBp )
        {
            *pusValue = g_pCurrentBp->usGpioLedLan4;

            if( g_pCurrentBp->usGpioLedLan4 != BP_NOT_DEFINED )
            {
                nRet = BP_SUCCESS;
            }
            else
            {
                nRet = BP_VALUE_NOT_DEFINED;
            }
        }
        else
        {
            *pusValue = BP_NOT_DEFINED;
            nRet = BP_BOARD_ID_NOT_SET;
        }
#endif
    }
    else
        return( nRet );
} /* BpGetPppLedGpio */

/**************************************************************************
 * Name       : BpGetWirelessLedEnableGpio
 *
 * Description: This function returns the GPIO pin assignment for the Wireless
 *              LED Enable.
 *
 * Parameters : [OUT] pusValue - Address of short word that the Wireless LED
 *              Enable GPIO pin is returned in.
 *
 * Returns    : BP_SUCCESS - Success, value is returned.
 *              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
 *              BP_VALUE_NOT_DEFINED - At least one return value is not defined
 *                  for the board.
 ***************************************************************************/
int BpGetWirelessLedEnableGpio( unsigned short *pusValue )
{
    int nRet;
    nRet =  GetLed( pusValue ,kLedWlanEable);
    if(BP_SUCCESS != nRet)
    {
        return( BpGetUs( bp_usGpioLedWirelessEnable, pusValue ) );
#if 0
    if( g_pCurrentBp )
    {
        *pusValue = g_pCurrentBp->usGpioLedWirelessEnable;

        if( g_pCurrentBp->usGpioLedWirelessEnable != BP_NOT_DEFINED )
        {
            nRet = BP_SUCCESS;
        }
        else
        {
            nRet = BP_VALUE_NOT_DEFINED;
        }
    }
    else
    {
        *pusValue = BP_NOT_DEFINED;
        nRet = BP_BOARD_ID_NOT_SET;
    }
#endif
    }
    else
        return( nRet );
} /* BpGetWirelessLedEnableGpio */

/**************************************************************************
* Name       : BpGetUart2SdoutGpio
*
* Description: This function returns the GPIO pin assignment for UART2 SDOUT
*
*
* Parameters : [OUT] pusValue - Address of short word that the bp_usGpioUart2Sdout
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetUart2SdoutGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioUart2Sdout, pusValue ) );
} /* BpGetUart2SdoutGpio */

/**************************************************************************
* Name       : BpGetUart2SdinGpio
*
* Description: This function returns the GPIO pin assignment for UART2 SDIN
*
*
* Parameters : [OUT] pusValue - Address of short word that the bp_usGpioUart2Sdin
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetUart2SdinGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioUart2Sdin, pusValue ) );
} /* BpGetUart2SdinGpio */

int BpGetPassDyingGaspGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioPassDyingGasp, pusValue ) );
} /* BpGetPassDyingGaspGpio */


/**************************************************************************
* Name       : BpGetAFELDRelayGpio
*
* Description: This function returns the GPIO pin assignment for switching LD relay
*
*
* Parameters : [OUT] pusValue - Address of short word that the bp_usGpioAFELDRelay
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetAFELDRelayGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioAFELDRelay, pusValue ) );
} /* BpGetAFELDRelayGpio */

/**************************************************************************
* Name       : BpGetIntAFELDModeGpio
*
* Description: This function returns the GPIO pin assignment for setting LD Mode to ADSL/VDSL
*                  for the internal path.
*
* Parameters : [OUT] pusValue - Address of short word that the bp_usGpioIntAFELDMode
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetIntAFELDModeGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioIntAFELDMode, pusValue ) );
} /* BpGetIntAFELDModeGpio */

/**************************************************************************
* Name       : BpGetIntAFELDPwrGpio
*
* Description: This function returns the GPIO pin assignment for turning on/off the internal AFE LD
*
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioExtAFELDPwr
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetIntAFELDPwrGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioIntAFELDPwr, pusValue ) );
} /* BpGetIntAFELDPwrGpio */

/**************************************************************************
* Name       : BpGetExtAFELDDataGpio
*
* Description: This function returns the GPIO pin assignment for sending config data to the external AFE LD
*
*
* Parameters : [OUT] pusValue - Address of short word that the bp_usGpioExtAFELDData
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetExtAFELDDataGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioExtAFELDData, pusValue ) );
} /* BpGetExtAFELDDataGpio */

/**************************************************************************
* Name       : BpGetExtAFELDClkGpio
*
* Description: This function returns the GPIO pin assignment for sending the clk to the external AFE LD
*
*
* Parameters : [OUT] pusValue - Address of short word that the bp_usGpioExtAFELDClk
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/

int BpGetExtAFELDClkGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioExtAFELDClk, pusValue ) );
} /* BpGetExtAFELDClkGpio */
/*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
/**************************************************************************
* Name       : BpGetDeviceOptions
*
* Description: This function returns the serial LED Mux selection.
*
* Parameters : [OUT] pulValue - Address of word that device options bitmap
                                is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetDeviceOptions( unsigned long *pulValue )
{
    return( BpGetUl(bp_ulDeviceOptions, pulValue ) );
} /* BpGetDeviceOptions */
int BpGetEphyBaseAddress( unsigned short *pusValue )
{
    return( BpGetUs(bp_usEphyBaseAddress, pusValue ) );
} /*  BpGetEphyBaseAddress */
int BpGetGphyBaseAddress( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGphyBaseAddress, pusValue ) );
} /*  BpGetGphyBaseAddress */
/*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
/**************************************************************************
* Name       : BpGetLaserTxPwrEnGpio
*
* Description: This function returns the GPIO pin assignment for enabling
*              the transmit power of the laser
*
* Parameters : [OUT] pusValue - Address of short word that the usGpioLaserTxPwrEn
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/

/**************************************************************************
* Name       : BpGetDectRestGpio   // ADD BY: l81004871 2012-10-15
*
* Description: This function returns the GPIO pin assignment for the DECT 
*              SWITCH GPIO.
*
* Parameters : [OUT] pusValue - Address of short word that the GPON LED
*                  GPIO pin is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - At least one return value is not defined
*                  for the board.
***************************************************************************/
int BpGetDectRestGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioDectRest, pusValue ) );
} /* BpGetDectRestGpio */

#ifdef SUPPORT_ATP_HYBRID
int BpGetLteRestGpio( unsigned short *pusValue )
{
    return( BpGetUs(bp_usGpioLteRest, pusValue ) );
} /* BpGetDectRestGpio */
#endif

#ifdef BROADCOM_RELEASE_VERSION_4_14_L02
/**************************************************************************
* Name       : BpGetVreg1P8
*
* Description: This function returns the desired state for 1P8 regulator
*
* Parameters : [OUT] pucValue - Address of unsigned char where to return
*                               whether 1P8 regulator is external or not
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_SET - Error, BpSetBoardId has not been called.
*              BP_VALUE_NOT_DEFINED - Not defined
***************************************************************************/
int BpGetVreg1P8( unsigned char *pucValue )
{
    return( BpGetUc(bp_ucVreg1P8, pucValue ) );
} /* BpGetVreg1P8 */
/**************************************************************************
/***************************************************************************
 * Function Name: kerSysGetExtIntInfo
 * Description  : return the external interrupt information which includes the
 *                trigger type, sharing enable.
 * Returns      : pointer to buf
 ***************************************************************************/
#if defined(CONFIG_BCM96368) || defined(CONFIG_BCM96816) || defined(CONFIG_BCM96818) || defined(CONFIG_BCM96838)
#define NUM_EXT_INT    (INTERRUPT_ID_EXTERNAL_5-INTERRUPT_ID_EXTERNAL_0+1)
#else
#define NUM_EXT_INT    (INTERRUPT_ID_EXTERNAL_3-INTERRUPT_ID_EXTERNAL_0+1)
#endif
static unsigned int extIntrInfo[NUM_EXT_INT];
unsigned int kerSysGetExtIntInfo(unsigned int irq)
{
	return extIntrInfo[irq-INTERRUPT_ID_EXTERNAL_0];
}



EXPORT_SYMBOL(BpGetGPIOverlays);
EXPORT_SYMBOL(BpGetPortConnectedToExtSwitch);
EXPORT_SYMBOL(kerSysGetExtIntInfo);
#endif
EXPORT_SYMBOL(BpGetDectRestGpio);
//EXPORT_SYMBOL(BpGetPressAndHoldResetGpio);
//EXPORT_SYMBOL(BpGetWirelessSesBtnGpio);
//EXPORT_SYMBOL(BpGetWirelessLedGpio);
EXPORT_SYMBOL(BpGetExtAFEResetGpio);
EXPORT_SYMBOL(BpGetExtAFELDPwrGpio);
EXPORT_SYMBOL(BpGetExtAFELDModeGpio);
EXPORT_SYMBOL(BpGetExtAFELDClkGpio);
EXPORT_SYMBOL(BpGetExtAFELDDataGpio);
EXPORT_SYMBOL(BpGetFemtoResetGpio);
/*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
EXPORT_SYMBOL(BpGetDeviceOptions);
EXPORT_SYMBOL(BpGetEphyBaseAddress);
EXPORT_SYMBOL(BpGetGphyBaseAddress);
/*Added by l00135113@20120720 for add GMAC of bcm4.12L06 version driver*/
EXPORT_SYMBOL(BpGetIntAFELDPwrGpio);
EXPORT_SYMBOL(BpGetAFELDRelayGpio);
EXPORT_SYMBOL(BpGetIntAFELDModeGpio);
//Luther debug for DSL
#endif

