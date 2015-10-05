/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : atpstuncapi.h
  作    者  : handy
  版    本  : V1.0
  创建日期  : 2007-10-26
  描    述  : ATP平台STUN协议客户端对外接口。本协议栈只实现了STUNC的BINDING请求
              并且按照STUN协议实现了报文收发，
  函数列表  :

  历史记录      :
   1.日    期   : 2007-10-26
     作    者   : handy
     修改内容   : 完成初稿

*********************************************************************************/

#ifndef __ATPSTUNCAPI_H__
#define __ATPSTUNCAPI_H__

#ifdef WIN32
#define ATP_STUNC_SOCKET_D      SOCKET
#else
#define ATP_STUNC_SOCKET_D      int
#endif

#define ATP_STUNC_INVALID_SOCKET_D      (-1)

#if 0

#ifndef VOS_OK
#define VOS_OK          (0)
#endif

#ifndef VOS_TRUE
#define VOS_TRUE        (1)
#endif

#ifndef VOS_FALSE
#define VOS_FALSE       (0)
#endif

#ifndef VOS_VOID
#define VOS_VOID        void
#endif

#ifndef VOS_BOOL
#define VOS_BOOL        unsigned int
#endif

#ifndef VOS_UINT32
#define VOS_UINT32      unsigned int
#endif

#ifndef VOS_INT32
#define VOS_INT32       int
#endif

#ifndef VOS_UINT16
#define VOS_UINT16      unsigned short
#endif

#ifndef VOS_INT16
#define VOS_INT16       short
#endif

#ifndef VOS_UINT8
#define VOS_UINT8       unsigned char
#endif

#ifndef VOS_INT8
#define VOS_INT8        char
#endif

#ifndef VOS_CHAR
#define VOS_CHAR        char
#endif

#endif

/* STUNC Transaction ID的长度*/
typedef enum tagATP_STUNC_ERR_CODE_EN
{
    ATP_ERR_STUNC_PARA_NULL         = 1,
    ATP_ERR_STUNC_PARA_INVALID,
    ATP_ERR_STUNC_MEM_NOT_ENOUGH,
    ATP_ERR_STUNC_NOT_INITED,
    ATP_ERR_STUNC_NOT_UNINITED,
    ATP_ERR_STUNC_HMAC,
    ATP_ERR_STUNC_STRING_TOO_LONG,
    ATP_ERR_STUNC_PACKET_TOO_LONG,
    ATP_ERR_STUNC_PACKET_TOO_SHORT,
    ATP_ERR_STUNC_SOCKET,
    ATP_ERR_STUNC_BIND,
    ATP_ERR_STUNC_UNKNOWN_HOST,
    ATP_ERR_STUNC_INVALID_PACKET,
    ATP_ERR_STUNC_INVALID_ATTR,
    ATP_ERR_STUNC_TLS,
    ATP_ERR_STUNC_TIMEOUT,
    ATP_ERR_STUNC_NO_RESPONSE,
    ATP_ERR_STUNC_FORMAT,
    ATP_ERR_STUNC_NONSTUN_DATA
} ATP_STUNC_ERR_CODE_EN;

/* STUNC Transaction ID的长度 */
#define ATP_STUNC_TRANSACTIONID_LEN_D           (16)

/* STUN服务器的默认端口 */
#define ATP_STUNC_DEFAULT_PORT_NUM_D            (3478)

/* STUN字符串最大长度 */
#define ATP_STUNC_STRING_MAX_LEN_D              (256)

/* STUNC的尝试次数 */
#define ATP_STUNC_MAX_RETRY_NUM_D               (9)

/* STUN 消息类型宏定义 */
#define ATP_STUNC_MSG_BINDING_REQUEST           (0x0001)
#define ATP_STUNC_MSG_BINDING_RESPONSE          (0x0101)
#define ATP_STUNC_MSG_BINDING_ERR_RESPONSE      (0x0111)

        // 本版本不支持
#define ATP_STUNC_MSG_SECRET_REQUEST            (0x0002)
#define ATP_STUNC_MSG_SECRET_RESPONSE           (0x0102)
#define ATP_STUNC_MSG_SECRET_ERR_RESPONSE       (0x0112)

