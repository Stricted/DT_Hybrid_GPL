#ifndef __ATP_VOICE_DB_H__
#define __ATP_VOICE_DB_H__

#define ATP_TAB_VOICE_8_LEN                                  8
#define ATP_TAB_VOICE_16_LEN                                 16
#define ATP_TAB_VOICE_32_LEN                                 32
#define ATP_TAB_VOICE_64_LEN                                 64
#define ATP_TAB_VOICE_128_LEN                                128
#define ATP_TAB_VOICE_256_LEN                                256
#define ATP_TAB_VOICE_512_LEN                                512

#define ATP_TAB_VOICE_DATETIME_LEN                           32
#define ATP_TAB_VOICE_MAX_NAME_LEN                           32
#define ATP_TAB_VOICE_MAX_NUMBER_LEN                         32
#define ATP_TAB_VOICE_MAX_BLOCKEDNUMBER_LEN                  252
#define ATP_TAB_VOICE_INCOMLIST_LEN                          128
#define ATP_TAB_VOICE_DECTEMAIL_LEN                          256
#define ATP_TAB_VOICE_MAX_LEN                                256
/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
#define ATP_TAB_VOICE_CALLLIST_MAX_NUMBER_LEN                24
/* END:   Added by zengjun 182276, 2011/5/30 */

#define ATP_TAB_EMAIL_TEXT_MAX_LEN                           650
#define ATP_TAB_EMAIL_UID_MAX_LEN                            128
#define ATP_TAB_EMAIL_TIME_MAX_LEN                           64
#define ATP_TAB_EMAIL_ADDR_MAX_LEN                           64
#define ATP_TAB_EMAIL_SUB_MAX_LEN                            128

typedef long long               VDB_ENTRY_ID;

/*table string*/
#define ATP_TAB_VOICE_MISSEDCALL_NAME              "missedcall"
#define ATP_TAB_VOICE_OUTGOINGCALL_NAME            "outcall"
#define ATP_TAB_VOICE_IN_ACCEPTCALL_NAME           "inaccepcall"
#define ATP_TAB_VOICE_ALLCALL_NAME                 "allcall"
#define ATP_TAB_VOICE_CONTACT_NAME                 "contact"
#define ATP_TAB_VOICE_INTERNELNAMES_NAME           "internalnames"
#define ATP_TAB_VOICE_DECTSETTINGS_NAME            "dectsetting"
#define ATP_TAB_VOICE_LINESETTING_NAME             "linesetting"
#define ATP_TAB_LANMAP_NAME                        "lanmap"
#define ATP_TAB_VOICE_MOBILE                       "mobilesetting"
#define ATP_TAB_VOICE_ONLINECONTACT_NAME           "onlinecontact"    //Added  by kf39808 2011 06.07		
#define ATP_TAB_VOICE_DECTEMAIL_NAME               "dectemailaddr"
#define ATP_TAB_VOICE_DECTEMAIL_INTERVAL           "dectemailinterval"
#define ATP_TAB_VOICE_DECTERSS_NAME                "dectrssinfo"
#define ATP_TAB_VOICE_DECTERSS_INTERVAL            "decterssinterval"
/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
#define ATP_TAB_VOICE_CALLLIST_NAME                "calllist"
/* END:   Added by zengjun 182276, 2011/5/30 */
#define ATP_TAB_LANMAP_MAC_NAME                    "mac"
#define ATP_TAB_LANMAP_FLAG_NAME                   "flag"
#define ATP_TAB_LANMAP_SET_FLAG_NAME               "setflag"
#define ATP_TAB_EMAIL_SUBLIST_NAME                 "emailsub"

/*fields name*/
#define ATP_TAB_ENTRY_ID_NAME             "id"          /*missedcall,inacceptcall,contact,internname,linesetting*/
#define ATP_TAB_VOICE_NUMBER_NAME         "number"      /*missedcall,inacceptcall,internname */
#define ATP_TAB_VOICE_NAME_NAME           "name"        /*missedcall,inacceptcall,contact,internname,linesetting*/
#define ATP_TAB_VOICE_DATETIME_NAME       "datetime"    /*missedcall,inacceptcall*/
#define ATP_TAB_VOICE_NEW_NAME            "new"         /*missedcall*/
#define ATP_TAB_VOICE_LINENAME_NAME       "linename"    /*missedcall,inacceptcall*/
#define ATP_TAB_VOICE_LINESUBTYPE_NAME    "linesubtype" /*missedcall,inacceptcall,contact,linesetting*/
#define ATP_TAB_VOICE_LINEID_NAME         "lineid"      /*missedcall,inacceptcall,contact,linesetting*/
#define ATP_TAB_VOICE_NUM_NAME            "num"         /*missedcall*/
#define ATP_TAB_VOICE_RECORD_FLAG         "flags"

