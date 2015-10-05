/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_omci_extern.h
  �� �� ��   : ����
  ��    ��   : y00185833
  ��������   : D2012_01_12

******************************************************************************/
#ifndef __HI_OMCI_EXTERN_H__
#define __HI_OMCI_EXTERN_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*****************************************************************************
 �� �� ��  : hi_omci_ext_create_inst
 ��������  : ����meid instid�����ݿ��д���ʵ��
 �������  : hi_ushort16 us_meid    
             hi_ushort16 us_instid     
 �������  : hi_void *pv_data  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_create_inst(hi_ushort16 us_meid, hi_ushort16 us_instid);

extern hi_uint32 hi_omci_ext_del_inst(hi_ushort16 us_meid, hi_ushort16 us_instid);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_get_inst
 ��������  : ����meid instid�����ݿ��ȡʵ��
 �������  : hi_ushort16 us_meid    
             hi_ushort16 us_instid     
 �������  : hi_void *pv_data  
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_inst(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_void *pv_data);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_set_inst
 ��������  : ����meid instid��pv_data��ֵ���õ����ݿ�ʵ����
 �������  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
             hi_void *pv_data       
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_set_inst(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_void *pv_data);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_get_instbyme
 ��������  : ����meid��ȡʵ������Ŀ
 �������  : hi_ushort16 us_meid  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instbyme(hi_ushort16 us_meid);

extern hi_uint32 hi_omci_ext_get_tabnum(hi_ushort16 us_meid, hi_ushort16 us_index, hi_uint32 *pui_tabnum);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_get_instnum
 ��������  : get inst count
 �������  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instnum(hi_ushort16 us_meid, hi_ushort16 us_instid);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_get_attr
 ��������  : ����meid instid�Լ����Ե�index�����ݿ��ȡ����ֵ
 �������  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
             hi_ushort16 us_index   
             hi_void *pv_data       
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_attr(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_ushort16 us_index, hi_void *pv_data);


/*****************************************************************************
 �� �� ��  : hi_omci_ext_set_attr
 ��������  : ����meid instid�Լ����Ե�index��������ֵ
 �������  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
             hi_ushort16 us_index   
             hi_void *pv_data       
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_set_attr(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_ushort16 us_index, hi_void *pv_data);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_get_instid_byattr
 ��������  : ����ʵ��id �Լ�ĳ�����Ե�ֵ����ȡʵ��id
 �������  : hi_ushort16 us_meid     
             hi_ushort16 us_index    
             hi_void *pv_attr        
             hi_ushort16 *ps_instid  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instid_byattr(hi_ushort16 us_meid, hi_ushort16 us_index, hi_void *pv_attr,hi_ushort16 *ps_instid);

/*****************************************************************************
 �� �� ��  : hi_omci_ext_get_instid_byattr2
 ��������  : ����ʵ��id�Լ�ĳ�������Ե�ֵ����ȡʵ��id
 �������  : hi_ushort16 us_meid     
             hi_ushort16 us_index1   
             hi_void *pv_attr1       
             hi_ushort16 us_index2   
             hi_void *pv_attr2       
             hi_ushort16 *ps_instid  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instid_byattr2(hi_ushort16 us_meid, hi_ushort16 us_index1, hi_void *pv_attr1, 
                                         hi_ushort16 us_index2, hi_void *pv_attr2, hi_ushort16 *ps_instid);


/*****************************************************************************
 �� �� ��  : hi_omci_ext_set_upgradefile
 ��������  : �ⲿ�������ýӿ�������ǰ���ø�������ļ�ȫ·��
 �������  : hi_uchar8 *puc_filename  
 �������  : ��
 �� �� ֵ  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_set_upgradefile( hi_uchar8 *puc_filename );

extern hi_uint32 hi_omci_ext_entity_set(hi_uint32 ui_cmdtype, hi_uint32 ui_callback, hi_uchar8 * puc_funcname);

extern hi_uint32 hi_omci_ext_ploam_omci_send(hi_void * pv_data, hi_uint32 ui_inlen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_EXTERN_H__ */
