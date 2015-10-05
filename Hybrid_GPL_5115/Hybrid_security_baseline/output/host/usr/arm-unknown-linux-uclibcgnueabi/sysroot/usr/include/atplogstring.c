/*******************************************************************************
Edit History of Huawei: 
<修改人>     <修改时间>    <问题单号>          <修改描述>
z00159386    2013/01/09    DTS2013010908899    [TELUS DDNS]上报公司名称和产品名称不正确
*******************************************************************************/
/*
  *     Product Language
  */
const VOS_UINT32 g_apcProductLanguages[] = {
    ATP_LOG_LANG_ENGLISH,
    ATP_LOG_LANG_CHINESE
};

const VOS_UINT32 g_ulNumOfLanguages = (VOS_UINT32)ATP_ARRAY_LEN(g_apcProductLanguages);

/*
  *     Log Type Definitions
  */
static const ATP_UTIL_STR_IDX_MAP_ST m_astLogTypeStr_en[] =
{
    {
        "Kernel",
        1 << 0
    },
    {
        "User Level",
        1 << 1
    },
    {
        "Daemon",
        1 << 3
    },
    {
        "Security",
        1 << 4
    },
    {
        "Syslogd",
        1 << 5
    },
    {
        "openwifi",
        1 << 6
    },
    {
        "alert",
        1 << 14
    },
    {
        "incall",
        1 << 15
    },
    {
        "outcall",
        1 << 16
    },
    {
        "wan",
        1 << 17
    },
    {
        "portforwarding",
        1 << 18
    },
    {
        "dsl",
        1 << 20
    },
    {
        "ppp",
        1 << 21
    },
    {
        "firewall",
        1 << 22
    },
    {
        "voice",
        1 << 23
    },
    {
        NULL,
        0
    }
};

static const ATP_UTIL_STR_IDX_MAP_ST m_astLogTypeStr_zh[] =
{
    {
        "Kernel",
        1 << 0
    },
    {
        "User Level",
        1 << 1
    },
    {
        "Daemon",
        1 << 3
    },
    {
        "Security",
        1 << 4
    },
    {
        "Syslogd",
        1 << 5
    },
    {
        "openwifi",
        1 << 6
    },
    {
        "alert",
        1 << 14
    },
    {
        "incall",
        1 << 15
    },
    {
        "outcall",
        1 << 16
    },
    {
        "wan",
        1 << 17
    },
    {
        "portforwarding",
        1 << 18
    },
    {
        "dsl",
        1 << 20
    },
    {
        "ppp",
        1 << 21
    },
    {
        "firewall",
        1 << 22
    },
    {
        "voice",
        1 << 23
    },
    {
        NULL,
        0
    }
};

/*
  *     Log Type Definitions
  */
static const VOS_CHAR *m_apcLogLevelStr_en[] =
{
    "Emergency",
    "Alert",
    "Critical",
    "Error",
    "Warning",
    "Notice",
    "Info",
    "Debug"
};

static const VOS_CHAR *m_apcLogLevelStr_zh[] =
{
    "致命",
    "告警",
    "危急",
    "错误",
    "警告",
    "提示",
    "信息",
    "调试"
};

