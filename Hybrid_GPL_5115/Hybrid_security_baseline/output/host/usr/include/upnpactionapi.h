/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpupnpactionapi.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2012-5-31
*/
#ifndef __ATP_UPNP_ACTION_API_H__
#define __ATP_UPNP_ACTION_API_H__

//!\brief tr064 <--> tr098 or tr181 domain mapping
/*!
 *
 */
typedef struct tgATP_UPNP_DomainMap_ST
{
    const VOS_CHAR* pszTr064Str; //! tr064 name
    const VOS_CHAR* pszDMStr;    //! datamodel name
}ATP_UPNP_DomainMap;


/*!
  \brief getaction
  \param[in]  *pstAction: 
  \param[in]  *astDomainMaps: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UPNP_DoGetAction(
                            ATP_UPNP_ACTION_ST *pstAction,
                            ATP_UPNP_DomainMap *astDomainMaps,
                            VOS_UINT32          ulLen);

/*!
  \brief setaction
  \param[in]  *pstAction: 
  \param[in]  *astDomainMaps: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UPNP_DoSetAction(
                            ATP_UPNP_ACTION_ST       *pstAction,
                            const ATP_UPNP_DomainMap *astDomainMaps,
                            VOS_UINT32                ulLen);

/*!
  \brief append response
  \param[in]  *pstAction: 
  \param[in]  *astNvpairs: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UPNP_SOAPAppendResponseValues(
                            const ATP_UPNP_ACTION_ST   *pstAction,
                            const ATP_UPNP_PAIR  *astNvpairs,
                            VOS_UINT32           ulLen);

/*!
  \brief append one name value
  \param[in]  *pstAction: 
  \param[in]  *pcParaName: 
  \param[in]  *pcParaValue: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UPNP_SOAPAppendResponseValue(
                            const ATP_UPNP_ACTION_ST  *pstAction,
                            const VOS_CHAR      *pcParaName,
                            const VOS_CHAR      *pcParaValue);


/*!
  \brief get request values
  \param[in]  *pstAction: 
  \param[in]  *astNvpairs: 
  \param[in]  ulLen: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UPNP_SOAPGetRequestValues(
                            ATP_UPNP_ACTION_ST  *pstAction,
                            ATP_UPNP_PAIR       *astNvpairs,
                            VOS_UINT32          ulLen);

/*!
  \brief get request value
  \param[in]  *pstAction: 
  \param[in]  *pcParaName: 
  \param[in]  **ppcParaValue: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/
VOS_UINT32 ATP_UPNP_SOAPGetRequestValue(
                            ATP_UPNP_ACTION_ST  *pstAction,
                            const VOS_CHAR      *pcParaName,
                            const VOS_CHAR      **ppcParaValue);

/*!
  \brief  get version
  \param[in] void: 
  \return 
  \retval ATP_E_ERR_CFM_SHM_NOT_ATTACH 初始化共享内存失败
*/

const VOS_CHAR * ATP_UPNPACTIONCORE_GetVersion(void);



#endif
