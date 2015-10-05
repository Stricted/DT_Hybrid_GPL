/*****************************************************************************
 *               Copyright (C) 2013, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file syscfgcms.c
  *\copyright 2007-2020，华为技术有限公司
  *\author w00189018
  *\date  2013-02-27
*/

/*  <DTS2013022804321 wanglong 2012-2-28 begin */

#ifndef __SYSCTL_CMSAPI_API_H__
#define __SYSCTL_CMSAPI_API_H__
/* <DTS2013062201618 z81004143 2013/6/20 begin */
#include "atptypes.h"

#if ATP_DEBUG
#define ATP_SYSCFG_DEBUG(format, args...)   printf("Syscfg [%s] line[%d] [%s] ", __FILE__, __LINE__, (char *)(__FUNCTION__)); printf(format "\r\n", ## args) /* DTS2013082500170 z81004143 2013/9/2 */
#else
#define ATP_SYSCFG_DEBUG(format, args...) 
#endif

#ifdef SUPPORT_ATP_PLMN_AUTO_MATCH
#define PLNUM                                           7
#define IMSI_TMP_FILE                                   "/var/log/imsi_match"
#define PLMN_LEN_5                                      5
#define STRING_LEN_64                                   64
#define LANG_AUTO_MODE                                  "auto"

#define INVALID_INSTANCE_INDEX     0    /* 无效实例索引 */
#define PLMN_NOT_MATCH  0   /* 标志当前没有匹配到plmn */

/* <DTS2013070300449 z81004143 2013/7/3 begin */
#define RETRY_ENABLE_FLAG      1        /* 表示需要retry */
#define RETRY_DISABLE_FLAG     0        /* 表示已经retry过，或者不需要retry */
/* DTS2013070300449 z81004143 2013/7/3 end> */

typedef enum
{    
    PROFILE_MATCH_NOR  = 0, /* 0: profile采用默认配置方式。仅将匹配到的profile作为默认profile
                                    （B890_c282_Singapore） */
    PROFILE_MATCH_HIDDEN_OTHER, /* 1: 将匹配到的profile作为默认profile，将没有被匹配到的profile隐藏起来，不在web上显示。
                                        （B190_c64_H3g） */
}E_PROFILE_MATCH_MODE;

typedef enum
{
    /* 语言匹配模式 */
    LANG_MATCH_NOR  = 0, /* 0: 不存在auto模式，只要匹配到新卡就更新当前语言 */
    LANG_MATCH_ONLY_IN_AUTO, /* 1: 存在auto模式，只有在auto模式下，匹配到新卡才更新当前语言（B190_c64_H3g） */
}E_LANGUAGE_MATCH_MODE;

/* <DTS2013070300449 z81004143 2013/7/3 begin */
typedef enum
{
    /* apn retry mode */
    RETRY_NONE  = 0,         /* 0: 不进行retry */
    RETRY_MATCH_AND_USR_ADD, /* 1: retry plmn匹配到的profile和用户通过web页面添加的profile */
}E_APN_RETRY_MODE;
/* DTS2013070300449 z81004143 2013/7/3 end> */

VOS_UINT32 ATP_SYSCFG_Init(VOS_VOID);
VOS_VOID Msg_Oper_AT_Server(const ATP_MSG_HEADER_ST *pstMsg);
VOS_VOID plmn_match_add_set();
void PAM_lang_to_auto_format(VOS_CHAR *language, VOS_UINT32 lang_len);
VOS_BOOL PAM_get_auto_lang(void);
void PAM_set_auto_lang(VOS_CHAR *language);
void PAM_set_lang(VOS_CHAR *language);
E_LANGUAGE_MATCH_MODE PAM_get_lang_match_mode(void);
E_PROFILE_MATCH_MODE PAM_get_profile_match_mode(void);
VOS_UINT32 PAM_get_custom_profile_num(void);
VOS_UINT32 PAM_get_usr_add_fst_prof_idx(void);
VOS_UINT32 PAM_get_cur_match_profile_idx(void);
/* <DTS2013070300449 z81004143 2013/7/3 begin */
E_APN_RETRY_MODE PAM_get_apn_retry_mode(void);
VOS_BOOL PAM_file_is_exist(char * filename);
void PAM_retry_flag_init(void);
void PAM_retry_flag_disable(VOS_UINT32 profile_idx);
void PAM_retry_flag_enable(VOS_UINT32 profile_idx);
VOS_CHAR PAM_retry_flag_get(VOS_UINT32 profile_idx);
VOS_UINT32 PAM_retry_get_next_prof_idx(VOS_UINT32 cur_profile_idx);
/* DTS2013070300449 z81004143 2013/7/3 end> */
/* DTS2013062201618 z81004143 2013/6/20 end> */
#endif
#endif
/*  DTS2013022804321 wanglong 2012-2-28 end> */