const ATP_LOG_LOGITEM_INFO_ST g_pstLogItems[] =
{
    {
        ATP_SYS_LOG_UPG_BEGIN, /* "Upgrading beginning." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL, /* "Upgrading fail:" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL_FILE_LARGE, /* "Upgrading fail:File too large." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL_VOICE_BUSY, /* "Upgrading fail:Voice busy." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL_SERV_CONFLICT, /* "Upgrading fail:Service conflict." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL_DL_ERR, /* "Upgrading fail:Download failure." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL_FMT_ERR, /* "Upgrading fail:File format error." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_FAIL_OTHER_ERR, /* "Upgrading fail:Other error." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_IMG_OK, /* "Image validation is successful." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_END, /* "Upgrading finished: $0." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_UPG_MODULE_NAME, /* "Upgrading module name: $0." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_MODULE_DATE, /* "Upgrading module date: $0." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_MODULE_VER, /* "Upgrading module version: $0." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_CFG_BEGIN, /* "Upgrade config beginning." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_CFG_DL_FAIL, /* "Upgrade config fail:Download failure" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_CFGFILE_FMT_FAIL, /* "Upgrade config fail:File format failure" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_CFG_SUC, /* "Upgrade config success." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_UL_CFGFILE_BEGIN, /* "Upload config file beginning..." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_UL_CFGFILE_FAIL, /* "Upload config file fail, upload failure." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_UL_CFGFILE_SUC, /* "Upload config file success." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_UL_LOGFILE_BEGIN, /* "Upload log file beginning..." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_UL_LOGFILE_FAIL, /* "Failed to upload log file to $0 ." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_UL_LOGFILE_SUC, /* "Upload log file success." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_UPG_SIGNATURE_ERR, /* "Upgrading error, signature error." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGIN_OK, /* "User $0 login from $1 successfully." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGIN_FAIL, /* "User $0 from $1 login failure." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGOUT, /* "User $0 logout from $1." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGIN_FAIL_USERNAME_ERR, /* "User $0 login from $1 failed. Username error." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGIN_FAIL_PASSWORD_ERR, /* "User $0 login from $1 failed. Password error." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGIN_FAIL_OVER_MAX_TIME, /* "User $0 from $1 login failed. Times of login failure overstep the max value." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WEB_UPDATE_PASSWORD, /* "Update web password of user $0!" */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WEB_LOGIN_FAIL_UNLOCK, /* "User $0 from $1 login failed unlocked." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_FACTORY_RESET, /* "Factory reset." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_REBOOT_LAN, /* "Reboot from the LAN side." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_REBOOT_WAN, /* "Reboot from the WAN side." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_CFM_BACKUP_CFGFILE_SUC, /* "Succeed to Backup config file." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_CFM_BACKUP_CFGFILE_FAIL, /* "Fail to Backup config file." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_CFM_RECOVER_CFGFILE_SUC, /* "Succeed to recover config file." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_CFM_RECOVER_CFGFILE_FAIL, /* "Fail to recover config file." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_CFM_CFGFILE_ERR, /* "Read config file error." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_ERROR,
        1
    },
    {
        ATP_VOICE_LOG_REG_FAIL_DNS, /* "Registering with VoIP number $0 to Register server $1 failed: Reason: DNS error." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_REG_FAIL_TIMEOUT, /* "Registering with VoIP number $0 to Register server $1 failed: Reason: no answer from remote station received." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_REG_FAIL_AUTH, /* "Registering with VoIP number $0 to Register server $1 failed: Reason: VoIP credentials are incorrect." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_REG_FAIL_OTHER, /* "Registering with VoIP number $0 to Register server $1 failed: Reason: $2." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_REG_SUC, /* "Registering with VoIP number $0 to Register server $1 successfully." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_UNREG_FAIL, /* "Un-Registering with VoIP number $0 to Register server $1 failed: Reason: $2." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_UNREG_SUC, /* "Un-Registering with VoIP number $0 to Register server $1 successfully." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_OUTGOING_CALL, /* "Outgoing call: Ports:$0, Caller:$1, Callee:$2, Line type:$3, Duration:$4, Media type:$5." */
        ATP_LOG_TYPE_VOICE << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_INCOMING_CALL, /* "Incoming call: Ports:$0, Caller:$1, Callee:$2, Line type:$3, Duration:$4, Media type:$5." */
        ATP_LOG_TYPE_VOICE << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_ADD_NEW_SIP_LINE_INFO, /* "Add New Sip Line information." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_ADD_NEW_SIP_PROF_INFO, /* "Add New Sip Profile information." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_UPDATE_SIP_LINE_INFO, /* "Update Sip Line information." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_UPDATE_SIP_PROF_INFO, /* "Update Sip Profile information." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_DEL_SIP_LINE_INFO, /* "Delete Sip Line information." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_DEL_SIP_PROF_INFO, /* "Delete Sip Profile information." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_VOICE_LOG_MSG_RCV, /* "New Message: Caller:$0, Callee:$1." */
        ATP_LOG_TYPE_VOICE << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_UPNP_TR064_AUTH_FAIL, /* "UPNP TR064 $0 authorization error." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_UPNP_TR064_AUTH_SUC , /* "UPNP TR064 $0 authorization OK." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_UPNP_TR064_ACTION_SUC, /* "Action $0 execute by UPNP/TR064 Success." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_UPNP_TR064_ACTION_FAIL, /* "Action $0 execute by UPNP/TR064 Fail, Errorcode:$1." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_SET_BY_TR069_OK, /* "CWMP inform message: Set parameter ok." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_DLD_FILE_OK, /* "CWMP inform message: Download finish." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_ULD_FILE_OK, /* "CWMP inform message: upload finish." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_AUTH_ERR, /* "ACS $0 connect to CPE failed due to authentication." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_ACS_CONN_TO_CPE_FAIL, /* "ACS $0 make a connection request to the CPE failed." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_ACS_CONN_TO_CPE_SUC, /* "ACS $0 make a connection request to the CPE successful." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_INFROM_EVENT, /* "CWMP inform message: event: $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_INFROM_GET_ACTIONS, /* "CWMP Inform message: get actions." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_INFROM_SET_ACTIONS, /* "CWMP Inform message: set actions." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_UPDATE_CERT_FILE, /* "CWMP: Update certification files: $0" */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_PERIOD_REPORT, /* "CWMP inform message: period report." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_UPDATE_URL, /* "CWMP inform message: url is changed." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_INFROM_ACS_CONN_OK, /* "CWMP inform message: acs connection ok." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_INFORM_SET_PARA_DONE, /* "CWMP inform message: parameter change." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_POST_INFORM_SUC, /* "CWMP:Cwmp post inform success." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CWMP_POST_INFORM_FAIL, /* "CWMP:Cwmp post inform failed." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONNECTED, /* "WAN connection $0 connected." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WAN_DISCONNETED, /* "WAN connection $0 disconnected.($1)" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONFIG_IP_CONN_DEL, /* "WAN configuration: Delete IP connection : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONFIG_IP_CONN_ADD, /* "WAN configuration: Add IP connection : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONFIG_IP_CONN_SET, /* "WAN configuration: Modify IP connection : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_DEL, /* "WAN configuration: Delete PPP connection : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_ADD, /* "WAN configuration: ADD PPP connection : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_SET, /* "WAN configuration: Modify PPP connection : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_WAN_UMTS_DIAL_SUCCESS, /* "UMTS connection success!" */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_WAN_UMTS_DIAL_FAIL, /* "UMTS connection exception:PPP dial fail!" */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_IGMP_LEAVE_INFO, /* "Message type: Leave, Reporter: $0, MAC: $1, Group: $2, version: 2!" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_IGMP_REPORT_INFO_MODE, /* "Message type: Report, Reporter: $0, MAC: $1, Group: $2, Version: 3, mode: $3!" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_IGMP_REPORT_INFO, /* "Message type: Report, Reporter: $0, MAC: $1, Group: $2, Version: 2!" */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_IGMP_QUERY_INFO, /* "Message type: Query, Querier: $0, Group: $1, version: $2." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_CHANGE_TO_LEVEL, /* "Firewall changed to Level:$0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_WARNING,
        1
    },
    {
        ATP_SYS_LOG_FW_FITER_LEVEL_ADD, /* "A filter level [$0] has been added, whose instance ID is $1." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_FILTER_LEVEL_MOD, /* "The filter level [$0] has been modified, whose instance ID is $1." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_NEW_ACC_CTRL_ENTRY_ADD, /* "A new Access Control entry has been added, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_ACC_CTRL_ENTRY_MOD, /* "An Access Control entry has been modified, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_ACC_CTRL_ENTRY_DEL, /* "An Access Control entry has been deleted, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_NEW_IP_FILTER_ENTRY_ADD, /* "A new IP filter entry has been added, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_IP_FILTER_ENTRY_MOD, /* "An IP filter entry has been modified, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_NEW_URL_FILTER_BLIST_ADD, /* "A new URL filter black list has been added, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_NEW_URL_FILTER_BLIST_MOD, /* "An URL filter black list has been modified, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_DOS_ENTRY_MOD, /* "The DOS entry has been modified." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_FILTER_LEVEL_DEL, /* "The filter level [$0] has been deleted." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_IP_FILTER_ENTRY_DEL, /* "An IP filter entry has been deleted, whose Instance ID Is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_KEYWORD_ADD, /* "A new Keyword added, Instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_KEYWORD_MOD, /* "The keyword has been modified, Instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_KEYWORD_DEL, /* "The keyword has been deleted, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_URL_FILTER_BLIST_DEL, /* "A URL filter black list has been deleted, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_URL_FILTER_WLIST_DEL, /* "A URL filter white list has been deleted, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_NEW_URL_FILTER_WLIST_ADD, /* "A new URL filter white list has been added, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_FW_URL_FILTER_WLIST_MOD, /* "An URL filter white list has been modified, whose instance ID is $0." */
        ATP_LOG_TYPE_FIREWALL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CLI_LOGIN_OK, /* "User $0 login success by $1 $2." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CLI_LOGOUT, /* "User $0 logout by $1 $2." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CLI_LOGIN_ERR, /* "User $0 login fail by $1 $2." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CLI_LOGIN_LOCKED, /* "User $0 login by $1 authorization failed after trying 3 times $2." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_CLI_LOGIN_UNLOCKED, /* "User $0 login by $1 unlocked $2." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_FLASH_READ_ERR, /* "Read flash error: $0." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_ERROR,
        1
    },
    {
        ATP_SYS_LOG_BHAL_FLASH_WRITE_ERR, /* "Write flash error: $0." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_ERROR,
        1
    },
    /* <DTS2013033004041 j81004134 2013/4/1 begin */
    {
        ATP_SYS_LOG_BHAL_RESTORE_DEFAULT_ALL,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_ATCMD_BLOCKED,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_EQUIP_COMMAND_REBOOT,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_RESTART_MSM,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_REBOOT_AFTER_UPGRADE,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_TIMEREBOOT,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_VOPCLIENT_REBOOT,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_BHAL_NAT_TYPE_CHANGED,
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    /* DTS2013033004041 j81004134 2013/4/1 end> */
    {
        ATP_SYS_LOG_XDSL_DSL_CONN, /* "DSL connection is active." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_XDSL_DSL_DISCONN, /* "DSL connection is disactive." */
        ATP_LOG_TYPE_SYS_DEAMON << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_NAT_DMZ_HOST_IP_ADD, /* "Add NAT DMZ host ip : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_NAT_DMZ_HOST_IP_DEL, /* "Delete NAT DMZ host ip : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_NAT_DMZ_HOST_IP_MOD, /* "Change NAT DMZ host ip : $0." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_LOG_CONFIG_SERVER, /* "Update the log configuration." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_LOG_CLEAR, /* "All logs are cleared by user($0)." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
/* <DTS2013010908899  z00159386  20130109  begin */
#ifdef SUPPORT_ATP_TELUS
    {
        ATP_SYS_LOG_DDNS_CLIENT_CONNECT_ERR, /* "DynDNS-Fehler: Der angegebene Domainname kann nicht aufgelst werden." */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_ERROR,
        1
    },
    {
        ATP_SYS_LOG_DDNS_REPLY_SUC, /* DDNS Log($0) */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_NOTICE,
        1
    },
    {
        ATP_SYS_LOG_DDNS_REPLY_FAIL, /* DDNS Log($0) */
        ATP_LOG_TYPE_USER_LEVEL << ATP_LOG_TYPE_OFFSET | ATP_LOG_LEVEL_ERROR,
        1
    },
#endif /* SUPPORT_ATP_TELUS */
/* DTS2013010908899  z00159386  20130109  end> */
    { 0, 0, 0 }
};

/*
 * Log String Definitions
 */
static const VOS_CHAR *m_apcLogStr_EN[] =
{
    "Upgrading beginning.", /* ATP_SYS_LOG_UPG_BEGIN */
    "Upgrading fail:", /* ATP_SYS_LOG_UPG_FAIL */
    "Upgrading fail:File too large.", /* ATP_SYS_LOG_UPG_FAIL_FILE_LARGE */
    "Upgrading fail:Voice busy.", /* ATP_SYS_LOG_UPG_FAIL_VOICE_BUSY */
    "Upgrading fail:Service conflict.", /* ATP_SYS_LOG_UPG_FAIL_SERV_CONFLICT */
    "Upgrading fail:Download failure.", /* ATP_SYS_LOG_UPG_FAIL_DL_ERR */
    "Upgrading fail:File format error.", /* ATP_SYS_LOG_UPG_FAIL_FMT_ERR */
    "Upgrading fail:Other error.", /* ATP_SYS_LOG_UPG_FAIL_OTHER_ERR */
    "Image validation is successful.", /* ATP_SYS_LOG_UPG_IMG_OK */
    "Upgrading finished: $0.", /* ATP_SYS_LOG_UPG_END */
    "Upgrading module name: $0.", /* ATP_SYS_LOG_UPG_MODULE_NAME */
    "Upgrading module date: $0.", /* ATP_SYS_LOG_UPG_MODULE_DATE */
    "Upgrading module version: $0.", /* ATP_SYS_LOG_UPG_MODULE_VER */
    "Upgrade config beginning.", /* ATP_SYS_LOG_UPG_CFG_BEGIN */
    "Upgrade config fail:Download failure", /* ATP_SYS_LOG_UPG_CFG_DL_FAIL */
    "Upgrade config fail:File format failure", /* ATP_SYS_LOG_UPG_CFGFILE_FMT_FAIL */
    "Upgrade config success.", /* ATP_SYS_LOG_UPG_CFG_SUC */
    "Upload config file beginning...", /* ATP_SYS_LOG_UPG_UL_CFGFILE_BEGIN */
    "Upload config file fail, upload failure.", /* ATP_SYS_LOG_UPG_UL_CFGFILE_FAIL */
    "Upload config file success.", /* ATP_SYS_LOG_UPG_UL_CFGFILE_SUC */
    "Upload log file beginning...", /* ATP_SYS_LOG_UPG_UL_LOGFILE_BEGIN */
    "Failed to upload log file to $0 .", /* ATP_SYS_LOG_UPG_UL_LOGFILE_FAIL */
    "Upload log file success.", /* ATP_SYS_LOG_UPG_UL_LOGFILE_SUC */
    "Upgrading error, signature error.", /* ATP_SYS_LOG_UPG_SIGNATURE_ERR */
    "User $0 login from $1 successfully.", /* ATP_SYS_LOG_WEB_LOGIN_OK */
    "User $0 from $1 login failure.", /* ATP_SYS_LOG_WEB_LOGIN_FAIL */
    "User $0 logout from $1.", /* ATP_SYS_LOG_WEB_LOGOUT */
    /* <DTS2013102809830 j81004134 2013/10/30 begin */
    "Login from $0 failed. Username error.", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_USERNAME_ERR */
    /* DTS2013102809830 j81004134 2013/10/30 end> */
    "User $0 login from $1 failed. Password error.", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_PASSWORD_ERR */
    "User $0 from $1 login failed. Times of login failure overstep the max value.", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_OVER_MAX_TIME */
    "Update web password of user $0!", /* ATP_SYS_LOG_WEB_UPDATE_PASSWORD */
    "User $0 from $1 login failed unlocked.", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_UNLOCK */
    "Factory reset.", /* ATP_SYS_LOG_FACTORY_RESET */
    "Reboot from the LAN side.", /* ATP_SYS_LOG_REBOOT_LAN */
    "Reboot from the WAN side.", /* ATP_SYS_LOG_REBOOT_WAN */
    "Succeed to Backup config file.", /* ATP_SYS_LOG_CFM_BACKUP_CFGFILE_SUC */
    "Fail to Backup config file.", /* ATP_SYS_LOG_CFM_BACKUP_CFGFILE_FAIL */
    "Succeed to recover config file.", /* ATP_SYS_LOG_CFM_RECOVER_CFGFILE_SUC */
    "Fail to recover config file.", /* ATP_SYS_LOG_CFM_RECOVER_CFGFILE_FAIL */
    "Read config file error.", /* ATP_SYS_LOG_CFM_CFGFILE_ERR */
    "Registering with VoIP number $0 to Register server $1 failed: Reason: DNS error.", /* ATP_VOICE_LOG_REG_FAIL_DNS */
    "Registering with VoIP number $0 to Register server $1 failed: Reason: no answer from remote station received.", /* ATP_VOICE_LOG_REG_FAIL_TIMEOUT */
    "Registering with VoIP number $0 to Register server $1 failed: Reason: VoIP credentials are incorrect.", /* ATP_VOICE_LOG_REG_FAIL_AUTH */
    "Registering with VoIP number $0 to Register server $1 failed: Reason: $2.", /* ATP_VOICE_LOG_REG_FAIL_OTHER */
    "Registering with VoIP number $0 to Register server $1 successfully.", /* ATP_VOICE_LOG_REG_SUC */
    "Un-Registering with VoIP number $0 to Register server $1 failed: Reason: $2.", /* ATP_VOICE_LOG_UNREG_FAIL */
    "Un-Registering with VoIP number $0 to Register server $1 successfully.", /* ATP_VOICE_LOG_UNREG_SUC */
    "Outgoing call: Ports:$0, Caller:$1, Callee:$2, Line type:$3, Duration:$4, Media type:$5.", /* ATP_VOICE_LOG_OUTGOING_CALL */
    "Incoming call: Ports:$0, Caller:$1, Callee:$2, Line type:$3, Duration:$4, Media type:$5.", /* ATP_VOICE_LOG_INCOMING_CALL */
    "Add New Sip Line information.", /* ATP_VOICE_LOG_ADD_NEW_SIP_LINE_INFO */
    "Add New Sip Profile information.", /* ATP_VOICE_LOG_ADD_NEW_SIP_PROF_INFO */
    "Update Sip Line information.", /* ATP_VOICE_LOG_UPDATE_SIP_LINE_INFO */
    "Update Sip Profile information.", /* ATP_VOICE_LOG_UPDATE_SIP_PROF_INFO */
    "Delete Sip Line information.", /* ATP_VOICE_LOG_DEL_SIP_LINE_INFO */
    "Delete Sip Profile information.", /* ATP_VOICE_LOG_DEL_SIP_PROF_INFO */
    "New Message: Caller:$0, Callee:$1.", /* ATP_VOICE_LOG_MSG_RCV */
    "UPNP TR064 $0 authorization error.", /* ATP_SYS_LOG_UPNP_TR064_AUTH_FAIL */
    "UPNP TR064 $0 authorization OK.", /* ATP_SYS_LOG_UPNP_TR064_AUTH_SUC  */
    "Action $0 execute by UPNP/TR064 Success.", /* ATP_SYS_LOG_UPNP_TR064_ACTION_SUC */
    "Action $0 execute by UPNP/TR064 Fail, Errorcode:$1.", /* ATP_SYS_LOG_UPNP_TR064_ACTION_FAIL */
    "CWMP inform message: Set parameter ok.", /* ATP_SYS_LOG_CWMP_SET_BY_TR069_OK */
    "CWMP inform message: Download finish.", /* ATP_SYS_LOG_CWMP_DLD_FILE_OK */
    "CWMP inform message: upload finish.", /* ATP_SYS_LOG_CWMP_ULD_FILE_OK */
    "ACS $0 connect to CPE failed due to authentication.", /* ATP_SYS_LOG_CWMP_AUTH_ERR */
    "ACS $0 make a connection request to the CPE failed.", /* ATP_SYS_LOG_CWMP_ACS_CONN_TO_CPE_FAIL */
    "ACS $0 make a connection request to the CPE successful.", /* ATP_SYS_LOG_CWMP_ACS_CONN_TO_CPE_SUC */
    "CWMP inform message: event: $0.", /* ATP_SYS_LOG_CWMP_INFROM_EVENT */
    "CWMP Inform message: get actions.", /* ATP_SYS_LOG_CWMP_INFROM_GET_ACTIONS */
    "CWMP Inform message: set actions.", /* ATP_SYS_LOG_CWMP_INFROM_SET_ACTIONS */
    "CWMP: Update certification files: $0", /* ATP_SYS_LOG_CWMP_UPDATE_CERT_FILE */
    "CWMP inform message: period report.", /* ATP_SYS_LOG_CWMP_PERIOD_REPORT */
    "CWMP inform message: url is changed.", /* ATP_SYS_LOG_CWMP_UPDATE_URL */
    "CWMP inform message: acs connection ok.", /* ATP_SYS_LOG_CWMP_INFROM_ACS_CONN_OK */
    "CWMP inform message: parameter change.", /* ATP_SYS_LOG_CWMP_INFORM_SET_PARA_DONE */
    "CWMP:Cwmp post inform success.", /* ATP_SYS_LOG_CWMP_POST_INFORM_SUC */
    "CWMP:Cwmp post inform failed.", /* ATP_SYS_LOG_CWMP_POST_INFORM_FAIL */
    "WAN connection $0 connected.", /* ATP_SYS_LOG_WAN_CONNECTED */
    "WAN connection $0 disconnected.($1)", /* ATP_SYS_LOG_WAN_DISCONNETED */
    "WAN configuration: Delete IP connection : $0.", /* ATP_SYS_LOG_WAN_CONFIG_IP_CONN_DEL */
    "WAN configuration: Add IP connection : $0.", /* ATP_SYS_LOG_WAN_CONFIG_IP_CONN_ADD */
    "WAN configuration: Modify IP connection : $0.", /* ATP_SYS_LOG_WAN_CONFIG_IP_CONN_SET */
    "WAN configuration: Delete PPP connection : $0.", /* ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_DEL */
    "WAN configuration: ADD PPP connection : $0.", /* ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_ADD */
    "WAN configuration: Modify PPP connection : $0.", /* ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_SET */
    "UMTS connection success!", /* ATP_SYS_LOG_WAN_UMTS_DIAL_SUCCESS */
    "UMTS connection exception:PPP dial fail!", /* ATP_SYS_LOG_WAN_UMTS_DIAL_FAIL */
    "Message type: Leave, Reporter: $0, MAC: $1, Group: $2, version: 2!", /* ATP_SYS_LOG_IGMP_LEAVE_INFO */
    "Message type: Report, Reporter: $0, MAC: $1, Group: $2, Version: 3, mode: $3!", /* ATP_SYS_LOG_IGMP_REPORT_INFO_MODE */
    "Message type: Report, Reporter: $0, MAC: $1, Group: $2, Version: 2!", /* ATP_SYS_LOG_IGMP_REPORT_INFO */
    "Message type: Query, Querier: $0, Group: $1, version: $2.", /* ATP_SYS_LOG_IGMP_QUERY_INFO */
    "Firewall changed to Level:$0.", /* ATP_SYS_LOG_FW_CHANGE_TO_LEVEL */
    "A filter level [$0] has been added, whose instance ID is $1.", /* ATP_SYS_LOG_FW_FITER_LEVEL_ADD */
    "The filter level [$0] has been modified, whose instance ID is $1.", /* ATP_SYS_LOG_FW_FILTER_LEVEL_MOD */
    "A new Access Control entry has been added, whose instance ID is $0.", /* ATP_SYS_LOG_FW_NEW_ACC_CTRL_ENTRY_ADD */
    "An Access Control entry has been modified, whose instance ID is $0.", /* ATP_SYS_LOG_FW_ACC_CTRL_ENTRY_MOD */
    "An Access Control entry has been deleted, whose instance ID is $0.", /* ATP_SYS_LOG_FW_ACC_CTRL_ENTRY_DEL */
    "A new IP filter entry has been added, whose instance ID is $0.", /* ATP_SYS_LOG_FW_NEW_IP_FILTER_ENTRY_ADD */
    "An IP filter entry has been modified, whose instance ID is $0.", /* ATP_SYS_LOG_FW_IP_FILTER_ENTRY_MOD */
    "A new URL filter black list has been added, whose instance ID is $0.", /* ATP_SYS_LOG_FW_NEW_URL_FILTER_BLIST_ADD */
    "An URL filter black list has been modified, whose instance ID is $0.", /* ATP_SYS_LOG_FW_NEW_URL_FILTER_BLIST_MOD */
    "The DOS entry has been modified.", /* ATP_SYS_LOG_FW_DOS_ENTRY_MOD */
    "The filter level [$0] has been deleted.", /* ATP_SYS_LOG_FW_FILTER_LEVEL_DEL */
    "An IP filter entry has been deleted, whose Instance ID Is $0.", /* ATP_SYS_LOG_FW_IP_FILTER_ENTRY_DEL */
    "A new Keyword added, Instance ID is $0.", /* ATP_SYS_LOG_FW_KEYWORD_ADD */
    "The keyword has been modified, Instance ID is $0.", /* ATP_SYS_LOG_FW_KEYWORD_MOD */
    "The keyword has been deleted, whose instance ID is $0.", /* ATP_SYS_LOG_FW_KEYWORD_DEL */
    "A URL filter black list has been deleted, whose instance ID is $0.", /* ATP_SYS_LOG_FW_URL_FILTER_BLIST_DEL */
    "A URL filter white list has been deleted, whose instance ID is $0.", /* ATP_SYS_LOG_FW_URL_FILTER_WLIST_DEL */
    "A new URL filter white list has been added, whose instance ID is $0.", /* ATP_SYS_LOG_FW_NEW_URL_FILTER_WLIST_ADD */
    "An URL filter white list has been modified, whose instance ID is $0.", /* ATP_SYS_LOG_FW_URL_FILTER_WLIST_MOD */
    "User $0 login success by $1 $2.", /* ATP_SYS_LOG_CLI_LOGIN_OK */
    "User $0 logout by $1 $2.", /* ATP_SYS_LOG_CLI_LOGOUT */
    "User $0 login fail by $1 $2.", /* ATP_SYS_LOG_CLI_LOGIN_ERR */
    "User $0 login by $1 authorization failed after trying 3 times $2.", /* ATP_SYS_LOG_CLI_LOGIN_LOCKED */
    "User $0 login by $1 unlocked $2.", /* ATP_SYS_LOG_CLI_LOGIN_UNLOCKED */
    "Read flash error: $0.", /* ATP_SYS_LOG_BHAL_FLASH_READ_ERR */
    "Write flash error: $0.", /* ATP_SYS_LOG_BHAL_FLASH_WRITE_ERR */
    /* <DTS2013033004041 j81004134 2013/4/1 begin */
    "BHAL status: 2",    /** ATP_SYS_LOG_BHAL_RESTORE_DEFAULT_ALL */
    "BHAL status: 3",    /** ATP_SYS_LOG_BHAL_ATCMD_BLOCKED */
    "BHAL status: 4",    /** ATP_SYS_LOG_BHAL_EQUIP_COMMAND_REBOOT */
    "BHAL status: 5",    /** ATP_SYS_LOG_BHAL_RESTART_MSM */
    "BHAL status: 6",    /** ATP_SYS_LOG_BHAL_REBOOT_AFTER_UPGRADE */
    "BHAL status: 7",    /** ATP_SYS_LOG_BHAL_TIMEREBOOT */
    "BHAL status: 8",    /** ATP_SYS_LOG_BHAL_VOPCLIENT_REBOOT */
    "BHAL status: 9",    /** ATP_SYS_LOG_BHAL_NAT_TYPE_CHANGED */
    /* DTS2013033004041 j81004134 2013/4/1 end> */
    "DSL connection is active.", /* ATP_SYS_LOG_XDSL_DSL_CONN */
    "DSL connection is disactive.", /* ATP_SYS_LOG_XDSL_DSL_DISCONN */
    "Add NAT DMZ host ip : $0.", /* ATP_SYS_LOG_NAT_DMZ_HOST_IP_ADD */
    "Delete NAT DMZ host ip : $0.", /* ATP_SYS_LOG_NAT_DMZ_HOST_IP_DEL */
    "Change NAT DMZ host ip : $0.", /* ATP_SYS_LOG_NAT_DMZ_HOST_IP_MOD */
    "Update the log configuration.", /* ATP_SYS_LOG_LOG_CONFIG_SERVER */
/* <DTS2013010908899  z00159386  20130109  begin */
#ifdef SUPPORT_ATP_TELUS
    "All logs are cleared by user($0).", /* ATP_SYS_LOG_LOG_CLEAR */
	"DDNS Log : connection $0：$1 error $2.", /* ATP_SYS_LOG_DDNS_CLIENT_CONNECT_ERR */
    "DDNS Log : request successful $0.", /* ATP_SYS_LOG_DDNS_REPLY_SUC */
    "DDNS Log : DDNS Update fail: $0 $1" /* ATP_SYS_LOG_DDNS_REPLY_FAIL */
#else
    "All logs are cleared by user($0)." /* ATP_SYS_LOG_LOG_CLEAR */
#endif /* SUPPORT_ATP_TELUS */
/* DTS2013010908899  z00159386  20130109  end> */
};

/*
 * Log String Definitions
 */
static const VOS_CHAR *m_apcLogStr_ZH[] =
{
    "开始升级。", /* ATP_SYS_LOG_UPG_BEGIN */
    "升级失败：", /* ATP_SYS_LOG_UPG_FAIL */
    "升级失败：文件过大。", /* ATP_SYS_LOG_UPG_FAIL_FILE_LARGE */
    "升级失败：语音业务忙。", /* ATP_SYS_LOG_UPG_FAIL_VOICE_BUSY */
    "升级失败：业务冲突。", /* ATP_SYS_LOG_UPG_FAIL_SERV_CONFLICT */
    "升级失败：文件下载失败。", /* ATP_SYS_LOG_UPG_FAIL_DL_ERR */
    "升级失败：文件格式错误。", /* ATP_SYS_LOG_UPG_FAIL_FMT_ERR */
    "升级失败：其他错误。", /* ATP_SYS_LOG_UPG_FAIL_OTHER_ERR */
    "镜像文件校验成功。", /* ATP_SYS_LOG_UPG_IMG_OK */
    "升级完成：$0。", /* ATP_SYS_LOG_UPG_END */
    "升级模块名：$0。", /* ATP_SYS_LOG_UPG_MODULE_NAME */
    "升级模块日期：$0。", /* ATP_SYS_LOG_UPG_MODULE_DATE */
    "升级模块版本号：$0。", /* ATP_SYS_LOG_UPG_MODULE_VER */
    "升级配置文件开始。", /* ATP_SYS_LOG_UPG_CFG_BEGIN */
    "升级配置文件失败：下载失败。", /* ATP_SYS_LOG_UPG_CFG_DL_FAIL */
    "升级配置文件失败：文件格式错误。", /* ATP_SYS_LOG_UPG_CFGFILE_FMT_FAIL */
    "升级配置文件成功。", /* ATP_SYS_LOG_UPG_CFG_SUC */
    "上传配置文件开始……", /* ATP_SYS_LOG_UPG_UL_CFGFILE_BEGIN */
    "上传配置文件失败。", /* ATP_SYS_LOG_UPG_UL_CFGFILE_FAIL */
    "上传配置文件成功。", /* ATP_SYS_LOG_UPG_UL_CFGFILE_SUC */
    "上传日志文件开始……", /* ATP_SYS_LOG_UPG_UL_LOGFILE_BEGIN */
    "上传日志文件到 $0 失败。", /* ATP_SYS_LOG_UPG_UL_LOGFILE_FAIL */
    "上传日志文件成功。", /* ATP_SYS_LOG_UPG_UL_LOGFILE_SUC */
    "升级签名失败。", /* ATP_SYS_LOG_UPG_SIGNATURE_ERR */
    "用户$0从$1成功登录。", /* ATP_SYS_LOG_WEB_LOGIN_OK */
    "用户$0从$1登录失败。", /* ATP_SYS_LOG_WEB_LOGIN_FAIL */
    "用户$0从$1退出登录。", /* ATP_SYS_LOG_WEB_LOGOUT */
    "用户$0从$1登录，认证失败! 用户名错误。", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_USERNAME_ERR */
    "用户$0从$1登录，认证失败! 密码错误。", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_PASSWORD_ERR */
    "用户$0从$1登录页面连续错误次数超过最大值。", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_OVER_MAX_TIME */
    "更新帐号$0的页面密码!", /* ATP_SYS_LOG_WEB_UPDATE_PASSWORD */
    "用户$0从$1登录页面连续错误解锁定。", /* ATP_SYS_LOG_WEB_LOGIN_FAIL_UNLOCK */
    "恢复出厂设置。", /* ATP_SYS_LOG_FACTORY_RESET */
    "用户从LAN侧重启设备。", /* ATP_SYS_LOG_REBOOT_LAN */
    "用户从WAN侧重启设备。", /* ATP_SYS_LOG_REBOOT_WAN */
    "备份配置文件完成。", /* ATP_SYS_LOG_CFM_BACKUP_CFGFILE_SUC */
    "备份配置文件失败。", /* ATP_SYS_LOG_CFM_BACKUP_CFGFILE_FAIL */
    "恢复配置文件完成。", /* ATP_SYS_LOG_CFM_RECOVER_CFGFILE_SUC */
    "恢复配置文件失败。", /* ATP_SYS_LOG_CFM_RECOVER_CFGFILE_FAIL */
    "配置文件读取发生错误。", /* ATP_SYS_LOG_CFM_CFGFILE_ERR */
    "SIP 帐号$0注册服务器$1失败:DNS 错误。", /* ATP_VOICE_LOG_REG_FAIL_DNS */
    "SIP 帐号$0注册服务器$1失败:没有从远端服务器收到回应。", /* ATP_VOICE_LOG_REG_FAIL_TIMEOUT */
    "SIP帐号$0注册服务器$1失败:VOIP提供的鉴权信息错误。", /* ATP_VOICE_LOG_REG_FAIL_AUTH */
    "SIP 帐号$0注册服务器$1失败:错误码为$2。", /* ATP_VOICE_LOG_REG_FAIL_OTHER */
    "SIP帐号$0注册服务器$1成功。", /* ATP_VOICE_LOG_REG_SUC */
    "SIP帐号$0注销服务器$1失败:错误码为$2。", /* ATP_VOICE_LOG_UNREG_FAIL */
    "SIP帐号$0注销服务器$1成功。", /* ATP_VOICE_LOG_UNREG_SUC */
    "去电:话机端口号:$0 主叫:$1 被叫:$2 线路类型:$3 通话时长:$4 媒体类型:$5。", /* ATP_VOICE_LOG_OUTGOING_CALL */
    "来电:话机端口号:$0 主叫:$1 被叫:$2 线路类型:$3 通话时长:$4 媒体类型:$5。", /* ATP_VOICE_LOG_INCOMING_CALL */
    "新增SIP帐号信息。", /* ATP_VOICE_LOG_ADD_NEW_SIP_LINE_INFO */
    "新增SIP服务器信息。", /* ATP_VOICE_LOG_ADD_NEW_SIP_PROF_INFO */
    "更新SIP帐号信息。", /* ATP_VOICE_LOG_UPDATE_SIP_LINE_INFO */
    "更新SIP服务器信息。", /* ATP_VOICE_LOG_UPDATE_SIP_PROF_INFO */
    "删除SIP帐号信息。", /* ATP_VOICE_LOG_DEL_SIP_LINE_INFO */
    "删除SIP服务器信息。", /* ATP_VOICE_LOG_DEL_SIP_PROF_INFO */
    "新消息:主叫:$0 被叫:$1。", /* ATP_VOICE_LOG_MSG_RCV */
    "UPNP TR064 $0 认证失败。", /* ATP_SYS_LOG_UPNP_TR064_AUTH_FAIL */
    "UPNP TR064 $0 认证成功。", /* ATP_SYS_LOG_UPNP_TR064_AUTH_SUC  */
    "UPNP/TR064执行的动作$0成功。", /* ATP_SYS_LOG_UPNP_TR064_ACTION_SUC */
    "UPNP/TR064执行的动作$0失败，错误码：$1。", /* ATP_SYS_LOG_UPNP_TR064_ACTION_FAIL */
    "CWMP通知消息： 设置参数成功。", /* ATP_SYS_LOG_CWMP_SET_BY_TR069_OK */
    "CWMP通知消息： 下载完成。", /* ATP_SYS_LOG_CWMP_DLD_FILE_OK */
    "CWMP通知消息： 上传完成。", /* ATP_SYS_LOG_CWMP_ULD_FILE_OK */
    "ACS $0 连接到CPE认证失败。", /* ATP_SYS_LOG_CWMP_AUTH_ERR */
    "ACS $0 连接CPE失败。", /* ATP_SYS_LOG_CWMP_ACS_CONN_TO_CPE_FAIL */
    "ACS $0 连接CPE成功。", /* ATP_SYS_LOG_CWMP_ACS_CONN_TO_CPE_SUC */
    "CWMP 通知消息: 导致该消息的操作事件: $0。", /* ATP_SYS_LOG_CWMP_INFROM_EVENT */
    "CWMP 通知消息: 获取操作。", /* ATP_SYS_LOG_CWMP_INFROM_GET_ACTIONS */
    "CWMP 通知消息: 设置操作。", /* ATP_SYS_LOG_CWMP_INFROM_SET_ACTIONS */
    "CWMP: 更新管理证书文件: $0!", /* ATP_SYS_LOG_CWMP_UPDATE_CERT_FILE */
    "CWMP通知消息： 周期上报。", /* ATP_SYS_LOG_CWMP_PERIOD_REPORT */
    "CWMP通知消息：url 发生变化。", /* ATP_SYS_LOG_CWMP_UPDATE_URL */
    "CWMP通知消息：acs 回连成功。", /* ATP_SYS_LOG_CWMP_INFROM_ACS_CONN_OK */
    "CWMP通知消息：参数改变。", /* ATP_SYS_LOG_CWMP_INFORM_SET_PARA_DONE */
    "CWMP:Cwmp 通知成功.", /* ATP_SYS_LOG_CWMP_POST_INFORM_SUC */
    "CWMP:Cwmp 通知失败.", /* ATP_SYS_LOG_CWMP_POST_INFORM_FAIL */
    "WAN连接$0启用。", /* ATP_SYS_LOG_WAN_CONNECTED */
    "WAN连接$0失败:($1)。", /* ATP_SYS_LOG_WAN_DISCONNETED */
    "WAN配置：删除WAN IP连接：$0。", /* ATP_SYS_LOG_WAN_CONFIG_IP_CONN_DEL */
    "WAN配置：新增WAN IP连接：$0。", /* ATP_SYS_LOG_WAN_CONFIG_IP_CONN_ADD */
    "WAN配置：修改WAN IP连接：$0。", /* ATP_SYS_LOG_WAN_CONFIG_IP_CONN_SET */
    "WAN配置：删除WAN PPP连接：$0。", /* ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_DEL */
    "WAN配置：新增WAN PPP连接：$0。", /* ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_ADD */
    "WAN配置：修改WAN PPP连接：$0。", /* ATP_SYS_LOG_WAN_CONFIG_PPP_CONN_SET */
    "无线接入模块通信正常!", /* ATP_SYS_LOG_WAN_UMTS_DIAL_SUCCESS */
    "无线接入模块通信故障:PPP拨号失败!", /* ATP_SYS_LOG_WAN_UMTS_DIAL_FAIL */
    "消息类型: 离开, 报告者: $0, MAC地址: $1, 组: $2, 版本号: 2!", /* ATP_SYS_LOG_IGMP_LEAVE_INFO */
    "消息类型: 公布, 报告者: $0, MAC地址: $1, 组: $2, 版本号: 3, 模式: $3!", /* ATP_SYS_LOG_IGMP_REPORT_INFO_MODE */
    "消息类型: 公布, 报告者: $0, MAC地址: $1, 组: $2, 版本号: 2!", /* ATP_SYS_LOG_IGMP_REPORT_INFO */
    "消息类型: 查询, 查询者: $0, 组: $1, 版本号: $2.", /* ATP_SYS_LOG_IGMP_QUERY_INFO */
    "防火墙级别设置为: $0。", /* ATP_SYS_LOG_FW_CHANGE_TO_LEVEL */
    "新增一条防火墙过滤级别[$0]，实例ID为$1。", /* ATP_SYS_LOG_FW_FITER_LEVEL_ADD */
    "修改防火墙过滤级别[$0]，实例ID为$1。", /* ATP_SYS_LOG_FW_FILTER_LEVEL_MOD */
    "增加了Access Control entry, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_NEW_ACC_CTRL_ENTRY_ADD */
    "Access Control entry 被修改, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_ACC_CTRL_ENTRY_MOD */
    "Access Control entry被删除, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_ACC_CTRL_ENTRY_DEL */
    "增加了IP filter entry, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_NEW_IP_FILTER_ENTRY_ADD */
    "IP filter entry 被修改, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_IP_FILTER_ENTRY_MOD */
    "增加了新的URL filter black list, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_NEW_URL_FILTER_BLIST_ADD */
    "URL filter black list被修改, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_NEW_URL_FILTER_BLIST_MOD */
    "修改了 DOS entry.", /* ATP_SYS_LOG_FW_DOS_ENTRY_MOD */
    "过滤级别[$0]被删除。", /* ATP_SYS_LOG_FW_FILTER_LEVEL_DEL */
    "IP filter entry 被删除, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_IP_FILTER_ENTRY_DEL */
    "New Keyword 增加, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_KEYWORD_ADD */
    "New Keyword 被修改, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_KEYWORD_MOD */
    "New Keyword 被删除, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_KEYWORD_DEL */
    "URL filter black list 被删除, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_URL_FILTER_BLIST_DEL */
    "URL filter white list 被删除, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_URL_FILTER_WLIST_DEL */
    "增加了新的 URL filter white list, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_NEW_URL_FILTER_WLIST_ADD */
    "URL filter white list 被修改, 实例号ID为 $0.", /* ATP_SYS_LOG_FW_URL_FILTER_WLIST_MOD */
    "用户$0通过$1登录网关成功 $2。", /* ATP_SYS_LOG_CLI_LOGIN_OK */
    "用户$0通过$1注销 $2。", /* ATP_SYS_LOG_CLI_LOGOUT */
    "用户$0通过$1登录网关失败 $2。", /* ATP_SYS_LOG_CLI_LOGIN_ERR */
    "用户$0尝试从$1登录失败超过3次 $2。", /* ATP_SYS_LOG_CLI_LOGIN_LOCKED */
    "用户$0从$1登录解锁 $2。", /* ATP_SYS_LOG_CLI_LOGIN_UNLOCKED */
    "读flash出错,flash位置:$0。", /* ATP_SYS_LOG_BHAL_FLASH_READ_ERR */
    "写flash出错,flash位置:$0。", /* ATP_SYS_LOG_BHAL_FLASH_WRITE_ERR */
    "DSL成功激活!", /* ATP_SYS_LOG_XDSL_DSL_CONN */
    "DSL去激活!", /* ATP_SYS_LOG_XDSL_DSL_DISCONN */
    "配置DMZ主机: 增加IP: $0。", /* ATP_SYS_LOG_NAT_DMZ_HOST_IP_ADD */
    "配置DMZ主机: 删除IP: $0。", /* ATP_SYS_LOG_NAT_DMZ_HOST_IP_DEL */
    "配置DMZ主机: 修改IP: $0。", /* ATP_SYS_LOG_NAT_DMZ_HOST_IP_MOD */
    "更新日志配置信息!", /* ATP_SYS_LOG_LOG_CONFIG_SERVER */
/* <DTS2013010908899  z00159386  20130109  begin */
#ifdef SUPPORT_ATP_TELUS
    "用户($0)清除了所有日志。", /* ATP_SYS_LOG_LOG_CLEAR */
	"DDNS Log : connection $0：$1 error $2.", /* ATP_SYS_LOG_DDNS_CLIENT_CONNECT_ERR */
    "DDNS Log : request successful $0.", /* ATP_SYS_LOG_DDNS_REPLY_SUC */
    "DDNS Log : DDNS Update fail: $0 $1" /* ATP_SYS_LOG_DDNS_REPLY_FAIL */
#else
    "用户($0)清除了所有日志。" /* ATP_SYS_LOG_LOG_CLEAR */
#endif /* SUPPORT_ATP_TELUS */
/* DTS2013010908899  z00159386  20130109  end> */
};

static const ATP_UTIL_STR_IDX_MAP_ST *m_apcLogTypeArrays[] =
{
    m_astLogTypeStr_en,
    m_astLogTypeStr_zh
};

static const VOS_CHAR **m_apcLogLevelArrays[] =
{
    m_apcLogLevelStr_en,
    m_apcLogLevelStr_zh
};

static const VOS_CHAR **m_apcLogStrArrays[] =
{
    m_apcLogStr_EN,
    m_apcLogStr_ZH
};
