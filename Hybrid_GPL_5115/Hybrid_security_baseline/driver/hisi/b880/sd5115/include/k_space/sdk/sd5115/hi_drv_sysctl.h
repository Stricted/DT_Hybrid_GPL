/******************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司

******************************************************************************
  文件名称: hi_drv_sysctl.h
  功能描述: 驱动头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组

  修改记录:
******************************************************************************/
#ifndef __HI_DRV_SYSCTL_H__
#define __HI_DRV_SYSCTL_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*SYS控制器 工作模式*/
typedef enum
{
    HI_CHIP_SLEEP_MODE_E = 0x0,
    HI_CHIP_DOZE_MODE_E   = 0x1,
    HI_CHIP_SLOW_MODE_E   = 0x2,
    HI_CHIP_NORMAL_MODE_E = 0x4
} HI_CHIP_WORK_MODE_E;

/*
SLEEP工作模式：此时除系统控制器工作在25MHz的时钟下，芯片内ECS子系统的其他模块的时钟都处于关闭状态。
DOZE工作模式：此时芯片ECS子系统的时钟来自25MHz的时钟。
SLOW工作模式：与DOZE工作模式相同，ECS子系统的工作时钟来自25MHz晶振时钟。
NORMAL工作模式：此时芯片ECS子系统的时钟工作在最高频率，来自PLL输出的250MHz。
 */

/*温度补偿*/
typedef enum
{
    HI_SYSCTL_TEMP_NO_ADJ_E = 0x0,               /*无补偿*/
    HI_SYSCTL_TEMP_DEC_4_E,                      /*减4度*/
    HI_SYSCTL_TEMP_DEC_8_E,                     /*减8度*/
    HI_SYSCTL_TEMP_DEC_12_E,                     /*减12度*/
    HI_SYSCTL_TEMP_ADD_4_E,                      /*加4度*/
    HI_SYSCTL_TEMP_ADD_8_E,                     /*加8度*/
    HI_SYSCTL_TEMP_ADD_12_E,                     /*加12度*/
    HI_SYSCTL_TEMP_ADD_16_E                   /*加16度*/
} HI_SYSCTL_TEMP_ADJ_E;

/*硬复位类型*/
typedef enum
{
    HI_SYSCTL_RESET_POWER_E = 0x0,               /*上一次硬件复位是上电复位*/
    HI_SYSCTL_RESET_SYS_RESET_N_E,               /*上一次硬件复位是通过管脚SYS_RESET_N输入的复位*/
    HI_SYSCTL_RESET_WATCHDOG_E,                  /*上一次硬件复位是看门狗触发的复位*/
} HI_SYSCTL_HARD_RESET_TYPE_E;

/*温度*/
typedef enum
{
    HI_SYSCTL_TEMP_UNDER_N20_E = 0x0,            /*低于 －20度*/
    HI_SYSCTL_TEMP_N20_TO_0_E = 0x1,            /* －20度 到0度之间*/
    HI_SYSCTL_TEMP_20_TO_40_E = 0x2,             /* 20度 到 40度之间*/
    HI_SYSCTL_TEMP_0_TO_20_E    = 0x3,           /* 0度 到 20度之间*/
    HI_SYSCTL_TEMP_90_TO_95_E   = 0x4,           /* 90度 到 95度之间*/
    HI_SYSCTL_TEMP_80_TO_90_E   = 0x5,           /* 80度 到 90度之间*/
    HI_SYSCTL_TEMP_40_TO_60_E   = 0x6,           /* 40度 到60度之间*/
    HI_SYSCTL_TEMP_60_TO_80_E   = 0x7,           /* 60度 到 80度之间*/
    HI_SYSCTL_TEMP_OVER_145_E   = 0x8,           /* 高于145度*/
    HI_SYSCTL_TEMP_125_TO_145_E = 0x9,           /* 125度 到 145度之间*/
    HI_SYSCTL_TEMP_115_TO_120_E = 0xa,            /* 115度 到 120度之间*/
    HI_SYSCTL_TEMP_120_TO_125_E = 0xb,           /* 120度 到 125度之间*/
    HI_SYSCTL_TEMP_95_TO_100_E  = 0xc,            /* 95度 到 100度之间*/
    HI_SYSCTL_TEMP_100_TO_105_E = 0xd,             /* 100度 到 105度之间*/
    HI_SYSCTL_TEMP_110_TO_115_E = 0xe,             /* 110度 到 115度之间*/
    HI_SYSCTL_TEMP_105_TO_110_E = 0xf,             /* 105度 到 110度之间*/
} HI_SYSCTL_TEMP_E;

