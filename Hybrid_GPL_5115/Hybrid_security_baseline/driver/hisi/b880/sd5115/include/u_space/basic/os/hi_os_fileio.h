/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_fileio.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_06
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_FILEIO_H__
#define __HI_OS_FILEIO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef hi_int32 ( *hi_os_scandir_selectcallback )(const struct dirent *);
typedef hi_void* hi_os_dir;

#define HI_PROT_NONE                            0x0   /* Page can not be accessed.  */
#define HI_PROT_READ                            0x1   /* Page can be read.  */
#define HI_PROT_WRITE                           0x2   /* Page can be written.  */
#define HI_PROT_EXEC                            0x4   /* Page can be executed.  */

#define HI_SEEK_SET                             0    /* Seek from beginning of file.  */
#define HI_SEEK_CUR                             1    /* Seek from current position.  */
#define HI_SEEK_END                             2    /* Seek from end of file.  */

#define HI_FILENAME_MAX_LEN                     255
#define HI_FILE_BUFFER_SIZE                     512
#define HI_EOF                                  EOF

/* MAP宏添加 */
#define HI_MAP_SHARED                           MAP_SHARED
#define HI_MAP_PRIVATE                          MAP_PRIVATE
#define HI_MAP_ANONYMOUS                        MAP_ANONYMOUS
#define HI_MAP_DENYWRITE                        MAP_DENYWRITE
#define HI_MAP_LOCKED                           MAP_LOCKED
#define HI_MAP_FAILED                           MAP_FAILED

/* 文件操作 */
#define HI_F_OK                                 0     /* Test for existence.  */
#define HI_X_OK                                 1     /* Test for execute permission.  */
#define HI_W_OK                                 2     /* Test for write permission.  */
#define HI_R_OK                                 4     /* Test for read permission.  */

#define HI_GENERIC_READ                         HI_O_RDONLY
#define HI_GENERIC_WRITE                        HI_O_WRONLY
#define HI_GENERIC_EXECUTE                      HI_O_EXCL
#define HI_GENERIC_ALL                          HI_O_RDWR

/* 访问模式与权限 */
#define HI_O_RDONLY                             O_RDONLY    /* 只读 */
#define HI_O_WRONLY                             O_WRONLY
#define HI_O_RDWR                               O_RDWR
#define HI_O_CREAT                              O_CREAT
#define HI_O_EXCL                               O_EXCL
#define HI_O_TRUNC                              O_TRUNC
#define HI_O_APPEND                             O_APPEND
#define HI_O_NOCTTY                             O_NOCTTY
#define HI_O_ACCMODE                            O_ACCMODE
#define HI_O_NONBLOCK                           O_NONBLOCK
#define HI_O_NDELAY                             HI_O_NONBLOCK
#define HI_O_SYNC                               O_SYNC
#define HI_O_FSYNC                              HI_O_SYNC
#define HI_O_ASYNC                              O_ASYNC
#define HI_O_BINARY                             0

/* File types.  */
#define HI_S_IFIFO                              (0010000)    /* FIFO.  */
#define HI_S_IFCHR                              (0020000)    /* Character device.  */
#define HI_S_IFDIR                              (0040000)    /* Directory.  */
#define HI_S_IFBLK                              (0060000)    /* Block device.  */
#define HI_S_IFREG                              (0100000)    /* Regular file.  */
#define HI_S_IFLNK                              (0120000)    /* Symbolic link.  */
#define HI_S_IFSOCK                             (0140000)    /* Socket.  */
#define HI_S_IFMT                               (0170000)    /* file type mask */
#define HI_S_ISTYPE(mode, mask)                 (((mode) & HI_S_IFMT) == (mask))
#define HI_S_ISDIR(mode)                        HI_S_ISTYPE((mode), HI_S_IFDIR)
#define HI_S_ISCHR(mode)                        HI_S_ISTYPE((mode), HI_S_IFCHR)
#define HI_S_ISREG(mode)                        HI_S_ISTYPE((mode), HI_S_IFREG)
#define HI_S_ISFIFO(mode)                       HI_S_ISTYPE((mode), HI_S_IFIFO)
#define HI_S_ISLNK(mode)                        HI_S_ISTYPE((mode), HI_S_IFLNK)
#define HI_S_ISSOCK(mode)                       HI_S_ISTYPE((mode), HI_S_IFSOCK)
#define HI_S_ISBLK(mode)                        HI_S_ISTYPE((mode), HI_S_IFBLK)

