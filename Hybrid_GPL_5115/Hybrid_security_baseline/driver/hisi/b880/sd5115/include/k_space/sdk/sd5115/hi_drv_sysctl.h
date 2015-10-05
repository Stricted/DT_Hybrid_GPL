/******************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

******************************************************************************
  �ļ�����: hi_drv_sysctl.h
  ��������: ����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������

  �޸ļ�¼:
******************************************************************************/
#ifndef __HI_DRV_SYSCTL_H__
#define __HI_DRV_SYSCTL_H__

#include "hi_drv_common.h"
#include "base_type_define.h"
#include "hi_type_define.h"

/*SYS������ ����ģʽ*/
typedef enum
{
    HI_CHIP_SLEEP_MODE_E = 0x0,
    HI_CHIP_DOZE_MODE_E   = 0x1,
    HI_CHIP_SLOW_MODE_E   = 0x2,
    HI_CHIP_NORMAL_MODE_E = 0x4
} HI_CHIP_WORK_MODE_E;

/*
SLEEP����ģʽ����ʱ��ϵͳ������������25MHz��ʱ���£�оƬ��ECS��ϵͳ������ģ���ʱ�Ӷ����ڹر�״̬��
DOZE����ģʽ����ʱоƬECS��ϵͳ��ʱ������25MHz��ʱ�ӡ�
SLOW����ģʽ����DOZE����ģʽ��ͬ��ECS��ϵͳ�Ĺ���ʱ������25MHz����ʱ�ӡ�
NORMAL����ģʽ����ʱоƬECS��ϵͳ��ʱ�ӹ��������Ƶ�ʣ�����PLL�����250MHz��
 */

/*�¶Ȳ���*/
typedef enum
{
    HI_SYSCTL_TEMP_NO_ADJ_E = 0x0,               /*�޲���*/
    HI_SYSCTL_TEMP_DEC_4_E,                      /*��4��*/
    HI_SYSCTL_TEMP_DEC_8_E,                     /*��8��*/
    HI_SYSCTL_TEMP_DEC_12_E,                     /*��12��*/
    HI_SYSCTL_TEMP_ADD_4_E,                      /*��4��*/
    HI_SYSCTL_TEMP_ADD_8_E,                     /*��8��*/
    HI_SYSCTL_TEMP_ADD_12_E,                     /*��12��*/
    HI_SYSCTL_TEMP_ADD_16_E                   /*��16��*/
} HI_SYSCTL_TEMP_ADJ_E;

/*Ӳ��λ����*/
typedef enum
{
    HI_SYSCTL_RESET_POWER_E = 0x0,               /*��һ��Ӳ����λ���ϵ縴λ*/
    HI_SYSCTL_RESET_SYS_RESET_N_E,               /*��һ��Ӳ����λ��ͨ���ܽ�SYS_RESET_N����ĸ�λ*/
    HI_SYSCTL_RESET_WATCHDOG_E,                  /*��һ��Ӳ����λ�ǿ��Ź������ĸ�λ*/
} HI_SYSCTL_HARD_RESET_TYPE_E;

/*�¶�*/
typedef enum
{
    HI_SYSCTL_TEMP_UNDER_N20_E = 0x0,            /*���� ��20��*/
    HI_SYSCTL_TEMP_N20_TO_0_E = 0x1,            /* ��20�� ��0��֮��*/
    HI_SYSCTL_TEMP_20_TO_40_E = 0x2,             /* 20�� �� 40��֮��*/
    HI_SYSCTL_TEMP_0_TO_20_E    = 0x3,           /* 0�� �� 20��֮��*/
    HI_SYSCTL_TEMP_90_TO_95_E   = 0x4,           /* 90�� �� 95��֮��*/
    HI_SYSCTL_TEMP_80_TO_90_E   = 0x5,           /* 80�� �� 90��֮��*/
    HI_SYSCTL_TEMP_40_TO_60_E   = 0x6,           /* 40�� ��60��֮��*/
    HI_SYSCTL_TEMP_60_TO_80_E   = 0x7,           /* 60�� �� 80��֮��*/
    HI_SYSCTL_TEMP_OVER_145_E   = 0x8,           /* ����145��*/
    HI_SYSCTL_TEMP_125_TO_145_E = 0x9,           /* 125�� �� 145��֮��*/
    HI_SYSCTL_TEMP_115_TO_120_E = 0xa,            /* 115�� �� 120��֮��*/
    HI_SYSCTL_TEMP_120_TO_125_E = 0xb,           /* 120�� �� 125��֮��*/
    HI_SYSCTL_TEMP_95_TO_100_E  = 0xc,            /* 95�� �� 100��֮��*/
    HI_SYSCTL_TEMP_100_TO_105_E = 0xd,             /* 100�� �� 105��֮��*/
    HI_SYSCTL_TEMP_110_TO_115_E = 0xe,             /* 110�� �� 115��֮��*/
    HI_SYSCTL_TEMP_105_TO_110_E = 0xf,             /* 105�� �� 110��֮��*/
} HI_SYSCTL_TEMP_E;

