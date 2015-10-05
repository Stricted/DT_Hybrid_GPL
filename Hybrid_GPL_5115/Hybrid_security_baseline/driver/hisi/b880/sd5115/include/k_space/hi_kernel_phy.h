/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

******************************************************************************
  �� �� ��   : hi_kernel_phy.h
  �� �� ��   : ����
  ��    ��   : l00204224
  ��������   : D2012_04_10

******************************************************************************/
#ifndef __HI_KERNEL_PHY_H__
#define __HI_KERNEL_PHY_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum {
    HI_KERNEL_PHY_STATUS_DOWN_E = 0,
    HI_KERNEL_PHY_STATUS_10_HALF_E,
    HI_KERNEL_PHY_STATUS_10_FULL_E,
    HI_KERNEL_PHY_STATUS_100_HALF_E,
    HI_KERNEL_PHY_STATUS_100_FULL_E,
    HI_KERNEL_PHY_STATUS_1000_HALF_E,
    HI_KERNEL_PHY_STATUS_1000_FULL_E,
    HI_KERNEL_PHY_STATUS_AUTO_E,
}hi_kernel_phy_status_e;




typedef struct {
    hi_uint32 ui_phy_addr;    /* ����phy address  */  
    hi_uint32 ui_enable;      /* 0:�˳�������ͼ����ģʽ 1:��������ͼ����ģʽ��*/
}hi_kernel_test_mode_s;



typedef hi_void (*hi_kernel_phy_callback)( hi_uchar8 *puc_devname, hi_uint32 ui_portnum, hi_uint32 ui_status );

extern hi_void hi_kernel_phy_reg( hi_kernel_phy_callback pv_callback );
extern hi_void hi_kernel_phy_unreg( hi_void );
extern hi_uint32 hi_kernel_phy_inner_get(hi_uint32 ui_phyid, hi_uint32 *pui_status);
extern hi_uint32 hi_kernel_phy_extern_get( hi_uint32 ui_phyid, hi_uint32 *pui_status );
 
#endif /* __HI_KERNEL_PHY_H__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

