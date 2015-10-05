#ifndef __UPG_API_H__
#define __UPG_API_H__

#include "upgtypes.h"
#include "upgpackapi.h"
#include "upginiapi.h"

#define ATP_UPG_NEED_REBOOT_BIT_D       (0x40)

/* error num for trans tools */
#define ATP_TRANS_OK                (0)
#define ATP_TRANS_TIMEOUT           (0xF1)
#define ATP_TRANS_FILE_ERR          (0xF2)
#define ATP_TRANS_SYS_ERR           (0xF3)
#define ATP_TRANS_AUTH_ERR          (0xF4)

/* 文件传输方向 */
typedef enum tagATP_UPG_TRANS_DIR_EN
{
    ATP_TRANS_DIR_UPLOAD,
    ATP_TRANS_DIR_DOWNLOAD,
    ATP_TRANS_DIR_BOTH,
    ATP_TRANS_DIR_CHECK
} ATP_UPG_TRANS_DIR_EN;

/* 文件传输方向 */
typedef enum tagATP_UPG_STATE_EN
{
    ATP_UPG_BEFORE_TRANS,
    ATP_UPG_AFTER_TRANS,
    ATP_UPG_ERR_TRANS
} ATP_UPG_STATE_EN;

typedef struct tagATP_UPG_CTX_ST
{
    ATP_UPG_FILE_ST         *pstPkgList;
    ATP_UPG_INI_SECTION_ST  *pstIniSection;
    unsigned char           *pcImageBuff;
    unsigned int             ulLen;
    const char              *pcCfgTool;
    const char              *pcVersion;   
}ATP_UPG_CTX_ST;

/* 子文件处理函数 */
typedef unsigned int (* AtpSubFileUpgProc)(const ATP_UPG_CTX_ST* pstUpgCtx);

typedef unsigned int (* AtpUpgFlashErrorHook)(
                                    const ATP_UPG_CTX_ST* pstUpgCtx,
                                    unsigned int ulRet);
extern AtpUpgFlashErrorHook g_pfUpgFlashErrorHook;

/* 传输项处理函数 */
typedef unsigned int (* AtpTransItemProc)(
                                ATP_UPG_STATE_EN    enTranState,
                                const char      *pcCfgTool,
                                const char      *pcFilePath,
                                const char            *apcArg[],
                                unsigned int          ulCurArgLen);

/* 升级预处理函数 */
typedef unsigned int (* AtpUpgPreProc)(
                                 char      *pcCfgTool,
                                ATP_UPG_FILE_ST     *pstPkgList);

/* 传输项结构体 */
typedef struct tagATP_UPG_TRANS_ITEM_ST
{
     char                  *pcItemName;
     char                  *pcFilePath;
    AtpTransItemProc                pfProc;
    unsigned int                      ulDir;      // ATP_UPG_TRANS_DIR_EN
    unsigned int                      ulMaxSize;

    void                        *pvSubProcs;    // Sub proc list

    struct tagATP_UPG_TRANS_ITEM_ST *pstNext;
} ATP_UPG_TRANS_ITEM_ST;

/* 传输工具结构体 */
typedef struct tagATP_UPG_TRANS_TOOL_ST
{
     char                  *pcExtCliName;
     char                  *pcProtocol;
    unsigned short                      usTransDir;
    unsigned short                      usDftPort;
    struct tagATP_UPG_TRANS_TOOL_ST *pstNext;
} ATP_UPG_TRANS_TOOL_ST;

#define UPG_MAX_PATH_LEN        (256)

