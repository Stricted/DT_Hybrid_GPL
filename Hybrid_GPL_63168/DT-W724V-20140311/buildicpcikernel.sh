#!/bin/bash
set +x


BUILDSTARTTIME=`eval date +%Y_%m_%d_%H_%M_%S`


make VENDOR=W723v CUSTOMER=dt icpcipretreatment icpcikernel


BUILDENDTIME=`eval date +%Y_%m_%d_%H_%M_%S`


echo "All kernel build finished, start time : $BUILDSTARTTIME -> end time : $BUILDENDTIME"


exit 0



#start check the arguments ------------------------------------------------------------------------------

if ! [ -n "$1" ]
then
    echo "Please enter the username, config file path, chip name, vendor name and target names";
    exit -1
fi

if ! [ -n "$2" ]
then
    echo "Please enter the config file path, chip name, vendor name and target names";
    exit -2
fi


if ! [ -n "$3" ]
then
    echo "Please enter the chip name, vendor name and target names";
    exit -3
fi

if ! [ -n "$4" ]
then
    echo "Please enter the vendor name and target names";
    exit -4
fi

if ! [ -n "$5" ]
then
    echo "Please enter the target names";
    exit -5
fi

if [ -n "$6" ]
then
    echo "Too many arguments begin with $6";
    exit -6
fi

#end check the arguments --------------------------------------------------------------------------------



#start set parameters with arguments --------------------------------------------------------------------

USERNAME=$1
CFGPATH=$2/
CHIPNAME=$3
VENDORNAME=$4
TARGETNAMES=$5

CURRENTDIR=`eval pwd`/

CHIPMAKFILE=$CHIPNAME.mak
CHIPMAKFOLDER="project/target/make/$CHIPNAME/"
VENDORMAKFILE=$VENDORNAME.mak
VENDORMAKFOLDER="project/target/make/$CHIPNAME/"
VENDORPROFILEFILE=$VENDORNAME.profile
VENDORPROFILEFOLDER="project/target/profile/$CHIPNAME/profile/"
VENDORVENDORFILE=$VENDORNAME.version
VENDORVENDORFOLDER="project/target/profile/$CHIPNAME/version/"

CMODEFFILE="cmo_def.h"
CMODEFFOLDER="project/target/fs/$CHIPNAME/$VENDORNAME/"
TTREEFILE="t_tree.xml"
TTREEFOLDER="project/target/fs/$CHIPNAME/$VENDORNAME/"
SHAREMAPFILE="share.map"
SHAREMAPFOLDER="project/target/fs/$CHIPNAME/$VENDORNAME/fs.src/etc/"

#end set parameters with arguments ----------------------------------------------------------------------



#start check the config files ---------------------------------------------------------------------------
cd "$CURRENTDIR"

#check the mak files
if ! [ -f "$CFGPATH""$CHIPMAKFILE" ]
then
    echo "Can't find the '$CHIPMAKFILE' file";
    exit -11
fi
if ! [ -f "$CFGPATH""$VENDORMAKFILE" ]
then
    echo "Can't find the '$VENDORMAKFILE' file";
    exit -12
fi

#check the cfg files
if ! [ -f "$CFGPATH""$VENDORPROFILEFILE" ]
then
    echo "Can't find the '$VENDORPROFILEFILE' file";
    exit -13
fi
if ! [ -f "$CFGPATH""$VENDORVENDORFILE" ]
then
    echo "Can't find the '$VENDORVENDORFILE' file";
    exit -14
fi

#check the fs files
if ! [ -f "$CFGPATH""$CMODEFFILE" ]
then
    echo "Can't find the '$CMODEFFILE' file";
    exit -15
fi
if ! [ -f "$CFGPATH""$TTREEFILE" ]
then
    echo "Can't find the '$TTREEFILE' file";
    exit -16
fi
if ! [ -f "$CFGPATH""$SHAREMAPFILE" ]
then
    echo "Can't find the '$SHAREMAPFILE' file";
    exit -17
fi

#modify the chip makefile to avoid voice makefile error
sed -e "s/^[ \t]*include[ \t][ \t]*project\/target\/make\/\$(CHIP_NAME)\/make.voice[ \t]*$/#&/g" "$CFGPATH""$CHIPMAKFILE" -i

#end check the config files -----------------------------------------------------------------------------



#start set task id --------------------------------------------------------------------------------------

CURRENTTIME=`eval date +%Y_%m_%d_%H_%M_%S`
CURRENTYEAR=`eval date +%Y`
CURRENTMONTH=`eval date +%m`
TIMENUM=`eval date +%s`
RANDOMNUM=`eval date +%N`
PIDNUM=$$

TASKID="$TIMENUM""$RANDOMNUM""$PIDNUM"

#end set task id ----------------------------------------------------------------------------------------



#start prepare the working directory --------------------------------------------------------------------
cd "$CURRENTDIR"

PROJECTNAME="v1r3c3"
SVNSERVERPATH="http://szxsvn01-tm:6801/svn/TC_ATP_CODE_SVN/ATPCORE_V100R003"
COMPILEWORKDIR="ATPCORE_V100R003/baseline/"
USERWORKDIRKEYWORD="$PROJECTNAME"_"$USERNAME"

let "BUILDTIMES=`eval ls -p | grep / | grep "$USERWORKDIRKEYWORD" | wc -l` + 1"

CURRENTWORKDIR="$USERWORKDIRKEYWORD"_"$CURRENTTIME"_"$TASKID"_"$BUILDTIMES"/

#download the project files
cd "$CURRENTDIR"
mkdir "$CURRENTDIR""$CURRENTWORKDIR"
cd "$CURRENTDIR""$CURRENTWORKDIR"
svn co $SVNSERVERPATH
chmod -R 777 $COMPILEWORKDIR
cd "$CURRENTDIR"

#test whether the vendor compile method exist
if [ -f "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$CHIPMAKFOLDER""$CHIPMAKFILE" ] && [ -f "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$VENDORMAKFOLDER""$VENDORMAKFILE" ]
then
    VENDORCOMILEMETHODFALG=1
else
    VENDORCOMILEMETHODFALG=0
fi

#build the vendor compile method if it is not exist
if [ "$VENDORCOMILEMETHODFALG" -eq 0 ]
then
cd "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR"
make VENDOR=hg553 NEW_CHIP_NAME=$CHIPNAME NEW_PRODUCT_NAME=$VENDORNAME newvendor
cd "$CURRENTDIR"
fi

#copy the configure file to the project
cd "$CURRENTDIR"
cp -rf "$CFGPATH""$CHIPMAKFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$CHIPMAKFOLDER""$CHIPMAKFILE"
cp -rf "$CFGPATH""$VENDORMAKFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$VENDORMAKFOLDER""$VENDORMAKFILE"
cp -rf "$CFGPATH""$VENDORPROFILEFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$VENDORPROFILEFOLDER""$VENDORPROFILEFILE"
cp -rf "$CFGPATH""$VENDORVENDORFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$VENDORVENDORFOLDER""$VENDORVENDORFILE"
cp -rf "$CFGPATH""$CMODEFFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$CMODEFFOLDER""$CMODEFFILE"
cp -rf "$CFGPATH""$TTREEFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$TTREEFOLDER""$TTREEFILE"
cp -rf "$CFGPATH""$SHAREMAPFILE" "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$SHAREMAPFOLDER""$SHAREMAPFILE"

#make the output directory
OUTPUTDIR="$CHIPNAME"_"$VENDORNAME"_"$CURRENTTIME"_"$USERNAME"_"$TASKID"_"$BUILDTIMES"_"output/"
mkdir -p "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$OUTPUTDIR"

#end prepare the working directory ----------------------------------------------------------------------




#start write the build information in the log file ------------------------------------------------------
cd "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$OUTPUTDIR"

READMELOGFILE=readme.txt

#write the log about file header
echo -ne "--------------------------------------------------------------------------------\n" > $READMELOGFILE
echo -ne "|                                                                              |\n" >> $READMELOGFILE
echo -ne "|              Welcome to build the ATP base line core program !               |\n" >> $READMELOGFILE
echo -ne "|                                                                              |\n" >> $READMELOGFILE
echo -ne "--------------------------------------------------------------------------------\n" >> $READMELOGFILE

echo -ne "\n\n" >> $READMELOGFILE

#write the log about start work time
echo -ne "Start working time: $CURRENTTIME\n" >> $READMELOGFILE
echo -ne "\n\n" >> $READMELOGFILE

#write the log about start compile time and host user
echo -ne "Start compile time host computer and compile user info:\n" >> $READMELOGFILE
who am i >> $READMELOGFILE
cat /proc/version >> $READMELOGFILE
echo -ne "\n\n" >> $READMELOGFILE

#write the svn info
echo -ne "Svn infomation:\n" >> $READMELOGFILE
cd "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR"
svn info >> "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$OUTPUTDIR""$READMELOGFILE"
cd "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$OUTPUTDIR"
echo -ne "\n\n" >> $READMELOGFILE

#write the task info
echo -ne "Task infomation:\n" >> $READMELOGFILE
echo -ne "Request user: $USERNAME\n" >> $READMELOGFILE
echo -ne "Request chip name: $CHIPNAME\n" >> $READMELOGFILE
echo -ne "Request vendor name: $VENDORNAME\n" >> $READMELOGFILE
echo -ne "Request target names: $TARGETNAMES\n" >> $READMELOGFILE
echo -ne "\n\n" >> $READMELOGFILE

