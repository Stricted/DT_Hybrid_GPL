/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : hi_os_unistdio.h
  �� �� ��   : ����
  ��    ��   : l00183967
  ��������   : D2011_08_06
  ����޸�   :

******************************************************************************/
#ifndef __HI_OS_UNISTDIO_H__
#define __HI_OS_UNISTDIO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern hi_int32 hi_os_close(hi_int32 i_fd);
extern hi_int32 hi_os_creat(const hi_char8 *pc_pathname, hi_uint32 ui_mode);
extern hi_int32 hi_os_lseek(hi_int32 i_fildes, hi_int32 i_offset, hi_int32 i_whence);
extern hi_int32 hi_os_open(const hi_char8 *pc_pathname, hi_int32 i_flags, hi_uint32 ui_mode);
extern hi_int32 hi_os_read(hi_int32 i_fd, hi_void *pv_buf, hi_uint32 ui_count);
extern hi_int32 hi_os_write(hi_int32 i_fd, const hi_void *pv_buf, hi_uint32 ui_count);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_UNISTDIO_H__ */
