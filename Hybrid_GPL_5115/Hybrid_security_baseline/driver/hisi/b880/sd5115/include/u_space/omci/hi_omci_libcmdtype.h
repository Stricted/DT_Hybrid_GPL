/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_omci_libcmdtype.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_10_27

******************************************************************************/
#ifndef __HI_OMCI_LIBCMDTYPE_H__
#define __HI_OMCI_LIBCMDTYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum
{
    HI_OMCI_CMDTYPE_INSERT_ME_E           = ( HI_SUBMODULE_OMCI_LIB  | 0x01 ),
    HI_OMCI_CMDTPYE_DEL_ME_E              = ( HI_SUBMODULE_OMCI_LIB  | 0x02 ),
    HI_OMCI_CMDTYPE_INSERT_INST_E         = ( HI_SUBMODULE_OMCI_LIB  | 0x03 ),
    HI_OMCI_CMDTYPE_DEL_INST_E            = ( HI_SUBMODULE_OMCI_LIB  | 0x04 ),
    HI_OMCI_CMDTYPE_INSERT_CALLBACK_E     = ( HI_SUBMODULE_OMCI_LIB  | 0x05 ),
    HI_OMCI_CMDTYPE_INSERT_ATT_E          = ( HI_SUBMODULE_OMCI_LIB  | 0x06 ),
    HI_OMCI_CMDTYPE_DEL_ATT_E             = ( HI_SUBMODULE_OMCI_LIB  | 0x07 ),
    HI_OMCI_CMDTYPE_INSERT_ALARM_E        = ( HI_SUBMODULE_OMCI_LIB  | 0x08 ),
    HI_OMCI_CMDTYPE_DEL_ALARM_E           = ( HI_SUBMODULE_OMCI_LIB  | 0x09 ),
    HI_OMCI_CMDTYPE_SET_INST_E            = ( HI_SUBMODULE_OMCI_LIB  | 0x0A ),
    HI_OMCI_CMDTYPE_GET_INST_E            = ( HI_SUBMODULE_OMCI_LIB  | 0x0B ),
    HI_OMCI_CMDTYPE_LIB_DUMP_E            = ( HI_SUBMODULE_OMCI_LIB  | 0x0C ),
    HI_OMCI_CMDTYPE_LIB_DUMP_MEID_E       = ( HI_SUBMODULE_OMCI_LIB  | 0x0D ),
    HI_OMCI_CMDTYPE_LIB_INIT_MIBUPLOAD_E  = ( HI_SUBMODULE_OMCI_LIB  | 0x0E ),
    HI_OMCI_CMDTYPE_LIB_DUMP_API_E        = ( HI_SUBMODULE_OMCI_LIB  | 0x0F ),
    HI_OMCI_CMDTYPE_LIB_SAVE_API_E        = ( HI_SUBMODULE_OMCI_LIB  | 0x10 ),
    HI_OMCI_CMDTYPE_LIB_RECV_MSG_E        = ( HI_SUBMODULE_OMCI_LIB  | 0x11 ),
    HI_OMCI_CMDTYPE_CLI_SET_UPGRADEFILE_E = ( HI_SUBMODULE_OMCI_LIB  | 0x12 ),
}hi_omci_lib_cmdtype_e;

extern hi_int32 hi_omci_sdkinit( hi_void );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_OMCI_LIBCMDTYPE_H__ */