#if 0
#define ATP_TAB_VOICE_FSTNAME_NAME        "fstname"     /*contact*/
#define ATP_TAB_VOICE_DEFAULTNUMBER_NAME  "defaultNum"  /*contact*/
#define ATP_TAB_VOICE_OWNNUMBER_NAME      "own"         /*contact*/
#define ATP_TAB_VOICE_FIXNUMBER_NAME      "fix"         /*contact*/
#define ATP_TAB_VOICE_MOBILENUMBER_NAME   "mobile"      /*contact*/
#define ATP_TAB_VOICE_WORKINGNUMBER_NAME  "working"     /*contact*/
#define ATP_TAB_VOICE_MELODY_NAME         "melody"      /*contact,linesetting*/
#endif
#define ATP_TAB_VOICE_FSTNAME_NAME        "fstname"     /*contact*/
#define ATP_TAB_VOICE_CON_NUM1_NAME       "num1"        /*contact*/
#define ATP_TAB_VOICE_CON_NUM1_FLAG_NAME  "num1flag"    /*contact*/
#define ATP_TAB_VOICE_CON_NUM2_NAME       "num2"        /*contact*/
#define ATP_TAB_VOICE_CON_NUM2_FLAG_NAME  "num2flag"    /*contact*/
#define ATP_TAB_VOICE_CON_NUM3_NAME       "num3"        /*contact*/
#define ATP_TAB_VOICE_CON_NUM3_FLAG_NAME  "num3flag"    /*contact*/
#define ATP_TAB_VOICE_CON_NUMRESERV_NAME  "numreserved" /*contact*/
#define ATP_TAB_VOICE_MELODY_NAME         "melody"      /*contact,linesetting*/

#define ATP_TAB_VOICE_BITMAP_NAME         "bitmap"          /*linesetting*/
#define ATP_TAB_VOICE_DIALPREFIX_NAME     "dialprefix"      /*linesetting*/
#define ATP_TAB_VOICE_FPVOLUME_NAME       "fpvolume"        /*linesetting*/
#define ATP_TAB_VOICE_BLOCKEDNUMBER_NAME  "blocked"         /*linesetting*/
#define ATP_TAB_VOICE_MULCALLMODE_NAME    "mulcallmode"     /*linesetting*/
#define ATP_TAB_VOICE_INTRUCALL_NAME      "intrucall"       /*linesetting*/
#define ATP_TAB_VOICE_CLIRVALUE_NAME      "clirValue"       /*linesetting*/
#define ATP_TAB_VOICE_CLIRACT_NAME        "clirActcode"     /*linesetting*/
#define ATP_TAB_VOICE_CLIRDEACT_NAME      "clirDeActcode"   /*linesetting*/
#define ATP_TAB_VOICE_CFUVALUE_NAME       "cfuValue"        /*linesetting*/
#define ATP_TAB_VOICE_CFUACT_NAME         "cfuActcode"      /*linesetting*/
#define ATP_TAB_VOICE_CFUDEACT_NAME       "cfuDeActcode"    /*linesetting*/
#define ATP_TAB_VOICE_CFUNUM_NAME         "cfuNum"          /*linesetting*/
#define ATP_TAB_VOICE_CFNRVALUE_NAME      "cfnrValue"       /*linesetting*/
#define ATP_TAB_VOICE_CFNRSEC_NAME        "cfnrSec"         /*linesetting*/
#define ATP_TAB_VOICE_CFNRACT_NAME        "cfnrActcode"     /*linesetting*/
#define ATP_TAB_VOICE_CFNRDEACT_NAME      "cfnrDeActcode"   /*linesetting*/
#define ATP_TAB_VOICE_CFNRNUM_NAME        "cfnrNum"         /*linesetting*/
#define ATP_TAB_VOICE_CFBVALUE_NAME       "cfbValue"        /*linesetting*/
#define ATP_TAB_VOICE_CFBACT_NAME         "cfbActcode"      /*linesetting*/
#define ATP_TAB_VOICE_CFBDEACT_NAME       "cfbDeActcode"    /*linesetting*/
#define ATP_TAB_VOICE_CFBNUM_NAME         "cfbNum"          /*linesetting*/
#define ATP_TAB_VOICE_LINE_USABLE         "usable"          /*linesetting*/

#define ATP_TAB_VOICE_MOBILE_ID           "mobileID"        /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_INALL        "incomAll"        /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_OUT          "outgoLine"       /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_CW           "callWait"        /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_ONPHONE      "onlinePhone"     /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_EMAIL        "emailList"       /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_RSS          "rssList"         /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_CFU          "cfu"             /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_CFB          "cfb"             /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_CFNR         "cfnr"            /*mobilesetting*/
#define ATP_TAB_VOICE_MOBILE_TYPE         "hansetType"      /*mobilesetting*/

/*Added by kf39808 2011-06-07*/
#define ATP_TAB_VOICE_NAME_NAME          "name"          /*onlinecontact*/
#define ATP_TAB_VOICE_BENNAME_NAME         "benutzername"       /*onlinecontact*/
#define ATP_TAB_VOICE_PASSWORD_NAME            "password"         /*onlinecontact*/

/*End of by kf39808 2011-06-07*/
#define ATP_TAB_VOICE_DECTMAIL_ENABLE     "enable"          /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_ACCOUNT    "emailAccount"    /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_SVRTYPE    "serverType"      /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_SVRCNT     "serverConnct"    /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_SVRADDR    "mailServerAddr"  /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_SVRPORT    "serverPort"      /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_USRNAME    "userName"        /*dectmailaddr*/
#define ATP_TAB_VOICE_DECTMAIL_PASSWORD   "passWord"        /*dectmailaddr*/

