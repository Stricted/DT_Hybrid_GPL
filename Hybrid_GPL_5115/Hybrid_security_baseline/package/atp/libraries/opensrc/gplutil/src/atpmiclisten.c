#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>

#include "atptypes.h"
#include "atpmiclisten.h"


/*Add by  yehuisheng 00183935@20111108 for ���mic�����ӽ��̺��ӽ�����accept��ǰ�쳣�˳�������ѭ�������˳������⡣*/
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
VOS_UINT32 ATP_UTIL_MIC_ResetBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize )
{
    VOS_UINT32              ulIdx = 0;

    if( pulFdsArray != NULL && ulFdsArrSize > 0 )
    {
        for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
        {
            *(pulFdsArray + ulIdx) = 0;
        }

        return VOS_OK;
    }

    return VOS_NOK;
}

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
VOS_UINT32 ATP_UTIL_MIC_AddBackupAppFds( VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize, const VOS_UINT32 ulAddFd )
{
    VOS_UINT32              ulIdx = 0;

    if( pulFdsArray != NULL && ulFdsArrSize > 0 && ulAddFd > 0 )
    {
        for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
        {
            if( 0 == *(pulFdsArray + ulIdx) )
            {
                *(pulFdsArray + ulIdx) = ulAddFd;
                return VOS_OK;
            }
        }

        return VOS_NOK;
    }

    return VOS_NOK;
}

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
VOS_UINT32 ATP_UTIL_MIC_CloseBackupAppFdsReadableConnection( const VOS_UINT32 * pulFdsArray, const VOS_UINT32 ulFdsArrSize )
{
    VOS_UINT32              ulMaxFd = 0;
    VOS_UINT32              ulIdx = 0;
    VOS_INT32               lAcceptFd = 0;
    VOS_INT32               lFind = 0;

    fd_set                  stFdset;
    struct timeval          stTimeVal;

    FD_ZERO(&stFdset);

    for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
    {
        if( *(pulFdsArray + ulIdx) > 0 )
        {
            FD_SET( *(pulFdsArray + ulIdx), &stFdset );
            ulMaxFd = ( ulMaxFd > *(pulFdsArray + ulIdx) ) ? ulMaxFd : *(pulFdsArray + ulIdx);

            lFind = 1;
        }
    }

    if( 1 == lFind )
    {
        stTimeVal.tv_sec  = 0;
        stTimeVal.tv_usec = 0;
        while( select( ulMaxFd + 1, &stFdset, NULL, NULL, &stTimeVal ) > 0 )
        {
            for( ulIdx = 0; ulIdx < ulFdsArrSize; ulIdx++ )
            {
                if ( *(pulFdsArray + ulIdx) > 0 && FD_ISSET( *(pulFdsArray + ulIdx), &stFdset ) )
                {
                    lAcceptFd = accept( *(pulFdsArray + ulIdx), NULL, NULL );

                    if( lAcceptFd > 0 )
                    {
                        close( lAcceptFd );
                    }
                }
            }
        }
    }

    return VOS_OK;
}
/*Add by  yehuisheng 00183935@20111108 for ���mic�����ӽ��̺��ӽ�����accept��ǰ�쳣�˳�������ѭ�������˳������⡣*/


