/******************************************************************************
  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
 文件名称: hi_sysctl.h
 功能描述: sysctl api接口定义 
 版本描述: V1.0
 创建日期: 2008年12月8日
 创建作者: 海思驱动开发组    
 修改记录: 
 ******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef  __HI_INTERFACE_H__
#define  __HI_INTERFACE_H__

#include "sdk_autoconf.h"

#include "base_type_define.h"
#include "hi_type_define.h"

#include "hi_drv_ioctl_interface.h"




/***********************************************************************************************/             	        
/*********************************设备打开 关闭*************************************************/
extern uint hi_interface_dev_open(void);
extern uint hi_interface_dev_clsoe(void);

/***********************************************************************************************/             	        
/*********************************SPI**********************************************/
extern uint hi_spi_send(hi_spi_send_s  *pst_attr);
extern uint hi_spi_receive(hi_spi_receive_s  *pst_attr);
extern uint hi_spi_send_receive(hi_spi_send_receive_info_s  *pst_attr);

/***********************************************************************************************/             	        
/*********************************I2C*************************************************/
extern uint hi_i2c_attr_set(hi_i2c_attr_s *pst_attr);
extern uint hi_i2c_attr_get(hi_i2c_attr_s *pst_attr);
extern uint hi_i2c_receive(hi_i2c_send_receive_info_s *pst_attr);
extern uint hi_i2c_send(hi_i2c_send_receive_info_s *pst_attr);
/*************************************************************************************/             	        
/*********************************   DEBUG *******************************************/


#define HI_DEBUG_PRINT_POSITION()   printf("\n\r FILE = %s, \n\r LINE = %d, FUNC = %s \n\r ", __FILE__, __LINE__, __func__)
#define HI_DEBUG(fmt,arg...)        if ( g_sysctl_debug_en )    (HI_DEBUG_PRINT_POSITION(),printf(fmt,##arg))

#endif /* end of __HI_SYSCTL_H__*/

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of __cplusplus */
