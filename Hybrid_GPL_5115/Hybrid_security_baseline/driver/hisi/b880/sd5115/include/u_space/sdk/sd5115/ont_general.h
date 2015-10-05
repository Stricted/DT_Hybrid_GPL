/*******************************************************************************
                  版权所有 (C), 2001-2011, 华为技术有限公司
********************************************************************************
  文 件 名   : ont_register.h
  版 本 号   : 初稿
  作    者   : g45544
  生成日期   : 2008年7月22日
  最近修改   :
  功能描述   : 提供底层软件通用宏和操作
  函数列表   :
  修改历史   :
  1.日    期   : 2008年7月22日
    作    者   : g45544
    修改内容   : 修改文件
*******************************************************************************/
#ifndef _ONT_GENERAL_H
#define _ONT_GENERAL_H

/*对于仅仅用于调试的代码，请使用该宏隔离，在正式版本中去除*/
//#define ONT_DRV_DEBUG_CODE


/*******************************************************************************
  字节序转换宏定义
*******************************************************************************/
#ifdef ONT_ENDIAN_BIG
#define ONT_NTOHS(x)    (x)                         /* 大端到小端, 2 字节     */
#define ONT_HTONS(x)    (x)                         /* 小端到大端, 2 字节     */
#define ONT_NTOHL(x)    (x)                         /* 大端到小端, 4 字节     */
#define ONT_HTONL(x)    (x)                         /* 小端到大端, 4 字节     */
#else
#define ONT_NTOHS(x)    ((((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8))
#define ONT_HTONS(x)    ONT_NTOHS(x)
#define ONT_NTOHL(x)    ( (((x) & 0x000000FF) << 24) \
                        | (((x) & 0x0000FF00) << 8)  \
                        | (((x) & 0x00FF0000) >> 8)  \
                        | (((x) & 0xFF000000) >> 24) )
#define ONT_HTONL(x)    ONT_NTOHL(x)
#endif


/*******************************************************************************
  寄存器操作
*******************************************************************************/
/* 输入的reg是寄存器的虚拟地址 */
#define ONT_READB(reg)          readb((reg))        /* 从寄存器读一个byte(8bit)  */
#define ONT_READW(reg)          readw((reg))        /* 从寄存器读一个word(16bit) */
#define ONT_READL(reg)          readl((reg))        /* 从寄存器读一个long(32bit) */

#define ONT_WRITEB(reg, value)  writeb((value), (reg))  /* 向寄存器写一个byte(8bit)  */
#define ONT_WRITEW(reg, value)  writew((value), (reg))  /* 向寄存器写一个word(16bit) */
#define ONT_WRITEL(reg, value)  writel((value), (reg))  /* 向寄存器写一个long(32bit) */

/* 定义的变量, 用于设置一个bit为1或0, 使用这些宏避免算错 */
#define ONT_BIT31_1     (0x00000001 << 31)
#define ONT_BIT30_1     (0x00000001 << 30)
#define ONT_BIT29_1     (0x00000001 << 29)
#define ONT_BIT28_1     (0x00000001 << 28)
#define ONT_BIT27_1     (0x00000001 << 27)
#define ONT_BIT26_1     (0x00000001 << 26)
#define ONT_BIT25_1     (0x00000001 << 25)
#define ONT_BIT24_1     (0x00000001 << 24)
#define ONT_BIT23_1     (0x00000001 << 23)
#define ONT_BIT22_1     (0x00000001 << 22)
#define ONT_BIT21_1     (0x00000001 << 21)
#define ONT_BIT20_1     (0x00000001 << 20)
#define ONT_BIT19_1     (0x00000001 << 19)
#define ONT_BIT18_1     (0x00000001 << 18)
#define ONT_BIT17_1     (0x00000001 << 17)
#define ONT_BIT16_1     (0x00000001 << 16)
#define ONT_BIT15_1     (0x00000001 << 15)
#define ONT_BIT14_1     (0x00000001 << 14)
#define ONT_BIT13_1     (0x00000001 << 13)
#define ONT_BIT12_1     (0x00000001 << 12)
#define ONT_BIT11_1     (0x00000001 << 11)
#define ONT_BIT10_1     (0x00000001 << 10)
#define ONT_BIT09_1     (0x00000001 << 9)
#define ONT_BIT08_1     (0x00000001 << 8)
#define ONT_BIT07_1     (0x00000001 << 7)
#define ONT_BIT06_1     (0x00000001 << 6)
#define ONT_BIT05_1     (0x00000001 << 5)
#define ONT_BIT04_1     (0x00000001 << 4)
#define ONT_BIT03_1     (0x00000001 << 3)
#define ONT_BIT02_1     (0x00000001 << 2)
#define ONT_BIT01_1     (0x00000001 << 1)
#define ONT_BIT00_1     (0x00000001)

