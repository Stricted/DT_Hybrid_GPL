/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_errdef.h
  ��������   : hi_errdef.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : D2011_07_19
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __HI_ERRDEF_H__
#define __HI_ERRDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************************************
		��˼ʹ�ã�0x00000000 - 0x0FFFFFFF
               ������ĸ˳������		
****************************************************/
typedef enum  {
    HI_RET_SUCC                     = (0   ),                        /*���سɹ�*/
    HI_RET_FAIL                     = (0xFFFFFFFF   ),               /*����ͨ�ô�����*/
    HI_RET_BASEVALUE                = (0x00000000   ),               /*��������ʼ��ֵ*/
    HI_RET_CHIP_NOT_SUPPORTED       = (HI_RET_BASEVALUE | 0x01  ),     /*оƬ��֧�ָù��� */
    HI_RET_CHIP_ID_ERROR            = (HI_RET_BASEVALUE | 0x02  ),     /*оƬ�Ŵ��� */
    HI_RET_CRC_ERR                  = (HI_RET_BASEVALUE | 0x03  ),     /*CRCУ��������*/
    HI_RET_DEVBUSY                  = (HI_RET_BASEVALUE | 0x04  ),     /*�����豸æ*/
    HI_RET_DEVEMPTY                 = (HI_RET_BASEVALUE | 0x05  ),     /*�����豸��*/
    HI_RET_DEVFULL                  = (HI_RET_BASEVALUE | 0x06  ),     /*�����豸��*/
    HI_RET_DEVTIMEOUT               = (HI_RET_BASEVALUE | 0x07  ),     /*�����豸��ʱ*/
    HI_RET_DEVCLOSE                 = (HI_RET_BASEVALUE | 0x08  ),     /*�豸δʹ�� */
    HI_RET_FILE_OPEN_FAIL           = (HI_RET_BASEVALUE | 0x09  ),     /*���ļ�����*/
    HI_RET_FILE_CLOSE_FAIL          = (HI_RET_BASEVALUE | 0x0A  ),     /*�ر��ļ�����*/
    HI_RET_FILE_READ_FAIL           = (HI_RET_BASEVALUE | 0x0B  ),     /*���ļ�����*/
    HI_RET_FILE_WRITE_FAIL          = (HI_RET_BASEVALUE | 0x0C  ),     /*д�ļ�����*/
    HI_RET_INIT_FAIL                = (HI_RET_BASEVALUE | 0x0D  ),     /*��ʼ��ʧ�� */
    HI_RET_ITEM_NOTEXIST            = (HI_RET_BASEVALUE | 0x0E  ),     /*����item������*/
    HI_RET_ITEM_EXIST               = (HI_RET_BASEVALUE | 0x0F  ),     /*����item�Ѿ�����*/
    HI_RET_ITEM_FULL                = (HI_RET_BASEVALUE | 0x10  ),     /*����item������*/
    HI_RET_ITEM_EXCEPT              = (HI_RET_BASEVALUE | 0x11  ),     /*����item�����쳣*/
    HI_RET_INVALID_PARA             = (HI_RET_BASEVALUE | 0x12  ),     /*������Ч����*/
    HI_RET_INVALID_STATE            = (HI_RET_BASEVALUE | 0x13  ),     /*�����״̬ */
    HI_RET_INVALID_VLAN_ID          = (HI_RET_BASEVALUE | 0x14  ),     /*�Ƿ�VLAN ID */
    HI_RET_INVALID_PRIORITY_TYPE    = (HI_RET_BASEVALUE | 0x15  ),     /*���ȼ����ʹ��� */
    HI_RET_INVALID_MAC_AGE_TIME     = (HI_RET_BASEVALUE | 0x16  ),     /*MAC��ַ�ϻ�ʱ��Ƿ� */
    HI_RET_INVALID_MAC_LEARN_LIMIT  = (HI_RET_BASEVALUE | 0x17  ),     /*MAC��ַѧϰ���Ƿ� */
    HI_RET_INVALID_MAC_ADDR         = (HI_RET_BASEVALUE | 0x18  ),     /*MAC��ַ�Ƿ� */
    HI_RET_INVALID_PORT_TAG_MODE    = (HI_RET_BASEVALUE | 0x19  ),     /*�˿�TAG���ʹ��� */
    HI_RET_INVALID_ADDR             = (HI_RET_BASEVALUE | 0x1A  ),     /*����ĵ�ַ */
    HI_RET_MALLOC_FAIL              = (HI_RET_BASEVALUE | 0x1B  ),     /*�����ڴ�ʧ��*/
    HI_RET_MFREE_FAIL               = (HI_RET_BASEVALUE | 0x1C  ),     /*�ͷ��ڴ�ʧ��*/
    HI_RET_MSG_UNKNOWN              = (HI_RET_BASEVALUE | 0x1D  ),     /*δ֪����Ϣ����*/
    HI_RET_MSG_RCV_ERRSIZE          = (HI_RET_BASEVALUE | 0x1E  ),     /*���յ�����Ϣ���ȴ��� */
    HI_RET_NULLPTR                  = (HI_RET_BASEVALUE | 0x1F  ),     /*���������ָ��*/
    HI_RET_NOTSUPPORT               = (HI_RET_BASEVALUE | 0x20  ),     /*���ز�֧��*/
    HI_RET_OUTRANG                  = (HI_RET_BASEVALUE | 0x21  ),     /*���س���ϵͳ��Χ*/
    HI_RET_REG_WRITE_ERR            = (HI_RET_BASEVALUE | 0x22  ),     /*д�Ĵ���ʧ��*/
    HI_RET_REG_READ_ERR             = (HI_RET_BASEVALUE | 0x23  ),     /*���Ĵ���ʧ��*/
    HI_RET_REPEAT_OPER              = (HI_RET_BASEVALUE | 0x24  ),     /*�ظ�����*/
    HI_RET_SYS_CALLFAIL             = (HI_RET_BASEVALUE | 0x25  ),     /*ϵͳ����ʧ��*/
    HI_RET_SYS_EXCEPTION            = (HI_RET_BASEVALUE | 0x26  ),     /*ִ��ʧ��*/
    HI_RET_SIGNAL                   = (HI_RET_BASEVALUE | 0x27  ),     /*�ȴ��źŷ��� */
    HI_RET_TIMEOUT                  = (HI_RET_BASEVALUE | 0x28  ),     /*���ͳ�ʱ*/
    HI_RET_TIMER_CREATE_FAIL        = (HI_RET_BASEVALUE | 0x29  ),     /*������ʱ��ʧ��*/
    HI_RET_TABLE_FULL               = (HI_RET_BASEVALUE | 0x2A  ),     /*���� */
    HI_RET_TABLE_EXIST              = (HI_RET_BASEVALUE | 0x2B  ),     /*�����Ѵ��� */
    HI_RET_TABLE_NOTEXIST           = (HI_RET_BASEVALUE | 0x2C  ),     /*���в����� */
    HI_RET_TABLE_EXCEPT             = (HI_RET_BASEVALUE | 0x2D  ),     /*����item�����쳣*/
    HI_RET_UNINIT                   = (HI_RET_BASEVALUE | 0x2E  ),     /*û�г�ʼ�� */
    HI_RET_ALREADYINIT              = (HI_RET_BASEVALUE | 0x2F  ),     /*�Ѿ���ʼ�� */
    HI_RET_UNACTIVE                 = (HI_RET_BASEVALUE | 0x30  ),     /*����û�д򿪻��߼���*/
    HI_RET_FORKFAIL                 = (HI_RET_BASEVALUE | 0x31  ),     /*���ش�������ʧ��*/
}hi_ret_base_e;


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_ERRDEF_H__ */


