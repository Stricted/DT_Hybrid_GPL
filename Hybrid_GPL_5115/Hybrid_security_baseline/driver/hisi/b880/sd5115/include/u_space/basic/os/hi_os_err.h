/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_err.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_ERR_H__
#define __HI_OS_ERR_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    HI_ERR_OS_EPERM              = (HI_SRCMODULE_UBASIC_UOS + 1  ),    /* Operation not permitted */
    HI_ERR_OS_ENOENT             = (HI_SRCMODULE_UBASIC_UOS + 2  ),    /* No such file or directory */
    HI_ERR_OS_ESRCH              = (HI_SRCMODULE_UBASIC_UOS + 3  ),    /* No such process */
    HI_ERR_OS_EINTR              = (HI_SRCMODULE_UBASIC_UOS + 4  ),    /* Interrupted system call */
    HI_ERR_OS_EIO                = (HI_SRCMODULE_UBASIC_UOS + 5  ),    /* I/O error */
    HI_ERR_OS_ENXIO              = (HI_SRCMODULE_UBASIC_UOS + 6  ),    /* No such device or address */
    HI_ERR_OS_E2BIG              = (HI_SRCMODULE_UBASIC_UOS + 7  ),    /* Arg list too long */
    HI_ERR_OS_ENOEXEC            = (HI_SRCMODULE_UBASIC_UOS + 8  ),    /* Exec format error */
    HI_ERR_OS_EBADF              = (HI_SRCMODULE_UBASIC_UOS + 9  ),    /* Bad file number */
    HI_ERR_OS_ECHILD             = (HI_SRCMODULE_UBASIC_UOS + 10 ),   /* No child processes */
    HI_ERR_OS_EAGAIN             = (HI_SRCMODULE_UBASIC_UOS + 11 ),   /* Try again */
    HI_ERR_OS_ENOMEM             = (HI_SRCMODULE_UBASIC_UOS + 12 ),   /* Out of memory */
    HI_ERR_OS_EACCES             = (HI_SRCMODULE_UBASIC_UOS + 13 ),   /* Permission denied */
    HI_ERR_OS_EFAULT             = (HI_SRCMODULE_UBASIC_UOS + 14 ),   /* Bad address */
    HI_ERR_OS_ENOTBLK            = (HI_SRCMODULE_UBASIC_UOS + 15 ),   /* Block device required */
    HI_ERR_OS_EBUSY              = (HI_SRCMODULE_UBASIC_UOS + 16 ),   /* Device or resource busy */
    HI_ERR_OS_EEXIST             = (HI_SRCMODULE_UBASIC_UOS + 17 ),   /* File exists */
    HI_ERR_OS_EXDEV              = (HI_SRCMODULE_UBASIC_UOS + 18 ),   /* Cross-device link */
    HI_ERR_OS_ENODEV             = (HI_SRCMODULE_UBASIC_UOS + 19 ),   /* No such device */
    HI_ERR_OS_ENOTDIR            = (HI_SRCMODULE_UBASIC_UOS + 20 ),   /* Not a directory */
    HI_ERR_OS_EISDIR             = (HI_SRCMODULE_UBASIC_UOS + 21 ),   /* Is a directory */
    HI_ERR_OS_EINVAL             = (HI_SRCMODULE_UBASIC_UOS + 22 ),   /* Invalid argument */
    HI_ERR_OS_ENFILE             = (HI_SRCMODULE_UBASIC_UOS + 23 ),   /* File table overflow */
    HI_ERR_OS_EMFILE             = (HI_SRCMODULE_UBASIC_UOS + 24 ),   /* Too many open files */
    HI_ERR_OS_ENOTTY             = (HI_SRCMODULE_UBASIC_UOS + 25 ),   /* Not a typewriter */
    HI_ERR_OS_ETXTBSY            = (HI_SRCMODULE_UBASIC_UOS + 26 ),   /* Text file busy */
    HI_ERR_OS_EFBIG              = (HI_SRCMODULE_UBASIC_UOS + 27 ),   /* File too large */
    HI_ERR_OS_ENOSPC             = (HI_SRCMODULE_UBASIC_UOS + 28 ),   /* No space left on device */
    HI_ERR_OS_ESPIPE             = (HI_SRCMODULE_UBASIC_UOS + 29 ),   /* Illegal seek */
    HI_ERR_OS_EROFS              = (HI_SRCMODULE_UBASIC_UOS + 30 ),   /* Read-only file system */
    HI_ERR_OS_EMLINK             = (HI_SRCMODULE_UBASIC_UOS + 31 ),   /* Too many links */
    HI_ERR_OS_EPIPE              = (HI_SRCMODULE_UBASIC_UOS + 32 ),   /* Broken pipe */
    HI_ERR_OS_EDOM               = (HI_SRCMODULE_UBASIC_UOS + 33 ),   /* Math argument out of domain of func */
    HI_ERR_OS_ERANGE             = (HI_SRCMODULE_UBASIC_UOS + 34 ),   /* Math result not representable */
    HI_ERR_OS_EDEADLK            = (HI_SRCMODULE_UBASIC_UOS + 35 ),   /* Resource deadlock would occur */
    HI_ERR_OS_ENAMETOOLONG       = (HI_SRCMODULE_UBASIC_UOS + 36 ),   /* File name too long */
    HI_ERR_OS_ENOLCK             = (HI_SRCMODULE_UBASIC_UOS + 37 ),   /* No record locks available */
    HI_ERR_OS_ENOSYS             = (HI_SRCMODULE_UBASIC_UOS + 38 ),   /* Function not implemented */
    HI_ERR_OS_ENOTEMPTY          = (HI_SRCMODULE_UBASIC_UOS + 39 ),   /* Directory not empty */
    HI_ERR_OS_ELOOP              = (HI_SRCMODULE_UBASIC_UOS + 40 ),   /* Too many symbolic links encountered */
    HI_ERR_OS_ENOMSG             = (HI_SRCMODULE_UBASIC_UOS + 42 ),   /* No message of desired type */
    HI_ERR_OS_EIDRM              = (HI_SRCMODULE_UBASIC_UOS + 43 ),   /* Identifier removed */
    HI_ERR_OS_ECHRNG             = (HI_SRCMODULE_UBASIC_UOS + 44 ),   /* Channel number out of range */
    HI_ERR_OS_EL2NSYNC           = (HI_SRCMODULE_UBASIC_UOS + 45 ),   /* Level 2 not synchronized */
    HI_ERR_OS_EL3HLT             = (HI_SRCMODULE_UBASIC_UOS + 46 ),   /* Level 3 halted */
    HI_ERR_OS_EL3RST             = (HI_SRCMODULE_UBASIC_UOS + 47 ),   /* Level 3 reset */
    HI_ERR_OS_ELNRNG             = (HI_SRCMODULE_UBASIC_UOS + 48 ),   /* Link number out of range */
    HI_ERR_OS_EUNATCH            = (HI_SRCMODULE_UBASIC_UOS + 49 ),   /* Protocol driver not attached */
    HI_ERR_OS_ENOCSI             = (HI_SRCMODULE_UBASIC_UOS + 50 ),   /* No CSI structure available */
    HI_ERR_OS_EL2HLT             = (HI_SRCMODULE_UBASIC_UOS + 51 ),   /* Level 2 halted */
    HI_ERR_OS_EBADE              = (HI_SRCMODULE_UBASIC_UOS + 52 ),   /* Invalid exchange */
    HI_ERR_OS_EBADR              = (HI_SRCMODULE_UBASIC_UOS + 53 ),   /* Invalid request descriptor */
    HI_ERR_OS_EXFULL             = (HI_SRCMODULE_UBASIC_UOS + 54 ),   /* Exchange full */
    HI_ERR_OS_ENOANO             = (HI_SRCMODULE_UBASIC_UOS + 55 ),   /* No anode */
    HI_ERR_OS_EBADRQC            = (HI_SRCMODULE_UBASIC_UOS + 56 ),   /* Invalid request code */
    HI_ERR_OS_EBADSLT            = (HI_SRCMODULE_UBASIC_UOS + 57 ),   /* Invalid slot */
    HI_ERR_OS_EBFONT             = (HI_SRCMODULE_UBASIC_UOS + 59 ),   /* Bad font file format */
    HI_ERR_OS_ENOSTR             = (HI_SRCMODULE_UBASIC_UOS + 60 ),   /* Device not a stream */
    HI_ERR_OS_ENODATA            = (HI_SRCMODULE_UBASIC_UOS + 61 ),   /* No data available */
    HI_ERR_OS_ETIME              = (HI_SRCMODULE_UBASIC_UOS + 62 ),   /* Timer expired */
    HI_ERR_OS_ENOSR              = (HI_SRCMODULE_UBASIC_UOS + 63 ),   /* Out of streams resources */
    HI_ERR_OS_ENONET             = (HI_SRCMODULE_UBASIC_UOS + 64 ),   /* Machine is not on the network */
    HI_ERR_OS_ENOPKG             = (HI_SRCMODULE_UBASIC_UOS + 65 ),   /* Package not installed */
    HI_ERR_OS_EREMOTE            = (HI_SRCMODULE_UBASIC_UOS + 66 ),   /* Object is remote */
    HI_ERR_OS_ENOLINK            = (HI_SRCMODULE_UBASIC_UOS + 67 ),   /* Link has been severed */
    HI_ERR_OS_EADV               = (HI_SRCMODULE_UBASIC_UOS + 68 ),   /* Advertise error */
    HI_ERR_OS_ESRMNT             = (HI_SRCMODULE_UBASIC_UOS + 69 ),   /* Srmount error */
    HI_ERR_OS_ECOMM              = (HI_SRCMODULE_UBASIC_UOS + 70 ),   /* Communication error on send */
    HI_ERR_OS_EPROTO             = (HI_SRCMODULE_UBASIC_UOS + 71 ),   /* Protocol error */
    HI_ERR_OS_EMULTIHOP          = (HI_SRCMODULE_UBASIC_UOS + 72 ),   /* Multihop attempted */
    HI_ERR_OS_EDOTDOT            = (HI_SRCMODULE_UBASIC_UOS + 73 ),   /* RFS specific error */
    HI_ERR_OS_EBADMSG            = (HI_SRCMODULE_UBASIC_UOS + 74 ),   /* Not a data message */
    HI_ERR_OS_EOVERFLOW          = (HI_SRCMODULE_UBASIC_UOS + 75 ),   /* Value too large for defined data type */
    HI_ERR_OS_ENOTUNIQ           = (HI_SRCMODULE_UBASIC_UOS + 76 ),   /* Name not unique on network */
    HI_ERR_OS_EBADFD             = (HI_SRCMODULE_UBASIC_UOS + 77 ),   /* File descriptor in bad state */
    HI_ERR_OS_EREMCHG            = (HI_SRCMODULE_UBASIC_UOS + 78 ),   /* Remote address changed */
    HI_ERR_OS_ELIBACC            = (HI_SRCMODULE_UBASIC_UOS + 79 ),   /* Can not access a needed shared library */
    HI_ERR_OS_ELIBBAD            = (HI_SRCMODULE_UBASIC_UOS + 80 ),   /* Accessing a corrupted shared library */
    HI_ERR_OS_ELIBSCN            = (HI_SRCMODULE_UBASIC_UOS + 81 ),   /* .lib section in a.out corrupted */
    HI_ERR_OS_ELIBMAX            = (HI_SRCMODULE_UBASIC_UOS + 82 ),   /* Attempting to link in too many shared libraries */
    HI_ERR_OS_ELIBEXEC           = (HI_SRCMODULE_UBASIC_UOS + 83 ),   /* Cannot exec a shared library directly */
    HI_ERR_OS_EILSEQ             = (HI_SRCMODULE_UBASIC_UOS + 84 ),   /* Illegal byte sequence */
    HI_ERR_OS_ERESTART           = (HI_SRCMODULE_UBASIC_UOS + 85 ),   /* Interrupted system call should be restarted */
    HI_ERR_OS_ESTRPIPE           = (HI_SRCMODULE_UBASIC_UOS + 86 ),   /* Streams pipe error */
    HI_ERR_OS_EUSERS             = (HI_SRCMODULE_UBASIC_UOS + 87 ),   /* Too many users */
    HI_ERR_OS_ENOTSOCK           = (HI_SRCMODULE_UBASIC_UOS + 88 ),   /* Socket operation on non-socket */
    HI_ERR_OS_EDESTADDRREQ       = (HI_SRCMODULE_UBASIC_UOS + 89 ),   /* Destination address required */
    HI_ERR_OS_EMSGSIZE           = (HI_SRCMODULE_UBASIC_UOS + 90 ),   /* Message too long */
    HI_ERR_OS_EPROTOTYPE         = (HI_SRCMODULE_UBASIC_UOS + 91 ),   /* Protocol wrong type for socket */
    HI_ERR_OS_ENOPROTOOPT        = (HI_SRCMODULE_UBASIC_UOS + 92 ),   /* Protocol not available */
    HI_ERR_OS_EPROTONOSUPPORT    = (HI_SRCMODULE_UBASIC_UOS + 93 ),   /* Protocol not supported */
    HI_ERR_OS_ESOCKTNOSUPPORT    = (HI_SRCMODULE_UBASIC_UOS + 94 ),   /* Socket type not supported */
    HI_ERR_OS_EOPNOTSUPP         = (HI_SRCMODULE_UBASIC_UOS + 95 ),   /* Operation not supported on transport endpoint */
    HI_ERR_OS_EPFNOSUPPORT       = (HI_SRCMODULE_UBASIC_UOS + 96 ),   /* Protocol family not supported */
    HI_ERR_OS_EAFNOSUPPORT       = (HI_SRCMODULE_UBASIC_UOS + 97 ),   /* Address family not supported by protocol */
    HI_ERR_OS_EADDRINUSE         = (HI_SRCMODULE_UBASIC_UOS + 98 ),   /* Address already in use */
    HI_ERR_OS_EADDRNOTAVAIL      = (HI_SRCMODULE_UBASIC_UOS + 99 ),   /* Cannot assign requested address */
    HI_ERR_OS_ENETDOWN           = (HI_SRCMODULE_UBASIC_UOS + 100),  /* Network is down */
    HI_ERR_OS_ENETUNREACH        = (HI_SRCMODULE_UBASIC_UOS + 101),  /* Network is unreachable */
    HI_ERR_OS_ENETRESET          = (HI_SRCMODULE_UBASIC_UOS + 102),  /* Network dropped connection because of reset */
    HI_ERR_OS_ECONNABORTED       = (HI_SRCMODULE_UBASIC_UOS + 103),  /* Software caused connection abort */
    HI_ERR_OS_ECONNRESET         = (HI_SRCMODULE_UBASIC_UOS + 104),  /* Connection reset by peer */
    HI_ERR_OS_ENOBUFS            = (HI_SRCMODULE_UBASIC_UOS + 105),  /* No buffer space available */
    HI_ERR_OS_EISCONN            = (HI_SRCMODULE_UBASIC_UOS + 106),  /* Transport endpoint is already connected */
    HI_ERR_OS_ENOTCONN           = (HI_SRCMODULE_UBASIC_UOS + 107),  /* Transport endpoint is not connected */
    HI_ERR_OS_ESHUTDOWN          = (HI_SRCMODULE_UBASIC_UOS + 108),  /* Cannot send after transport endpoint shutdown */
    HI_ERR_OS_ETOOMANYREFS       = (HI_SRCMODULE_UBASIC_UOS + 109),  /* Too many references: cannot splice */
    HI_ERR_OS_ETIMEDOUT          = (HI_SRCMODULE_UBASIC_UOS + 110),  /* Connection timed out */
    HI_ERR_OS_ECONNREFUSED       = (HI_SRCMODULE_UBASIC_UOS + 111),  /* Connection refused */
    HI_ERR_OS_EHOSTDOWN          = (HI_SRCMODULE_UBASIC_UOS + 112),  /* Host is down */
    HI_ERR_OS_EHOSTUNREACH       = (HI_SRCMODULE_UBASIC_UOS + 113),  /* No route to host */
    HI_ERR_OS_EALREADY           = (HI_SRCMODULE_UBASIC_UOS + 114),  /* Operation already in progress */
    HI_ERR_OS_EINPROGRESS        = (HI_SRCMODULE_UBASIC_UOS + 115),  /* Operation now in progress */
    HI_ERR_OS_ESTALE             = (HI_SRCMODULE_UBASIC_UOS + 116),  /* Stale NFS file handle */
    HI_ERR_OS_EUCLEAN            = (HI_SRCMODULE_UBASIC_UOS + 117),  /* Structure needs cleaning */
    HI_ERR_OS_ENOTNAM            = (HI_SRCMODULE_UBASIC_UOS + 118),  /* Not a XENIX named type file */
    HI_ERR_OS_ENAVAIL            = (HI_SRCMODULE_UBASIC_UOS + 119),  /* No XENIX semaphores available */
    HI_ERR_OS_EISNAM             = (HI_SRCMODULE_UBASIC_UOS + 120),  /* Is a named type file */
    HI_ERR_OS_EREMOTEIO          = (HI_SRCMODULE_UBASIC_UOS + 121),  /* Remote I/O error */
    HI_ERR_OS_EDQUOT             = (HI_SRCMODULE_UBASIC_UOS + 122),  /* Quota exceeded */
    HI_ERR_OS_ENOMEDIUM          = (HI_SRCMODULE_UBASIC_UOS + 123),  /* No medium found */
    HI_ERR_OS_EMEDIUMTYPE        = (HI_SRCMODULE_UBASIC_UOS + 124),  /* Wrong medium type */
    HI_ERR_OS_CUSTOMBASEERR      = (HI_SRCMODULE_UBASIC_UOS + 500),
    HI_ERR_OS_NOT_IMPLEMENT      = (HI_SRCMODULE_UBASIC_UOS + 501),  /* Operation not implement */
    HI_ERR_OS_ARRAY_FULL         = (HI_SRCMODULE_UBASIC_UOS + 502),  /* Array is full */
}hi_os_errcode_e;

     
/*****************************************************************************
 函 数 名  : hi_os_errno
 功能描述  : 函数获取系统全局变量errno的值
*****************************************************************************/          
hi_uint32 hi_os_errno(hi_void);

/*****************************************************************************
 函 数 名  : hi_os_errset
 功能描述  : 设置全局变量errno的值
*****************************************************************************/          
hi_void hi_os_errset(hi_uint32 ui_new_err_no);

/*****************************************************************************
 函 数 名  : hi_os_errset
 功能描述  : 获取全局变量errno的值对应的字符串说明信息
*****************************************************************************/          
hi_char8 *hi_os_strerror(hi_uint32 ui_err_no);


#ifdef __cplusplus
#if __cplusplus
}

#endif /* __cpluscplus */          
#endif /* __cpluscplus */          

#endif /* __HI_OS_ERR_H__ */          



