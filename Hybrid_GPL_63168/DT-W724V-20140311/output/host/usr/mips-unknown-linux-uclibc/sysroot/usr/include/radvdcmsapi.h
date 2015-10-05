/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file radvdcmsapi.h
  *
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author x192803
  *\date  2011-12-20
*/


/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/


#ifndef __RADVDAPI_H__
#define __RADVDAPI_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "ipv6commoncmsapi.h"

#ifdef ATP_DEBUG
#define IPV6_LAN_DEBUG(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__) 
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] FUNC[%s] LINE[%d] \r\n",## arg,__FILE__,__func__,__LINE__)
#else
#define IPV6_LAN_DEBUG(format,arg...)
#define IPV6_LAN_ERROR(format,arg...) printf("\r\n"format"FILE[%s] LINE[%d] \r\n",## arg,__FILE__,__LINE__)
#endif

/* BEGIN: Added by h00169677, 2012/9/12   PN:DTS2012081706429 */
#ifdef SUPPORT_ATP_RADVD_ULA_PREFIX_FC
#define ULA_PREFIX  "fc"
#define IS_ULA_ADDR(a) ((strncasecmp("fc", a, 2) == 0 )||(strncasecmp("fd", a, 2) == 0 ))
#else
#define ULA_PREFIX  "fd"
#define IS_ULA_ADDR(a) (strncasecmp("fd", a, 2) == 0)
#endif
/* END:   Added by h00169677, 2012/9/12 */

/* Radvd ��־λö�� */
//!\brief Radvd ��־λö�� 
/*!
 * Radvd ��־λö�� 
 */
typedef enum tag_RADVD_ENABLE_FLAG_E
{
    RADVD_FLAG_FALSE = 0,/*!< RADVD is disable */
    RADVD_FLAG_TRUE  = 1,/*!<  RADVD is enable*/
    RADVD_FLAG_EMPTY = 2,/*!< not used now */
    RADVD_FLAG_BUFF
}RADVD_ENABLE_FLAG_E;


/*!
  \brief RADVD CMS ��ʼ������
  \param[in] VOS_VOID: 
  \return VOS_UINT32
  \retval ����VOS_OK�����ʼ���ɹ�
*/
extern VOS_UINT32 ATP_RADVD_Init(VOS_VOID);

/*!
  \brief ��������RADVD����
  \param[in] VOS_VOID: 
  \return VOS_INT32
  \retval ���������Ƿ�ɹ�
*/
extern VOS_INT32 ATP_RADVD_ReconfigRadvdPro(VOS_INT32 lBridgeKey);

/*!
  \brief ʹ��WAN����������RADVD����
  \param[in]  pulSetValues: 
  \param[in]  pstDHCPv6Para: 
  \return 
  \retval �����Ƿ�ɹ�
*/
extern VOS_INT32 ATP_RADVD_UseParameterFromWAN(VOS_UINT32 * pulSetValues, const ATP_DHCP6S_PARAMETER_ST * pstDHCPv6Para);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* __RADVDAPI_H__ */
