/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : atpstuncapi.h
  ��    ��  : handy
  ��    ��  : V1.0
  ��������  : 2007-10-26
  ��    ��  : ATPƽ̨STUNЭ��ͻ��˶���ӿڡ���Э��ջֻʵ����STUNC��BINDING����
              ���Ұ���STUNЭ��ʵ���˱����շ���
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-10-26
     ��    ��   : handy
     �޸�����   : ��ɳ���

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

/* STUNC Transaction ID�ĳ���*/
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

/* STUNC Transaction ID�ĳ��� */
#define ATP_STUNC_TRANSACTIONID_LEN_D           (16)

/* STUN��������Ĭ�϶˿� */
#define ATP_STUNC_DEFAULT_PORT_NUM_D            (3478)

/* STUN�ַ�����󳤶� */
#define ATP_STUNC_STRING_MAX_LEN_D              (256)

/* STUNC�ĳ��Դ��� */
#define ATP_STUNC_MAX_RETRY_NUM_D               (9)

/* STUN ��Ϣ���ͺ궨�� */
#define ATP_STUNC_MSG_BINDING_REQUEST           (0x0001)
#define ATP_STUNC_MSG_BINDING_RESPONSE          (0x0101)
#define ATP_STUNC_MSG_BINDING_ERR_RESPONSE      (0x0111)

        // ���汾��֧��
#define ATP_STUNC_MSG_SECRET_REQUEST            (0x0002)
#define ATP_STUNC_MSG_SECRET_RESPONSE           (0x0102)
#define ATP_STUNC_MSG_SECRET_ERR_RESPONSE       (0x0112)

/* STUN �������ͺ궨�� */
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
    STUNC��׼�����б��ο�RFC3489 Section 11.2 Message Attributes
    N/A :   ������
    O   :   ��ѡ
    M   :   ����
    C   :   ���������
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

/* TR111��չ�������ͺ궨�� */
#define ATP_TR111_ATTR_CONNECTION_REQUEST       (0xC001)
#define ATP_TR111_ATTR_BINDING_CHANGE           (0xC002)

/*
 * TR111��չ��CONNECTION-REQUEST-BINDING���Ե�ֵ
 * ����Ϊ���ַ���ֵ�����������Ŀո񣬴ճ�20�ֽڵĳ���
 */
#define ATP_TR111_VALUE_CONN_REQUEST            "dslforum.org/TR-111 "

/* CHANGE-REQUESTȡֵ�궨�� */
#define ATP_STUNC_CHANGE_ADDRESS_IP             (0x04)
#define ATP_STUNC_CHANGE_ADDRESS_PORT           (0x02)

/* STUN ������궨�� */
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

/* STUNC���Խṹ */
typedef struct tagATP_STUNC_ATTRIBUTE_ST
{
	VOS_UINT16              usAttrType;
	VOS_UINT16              usAttrLen;
	VOS_UINT8               *pucAttrValue;
	struct tagATP_STUNC_ATTRIBUTE_ST    *pstNext;
} ATP_STUNC_ATTRIBUTE_ST;

/* STUNC���ṹ */
typedef struct tagATP_STUNC_PACKET_ST
{
    VOS_UINT16              usMsgType;
    VOS_UINT16              usMsgLen;
    VOS_UINT8               aucTransactionID[ATP_STUNC_TRANSACTIONID_LEN_D];
    ATP_STUNC_ATTRIBUTE_ST  *pstAttrList;
} ATP_STUNC_PACKET_ST;