#define ATP_TAB_VOICE_DECTMAIL_INTERVALID "intervalId"      /*dectmailinterval*/

#define ATP_TAB_VOICE_DECTRSS_INSTID      "instid"          /*dectrssinfo*/
#define ATP_TAB_VOICE_DECTRSS_NAME        "rssName"         /*dectrssinfo*/
#define ATP_TAB_VOICE_DECTRSS_URL         "addrURL"         /*dectrssinfo*/
#define ATP_TAB_VOICE_DECTRSS_INFO        "rssInfo"         /*dectrssinfo*/

#define ATP_TAB_VOICE_DECTRSS_INTERVALID  "rssIntervalId"   /*dectrssinterval*/

#define ATP_TAB_EMAIL_SUBLIST_ACCOUNTID   "account"       /*emailsublist*/
#define ATP_TAB_EMAIL_SUBLIST_MAILNUMID   "mailnumber"       /*emailsublist*/
#define ATP_TAB_EMAIL_SUBLIST_UID         "UID"             /*emailsublist*/
#define ATP_TAB_EMAIL_SUBLIST_DATE        "datetime"        /*emailsublist*/
#define ATP_TAB_EMAIL_SUBLIST_EMAILADDR   "emailAddr"       /*emailsublist*/
#define ATP_TAB_EMAIL_SUBLIST_SUBJECT     "subject"         /*emailsublist*/
#define ATP_TAB_EMAIL_SUBLIST_TEXT        "text"            /*emailsublist*/

//calllist非dect的呼叫历史，是整个网关的包括x口的呼叫历史
/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
#define ATP_TAB_VOICE_CALLLIST_PHONENUM     "phoneNum"          /*calllist*/
/* END:   Added by zengjun 182276, 2011/5/30 */
#if 0
/*table type*/
typedef enum tagATP_TAB_TYPE_EN
{
    ATP_TAB_VOICE_MISSEDCALL,              /*missed call list*/
    ATP_TAB_VOICE_IN_ACCEPTCALL,           /*incoming accpeted call list*/
    ATP_TAB_VOICE_CONTACT,                 /*cantact list*/
    ATP_TAB_VOICE_INTERNELNAMES,           /*internel names list*/
    ATP_TAB_VOICE_LINESETTING,             /*line setting list*/   
    
}ATP_TAB_TYPE_EN;
#endif

#if 0
/*line id type*/
typedef enum tagATP_VOICE_LINEID_TYPE_EN
{
    ATP_VOICE_LINEID_EXTERNALCALL,            /*Line identifier for external call*/
    ATP_VOICE_LINEID_SINTERNALCALL,            /*Source line identifier for internal call*/
    ATP_VOICE_LINEID_TINTERNALCALL,            /*Target line identifier for internal call*/
    ATP_VOICE_LINEID_RELATINGTO,              /*"Relating-to" line identifier*/
    ATP_VOICE_LINEID_ALLLINES,                /*"All lines" related-to (see note 3)t*/        
}ATP_VOICE_LINEID_TYPE_EN;
#endif

#if 0
typedef enum tagATP_VOICE_LINE_TYPE_EN
{
    ATP_VOICE_LINE_VOIP = 0,                /*SIP类型号码*/
    ATP_VOICE_LINE_PSTN,
    ATP_VOICE_LINE_ISDN,
}ATP_VOICE_LINE_TYPE_EN;
#endif

typedef enum tagATP_VOICE_CALLRECORD_TYPE_EN
{
    ATP_VOICE_INCOMING_CALL,
    ATP_VOICE_OUTGOING_CALL,
    ATP_VOICE_MISSED_CALL,
    ATP_VOICE_MISSED_UNREAD_CALL
} ATP_VOICE_CALLRECORD_TYPE_EN;

#if 0
/*struct of line ID*/
typedef struct tagATP_VOICE_LINEID_ST
{
    int lSubtype;                      /*see enum ATP_VOICE_LINE_TYPE_EN*/
    int lID;                           /*line ID value*/     
}ATP_VOICE_LINEID_ST;
#endif

/*struct of contact number*/
#if 0
typedef  struct tagATP_VOICE_CONTACT_NUM_ST
{
    char         acDefaultNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                /*default number*/
    char         acOwnNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                    /*own mubern*/
    char         acFixedNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                  /*fixed number*/
    char         acMobileNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                 /*mobile number*/
    char         acWorkNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                   /*work number*/   
}ATP_VOICE_CONTACT_NUM_ST;
#endif
typedef enum tagATP_VOICE_CONTACT_TYPE_EN
{
    ATP_VOICE_CONTACT_TYPE_WORK   = 0x02,
    ATP_VOICE_CONTACT_TYPE_MOBILE = 0x04,
    ATP_VOICE_CONTACT_TYPE_FIXED  = 0x08,
}ATP_VOICE_CONTACT_TYPE_EN;

#define ATP_VOICE_CONTACT_DEFAULT   0x10
#define ATP_VOICE_CONTACT_OWN       0x20