/*用于获取LDOC2 的输出电压*/
typedef enum
{
    HI_SYSCTL_LDOC2_VOUT_1V00_E = 0x0,     /*输出1.00V*/
    HI_SYSCTL_LDOC2_VOUT_1V05_E,           /*输出1.05V*/
    HI_SYSCTL_LDOC2_VOUT_1V10_E,        /*输出1.10V*/
    HI_SYSCTL_LDOC2_VOUT_1V15_E,        /*输出1.15V*/
    HI_SYSCTL_LDOC2_VOUT_1V20_E,        /*输出1.20V*/
    HI_SYSCTL_LDOC2_VOUT_1V25_E,        /*输出1.25V*/
    HI_SYSCTL_LDOC2_VOUT_1V30_E,        /*输出1.30V*/
    H_SYSCTLI_LDOC2_VOUT_1V50_E,        /*输出1.50V*/
    HI_SYSCTL_LDOC2_VOUT_NONE_E            /*不支持*/
} HI_SYSCTL_LD0C2_VOUT_E;

/*用于获取LDOC2的输出使能状态*/
typedef enum
{
    HI_SYSCTL_LDOC2_DISABLE_E = 0x0,  /*禁止LDOC2 的输出*/
    HI_SYSCTL_LDOC2_ENABLE_E,         /*使能LDOC2 的输出*/
    HI_SYSCTL_LDOC2_NONE_E            /*不支持获取此状态*/
} HI_SYSCTL_LD0C2_EN_TYPE_E;

/*启动FLASH的地址宽度*/
typedef enum
{
    HI_SYSCTL_SF_ADDR_WIDTH_3_BIT_E = 0x0,       /*SPI FLASH地址宽度为3BYTE*/
    HI_SYSCTL_SF_ADDR_WIDTH_4_BIT_E,       /*SPI FLASH地址宽度为4BYTE*/
    HI_SYSCTL_SF_ADDR_WIDTH_NONE_E
} HI_SYSCTL_SF_ADDR_WIDTH_TYPE_E;

/*启动FLASH的地址宽度*/
typedef enum
{
    HI_SYSCTL_NF_ADDR_WIDTH_4_BIT_E = 0x0, /*NAND FLASH地址宽度为4BYTE*/
    HI_SYSCTL_NF_ADDR_WIDTH_5_BIT_E,       /*NAND FLASH地址宽度为5BYTE*/
    HI_SYSCTL_NF_ADDR_WIDTH_NONE_E
} HI_SYSCTL_NF_ADDR_WIDTH_TYPE_E;

/*看门狗*/
typedef enum
{
    HI_SYSCTL_WDG_DISABLE_E = 0x0, /*看门狗关闭*/
    HI_SYSCTL_WDG_ENABLE_E,        /*看门狗使能*/
    HI_SYSCTL_WDG_NONE_E           /*不支持获取此状态*/
} HI_SYSCTL_WDG_EN_TYPE_E;

/*NAND FLASH的块大小*/
typedef enum
{
    HI_SYSCTL_NF_BLK_SIZE_64_PAGE_E = 0x0,  /*NAND FLASH的每个BLOCK64PAGE*/
    HI_SYSCTL_NF_BLK_SIZE_128_PAGE_E,       /*NAND FLASH的每个BLOCK128PAGE*/
    HI_SYSCTL_NF_BLK_SIZE_NONE_E
} HI_SYSCTL_NF_BLK_SIZE_TYPE_E;

