/*!\mainpage ATP (Access Terminal Platform) application interface document
  *\section intro_sec Introduction
  *     ATP是一个CPE类网关设备的软件平台，用于快速进行接入类产品开发。
  *     ATP主要包括以下几个层次
  *         - Kernel & Driver Adapt Layer        硬件及内核适配层
  *         - Core Application Service             系统核心服务，此类服务包括动态库及核心服务进程
  *         - Network Service                         网络服务，提供各种网络协议的支持
  *         - Configuration Service                  配置服务，提供各种配置管理的支持，包括web, tr069等，遵循BBF论坛的标准
  *         - Voice Service                             语音服务，提供各种语音业务，包括SIP, PSTN, ISDN, DECT, CS等。
  *         - Application Service                     特色应用服务，包括DLNA，存储管理等，后续还会扩展OSGI等的支持。
  *\section kdal_sec Kernel & Driver Adapt Layer
  *\section core_sec Core Application Service
  *\section network_sec Network Service
  *\section config_sec Configuration Service
  *\section voice_sec Voice Service
  *\section app_sec Application Service
  *\section copyrigh_sec Copy Right
  *\copyright  2007-2020，华为技术有限公司
  *\author      z65940
  *\date         2011-12-17
  
*********************************************************************************/

#ifndef __ATP_TYPES_H__
#define __ATP_TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "atpconfig.h"
#include "atpcbbid.h"
#include "atpconst.h"

typedef const char  VOS_CONST_CHAR;
#define VOS_NOK                 1L

//Add by h00170023 for ATP_VERSION_CMD_KEY
#define ATP_VERSION_CMD_KEY     "atpv"

#define ATP_ARRAY_LEN(x)        ((sizeof(x))/(sizeof(((x)[0]))))

#define ATP_FREE(x) \
{\
    if (x)\
    {\
        free(x);\
    }\
}
#define ATP_CHECK_RETURN(ret)   \
{\
    if (VOS_OK != ret)\
    {\
        return ret;\
    }\
}
#ifdef SUPPORT_DESKTOP
#define ATP_THREAD_SAFE     __thread
#else
#define ATP_THREAD_SAFE
#endif

/* if vos.h is not included */
#ifndef  _V_TYPDEF_H

/************************* MACRO & TYPE DEFINITION ***************************/
#define VOS_NULL               (0)
#define VOS_NULL_PTR           (0L)

#define VOS_OK                 (0)
#define VOS_ERROR              (-1)
#define VOS_TRUE               1L 
#define VOS_FALSE              0L

#ifndef  VOS_TYPE
#define  VOS_TYPE               1
#if defined(__cplusplus)
#define                     VOS_VOID void
#else
typedef void                    VOS_VOID;
#endif

typedef unsigned char           VOS_UINT8;
typedef char                    VOS_INT8;
typedef char                    VOS_CHAR;

typedef unsigned short          VOS_UINT16;
typedef signed short            VOS_INT16;

typedef unsigned int           VOS_UINT32;
typedef signed int             VOS_INT32;

typedef unsigned int            VOS_UINT;
typedef int                     VOS_INT;
#endif

typedef unsigned int           VOS_BOOL;

typedef float                   VOS_FLOAT;
typedef double                  VOS_DOUBLE;

typedef unsigned int           VOS_SOCKET;

typedef unsigned long long      VOS_UINT64;
typedef long long               VOS_INT64;
#endif /* _V_TYPDEF_H */

#define    VTOP_LITTLE_ENDIAN        1234 /* least-significant byte first (vax) */
#define    VTOP_BIG_ENDIAN           4321 /* most-significant byte first (IBM, net) */

#define VTOP_LITTLE_BYTE_ORDER

#ifdef VTOP_LITTLE_BYTE_ORDER
#define    VTOP_BYTE_ORDER          VTOP_LITTLE_ENDIAN  
#endif

#ifdef VTOP_BIG_BYTE_ORDER
#define    VTOP_BYTE_ORDER          VTOP_BIG_ENDIAN  
#endif


typedef enum tagATP_TEAM_EN
{
    ATP_TEAM_MNT        = 1,
    ATP_TEAM_SYS,
    ATP_TEAM_NTWK,
    ATP_TEAM_VOICE,
    ATP_TEAM_CMS,
} ATP_TEAM_EN;

#define ATP_MNT_ERR_CODE_DEF(team, module)    ((module) << 16)
#define ATP_SEC_KEYID_CODE_DEF(module)  ((module) << 16)

#ifndef ATP_MSG_LOCAL_HOST
#define ATP_MSG_LOCAL_HOST        ("127.0.0.1")
#endif
#ifndef ATP_MSG_LOCAL_PORT
#define ATP_MSG_LOCAL_PORT        (9267)
#endif

