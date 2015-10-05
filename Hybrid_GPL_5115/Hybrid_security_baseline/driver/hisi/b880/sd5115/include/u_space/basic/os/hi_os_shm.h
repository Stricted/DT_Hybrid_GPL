/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_shm.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_SHM_H__
#define __HI_OS_SHM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct
{
    hi_int32    i_key;        /* Key.  */
    hi_uint32   ui_uid;            /* Owner's user ID.  */
    hi_uint32   ui_gid;            /* Owner's group ID.  */
    hi_uint32   ui_cuid;            /* Creator's user ID.  */
    hi_uint32   ui_cgid;            /* Creator's group ID.  */
    hi_ushort16 us_mode;        /* Read/write permission.  */
    hi_ushort16 us_pad1;
    hi_ushort16 us_seq;        /* Sequence number.  */
    hi_ushort16 us_pad2;
    hi_uint32   ui_unused1;
    hi_uint32   ui_unused2;
}hi_os_ipcperm_s;

typedef struct
{
    hi_os_ipcperm_s st_ipcperm;        /* operation permission struct */
    hi_uint32       ui_segsz;            /* size of segment in bytes */
    hi_uint32       ui_atime;            /* time of last shmat() */
    hi_uint32       ui_unused1;
    hi_uint32       ui_dtime;            /* time of last shmdt() */
    hi_uint32       ui_unused2;
    hi_uint32       ui_ctime;            /* time of last change by shmctl() */
    hi_uint32       ui_unused3;
    hi_int32        i_cpid;            /* pid of creator */
    hi_int32        i_lpid;            /* pid of last shmop */
    hi_uint32       ui_nattch;        /* number of current attaches */
    hi_uint32       ui_unused4;
    hi_uint32       ui_unused5;
}hi_os_shmidds_s;

extern hi_void *hi_os_shmat(hi_int32 i_shmid, const hi_void *pv_shmaddr, hi_int32 i_shmflg);
extern hi_int32 hi_os_shmctl(hi_int32 i_shmid, hi_int32 i_cmd, hi_os_shmidds_s *pst_buf);
extern hi_int32 hi_os_shmdt(const hi_void *pv_shmaddr);
extern hi_int32 hi_os_ftok(const hi_char8 *pc_pathname,hi_int32 i_proj_id);
extern hi_int32 hi_os_shmget(hi_int32 i_key, hi_uint32 ui_size, hi_int32 i_shmflg);
extern hi_void*  hi_os_mmap(hi_void *pv_start, hi_uint32 ui_length, hi_int32 i_prot , hi_int32 i_flags, hi_int32 i_fd, hi_int32 i_offset);
extern hi_int32 hi_os_munmap(const hi_void *pv_start, hi_uint32 ui_length);
extern hi_int32  hi_os_shm_open (const hi_uchar8 *puc_name, hi_int32 i_operateflag,  hi_uint32 ui_mode);
extern hi_int32 hi_os_shm_unlink(const hi_uchar8 *puc_name);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_SHM_H__ */