/* STUN 属性类型宏定义 */
#define ATP_STUNC_ATTR_MAPPED_ADDRESS           (0x0001)
#define ATP_STUNC_ATTR_RESPONSE_ADDRESS         (0x0002)
#define ATP_STUNC_ATTR_CHANGE_ADDRESS           (0x0003)
#define ATP_STUNC_ATTR_SOURCE_ADDRESS           (0x0004)
#define ATP_STUNC_ATTR_CHANGED_ADDRESS          (0x0005)
#define ATP_STUNC_ATTR_USERNAME                 (0x0006)
#define ATP_STUNC_ATTR_PASSWORD                 (0x0007)
#define ATP_STUNC_ATTR_MSG_INTEGRITY            (0x0008)
#define ATP_STUNC_ATTR_ERR_CODE                 (0x0009)
#define ATP_STUNC_ATTR_UNKNOWN_ATTR             (0x000A)
#define ATP_STUNC_ATTR_REFLECTED_FROM           (0x000B)

/*
    STUNC标准属性列表，参考RFC3489 Section 11.2 Message Attributes
    N/A :   不适用
    O   :   可选
    M   :   必须
    C   :   视情况而定
                                       Binding  Shared  Shared  Shared
                     Binding  Binding  Error    Secret  Secret  Secret
 Att.                Req.     Resp.    Resp.    Req.    Resp.   Error
                                                                Resp.
 _____________________________________________________________________
 MAPPED-ADDRESS      N/A      M        N/A      N/A     N/A     N/A
 RESPONSE-ADDRESS    O        N/A      N/A      N/A     N/A     N/A
 CHANGE-REQUEST      O        N/A      N/A      N/A     N/A     N/A
 SOURCE-ADDRESS      N/A      M        N/A      N/A     N/A     N/A
 CHANGED-ADDRESS     N/A      M        N/A      N/A     N/A     N/A
 USERNAME            O        N/A      N/A      N/A     M       N/A
 PASSWORD            N/A      N/A      N/A      N/A     M       N/A
 MESSAGE-INTEGRITY   O        O        N/A      N/A     N/A     N/A
 ERROR-CODE          N/A      N/A      M        N/A     N/A     M
 UNKNOWN-ATTRIBUTES  N/A      N/A      C        N/A     N/A     C
 REFLECTED-FROM      N/A      C        N/A      N/A     N/A     N/A
 */

/* TR111扩展属性类型宏定义 */
#define ATP_TR111_ATTR_CONNECTION_REQUEST       (0xC001)
#define ATP_TR111_ATTR_BINDING_CHANGE           (0xC002)

/*
 * TR111扩展的CONNECTION-REQUEST-BINDING属性的值
 * 必须为该字符串值，必须有最后的空格，凑成20字节的长度
 */
#define ATP_TR111_VALUE_CONN_REQUEST            "dslforum.org/TR-111 "

/* CHANGE-REQUEST取值宏定义 */
#define ATP_STUNC_CHANGE_ADDRESS_IP             (0x04)
#define ATP_STUNC_CHANGE_ADDRESS_PORT           (0x02)

/* STUN 错误码宏定义 */
#define ATP_STUNC_RESPONSE_BAD_REQUEST          (400)
#define ATP_STUNC_RESPONSE_UNAUTHORIZED         (401)
#define ATP_STUNC_RESPONSE_UNKNOWN_ATTR         (420)
#define ATP_STUNC_RESPONSE_STALE_CREDENTIAL     (430)
#define ATP_STUNC_RESPONSE_INT_CHECK_FAIL       (431)
#define ATP_STUNC_RESPONSE_MISSING_USERNAME     (432)
#define ATP_STUNC_RESPONSE_MISSING_USERNAME     (432)
#define ATP_STUNC_RESPONSE_USE_TLS              (433)
#define ATP_STUNC_RESPONSE_SERVER_ERR           (500)   // Client should retry
#define ATP_STUNC_RESPONSE_GLOBAL_FAILURE       (600)   // Should not retry

/* STUNC属性结构 */
typedef struct tagATP_STUNC_ATTRIBUTE_ST
{
	VOS_UINT16              usAttrType;
	VOS_UINT16              usAttrLen;
	VOS_UINT8               *pucAttrValue;
	struct tagATP_STUNC_ATTRIBUTE_ST    *pstNext;
} ATP_STUNC_ATTRIBUTE_ST;

/* STUNC包结构 */
typedef struct tagATP_STUNC_PACKET_ST
{
    VOS_UINT16              usMsgType;
    VOS_UINT16              usMsgLen;
    VOS_UINT8               aucTransactionID[ATP_STUNC_TRANSACTIONID_LEN_D];
    ATP_STUNC_ATTRIBUTE_ST  *pstAttrList;
} ATP_STUNC_PACKET_ST;

