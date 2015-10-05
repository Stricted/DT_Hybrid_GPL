#!/bin/bash
set +x

CURRENTWORKDIR=`pwd`/

if [ -n "$2" ]
then
	VENDORPARAMETER="VENDOR=$2"
else
    VENDORPARAMETER=""
fi

if [ -n "$3" ]
then
	if [ "$3" != "atp" ]
	then
		CUSTOMERPARAMETER="CUSTOMER=$3"
	else
		CUSTOMERPARAMETER=""
	fi
else
    CUSTOMERPARAMETER=""
fi

ATPICPCIDEBUG="prepare-debug-version"
ATPICPCIPARAMETER="ATP_CI=y"

if [ "$1" = "coverage_debug" ]
then
	ATPICPCIPARAMETER="${ATPICPCIPARAMETER} ATP_COVERAGE=y"
fi

if [ ${SCM_INFO_FILE} ]
then
	if [ -f "${SCM_INFO_FILE}.xml" ]
	then
		ATPICPCISVN=`grep -R "svnVersion" ${SCM_INFO_FILE}.xml  | awk -F "\"" '{print $4}'`
	fi
fi

if [ ${ATPICPCISVN} ]
then
	ATPICPCIPARAMETER="${ATPICPCIPARAMETER} ATP_SVN=${ATPICPCISVN}"
	BUILDCODESVNVERSION=${ATPICPCISVN}
else
	BUILDCODESVNVERSION=`eval scripts/getsvnversion`
fi

BUILDSTARTTIME=`eval date +%Y%m%d%H%M%S`

echo "make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER}"

make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} clean  2>/dev/null 1>/dev/null

if [ "$1" = "debug_yes" -o "$1" = "coverage_debug" ]
then
	echo -ne "\ncompile debug flag...\n"
	make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} ${ATPICPCIDEBUG}
fi

make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER}
make ${VENDORPARAMETER} ${CUSTOMERPARAMETER} ${ATPICPCIPARAMETER} citools  2>/dev/null 1>/dev/null

tar -cf sysmap.tar output/images/sysmap
mv sysmap.tar output/images
rm -rf output/images/sysmap

BUILDENDTIME=`eval date +%Y%m%d%H%M%S`
HUTAFIMAGEDIR="hutaf"
OUTPUT_DEBUG_FOLDE="output_debug_${BUILDCODESVNVERSION}_${BUILDSTARTTIME}"
OUTPUT_RELEASE_FOLDER="output_release_${BUILDCODESVNVERSION}_${BUILDSTARTTIME}"
OUTPUT_COVERAGE_FOLDER="output_coverage_${BUILDCODESVNVERSION}_${BUILDSTARTTIME}"

if [ -n "$2" ]
then
	OUTPUT_DEBUG_FOLDE=${OUTPUT_DEBUG_FOLDE}_$2
	OUTPUT_RELEASE_FOLDER=${OUTPUT_RELEASE_FOLDER}_$2
	OUTPUT_COVERAGE_FOLDER=${OUTPUT_COVERAGE_FOLDER}_$2
fi

if [ -n "$3" ]
then
	OUTPUT_DEBUG_FOLDE=${OUTPUT_DEBUG_FOLDE}_$3
	OUTPUT_RELEASE_FOLDER=${OUTPUT_RELEASE_FOLDER}_$3
	OUTPUT_COVERAGE_FOLDER=${OUTPUT_COVERAGE_FOLDER}_$3
fi

if [ "$1" = "debug_yes" ]
then
	mkdir ${OUTPUT_DEBUG_FOLDE}
	mv output/images/* ${OUTPUT_DEBUG_FOLDE}
#	find ${OUTPUT_DEBUG_FOLDE} ! -name *main_web\.bin -a ! -name *multicast_with_multicfg* -a ! -name *debug\.bin -a ! -name sysmap.tar -type f | xargs rm -rf
	find ${OUTPUT_DEBUG_FOLDE} -type d -empty | xargs rm -rf
	find ${OUTPUT_DEBUG_FOLDE} -type d -empty | xargs rm -rf
	find ${OUTPUT_DEBUG_FOLDE} -type d -empty | xargs rm -rf
	find ${OUTPUT_DEBUG_FOLDE} -type d -empty | xargs rm -rf
	mv ${OUTPUT_DEBUG_FOLDE} output/images/

	if [ `eval ls output/images/${OUTPUT_DEBUG_FOLDE}/*main_debug.bin | grep -v "multicast_with_multicfg" | wc -l` = 1 ]
	then
		mkdir -p output/images/${HUTAFIMAGEDIR}
		ls output/images/${OUTPUT_DEBUG_FOLDE}/*main_debug.bin | grep -v "multicast_with_multicfg" | xargs -i cp {} output/images/${HUTAFIMAGEDIR}/main.bin
	fi
fi

if [ "$1" = "debug_no" ]
then
	find output/images -type f ! -name Speedport*\.bin -a ! -name sysmap.tar -a ! -name *.w -a ! -name Speedport*\.ini -a ! -name *.mbn -a ! -name 8bits*\.txt | xargs -i rm -rf {}
	find output/images -type f -name "*_multicast_not_restore_*" -o -name "*_multicast_restore_*" -o -name "*multicast_with_multicfg_main.*" -o -name "*nolte.bin" -o -name "*PartitionTable.bin" | xargs -i rm -rf {}
	find output/images -type d -empty | xargs rm -rf
	
	mkdir -p output/images/${OUTPUT_RELEASE_FOLDER}
	mv output/images/*pack.bin output/images/${OUTPUT_RELEASE_FOLDER}
	
	mkdir -p output/images/${HUTAFIMAGEDIR}
	cp output/images/*.bin output/images/${HUTAFIMAGEDIR}/main.bin
	mv output/images/*.bin output/images/${OUTPUT_RELEASE_FOLDER}
	mv output/images/*.ini output/images/${OUTPUT_RELEASE_FOLDER}
	mv output/images/*.w output/images/${OUTPUT_RELEASE_FOLDER}
	mv output/images/*.mbn output/images/${OUTPUT_RELEASE_FOLDER}
	mv output/images/8bits_ECC_checksum.* output/images/${OUTPUT_RELEASE_FOLDER}/4bits_ECC_checksum.txt
	
fi

if [ "$1" = "coverage_debug" ]
then
	mkdir ${OUTPUT_COVERAGE_FOLDER}
	mv output/images/* ${OUTPUT_COVERAGE_FOLDER}
	find ${OUTPUT_COVERAGE_FOLDER} ! -name *main_web\.bin -a ! -name *multicast_with_multicfg* -a ! -name *debug\.bin -type f | xargs rm -rf
	find ${OUTPUT_COVERAGE_FOLDER} -type d -empty | xargs rm -rf
	find ${OUTPUT_COVERAGE_FOLDER} -type d -empty | xargs rm -rf
	find ${OUTPUT_COVERAGE_FOLDER} -type d -empty | xargs rm -rf
	find ${OUTPUT_COVERAGE_FOLDER} -type d -empty | xargs rm -rf
	if [ -d lltprj/stubdb/ ]
	then
		rm -rf ./stubdb
		mv lltprj/stubdb/ .
		zip -r stubdb.zip stubdb
		mv stubdb.zip ${OUTPUT_COVERAGE_FOLDER}
	else
		echo "Failed to find the DB about Coverage. "
	fi
	mv ${OUTPUT_COVERAGE_FOLDER} output/images/
fi

echo "All targets build finished, start time : $BUILDSTARTTIME -> end time : $BUILDENDTIME"

exit 0
