#ifndef _ATP_SYSTEM_BOOT_H
#define _ATP_SYSTEM_BOOT_H

/*added by lvxin00135113增加判断flash是否大于16M的宏*/
//#define FLASH_SIXTEEN_SIZE          0x1000000
#ifdef SUPPORT_ATP_NANDFLASH
#define FLASH_DOUBLE_SIZE          0x2000000
#else
#define FLASH_DOUBLE_SIZE          0x1000000
#endif

#define IMAGE_CHKSIGNTAG_SIZE   4
typedef enum
{
    ALWALYS_CHK_SIGN=0,
    FIR_CHK_SIGN,
    NONE_CHK_SIGN,
}UPG_SIGN_CHK;

#define SIGN_CHK_ALWALYS   "0"
#define SIGN_CHK_FIR             "1"
#define SIGN_CHK_NONE          "2"

/*end of add*/
#define IMAGE_MAIN_F             "MAINFF"
#define IMAGE_SLAVE_F            "SLAVEFF"
#define IMAGE_MAIN_S                "MAINSS"
#define IMAGE_SLAVE_S            "SLAVESS"
typedef enum
{
    BOOT_FROM_MAIN=1,
    BOOT_FROM_SLAVE,
    BOOT_FROM_NET
}UPG_BOOT_FROM;

#define FLASHFLAGBACKOFFSET             (ATP_BHAL_BBT_SIZE*ATP_KILO_SIZE)                //Flag中bbt分区的大小
#define FLASHFLAGSIZE                           (ATP_BHAL_UPG_FLAG_SIZE * ATP_KILO_SIZE)    //FlashFlag分区的大小

#define IMAGE_FLAG_SIZE           16
#define SIG_FLAG_SIZE              4
#define CON_FLAG_SIZE             16
#define REBOOT_FLAG_SIZE        5

typedef struct ATP_FLASH_FLAG
{
    unsigned char Boot_Flag[IMAGE_FLAG_SIZE];       // 主从系统
    unsigned char Sign_Flag[SIG_FLAG_SIZE];          // 启动签名
    unsigned char Con_Flag[CON_FLAG_SIZE];          //串口开关
    unsigned char Reboot_Flag[REBOOT_FLAG_SIZE];          //硬重启还是软重启标志
} FLASH_FLAG, *PFLASH_FLAG;

//#define BHAL_PPP_SESSION_SIZE               (8 * KILO_SIZE)
#define BHAL_PPP_SESSION_OFFSET              0


#define ATP_PPPC_IFC_LEN 32
#define WAN_PPP_SESSION_LEN 32
#define WAN_MAX_NUM 16

/*Added by lvxin00135113 for suport bootloader dual system upgrade*/
#define UPG_MAIN_SYS 1
#define UPG_SLAVE_SYS 2

#define MIN_CFE_SIZE                    (32 * KILO_SIZE)

#define MAX_CFE_SIZE                    (128* KILO_SIZE)

#endif /* _ATP_SYSTEM_BOOT_H */