/*���ڻ�ȡLDOC2 �������ѹ*/
typedef enum
{
    HI_SYSCTL_LDOC2_VOUT_1V00_E = 0x0,     /*���1.00V*/
    HI_SYSCTL_LDOC2_VOUT_1V05_E,           /*���1.05V*/
    HI_SYSCTL_LDOC2_VOUT_1V10_E,        /*���1.10V*/
    HI_SYSCTL_LDOC2_VOUT_1V15_E,        /*���1.15V*/
    HI_SYSCTL_LDOC2_VOUT_1V20_E,        /*���1.20V*/
    HI_SYSCTL_LDOC2_VOUT_1V25_E,        /*���1.25V*/
    HI_SYSCTL_LDOC2_VOUT_1V30_E,        /*���1.30V*/
    H_SYSCTLI_LDOC2_VOUT_1V50_E,        /*���1.50V*/
    HI_SYSCTL_LDOC2_VOUT_NONE_E            /*��֧��*/
} HI_SYSCTL_LD0C2_VOUT_E;

/*���ڻ�ȡLDOC2�����ʹ��״̬*/
typedef enum
{
    HI_SYSCTL_LDOC2_DISABLE_E = 0x0,  /*��ֹLDOC2 �����*/
    HI_SYSCTL_LDOC2_ENABLE_E,         /*ʹ��LDOC2 �����*/
    HI_SYSCTL_LDOC2_NONE_E            /*��֧�ֻ�ȡ��״̬*/
} HI_SYSCTL_LD0C2_EN_TYPE_E;

/*����FLASH�ĵ�ַ���*/
typedef enum
{
    HI_SYSCTL_SF_ADDR_WIDTH_3_BIT_E = 0x0,       /*SPI FLASH��ַ���Ϊ3BYTE*/
    HI_SYSCTL_SF_ADDR_WIDTH_4_BIT_E,       /*SPI FLASH��ַ���Ϊ4BYTE*/
    HI_SYSCTL_SF_ADDR_WIDTH_NONE_E
} HI_SYSCTL_SF_ADDR_WIDTH_TYPE_E;

/*����FLASH�ĵ�ַ���*/
typedef enum
{
    HI_SYSCTL_NF_ADDR_WIDTH_4_BIT_E = 0x0, /*NAND FLASH��ַ���Ϊ4BYTE*/
    HI_SYSCTL_NF_ADDR_WIDTH_5_BIT_E,       /*NAND FLASH��ַ���Ϊ5BYTE*/
    HI_SYSCTL_NF_ADDR_WIDTH_NONE_E
} HI_SYSCTL_NF_ADDR_WIDTH_TYPE_E;

/*���Ź�*/
typedef enum
{
    HI_SYSCTL_WDG_DISABLE_E = 0x0, /*���Ź��ر�*/
    HI_SYSCTL_WDG_ENABLE_E,        /*���Ź�ʹ��*/
    HI_SYSCTL_WDG_NONE_E           /*��֧�ֻ�ȡ��״̬*/
} HI_SYSCTL_WDG_EN_TYPE_E;

