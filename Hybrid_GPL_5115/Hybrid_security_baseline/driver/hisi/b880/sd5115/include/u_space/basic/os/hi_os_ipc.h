/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_ipc.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/
#ifndef __HI_OS_IPC_H__
#define __HI_OS_IPC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_IPC_CREAT    (00001000)        /* Create key if key does not exist. */
#define HI_IPC_EXCL     (00002000)        /* Fail if key exists.  */
#define HI_IPC_NOWAIT   (00004000)        /* Return error on wait.  */
#define HI_IPC_WRALL    (0666)
#define HI_IPC_RMID     (IPC_RMID)

#define HI_IPC_PRIVATE  (0)

#define HI_S_IREAD      (0400)    /* Read by owner.  */
#define HI_S_IWRITE     (0200)    /* Write by owner.  */
#define HI_S_IEXEC      (0100)    /* Execute by owner.  */
#define HI_S_IRWXU      (HI_S_IREAD|HI_S_IWRITE|HI_S_IEXEC)

#define HI_S_IRGRP      (040)    /* Read by group.  */
#define HI_S_IWGRP      (020)    /* Write by group.  */
#define HI_S_IXGRP      (010)    /* Execute by group.  */
#define HI_S_IRWXG      (HI_S_IRGRP|HI_S_IWGRP|HI_S_IXGRP)

#define HI_S_IROTH      (04)    /* Read by other.  */
#define HI_S_IWOTH      (02)    /* Write by other.  */
#define HI_S_IXOTH      (01)    /* Execute by other.  */
#define HI_S_IRWXO      (HI_S_IROTH|HI_S_IWOTH|HI_S_IXOTH)

#define HI_S_IRWXALL    (HI_S_IRWXU|HI_S_IRWXG|HI_S_IRWXO)

#define HI_MSG_EXCEPT   (020000)  /* recv any msg except of specified type */
#define HI_MSG_NOERROR  (010000)  /* no error if message is too big */


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_IPC_H__ */

