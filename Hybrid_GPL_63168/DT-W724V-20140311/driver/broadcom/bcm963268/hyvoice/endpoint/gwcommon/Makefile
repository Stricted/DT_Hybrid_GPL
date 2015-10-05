#
# <:copyright-BRCM:2011:proprietary:standard
# 
#    Copyright (c) 2011 Broadcom Corporation
#    All Rights Reserved
# 
#  This program is the proprietary software of Broadcom Corporation and/or its
#  licensors, and may only be used, duplicated, modified or distributed pursuant
#  to the terms and conditions of a separate, written license agreement executed
#  between you and Broadcom (an "Authorized License").  Except as set forth in
#  an Authorized License, Broadcom grants no license (express or implied), right
#  to use, or waiver of any kind with respect to the Software, and Broadcom
#  expressly reserves all rights in and to the Software and all intellectual
#  property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
#  NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
#  BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
# 
#  Except as expressly set forth in the Authorized License,
# 
#  1. This program, including its structure, sequence and organization,
#     constitutes the valuable trade secrets of Broadcom, and you shall use
#     all reasonable efforts to protect the confidentiality thereof, and to
#     use this information only in connection with your use of Broadcom
#     integrated circuit products.
# 
#  2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#     AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
#     WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
#     RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
#     ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
#     FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
#     COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
#     TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
#     PERFORMANCE OF THE SOFTWARE.
# 
#  3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
#     ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
#     INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
#     WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
#     IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
#     OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
#     SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
#     SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
#     LIMITED REMEDY.
# :>

obj-m := gwcommon.o
gwcommon-objs := gwcommon-lib.a

ENDPT_LIBS_SUBDIR := eptlib

XCHANGE_BUILD_EXTRA_CFLAGS = $(addprefix -D ,$(BRCM_VOICE_GLOBAL_CFLAGS))
ifeq ($(strip $(BRCM_PROFILER_TOOL)),1)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_PROFILER_ENABLED)
endif
ifeq ($(strip $(BRCM_CHIP)),6368)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_6368)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),6362)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_6362)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),6816)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_6816)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),6818)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_6818)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),6328)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_6328)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),63268)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_63268)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),6828)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_6828)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_CHIP)),68500)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_68500)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , HAUS_OS_LINUXKERNEL)
endif
ifeq ($(strip $(BRCM_VOIP_TO_FXO)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_VOIP_TO_FXO)
endif

ifeq ($(strip $(BRCM_TPD_SUPPORT)),1)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , VRG_TPD_SUPPORT)
endif

ifeq ($(strip $(BRCM_G168_TEST)),1)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , G168_TEST)
endif

ifeq ($(strip $(BRCM_NTR_SUPPORT)),1)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , NTR_SUPPORT)
endif

ifeq ($(strip $(BRCM_G729_DSPTEST_APP)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , G729_DSPTEST_APP)
endif

ifneq ($(strip $(CONFIG_BCM_HOSTMIPS_PWRSAVE)),)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , PWRMNGT_ENABLED=1)
endif

XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_TRACK_IDLE_STATUS=1)

