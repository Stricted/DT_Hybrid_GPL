/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_stat.h
  版 本 号   : 初稿
  作    者   : y00185833
  生成日期   : D2012_04_26

******************************************************************************/
#ifndef __HI_OMCI_STAT_H__
#define __HI_OMCI_STAT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*****************************************************************************
 函 数 名  : hi_omci_stat_clear_current
 功能描述  : 清除当前统计数据
 输入参数  : hi_void  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_clear_current(hi_void);

/*****************************************************************************
 函 数 名  : hi_omci_stat_ethpm2_get
 功能描述  : 获取eth pm 2历史数据
 输入参数  : hi_omci_me_ethpm2_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm2_get(hi_omci_me_ethpm2_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_ethpm2_getcur
 功能描述  : 获取eth pm 2当前数据
 输入参数  : hi_omci_me_ethpm2_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm2_getcur(hi_omci_me_ethpm2_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_ethpm3_get
 功能描述  : 获取eth pm 3历史数据
 输入参数  : hi_omci_me_msg_ethpm3_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm3_get(hi_omci_me_msg_ethpm3_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_ethpm3_getcur
 功能描述  : 获取eth pm 3当前数据
 输入参数  : hi_omci_me_msg_ethpm3_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm3_getcur(hi_omci_me_msg_ethpm3_s *pst_data);

extern hi_uint32 hi_omci_stat_ethpm4_getcur(hi_omci_me_msg_ethpm4_s *pst_data);

extern hi_uint32 hi_omci_stat_ethpm4_get(hi_omci_me_msg_ethpm4_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_ethpm_get
 功能描述  : 获取eth pm历史数据
 输入参数  : hi_omci_me_msg_ethpm_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm_get(hi_omci_me_msg_ethpm_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_ethpm_getcur
 功能描述  : 获取eth pm 当前数据
 输入参数  : hi_omci_me_msg_ethpm_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_ethpm_getcur(hi_omci_me_msg_ethpm_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_init
 功能描述  : 初始化统计数据
 输入参数  : hi_void  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_init(hi_void);

/*****************************************************************************
 函 数 名  : hi_omci_stat_macportpm_get
 功能描述  : 获取mca port pm历史数据
 输入参数  : hi_omci_me_macportpm_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_macportpm_get(hi_omci_me_macportpm_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_macportpm_getcur
 功能描述  : 获取mac port pm当前数据
 输入参数  : hi_omci_me_macportpm_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_macportpm_getcur(hi_omci_me_macportpm_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_fec_pm_get
 功能描述  : 获取fec pm历史数据
 输入参数  : hi_omci_me_fec_pm_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_fec_pm_get(hi_omci_me_fec_pm_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_fec_pm_get
 功能描述  : 获取fec pm统计当前数据
 输入参数  : hi_omci_me_fec_pm_s *pst_data  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_fec_pm_getcur(hi_omci_me_fec_pm_s *pst_data);

/*****************************************************************************
 函 数 名  : hi_omci_stat_save_history
 功能描述  : 保存统计历史数据
 输入参数  : hi_void  
 输出参数  : 无
 返 回 值  : extern hi_uint32
*****************************************************************************/
extern hi_uint32 hi_omci_stat_save_history(hi_void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_STAT_H__ */






