/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_logdef.h
  �� �� ��   : ����
  ��    ��   : l00183967
  ��������   : D2011_07_28
  ����޸�   :

******************************************************************************/
#ifndef __HI_LOGDEF_H__
#define __HI_LOGDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_LOG_OUTPUTMAXLEN    2048

typedef enum  {
    HI_LOGTYPE_DEBUG_E = 0,
    HI_LOGTYPE_SYSTRACE_E,
    HI_LOGTYPE_PRINT_E,
    HI_LOGTYPE_PRINTMEM_E,
}hi_logtype_e;

/*���˸�����,��ΪDEBUGΪUCHAR8����*/
typedef enum  {
    HI_LOG_LEVEL_DBG_E = 0, /* ���� */
    HI_LOG_LEVEL_ALM_E,     /* �澯 */
    HI_LOG_LEVEL_WRN_E,     /* ��ʾ */
    HI_LOG_LEVEL_INFO_E,    /* ��Ϣ */
    HI_LOG_LEVEL_MSG_E,     /* ��Ϣ */
    HI_LOG_LEVEL_ERR_E,     /* ���� */
    HI_LOG_LEVEL_EVT_E,     /* �¼� */
    HI_LOG_LEVEL_RESV_E,    /* �ڴ��ӡ */
}hi_dbglevel_e; 

typedef struct  {
    hi_uint32 ui_srcmodule;
    hi_uint32 ui_maskdbgflag;   /*bit0:31,ÿ�����ض�Ӧhi_dbglevel_eö��*/
    hi_uchar8 uc_systraceflag;  /* 0x0: �رղ����; 0x1: ���������Ϣ; 0x2: ֻ���ʧ����Ϣ; 
                                   0x3: ֻ����ɹ���Ϣ;0x4: �������÷���ֵ��ȷƥ��*/
    hi_uchar8 uc_printflag;
    hi_uchar8 uc_flag;          /*bit0:1Զ�̴�ӡ bit0:0���ش�ӡ*/
    hi_uchar8 uc_resv;
    hi_uint32 ui_retcode;
}hi_log_cfg_s;

typedef struct  {
    hi_uint32 ui_srcmodule;
    hi_uint32 ui_logtype;
    hi_uint32 ui_level; 
    hi_uint32 ui_ret;
    hi_uint32 ui_arg1;
    hi_uint32 ui_arg2;
    hi_uint32 ui_arg3;
    hi_uint32 ui_arg4;
    hi_uint32 ui_datalen; 
    hi_uchar8 uc_data[HI_LOG_OUTPUTMAXLEN];
}hi_log_data_s;

#define HI_BUG()              { printk("%s %d\r\n",__func__,__LINE__); (*(hi_int32*)0 = 0);}
#define HI_ASSERT(expt)       if (expt) HI_BUG();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_LOGDEF_H__ */