#if 0
typedef  struct tagATP_VOICE_CONTACT_NUM_ST
{
    ATP_VOICE_CONTACT_TYPE_EN enDefalutNumType;          
	ATP_VOICE_CONTACT_TYPE_EN enOwnNumType;
    char         acFixedNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                  /*fixed number*/
    char         acMobileNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                 /*mobile number*/
    char         acWorkNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                   /*work number*/   
}ATP_VOICE_CONTACT_NUM_ST;
#endif
typedef  struct tagATP_VOICE_CONTACT_NUM_ST
{
    char         acNumber1[ATP_TAB_VOICE_64_LEN];                      /* fixed number*/
    char         ucNumber1_flags;
    char         acNumber2[ATP_TAB_VOICE_64_LEN];                      /* mobile number*/
    char         ucNumber2_flags;
    char         acNumber3[ATP_TAB_VOICE_64_LEN];                      /* work number*/   
    char         ucNumber3_flags;
    char         acReserved[5];
}ATP_VOICE_CONTACT_NUM_ST;

/*struc of supply service*/
#if 0
typedef struct tagATP_VOICE_SERVICE_PARM_ST
{
    int  bActive;                                       /*actice state*/                       
    char acCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];          /**/    
}ATP_VOICE_SERVICE_PARM_ST;
#else
typedef struct tagATP_VOICE_CLIR_SRV_PARM_ST
{
    int  bActive;
    char acActiveCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];    /*Activation code number*/
    char acDeactiveCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];  /*Deactivation code number*/
}ATP_VOICE_CLIR_SRV_PARM_ST;

typedef struct tagATP_VOICE_CFX_SRV_PARM_ST
{
    int  bActive;
    char acActiveCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];    /*Activation code number*/
    char acDeactiveCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];  /*Deactivation code number*/
    char acCfNumber[ATP_TAB_VOICE_64_LEN];      /*Call forwarding number*/
}ATP_VOICE_CFU_SRV_PARM_ST, ATP_VOICE_CFB_SRV_PARM_ST;

typedef struct tagATP_VOICE_CFNR_SRV_PARM_ST
{
    int  bActive;
    int  lSecCfnr;                                      /*Nb of seconds before call is forward(0~64)*/
    char acActiveCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];    /*Activation code number*/
    char acDeactiveCode[ATP_TAB_VOICE_MAX_NUMBER_LEN];  /*Deactivation code number*/
    char acCfNumber[ATP_TAB_VOICE_64_LEN];      /*Call forwarding number*/
}ATP_VOICE_CFNR_SRV_PARM_ST;

#endif


typedef struct tagATP_VOICE_FP_IPADDR_ST
{
    char    acAddr[ATP_TAB_VOICE_64_LEN];               /*ip address value*/
    char    acSubnetMask[ATP_TAB_VOICE_64_LEN];         /*ip subnet mask*/
    char    acGateWay[ATP_TAB_VOICE_64_LEN];            /*ip gateway*/
    char    acDNSServer[ATP_TAB_VOICE_64_LEN];          /*ip dns server*/
} ATP_VOICE_FP_IPADDR_ST;

/*
 *  SQL table
 */

/*struct of missed call list*/
typedef struct tagATP_TAB_VOICE_MISSEDCALL_ST
{
    VDB_ENTRY_ID       llEntryID;                       /*Entry identifie*/
    char      acNumber[ATP_TAB_VOICE_64_LEN];   /*calling number*/
    char      acName[ATP_TAB_VOICE_64_LEN];       /*calling name*/
    char      acDateTime[ATP_TAB_VOICE_DATETIME_LEN];   /*date and time of the call*/    
    char      acLineName[ATP_TAB_VOICE_64_LEN];   /*Indicates name of line used for the call*/
    int       lLineID;                                  /*Id of line line used for the call*/    
    int       bNew;                                     /*Indicates whether entry is shown firsttime*/
}ATP_TAB_VOICE_MISSEDCALL_ST;

/*struct of Incoming accpted call list, Outgoing call list*/
typedef struct tagATP_TAB_VOICE_IN_ACCEPTCALL_ST
{
	VDB_ENTRY_ID   llEntryID;                             /*Entry identifie*/         
    char         acNumber[ATP_TAB_VOICE_64_LEN];  /*calling number*/
    char         acName[ATP_TAB_VOICE_64_LEN];      /*calling name*/
    char         acDateTime[ATP_TAB_VOICE_DATETIME_LEN];  /*date and time of the call*/
    char         acLineName[ATP_TAB_VOICE_64_LEN];  /*Indicates name of line used for the call*/
    int          lLineID;                                 /*Id of line line used for the call*/     
}ATP_TAB_VOICE_IN_ACCEPTCALL_ST, ATP_TAB_VOICE_OUTCALL_ST;

/*struct of all call list(include missed call, Incoming call, Outgoing call)*/
typedef struct tagATP_TAB_VOICE_ALLCALL_ST
{
	VDB_ENTRY_ID   llEntryID;                             /*Entry identifie*/         
    char         acNumber[ATP_TAB_VOICE_64_LEN];    /*calling number*/
    char         acName[ATP_TAB_VOICE_64_LEN];      /*calling name*/
    char         acDateTime[ATP_TAB_VOICE_DATETIME_LEN];  /*date and time of the call*/
    char         acLineName[ATP_TAB_VOICE_64_LEN];  /*Indicates name of line used for the call*/
    int          lLineID;                                 /*Id of line line used for the call*/
    int          lCallType;                               /*missed/accepted/outgoing, with type ATP_VOICE_CALLRECORD_TYPE_EN*/
} ATP_TAB_VOICE_ALL_CALL_ST;

