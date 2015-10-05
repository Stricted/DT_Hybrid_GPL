#ifndef __UPG_TYPES_H__
#define __UPG_TYPES_H__

#ifdef BUILD_BOOTLOADER
#include "atpconfig.h"

#ifdef HISI_CHIP
#include <common.h>
#endif

#ifdef BROADCOM_CHIP
#include "lib_types.h"
#include "lib_printf.h"
#include "lib_string.h"
#include "lib_malloc.h"
#endif

#define VOS_OK                 (0)
#define VOS_NOK                (1)
#define VOS_ERROR              (-1)
#define VOS_TRUE               1L 
#define VOS_FALSE              0L

#ifdef BROADCOM_CHIP
#define UPG_FREE                KFREE
#define UPG_MALLOC(size)        KMALLOC(size, 0)
#else
#define UPG_FREE                free
#define UPG_MALLOC(size)        malloc(size, 0)
#endif

#ifdef  HISI_CHIP
#define UPG_XTOI(str)           hi_xtoi(str) 
#else
#define UPG_XTOI(str)           xtoi(str) 
#endif


#define UPG_SNPRINTF(str, size, format, args...)  xsprintf(str, format, ##args)

struct in_addr {
    unsigned int s_addr;
};

#define __bswap_32(x) (\
    (((x) & 0x000000ff) << 24) | \
    (((x) & 0x0000ff00) <<  8) | \
    (((x) & 0x00ff0000) >>  8) | \
    (((x) & 0xff000000) >> 24))

#define __bswap_16(x) ( \
    (((x) & 0x00ff) <<  8) | \
    (((x) & 0xff00) >>  8))

#ifdef __MIPSEB
#define ntohl(x)    (x)
#define ntohs(x)    (x)
#define htonl(x)    (x)
#define htons(x)    (x)
#else
#define ntohl(x)    __bswap_32(x)
#define ntohs(x)    __bswap_16(x)
#define htonl(x)    __bswap_32(x)
#define htons(x)    __bswap_16(x)
#endif

#else
#include "atptypes.h"

#define UPG_FREE                free
#define UPG_MALLOC(size)        malloc(size)
#define UPG_XTOI(str)           strtoul(str, NULL, 16)

#ifdef WIN32
#define UPG_SNPRINTF _snprintf
#define ATP_MODULE_UPG 26
#else
#define UPG_SNPRINTF(str, size, format, args...) snprintf(str, size, format, ##args)
#endif

#endif

typedef enum tagATP_UPG_ERR_CODE_EN
{
#ifndef SUPPROT_ATP_UPG
    ATP_ERR_UPG_INVALID_PARA            = 0x1a00000,
#else
    ATP_ERR_UPG_INVALID_PARA            = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_UPG),
#endif
    ATP_ERR_UPG_INVALID_URL,
    ATP_ERR_UPG_ITEM_NOT_FOUND,
    ATP_ERR_UPG_SUBFILE_NOT_FOUND,
    ATP_ERR_UPG_PRO_NOT_SPT,
    ATP_ERR_UPG_NOT_AVAILABLE,
    ATP_ERR_UPG_CMD_TOO_LONG,
    ATP_ERR_UPG_SYSTEM,
    ATP_ERR_UPG_MEMORY,
    ATP_ERR_UPG_FORMAT_ERROR,
    ATP_ERR_UPG_FILE_NOT_FOUND,
    ATP_ERR_UPG_FILE_TOO_SHORT,
    ATP_ERR_UPG_FILE_READ,
    ATP_ERR_UPG_INVALID_INIFILE,
    ATP_ERR_UPG_INVALID_SIGNATURE,
    ATP_ERR_UPG_INVALID_KEY,
    ATP_ERR_UPG_ALG_NOT_SUPPORT,    /* 10 */
    ATP_ERR_UPG_HEAD,
    ATP_ERR_UPG_CRC,
    ATP_ERR_UPG_INVALID_PRODUCTCLASS,
    ATP_ERR_UPG_DECODE,

    // Transport Error
    ATP_ERR_UPG_TRANS_AUTH,
    ATP_ERR_UPG_TRANS_FILE,
    ATP_ERR_UPG_TRANS_SYSTEM,
    ATP_ERR_UPG_TRANS_TIMEOUT,
    ATP_ERR_UPG_TRANS_UNKNOWN,

    // Upg return code
    ATP_ERR_UPG_SKIP_CUR_PROC
} ATP_UPG_ERR_CODE_EN;


#endif

