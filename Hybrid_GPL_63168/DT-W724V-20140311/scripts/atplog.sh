#!/bin/bash
# ----------------------------------------
# w00200480
#-----------------------------------------

declare -a ids
declare -a vals
declare -a valstrs
declare -a items
declare -a strings

configFile=logitem.txt
logstringFile=package/atp/libraries/private/msg/logutil/src/dt/Hybrid/atplogstring.c
logHeadFile=package/atp/libraries/private/msg/logutil/inc/atplogdef.h

itemIdx=0

while read -r line
do
    str=$line
    if echo $str | grep "^#define ATP"
    then
        # get log id.
        str=${str#*define}
        id=`expr ${str%0x*} | sed 's/^[ \t]*\(.*\)[ \t]*$/\1/'`

        # get value.
        valstr=`expr ${str#*0x} | sed 's/^[ \t]*\(.*\)[ \t]*$/\1/'`
        val=$((16#${valstr}))

        ids[$itemIdx]=$id
        valstrs[$itemIdx]=$valstr
        vals[$itemIdx]=$val

        if grep "$id," $configFile
        then
            newstr=`grep "$id," $configFile`
            newstr=${newstr#*,}

            # get log type
            logtype=${newstr%%,*}
            newstr=${newstr#*,}

            # get log level
            loglevel=${newstr%%,*}

            # get log string
            logstring=${newstr#*,}

            items[$itemIdx]="    LOG_ELEMENT($id, $logtype, $loglevel),"
            strings[$itemIdx]="    $logstring, /* $id */"
            itemIdx=$(($itemIdx+1))
            continue
        fi

        if grep "LOG_ELEMENT($id," $logstringFile
        then
            items[$itemIdx]=`grep "LOG_ELEMENT($id," $logstringFile`
        else
            continue
        fi

        if grep "/\* $id \*/" $logstringFile
        then
            strings[$itemIdx]=`grep "/\* $id \*/" $logstringFile`
        else
            echo "Error...................can not find the log string ..$id"
            continue
        fi

        itemIdx=$(($itemIdx+1))
    fi
done < $logHeadFile

length=${#ids[@]}
arrayAsc=(`echo ${vals[@]}|tr ' ' '\n'|sort -n`)

WRITE_STRING="/*
 *  Product Language
 */
const VOS_UINT32 g_apcProductLanguages[] = {
    ATP_LOG_LANG_GERMAN,
};

const VOS_UINT32 g_ulNumOfLanguages = (VOS_UINT32)ATP_ARRAY_LEN(g_apcProductLanguages);

/*
 *  Log Type Definitions
 */
static const ATP_UTIL_STR_IDX_MAP_ST m_astLogTypeStr_ge[] =
{
    {
        \"Kernel\",
        1 << 0
    },
    {
        \"User Level\",
        1 << 1
    },
    {
        \"Daemon\",
        1 << 3
    },
    {
        \"Security\",
        1 << 4
    },
    {
        \"Syslogd\",
        1 << 5
    },
    {
        \"openwifi\",
        1 << 6
    },
    {
        \"misscall\",
        1 << 13
    },
    {
        \"alert\",
        1 << 14
    },
    {
        \"incall\",
        1 << 15
    },
    {
        \"outcall\",
        1 << 16
    },
    {
        \"wan\",
        1 << 17
    },
    {
        \"portforwarding\",
        1 << 18
    },
    {
        \"dsl\",
        1 << 20
    },
    {
        \"ppp\",
        1 << 21
    },
    {
        \"firewall\",
        1 << 22
    },
    {
        \"voice\",
        1 << 23
    },
    {
        NULL,
        0
    },
};

/*
 *  Log Type Definitions
 */
static const VOS_CHAR *m_apcLogLevelStr_ge[] =
{
    \"Emergency\",
    \"Alert\",
    \"Critical\",
    \"Error\",
    \"Warning\",
    \"Notice\",
    \"Info\",
    \"Debug\"
};

static const ATP_UTIL_STR_IDX_MAP_ST *m_apcLogTypeArrays[] =
{
    m_astLogTypeStr_ge,
};

static const VOS_CHAR **m_apcLogLevelArrays[] =
{
    m_apcLogLevelStr_ge,
};

const ATP_LOG_LOGITEM_INFO_ST g_pstLogItems[] =
{"

echo "$WRITE_STRING" > $logstringFile

for ((i=0; i<$length; i++))
do
    for ((j=0; j<$length; j++))
    do
        if [ ${vals[$i]} -eq ${arrayAsc[$j]} ]; then
            echo "${items[$i]}" >> $logstringFile
        fi
    done
done

WRITE_STRING="    LOG_ELEMENT(0, 0, 0)
};

/*
 * Log String Definitions
 */
static const VOS_CHAR *m_apcLogStr_GE[] =
{"

echo "$WRITE_STRING" >> $logstringFile

for ((i=0; i<$length; i++))
do
    for ((j=0; j<$length; j++))
    do
        if [ ${vals[$i]} -eq ${arrayAsc[$j]} ]; then
            echo "${strings[$i]}" >> $logstringFile
        fi
    done
done

WRITE_STRING="};

static const VOS_CHAR **m_apcLogStrArrays[] =
{
    m_apcLogStr_GE,
};"

echo "$WRITE_STRING" >> $logstringFile