/*NAND FLASH类型*/
typedef enum
{
    HI_SYSCTL_NF_BOOT_PIN_512BYTE_PAGE_1BIT_ECC_E = 0x0,  /*NAND FLASH类型512Byte page size,1bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_1BIT_ECC_E,            /*NAND FLASH类型2k page size,1bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_1BIT_ECC_E,            /*NAND FLASH类型4k page size,1bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_4BIT_ECC_E,            /*NAND FLASH类型2k page size,4bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_4BIT_ECC_E,            /*NAND FLASH类型4k page size,4bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_8BIT_ECC_E,            /*NAND FLASH类型2k page size,8bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_8BIT_ECC_E,            /*NAND FLASH类型4k page size,8bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_24BIT_ECC_E,           /*NAND FLASH类型2k page size,24bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_24BIT_ECC_E,           /*NAND FLASH类型4k page size,24bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_8KB_PAGE_24BIT_ECC_E,           /*NAND FLASH类型8k page size,24bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_NONE_E                       /*保留*/
} HI_SYSCTL_NF_BOOT_PIN_TYPE_E;

/*启动方式类型*/
typedef enum
{
    HI_SYSCTL_BOOT_INSIDE_E = 0x0,  /*启动方式为从内部BOOT启动*/
    HI_SYSCTL_BOOT_OUTSIDE_E,       /*启动方式为从外部FLASH启动*/
    HI_SYSCTL_BOOT_NONE_E
} HI_SYSCTL_BOOT_TYPE_E;

/*FLASH类型*/
typedef enum
{
    HI_SYSCTL_BOOT_FLASH_SPI_E = 0x0,   /*FLASH类型为NAND FLASH*/
    HI_SYSCTL_BOOT_FLASH_NAND_E,        /*FLASH类型为SPI FLASH*/
    HI_SYSCTL_BOOT_FLASH_NONE_E
} HI_SYSCTL_BOOT_FLASH_TYPE_E;

/*硬件配置字*/
typedef struct
{
    HI_SYSCTL_BOOT_TYPE_E          em_boot_type; /*启动方式,判断是从Bootrom启动，还是从FLASH启动*/
    HI_SYSCTL_BOOT_FLASH_TYPE_E    em_flash_type;      /*如果是从FLASH启动，判断是从SPI FLASH启动，还是从NAND FLASH启动*/
    HI_SYSCTL_SF_ADDR_WIDTH_TYPE_E em_sf_addr_width;   /*启动SPI FLASH地址宽度*/
    HI_SYSCTL_NF_ADDR_WIDTH_TYPE_E em_nf_addr_width;   /*启动NAND FLASH地址宽度*/
    HI_SYSCTL_NF_BOOT_PIN_TYPE_E   em_nf_type;         /*NF类型*/
    HI_SYSCTL_NF_BLK_SIZE_TYPE_E   em_nf_block_size;   /*NF的块大小*/
    HI_SYSCTL_WDG_EN_TYPE_E        em_wdg_en;          /*看门狗使能状态， pilot-s可以获取，pilot-h不可以获取*/
    HI_SYSCTL_LD0C2_EN_TYPE_E      em_ldoc2_en_type;   /*用于pilot-h,用于获取LDOC2的输出使能状态*/
    HI_SYSCTL_LD0C2_VOUT_E         em_ldoc2_vout;      /*对于pilot-h,用于获取LDOC2的输出电压值*/
} hi_sysctl_hw_cw_s;