/******************************************************************************
  函数名称  : ATP_UPG_ParseURL
  功能描述  : 解析URL地址
  输入参数  :
              1. pcUrl      :   输入的URL字符串
              2. pcProtocol:  存放输出的协议字符串，可以为空；不为空，则长度为UPG_MAX_PATH_LEN
              3. pcAdrr     :   存放输出主机地址的缓冲区，长度UPG_MAX_PATH_LEN
              4. pcRemote   :   存放远程路径的缓冲区，长度UPG_MAX_PATH_LEN
              5. usPort     :   端口，默认值为0
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              VOS_TRUE  :   解析成功
              VOS_FALSE :   解析失败

  修改历史      :
   1.日    期   : 2008-5-20
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_ParseURL(  const char                *pcUrl,
                                char                *pcProtocol,
                                char                *pcAdrr,
                                char                *pcRemote,
                                unsigned short      *pusPort);

/******************************************************************************
  函数名称  : ATP_UPG_RegTransTool
  功能描述  : 注册传输工具
  输入参数  :
              1. pcExeName  :   传输工具名称
              2. pcProName  :   传输工具所支持的协议名称
              3. eTransDir  :   传输工具支持的传输方向
              4. usDftPort  :   传输工具的默认端口
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-19
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_RegTransTool(
                        const char              *pcExeName,
                        const char              *pcProName,
                        ATP_UPG_TRANS_DIR_EN    eTransDir,
                        unsigned short          usDftPort);

/******************************************************************************
  函数名称  : ATP_UPG_RegTransItem
  功能描述  : 注册传输项
  输入参数  :
              1. pcItemName :   传输项名称
              2. pcFileName :   传输项的本地文件名称
              3. eDir       :   传输项的方向
              4. pfUpgProc  :   传输项的处理函数
              5. ulMaxSize  :   传输项的最大长度，0表示没有限制
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-19
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_RegTransItem(
                        const char              *pcItemName,
                        const char              *pcFileName,
                        ATP_UPG_TRANS_DIR_EN    eDir,
                        AtpTransItemProc        pfUpgProc,
                        unsigned int            ulMaxSize);

/******************************************************************************
  函数名称  : ATP_UPG_SetDftUpgProc
  功能描述  : 设置默认升级处理函数
  输入参数  : 
              1.  ulFileType    :   默认的文件类型
  调用函数  : 
  被调函数  : 
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-10-14
     作    者   : z65940
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_SetDftUpgProc(unsigned int  ulFileType);

/******************************************************************************
  函数名称  : ATP_UPG_RegSubFile
  功能描述  : 注册子文件
  输入参数  :
              1. ulFileType :   子文件类型
              2. pcFileName :   子文件的下载路径
              3. bDftProc   :   该子文件是否为默认的升级文件
                                当下载的文件不是ATP打包的文件格式时，需要使用默认
                                升级函数进行升级，此处指明是否认为是该子文件
                                VOS_TRUE:是；VOS_FALSE:否
              4. ulMaxSize  :   子文件的最大大小
              5. pfUpgProc  :   子文件的处理函数
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-20
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_RegSubFile(
                        unsigned int            ulFileType,
                        unsigned int            bDftProc,
                        AtpSubFileUpgProc       pfUpgProc);

/******************************************************************************
  函数名称  : ATP_UPG_GetProcByType
  功能描述  : 根据子文件类型获取其处理函数
  输入参数  :
              1. ulSubType  :   子文件类型
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              NULL      : 没有找到子文件处理函数
              Not NULL  : 子文件处理函数

  修改历史      :
   1.日    期   : 2008-6-25
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
AtpSubFileUpgProc ATP_UPG_GetProcByType(
                                    unsigned int      ulSubType);

/******************************************************************************
  函数名称  : ATP_UPG_StartTransItem
  功能描述  : 开始一次传输
  输入参数  :
              1. pcItemName : 传输项的名称，不允许为空
              2. pcCfgTool  : 发起传输工具的名称，可以为空
              3. pcUrl      : 远程URL，不能为空
              4. pcLocalIp  : 绑定本地IP，为空表示不绑定
              5. pcUserName : 传输时认证的用户名，可以为空
              6. pcPassWord : 传输时认证的密码，可以为空
              7. enDir      : 传输方向，上传或者下载
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-20
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_StartTransItem(
                            const char          *pcItemName,
                            const char          *pcCfgTool,
                            const char          *pcUrl,
                            const char          *pcLocalIp,
                            const char          *pcUserName,
                            const char          *pcPassWord,
                            ATP_UPG_TRANS_DIR_EN    enDir);

/******************************************************************************
  函数名称  : ATP_UPG_UpgradeFromHeaderFile
  功能描述  : 从多系统文件头来完成每个子文件的升级
              现有版本支持两种类型的文件头:
              1、多系统打包文件的文件头
              2、组播升级头
  输入参数  :
              1. pcBaseCmd      :   下载多系统打包文件头的命令行，
                                    如果为空，表示组播升级
              2. pcLocalIp      :   传输绑定的本地IP，可以为空，只在组播时使用
              3. pcFileHeader   :   文件头的路径，不允许为空
              4. pcCfgTool      :   配置工具名称，可以为空
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-20
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
unsigned int ATP_UPG_UpgradeFromHeaderFile(
                        const char               *apcBaseArgs[],
                        unsigned int        ulBaseArgCnt,
                        const char               *pcLocalIp,
                        const char               *pcFileHeader,
                        const char               *pcCfgTool);

unsigned int ATP_UPG_IsPkgTypeExist(
                                const ATP_UPG_FILE_ST   *pstPkgList,
                                unsigned int              ulPkgType);

ATP_UPG_SUBFILE_ST *ATP_UPG_SubFileFindByType(
                                ATP_UPG_FILE_ST     *pstPkgList,
                                unsigned int          ulPkgType);

unsigned int ATP_UPG_SubFileMoveToTheLast(
                                ATP_UPG_FILE_ST     *pstPkgList,
                                unsigned int          ulPkgType);

unsigned int ATP_UPG_SubFileMoveToTheFirst(
                                ATP_UPG_FILE_ST     *pstPkgList,
                                unsigned int          ulPkgType);

const char * ATP_UPG_GetVersion(void);

unsigned int ATP_UPG_StartTransItemEx(
                            const char          *pcItemName,
                            const char          *pcCfgTool,
                            const char          *pcUrl,
                            const char          *pcLocalIp,
                            const char          *pcUserName,
                            const char          *pcPassWord,
                            ATP_UPG_TRANS_DIR_EN    enDir,
                            const char          *pcHostName);


unsigned int ATP_UPG_Upgrade(unsigned char *pcImage, unsigned int ulLen, const char *pcCfgTool);

#ifdef ATP_DEBUG
#define ATP_UPG_DEBUG(format, args...)  {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#ifdef WIN32
#define ATP_UPG_DEBUG printf
#else
#define ATP_UPG_DEBUG(x...)
#endif
#endif

#endif

