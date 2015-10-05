#ifndef __CUPSCMSAPI_H__
#define __CUPSCMSAPI_H__



//!...........................................�ⲿapi�ӿ�start.................................................
#if defined(__cplusplus)
extern "C" {
#endif

/*!
  \brief set up cups
  \param[in] VOS_VOID: 
  \return success: VOS_OK;fail :VOS_NOK
  \retval
*/
VOS_INT32 ATP_CupsStart(VOS_VOID);


/*!
  \brief ����Ƿ��д�ӡ������
  \param[in] VOS_VOID: 
  \return ��:VOS_TRUE; ��:VOS_FALSE
  \retval
*/
VOS_BOOL ATP_UsbStoragePrinterExist(VOS_VOID);


/*!
  \brief restare or kill cups and samba
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_VOID ATP_ResetCupsdAndSamba(const ATP_MSG_HEADER_ST *pstMsg);

/*!
  \brief disconnect  cups printer
  \param[in] VOS_VOID: 
  \return 
  \retval
*/
VOS_VOID ATP_DisconnetCupsdPrinter(VOS_VOID);

#if defined(__cplusplus)
}
#endif
//!...........................................�ⲿapi�ӿ�end.................................................

#endif
