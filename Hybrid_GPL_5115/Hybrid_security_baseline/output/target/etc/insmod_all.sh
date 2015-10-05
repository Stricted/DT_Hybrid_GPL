# /etc/profile: system-wide .profile file for the Bourne shells

PATH=/bin:/sbin:/usr/bin
export PATH
echo "INSMOD START......"
 test -e /lib/extra/bcm_ingqos.ko && insmod /lib/extra/bcm_ingqos.ko
 test -e /lib/extra/bcm_bpm.ko && insmod /lib/extra/bcm_bpm.ko
 test -e /lib/extra/pktflow.ko && insmod /lib/extra/pktflow.ko
 test -e /lib/extra/bcmfap.ko && insmod /lib/extra/bcmfap.ko
 test -e /lib/extra/pktcmf.ko && insmod /lib/extra/pktcmf.ko
 test -e /lib/extra/profdrvdd.ko && insmod /lib/extra/profdrvdd.ko
 test -e /lib/extra/bcmxtmcfg.ko && insmod /lib/extra/bcmxtmcfg.ko
 test -e /lib/extra/atmapi.ko && insmod /lib/extra/atmapi.ko
 
 test -e /lib/extra/atmbonding.ko && insmod /lib/extra/atmbonding.ko
 test -e /lib/extra/atmbondingeth.ko && insmod /lib/extra/atmbondingeth.ko
 test -e /lib/extra/adsldd.ko && insmod /lib/extra/adsldd.ko
 test -e /lib/extra/blaa_dd.ko && insmod /lib/extra/blaa_dd.ko
 test -e /lib/extra/bcmprocfs.ko && insmod /lib/extra/bcmprocfs.ko
 test -e /lib/kernel/net/ipv6/ipv6.ko && insmod /lib/kernel/net/ipv6/ipv6.ko
 test -e /lib/kernel/net/atm/br2684.ko && insmod /lib/kernel/net/atm/br2684.ko
 test -e /lib/extra/bcm_enet.ko && insmod /lib/extra/bcm_enet.ko
 test -e /lib/extra/bcmsw.ko && insmod /lib/extra/bcmsw.ko && ifconfig bcmsw up
 test -e /lib//extra/bcm_usb.ko && insmod /lib/extra/bcm_usb.ko
 test -e /lib/extra/wl.ko && insmod /lib/extra/wl.ko
 test -e /lib/extra/rt5390ap.ko && insmod /lib/extra/rt5390ap.ko
#test -e /lib/extra/dspdd.ko && insmod /lib/extra/dspdd.ko
 test -e /lib/extra/endpointdd.ko && insmod /lib/extra/endpointdd.ko
 test -e /lib/extra/bcmvlan.ko && insmod /lib/extra/bcmvlan.ko
 test -e /lib/extra/p8021ag.ko && insmod /lib/extra/p8021ag.ko

echo "INSMOD Done"