/*struct of contact list*/
typedef struct tagATP_TAB_VOICE_CONTACT_ST
{
	VDB_ENTRY_ID             llEntryID;                             /*Entry identifie*/         
    char                     acName[ATP_TAB_VOICE_64_LEN];    /*Name of the contact*/
    char                     acFstName[ATP_TAB_VOICE_64_LEN]; /*First name of the contact*/
    ATP_VOICE_CONTACT_NUM_ST stContactNum;                           /*Number of the contact*/
    int                      lMelody;                               /*Ringing melody used for the contact*/
    int                      lLineID;                               /*Id of line line used for the call*/    
}ATP_TAB_VOICE_CONTACT_ST;

/*struct of internel name list*/
typedef struct tagATP_TAB_VOICE_INTERNELNAMES_ST
{
	VDB_ENTRY_ID   llEntryID;                             /*Entry identifie*/         
    int  lTerminalId;                                     /*Terminal identity number*/
    char acName[ATP_TAB_VOICE_64_LEN];              /*Name of the internal party*/
} ATP_TAB_VOICE_INTERNELNAMES_ST;

typedef enum tagATP_DECT_SETTINGS_TYPE_EN
{
    ATP_DECT_CLOCK_MASTER_FLAGS     = (1 << 0),   // 0 (FP 30H), 1 (PP 31H)
    ATP_DECT_BASE_RESET_FLAG        = (1 << 1),   // 0 (No 30H), 1 (Yes 31H)
    ATP_DECT_IP_ADDR_FLAG           = (1 << 2),   // 0 (dhcp), 1 (static)
    ATP_DECT_IP_VER_FLAG            = (1 << 3),   // 0 (IPv4), 1 (IPv6)
} ATP_DECT_SETTINGS_TYPE_EN;

/*struct of dect system settings list*/
typedef struct tagATP_TAB_VOICE_DECTSETTINGS_ST
{
	VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    int             lFlags;                                 /* Flags, of type ATP_DECT_SETTINGS_TYPE_EN */
	char            acPinCode[ATP_TAB_VOICE_64_LEN];         /*Pincode*/
	ATP_VOICE_FP_IPADDR_ST stIPAddr;                        /*ip address struct */
	char            acFirmwareVer[ATP_TAB_VOICE_64_LEN];     /*cmbs firmware version*/
	char            acEepromVer[ATP_TAB_VOICE_64_LEN];       /*cmbs eeprom version*/
	char            acHardwareVer[ATP_TAB_VOICE_64_LEN];     /*cmbs hardware version*/
} ATP_TAB_VOICE_DECTSETTINGS_ST;

/*struct of line setting list*/
typedef struct tagATP_TAB_VOICE_LINESETTING_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    char            acName[ATP_TAB_VOICE_MAX_NAME_LEN];     /*Name of the line*/
    int             lLineID;                                /*Line identifier*/
    int             bUsable;                                /*if can be use to call*/
    unsigned int    ulHandsetBitmap;                        /*only have 5 handsets, so use bit1 to bit5*/
    char            acDialingPrefex[ATP_TAB_VOICE_MAX_NUMBER_LEN]; /*Field 'Dialling Prefix'*/
    int             lMelody;                                /*Field 'FP melody'*/
    int             lFpVolume;                              /*Field 'FP volume'*/
    char            acBlockedNumber[ATP_TAB_VOICE_MAX_BLOCKEDNUMBER_LEN]; /*syntax:"number1:number2:number3"*/
    int             bMulCallsMode;                          /*ture: multile call mode*/
    int             bIntrusionCall;                         /*true: allow intrusioncall*/
    ATP_VOICE_CLIR_SRV_PARM_ST stClir;                       /*Field 'Permanent CLIR'*/
    ATP_VOICE_CFU_SRV_PARM_ST  stCFU;                        /*Field 'Call forwarding', CFU*/    
    ATP_VOICE_CFB_SRV_PARM_ST  stCFB;                        /*Field 'Call forwarding', CFB*/
    ATP_VOICE_CFNR_SRV_PARM_ST stCFNR;                       /*Field 'Call forwarding', CFNR*/
}ATP_TAB_VOICE_LINESETTING_ST;

typedef struct tagATP_TAB_LAN_MAP_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    char            acName[64];                             /*Name of the lanmap*/
    char            acMac[32];                              /*mac address of the lanmap*/
    unsigned int    ulFlag;
    unsigned int    ulSetFlag;
} ATP_TAB_LAN_MAP_ST;

