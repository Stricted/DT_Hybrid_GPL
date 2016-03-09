#ifndef __SUPPORT_LUA_CFM_H__
#define __SUPPORT_LUA_CFM_H__
#include "atptypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#define ATP_LCFM_MODULE_NAME_D      "dm"

typedef struct stATP_LUA_CTX_ST
{
    lua_State               *pstLuaState;
    const VOS_CHAR          *pcErrorString;
    VOS_UINT32              ulErrorCode;
    VOS_VOID                *pvEnv;        /* 扩展字段 */
} ATP_LUA_CTX_ST;

extern const luaL_Reg g_astLcfmLib[];
extern VOS_BOOL cfmIsStringStyleOID(VOS_UINT32 ulOID);

extern VOS_CHAR *ScanIconvTryConvert(const VOS_CHAR *pszInput);

extern VOS_CHAR *HostNameIconvTryConvert(const VOS_CHAR *pszInput);


typedef VOS_UINT32 (*PFATP_LUA_OpenLib)(lua_State *pstLuaState);

/*!
  \brief 创建lua context
  \param[in]  *pfLuaOpenLibs: 自定义libs 数组
  \param[in]  ulSize: 数组长度
  \return 
  \retval ATP_LUA_CTX_ST* lua ctx
*/
ATP_LUA_CTX_ST *ATP_LUA_Init(const PFATP_LUA_OpenLib *pfLuaOpenLibs, VOS_UINT32 ulSize);

/*!
  \brief 删除lua context
  \param[in]  *pstLuaCtx: 
  \return 
  \retval 
*/
VOS_VOID        ATP_LUA_UnInit(ATP_LUA_CTX_ST *pstLuaCtx);

/*!
  \brief 执行lua 脚本
  \param[in]  *pstLuaCtx: 
  \param[in]  *pcScriptStr: 
  \return 
  \retval VOS_OK 
*/
VOS_UINT32 ATP_LUA_ExecuteScript(
                    ATP_LUA_CTX_ST     *pstLuaCtx,
                    const VOS_CHAR      *pcScriptStr);

/*!
  \brief 执行lua文件
  \param[in]  *pstLuaCtx: 
  \param[in]  *pcFileName: 
  \return 
  \retval VOS_OK
*/
VOS_UINT32 ATP_LUA_DoFile(ATP_LUA_CTX_ST     *pstLuaCtx,
                    const VOS_CHAR      *pcFileName);

VOS_UINT32 ATP_LUA_DoString(ATP_LUA_CTX_ST     *pstLuaCtx,
                    const VOS_CHAR      *pcScript);


VOS_UINT32 ATP_LCFM_OpenCfm(lua_State *pstLuaState);

VOS_INT ATP_LCFM_LogLua(lua_State *pstLuaState);
VOS_INT ATP_LCFM_GetParameterNames(lua_State *pstLuaState);
VOS_INT ATP_LCFM_GetParameterValues(lua_State *pstLuaState);
VOS_INT ATP_LCFM_SetParameterValues(lua_State *pstLuaState);

VOS_INT ATP_LCFM_AddObjectWithValues(lua_State *pstLuaState);
VOS_INT ATP_LCFM_StartTransaction(lua_State *pstLuaState);
VOS_INT ATP_LCFM_EndTransaction(lua_State *pstLuaState);

VOS_INT ATP_LCFM_AddObject(lua_State *pstLuaState);
VOS_INT ATP_LCFM_DeleteObject(lua_State *pstLuaState);
VOS_INT ATP_LCFM_ExecuteResults(lua_State *pstLuaState);
VOS_INT ATP_IconvConvert(lua_State *pstLuaState);
VOS_INT ATP_HostNameIconvConvert(lua_State *pstLuaState);




#ifdef ATP_DEBUG
#define ATP_LUACFM_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_LUACFM_DEBUG(x...)
#endif

#ifdef __cplusplus
}
#endif
#endif

