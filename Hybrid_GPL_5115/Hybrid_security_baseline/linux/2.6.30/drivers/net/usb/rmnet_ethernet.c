/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : Rmnet_ethernet.c
  生成日期   : 2010年10月30日
  最近修改   :
  功能描述   : 注册rmnet驱动
  函数列表   :
                int usbnet_generic_rmnet_bind(struct usbnet *dev, struct usb_interface *intf)
                static inline int get_ethernet_addr(struct usbnet *dev)
                static int rmnet_bind(struct usbnet *dev, struct usb_interface *intf)
                void usbnet_rmnet_unbind(struct usbnet *dev, struct usb_interface *intf)


******************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/usb/cdc.h>

#include <linux/usb/usbnet.h>
#include "../../usb/core/usb.h"

#define HUAWEI_VENDOR_ID 0x12d1
#define HUAWEI_PRODUCT_B683 0x9401
#define MDM9215_VENDOR_ID			0x05C6
#define HUAWEI_PRODUCT_B890             0x9025

/*add by kf31716 Begin */

/*-------------------------------------------------------------------------
 int usb_rmnet_driver_claim_interface - bind a driver to an interface
 @driver: the driver to be bound
 @iface: the interface to which it will be bound; must be in the
    usb device's active configuration
 @priv: driver data associated with that interface
 -------------------------------------------------------------------------*/
int usb_rmnet_driver_claim_interface(struct usb_driver *driver,
                                     struct usb_interface *iface, void* priv)
{
    struct device *dev = &iface->dev;
    struct usb_device *udev = interface_to_usbdev(iface);
    int retval = 0;
/*< DTS2010112202867  kf31716 1122 驱动与设备已经绑定begin*/
    if (dev->driver)
    {
        printk("the dev->driver has already bind\n");

        // return -EBUSY;
    }

    dev->driver = &driver->drvwrap.driver;
    usb_set_intfdata(iface, priv);
    usb_pm_lock(udev);
    iface->condition = USB_INTERFACE_BOUND;
    mark_active(iface);
    iface->pm_usage_cnt = !(driver->supports_autosuspend);
    usb_pm_unlock(udev);

    /* if interface was already added, bind now; else let
     * the future device_add() bind it, bypassing probe()
     */
#if 0
    if (device_is_registered(dev))
    {
        printk("===%s====%d===\n", __FUNCTION__, __LINE__);
        retval = device_bind_driver(dev);
    }
#endif
/* DTS2010112202867 kf31716 1122 end>*/

    return retval;
}

EXPORT_SYMBOL_GPL(usb_rmnet_driver_claim_interface);
/*add by kf31716 End */

/*-------------------------------------------------------------------------
  probes control interface, claims data interface, collects the bulk
  endpoints, activates data interface (if needed), maybe sets MTU.
  all pure cdc, except for certain firmware workarounds, and knowing
  that rndis uses one different rule.
 -------------------------------------------------------------------------*/
static int usbnet_generic_rmnet_bind(struct usbnet *dev, struct usb_interface *intf)
{
    int status = 0;
    struct usb_driver       *driver = driver_of(intf);

    /* claim data interface and set it up ... with side effects.
     * network traffic can't flow until an altsetting is enabled.
     */
    status = usb_rmnet_driver_claim_interface(driver, intf, dev);

    if (status < 0)
    {
        return status;
    }

    status = usbnet_get_endpoints(dev, intf);

    if (status < 0)
    {
        /* ensure immediate exit from usbnet_disconnect */
        usb_set_intfdata(intf, NULL);
        usb_driver_release_interface(driver, intf);
        return status;
    }

    return 0;
}

static inline int get_ethernet_addr(struct usbnet *dev)
{
    dev->net->dev_addr[0] = 0x00;
    dev->net->dev_addr[1] = 0x1e;
    dev->net->dev_addr[2] = 0x10;
    dev->net->dev_addr[3] = 0x1f;
    dev->net->dev_addr[4] = 0x00;
    dev->net->dev_addr[5] = 0x01;

    return 0;
}