typedef struct tagATP_TAB_MOBILE_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    int             lTerminalId;                            /*Terminal identity number*/
    int             ulIncomAllFlag;                         /*Relate all line:0/1*/
    unsigned int    ulOutgoLine;                            /*Outgoing Line Id*/       
    VDB_ENTRY_ID    llOnlinePhonebook;
    char            acEmaiList[ATP_TAB_VOICE_64_LEN];       /*List of Email*/
    char            acRSSList[ATP_TAB_VOICE_64_LEN];        /*List of RSS*/
    unsigned int    ulCallWaitFlag;                         /*Flag of CallWait*/
    int             lCFUDestPort;                           /*Port Id of CFU Dest*/
    int             lCFBDestPort;                           /*Port Id of CFB Dest*/
    int             lCFNRDestPort;                          /*Port Id of CFNR Dest*/
    unsigned int    ulHansetType;                           /*Type of HanSet*/
} ATP_TAB_MOBILE_ST;

/*Added by kf39808 2011-06-07*/
typedef struct tagATP_TAB_ONLINECONTACT_ST
{
     VDB_ENTRY_ID    llEntryId;                                                                /*Entry identifie*/ 
     char           acName[ATP_TAB_VOICE_MAX_NAME_LEN];                    /*Name of OnlineContact*/
     char           acBenName[ATP_TAB_VOICE_MAX_LEN];              /*BenuzterName of OnlineContact*/
     char           acPassword[ATP_TAB_VOICE_MAX_LEN];              /*password of OnlineContact*/
}ATP_TAB_ONLINECONTACT_ST;
 /*End of by kf39808 2011-06-07*/
 
typedef struct tagATP_TAB_DECTMAIL_ST
{
    VDB_ENTRY_ID    llEntryID;                                 /*Entry identifie*/
    unsigned int    ulEnable;
    char            acEmailAcount[ATP_TAB_VOICE_MAX_NAME_LEN];
    unsigned int    ulServerType;
    unsigned int    ulServerConnect;
    char            acMailServer[ATP_TAB_VOICE_DECTEMAIL_LEN];
    unsigned int    ulServerPort;
    char            acUserName[ATP_TAB_VOICE_DECTEMAIL_LEN];
    char            acPassWord[ATP_TAB_VOICE_DECTEMAIL_LEN];
} ATP_TAB_DECTMAIL_ST;

typedef struct tagATP_TAB_MAIL_ST
{
    VDB_ENTRY_ID    llEntryID;                                 /*Entry identifie*/
    unsigned int    account;//第几个账号
    unsigned int    mailnumber;//邮件编号
    char            acEmailUid[ATP_TAB_EMAIL_UID_MAX_LEN];
    char            acEmailTime[ATP_TAB_EMAIL_TIME_MAX_LEN];
    char            acEmailAddr[ATP_TAB_EMAIL_ADDR_MAX_LEN];
    char            acEmailSub[ATP_TAB_EMAIL_SUB_MAX_LEN];
    char            acEmailText[ATP_TAB_EMAIL_TEXT_MAX_LEN];
} ATP_TAB_MAIL_ST;

typedef struct tagATP_TAB_DECTMAILINTRVAL_ST
{
    VDB_ENTRY_ID    llEntryID;                                 /*Entry identifie*/
    unsigned int    ulIntervalID;
} ATP_TAB_DECTMAILINTRVAL_ST;

typedef struct tagATP_TAB_DECTRSS_ST
{
    VDB_ENTRY_ID    llEntryID;                                 /*Entry identifie*/
    unsigned int    ulInstID;
    char            acRSSName[ATP_TAB_VOICE_16_LEN];
    char            acRSSURL[ATP_TAB_VOICE_DECTEMAIL_LEN];
    unsigned int    ulEnable;
} ATP_TAB_DECTRSS_ST;

typedef struct tagATP_TAB_DECTRSSINTRVAL_ST
{
    VDB_ENTRY_ID    llEntryID;                                 /*Entry identifie*/
    unsigned int    ulIntervalID;
} ATP_TAB_DECTRSSINTRVAL_ST;

typedef struct tagATP_TAB_DECTRSSSUB_ST
{
    VDB_ENTRY_ID    llEntryID;
    VDB_ENTRY_ID    llChannelID;
    char            acPubDateTime[32];
    char            acSqlDateTime[32];
    char            acSubListTitle[64];
    char            acSubListText[512];
    char            acSubListUrl[64];
    unsigned int    ulReadStatus;
} ATP_TAB_DECTRSSSUB_ST;

/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
typedef struct tagATP_TAB_CALLLIST_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/                           
    char            lphoneNum[ATP_TAB_VOICE_CALLLIST_MAX_NUMBER_LEN];  /*Number*/

} ATP_TAB_CALLLIST_ST;
/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/

/* for QMR VDB TABLE Cotact*/
typedef struct tagATP_TAB_MOBILE_QMR_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    int             lTerminalId;                            /*Terminal identity number*/
    char            acIncomList[ATP_TAB_VOICE_INCOMLIST_LEN];  /*List of Number*/
    unsigned int    ulCallWaitFlag;                         /*Flag of CallWait*/
    unsigned int    ulClirFlag;                             /*Flag of Clir*/
    int             lCFUDestPort;                           /*Port Id of CFU Dest*/
    int             lCFBDestPort;                           /*Port Id of CFB Dest*/
    int             lCFNRDestPort;                          /*Port Id of CFNR Dest*/
    unsigned int    ulHansetType;                           /*Type of HanSet*/
} ATP_TAB_MOBILE_QMR_ST;