/* STUN参数结构体 */
typedef struct tagATP_STUNC_NAT_ENTRY_ST
{
    ATP_STUNC_SOCKET_D  sSocket;        // -1 表示需要创建socket；
                                        // 否则表示使用已有的socket

    VOS_UINT32          ulLocalIP;      // 本地IP，如果为0，则走默认路由。
    VOS_UINT32          ulNatIP;        // NAT后的IP
    VOS_UINT32          ulRemoteIP;	    // 服务器IP
    VOS_UINT16          usLocalPort;    // 本地端口
    VOS_UINT16          usNatPort;      // NAT后的端口
    VOS_UINT16          usRemotePort;   // 服务器端口
    VOS_UINT16          usResponsePort; // 回应报文的源端口
    VOS_UINT32          ulResponseIP;   // 回应报文的源IP
    VOS_UINT8           *pucRecvBuf;    // 如果用户需要提供缓冲区，则由此提供缓冲区指针
    VOS_UINT32          ulBufLen;       // 用户提供的缓冲区长度
    VOS_UINT32          ulRecvdDataLen; // 接收到的数据长度
    VOS_CHAR            *pcUserName;    // 认证用户名，可以为空
    VOS_CHAR            *pcPassword;    // 认证密码，可以为空
#ifdef SUPPORT_ATP_STUNC_TLS
    VOS_UINT32          ulLastTlsTtime;  // 上一次使用TLS的时间，内部使用
#endif
} ATP_STUNC_NAT_ENTRY_ST;

/* STUNC请求或者回应报文的回调函数 */
typedef VOS_UINT32 (*PFAtpStunPacketCb)(
                                ATP_STUNC_SOCKET_D  sSock,
                                ATP_STUNC_PACKET_ST *pstResponse);

/* 接收到非STUN报文时的回调函数 */
typedef VOS_UINT32 (*PFAtpNonStunPacketCb)(
                                const VOS_UINT8 *pucPack,
                                VOS_UINT16 usPackLen,
                                ATP_STUNC_NAT_ENTRY_ST *pstNat);
                                

/*****************************************************************************
 函 数 名  : ATP_STUNC_GetVersion
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 返回组件版本号

 修改历史      :
  1.日    期   : 2009年4月20日
    作    者   : 俞佑清42304
    修改内容   : 新生成函数

*****************************************************************************/
const VOS_CHAR * ATP_STUNC_GetVersion(void);

