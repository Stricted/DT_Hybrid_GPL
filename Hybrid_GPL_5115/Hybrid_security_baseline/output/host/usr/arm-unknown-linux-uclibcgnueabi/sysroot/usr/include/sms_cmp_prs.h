/***********************************************************************
  ��Ȩ��Ϣ : ��Ȩ����(C) 1988-2007, ��Ϊ�������޹�˾.
  �ļ���   : Sms_cmp_prs.h
  ����     : liguangyuan
  �汾     : 1.0
  �������� : 2007-11-01
  ������� : 2007-12-15
  �������� : ���Ź����еĶ���PDU ������װ���������
  �Ĺ���ͷ�ļ������еĺ�����������������
  
  ��Ҫ�����б�: ��
  
      
  �޸���ʷ��¼�б�: 
    <��  ��>     <�޸�ʱ��>  <�汾>  <�޸�����>
      121286              11-01               1.0            ��ʼ�汾     
  ��ע: 
************************************************************************/

#ifndef _sms_cmp_prs_H_
#define _sms_cmp_prs_H_
/*<DTS2011041902025:֧�������������7bit������ by l129990,2011-4-19 begin*/
#include "atpconfig.h"
/*DTS2011041902025:֧�������������7bit������ by l129990,2011-4-19 end>*/
// ʵ���ṩ����ַ�����ֻ��10���ֽ�, Ҳ����˵���԰��ֽڱ�ʾ, ֻ�����ﵽ20������.
/* <DTS2010120205677.SMS: gKF34687 2010-11-26 MODIFY BEGIN */
/*<DTS2010092101870:�������ĺ��벻֧��#�� by l129990,2010-9-26 begin*/
#define SMS_NUMBER_LENGTH  21
/*DTS2010092101870:�������ĺ��벻֧��#�� by l129990,2010-9-26 end>*/
/* DTS2010120205677.SMS: gKF34687 2010-11-26 MODIFY END > */
#define SMS_CONTENT_LENGTH  512
#define SMS_PDU_SIZE 512
#define PER_SMS_RESERVED_SIZE 16
/*<DTS2010043000117  w00126555 20100513  modify begin*/
/*AP7D03194 chenyu 2009-08-14 ADD BEGIN*/
/*AP7D01422.Optus sms: liudan 2008-3-10 MODIFY BEGIN*/
#define SMS_UCS2_SUBMIT_SIZE 620        //������ʱ�������UTF8 תUCS2 ����ַ���
/* AP7D01422.Optus sms: liudan 2008-3-10 MODIFY END*/
/*AP7D03194 chenyu 2009-08-14 ADD END*/
/*DTS2010043000117   w00126555 20100513  modify end>*/
/* <DTS2012080601958 z00189398 2012-08-08 begin */
#define SCA_NUM_MIN_LENGTH 11
/* DTS2012080601958 z00189398 2012-08-08 end> */

/*AP7D01335.SMS: liguangyuan 2008-1-17 DEL BEGIN */
/*
#define DELIVER_SMS  0
#define SUBMIT_SMS 1
*/
/*AP7D01335.SMS: liguangyuan 2008-1-17 DEL END*/

#define HEADER_PRESENT 1
#define HEADER_NOT_PRESENT 0
#define SCA_DIGITS_IN_PDU 3

/*AP7D01440.Optus SMS: liguangyuan 2008-3-10 MODIFY begin*/
#define LONG_SMS_MAX_NUM_UCS2 67  //UCS2 �������������ַ���
#define NORMAL_SMS_MAX_NUM_UCS2 70 //UCS2 �̶����������ַ���
#define LONG_SMS_MAX_NUM_7BIT 153 //7bit �������������ַ���
#define NORMAL_SMS_MAX_NUM_7BIT 160 //7bit �̶����������ַ���
/*AP7D01440.Optus SMS: liguangyuan 2008-3-10 MODIFY END*/
/*AP7D01335.SMS: liguangyuan 2008-1-18 ADD BEGIN */
#define NORMAL_SMS 0   //������ ����
#define SMS_STATUS_REPORT 1  //����״̬����
/*AP7D01335.Optus SMS: liguangyuan 2008-1-18 ADD END*/

// ���ű��뷽ʽ

enum EMSGCodeType
{    
    EW_MSG_UCS2_CODE= 0,				// UCS2����;
    EW_MSG_GSM_7BIT_CODE,		// GSM_7BIT����;	
    EW_MSG_ASCII_CODE,				// ASCII����;
    EW_MSG_RAW_DATA,			    // RAW DATA;
};

// �˶��ŵ�״̬��0=���յ���δ�����ţ�1=���յ����Ѷ����� 2=�洢��δ������ 3=�洢���ѷ�����

enum EMSGState
{
    DELIVER_NOT_READ = 0,
    DELIVER_READ,
    SUBMIT_NOT_SEND,
    SUBMIT_SEND
};

/*AP7D01335.SMS: liguangyuan 2008-1-17 ADD BEGIN */
//���ŵ����͡�0=���յ��Ķ��ţ� 1=���ͻ򱣴�� ���ţ�2=�յ���״̬����
enum EMSGType
{
    DELIVER_SMS = 0,
    SUBMIT_SMS,
    STATUS_REPORT 
};
/* AP7D01335.SMS: liguangyuan 2008-1-17 ADD END*/