/* STUN�����ṹ�� */
typedef struct tagATP_STUNC_NAT_ENTRY_ST
{
    ATP_STUNC_SOCKET_D  sSocket;        // -1 ��ʾ��Ҫ����socket��
                                        // �����ʾʹ�����е�socket

    VOS_UINT32          ulLocalIP;      // ����IP�����Ϊ0������Ĭ��·�ɡ�
    VOS_UINT32          ulNatIP;        // NAT���IP
    VOS_UINT32          ulRemoteIP;	    // ������IP
    VOS_UINT16          usLocalPort;    // ���ض˿�
    VOS_UINT16          usNatPort;      // NAT��Ķ˿�
    VOS_UINT16          usRemotePort;   // �������˿�
    VOS_UINT16          usResponsePort; // ��Ӧ���ĵ�Դ�˿�
    VOS_UINT32          ulResponseIP;   // ��Ӧ���ĵ�ԴIP
    VOS_UINT8           *pucRecvBuf;    // ����û���Ҫ�ṩ�����������ɴ��ṩ������ָ��
    VOS_UINT32          ulBufLen;       // �û��ṩ�Ļ���������
    VOS_UINT32          ulRecvdDataLen; // ���յ������ݳ���
    VOS_CHAR            *pcUserName;    // ��֤�û���������Ϊ��
    VOS_CHAR            *pcPassword;    // ��֤���룬����Ϊ��
#ifdef SUPPORT_ATP_STUNC_TLS
    VOS_UINT32          ulLastTlsTtime;  // ��һ��ʹ��TLS��ʱ�䣬�ڲ�ʹ��
#endif
} ATP_STUNC_NAT_ENTRY_ST;

/* STUNC������߻�Ӧ���ĵĻص����� */
typedef VOS_UINT32 (*PFAtpStunPacketCb)(
                                ATP_STUNC_SOCKET_D  sSock,
                                ATP_STUNC_PACKET_ST *pstResponse);

/* ���յ���STUN����ʱ�Ļص����� */
typedef VOS_UINT32 (*PFAtpNonStunPacketCb)(
                                const VOS_UINT8 *pucPack,
                                VOS_UINT16 usPackLen,
                                ATP_STUNC_NAT_ENTRY_ST *pstNat);
                                

/*****************************************************************************
 �� �� ��  : ATP_STUNC_GetVersion
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��������汾��

 �޸���ʷ      :
  1.��    ��   : 2009��4��20��
    ��    ��   : ������42304
    �޸�����   : �����ɺ���

*****************************************************************************/
const VOS_CHAR * ATP_STUNC_GetVersion(void);

