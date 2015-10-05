#define RMNET_OK                        0
#define RMNET_ERROR                  1
typedef unsigned int UINT32;
typedef unsigned char   UINT8;


/*WDS进程向USBNET下发获取流量统计信息的命令*/

//#define SIOCDEVPRIVATE	0x89F0	/* to 89FF */
#define SIOCGUSBNETSTATS 0x89F0//(SIOCDEVPRIVATE + 0)

/* USBNET网络接口的流量统计结构，传递给用户层*/
struct usbnet_user_stats
{
    unsigned long	rx_packets;     /* 接收包的个数*/
    unsigned long	tx_packets;     /* 发送包的个数*/
    unsigned long	rx_bytes;       /* 接收包的总字节数*/
    unsigned long	tx_bytes;       /* 发送包的总字节数*/
    unsigned long	rx_errors;      /* 接收错误包的个数*/
    unsigned long	tx_errors;      /*发送错误包的个数*/
    unsigned long	rx_dropped;     /*接收丢包计数*/
    unsigned long	tx_dropped;     /*发送丢包计数*/
    unsigned long	sip_packets;    /*sip包个数*/
    unsigned long	sip_bytes;      /*sip包字节数*/
    unsigned long	voice_packets;  /*voice包个数*/
    unsigned long	voice_bytes;    /*voice包字节数*/
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
