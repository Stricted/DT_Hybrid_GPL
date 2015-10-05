#
#  Copyright 2006-2012, Broadcom Corporation
#
# <:label-BRCM:2012:proprietary:standard
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
# File: bcmdrivers/char/dspapp/impl1/Makefile
#
# Makefile for the DSP application driver.
#

obj-$(CONFIG_BCM_BCMDSP) += dspdd.o
dspdd-objs := dspdrv.o dspApp$(BRCM_CHIP).o

$(obj)/dspApp$(BRCM_CHIP).o:
	cd $(XCHANGE_DIR)/dslx;                                  \
	$(XCHANGE_DIR)/dslx/build -a $(XCHANGE_BUILD_APP) -d $(XCHANGE_DSP_APP)$(XCHANGE_DSP_APP_EXTENSION) -t dspdd
ifeq ($(strip $(BRCM_G729_DSPTEST_APP)),y)
	cp -f $(XCHANGE_DIR)/dslx/obj/LinuxKernel/$(XCHANGE_BUILD_APP)/dspApp*$(BRCM_CHIP)*.o $(src)/dspApp$(BRCM_CHIP).o;
else
	cp -f $(XCHANGE_DIR)/dslx/obj/LinuxKernel/$(XCHANGE_BUILD_APP)/dspApp*.o $(src)/dspApp$(BRCM_CHIP).o;
endif
	$(OBJCOPY) --prefix-symbol TP0 $(src)/dspApp$(BRCM_CHIP).o $(src)/dspApp$(BRCM_CHIP)New.o	
	$(OBJCOPY) --redefine-sym TP0itpcMailbox=itpcMailbox $(src)/dspApp$(BRCM_CHIP)New.o $(src)/dspApp$(BRCM_CHIP)New1.o
	$(OBJCOPY) --redefine-sym TP0halDspGetVersion=halDspGetVersion $(src)/dspApp$(BRCM_CHIP)New1.o $(src)/dspApp$(BRCM_CHIP).o

EXTRA_CFLAGS += -I. -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -fno-exceptions

-include $(TOPDIR)/Rules.make

ifndef GPLREL
clean:
	rm -f core *.o *.a *.s
endif
