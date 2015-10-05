/* *****************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司
 
******************************************************************************
  文件名称: hi_drv_common.h
  功能描述: 海思驱动公共定义头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组    

  修改记录: 
***************************************************************************** */

#ifdef __cplusplus 
#if __cplusplus 
extern "C" { 
#endif /* __cpluscplus  */ 
#endif /* __cpluscplus  */

#ifndef  __HI_DRV_COMMON_H__
#define   __HI_DRV_COMMON_H__

#include "base_type_define.h"
#include "hi_type_define.h"

#include "sdk_autoconf.h"
#ifdef  CONFIG_HISI_SD5115_TYPE_T
#define HI_DP_CAP_PKT_CFG_SIZE  33
#else
#define HI_DP_CAP_PKT_CFG_SIZE  32
#endif

#define HI_BIT_0                           0X00000001
#define HI_BIT_1                           0X00000002
#define HI_BIT_2                           0X00000004
#define HI_BIT_3                           0X00000008
#define HI_BIT_4                           0X00000010
#define HI_BIT_5                           0X00000020
#define HI_BIT_6                           0X00000040
#define HI_BIT_7                           0X00000080
#define HI_BIT_8                           0X00000100
#define HI_BIT_9                           0X00000200

#define HI_BIT_10                         0X00000400
#define HI_BIT_11                         0X00000800
#define HI_BIT_12                         0X00001000
#define HI_BIT_13                         0X00002000
#define HI_BIT_14                         0X00004000
#define HI_BIT_15                         0X00008000
#define HI_BIT_16                         0X00010000
#define HI_BIT_17                         0X00020000
#define HI_BIT_18                         0X00040000
#define HI_BIT_19                         0X00080000

#define HI_BIT_20                         0X00100000
#define HI_BIT_21                         0X00200000
#define HI_BIT_22                         0X00400000
#define HI_BIT_23                         0X00800000
#define HI_BIT_24                         0X01000000
#define HI_BIT_25                         0X02000000
#define HI_BIT_26                         0X04000000
#define HI_BIT_27                         0X08000000
#define HI_BIT_28                         0X10000000
#define HI_BIT_29                         0X20000000

#define HI_BIT_30                         0X40000000
#define HI_BIT_31                         0X80000000


 
#define HI_BITS_LOW_1                0X00000001
#define HI_BITS_LOW_2                0X00000003
#define HI_BITS_LOW_3                0X00000007
#define HI_BITS_LOW_4                0X0000000F
#define HI_BITS_LOW_5                0X0000001F
#define HI_BITS_LOW_6                0X0000003F
#define HI_BITS_LOW_7                0X0000007F
#define HI_BITS_LOW_8                0X000000FF
#define HI_BITS_LOW_9                0X000001FF

#define HI_BITS_LOW_10              0X000003FF
#define HI_BITS_LOW_11              0X000007FF
#define HI_BITS_LOW_12              0X00000FFF
#define HI_BITS_LOW_13              0X00001FFF
#define HI_BITS_LOW_14              0X00003FFF
#define HI_BITS_LOW_15              0X00007FFF
#define HI_BITS_LOW_16              0X0000FFFF
#define HI_BITS_LOW_17              0X0001FFFF
#define HI_BITS_LOW_18              0X0003FFFF
#define HI_BITS_LOW_19              0X0007FFFF

#define HI_BITS_LOW_20              0X000FFFFF
#define HI_BITS_LOW_21              0X001FFFFF
#define HI_BITS_LOW_22              0X003FFFFF
#define HI_BITS_LOW_23              0X007FFFFF
#define HI_BITS_LOW_24              0X00FFFFFF
#define HI_BITS_LOW_25              0X01FFFFFF
#define HI_BITS_LOW_26              0X03FFFFFF
#define HI_BITS_LOW_27              0X07FFFFFF
#define HI_BITS_LOW_28              0X0FFFFFFF
#define HI_BITS_LOW_29              0X1FFFFFFF

#define HI_BITS_LOW_30              0X3FFFFFFF
#define HI_BITS_LOW_31              0X7FFFFFFF
#define HI_BITS_LOW_32              0XFFFFFFFF

/* 1:单BIT 操作 */
#define HI_BIT_GET(reg,bit)                ((0 ==  ((reg) & (bit)))  ? 0:1)
#define HI_BIT_SET(reg,bit)                ((reg) |= (bit)) 
#define HI_BIT_CLR(reg,bit)                ((reg) &= (~(bit))) 


#define HI_BIT_MODIFY(reg,bit,value)     ((0 == (value))  ? HI_BIT_CLR((reg),(bit)) :HI_BIT_SET((reg),(bit))  )
#define HI_POSITION()                  printk("\n\r  LINE = %d, FUNC = %s \n\r ", __LINE__, __func__)
#define HI_KDEBUG(fmt,arg...)         (HI_POSITION(),printk(fmt,##arg),printk(" \n\r"))
#define HI_EDGE_CHECK(arg)         ((HI_UP_EDGE_E == arg) || (HI_DOWN_EDGE_E == arg))
#define HI_LEVEL_CHECK(arg)        ((HI_HIGH_LEVEL_E == arg) || (HI_LOW_LEVEL_E == arg))
#define HI_BOOL_CHECK(arg)         ((HI_TRUE_E == arg) || (HI_FALSE_E == arg))


/*做pclint时，防止由于printk需要返回值而报错*/
#ifdef CONFIG_PCLINT_DEBUG
void printk(const char * fmt,...);
#endif

/* 设置寄存器中的多位 */
extern void hi_kernel_set_multi_bits(hi_v_u32 reg_addr, uint ui_value, uint ui_mask);

/* 清除寄存器中的多位 */
extern void hi_kernel_clr_multi_bits(hi_v_u32 reg_addr, uint ui_mask);


extern  void hi_kernel_invert_mac(const uchar *in_mac, uchar *out_mac);
extern  void hi_kernel_invert_ipv6(const uchar *in_ipv6, uchar *out_ipv6);
extern  void hi_kernel_invert_common(const uchar *in, uchar *out, uint length_byte);
extern  uint hi_kernel_ahb_clk_get(uint *pui_ahb_clk);

extern void hi_kernel_dp_mac_reg_get(hi_v_u32 *mac_h, hi_v_u32 *mac_l, uchar *mac);
extern void hi_kernel_dp_mac_reg_set(hi_v_u32 *mac_h, hi_v_u32 *mac_l, uchar *mac);




/* ******************************************************************************
  字节序转换宏定义   net <--> host
****************************************************************************** */
#ifdef   HI_ENDIAN_BIG
#define  HI_NTOHS(x)    (x)                         /* 大端到小端, 2 字节      */
#define  HI_HTONS(x)    (x)                         /* 小端到大端, 2 字节      */
#define  HI_NTOHL(x)    (x)                         /* 大端到小端, 4 字节      */
#define  HI_HTONL(x)    (x)                         /* 小端到大端, 4 字节      */
#else
#define HI_NTOHS(x)    ((((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8))
#define HI_HTONS(x)    HI_NTOHS(x)
#define HI_NTOHL(x)    ( (((x) & 0x000000FF) << 24) \
                        | (((x) & 0x0000FF00) << 8)  \
                        | (((x) & 0x00FF0000) >> 8)  \
                        | (((x) & 0xFF000000) >> 24) )
