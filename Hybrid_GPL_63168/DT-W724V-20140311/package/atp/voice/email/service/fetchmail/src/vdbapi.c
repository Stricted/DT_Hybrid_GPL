#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "atptypes.h"
#include "atputil.h"
#include "cfmapi.h"
#include "cmsapi.h"
#include "msgapi.h"
#include "voiceutil.h"
#include "voicecfg.h"
#include "voiceapi.h"
#include "voicecms.h"
#include "cmo_def.h"
#include "sqlite3.h"
#include "voicedb.h"
#include "voicepub.h"
#ifndef SUPPORT_DESKTOP
#include "atpflashapi.h"
#endif

#ifdef SUPPORT_DESKTOP
#define VDB_DB_STATUS           "./vdb.status"
#define VDB_DB_PATH             "./vdb.s3db"
#define VDB_SQL_CMD_PATH        "./vdb.sql"
#define VDB_DUMMYDB_PATH        "./dummyvdb.s3db"
#define VDB_SQL_DUMMY_PATH      "./dummyvdb.sql"
#else
#define VDB_DB_STATUS           "/var/vdb.status"
#define VDB_DB_PATH             "/var/vdb.s3db"
#define VDB_SQL_CMD_PATH        "/etc/vdb.sql"
#define VDB_DUMMYDB_PATH        "/var/dummyvdb.s3db"
#define VDB_SQL_DUMMY_PATH      "/etc/dummyvdb.sql"
//#define VDB_SQL_CMD_PATH        "/var/vdb.sql"
#endif

int ATP_VDB_busy_handler_callback(void * pvMyData, int RetryCount)
{

	/* 当前需要delay的时间 */
	static const unsigned char delays[] = { 1, 2, 5, 10, 15, 20, 25, 25,  25,  50,  50, 100 };  /*ms*/
	/* 之前delay的总时间 */
  	static const unsigned char totals[] = { 0, 1, 3,  8, 18, 33, 53, 78, 103, 128, 178, 228 };  /*ms*/

	int delay = 0, prior = 0; 
	
#ifdef ATP_PCLINT
    pvMyData = pvMyData;
#endif

	#define NDELAY (sizeof(delays)/sizeof(delays[0]))
	#define TIMEOUT 2000  /*1000 ms*/


	printf("Datebase is locked! RetryCount = %d \n",RetryCount);

	if(RetryCount < 0)
	{
		return SQLITE_ABORT;
	}

	if( RetryCount < NDELAY )
	{  
    	delay = delays[RetryCount];  
    	prior = totals[RetryCount];  
  	}
	else
  	{  
    	delay = delays[NDELAY-1];  
    	prior = totals[NDELAY-1] + delay*(RetryCount-(NDELAY-1));  
  	}
	
  	if( prior + delay > TIMEOUT )
	{  
    	delay = TIMEOUT - prior;  
    	if( delay<=0 )
		{
			printf("Write vdb failed!!\n");
			
			return 0; /*尝试了一定的次数,还是失败!!*/
		}
  	}

	printf("delay %d ms \n",delay);
	usleep(delay * 1000);

	return 1; /*重新尝试*/
}

#ifndef SUPPORT_DESKTOP
static int VdbWriteBufferToFile(const char *pcFileName, char *pcBuf, unsigned int ulFileLen)
{
    FILE            *pfFile;

    // Open the sql file
	pfFile = fopen(pcFileName, "wb");
	if (NULL == pfFile)
    {
        //printf("Write to file %s with length %x failed.", pcFileName, ulFileLen);
        return -1;
    }

    fwrite((void *)pcBuf, (unsigned int)ulFileLen, 1, pfFile);
    fclose(pfFile);

    return 0;
}
#endif

typedef unsigned short u16;

static char *VdbReadFileToBuffer(const char *pcFileName, unsigned int *pulFileLen, int moreLen)
{
    char            *pcRet;
    FILE            *pfFile;
	unsigned int    ulLen;

    if (NULL != pulFileLen)
    {
        (*pulFileLen) = 0;
    }

    // Open the sql file
	pfFile = fopen(pcFileName, "rb");
	if (NULL == pfFile)
    {
        printf("fopen [%s] fail.\n",pcFileName);
        return NULL;
    }

	// Get file length
    fseek(pfFile, 0, SEEK_END);
    ulLen = (unsigned int)ftell(pfFile);
    fseek(pfFile, 0, SEEK_SET);

	// Malloc memory
    pcRet = malloc(ulLen + 1 + moreLen + moreLen);
	if (NULL == pcRet)
    {        
        fclose(pfFile);
        printf("malloc fail.\n");
        return NULL;
    }
    fread((void *)(pcRet + moreLen), (unsigned int)ulLen, 1, pfFile);
    pcRet[ulLen + moreLen] = '\0';
    fclose(pfFile);

    // Set return len if necessary
    if (NULL != pulFileLen)
    {
        (*pulFileLen) = ulLen;
    }

    return pcRet;
}

#if 0
#define SQL_DEBUG(format, args...)       {printf("\r\n[(DBG)%s:L%d]", __FILE__, __LINE__);printf(format, ##args);}
#define SQL_ERROR(format, args...)       {printf("\r\n[(ERR)%s:L%d]", __FILE__, __LINE__);printf(format, ##args);}
#else
#define SQL_DEBUG(x...)
#define SQL_ERROR(x...)
#endif
static int VdbGetEntryIdCallback(void *pvArg, int argc, char **argv, char **azColName)
{
    VDB_ENTRY_ID        llEntryId;
    VDB_ENTRY_ID        llTryEntryId;
    VDB_ENTRY_ID        *pllTryEntryId;

    if (NULL != argv[0])
    {
        llEntryId = (VDB_ENTRY_ID)atoll(argv[0]);
        pllTryEntryId = (VDB_ENTRY_ID *)pvArg;
		llTryEntryId	= *pllTryEntryId;
		SQL_DEBUG("llTryEntryId = %d, llEntryId = %d", (int)llTryEntryId, (int)llEntryId);
        if (llTryEntryId >= llEntryId)  // If any entry id is less then the tried one, we will try the next
        {
            llTryEntryId	= llEntryId + 1;
			SQL_DEBUG("Next llTryEntryId = %d", (int)llTryEntryId);
        }
		*pllTryEntryId	= llTryEntryId;
    }

    return 0;
}

static int VdbGetRecycledEntryID(void *pvVdbHdl, const char *pcTblName, VDB_ENTRY_ID *pllEntryId)
{
    int         lRet;
    char        *pcSqlCmd;

	if (NULL == pvVdbHdl)
	{
		return ATP_VDB_RET_SQL_ERR;
	}

    pcSqlCmd = sqlite3_mprintf("select id from '%q' order by id asc", pcTblName);
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

	SQL_DEBUG("sqlite3_exec: %s", pcSqlCmd);
    // Start from 1
    (*pllEntryId) = (VDB_ENTRY_ID)1;

	SQL_DEBUG("*pllEntryId = %d", (int)(*pllEntryId));
    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetEntryIdCallback, pllEntryId, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return 0;
}

static char *VdbBuildCallRecordTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId, int flags)
{
    ATP_TAB_VOICE_ALL_CALL_ST       *pstTbl;

    pstTbl = (ATP_TAB_VOICE_ALL_CALL_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, '%q', '%q', '%s', '%q', %d, %d",
                    pstTbl->acNumber, pstTbl->acName, pstTbl->acDateTime,
                    pstTbl->acLineName, pstTbl->lLineID, flags);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_NUMBER_NAME"='%q', " ATP_TAB_VOICE_NAME_NAME"='%q', " \
                           ATP_TAB_VOICE_DATETIME_NAME"='%s', " \
                           ATP_TAB_VOICE_LINENAME_NAME"='%q', " \
                           ATP_TAB_VOICE_LINEID_NAME"=%d, " \
                           ATP_TAB_VOICE_RECORD_FLAG"=%d",
                           pstTbl->acNumber, pstTbl->acName, pstTbl->acDateTime,
                           pstTbl->acLineName, pstTbl->lLineID, flags);
}

static char *VdbBuildMissedCallTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_VOICE_MISSEDCALL_ST     *pstTbl;

    pstTbl = (ATP_TAB_VOICE_MISSEDCALL_ST *)pvTblValue;

    if (pstTbl->bNew)
    {
        return VdbBuildCallRecordTbl(pvHandle, pvTblValue, bIsAdd, pllEntryId, ATP_VOICE_MISSED_UNREAD_CALL);
    }
    else
    {
        return VdbBuildCallRecordTbl(pvHandle, pvTblValue, bIsAdd, pllEntryId, ATP_VOICE_MISSED_CALL);
    }
}

static char *VdbBuildIncomingCallTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    return VdbBuildCallRecordTbl(pvHandle, pvTblValue, bIsAdd, pllEntryId, ATP_VOICE_INCOMING_CALL);
}

static char *VdbBuildOutgoingCallTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    return VdbBuildCallRecordTbl(pvHandle, pvTblValue, bIsAdd, pllEntryId, ATP_VOICE_OUTGOING_CALL);
}

static char *VdbBuildContactTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_VOICE_CONTACT_ST        *pstTbl;
    VDB_ENTRY_ID                    llEntryId;

    pstTbl = (ATP_TAB_VOICE_CONTACT_ST *)pvTblValue;

    if (bIsAdd)
    {
        if (0 != VdbGetRecycledEntryID(pvHandle, ATP_TAB_VOICE_CONTACT_NAME, &llEntryId))
        {
			SQL_DEBUG("sqlite3_mprintf NULL ");
            return sqlite3_mprintf("NULL, '%q', '%q', '%q', %d, '%q', %d, '%q', %d, '%q', %d, %d",
                            pstTbl->acName, pstTbl->acFstName, 
                            pstTbl->stContactNum.acNumber1, pstTbl->stContactNum.ucNumber1_flags,
                            pstTbl->stContactNum.acNumber2, pstTbl->stContactNum.ucNumber2_flags,
                            pstTbl->stContactNum.acNumber3, pstTbl->stContactNum.ucNumber3_flags,
                            pstTbl->stContactNum.acReserved, pstTbl->lMelody, pstTbl->lLineID);
        }
		else
		{
			SQL_DEBUG("sqlite3_mprintf %d ", (int)llEntryId);
            
            return sqlite3_mprintf("%d, '%q', '%q', '%q', %d, '%q', %d, '%q', %d, '%q', %d, %d",
                            (int)llEntryId, pstTbl->acName, pstTbl->acFstName, 
                            pstTbl->stContactNum.acNumber1, pstTbl->stContactNum.ucNumber1_flags,
                            pstTbl->stContactNum.acNumber2, pstTbl->stContactNum.ucNumber2_flags,
                            pstTbl->stContactNum.acNumber3, pstTbl->stContactNum.ucNumber3_flags,
                            pstTbl->stContactNum.acReserved, pstTbl->lMelody, pstTbl->lLineID);
		}
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }

    return sqlite3_mprintf(ATP_TAB_VOICE_NAME_NAME"='%q', " ATP_TAB_VOICE_FSTNAME_NAME"='%q', " \
                           ATP_TAB_VOICE_CON_NUM1_NAME"='%q', " \
                           ATP_TAB_VOICE_CON_NUM1_FLAG_NAME"=%d, " \
                           ATP_TAB_VOICE_CON_NUM2_NAME"='%q', " \
                           ATP_TAB_VOICE_CON_NUM2_FLAG_NAME"=%d, " \
                           ATP_TAB_VOICE_CON_NUM3_NAME"='%q', " \
                           ATP_TAB_VOICE_CON_NUM3_FLAG_NAME"=%d, " \
                           ATP_TAB_VOICE_CON_NUMRESERV_NAME"='%q', " \
                           ATP_TAB_VOICE_MELODY_NAME"=%d, " \
                           ATP_TAB_VOICE_LINEID_NAME"=%d",
                           pstTbl->acName, pstTbl->acFstName,
                           pstTbl->stContactNum.acNumber1,
                           pstTbl->stContactNum.ucNumber1_flags,
                           pstTbl->stContactNum.acNumber2,
                           pstTbl->stContactNum.ucNumber2_flags,
                           pstTbl->stContactNum.acNumber3,
                           pstTbl->stContactNum.ucNumber3_flags,
                           pstTbl->stContactNum.acReserved,
                           pstTbl->lMelody, pstTbl->lLineID);
}

static char *VdbBuildInternalNameListTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_VOICE_INTERNELNAMES_ST  *pstTbl;

    pstTbl = (ATP_TAB_VOICE_INTERNELNAMES_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, '%d', '%q'", pstTbl->lTerminalId, pstTbl->acName);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_NUMBER_NAME"='%d', " ATP_TAB_VOICE_NAME_NAME"='%q'",
                           pstTbl->lTerminalId, pstTbl->acName);
}

static char *VdbBuildDectSettingsTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_VOICE_DECTSETTINGS_ST       *pstTbl;

    pstTbl = (ATP_TAB_VOICE_DECTSETTINGS_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, %d, '%q', '%q', '%q', '%q', '%q', '%q', '%q', '%q'",
                        pstTbl->lFlags, pstTbl->acPinCode, pstTbl->stIPAddr.acAddr,
                        pstTbl->stIPAddr.acDNSServer, pstTbl->stIPAddr.acGateWay,
                        pstTbl->stIPAddr.acSubnetMask, pstTbl->acFirmwareVer,
                        pstTbl->acEepromVer, pstTbl->acHardwareVer);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_RECORD_FLAG "=%d, pin='%q', addr='%q', netmask='%q', " \
                           "gateway='%q', 'dnsserver=%q', 'firmwareVer=%q', 'eepromVer=%q', 'hardwareVer=%q'",
                           pstTbl->lFlags, pstTbl->acPinCode, pstTbl->stIPAddr.acAddr,
                           pstTbl->stIPAddr.acDNSServer, pstTbl->stIPAddr.acGateWay,
                           pstTbl->stIPAddr.acSubnetMask, pstTbl->acFirmwareVer,
                           pstTbl->acEepromVer, pstTbl->acHardwareVer);
}

static char *VdbBuildLineSettingsTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_VOICE_LINESETTING_ST    *pstTbl;
	VDB_ENTRY_ID                    llEntryId;

    pstTbl = (ATP_TAB_VOICE_LINESETTING_ST *)pvTblValue;

    if (bIsAdd)
    {
        if (0 != VdbGetRecycledEntryID(pvHandle, ATP_TAB_VOICE_LINESETTING_NAME, &llEntryId))
        {
			SQL_DEBUG("sqlite3_mprintf NULL ");
	        return sqlite3_mprintf("NULL, '%q', %d, %d, %u, '%q', %d, %d, '%q', %d, %d, %d, '%q', '%q', %d, " \
	                               "'%q', '%q', '%q', %d, %d, '%q', '%q', '%q', %d, '%q', '%q', '%q'",
	                        pstTbl->acName, pstTbl->lLineID,
	                        pstTbl->bUsable,
	                        pstTbl->ulHandsetBitmap, pstTbl->acDialingPrefex, pstTbl->lMelody,
	                        pstTbl->lFpVolume, pstTbl->acBlockedNumber, pstTbl->bMulCallsMode, 
	                        pstTbl->bIntrusionCall, pstTbl->stClir.bActive, pstTbl->stClir.acActiveCode,
	                        pstTbl->stClir.acDeactiveCode, pstTbl->stCFU.bActive, pstTbl->stCFU.acActiveCode,
	                        pstTbl->stCFU.acDeactiveCode, pstTbl->stCFU.acCfNumber, pstTbl->stCFNR.bActive,
	                        pstTbl->stCFNR.lSecCfnr, pstTbl->stCFNR.acActiveCode, pstTbl->stCFNR.acDeactiveCode,
	                        pstTbl->stCFNR.acCfNumber, pstTbl->stCFB.bActive, pstTbl->stCFB.acActiveCode,
	                        pstTbl->stCFB.acDeactiveCode, pstTbl->stCFB.acCfNumber);
        }
		else
		{
			SQL_DEBUG("sqlite3_mprintf %d ", (int)llEntryId);
	        return sqlite3_mprintf("%d, '%q', %d, %d, %u, '%q', %d, %d, '%q', %d, %d, %d, '%q', '%q', %d, " \
	                               "'%q', '%q', '%q', %d, %d, '%q', '%q', '%q', %d, '%q', '%q', '%q'",
	                        (int)llEntryId, pstTbl->acName, pstTbl->lLineID,
	                        pstTbl->bUsable,
	                        pstTbl->ulHandsetBitmap, pstTbl->acDialingPrefex, pstTbl->lMelody,
	                        pstTbl->lFpVolume, pstTbl->acBlockedNumber, pstTbl->bMulCallsMode, 
	                        pstTbl->bIntrusionCall, pstTbl->stClir.bActive, pstTbl->stClir.acActiveCode,
	                        pstTbl->stClir.acDeactiveCode, pstTbl->stCFU.bActive, pstTbl->stCFU.acActiveCode,
	                        pstTbl->stCFU.acDeactiveCode, pstTbl->stCFU.acCfNumber, pstTbl->stCFNR.bActive,
	                        pstTbl->stCFNR.lSecCfnr, pstTbl->stCFNR.acActiveCode, pstTbl->stCFNR.acDeactiveCode,
	                        pstTbl->stCFNR.acCfNumber, pstTbl->stCFB.bActive, pstTbl->stCFB.acActiveCode,
	                        pstTbl->stCFB.acDeactiveCode, pstTbl->stCFB.acCfNumber);
		}
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_NAME_NAME"='%q', " \
                           ATP_TAB_VOICE_LINEID_NAME"=%d, " \
                           ATP_TAB_VOICE_LINE_USABLE"=%d, " ATP_TAB_VOICE_BITMAP_NAME"=%u, " \
                           ATP_TAB_VOICE_DIALPREFIX_NAME"='%q', " ATP_TAB_VOICE_MELODY_NAME"=%d, " \
                           ATP_TAB_VOICE_FPVOLUME_NAME"=%d, " ATP_TAB_VOICE_BLOCKEDNUMBER_NAME"='%q', " \
                           ATP_TAB_VOICE_MULCALLMODE_NAME"=%d, " ATP_TAB_VOICE_INTRUCALL_NAME"=%d, " \
                           ATP_TAB_VOICE_CLIRVALUE_NAME"=%d, " ATP_TAB_VOICE_CLIRACT_NAME"='%q', " \
                           ATP_TAB_VOICE_CLIRDEACT_NAME"='%q', " ATP_TAB_VOICE_CFUVALUE_NAME"=%d, " \
                           ATP_TAB_VOICE_CFUACT_NAME"='%q', " ATP_TAB_VOICE_CFUDEACT_NAME"='%q', " \
                           ATP_TAB_VOICE_CFUNUM_NAME"='%q', " ATP_TAB_VOICE_CFNRVALUE_NAME"=%d, " \
                           ATP_TAB_VOICE_CFNRSEC_NAME"=%d, " ATP_TAB_VOICE_CFNRACT_NAME"='%q', " \
                           ATP_TAB_VOICE_CFNRDEACT_NAME"='%q', " ATP_TAB_VOICE_CFNRNUM_NAME"='%q', " \
                           ATP_TAB_VOICE_CFBVALUE_NAME"=%d, " ATP_TAB_VOICE_CFBACT_NAME"='%q', " \
                           ATP_TAB_VOICE_CFBDEACT_NAME"='%q', " ATP_TAB_VOICE_CFBNUM_NAME"='%q'",
                       pstTbl->acName, pstTbl->lLineID, pstTbl->bUsable,
                       pstTbl->ulHandsetBitmap, pstTbl->acDialingPrefex, pstTbl->lMelody,
                       pstTbl->lFpVolume, pstTbl->acBlockedNumber, pstTbl->bMulCallsMode, 
                       pstTbl->bIntrusionCall, pstTbl->stClir.bActive, pstTbl->stClir.acActiveCode,
                       pstTbl->stClir.acDeactiveCode, pstTbl->stCFU.bActive, pstTbl->stCFU.acActiveCode,
                       pstTbl->stCFU.acDeactiveCode, pstTbl->stCFU.acCfNumber, pstTbl->stCFNR.bActive,
                       pstTbl->stCFNR.lSecCfnr, pstTbl->stCFNR.acActiveCode, pstTbl->stCFNR.acDeactiveCode,
                       pstTbl->stCFNR.acCfNumber, pstTbl->stCFB.bActive, pstTbl->stCFB.acActiveCode,
                       pstTbl->stCFB.acDeactiveCode, pstTbl->stCFB.acCfNumber);
}

static char *VdbBuildLanMapTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_LAN_MAP_ST          *pstTbl;

    pstTbl = (ATP_TAB_LAN_MAP_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, '%q', '%q', %u, %u",
                        pstTbl->acName, pstTbl->acMac,
                        pstTbl->ulFlag, pstTbl->ulSetFlag);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_NAME_NAME"='%q', " \
                           ATP_TAB_LANMAP_MAC_NAME"='%q', " ATP_TAB_LANMAP_FLAG_NAME"=%u, " \
                           ATP_TAB_LANMAP_SET_FLAG_NAME"=%u",
                           pstTbl->acName, pstTbl->acMac,
                           pstTbl->ulFlag, pstTbl->ulSetFlag);
}

static char *VdbBuildMobileTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_MOBILE_ST          *pstTbl;

    pstTbl = (ATP_TAB_MOBILE_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, %d, %u, %u, %d, '%q', '%q', %u, %d, %d, %d, %u",
                        pstTbl->lTerminalId, pstTbl->ulIncomAllFlag,
                        pstTbl->ulOutgoLine, (int)pstTbl->llOnlinePhonebook,
                        pstTbl->acEmaiList, pstTbl->acRSSList,
                        pstTbl->ulCallWaitFlag, pstTbl->lCFUDestPort,
                        pstTbl->lCFBDestPort, pstTbl->lCFNRDestPort,
                        pstTbl->ulHansetType);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_MOBILE_ID"=%d, " ATP_TAB_VOICE_MOBILE_INALL"=%u, " \
                           ATP_TAB_VOICE_MOBILE_OUT"=%u, " ATP_TAB_VOICE_MOBILE_ONPHONE"=%d, " \
                           ATP_TAB_VOICE_MOBILE_EMAIL"='%q', " ATP_TAB_VOICE_MOBILE_RSS"='%q', " \                           
                           ATP_TAB_VOICE_MOBILE_CW"=%u, " ATP_TAB_VOICE_MOBILE_CFU"=%d, " \
                           ATP_TAB_VOICE_MOBILE_CFB"=%d, " ATP_TAB_VOICE_MOBILE_CFNR"=%d, " \
                           ATP_TAB_VOICE_MOBILE_TYPE"=%u",
                           pstTbl->lTerminalId, pstTbl->ulIncomAllFlag,
                           pstTbl->ulOutgoLine, (int)pstTbl->llOnlinePhonebook,
                           pstTbl->acEmaiList, pstTbl->acRSSList,
                           pstTbl->ulCallWaitFlag, pstTbl->lCFUDestPort,
                           pstTbl->lCFBDestPort, pstTbl->lCFNRDestPort,
                           pstTbl->ulHansetType);
}
/*Added by kf39808 2011-06-07 for vdbsql*/
static char *VdbBuildOnlineContact(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
     ATP_TAB_ONLINECONTACT_ST     *pstTbl;
     VDB_ENTRY_ID                 llEntryId;
        
     pstTbl = (ATP_TAB_ONLINECONTACT_ST *)pvTblValue;
     
     if(bIsAdd)
     {
        if (0 != VdbGetRecycledEntryID(pvHandle, ATP_TAB_VOICE_ONLINECONTACT_NAME, &llEntryId))
        {
			SQL_DEBUG("sqlite3_mprintf NULL ");
            return sqlite3_mprintf("NULL, '%q', '%q', '%q'",
                                   pstTbl->acName,pstTbl->acBenName,
                                   pstTbl->acPassword);
        }
		else
		{
			SQL_DEBUG("sqlite3_mprintf %d ", (int)llEntryId);
            return sqlite3_mprintf("%d, '%q', '%q', '%q'",
                                   (int)llEntryId, pstTbl->acName,
                                   pstTbl->acBenName,
                                   pstTbl->acPassword);
		}
     }

     if(NULL != pllEntryId)
     {
     	(*pllEntryId) = pstTbl->llEntryId;
     }

     return sqlite3_mprintf(ATP_TAB_VOICE_NAME_NAME"='%q', " ATP_TAB_VOICE_BENNAME_NAME"='%q', " \
                            ATP_TAB_VOICE_PASSWORD_NAME"='%q'",
                                         pstTbl->acName,pstTbl->acBenName,
                            pstTbl->acPassword);
}
/*End of by kf39808 2011-06-07*/

static char *VdbBuildDectEmailTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_DECTMAIL_ST          *pstTbl;
    VDB_ENTRY_ID                 llEntryId;

    pstTbl = (ATP_TAB_DECTMAIL_ST *)pvTblValue;

    if (bIsAdd)
    {
        if (0 != VdbGetRecycledEntryID(pvHandle, ATP_TAB_VOICE_DECTEMAIL_NAME, &llEntryId))
        {
			SQL_DEBUG("sqlite3_mprintf NULL ");
            return sqlite3_mprintf("NULL, %u, '%q', %u, %u, '%q', %u, '%q', '%q'",
                            pstTbl->ulEnable, pstTbl->acEmailAcount,
                            pstTbl->ulServerType, pstTbl->ulServerConnect,
                            pstTbl->acMailServer, pstTbl->ulServerPort,
                            pstTbl->acUserName, pstTbl->acPassWord);
        }
		else
		{
			SQL_DEBUG("sqlite3_mprintf %d ", (int)llEntryId);
            return sqlite3_mprintf("%d, %u, '%q', %u, %u, '%q', %u, '%q', '%q'",
                            (int)llEntryId, pstTbl->ulEnable, pstTbl->acEmailAcount,
                            pstTbl->ulServerType, pstTbl->ulServerConnect,
                            pstTbl->acMailServer, pstTbl->ulServerPort,
                            pstTbl->acUserName, pstTbl->acPassWord);
		}
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_DECTMAIL_ENABLE"=%u, " \
                           ATP_TAB_VOICE_DECTMAIL_ACCOUNT"='%q', " ATP_TAB_VOICE_DECTMAIL_SVRTYPE"=%u, " \
                           ATP_TAB_VOICE_DECTMAIL_SVRCNT"=%u, " ATP_TAB_VOICE_DECTMAIL_SVRADDR"='%q', " \
                           ATP_TAB_VOICE_DECTMAIL_SVRPORT"=%u, " ATP_TAB_VOICE_DECTMAIL_USRNAME"='%q', " \
                           ATP_TAB_VOICE_DECTMAIL_PASSWORD"='%q'",
                           pstTbl->ulEnable, pstTbl->acEmailAcount,
                           pstTbl->ulServerType, pstTbl->ulServerConnect,
                           pstTbl->acMailServer, pstTbl->ulServerPort,
                           pstTbl->acUserName, pstTbl->acPassWord);
}

static char *VdbBuildDectEmailIntervalTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_DECTMAILINTRVAL_ST          *pstTbl;

    pstTbl = (ATP_TAB_DECTMAILINTRVAL_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, %u",
                        pstTbl->ulIntervalID);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_DECTMAIL_INTERVALID"=%u",
                           pstTbl->ulIntervalID);
}

static char *VdbBuildDectRSSTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_DECTRSS_ST          *pstTbl;
    VDB_ENTRY_ID                llEntryId;

    pstTbl = (ATP_TAB_DECTRSS_ST *)pvTblValue;

    if (bIsAdd)
    {
        
        if (0 != VdbGetRecycledEntryID(pvHandle, ATP_TAB_VOICE_DECTERSS_NAME, &llEntryId))
        {
			SQL_DEBUG("sqlite3_mprintf NULL ");
            return sqlite3_mprintf("NULL, %u, '%q', '%q', %u",
                            pstTbl->ulInstID, pstTbl->acRSSName,
                            pstTbl->acRSSURL, pstTbl->ulEnable);
        }
		else
		{
			SQL_DEBUG("sqlite3_mprintf %d ", (int)llEntryId);
            return sqlite3_mprintf("%d, %u, '%q', '%q', %u",
                            (int)llEntryId, pstTbl->ulInstID, pstTbl->acRSSName,
                            pstTbl->acRSSURL, pstTbl->ulEnable);
		}
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_DECTRSS_INSTID"=%u, " \
                           ATP_TAB_VOICE_DECTRSS_NAME"='%q', "\
                           ATP_TAB_VOICE_DECTRSS_URL"='%q', " \
                           ATP_TAB_VOICE_DECTRSS_INFO"=%u",
                           pstTbl->ulInstID, pstTbl->acRSSName,
                           pstTbl->acRSSURL, pstTbl->ulEnable);
}

static char *VdbBuildDectRSSIntervalTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_DECTRSSINTRVAL_ST          *pstTbl;

    pstTbl = (ATP_TAB_DECTRSSINTRVAL_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL, %u",
                        pstTbl->ulIntervalID);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_DECTRSS_INTERVALID"=%u",
                           pstTbl->ulIntervalID);
}
/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
static char *VdbBuildCalllistTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_CALLLIST_ST          *pstTbl;

    pstTbl = (ATP_TAB_CALLLIST_ST *)pvTblValue;

    if (bIsAdd)
    {
        return sqlite3_mprintf("NULL,'%q'",pstTbl->lphoneNum);
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_VOICE_CALLLIST_PHONENUM"='%q'", pstTbl->lphoneNum);
}
/* END:   Added by zengjun 182276, 2011/5/30 */
static char *VdbBuildEmailSubListTbl(void *pvHandle, void *pvTblValue, int bIsAdd, VDB_ENTRY_ID *pllEntryId)
{
    ATP_TAB_EMAIL_SUB_ST    *pstTbl;
	VDB_ENTRY_ID                    llEntryId;

    pstTbl = (ATP_TAB_EMAIL_SUB_ST *)pvTblValue;

    if (bIsAdd)
    {
        if (0 != VdbGetRecycledEntryID(pvHandle, ATP_TAB_EMAIL_SUBLIST_NAME, &llEntryId))
        {
			SQL_DEBUG("sqlite3_mprintf NULL ");
	        return sqlite3_mprintf("NULL, '%q', %d, '%q', '%q', '%q', '%q'",
	                        pstTbl->acAccount,
	                        pstTbl->lUid,
	                        pstTbl->acDate, 
	                        pstTbl->acEmailAddr,
	                        pstTbl->acSubject,
	                        pstTbl->acText);
        }
		else
		{
			SQL_DEBUG("sqlite3_mprintf %d ", (int)llEntryId);
	        return sqlite3_mprintf("%d, '%q', %d, '%q', '%q', '%q', '%q'",
	                        (int)llEntryId, 
	                        pstTbl->acAccount,
	                        pstTbl->lUid,
	                        pstTbl->acDate, 
	                        pstTbl->acEmailAddr,
	                        pstTbl->acSubject,
	                        pstTbl->acText);
		}
    }

    if (NULL != pllEntryId)
    {
        (*pllEntryId) = pstTbl->llEntryID;
    }
    return sqlite3_mprintf(ATP_TAB_EMAIL_SUBLIST_ACCOUNTID"='%q', " \
                           ATP_TAB_EMAIL_SUBLIST_UID"=%d, " \
                           ATP_TAB_EMAIL_SUBLIST_DATE"='%q', " \
                           ATP_TAB_EMAIL_SUBLIST_EMAILADDR"='%q', " \                           
                           ATP_TAB_EMAIL_SUBLIST_SUBJECT"='%q', " \
                           ATP_TAB_EMAIL_SUBLIST_TEXT"='%q'",
                           pstTbl->lAccountId, pstTbl->lUid, pstTbl->acDate,
                           pstTbl->acEmailAddr, pstTbl->acSubject, pstTbl->acText);
}

static void VdbFillOutInCallRecordTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_IN_ACCEPTCALL_ST       *pstTbl;

    pstTbl = (ATP_TAB_VOICE_IN_ACCEPTCALL_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_IN_ACCEPTCALL_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        snprintf(pstTbl->acNumber, sizeof(pstTbl->acNumber), "%s", argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acName, sizeof(pstTbl->acName), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {
        snprintf(pstTbl->acDateTime, sizeof(pstTbl->acDateTime), "%s", argv[3]);
    }
    if (NULL != argv[4])
    {
        snprintf(pstTbl->acLineName, sizeof(pstTbl->acLineName), "%s", argv[4]);
    }
    if (NULL != argv[5])
    {
        pstTbl->lLineID = (int)atoi(argv[5]);
    }
/*    
    if (NULL != argv[6])
    {
        pstTbl->stLineID.lID = (int)atoi(argv[6]);
    }
*/    
}

static void VdbFillAllCallRecordTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_ALL_CALL_ST       *pstTbl;

    pstTbl = (ATP_TAB_VOICE_ALL_CALL_ST *)pvTblValue;
    VdbFillOutInCallRecordTbl(pvTblValue, argc, argv);
    pstTbl->lCallType = 0;
    if (NULL != argv[6])
    {
        pstTbl->lCallType = (int)atoi(argv[6]);
    }
}

static void VdbFillMissedCallTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_MISSEDCALL_ST     *pstTbl;

    VdbFillOutInCallRecordTbl(pvTblValue, argc, argv);

    pstTbl = (ATP_TAB_VOICE_MISSEDCALL_ST *)pvTblValue;
    pstTbl->bNew = 0;
    if (NULL != argv[6])
    {
        pstTbl->bNew = (int)atoi(argv[6]);
    }
}

static void VdbFillContactTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_CONTACT_ST        *pstTbl;

    pstTbl = (ATP_TAB_VOICE_CONTACT_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_CONTACT_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        snprintf(pstTbl->acName, sizeof(pstTbl->acName), "%s", argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acFstName, sizeof(pstTbl->acFstName), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {        
        snprintf(pstTbl->stContactNum.acNumber1, sizeof(pstTbl->stContactNum.acNumber1), "%s", argv[3]);
    }
    if (NULL != argv[4])
    {
        pstTbl->stContactNum.ucNumber1_flags = (unsigned char)atoi(argv[4]);
    }
    if (NULL != argv[5])
    {
        snprintf(pstTbl->stContactNum.acNumber2, sizeof(pstTbl->stContactNum.acNumber2), "%s", argv[5]);
    }
    if (NULL != argv[6])
    {
        pstTbl->stContactNum.ucNumber2_flags = (unsigned char)atoi(argv[6]);
    }
    if (NULL != argv[7])
    {
        snprintf(pstTbl->stContactNum.acNumber3, sizeof(pstTbl->stContactNum.acNumber3), "%s", argv[7]);
    }    
    if (NULL != argv[8])
    {
        pstTbl->stContactNum.ucNumber3_flags = (unsigned char)atoi(argv[8]);
    }
    if (NULL != argv[9])
    {
        snprintf(pstTbl->stContactNum.acReserved, sizeof(pstTbl->stContactNum.acReserved), "%s", argv[9]);
    }    
    if (NULL != argv[10])
    {
        pstTbl->lMelody = (int)atoi(argv[10]);
    }
    if (NULL != argv[11])
    {
        pstTbl->lLineID = (int)atoi(argv[11]);
    }
}

static void VdbFillContactTblQMR(void *pvTblValue, int argc, char **argv)
{
	ATP_TAB_VOICE_CONTACT_QMR_ST		*pstTbl;

	pstTbl = (ATP_TAB_VOICE_CONTACT_QMR_ST *)pvTblValue;
	memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_CONTACT_QMR_ST));

	if (NULL != argv[0])
	{
		pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
	}
	if (NULL != argv[1])
	{
		snprintf(pstTbl->acName, ATP_TAB_VOICE_MAX_NAME_LEN, "%s", argv[1]);
	}
	if (NULL != argv[2])
	{
		snprintf(pstTbl->acFstName, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[2]);
	}
	if (NULL != argv[3])
	{
		pstTbl->stContctNum.enDefalutNumType = (int)atoi(argv[3]);
	}
	if (NULL != argv[4])
	{
		pstTbl->stContctNum.enOwnNumType = (int)atoi(argv[4]);
	}
	if (NULL != argv[5])
	{
		snprintf(pstTbl->stContctNum.acFixedNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[5]);
	}
	if (NULL != argv[6])
	{
		snprintf(pstTbl->stContctNum.acMobileNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[6]);
	}
	if (NULL != argv[7])
	{
		snprintf(pstTbl->stContctNum.acWorkNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[7]);
	}
	if (NULL != argv[8])
	{
		pstTbl->lMelody = (int)atoi(argv[8]);
	}
	if (NULL != argv[9])
	{
		pstTbl->lLineID = (int)atoi(argv[9]);
	}
}

static void VdbFillInternalNameListTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_INTERNELNAMES_ST  *pstTbl;

    pstTbl = (ATP_TAB_VOICE_INTERNELNAMES_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_INTERNELNAMES_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        //snprintf(pstTbl->acNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[1]);
        pstTbl->lTerminalId = (int)atoi(argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acName, sizeof(pstTbl->acName), "%s", argv[2]);
    }
}

static void VdbFillDectSettingsTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_DECTSETTINGS_ST       *pstTbl;

    pstTbl = (ATP_TAB_VOICE_DECTSETTINGS_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_DECTSETTINGS_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->lFlags = (int)atoi(argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acPinCode, sizeof(pstTbl->acPinCode), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {
        snprintf(pstTbl->stIPAddr.acAddr, sizeof(pstTbl->stIPAddr.acAddr), "%s", argv[3]);
    }
    if (NULL != argv[4])
    {
        snprintf(pstTbl->stIPAddr.acSubnetMask, sizeof(pstTbl->stIPAddr.acSubnetMask), "%s", argv[4]);
    }
    if (NULL != argv[5])
    {
        snprintf(pstTbl->stIPAddr.acGateWay, sizeof(pstTbl->stIPAddr.acGateWay), "%s", argv[5]);
    }
    if (NULL != argv[6])
    {
        snprintf(pstTbl->stIPAddr.acDNSServer, sizeof(pstTbl->stIPAddr.acDNSServer), "%s", argv[6]);
    }
    if (NULL != argv[7])
    {
        snprintf(pstTbl->acFirmwareVer, sizeof(pstTbl->acFirmwareVer), "%s", argv[7]);
    }
    if (NULL != argv[8])
    {
        snprintf(pstTbl->acEepromVer, sizeof(pstTbl->acEepromVer), "%s", argv[8]);
    }
    if (NULL != argv[9])
    {
        snprintf(pstTbl->acHardwareVer, sizeof(pstTbl->acHardwareVer), "%s", argv[9]);
    }
}

static void VdbFillLineSettingsTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_LINESETTING_ST    *pstTbl;

    pstTbl = (ATP_TAB_VOICE_LINESETTING_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_LINESETTING_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        snprintf(pstTbl->acName, sizeof(pstTbl->acName), "%s", argv[1]);
    }
    if (NULL != argv[2])
    {
        pstTbl->lLineID = (int)atoi(argv[2]);
    }
    if (NULL != argv[3])
    {
        pstTbl->bUsable = (int)atoi(argv[3]);
    }
    if (NULL != argv[4])
    {
        pstTbl->ulHandsetBitmap = (int)atoi(argv[4]);
    }
    if (NULL != argv[5])
    {
        snprintf(pstTbl->acDialingPrefex, sizeof(pstTbl->acDialingPrefex), "%s", argv[5]);
    }
    if (NULL != argv[6])
    {
        pstTbl->lMelody = (int)atoi(argv[6]);
    }
    if (NULL != argv[7])
    {
        pstTbl->lFpVolume = (int)atoi(argv[7]);
    }
    if (NULL != argv[8])
    {
        snprintf(pstTbl->acBlockedNumber, sizeof(pstTbl->acBlockedNumber), "%s", argv[8]);
    }
    if (NULL != argv[9])
    {
        pstTbl->bMulCallsMode = (int)atoi(argv[9]);
    }
    if (NULL != argv[10])
    {
        pstTbl->bIntrusionCall = (int)atoi(argv[10]);
    }
    if (NULL != argv[11])
    {
        pstTbl->stClir.bActive = (int)atoi(argv[11]);
    }
    if (NULL != argv[12])
    {
        snprintf(pstTbl->stClir.acActiveCode, sizeof(pstTbl->stClir.acActiveCode), "%s", argv[12]);
    }
    if (NULL != argv[13])
    {
        snprintf(pstTbl->stClir.acDeactiveCode, sizeof(pstTbl->stClir.acDeactiveCode), "%s", argv[13]);
    }
    if (NULL != argv[14])
    {
        pstTbl->stCFU.bActive = (int)atoi(argv[14]);
    }
    if (NULL != argv[15])
    {
        snprintf(pstTbl->stCFU.acActiveCode, sizeof(pstTbl->stCFU.acActiveCode), "%s", argv[15]);
    }
    if (NULL != argv[16])
    {
        snprintf(pstTbl->stCFU.acDeactiveCode, sizeof(pstTbl->stCFU.acDeactiveCode), "%s", argv[16]);
    }
    if (NULL != argv[17])
    {
        snprintf(pstTbl->stCFU.acCfNumber, sizeof(pstTbl->stCFU.acCfNumber), "%s", argv[17]);
    }
    if (NULL != argv[18])
    {
        pstTbl->stCFNR.bActive= (int)atoi(argv[18]);
    }
    if (NULL != argv[19])
    {
        pstTbl->stCFNR.lSecCfnr = (int)atoi(argv[19]);
    }
    if (NULL != argv[20])
    {
        snprintf(pstTbl->stCFNR.acActiveCode, sizeof(pstTbl->stCFNR.acActiveCode), "%s", argv[20]);
    }
    if (NULL != argv[21])
    {
        snprintf(pstTbl->stCFNR.acDeactiveCode, sizeof(pstTbl->stCFNR.acDeactiveCode), "%s", argv[21]);
    }
    if (NULL != argv[22])
    {
        snprintf(pstTbl->stCFNR.acCfNumber, sizeof(pstTbl->stCFNR.acCfNumber), "%s", argv[22]);
    }
    if (NULL != argv[23])
    {
        pstTbl->stCFB.bActive= (int)atoi(argv[23]);
    }
    if (NULL != argv[24])
    {
        snprintf(pstTbl->stCFB.acActiveCode, sizeof(pstTbl->stCFB.acActiveCode), "%s", argv[24]);
    }
    if (NULL != argv[25])
    {
        snprintf(pstTbl->stCFB.acDeactiveCode, sizeof(pstTbl->stCFB.acDeactiveCode), "%s", argv[25]);
    }
    if (NULL != argv[26])
    {
        snprintf(pstTbl->stCFB.acCfNumber, sizeof(pstTbl->stCFB.acCfNumber), "%s", argv[26]);
    }

}
static void VdbFillLineSettingsTblQMR(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_VOICE_LINESETTING_QMR_ST    *pstTbl;

    pstTbl = (ATP_TAB_VOICE_LINESETTING_QMR_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_VOICE_LINESETTING_QMR_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }

    if (NULL != argv[1])
    {
        snprintf(pstTbl->acName, ATP_TAB_VOICE_MAX_NAME_LEN, "%s", argv[1]);
    }

    if (NULL != argv[2])
    {
        pstTbl->lLineID = (int)atoi(argv[2]);
    }

    if (NULL != argv[3])
    {
        pstTbl->ulHandsetBitmap = (int)atoi(argv[3]);
    }

    if (NULL != argv[4])
    {
        snprintf(pstTbl->acDialingPrefex, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[4]);
    }

    if (NULL != argv[5])
    {
        pstTbl->lMelody = (int)atoi(argv[5]);
    }

    if (NULL != argv[6])
    {
        pstTbl->lFpVolume = (int)atoi(argv[6]);
    }

    if (NULL != argv[7])
    {
        snprintf(pstTbl->acBlockedNumber, ATP_TAB_VOICE_MAX_BLOCKEDNUMBER_LEN, "%s", argv[7]);
    }

    if (NULL != argv[8])
    {
        pstTbl->bMulCallsMode = (int)atoi(argv[8]);
    }

    if (NULL != argv[9])
    {
        pstTbl->bIntrusionCall = (int)atoi(argv[9]);
    }

    if (NULL != argv[10])
    {
        pstTbl->stClir.bActive = (int)atoi(argv[10]);
    }

    if (NULL != argv[11])
    {
        snprintf(pstTbl->stClir.acActiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[11]);
    }

    if (NULL != argv[12])
    {
        snprintf(pstTbl->stClir.acDeactiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[12]);
    }

    if (NULL != argv[13])
    {
        pstTbl->stCFU.bActive = (int)atoi(argv[13]);
    }

    if (NULL != argv[14])
    {
        snprintf(pstTbl->stCFU.acActiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[14]);
    }

    if (NULL != argv[15])
    {
        snprintf(pstTbl->stCFU.acDeactiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[15]);
    }

    if (NULL != argv[16])
    {
        snprintf(pstTbl->stCFU.acCfNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[16]);
    }

    if (NULL != argv[17])
    {
        pstTbl->stCFNR.bActive= (int)atoi(argv[17]);
    }

    if (NULL != argv[18])
    {
        pstTbl->stCFNR.lSecCfnr = (int)atoi(argv[18]);
    }

    if (NULL != argv[19])
    {
        snprintf(pstTbl->stCFNR.acActiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[19]);
    }

    if (NULL != argv[20])
    {
        snprintf(pstTbl->stCFNR.acDeactiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[20]);
    }

    if (NULL != argv[21])
    {
        snprintf(pstTbl->stCFNR.acCfNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[21]);
    }

    if (NULL != argv[22])
    {
        pstTbl->stCFB.bActive= (int)atoi(argv[22]);
    }

    if (NULL != argv[23])
    {
        snprintf(pstTbl->stCFB.acActiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[23]);
    }

    if (NULL != argv[24])
    {
        snprintf(pstTbl->stCFB.acDeactiveCode, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[24]);
    }

    if (NULL != argv[25])
    {
        snprintf(pstTbl->stCFB.acCfNumber, ATP_TAB_VOICE_MAX_NUMBER_LEN, "%s", argv[25]);
    }


}


