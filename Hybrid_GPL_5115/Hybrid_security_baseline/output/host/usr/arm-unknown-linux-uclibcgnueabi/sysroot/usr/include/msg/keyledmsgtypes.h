/*  <DTS2013022804321 wanglong 2012-2-28 begin */
/* ����PCLINT #if unclose ERROR */
#ifndef __ATP_KEYLED_MSGTYPES_H__
#define __ATP_KEYLED_MSGTYPES_H__
/***********************************************************************
��Ȩ��Ϣ : ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.�ļ���   : keyledmsgtypes.h
����     : 
�汾     : 
�������� : 2012-07-25������� : 
�������� : ������⣬LED����  
��Ҫ�����б�:   
�޸���ʷ��¼�б�: <�޸���>        <ʱ��>        <���ⵥ��>       <��Ҫ����>
l00205975           2012/07/27          DTS2012072100113       ��Ӱ�����⼰LED���ƹ��� 
l00205975           2012/08/06          DTS2012071900608       ��LED��[B890]V100R001C994B101(Amss):��ҳ���ϻָ�������ָʾ��û����
*************************************************************************/
#define ATP_CBBID_KEYLED_NAME	"keyled"
typedef enum
{    /* <DTS2013072204135 z81004143 2013/7/22 begin */
    LED_STATE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_KEYLED),    LED_STATE_ALL_ON,    LED_STATE_ALL_OFF,    /* WIFI */    LED_WIFI_ON,    LED_WIFI_OFF,    LED_WIFI_WPS,    /* QMI */    LED_QMI_ON,    LED_QMI_OFF,    LED_QMI_QUIK_BLINK,    LED_QMI_SLOW_BLINK,    /* <DTS2012071900608 l00205975 2012/8/6 begin*/    /* LAN4 ���߲���ָʾ */    LED_LAN4_ON,    LED_LAN4_OFF,    LED_LAN4_SLOW_BLINK,    /* RESET �ָ��������� */    LED_STATE_RESET,    /* DTS2012071900608 l00205975 2012/8/6 end> */    /* <DTS2013061708346 l00162999 20130620 begin */
#ifdef SUPPORT_ATP_B880_CUBE
	LED_NDIS_BLINK,
    LED_NDIS_CONNECTED,
	LED_NDIS_CONNECTION_FAIL,
	LED_NDIS_DISCONNECT,
	LED_NDIS_MANUAL_TIP,
#endif
    /* DTS2013061708346 l00162999 20130620 end> */	
    LED_STATE_INVALID,    /* DTS2013072204135 z81004143 2013/7/22 end> */
}LED_STATE_E;
#endif
/*  DTS2013022804321 wanglong 2012-2-28 end> */
