/* <DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 BEGIN */
/***********************************************************************
  版权信息: 版权所有(C) 1988-2008, 华为技术有限公司.
  文件名: if_usbnet.h
  作者: mayang
  版本: 001
  创建日期: 2008-5-24
  完成日期: 
  功能描述: 
      
  主要函数列表: 
      
  修改历史记录列表: 
    <作  者>    <修改时间>  <版本>  <修改描述>
    mayang       2008-5-17                   001             init
    gKF34687    2010-11-4                    002             b683 移植
  备注: 
************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<修改人>     	 	<时间>       	 <问题单号>            		 <简要描述>
z00185914           2013/01/28          DTS2013012808057		【拨号】新建ndis拨号文件夹 
========================================================================================*/

#ifndef _IF_USBNET_H_
#define _IF_USBNET_H_

/*用户层使用的流量统计结构*/
struct usbnet_stats
{
    unsigned long   bytes_in;            /* 上次复位后，此接口接收的总字节数 */
    unsigned long   packets_in;        /* 上次复位后，此接口接收的分组总数 */
    unsigned long   errors_in;          /* 上次复位后，此接口接收的错误的分组总数*/
    unsigned long   dropped_in;       /* 上次复位后，此接口接收的丢弃的分组总数*/
    unsigned long   bytes_out;         /* 上次复位后，此接口发送的总字节数 */
    unsigned long   packets_out;     /* 上次复位后，此接口发送的分组总数 */
    unsigned long   errors_out;        /* 上次复位后，此接口发送的错误分组总数 */
    unsigned long   dropped_out;     /* 上次复位后，此接口发送的丢弃分组总数 */
};

int  clear_usbnet_stats(const char *name);  /* <DTS2010121702766.QMI: gkf34687 2010-12-28 ADD>*/

/* 获取usbnet网卡统计信息*/
int get_usbnet_stats (const char *name, struct usbnet_stats *stats);


#endif
/* DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 END> */