/*软件配置字*/
typedef union
{
    hi_v_u32 value;
    struct
    {
        hi_v_u32 hwcw_sw_0 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_1 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_2 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_3 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_4 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_5 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_6 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_7 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_8 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_9 : 1;             /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_10 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_11 : 1;      /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_12 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_13 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_14 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_15 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_16 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_17 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_18 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_19 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_20 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_21 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_22 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_23 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_24 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_25 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_26 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_27 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_28 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_29 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_30 : 1;            /*自定义软件配置字*/
        hi_v_u32 hwcw_sw_31 : 1;      /*自定义软件配置字*/
    } bits;
} hi_sysctl_sw_cw_u;

typedef enum
{
    HI_SYSCTL_CHIP_ID_NONE_E = 0x0,
    HI_SYSCTL_CHIP_ID_S_E,
    HI_SYSCTL_CHIP_ID_H_E,
    HI_SYSCTL_CHIP_ID_T_E,
    HI_SYSCTL_CHIP_ID_MPW_E,
} HI_SYSCTL_CHIP_ID_E;

typedef enum
{
    HI_USB_CLK_OFF = 0,
    HI_USB_CLK_ON
} HI_USB_CLK_CTRL;

/*配置属性*/
typedef struct
{
    HI_CHIP_WORK_MODE_E  em_chip_work_mode;       /*工作模式*/
    HI_BOOL_E            em_temp_sensor_en;       /*温度传感器使能*/
    HI_SYSCTL_TEMP_ADJ_E em_temp_adj;             /*温度补偿*/
    HI_USB_CLK_CTRL      usb_clk_ctrl;            /*USB时钟开关属性：打开或关断*/
    HI_BOOL_E            em_console_en;
    HI_BOOL_E            em_jtag_en;
} hi_sysctl_attr_s;

#define HI_SOFTWARE_VERSION_LEN 64

/*状态查询*/
typedef struct
{
    /*芯片版本相关信息*/
    uint                        ui_year;           /*DATE*/
    uint                        ui_month;
    uint                        ui_day;
    uint                        ui_chip_id;        /*32'h51150100*/
    uint                        ui_base_line;      /*基线版本号*/
    uint                        ui_sub_version;    /*子版本号*/
    uint                        ui_vendor_id;      /*厂商ID*/
    HI_SYSCTL_HARD_RESET_TYPE_E em_reset_type;     /*上一次硬件复位类型*/
    uchar                       uc_softare_version[HI_SOFTWARE_VERSION_LEN]; /* 软件版本 */
    HI_BOOL_E                   em_usb1_link;      /* USB 端口1连接状态 */
    HI_BOOL_E                   em_usb2_link;      /* USB 端口2连接状态 */
    hi_sysctl_hw_cw_s           st_hi_sysctl_hw_cw;     /*硬件配置字*/
    hi_sysctl_sw_cw_u           u_hi_sysctl_sw_cw;       /*软件件配置字*/
} hi_sysctl_sta_s;

/*统计，异常记录等*/
typedef struct
{
    uint ui_invalid_parameter_cnt;               /*无效参数计数*/
    uint ui_internal_err;
} hi_sysctl_cnt_s;

/*配置属性操作*/
extern uint  hi_kernel_sysctl_attr_get(hi_sysctl_attr_s *pst_attr);

extern uint  hi_kernel_sysctl_attr_set(hi_sysctl_attr_s *pst_attr);

/*状态信息操作*/
extern uint  hi_kernel_sysctl_sta_get(hi_sysctl_sta_s *pst_sta);

/*统计信息操作*/
extern uint  hi_kernel_sysctl_cnt_get(hi_sysctl_cnt_s *pst_cnt);

/*复位系统*/
extern uint  hi_kernel_sysctl_reset(void);

/*温度获取*/
extern uint  hi_kernel_sysctl_temp_get(HI_SYSCTL_TEMP_E *em_temp);

/*芯片版本获取*/
extern uint  hi_kernel_sysctl_chip_id_get(HI_SYSCTL_CHIP_ID_E *em_chip_id);

extern uint  hi_kernel_sysctl_physic_addr_get(void *get_info);

extern uint  hi_kernel_sysctl_physic_addr_set(void *get_info);

#endif /* __HI_DRV_XXX_H__ */
