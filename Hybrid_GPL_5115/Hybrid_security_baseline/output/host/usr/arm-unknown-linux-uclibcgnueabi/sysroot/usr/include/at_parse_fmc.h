#ifndef __AT_PARSE_H
#define __AT_PARSE_H
/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2007, ��Ϊ�������޹�˾.
  �ļ���: 
  ����: liudan 60024226
  �汾: B108
  ��������: 2007-11-13
  �������: 
  ��������: 
      ����AT����ͨ�ý����⺯���Ľṹ��ͺ���.
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#define MAX_ATCMD_SIZE          512     /* max size of at command */
#define MAX_AT_BUF 256    /*max size of string*/
#define  AT_SMS_INDEX    "index_"    //����AT�����������û������ֻ����ֵ�����
#define  AT_COPS_OPTION    "option"    //����COPS�����option
#define  AT_COPS_MODE    "mode"    //����COPS�����mode
#define  AT_COPS_FORMAT    "format"    //����COPS�����format
#define AT_RESULT_MAX_NUM_PARAM  20       //AT�����������operator����
#define AT_SPECIAL_RESULT_MAX 20
#define MAX_SMS_INDEX_LENGTH 10
#define AT_PARSE_LOG  "var/at/at_parse.log"
/*���º궨��Ϊ�쳣���󷵻ؽ��*/
#define  AT_RETURN_SUSS                        0    //AT�����������ȷ���
#define  AT_RETURN_NAME_ERR              -1   //AT����������ַ���Ϊ��
#define  AT_RETURN_RESULT_ERR           -2   //AT����Ľ���ַ���Ϊ��
#define  AT_RETURN_FUNC_ERR              -3   //AT����Ĺ������Ͳ�ƥ��
#define  AT_RETURN_APP_ERR                -4   //û���ҵ�AT�����������
#define  AT_RETURN_MALLOC_ERR          -5   //�ڴ����ʧ��
#define  AT_RETURN_MODE_NAME_ERR  -6   //�����ַ���Ϊ��

/* AtCmdParse�ṹ��ΪAT������ַ��������Ժ������ṹ */
typedef struct AtCmdParseStruct
{
    char *AtFieldName;    //AT������������ɱ�׼AT����"���ս����ַ���"�ṩ��
    char *AtFieldValue;    //AT���������ֵ�����յ����Ӧ��ֵ��
    struct AtCmdParseStruct *plist;  //ָ����һ��ͬ���͵�Ԫ�е�ͬ���͵����ָ�롣
    struct AtCmdParseStruct *next;  //ָ��Ԫ�ڵĲ�ͬ���͵����ָ�롣
}AtCmdParse;  


typedef enum
{
    AT_FUNCTION_INITIATIVE_REPORT  = 0,  //�����ϱ�
    AT_FUNCTION_NETWORK_CFG          , //��������
    AT_FUNCTION_PIN_CFG                    ,   //PIN����
    AT_FUNCTION_SMS_CFG                  ,   //���Ų���
    AT_FUNCTION_BASIC_INFO_CFG      ,  //������Ϣ������
    AT_FUNCTION_VOICE_CALL_CFG      ,  //��������
    AT_FUNCTION_ADD_OPERATION      ,   //����ҵ��
    AT_FUNCTION_DATA_OPERATION    ,    //����ҵ��
    AT_FUNCTION_PHONE_RECORD       ,    //�绰��
    AT_FUNCTION_SYSTEM_CFG            ,    //ϵͳ����

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
    /* AT���������*/
    char* at_name;
    /* ��AT����Ĺ��ܽ��еķ��࣬
    �������ص����PIN�����������ȡ�*/
    int  func_cmd_type;
    /* ������ս����ַ�������Ӧ����AT��������ͺ�һЩ��������͡�
    Parse_contrast[0]��Ӧ������ģ�Parse_contrast[1]��Ӧexe����,
    Parse_contrast[2]��Ӧtest����,Parse_contrast[3]��Ӧ�����ϱ�����������ķ��ؽ��.
    */
    char* parse_contrast[4];
    /* ����������resultָ�������Ľ���ĵ�ַ��
    parse_contrast_str ���ս����ַ�����
    at_value_str��ʾδ����ǰ��at����ķ����ַ�����*/
    int (*parse_read_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
    int (*parse_exe_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
    int (*parse_test_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);
    int (*parse_other_func)(AtCmdParse **result, char* parse_contrast_str, char* at_value_str, char* at_name);

    /*���û��ѽ�����Ľ���������ṹ����ͷ��ڴ�ĺ��� */
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
