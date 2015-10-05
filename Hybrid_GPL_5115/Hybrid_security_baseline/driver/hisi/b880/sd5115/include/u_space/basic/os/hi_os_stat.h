/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_stat.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_03
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_STAT_H__
#define __HI_OS_STAT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct {
    hi_uint32   st_dev;      /* device */
    hi_uint32   st_ino;      /* inode */
    hi_ushort16 st_mode;
    hi_ushort16 st_nlink;
    hi_ushort16 st_uid;
    hi_ushort16 st_gid;
    hi_uint32   st_rdev;      /* device */
    hi_uint32   st_size;
    hi_uint32   st_blksize;  /* blocksize for filesystem I/O */
    hi_uint32   st_blocks;   /* number of blocks allocated */
     #if 1
    //hi_uint32   st_atime;            /* Time of last access  */
    hi_uint32   st_atime_nsec;    /* Nscecs of last access  */
    //hi_uint32   st_mtime;            /* Time of last modification  */
    hi_uint32   st_mtime_nsec;    /* Nsecs of last modification  */
    //hi_uint32   st_ctime;            /* Time of last status change  */
    hi_uint32   st_ctime_nsec;    /* Nsecs of last status change  */
    #endif
    hi_uint32   ui_unused4;
    hi_uint32   ui_unused5;
}hi_os_fstat_s;

extern hi_int32 hi_os_fstat(const hi_int32 i_fd, hi_os_fstat_s *pst_buf);
extern hi_int32 hi_os_stat(const hi_char8 *pc_filename, hi_os_fstat_s *pst_buf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_STAT_H__ */