typedef  struct tagATP_VOICE_CONTACT_NUM_QMR_ST
{
    ATP_VOICE_CONTACT_TYPE_EN enDefalutNumType;          
	ATP_VOICE_CONTACT_TYPE_EN enOwnNumType;
    char         acFixedNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                  /*fixed number*/
    char         acMobileNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                 /*mobile number*/
    char         acWorkNumber[ATP_TAB_VOICE_MAX_NUMBER_LEN];                   /*work number*/   
}ATP_VOICE_CONTACT_NUM_QMR_ST;

/*struct of contact list*/
typedef struct tagATP_TAB_VOICE_CONTACT_QMR_ST
{
	VDB_ENTRY_ID                 llEntryID;                             /*Entry identifie*/         
    char                         acName[ATP_TAB_VOICE_MAX_NAME_LEN];    /*Name of the contact*/
    char                         acFstName[ATP_TAB_VOICE_MAX_NAME_LEN]; /*First name of the contact*/
    ATP_VOICE_CONTACT_NUM_QMR_ST stContctNum;                           /*Number of the contact*/
    int                          lMelody;                               /*Ringing melody used for the contact*/
    int                          lLineID;                               /*Id of line line used for the call*/    
}ATP_TAB_VOICE_CONTACT_QMR_ST;

/*struct of line setting list*/
typedef struct tagATP_TAB_VOICE_LINESETTING_QMR_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    char            acName[ATP_TAB_VOICE_MAX_NAME_LEN];     /*Name of the line*/
    int             lLineID;                                /*Line identifier*/
    unsigned int    ulHandsetBitmap;                        /*only have 5 handsets, so use bit1 to bit5*/
    char            acDialingPrefex[ATP_TAB_VOICE_MAX_NUMBER_LEN]; /*Field 'Dialling Prefix'*/
    int             lMelody;                                /*Field 'FP melody'*/
    int             lFpVolume;                              /*Field 'FP volume'*/
    char            acBlockedNumber[ATP_TAB_VOICE_MAX_BLOCKEDNUMBER_LEN]; /*syntax:"number1:number2:number3"*/
    int             bMulCallsMode;                          /*ture: multile call mode*/
    int             bIntrusionCall;                         /*true: allow intrusioncall*/
    ATP_VOICE_CLIR_SRV_PARM_ST stClir;                       /*Field 'Permanent CLIR'*/
    ATP_VOICE_CFU_SRV_PARM_ST  stCFU;                        /*Field 'Call forwarding', CFU*/    
    ATP_VOICE_CFB_SRV_PARM_ST  stCFB;                        /*Field 'Call forwarding', CFB*/
    ATP_VOICE_CFNR_SRV_PARM_ST stCFNR;                       /*Field 'Call forwarding', CFNR*/
}ATP_TAB_VOICE_LINESETTING_QMR_ST;