#define ONT_BIT31_0     (~ONT_BIT31_1)
#define ONT_BIT30_0     (~ONT_BIT30_1)
#define ONT_BIT29_0     (~ONT_BIT29_1)
#define ONT_BIT28_0     (~ONT_BIT28_1)
#define ONT_BIT27_0     (~ONT_BIT27_1)
#define ONT_BIT26_0     (~ONT_BIT26_1)
#define ONT_BIT25_0     (~ONT_BIT25_1)
#define ONT_BIT24_0     (~ONT_BIT24_1)
#define ONT_BIT23_0     (~ONT_BIT23_1)
#define ONT_BIT22_0     (~ONT_BIT22_1)
#define ONT_BIT21_0     (~ONT_BIT21_1)
#define ONT_BIT20_0     (~ONT_BIT20_1)
#define ONT_BIT19_0     (~ONT_BIT19_1)
#define ONT_BIT18_0     (~ONT_BIT18_1)
#define ONT_BIT17_0     (~ONT_BIT17_1)
#define ONT_BIT16_0     (~ONT_BIT16_1)
#define ONT_BIT15_0     (~ONT_BIT15_1)
#define ONT_BIT14_0     (~ONT_BIT14_1)
#define ONT_BIT13_0     (~ONT_BIT13_1)
#define ONT_BIT12_0     (~ONT_BIT12_1)
#define ONT_BIT11_0     (~ONT_BIT11_1)
#define ONT_BIT10_0     (~ONT_BIT10_1)
#define ONT_BIT09_0     (~ONT_BIT09_1)
#define ONT_BIT08_0     (~ONT_BIT08_1)
#define ONT_BIT07_0     (~ONT_BIT07_1)
#define ONT_BIT06_0     (~ONT_BIT06_1)
#define ONT_BIT05_0     (~ONT_BIT05_1)
#define ONT_BIT04_0     (~ONT_BIT04_1)
#define ONT_BIT03_0     (~ONT_BIT03_1)
#define ONT_BIT02_0     (~ONT_BIT02_1)
#define ONT_BIT01_0     (~ONT_BIT01_1)
#define ONT_BIT00_0     (~ONT_BIT00_1)

#define ONT_BITMASK_LOW01   0x00000001
#define ONT_BITMASK_LOW02   0x00000003
#define ONT_BITMASK_LOW03   0x00000007
#define ONT_BITMASK_LOW04   0x0000000f
#define ONT_BITMASK_LOW05   0x0000001f
#define ONT_BITMASK_LOW06   0x0000003f
#define ONT_BITMASK_LOW07   0x0000007f
#define ONT_BITMASK_LOW08   0x000000ff
#define ONT_BITMASK_LOW09   0x000001ff
#define ONT_BITMASK_LOW10   0x000003ff
#define ONT_BITMASK_LOW11   0x000007ff
#define ONT_BITMASK_LOW12   0x00000fff
#define ONT_BITMASK_LOW13   0x00001fff
#define ONT_BITMASK_LOW14   0x00003fff
#define ONT_BITMASK_LOW15   0x00007fff
#define ONT_BITMASK_LOW16   0x0000ffff
#define ONT_BITMASK_LOW17   0x0001ffff
#define ONT_BITMASK_LOW18   0x0003ffff
#define ONT_BITMASK_LOW19   0x0007ffff
#define ONT_BITMASK_LOW20   0x000fffff
#define ONT_BITMASK_LOW21   0x001fffff
#define ONT_BITMASK_LOW22   0x003fffff
#define ONT_BITMASK_LOW23   0x007fffff
#define ONT_BITMASK_LOW24   0x00ffffff
#define ONT_BITMASK_LOW25   0x01ffffff
#define ONT_BITMASK_LOW26   0x03ffffff
#define ONT_BITMASK_LOW27   0x07ffffff
#define ONT_BITMASK_LOW28   0x0fffffff
#define ONT_BITMASK_LOW29   0x1fffffff
#define ONT_BITMASK_LOW30   0x3fffffff
#define ONT_BITMASK_LOW31   0x7fffffff
#define ONT_BITMASK_LOW32   0xffffffff


