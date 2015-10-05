/* <DTS2013012808057 �½�ndis�����ļ���  z00185914 20130128 BEGIN */
/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2008, ��Ϊ�������޹�˾.
  �ļ���: utils.h
  ����: mayang
  �汾: 001
  ��������: 2008-5-24
  �������: 
  ��������: 
      
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    mayang       2008-5-17                   001             init
    gKF34687    2010-11-4                    002             b683 ��ֲ
  ��ע: 
************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<�޸���>     	 	<ʱ��>       	 <���ⵥ��>            		 <��Ҫ����>
z00185914           2013/01/28          DTS2013012808057		�����š��½�ndis�����ļ��� 
========================================================================================*/


#ifndef _WDS_UTILS_H_
#define _WDS_UTILS_H_

#define DEBUG_WDS
/*#define DEBUG_UT_WDS*/

void dbglog (char *, ...);	/* log a debug message */
void info (char *, ...);	/* log an informational message */
void notice (char *, ...);	/* log a notice-level message */
void warn (char *, ...);	/* log a warning message */
void error (char *, ...);	/* log an error message */
void fatal (char *, ...);	/* log an error message and die(1) */

/* log hex msg, only for debug */
void print_hex_msg(const unsigned char* msg, int nlen);

#ifdef DEBUG_WDS
#define WDS_DEBUG(fmt, args...)	\
    dbglog("[  dbg][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)
#else
#define WDS_DEBUG(fmt, args...)
#endif

#ifdef DEBUG_WDS
#define ASSERT(expr) \
    if(!(expr)) \
    { \
        dbglog("Assertion failed! %s,%s,%s,line=%d\n", #expr,__FILE__,__FUNCTION__,__LINE__); \
    }
#else
#define ASSERT(expr) do {} while (0)
#endif

#ifdef DEBUG_WDS
#define PRINT_MSG(msg, nlen) \
    print_hex_msg(msg, nlen)
#else
#define PRINT_MSG(msg, nlen) do {} while(0)
#endif

#define INFO(fmt, args...) dbglog("[  dbg][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)
    //info("[ info][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)

#define WARN(fmt, args...) dbglog("[  dbg][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)
    //warn("[ warn][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)

#define FATAL(fmt, args...)	dbglog("[  dbg][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)
    //fatal("[fatal][%16s:%4d:pid=%3d] " fmt " errno = %d <%s> \n", __FILE__, __LINE__, getpid(), ## args, errno, strerror(errno))

#define ERROR(fmt, args...)	dbglog("[  dbg][%16s:%4d:pid=%3d] " fmt "\n", __FILE__, __LINE__, getpid(), ## args)
    //error("[error][%16s:%4d:pid=%3d] " fmt " errno = %d <%s> \n", __FILE__, __LINE__, getpid(), ## args, errno, strerror(errno))


#endif /* _WDS_UTILS_H_*/
/* DTS2013012808057 �½�ndis�����ļ���  z00185914 20130128 END> */