/******************************************************************************
  ��������  : ATP_STUNC_ParseAddrMap
  ��������  : ����STUN���������е�MAPPED-ADDRESS��ֵ
  �������  :
              1. pucAttrValue   :   ����ֵ
              2. ulAttrLen      :   ����ֵ����
  ���ú���  :
  ��������  :
  �������  :
              1. pusPort        :   ������Ķ˿ںţ�������
              2. pulIP          :   �������IP��ַ��������
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_ParseAddrMap(
                                           const VOS_UINT8      *pucAttrValue,
                                           VOS_UINT16           ulAttrLen,
                                           VOS_UINT16           *pusPort,
                                           VOS_UINT32           *pulIP);

/******************************************************************************
  ��������  : ATP_STUNC_AppendAddrMapAttr
  ��������  : ��STUN�������MAPPED-ADDRESSֵ����������Ѿ����ڣ��򸲸�
  �������  :
              1. pstPacket      :   ����ӵ�STUN��
              2. usAttrType     :   ����ӵ���������
              3. usPort         :   MAPPED-ADDRESS�Ķ˿�ֵ��������
              4. ulIP           :   MAPPED-ADDRESS��IP��ֵַ��������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_AppendAddrMapAttr(
                                           ATP_STUNC_PACKET_ST  *pstPacket,
                                           VOS_UINT16           usAttrType,
                                           VOS_UINT16           usPort,
                                           VOS_UINT32           ulIP);

/******************************************************************************
  ��������  : ATP_STUNC_AppendStrAttr
  ��������  : ��STUN��������ַ���ֵ������STUNЭ��Ҫ��ֵ����4�ֽڶ��룬
              ��ˣ��ṩ�˽ӿڱ�֤����ַ���ʱ���������
              ��������Ѿ����ڣ��򸲸�
  �������  :
              1. pstPacket      :   ����ӵ�STUN��
              2. usAttrType     :   ����ӵ���������
              3. pcAttrStr      :   ����ӵ�����ֵ�ַ���
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_AppendStrAttr(ATP_STUNC_PACKET_ST          *pstPacket,
                                           VOS_UINT16           usAttrType,
                                           const VOS_CHAR       *pcAttrStr);

/******************************************************************************
  ��������  : ATP_STUNC_AppendAttr
  ��������  : ��STUN�������һ��STUN���ԣ���������Ѿ����ڣ��򸲸�
  �������  :
              1. pstPacket      :   STUN��ָ��
              2. usAttrType     :   ��������
              3. usAttrLen      :   ����ֵ����
              4. pucAttrValue   :   ����ֵ�����Ϊ���������ݣ����뱣֤Ϊ��·��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-26
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_AppendAttr(ATP_STUNC_PACKET_ST *pstPacket,
                                        VOS_UINT16          usAttrType,
                                        VOS_UINT16          usAttrLen,
                                        const VOS_UINT8     *pucAttrValue);

/******************************************************************************
  ��������  : ATP_STUNC_GetAttribute
  ��������  : ��ȡSTUN����ָ��������
  �������  :
              1. pstPacket  :   STUN��ָ��
              2. usAttrType :   ����ȡ����������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              ����ҵ����򷵻ظ����Խṹ��ָ��
              ���û���ҵ����򷵻�NULL

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
const ATP_STUNC_ATTRIBUTE_ST *ATP_STUNC_GetAttribute(
                                const ATP_STUNC_PACKET_ST     *pstPacket,
                                VOS_UINT16                    usAttrType);

/******************************************************************************
  ��������  : ATP_STUNC_GenerateTransactionID
  ��������  : ΪSTUN�����������TransactionID
  �������  :
              1. pstPacket  :   ������TransactionID��STUN��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_GenerateTransactionID(
                                ATP_STUNC_PACKET_ST            *pstPacket);

/******************************************************************************
  ��������  : ATP_STUNC_DestroyPacketAttrs
  ��������  : ����STUN���е���������
  �������  :
              1. pstPacket  :   STUN��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_DestroyPacketAttrs(
                                    ATP_STUNC_PACKET_ST        *pstPacket);

/******************************************************************************
  ��������  : ATP_STUNC_SendPacket
  ��������  : �����Ѿ�����õ�STUN��������յ���Ӧ����������Ӧ����
              ���Ұѽ��������ŵ���Ӧ����
  �������  :
              1. sSecondSocket  :   ����Socket�������Ϊ-1��
                                    ��ʾʹ�ø�Socket���ͱ��ģ�
                                    ����ʹ��pstNat�е�Socket���ͱ���
              2. pstNat         :   NAT�ṹ��
              3. pstPacket      :   �����͵�STUN��
              4. pstResponse    :   ���յ��Ļ�Ӧ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_SendPacket(
                    ATP_STUNC_SOCKET_D                 sSecondSocket,
                    ATP_STUNC_NAT_ENTRY_ST             *pstNat,
                    ATP_STUNC_PACKET_ST                *pstPacket,
                    ATP_STUNC_PACKET_ST                *pstResponse);

/******************************************************************************
  ��������  : ATP_STUNC_Init
  ��������  : ��ʼ��STUNCģ��ı���IP�Ͷ˿�
  �������  :
              1. usRetryTimes : ���Դ���������Ϊ[1-9]��
                                �����������ֵ��ʹ��Ĭ��ֵ9
              2. pcUsername   : STUNC����֤�û���
                                �������Ϊ�գ����ͨ��TLS��ȡ��֤��Ϣ��
                                ����ֻʹ������ֵ
              3. pcPassword   : STUNC����֤���룬ͬ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-27
     ��    ��   : handy
     �޸�����   : ��ɳ���
   2.��    ��   : 2008-3-12
     ��    ��   : handy
     �޸�����   : ���V100R002C02����

******************************************************************************/
VOS_UINT32 ATP_STUNC_Init(
                                VOS_UINT16          usRetryTimes,
                                const VOS_CHAR      *pcUsername,
                                const VOS_CHAR      *pcPassword);

/******************************************************************************
  ��������  : ATP_STUNC_UnInit
  ��������  : ȥ��ʼ��STUNCģ��
  �������  : ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-26
     ��    ��   : handy
     �޸�����   : ��ɳ���
   2.��    ��   : 2008-3-12
     ��    ��   : handy
     �޸�����   : ���V100R002C02����

******************************************************************************/
VOS_UINT32 ATP_STUNC_UnInit(VOS_VOID);

/******************************************************************************
  ��������  : ATP_STUNC_GetNatIPPort
  ��������  : ��ȡNATת�����IP�Ͷ˿�
  �������  :
              1. pstNat     :   NAT�ṹ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-27
     ��    ��   : handy
     �޸�����   : ��ɳ���
   2.��    ��   : 2008-3-12
     ��    ��   : handy
     �޸�����   : ���V100R002C02����

******************************************************************************/
VOS_UINT32 ATP_STUNC_GetNatIPPort(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat);

