#ifndef __WEBASP_API_H__
#define __WEBASP_API_H__

#define WEB_ASP_MAX_NODE_COUNT  50   //ASP中最大节点个数
#define WEB_ASPCMD_PARANUM 16       //ASP最大参数个数

/*****************************************************************************/
/*  枚举类型定义                                                             */
/*****************************************************************************/

/*获取ASP命令的错误码*/
typedef enum tagWEB_GETASPERR_EN
{
    ATP_WEB_ASPPROC_FAILED = 1,
    ATP_WEB_ASPEXTEND_NOTFIND, 
    ATP_WEB_ASPEXTEND_FAILED,
    ATP_WEB_GETASP_FAILED,
}WEB_GETASPERR_EN;

VOS_INT32 webMacheAspProc(ATP_HTTP_CLIENT_ST *pstHttpClient, VOS_INT8 *pstFile);
VOS_INT32 webReadAspFile(ATP_HTTP_CLIENT_ST *pstHttpClient, FILE *pfile);

#endif