static void VdbFillLanMapTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_LAN_MAP_ST      *pstTbl;

    pstTbl = (ATP_TAB_LAN_MAP_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_LAN_MAP_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        snprintf(pstTbl->acName, sizeof(pstTbl->acName), "%s", argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acMac, sizeof(pstTbl->acMac), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {
        pstTbl->ulFlag = (unsigned int)atoi(argv[3]);
    }
    if (NULL != argv[4])
    {
        pstTbl->ulSetFlag = (unsigned int)atoi(argv[4]);
    }
}
static void VdbFillMobileTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_MOBILE_ST      *pstTbl;

    pstTbl = (ATP_TAB_MOBILE_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_MOBILE_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->lTerminalId = atoll(argv[1]);
    }
    if (NULL != argv[2])
    {
        pstTbl->ulIncomAllFlag = (unsigned int)atoi(argv[2]);
    }
    if (NULL != argv[3])
    {
        pstTbl->ulOutgoLine = (unsigned int)atoi(argv[3]);
    }    
    if (NULL != argv[4])
    {
        pstTbl->llOnlinePhonebook = (VDB_ENTRY_ID)atoi(argv[4]);
    }
    if (NULL != argv[5])
    {
        snprintf(pstTbl->acEmaiList, sizeof(pstTbl->acEmaiList), "%s", argv[5]);
    }
    if (NULL != argv[6])
    {
        snprintf(pstTbl->acRSSList, sizeof(pstTbl->acRSSList), "%s", argv[6]);
    }
    if (NULL != argv[7])
    {
        pstTbl->ulCallWaitFlag = (unsigned int)atoi(argv[7]);
    }
    if (NULL != argv[8])
    {
        pstTbl->lCFUDestPort = atoi(argv[8]);
    }
    if (NULL != argv[9])
    {
        pstTbl->lCFBDestPort = atoi(argv[9]);
    }
     if (NULL != argv[10])
    {
        pstTbl->lCFNRDestPort = atoi(argv[10]);
    }
    if (NULL != argv[11])
    {
        pstTbl->ulHansetType = (unsigned int)atoi(argv[11]);
    }    
}
static void VdbFillMobileTblQMR(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_MOBILE_QMR_ST      *pstTbl;

    pstTbl = (ATP_TAB_MOBILE_QMR_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_MOBILE_QMR_ST));
    
    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->lTerminalId = atoll(argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acIncomList, sizeof(pstTbl->acIncomList), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {
        pstTbl->ulCallWaitFlag = (unsigned int)atoi(argv[3]);
    }
    if (NULL != argv[4])
    {
        pstTbl->ulClirFlag = (unsigned int)atoi(argv[4]);
    }
    if (NULL != argv[5])
    {
        pstTbl->lCFUDestPort = atoi(argv[5]);
    }
    if (NULL != argv[6])
    {
        pstTbl->lCFBDestPort = atoi(argv[6]);
    }
     if (NULL != argv[7])
    {
        pstTbl->lCFNRDestPort = atoi(argv[7]);
    }
    if (NULL != argv[8])
    {
        pstTbl->ulHansetType = (unsigned int)atoi(argv[8]);
    }    
}


static void VdbFillOnlineContact(void *pvTblValue, int argc, char **argv)
{
     ATP_TAB_ONLINECONTACT_ST      *pstTbl;

     pstTbl = (ATP_TAB_ONLINECONTACT_ST *)pvTblValue;
     memset((void *)pstTbl,0,sizeof(ATP_TAB_ONLINECONTACT_ST));

     if(NULL != argv[0])
     {
     	pstTbl->llEntryId = (VDB_ENTRY_ID)atoll(argv[0]);
     }
     if(NULL != argv[1])
     {
     	snprintf(pstTbl->acName, sizeof(pstTbl->acName), "%s", argv[1]);
     }
     if(NULL != argv[2])
     {
     	snprintf(pstTbl->acBenName, sizeof(pstTbl->acBenName), "%s", argv[2]);
     }
     if(NULL != argv[3])
     {
     	snprintf(pstTbl->acPassword, sizeof(pstTbl->acPassword), "%s", argv[3]);
     }
}

static void VdbFillDectEmailTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_DECTMAIL_ST      *pstTbl;

    pstTbl = (ATP_TAB_DECTMAIL_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_DECTMAIL_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->ulEnable= (unsigned int)atoi(argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acEmailAcount, sizeof(pstTbl->acEmailAcount), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {
        pstTbl->ulServerType = (unsigned int)atoi(argv[3]);
    }
    if (NULL != argv[4])
    {
        pstTbl->ulServerConnect = (unsigned int)atoi(argv[4]);
    }
    if (NULL != argv[5])
    {
        snprintf(pstTbl->acMailServer, sizeof(pstTbl->acMailServer), "%s", argv[5]);
    }
    if (NULL != argv[6])
    {
        pstTbl->ulServerPort = (unsigned int)atoi(argv[6]);
    }
     if (NULL != argv[7])
    {
        snprintf(pstTbl->acUserName, sizeof(pstTbl->acUserName), "%s", argv[7]);
    }
    if (NULL != argv[8])
    {
        snprintf(pstTbl->acPassWord, sizeof(pstTbl->acPassWord), "%s", argv[8]);
    }    
}

static void VdbFillDectEmailIntervalTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_DECTMAILINTRVAL_ST      *pstTbl;

    pstTbl = (ATP_TAB_DECTMAILINTRVAL_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_DECTMAILINTRVAL_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->ulIntervalID= (unsigned int)atoi(argv[1]);
    }
}

static void VdbFillDectRSSTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_DECTRSS_ST      *pstTbl;

    pstTbl = (ATP_TAB_DECTRSS_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_DECTRSS_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->ulInstID= (unsigned int)atoi(argv[1]);
    }
    if (NULL != argv[2])
    {
        snprintf(pstTbl->acRSSName, sizeof(pstTbl->acRSSName), "%s", argv[2]);
    }
    if (NULL != argv[3])
    {
        snprintf(pstTbl->acRSSURL, sizeof(pstTbl->acRSSURL), "%s", argv[3]);
    }
    if (NULL != argv[4])
    {
        pstTbl->ulEnable = (unsigned int)atoi(argv[4]);
    }
}

static void VdbFillDectRSSIntervalTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_DECTRSSINTRVAL_ST      *pstTbl;

    pstTbl = (ATP_TAB_DECTRSSINTRVAL_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_DECTRSSINTRVAL_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        pstTbl->ulIntervalID= (unsigned int)atoi(argv[1]);
    }
}
/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
static void VdbFillCalllistTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_CALLLIST_ST *pstTbl;
	pstTbl = (ATP_TAB_CALLLIST_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_CALLLIST_ST));

	if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        snprintf(pstTbl->lphoneNum, sizeof(pstTbl->lphoneNum), "%s", argv[1]);
    }
    
}
static void VdbFillEmailSubListTbl(void *pvTblValue, int argc, char **argv)
{
    ATP_TAB_EMAIL_SUB_ST      *pstTbl;

    pstTbl = (ATP_TAB_EMAIL_SUB_ST *)pvTblValue;
    memset((void *)pstTbl, 0, sizeof(ATP_TAB_EMAIL_SUB_ST));

    if (NULL != argv[0])
    {
        pstTbl->llEntryID = (VDB_ENTRY_ID)atoll(argv[0]);
    }
    if (NULL != argv[1])
    {
        snprintf(pstTbl->acAccount, sizeof(pstTbl->acAccount), "%s", argv[1]);
    }
    if (NULL != argv[2])
    {
        pstTbl->lUid = atoi(argv[2]);
    }
    if (NULL != argv[3])
    {
        snprintf(pstTbl->acDate, sizeof(pstTbl->acDate), "%s", argv[3]);
    }
    if (NULL != argv[4])
    {
        snprintf(pstTbl->acEmailAddr, sizeof(pstTbl->acEmailAddr), "%s", argv[4]);
    }
    if (NULL != argv[5])
    {
        snprintf(pstTbl->acSubject, sizeof(pstTbl->acSubject), "%s", argv[5]);
    }
    if (NULL != argv[6])
    {
        snprintf(pstTbl->acText, sizeof(pstTbl->acText), "%s", argv[6]);
    }
}


