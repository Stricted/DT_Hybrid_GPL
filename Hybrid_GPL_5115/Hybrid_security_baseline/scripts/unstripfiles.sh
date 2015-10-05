#!/bin/sh

TARGET_DIR=$1
UNSTRIP_FILES=$2
DIRECTION=$3  # 0 mv to tmp ; 1: mv to target

if [ $DIRECTION = 0 ];then
	mkdir -p $TARGET_DIR/unstriptmp
	mkdir -p $TARGET_DIR/unstriptmp/bin
	mkdir -p $TARGET_DIR/unstriptmp/lib

	for FILE in $UNSTRIP_FILES; do
		for FILE_PATH in `find $TARGET_DIR -name $FILE`; do
			STAGING_STRIPPED=${FILE_PATH##${TARGET_DIR}}			
			mv $FILE_PATH $TARGET_DIR/unstriptmp/$STAGING_STRIPPED
		done
	done	
else
	for FILE in $UNSTRIP_FILES; do
		for FILE_PATH in `find $TARGET_DIR/unstriptmp -name $FILE`; do
			STAGING_STRIPPED=${FILE_PATH##${TARGET_DIR}/unstriptmp}
			mv $FILE_PATH $TARGET_DIR$STAGING_STRIPPED
		done
	done
	rm -rf $TARGET_DIR/unstriptmp
fi