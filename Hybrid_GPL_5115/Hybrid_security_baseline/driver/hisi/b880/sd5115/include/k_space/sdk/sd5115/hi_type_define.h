/* *****************************************************************************
          ��Ȩ���� (C), 2008-2018, ��˼�뵼�����޹�˾
 
******************************************************************************
  �ļ�����: hi_type_define.h
  ��������: ��˼�����������ͺͷ���ֵ����ͷ�ļ�
  �汾����: V1.0

  ��������: 2008��12��8��
  ��������: ��˼����������    

  �޸ļ�¼: 
***************************************************************************** */
#ifdef __cplusplus 
#if __cplusplus 
extern "C" 
{ 
#endif /* __cpluscplus */ 
#endif /* __cpluscplus */

#ifndef  __HI_TYPE_DEFINE_H__
#define  __HI_TYPE_DEFINE_H__

/* �����������Ͷ������� */
typedef volatile unsigned char         hi_v_u8;
typedef volatile unsigned short        hi_v_u16;
typedef volatile unsigned int          hi_v_u32;

typedef volatile unsigned long  long   hi_v_u64;
#ifndef HI_NULL
#define HI_NULL                        0
#endif

typedef enum
{
    HI_DP_BOTH_LOS_E = 0,   /*ͬʱѡ���ģ���Serdes LOS�źţ���*/
    HI_DP_SDS_LOS_E,        /*Serdes��LOS�ź�*/
    HI_DP_OPT_SD_E,         /*��ģ���SD�ź�*/
}HI_DP_LOS_SEL_E;

/* ����ĳЩӲ�����Եĵ�ƽ���� */
typedef enum
{
    HI_LOW_LEVEL_E = 0 , 
    HI_HIGH_LEVEL_E       
}HI_LEVEL_E;

/* ����ĳЩӲ�����Ե��źŴ��������� */
typedef enum
{
    HI_UP_EDGE_E,           /* ������ */
    HI_DOWN_EDGE_E,         /* �½��� */        
} HI_EDGE_E;


/* ����ĳ�������Ե�ʹ�� */
typedef enum
{
    HI_FALSE_E  = 0   , 
    HI_TRUE_E     
}HI_BOOL_E;



/*      rule:   PRECODE   GROUP  RESV_MODULE  RESV_NO       */
/*                0xF            x          x        xx           x       xx  */   


/* hi:         0xF0000000   - 0xF6FFFFFF */
/* hw:       0xF7000000   - 0xFFFFFFFF */

#define HI_RET_COMM_BASE                     0xF0000000

/* ����ģ�鷵���������ַ�ڴ�ͳһ���� */
/* #define HI_RET_SPI_BASE                   0xF0001000  */
/* #define HI_RET_I2C_BASE                   0xF0002000  */
/* ......  */

/* ����ֵ���� */
#define HI_RET_SUCCESS                 0                              /* ���ز����ɹ� */
#define HI_RET_COMM_NO(x)              (HI_RET_COMM_BASE + (x))    
#define HI_RET_DEVICE_BUSY             HI_RET_COMM_NO(0)              /* �����豸æµ */
#define HI_RET_DEVICE_EMPTY            HI_RET_COMM_NO(1)              /* �����豸�� */
#define HI_RET_DEVICE_FULL             HI_RET_COMM_NO(2)              /* �����豸�� */
#define HI_RET_DUMP_LESS               HI_RET_COMM_NO(3)              /* ����DUMP����δ���� */

#define HI_RET_FAILURE                 HI_RET_COMM_NO(0x100)          /* ���ز���ʧ�� */
#define HI_RET_INVALID_PARAMETER       HI_RET_COMM_NO(0x101)          /* ������Ч��������� */
#define HI_RET_DEVICE_NOT_OPEN         HI_RET_COMM_NO(0x102)          /* */
#define HI_RET_DEVICE_NOT_ENABLE       HI_RET_COMM_NO(0x103)          /* */
#define HI_RET_DEVICE_OVER_TIME        HI_RET_COMM_NO(0x104)          /* �豸���ʳ�ʱ */

#define HI_RET_TABLE_ITEM_NOT_EXIST       	HI_RET_COMM_NO(0x105)          /* �������*/









#endif /* end of  */

#ifdef  __cplusplus
#if __cplusplus
}
#endif  /* end of __cplusplus  */
#endif  /* end of __cplusplus  */


