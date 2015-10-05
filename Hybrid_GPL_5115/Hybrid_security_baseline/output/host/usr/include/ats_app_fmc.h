
#include "at_if_fmc.h"
#include "at_server_fmc.h"


/* <DTS2012071004848 liukai 20120725 PIN码自动校验功能没有实现 begin */
#define MAX_NV_12  12
#define MAX_NV_64  64
#define MAX_NV_256  256
typedef struct{    
    char acSimscid[MAX_NV_64];
    char acSaveScid[MAX_NV_64];    
    char acSavePin[MAX_NV_256];
    char acPinEnable[MAX_NV_12];
}BHAL_NVRAM_ST;
/* DTS2012071004848 liukai 20120725 PIN码自动校验功能没有实现 end> */


#define SUBNET_PLMN_NUMBER     10   //T-Mobile定制网络个数
#define PLMN_LENGTH   5             //plmn length

/*< AP7D01587,liukeke,2008-04-14,MODIFY BEGIN*/
typedef struct NV_VENDORStruct
{
    char* plmn_number;             //usim plmn number
    char* current_country;         //select country
    char* nvcurprofile_name;       //current profile
    char* nvwl_country;            //wlan country
    char* nvwan_auth;              //ppp auth
    char* vendor_numeric;              //vendor numeric
}NV_VENDOR;
/*AP7D01587,liukeke,2008-04-14,ADD END >*/


/* <AP7D00948.Zain: liukeke 2007-9-10 ADD BEGIN */
#define SIM_IMSI_LEN        16      /* IMSI号的最大长度 */
#define SIM_PLMN_LEN        8       /* PLMN的最大长度 */
/* AP7D00948.Zain: liukeke 2007-9-10 ADD END > */

//#define PROFILE_SUB_CHAR_0XFE        '\376' 
//#define PROFILE_SUB_CHAR_SAPCE       ' '
#define USIM_NOT_PLMN                       "default"
#define MAX_PROFILE_NAME_SIZE    24  /*profile name lenght*/
#define FALSE       0
#define TRUE        1
#define SCID_MAX_LEN      20

int ats_common_set_at_cmd(char* at_cmd, const char* at_match, rsp_msg* cmd_result);
int parse_scid(rsp_msg *cmd_report, StScid* result);
int get_sim_scid( char *psz_scid_string );
int check_sim_card();
int validate_pin_code();
int validate_pin(void);
int parse_pin_ex(rsp_msg* cmd_report, PinMgResultCodeEx* Result, int CmdType);
int sim_card_need_validate(int* b_is_check_pin);
void validate_pin_set_profile(void);
////////////////////////////////////
#if 0
int at_cimi_set_nvram();
#endif
int set_current_country_nvram(const char *psz_plmn_string, int flag );
 /* DTS2013071105737 z81004485 2013/7/11, Coverity delete 1 line */
int get_plmn(char *psz_plmn_string, const char *psz_imsi_string);
int check_imsi(const char *psz_imsi_string );
int set_nvram_default(int plmn_id);
int set_nvram_sub(const char *nv_flag, char *nv_name , char*nv_content);
int set_nvram_curprofile(const char *nv_flag, char *nv_name , const char *nv_content);
 /* DTS2013071105737 z81004485 2013/7/11, Coverity delete 2 lines */
/* <DTS2012041104472 c00121606 20120327 begin */
#ifdef SUPPORT_ATP_JORDAN_IMSI_ENCRYPT
unsigned long F(unsigned long x);
void Blowfish_encipher(unsigned long *xl, unsigned long *xr);
void Blowfish_decipher(unsigned long *xl, unsigned long *xr);
void InitializeBlowfish(char key[], short keybytes);
void unsigned_long_to_string (char str[], unsigned long data);
void encoded_data_transform_to_string (char * encoded_imis, unsigned long L1, unsigned long R1);
int transform_imsi_string_to_double_data(char * imsi, unsigned long * L1, unsigned long * R1);
unsigned long string_to_double (char * temp_str);
void encoded_string_transform_to_double (char * encoded_imis, unsigned long * L1, unsigned long * R1);
void decoded_data_transform_to_string (char * decoded_imsi, unsigned long L1, unsigned long R1);
#endif
/* DTS2012041104472 c00121606 20120327 end> */

