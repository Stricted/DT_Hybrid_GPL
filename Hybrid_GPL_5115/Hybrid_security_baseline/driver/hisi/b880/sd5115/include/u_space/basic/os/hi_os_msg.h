/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_os_msg.h
  版 本 号   : 初稿
  作    者   : l00183967
  生成日期   : D2011_08_02
  最近修改   :

******************************************************************************/

#ifndef __HI_OS_MSG_H__
#define __HI_OS_MSG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef  struct 
{
    hi_int32 mtype;     /* message type, must be > 0 */
    hi_char8 mtext[4];  /* message data */
} hi_os_msgbuf_s;

/* Structure of record for one message inside the kernel.
   The type `struct msg' is opaque.  */
typedef struct {
    hi_os_ipcperm_s msg_perm;    /* structure describing operation permission */
    hi_uint32 msg_stime;        /* time of last msgsnd command */
    hi_uint32 unused1;
    hi_uint32 msg_rtime;        /* time of last msgrcv command */
    hi_uint32 unused2;
    hi_uint32 msg_ctime;        /* time of last change */
    hi_uint32 unused3;
    hi_uint32 msg_cbytes; /* current number of bytes on queue */
    hi_uint32 msg_qnum;        /* number of messages currently on queue */
    hi_uint32 msg_qbytes;        /* max number of bytes allowed on queue */
    hi_int32 msg_lspid;        /* pid of last msgsnd() */
    hi_int32 msg_lrpid;        /* pid of last msgrcv() */
    hi_uint32 unused4;
    hi_uint32 unused5;
}hi_os_msgidds_s;


extern hi_int32 hi_os_msgctl (hi_int32 i_msqid, hi_int32 i_cmd, hi_os_msgidds_s *pst_buf);
extern hi_int32 hi_os_msgget(hi_int32 key, hi_int32 i_msgflg);
extern hi_int32 hi_os_msgsnd(hi_int32 i_msqid, hi_os_msgbuf_s*pst_msg, hi_uint32 ui_msg, hi_int32 i_msgflg);
extern hi_int32 hi_os_msgrcv(hi_int32 i_msqid, hi_os_msgbuf_s *pst_msg, hi_uint32 ui_msg, hi_int32 i_msgtyp, hi_int32 i_msgflg);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __HI_OS_MSG_H__ */

