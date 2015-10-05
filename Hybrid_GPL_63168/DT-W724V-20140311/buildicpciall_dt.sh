#!/bin/bash
set +x

CURRENTWORKDIR=`pwd`/

if [ -n "$1" ]
then
	VENDORPARAMETER="VENDOR=$1"
else
    VENDORPARAMETER=""
fi

if [ -n "$2" ]
then
	CUSTOMERPARAMETER="CUSTOMER=$2"
else
    CUSTOMERPARAMETER=""
fi

ATPICPCIPARAMETER="ATP_CI=y"

BUILDCODESVNVERSION=`eval scripts/getsvnversion`

BUILDSTARTTIME=`eval date +%Y%m%d%H%M%S`

echo "make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} && make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} mkp dtdtdt"
make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} && make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} mkp

#for cferom and cferam in one block mkp error, should be deleted when the problem is fixed
echo "make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} mkp"
make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} mkp

BUILDENDTIME=`eval date +%Y%m%d%H%M%S`

STOREIMAGEDIR="atp_${BUILDCODESVNVERSION}_${BUILDSTARTTIME}"

if [ -n "$1" ]
then
	STOREIMAGEDIR=${STOREIMAGEDIR}_$1
fi

if [ -n "$2" ]
then
	STOREIMAGEDIR=${STOREIMAGEDIR}_$2
fi

mkdir -p output/images/${STOREIMAGEDIR}

mv output/images/*packet.bin output/images/${STOREIMAGEDIR}
if [ 0 = $? ]
then
	echo "All targets build finished, start time : $BUILDSTARTTIME -> end time : $BUILDENDTIME"
else
	echo -ne "\nError : Failed to get the image files."
fi

exit 0
