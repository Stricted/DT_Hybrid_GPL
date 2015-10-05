/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_sysdef.h
  功能描述   : hi_sysdef.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : D2011_07_19
    修改内容   : 创建文件

******************************************************************************/
#ifndef __HI_SYSDEF_H__
#define __HI_SYSDEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*******************************************************
high level protocol system defined
-------------------------------------------
31    -   28   -  20  -  12   -    8   -     0
| precode(0xF) | system | mod  | submod | cmdtype | 
-------------------------------------------
global retcode system defined
-------------------------------------------
31    -   28   -  20  -  12   -    8   -     0
| precode(0xE) | system | mod  | submod | retcode | 
********************************************************/
#define     HI_PRECODE_MAX          0xF   /* 最大前导码数*/
#define     HI_SYSTEM_MAX           0xFF  /* 最大系统数 */
#define     HI_SRCMODULE_MAX        0x100 /* 最大模块数 */
#define     HI_SUBMODULE_MAX        0xF   /* 最大子模块数 */
#define     HI_RETCODE_MAX          0xFF  /* 最大返回码数 */
#define     HI_CMDTYPE_MAX          0xFFF /* 最大命令数 */

#define     HI_PRECODE_MASK         0xF   /* 获取前导码掩码*/
#define     HI_SYSTEM_MASK          0xFF  /* 获取系统掩码 */
#define     HI_SRCMODULE_MASK       0xFF  /* 获取模块掩码 */
#define     HI_SUBMODULE_MASK       0xF   /* 获取子模块掩码 */
#define     HI_RETCODE_MASK         0xFF  /* 获取返回值掩码 */
#define     HI_CMDTYPE_MASK         0xFFF /* 获取命令数掩码 */

#define     HI_PRECODE_OFFSET       28    /* 获取前导码偏移值 */
#define     HI_SYSTEM_OFFSET        20    /* 获取系统偏移值 */
#define     HI_SRCMODULE_OFFSET     12    /* 获取模块偏移值 */
#define     HI_SUBMODULE_OFFSET     8     /* 获取子模块偏移值 */

typedef enum  {
    HI_SYSBASE_GLB                = 0xF0000000,     /*全局通用*/
    HI_SYSBASE_KBASIC             = 0xF1000000,     /*内核基本类库系统*/
    HI_SYSBASE_KDRIVER            = 0xF2000000,     /*内核驱动系统*/
    HI_SYSBASE_KSOC               = 0xF3000000,     /*内核SOC适配系统*/
    HI_SYSBASE_KSAMPLE            = 0xF4000000,     /*内核样例系统*/
    HI_SYSBASE_SYSTEM             = 0xF5000000,     /*用户态系统管理系统*/
    HI_SYSBASE_APPS               = 0xF6000000,     /*用户态通用应用系统*/
    HI_SYSBASE_USERBASE           = 0xF7000000,     /*用户态基本类库系统*/
    HI_SYSBASE_VENDOR             = 0xF8000000,     /*用户态厂家适配系统*/
    HI_SYSBASE_UBOOT              = 0xF9000000,     /*UBOOT系统*/
    HI_SYSBASE_KERNEL             = 0xFA000000,     /*kernel系统*/
    HI_SYSBASE_OPENSRC            = 0xFB000000,     /*opensource系统*/
    HI_SYSBASE_SDK                = 0xFC000000,     /*sdk系统*/
}hi_sysbase_e;

typedef enum  {
    HI_SRCMODULE_KBASIC_DEV       = ( HI_SYSBASE_KBASIC | 0x00001000),
    HI_SRCMODULE_KBASIC_HLP       = ( HI_SYSBASE_KBASIC | 0x00002000),
    HI_SRCMODULE_KBASIC_LOG       = ( HI_SYSBASE_KBASIC | 0x00003000),
    HI_SRCMODULE_KBASIC_MSG       = ( HI_SYSBASE_KBASIC | 0x00004000),
    HI_SRCMODULE_KBASIC_NETLINK   = ( HI_SYSBASE_KBASIC | 0x00005000),
    HI_SRCMODULE_KBASIC_OS        = ( HI_SYSBASE_KBASIC | 0x00006000),
    HI_SRCMODULE_KBASIC_QUEUE     = ( HI_SYSBASE_KBASIC | 0x00007000),
    HI_SRCMODULE_KBASIC_TIMER     = ( HI_SYSBASE_KBASIC | 0x00008000),
    HI_SRCMODULE_KBASIC_HSAH      = ( HI_SYSBASE_KBASIC | 0x00009000),
}hi_kbasic_e;

typedef enum  {
    HI_SRCMODULE_KDRIVER_VOIP     = ( HI_SYSBASE_KDRIVER | 0x00001000),
    HI_SRCMODULE_KDRIVER_NETWORKS = ( HI_SYSBASE_KDRIVER | 0x00002000),
    HI_SRCMODULE_KDRIVER_CODEC    = ( HI_SYSBASE_KDRIVER | 0x00003000),
    HI_SRCMODULE_KDRIVER_GPIO    = ( HI_SYSBASE_KDRIVER | 0x00004000),
}hi_kdriver_e;

