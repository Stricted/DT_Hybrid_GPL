# Makefile for buildroot2
#
# Copyright (C) 1999-2005 by Erik Andersen <andersen@codepoet.org>
# Copyright (C) 2006-2011 by the Buildroot developers <buildroot@uclibc.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#

#--------------------------------------------------------------
# Just run 'make menuconfig', configure stuff, then run 'make'.
# You shouldn't need to mess with anything beyond this point...
#--------------------------------------------------------------

# Set and export the version string
export BR2_VERSION:=2011.08

# Set and export the build environment value about the local language, it will affect the host gcc warning and error symbol...  ----  j00196195 / y00183935 @ 20120430  ----
export LANG=UTF-8

# This top-level Makefile can *not* be executed in parallel
.NOTPARALLEL:

TOPMESSAGE = echo "$(TERM_BOLD)>>> $(1)$(TERM_RESET)"
ifneq ("$(ATP_CI)", "")
TOPTERM_BOLD :=
TOPTERM_RESET :=
else
TOPTERM_BOLD := $(shell tput smso)
TOPTERM_RESET := $(shell tput rmso)
endif

# absolute path
TOPDIR:=$(shell pwd)

SVNVERSION:=$(shell $(TOPDIR)/scripts/getsvnversion)

CONFIG_CONFIG_IN=Config.in
CONFIG=package/config
DATE:=$(shell date +%Y%m%d)

# Compute the full local version string so packages can use it as-is
# Need to export it, so it can be got from environment in children (eg. mconf)
export BR2_VERSION_FULL:=$(BR2_VERSION)$(shell $(TOPDIR)/scripts/setlocalversion)

noconfig_targets:=menuconfig nconfig gconfig xconfig config oldconfig randconfig \
	defconfig %_defconfig savedefconfig allyesconfig allnoconfig silentoldconfig release \
	randpackageconfig allyespackageconfig allnopackageconfig \
	source-check