/* 标准输入/输出/错误文件描述符 */
#define HI_OS_NCCS                              32
#define HI_OS_STDIN_FINENO                      STDIN_FILENO
#define HI_OS_STDOUT_FINENO                     STDOUT_FILENO
#define HI_OS_STDERR_FINENO                     STDERR_FILENO
#define HI_OS_Fcntl                             fcntl
#define HI_OS_PC_VDISABLE                       _PC_VDISABLE
#define HI_OS_VINTR                             VINTR
#define HI_OS_VQUIT                             VQUIT
#define HI_OS_VERASE                            VERASE
#define HI_OS_VKILL                             VKILL
#define HI_OS_VEOF                              VEOF
#define HI_OS_VTIME                             VTIME
#define HI_OS_VMIN                              VMIN
#define HI_OS_VSWTC                             VSWTC
#define HI_OS_VSTART                            VSTART
#define HI_OS_VSTOP                             VSTOP
#define HI_OS_VSUSP                             VSUSP
#define HI_OS_VEOL                              VEOL
#define HI_OS_VREPRINT                          VREPRINT
#define HI_OS_VDISCARD                          VDISCARD
#define HI_OS_VWERASE                           VWERASE
#define HI_OS_VLNEXT                            VLNEXT
#define HI_OS_VEOL2                             VEOL2
#define HI_OS_ISIG                              ISIG
#define HI_OS_ICANON                            ICANON
#define HI_OS_XCASE                             XCASE
#define HI_OS_ECHO                              ECHO
#define HI_OS_ECHOE                             ECHOE
#define HI_OS_ECHOK                             ECHOK
#define HI_OS_ECHONL                            ECHONL
#define HI_OS_NOFLSH                            NOFLSH
#define HI_OS_TOSTOP                            TOSTOP
#define HI_OS_ECHOCTL                           ECHOCTL
#define HI_OS_ECHOPRT                           ECHOPRT
#define HI_OS_ECHOKE                            ECHOKE
#define HI_OS_FLUSHO                            FLUSHO
#define HI_OS_PENDIN                            PENDIN
#define HI_OS_IEXTEN                            IEXTEN
#define HI_OS_OPOST                             OPOST
#define HI_OS_ONLCR                             ONLCR
#define HI_OS_TCSANOW                           TCSANOW
#define HI_OS_TCSADRAIN                         TCSADRAIN
#define HI_OS_TCSAFLUSH                         TCSAFLUSH

/* 目录操作函数 */
typedef struct {
    hi_uint32   ui_dino;
    hi_int32    i_doff;
    hi_ushort16 us_dreclen;
    hi_uchar8   uc_dtype;
    hi_char8    c_dname[256];   /* We must not include limits.h! */
} hi_os_dirent_s;

typedef struct {
    hi_uint32 ui_inflag;      /* Input modes.  */
    hi_uint32 ui_outflag;     /* Output modes.  */
    hi_uint32 ui_ctrlflag;    /* control modes */
    hi_uint32 ui_localflag;   /* Loacl modes */
    hi_uchar8 uc_line;        /* line discipline */
    hi_uchar8 uc_ctrl[HI_OS_NCCS]; /* control chars */
    hi_uint32 ui_ispeed;     /* input speed */
    hi_uint32 ui_ospeed;     /* output speed */
}hi_os_termios_s;

/* Conversion state information.  */
typedef struct {
    hi_int32 i_count;
    union
    {
        hi_uint32 i_wch;
        hi_char8  c_acwchb[4];
    } unValue;            /* Value so far.  */
} hi_os_mbstate_s;

typedef struct {
    hi_int32 i_pos;
    hi_os_mbstate_s st_state;
} hi_os_fpos_s;
/*************************extern prototype**********************************/
//extern hi_int32 vfscanf(FILE *stream, const hi_char8 *format, HI_VA_LIST param); 
//extern hi_int32 vscanf(hi_char8 *format, HI_VA_LIST param);
//extern hi_int32 vsscanf(hi_char8 *s, const hi_char8 *format, HI_VA_LIST param); 
extern hi_int32 unlockpt (hi_int32 fd);
extern hi_int32 grantpt (hi_int32 fd);
extern hi_char8* strcasestr(const hi_char8* haystack,const hi_char8* needle);
extern hi_int32 prctl(hi_int32 option,hi_ulong32 arg2,hi_ulong32 arg3,hi_ulong32 arg4,hi_ulong32 arg5);
/***************************************************************************/
extern hi_int32 hi_os_access(const hi_char8 *pc_pathname, hi_int32 i_mode);
extern hi_int32 hi_os_chmod(hi_char8 *pc_path, hi_uint32 ui_mode);
extern hi_void hi_os_clearerr (HI_FILE_S *p_stream);
extern hi_int32 hi_os_closedir(hi_os_dir p_dir);
extern hi_int32 hi_os_dup2(hi_int32 i_oldfd, hi_int32 i_newfd);
extern  hi_int32 hi_os_fclose(HI_FILE_S *p_stream);
extern hi_uint32 hi_os_fcopy(hi_uchar8 *pc_srcfile, hi_char8 *pc_destfile);
extern hi_uint32 hi_os_fdelete(hi_char8 *pc_file);
extern hi_int32 hi_os_feof (const HI_FILE_S *p_stream);
extern hi_int32 hi_os_ferror (const HI_FILE_S *p_stream);
extern hi_int32  hi_os_fflush(HI_FILE_S *p_stream);
extern hi_int32 hi_os_fgetc (HI_FILE_S *p_stream);
extern hi_int32 hi_os_fgetpos (HI_FILE_S * p_stream, hi_os_fpos_s * pst_pos);
extern hi_char8 *hi_os_fgets (hi_char8 * pc_s, hi_int32 i_n, HI_FILE_S * p_stream);
extern hi_int32 hi_os_fileno (const HI_FILE_S *p_stream);
extern HI_FILE_S *hi_os_fopen (const hi_char8 * pc_filename,const hi_char8 *c_modes);
extern hi_int32 hi_os_fpathconf(hi_int32 i_filedes,hi_int32 i_name);
extern hi_int32 hi_os_fprintf (HI_FILE_S *p_stream, const hi_char8 *pc_format, ...);
extern hi_int32 hi_os_fputc (int i_c, HI_FILE_S *p_stream);
extern hi_int32 hi_os_fputs (const hi_char8 * pc_s, HI_FILE_S * p_stream);
extern hi_uint32 hi_os_fread (hi_void * p_tr, hi_uint32 ui_size,
                    hi_uint32 ui_n, HI_FILE_S * p_stream);
