#!/bin/bash
set +x

CURRENTDIR=`eval pwd`

echo ${CURRENTDIR}

echo "" > ./scripts/getsvnversion

if ! [ -f ./package/atp/hosttool/TQE_Linux32.zip ]
then
    exit -1
fi

OUTPUT_GCC_FILENAME="mips-linux-uclibc-gcc"

if [ "$1" = "hg532t" ]
then
    OUTPUT_GCC_FILENAME="mips-linux-gcc"
fi

if [ "$1" = "ws880" ]
then
	OUTPUT_GCC_FILENAME="arm-uclibc-linux-2.6.36-gcc"
	export LD_LIBRARY_PATH=/opt/toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib
fi
if [ "$1" = "Hybrid" ]
then
	OUTPUT_GCC_FILENAME="mips-unknown-linux-uclibc-gcc"
	#export LD_LIBRARY_PATH=/opt/toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib
fi

if ! [ -f ./output/host/usr/bin/${OUTPUT_GCC_FILENAME} ]
then
    ./buildicpciheaders.sh $1 $2
fi

if ! [ -f ./output/coverity ]
then
    mkdir -p ./output/coverity
fi

if ! [ -f ./output/coverity/middledir ]
then
    mkdir -p ./output/coverity/middledir
else
    rm -rf ./output/coverity/middledir/*
fi

if ! [ -f ./output/coverity/TQE/tqebuild.sh -a -f ./output/coverity/TQE/tqeconfig.ini -a -f ./output/coverity/TQE/tqeconfig.sh ]
then
	rm -rf ./output/coverity/TQE
    unzip ./package/atp/hosttool/TQE_Linux32.zip -d ./output/coverity
fi

if ! [ -f ./output/coverity/TQE/tqebuild.sh -a -f ./output/coverity/TQE/tqeconfig.ini -a -f ./output/coverity/TQE/tqeconfig.sh ]
then
	exit -2
fi

chmod -R 755 ./output/coverity/TQE/

#modify the chip makefile to avoid voice makefile error
sed -i -e "s:^intermediatedir=$:&${CURRENTDIR}/output/coverity/middledir:g" ${CURRENTDIR}/output/coverity/TQE/tqeconfig.ini

sed -i -e "s:^scriptfile=$:&${CURRENTDIR}/coveritytarget.sh:g" ${CURRENTDIR}/output/coverity/TQE/tqeconfig.ini

sed -i -e "s:^#gcc=/usr/bin/gcc$:gcc=${CURRENTDIR}/output/host/usr/bin/${OUTPUT_GCC_FILENAME}:g" ${CURRENTDIR}/output/coverity/TQE/tqeconfig.ini

sed -e "s:^Encoding=$:&GB2312:g" ${CURRENTDIR}/output/coverity/TQE/tqeconfig.ini

cd ${CURRENTDIR}/output/coverity/TQE

./tqeconfig.sh

./tqebuild.sh

