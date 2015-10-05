
/* <DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 BEGIN */
/***********************************************************************
  版权信息: 版权所有(C) 1988-2012 华为技术有限公司.
  文件名: atrwif.h
  作者: zhangjiqiang
  版本: 001
  创建日期: 2012-11-23
  完成日期: 
  功能描述: 
      
  主要函数列表: 
  
      
  修改历史记录列表: 
    <作  者>    <修改时间>  <版本>  <修改描述>
    zhangjiqiang      2012-11-23     001     init
    
  备注: 
************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<修改人>     	 	<时间>       	 <问题单号>            		 <简要描述>
z00185914           2013/01/28          DTS2013012808057		【拨号】新建ndis拨号文件夹 
z00185914    		2013/05/08    		DTS2013050705194    	【安全】消除coverity扫描问题
========================================================================================*/
/* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 BEGIN */

#ifndef __ATRWAPI_H
#define __ATRWAPI_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "at_if_fmc.h"
#include <sys/types.h>
#include <sys/stat.h>
#define AT_STATE_SIZE           32      /* size of AT state queue */
#define AT_STATE_MASK         0x1f    /* mask for AT state queue  */
#define AT_CLIENTS_SIZE       32      /* size of AT clients array */
#define WM_START_TIME         3  
#define SECOND                    1000
#include <sys/ioctl.h>
#include <netinet/in.h>
#ifdef SUPPORT_TD_CARD  //支持 TD ET128
#define USB_PCUI                 "/dev/ttyUSB4"
#define USB_DIAG                 "/dev/ttyUSB1"             //DIAG 口设备节点
#define USB_MODEM              "/dev/ttyUSB2"             //MODEM_PORT端口设备节点
#else
#define USB_PCUI                 "/dev/ttyUSB2"
#define USB_DIAG                 "/dev/ttyUSB1"             //DIAG 口设备节点
#define USB_MODEM              "/dev/ttyUSB0"             //MODEM_PORT端口设备节点
#endif
#define ATS_REP_CMD                "REP_CMD"
#define AT_TEMP_SIZE               20
#define LOOPBUFSIZE       2048
#define READ_SIZE                        512
/* <DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 BEGIN */
#define ATS_CMD_BASE_TIMEOUT    30
/* DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 END> */
#include <sys/socket.h>
#include "wdsapi.h"

#define NDISAT_LOG 	"/var/log/ndis_at.log"
//#define UNIT_TEST
#ifdef UNIT_TEST
FILE*     g_ptest_log_fd = NULL; 
#define UNIT_TEST_LOG	"var/log/atrw_unit_test.log"
#define UNIT_TEST_OUTPUT(logfile, format, arg...)		fprintf(logfile, "line = [%d], function = [%s] " format "\n", __LINE__, __FUNCTION__, ##arg)
#endif

/* AT事件 */
typedef enum
{
    AT_NULL_E = 1,            /*空事件*/
    AT_REQ_E,            /*请求事件*/
    /* <DTS2012030904901,wanglong added begin,2012-3-21 */
    AT_RESETWM_E,         /* 重启无线事件 */
    /* DTS2012030904901,wanglong added end,2012-3-21> */
    AT_TIME_E                   /*定时事件*/
}at_event_e;
/*<AP7D01374    chenkeyun    2008-02-25    mod    begin*/

typedef enum
{
    FIND_CR_CF = 0,            /*发现\r\n*/
    FIND_CR,                      /*发现\r*/
    NO_FIND_CR_CF          /*没有发现\r\n 或者\r*/     
}at_found_param;
typedef enum 
{
    AT_RET_OK,
    AT_RET_ERROR,
    
}AT_RET_CODE;

typedef enum 
{
    NO_FOUND,
    FOUND
}is_find;
/*at读取状态 枚举 */
enum read_state_enum
{
    read_usb,       /*读状态*/
    parse_recData,  /*解释状态*/
    find_result,    /*获取正确值状态*/
};

typedef enum
{
    SUPPORT_IP_NULL = 0,           
    SUPPORT_IPV4ONLY = 1,            
    SUPPORT_IPV6ONLY = 2,         
    SUPPORT_IPV4V6 = 7                  
}SUPP_IPV6CAP;

/* <DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 BEGIN */
/* DTS2013010602637 implement haisi NDIS ipv6 dial up and improve ndis dialup z00185914 20130106 END> */

#define AT_MODULE 1

#define MSG_AT_QUEUE  100
#define MSG_VOICE_QUEUE  101
#define MSG_MODEM_QUEUE  102
#define MSG_TR069_QUEUE  103
#define MSG_HTTP_QUEUE  104
#define MSG_LED_QUEUE  105
#define MSG_CMD_QUEUE  106
#define AT_RCV_MSG_LENGTH  252   //接收消息缓冲区长度

#define STRING_LENGTH_64 64
#define STRING_LENGTH_128 128
#define STRING_LENGTH_256 256

#define DEVICE_TTY_PATH "/dev/qmitty0"

struct stAtRcvMsg g_stAtToOther;
struct stAtRcvMsg
{
    long int lMsgType; 
    char acText[AT_RCV_MSG_LENGTH];
};

#define    HSPA_FROM_POSITION 0
#define    HSPA_RESULT_POSITION 1
#define    HSPA_OK    0
void at_timer_callback(int arg);
unsigned int get_ats_report_seq();

//void at_client_handle(int sig);

//int check_ats_ready();

//void at_idle(void);
//at_event_e at_event_read( void );
//int  at_event_write(at_event_e event);
//int  is_in_at_event( at_event_e event );


//void at_create_message_queue(void);
int ats_cmee(int *retCode);
//int at_sm_report_handler_csq (rsp_msg* rssi_res);

//void at_server_parse_unknow_pid_probe(int signum);
//void at_init_unknow_pid_parms(void);
int at_probe_open_port(const char* cmd_path, int* open_port_fd);
int at_read_ttyUSBX_data(int lFd);
int at_write_getport_cmd(int lFd,char* pszCmdString,char *pszMatchString,char * pszResult);
//void free_msg_queue();
void at_timer_callback(int arg);
int wait_wm_ready();
int ndis_query_ipv6_cap(SUPP_IPV6CAP* pipv6cap);
int ndis_init_PDP(SUPP_IPV6CAP ipv6PDP);
int ndis_start_connection(USER_PROFILE_ST *_profile);
int ndis_getdialupstatus(SUPP_IPV6CAP ipv6PDP);
int ndis_stop_connection();
int ndis_set_staticip(char *pszStaticIP, char *pszStaticDNS);
int ndis_get_runtime_settings(wds_runtime_settings *runtime_settings,SUPP_IPV6CAP ipv6PDP);
int ndis_read_active_report();
/* <DTS2013050705194 z00185914 2013/05/08 BEGIN */
void ndis_close_atif();
/* DTS2013050705194 z00185914 2013/05/08 END> */
int ndis_getdialuperrcode(SUPP_IPV6CAP ipv6PDP);
int is_ndis_getdialupstatus_returned(SUPP_IPV6CAP ipv6PDP);
void clear_ndis_ret_mark();
#ifdef UNIT_TEST
void close_unittest_log( );
#endif

#endif 
/* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 END> */
/* DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 END> */

