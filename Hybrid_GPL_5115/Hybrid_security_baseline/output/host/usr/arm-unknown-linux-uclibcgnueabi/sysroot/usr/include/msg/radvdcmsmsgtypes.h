/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file radvdcmsmsgtypes.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author x00192803 
  *\date  2012-7-6
*/

#ifndef __ATP_RADVDCMS_MSGTYPES_H__
#define __ATP_RADVDCMS_MSGTYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define ATP_CBBID_RADVD_NAME                  "radvd"

enum ATP_RADVD_MSGTYPES_EN
{
    ATP_MSG_RADVD_SEND_OPTION = ATP_MSG_CATEGORY_DEF(ATP_MODULE_RADVDCMS),    //发送option      
/* <DTS2012082701764 w00211169 2012-9-4 begin */
    ATP_MSG_CMS_RADVD_FAKE,
/* DTS2012082701764 w00211169 2012-9-4 end> */   
} ;

//!option 长度
#define RADVD_OPTION_LEN  256

struct sRASendOpt
{
    VOS_BOOL          bEnable;        
    VOS_UINT32       ulOptTag;        /*!< option  code */
    VOS_UINT8         acOptValue[RADVD_OPTION_LEN];        /*!< option  value  */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  