/*-------------------------------------------------------------------------
 函数原型:static int rmnet_bind(struct usbnet *dev, struct usb_interface *intf)
           描述 : NDIS网卡与驱动绑定函数
           输入 :  struct usbnet *dev :网络设备的私有数据结构；
                          struct usb_interface *intf :USB接口；
           输出  :无
       返回值:无
-------------------------------------------------------------------------*/
static int rmnet_bind(struct usbnet *dev, struct usb_interface *intf)
{
    int status;

    status = usbnet_generic_rmnet_bind(dev, intf);
    if (status < 0)
    {
        return status;
    }

    status = get_ethernet_addr(dev);
    if (status < 0)
    {
        usb_set_intfdata(intf, NULL);
        usb_driver_release_interface(driver_of(intf), intf);
        return status;
    }

    /* FIXME cdc-ether has some multicast code too, though it complains
     * in routine cases.  info->ether describes the multicast support.
     * Implement that here, manipulating the cdc filter as needed.
     */
    return 0;
}

/*-------------------------------------------------------------------------
 函数原型:void usbnet_rmnet_unbind(struct usbnet *dev, struct usb_interface *intf)
           描述 : NDIS网卡与驱动断开函数
           输入 :  struct usbnet *dev :网络设备的私有数据结构；
                          struct usb_interface *intf :USB接口；
           输出  :无
       返回值:无
-------------------------------------------------------------------------*/
void rmnet_unbind(struct usbnet *dev, struct usb_interface *intf)
{
    struct usb_driver       *driver = driver_of(intf);

    /* disconnect master --> disconnect slave */
    /* ensure immediate exit from usbnet_disconnect */
    usb_set_intfdata(intf, NULL);
    usb_driver_release_interface(driver, intf);
}

EXPORT_SYMBOL_GPL(rmnet_unbind);

static const struct driver_info rmnet_info =
{
    .description = "RMNET Ethernet Device",
    .flags       = FLAG_ETHER,
    .bind        = rmnet_bind,
    .unbind      = rmnet_unbind,
};
/*---------------------------------------------------------------
USB_DEVICE_MATCH_HUAWEI(3,HUAWEI_VENDOR_ID)这种机制是通过
判断接口编号来识别要接入的接口
----------------------------------------------------------------*/
/* <DTS2010112503937.QMITTY: mayang 2010-11-25 MOD BEGIN */
//#define HUAWEI_PID_140C
static struct usb_device_id products[] =
{
#if 0
    {
        USB_DEVICE(HUAWEI_VENDOR_ID, HUAWEI_PRODUCT_B683),
        .driver_info = (unsigned long) &rmnet_info,
    },
  
    {
        USB_DEVICE_MATCH_HUAWEI(3, 0x12D1, 0x9401, 0xff, 0xff, 0xff),
        .driver_info = (unsigned long) &rmnet_info,
    },
#endif
#if defined(HUAWEI_PID_140C)
    {
        USB_DEVICE_MATCH_HUAWEI(1,0x12d1),   // for datacard E1820
        .driver_info = (unsigned long) &rmnet_info,
    },
#else
    {
        USB_DEVICE_MATCH_HUAWEI(4,MDM9215_VENDOR_ID), // for B890, PID 05C6, NDIS上报端点为4
        //USB_DEVICE_MATCH_HUAWEI(4,0x12d1),
        .driver_info = (unsigned long) &rmnet_info,
    },
#endif
    {},
};
/* DTS2010112503937.QMITTY: mayang 2010-11-25 MOD END> */

MODULE_DEVICE_TABLE(usb, products);
static struct usb_driver rmnet_driver =
{
    .name		= "rmnet",
    .id_table	= products,
    .probe		= usbnet_probe,
    .disconnect = usbnet_disconnect,
    .suspend	= usbnet_suspend,
    .resume		= usbnet_resume,

    //.ioctl = usbnet_ioctl,
};

static int __init rmnet_init(void)
{
    return usb_register(&rmnet_driver);
}

module_init(rmnet_init);

static void __exit rmnet_exit(void)
{
    usb_deregister(&rmnet_driver);
}

module_exit(rmnet_exit);

MODULE_AUTHOR("David Brownell");
MODULE_DESCRIPTION("RMNET driver");
MODULE_LICENSE("GPL");
