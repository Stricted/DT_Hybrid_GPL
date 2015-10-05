/***********************************************************************
  版权信息 : 版权所有(C) 1988-2007, 华为技术有限公司.
  文件名   : Sms_cmp_prs.h
  作者     : liguangyuan
  版本     : 1.0
  创建日期 : 2007-11-01
  完成日期 : 2007-12-15
  功能描述 : 短信功能中的短信PDU 包的组装与解析函数
  的公共头文件，其中的函数被其它函数调用
  
  主要函数列表: 无
  
      
  修改历史记录列表: 
    <作  者>     <修改时间>  <版本>  <修改描述>
      121286              11-01               1.0            初始版本     
  备注: 
************************************************************************/

#ifndef _sms_cmp_prs_H_
#define _sms_cmp_prs_H_
/*<DTS2011041902025:支持西班牙语短信7bit需求定制 by l129990,2011-4-19 begin*/
#include "atpconfig.h"
/*DTS2011041902025:支持西班牙语短信7bit需求定制 by l129990,2011-4-19 end>*/
// 实际提供给地址号码的只有10个字节, 也就是说若以半字节表示, 只能最多达到20个数字.
/* <DTS2010120205677.SMS: gKF34687 2010-11-26 MODIFY BEGIN */
/*<DTS2010092101870:短信中心号码不支持#符 by l129990,2010-9-26 begin*/
#define SMS_NUMBER_LENGTH  21
/*DTS2010092101870:短信中心号码不支持#符 by l129990,2010-9-26 end>*/
/* DTS2010120205677.SMS: gKF34687 2010-11-26 MODIFY END > */
#define SMS_CONTENT_LENGTH  512
#define SMS_PDU_SIZE 512
#define PER_SMS_RESERVED_SIZE 16
/*<DTS2010043000117  w00126555 20100513  modify begin*/
/*AP7D03194 chenyu 2009-08-14 ADD BEGIN*/
/*AP7D01422.Optus sms: liudan 2008-3-10 MODIFY BEGIN*/
#define SMS_UCS2_SUBMIT_SIZE 620        //定义临时用来存放UTF8 转UCS2 后的字符串
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
#define LONG_SMS_MAX_NUM_UCS2 67  //UCS2 长短信中最大的字符数
#define NORMAL_SMS_MAX_NUM_UCS2 70 //UCS2 短短信中最大的字符数
#define LONG_SMS_MAX_NUM_7BIT 153 //7bit 长短信中最大的字符数
#define NORMAL_SMS_MAX_NUM_7BIT 160 //7bit 短短信中最大的字符数
/*AP7D01440.Optus SMS: liguangyuan 2008-3-10 MODIFY END*/
/*AP7D01335.SMS: liguangyuan 2008-1-18 ADD BEGIN */
#define NORMAL_SMS 0   //真正的 短信
#define SMS_STATUS_REPORT 1  //短信状态报告
/*AP7D01335.Optus SMS: liguangyuan 2008-1-18 ADD END*/

// 短信编码方式

enum EMSGCodeType
{    
    EW_MSG_UCS2_CODE= 0,				// UCS2编码;
    EW_MSG_GSM_7BIT_CODE,		// GSM_7BIT编码;	
    EW_MSG_ASCII_CODE,				// ASCII编码;
    EW_MSG_RAW_DATA,			    // RAW DATA;
};

// 此短信的状态。0=接收到的未读短信；1=接收到的已读短信 2=存储的未发短信 3=存储的已发短信

enum EMSGState
{
    DELIVER_NOT_READ = 0,
    DELIVER_READ,
    SUBMIT_NOT_SEND,
    SUBMIT_SEND
};

/*AP7D01335.SMS: liguangyuan 2008-1-17 ADD BEGIN */
//短信的类型。0=接收到的短信； 1=发送或保存的 短信；2=收到的状态报告
enum EMSGType
{
    DELIVER_SMS = 0,
    SUBMIT_SMS,
    STATUS_REPORT 
};
/* AP7D01335.SMS: liguangyuan 2008-1-17 ADD END*/

/*AP7D01335.SMS: liguangyuan 2008-1-17 ADD BEGIN */
//短信 状态报告的返回类型0=短信已送达; 1=短信未送达;
enum EMSGStatusReport
{
    SMS_STATUS_RECEIVED_OK = 0,      // 传送完成, 对方成功接收
    SMS_STATUS_UNABLE_CONFIRM,     // 传送完成, SC虽传送但不能明确结果
    SMS_STATUS_REPLACED,                  // 传送完成, 短信被SC覆盖
    SMS_STATUS_TRYING,                      // 暂时失败SC仍继续传
    SMS_STATUS_PERMANENT_ERROR,   // 永久性失败，SC不再传
    SMS_STATUS_TEMPORARY_ERROR    // 临时性失败，但SC还是不再传送
};
/* AP7D01335.SMS: liguangyuan 2008-1-17 ADD END*/

