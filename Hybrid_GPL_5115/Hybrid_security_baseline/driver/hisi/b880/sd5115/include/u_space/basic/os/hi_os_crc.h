/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_crc.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_CRC_H__
#define __HI_OS_CRC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

hi_uint32 hi_os_crc32( hi_uint32 ui_crc_base,
                        hi_uchar8 *puc_data,
                        hi_uint32 ui_data_len );

hi_ushort16 hi_os_crc16( hi_ushort16 us_crc_base, 
					hi_uchar8 *puc_data,
					hi_uint32 ui_data_len );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_CRC_H__ */

