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

#ifndef  __HI_SYSCTL_H__
#define  __HI_SYSCTL_H__

#include "base_type_define.h"
#include "hi_type_define.h"

#include "sdk_autoconf.h"

#include "hi_drv_ioctl_sysctl.h"




/***********************************************************************************************/             	        
/*********************************设备打开 关闭*************************************************/

/*设备打开*/
extern uint hi_sysctl_dev_open(void);

/*设备关闭*/
extern void hi_sysctl_dev_close(void);


#if 0
/***********************************************************************************************/             	        
/*********************************访问权限获取**************************************************/
/*访问权限*/
extern uint hi_sysctl_right_get(void);
extern uint hi_sysctl_right_release(void);
#endif


/***********************************************************************************************/             	        
extern uint hi_sysctl_attr_get(hi_sysctl_attr_s *pst_attr);
extern uint hi_sysctl_attr_set(hi_sysctl_attr_s *pst_attr);
extern uint hi_sysctl_cnt_get(hi_sysctl_cnt_s *pst_cnt);
extern uint hi_sysctl_sta_get(hi_sysctl_sta_s *pst_sta);
extern uint hi_sysctl_temp_get(HI_SYSCTL_TEMP_E *pem_temp);
extern uint hi_sysctl_chip_reset(void);
/***********************************************************************************************/             	        

/*************************************************************************************/             	        
/*********************************   DEBUG *******************************************/

/*for debug*/
extern uint  hi_sysctl_physical_addr_get(void *p_info);
extern uint  hi_sysctl_physical_addr_set(void *p_info);



#define HI_DEBUG_PRINT_POSITION()   printf("\n\r LINE = %d, FUNC = %s \n\r ", __LINE__, __func__)
#define HI_DEBUG(fmt,arg...)        if ( g_sysctl_debug_en )    (HI_DEBUG_PRINT_POSITION(),printf(fmt,##arg))


                  
#endif /* end of __HI_SYSCTL_H__*/

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of __cplusplus */
