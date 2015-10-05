/* *****************************************************************************
          版权所有 (C), 2008-2018, 海思半导体有限公司
 
******************************************************************************
  文件名称: hi_type_define.h
  功能描述: 海思驱动数据类型和返回值定义头文件
  版本描述: V1.0

  创建日期: 2008年12月8日
  创建作者: 海思驱动开发组    

  修改记录: 
***************************************************************************** */
#ifdef __cplusplus 
#if __cplusplus 
extern "C" 
{ 
#endif /* __cpluscplus */ 
#endif /* __cpluscplus */

#ifndef  __HI_TYPE_DEFINE_H__
#define  __HI_TYPE_DEFINE_H__

/* 基本数据类型定义如下 */
typedef volatile unsigned char         hi_v_u8;
typedef volatile unsigned short        hi_v_u16;
typedef volatile unsigned int          hi_v_u32;

typedef volatile unsigned long  long   hi_v_u64;
#ifndef HI_NULL
#define HI_NULL                        0
#endif

typedef enum
{
    HI_DP_BOTH_LOS_E = 0,   /*同时选择光模块和Serdes LOS信号（或）*/
    HI_DP_SDS_LOS_E,        /*Serdes的LOS信号*/
    HI_DP_OPT_SD_E,         /*光模块的SD信号*/
}HI_DP_LOS_SEL_E;

/* 用于某些硬件特性的电平配置 */
typedef enum
{
    HI_LOW_LEVEL_E = 0 , 
    HI_HIGH_LEVEL_E       
}HI_LEVEL_E;

/* 用于某些硬件特性的信号触发沿配置 */
typedef enum
{
    HI_UP_EDGE_E,           /* 上升沿 */
    HI_DOWN_EDGE_E,         /* 下降沿 */        
} HI_EDGE_E;


/* 用于某功能属性的使能 */
typedef enum
{
    HI_FALSE_E  = 0   , 
    HI_TRUE_E     
}HI_BOOL_E;



/*      rule:   PRECODE   GROUP  RESV_MODULE  RESV_NO       */
/*                0xF            x          x        xx           x       xx  */   


/* hi:         0xF0000000   - 0xF6FFFFFF */
/* hw:       0xF7000000   - 0xFFFFFFFF */

#define HI_RET_COMM_BASE                     0xF0000000

/* 各子模块返回码基础地址在此统一分配 */
/* #define HI_RET_SPI_BASE                   0xF0001000  */
/* #define HI_RET_I2C_BASE                   0xF0002000  */
/* ......  */

/* 返回值定义 */
#define HI_RET_SUCCESS                 0                              /* 返回操作成功 */
#define HI_RET_COMM_NO(x)              (HI_RET_COMM_BASE + (x))    
#define HI_RET_DEVICE_BUSY             HI_RET_COMM_NO(0)              /* 返回设备忙碌 */
#define HI_RET_DEVICE_EMPTY            HI_RET_COMM_NO(1)              /* 返回设备空 */
#define HI_RET_DEVICE_FULL             HI_RET_COMM_NO(2)              /* 返回设备满 */
#define HI_RET_DUMP_LESS               HI_RET_COMM_NO(3)              /* 返回DUMP操作未结束 */

#define HI_RET_FAILURE                 HI_RET_COMM_NO(0x100)          /* 返回操作失败 */
#define HI_RET_INVALID_PARAMETER       HI_RET_COMM_NO(0x101)          /* 返回无效的输入参数 */
#define HI_RET_DEVICE_NOT_OPEN         HI_RET_COMM_NO(0x102)          /* */
#define HI_RET_DEVICE_NOT_ENABLE       HI_RET_COMM_NO(0x103)          /* */
#define HI_RET_DEVICE_OVER_TIME        HI_RET_COMM_NO(0x104)          /* 设备访问超时 */

#define HI_RET_TABLE_ITEM_NOT_EXIST       	HI_RET_COMM_NO(0x105)          /* 表项不存在*/









#endif /* end of  */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */


