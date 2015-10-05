#!/bin/bash
set +x

COMPILETARGETS=" key lgplutil gplutil usbstoragecms usbstorageweb ftpservercms sambacms usbprintercms usbmountcms usbmountcli ftpclientcms atshared datacardweb atserversdk atcmdsdk qmisdk oamcms oamcli timerebootcms bhalapicore bhalapicli bhalapi bootloader ledservicecms sshdcms mirror wlancms wlanbcmconfigcms wlancli cups bftpd ntfs samba sshd busybox "

PARAMETERSARRAY=( $* )
VALIDPARAMETERNUM=3
MINICOMPILETARGETSNUM=1

if [ ${#PARAMETERSARRAY[@]} -ne ${VALIDPARAMETERNUM} ]
then
	echo "The parameters ( ${PARAMETERSARRAY[@]} ) are invalid."
	exit -1
fi

ATPICPCIDEBUG="prepare-debug-version"
ATPICPCIPARAMETER="ATP_CI=y"
VENDORPARAMETER="VENDOR=${PARAMETERSARRAY[1]}"
COMPILETARGETSARRAY=( `echo ${COMPILETARGETS}` )

if [ "${PARAMETERSARRAY[2]}" != "atp" ]
then
CUSTOMERPARAMETER="CUSTOMER=${PARAMETERSARRAY[2]}"
else
CUSTOMERPARAMETER=""
fi


if [ ${#COMPILETARGETSARRAY[@]} -lt ${MINICOMPILETARGETSNUM} ]
then
	echo "The compile targets ( ${COMPILETARGETS} ) are invalid."
	exit -2
fi


make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER}
exit 0


MAKETARGETS=`echo ${COMPILETARGETSARRAY[@]}`
CLEANTARGETS=`echo ${COMPILETARGETSARRAY[@]/%/-clean}`

echo "make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} ${CLEANTARGETS}"
make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} ${CLEANTARGETS}

if [ "${PARAMETERSARRAY[0]}" = "debug_yes" ]
then
	echo -ne "\ncompile debug flag...\n"
	make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIDEBUG}
fi

echo "make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} ${MAKETARGETS}"
make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} ${MAKETARGETS}

exit 0

