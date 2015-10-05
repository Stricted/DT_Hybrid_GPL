/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file neighdisccmsapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x00192803
  *\date  2012-7-2
*/


#ifndef __NEIGHDISCAPI_H__
#define __NEIGHDISCAPI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifdef ATP_DEBUG
#define NEIGH_DISC_DEBUG(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define NEIGH_DISC_ERROR(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__)
#else
#define NEIGH_DISC_DEBUG(format,arg...)
#define NEIGH_DISC_ERROR(format,arg...) printf("\r\n"format"FILE[%s] LINE[%d] \r\n",## arg,__FILE__,__LINE__)
#endif

VOS_UINT32 ATP_NeighDiscCms_Init();
VOS_UINT32 ATP_NeighDiscCms_TakeEffect(const VOS_CHAR *pcInterface);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __NEIGHDISCAPI_H__ */


