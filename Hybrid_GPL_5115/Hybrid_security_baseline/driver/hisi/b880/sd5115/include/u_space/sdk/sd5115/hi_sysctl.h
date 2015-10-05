/******************************************************************************
  ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾

 ******************************************************************************
 �ļ�����: hi_sysctl.h
 ��������: sysctl api�ӿڶ��� 
 �汾����: V1.0
 ��������: 2008��12��8��
 ��������: ��˼����������    
 �޸ļ�¼: 
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
/*********************************�豸�� �ر�*************************************************/

/*�豸��*/
extern uint hi_sysctl_dev_open(void);

/*�豸�ر�*/
extern void hi_sysctl_dev_close(void);


#if 0
/***********************************************************************************************/             	        
/*********************************����Ȩ�޻�ȡ**************************************************/
/*����Ȩ��*/
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
