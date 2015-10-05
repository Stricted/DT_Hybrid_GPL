#!/bin/bash
set +x

BUILDSTARTTIME=`eval date +%Y_%m_%d_%H_%M_%S`

make VENDOR=W723v CUSTOMER=dt icpcipretreatment icpciapplication

BUILDENDTIME=`eval date +%Y_%m_%d_%H_%M_%S`

echo "All application build finished, start time : $BUILDSTARTTIME -> end time : $BUILDENDTIME"

exit 0