/*NAND FLASH�Ŀ��С*/
typedef enum
{
    HI_SYSCTL_NF_BLK_SIZE_64_PAGE_E = 0x0,  /*NAND FLASH��ÿ��BLOCK64PAGE*/
    HI_SYSCTL_NF_BLK_SIZE_128_PAGE_E,       /*NAND FLASH��ÿ��BLOCK128PAGE*/
    HI_SYSCTL_NF_BLK_SIZE_NONE_E
} HI_SYSCTL_NF_BLK_SIZE_TYPE_E;

/*NAND FLASH����*/
typedef enum
{
    HI_SYSCTL_NF_BOOT_PIN_512BYTE_PAGE_1BIT_ECC_E = 0x0,  /*NAND FLASH����512Byte page size,1bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_1BIT_ECC_E,            /*NAND FLASH����2k page size,1bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_1BIT_ECC_E,            /*NAND FLASH����4k page size,1bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_4BIT_ECC_E,            /*NAND FLASH����2k page size,4bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_4BIT_ECC_E,            /*NAND FLASH����4k page size,4bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_8BIT_ECC_E,            /*NAND FLASH����2k page size,8bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_8BIT_ECC_E,            /*NAND FLASH����4k page size,8bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_2KB_PAGE_24BIT_ECC_E,           /*NAND FLASH����2k page size,24bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_4KB_PAGE_24BIT_ECC_E,           /*NAND FLASH����4k page size,24bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_8KB_PAGE_24BIT_ECC_E,           /*NAND FLASH����8k page size,24bit ecc*/
    HI_SYSCTL_NF_BOOT_PIN_NONE_E                       /*����*/
} HI_SYSCTL_NF_BOOT_PIN_TYPE_E;

/*������ʽ����*/
typedef enum
{
    HI_SYSCTL_BOOT_INSIDE_E = 0x0,  /*������ʽΪ���ڲ�BOOT����*/
    HI_SYSCTL_BOOT_OUTSIDE_E,       /*������ʽΪ���ⲿFLASH����*/
    HI_SYSCTL_BOOT_NONE_E
} HI_SYSCTL_BOOT_TYPE_E;

/*FLASH����*/
typedef enum
{
    HI_SYSCTL_BOOT_FLASH_SPI_E = 0x0,   /*FLASH����ΪNAND FLASH*/
    HI_SYSCTL_BOOT_FLASH_NAND_E,        /*FLASH����ΪSPI FLASH*/
    HI_SYSCTL_BOOT_FLASH_NONE_E
} HI_SYSCTL_BOOT_FLASH_TYPE_E;

/*Ӳ��������*/
typedef struct
{
    HI_SYSCTL_BOOT_TYPE_E          em_boot_type; /*������ʽ,�ж��Ǵ�Bootrom���������Ǵ�FLASH����*/
    HI_SYSCTL_BOOT_FLASH_TYPE_E    em_flash_type;      /*����Ǵ�FLASH�������ж��Ǵ�SPI FLASH���������Ǵ�NAND FLASH����*/
    HI_SYSCTL_SF_ADDR_WIDTH_TYPE_E em_sf_addr_width;   /*����SPI FLASH��ַ���*/
    HI_SYSCTL_NF_ADDR_WIDTH_TYPE_E em_nf_addr_width;   /*����NAND FLASH��ַ���*/
    HI_SYSCTL_NF_BOOT_PIN_TYPE_E   em_nf_type;         /*NF����*/
    HI_SYSCTL_NF_BLK_SIZE_TYPE_E   em_nf_block_size;   /*NF�Ŀ��С*/
    HI_SYSCTL_WDG_EN_TYPE_E        em_wdg_en;          /*���Ź�ʹ��״̬�� pilot-s���Ի�ȡ��pilot-h�����Ի�ȡ*/
    HI_SYSCTL_LD0C2_EN_TYPE_E      em_ldoc2_en_type;   /*����pilot-h,���ڻ�ȡLDOC2�����ʹ��״̬*/
    HI_SYSCTL_LD0C2_VOUT_E         em_ldoc2_vout;      /*����pilot-h,���ڻ�ȡLDOC2�������ѹֵ*/
} hi_sysctl_hw_cw_s;