#write the file list
echo -ne "Target File list:\n" >> $READMELOGFILE
find -type f >> $READMELOGFILE
echo -ne "\n\n" >> $READMELOGFILE

#end write the build information in the log file -------------------------------------------------------



#start build the target --------------------------------------------------------------------------------
cd "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR"

TARGETLIBFOLDER="project/target/atp/lib/$CHIPNAME/"
TARGETFSSRCFOLDER="project/target/fs/$CHIPNAME/"
TARGZOUTPUTFILENAME=${OUTPUTDIR%/}.tar.gz

make VENDOR=$VENDORNAME CHIP_NAME=$CHIPNAME atp_user_cfg_clean atp_user_cfg baseline_core_clean $TARGETNAMES

mkdir -p "$OUTPUTDIR""$CHIPMAKFOLDER"
cp -rf "$CHIPMAKFOLDER""$CHIPMAKFILE" "$OUTPUTDIR""$CHIPMAKFOLDER""$CHIPMAKFILE"

mkdir -p "$OUTPUTDIR""$VENDORMAKFOLDER"
cp -rf "$VENDORMAKFOLDER""$VENDORMAKFILE" "$OUTPUTDIR""$VENDORMAKFOLDER""$VENDORMAKFILE"

mkdir -p "$OUTPUTDIR""$VENDORPROFILEFOLDER"
cp -rf "$VENDORPROFILEFOLDER""$VENDORPROFILEFILE" "$OUTPUTDIR""$VENDORPROFILEFOLDER""$VENDORPROFILEFILE"

mkdir -p "$OUTPUTDIR""$VENDORVENDORFOLDER"
cp -rf "$VENDORVENDORFOLDER""$VENDORVENDORFILE" "$OUTPUTDIR""$VENDORVENDORFOLDER""$VENDORVENDORFILE"

mkdir -p "$OUTPUTDIR""$TARGETLIBFOLDER"
cp -rf "$TARGETLIBFOLDER""$VENDORNAME"/ "$OUTPUTDIR""$TARGETLIBFOLDER"

mkdir -p "$OUTPUTDIR""$TARGETFSSRCFOLDER"
cp -rf "$TARGETFSSRCFOLDER""$VENDORNAME"/ "$OUTPUTDIR""$TARGETFSSRCFOLDER"

find "$OUTPUTDIR" -name ".svn" | xargs rm -rf

tar zcvf "$TARGZOUTPUTFILENAME" "$OUTPUTDIR"

chmod -R 555 "$TARGZOUTPUTFILENAME"

#end build the target ----------------------------------------------------------------------------------



#start write the file list in the log file -------------------------------------------------------------
cd "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR""$OUTPUTDIR"

#write the file list
echo -ne "Target File list:\n" >> $READMELOGFILE
find -type f >> $READMELOGFILE
echo -ne "\n\n" >> $READMELOGFILE

#end write the file list in the log file ---------------------------------------------------------------



#start backup header files -----------------------------------------------------------------------------
cd "$CURRENTDIR""$CURRENTWORKDIR"

HEADERFILESFOLDER="Headerfiles"_"$CURRENTTIME"/
TARGZHEADERSFILENAME=${HEADERFILESFOLDER%/}.tar.gz

mkdir -p "$HEADERFILESFOLDER"

cp -rf "$CURRENTDIR""$CURRENTWORKDIR""$COMPILEWORKDIR" "$HEADERFILESFOLDER"

#remove other files
find "$HEADERFILESFOLDER" -name ".svn" | xargs rm -rf
find "$HEADERFILESFOLDER" -type f ! -name "*.h" | xargs rm -rf
for ((tempindex=1; tempindex <= 10 ; tempindex++))
do
    find "$HEADERFILESFOLDER" -type d -empty | xargs rm -rf
done

tar zcvf "$TARGZHEADERSFILENAME" "$HEADERFILESFOLDER"
mv "$TARGZHEADERSFILENAME" "$COMPILEWORKDIR"

rm -rf "$HEADERFILESFOLDER"

#end backup header files -------------------------------------------------------------------------------




#start record the release ------------------------------------------------------------------------------
cd "$CURRENTDIR"

RELEASERECORDFOLDER="RELEASERECORDS/""$CURRENTYEAR"/"$CURRENTMONTH"/

if ! [ -f "$RELEASERECORDFOLDER" ]
then
mkdir -p "$RELEASERECORDFOLDER"
fi

cp -rf "$CURRENTWORKDIR""$COMPILEWORKDIR""$TARGZOUTPUTFILENAME" "$RELEASERECORDFOLDER"

#end record the release --------------------------------------------------------------------------------



#start display attention -------------------------------------------------------------------------------
cd "$CURRENTDIR"

echo -ne "\nAll jobs have been finished, please get the target file!\n";

#end display attention ---------------------------------------------------------------------------------