ifeq ($(strip $(BRCM_SMP_BUILD)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += BRCM_SMP_BUILD=1
endif

ifeq ($(CONFIG_BCM_ENDPOINT),m)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , MODULE)
endif

#
# Locale Selection
#
XCHANGE_BUILD_EXTRA_CFLAGS += $(LOCALEFLAGS_TYPE2)

#
# Channel configuration
#
XCHANGE_BUILD_EXTRA_CFLAGS += $(BRCM_DSP_CHAN_DEFINES_TYPE2)

#
# xchg_common libraries
#
XCHG_LIBS :=           \
assert                 \
bos                    \
mem                    \
crt                    \
sme                    \
str

XCHG_LIBS += blog

#
# voice_res_gw libraries
#
VRG_LIBS :=            \
bcm$(BRCM_CHIP)$(BRCM_DSP_HAL)$(BRCM_DSP_HAL_EXTENSION) \
casCtl                 \
classStm               \
codec                  \
endpt                  \
hdsp                   \
heartbeat              \
lhapi                  \
cmtdCtl


#
# xchg_drivers libraries
#
XDRV_LIBS :=           \
bcm63xx_intCtrl        \
bcm63xx_timer          \
casDriver              \
circBuf                \
dbgCom


#
# Basic libraries for 68500 load
#
ifeq ($(strip $(BRCM_CHIP)),68500)
XDRV_LIBS +=  apmSim               \
              slicSim              \
              dlbStub              \
              ledSim               \
              bcm68500_pcm         \
              bcm68500_ceva        \
              bcm63xx_silabs       \
              bcm63xx_zarlink      \
              bcm63xx_resetCallout \
              exceptionHdlrMips    \
              flowCtrl             \
              spinLockMips         \
              cacheMips            \
              cp0Mips              \
              globalInterruptMips
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

#
# Basic libraries for 6368 load
#
ifeq ($(strip $(BRCM_CHIP)),6368)
XDRV_LIBS +=  apmSim               \
              slicSim              \
              dlbStub              \
              bcm6368_pcm          \
              bcm6368_led          \
              bcm6368_relay        \
              bcm63xx_silabs       \
              bcm63xx_zarlink      \
              bcm63xx_resetCallout \
              exceptionHdlrMips    \
              flowCtrl             \
              spinLockMips         \
              bcm6368_handshake    \
              cacheMips            \
              cp0Mips              \
              globalInterruptMips
endif


#
# Basic libraries for 6362 load
#
ifeq ($(strip $(BRCM_CHIP)),6362)
XDRV_LIBS +=  apmSim               \
              slicSim              \
              dlbStub              \
              bcm6362_pcm          \
              bcm6362_led          \
              bcm6362_relay        \
              bcm63xx_silabs       \
              bcm63xx_zarlink      \
              bcm63xx_resetCallout \
              exceptionHdlrMips    \
              flowCtrl             \
              spinLockMips         \
              bcm6362_handshake    \
              cacheMips            \
              cp0Mips              \
              globalInterruptMips
endif

#
# Basic libraries for 63268 load
#
ifeq ($(strip $(BRCM_CHIP)),63268)
XDRV_LIBS +=  apmSim               \
              slicSim              \
              dlbStub              \
              bcm63268_pcm          \
              bcm63268_led          \
              bcm63268_relay        \
              bcm63xx_silabs       \
              bcm63xx_zarlink      \
              bcm63xx_resetCallout \
              exceptionHdlrMips    \
              flowCtrl             \
              spinLockMips         \
              bcm63268_handshake    \
              cacheMips            \
              cp0Mips              \
              globalInterruptMips
endif


#
# Basic libraries for 6828 load
#
ifeq ($(strip $(BRCM_CHIP)),6828)
XDRV_LIBS +=  bcm6828_apm           \
              bcm6828_pcm          \
              bcm6828_clock         \
              bcm6828_handshake     \
              bcm6828_slicl9530     \
              bcm6828_slicsi3239    \
              bcm6828_slicsi32392   \
              bcm63xx_silabs        \
              bcm63xx_zarlink       \
              bcm6828_tpdl9530      \
              bcm6828_tpdsi3239     \
              bcm6828_tpdsi3239     \
              dlbStub               \
              ledSim                \
              bcm63xx_resetCallout  \
              flowCtrl              \
              cacheMips             \
              cp0Mips               \
              exceptionHdlrMips     \
              globalInterruptMips   \
              spinLockMips
endif

#
# Basic libraries for 6328 load
#
ifeq ($(strip $(BRCM_CHIP)),6328)
XDRV_LIBS +=  apmSim               \
              slicSim              \
              dlbStub              \
              bcm6328_pcm          \
              bcm6328_led          \
              bcm6328_relay        \
              bcm63xx_silabs       \
              bcm63xx_zarlink      \
              bcm63xx_resetCallout \
              exceptionHdlrMips    \
              flowCtrl             \
              spinLockMips         \
              bcm6328_handshake    \
              cacheMips            \
              cp0Mips              \
              globalInterruptMips
endif

#
# Basic libraries for 6816 load
#
ifeq ($(strip $(BRCM_CHIP)),6816)
XDRV_LIBS +=  bcm6816_apm           \
              bcm6816_clock         \
              bcm6816_handshake     \
              bcm6816_slicl9530     \
              bcm6816_slicsi3239    \
              bcm63xx_silabs        \
              bcm63xx_zarlink       \
              bcm6816_tpdl9530      \
              bcm6816_tpdsi3239     \
              bcm6816_tpdsi3239     \
              dlbStub               \
              ledSim                \
              bcm63xx_resetCallout  \
              flowCtrl              \
              cacheMips             \
              cp0Mips               \
              exceptionHdlrMips     \
              globalInterruptMips   \
              spinLockMips
endif


ifeq ($(strip $(BRCM_6816)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , USE_CAS_DRIVER_HISTORY_LOG)
VRG_LIBS  := $(VRG_LIBS) tpdCtl
endif

#
# Basic libraries for 6818 load
#
ifeq ($(strip $(BRCM_CHIP)),6818)
XDRV_LIBS +=  bcm6818_apm           \
              bcm6818_pcm           \
              bcm6818_clock         \
              bcm6818_handshake     \
              bcm6818_slicl9530     \
              bcm6818_slicsi3239    \
              bcm6818_slicsi32392   \
              bcm63xx_silabs        \
              bcm63xx_zarlink       \
              bcm6818_tpdl9530      \
              bcm6818_tpdsi3239     \
              bcm6818_tpdsi3239     \
              dlbStub               \
              ledSim                \
              bcm63xx_resetCallout  \
              flowCtrl              \
              cacheMips             \
              cp0Mips               \
              exceptionHdlrMips     \
              globalInterruptMips   \
              spinLockMips
endif


ifeq ($(strip $(BRCM_6818)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , USE_CAS_DRIVER_HISTORY_LOG)
VRG_LIBS  := $(VRG_LIBS) tpdCtl
endif


ifeq ($(strip $(BRCM_6368_G729)),y)
XDRV_LIBS := $(XDRV_LIBS)
endif

ifeq ($(strip $(BRCM_6368_G729_FXO)),y)
XDRV_LIBS := $(XDRV_LIBS) bcm6368_daasi3050
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

ifeq ($(strip $(BRCM_6368_FXO_VE890)),y)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

ifeq ($(strip $(BRCM_6368_MBG)),y)
ifeq ($(BRCM_6368_LE88276_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6368_LE89116_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6368_LE89316_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6368_LE89116_89316_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6368_LE88506_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6368_SI32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6368_SI32176_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6368_SI32176_32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6368_SI3226_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
endif

ifeq ($(strip $(BRCM_6368_MVNGR)),y)
ifeq ($(BRCM_6368_LE88276_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6368_SI32176_32178_NOFXO_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6368_SI32176_32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
endif

ifeq ($(strip $(BRCM_6368_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

ifeq ($(strip $(BRCM_6362_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
ifeq ($(BRCM_NTR_SUPPORT),1)
XDRV_LIBS += bcm6362_ntr
endif
endif

ifeq ($(strip $(BRCM_63268_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
ifeq ($(BRCM_NTR_SUPPORT),1)
XDRV_LIBS += bcm63268_ntr
endif
endif

ifeq ($(strip $(BRCM_6818_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
ifeq ($(BRCM_NTR_SUPPORT),1)
XDRV_LIBS += bcm6818_ntr
endif
ifeq ($(BRCM_TPD_SUPPORT),1)
VRG_LIBS  := $(VRG_LIBS) tpdCtl
endif
endif

ifeq ($(strip $(BRCM_6828_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
ifeq ($(BRCM_NTR_SUPPORT),1)
XDRV_LIBS += bcm6828_ntr
endif
ifeq ($(BRCM_TPD_SUPPORT),1)
VRG_LIBS  := $(VRG_LIBS) tpdCtl
endif
endif

ifeq ($(strip $(BRCM_6816_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
ifeq ($(BRCM_TPD_SUPPORT),1)
VRG_LIBS  := $(VRG_LIBS) tpdCtl
endif
endif

ifeq ($(strip $(BRCM_6328_UNI)),y)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

ifeq ($(strip $(BRCM_6362_ADVNG)),y)
ifeq ($(BRCM_6362_NOSLIC_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_LE88276_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_LE88506_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_LE89116_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_LE89316_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6362_LE89116_89316_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6362_SI32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6362_SI32176_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_SI3226_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_SI32176_32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
endif

ifeq ($(strip $(BRCM_6362_ADVNGR)),y)
ifeq ($(BRCM_6362_LE88276_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6362_SI32176_32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6362_SI32176_32178_NOFXO_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
endif

ifeq ($(strip $(BRCM_VOICE_NONDIST)),y)

XDRV_LIBS += debugSim

XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_NONDIST_BUILD)

ifeq ($(strip $(BRCM_CHIP)),6362)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET)
endif

ifeq ($(strip $(BRCM_CHIP)),63268)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET)
endif

ifeq ($(strip $(BRCM_CHIP)),6818)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET)
endif

ifeq ($(strip $(BRCM_CHIP)),68500)
#XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET)
endif

ifeq ($(strip $(BRCM_CHIP)),6828)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET)
endif

XCHANGE_BUILD_EXTRA_CFLAGS += BRCM_NONDIST_BUILD=1

else

XDRV_LIBS += itpcMips             \
             exceptionHdlrCmtMips \
             ipcSharedMem         \
             debugCmtInterface

ifeq ($(strip $(BRCM_CHIP)),6368)
XDRV_LIBS +=   bcm6368_boot \
               bcm6368_handshake
endif

ifeq ($(strip $(BRCM_CHIP)),6328)
XDRV_LIBS +=   bcm6328_boot \
               bcm6328_handshake
endif
            
ifeq ($(strip $(BRCM_CHIP)),6362)
XDRV_LIBS +=   bcm6362_boot \
               bcm6362_handshake
endif

ifeq ($(strip $(BRCM_CHIP)),63268)
XDRV_LIBS +=   bcm63268_boot \
               bcm63268_handshake
endif

ifeq ($(strip $(BRCM_CHIP)),6816)
XDRV_LIBS +=   bcm6816_boot \
               bcm6816_handshake
endif

ifeq ($(strip $(BRCM_CHIP)),6818)
XDRV_LIBS +=   bcm6818_boot \
               bcm6818_handshake
endif

ifeq ($(strip $(BRCM_CHIP)),6828)
XDRV_LIBS +=   bcm6828_boot \
               bcm6828_handshake
endif     
        
endif

ifeq ($(BRCM_6362_DECTTEST_SUPPORT),1)
XDRV_LIBS += bcm6362_dectTest
endif

ifeq ($(BRCM_63268_DECTTEST_SUPPORT),1)
XDRV_LIBS += bcm63268_dectTest
endif

ifeq ($(BRCM_6828_DECTTEST_SUPPORT),1)
XDRV_LIBS += bcm6828_dectTest
endif

ifeq ($(strip $(BRCM_6328)),y) 
ifeq ($(BRCM_6328_LE88276_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6328_LE88506_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6328_LE89116_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6328_LE89316_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6328_LE89116_89316_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6328_SI32176_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
endif
ifeq ($(BRCM_6328_SI32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif
ifeq ($(BRCM_6328_SI32176_32178_PCM_MODE),1)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

ifeq ($(BRCM_NTR_SUPPORT),1)
XDRV_LIBS += bcm6328_ntr
endif
endif

ifeq ($(strip $(BRCM_6368_NTR)),y)
XDRV_LIBS := $(XDRV_LIBS) bcm6368_daasi3050
VRG_LIBS  := $(VRG_LIBS) pstnCtl
ifeq ($(BRCM_NTR_SUPPORT),1)
XDRV_LIBS += bcm6368_ntr
endif
endif

ifeq ($(strip $(BRCM_6368_G729_WB_DECT_SITEL)),y)
XDRV_LIBS := $(XDRV_LIBS)
VRG_LIBS  := $(VRG_LIBS) pstnCtl
endif

ifeq ($(strip $(BRCM_HAL_CAP_SUPPORT)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , BRCM_HAL_CAPTURE_TO_FILE)
endif

$(obj)/gwcommon-lib.a:
	cd $(XCHANGE_DIR)/dslx;                                  \
	$(XCHANGE_DIR)/dslx/build -a $(XCHANGE_BUILD_APP) -d $(XCHANGE_DSP_APP)$(XCHANGE_DSP_APP_EXTENSION) -t bosk $(XCHANGE_BUILD_EXTRA_CFLAGS) &&   \
	$(XCHANGE_DIR)/dslx/build -a $(XCHANGE_BUILD_APP) -d $(XCHANGE_DSP_APP)$(XCHANGE_DSP_APP_EXTENSION) -t xdrv $(XCHANGE_BUILD_EXTRA_CFLAGS) &&   \
	$(XCHANGE_DIR)/dslx/build -a $(XCHANGE_BUILD_APP) -d $(XCHANGE_DSP_APP)$(XCHANGE_DSP_APP_EXTENSION) -t vrg $(XCHANGE_BUILD_EXTRA_CFLAGS)

	mkdir -p $(CURDIR)/$(ENDPT_LIBS_SUBDIR)

	for i in $(XCHG_LIBS); do                                \
	cp -f $(XCHANGE_DIR)/dslx/lib/LinuxKernel/$(XCHANGE_BUILD_APP)/xchg/$$i.a $(CURDIR)/$(ENDPT_LIBS_SUBDIR)/$$i.lib;    \
	done;                                                    

	for i in $(VRG_LIBS); do                                 \
	cp -f $(XCHANGE_DIR)/dslx/lib/LinuxKernel/$(XCHANGE_BUILD_APP)/vrg/$$i.a $(CURDIR)/$(ENDPT_LIBS_SUBDIR)/$$i.lib;     \
	done;                                                    

	for i in $(XDRV_LIBS); do                                \
	cp -f $(XCHANGE_DIR)/dslx/lib/LinuxKernel/$(XCHANGE_BUILD_APP)/xdrv/$$i.a $(CURDIR)/$(ENDPT_LIBS_SUBDIR)/$$i.lib;     \
	done;

	cd $(CURDIR)/$(ENDPT_LIBS_SUBDIR);                       \
	for i in $(XCHG_LIBS); do                                \
	   $(AR) x $$i.lib;                                      \
	done;                                                    \
	for i in $(VRG_LIBS); do                                 \
	   $(AR) x $$i.lib;                                      \
	done;                                                    \
	for i in $(XDRV_LIBS); do                                \
	   $(AR) x $$i.lib;                                      \
	done;                                                    \

	$(LD)	-G	0 -r -s $(CURDIR)/$(ENDPT_LIBS_SUBDIR)/*.o -o $(obj)/gwcommon-lib.a
	rm	-f	$(CURDIR)/$(ENDPT_LIBS_SUBDIR)/*.o

clean:
	rm	-f	gwcommon.mod.c modules.order *.o *.a .*.cmd *.ko