#ifndef SUPPORT_DESKTOP
/*MR版本的flag*/
#define VDB_START_TAG_0     0x39
#define VDB_START_TAG_1     0x37
/*QMR版本的flag*/
#define VDB_START_TAGQMR_0  0x30
#define VDB_START_TAGQMR_1  0x37
#endif
static int VdbBrowseMobileSetProc (ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    ATP_VDB_TABLE_QMR_ST     *pstArg;
    ATP_TAB_MOBILE_QMR_ST    *pstTblValue;
	static int ulIdx = 0;

#ifdef ATP_PCLINT
    enTblId = enTblId;
#endif   

    if ((NULL ==pvEntry) || (NULL == pvExtraArg))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
	if (ulIdx >= ATP_TAB_VOICE_8_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg		= (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
	pstTblValue = (ATP_TAB_MOBILE_QMR_ST *)pvEntry;
	memcpy(&(pstArg->astMobileSetting[ulIdx]), pstTblValue, sizeof(ATP_TAB_MOBILE_QMR_ST));
	
    ulIdx++;
	return ATP_VDB_RET_OK;
}

static int VdbBrowseLanmapProc (ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    ATP_VDB_TABLE_QMR_ST     *pstArg;
    ATP_TAB_LAN_MAP_ST       *pstTblValue;
	static int ulIdx = 0;

#ifdef ATP_PCLINT
    enTblId = enTblId;
#endif    

    if ((NULL ==pvEntry) || (NULL == pvExtraArg))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
	if (ulIdx >= ATP_TAB_VOICE_256_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg		= (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
	pstTblValue = (ATP_TAB_LAN_MAP_ST *)pvEntry;
	memcpy(&(pstArg->astLanmap[ulIdx]), pstTblValue, sizeof(ATP_TAB_LAN_MAP_ST));
		
    ulIdx++;
	return ATP_VDB_RET_OK;
}

static int VdbBrowseCallRecProc (ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    ATP_VDB_TABLE_QMR_ST       *pstArg;
    ATP_TAB_VOICE_ALL_CALL_ST  *pstTblValue;
	static int ulIdx = 0;

#ifdef ATP_PCLINT
    enTblId = enTblId;
#endif    

    if ((NULL ==pvEntry) || (NULL == pvExtraArg))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
	if (ulIdx >= ATP_TAB_VOICE_512_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg		= (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
	pstTblValue = (ATP_TAB_VOICE_ALL_CALL_ST *)pvEntry;
	memcpy(&(pstArg->astVoiceCallRec[ulIdx]), pstTblValue, sizeof(ATP_TAB_VOICE_ALL_CALL_ST));
		
    ulIdx++;
	return ATP_VDB_RET_OK;
}

static int VdbBrowseContactProc(ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    ATP_VDB_TABLE_QMR_ST            *pstArg;
	ATP_TAB_VOICE_CONTACT_QMR_ST    *pstTblValue;
	static int ulIdx = 0;
	
#ifdef ATP_PCLINT
		enTblId = enTblId;
#endif

	if ((NULL ==pvEntry) || (NULL == pvExtraArg))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
	if (ulIdx >= ATP_TAB_VOICE_256_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg      = (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
	pstTblValue = (ATP_TAB_VOICE_CONTACT_QMR_ST*)pvEntry;
	memcpy(&(pstArg->astPhoneBook[ulIdx]), pstTblValue, sizeof(ATP_TAB_VOICE_CONTACT_QMR_ST));
	
	ulIdx++;
	return ATP_VDB_RET_OK;
}

static int VdbBrowseDectSettingProc (ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    ATP_VDB_TABLE_QMR_ST           *pstArg;
    ATP_TAB_VOICE_DECTSETTINGS_ST  *pstTblValue;
	static int ulIdx = 0;

#ifdef ATP_PCLINT
    enTblId = enTblId;
#endif

    if ((NULL ==pvEntry) || (NULL == pvExtraArg))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
	if (ulIdx >= ATP_TAB_VOICE_8_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg		= (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
	pstTblValue = (ATP_TAB_VOICE_DECTSETTINGS_ST *)pvEntry;
    memcpy(&(pstArg->astDectSetting[ulIdx]), pstTblValue, sizeof(ATP_TAB_VOICE_DECTSETTINGS_ST));
    	
    ulIdx++;
	return ATP_VDB_RET_OK;
}

static int VdbBrowseInternalNameProc(ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
	ATP_VDB_TABLE_QMR_ST		    *pstArg;
	ATP_TAB_VOICE_INTERNELNAMES_ST  *pstTblValue;
	static int ulIdx = 0;

#ifdef ATP_PCLINT
	enTblId = enTblId;
#endif

	if ((NULL ==pvEntry) || (NULL == pvExtraArg))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
	if (ulIdx >= ATP_TAB_VOICE_8_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg		= (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
	pstTblValue = (ATP_TAB_VOICE_INTERNELNAMES_ST *)pvEntry;
	memcpy(&(pstArg->astInternalName[ulIdx]), pstTblValue, sizeof(ATP_TAB_VOICE_INTERNELNAMES_ST));

	ulIdx++;
	return ATP_VDB_RET_OK;
}

static int VdbBrowseLineSettingProc(ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    ATP_VDB_TABLE_QMR_ST                *pstArg;
	ATP_TAB_VOICE_LINESETTING_QMR_ST    *pstTblValue;
	static int ulIdx = 0;

#ifdef ATP_PCLINT
	enTblId = enTblId;
#endif

	if ((NULL == pvEntry) || (NULL == pvExtraArg))
	{
		return ATP_VDB_RET_PARA_ERR;
	}
	if (ulIdx >= ATP_TAB_VOICE_64_LEN)
	{
	    return ATP_VDB_RET_OK;
	}
	pstArg      = (ATP_VDB_TABLE_QMR_ST *)pvExtraArg;
    pstTblValue = (ATP_TAB_VOICE_LINESETTING_QMR_ST *)pvEntry;

    memcpy(&(pstArg->astLineSetting[ulIdx]), pstTblValue, sizeof(ATP_TAB_VOICE_LINESETTING_QMR_ST));
	
	ulIdx++;
	return ATP_VDB_RET_OK;
}

ATP_VDB_TABLE_QMR_ST * ATP_VDB_ReadQMRTable()
{
    ATP_VDB_TABLE_QMR_ST    *pstVdbTableQMR;
	sqlite3                 *db;
	int                     lRet;
	ATP_TAB_MOBILE_QMR_ST   stMobileSettingQMR;
	ATP_TAB_LAN_MAP_ST      stLanMapQMR;
	ATP_TAB_VOICE_ALL_CALL_ST        stCallRecQMR;
	ATP_TAB_VOICE_CONTACT_QMR_ST     stContactQMR;
	ATP_TAB_VOICE_DECTSETTINGS_ST    stDectSettingQMR;
	ATP_TAB_VOICE_INTERNELNAMES_ST   stInternalNamesQMR;
	ATP_TAB_VOICE_LINESETTING_QMR_ST stLineSetQMR;

    // Try to open vdb
    db = NULL;
    lRet = sqlite3_open(VDB_DB_PATH, &db);
    if (SQLITE_OK != lRet)
    {
        sqlite3_close(db);
        return NULL;
    }

	// set busy_handler
	sqlite3_busy_handler(db,ATP_VDB_busy_handler_callback,NULL);	

	pstVdbTableQMR = (ATP_VDB_TABLE_QMR_ST *)malloc(sizeof(ATP_VDB_TABLE_QMR_ST));
	if (NULL == pstVdbTableQMR)
	{
	    printf("\n malloc QMR VDB Table failed.\n");
		return NULL;
	}
	memset(pstVdbTableQMR, 0, sizeof(ATP_VDB_TABLE_QMR_ST));
    lRet = ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseMobileSetProc, ATP_VDB_TABLE_MOBILE,
                                 (void *)(&stMobileSettingQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);
	
    lRet += ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseLanmapProc, ATP_VDB_TABLE_LAN_MAP,
                                  (void *)(&stLanMapQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);

    lRet += ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseCallRecProc, ATP_VDB_TABLE_ALLCALL,
                                  (void *)(&stCallRecQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);

    lRet += ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseContactProc, ATP_VDB_TABLE_CONTACT,
                                  (void *)(&stContactQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);
	
    lRet += ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseDectSettingProc, ATP_VDB_TABLE_DECT_SYSTEM_SETTINGS,
                                  (void *)(&stDectSettingQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);
	
    lRet += ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseInternalNameProc, ATP_VDB_TABLE_INTERNAL_NAMES,
                                  (void *)(&stInternalNamesQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);
	
    lRet += ATP_VDB_BrowseEntriesQMR((void *)db, VdbBrowseLineSettingProc, ATP_VDB_TABLE_LINE_SETTINGS,
                                  (void *)(&stLineSetQMR), (void *)(pstVdbTableQMR), NULL, 0, 0, NULL);

	if (ATP_VDB_RET_OK != lRet)
    {
        free((void *)pstVdbTableQMR);
		pstVdbTableQMR = NULL;
    }
	return pstVdbTableQMR;
}

static int VdbAddMRMobileSettingProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_MOBILE_ST stMobileSetting;
	
	while (0 != pstVdbMRTable->astMobileSetting[ulIdx].llEntryID)
	{
		memset((void *)(&stMobileSetting), 0, sizeof(stMobileSetting));
		memcpy(&stMobileSetting, &(pstVdbMRTable->astMobileSetting[ulIdx]), sizeof(ATP_TAB_MOBILE_ST));
		
	    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_MOBILE, (void *)(&stMobileSetting));
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_8_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}

static int VdbAddMRLanmapProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_LAN_MAP_ST stLanmapInfo;
	
	while (0 != pstVdbMRTable->astLanmap[ulIdx].llEntryID)
	{
	    memset((void *)(&stLanmapInfo), 0, sizeof(stLanmapInfo));
		memcpy(&stLanmapInfo, &(pstVdbMRTable->astLanmap[ulIdx]), sizeof(ATP_TAB_LAN_MAP_ST));

	    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_LAN_MAP, (void *)(&stLanmapInfo));
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_256_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}

static int VdbAddMRCallRecProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_VOICE_ALL_CALL_ST stCallRecord;
	
	while (0 != pstVdbMRTable->astVoiceCallRec[ulIdx].llEntryID)
	{
	    memset((void *)(&stCallRecord), 0, sizeof(stCallRecord));
		memcpy(&stCallRecord, &(pstVdbMRTable->astVoiceCallRec[ulIdx]), sizeof(ATP_TAB_VOICE_ALL_CALL_ST));
	
		switch((ATP_VOICE_CALLRECORD_TYPE_EN)stCallRecord.lCallType)
		{
			case ATP_VOICE_MISSED_CALL:
			case ATP_VOICE_MISSED_UNREAD_CALL:
				ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_MISSED_CALL, (void *)(&stCallRecord));
				break;
			case ATP_VOICE_OUTGOING_CALL:
				ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_OUTGOING_CALL, (void *)(&stCallRecord));
				break;
			case ATP_VOICE_INCOMING_CALL:
			    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_INCOMING_CALL, (void *)(&stCallRecord));
				break;
			default:
			    break;
		}
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_512_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}

static int VdbAddMRContactProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_VOICE_CONTACT_ST stContact;

	while (0 != pstVdbMRTable->astPhoneBook[ulIdx].llEntryID)
	{
	    memset((void *)(&stContact), 0, sizeof(stContact));
		memcpy(&stContact, &(pstVdbMRTable->astPhoneBook[ulIdx]), sizeof(ATP_TAB_VOICE_CONTACT_ST));
        
	    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_CONTACT, (void *)(&stContact));
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_256_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}

static int VdbAddMRDectSettingProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_VOICE_DECTSETTINGS_ST stDectSetting;
	
	while (0 != pstVdbMRTable->astDectSetting[ulIdx].llEntryID)
	{
	    memset((void *)(&stDectSetting), 0, sizeof(stDectSetting));
		memcpy(&stDectSetting, &(pstVdbMRTable->astDectSetting[ulIdx]), sizeof(ATP_TAB_VOICE_DECTSETTINGS_ST));
		
	    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_DECT_SYSTEM_SETTINGS, (void *)(&stDectSetting));
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_8_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}

static int VdbAddMRInternalNameProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_VOICE_INTERNELNAMES_ST stInternalName;
	while (0 != pstVdbMRTable->astInternalName[ulIdx].llEntryID)
	{
	    memset((void *)(&stInternalName), 0, sizeof(stInternalName));
		memcpy(&stInternalName, &(pstVdbMRTable->astInternalName[ulIdx]), sizeof(ATP_TAB_VOICE_INTERNELNAMES_ST));
		
	    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_INTERNAL_NAMES, (void *)(&stInternalName));
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_8_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}

static int VdbAddMRLineSettingProc(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    int ulIdx = 0;
	ATP_TAB_VOICE_LINESETTING_ST stLineSetting;
	while (0 != pstVdbMRTable->astLineSetting[ulIdx].llEntryID)
	{
	    memset((void *)(&stLineSetting), 0, sizeof(stLineSetting));
		memcpy(&stLineSetting, &(pstVdbMRTable->astLineSetting[ulIdx]), sizeof(ATP_TAB_VOICE_LINESETTING_ST));
		
	    ATP_VDB_AddEntry(pvVdbHdl, ATP_VDB_TABLE_LINE_SETTINGS, (void *)(&stLineSetting));
		ulIdx++;
		if (ulIdx >= ATP_TAB_VOICE_64_LEN)
		{
		    break;
		}
	}
	return ATP_VDB_RET_OK;
}


int ATP_VDB_WriteMRTable(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST *pstVdbMRTable)
{
    if ((NULL == pvVdbHdl) || (NULL == pstVdbMRTable))
    {
        return ATP_VDB_RET_PARA_ERR;
    }

	VdbAddMRMobileSettingProc(pvVdbHdl, pstVdbMRTable);
	VdbAddMRLanmapProc(pvVdbHdl, pstVdbMRTable);
	VdbAddMRCallRecProc(pvVdbHdl, pstVdbMRTable);
	VdbAddMRContactProc(pvVdbHdl, pstVdbMRTable);
	VdbAddMRDectSettingProc(pvVdbHdl, pstVdbMRTable);
	VdbAddMRInternalNameProc(pvVdbHdl, pstVdbMRTable);
	VdbAddMRLineSettingProc(pvVdbHdl, pstVdbMRTable);

    return ATP_VDB_RET_OK;
}

VOS_UINT32 ConvertQMRToMR(ATP_VDB_TABLE_QMR_ST *ptVdbQMRTable, ATP_VDB_TABLE_MR_ST *ptVdbMRTable)
{

  VOS_UINT32 uloop = 0;
  VOS_UINT32 uloop_temp1 = 0;
  VOS_UINT32 uloop_temp2 = 0;

  VOS_CHAR   linename[ATP_TAB_VOICE_64_LEN];
  VOS_UINT32 ulineid;
  VOS_UINT32 ulmask;
  VOS_UINT32 ulBitmap;

  VOS_CHAR   acIncomList[ATP_TAB_VOICE_INCOMLIST_LEN];
  VOS_CHAR*  pcIncomList;

  VOS_CHAR   acline[10];
  VOS_INT32  ulNum = 0;
  VOS_INT32  ulline = 0;
  VOS_INT32  i;
  
  if (NULL == ptVdbQMRTable || NULL == ptVdbMRTable)
  {
      return ATP_VDB_RET_PARA_ERR;
  }
  /*先修改可以直接赋值，不需转换的表格*/
  /* 1:lanmap*/
  while(ptVdbQMRTable->astLanmap[uloop].llEntryID)
  {
     ptVdbMRTable->astLanmap[uloop].llEntryID = ptVdbQMRTable->astLanmap[uloop].llEntryID;
	 snprintf(ptVdbMRTable->astLanmap[uloop].acName, sizeof(ptVdbMRTable->astLanmap[uloop].acName), "%s", ptVdbQMRTable->astLanmap[uloop].acName);
	 snprintf(ptVdbMRTable->astLanmap[uloop].acMac, sizeof(ptVdbMRTable->astLanmap[uloop].acMac), "%s",ptVdbQMRTable->astLanmap[uloop].acMac);
     ptVdbMRTable->astLanmap[uloop].ulFlag = ptVdbQMRTable->astLanmap[uloop].ulFlag;
	 ptVdbMRTable->astLanmap[uloop].ulSetFlag = ptVdbQMRTable->astLanmap[uloop].ulSetFlag;
	 uloop++;
	 if(ATP_TAB_VOICE_256_LEN <= uloop)
	 {
       break;
     }
  }
  /* 2:callrecord*/
  uloop = 0;
  while(ptVdbQMRTable->astVoiceCallRec[uloop].llEntryID)
  {
     ptVdbMRTable->astVoiceCallRec[uloop].llEntryID = ptVdbQMRTable->astVoiceCallRec[uloop].llEntryID;
	 snprintf(ptVdbMRTable->astVoiceCallRec[uloop].acNumber, sizeof(ptVdbMRTable->astVoiceCallRec[uloop].acNumber), "%s", ptVdbQMRTable->astVoiceCallRec[uloop].acNumber);
	 snprintf(ptVdbMRTable->astVoiceCallRec[uloop].acName, sizeof(ptVdbMRTable->astVoiceCallRec[uloop].acName), "%s", ptVdbQMRTable->astVoiceCallRec[uloop].acName);
	 snprintf(ptVdbMRTable->astVoiceCallRec[uloop].acDateTime, sizeof(ptVdbMRTable->astVoiceCallRec[uloop].acDateTime), "%s", ptVdbQMRTable->astVoiceCallRec[uloop].acDateTime);
	 snprintf(ptVdbMRTable->astVoiceCallRec[uloop].acLineName, sizeof(ptVdbMRTable->astVoiceCallRec[uloop].acLineName), "%s", ptVdbQMRTable->astVoiceCallRec[uloop].acLineName);
     ptVdbMRTable->astVoiceCallRec[uloop].lLineID = ptVdbQMRTable->astVoiceCallRec[uloop].lLineID;
	 ptVdbMRTable->astVoiceCallRec[uloop].lCallType = ptVdbQMRTable->astVoiceCallRec[uloop].lCallType;
	 
	 uloop++;
	 if(ATP_TAB_VOICE_512_LEN <= uloop)
	 {
       break;
     }
  }
  /* 3:dectsetting*/
  uloop = 0;
  while(ptVdbQMRTable->astDectSetting[uloop].llEntryID)
  {
     ptVdbMRTable->astDectSetting[uloop].llEntryID = ptVdbQMRTable->astDectSetting[uloop].llEntryID;
	 ptVdbMRTable->astDectSetting[uloop].lFlags = ptVdbQMRTable->astDectSetting[uloop].lFlags;
	 snprintf(ptVdbMRTable->astDectSetting[uloop].acPinCode, sizeof(ptVdbMRTable->astDectSetting[uloop].acPinCode), "%s", ptVdbQMRTable->astDectSetting[uloop].acPinCode);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acAddr, sizeof(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acAddr), "%s", ptVdbQMRTable->astDectSetting[uloop].stIPAddr.acAddr);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acDNSServer, sizeof(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acDNSServer), "%s", ptVdbQMRTable->astDectSetting[uloop].stIPAddr.acDNSServer);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acGateWay, sizeof(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acGateWay), "%s", ptVdbQMRTable->astDectSetting[uloop].stIPAddr.acGateWay);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acSubnetMask, sizeof(ptVdbMRTable->astDectSetting[uloop].stIPAddr.acSubnetMask), "%s", ptVdbQMRTable->astDectSetting[uloop].stIPAddr.acSubnetMask);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].acFirmwareVer, sizeof(ptVdbMRTable->astDectSetting[uloop].acFirmwareVer), "%s", ptVdbQMRTable->astDectSetting[uloop].acFirmwareVer);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].acEepromVer, sizeof(ptVdbMRTable->astDectSetting[uloop].acEepromVer), "%s", ptVdbQMRTable->astDectSetting[uloop].acEepromVer);
	 snprintf(ptVdbMRTable->astDectSetting[uloop].acHardwareVer, sizeof(ptVdbMRTable->astDectSetting[uloop].acHardwareVer), "%s", ptVdbQMRTable->astDectSetting[uloop].acHardwareVer);
	 uloop++;
	 if(ATP_TAB_VOICE_8_LEN <= uloop)
	 {
       break;
     }
  }
  /* 4:internalnames*/
  uloop = 0;
  while(ptVdbQMRTable->astInternalName[uloop].llEntryID)
  {
     ptVdbMRTable->astInternalName[uloop].llEntryID = ptVdbQMRTable->astInternalName[uloop].llEntryID;
	 ptVdbMRTable->astInternalName[uloop].lTerminalId = ptVdbQMRTable->astInternalName[uloop].lTerminalId;
     snprintf(ptVdbMRTable->astInternalName[uloop].acName, sizeof(ptVdbMRTable->astInternalName[uloop].acName), "%s", ptVdbQMRTable->astInternalName[uloop].acName);
	 uloop++;
	 if(ATP_TAB_VOICE_8_LEN <= uloop)
	 {
       break;
     }
  }

 /*对于新加的表，采取初始化默认值，不修改 */

 /*下面修改有变化的表*/
 /* 1:contact*/
  uloop = 0;
  while(ptVdbQMRTable->astPhoneBook[uloop].llEntryID)
  {
     ptVdbMRTable->astPhoneBook[uloop].llEntryID = ptVdbQMRTable->astPhoneBook[uloop].llEntryID;
	 snprintf(ptVdbMRTable->astPhoneBook[uloop].acName, sizeof(ptVdbMRTable->astPhoneBook[uloop].acName), "%s", ptVdbQMRTable->astPhoneBook[uloop].acName);
     snprintf(ptVdbMRTable->astPhoneBook[uloop].acFstName, sizeof(ptVdbMRTable->astPhoneBook[uloop].acFstName), "%s", ptVdbQMRTable->astPhoneBook[uloop].acFstName);
	 
	 snprintf(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acNumber1, sizeof(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acNumber1), "%s", ptVdbQMRTable->astPhoneBook[uloop].stContctNum.acFixedNumber);
	 ptVdbMRTable->astPhoneBook[uloop].stContactNum.ucNumber1_flags = 8;

	 snprintf(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acNumber2, sizeof(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acNumber2), "%s", ptVdbQMRTable->astPhoneBook[uloop].stContctNum.acMobileNumber);
	 ptVdbMRTable->astPhoneBook[uloop].stContactNum.ucNumber2_flags = 4;

	 snprintf(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acNumber3, sizeof(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acNumber3), "%s", ptVdbQMRTable->astPhoneBook[uloop].stContctNum.acWorkNumber);
	 ptVdbMRTable->astPhoneBook[uloop].stContactNum.ucNumber3_flags = 2;

     strcpy(ptVdbMRTable->astPhoneBook[uloop].stContactNum.acReserved,"\0");
	 
	 ptVdbMRTable->astPhoneBook[uloop].lMelody = ptVdbQMRTable->astPhoneBook[uloop].lMelody;
	 ptVdbMRTable->astPhoneBook[uloop].lLineID = ptVdbQMRTable->astPhoneBook[uloop].lLineID;
	 
	 uloop++;
	 if(ATP_TAB_VOICE_256_LEN <= uloop)
	 {
       break;
     }
  }

  /* 2:mobilesetting*/
   uloop = 0;

  while(ptVdbQMRTable->astMobileSetting[uloop].llEntryID)
  {
    ptVdbMRTable->astMobileSetting[uloop].llEntryID = ptVdbQMRTable->astMobileSetting[uloop].llEntryID;
	ptVdbMRTable->astMobileSetting[uloop].lTerminalId = ptVdbQMRTable->astMobileSetting[uloop].lTerminalId;

	/* incomAll赋值，如果为incomList为"all"，则incomAll为1，否则为0 */
	if(strcmp(ptVdbQMRTable->astMobileSetting[uloop].acIncomList,"all") == 0)
	{
       ptVdbMRTable->astMobileSetting[uloop].ulIncomAllFlag = 1;
	}
	else
	{
       ptVdbMRTable->astMobileSetting[uloop].ulIncomAllFlag = 0;
	}
    /* outgoLine赋值，此变量为该子机关联的呼出线路，可以由老版本bitmap得到该值 */
	uloop_temp1 = 0;
	ulmask = 1<<(ptVdbMRTable->astMobileSetting[uloop].lTerminalId-1);
    while(ptVdbQMRTable->astLineSetting[uloop_temp1].llEntryID)
    {
	   ulineid = ptVdbQMRTable->astLineSetting[uloop_temp1].lLineID;
	   ulBitmap = ptVdbQMRTable->astLineSetting[uloop_temp1].ulHandsetBitmap;
	   /*ISDN默认线路不处理*/
	   if((VCE_ISDN_BASE + VCE_ISDN_MAX)== ulineid)
       {
         
	   }
	   /*内部线路不处理*/
	   else if((ulineid>=VCE_FXS_INCALL_BASE)&&(ulineid<VCE_OUTGO_AUTO))
	   {
         
	   }
	   /*其它线路绑定该账号*/
	   else
	   {
		  if(ulBitmap&ulmask)
		  {
			ptVdbMRTable->astMobileSetting[uloop].ulOutgoLine = ulineid;
		  }
	   }
       uloop_temp1++;
	}
	
    /* onlinePhone赋值，默认为0 */
	ptVdbMRTable->astMobileSetting[uloop].llOnlinePhonebook = 0;

	/* emailList赋值，默认为0 */
	ptVdbMRTable->astMobileSetting[uloop].acEmaiList[0] = '\0' ;
	
	/* emailList赋值，默认为0 */
    ptVdbMRTable->astMobileSetting[uloop].acRSSList[0] = '\0' ;
	
    /* 其它值 */
	ptVdbMRTable->astMobileSetting[uloop].ulCallWaitFlag = ptVdbQMRTable->astMobileSetting[uloop].ulCallWaitFlag;
	ptVdbMRTable->astMobileSetting[uloop].lCFUDestPort = ptVdbQMRTable->astMobileSetting[uloop].lCFUDestPort;
    ptVdbMRTable->astMobileSetting[uloop].lCFBDestPort = ptVdbQMRTable->astMobileSetting[uloop].lCFBDestPort;
	ptVdbMRTable->astMobileSetting[uloop].lCFNRDestPort = ptVdbQMRTable->astMobileSetting[uloop].lCFNRDestPort;
	ptVdbMRTable->astMobileSetting[uloop].ulHansetType = ptVdbQMRTable->astMobileSetting[uloop].ulHansetType;
	
	uloop++;
	if(ATP_TAB_VOICE_8_LEN <= uloop)
	{
      break;
    }
	
  }

   /* 3:linesetting */
  uloop = 0;
  uloop_temp2 = 0;
  while(ptVdbQMRTable->astLineSetting[uloop].llEntryID)
  {
     //strcpy(linename,ptVdbQMRTable->astLineSetting[uloop].acName);
	 snprintf(linename, ATP_TAB_VOICE_MAX_NAME_LEN, "%s", ptVdbQMRTable->astLineSetting[uloop].acName);
	 ulineid = ptVdbQMRTable->astLineSetting[uloop].lLineID;

    /*ISDN默认线路和auto线路新版本中没有，则去掉该线路。*/	 
	 if(((VCE_ISDN_BASE + VCE_ISDN_MAX) == ulineid)||(VCE_OUTGO_AUTO == ulineid))
    {
		 uloop++;
	     continue;
	}
	 /* 新版本中内部线路也不保存。*/
	 if((ulineid>=VCE_FXS_INCALL_BASE)&&(ulineid<VCE_OUTGO_AUTO))
    {
		 uloop++;
	     continue;
	}

     ptVdbMRTable->astLineSetting[uloop_temp2].llEntryID = ptVdbQMRTable->astLineSetting[uloop].llEntryID;

	 
     /* name字段赋值，新版本中的线路名字都是以号码命名的，因此要做修改。*/
     if(VCE_PSTN_BASE == ulineid)
     {
       /* 线路为PSTN则先判断有没配置号码，若有，去掉前缀PSTN,没有的话直接赋值 。*/
	   if(strlen(linename) > strlen("PSTN"))
	   {
         strcpy(ptVdbMRTable->astLineSetting[uloop_temp2].acName,ptVdbQMRTable->astLineSetting[uloop].acName + strlen("PSTN"));
	   }
	   else
	   {
         strcpy(ptVdbMRTable->astLineSetting[uloop_temp2].acName,ptVdbQMRTable->astLineSetting[uloop].acName);
	   }	   
	 }
	 else if(VCE_CS_BASE == ulineid)
	 { 
       /* 线路为cs则先判断有没配置号码，若有，去掉前缀cs,没有的话直接赋值 。*/
	   if(strlen(linename) > strlen("CS"))
	   {
         strcpy(ptVdbMRTable->astLineSetting[uloop_temp2].acName,ptVdbQMRTable->astLineSetting[uloop].acName + strlen("CS"));
	   }
	   else
	   {
         strcpy(ptVdbMRTable->astLineSetting[uloop_temp2].acName,ptVdbQMRTable->astLineSetting[uloop].acName);
	   }	   
	 }
	 else if((ulineid >= VCE_VOIP_BASE)&&(ulineid < VCE_ISDN_BASE))
	 { 
       /* 线路为SIP,则去掉前缀SIP。*/
         strcpy(ptVdbMRTable->astLineSetting[uloop_temp2].acName,ptVdbQMRTable->astLineSetting[uloop].acName + strlen("SIP"));	   
	 }
	 else
	 {
       /* 剩下线路为ISDN,则去掉前缀ISDN。*/
	   strcpy(ptVdbMRTable->astLineSetting[uloop_temp2].acName,ptVdbQMRTable->astLineSetting[uloop].acName + strlen("ISDN"));
	 }
	 
     ptVdbMRTable->astLineSetting[uloop_temp2].lLineID = ptVdbQMRTable->astLineSetting[uloop].lLineID;
	 
	  /* bUsable字段赋值，默认为1，CMS启动后更新。*/
	 ptVdbMRTable->astLineSetting[uloop_temp2].bUsable = 1;

	 /* bitmap字段赋值，由老版本的incomList决定绑定了那几个子机。*/
     uloop_temp1 = 0;
	 while(ptVdbQMRTable->astMobileSetting[uloop_temp1].llEntryID)
    {
       strcpy(acIncomList,ptVdbQMRTable->astMobileSetting[uloop_temp1].acIncomList);
       pcIncomList = acIncomList;
	   /* acIncomList为all，代表该子机绑定了所有线路 */
	   if(strcmp(acIncomList,"all") == 0)
	   {          
		  ulmask = 1<<(ptVdbQMRTable->astMobileSetting[uloop_temp1].lTerminalId-1);
		  ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap = ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap|ulmask;
	   }
	   /* acIncomList不为all，则解析acIncomList，看子机绑定本条线路没有 */
	   else
	   {
         /* 解析acIncomList*/
	     while(*pcIncomList)
	     {
            if(*pcIncomList != ',')
            {
                acline[ulNum] = *pcIncomList;
				ulNum++;
			}			
			
            if((*pcIncomList == ',')||(*(pcIncomList+1) == '\0'))
            {
               acline[ulNum] = '\0';
			   /* CS线路 */
               if(strncmp(acline,"CS",2)==0)
               {
                 if(VCE_CS_BASE == ptVdbQMRTable->astLineSetting[uloop].lLineID)
                 {
                   ulmask = 1<<(ptVdbQMRTable->astMobileSetting[uloop_temp1].lTerminalId-1);
				   ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap = ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap|ulmask;
				 }
			   }
			   /* PSTN线路 */
			   else if(strncmp(acline,"PSTN",4)==0)
			   {
                 if(VCE_PSTN_BASE == ptVdbQMRTable->astLineSetting[uloop].lLineID)
                 {
                   ulmask = 1<<(ptVdbQMRTable->astMobileSetting[uloop_temp1].lTerminalId-1);
				   ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap = ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap|ulmask;
				 }
			   }
			   /* ISDN线路 如 ISDN1----> 1 或者 ISDN10----> 10 */
			   else if((5 == strlen(acline))||(6 == strlen(acline)))
			   {
                 /*字符转数字*/
				  ulline = VCE_ISDN_BASE + (VOS_UINT32)atoi(acline+4)-1;
				 if(ulline == ptVdbQMRTable->astLineSetting[uloop].lLineID)
                 {
                   ulmask = 1<<(ptVdbQMRTable->astMobileSetting[uloop_temp1].lTerminalId-1);
				   ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap = ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap|ulmask;
				 }
			   }
			    /* 剩下是SIP线路 */
			   else //if((1 == strlen(acline))||(2 == strlen(acline)))
			   {
                  /*字符串转数字*/
                  ulline = (VOS_UINT32)atoi(acline);
				  if(ulline == ptVdbQMRTable->astLineSetting[uloop].lLineID)
				  {
                    ulmask = 1<<(ptVdbQMRTable->astMobileSetting[uloop_temp1].lTerminalId-1);
					ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap = ptVdbMRTable->astLineSetting[uloop_temp2].ulHandsetBitmap|ulmask;
				  }
			   }			    
				ulNum=0;
				memset(acline, 0, sizeof(acline));
			}
			pcIncomList++;
		 }
	   }
       uloop_temp1++;
	}

	 /* 其它值 */

	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].acDialingPrefex, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].acDialingPrefex), "%s", ptVdbQMRTable->astLineSetting[uloop].acDialingPrefex);
	 ptVdbMRTable->astLineSetting[uloop_temp2].lMelody = ptVdbQMRTable->astLineSetting[uloop].lMelody;
	 ptVdbMRTable->astLineSetting[uloop_temp2].lFpVolume = ptVdbQMRTable->astLineSetting[uloop].lFpVolume;
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].acBlockedNumber, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].acBlockedNumber), "%s", ptVdbQMRTable->astLineSetting[uloop].acBlockedNumber);
	 ptVdbMRTable->astLineSetting[uloop_temp2].bMulCallsMode = ptVdbQMRTable->astLineSetting[uloop].bMulCallsMode;
	 ptVdbMRTable->astLineSetting[uloop_temp2].bIntrusionCall = ptVdbQMRTable->astLineSetting[uloop].bIntrusionCall;

	 ptVdbMRTable->astLineSetting[uloop_temp2].stClir.bActive = ptVdbQMRTable->astLineSetting[uloop].stClir.bActive;
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stClir.acActiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stClir.acActiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stClir.acActiveCode);
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stClir.acDeactiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stClir.acDeactiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stClir.acDeactiveCode);

	 ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.bActive = ptVdbQMRTable->astLineSetting[uloop].stCFU.bActive;
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.acActiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.acActiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFU.acActiveCode);
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.acDeactiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.acDeactiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFU.acDeactiveCode);
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.acCfNumber, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFU.acCfNumber), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFU.acCfNumber);

	 ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.bActive = ptVdbQMRTable->astLineSetting[uloop].stCFB.bActive;
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.acActiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.acActiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFB.acActiveCode);
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.acDeactiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.acDeactiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFB.acDeactiveCode);
     snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.acCfNumber, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFB.acCfNumber), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFB.acCfNumber);

	 ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.bActive = ptVdbQMRTable->astLineSetting[uloop].stCFNR.bActive;
	 ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.lSecCfnr = ptVdbQMRTable->astLineSetting[uloop].stCFNR.lSecCfnr;
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.acActiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.acActiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFNR.acActiveCode);
	 snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.acDeactiveCode, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.acDeactiveCode), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFNR.acDeactiveCode);
     snprintf(ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.acCfNumber, sizeof(ptVdbMRTable->astLineSetting[uloop_temp2].stCFNR.acCfNumber), "%s", ptVdbQMRTable->astLineSetting[uloop].stCFNR.acCfNumber);

	 uloop_temp2++;
	 uloop++;
	if(ATP_TAB_VOICE_64_LEN <= uloop)
	{
      break;
    }
  	}	
	 
	return VOS_OK;
}


