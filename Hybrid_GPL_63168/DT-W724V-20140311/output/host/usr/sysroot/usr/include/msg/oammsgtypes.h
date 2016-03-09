/**\file atsmsgtypes.h
  *oam��Ϣ������ҵ��ģ�����Ϣ
  *\copyright 2007-2020����Ϊ�������޹�˾
  *\author l00135113
  *\date  2012-6-5
*/
#ifndef __OAMMSGTYPES_H__
#define __OAMMSGTYPES_H__

#define ATP_CBBID_OAM_NAME "oam"

#define OAM_INFO_SEND_FILE                 "/var/oam/infooampdusend"
#define OAM_INFO_RECV_FILE                 "/var/oam/infooampdurecv"

#define OAM_UNI_EVENT_SEND_FILE                 "/var/oam/unieventsend"
#define OAM_UNI_EVENT_RECV_FILE                 "/var/oam/unieventrecv"

#define OAM_DUP_EVENT_SEND_FILE                 "/var/oam/dupeventsend"
#define OAM_DUP_EVENT_RECV_FILE                 "/var/oam/dupeventrecv"


#define OAM_SYS_CMD_LEN                 512


/*��Ϣ����*/
typedef enum tagATP_OAM_MSG_TYPE_EN
{
    ATP_OAM_MSG_OAM_START       =  ATP_MSG_CATEGORY_DEF(ATP_MODULE_OAM),
    ATP_MSG_OAM_DYING_GASP,

    ATP_OAM_MSG_OAM_END
} ATP_OAM_MSG_TYPE_EN;

/*OAMͳ����Ϣ*/
typedef struct tagATP_OAM_SHOW_STATISTICS_MSG{
    VOS_UINT64  udlInfoOamPduRecv;
    VOS_UINT64  udlInfoOamPduSend;
    VOS_UINT64  udlUniEventOamPduRecv;
    VOS_UINT64  udlDuplicateEventOamPduRecv;
    VOS_UINT64  udlUniEventOamPduSend;
    VOS_UINT64  udlDuplicateEventOamPduSend;
}ATP_OAM_SHOW_STATISTICS_MSG;

#endif