/******************************************************************************
  ��������  : ATP_STUNC_GetMultiNatIPPort
  ��������  : ��ȡ���NAT��ÿ��NAT�����Լ�����socket
  �������  :
              1. pstNat[]   :   ����ѯ��NATָ������
              2. ulNum      :   ����ѯ��NAT����
  ���ú���  :
  ��������  :
  �������  :
              1. pulIdx     :   ��ȡ�����ݵ�NAT���
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-5-16
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_GetMultiNatIPPort(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat[],
                            VOS_UINT32              ulNum,
                            VOS_INT32               lSendIdx,
                            VOS_UINT32              *pulIdx);


/******************************************************************************
  ��������  : ATP_STUNC_IsNatTimeout
  ��������  : �ж�NAT�Ƿ��Ѿ���ʱ
  �������  :
              1. pstNat     :   NAT�ṹ��
              2. pbTimeout  :   �Ƿ�ʱ��VOS_TRUE: �ǣ�VOS_FALSE: ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-27
     ��    ��   : handy
     �޸�����   : ��ɳ���
   2.��    ��   : 2008-3-12
     ��    ��   : handy
     �޸�����   : ���V100R002C02����

******************************************************************************/
VOS_UINT32 ATP_STUNC_IsNatTimeout(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat,
                            VOS_BOOL                *pbTimeout);

/******************************************************************************
  ��������  : ATP_STUNC_IsNatSymmetric
  ��������  : �ж�NAT�豸�Ƿ�Ϊ�Գ�NAT��STUNЭ�鲻��������Խ�Գ�NAT�豸
  �������  :
              1. pstNat     :   NAT�ṹ��
              2. pbSymmetric:   �Ƿ�Ϊ�Գ�NAT�豸��VOS_TRUE: �ǣ�VOS_FALSE: ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-27
     ��    ��   : handy
     �޸�����   : ��ɳ���
   2.��    ��   : 2008-3-12
     ��    ��   : handy
     �޸�����   : ���V100R002C02����

******************************************************************************/
VOS_UINT32 ATP_STUNC_IsNatSymmetric(
                            ATP_STUNC_NAT_ENTRY_ST  *pstNat,
                            VOS_BOOL                *pbSymmetric);

/******************************************************************************
  ��������  : ATP_STUNC_Start
  ��������  : ����STUNCģ�飬������ִ���NAT�󣬸ú��������˳�����ʱ��
              ���Զ�ѧϰNAT�豸��ˢ�����ڣ���������ˢ��NAT��
  �������  :
              1. sSocket        :   ����socket�������Ϊ-1����ʾ�������Ѿ�������
                                    socket�����򱾺��������д���socket
              2. ulLocalIP      :   ���ص�IP�������ʽ��IP
              3. usLocalPort    :   ���صĶ˿�
              4. lMaxPeriod     :   STUNCˢ�µ�������ڣ�����Ϊ-1����ʾû������
              5. ulMinPeriod    :   STUNCˢ�µ���С����
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-10-26
     ��    ��   : handy
     �޸�����   : ��ɳ���
   2.��    ��   : 2008-3-12
     ��    ��   : handy
     �޸�����   : ���V100R002C02����

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
  ��������  : ATP_STUNC_RegPacketHooks
  ��������  : ע��STUN���Ļص�����
  �������  :
              1. pfNonStunCb    :   ��STUN���ĵĴ�����
              2. pfReqCb        :   STUN�����Ļص�����
              3. pfResCb        :   STUN��Ӧ���Ļص�����
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2007-10-26
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_STUNC_RegPacketHooks(
                                        PFAtpNonStunPacketCb    pfNonStunCb,
                                        PFAtpStunPacketCb       pfReqCb,
                                        PFAtpStunPacketCb       pfResCb);

#ifdef SUPPORT_ATP_STUNC_TLS
/******************************************************************************
  ��������  : ATP_STUNC_RegRootCertPath
  ��������  : ע��TLS��ȡ�û�������֤�������ĸ�֤��·����ֻ��֧��TLSʱʹ��
  �������  :
              1. pcPath :   ��֤��·��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-4-15
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_STUNC_RegRootCertPath(const VOS_CHAR *pcPath);
#endif

#endif