typedef enum  {
    HI_SUBMODULE_KNETWORKS_VIF    = ( HI_SRCMODULE_KDRIVER_NETWORKS | 0x00000100),
    HI_SUBMODULE_KNETWORKS_ETH    = ( HI_SRCMODULE_KDRIVER_NETWORKS | 0x00000200),
    HI_SUBMODULE_KNETWORKS_ACCEL  = ( HI_SRCMODULE_KDRIVER_NETWORKS | 0x00000300),    
    HI_SUBMODULE_KNETWORKS_QOS    = ( HI_SRCMODULE_KDRIVER_NETWORKS | 0x00000400),    
}hi_knetworks_submod_e;

typedef enum  {
    HI_SRCMODULE_KSOC_BCM           = ( HI_SYSBASE_KSOC | 0x00001000),
    HI_SRCMODULE_KSOC_SD5113        = ( HI_SYSBASE_KSOC | 0x00002000),
}hi_ksoc_e;

typedef enum  {
    HI_SUBMODULE_KSOC_MPCP          = ( HI_SRCMODULE_KSOC_SD5113 | 0x00000100),
    HI_SUBMODULE_KSOC_PLOAM         = ( HI_SRCMODULE_KSOC_SD5113 | 0x00000200),
    HI_SUBMODULE_KSOC_IGMP          = ( HI_SRCMODULE_KSOC_SD5113 | 0x00000300),  
}hi_ksoc_submod_e;

typedef enum 
{
    HI_SRCMODULE_UBASIC_DATABASE  = ( HI_SYSBASE_USERBASE | 0x00001000),
    HI_SRCMODULE_UBASIC_HLP       = ( HI_SYSBASE_USERBASE | 0x00002000),
    HI_SRCMODULE_UBASIC_IFM       = ( HI_SYSBASE_USERBASE | 0x00003000),
    HI_SRCMODULE_UBASIC_IFMSVR    = ( HI_SYSBASE_USERBASE | 0x00004000),
    HI_SRCMODULE_UBASIC_KLOG      = ( HI_SYSBASE_USERBASE | 0x00005000),
    HI_SRCMODULE_UBASIC_LOG       = ( HI_SYSBASE_USERBASE | 0x00006000),
    HI_SRCMODULE_UBASIC_LOGSVR    = ( HI_SYSBASE_USERBASE | 0x00007000),
    HI_SRCMODULE_UBASIC_RPC       = ( HI_SYSBASE_USERBASE | 0x00008000),
    HI_SRCMODULE_UBASIC_TIMER     = ( HI_SYSBASE_USERBASE | 0x00009000),
    HI_SRCMODULE_UBASIC_UOS       = ( HI_SYSBASE_USERBASE | 0x0000A000),
}hi_ubasic_e;

typedef enum 
{
    HI_SRCMODULE_VENDOR_CODEC     = ( HI_SYSBASE_VENDOR | 0x00001000),
    HI_SRCMODULE_VENDOR_LSW       = ( HI_SYSBASE_VENDOR | 0x00002000),
    HI_SRCMODULE_VENDOR_PHY       = ( HI_SYSBASE_VENDOR | 0x00003000),
    HI_SRCMODULE_VENDOR_SOC_SD5113= ( HI_SYSBASE_VENDOR | 0x00005000),
}hi_vendor_e;

typedef enum 
{
    HI_SUBMODULE_VENDOR_SOC_VOIP  = ( HI_SRCMODULE_VENDOR_SOC_SD5113 | 0x00000000),
    HI_SUBMODULE_VENDOR_SOC_EMAC  = ( HI_SRCMODULE_VENDOR_SOC_SD5113 | 0x00000500),
    HI_SUBMODULE_VENDOR_SOC_GMAC  = ( HI_SRCMODULE_VENDOR_SOC_SD5113 | 0x00000700),
    HI_SUBMODULE_VENDOR_SOC_LSW   = ( HI_SRCMODULE_VENDOR_SOC_SD5113 | 0x00000800),
}hi_vendor_submod_e;

typedef enum  {
    HI_SRCMODULE_SYSTEM_CLI       = ( HI_SYSBASE_SYSTEM | 0x00001000),
    HI_SRCMODULE_SYSTEM_INIT      = ( HI_SYSBASE_SYSTEM | 0x00002000),
}hi_system_e;

typedef enum 
{
    HI_SRCMODULE_APPS_OMCI        = ( HI_SYSBASE_APPS | 0x00001000),
    HI_SRCMODULE_APPS_OAM         = ( HI_SYSBASE_APPS | 0x00002000),
    HI_SRCMODULE_APPS_TOOLS       = ( HI_SYSBASE_APPS | 0x00003000),
}hi_apps_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SYSDEF_H__ */


