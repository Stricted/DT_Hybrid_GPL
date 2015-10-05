/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_extern.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2012_01_12

******************************************************************************/
#ifndef __HI_OMCI_EXTERN_H__
#define __HI_OMCI_EXTERN_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*****************************************************************************
 函 数 名  : hi_omci_ext_create_inst
 功能描述  : 根据meid instid在数据库中创建实例
 输入参数  : hi_ushort16 us_meid    
             hi_ushort16 us_instid     
 输出参数  : hi_void *pv_data  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_create_inst(hi_ushort16 us_meid, hi_ushort16 us_instid);

extern hi_uint32 hi_omci_ext_del_inst(hi_ushort16 us_meid, hi_ushort16 us_instid);

/*****************************************************************************
 函 数 名  : hi_omci_ext_get_inst
 功能描述  : 根据meid instid从数据库获取实例
 输入参数  : hi_ushort16 us_meid    
             hi_ushort16 us_instid     
 输出参数  : hi_void *pv_data  
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_inst(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_void *pv_data);

/*****************************************************************************
 函 数 名  : hi_omci_ext_set_inst
 功能描述  : 根据meid instid将pv_data的值设置到数据库实例中
 输入参数  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
             hi_void *pv_data       
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_set_inst(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_void *pv_data);

/*****************************************************************************
 函 数 名  : hi_omci_ext_get_instbyme
 功能描述  : 根据meid获取实例的数目
 输入参数  : hi_ushort16 us_meid  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instbyme(hi_ushort16 us_meid);

extern hi_uint32 hi_omci_ext_get_tabnum(hi_ushort16 us_meid, hi_ushort16 us_index, hi_uint32 *pui_tabnum);

/*****************************************************************************
 函 数 名  : hi_omci_ext_get_instnum
 功能描述  : get inst count
 输入参数  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instnum(hi_ushort16 us_meid, hi_ushort16 us_instid);

/*****************************************************************************
 函 数 名  : hi_omci_ext_get_attr
 功能描述  : 根据meid instid以及属性的index从数据库获取属性值
 输入参数  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
             hi_ushort16 us_index   
             hi_void *pv_data       
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_attr(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_ushort16 us_index, hi_void *pv_data);


/*****************************************************************************
 函 数 名  : hi_omci_ext_set_attr
 功能描述  : 根据meid instid以及属性的index设置属性值
 输入参数  : hi_ushort16 us_meid    
             hi_ushort16 us_instid  
             hi_ushort16 us_index   
             hi_void *pv_data       
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_set_attr(hi_ushort16 us_meid, hi_ushort16 us_instid, hi_ushort16 us_index, hi_void *pv_data);

/*****************************************************************************
 函 数 名  : hi_omci_ext_get_instid_byattr
 功能描述  : 根据实体id 以及某个属性的值，获取实例id
 输入参数  : hi_ushort16 us_meid     
             hi_ushort16 us_index    
             hi_void *pv_attr        
             hi_ushort16 *ps_instid  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instid_byattr(hi_ushort16 us_meid, hi_ushort16 us_index, hi_void *pv_attr,hi_ushort16 *ps_instid);

/*****************************************************************************
 函 数 名  : hi_omci_ext_get_instid_byattr2
 功能描述  : 根据实体id以及某两个属性的值，获取实例id
 输入参数  : hi_ushort16 us_meid     
             hi_ushort16 us_index1   
             hi_void *pv_attr1       
             hi_ushort16 us_index2   
             hi_void *pv_attr2       
             hi_ushort16 *ps_instid  
 输出参数  : 无
 返 回 值  : hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_ext_get_instid_byattr2(hi_ushort16 us_meid, hi_ushort16 us_index1, hi_void *pv_attr1, 
                                         hi_ushort16 us_index2, hi_void *pv_attr2, hi_ushort16 *ps_instid);


/*****************************************************************************
 函 数 名  : hi_omci_ext_set_upgradefile
 功能描述  : 外部函数调用接口在升级前调用更改输出文件全路径
 输入参数  : hi_uchar8 *puc_filename  
 输出参数  : 无
 返 回 值  : hi_uint32
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
