#ifndef _IPT_TRIGGER_H_target
#define _IPT_TRIGGER_H_target

#ifndef ATP_SUPPORT_PRTT_RPROTOCOL
#define ATP_SUPPORT_PRTT_RPROTOCOL
#endif

#define TRIGGER_TIMEOUT 600	/* 600 secs */

enum ipt_trigger_type
{
	IPT_TRIGGER_DNAT = 1,
	IPT_TRIGGER_IN = 2,
	IPT_TRIGGER_OUT = 3
};

/******************һ������:�ں���չ����һ������֧��2000-2038,2050-2051,2069,2085,3010-3030��ʽ��ģʽ��
    ����Ҫ���ⲿ�˿ںŽ������ֶΣ�������У�鱣֤��ʽ��ȷ*****************************/
#define PT_OPEN_PORT_RANGE_MAX 8
typedef struct tatPT_OPEN_APP_PORT_RANGE
{

	u_int16_t ulStartPort;                                             /*������ʼ�˿�*/
	u_int16_t ulEndPort;                                               /*���Ž����˿�*/

}PT_OPEN_APP_PORT_RANGE;

struct ipt_trigger_ports {
	u_int16_t mport[2];	/* Related destination port range */
//	u_int16_t rport[2];
	PT_OPEN_APP_PORT_RANGE stOpenPort[PT_OPEN_PORT_RANGE_MAX];	/* Port range to map related destination port range to */
};

struct ipt_trigger_info {
	enum ipt_trigger_type type;
	u_int16_t proto;	/* Related protocol */
    
    /* �����˶�relatedЭ��ķ������� */
    #ifdef ATP_SUPPORT_PRTT_RPROTOCOL
    u_int16_t rproto;
    #endif
    
	struct ipt_trigger_ports ports;
};

#endif /*_IPT_TRIGGER_H_target*/
