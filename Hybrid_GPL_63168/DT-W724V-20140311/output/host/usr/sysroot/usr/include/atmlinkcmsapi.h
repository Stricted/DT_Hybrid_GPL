#ifndef __DSLCHANNELCMS_API_H__
#define __DSLCHANNELCMS_API_H__

//#define ATP_DEBUG
#ifdef ATP_DEBUG
#define ATP_ATMLINK_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);printf("\n");}
#else
#define ATP_ATMLINK_DEBUG(x...)
#endif

#define NASNAME(n,m,a,b) snprintf(n,m,"nas_%d_%d",a,b)

#define SUPPORT_ATP_XTM_DIAG

#define XTM_PORT_PATH0              0
#define XTM_PORT_PATH1              1
#define XTM_PORT_BOTH               4
#define XTM_PORTMASK_PATH0          0x1              
#define XTM_PORTMASK_PATH1          0x2              
#define XTM_PORTMASK_BOTH           (XTM_PORTMASK_PATH0 | XTM_PORTMASK_PATH1)              
#define XTM_NAME_LEN                16

// Values for ATM_SET_CONFIG ulATMQoS.
#define    ATM_SC_OTHER               0
#define    ATM_SC_UBR                 1
#define    ATM_SC_UBR_PCR             2
#define    ATM_SC_CBR                 3
#define    ATM_SC_NRT_VBR             4
#define    ATM_SC_RT_VBR              5

#define XTM_BUFF_LEN_16             16
#define XTM_BUFF_LEN_32             32
#define XTM_BUFF_LEN_128            128
#define XTM_BUFF_LEN_256            256
#define XTM_BUFF_LEN_512            512
#define XTM_BUFF_LEN_264            264
#define XTM_MAX_CMD_LEN             512

#define ATM_SC_UBR_STR              "ubr"
#define ATM_SC_UBR_PCR_STR          "ubr_pcr"
#define ATM_SC_CBR_STR              "cbr"
#define ATM_SC_NRT_VBR_STR          "nrtvbr"
#define ATM_SC_RT_VBR_STR           "rtvbr"

#define XTM_WANPRO_PPPOE            "pppoe"
#define XTM_WANPRO_IPOE             "ipoe"
#define XTM_WANPRO_IPOA             "ipoa"
#define XTM_WANPRO_PPP              "ppp"
#define XTM_WANPRO_PPPOA            "pppoa"
#define XTM_WANPRO_BRIDGE           "bridge"



#define ATM_TDTE_DEC_FILE           "/var/tdteshow"
#define ATM_MAX_VPI                 255
#define ATM_MIN_VCI                 32
#define ATM_MAX_VCI                 65535

#define ATM_LLC_ENCAPSULATION       1
#define ATM_VCMUX_ENCAPSULATION     2

// Values for network protocol
typedef enum
{
    ATP_ATM_PROTO_BEGIN,
    ATP_ATM_PROTO_EOA = ATP_ATM_PROTO_BEGIN,
    ATP_ATM_PROTO_PPPOE,
    ATP_ATM_PROTO_IPOA,
    ATP_ATM_PROTO_PPPOA,
    ATP_ATM_PROTO_CIP,
    ATP_ATM_PROTO_NONE,
}ATM_LINKTYPE_EN;

typedef struct tagAtm_Config_Para
{
    VOS_BOOL   bEnable;
    VOS_UINT32 ulVpi;
    VOS_UINT32 ulVci;
    VOS_UINT32 ulLinkType;                  // Link Type: EOA / PPPOA , IPOA
    VOS_UINT32 ulAalType;                   // AAL5
    VOS_UINT32 ulEncapMode;                 // encapsulation mode : LLC, VC-MUTEX
    VOS_UINT32 ulATMQoS;                    // ATM QOS: UBR etc
    VOS_UINT32 ulATMPeakCellRate;
    VOS_UINT32 ulATMMaximumBurstSize;
    VOS_UINT32 ulATMSustainableCellRate;
} ATM_CONFIG_ST;

#endif
