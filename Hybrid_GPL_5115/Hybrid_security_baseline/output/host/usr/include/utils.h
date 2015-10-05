/* <DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 BEGIN */
/***********************************************************************
  版权信息: 版权所有(C) 1988-2008, 华为技术有限公司.
  文件名: utils.h
  作者: mayang
  版本: 001
  创建日期: 2008-5-24
  完成日期: 
  功能描述: 
      
  主要函数列表: 
      
  修改历史记录列表: 
    <作  者>    <修改时间>  <版本>  <修改描述>
    mayang       2008-5-17                   001             init
    gKF34687    2010-11-4                    002             b683 移植
  备注: 
************************************************************************/
/*========================================================================================
Edit History of Huawei: 
<修改人>     	 	<时间>       	 <问题单号>            		 <简要描述>
z00185914           2013/01/28          DTS2013012808057		【拨号】新建ndis拨号文件夹 
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
/* DTS2013012808057 新建ndis拨号文件夹  z00185914 20130128 END> */
