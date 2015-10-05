/*
    sdk config file autogened
    include this file at the top of each c file before using other sdk heads
*/
#ifndef __SDK_AUTOCONF_H__
#define __SDK_AUTOCONF_H__

#ifdef CONFIG_HISI_SD5115_VER_100
#undef CONFIG_HISI_SD5115_VER_100
#endif
#define CONFIG_HISI_SD5115_VER_100  1

#ifdef CONFIG_HISI_SD5115_TYPE_H
#undef CONFIG_HISI_SD5115_TYPE_H
#endif
#define CONFIG_HISI_SD5115_TYPE_H  1

#ifdef CONFIG_HISI_SD5115_AF_ASIC
#undef CONFIG_HISI_SD5115_AF_ASIC
#endif
#define CONFIG_HISI_SD5115_AF_ASIC  1

#ifdef CONFIG_HISI_SD5115_MP_PILOT
#undef CONFIG_HISI_SD5115_MP_PILOT
#endif
#define CONFIG_HISI_SD5115_MP_PILOT  1


#define CONFIG_HISI_SD5115_MP_PILOT
#define CONFIG_HISI_SD5115_AF_ASIC
#define CONFIG_HISI_SD5115_TYPE_H

#if !defined (CONFIG_HISI_SD5115_MP_PILOT) && !defined (CONFIG_HISI_SD5115_MP_MPW)
#error "SDK config is invalid!!!"
#endif

#if !defined (CONFIG_HISI_SD5115_AF_ASIC) && !defined (CONFIG_HISI_SD5115_AF_FPGA)
#error "SDK config is invalid!!!"
#endif

#if defined(CONFIG_HISI_SD5115_MP_PILOT)
#if !defined (CONFIG_HISI_SD5115_TYPE_S) && !defined (CONFIG_HISI_SD5115_TYPE_H) && !defined (CONFIG_HISI_SD5115_TYPE_T)
#error "SDK config is invalid!!!"
#endif
#endif

#endif	//#ifndef __SDK_AUTOCONF_H__