/*******************************************************************************
  misc设备的各个设备的次设备号
*******************************************************************************/
#define ONT_CODECSLIC_SENIOR_NUM    0x0001      /* CODECSLIC设备的次设备号 */
#define ONT_CAPTURE_SENIOR_NUM      0x0002      /* CAPTURE模块的次设备号   */
#define ONT_DELIVERY_SENIOR_NUM     0x0003      /* DELIVERY模块的次设备号  */
#define ONT_EAPOL_SENIOR_NUM        0x0004      /* EAPOL模块的次设备号     */
#define ONT_EVENT_SENIOR_NUM        0x0005      /* 事件管理模块的次设备号  */
#define ONT_FLASH_SENIOR_NUM        0x0006      /* FLASH的次设备号         */
#define ONT_GEFEMAC_SENIOR_NUM      0x0007      /* GEFEMAC的次设备号       */
#define ONT_GPONMAC_SENIOR_NUM      0x0008      /* GPONMAC的次设备号       */
#define ONT_GPIO_SENIOR_NUM         0x0009      /* GPIO的次设备号          */
#define ONT_I2C_SENIOR_NUM          0x000A      /* I2C设备的次设备号       */
#define ONT_IGMP_SENIOR_NUM         0x000B      /* IGMP模块的次设备号      */
#define ONT_LOG_SENIOR_NUM          0x000C      /* LOG模块的次设备号       */
#define ONT_LSW_SENIOR_NUM          0x000D      /* LSW的次设备号           */
#define ONT_LSWDEV_SENIOR_NUM       0x000E      /* LSWDEV的次设备号        */
#define ONT_MACLRN_SENIOR_NUM       0x000F      /* MACLRN模块的次设备号    */
#define ONT_MDIO_SENIOR_NUM         0x0010      /* MDIO设备的次设备号      */
#define ONT_NAPT_SENIOR_NUM         0x0011      /* NAPT模块的次设备号      */
#define ONT_OAM8023AH_SENIOR_NUM    0x0012      /* OAM8023AH模块的次设备号 */
#define ONT_OAM8021AG_SENIOR_NUM    0x0013      /* OAM8021AG模块的次设备号 */
#define ONT_OAMY1731_SENIOR_NUM     0x0014      /* OAMY1731模块的次设备号  */
#define ONT_OPTIC_SENIOR_NUM        0x0015      /* 光模块的次设备号        */
#define ONT_PLOAM_SENIOR_NUM        0x0016      /* PLOAM模块的次设备号     */
#define ONT_STPRSTP_SENIOR_NUM      0x0017      /* STP/RSTP模块的次设备号  */
#define ONT_TDM_SENIOR_NUM          0x0018      /* TDM的次设备号           */
#define ONT_UART_SENIOR_NUM         0x0019      /* UART设备的次设备号      */
#define ONT_VLANOPS_SENIOR_NUM      0x001A      /* VLANOPS模块的次设备号   */
#define ONT_VOIP_SENIOR_NUM         0x001B      /* VOIP的次设备号          */
#define ONT_WANDEV_SENIOR_NUM       0x001C      /* WANDEV的次设备号        */
#define ONT_WIFI_SENIOR_NUM         0x001D      /* WIFI的次设备号          */
#define ONT_SSP_SENIOR_NUM          0x001E      /* SSP的次设备号          */
#define ONT_LSWDEBUG_SENIOR_NUM     0x001F      /* LSWDEBUG的次设备号           */
#define ONT_EPONMAC_SENIOR_NUM      0x0020
#define ONT_MPCP_SENIOR_NUM         0x0021
#define ONT_STDOAM_SENIOR_NUM       0x0022
#define ONT_CTCOAM_SENIOR_NUM       0x0023
#define ONT_SYSINIT_SENIOR_NUM      0x0024      /* SYSINIT的次设备号       */
#define ONT_ADAPT_SENIOR_NUM        0x0025      /* ADAPT的次设备号       */