/*AP7D01335.SMS: liguangyuan 2008-1-17 ADD BEGIN */
//���� ״̬����ķ�������0=�������ʹ�; 1=����δ�ʹ�;
enum EMSGStatusReport
{
    SMS_STATUS_RECEIVED_OK = 0,      // �������, �Է��ɹ�����
    SMS_STATUS_UNABLE_CONFIRM,     // �������, SC�䴫�͵�������ȷ���
    SMS_STATUS_REPLACED,                  // �������, ���ű�SC����
    SMS_STATUS_TRYING,                      // ��ʱʧ��SC�Լ�����
    SMS_STATUS_PERMANENT_ERROR,   // ������ʧ�ܣ�SC���ٴ�
    SMS_STATUS_TEMPORARY_ERROR    // ��ʱ��ʧ�ܣ���SC���ǲ��ٴ���
};
/* AP7D01335.SMS: liguangyuan 2008-1-17 ADD END*/

//���ű�־λ������ 0=��ͨ �̶��� 1=������ 2=�������Ͷ���
enum EMSGFlagType
{
    NORMAL_TXT_SMS = 0,
    LONG_TXT_SMS,
    OTHER_TYPE_SMS
};

/*<DTS2011041902025:֧�������������7bit������ by l129990,2011-4-19 begin*/
//���ű�������� 0=��ͨ �̶��� 1=�������� 2=��������
#ifdef SUPPORT_ATP_MUTILAG_SMS
enum EMSGCodingType
{
    DEFAULT_SMS = 0,
    TURKISH_SMS,
    SPANISH_SMS
};
#endif
/*DTS2011041902025:֧�������������7bit������ by l129990,2011-4-19 end>*/
//���ŵ���ʱ��ı�ʾ�ṹ��
 struct SEWTime{
    unsigned short   wYear;          //�� 2007
    unsigned char    byteMonth;      //��  11
    unsigned char    byteDay;        //�� 30
    unsigned char    byteHour;       //ʱ 24
    unsigned char    byteMinute;     //�� 59
    unsigned char    byteSecond;     //�� 59
}; 

//��������Ϣ�Ľṹ�塣
 typedef struct LongSmsInfoStruct{
    unsigned short mr;         // ���ű�ʶ����ȡֵ0��255 
    unsigned char btTotalPack;  //�����ݰ����� 
    unsigned char btPack;      // ���ݰ����     
}StLongSmsInfo; 

//���ǰ�ͽ��������ݵ�ͨ�ýṹ��
/* ���Ķ��ŵĽṹ�� */
typedef struct SmsInfoStruct{
    int   index;        //���ŵ����ֵ
    enum EMSGState state;      /* �˶��ŵ����͡�0=���յ���δ�����ţ�1=���յ����Ѷ����� 2=�洢��δ������ 3=�洢���ѷ�����*/
/*AP7D01335.SMS: liguangyuan 2008-1-18 ADD BEGIN */
    unsigned char   parsed_sms_type;      //���ͣ������Ķ��ţ������ʹ�ı��档
/* AP7D01335.SMS: liguangyuan 2008-1-18 ADD END*/
    char  number[SMS_NUMBER_LENGTH+1];     //����
    unsigned char  length;        // PDU���ݵ��ֽ���,���Ϊ255
    unsigned char content[SMS_CONTENT_LENGTH+1];    //���ŵ����ݡ�
    struct SEWTime StSmsTime;
    enum EMSGFlagType sms_flag_type;       // �����ŵı�־λ 
    StLongSmsInfo long_sms_info;
    char center_number[SMS_NUMBER_LENGTH+1];
    enum EMSGCodeType sms_content_code;    //���ŵ��ı���ʽ; UCS2 ,7bit,8bit
/*AP7D01335.SMS: liguangyuan 2008-1-17 ADD BEGIN */
    enum EMSGStatusReport  sms_status_report;  //���ű�������
/* AP7D01335.SMS: liguangyuan 2008-1-17 ADD END*/
/*<DTS2011041902025:֧�������������7bit������ by l129990,2011-4-19 begin*/
#ifdef SUPPORT_ATP_MUTILAG_SMS
    enum EMSGCodingType sms_coding;
#endif
/*DTS2011041902025:֧�������������7bit������ by l129990,2011-4-19 end>*/
}StSmsInfo;

/*���ű���װ��Ľṹ��.*/
typedef struct CmgsInfoStruct{
    unsigned short  length;        // PDU���ݵ��ֽ���,���Ϊ255
    unsigned char  pdu[SMS_PDU_SIZE];      //pdu����
} StCmgsInfo;

/* �ӹ����ļ��ж�ȡ����Ԥ���� ,�����ǽ���ǰ�Ķ��Žṹ��.*/
typedef struct PerSmsStruct{
    int  index;                   /*���ŵ����ֵ��*/
    enum EMSGState state;         /* �˶��ŵ����͡�0=���յ���δ�����ţ�1=���յ����Ѷ����ţ�2=�洢��δ������ 3=�洢���ѷ�����*/
    unsigned short length;        /* PDU���ݵ��ֽ���,���Ϊ255*/
    unsigned char reserved[PER_SMS_RESERVED_SIZE];         /* �����֣�����û��ʹ�á�*/
    unsigned char pdu[SMS_PDU_SIZE];    /*pdu��*/
}StPerSms;

/*<DTS2010043000117  w00126555 20100513  del begin*/
//���ǰ�ĵ��ú���
//int ComposeSms(struct SmsInfoStruct *pInputSms,struct CmgsInfoStruct *pOutputSms );

//����ǰ���õĺ���   
//int ParseSms(struct PerSmsStruct *pInputSms,struct SmsInfoStruct *pOutputSms);
/*DTS2010043000117    w00126555 20100513  del end>*/

#endif