//短信标志位的类型 0=普通 短短信 1=长短信 2=其它类型短信
enum EMSGFlagType
{
    NORMAL_TXT_SMS = 0,
    LONG_TXT_SMS,
    OTHER_TYPE_SMS
};

/*<DTS2011041902025:支持西班牙语短信7bit需求定制 by l129990,2011-4-19 begin*/
//短信编码的类型 0=普通 短短信 1=土耳其语 2=西班牙语
#ifdef SUPPORT_ATP_MUTILAG_SMS
enum EMSGCodingType
{
    DEFAULT_SMS = 0,
    TURKISH_SMS,
    SPANISH_SMS
};
#endif
/*DTS2011041902025:支持西班牙语短信7bit需求定制 by l129990,2011-4-19 end>*/
//短信到达时间的表示结构体
 struct SEWTime{
    unsigned short   wYear;          //年 2007
    unsigned char    byteMonth;      //月  11
    unsigned char    byteDay;        //日 30
    unsigned char    byteHour;       //时 24
    unsigned char    byteMinute;     //分 59
    unsigned char    byteSecond;     //秒 59
}; 

//长短信信息的结构体。
 typedef struct LongSmsInfoStruct{
    unsigned short mr;         // 短信标识符，取值0～255 
    unsigned char btTotalPack;  //总数据包数量 
    unsigned char btPack;      // 数据包序号     
}StLongSmsInfo; 

//打包前和解析后数据的通用结构体
/* 明文短信的结构体 */
typedef struct SmsInfoStruct{
    int   index;        //短信的序号值
    enum EMSGState state;      /* 此短信的类型。0=接收到的未读短信；1=接收到的已读短信 2=存储的未发短信 3=存储的已发短信*/
/*AP7D01335.SMS: liguangyuan 2008-1-18 ADD BEGIN */
    unsigned char   parsed_sms_type;      //类型：真正的短信，短信送达的报告。
/* AP7D01335.SMS: liguangyuan 2008-1-18 ADD END*/
    char  number[SMS_NUMBER_LENGTH+1];     //号码
    unsigned char  length;        // PDU数据的字节数,最大为255
    unsigned char content[SMS_CONTENT_LENGTH+1];    //短信的内容。
    struct SEWTime StSmsTime;
    enum EMSGFlagType sms_flag_type;       // 长短信的标志位 
    StLongSmsInfo long_sms_info;
    char center_number[SMS_NUMBER_LENGTH+1];
    enum EMSGCodeType sms_content_code;    //短信的文本格式; UCS2 ,7bit,8bit
/*AP7D01335.SMS: liguangyuan 2008-1-17 ADD BEGIN */
    enum EMSGStatusReport  sms_status_report;  //短信报告类型
/* AP7D01335.SMS: liguangyuan 2008-1-17 ADD END*/
/*<DTS2011041902025:支持西班牙语短信7bit需求定制 by l129990,2011-4-19 begin*/
#ifdef SUPPORT_ATP_MUTILAG_SMS
    enum EMSGCodingType sms_coding;
#endif
/*DTS2011041902025:支持西班牙语短信7bit需求定制 by l129990,2011-4-19 end>*/
}StSmsInfo;

/*短信被组装后的结构体.*/
typedef struct CmgsInfoStruct{
    unsigned short  length;        // PDU数据的字节数,最大为255
    unsigned char  pdu[SMS_PDU_SIZE];      //pdu包。
} StCmgsInfo;

/* 从共享文件中读取出的预短信 ,并且是解析前的短信结构体.*/
typedef struct PerSmsStruct{
    int  index;                   /*短信的序号值。*/
    enum EMSGState state;         /* 此短信的类型。0=接收到的未读短信；1=接收到的已读短信；2=存储的未发短信 3=存储的已发短信*/
    unsigned short length;        /* PDU数据的字节数,最大为255*/
    unsigned char reserved[PER_SMS_RESERVED_SIZE];         /* 保留字，现在没有使用。*/
    unsigned char pdu[SMS_PDU_SIZE];    /*pdu包*/
}StPerSms;

/*<DTS2010043000117  w00126555 20100513  del begin*/
//打包前的调用函数
//int ComposeSms(struct SmsInfoStruct *pInputSms,struct CmgsInfoStruct *pOutputSms );

//解析前调用的函数   
//int ParseSms(struct PerSmsStruct *pInputSms,struct SmsInfoStruct *pOutputSms);
/*DTS2010043000117    w00126555 20100513  del end>*/

#endif