void *ATP_VDB_Init()
{
    sqlite3         *db;
    int             bDbExist;
	int             bReadFalsh;
    int             lRet;
    char 			*pcSql;
#ifndef SUPPORT_DESKTOP
    struct stat     info;
    unsigned int    ulLen;
    unsigned char   aucStartTag[4];
#endif
	ATP_VDB_TABLE_QMR_ST *pstVdbQMRTable = NULL;
    ATP_VDB_TABLE_MR_ST  *pstVdbMRTable = NULL;
    int bConvertQMRVdb = 0;
    bReadFalsh = 0;
#ifndef SUPPORT_DESKTOP
    if (stat(VDB_DB_PATH, &info) < 0)
    //if (0)
    {
        //printf("Need read flash now ...\n");
        lRet = ATP_FLASH_ReadArea(FOM_E_FLASH_AREA_DECT_PHONEBOOK,
                                  (char *)aucStartTag, 0, sizeof(aucStartTag));
        //printf("Read flash return %d %x:%x:%x:%x.\n", lRet, aucStartTag[0], aucStartTag[1],
        //    aucStartTag[2], aucStartTag[3]);
        //如果为QMR版本的flag(QMR升级到MR版本)，也进行读取flash的操作
        if ((0 == lRet) 
			&& (((VDB_START_TAG_0 == aucStartTag[0]) 
			&& (VDB_START_TAG_1 == aucStartTag[1]))
            || ((VDB_START_TAGQMR_0 == aucStartTag[0])
            && (VDB_START_TAGQMR_1 == aucStartTag[1]))))
        {
            ulLen = aucStartTag[2];
            ulLen = (ulLen << 8);
            ulLen += aucStartTag[3];

            //printf("Length %d.\n", ulLen);

            if (ulLen < (128 * 1024 - 4))
            {
                ulLen += 8;
                pcSql = (char *)malloc(ulLen);
                if (NULL != pcSql)
                {
                    lRet = ATP_FLASH_ReadArea(FOM_E_FLASH_AREA_DECT_PHONEBOOK,
                                              pcSql, 0, ulLen);
                    if (0 == lRet)
                    {
                        if ((VDB_START_TAGQMR_0 != aucStartTag[0])&&
							((VDB_START_TAG_0 != ((VOS_UINT8)(pcSql[ulLen - 4]))) ||
                            (VDB_START_TAG_1 != ((VOS_UINT8)(pcSql[ulLen - 3]))) ||
                            (VDB_START_TAG_0 != ((VOS_UINT8)(pcSql[ulLen - 2]))) ||
                            (VDB_START_TAG_1 != ((VOS_UINT8)(pcSql[ulLen - 1])))))
                        {
                            printf("Incomplete vdb in flash detected.\n");
                        }
                        else
                        {
                            VdbWriteBufferToFile(VDB_DB_PATH, (pcSql + 4), ulLen - 8);
							bReadFalsh = 1;
                        }
                    }
                    else
                    {
                        printf("Read total block failed.\n");
                    }
                }
            }
        }
    }
#endif
    /*如果读取flash成功, 并且是QMR升级到MR版本则将QMR版本的VDB内容取出*/
    if ((1 == bReadFalsh) 
		&& (VDB_START_TAGQMR_0 == aucStartTag[0])
        && (VDB_START_TAGQMR_1 == aucStartTag[1]))
    {
        pstVdbQMRTable = ATP_VDB_ReadQMRTable();
		unlink(VDB_DB_PATH);
		pstVdbMRTable  = (ATP_VDB_TABLE_MR_ST *)malloc(sizeof(ATP_VDB_TABLE_MR_ST));
		if (NULL != pstVdbMRTable)
		{
			memset(pstVdbMRTable, 0, sizeof(ATP_VDB_TABLE_MR_ST));
			// TODO:调整数据结构
			ConvertQMRToMR(pstVdbQMRTable, pstVdbMRTable);
			bConvertQMRVdb = 1;
		}
		if (NULL != pstVdbQMRTable)
		{
			free((void *)pstVdbQMRTable);
			pstVdbQMRTable = NULL;
		}
    }
    // Try to open with read only mode to check if the db exist
    db = NULL;
    bDbExist = 0;
    lRet = sqlite3_open_v2(VDB_DB_PATH, &db, SQLITE_OPEN_READONLY, NULL);
    sqlite3_close(db);
    if (SQLITE_OK == lRet)      // DB already exist
    {
        //printf("Vdb already exists.\n");
        bDbExist = 1;
    }

    // Try to open again
    db = NULL;
    lRet = sqlite3_open(VDB_DB_PATH, &db);
    if (SQLITE_OK != lRet)
    {
        sqlite3_close(db);
		if (NULL != pstVdbMRTable)
		{
			free((void *)pstVdbMRTable);
			pstVdbMRTable = NULL;
		}
        return NULL;
    }

	// set busy_handler
	sqlite3_busy_handler(db,ATP_VDB_busy_handler_callback,NULL);

    if (0 != bDbExist)
    {
	    if (NULL != pstVdbMRTable)
		{
			free((void *)pstVdbMRTable);
			pstVdbMRTable = NULL;
		}
        return db;
    }

    // Create the table again
    printf("Create table again.\n");

    /*初始化异常时需要将状态计入VDB状态文件*/
    FILE*       pFile  = VOS_NULL_PTR;    
    pFile = fopen(VDB_DB_STATUS, "w");
    if(VOS_NULL_PTR != pFile)
    {
        //QMR升级到MR版本时不能清空VDB
        if(1 == bConvertQMRVdb)
        {
           bDbExist = 1;
		}
		fprintf(pFile,"%u", bDbExist);
        fclose(pFile);
    }

    // Read the sql cmd file
    pcSql = VdbReadFileToBuffer(VDB_SQL_CMD_PATH, NULL, 0);
    if (NULL == pcSql)
    {
        sqlite3_close(db);
		if (NULL != pstVdbMRTable)
		{
			free((void *)pstVdbMRTable);
			pstVdbMRTable = NULL;
		}
        return NULL;
    }

    // Create the db
    lRet = sqlite3_exec(db, pcSql, NULL, NULL, NULL);
    free((void *)pcSql);
    if (SQLITE_OK != lRet)
    {
        sqlite3_close(db);
		if (NULL != pstVdbMRTable)
		{
			free((void *)pstVdbMRTable);
			pstVdbMRTable = NULL;
		}
        return NULL;
    }
    /*如果读取flash成功, 并且是QMR升级到MR版本,则将调整后的内容增加到VDB*/
#ifndef SUPPORT_DESKTOP
    if ((1 == bReadFalsh) 
		&& (VDB_START_TAGQMR_0 == aucStartTag[0])
        && (VDB_START_TAGQMR_1 == aucStartTag[1]))
    {
        /* BEGIN: Added by zKF39925 2011/8/22  标记这次启动是从80 到83, 用于lanmap 一次读取vdb 的操作*/
        system (" echo 1 > /var/qmr_to_mr ");
        /* END:   Added by zKF39925 2011/8/22 */
        
		ATP_VDB_WriteMRTable(db, pstVdbMRTable);
		if (NULL != pstVdbMRTable)
		{
			free((void *)pstVdbMRTable);
			pstVdbMRTable = NULL;
		}
    }
#endif
    return db;
}

