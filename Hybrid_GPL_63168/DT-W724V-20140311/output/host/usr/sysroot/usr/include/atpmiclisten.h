/**\file atpmiclisten.h
  * ATP���ƽ̨��������ͷ�ļ�
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author y00183935
  *\date  2012-04-09
*/

#ifndef __ATP_MIC_LISTEN_H__
#define __ATP_MIC_LISTEN_H__

/********************** Include Files ***************************************/
#include "atptypes.h"



/*function macro*/
#if defined(__cplusplus)
extern "C" {
#endif


/******************************************************************************
  ��������  : ATP_UTIL_MIC_ResetBackupAppFds
  ��������  : ���ñ���fd������
  �������  : 
              1.  *pulFdsArray: �����׵�ַ
              2.  ulFdsArrSize: �����С
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ����VOS_OK��ʾ����ִ�гɹ�������VOS_NOK��ʾ����ִ��ʧ��


  �޸���ʷ     :
  1.��    ��   : 2011-11-08
    ��    ��   : y00183935
    �޸�����   : �����ɺ���
******************************************************************************/
VOS_UINT32 ATP_UTIL_MIC_ResetBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize );


/******************************************************************************
  ��������  : ATP_UTIL_MIC_AddBackupAppFds
  ��������  : ���fd������fd������
  �������  : 
              1.  *pulFdsArray: �����׵�ַ
              2.  ulFdsArrSize: �����С
              3. ulAddFd : ��ӵ�fd
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ����VOS_OK��ʾ����ִ�гɹ�������VOS_NOK��ʾ����ִ��ʧ��


  �޸���ʷ     :
  1.��    ��   : 2011-11-08
    ��    ��   : y00183935
    �޸�����   : �����ɺ���
******************************************************************************/
VOS_UINT32 ATP_UTIL_MIC_AddBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize, const VOS_UINT32 ulAddFd );


/******************************************************************************
  ��������  : ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection
  ��������  : �رձ���fd���������������
  �������  : 
              1.  *pulFdsArray: �����׵�ַ
              2.  ulFdsArrSize: �����С
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ����VOS_OK��ʾ����ִ�гɹ�������VOS_NOK��ʾ����ִ��ʧ��


  �޸���ʷ     :
  1.��    ��   : 2011-11-08
    ��    ��   : y00183935
    �޸�����   : �����ɺ���
******************************************************************************/
VOS_UINT32 ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection( const VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize );



#if defined(__cplusplus)
}
#endif

#endif

