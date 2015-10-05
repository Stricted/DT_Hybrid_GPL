#ifndef __MCAST_API_H__
#define __MCAST_API_H__

#define ATP_MCAST_SUBFILE_PACK_LEN          (1024)

/* ����Ƶ������ÿ����Ƶ���ı��ĸ�ʽ */
typedef struct tagATP_UPG_CHANNEL_INFO
{
    unsigned char               aucAddrType[4];
    unsigned char               aucChannelAddr[16];
    unsigned char               aucChannelPort[4];
    unsigned char               aucFileType[4];
    unsigned char               aucFileLen[4];
    unsigned char               aucSoftVersion[32];
} ATP_UPG_CHANNEL_INFO;

/* ����Ƶ���鲥����ͷ��Ϣ */
typedef struct tagATP_UPG_COMMON_HEADER
{
    unsigned char               aucPacketLen[4];        // ��ǰ�鲥������
    unsigned char               aucCrc[4];              // ��ǰ�鲥����CRC
    unsigned char               aucVersion[32];         // �ܰ汾��Ϣ
    unsigned char               aucProductInfo[32];     // ��Ʒ��Ϣ
    unsigned char               aucChannelNum[4];       // �����ļ��鲥Ƶ���ĸ���
    unsigned char               aucStatusUrl[32];       // ״̬URL
} ATP_UPG_COMMON_HEADER;

/* ��Ƶ���鲥����ͷ��Ϣ */
typedef struct tagATP_UPG_MULT_PACKET
{
    unsigned char               aucPacketLen[4];        // ��ǰ�鲥���ĳ���
    unsigned char               aucCrc[4];              // ��ǰ�鲥����У���
    unsigned char               aucSeqNum[4];           // ��ǰ������ţ���0��ʼ
    unsigned char               aucImgLen[4];           // �ļ��ܳ���
    unsigned char               aucPkgType[4];          // ��ǰ�����ļ�����
    char                aucVersion[32];         // �汾��Ϣ
    char                aucProductInfo[32];     // ��Ʒ��Ϣ
} ATP_UPG_MULT_PACKET;



#endif