/*���������*/
typedef union
{
    hi_v_u32 value;
    struct
    {
        hi_v_u32 hwcw_sw_0 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_1 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_2 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_3 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_4 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_5 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_6 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_7 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_8 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_9 : 1;             /*�Զ������������*/
        hi_v_u32 hwcw_sw_10 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_11 : 1;      /*�Զ������������*/
        hi_v_u32 hwcw_sw_12 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_13 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_14 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_15 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_16 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_17 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_18 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_19 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_20 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_21 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_22 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_23 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_24 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_25 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_26 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_27 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_28 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_29 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_30 : 1;            /*�Զ������������*/
        hi_v_u32 hwcw_sw_31 : 1;      /*�Զ������������*/
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

/*��������*/
typedef struct
{
    HI_CHIP_WORK_MODE_E  em_chip_work_mode;       /*����ģʽ*/
    HI_BOOL_E            em_temp_sensor_en;       /*�¶ȴ�����ʹ��*/
    HI_SYSCTL_TEMP_ADJ_E em_temp_adj;             /*�¶Ȳ���*/
    HI_USB_CLK_CTRL      usb_clk_ctrl;            /*USBʱ�ӿ������ԣ��򿪻�ض�*/
    HI_BOOL_E            em_console_en;
    HI_BOOL_E            em_jtag_en;
} hi_sysctl_attr_s;

#define HI_SOFTWARE_VERSION_LEN 64

/*״̬��ѯ*/
typedef struct
{
    /*оƬ�汾�����Ϣ*/
    uint                        ui_year;           /*DATE*/
    uint                        ui_month;
    uint                        ui_day;
    uint                        ui_chip_id;        /*32'h51150100*/
    uint                        ui_base_line;      /*���߰汾��*/
    uint                        ui_sub_version;    /*�Ӱ汾��*/
    uint                        ui_vendor_id;      /*����ID*/
    HI_SYSCTL_HARD_RESET_TYPE_E em_reset_type;     /*��һ��Ӳ����λ����*/
    uchar                       uc_softare_version[HI_SOFTWARE_VERSION_LEN]; /* ����汾 */
    HI_BOOL_E                   em_usb1_link;      /* USB �˿�1����״̬ */
    HI_BOOL_E                   em_usb2_link;      /* USB �˿�2����״̬ */
    hi_sysctl_hw_cw_s           st_hi_sysctl_hw_cw;     /*Ӳ��������*/
    hi_sysctl_sw_cw_u           u_hi_sysctl_sw_cw;       /*�����������*/
} hi_sysctl_sta_s;

/*ͳ�ƣ��쳣��¼��*/
typedef struct
{
    uint ui_invalid_parameter_cnt;               /*��Ч��������*/
    uint ui_internal_err;
} hi_sysctl_cnt_s;

/*�������Բ���*/
extern uint  hi_kernel_sysctl_attr_get(hi_sysctl_attr_s *pst_attr);

extern uint  hi_kernel_sysctl_attr_set(hi_sysctl_attr_s *pst_attr);

/*״̬��Ϣ����*/
extern uint  hi_kernel_sysctl_sta_get(hi_sysctl_sta_s *pst_sta);

/*ͳ����Ϣ����*/
extern uint  hi_kernel_sysctl_cnt_get(hi_sysctl_cnt_s *pst_cnt);

/*��λϵͳ*/
extern uint  hi_kernel_sysctl_reset(void);

/*�¶Ȼ�ȡ*/
extern uint  hi_kernel_sysctl_temp_get(HI_SYSCTL_TEMP_E *em_temp);

/*оƬ�汾��ȡ*/
extern uint  hi_kernel_sysctl_chip_id_get(HI_SYSCTL_CHIP_ID_E *em_chip_id);

extern uint  hi_kernel_sysctl_physic_addr_get(void *get_info);

extern uint  hi_kernel_sysctl_physic_addr_set(void *get_info);

#endif /* __HI_DRV_XXX_H__ */