RCS_FIND_IGNORE := \( -name SCCS -o -name BitKeeper -o -name .svn -o -name CVS -o -name .pc -o -name .hg -o -name .git \) -prune -o
# Strip quotes and then whitespaces
qstrip=$(strip $(subst ",,$(1)))
#"))

# Variables for use in Make constructs
comma:=,
empty:=
space:=$(empty) $(empty)

ifneq ("$(origin O)", "command line")
O:=$(TOPDIR)/output
CONFIG_DIR:=$(TOPDIR)
NEED_WRAPPER=
else
# other packages might also support Linux-style out of tree builds
# with the O=<dir> syntax (E.G. Busybox does). As make automatically
# forwards command line variable definitions those packages get very
# confused. Fix this by telling make to not do so
MAKEOVERRIDES =
# strangely enough O is still passed to submakes with MAKEOVERRIDES
# (with make 3.81 atleast), the only thing that changes is the output
# of the origin function (command line -> environment).
# Unfortunately some packages don't look at origin (E.G. uClibc 0.9.31+)
# To really make O go away, we have to override it.
override O:=$(O)
CONFIG_DIR:=$(O)
# we need to pass O= everywhere we call back into the toplevel makefile
EXTRAMAKEARGS = O=$(O)
NEED_WRAPPER=y
endif

ifndef CUSTOMER
  CUSTOMER=dt
endif

export CUSTOMER

ifndef VENDOR
  VENDOR=Hybrid
endif

CONFIG_DIR=$(TOPDIR)/configs/$(CUSTOMER)/$(VENDOR)

UNSTRIP_FILES="swapdev startbsp"

# Pull in the user's configuration file
ifeq ($(filter $(noconfig_targets),$(MAKECMDGOALS)),)
-include $(CONFIG_DIR)/.config
ifneq ("$(ATP_CI_TQE)", "")
BR2_CCACHE=
endif
endif


# Override BR2_DL_DIR if shell variable defined
ifneq ($(BUILDROOT_DL_DIR),)
BR2_DL_DIR:=$(BUILDROOT_DL_DIR)
endif

ifdef DBG
  ifeq ("$(origin DBG)", "command line")
    ATP_DEBUG=1
  endif
endif

ifdef REPORT
  ifeq ("$(origin REPORT)", "command line")
    ATP_REPORT=1
  endif
endif

# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands
ifdef V
  ifeq ("$(origin V)", "command line")
    KBUILD_VERBOSE=$(V)
  endif
endif
ifndef KBUILD_VERBOSE
  KBUILD_VERBOSE=0
endif

ifeq ($(KBUILD_VERBOSE),1)
  quiet=
  Q=
ifndef VERBOSE
  VERBOSE=1
endif
else
  quiet=quiet_
  Q=@
endif

# we want bash as shell
SHELL:=$(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	else if [ -x /bin/bash ]; then echo /bin/bash; \
	else echo sh; fi; fi)

# kconfig uses CONFIG_SHELL
CONFIG_SHELL:=$(SHELL)

export SHELL CONFIG_SHELL quiet Q KBUILD_VERBOSE VERBOSE

ifndef HOSTAR
HOSTAR:=ar
endif
ifndef HOSTAS
HOSTAS:=as
endif
ifndef HOSTCC
HOSTCC:=gcc
HOSTCC:=$(shell which $(HOSTCC) || type -p $(HOSTCC) || echo gcc)
endif
HOSTCC_NOCCACHE:=$(HOSTCC)
ifndef HOSTCXX
HOSTCXX:=g++
HOSTCXX:=$(shell which $(HOSTCXX) || type -p $(HOSTCXX) || echo g++)
endif
HOSTCXX_NOCCACHE:=$(HOSTCXX)
ifndef HOSTFC
HOSTFC:=gfortran
endif
ifndef HOSTCPP
HOSTCPP:=cpp
endif
ifndef HOSTLD
HOSTLD:=ld
endif
ifndef HOSTLN
HOSTLN:=ln
endif
ifndef HOSTNM
HOSTNM:=nm
endif
HOSTAR:=$(shell which $(HOSTAR) || type -p $(HOSTAR) || echo ar)
HOSTAS:=$(shell which $(HOSTAS) || type -p $(HOSTAS) || echo as)
HOSTFC:=$(shell which $(HOSTLD) || type -p $(HOSTLD) || echo || which g77 || type -p g77 || echo gfortran)
HOSTCPP:=$(shell which $(HOSTCPP) || type -p $(HOSTCPP) || echo cpp)
HOSTLD:=$(shell which $(HOSTLD) || type -p $(HOSTLD) || echo ld)
HOSTLN:=$(shell which $(HOSTLN) || type -p $(HOSTLN) || echo ln)
HOSTNM:=$(shell which $(HOSTNM) || type -p $(HOSTNM) || echo nm)

export HOSTAR HOSTAS HOSTCC HOSTCXX HOSTFC HOSTLD
export HOSTCC_NOCCACHE HOSTCXX_NOCCACHE

# bash prints the name of the directory on 'cd <dir>' if CDPATH is
# set, so unset it here to not cause problems. Notice that the export
# line doesn't affect the environment of $(shell ..) calls, so
# explictly throw away any output from 'cd' here.
export CDPATH:=
export BASE_DIR := $(shell mkdir -p $(O) && cd $(O) >/dev/null && pwd)
$(if $(BASE_DIR),, $(error output directory "$(O)" does not exist))

BUILD_DIR:=$(BASE_DIR)/build

export PATH:=$(PATH):$(TOPDIR)/output/host/usr/bin


ifeq ($(BR2_HAVE_DOT_CONFIG),y)

# cc-option
# Usage: cflags-y+=$(call cc-option, -march=winchip-c6, -march=i586)
# sets -march=winchip-c6 if supported else falls back to -march=i586
# without checking the latter.
cc-option=$(shell if $(TARGET_CC) $(TARGET_CFLAGS) $(1) -S -o /dev/null -xc /dev/null \
	> /dev/null 2>&1; then echo "$(1)"; else echo "$(2)"; fi ;)

#############################################################
#
# Hide troublesome environment variables from sub processes
#
#############################################################
unexport CROSS_COMPILE
unexport ARCH
unexport CC
unexport CXX
unexport CPP
unexport CFLAGS
unexport CXXFLAGS
unexport GREP_OPTIONS

GNU_HOST_NAME:=$(shell package/gnuconfig/config.guess)

	
#############################################################
#
# Setup the proper filename extensions for the host
#
##############################################################
ifneq ($(findstring linux,$(GNU_HOST_NAME)),)
HOST_EXEEXT:=
HOST_LIBEXT:=.a
HOST_SHREXT:=.so
endif
ifneq ($(findstring apple,$(GNU_HOST_NAME)),)
HOST_EXEEXT:=
HOST_LIBEXT:=.a
HOST_SHREXT:=.dylib
endif
ifneq ($(findstring cygwin,$(GNU_HOST_NAME)),)
HOST_EXEEXT:=.exe
HOST_LIBEXT:=.lib
HOST_SHREXT:=.dll
HOST_LOADLIBES=-lcurses -lintl
export HOST_LOADLIBES
endif
ifneq ($(findstring mingw,$(GNU_HOST_NAME)),)
HOST_EXEEXT:=.exe
HOST_LIBEXT:=.lib
HOST_SHREXT:=.dll
endif

##############################################################
#
# The list of stuff to build for the target toolchain
# along with the packages to build for the target.
#
##############################################################

ifeq ($(BR2_CCACHE),y)
BASE_TARGETS += host-ccache
endif

ifneq ($(ATP_REPORT),)
REPORT_TARGETS:=
endif

ifeq ($(BR2_TOOLCHAIN_BUILDROOT),y)
BASE_TARGETS += uclibc-configured host-binutils cross_compiler uclibc-target-utils kernel-headers
else
BASE_TARGETS += uclibc
endif
TARGETS:=
ATARGETS:=
CMSTARGETS:=
WEBTARGETS:=
CLITARGETS:=
HTMLTARGETS:=
KERNELTARGETS:=
CBBIDTARGETS:=
TESTTARGETS:=
FSTARGETS:=
KCONFIGTARGETS:=
GENKEYTARGETS:=
PETABLETARGETS:=
BRANCHATPCORETARGETS:=
BRANCHOPENSOURCETARGETS:=
BRANCHHUWEIPRIVATETARGETS:=
BRANCHUNDEFINETARGETS:=
BRANCHKERNELTARGETS:=

ICPCIAPPLICATIONTARGETS:=
ICPCIKERNELTARGETS:=
ICPCIMKPTARGETS:=

# silent mode requested?
QUIET:=$(if $(findstring s,$(MAKEFLAGS)),-q)

# Strip off the annoying quoting
ARCH:=$(call qstrip,$(BR2_ARCH))
ifeq ($(ARCH),xtensa)
ARCH:=$(ARCH)_$(call qstrip,$(BR2_xtensa_core_name))
endif

KERNEL_ARCH:=$(shell echo "$(ARCH)" | sed -e "s/-.*//" \
	-e s/i.86/i386/ -e s/sun4u/sparc64/ \
	-e s/arm.*/arm/ -e s/sa110/arm/ \
	-e s/bfin/blackfin/ \
	-e s/parisc64/parisc/ \
	-e s/powerpc64/powerpc/ \
	-e s/ppc.*/powerpc/ -e s/mips.*/mips/ \
	-e s/sh.*/sh/)

#hg532t编译报错/opt/trendchip/mips-linux-uclibc/usr/lib/libncurses.so.5: ELF file data encoding not little-endian
#所以暂时先用ws880产品宏规定下。
ifeq ($(BUILD_PRODUCT_NAME),"ws880")
export LD_LIBRARY_PATH:=$(call qstrip,$(BR2_TOOLCHAIN_EXTERNAL_PATH))/lib
endif

ZCAT:=$(call qstrip,$(BR2_ZCAT))
BZCAT:=$(call qstrip,$(BR2_BZCAT))
XZCAT:=$(call qstrip,$(BR2_XZCAT))
TAR_OPTIONS=$(call qstrip,$(BR2_TAR_OPTIONS)) -xf

GNU_TARGET_SUFFIX:=-$(call qstrip,$(BR2_GNU_TARGET_SUFFIX))

# packages compiled for the host go here
export HOST_DIR:=$(call qstrip,$(BR2_HOST_DIR))

# stamp (dependency) files go here
STAMP_DIR:=$(BASE_DIR)/stamps

BINARIES_DIR:=$(BASE_DIR)/images
export TARGET_DIR:=$(BASE_DIR)/target
TOOLCHAIN_DIR=$(BASE_DIR)/toolchain
TARGET_SKELETON=$(TOPDIR)/fs/skeleton
export STAGING_INC_DIR=$(BASE_DIR)/staging/usr/include
BR2_DEPENDS_DIR=$(BUILD_DIR)/buildroot-config
export PACKAGE_DIR=$(TOPDIR)/package
DEVICETABLE_DIR=$(BASE_DIR)/devicetable
HUAWEI_DRIVERS_DIR:=

ifeq ($(BR2_CCACHE),y)
CCACHE:=$(HOST_DIR)/usr/bin/ccache
CCACHE_CACHE_DIR=$(HOME)/.buildroot-ccache
HOSTCC  := $(CCACHE) $(HOSTCC)
HOSTCXX := $(CCACHE) $(HOSTCXX)
endif

include toolchain/Makefile.in
include package/Makefile.in

#############################################################
#
# You should probably leave this stuff alone unless you know
# what you are doing.
#
#############################################################

all: world

# We also need the various per-package makefiles, which also add
# each selected package to TARGETS if that package was selected
# in the .config file.
ifeq ($(BR2_TOOLCHAIN_BUILDROOT),y)
include toolchain/toolchain-buildroot.mk
else ifeq ($(BR2_TOOLCHAIN_EXTERNAL),y)
include toolchain/toolchain-external.mk
else ifeq ($(BR2_TOOLCHAIN_CTNG),y)
include toolchain/toolchain-crosstool-ng.mk
endif

ifeq ($(BROADCOM_CHIP),y)
export CHIP_NAME:=$(call qstrip,$(BRCM_CHIP_NAME))
export DRIVER_RELEASE_VERSION:=$(call qstrip,$(BROADCOM_RELEASE_VERSION))
export PRODUCT_NAME:=$(call qstrip,$(BRCM_CHIP_NAME))
export SUB_CHIP:=$(call qstrip,$(BRCM_CHIP_NAME))
export BRCM_CHIP:=$(call qstrip,$(BRCM_CHIP))
endif

ifeq ($(MTK_CHIP),y)
export CHIP_NAME:=$(call qstrip,$(MTK_CHIP_NAME))
export DRIVER_RELEASE_VERSION:=$(call qstrip,$(MTK_RELEASE_VERSION))
export PRODUCT_NAME:=$(call qstrip,$(MTK_CHIP_NAME))
export SUB_CHIP:=$(call qstrip,$(MTK_CHIP_NAME))
export MTK_CHIP:=$(call qstrip,$(MTK_CHIP))
endif

ifeq ($(QUALCOMM_CHIP),y)
export CHIP_NAME:=$(call qstrip,$(QUALCOMM_CHIP_NAME))
export DRIVER_RELEASE_VERSION:=$(call qstrip,$(QUALCOMM_RELEASE_VERSION))
export PRODUCT_NAME:=$(call qstrip,$(QUALCOMM_CHIP_NAME))
export SUB_CHIP:=$(call qstrip,$(QUALCOMM_CHIP_NAME))
export QUALCOMM_CHIP:=$(call qstrip,$(QUALCOMM_CHIP))
endif

ifeq ($(HISI_CHIP),y)
export CHIP_NAME:=$(call qstrip,$(HISI_CHIP_NAME))
endif

ifeq ($(BUILD_NANDFLASH),y)
export BUILD_NANDFLASH=y
endif

ifeq ($(strip $(BR2_ENDIAN)),"LITTLE")
BUILD_ENDIAN = l
else
BUILD_ENDIAN = b
endif

##export 
export CHIP_PRODUCT_NAME:=$(call qstrip,$(CHIP_PRODUCT_NAME))
export KERNEL_VERSION:=$(call qstrip,$(BR2_LINUX_KERNEL_VERSION))

##just instead of dir.mak
IMAGE_DIR               = $(TOPDIR)/output/image
OUTPUT_DIR         		= $(TOPDIR)/output
PROFILE_DIR         	= $(TOPDIR)/output
PACKAGE_DIR            	= $(TOPDIR)/package

##export
export LIB_ATP_DIR                    = $(TOPDIR)/output/staging/lib
export KERNEL_DIR                     = $(TOPDIR)/linux/$(KERNEL_VERSION)
export HOST_INCLUDE_DIR               = $(KERNEL_DIR)/include
export DRIVER_VENDOR_DIR              = $(TOPDIR)/driver/$(CHIP_PRODUCT_NAME)/$(CHIP_NAME)/$(DRIVER_RELEASE_VERSION)
export INC_DRIVER_VENDOR_DIR          = $(DRIVER_VENDOR_DIR)/include

export INSTALL_DIR         = $(TARGET_DIR)
export INC_ADSLDRV_PATH    = $(DRIVER_VENDOR_DIR)/adsl


## rootfs file name
ROOTFS_FILE_NAME := 

include package/*/*.mk

##/*Added by l00135113@20120225 for support 257*/


ifeq ($(BUILD_VOICE),y)
include scripts/make.voice
endif

include driver/package.mk
TARGETS+=$(ATARGETS)
TARGETS+=$(CMSTARGETS)
TARGETS+=$(WEBTARGETS)
TARGETS+=$(HTMLTARGETS)
TARGETS+=$(CLITARGETS)
TARGETS+=$(PETABLETARGETS)


ICPCIAPPLICATIONTARGETS+=$(TARGETS)
TARGETS=

include linux/linux.atp.mk
TARGETS+=$(KERNELTARGETS)

ICPCIKERNELTARGETS+=$(TARGETS)
TARGETS=


##PC版本不需要如下目标
ifneq ($(strip $(BR2_ARCH)),"i386")

include boot/common.mk
include target/Makefile.in

TARGETS+=target-finalize

ifeq ($(BR2_ENABLE_LOCALE_PURGE),y)
TARGETS+=target-purgelocales
endif

include fs/common.mk
ifndef GPLREL
include package/atp/hosttool/package/package.make.in
endif
TARGETS+=erase-fakeroots
endif ##ifneq ($(strip $(BR2_ARCH)),"i386")

ICPCIMKPTARGETS+=$(TARGETS)
TARGETS=

TARGETS=$(ICPCIAPPLICATIONTARGETS)$(ICPCIKERNELTARGETS)$(ICPCIMKPTARGETS)

TARGETS_KCONFIG_REMOVE:=$(patsubst %,%-clean,$(KCONFIGTARGETS))

TARGETS_CLEAN:=$(patsubst %,%-clean,$(TARGETS))
TARGETS_SOURCE:=$(patsubst %,%-source,$(TARGETS) $(BASE_TARGETS))
TARGETS_DIRCLEAN:=$(patsubst %,%-dirclean,$(TARGETS))
TARGETS_ALL:=$(patsubst %,__real_tgt_%,$(TARGETS))
TARGETS_CHANGELOG:=$(patsubst %,%-changelog,$(ATARGETS) $(CMSTARGETS)) $(CLITARGETS)
TARGETS_INSTALLHEADERS:=$(patsubst %,%-install-headers,$(ATARGETS) $(CMSTARGETS) $(WEBTARGETS) $(CLITARGETS) $(KERNELTARGETS))

TARGETS_ICPCI_APPLICATION:=$(patsubst %,__real_tgt_%,$(ICPCIAPPLICATIONTARGETS))
TARGETS_ICPCI_KERNEL:=$(patsubst %,__real_tgt_%,$(ICPCIKERNELTARGETS))
TARGETS_ICPCI_MKP:=$(patsubst %,__real_tgt_%,$(ICPCIMKPTARGETS))

TARGETS_TEST_RUN:=$(patsubst %,%-run,$(TESTTARGETS))

CMSTARGETS_LIBS:=$(patsubst %,-l%,$(CMSTARGETS))
WEBTARGETS_LIBS:=$(patsubst %,-l%,$(WEBTARGETS))
CLITARGETS_LIBS:=$(patsubst %,-l%,$(CLITARGETS))
CMSTARGETS_CLEAN:=$(patsubst %,%-clean,$(CMSTARGETS))
CLITARGETS_CLEAN:=$(patsubst %,%-clean,$(CLITARGETS))
TESTARGETS_CLEAN:=$(patsubst %,%-clean,$(TESTTARGETS))
DOCTARGETS:=$(patsubst %,%-doc,$(ATARGETS))
GENKEYTARGETS_CLEAN:=$(patsubst %,%-clean,$(GENKEYTARGETS))

TARGETS_REMOVEHEADERS+=$(patsubst %,%-remove-headers,$(ATARGETS) $(CMSTARGETS) $(CLITARGETS) $(WEBTARGETS) $(KERNELTARGETS))

# host-* dependencies have to be handled specially, as those aren't
# visible in Kconfig and hence not added to a variable like TARGETS.
# instead, find all the host-* targets listed in each <PKG>_DEPENDENCIES
# variable for each enabled target.
# Notice: this only works for newstyle gentargets/autotargets packages
TARGETS_HOST_DEPS = $(sort $(filter host-%,$(foreach dep,\
		$(addsuffix _DEPENDENCIES,$(call UPPERCASE,$(TARGETS))),\
		$($(dep)))))
# Host packages can in turn have their own dependencies. Likewise find
# all the package names listed in the HOST_<PKG>_DEPENDENCIES for each
# host package found above. Ideally this should be done recursively until
# no more packages are found, but that's hard to do in make, so limit to
# 1 level for now.
HOST_DEPS = $(sort $(foreach dep,\
		$(addsuffix _DEPENDENCIES,$(call UPPERCASE,$(TARGETS_HOST_DEPS))),\
		$($(dep))))
HOST_SOURCE += $(addsuffix -source,$(sort $(TARGETS_HOST_DEPS) $(HOST_DEPS)))

ATARGET_CLEAN += $(addsuffix -clean,$(ATARGETS))

# all targets depend on the crosscompiler and it's prerequisites
$(TARGETS_ALL): __real_tgt_%: $(BASE_TARGETS) %

dirs: $(DL_DIR) $(TOOLCHAIN_DIR) $(BUILD_DIR) $(STAGING_DIR) $(TARGET_DIR) \
	$(HOST_DIR) $(BR2_DEPENDS_DIR) $(BINARIES_DIR) $(STAMP_DIR) $(DEVICETABLE_DIR)
	
	

$(BASE_TARGETS): dirs $(O)/toolchainfile.cmake install-all-atpkconfigs install-all-headers

$(BUILD_DIR)/buildroot-config/auto.conf: $(CONFIG_DIR)/.config
	$(MAKE) $(EXTRAMAKEARGS) HOSTCC="$(HOSTCC_NOCCACHE)" HOSTCXX="$(HOSTCXX_NOCCACHE)" silentoldconfig; \
	sed -i 's/HGW_PRODUCTNAME=\"Speedport\ W\ 724V_Typ\ A_FW\ \"/HGW_PRODUCTNAME=\"Speedport\\\ W\\\ 724V_Typ\\\ A_FW\\\ \"/g' $(CONFIG_DIR)/.config; \
	sed -i 's/HGW_PRODUCTNAME=\"Speedport\ Hybrid\ \"/HGW_PRODUCTNAME=\"Speedport\\\ Hybrid\\\ \"/g' $(CONFIG_DIR)/.config

prepare: $(BUILD_DIR)/buildroot-config/auto.conf

ifneq ($(ATP_REPORT),)
world_prepare:
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > $(TOPDIR)/report.xml
	@echo "<sub_modules>" >> $(TOPDIR)/report.xml

world_end:
	@echo "</sub_modules>" >> $(TOPDIR)/report.xml

world: world_prepare $(REPORT_TARGETS) world_end
	@echo "All packages are exported into " $(TOPDIR)/report.xml
else
world: icpcicloudycheck prepare dependencies dirs  $(BASE_TARGETS)  $(TARGETS_ALL) 
endif

icpcicloudycheck:
	if ! [ -L ./package/atp/driver ]; then \
		ln -sf ../../driver ./package/atp/driver; \
	fi;
	if ! [ -L ./linux/$(KERNEL_VERSION)/package ]; then \
		ln -sf ../../package ./linux/$(KERNEL_VERSION)/package; \
	fi;
	if ! [ -L ./linux/$(KERNEL_VERSION)/driver ]; then \
		ln -sf ../../driver ./linux/$(KERNEL_VERSION)/driver; \
	fi;
	if ! [ -L ./linux/$(KERNEL_VERSION)/include/asm ]; then \
		ln -sf asm-$(ARCH) ./linux/$(KERNEL_VERSION)/include/asm; \
	fi;

icpcipretreatment: icpcicloudycheck prepare dependencies dirs $(BASE_TARGETS) atptypes-clean atptypes datamodel-clean datamodel

icpciapplication:  $(TARGETS_ICPCI_APPLICATION) 

icpcikernel: $(TARGETS_ICPCI_KERNEL) 

icpcimkp: $(TARGETS_ICPCI_MKP) 

prepare-debug-version:
	python scripts/build_debug_version.py $(CONFIG_DIR)/.config 

$(O)/toolchainfile.cmake:
	@echo -en "\
	set(CMAKE_SYSTEM_NAME Linux)\n\
	set(CMAKE_C_COMPILER $(TARGET_CC_NOCCACHE))\n\
	set(CMAKE_CXX_COMPILER $(TARGET_CXX_NOCCACHE))\n\
	set(CMAKE_C_FLAGS \"\$${CMAKE_C_FLAGS} $(TARGET_CFLAGS)\" CACHE STRING \"Buildroot CFLAGS\" FORCE)\n\
	set(CMAKE_CXX_FLAGS \"\$${CMAKE_CXX_FLAGS} $(TARGET_CXXFLAGS)\" CACHE STRING \"Buildroot CXXFLAGS\" FORCE)\n\
	set(CMAKE_INSTALL_SO_NO_EXE 0)\n\
	set(CMAKE_PROGRAM_PATH \"$(HOST_DIR)/usr/bin\")\n\
	set(CMAKE_FIND_ROOT_PATH \"$(STAGING_DIR)\")\n\
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)\n\
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)\n\
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)\n\
	set(ENV{PKG_CONFIG_SYSROOT_DIR} \"$(STAGING_DIR)\")\n\
	" > $@

.PHONY: all world dirs mkp clean distclean source outputmakefile \
	$(BASE_TARGETS) $(TARGETS) $(TARGETS_ALL) $(CMSTARGETS) $(TARGETS_INSTALLHEADERS) $(TARGETS_REMOVEHEADERS) $(DOCTARGETS) \
	$(TARGETS_CLEAN) $(TARGETS_DIRCLEAN) $(TARGETS_SOURCE) $(TARGETS_CHANGELOG) $(CMSTARGETS_CLEAN) \
	$(DL_DIR) $(TOOLCHAIN_DIR) $(BUILD_DIR) $(STAGING_DIR) $(TARGET_DIR) \
	$(HOST_DIR) $(BR2_DEPENDS_DIR) $(BINARIES_DIR) $(STAMP_DIR) $(KCONFIGTARGETS)

mkp: permissions-table target-finalize $(FSTARGETS) pkgimage erase-fakeroots

branch-bin: $(BRANCHATPCORETARGETS) $(BRANCHHUWEIPRIVATETARGETS)

#releas opensource
#1 make branch-bin
#2 make release-gpl
#3 make openall-clean GPLREL=1
# make all opensource  to press "make openall GPLREL=1"
release-gpl:
	rm -rf build
	rm -rf configs/atp
	rm -rf configs/bt
	rm -rf configs/tde
	rm -rf configs/telefonica
	rm -rf configs/dt/W723v
	rm -rf configs/dt/W724v_TR069_CERT
	rm -rf configs/dt/W724v
	rm -rf configs/dt/Hybrid/extbin
	rm -rf linux/2.6.30
	rm -rf package/atp/applications/oam
	rm -rf package/atp/applications/boardtest
	rm -rf package/atp/applications/boardtest
	rm -rf package/atp/applications/datacard/service
	rm -rf package/atp/applications/datacard/web
	rm -rf package/atp/applications/datacard/cms
	rm -rf package/atp/applications/ftpclient
	rm -rf package/atp/applications/itunes
	rm -rf package/atp/applications/ledservice/cms
	rm -rf package/atp/applications/ledservice/kernel/atp
	rm -rf package/atp/applications/ledservice/kernel/bt
	rm -rf package/atp/applications/ledservice/kernel/dt/W723v
	rm -rf package/atp/applications/ledservice/kernel/dt/W724v	
	rm -rf package/atp/applications/ledservice/kernel/tde
	rm -rf package/atp/applications/ledservice/kernel/telefonica
	rm -rf package/atp/applications/nas
	rm -rf package/atp/applications/dlna/cms
	rm -rf package/atp/applications/dlna/service
	rm -rf package/atp/applications/dlna/web
	rm -rf package/atp/applications/tr143
	rm -rf package/atp/applications/usbmngr
	rm -rf package/atp/applications/usbmount/cli
	rm -rf package/atp/applications/usbmount/cms
	rm -rf package/atp/applications/usbmount/service
	rm -rf package/atp/applications/usbmount/web
	rm -rf package/atp/applications/usbprinter/service/ipp
	rm -rf package/atp/applications/usbmount/web
	rm -rf package/atp/applications/usbprinter/cms
	rm -rf package/atp/applications/usbstorage/cms
	rm -rf package/atp/applications/usbstorage/web
	rm -rf package/atp/applications/powermanagement
	rm -rf package/atp/configuration/atpget
	rm -rf package/atp/configuration/cli
	rm -rf package/atp/configuration/cmsmain/src
	rm -rf package/atp/configuration/cmsmain/msg
	rm -rf package/atp/configuration/cwmp
	rm -rf package/atp/configuration/datamodel
	rm -rf package/atp/configuration/default
	rm -rf package/atp/configuration/ipping
	rm -rf package/atp/configuration/log
	rm -rf package/atp/configuration/mic
	rm -rf package/atp/configuration/monitor/main
	rm -rf package/atp/configuration/monitor/sdk
	rm -rf package/atp/configuration/portmnt
	rm -rf package/atp/configuration/service
	rm -rf package/atp/configuration/sshd
	rm -rf package/atp/configuration/sysinfo
	rm -rf package/atp/configuration/telnetd
	rm -rf package/atp/configuration/tr111
	rm -rf package/atp/configuration/traceroute
	rm -rf package/atp/configuration/upg
	rm -rf package/atp/configuration/upnp
	rm -rf package/atp/configuration/userintf
	rm -rf package/atp/libraries/private/atshared
	rm -rf package/atp/libraries/private/bhalapi/cli
	rm -rf package/atp/libraries/private/bhalapi/main
	rm -rf package/atp/libraries/private/bhalapi/sdk
	rm -rf package/atp/libraries/private/cfm
	rm -rf package/atp/libraries/private/cmscore
	rm -rf package/atp/libraries/private/dtcenter
	rm -rf package/atp/libraries/private/http
	rm -rf package/atp/libraries/private/interfacecore
	rm -rf package/atp/libraries/private/lgplutil
	rm -rf package/atp/libraries/private/msg
	rm -rf package/atp/libraries/private/stunc
	rm -rf package/atp/libraries/private/testcore
	rm -rf package/atp/libraries/private/timer
	rm -rf package/atp/libraries/private/vdb
	rm -rf package/atp/libraries/private/xmlapi
	rm -rf package/atp/libraries/opensrc/libid3tag
	rm -rf package/atp/libraries/opensrc/gdbm
	rm -rf package/atp/libraries/opensrc/ipsectools
	rm -rf package/atp/network/alg/cms
	rm -rf package/atp/network/alg/service/siproxd/siproxd
	rm -rf package/atp/network/arpsender
	rm -rf package/atp/network/bridge/bridgeportcms
	rm -rf package/atp/network/bridge/cms
	rm -rf package/atp/network/bridge/web
	rm -rf package/atp/network/common/cli
	rm -rf package/atp/network/common/ipv6common/cms
	rm -rf package/atp/network/ddns
	rm -rf package/atp/network/dhcp6c
	rm -rf package/atp/network/dhcp6s
	rm -rf package/atp/network/dhcpc/cms
	rm -rf package/atp/network/dhcps
	rm -rf package/atp/network/dns
	rm -rf package/atp/network/eth/cli
	rm -rf package/atp/network/eth/ethintfcms
	rm -rf package/atp/network/eth/ethlinkcms
	rm -rf package/atp/network/eth/lanethcms
	rm -rf package/atp/network/eth/service
	rm -rf package/atp/network/eth/wanethcms
	rm -rf package/atp/network/ethdslchg
	rm -rf package/atp/network/firewall/cli
	rm -rf package/atp/network/firewall/cms
	rm -rf package/atp/network/firewall/service
	rm -rf package/atp/network/fon/cms
	rm -rf package/atp/network/fon/service/foncontrl
	rm -rf package/atp/network/fon/web
	rm -rf package/atp/network/hurl
	rm -rf package/atp/network/igmp/cli
	rm -rf package/atp/network/igmp/cms
	rm -rf package/atp/network/igmp/service
	rm -rf package/atp/network/igmp/web
	rm -rf package/atp/network/ipcheck
	rm -rf package/atp/network/ipinterface
	rm -rf package/atp/network/ipsecvpn
	rm -rf package/atp/network/iptunnel
	rm -rf package/atp/network/lanname
	rm -rf package/atp/network/mirror
	rm -rf package/atp/network/mld/cli
	rm -rf package/atp/network/mld/cms
	rm -rf package/atp/network/mld/service
	rm -rf package/atp/network/nat/cms
	rm -rf package/atp/network/nat/web
	rm -rf package/atp/network/neighbordiscovery
	rm -rf package/atp/network/openwifi
	rm -rf package/atp/network/pcp
	rm -rf package/atp/network/pppc/cms
	rm -rf package/atp/network/pppc/pppintfcms
	rm -rf package/atp/network/pppc/web
	rm -rf package/atp/network/privacybutton
	rm -rf package/atp/network/pvcsearch
	rm -rf package/atp/network/qos/cms
	rm -rf package/atp/network/qos/dtqoscms
	rm -rf package/atp/network/qos/fapctl
	rm -rf package/atp/network/qos/fcctl
	rm -rf package/atp/network/qos/qoschannel
	rm -rf package/atp/network/radvd/cli
	rm -rf package/atp/network/radvd/cms
	rm -rf package/atp/network/route/cli
	rm -rf package/atp/network/route
	rm -rf package/atp/network/route6
	rm -rf package/atp/network/sntp/cms
	rm -rf package/atp/network/vlan/cms
	rm -rf package/atp/network/vlan/service
	rm -rf package/atp/network/wan/cli
	rm -rf package/atp/network/wan/cms
	rm -rf package/atp/network/wan/wancorecms
	rm -rf package/atp/network/wan/web
	rm -rf package/atp/network/wanumts
	rm -rf package/atp/network/wanvlan
	rm -rf package/atp/network/wlan
	rm -rf package/atp/network/xdsl
	rm -rf package/atp/network/hybrid
	rm -rf package/atp/network/lte
	rm -rf package/atp/voice/email/cms
	rm -rf package/atp/voice/netpb
	rm -rf package/atp/voice/rss
	rm -rf package/atp/voice/voiper
	rm -rf package/boot
	rm -rf driver/broadcom/bcm963268/4.12
	rm -rf driver/broadcom/bcm963268/4.12L06
	rm -rf driver/broadcom/bcm963268/4.12L07
	rm -rf driver/broadcom/bcm963268/4.12L08
	rm -rf driver/broadcom/bcm963268/dtvoice
	rm -rf driver/broadcom/bcm963268/voice
	rm -rf dl/busybox-1.18.5.tar.bz2
	rm -rf dl/ccache-3.1.4.tar.bz2
	rm -rf dl/e2fsprogs-1.42.tar.gz
	rm -rf dl/ebtables-2.0.6.1.tgz
	rm -rf dl/expat-2.0.1.tar.gz
	rm -rf dl/fakeroot_1.9.5.tar.gz
	rm -rf dl/gdb-6.8.tar.bz2
	rm -rf dl/iptables-1.2.11.tgz
	rm -rf dl/iptables-1.4.0.1.tgz
	rm -rf dl/iptables-1.4.12.tgz
	rm -rf dl/lzo-2.05.tar.gz
	rm -rf dl/mklibs-0.1.34-modify.tar.gz
	rm -rf dl/mtd-utils-1.4.6.tar.bz2
	rm -rf dl/ncurses-5.7.tar.gz
	rm -rf dl/siproxd-0.5.10.tgz
	rm -rf dl/squashfs2.0.tar.gz
	rm -rf dl/squashfs4.0.tar.gz
	rm -rf dl/squashfs4.2.tar.gz
	rm -rf dl/sstrip-20154.tar.gz
	rm -rf dl/u-boot-2011.03.tar.bz2
	rm -rf dl/xz-5.0.3.tar.bz2
	rm -rf dl/siproxd-0.5.10.tar.gz
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6318
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6318B
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6362
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6362B
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6368
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6368ao
		
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6368aobnd	
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6368B	
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore6368bnd
		
	find package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore63268 -name "*.c" | xargs rm -f	
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore63268aobnd	
	find package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore63268B -name "*.c" | xargs rm -f
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore63268bnd	
	rm -rf package/atp/driver/broadcom/bcm963268/4.14L02/adsl/adslcore63268bnd5	
	find package/atp/driver/broadcom/bcm963268/4.14L02/adsl/softdsl -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/adsl -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/bpm -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/ethernet -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/fap -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/ingqos -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/ingqos -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/pcmshim -name "*.c" | xargs rm -f
	#find package/atp/driver/broadcom/bcm963268/4.14L02/pktdma -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/pktflow -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/pwrmngt -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/serial -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/spi -name "*.c" | xargs rm -f
	
	find package/atp/driver/broadcom/bcm963268/4.14L02/tms -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/vlan -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/wlan -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/4.14L02/xtmcfg -name "*.c"  -name "*.cpp" | xargs rm -f
	
	find package/atp/driver/broadcom/bcm963268/hyvoice -name "*.c" | xargs rm -f
	find package/atp/driver/broadcom/bcm963268/hyvoice/dspapp -name "*.c" -name "*.h" | xargs rm -f
	rm -rf package/atp/driver/broadcom/bcm963268/hyvoice/xChange/dslx
	rm -rf package/atp/driver/broadcom/bcm963268/hyvoice/xChange/dslx_common
	rm -rf package/atp/configuration/web
	rm -rf package/atp/configuration/tr232
	rm -rf package/atp/hosttool/PCLINT
	rm -rf package/atp/hosttool/symboldb
	rm -rf package/atp/hosttool/cfgcodec
	rm -rf package/atp/hosttool/documentation
	rm -rf package/atp/hosttool/package/imagecmd
	rm -rf package/atp/hosttool/package/sign
	rm -rf package/atp/hosttool/package/mlzma
	rm -rf package/atp/hosttool/package/qlzma
	rm -rf package/atp/hosttool/package/hisi_addecc
	rm -rf package/atp/hosttool/package/*/src/*
	rm -rf output/build/host-e2fsprogs-1.42
	rm -rf output/build/host-fakeroot-1.9.5
	rm -rf output/build/host-lzo-2.05
	rm -rf output/build/host-makedevs-undefined
	rm -rf output/build/host-mtd-1.4.6
	rm -rf output/build/host-zlib-1.2.5
	rm -rf output/target/html
	rm -rf package/atp/network/fon/service/fon/src/DEBIAN
	rm -rf package/atp/network/fon/service/fon/src/doc
	rm -rf package/atp/network/fon/service/fon/src/files
	rm -rf package/atp/network/fon/service/fon/src/patches
	rm -rf package/atp/network/fon/service/fon/src/test
	rm -rf package/atp/network/fon/service/fon/src/src/chillout-mod
	rm -rf package/atp/network/fon/service/fon/src/src/fon-api
	rm -rf package/atp/network/fon/service/fon/src/src/fonsmcd
	
userapp-clean:bftpd-clean brctl-clean busybox-clean ebtables-clean iproute2-clean iptables-clean \
			siproxd-clean sntp-clean pppc-clean gplutil-clean ssl-clean osip-clean samba-clean ntfs-clean libpcap-clean fon-clean\
			libdaemon-clean radvd-clean sntp-clean iupnp-clean ixml-clean threadutil-clean zlib-clean sqlite-clean\
			lua-clean iconv-clean ffmpeg-clean fetchmail-clean cadaver-clean esmtp-clean cups-clean
	
userapp: gplutil bftpd brctl busybox ebtables iproute2 iptables siproxd sntp ssl osip samba ntfs libdaemon \
		 fon radvd sntp pppc iupnp ixml threadutil zlib sqlite lua iconv ffmpeg fetchmail cadaver esmtp cups
		 
openall: dirs  $(BASE_TARGETS) userapp linux

######################### clean all
openall-clean: dirs  $(BASE_TARGETS)-clean userapp-clean linux-clean 
			
#

#
#############################################################
#
# staging and target directories do NOT list these as
# dependencies anywhere else
#
#############################################################
$(DL_DIR) $(TOOLCHAIN_DIR) $(BUILD_DIR) $(STAMP_DIR) $(DEVICETABLE_DIR):
	@mkdir -p $@

$(BINARIES_DIR):
	@mkdir -p $@
	@mkdir -p $@/sysmap
	
$(STAGING_DIR):
	@mkdir -p $(STAGING_DIR)/bin
	@mkdir -p $(STAGING_DIR)/lib
	@mkdir -p $(STAGING_DIR)/usr/lib
	@mkdir -p $(STAGING_DIR)/usr/include
	@mkdir -p $(STAGING_DIR)/usr/bin
	@ln -snf $(STAGING_DIR) $(BASE_DIR)/staging
	
$(HOST_DIR):
	@mkdir -p $@
	@mkdir -p $(HOST_DIR)/usr/lib
	@mkdir -p $(HOST_DIR)/usr/bin
	@mkdir -p $(HOST_DIR)/usr/include

ifeq ($(BR2_ROOTFS_SKELETON_CUSTOM),y)
TARGET_SKELETON=$(BR2_ROOTFS_SKELETON_CUSTOM_PATH)
endif


export TARGET_SKELETON

$(BUILD_DIR)/.root:
	mkdir -p $(TARGET_DIR)
	cp -fa $(TARGET_SKELETON)/* $(TARGET_DIR)/
	if [ -d $(TARGET_DIR)/etc/ ]; then \
		ln -sf /var/group $(TARGET_DIR)/etc/group; \
		ln -sf /proc/mounts $(TARGET_DIR)/etc/mtab; \
		ln -sf /var/passwd $(TARGET_DIR)/etc/passwd; \
		ln -sf /var/TZ $(TARGET_DIR)/etc/TZ; \
	fi;
	-find $(TARGET_DIR) -type d -name CVS -print0 -o -name .svn -print0 | xargs -0 rm -rf
	-find $(TARGET_DIR) -type f \( -name .empty -o -name '*~' \) -print0 | xargs -0 rm -rf
	touch $@

#$(BUILD_DIR)/.root:
#	mkdir -p $(TARGET_DIR)
#	if ! [ -d "$(TARGET_DIR)/bin" ]; then \
#		if [ -d "$(TARGET_SKELETON)" ]; then \
#			cp -fa $(TARGET_SKELETON)/* $(TARGET_DIR)/; \
#		fi; \
#		touch $(STAGING_DIR)/.fakeroot.00000; \
#	fi
#	-find $(TARGET_DIR) -type d -name CVS -print0 -o -name .svn -print0 | xargs -0 rm -rf
#	-find $(TARGET_DIR) -type f \( -name .empty -o -name '*~' \) -print0 | xargs -0 rm -rf
#	touch $@

$(TARGET_DIR): $(BUILD_DIR)/.root

erase-fakeroots:
	rm -f $(BUILD_DIR)/.fakeroot*

target-finalize:
	chmod 777 scripts/*
ifeq ($(BR2_HAVE_DEVFILES),y)
	( scripts/copy.sh $(STAGING_DIR) $(TARGET_DIR) )
else
	rm -rf $(TARGET_DIR)/usr/include $(TARGET_DIR)/usr/lib/pkgconfig $(TARGET_DIR)/usr/share/aclocal
	find $(TARGET_DIR)/lib \( -name '*.a' -o -name '*.la' \) -print0 | xargs -0 rm -f
	find $(TARGET_DIR)/usr/lib \( -name '*.a' -o -name '*.la' \) -print0 | xargs -0 rm -f
	-find $(TARGET_DIR) -type d -name CVS -print0 -o -name .svn -print0 | xargs -0 rm -rf
endif
ifneq ($(BR2_PACKAGE_GDB),y)
	rm -rf $(TARGET_DIR)/usr/share/gdb
endif
ifneq ($(BR2_HAVE_DOCUMENTATION),y)
	rm -rf $(TARGET_DIR)/usr/man $(TARGET_DIR)/usr/share/man
	rm -rf $(TARGET_DIR)/usr/info $(TARGET_DIR)/usr/share/info
	rm -rf $(TARGET_DIR)/usr/doc $(TARGET_DIR)/usr/share/doc
	rm -rf $(TARGET_DIR)/usr/share/gtk-doc
	-rmdir $(TARGET_DIR)/usr/share 2>/dev/null
endif
ifeq ($(BR2_PACKAGE_PYTHON_PY_ONLY),y)
	find $(TARGET_DIR)/usr/lib/ -name '*.pyc' -print0 | xargs -0 rm -f
endif
ifeq ($(BR2_PACKAGE_PYTHON_PYC_ONLY),y)
	find $(TARGET_DIR)/usr/lib/ -name '*.py' -print0 | xargs -0 rm -f
endif
	rm -rf $(TARGET_DIR)/unstriptmp
	( scripts/unstripfiles.sh $(TARGET_DIR) $(UNSTRIP_FILES) 0 )
ifeq ($(BR2_REDUCE_BY_MAKELIBS),y)
	@rm -fr $(OUTPUT_DIR)/mklibstmp
	@mkdir -p $(OUTPUT_DIR)/mklibstmp
	@cp $(BR2_TOOLCHAIN_EXTERNAL_PATH)/lib/*_pic.a $(STAGING_DIR)/lib/
	@mklibs -v -D -L /lib --target $(TARGET_CROSS) --root $(HOST_DIR)/usr/sysroot --sysroot $(HOST_DIR)/usr/sysroot --libc-extras-dir $(HOST_DIR)/usr/sysroot -d $(OUTPUT_DIR)/mklibstmp `find $(STAGING_DIR)/bin -path $(STAGING_DIR) -prune -o -type f -print | file -f - | grep ELF | cut -d':' -f1`
	@cp -f $(OUTPUT_DIR)/mklibstmp/* $(TARGET_DIR)/lib
	@rm -fr $(OUTPUT_DIR)/mklibstmp
endif
	-$(TARGET_SSTRIP) $(TARGET_DIR)/bin/* $(TARGET_DIR)/lib/*.so.*	$(TARGET_DIR)/lib/*.so 
	#-$(TARGET_SSTRIP) $(TARGET_DIR)/usr/bin/* $(TARGET_DIR)/usr/lib/*.so.*	
	( scripts/unstripfiles.sh $(TARGET_DIR) $(UNSTRIP_FILES) 1 )
	chmod 777 $(TARGET_DIR)/lib/*
	mkdir -p $(TARGET_DIR)/etc
	# Mandatory configuration file and auxilliary cache directory
	# for recent versions of ldconfig
	touch $(TARGET_DIR)/etc/ld.so.conf
	mkdir -p $(TARGET_DIR)/var/cache/ldconfig
	echo $(BR2_VERSION_FULL) > $(TARGET_DIR)/etc/br-version
ifneq ($(BROADCOM_CHIP),)
	-find $(TARGET_DIR)/lib -name '*.ko' ! -name 'ip*.ko' |xargs $(STRIP) --strip-unneeded
endif
ifneq ($(BCM96318),)
	-rm $(TARGET_DIR)/bin/ip6tables
	-rm $(TARGET_DIR)/bin/ethswctl
	-rm $(TARGET_DIR)/bin/radvdump
	-rm $(TARGET_DIR)/bin/ss
	-rm $(TARGET_DIR)/bin/rtacct
endif
	#/*在文件系统打包前调用用户自己的脚本*/
ifneq ($(BR2_ROOTFS_POST_BUILD_SCRIPT),"")
	$(BR2_ROOTFS_POST_BUILD_SCRIPT) $(FLASH_NAND_FIRST_BLOCK_WITH_CFERAM) $(BUILD_FOR_HARDWARE_IAMGE)
endif

ifeq ($(BR2_ENABLE_LOCALE_PURGE),y)
LOCALE_WHITELIST=$(BUILD_DIR)/locales.nopurge
LOCALE_NOPURGE=$(call qstrip,$(BR2_ENABLE_LOCALE_WHITELIST))

target-purgelocales:
	rm -f $(LOCALE_WHITELIST)
	for i in $(LOCALE_NOPURGE); do echo $$i >> $(LOCALE_WHITELIST); done

	for dir in $(wildcard $(addprefix $(TARGET_DIR),/usr/share/locale /usr/share/X11/locale /usr/man /usr/share/man)); \
	do \
		for lang in $$(cd $$dir; ls .|grep -v man); \
		do \
			grep -qx $$lang $(LOCALE_WHITELIST) || rm -rf $$dir/$$lang; \
		done; \
	done
endif

source: dirs $(TARGETS_SOURCE) $(HOST_SOURCE)

test-all: $(TESTTARGETS)

test-all-run: $(TARGETS_TEST_RUN)

html: $(HTMLTARGETS)

atp: dirs install-all-atpkconfigs install-all-headers $(ATARGETS) $(PETABLETARGETS) html

atp-clean: $(ATARGET_CLEAN) $(CMSTARGETS_CLEAN) cli-clean-all $(TESTARGETS_CLEAN) remove-all-atpkconfigs remove-all-headers 
	rm -rf $(BASE_DIR)/package
	
cms-clean-all:	cms-clean $(CMSTARGETS_CLEAN)

cli-clean-all:	cli-clean $(CLITARGETS_CLEAN)

install-all-headers: $(TARGETS_INSTALLHEADERS)

remove-all-headers: $(TARGETS_REMOVEHEADERS)

install-all-docheaders: $(DOCTARGETS)

changelog: $(TARGETS_CHANGELOG)

genkey: $(GENKEYTARGETS)
genkey-clean: $(GENKEYTARGETS_CLEAN)

permissions-table: $(PETABLETARGETS)

# Kernel Configure
$(KERNEL_CONFIG_FILE).stamp_atpkconfigured:
	@cat $(KERNEL_DEFAULT_FILE) >$(KERNEL_CONFIG_FILE)
	@echo "#kernel config verify" >$(KERNEL_CONFIG_FILE).verify
	$(Q)touch $@

ifneq ($(KERNEL_VERSION),2.6.36)
install-all-atpkconfigs: $(KERNEL_CONFIG_FILE).stamp_atpkconfigured $(KCONFIGTARGETS)
	@echo "DONE :$(KCONFIGTARGETS)"
	scripts/sync_kconfig $(KERNEL_CONFIG_FILE) $(CONFIG_DIR)/.config
else
install-all-atpkconfigs:$(KERNEL_CONFIG_FILE).stamp_atpkconfigured
	@echo "No need do atpkconfig for :$(KCONFIGTARGETS)"
	scripts/sync_kconfig $(KERNEL_CONFIG_FILE) $(CONFIG_DIR)/.config
endif

atpkconfig_init:
	@rm -f $(KERNEL_CONFIG_FILE).stamp_atpkconfigured
	@rm -f $(KERNEL_CONFIG_FILE).verify

remove-all-atpkconfigs: atpkconfig_init $(TARGETS_KCONFIG_REMOVE)
	@echo "CLEAN KCONFIG DONE..."

external-deps:
	@$(MAKE) -Bs DL_MODE=SHOW_EXTERNAL_DEPS $(EXTRAMAKEARGS) source | sort -u

show-targets:
	@$(call TOPMESSAGE,"TARGETS: All targets to be compiled ...")
	@echo $(TARGETS)
	@$(call TOPMESSAGE,"KERNELTARGETS: All kernel targets ...")
	@echo $(KERNELTARGETS)
	@$(call TOPMESSAGE,"KCONFIGTARGETS: All kernel config ...")
	@echo $(KCONFIGTARGETS)
	@$(call TOPMESSAGE,"ATARGETS: All ATP targets ...")
	@echo $(ATARGETS)
	@$(call TOPMESSAGE,"CMSTARGETS: All cms targets ...")
	@echo $(CMSTARGETS)
	@$(call TOPMESSAGE,"WEBTARGETS: All web targets ...")
	@echo $(WEBTARGETS)
	@$(call TOPMESSAGE,"CLITARGETS: All cli targets ...")
	@echo $(CLITARGETS)
	@$(call TOPMESSAGE,"TARGETS_CLEAN: All targets to be cleaned ...")
	@echo $(TARGETS_CLEAN)
	@$(call TOPMESSAGE,"TARGETS_INSTALLHEADERS: All headers to be installed ...")
	@echo $(TARGETS_INSTALLHEADERS)
	@$(call TOPMESSAGE,"CBBIDTARGETS: All targets with cbbids ...")
	@echo $(CBBIDTARGETS)
	@$(call TOPMESSAGE,"TESTTARGETS: All test targets ...")
	@echo $(TESTTARGETS)
	@$(call TOPMESSAGE,"DOCTARGETS: All targets with document...")
	@echo $(DOCTARGETS)
	@$(call TOPMESSAGE,"GENKEYTARGETS: All targets with keys...")
	@echo $(GENKEYTARGETS)
	@$(call TOPMESSAGE,"BRANCHATPCORETARGETS: All targets with atp core license...")
	@echo $(BRANCHATPCORETARGETS)
	@$(call TOPMESSAGE,"BRANCHHUWEIPRIVATETARGETS: All targets with huawei private license...")
	@echo $(BRANCHHUWEIPRIVATETARGETS)
	@$(call TOPMESSAGE,"BRANCHKERNELTARGETS: All kernel targets to be branched...")
	@echo $(BRANCHKERNELTARGETS)
	@$(call TOPMESSAGE,"BRANCHOPENSOURCETARGETS: All opensrc targets...")
	@echo $(BRANCHOPENSOURCETARGETS)
	@$(call TOPMESSAGE,"BRANCHUNDEFINETARGETS: All license-undefine targets...")
	@echo $(BRANCHUNDEFINETARGETS)
	@$(call TOPMESSAGE,"ICPCITARGETS: All icpci application targets...")
	@echo $(ICPCIAPPLICATIONTARGETS)
	@$(call TOPMESSAGE,"ICPCITARGETS: All icpci kernel targets...")
	@echo $(ICPCIKERNELTARGETS)
	@$(call TOPMESSAGE,"ICPCITARGETS: All icpci mkp targets...")
	@echo $(ICPCIMKPTARGETS)

else # ifeq ($(BR2_HAVE_DOT_CONFIG),y)

all: menuconfig

# configuration
# ---------------------------------------------------------------------------

HOSTCFLAGS=$(CFLAGS_FOR_BUILD)
export HOSTCFLAGS

$(BUILD_DIR)/buildroot-config/%onf:
	mkdir -p $(@D)/lxdialog
	$(MAKE) CC="$(HOSTCC_NOCCACHE)" HOSTCC="$(HOSTCC_NOCCACHE)" obj=$(@D) -C $(CONFIG) -f Makefile.br $(@F)

COMMON_CONFIG_ENV = \
	KCONFIG_AUTOCONFIG=$(BUILD_DIR)/buildroot-config/auto.conf \
	KCONFIG_AUTOHEADER=$(BUILD_DIR)/buildroot-config/autoconf.h \
	KCONFIG_TRISTATE=$(BUILD_DIR)/buildroot-config/tristate.config \
	BUILDROOT_CONFIG=$(CONFIG_DIR)/.config
	
xconfig: $(BUILD_DIR)/buildroot-config/qconf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< $(CONFIG_CONFIG_IN)

gconfig: $(BUILD_DIR)/buildroot-config/gconf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) srctree=$(TOPDIR) $< $(CONFIG_CONFIG_IN)

menuconfig: $(BUILD_DIR)/buildroot-config/mconf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< $(CONFIG_CONFIG_IN)
	@sed -i 's/HGW_PRODUCTNAME=\"Speedport\ W\ 724V_Typ\ A_FW\ \"/HGW_PRODUCTNAME=\"Speedport\\\ W\\\ 724V_Typ\\\ A_FW\\\ \"/g' $(CONFIG_DIR)/.config;
	@sed -i 's/HGW_PRODUCTNAME=\"Speedport\ Hybrid\ \"/HGW_PRODUCTNAME=\"Speedport\\\ Hybrid\\\ \"/g' $(CONFIG_DIR)/.config;
	
nconfig: $(BUILD_DIR)/buildroot-config/nconf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< $(CONFIG_CONFIG_IN)

config: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< $(CONFIG_CONFIG_IN)

oldconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --oldconfig $(CONFIG_CONFIG_IN)

randconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --randconfig $(CONFIG_CONFIG_IN)

allyesconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --allyesconfig $(CONFIG_CONFIG_IN)

allnoconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --allnoconfig $(CONFIG_CONFIG_IN)

randpackageconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@grep -v BR2_PACKAGE_ $(CONFIG_DIR)/.config > $(CONFIG_DIR)/.config.nopkg
	@$(COMMON_CONFIG_ENV) \
		KCONFIG_ALLCONFIG=$(CONFIG_DIR)/.config.nopkg \
		$< --randconfig $(CONFIG_CONFIG_IN)
	@rm -f $(CONFIG_DIR)/.config.nopkg

allyespackageconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@grep -v BR2_PACKAGE_ $(CONFIG_DIR)/.config > $(CONFIG_DIR)/.config.nopkg
	@$(COMMON_CONFIG_ENV) \
		KCONFIG_ALLCONFIG=$(CONFIG_DIR)/.config.nopkg \
		$< --allyesconfig $(CONFIG_CONFIG_IN)
	@rm -f $(CONFIG_DIR)/.config.nopkg

allnopackageconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@grep -v BR2_PACKAGE_ $(CONFIG_DIR)/.config > $(CONFIG_DIR)/.config.nopkg
	@$(COMMON_CONFIG_ENV) \
		KCONFIG_ALLCONFIG=$(CONFIG_DIR)/.config.nopkg \
		$< --allnoconfig $(CONFIG_CONFIG_IN)
	@rm -f $(CONFIG_DIR)/.config.nopkg

silentoldconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	$(COMMON_CONFIG_ENV) $< --silentoldconfig $(CONFIG_CONFIG_IN)
	$(COMMON_CONFIG_ENV) $< --silentoldconfig $(CONFIG_CONFIG_IN)

defconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --defconfig $(CONFIG_CONFIG_IN)

%_defconfig: $(BUILD_DIR)/buildroot-config/conf $(TOPDIR)/configs/%_defconfig outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --defconfig=$(TOPDIR)/configs/$@ $(CONFIG_CONFIG_IN)

savedefconfig: $(BUILD_DIR)/buildroot-config/conf outputmakefile
	@mkdir -p $(BUILD_DIR)/buildroot-config
	@$(COMMON_CONFIG_ENV) $< --savedefconfig=$(CONFIG_DIR)/defconfig $(CONFIG_CONFIG_IN)

# check if download URLs are outdated
source-check: allyesconfig
	$(MAKE) DL_MODE=SOURCE_CHECK $(EXTRAMAKEARGS) source

endif # ifeq ($(BR2_HAVE_DOT_CONFIG),y)

#############################################################
#
# Cleanup and misc junk
#
#############################################################

# outputmakefile generates a Makefile in the output directory, if using a
# separate output directory. This allows convenient use of make in the
# output directory.
outputmakefile:
ifeq ($(NEED_WRAPPER),y)
	$(Q)$(TOPDIR)/scripts/mkmakefile $(TOPDIR) $(O)
endif

clean: $(TARGETS_CLEAN) atp-clean 
	rm -rf $(STAGING_DIR) $(TARGET_DIR) $(BINARIES_DIR) $(HOST_DIR) \
		$(STAMP_DIR) $(BUILD_DIR) $(TOOLCHAIN_DIR) $(BASE_DIR)/staging \
		$(DEVICETABLE_DIR) $(BASE_DIR)/package $(BASE_DIR)/linux \
		$(BASE_DIR)/driver $(BASE_DIR)/home

distclean: clean
ifeq ($(DL_DIR),$(TOPDIR)/dl)
	rm -rf $(DL_DIR)
endif
ifeq ($(O),output)
	rm -rf $(O)
endif
	rm -rf $(CONFIG_DIR)/.config $(CONFIG_DIR)/.config.old $(CONFIG_DIR)/.auto.deps

cross: $(BASE_TARGETS)

help:
	@echo 'Cleaning:'
	@echo '  clean                  - delete all files created by build'
	@echo '  distclean              - delete all non-source files (including .config)'
	@echo
	@echo 'Build:'
	@echo '  all                    - make world'
	@echo
	@echo 'Configuration:'
	@echo '  menuconfig             - interactive curses-based configurator'
	@echo '  nconfig                - interactive ncurses-based configurator'
	@echo '  xconfig                - interactive Qt-based configurator'
	@echo '  gconfig                - interactive GTK-based configurator'
	@echo '  oldconfig              - resolve any unresolved symbols in .config'
	@echo '  randconfig             - New config with random answer to all options'
	@echo '  defconfig              - New config with default answer to all options'
	@echo '  savedefconfig          - Save current config as ./defconfig (minimal config)'
	@echo '  allyesconfig           - New config where all options are accepted with yes'
	@echo '  allnoconfig            - New config where all options are answered with no'
	@echo '  randpackageconfig      - New config with random answer to package options'
	@echo '  allyespackageconfig    - New config where pkg options are accepted with yes'
	@echo '  allnopackageconfig     - New config where package options are answered with no'
ifeq ($(BR2_PACKAGE_BUSYBOX),y)
	@echo '  busybox-menuconfig     - Run BusyBox menuconfig'
endif
ifeq ($(BR2_LINUX_KERNEL),y)
	@echo '  linux-menuconfig       - Run Linux kernel menuconfig'
	@echo '  linux-savedefconfig    - Run Linux kernel savedefconfig'
endif
ifeq ($(BR2_TOOLCHAIN_BUILDROOT),y)
	@echo '  uclibc-menuconfig      - Run uClibc menuconfig'
endif
ifeq ($(BR2_TOOLCHAIN_CTNG),y)
	@echo '  ctng-menuconfig        - Run crosstool-NG menuconfig'
endif
ifeq ($(BR2_TARGET_BAREBOX),y)
	@echo '  barebox-menuconfig     - Run barebox menuconfig'
	@echo '  barebox-savedefconfig  - Run barebox savedefconfig'
endif
	@echo
	@echo 'Miscellaneous:'
	@echo '  source                 - download all sources needed for offline-build'
	@echo '  source-check           - check all packages for valid download URLs'
	@echo '  external-deps          - list external packages used'
	@echo
	@echo '  make V=0|1             - 0 => quiet build (default), 1 => verbose build'
	@echo '  make O=dir             - Locate all output files in "dir", including .config'
	@echo
	@$(foreach b, $(sort $(notdir $(wildcard $(TOPDIR)/configs/*_defconfig))), \
	  printf "  %-35s - Build for %s\\n" $(b) $(b:_defconfig=);)
	@echo
	@echo 'See docs/README and docs/buildroot.html for further details'
	@echo

menuconfig-all:
	@echo "Do menuconfig for all products now ..."
	@$(foreach b, $(sort $(notdir $(wildcard $(TOPDIR)/configs/*))), \
	  $(foreach c, $(sort $(notdir $(wildcard $(TOPDIR)/configs/$(b)/*))), make CUSTOMER=$(b) VENDOR=$(c) menuconfig;))

SVN_DIFF:=`svn diff configs/atp/llt/.config`

define GET_SVN_DIFF
	@if [ `svn diff $(1) | grep -w -c "$(1)"` -ge 1 ]; then \
		echo "$(1) Changed."; \
	else \
		$(call TOPMESSAGE,Nothing changed; no need to lock.) ; \
		exit 1; \
	fi
endef

config-lock:
	@$(call TOPMESSAGE,"Check if $(CONFIG_DIR)/.config changed ...")
	$(call GET_SVN_DIFF,$(CONFIG_DIR)/.config)

	@$(call TOPMESSAGE, "Generate patch file ...")
	@svn diff $(CONFIG_DIR)/.config > $(CONFIG_DIR)/$(CUSTOMER)_$(VENDOR).patch
	@cp $(CONFIG_DIR)/$(CUSTOMER)_$(VENDOR).patch $(CONFIG_DIR)/other.patch

	@$(call TOPMESSAGE, "Lock all .config ...")
	@$(foreach b, $(sort $(notdir $(wildcard $(TOPDIR)/configs/*))), \
	  $(foreach c, $(sort $(notdir $(wildcard $(TOPDIR)/configs/$(b)/*))), echo "Lock configs/$(b)/$(c)/.config ...";mv $(TOPDIR)/configs/$(b)/$(c)/.config $(TOPDIR)/configs/$(b)/$(c)/.config_bk;svn up $(TOPDIR)/configs/$(b)/$(c)/.config >/dev/null;svn lock $(TOPDIR)/configs/$(b)/$(c)/.config>/dev/null;))

config-patch-all:
	@$(foreach b, $(sort $(notdir $(wildcard $(TOPDIR)/configs/*))), \
	  $(foreach c, $(sort $(notdir $(wildcard $(TOPDIR)/configs/$(b)/*))), $(call TOPMESSAGE,Patching configs/$(b)/$(c)/.config ...);patch -p0 $(TOPDIR)/configs/$(b)/$(c)/.config $(if $(filter $(b)/$(c),$(CUSTOMER)/$(VENDOR)),$(CONFIG_DIR)/$(CUSTOMER)_$(VENDOR).patch,$(CONFIG_DIR)/other.patch);))
	  
config-unlock:
	@$(foreach b, $(sort $(notdir $(wildcard $(TOPDIR)/configs/*))), \
	  $(foreach c, $(sort $(notdir $(wildcard $(TOPDIR)/configs/$(b)/*))), echo "Unlock configs/$(b)/$(c)/.config ...";svn unlock $(TOPDIR)/configs/$(b)/$(c)/.config 2>/dev/null;))\
	echo " "
	
modify-diff:
	python scripts/modify_diff.py modifyfiles.txt
	
release-keys:
	rm -rf $(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)
	mkdir -p $(BINARIES_DIR)/$(CUSTOMER)
	mkdir -p $(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)
	$(GEN_INI_CMD)
	cp $(BINARIES_DIR)/config.ini $(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/config.ini
	touch $(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/keys.pem
	find $(TOPDIR)/package/atp/libraries/private/msg/sec/certs/$(CUSTOMER)/$(VENDOR) -name "*.pem" | xargs cat >>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/keys.pem
	echo "[cfg]">$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini
	$(if $(BUILD_CFGFILE_ENCRYPT_EXPORT),echo "encode=1">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini,echo "encode=0">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini)
	$(if $(BUILD_CFGFILE_SIGN_EXPORT),echo "sign=1">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini,echo "sign=0">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini)
	$(if $(BUILD_LIBZ),echo "zip=1">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini,echo "zip=0">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini)
	echo "[sys]">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini
	echo "productclass=$(HGW_PRODUCTCLASS)">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini
	echo "[pkg]">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini
	echo "keysize=$(BUILD_UPG_KEY1_KEYSIZE)">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini
	echo "keyalg=$(BUILD_UPG_KEY1_ALGID)">>$(BINARIES_DIR)/$(CUSTOMER)/$(VENDOR)/cfg.ini
release: OUT=buildroot-$(BR2_VERSION)

release:
	git archive --format=tar --prefix=$(OUT)/ master|gzip -9 >$(OUT).tar.gz

.PHONY: $(noconfig_targets)

