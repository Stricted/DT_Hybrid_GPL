/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file luaweb.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2012-5-3
*/
#ifndef __LUAWEB_H_
#define __LUAWEB_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
#include "lua.h"
    
#include "lauxlib.h"
#include "lualib.h"
#include "httpapi.h"
#include "webapi.h"

#define ATP_LWEB_MODULE_NAME_D      "web"
#define ATP_JSON_OK                 "ok"
#define ATP_JSON_FAIL               "fail"

extern VOS_BOOL g_bRebooting;
extern VOS_BOOL g_bAskAssist;
extern const luaL_Reg g_astLwebLib[];
extern VOS_BOOL g_bDoAutoUpgrade;

/*add by w00191600  2012-10-24 for 语音电话本排序*/
#ifndef __QUICK_SORT_H__
#define __QUICK_SORT_H__

#define MAX_NAME_LEN  130
#define ASCII_MAXVALUE 0x80
#define MAX_CONTACT_ENTRY  251


typedef struct tagName
{
    int m_ui_id;
    char m_sz_name[MAX_NAME_LEN];
    char m_sz_fstname[MAX_NAME_LEN];
}ST_CONTACT_SORT;

enum __UMLAUT_TYPE
{
    FIR_UMLAUT = 1,
    SEC_UMLAUT = 2,
    DOUBLE_UMLAUT = 3
};

// 判断是否为小写字母
#define IS_LOWER(c) (0x61 <= (c) && 0x7a >= (c))

// 转换小写字母为大写字母(前提:传入的参数必须是小写字母)
#define TO_UPPER(c) ((c) - 0x20)

enum __UPPER
{
    UPPER_A = 0x41,
    LOWER_A = 0x61,
    
    UPPER_E = 0x45,
    LOWER_E = 0x65,
    
    UPPER_I = 0x49,
    LOWER_I = 0x69,
    
    UPPER_N = 0x4e,
    LOWER_N = 0x6e,
    
    UPPER_O = 0x4f,
    LOWER_O = 0X6f,
    
    UPPER_S = 0x53,
    LOWER_S = 0x73,
    
    UPPER_U = 0x55,
    LOWER_U = 0x75,
    
    UPPER_Y = 0x59,
    LOWER_Y = 0x79,
    
    UPPER_Z = 0x5a,
    LOWER_Z = 0x7a
};

enum __UMLAUT
{
    // Latin Capital Letter A With Grave
    LCL_A_GRAVE = 0xc380,

    // Latin Capital Letter A With Acute
    LCL_A_ACUTE = 0xc381,

    // Latin Capital Letter A With Circumflex
    LCL_A_CIRCUMFLEX = 0xc382,

    // Latin Capital Letter A With Tilde
    LCL_A_TILDE = 0xc383,

    // Latin Capital Letter A With Diaeresis
    LCL_A_DIAERESIS = 0xc384,

    // Latin Capital Letter A With Ring Above
    LCL_A_RINGABOVE = 0xc385,

    // Latin Small Letter A With Grave
    LSL_A_GRAVE = 0xc3a0,

    // Latin Small Letter A With Acute
    LSL_A_ACUTE = 0xc3a1,

    // Latin Small Letter A With Circumflex
    LSL_A_CIRCUMFLEX = 0xc3a2,

    // Latin Small Letter A With Tilde
    LSL_A_TILDE = 0xc3a3,

    // Latin Small Letter A With Diaeresis
    LSL_A_DIAERESIS = 0xc3a4,

    // Latin Small Letter A With Ring Above
    LSL_A_RINGABOVE = 0xc3a5,

    // Latin Capital Letter E With Grave
    LCL_E_GRAVE = 0xc388,

    // Latin Capital Letter E With Acute
    LCL_E_ACUTE = 0xc389,

    // Latin Capital Letter E With Circumflex
    LCL_E_CIRCUMFLEX = 0xc38a,

    // Latin Capital Letter E With Diaeresis
    LCL_E_DIAERESIS = 0xc38b,

    // Latin Small Letter E With Grave
    LSL_E_GRAVE = 0xc3a8,

    // Latin Small Letter E With Acute
    LSL_E_ACUTE = 0xc3a9,

    // Latin Small Letter E With Circumflex
    LSL_E_CIRCUMFLEX = 0xc3aa,

    // Latin Small Letter E With Diaeresis
    LSL_E_DIAERESIS = 0xc3ab,

    // Latin Capital Letter I With Grave
    LCL_I_GRAVE = 0xc38c,

    // Latin Capital Letter I With Acute
    LCL_I_ACUTE = 0xc38d,

    // Latin Capital Letter I With Circumflex
    LCL_I_CIRCUMFLEX = 0xc38e,

    // Latin Capital Letter I With Diaeresis
    LCL_I_DIAERESIS = 0xc38f,

    // Latin Small Letter I With Grave
    LSL_I_GRAVE = 0xc3ac,

    // Latin Small Letter I With Acute
    LSL_I_ACUTE = 0xc3ad,