#ifdef SUPPORT_DESKTOP
#define ATP_MSG_SVR_PATH        "../var/msgsvr"
#else
#define ATP_MSG_SVR_PATH        "/var/msgsvr"
#endif


#ifdef SUPPORT_UNIT_TEST
#define ATP_STATIC
#else
#define ATP_STATIC              static
#endif

#define ATP_SOCK_INVALID_FD     (-1)
#define ATP_PID_INVALID_NUM     (-1)

#define ATP_DIGIT_STR_MAX_LEN_D     (32)

#define ATP_INSTALL_BIN_DIR_D       "/mupg/install"
#define ATP_TMP_BIN_DIR_D           "/mupg/slave"


/****************************************************************************
 BELOW IS OID MAP:

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |I|W|S|K|N|M|VTYPE| MPROC |X_E|  OBJECT_ID      | PARAMETER_ID  |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 I: if TRUE, object has multi-INSTANCE, else object has single-instance.
 W: if TRUE, object or parameter can WRITE, or cannot write.
 S: if TRUE, object or parameter need to SAVE to database.
 K: if TRUE, parameter is KEY value.
 N: if FALSE, patameter cannot set attribute NOTIFY.
 M: if TRUE, get or get inst MSG need to send to process.
 VTYPE: VALUE TYPE: 000:object, 001:string, 010:int, 011:unsignedint, 100:bool, 101:datetime,
                    110:base64 , 111: anysimpletype
 MPROC: MAIN PROC
 OBJECT_ID: 9bit
 PARAMETER_ID: 8bit
 X_E: 2bit X_EXTENTION VALUE: 0, 1, 2, 3
****************************************************************************/
#define CMO_OBJ_HAS_INSTANCE        0x80000000
#define CMO_OBJ_ATTR_HAS_VALUE      0x80000000
#define CMO_OBJ_ATTR_CAN_WRITE      0x40000000
#define CMO_OBJ_ATTR_CAN_SAVE       0x20000000
#define CMO_ATTR_IS_KEY             0x10000000
#define CMO_ATTR_IS_NOTIFY          0x08000000
#define CMO_ATTR_SEND_GET_MSG       0x04000000
#define CMO_DATA_TYPE_OBJECT        0x00000000
#define CMO_DATA_TYPE_STRING        0x00800000
#define CMO_DATA_TYPE_INT           0x01000000
#define CMO_DATA_TYPE_UINT          0x01800000
#define CMO_DATA_TYPE_BOOL          0x02000000
#define CMO_DATA_TYPE_DATETIME      0x02800000
#define CMO_DATA_TYPE_BASE64        0x03000000
#define CMO_DATA_TYPE_UINTL         0x03800000
#define CMO_DATA_TYPE_MASK          0x03800000

#define CMO_NODE_INDEX_MASK         0x0FFFFF00
#define CMO_NODE_OBJID_MASK         0x0001FF00
#define CMO_MAIN_PROC_MASK          0x00780000
#define CMO_X_EXT_MASK              0x00060000

#define IS_OBJECT_HAS_INSTANCE(x)   ((((x)&CMO_OBJ_HAS_INSTANCE) == 0) ? VOS_FALSE : VOS_TRUE)
#define IS_ATTR_HAS_VALUE(x)        ((((x)&CMO_OBJ_HAS_INSTANCE) == 0) ? VOS_FALSE : VOS_TRUE)
#define IS_ATTR_CAN_WRITE(x)        ((((x)&CMO_OBJ_ATTR_CAN_WRITE) == 0) ? VOS_FALSE : VOS_TRUE)
#define IS_ATTR_CAN_SAVE(x)         ((((x)&CMO_OBJ_ATTR_CAN_SAVE) == 0) ? VOS_FALSE : VOS_TRUE)
#define IS_ATTR_IS_KEY(x)           ((((x)&CMO_ATTR_IS_KEY) == 0) ? VOS_FALSE : VOS_TRUE)
#define IS_ATTR_SEND_GET_MSG(x)     ((((x)&CMO_ATTR_SEND_GET_MSG) == 0) ? VOS_FALSE : VOS_TRUE)
#define IS_ATTR_NOTIFY(x)           ((((x)&CMO_ATTR_IS_NOTIFY) == 0) ? VOS_FALSE : VOS_TRUE)

#define IS_TYPE_OBJ(x)              ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_OBJECT)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_STRING(x)           ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_STRING)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_INT(x)              ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_INT)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_UINT(x)             ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_UINT)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_BOOL(x)             ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_BOOL)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_DATETIME(x)         ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_DATETIME)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_BASE64(x)           ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_BASE64)?VOS_TRUE : VOS_FALSE)
#define IS_TYPE_UINTL(x)            ((((x)&CMO_DATA_TYPE_MASK) == CMO_DATA_TYPE_UINTL)?VOS_TRUE : VOS_FALSE)