#define HI_HTONL(x)    HI_NTOHL(x)
#endif

typedef enum
{
    HI_CHIP_VERSION_110_E = 0,                              
    HI_CHIP_VERSION_100_E,
    HI_CHIP_VERSION_120_E, /* SD5113 V120 增加版本号 c00133939 20100712 */
    /* BEGIN: Added by shengjing, 2011/6/7   PN:DTS2011060104493*/
    HI_CHIP_VERSION_200_E
    /* END:   Added by shengjing, 2011/6/7 */    
} HI_CHIP_VERSION_E;

#define HI_DP_CHECK_IP_MASK(v4_mask, v6_mask)                \
do                                                           \
{                                                            \
    switch ( pst_item->em_ip_version )                       \
    {                                                        \
        case HI_DP_IP_V4_E :                                 \
            ui_mask_max = (v4_mask);                         \
            break;                                           \
        default:    /* HI_DP_IP_V6_E */                      \
            ui_mask_max = (v6_mask);                         \
            break;                                           \
    }                                                        \
    if(pst_item->ui_mask_range > ui_mask_max)                \
    {                                                        \
        HI_KDEBUG(" the input parameter em_ip_version(%d), ui_mask_range(%u)\n", pst_item->em_ip_version, pst_item->ui_mask_range); \
        return HI_RET_INVALID_PARAMETER;                     \
    }                                                        \
} while ( 0 )                                                \


#endif /* end of  */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */


