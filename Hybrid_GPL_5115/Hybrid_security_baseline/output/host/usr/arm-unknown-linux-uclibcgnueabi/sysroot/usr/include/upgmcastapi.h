#ifndef __MCAST_API_H__
#define __MCAST_API_H__

#define ATP_MCAST_SUBFILE_PACK_LEN          (1024)

/* 公共频道描述每个子频道的报文格式 */
typedef struct tagATP_UPG_CHANNEL_INFO
{
    unsigned char               aucAddrType[4];
    unsigned char               aucChannelAddr[16];
    unsigned char               aucChannelPort[4];
    unsigned char               aucFileType[4];
    unsigned char               aucFileLen[4];
    unsigned char               aucSoftVersion[32];
} ATP_UPG_CHANNEL_INFO;

/* 公共频道组播报文头信息 */
typedef struct tagATP_UPG_COMMON_HEADER
{
    unsigned char               aucPacketLen[4];        // 当前组播包长度
    unsigned char               aucCrc[4];              // 当前组播包的CRC
    unsigned char               aucVersion[32];         // 总版本信息
    unsigned char               aucProductInfo[32];     // 产品信息
    unsigned char               aucChannelNum[4];       // 镜像文件组播频道的个数
    unsigned char               aucStatusUrl[32];       // 状态URL
} ATP_UPG_COMMON_HEADER;

/* 子频道组播报文头信息 */
typedef struct tagATP_UPG_MULT_PACKET
{
    unsigned char               aucPacketLen[4];        // 当前组播包的长度
    unsigned char               aucCrc[4];              // 当前组播包的校验和
    unsigned char               aucSeqNum[4];           // 当前包的序号，从0开始
    unsigned char               aucImgLen[4];           // 文件总长度
    unsigned char               aucPkgType[4];          // 当前镜像文件类型
    char                aucVersion[32];         // 版本信息
    char                aucProductInfo[32];     // 产品信息
} ATP_UPG_MULT_PACKET;



#endif

