#!/bin/sh
set -x
echo "inside endptdrv.sh "
echo $XCHANGE_DIR
echo $XCHANGE_BUILD_APP
TEMP_TARGET=$CDIR/../endptdrv.o
ENDPTDRV_LIBS_SUBDIR=lib
ENDPTDRV_LIBS=endptdrv
echo $CDIR
	( cd	${XCHANGE_DIR}/dslx;		
${XCHANGE_DIR}/dslx/build -a ${XCHANGE_BUILD_APP} -d ${XCHANGE_DSP_APP} \
-t mta ${XCHANGE_BUILD_EXTRA_CFLAGS}
	cp ${XCHANGE_DIR}/dslx/lib/LinuxKernel/${XCHANGE_BUILD_APP}/mta/$ENDPTDRV_LIBS.a ${CDIR}/${ENDPTDRV_LIBS_SUBDIR}/$ENDPTDRV_LIBS.lib;)
	(cd	$CDIR/$ENDPTDRV_LIBS_SUBDIR
		$AR	x $ENDPTDRV_LIBS.lib; )
	cp  $CDIR/$ENDPTDRV_LIBS_SUBDIR/*.o $TEMP_TARGET;

set +x