#define IS_CMO_OBJECT(x)            ((((x)&0x000000FF) == 0) ? VOS_TRUE: VOS_FALSE)
#define IS_CMO_ATTRIBUTE(x)         ((((x)&0x000000FF) != 0) ? VOS_TRUE: VOS_FALSE)

#define GET_PARA_IDX(x)             ((((x)&0x000000FF) == 0) ? 0: (((x)&0x000000FF) - 1))
#define GET_OBJ_CMO(x)              ((x)&0xFFFFFF00)
#define GET_DATA_TYPE(x)            ((x)&0x03800000)
#define GET_OBJID_VALUE(x)          (((x)&CMO_NODE_OBJID_MASK) >> 8)
#define GET_MAINPROC(x)             (((x)&CMO_MAIN_PROC_MASK) >> 19)
#define GET_EXT_IDX(x)              (((x)&CMO_X_EXT_MASK) >> 17)

#define NOT_USE 0

#define MULTI_INSTANCE 1
#define SINGLE_INSTANCE 0

#define WRITE_TRUE 1
#define WRITE_FALSE 0

#define SAVE_TRUE 1
#define SAVE_FALSE 0

#define KEY_TRUE 1

#define KEY_FALSE 0

#define CMO_DEF(InstFlag, WriteFlag, SaveFlag, KeyFlag, NotifyFlag, GetMsgFlag, ValueType, MainProc, ObjID, ParaID)\
        (unsigned int)( ((InstFlag)<<31) | ((WriteFlag)<<30) | ((SaveFlag)<<29) | ((KeyFlag)<<28) | ((NotifyFlag)<<27) | ((GetMsgFlag)<<26) | (ValueType)<<23 | (MainProc)<<19 | ((ObjID)<<8) | (ParaID) )


// BELOW IS LINT MACRO FOR ATP CMS

#ifdef ATP_PCLINT
#define SET_MSG_LINT_MACRO \
{ \
    ulObjId      = ulObjId; \
    ulNumOfPara  = ulNumOfPara; \
    ulNextObjId  = ulNextObjId; \
    pulSetValues = pulSetValues; \
    pulChgParas  = pulChgParas; \
    pulOldValues = pulOldValues;\
    aucInstId    = aucInstId;\
}

#define GET_MSG_LINT_MACRO \
{ \
    ulObjId      = ulObjId; \
    ulNumOfPara  = ulNumOfPara; \
    aucInstId    = aucInstId; \
    pbFocusParas = pbFocusParas; \
    pulGetdValues = pulGetdValues; \
}

#define TR181_SET_MSG_LINT_MACRO \
{ \
    ulObjId      = ulObjId; \
    ulNumOfPara  = ulNumOfPara; \
    ulNextObjId  = ulNextObjId; \
    pulSetValues = pulSetValues; \
    pulChgParas  = pulChgParas; \
    pulOldValues = pulOldValues;\
    aucInstId    = aucInstId;\
    bSetObj = bSetObj;\
}

#define TR181_GET_MSG_LINT_MACRO \
{ \
    ulObjId      = ulObjId; \
    ulNumOfPara  = ulNumOfPara; \
    aucInstId    = aucInstId; \
    pulFocusParas = pulFocusParas; \
    pulGetdValues = pulGetdValues; \
}

#define CHECK_MSG_LINT_MACRO \
{ \
    ulObjId      = ulObjId; \
    ulNumOfPara  = ulNumOfPara; \
    pulSetValues = pulSetValues; \
    pulOldValues = pulOldValues; \
    aucInstId    = aucInstId; \
}

#define DEL_MSG_LINT_MACRO \
{ \
    ulObjId      = ulObjId; \
    aucInstId    = aucInstId; \
}

#define GETINSTNUM_MSG_LINT_MACRO \
{ \
    aucInstId  = aucInstId;   \
    ulObjId    = ulObjId;     \
    ulArrayLen = ulArrayLen;  \
    pulInstValues = pulInstValues; \
    pulNumOfInst = pulNumOfInst; \
}
#else

#define SET_MSG_LINT_MACRO
#define GET_MSG_LINT_MACRO
#define TR181_SET_MSG_LINT_MACRO
#define TR181_GET_MSG_LINT_MACRO
#define CHECK_MSG_LINT_MACRO
#define DEL_MSG_LINT_MACRO
#define GETINSTNUM_MSG_LINT_MACRO

#endif

#endif /* __VTOP_TYPES_H__ */