void *ATP_DummyVDB_Init()
{
    sqlite3         *db;
    int             bDbExist;
    int             lRet;
    char 			*pcSql;

    // Try to open with read only mode to check if the db exist
    db = NULL;
    bDbExist = 0;
    lRet = sqlite3_open_v2(VDB_DUMMYDB_PATH, &db, SQLITE_OPEN_READONLY, NULL);
    sqlite3_close(db);
    if (SQLITE_OK == lRet)      // DB already exist
    {
        //printf("Vdb already exists.\n");
        printf("\naaaaaaaa\n");
        bDbExist = 1;
    }

    // Try to open again
    db = NULL;
    lRet = sqlite3_open(VDB_DUMMYDB_PATH, &db);
    if (SQLITE_OK != lRet)
    {
        sqlite3_close(db);
        printf("\nbbbbbbbbbb\n");
		return NULL;
	}

	// set busy_handler
	sqlite3_busy_handler(db,ATP_VDB_busy_handler_callback,NULL);

    if (0 != bDbExist)
    {
        printf("\ncccccccccc\n");
        return db;
    }

    // Create the table again
    printf("Create rss table again.\n");

    // Read the sql cmd file
    pcSql = VdbReadFileToBuffer(VDB_SQL_DUMMY_PATH, NULL, 0);
    if (NULL == pcSql)
    {
        sqlite3_close(db);
        return NULL;
    }

    // Create the db
    lRet = sqlite3_exec(db, pcSql, NULL, NULL, NULL);
    free((void *)pcSql);
    if (SQLITE_OK != lRet)
    {
        printf("Create rss table fail.\n");
        sqlite3_close(db);
        return NULL;
    }
 
    return db;
}
int ATP_VDB_Save(void *pvVdbHdl)
{
#ifndef SUPPORT_DESKTOP
    int             lRet;
    unsigned int    ulFileLen;
    unsigned char   *pcData;

    pcData = (unsigned char *)VdbReadFileToBuffer(VDB_DB_PATH, &ulFileLen, 4);
    if (NULL == pcData)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    pcData[0] = VDB_START_TAG_0;
    pcData[1] = VDB_START_TAG_1;
    //printf("File length: %x.\n", ulFileLen);
    pcData[2] = (unsigned char)((ulFileLen & 0x0000FF00) >> 8);
    pcData[3] = (unsigned char)(ulFileLen & 0x000000FF);
    pcData[ulFileLen + 4] = VDB_START_TAG_0;
    pcData[ulFileLen + 5] = VDB_START_TAG_1;
    pcData[ulFileLen + 6] = VDB_START_TAG_0;
    pcData[ulFileLen + 7] = VDB_START_TAG_1;

    //printf("Write length %x:%x.\n", pcData[2], pcData[3]);
    printf("\n************************Write flash to vdb now ...\n");
    lRet = BSP_FLASH_WriteArea(BSP_E_FLASH_AREA_DECT_PHONEBOOK,
                               (char *)pcData, 0, (ulFileLen + 4 * 2));
    free((void *)pcData);
    if (0 != lRet)
    {
        return ATP_VDB_RET_SYS_ERR;
    }
#endif

    return ATP_VDB_RET_OK;
}

typedef void (*PfVdbTblFiller)(void *pvTblVal, int argc, char **argv);
typedef char *(*PfVdbTblBuilder)(void *pvHandle, void *pvTblVal, int bIsAdd, VDB_ENTRY_ID *pllEntryId);

typedef struct tagVDB_TBL_ENTRY_ST
{
    const char          *pcTblName;
    PfVdbTblFiller      pfFiller;
    PfVdbTblBuilder     pfBuilder;
	PfVdbTblFiller      pfQMRFiller;
} VDB_TBL_ENTRY_ST;

static const VDB_TBL_ENTRY_ST m_astVdbTbls[] =
{
    {
        ATP_TAB_VOICE_MISSEDCALL_NAME,
        VdbFillMissedCallTbl,
        VdbBuildMissedCallTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_OUTGOINGCALL_NAME,
        VdbFillOutInCallRecordTbl,
        VdbBuildOutgoingCallTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_IN_ACCEPTCALL_NAME,
        VdbFillOutInCallRecordTbl,
        VdbBuildIncomingCallTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_ALLCALL_NAME,
        VdbFillAllCallRecordTbl,
        NULL,
        NULL
    },
    {
        ATP_TAB_VOICE_CONTACT_NAME,
        VdbFillContactTbl,
        VdbBuildContactTbl,
        VdbFillContactTblQMR
    },
    {
        ATP_TAB_VOICE_INTERNELNAMES_NAME,
        VdbFillInternalNameListTbl,
        VdbBuildInternalNameListTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_DECTSETTINGS_NAME,
        VdbFillDectSettingsTbl,
        VdbBuildDectSettingsTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_LINESETTING_NAME,
        VdbFillLineSettingsTbl,
        VdbBuildLineSettingsTbl,
        VdbFillLineSettingsTblQMR
    },
    {
        ATP_TAB_LANMAP_NAME,
        VdbFillLanMapTbl,
        VdbBuildLanMapTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_MOBILE,
        VdbFillMobileTbl,
        VdbBuildMobileTbl,
        VdbFillMobileTblQMR
    },
    /*Added by kf39808 2011-06-07*/
    {
    	ATP_TAB_VOICE_ONLINECONTACT_NAME,
    	VdbFillOnlineContact,
    	VdbBuildOnlineContact,
    	NULL
    },
    /*End of by kf39808 2011-06-07*/
    {
        ATP_TAB_VOICE_DECTEMAIL_NAME,
        VdbFillDectEmailTbl,
        VdbBuildDectEmailTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_DECTEMAIL_INTERVAL,
        VdbFillDectEmailIntervalTbl,
        VdbBuildDectEmailIntervalTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_DECTERSS_NAME,
        VdbFillDectRSSTbl,
        VdbBuildDectRSSTbl,
        NULL
    },
    {
        ATP_TAB_VOICE_DECTERSS_INTERVAL,
        VdbFillDectRSSIntervalTbl,
        VdbBuildDectRSSIntervalTbl,
        NULL
    },
	/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
	{
        ATP_TAB_VOICE_CALLLIST_NAME,
        VdbFillCalllistTbl,
        VdbBuildCalllistTbl,
        NULL
    },    
    /* END:   Added by zengjun 182276, 2011/5/30 */
    {
        ATP_TAB_EMAIL_SUBLIST_NAME,
        VdbFillEmailSubListTbl,
        VdbBuildEmailSubListTbl,
        NULL
    }
};

typedef struct tagVDB_QUERY_ARG_ST
{
    unsigned int            ulTblIdx;
    void                    *pvTblVal;
    PfVdbBrowseEntryProc    pfProc;
    void                    *pvExtraArg;
	unsigned long			EntryNum;
} VDB_QUERY_ARG_ST;