    // Latin Small Letter I With Circumflex
    LSL_I_CIRCUMFLEX = 0xc3ae,

    // Latin Small Letter I With Diaeresis
    LSL_I_DIAERESIS = 0xc3af,

    // Latin Capital Letter N With Tilde
    LCL_N_TILDE = 0xc391,

    // Latin Small Letter N With Tilde
    LSL_N_TILDE = 0xc3b1,

    // Latin Capital Letter O With Grave
    LCL_O_GRAVE = 0xc392,

    // Latin Capital Letter O With Acute
    LCL_O_ACUTE = 0xc393,

    // Latin Capital Letter O With Circumflex
    LCL_O_CIRCUMFLEX = 0xc394,

    // Latin Capital Letter O With Tilde
    LCL_O_TILDE = 0xc395,

    // Latin Capital Letter O With Diaeresis
    LCL_O_DIAERESIS = 0xc396,

    // Latin Small Letter O With Grave
    LSL_O_GRAVE = 0xc3b2,

    // Latin Small Letter O With Acute
    LSL_O_ACUTE = 0xc3b3,

    // Latin Small Letter O With Circumflex
    LSL_O_CIRCUMFLEX = 0xc3b4,

    // Latin Small Letter O With Tilde
    LSL_O_TILDE = 0xc3b5,

    // Latin Small Letter O With Diaeresis
    LSL_O_DIAERESIS = 0xc3b6,

    // Latin Capital Letter S With Caron
    LCL_S_CARON = 0xc5a0,

    // Latin Small Letter Sharp S
    LSL_S_SHARP = 0xc39f,

    // Latin Small Letter S With Caron
    LSL_S_CARON = 0xc5a1,

    // Latin Capital Letter U With Grave
    LCL_U_GRAVE = 0xc399,

    // Latin Capital Letter U With Acute
    LCL_U_ACUTE = 0xc39a,

    // Latin Capital Letter U With Circumflex
    LCL_U_CIRCUMFLEX = 0xc39b,

    // Latin Capital Letter U With Diaeresis
    LCL_U_DIAERESIS = 0xc39c,

    // Latin Small Letter U With Grave
    LSL_U_GRAVE = 0xc3b9,

    // Latin Small Letter U With Acute
    LSL_U_ACUTE = 0xc3ba,

    // Latin Small Letter U With Circumflex
    LSL_U_CIRCUMFLEX = 0xc3bb,

    // Latin Small Letter U With Diaeresis
    LSL_U_DIAERESIS = 0xc3bc,

    // Latin Capital Letter Y With Diaeresis
    LCL_Y_DIAERESIS = 0xc5b8,

    // Latin Capital Letter Y With Acute
    LCL_Y_ACUTE = 0xc39d,

    // Latin Small Letter Y With Acute
    LSL_Y_ACUTE = 0xc3bd,

    // Latin Small Letter Y With Diaeresis
    LSL_Y_DIAERESIS = 0xc3bf,

    // Latin Capital Letter Z With Caron
    LCL_Z_CARON = 0xc5bd,

    // Latin Small Letter Z With Caron
    LSL_Z_CARON = 0xc5be
};

#endif
/*end add by w00191600  2012-10-24 for 语音电话本排序*/


VOS_INT32 ATP_LWEB_Printf(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_JsonEncode(lua_State *pstLuaState);

VOS_UINT32 ATP_WEB_LuaDoGetJsonMethodProc(
                            ATP_HTTP_CLIENT_ST      *pstHttpClient,
                            FILE                    *pstFile,
                            VOS_INT32               lFileLen);

VOS_UINT32 ATP_WEB_LuaDoGetLuaMethodProc(
                            ATP_HTTP_CLIENT_ST      *pstHttpClient,
                            FILE                    *pstFile,
                            VOS_INT32               lFileLen);


VOS_UINT32 ATP_WEB_LuaDoPostMethodProc(
                            ATP_HTTP_CLIENT_ST          *pstHttpClient,
                            const VOS_CHAR              *pcUri,
                            ATP_WEB_NVPAIR_ST           *apstNvs,
                            VOS_UINT32                  ulNumOfNvs);

VOS_UINT32 LwebSetResponseValues(lua_State *pstLuaState,
                                                const VOS_CHAR *pcKey,
                                                const VOS_CHAR *pcValue);



VOS_INT32 ATP_LWEB_WriteLogToFile(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_ExecCmdNoHang(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetMemAndCpuInfo(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetVersionType(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_setAskAssitant(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_ContactSort(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetDNSMemory(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetLteInfo(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetHGRoutable(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetHybridInfo(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetWlanVer(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetWebVer(lua_State *pstLuaState);
VOS_INT32 ATP_LWEB_GetChallengeValue(lua_State *pstLuaState);

typedef VOS_UINT32 (*PfLuaSetGlobalValues)(lua_State *pstLuaState,  ATP_HTTP_CLIENT_ST          *pstHttpClient);

extern PfLuaSetGlobalValues g_pfLuaSetGlobalValues;

#ifdef __cplusplus
}
#endif

#endif

