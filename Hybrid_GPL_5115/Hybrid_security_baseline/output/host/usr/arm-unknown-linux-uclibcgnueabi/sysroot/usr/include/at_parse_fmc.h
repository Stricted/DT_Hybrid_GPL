#ifndef __AT_PARSE_H
#define __AT_PARSE_H
/***********************************************************************
  版权信息: 版权所有(C) 1988-2007, 华为技术有限公司.
  文件名: 
  作者: liudan 60024226
  版本: B108
  创建日期: 2007-11-13
  完成日期: 
  功能描述: 
      定义AT命令通用解析库函数的结构体和函数.
  主要函数列表: 
      
  修改历史记录列表: 
    <作  者>    <修改时间>  <版本>  <修改描述>
    
  备注: 
************************************************************************/
#define MAX_ATCMD_SIZE          512     /* max size of at command */
#define MAX_AT_BUF 256    /*max size of string*/
#define  AT_SMS_INDEX    "index_"    //特殊AT命令的域名（没有域名只有域值结果）
#define  AT_COPS_OPTION    "option"    //特殊COPS命令的option
#define  AT_COPS_MODE    "mode"    //特殊COPS命令的mode
#define  AT_COPS_FORMAT    "format"    //特殊COPS命令的format
#define AT_RESULT_MAX_NUM_PARAM  20       //AT命令结果中最大operator个数
#define AT_SPECIAL_RESULT_MAX 20
#define MAX_SMS_INDEX_LENGTH 10
#define AT_PARSE_LOG  "var/at/at_parse.log"
/*以下宏定义为异常错误返回结果*/
#define  AT_RETURN_SUSS                        0    //AT命令解析的正确结果
#define  AT_RETURN_NAME_ERR              -1   //AT命令的名字字符串为空
#define  AT_RETURN_RESULT_ERR           -2   //AT命令的结果字符串为空
#define  AT_RETURN_FUNC_ERR              -3   //AT命令的功能类型不匹配
#define  AT_RETURN_APP_ERR                -4   //没有找到AT命令的特殊结果
#define  AT_RETURN_MALLOC_ERR          -5   //内存分配失败
#define  AT_RETURN_MODE_NAME_ERR  -6   //域名字符串为空

/* AtCmdParse结构体为AT命令返回字符串解析以后的链表结构 */
typedef struct AtCmdParseStruct
{
    char *AtFieldName;    //AT命令的域名，由标准AT命令"对照解析字符串"提供。
    char *AtFieldValue;    //AT命令的域名值，最终的域对应的值。
    struct AtCmdParseStruct *plist;  //指向下一个同类型单元中的同类型的域的指针。
    struct AtCmdParseStruct *next;  //指向单元内的不同类型的域的指针。
}AtCmdParse;  


typedef enum
{
    AT_FUNCTION_INITIATIVE_REPORT  = 0,  //主动上报
    AT_FUNCTION_NETWORK_CFG          , //网络设置
    AT_FUNCTION_PIN_CFG                    ,   //PIN操作
    AT_FUNCTION_SMS_CFG                  ,   //短信操作
    AT_FUNCTION_BASIC_INFO_CFG      ,  //基本信息和设置
    AT_FUNCTION_VOICE_CALL_CFG      ,  //语音呼叫
    AT_FUNCTION_ADD_OPERATION      ,   //补充业务
    AT_FUNCTION_DATA_OPERATION    ,    //数据业务
    AT_FUNCTION_PHONE_RECORD       ,    //电话本
    AT_FUNCTION_SYSTEM_CFG            ,    //系统设置

}at_function_type;

typedef enum
{
    AT_OPERATION_TYPE_READ = 0,
    AT_OPERATION_TYPE_EXECUTE,
    AT_OPERATION_TYPE_TEST,
    AT_OPERATION_TYPE_OTHER,
    AT_OPERATION_TYPE_NULL
}at_operation_type;
struct at_parse_handler
 {
    /* AT命令的名字*/
    char* at_name;
    /* 按AT命令的功能进行的分类，
    如短信相关的命令，PIN操作相关命令等。*/
    int  func_cmd_type;
    /* 四组对照解析字符串，对应三种AT命令的类型和一些特殊的类型。
    Parse_contrast[0]对应读命令的，Parse_contrast[1]对应exe命令,
    Parse_contrast[2]对应test命令,Parse_contrast[3]对应主动上报等特殊命令的返回结果.
    */
    char* parse_contrast[4];
    /* 解析函数，result指向解析后的结果的地址，
    parse_contrast_str 对照解析字符串，
    at_value_str表示未解析前的at命令的返回字符串。*/
    int (*parse_read_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
    int (*parse_exe_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
    int (*parse_test_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
    int (*parse_other_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);

    /*在用户把解析后的结果拷贝到结构体后，释放内存的函数 */
    void (*free_mem)(AtCmdParse *result);
 };

/*
extern struct at_parse_handler initivatve_report_handlers[];
extern struct at_parse_handler network_cfg_handlers[];
extern struct at_parse_handler pin_cfg_handlers[];            
extern struct at_parse_handler sms_cfg_handlers[];              
extern struct at_parse_handler basic_info_cfg_handlers[];  
extern struct at_parse_handler voice_call_cfg_handlers[];            
extern struct at_parse_handler add_operation_cfg_handlers[];                    
extern struct at_parse_handler data_operation_cfg_handlers[];
extern struct at_parse_handler phone_record_cfg_handlers[];
extern struct at_parse_handler sys_cfg_handlers[];
*/


int print_operator(AtCmdParse *result);
int count_separator(char* str, char* sep);
AtCmdParse* GreatLinkList_AT(char* mode_name[],char* mode_value[], int mode_number);
int parse_field(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* separator );
int parse_many_field(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* separator );
int parse_sms_cmgr_at_str(AtCmdParse **result, char* parse_contrast_str, char* at_value_str,char* at_name);
int parse_cmgi_list_at_str(AtCmdParse **result, char* parse_contrast_str, char* at_value_str,char* at_name);
int parse_sms_index_list_at_str(AtCmdParse **result, char* parse_contrast_str, char* at_value_str,char* at_name);
int at_const_parse (char* at_para_value[], char* parse_str, char* parse_str_const, int *number);
int parse_common_at_str(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
int parse_soft_str(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
int parse_cops_at_str (AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
//void free_at_cmd (AtCmdParse *result);
int parse_final_result(AtCmdParse **result, char* at_value_str);
int at_cmd_parse(AtCmdParse** result, char *AtName, char *AtValue, at_function_type func_type, at_operation_type oper_type);
void free_at_cmd (AtCmdParse *result);
void free_node (AtCmdParse *node);
void free_at_res_tree(AtCmdParse *tree_head);
void free_at_res_link(AtCmdParse *head);
void free_cops_link(AtCmdParse *head);
int parse_operator_list(AtCmdParse **result, char* parse_contrast_str, char* at_value_str,char* cops_field_name);


#endif