/* 不确定下面的设备号是否会使用, 先分配, 后续再定夺 */
#define ONT_HWIF_SENIOR_NUM         0x0030      /* HW-IF的次设备号         */
#define ONT_PCIB_SENIOR_NUM         0x0031      /* PCI桥的次设备号         */
#define ONT_RTC_SENIOR_NUM          0x0032      /* RTC的次设备号           */
#define ONT_WATCHDOG_SENIOR_NUM     0x0033      /* WatchDog的次设备号      */

#define ONT_INTERFACE_SENIOR_NUM    0x0034     
/*目前支持的最大的设备数目*/
#define ONT_MISCDEV_MAX_NUM         64


/*******************************************************************************
  各个设备在ONT中的序号
*******************************************************************************/
#define ONT_CODECSLIC_DRV_SN        1
#define ONT_DATABASE_DRV_SN         2
#define ONT_DELIVERY_DRV_SN         3
#define ONT_EAPOL_DRV_SN            4
#define ONT_EVENT_DRV_SN            5
#define ONT_FLASH_DRV_SN            6
#define ONT_GEFEMAC_DRV_SN          7
#define ONT_GMAC_DRV_SN             8
#define ONT_GPIO_DRV_SN             9
#define ONT_I2C_DRV_SN              10
#define ONT_IGMP_DRV_SN             11
#define ONT_LOG_DRV_SN              12
#define ONT_LSW_DRV_SN              13
#define ONT_LSWDEV_DRV_SN           14
#define ONT_MACLRN_DRV_SN           15
#define ONT_MDIO_DRV_SN             16
#define ONT_NAPT_DRV_SN             17
#define ONT_OAM8023AH_DRV_SN        18
#define ONT_OAM8021AG_DRV_SN        19
#define ONT_OAMY1731_DRV_SN         20
#define ONT_OPTIC_DRV_SN            21
#define ONT_PLOAM_DRV_SN            22
#define ONT_STPRSTP_DRV_SN          23
#define ONT_TDM_DRV_SN              24
#define ONT_UART_DRV_SN             25
#define ONT_VLANOPS_DRV_SN          26
#define ONT_VOIP_DRV_SN             27
#define ONT_WANDEV_DRV_SN           28
#define ONT_WIFI_DRV_SN             29
#define ONT_HWIF_DRV_SN             30
#define ONT_PCIB_DRV_SN             31
#define ONT_RTC_DRV_SN              32
#define ONT_WATCHDOG_DRV_SN         33
#define ONT_SSP_DRV_SN              34


/*******************************************************************************
  内核模块状态
*******************************************************************************/
enum ont_module_status_e
{
    ONT_MODULE_INSTALLED   = 0,                     /* 该模块已成功安装 */
    ONT_MODULE_UNINSTALLED = 1,                     /* 该模块还没有安装 */
    ONT_MODULE_UNDEFINED   = 0xff,                  /* 该模块未定义     */
};

/*
    ont_module_status[ONT_##DRV_SN]: 表示各个设备的状态, 已安装还是没有安
    装; 在设备成功初始化后设置为ONT_MODULE_INSTALED；当卸载或去初始化了设
    备后设置为ONT_MODULE_UNDEFINED;
*/
extern unsigned char ont_module_status[ONT_MISCDEV_MAX_NUM];


/*******************************************************************************
  设备占用的中断号
*******************************************************************************/
/*这里定义各个设备占用的中断号*/
/*PCI桥的异常中断号*/
/*#define ONT_PCID_INTREPT_ID  0x00000*/


/*******************************************************************************
  寄存器操作函数
*******************************************************************************/
extern void ONT_SETONEBITB(unsigned int reg, unsigned char offset);
extern void ONT_SETONEBITW(unsigned int reg, unsigned char offset);
extern void ONT_SETONEBITL(unsigned int reg, unsigned char offset);
extern void ONT_CLRONEBITB(unsigned int reg, unsigned char offset);
extern void ONT_CLRONEBITW(unsigned int reg, unsigned char offset);
extern void ONT_CLRONEBITL(unsigned int reg, unsigned char offset);
extern void ONT_SETMULTIBITW(unsigned int reg, unsigned short value, unsigned short bitmask);
extern void ONT_SETMULTIBITL(unsigned int reg, unsigned int value, unsigned int bitmask);
extern unsigned int ONT_READONEBITL(unsigned int reg, unsigned char offset);
extern unsigned int ONT_READONEBITW(unsigned int reg, unsigned char offset);

#endif