/******************************************************************************
  函数名称  : ATP_STUNC_ParseAddrMap
  功能描述  : 解析STUN报文属性中的MAPPED-ADDRESS的值
  输入参数  :
              1. pucAttrValue   :   属性值
              2. ulAttrLen      :   属性值长度
  调用函数  :
  被调函数  :
  输出参数  :
              1. pusPort        :   解析后的端口号，主机序
              2. pulIP          :   解析后的IP地址，网络序
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_ParseAddrMap(
                                           const VOS_UINT8      *pucAttrValue,
                                           VOS_UINT16           ulAttrLen,
                                           VOS_UINT16           *pusPort,
                                           VOS_UINT32           *pulIP);

/******************************************************************************
  函数名称  : ATP_STUNC_AppendAddrMapAttr
  功能描述  : 往STUN包中添加MAPPED-ADDRESS值，如果属性已经存在，则覆盖
  输入参数  :
              1. pstPacket      :   待添加的STUN包
              2. usAttrType     :   待添加的属性类型
              3. usPort         :   MAPPED-ADDRESS的端口值，主机序
              4. ulIP           :   MAPPED-ADDRESS的IP地址值，网络序
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_AppendAddrMapAttr(
                                           ATP_STUNC_PACKET_ST  *pstPacket,
                                           VOS_UINT16           usAttrType,
                                           VOS_UINT16           usPort,
                                           VOS_UINT32           ulIP);

/******************************************************************************
  函数名称  : ATP_STUNC_AppendStrAttr
  功能描述  : 往STUN包中添加字符串值，由于STUN协议要求值必须4字节对齐，
              因此，提供此接口保证添加字符串时满足该需求。
              如果属性已经存在，则覆盖
  输入参数  :
              1. pstPacket      :   待添加的STUN包
              2. usAttrType     :   待添加的属性类型
              3. pcAttrStr      :   待添加的属性值字符串
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_AppendStrAttr(ATP_STUNC_PACKET_ST          *pstPacket,
                                           VOS_UINT16           usAttrType,
                                           const VOS_CHAR       *pcAttrStr);

/******************************************************************************
  函数名称  : ATP_STUNC_AppendAttr
  功能描述  : 往STUN包中添加一个STUN属性，如果属性已经存在，则覆盖
  输入参数  :
              1. pstPacket      :   STUN包指针
              2. usAttrType     :   属性类型
              3. usAttrLen      :   属性值长度
              4. pucAttrValue   :   属性值，如果为二进制数据，必须保证为网路序
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-26
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_AppendAttr(ATP_STUNC_PACKET_ST *pstPacket,
                                        VOS_UINT16          usAttrType,
                                        VOS_UINT16          usAttrLen,
                                        const VOS_UINT8     *pucAttrValue);

/******************************************************************************
  函数名称  : ATP_STUNC_GetAttribute
  功能描述  : 获取STUN包中指定的属性
  输入参数  :
              1. pstPacket  :   STUN包指针
              2. usAttrType :   待获取的属性类型
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  :
              如果找到，则返回该属性结构体指针
              如果没有找到，则返回NULL

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
const ATP_STUNC_ATTRIBUTE_ST *ATP_STUNC_GetAttribute(
                                const ATP_STUNC_PACKET_ST     *pstPacket,
                                VOS_UINT16                    usAttrType);

/******************************************************************************
  函数名称  : ATP_STUNC_GenerateTransactionID
  功能描述  : 为STUN包产生随机的TransactionID
  输入参数  :
              1. pstPacket  :   待产生TransactionID的STUN包
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_GenerateTransactionID(
                                ATP_STUNC_PACKET_ST            *pstPacket);

/******************************************************************************
  函数名称  : ATP_STUNC_DestroyPacketAttrs
  功能描述  : 销毁STUN包中的所有属性
  输入参数  :
              1. pstPacket  :   STUN包指针
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_DestroyPacketAttrs(
                                    ATP_STUNC_PACKET_ST        *pstPacket);

/******************************************************************************
  函数名称  : ATP_STUNC_SendPacket
  功能描述  : 发送已经构造好的STUN包，如果收到回应，则会解析回应包，
              并且把解析结果存放到回应包中
  输入参数  :
              1. sSecondSocket  :   发送Socket，如果不为-1，
                                    表示使用该Socket发送报文；
                                    否则，使用pstNat中的Socket发送报文
              2. pstNat         :   NAT结构体
              3. pstPacket      :   待发送的STUN包
              4. pstResponse    :   接收到的回应包
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-31
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_SendPacket(
                    ATP_STUNC_SOCKET_D                 sSecondSocket,
                    ATP_STUNC_NAT_ENTRY_ST             *pstNat,
                    ATP_STUNC_PACKET_ST                *pstPacket,
                    ATP_STUNC_PACKET_ST                *pstResponse);

/******************************************************************************
  函数名称  : ATP_STUNC_Init
  功能描述  : 初始化STUNC模块的本地IP和端口
  输入参数  :
              1. usRetryTimes : 重试次数，可以为[1-9]，
                                如果输入其它值则使用默认值9
              2. pcUsername   : STUNC的认证用户名
                                如果配置为空，则会通过TLS获取认证信息；
                                否则，只使用配置值
              3. pcPassword   : STUNC的认证密码，同上
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-27
     作    者   : handy
     修改内容   : 完成初稿
   2.日    期   : 2008-3-12
     作    者   : handy
     修改内容   : 完成V100R002C02需求

******************************************************************************/
VOS_UINT32 ATP_STUNC_Init(
                                VOS_UINT16          usRetryTimes,
                                const VOS_CHAR      *pcUsername,
                                const VOS_CHAR      *pcPassword);

/******************************************************************************
  函数名称  : ATP_STUNC_UnInit
  功能描述  : 去初始化STUNC模块
  输入参数  : 无
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-26
     作    者   : handy
     修改内容   : 完成初稿
   2.日    期   : 2008-3-12
     作    者   : handy
     修改内容   : 完成V100R002C02需求

******************************************************************************/
VOS_UINT32 ATP_STUNC_UnInit(VOS_VOID);

/******************************************************************************
  函数名称  : ATP_STUNC_GetNatIPPort
  功能描述  : 获取NAT转换后的IP和端口
  输入参数  :
              1. pstNat     :   NAT结构体
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-27
     作    者   : handy
     修改内容   : 完成初稿
   2.日    期   : 2008-3-12
     作    者   : handy
     修改内容   : 完成V100R002C02需求

******************************************************************************/
VOS_UINT32 ATP_STUNC_GetNatIPPort(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat);

