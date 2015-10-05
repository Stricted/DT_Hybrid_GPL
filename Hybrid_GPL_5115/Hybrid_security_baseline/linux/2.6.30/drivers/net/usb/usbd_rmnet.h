#define RMNET_OK                        0
#define RMNET_ERROR                  1
typedef unsigned int UINT32;
typedef unsigned char   UINT8;


/*WDS������USBNET�·���ȡ����ͳ����Ϣ������*/

//#define SIOCDEVPRIVATE	0x89F0	/* to 89FF */
#define SIOCGUSBNETSTATS 0x89F0//(SIOCDEVPRIVATE + 0)

/* USBNET����ӿڵ�����ͳ�ƽṹ�����ݸ��û���*/
struct usbnet_user_stats
{
    unsigned long	rx_packets;     /* ���հ��ĸ���*/
    unsigned long	tx_packets;     /* ���Ͱ��ĸ���*/
    unsigned long	rx_bytes;       /* ���հ������ֽ���*/
    unsigned long	tx_bytes;       /* ���Ͱ������ֽ���*/
    unsigned long	rx_errors;      /* ���մ�����ĸ���*/
    unsigned long	tx_errors;      /*���ʹ�����ĸ���*/
    unsigned long	rx_dropped;     /*���ն�������*/
    unsigned long	tx_dropped;     /*���Ͷ�������*/
    unsigned long	sip_packets;    /*sip������*/
    unsigned long	sip_bytes;      /*sip���ֽ���*/
    unsigned long	voice_packets;  /*voice������*/
    unsigned long	voice_bytes;    /*voice���ֽ���*/
};

typedef struct
{
	//struct usb_endpoint_descriptor *interrupt_in_endpoint;
	//struct usb_endpoint_descriptor *bulk_in_endpoint;
	//struct usb_endpoint_descriptor *bulk_out_endpoint;
	struct usb_host_endpoint *interrupt_in_endpoint;
       struct usb_host_endpoint *bulk_in_endpoint;
       struct usb_host_endpoint *bulk_out_endpoint;
	UINT32 num_interrupt_in;
	UINT32 num_bulk_in;
	UINT32 num_bulk_out;
	struct usb_device *pdev;
#ifdef USBD_FUNC_TEST
	UINT8 interrupt_in_buf[1024];
	UINT8 control_in_buf[1024];
	UINT8 control_out_buf[1024];
	UINT8 bulk_in_buf[1024];
	UINT8 bulk_out_buf[1024];
	UINT8 control_in_setup_packet[16];
	UINT8 control_out_setup_packet[16];
#endif
	UINT8 *p_int_msg;
	UINT8 *p_ctl_in_msg;
	UINT8 *p_ctl_out_msg;
	
}rmnet_ep_str;