typedef struct tagATP_VDB_TABLE_QMR_ST
{
    ATP_TAB_MOBILE_QMR_ST 	       astMobileSetting[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_LAN_MAP_ST             astLanmap[ATP_TAB_VOICE_256_LEN];
	ATP_TAB_VOICE_ALL_CALL_ST      astVoiceCallRec[ATP_TAB_VOICE_512_LEN];
	ATP_TAB_VOICE_CONTACT_QMR_ST   astPhoneBook[ATP_TAB_VOICE_256_LEN];
	ATP_TAB_VOICE_DECTSETTINGS_ST      astDectSetting[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_VOICE_INTERNELNAMES_ST     astInternalName[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_VOICE_LINESETTING_QMR_ST   astLineSetting[ATP_TAB_VOICE_64_LEN];	
} ATP_VDB_TABLE_QMR_ST;


typedef struct tagATP_VDB_TABLE_MR_ST
{
    ATP_TAB_MOBILE_ST 	         astMobileSetting[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_LAN_MAP_ST           astLanmap[ATP_TAB_VOICE_256_LEN];
	ATP_TAB_VOICE_ALL_CALL_ST    astVoiceCallRec[ATP_TAB_VOICE_512_LEN];
	ATP_TAB_VOICE_CONTACT_ST     astPhoneBook[ATP_TAB_VOICE_256_LEN];
	ATP_TAB_VOICE_DECTSETTINGS_ST     astDectSetting[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_VOICE_INTERNELNAMES_ST    astInternalName[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_VOICE_LINESETTING_ST      astLineSetting[ATP_TAB_VOICE_64_LEN];
	ATP_TAB_DECTMAIL_ST               astDectMail[ATP_TAB_VOICE_64_LEN];
	ATP_TAB_DECTMAILINTRVAL_ST        astDectMailIntVal[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_DECTRSS_ST                astDectRss[ATP_TAB_VOICE_16_LEN];
	ATP_TAB_DECTRSSINTRVAL_ST         astDectRssIntVal[ATP_TAB_VOICE_8_LEN];
	ATP_TAB_CALLLIST_ST               astVoiceCallList[ATP_TAB_VOICE_256_LEN];
} ATP_VDB_TABLE_MR_ST;

typedef struct tagATP_TAB_EMAIL_SUB_ST
{
    VDB_ENTRY_ID    llEntryID;                              /*Entry identifie*/
    char            acAccount[ATP_TAB_VOICE_64_LEN];        /*email account id*/
    int             lUid;                                   /*id of email list*/
    char            acDate[ATP_TAB_VOICE_64_LEN];           /*date and time of email*/
    char            acEmailAddr[ATP_TAB_VOICE_64_LEN];      /*address of email sender*/
    char            acSubject[ATP_TAB_VOICE_128_LEN];       /*subject of email*/
    char            acText[ATP_TAB_EMAIL_TEXT_MAX_LEN];     /*text of email*/
}ATP_TAB_EMAIL_SUB_ST;

typedef enum tagATP_VDB_TABLE_ID_EN
{
    ATP_VDB_TABLE_MISSED_CALL,
    ATP_VDB_TABLE_OUTGOING_CALL,
    ATP_VDB_TABLE_INCOMING_CALL,
    ATP_VDB_TABLE_ALLCALL,
    ATP_VDB_TABLE_CONTACT,
    ATP_VDB_TABLE_INTERNAL_NAMES,
    ATP_VDB_TABLE_DECT_SYSTEM_SETTINGS,
    ATP_VDB_TABLE_LINE_SETTINGS,
    ATP_VDB_TABLE_LAN_MAP,
    ATP_VDB_TABLE_MOBILE,
    ATP_VDB_TABLE_ONLINECONTACT,      /*Added by kf39808 2011-06-07*/
    ATP_VDB_TABLE_DECT_MAIL,
    ATP_VDB_TABLE_DECT_MAILINTERVAL,
    ATP_VDB_TABLE_DECTRSS_INFO,
    ATP_VDB_TABLE_DECT_RSSINTERVAL,
	/* BEGIN: Added by zengjun 182276, 2011/5/30   问题单号:DT特性*/
	ATP_VDB_TABLE_CALLLIST,
    /* END:   Added by zengjun 182276, 2011/5/30 */
} ATP_VDB_TABLE_ID_EN;

typedef enum tagATP_VDB_RET_CODE_EN
{
    ATP_VDB_RET_OK,
    ATP_VDB_RET_PARA_ERR,
    ATP_VDB_RET_MEM_ERR,
    ATP_VDB_RET_SQL_ERR,
    ATP_VDB_RET_SYS_ERR
} ATP_VDB_RET_CODE_EN;

void *ATP_VDB_Init();
void *ATP_DummyVDB_Init();
int ATP_VDB_Save(void *pvVdbHdl);

// Get single entry from db by sequence number
int ATP_VDB_GetEntryByIdx(
                            void                    *pvVdbHdl,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            unsigned int            ulIdx);

// Get single entry from db by entry id
int ATP_VDB_GetEntryByEntryId(
                            void                    *pvVdbHdl,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            VDB_ENTRY_ID            llEntryId);

// Browse multiple entries from db
typedef int (*PfVdbBrowseEntryProc)(ATP_VDB_TABLE_ID_EN enTblId, void *pvEntry, void *pvExtraArg);
int ATP_VDB_BrowseEntries(
                            void                    *pvVdbHdl,
                            PfVdbBrowseEntryProc    pfBrowseProc,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            void                    *pvExtraArg,
                            const char              *pcSqlCondition,
                            int                     lRequestCnt,
                            int                     lStartIdx,
                            int                     *plTotalMatched);
int ATP_VDB_BrowseEntriesQMR(
                            void                    *pvVdbHdl,
                            PfVdbBrowseEntryProc    pfBrowseProc,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            void                    *pvExtraArg,
                            const char              *pcSqlCondition,
                            int                     lRequestCnt,
                            int                     lStartIdx,
                            int                     *plTotalMatched);

// Browse multiple entries from db,But this function support read entry fields.
int ATP_VDB_BrowseEntryId(
                            void                    *pvVdbHdl,
                            PfVdbBrowseEntryProc    pfBrowseProc,
                            ATP_VDB_TABLE_ID_EN     enTblId,
                            void                    *pvTblValue,
                            void                    *pvExtraArg,
                            const char              *pcSqlCondition);

// Modify the value of an entry, the value of id must be specified in pvTblValue
int ATP_VDB_ModifyEntry(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId, void *pvTblValue);

// Add a new entry, the value of newly allocated id will be set in pvTblValue
int ATP_VDB_AddEntry(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId, void *pvTblValue);

// Delete an entry by entry Id
int ATP_VDB_DeleteEntry(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId, VDB_ENTRY_ID llEntryId);

// Delete all entries in a table
int ATP_VDB_DeleteAllEntries(void *pvVdbHdl, ATP_VDB_TABLE_ID_EN enTblId);

// Print an entry
int ATP_VDB_UtilPrintEntry(void *pvTblValue, ATP_VDB_TABLE_ID_EN enTblId);
ATP_VDB_TABLE_QMR_ST * ATP_VDB_ReadQMRTable();
int ATP_VDB_WriteMRTable(void *pvVdbHdl, ATP_VDB_TABLE_MR_ST * pstVdbMRTable);
#endif

