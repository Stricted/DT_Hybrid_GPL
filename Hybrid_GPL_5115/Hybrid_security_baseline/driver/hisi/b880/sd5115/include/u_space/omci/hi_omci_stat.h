/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_omci_stat.h
  �� �� ��   : ����
  ��    ��   : y00185833
  ��������   : D2012_04_26

******************************************************************************/
#ifndef __HI_OMCI_STAT_H__
#define __HI_OMCI_STAT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*****************************************************************************
 �� �� ��  : hi_omci_stat_clear_current
 ��������  : �����ǰͳ������
 �������  : hi_void  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_clear_current(hi_void);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_ethpm2_get
 ��������  : ��ȡeth pm 2��ʷ����
 �������  : hi_omci_me_ethpm2_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm2_get(hi_omci_me_ethpm2_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_ethpm2_getcur
 ��������  : ��ȡeth pm 2��ǰ����
 �������  : hi_omci_me_ethpm2_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm2_getcur(hi_omci_me_ethpm2_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_ethpm3_get
 ��������  : ��ȡeth pm 3��ʷ����
 �������  : hi_omci_me_msg_ethpm3_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm3_get(hi_omci_me_msg_ethpm3_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_ethpm3_getcur
 ��������  : ��ȡeth pm 3��ǰ����
 �������  : hi_omci_me_msg_ethpm3_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm3_getcur(hi_omci_me_msg_ethpm3_s *pst_data);

extern hi_uint32 hi_omci_stat_ethpm4_getcur(hi_omci_me_msg_ethpm4_s *pst_data);

extern hi_uint32 hi_omci_stat_ethpm4_get(hi_omci_me_msg_ethpm4_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_ethpm_get
 ��������  : ��ȡeth pm��ʷ����
 �������  : hi_omci_me_msg_ethpm_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm_get(hi_omci_me_msg_ethpm_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_ethpm_getcur
 ��������  : ��ȡeth pm ��ǰ����
 �������  : hi_omci_me_msg_ethpm_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm_getcur(hi_omci_me_msg_ethpm_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_init
 ��������  : ��ʼ��ͳ������
 �������  : hi_void  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_init(hi_void);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_macportpm_get
 ��������  : ��ȡmca port pm��ʷ����
 �������  : hi_omci_me_macportpm_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_macportpm_get(hi_omci_me_macportpm_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_macportpm_getcur
 ��������  : ��ȡmac port pm��ǰ����
 �������  : hi_omci_me_macportpm_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_macportpm_getcur(hi_omci_me_macportpm_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_fec_pm_get
 ��������  : ��ȡfec pm��ʷ����
 �������  : hi_omci_me_fec_pm_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_fec_pm_get(hi_omci_me_fec_pm_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_fec_pm_get
 ��������  : ��ȡfec pmͳ�Ƶ�ǰ����
 �������  : hi_omci_me_fec_pm_s *pst_data  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_fec_pm_getcur(hi_omci_me_fec_pm_s *pst_data);

/*****************************************************************************
 �� �� ��  : hi_omci_stat_save_history
 ��������  : ����ͳ����ʷ����
 �������  : hi_void  
 �������  : ��
 �� �� ֵ  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_save_history(hi_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_STAT_H__ */






