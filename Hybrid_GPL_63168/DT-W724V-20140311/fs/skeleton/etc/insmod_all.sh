# /etc/profile: system-wide .profile file for the Bourne shells

PATH=/bin:/sbin:/usr/bin
export PATH
echo "INSMOD START......"
##/* BEGIN: add by l00203771 2012/11/30 for DTS2012081602530 */ 
 insmod /lib/extra/bcm_ingqos.ko
 insmod /lib/extra/bcm_bpm.ko
 insmod /lib/extra/pktflow.ko
 insmod /lib/extra/bcmfap.ko
 insmod /lib/extra/bcmxtmcfg.ko 
 insmod /lib/extra/adsldd.ko
 insmod /lib/extra/bcm_enet.ko
 insmod /lib/extra/nciTMSkmod.ko
##/* BEGIN: modify by y00181549 2013/8/21 for DTS2013082106114 */ 
 insmod /lib/extra/wl.ko
 insmod /lib/kernel/drivers/usb/storage/usb-storage.ko
 insmod /lib/kernel/drivers/usb/class/usblp.ko
 insmod /lib/extra/bcmvlan.ko
 insmod /lib/extra/pwrmngtd.ko
 insmod /lib/extra/endpointdd.ko &
##/* END: modify by y00181549 2013/8/21 for DTS2013082106114 */ 
##/* END: add by l00203771 2012/11/30 for DTS2012081602530 */ 
echo "INSMOD Done"


