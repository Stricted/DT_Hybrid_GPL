#
# Makefile for USB Network drivers
#

obj-$(CONFIG_USB_CATC)		+= catc.o
obj-$(CONFIG_USB_KAWETH)	+= kaweth.o
obj-$(CONFIG_USB_PEGASUS)	+= pegasus.o
obj-$(CONFIG_USB_RTL8150)	+= rtl8150.o
obj-$(CONFIG_USB_HSO)		+= hso.o
obj-$(CONFIG_USB_NET_AX8817X)	+= asix.o
obj-$(CONFIG_USB_NET_CDCETHER)	+= cdc_ether.o
# /* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 BEGIN */
obj-$(CONFIG_USB_NET_HW_CDC_DRIVER) += hw_cdc_driver.o
# /* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 END> */
obj-$(CONFIG_USB_NET_CDC_EEM)	+= cdc_eem.o
obj-$(CONFIG_USB_NET_DM9601)	+= dm9601.o
obj-$(CONFIG_USB_NET_SMSC95XX)	+= smsc95xx.o
obj-$(CONFIG_USB_NET_GL620A)	+= gl620a.o
obj-$(CONFIG_USB_NET_NET1080)	+= net1080.o
obj-$(CONFIG_USB_NET_PLUSB)	+= plusb.o
obj-$(CONFIG_USB_NET_RNDIS_HOST)	+= rndis_host.o
obj-$(CONFIG_USB_NET_CDC_SUBSET)	+= cdc_subset.o
obj-$(CONFIG_USB_NET_ZAURUS)	+= zaurus.o
obj-$(CONFIG_USB_NET_MCS7830)	+= mcs7830.o
obj-$(CONFIG_USB_NDISTTY)	+= ndistty.o
# /* <DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 BEGIN */
ifeq ($(PRODUCT),b880)
#/* <DTS2013092310511 z00182709 2013/09/24 begin */
else ifeq ($(PRODUCT),b881)
#/* DTS2013092310511 z00182709 2013/09/24 end> */
else
obj-$(CONFIG_USB_QMITTY)	+= qmitty.o

obj-$(CONFIG_USB_USBNET)	+= usbnet.o
endif
# /* DTS2012120501182 implement haisi NDIS dial up z00185914 20121205 END> */
obj-$(CONFIG_USB_NET_RMNET_ETHERNET) +=rmnet_ethernet.o