extern hi_int32 hi_os_fscanf (HI_FILE_S * p_stream,const hi_char8 * pc_format, ...);
extern hi_int32 hi_os_fseek (HI_FILE_S *p_stream, hi_int32 i_off, hi_int32 i_whence);
extern hi_int32 hi_os_fsetpos (HI_FILE_S *p_stream, const hi_os_fpos_s *pst_pos);
extern hi_int32 hi_os_fsize(const hi_char8 * pc_filename,hi_uint32 *pui_filesize);
extern hi_int32 hi_os_ftell (HI_FILE_S *p_stream);
extern hi_int32 hi_os_ftruncate(hi_int32 i_fd, hi_int32 i_length);
extern hi_uint32 hi_os_fwrite (const hi_void * p_tr, hi_uint32 ui_size,
                     hi_uint32 ui_n, HI_FILE_S * p_s);
extern hi_int32 hi_os_grantpt(hi_int32 i_fd);
extern hi_int32 hi_os_mkdir( const hi_char8 *c_dirname ,hi_uint32 mode);
extern hi_os_dir hi_os_opendir(const hi_char8 *pc_name);
extern hi_int32  hi_os_printf (const hi_char8 *pc_format, ...);
extern hi_os_dirent_s * hi_os_readdir(hi_os_dir p_dir);
extern hi_int32 hi_os_remove(const hi_char8*pc_filename);
extern hi_void hi_os_rewind (HI_FILE_S *p_stream);
extern hi_int32 hi_os_scandir(const hi_char8 *p_dir,
                    hi_os_dirent_s ***pst_namelist,
                    hi_int32(*select)(const hi_os_dirent_s *),
                    hi_int32(*cmp)(const struct dirent **, const struct dirent **));
extern hi_int32 hi_os_setsid(hi_void);
extern hi_int32 hi_os_snprintf(hi_char8 *pc_str, hi_uint32 ui_size, const hi_char8 *pc_format, ...);
extern hi_int32 hi_os_sprintf(hi_char8 *pc_str, const hi_char8 *pc_format, ...);
extern hi_int32 hi_os_sscanf( hi_char8 *pc_str, const hi_char8 *pc_format, ...);
extern hi_int32 hi_os_scanf( hi_char8 * pc_format, ...);
extern hi_int32 hi_os_tcgetattr(hi_int32 i_fd, hi_os_termios_s* pst_termios);
extern hi_int32 hi_os_tcsetattr(hi_int32 i_fd,hi_int32 i_optional,hi_os_termios_s* pst_termios);
extern HI_FILE_S *hi_os_tmpfile (hi_void);
extern hi_int32 hi_os_unlink(const hi_char8 * pc_pathnames);
extern hi_int32 hi_os_unlockpt(hi_int32 i_fd);
extern hi_int32 hi_os_vfprintf(HI_FILE_S *p_f, const hi_char8 *pc_format, HI_VA_LIST arglist);
extern hi_int32 hi_os_vsnprintf(hi_char8 *pc_str, hi_uint32 ui_size, const hi_char8 *pc_format, HI_VA_LIST arglist);
extern hi_int32 hi_os_vsprintf(hi_char8 *pc_str, const hi_char8 *pc_format, HI_VA_LIST argList);
extern hi_char8 *hi_os_getcwd( hi_char8 *pc_buf, hi_int32 ui_size );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_OS_FILEIO_H__ */
