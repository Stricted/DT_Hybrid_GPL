#!/bin/sh

FONCTL=/usr/sbin/fonctl
FAILED_OTHER=52

log() {
    log=/var/fon/tmp/example.log
    echo "$*" >> $log
}

fonctl_authorize() {
    $FONCTL authorize \
	mac $CALLING_STATION_ID \
	username "$CALLING_STATION_ID" \
	interiminterval 10 \
	noacct
}

fonctl_login() {
    $FONCTL login \
	mac $CALLING_STATION_ID \
	username "$USER_NAME" \
	password "$PASSWORD" 
}

fonctl_logout() {
    $FONCTL logout \
	mac $CALLING_STATION_ID 
} 

# BEGIN: add for UDK0000287751 FON: The Fon client does not calculate the TR-069 statistics parameter correctly by l00214441 2013.06.28
fon_logfailed() {
# Add Total AF
/bin/foncontrl incfilevalue /var/fon/stats/total/af

# Add Session AF
/bin/foncontrl incfilevalue /var/fon/stats/session/af
}

# END: add for UDK0000287751 FON: The Fon client does not calculate the TR-069 statistics parameter correctly by l00214441 2013.06.28

case "$1" in
    login)
	log Login \
	    mac:$CALLING_STATION_ID \
	    ip:$FRAMED_IP_ADDRESS \
	    username:$USER_NAME \
	    password:$PASSWORD

	/usr/sbin/chillout-client redir "login $FRAMED_IP_ADDRESS $USER_NAME $PASSWORD" error
	retvalue=$?
	if [ $retvalue != 0 ]; then
		fon_logfailed
		exit 52
	else
		exit 0
	fi
	# Example error
	# echo "Too many users currently at this location"
	# exit $FAILED_OTHER
	# exit 0
	;;

    dhcp-lease)
	log DHCP Lease \
	    mac:$CALLING_STATION_ID \
	    ip:$FRAMED_IP_ADDRESS
	/usr/sbin/chillout-client redir "dhcp lease $CALLING_STATION_ID $FRAMED_IP_ADDRESS"
	;;

    dhcp-release)
	log DHCP Release \
	    mac:$CALLING_STATION_ID \
	    ip:$FRAMED_IP_ADDRESS
	/usr/sbin/chillout-client redir "dhcp release $CALLING_STATION_ID $FRAMED_IP_ADDRESS"
	;;

    session-start)
	log Session Start \
	    mac:$CALLING_STATION_ID \
	    ip:$FRAMED_IP_ADDRESS \
	    username:$USER_NAME
	;;

    session-update)
	log Session Update \
	    mac:$CALLING_STATION_ID \
	    ip:$FRAMED_IP_ADDRESS \
	    username:$USER_NAME \
	    duration:$SESSION_TIME \
	    idle:$IDLE_TIME \
	    in:$INPUT_OCTETS \
	    out:$OUTPUT_OCTETS
	;;

    session-stop)
	log Session Stop \
	    mac:$CALLING_STATION_ID \
	    ip:$FRAMED_IP_ADDRESS \
	    username:$USER_NAME \
	    duration:$SESSION_TIME \
	    in:$INPUT_OCTETS \
	    out:$OUTPUT_OCTETS
	/usr/sbin/chillout-client redir "logout $FRAMED_IP_ADDRESS $USER_NAME whatif"
	;;

    fon-startup)
	log FON Startup
	;;

    fon-shutdown)
	log FON Shutdown
	;;
esac
