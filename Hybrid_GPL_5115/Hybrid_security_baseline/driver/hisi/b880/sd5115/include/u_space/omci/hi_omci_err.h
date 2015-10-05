/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_omci_err.h
  �� �� ��   : ����
  ��    ��   : l00183967
  ��������   : D2011_10_27

******************************************************************************/
#ifndef __HI_OMCI_ERR_H__
#define __HI_OMCI_ERR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* OMCIģ������� */
typedef enum 
{
    HI_RET_OMCI_BASE                     = ( HI_SRCMODULE_APPS_OMCI ),
    HI_RET_OMCI_SQL_RETBASE_E            = ( HI_RET_OMCI_BASE | 0x100 ),/* SQL���������ַ */
    HI_RET_OMCI_SEND_ERR_E               = ( HI_RET_OMCI_BASE | 0x001 ),/* OMCI����ʧ�� */
    HI_RET_OMCI_UNKNOWN_MSG_E            = ( HI_RET_OMCI_BASE | 0x002 ),/* δ֪����Ϣ����*/
    HI_RET_OMCI_REPEAT_PACKET_E          = ( HI_RET_OMCI_BASE | 0x003 ),/* �ظ�OMCI������Ϣ */
    HI_RET_OMCI_ALARM_EXIST_E            = ( HI_RET_OMCI_BASE | 0x004 ),/* �澯�Ѿ����� */
    HI_RET_OMCI_OPEN_DBBASE_E            = ( HI_RET_OMCI_BASE | 0x005 ),/* �����ݿ����*/
    HI_RET_OMCI_READ_DATABSE_E           = ( HI_RET_OMCI_BASE | 0x006 ),/* �����ݿ�ʧ�� */
    HI_RET_OMCI_WRITER_DATABSE_E         = ( HI_RET_OMCI_BASE | 0x007 ),/* д���ݿ�ʧ�� */
    HI_RET_OMCI_RULE_NOT_SUPPORTED_E     = ( HI_RET_OMCI_BASE | 0x008 ),/* ��֧�ִ��������Ķ��� */
    HI_RET_OMCI_SQL_NODATA_E             = ( HI_RET_OMCI_BASE | 0x009 ),/* û������*/
    HI_RET_OMCI_SQL_TABLE_UNEXISTED_E    = ( HI_RET_OMCI_BASE | 0x00A ),/* �������*/
    HI_RET_OMCI_SQL_TABLE_EXISTED_E      = ( HI_RET_OMCI_BASE | 0x00B ),/* �����Ѵ���*/
    HI_RET_OMCI_SQL_NORIGHT_E            = ( HI_RET_OMCI_BASE | 0x00C ),/* û��Ȩ��*/
    HI_RET_OMCI_MIB_DONT_UPDATEDB_E      = ( HI_RET_OMCI_BASE | 0x00D ),/* ����Ҫˢ�����ݿ�*/
    HI_RET_OMCI_UPGRADE_FILEPATH_E       = ( HI_RET_OMCI_BASE | 0x00E ),/* �ļ�·�������ƴ���*/
}hi_omci_err_e;

#define HI_OMCI_SQL_RET(ret)  (HI_RET_OMCI_SQL_RETBASE_E | ret )


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OMCI_ERR_H__ */
