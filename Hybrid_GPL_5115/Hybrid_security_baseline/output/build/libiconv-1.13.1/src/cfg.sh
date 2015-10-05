#!/bin/sh

if [ $# != 2 ]
then
    echo "Usage: cfg.sh host CC"
else
    echo "configure libiconv: " "--host" $1 "CC="$2 "--disable-debug"
    ./configure --host=$1 CC=$2 --disable-debug
fi