static int VdbGetEntryCallback(void *pvArg, int argc, char **argv, char **azColName)
{
    VDB_QUERY_ARG_ST        *pstArg;

//#ifdef ATP_DEBUG
#if 0
    int         i;

    printf("Vdb entry call back begin %d ***************************\n", argc);
    for (i = 0; i < argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
#endif

    pstArg = (VDB_QUERY_ARG_ST *)pvArg;
    if ((NULL != pstArg) && (pstArg->ulTblIdx < (unsigned int)(sizeof(m_astVdbTbls)/sizeof(m_astVdbTbls[0]))))
    {
        m_astVdbTbls[pstArg->ulTblIdx].pfFiller(pstArg->pvTblVal, argc, argv);
        if (NULL != pstArg->pfProc)
        {
            pstArg->pfProc((ATP_VDB_TABLE_ID_EN)(pstArg->ulTblIdx), pstArg->pvTblVal, pstArg->pvExtraArg);
        }
    }

#ifdef ATP_DEBUG
    //printf("Vdb entry call back end ***************************\n");
#endif

    return 0;
}
static int VdbGetEntryCallbackQMR(void *pvArg, int argc, char **argv, char **azColName)
{
    VDB_QUERY_ARG_ST        *pstArg;

//#ifdef ATP_DEBUG
#if 0
    int         i;

    printf("Vdb entry call back begin %d ***************************\n", argc);
    for (i = 0; i < argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
#endif

    pstArg = (VDB_QUERY_ARG_ST *)pvArg;
    if ((NULL != pstArg) && (pstArg->ulTblIdx < (unsigned int)(sizeof(m_astVdbTbls)/sizeof(m_astVdbTbls[0]))))
    {
        if (NULL != m_astVdbTbls[pstArg->ulTblIdx].pfQMRFiller)
    	{
    		m_astVdbTbls[pstArg->ulTblIdx].pfQMRFiller(pstArg->pvTblVal, argc, argv);
    	}
		else
		{
		    m_astVdbTbls[pstArg->ulTblIdx].pfFiller(pstArg->pvTblVal, argc, argv);
		}
        if (NULL != pstArg->pfProc)
        {
            pstArg->pfProc((ATP_VDB_TABLE_ID_EN)(pstArg->ulTblIdx), pstArg->pvTblVal, pstArg->pvExtraArg);
        }
    }

#ifdef ATP_DEBUG
    //printf("Vdb entry call back end ***************************\n");
#endif

    return 0;
}



static int VdbGetAllEntryIdCallback(void *pvArg, int argc, char **argv, char **azColName)
{
    VDB_QUERY_ARG_ST        *pstArg;
	VDB_ENTRY_ID 			llEntryID; 
	
    pstArg = (VDB_QUERY_ARG_ST *)pvArg;
    if ((NULL != pstArg) && (pstArg->ulTblIdx < (unsigned int)(sizeof(m_astVdbTbls)/sizeof(m_astVdbTbls[0]))))
    { 
        if (NULL != pstArg->pvTblVal)
        {
    	    if (NULL != argv[0])
		    {
		        llEntryID = (VDB_ENTRY_ID)atoll(argv[0]); 
		    }
			if (250 >= pstArg->EntryNum)
			{
				*(((u16 *)(pstArg->pvTblVal)) + pstArg->EntryNum) = (u16)llEntryID;
				pstArg->EntryNum++;
			}	            
        } 
    }
    return 0;
}

static int VdbGetCountCallback(void *pvArg, int argc, char **argv, char **azColName)
{
    int         *lCnt;

    lCnt = (int *)pvArg;
    if (NULL != lCnt)
    {
        (*lCnt) = atoi(argv[0]);
    }

    return 0;
}

static int VdbDummyBrowse(ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg)
{
    // Do nothing
    return 0;
}

int ATP_VDB_GetEntryByEntryId(
                            void                    *pvVdbHdl,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            VDB_ENTRY_ID            llEntryId)
{
    char        acCondition[256];

    snprintf(acCondition, sizeof(acCondition), "where " ATP_TAB_ENTRY_ID_NAME "=%lld", llEntryId);

    return ATP_VDB_BrowseEntries(pvVdbHdl, VdbDummyBrowse, enTblId, pvTblValue, NULL,
                                 acCondition, 0, 0, NULL);;
}

// Get single entry from db by sequence number
int ATP_VDB_GetEntryByIdx(
                            void                    *pvVdbHdl,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            unsigned int            ulIdx)
{
#if 0
    int                 lRet;
    char                *pcSqlCmd;
    VDB_QUERY_ARG_ST    stArg;

    if ((NULL == pvVdbHdl) || (NULL == pvTblValue))
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    pcSqlCmd = sqlite3_mprintf("select * from %s LIMIT 1 OFFSET %u", m_astVdbTbls[enTblId].pcTblName, ulIdx);
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    stArg.pfProc   = NULL;
    stArg.pvTblVal = pvTblValue;
    stArg.ulTblIdx = (unsigned int)enTblId;
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetEntryCallback, (void *)(&stArg), NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
#else
    return ATP_VDB_BrowseEntries(pvVdbHdl, VdbDummyBrowse, enTblId, pvTblValue, NULL, NULL, 1, ulIdx, NULL);
#endif
}

// Browse multiple entries from db
int ATP_VDB_BrowseEntries(
                            void                    *pvVdbHdl,
                            PfVdbBrowseEntryProc    pfBrowseProc,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            void                    *pvExtraArg,
                            const char              *pcSqlCondition,
                            int                     lRequestCnt,
                            int                     lStartIdx,
                            int                     *plTotalMatched)
{
    int                 lRet;
    char                *pcSqlCmd;
    VDB_QUERY_ARG_ST    stArg;
    char                acLimitArg[256];

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    acLimitArg[0] = '\0';
    if (lRequestCnt > 0)
    {
        if (lStartIdx >= 0)
        {
            snprintf(acLimitArg, sizeof(acLimitArg), " LIMIT %d OFFSET %d", lRequestCnt, lStartIdx);
        }
        else
        {
            snprintf(acLimitArg, sizeof(acLimitArg), " LIMIT %d", lRequestCnt);
        }
    }

    // Get total matched if necessary
    if (plTotalMatched)
    {
        if (NULL == pcSqlCondition)
        {
            pcSqlCmd = sqlite3_mprintf("select count(*) from %s%s", m_astVdbTbls[enTblId].pcTblName, acLimitArg);
        }
        else
        {
            pcSqlCmd = sqlite3_mprintf("select count(*) from %s %s%s",
                            m_astVdbTbls[enTblId].pcTblName, pcSqlCondition, acLimitArg);
        }
        if (NULL == pcSqlCmd)
        {
            return ATP_VDB_RET_MEM_ERR;
        }
        lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetCountCallback, (void *)plTotalMatched, NULL);
        sqlite3_free(pcSqlCmd);
        if (SQLITE_OK != lRet)
        {
            return ATP_VDB_RET_SQL_ERR;
        }
    }

    if ((NULL == pfBrowseProc) || (NULL == pvTblValue))
    {
        return ATP_VDB_RET_OK;
    }

    // Build the browse command
    if (NULL == pcSqlCondition)
    {
        pcSqlCmd = sqlite3_mprintf("select * from %s%s", m_astVdbTbls[enTblId].pcTblName, acLimitArg);
    }
    else
    {
        pcSqlCmd = sqlite3_mprintf("select * from %s %s%s",
                    m_astVdbTbls[enTblId].pcTblName, pcSqlCondition, acLimitArg);
    }
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    // Do the query
    stArg.pfProc     = pfBrowseProc;
    stArg.pvExtraArg = pvExtraArg;
    stArg.pvTblVal   = pvTblValue;
    stArg.ulTblIdx   = (unsigned int)enTblId;
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetEntryCallback, (void *)(&stArg), NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
}

int ATP_VDB_BrowseEntriesQMR(
                            void                    *pvVdbHdl,
                            PfVdbBrowseEntryProc    pfBrowseProc,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            void                    *pvExtraArg,
                            const char              *pcSqlCondition,
                            int                     lRequestCnt,
                            int                     lStartIdx,
                            int                     *plTotalMatched)
{
    int                 lRet;
    char                *pcSqlCmd;
    VDB_QUERY_ARG_ST    stArg;
    char                acLimitArg[256];

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    acLimitArg[0] = '\0';
    if (lRequestCnt > 0)
    {
        if (lStartIdx >= 0)
        {
            snprintf(acLimitArg, sizeof(acLimitArg), " LIMIT %d OFFSET %d", lRequestCnt, lStartIdx);
        }
        else
        {
            snprintf(acLimitArg, sizeof(acLimitArg), " LIMIT %d", lRequestCnt);
        }
    }

    // Get total matched if necessary
    if (plTotalMatched)
    {
        if (NULL == pcSqlCondition)
        {
            pcSqlCmd = sqlite3_mprintf("select count(*) from %s%s", m_astVdbTbls[enTblId].pcTblName, acLimitArg);
        }
        else
        {
            pcSqlCmd = sqlite3_mprintf("select count(*) from %s %s%s",
                            m_astVdbTbls[enTblId].pcTblName, pcSqlCondition, acLimitArg);
        }
        if (NULL == pcSqlCmd)
        {
            return ATP_VDB_RET_MEM_ERR;
        }
        lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetCountCallback, (void *)plTotalMatched, NULL);
        sqlite3_free(pcSqlCmd);
        if (SQLITE_OK != lRet)
        {
            return ATP_VDB_RET_SQL_ERR;
        }
    }

    if ((NULL == pfBrowseProc) || (NULL == pvTblValue))
    {
        return ATP_VDB_RET_OK;
    }

    // Build the browse command
    if (NULL == pcSqlCondition)
    {
        pcSqlCmd = sqlite3_mprintf("select * from %s%s", m_astVdbTbls[enTblId].pcTblName, acLimitArg);
    }
    else
    {
        pcSqlCmd = sqlite3_mprintf("select * from %s %s%s",
                    m_astVdbTbls[enTblId].pcTblName, pcSqlCondition, acLimitArg);
    }
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }
    // Do the query
    stArg.pfProc     = pfBrowseProc;
    stArg.pvExtraArg = pvExtraArg;
    stArg.pvTblVal   = pvTblValue;
    stArg.ulTblIdx   = (unsigned int)enTblId;
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetEntryCallbackQMR, (void *)(&stArg), NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
}


// Browse multiple entries from db,But this function support read entry fields.
int ATP_VDB_BrowseEntryId(
                            void                    *pvVdbHdl,
                            PfVdbBrowseEntryProc    pfBrowseProc,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            void                    *pvExtraArg,
                            const char              *pcSqlCondition)
{
    int                 lRet;
    char                *pcSqlCmd;
    VDB_QUERY_ARG_ST    stArg;

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    } 

    if ((NULL == pfBrowseProc) || (NULL == pvTblValue))
    {
        return ATP_VDB_RET_OK;
    }

    // Build the browse command
    if (NULL == pcSqlCondition)
    {
        pcSqlCmd = sqlite3_mprintf("select %s from %s", 
                    ATP_TAB_ENTRY_ID_NAME, 
                    m_astVdbTbls[enTblId].pcTblName);
    }
    else
    {
        pcSqlCmd = sqlite3_mprintf("select %s from %s %s",
                    ATP_TAB_ENTRY_ID_NAME,
                    m_astVdbTbls[enTblId].pcTblName, 
                    pcSqlCondition);
    }
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }
    
    // Do the query
    stArg.pfProc     = pfBrowseProc;
    stArg.pvExtraArg = pvExtraArg;
    stArg.pvTblVal   = pvTblValue;
    stArg.ulTblIdx   = (unsigned int)enTblId;
	stArg.EntryNum   = 0;
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, VdbGetAllEntryIdCallback, (void *)(&stArg), NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
}

// Modify the value of an entry, the value of id must be specified in pvTblValue
int ATP_VDB_ModifyEntry(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId, void *pvTblValue)
{
    int                 lRet;
    char                *pcSqlCmd;
    char                *pcBuildStr;
    VDB_ENTRY_ID        llEntryId;
    const char          *pcTblName;

    if ((NULL == pvVdbHdl) || (NULL == pvTblValue))
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    // Build the value string
    pcBuildStr = m_astVdbTbls[enTblId].pfBuilder(pvVdbHdl, pvTblValue, 0, &llEntryId);
    if (NULL == pcBuildStr)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    pcTblName = m_astVdbTbls[enTblId].pcTblName;
    switch (enTblId)
    {
    case ATP_VDB_TABLE_MISSED_CALL:
    case ATP_VDB_TABLE_OUTGOING_CALL:
    case ATP_VDB_TABLE_INCOMING_CALL:
    case ATP_VDB_TABLE_ALLCALL:
        pcTblName = "callrecord";
        break;
    case ATP_VDB_TABLE_LINE_SETTINGS:
        pcTblName = "linesetting";
        break;
	/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
	case ATP_VDB_TABLE_CALLLIST:
		pcTblName = "calllist";
		break;
    /* END:   Added by zengjun 182276, 2011/5/30 */
    default:
    break;
    }

    // Build the sql string
    pcSqlCmd = sqlite3_mprintf("update %s set %s where id=%lld", pcTblName, pcBuildStr, llEntryId);
    sqlite3_free(pcBuildStr);
    if (NULL == pcBuildStr)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
}

// Add a new entry, the value of newly allocated id will be set in pvTblValue
int ATP_VDB_AddEntry(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId, void *pvTblValue)
{
    int                 lRet;
    const char          *pcTblName;
    char                *pcSqlCmd;
    char                *pcBuildStr;

    if ((NULL == pvVdbHdl) || (NULL == pvTblValue))
    {
        return ATP_VDB_RET_PARA_ERR;
    }
    // Build the value string
    pcBuildStr = m_astVdbTbls[enTblId].pfBuilder(pvVdbHdl, pvTblValue, 1, NULL);
    if (NULL == pcBuildStr)
    {
        return ATP_VDB_RET_MEM_ERR;
    }
    // Build the table name
    pcTblName = m_astVdbTbls[enTblId].pcTblName;
    switch (enTblId)
    {
    case ATP_VDB_TABLE_MISSED_CALL:
    case ATP_VDB_TABLE_OUTGOING_CALL:
    case ATP_VDB_TABLE_INCOMING_CALL:
    case ATP_VDB_TABLE_ALLCALL:
        pcTblName = "callrecord";
    break;
	/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
	case ATP_VDB_TABLE_CALLLIST:
		pcTblName = "calllist";
		break;
    /* END:   Added by zengjun 182276, 2011/5/30 */
    default:
    break;
    }
    // Build the sql string
    pcSqlCmd = sqlite3_mprintf("insert into %s values (%s)", pcTblName, pcBuildStr);
    sqlite3_free(pcBuildStr);
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }
    //printf("Do sql cmd: %s\n", pcSqlCmd);

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    // Store the new entryId
    *((VDB_ENTRY_ID *)pvTblValue) = sqlite3_last_insert_rowid((sqlite3 *)pvVdbHdl);
    return ATP_VDB_RET_OK;
}

// Delete an entry by entry Id
int ATP_VDB_DeleteEntry(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId, VDB_ENTRY_ID llEntryId)
{
    int                 lRet;
    const char          *pcTblName;
    char                *pcSqlCmd;

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    // Build the table name
    pcTblName = m_astVdbTbls[enTblId].pcTblName;
    switch (enTblId)
    {
    case ATP_VDB_TABLE_MISSED_CALL:
    case ATP_VDB_TABLE_OUTGOING_CALL:
    case ATP_VDB_TABLE_INCOMING_CALL:
    case ATP_VDB_TABLE_ALLCALL:
        pcTblName = "callrecord";
    break;
	/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
	case ATP_VDB_TABLE_CALLLIST:
		pcTblName = "calllist";
		break;
	/* END:   Added by zengjun 182276, 2011/5/30 */
    default:
    break;
    }

    // Build the cmd
    pcSqlCmd = sqlite3_mprintf("delete from %s where id=%lld",
                               pcTblName, llEntryId);
    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
}

// Delete all entries in a table
int ATP_VDB_DeleteAllEntries(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId)
{
    int                 lRet;
    char                *pcSqlCmd;
    char                acCond[256];

    if (NULL == pvVdbHdl)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    // Build the cmd
    acCond[0] = '\0';
    switch (enTblId)
    {
    case ATP_VDB_TABLE_MISSED_CALL:
        snprintf(acCond, sizeof(acCond), " where " ATP_TAB_VOICE_RECORD_FLAG "=%d or " \
            ATP_TAB_VOICE_RECORD_FLAG "=%d", ATP_VOICE_MISSED_UNREAD_CALL, ATP_VOICE_MISSED_CALL);
    break;
    case ATP_VDB_TABLE_OUTGOING_CALL:
        snprintf(acCond, sizeof(acCond), " where " ATP_TAB_VOICE_RECORD_FLAG "=%d", ATP_VOICE_OUTGOING_CALL);
    break;
    case ATP_VDB_TABLE_INCOMING_CALL:
        snprintf(acCond, sizeof(acCond), " where " ATP_TAB_VOICE_RECORD_FLAG "=%d", ATP_VOICE_INCOMING_CALL);
    break;
    case ATP_VDB_TABLE_ALLCALL:
    break;
    default:
        
    break;
    }

    if ((ATP_VDB_TABLE_MISSED_CALL   == enTblId) ||
        (ATP_VDB_TABLE_OUTGOING_CALL == enTblId) ||
        (ATP_VDB_TABLE_INCOMING_CALL == enTblId) ||
        (ATP_VDB_TABLE_ALLCALL       == enTblId))
    {
        pcSqlCmd = sqlite3_mprintf("delete from callrecord%s", acCond);
    }
    else
    {
        pcSqlCmd = sqlite3_mprintf("delete from %s", m_astVdbTbls[enTblId].pcTblName);
    }

    if (NULL == pcSqlCmd)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    // Do the cmd
    lRet = sqlite3_exec((sqlite3 *)pvVdbHdl, pcSqlCmd, NULL, NULL, NULL);
    sqlite3_free(pcSqlCmd);
    if (SQLITE_OK != lRet)
    {
        return ATP_VDB_RET_SQL_ERR;
    }

    return ATP_VDB_RET_OK;
}

// Print an entry
int ATP_VDB_UtilPrintEntry(void *pvTblValue, ATP_VDB_TABLE_ID_EN enTblId)
{
    VDB_ENTRY_ID    llEntryId;
    char            *pcStr;

    if (NULL == pvTblValue)
    {
        return ATP_VDB_RET_PARA_ERR;
    }

    pcStr = m_astVdbTbls[enTblId].pfBuilder(NULL, pvTblValue, 0, &llEntryId);
    if (NULL == pcStr)
    {
        return ATP_VDB_RET_MEM_ERR;
    }

    printf("id=%lld, %s\n", llEntryId, pcStr);
    sqlite3_free(pcStr);

    return ATP_VDB_RET_OK;
}

