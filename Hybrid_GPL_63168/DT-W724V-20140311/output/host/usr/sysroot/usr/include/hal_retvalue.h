/********************************************************************************
*               Copyright (C) 2006, Huawei Tech. Co., Ltd.              
*                           ALL RIGHTS RESERVED                              
* ��    ��: 1.0                                                       
* �ļ�����: ���ݡ��ײ�����������̹淶V1.0��Ҫ��Ϊ�ײ���������ṩ��ͳһ�ĺ�������
*          ���뼯�ϡ����󷵻�����Ը������������ӣ��޸Ľϴ�ʱע�������Ӧ�淶��                              
* ��    �ߣ������ն�������OS���������Ŷ�                                                        
* �޸���ʷ:                                                                                                                                                 
* 1. 2006-3-1,�����ն�������OS���������Ŷ� �������ļ�                                                                                                                                                                                        
********************************************************************************/

#ifndef __HAL_RETVALUE_H__
#define __HAL_RETVALUE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE   1
#endif

#define ERR_NO(x)	(0-x)
       
typedef enum
{
    HAL_E_RETVAL_OK                      = 0,            /* ������ȷ���� */
    HAL_E_RETVAL_EDEFAULT                = ERR_NO(1),    /* Ĭ�ϴ��� */
    HAL_E_RETVAL_EFILE_OPEN              = ERR_NO(101),  /* �ļ���ʧ�� */
    HAL_E_RETVAL_EFILE_LEN               = ERR_NO(102),  /* �ļ���С���� */
    HAL_E_RETVAL_EFILE_READ              = ERR_NO(103),  /* ��ȡ�ļ�ʧ�� */
    HAL_E_RETVAL_EFILE_WRITE             = ERR_NO(104),  /* д���ļ�ʧ�� */
    HAL_E_RETVAL_EFILE_SEEK              = ERR_NO(105),  /* �ļ�ָ�붨λʧ�� */
    HAL_E_RETVAL_EPBUF_NULL              = ERR_NO(201),  /* �յ�ַ */
    HAL_E_RETVAL_EMEM_GET                = ERR_NO(202),  /* ��̬�����ڴ�ʧ�� */
    HAL_E_RETVAL_EBUF_OVERLEN            = ERR_NO(203),  /* д�뻺�������� */
    HAL_E_RETVAL_EBUF_TOOSMALL           = ERR_NO(204),  /* ��ȡ������̫С */
    HAL_E_RETVAL_EFLASH_READ             = ERR_NO(301),  /* ��FLASH����*/
    HAL_E_RETVAL_EFLASH_WRITE            = ERR_NO(302),  /* дFLASH����*/
    HAL_E_RETVAL_EFLASH_ERASE            = ERR_NO(303),  /* ����Flash Blockʧ��*/
    HAL_E_RETVAL_EFLASH_ADDRER           = ERR_NO(304),  /* ��ַԽ��*/   
    HAL_E_RETVAL_EFLASH_BLOCK_BEGIN_ADDR = ERR_NO(305),  /* ���ǿ���׵�ַ */   
    HAL_E_RETVAL_EFLASH_BLOCK_END_ADDR   = ERR_NO(305),  /* ���ǿ��β��ַ */ 
    HAL_E_RETVAL_EFLASH_NOT_EXIST        = ERR_NO(306),  /* FLASH������ */    
    HAL_E_RETVAL_EFLASH_NOT_READY        = ERR_NO(307),  /* FLASH��æ */    
    HAL_E_RETVAL_EFLASH_NOT_SUPPORT_CFI  = ERR_NO(308),  /* ��֧��CFI */    
    HAL_E_RETVAL_EFLASH_PARAMETER        = ERR_NO(309),  /* �����Ƿ� */
    HAL_E_RETVAL_EDEFLATE_DATA           = ERR_NO(401),  /* ����ѹ��ʧ�� */
    HAL_E_RETVAL_EINFLATE_DATA           = ERR_NO(402),  /* ���ݽ�ѹʧ�� */
    
}HAL_E_FUNCRETVAL;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* __HAL_RETVALUE_H__ */