/******************************************************************************
  函数名称  : ATP_STUNC_GetMultiNatIPPort
  功能描述  : 获取多个NAT，每个NAT必须自己创建socket
  输入参数  :
              1. pstNat[]   :   待查询的NAT指针数组
              2. ulNum      :   待查询的NAT个数
  调用函数  :
  被调函数  :
  输出参数  :
              1. pulIdx     :   获取到数据的NAT序号
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-5-16
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_GetMultiNatIPPort(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat[],
                            VOS_UINT32              ulNum,
                            VOS_INT32               lSendIdx,
                            VOS_UINT32              *pulIdx);


/******************************************************************************
  函数名称  : ATP_STUNC_IsNatTimeout
  功能描述  : 判断NAT是否已经超时
  输入参数  :
              1. pstNat     :   NAT结构体
              2. pbTimeout  :   是否超时，VOS_TRUE: 是；VOS_FALSE: 否
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-27
     作    者   : handy
     修改内容   : 完成初稿
   2.日    期   : 2008-3-12
     作    者   : handy
     修改内容   : 完成V100R002C02需求

******************************************************************************/
VOS_UINT32 ATP_STUNC_IsNatTimeout(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat,
                            VOS_BOOL                *pbTimeout);

/******************************************************************************
  函数名称  : ATP_STUNC_IsNatSymmetric
  功能描述  : 判断NAT设备是否为对称NAT，STUN协议不能正常穿越对称NAT设备
  输入参数  :
              1. pstNat     :   NAT结构体
              2. pbSymmetric:   是否为对称NAT设备，VOS_TRUE: 是；VOS_FALSE: 否
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-27
     作    者   : handy
     修改内容   : 完成初稿
   2.日    期   : 2008-3-12
     作    者   : handy
     修改内容   : 完成V100R002C02需求

******************************************************************************/
VOS_UINT32 ATP_STUNC_IsNatSymmetric(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat,
                            VOS_BOOL                *pbSymmetric);

/******************************************************************************
  函数名称  : ATP_STUNC_Start
  功能描述  : 启动STUNC模块，如果发现处在NAT后，该函数不会退出，此时，
              会自动学习NAT设备的刷新周期，并且周期刷新NAT。
  输入参数  :
              1. sSocket        :   本地socket，如果不为-1，表示调用者已经创建了
                                    socket；否则本函数会自行创建socket
              2. ulLocalIP      :   本地的IP，网络格式的IP
              3. usLocalPort    :   本地的端口
              4. lMaxPeriod     :   STUNC刷新的最大周期，可以为-1，表示没有限制
              5. ulMinPeriod    :   STUNC刷新的最小周期
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2007-10-26
     作    者   : handy
     修改内容   : 完成初稿
   2.日    期   : 2008-3-12
     作    者   : handy
     修改内容   : 完成V100R002C02需求

******************************************************************************/
VOS_UINT32 ATP_STUNC_Start(
                                ATP_STUNC_SOCKET_D  sSocket,
                                VOS_UINT32          ulLocalIP,
                                VOS_UINT16          usLocalPort,
                                VOS_UINT32          ulRemoteIP,
                                VOS_UINT16          usRemotePort,
                                VOS_INT32           lMaxPeriod,
                                VOS_UINT32          ulMinPeriod);

/******************************************************************************
  函数名称  : ATP_STUNC_RegPacketHooks
  功能描述  : 注册STUN报文回调函数
  输入参数  :
              1. pfNonStunCb    :   非STUN报文的处理函数
              2. pfReqCb        :   STUN请求报文回调函数
              3. pfResCb        :   STUN回应报文回调函数
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : VOS_OK

  修改历史      :
   1.日    期   : 2007-10-26
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32 ATP_STUNC_RegPacketHooks(
                                        PFAtpNonStunPacketCb    pfNonStunCb,
                                        PFAtpStunPacketCb       pfReqCb,
                                        PFAtpStunPacketCb       pfResCb);

#ifdef SUPPORT_ATP_STUNC_TLS
/******************************************************************************
  函数名称  : ATP_STUNC_RegRootCertPath
  功能描述  : 注册TLS获取用户名是认证服务器的根证书路径，只在支持TLS时使用
  输入参数  :
              1. pcPath :   根证书路径
  调用函数  :
  被调函数  :
  输出参数  : 无
  返 回 值  : 无

  修改历史      :
   1.日    期   : 2008-4-15
     作    者   : handy
     修改内容   : 完成初稿

******************************************************************************/
VOS_UINT32  ATP_STUNC_RegRootCertPath(const VOS_CHAR *pcPath);
#endif

#endif

