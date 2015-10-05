// JavaScript Document
/***************************the common var******************/
// Global Features
var g_feature = null;

var g_moduleswitch = null;
var g_module = {};
var g_dailup = null;
var g_destnation = null;
var g_nav = null;
var g_login_usertype = 'admin';
var g_feature_ipv6 = false;
var g_setting_click = false;

var LOGOUT_TIMEOUT_MAIN = 300000;
var g_logoutTimer = '';
//Language list scroll control
var g_LANGUAGE_COUNT = 13;
var g_LANGUAGE_LIST_HEIGHT = '300px';
// constant variable
var G_MonitoringStatus = null;
var G_NotificationsStatus = null;
var G_StationStatus = null;
var MACRO_SHOW_POPUP = 1;
//Pin status
var MACRO_CARD_LOCK_PIN_RIGNT = '253';
var MACRO_CARD_LOCK_PIN_WRONG = '254';
var MACRO_NO_SIM_CARD = '255';
var MACRO_CPIN_FAIL = '256';
var MACRO_PIN_READY = '257';
var MACRO_PIN_DISABLE = '258';
var MACRO_PIN_VALIDATE = '259';
var MACRO_PIN_REQUIRED = '260';
var MACRO_PUK_REQUIRED = '261';
var MACRO_INIT_SIM_CARD = '262';

//Connection status
var MACRO_CONNECTION_CONNECTING = '900';
var MACRO_CONNECTION_CONNECTED = '901';
var MACRO_CONNECTION_DISCONNECTED = '902';
var MACRO_CONNECTION_DISCONNECTING = '903';
var FORBID_AUTO_CONNECT_OPEN_DEVICE = '112';
var FORBID_AUTO_CONNECT_OPEN_DEVICE_ROAMING = '113';
var FORBID_RE_CONNECT_DROPLINE = '114';
var FORBID_RE_CONNECT_DROPLINE_ROAMING = '115';
var MACRO_CONNECTION_IPV6CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_IPV6CONNECTED = '2';
//SIM status
var MACRO_SIM_STATUS_USIM_N = '0';
var MACRO_SIM_STATUS_USIM_Y = '1';
var MACRO_SIM_STATUS_USIM_CS_N = '2';
var MACRO_SIM_STATUS_USIM_PS_N = '3';
var MACRO_SIM_STATUS_USIM_PS_CS_N = '4';
var MACRO_SIM_STATUS_ROMSIM = '240';
var MACRO_SIM_STATUS_USIM_NE = '255';
//Battery status
var MACRO_BATTERY_STATUS_NORMAL = '0';
var MACRO_BATTERY_STATUS_ELECT = '1';
var MACRO_BATTERY_STATUS_LOW = '-1';
//Battery level
var MACRO_BATTERY_LEVEL_ZERO = '0';
var MACRO_BATTERY_LEVEL_ONE = '1';
var MACRO_BATTERY_LEVEL_TWO = '2';
var MACRO_BATTERY_LEVEL_THREE = '3';
var MACRO_BATTERY_LEVEL_FOUR = '4';
//Signal status
var MACRO_EVDO_LEVEL_ZERO = '0';
var MACRO_EVDO_LEVEL_ONE = '1';
var MACRO_EVDO_LEVEL_TWO = '2';
var MACRO_EVDO_LEVEL_THREE = '3';
var MACRO_EVDO_LEVEL_FOUR = '4';
var MACRO_EVDO_LEVEL_FIVE = '5';

//CurrentPlmn
var MACRO_CURRENT_NETWOORK_2G = '0';
var MACRO_CURRENT_NETWOORK_3G = '2';
var MACRO_CURRENT_NETWOORK_H = '5';
var MACRO_CURRENT_NETWOORK_4G = '7';

var SERVICE_STATUS_AVAIABLE = "2";
var WIRELINESERVICE_STATUS_AVAIABLE = "1";
//CurrentNetworkType
var MACRO_NET_WORK_TYPE_NOSERVICE         = '0';          /* 无服务            */

var MACRO_NET_WORK_TYPE_GSM               = '1';          /* GSM模式           */
var MACRO_NET_WORK_TYPE_GPRS              = '2';          /* GPRS模式          */
var MACRO_NET_WORK_TYPE_EDGE              = '3';          /* EDGE模式          */

var MACRO_NET_WORK_TYPE_WCDMA             = '4';          /* WCDMA模式         */
var MACRO_NET_WORK_TYPE_HSDPA             = '5';          /* HSDPA模式         */
var MACRO_NET_WORK_TYPE_HSUPA             = '6';          /* HSUPA模式         */
var MACRO_NET_WORK_TYPE_HSPA              = '7';          /* HSPA模式          */
var MACRO_NET_WORK_TYPE_TDSCDMA           = '8';          /* TDSCDMA模式       */
var MACRO_NET_WORK_TYPE_HSPA_PLUS         = '9';          /* HSPA_PLUS模式     */
var MACRO_NET_WORK_TYPE_EVDO_REV_0        = '10';         /* EVDO_REV_0模式    */
var MACRO_NET_WORK_TYPE_EVDO_REV_A        = '11';         /* EVDO_REV_A模式    */
var MACRO_NET_WORK_TYPE_EVDO_REV_B        = '12';         /* EVDO_REV_A模式    */
var MACRO_NET_WORK_TYPE_1xRTT             = '13';         /* 1xRTT模式         */
var MACRO_NET_WORK_TYPE_UMB               = '14';         /* UMB模式           */
var MACRO_NET_WORK_TYPE_1xEVDV            = '15';         /* 1xEVDV模式        */
var MACRO_NET_WORK_TYPE_3xRTT             = '16';         /* 3xRTT模式         */
var MACRO_NET_WORK_TYPE_HSPA_PLUS_64QAM   = '17';         /* HSPA+64QAM模式    */
var MACRO_NET_WORK_TYPE_HSPA_PLUS_MIMO    = '18';          /* HSPA+MIMO模式     */
var MACRO_NET_WORK_TYPE_DC_HSPA           = '19';          /* DC-HSPA模式     */
var MACRO_NET_WORK_TYPE_LTE               = '20';          /*LTE 模式*/
var DSAT_SYS_SUBMODE_TDS                  = '21';          /*TD-SCDMA ģʽ */ 
var DSAT_SYS_SUBMODE_TDS_HSDPA            = '22';          /*TDS_HSDPA ģʽ */ 
var DSAT_SYS_SUBMODE_TDS_HSUPA            = '23';          /*TDS_HSUPA ģʽ */ 
var DSAT_SYS_SUBMODE_TDS_HSPA             = '24';          /*TDS_HSPA ģʽ */



//Wifi status
var MACRO_WIFI_OFF = '0';
var MACRO_WIFI_ON = '1';

var AJAX_HEADER = '../';
var AJAX_TAIL = '';

var AJAX_TIMEOUT = 30000;
var LOGIN_STATES_SUCCEED = '0';
var LOGIN_STATES_FAIL = '-1';
var LOGIN_STATES_REPEAT = '-2';
var g_needToLogin = '';
var g_needHelp = '';
var g_needFooter = '';

var ERROR_LOGIN_USERNAME_WRONG = 108001;
var ERROR_LOGIN_PASSWORD_WRONG = 108002;
var ERROR_LOGIN_ALREADY_LOGIN = 108003;
var MACRO_LOGIN = '1';
var MACRO_LOGOUT = '2';
var MACRO_NEWVERSIONFOUND = '12';
var MACRO_READYTOUPDATE = '50';
var WIRELESS_SERVICE_STATUS_AVAIABLE = '2';

var g_monitoring_dumeter_kb = 1024;
var g_monitoring_dumeter_mb = 1024 * 1024;
var g_monitoring_dumeter_gb = 1024 * 1024 * 1024;
var g_monitoring_dumeter_tb = 1024 * 1024 * 1024 * 1024;

var plmn_rat_index = {
    0 : 2,
    2 : 3
};

var g_is_connect_clicked = false;
var g_is_disconnect_clicked = false;
var g_base64EncodeChars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
var base64DecodeChars = new Array(-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1);

var g_is_login_opened = false;
// Aim to take all the prompt displaying on a page as a stack.
var g_main_displayingPromptStack = new Array();

var DATA_READY = {
    statusReady: false,
    apStationReady: false,
    notificationsReady: false
};
var LOGIN_PATH = 'login.html';
var g_user_manual_url = '';
//If remove when show confirm dialog
var DIALOG_UNREMOVE = 0;
//Constant for initialization tooltip
var STATUS_BAR_ICON_STATUS = {
    sim_tooltip_state: '',
    plmn_tooltip_state: '',
    station_tooltip_state: '',
    wan_tooltip_state: '',
    wifi_tooltip_state: '',
    battery_tooltip_state: ''
};

var STATUS_LISTENER_FUN_LIST = new Array();
var g_lastBatteryStatus = null;
//
var header_icon_status = {
    ConnectionStatus: ' ',
    SignalStrength: ' ',
    BatteryStatus: ' ',
    BatteryLevel: ' ',
    SimStatus: ' ',
    WifiStatus: ' ',
    ServiceStatus: ' ',
    CurrentNetworkType: ' ',
    x_WireLineLinkStatus:' ',
    X_IPv6ConnectionStatus : ' ',
    X_IPv4WireLineConnectionStatus : ' ',
    X_IPv6WireLineConnectionStatus : ' '
};

//Wifi station connection status
var WIFI_STATION_DISCONNECTED = '0';
var WIFI_STATION_CONNECTING = '1';
var WIFI_STATION_CONNECTED = '2';
var WIFI_STATION_DISCONNECTING = '3';
//URL to home page
var HOME_PAGE_URL = 'home.html';
var LOGIN_PAGE_URL = 'login.html';

/*invialid page*/
var g_PageUrlTree = null;
var g_helpurl = null;

/* ------------------------ Language ----------------------------------- */
var LANGUAGE_DATA = {
    current_language: 'en_us',
    supportted_languages: new Array()
};
var current_href = window.location.href;
var TOPMENU = {
    nav: new Array(),
    have: false
};
var log = log4javascript.getNullLogger();
var _logEnable = getQueryStringByName('log');
var g_main_convergedStatus = null;
var g_plmn_rat = '';


var PriacyUrl = null;

/******************************** DHCP ***************************************/
var DHCP_IP_ADDRESS_DEFAULT = '192.168.1.1';
var DHCP_START_IP_ADDRESS_DEFAULT = '192.168.1.100';
var DHCP_END_IP_ADDRESS_DEFAULT = '192.168.1.200';
var DHCP_STATUS_DEFAULT = '1';
var DHCP_STATUS_DISABLED = '0';
var DHCP_LAN_NET_MASK_DEFAULT = '255.255.255.0';
var DHCP_LEASE_TIME_DEFAULT = '0';
var dhcpLanIPAddress;
var dhcpLanNetmask;
var dhcpEnable;
var dhcpLanStartIP;
var dhcpLanEndIP;
var dhcpLeaseTime;
var dhcpPageVar = {
    DhcpIPAddress: DHCP_IP_ADDRESS_DEFAULT,
    DhcpLanNetmask: DHCP_LAN_NET_MASK_DEFAULT,
    DhcpStatus: DHCP_STATUS_DEFAULT,
    DhcpStartIPAddress: DHCP_START_IP_ADDRESS_DEFAULT,
    DhcpEndIPAddress: DHCP_END_IP_ADDRESS_DEFAULT,
    DHCPLeaseTime: DHCP_LEASE_TIME_DEFAULT
};
function initDhcp() {
    dhcpLanIPAddress = dhcpPageVar.DhcpIPAddress;
    dhcpLanNetmask = dhcpPageVar.DhcpLanNetmask;
    dhcpEnable = dhcpPageVar.DhcpStatus;
    dhcpLanStartIP = dhcpPageVar.DhcpStartIPAddress;
    dhcpLanEndIP = dhcpPageVar.DhcpEndIPAddress;
    dhcpLeaseTime = dhcpPageVar.DHCPLeaseTime;
}

/********************************function***************************************/

function matchLanguageExist(current_lang, lang_list_arr) {
    if ($.isArray(lang_list_arr)) {
        var lang_exsit = 0;
        $.each(lang_list_arr, function(i, v) {
            if (v.replace(/-/, '_') == current_lang.replace(/-/, '_')) {
                lang_exsit = i;
                return false;
            }
        });
        LANGUAGE_DATA.current_language = lang_list_arr[lang_exsit].replace(/-/, '_');
    }
    else if ('undefined' != typeof (lang_list_arr)) {
        LANGUAGE_DATA.current_language = lang_list_arr.replace(/-/, '_');
    }
    else {
        LANGUAGE_DATA.current_language = 'en_us';
    }
}

function reloadLeftMenu() {
    log.debug('start reloadLeftMenu');
    //reload left menu
    if($(".content").children().first().is(".main_left")) {
        if ($('.main_left').children().is('#settings_menu')) {
            log.debug('checking setting menu');
            if($.trim($('#settings_menu').html()) == '' || $('#settings_menu').html() == null)
            {
                log.debug('setting menu is null');
                showleftMenu();
            }
        }
        else if ($('.main_left').children().is('#sms_menu')) {
                log.debug('checking sms menu');
                if($.trim($('#sms_menu').html()) == '' || $('#sms_menu').html() == null)
                {
                    log.debug('sms menu is null');
                    showleftMenu();
                }
        }
        else if ($('.main_left').children().is('#maintenance_menu')) {
            log.debug('checking setting menu');
            if($.trim($('#maintenance_menu').html()) == '' || $('#maintenance_menu').html() == null)
            {
                log.debug('setting menu is null');
                showleftMenu();
            }
        }

    }
   
    //reLoad footer
    if (g_needFooter) {
        if($.trim($('#footer').html()) == '' || $('#footer').html() == null) {
            log.debug('footer is null');
            showFooter();
        }
        
    }
    
}
// load lang file

function loadLangFile() {
    var langFile = '../language/lang_' + LANGUAGE_DATA.current_language + '.js';

    $.ajax({
        async: false,
        //cache: false,
        type: 'GET',
        timeout: '3000',
        url: langFile,
        dataType: 'script',
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            try {
                log.error('MAIN : loadLangFile() error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus' + textStatus);
                log.error('MAIN : errorThrown' + errorThrown);
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : loadLangFile() success.');
        }
    });
}

//load resource

function loadResource()
{
    var img=new Image();
    img.src='../res/waiting.gif';
    var img1=new Image();
    img1.src='../res/dialog_close_btn.gif';
    var img2=new Image();
    img2.src='../res/waiting_bg_top.png';
    var img3=new Image();
    img3.src='../res/waiting_bg_center.png';
    var img4=new Image();
    img4.src='../res/waiting_bg_bottom.png';        
}

/*
 For page load optimize, we use one request instead of 3 request
 api/monitoring/converged-status = api/pin/status + api/pin/simlock +
 api/language/current-language
 */
function main_getConvergedStatus() {
    getAjaxData('api/monitoring/converged-status', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            g_main_convergedStatus = ret.response;
            matchLanguageExist(g_main_convergedStatus.CurrentLanguage, LANGUAGE_DATA.supportted_languages);

            // get language file
            loadLangFile();
            //load resource
            loadResource();
        }
    }, {
        sync: true
    });
    getAjaxData('api/pin/status', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            g_main_convergedStatus.SimState = ret.response.SimState;
        }
    }, {
        sync: true
    });
    if (LANGUAGE_DATA.current_language == 'ar_sa') {
        $('link').attr('href', '../css/main_Arabic.css');
    }

    $(document).ready(function() {
        $('#login_wrapper').show();
    });
}

function getLangList() {
    getConfigData('config/global/languagelist.xml', function($xml) {
        var lang_list_ret = xml2object($xml);
        if (lang_list_ret.type == 'config') {
            if($.isArray(lang_list_ret.config.languages.language)){
                LANGUAGE_DATA.supportted_languages = lang_list_ret.config.languages.language;
            }else{
              (LANGUAGE_DATA.supportted_languages).push(lang_list_ret.config.languages.language);
            }
        }
        else {
            log.error('Load language data failed');
        }
    }, {
        sync: true
    });
}

function initLanguage() {
    getLangList();
    main_getConvergedStatus();
}

function main_executeBeforeDocumentReady() {
    // initial logger
    if (_logEnable == 'debug') {
        log.setLevel(log4javascript.Level.DEBUG);
    }
    else if (_logEnable == 'trace') {
        log.setLevel(log4javascript.Level.TRACE);
    }
    log.debug('MAIN : entered ' + window.location.href);

    getGlobalFeatures();

    // get converged status to initial
    initLanguage();

    if ($.browser.msie && ($.browser.version == '6.0')) {
        try {
            document.execCommand('BackgroundImageCache', false, true);
        }
        catch (e) {
        }
    }

    //var temp = current_href;
    if (current_href.indexOf('?url') > 0) {
        current_href = current_href.substring(0, current_href.indexOf('?url'));
        current_href = current_href.substring(current_href.lastIndexOf('/') + 1);
        current_href = current_href.substring(0, current_href.indexOf('.'));

    }
    else {
        current_href = current_href.substring(current_href.lastIndexOf('/') + 1);
        if (current_href.lastIndexOf('?') > 0) {
            current_href = current_href.substring(0, current_href.lastIndexOf('?'));
            current_href = current_href.substring(0, current_href.indexOf('.'));
            if (current_href == 'smsinbox') {
                current_href = location.search.substring(1);
            }
        }
        else {
            current_href = current_href.substring(0, current_href.lastIndexOf('.'));
        }
    }

    selectCur(g_PageUrlTree);

    if (navigator.userAgent.indexOf('MSIE 6.') > -1) {
        window.attachEvent('onload', correctPNG);
    }

}

// document.write
function dw(text) {
    document.write(text);
}

// jquery  cookie
function newCookie(name, val, options) {
    if (typeof val != 'undefined') {// name and value given, set cookie
        options = options || {};
        if (val == null) 
        {
            val = '';
            options.expires = -1;
        }
        var expires = '';
        if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
            var day;
            if (typeof options.expires == 'number') {
                day = new Date();
                day.setTime(day.getTime() + (options.expires * 24 * 60 * 60 * 1000));
            }
            else {
                day = options.expires;
            }
            expires = '; expires=' + day.toUTCString();
            // use expires
            // attribute, max-age is not supported by IE
        }
        var route  = options.path ? '; path=' + options.path : '';
        var region = options.domain ? '; domain=' + options.domain : '';
        var safe   = options.secure ? '; secure' : '';
        document.cookie = [name, '=', encodeURIComponent(val), expires, route, region, safe].join('');
    }
    else {// only name given, get cookie
        var cookieVal = null;
        if (document.cookie && document.cookie != '') {
            var cookies = document.cookie.split(';');
            var i = 0;
            for (i = 0; i < cookies.length; i++) 
            {
                var cookie = jQuery.trim(cookies[i]);
                // Does this cookie string begin with the name we want?
                if (cookie.substring(0, name.length + 1) == (name + '=')) {
                    cookieVal = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieVal;
    }
}

// change lang
function changeLang(key, val) {
    key = false;
    if (key) {
        newCookie(key, val);
        window.location.reload();
    }
    else {
        var langObj = {
            CurrentLanguage: val
        };
        langObj.CurrentLanguage = langObj.CurrentLanguage.replace(/_/, '-');
        var res = object2xml('request', langObj);
        saveAjaxData('api/language/current-language', res, function($xml) {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                refresh();
            }
        });
    }
}

function setMainMenu() {
    if (current_href == 'pincoderequired' || current_href == 'pukrequired' || current_href == 'simlockrequired' || current_href == 'nocard' || current_href == 'connectionless' || current_href == 'opennewwindow') {
        $('#union_main_menu').hide();
        $('#logout_span').hide();
    }
    else {
        $('#home').children().text(common_home);

        if (g_module.statistic_enabled) {
            $('#statistic').children().text(connection_hilink_label_traffic_statistics);

        }
        else {
            $('#menu_statistic').remove();
        }

        if (g_module.quicksetup_enable) {
            $('#quicksetup').children().text(wizard_label_quick_setup);
        }
        else {
            $('#menu_quicksetup').remove();
        }

        if (g_module.sms_enabled) {
            $('#sms').children().text(sms_lable_sms);
        }
        else {
            $('#menu_sms').remove();
        }

        if (g_module.sdcard_enabled) {
            $('#sharing').children().text(sharing_label_sharing);
        }
        else {
            $('#menu_sharing').remove();
        }

        if (g_module.advanced_enabled) {
            $('#advanced').children().text(common_advanced);
        }
        else {
            $('#menu_advanced').remove();
        }

        if (g_module.maintenance_enabled) {
            $('#maintenance').children().text(advanced_label_maintenance);
        }
        else {
            $('#menu_maintenance').remove();
        }

        if (g_module.ussd_enabled) {
            $('#ussd').children().text(ussd_label_ussd);
        }
        else {
            $('#menu_ussd').remove();
        }

        $('#settings').children().text(common_settings);

        $('#union_main_menu').show();
    }
}

function selectCur(obj) {
    $.each(obj, function(i, n) {
        if (!TOPMENU.have) {
            TOPMENU.nav.push(i);
        }
        if (typeof n == 'object') {
            selectCur(n);
        }
        else if (typeof n == 'string') {
            if (n == current_href) {
                TOPMENU.nav.push(n);
                TOPMENU.have = true;
            }
        }
        if (!TOPMENU.have) {
            TOPMENU.nav.pop();
        }
    });
}

function showCur(noSubMenu) {
    if (noSubMenu) {
        $('#' + TOPMENU.nav[0]).addClass('active');
    }
    else {
        $('#' + TOPMENU.nav[1]).addClass('click');
        var cName = ($('#' + TOPMENU.nav[1]).get(0)).className;
        if (cName.indexOf('nosub') > -1) {
            cName = cName.replace('nosub ', 'nosub');
        }
        $('#' + TOPMENU.nav[1])[0].className = cName;
        //$("#" + TOPMENU.nav[1] + " ul li:eq(" + TOPMENU.nav[2] +
        // ")").addClass("subClick");
        $('#' + TOPMENU.nav[2]).addClass('subClick');
    }
}

// new log object for debug page
function getQueryStringByName(item) {
    var svalue = location.search.match(new RegExp('[\?\&]' + item + '=([^\&]*)(\&?)', 'i'));
    return svalue ? svalue[1] : svalue;
}

//Set defferent menu for each device type
function showleftMenu() {
    log.debug('showleftMenu');
    
    if ($('.main_left').children().is('#sms_menu')) {
        $('.main_left').load('leftmenu.html #sms_menu', function() {
            log.debug('MAIN : Load left sms menu successful1');
            clickMenu('menu_sms');
            $('#lable_sms').text(sms_lable_sms);
            //$("#label_new_message").text(sms_label_new_message);
            $('#label_inbox').text(sms_label_inbox);
            $('#label_sent').text(sms_label_sent);
            $('#label_drafts').text(sms_label_drafts);
            if (typeof(g_PageUrlTree.sms.sms_center_number) != 'undefined') {
                $('#label_sms_center_number').text(sms_label_sms_settings);
            }else{
                $('#messagesettings').remove();
            }
            showCur(false);
            $('#label_inbox_status').text(g_sms_inbox_store_status);
            $('#label_sent_status').text(g_sms_outbox_store_status);
            $('#label_drafts_status').text(g_sms_draftbox_store_status);
            // for IE
        });
    }

    if ($('.main_left').children().is('#settings_menu')) {
        log.debug('MAIN : Loading  leftmenu');
        $('.main_left').load('leftmenu.html #settings_menu', function() {
            log.debug('MAIN : Load left settings menu successfull');
            clickMenu('menu_settings');
            
            //dialup
            if (typeof(g_PageUrlTree.settings.dialup) != 'undefined') {
                $('#label_dialup').text(dialup_label_dialup);

                if (typeof(g_PageUrlTree.settings.dialup.connections) != 'undefined') {
                    $('#label_connection_status').text(hilink_label_auto_connection_link);
                }else{
                    $('#connections').remove();
                }
                if (typeof(g_PageUrlTree.settings.dialup.wan) != 'undefined') {
                    $('#label_mobile_wan').text(dialup_label_wireline_profile);
                }else{
                    $('#wan').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.dialup.mobilenetworksettings) != 'undefined') {
                    $('#label_mobile_network_settings').text(dialup_label_mobile_network_settings);
                }else{
                    $('#mobilenetworksettings').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.dialup.profilesmgr) != 'undefined') {
                    $('#label_profile_management').text(dialup_label_wireless_profile);
                }else{
                    $('#profilesmgr').remove();
                }
                
            }else{
                $('#dialup').remove();
            }
            
            //wlan
            if (typeof(g_PageUrlTree.settings.wlan) != 'undefined') {
                $('#label_wlan').text(wlan_label_wlan);
                if (typeof(g_PageUrlTree.settings.wlan.wlansettings) != 'undefined') {
                    $('#label_wlan_settings').text(wlan_label_wlan_settings);
                }else{
                    $('#wlansettings').remove();
                }
                
                
                if (typeof(g_PageUrlTree.settings.wlan.wlanmacfilter) != 'undefined') {
                    $('#label_wlan_mac_filter').text(wlan_label_wlan_mac_filter);
                }else{
                    $('#wlanmacfilter').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.wlan.wps) != 'undefined') {
                    $('#wlan_label_wps_settings').text(wlan_label_wps_settings);
                }else{
                    $('#wps').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.wlan.dhcp) != 'undefined') {
                    $('#label_dhcp').text(dhcp_label_dhcp);
                }else{
                    $('#dhcp').remove();
                }
                if (typeof(g_PageUrlTree.settings.wlan.wlanbridge) != 'undefined') {
                    $('#label_wlan_bridge_settings').text(wlan_bridge_settings);
                }else{
                    $('#wlanbridge').remove();
                }
                
            }else{
                $('#wlan').remove();
            }

            //security
            if (typeof(g_PageUrlTree.settings.security) != 'undefined') {
                if (typeof(g_PageUrlTree.settings.security.pincodeautovalidate) != 'undefined') {
                    $('#label_pin_auto_validation').text(dialup_label_pin_auto_validation);
                }else{
                    $('#pincodeautovalidate').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.pincodemanagement) != 'undefined') {
                    $('#label_pin_code_management').text(dialup_label_pin_code_management);
                }else{
                    $('#pincodemanagement').remove();
                }
                $('#label_security').text(firewall_label_security);

                if (typeof(g_PageUrlTree.settings.security.firewallswitch) != 'undefined') {
                    $('#label_firewall_switch').text(firewall_label_firewall_switch);
                }else{
                    $('#firewallswitch').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.lanipfilter) != 'undefined') {
                    $('#label_lan_ip_filter').text(firewall_label_lan_ip_filter);
                }else{
                    $('#lanipfilter').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.lanmacfilter) != 'undefined') {
                    $('#label_lan_mac_filter').text(lanmacfilter_label_lanmacfilter);
                }else{
                    $('#lanmacfilter').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.urlfilter) != 'undefined') {
                    $('#label_url_filter').text(urlfilter_label_urlfilter);
                }else{
                    $('#urlfilter').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.virtualserver) != 'undefined') {
                    $('#label_virtual_server').text(firewall_label_virtual_server);
                }else{
                    $('#virtualserver').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.specialapplication) != 'undefined') {
                    $('#label_special_application').text(firewall_label_special_application);
                }else{
                    $('#specialapplication').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.dmzsettings) != 'undefined') {
                    $('#label_dmz_settings').text(firewall_label_dmz_settings);
                }else{
                    $('#dmzsettings').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.sipalgsettings) != 'undefined') {
                    $('#label_sip_alg_settings').text(firewall_label_alg_settings);
                }else{
                    $('#sipalgsettings').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.upnp) != 'undefined') {
                    $('#label_upnp_setting').text(firewall_label_upnp_setting);
                }else{
                    $('#upnp').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.nat) != 'undefined') {
                    $('#label_nat_settings').text(IDS_wlan_title_nat_settings);
                }else{
                    $('#nat').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.security.aclsettings) != 'undefined') {
                    $('#label_acl_settings').text(acl_label_acl);
                }else{
                    $('#aclsettings').remove();
                }
            }else{
                $('#security').remove();
            }

            //system
            if (typeof(g_PageUrlTree.settings.system) != 'undefined') {
                $('#label_system').text(system_label_system);

                if (typeof(g_PageUrlTree.settings.system.deviceinformation) != 'undefined') {
                    $('#label_device_information').text(system_label_device_information);
                }else{
                    $('#deviceinformation').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.system.wanstatus) != 'undefined') {
                    $('#label_wan_status').text(wan_label_wan_status);
                }else{
                    $('#wanstatus').remove();
                }
                if (typeof(g_PageUrlTree.settings.system.lanstatus) != 'undefined') {
                    $('#label_lan_status').text(lan_label_lan_status);
                }else{
                    $('#lanstatus').remove();
                }
                if (typeof(g_PageUrlTree.settings.system.configuration) != 'undefined') {
                    $('#label_configuration').text(system_label_backup_restore);
                }else{
                    $('#configuration').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.system.modifypassword) != 'undefined') {
                    $('#label_modify_password').text(system_label_modify_password);
                }else{
                    $('#modifypassword').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.system.restore) != 'undefined') {
                    $('#label_restore_defaults').text(system_label_restore_defaults);
                }else{
                    $('#restore').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.system.reboot) != 'undefined') {
                    $('#label_reboot').text(system_label_reboot);
                }else{
                    $('#reboot').remove();
                }
                if (typeof(g_PageUrlTree.settings.system.autoreboot) != 'undefined') {
                    $('#label_autoreboot').text(label_autoreboot_schedule);
                }else{
                    $('#autoreboot').remove();
                }
            }else{
                $('#system').remove();
            }

            //dhcpmenu
            if (typeof(g_PageUrlTree.settings.dhcpmenu) != 'undefined') {
                $('#label_dhcpmenu').text(dhcp_label_dhcp);
                if (typeof(g_PageUrlTree.settings.dhcpmenu.ipv4dhcp) != 'undefined') {
                    $('#label_ipv4dhcp').text(dhcp_label_dhcp);
                }else{
                    $('#ipv4dhcp').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.dhcpmenu.ipv6dhcp) != 'undefined') {
                    $('#label_ipv6dhcp').text(ipv6dhcp_label_ipv6dhcp);
                }else{
                    $('#ipv6dhcp').remove();
                }
                
                if (typeof(g_PageUrlTree.settings.dhcpmenu.slaac) != 'undefined') {
                    $('#label_slaac').text(slaac_label_slaac);
                }else{
                    $('#slaac').remove();
                }

                if (typeof(g_PageUrlTree.settings.dhcpmenu.staticaddr) != 'undefined') {
                    $('#label_staticaddr').text(Label_static_address);
                }else{
                    $('#staticaddr').remove();
                }

                if (!g_feature_ipv6) {
                    $('#ipv6dhcp').remove();
                    $('#slaac').remove();
                }
            }else{
                $('#dhcpmenu').remove();
            }
            
            //advancedmenu
            if (typeof(g_PageUrlTree.settings.advancedmenu) != 'undefined') {
                $('#label_advancedmenu').text(common_advanced);
                //DDNS
                if (typeof(g_PageUrlTree.settings.advancedmenu.ddns) != 'undefined') {
                    $('#label_ddns').text(ddns_label_ddns);
                }else{
                    $('#ddns').remove();
                }
                //SNTP
                if (typeof(g_PageUrlTree.settings.advancedmenu.sntp) != 'undefined') {
                    $('#label_sntp').text(sntp_label_sntp);
                }else{
                    $('#sntp').remove();
                }
                //TR069
                if (typeof(g_PageUrlTree.settings.advancedmenu.tr069) != 'undefined') {
                    $('#label_tr069').text(tr069_label_tr069);
                }else{
                    $('#tr069').remove();
                }
                //TR111
                if (typeof(g_PageUrlTree.settings.advancedmenu.tr111) != 'undefined') {
                    $('#label_tr111').text(tr111_label_tr111);
                }else{
                    $('#tr111').remove();
                }
                //samba
                if (typeof(g_PageUrlTree.settings.advancedmenu.samba) != 'undefined') {
                    $('#label_samba').text(Label_storage_services);
                }else{
                    $('#samba').remove();
                }
                //ftp
                if (typeof(g_PageUrlTree.settings.advancedmenu.ftp) != 'undefined') {
                    $('#label_ftp').text(acl_ftp);
                }else{
                    $('#ftp').remove();
                }
                //antenna
                if (typeof(g_PageUrlTree.settings.advancedmenu.antenna) != 'undefined') {
                    $('#label_antenna').text(antenna_label_antenna);
                }else{
                    $('#antenna').remove();
                }
                //Static Route
                if (typeof(g_PageUrlTree.settings.advancedmenu.staticroute) != 'undefined') {
                    $('#label_staticroute').text(staticroute_label_staticroute);
                }else{
                    $('#staticroute').remove();
                }
                //IPsec VPN
                if (typeof(g_PageUrlTree.settings.advancedmenu.ipsecvpn) != 'undefined') {
                    $('#label_ipsecvpn').text(ipsecvpn_label_ipsecvpn);
                }else{
                    $('#ipsecvpn').remove();
                }
                //DMS
                if (typeof(g_PageUrlTree.settings.advancedmenu.dms) != 'undefined') {
                    $('#label_dms').text(dms_label_dms);
                }else{
                    $('#dms').remove();
                }
                //clip
                if (typeof(g_PageUrlTree.settings.advancedmenu.clip) != 'undefined') {
                    $('#label_clip').text(clip_label_clip);
                }else{
                    $('#clip').remove();
                }
            }else{
                $('#advancedmenu').remove();
            }
            
            showCur(false);
        });
    }

    if ($('.main_left').children().is('#maintenance_menu')) {
        log.debug('MAIN : Loading  leftmenu');
        $('.main_left').load('leftmenu.html #maintenance_menu', function() {
            log.debug('MAIN : Load left maintenance menu successfull');
            clickMenu('menu_maintenance');
            //logview
            if (typeof(g_PageUrlTree.maintenance.logview) != 'undefined') {
                $('#label_logview').text(advanced_label_logview);
            }else{
                $('#logview').remove();
            }
            //configuration
            if (typeof(g_PageUrlTree.maintenance.configuration) != 'undefined') {
                $('#label_configuration').text(system_label_configuration);
            }else{
                $('#configuration').remove();
            }
            if (typeof(g_PageUrlTree.maintenance.diagnosis) != 'undefined') {
                $('#label_diagnosis').text(diagnosis_device_states);
            }else{
                $('#diagnosis').remove();
            }
            if (typeof(g_PageUrlTree.maintenance.onekeydiag) != 'undefined') {
                $('#label_onekeydiag').text(system_label_diagnosis);
            }else{
                $('#onekeydiag').remove();
            }
            //update
            if (typeof(g_PageUrlTree.maintenance.update) != 'undefined') {
                $('#label_update').text(common_update);
            }else{
                $('#update').remove();
            }

            showCur(false);
        });
    }
    
}

function setLangList() {
    var g_langList='';
    if ($.isArray(LANGUAGE_DATA.supportted_languages)) {
        if (LANGUAGE_DATA.supportted_languages.length > 20) {
            $('#language_list').css({
                'overflow-x': 'hidden',
                'overflow-y': 'scroll'
            });
        }
        $.each(LANGUAGE_DATA.supportted_languages, function(n, value) {
           if (value.replace(/-/, '_') == LANGUAGE_DATA.current_language) {
               $('#lang').val(eval(value.replace(/-/, '_')));
            }
            g_langList += '<option value = ' + value.replace(/-/, '_') + '\>' + eval(value.replace(/-/, '_')) + '</option>';
        });
    }
    else if ('undefined' != typeof(LANGUAGE_DATA.supportted_languages)) {
        LANGUAGE_DATA.current_language = LANGUAGE_DATA.supportted_languages;
        var value = LANGUAGE_DATA.current_language;
        $('#lang').val(eval(LANGUAGE_DATA.supportted_languages.replace(/-/, '_')));
        g_langList += '<option value = ' + value.replace(/-/, '_') + '\>' + eval(value.replace(/-/, '_')) + '</option>';
    }
    else {
        log.error('Load language data failed');
    }
    
    $('#lang').append(g_langList);
    $('#lang').show();

    $('#lang').change(function() {
        changeLang('lang', $('#lang').val());
    });
}

function showCurrentLanguage() {
    //show the current lang to the lang inputBox
    var lang = LANGUAGE_DATA.current_language.replace(/-/, '_');
    setTimeout(function(){
        $('#lang').val(lang);
    },1);  
}


function showFooter()
{
    //Load footer
    if (g_needFooter) {
        $('#footer').load('footer.html img, span, rightcorner', function() {
            $('#footer span').html('&nbsp;' + common_copyright + 
                "<div class='rightcorner'><a id = 'privacy_url' href = \"" + PriacyUrl +"\" target =\"_blank\"><span>" + footer_product_privacy + "</span></a></div>");
            log.debug('MAIN : Load footer successful');
        });
    }
}
main_executeBeforeDocumentReady();
/**********************************After loaded (common)************/
$(document).ready(function() {
    //Display Title
    document.title = common_title;

    // hide right mouse click menu
    document.oncontextmenu = new Function('return false');

    $('.hotlinkstring').show();
    //load header,
    loadHeader();
    
    //load left menu
    if($(".content").children().first().is(".main_left")) {
        showleftMenu();
    }
    getPrivacyUrl();

    getDeviceStatus();

    showFooter();
    
    //Button effect when mouseover and out
    $('.button_wrapper:not(#login_btn,#connect_btn,#disconnect_btn,#cancel_btn)').live('mouseover', function() {
        $(this).addClass('mouse_on');
    });
    $('.button_wrapper').live('mouseout', function() {
        $(this).removeClass('mouse_on');
    });
    //Button effect when mousedown and mouseout
    $('.button_wrapper:not(#login_btn)').mousedown(function() {
        $(this).addClass('mouse_down');
    });
    $('.button_wrapper').mouseup(function() {
        $(this).removeClass('mouse_down');
    });
    //Drop down select, if there hasn't any drop down select, it won't work
    /*
    if ($("#lang").size() > 0) {
    drop_down_select($("#lang"), $(".lang_option").eq(0));//index page
    }
    */

    /******* Customlized dialog of which a button ID is "pop_OK" *****/
    $('#pop_OK').live('click', function() {
        if ('pop_OK' == g_main_displayingPromptStack[g_main_displayingPromptStack.length - 1]) {
            g_main_displayingPromptStack.pop();
        }
        setFocusToChildWindow();
        enableTabKey();
    });
    /*************Function for close dialog**************/
    $('.dialog_close_btn, .pop_Cancel').live('click', function() {
        if (g_nav != null) {
            g_nav.children().first().attr('href', g_destnation);
            g_nav = null;
        }
        clearDialog();
        g_main_displayingPromptStack.pop();
        enableTabKey();
        setFocusToChildWindow();
    });
    /*************Function for close dialog**************/
    /*$(window).resize(function(){
    windowResize($(".dialog"));
    });
    $("#div_wrapper").css({
    height: $(document).height()
    });*/

    /************* Key Event Handler **************/
    $(document).keyup(onKeyup);
    g_main_displayingPromptStack.length = 0;
  
});


$(window).load(function() {

    $("#all_content").show();
    log.debug("MAIN : all_content show");
    
    var login_status_items = $('.login_info > div:visible');
    if (login_status_items.size() > 2) {
        $.each(login_status_items, function(i) {
            $(this).css({
                width: '33.3%'
            });
        });
    }
    else {
        $.each(login_status_items, function(i) {
            $(this).css({
                width: '50%'
            });
        });
    }
    
    setTimeout(function() {
                reloadLeftMenu();
            }, 1500);
    setTimeout(function() {
                loadHeader();
            }, 2000);
    
});

function loadHeader () {
    //reload left menu
    log.debug('checking header');
    if($.trim($('#header').html()) == '' || $('#header').html() == null)
    {
        log.debug('header is null');
        var ret = '';
        var html = null;
        getAjaxData('api/user/state-login', function($xml) {
            ret = xml2object($xml);
            if (ret.type == 'response') {
                g_login_usertype = ret.response.Username;
            }
        }, {
            sync: true
        });
        if('admin' == g_login_usertype){
            html = 'header.html';
        } else if('user' == g_login_usertype) {
            html = 'user_header.html';
        }else{
            html = 'header.html';
        }
        $('#header').load(html, function() {
            showCur(true);
    
            setMainMenu();
    
            if (g_needHelp) {
                $('#help_span').text(common_help);
    		if (('home' == current_href)||('quicksetup' == current_href)) {
                    getUserManualUrl(current_href);
    		}
            }
            else {
                $('#help_span').remove();
                $('#help_url').remove();
            }
    
            if (g_needToLogin) {
                $('#logout_span').show();
                /*
                 * display login status(login or logout) after check login state
                 */
                if (ret.type == 'response') {
                    if (ret.response.State != 0) { //logout
                        $('#username_span').hide();
                        $('#logout_span').text(common_login);
                        cancelLogoutTimer();
                        gotoPageWithoutHistory(LOGIN_PATH);
                    }
                    else //login
                    {
                        g_login_usertype = ret.response.Username;
                        $('#username_span').text(ret.response.Username);
                        $('#username_span').show();
                        $('#logout_span').text(common_logout);
                        if ('home' == current_href) {
                            startLogoutTimer();
                        }
                    }
                    if('user' == g_login_usertype){
                        if(!('home'==current_href||'connectionstatus'==current_href||'wlanstatus'==current_href||'wanstatus'==current_href||'lanstatus'==current_href)){
                            gotoPageWithoutHistory(LOGIN_PATH);
                            return;
                        }
                    }
                }
            }
            else {
                $('#logout_span').hide();
            }
    
            if (!g_module.wifi_enabled) {
                $('#wifi_gif').remove();
            }
            if (!g_module.sms_enabled) {
                $('#sms_gif').remove();
            }
    
            if (!g_module.online_update_enabled) {
                $('#update_gif').remove();
            }
    
            if (!g_feature.battery_enabled) {
                $('#battery_gif').remove();
            }
            initialStatusIcon();
    
            /*
             * show language list
             */
            setLangList();
    
            showCurrentLanguage();
    
            /*
             * login logout
             */
            $('#logout_span').live('click', function() {
                g_destnation = null;
                loginout();
            });
            $('#pop_login').live('click', function() {
                if ((null == g_destnation) && ('sdcardsharing' == current_href)) {
                    login(current_href + '.html');
                }
                else {
                    login(g_destnation);
                }
            });
            $('#menu_statistic,#menu_index,#menu_sms,#menu_quicksetup,#menu_ussd,#menu_update,#menu_settings,#menu_maintenance,.nav01,.nav02,#profile_settings').live('click', function() {
                if (!g_needToLogin) {
                    return;
                }
                g_nav = $(this);
    
                getAjaxData('api/user/state-login', function($xml) {
                    var ret = xml2object($xml);
                    if (ret.type == 'response') {
                        if (ret.response.State != '0') { //logout
                            g_destnation = g_nav.children().first().attr('href');
                            g_nav.children().first().attr('href', 'javascript:void(0);');
                            window.location.href = LOGIN_PAGE_URL;
                        }
    
                    }
                }, {
                    sync: true
                });
                if(this.id == "menu_settings"){
                    g_href = "setting";
                    g_sub = "dialup";
                    g_name = "connections";
                    $.cookies.set('cookie', g_href+","+g_sub+","+g_name, { expiresAt: new Date().getTime()+5*60*1000 });
                } else if(this.id == "menu_maintenance"){
                    g_href = "maintenance";
                    g_sub = "";
                    g_name = "logview";
                    $.cookies.set('cookie', g_href+","+g_sub+","+g_name, { expiresAt: new Date().getTime()+5*60*1000 });
                } else if(this.id == "menu_sms"){
                    g_href = "sms";
                    g_sub = "";
                    g_name = "inbox";
                    $.cookies.set('cookie', g_href+","+g_sub+","+g_name, { expiresAt: new Date().getTime()+5*60*1000 });
                } 
            });
        });
    }
}

/********************Use Function  (end)************************/
/***********Function for create button*************/
function create_button(content, button_id, buttonClassName) {
    if (buttonClassName != '' && buttonClassName != ' ' && buttonClassName != null) {
        button = "<span class='button_wrapper " + buttonClassName + "' id='" + button_id + "'>";
    }
    else {
        button = "<span class='button_wrapper' id='" + button_id + "'>";
    }
    button += "<span class='button_left'>" + "<span class='button_right'>" + "<span class='button_center'>" + content + '</span></span></span></span>';
    document.write(button);
}

function create_button_html(content, button_id, buttonClassName) {
    if (buttonClassName != '' && buttonClassName != ' ' && buttonClassName != null) {
        button = "<span class='button_wrapper " + buttonClassName + "' id='" + button_id + "'>";
    }
    else {
        button = "<span class='button_wrapper' id='" + button_id + "'>";
    }
    button += "<span class='button_left'>" + "<span class='button_right'>" + "<span class='button_center'>" + content + '</span></span></span></span>';
    return button;
}

function isButtonEnable(button_id) {
    var disable = true;
    var $button = $('#' + button_id);
    if ($button) {
        if ($button.children()) {
            disable = $button.hasClass('disable_btn');
            //enable = $button.children().hasClass("button_left");
        }
    }
    return !disable;
}

/*************Function for append dialog**************/
function setFocusToChildWindow(){
    var iframe = document.getElementsByTagName("IFRAME")[0];
    if (iframe != null || typeof(iframe) != 'undefined'){
        var cr = iframe.contentWindow.document.getElementById("focus_hidden");
        var $cr = $(cr);
        if ($cr){
            setTimeout(function(){
                $cr.show();
                $cr.focus();
                $cr.hide();
            },0);
        }
    }
}
function call_dialog(dialogTitle, content, button1_text, button1_id, button2_text, button2_id,okFunc,cancelFunc,focus_id) {
    $('#div_wrapper').remove();
    $('.dialog').remove();

    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; width:100%; height:100%; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog' id='sms_dialog'>";
    dialogHtml += "    <div class='sms_dialog_top'></div>";
    dialogHtml += "    <div class='sms_dialog_content'>";
    dialogHtml += "        <div class='sms_dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + dialogTitle + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='close' class='dialog_close_btn'><img src='../res/dialog_close_btn.gif' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='sms_dialog_table'>" + content + "<input type='text' id='hidden_focus' style='display:none;width:0;border:0;height:0;' value='focus'></div>";
    dialogHtml += "        <div class='sms_dialog_table_bottom'>";
    dialogHtml += "            <div class='dialog_table_r'>";

    dialogHtml += "              <span class='button_wrapper " + button1_id + "' id='" + button1_id + "'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + button1_text + '</a></span>';
    dialogHtml += '                      </span></span></span>';

    if (button2_text != '' && button2_text != ' ' && button2_text != null) {
        dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper " + button2_id + "' id='" + button2_id + "'>";
        dialogHtml += "                  <span class='button_left'>";
        dialogHtml += "                      <span class='button_right'>";
        dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + button2_text + '</a></span>';
        dialogHtml += '                      </span></span></span>';
    }
    dialogHtml += '            </div>';
    dialogHtml += '        </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='sms_dialog_bottom'></div>";
    dialogHtml += '</div>';

    $('.body_bg').before(dialogHtml);

    reputPosition($('.dialog'), $('#div_wrapper'));
    
    if (typeof(focus_id) != 'undefined' && focus_id != "" && focus_id != null){
        var $focus = $('#'+focus_id);
        $focus.focus();
    }
    else{
        setTimeout(function(){
            $('#hidden_focus').show();
            $('#hidden_focus').focus();
            $('#hidden_focus').hide();
        },0);
    }
    $('#'+button1_id).bind('click', function() {
        var b = true;
        if (typeof (okFunc) == 'function') {
            b = okFunc();
        }
        if(b){
            okFunc = null;
            cancelFunc = null;
            g_main_displayingPromptStack.pop();
            setFocusToChildWindow();
        }
        return false;
    });
    
    $('#'+button2_id).bind('click', function() {
        if(cancelFunc != null && cancelFunc != "") {
            cancelFunc();
        }
        g_main_displayingPromptStack.pop();
        setFocusToChildWindow();
    });

    g_main_displayingPromptStack.push(button1_id);

    $('a').attr('tabindex', '-1');
    $('select').attr('tabindex', '-1');
}

//rewrite call_dialog for wan
function call_dialog_wan(dialogTitle, content, button1_text, button1_id, button2_text, button2_id,okFunc,cancelFunc) {
    $('#div_wrapper').remove();
    $('.dialog').remove();
    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; width:100%; height:100%; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog' id='sms_dialog_wan'>";
    dialogHtml += "    <div class='sms_dialog_top_wan'></div>";
    dialogHtml += "    <div class='sms_dialog_content_wan'>";
    dialogHtml += "        <div class='sms_dialog_header_wan'>";
    dialogHtml += "            <span class='dialog_header_left'>" + dialogTitle + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='close' class='dialog_close_btn'><img src='../res/dialog_close_btn.gif' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='sms_dialog_table_wan'>" + content + '</div>';
    dialogHtml += "        <div class='sms_dialog_table_bottom_wan'>";
    dialogHtml += "            <div class='dialog_table_r'>";
    dialogHtml += "              <span class='button_wrapper " + button1_id + "' id='" + button1_id + "'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + button1_text + '</a></span>';
    dialogHtml += '                      </span></span></span>';
    if (button2_text != '' && button2_text != ' ' && button2_text != null) {
        dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper " + button2_id + "' id='" + button2_id + "'>";
        dialogHtml += "                  <span class='button_left'>";
        dialogHtml += "                      <span class='button_right'>";
        dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + button2_text + '</a></span>';
        dialogHtml += '                      </span></span></span>';
    }
    dialogHtml += '            </div>';
    dialogHtml += '        </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='sms_dialog_bottom_wan'></div>";
    dialogHtml += '</div>';
    $('.body_bg').before(dialogHtml);
    reputPosition($('.dialog'), $('#div_wrapper'));
    $('#'+button1_id).bind('click', function() {
        var b = true;
        if (typeof (okFunc) == 'function') {
            b = okFunc();
        }
        if(b){
            okFunc = null;
            cancelFunc = null;
            g_main_displayingPromptStack.pop();
            setFocusToChildWindow();
        }
        return false;
    });
    
    $('#'+button2_id).bind('click', function() {
        cancelFunc();
        g_main_displayingPromptStack.pop();
    });
    g_main_displayingPromptStack.push(button1_id);
    $('a').attr('tabindex', '-1');
    $('select').attr('tabindex', '-1');
}

/*----------------call dialog for modifypassword--------------------*/
function showPswInfoDialog(tipsContent,info_title,focus_id) {
    if ($('.info_dialog').size() == 0) {
        var hideInfo;
        clearTimeout(hideInfo);
        var dialogHtml = '';
        if ($('#div_wrapper').size() < 1) {
            dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; position:absolute; width:100%; height:100%; z-index:-1; display: block;'> </iframe></div>";
        }
        dialogHtml += "<div class='info_dialog'>";
        dialogHtml += "    <div class='info_dialog_top'></div>";
        dialogHtml += "    <div class='info_dialog_content'>";
        dialogHtml += "        <div class='info_dialog_header'>";
        dialogHtml += "            <span class='dialog_header_left'>" + common_note + '</span>';
        dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='' class='info_dialog_close_btn'><img src='../res/dialog_close_btn.gif' title='' alt='' /></a></span>";
        dialogHtml += '        </div>';
        dialogHtml += "    <div class='info_dialog_table_psw'><div class='info_title'><h4>"+ info_title +"</h4></div>";
        dialogHtml += "        <div class='info_content'>" + tipsContent + '</div>';
        dialogHtml += '    </div>';
        dialogHtml += "    <div class='info_dialog_bottom'></div>";
        dialogHtml += '</div>';

        $('.body_bg').before(dialogHtml);

        $('.info_dialog_close_btn').bind('click', function() {
            clearTimeout(hideInfo);
            $('.info_dialog').remove();
            $('#div_wrapper').remove();
            $('#'+focus_id).focus();
        });

        reputPosition($('.info_dialog'), $('#div_wrapper'));
        disableTabKey();
        hideInfo = setTimeout(function() {
            $('.info_dialog').remove();
            $('#div_wrapper').remove();
            $('#'+focus_id).focus();
        }, 30000);
    }
}

function showloginDialog() {
    $('#div_wrapper').remove();
    $('.login_dialog').remove();

    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; position:absolute; width:100%; height:100%; z-index:-1; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='login_dialog' id='dialog'>";
    dialogHtml += "    <div class='login_dialog_top'></div>";
    dialogHtml += "    <div class='login_dialog_content'>";
    dialogHtml += "        <div class='login_dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + common_login + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a class='wait_dialog_btn' title='' href='javascript:void(0);'><img src='../res/dialog_close_btn.gif' title='' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='login_dialog_table'>";
    dialogHtml += "               <div class='login'>";
    dialogHtml += "               <div id='username_wrapper'>";
    dialogHtml += '                   <p>' + dialup_label_user_name + common_colon + '</p>';
    dialogHtml += "                   <span><input type='text' class='input' id='login_username' maxlength='15' value='admin'/></span>";
    dialogHtml += '               </div>';
    dialogHtml += "               <div id='password_wrapper'>";
    dialogHtml += '               <p>' + common_password + common_colon + '</p>';
    dialogHtml += "                   <span><input type='password'  class='input' id='login_password' maxlength='15'/></span>";
    dialogHtml += '               </div>';
    dialogHtml += '               </div>';
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='login_dialog_table_bottom'>";
    dialogHtml += "              <span class='button_wrapper pop_login' id='login'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_login + '</a></span>';
    dialogHtml += '                      </span></span></span>';
    dialogHtml += '            </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='login_dialog_bottom'></div>";
    dialogHtml += '</div>';

    $('.body_bg').before(dialogHtml);
    reputPosition($('#dialog'), $('#div_wrapper'));
    g_main_displayingPromptStack.push('login');

    $('#login_password').focus();
    g_is_login_opened = true;
    disableTabKey();
}

function showConfirmDialog(content, okFunc, cancelFunc, removeable) {
    if (DIALOG_UNREMOVE != removeable) {
        $('#div_wrapper').remove();
        $('.dialog').remove();
    }

    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; position:absolute; width:100%; height:100%; z-index:-1; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog'>";
    dialogHtml += "    <div class='dialog_top'></div>";
    dialogHtml += "    <div class='dialog_content'>";
    dialogHtml += "        <div class='dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + common_confirm + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='' class='dialog_close_btn'><img src='../res/dialog_close_btn.gif' title='' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='dialog_table'>"+ content +"<input type='text' id='hidden_focus' value='focus'style='display:none;width:0;border:0;height:0;'></div>";
    dialogHtml += "        <div class='dialog_table_bottom'>";
    dialogHtml += "            <div class='dialog_table_r'>";
    dialogHtml += "              <span class='button_wrapper pop_confirm' id='pop_confirm'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_ok + '</a></span>';
    dialogHtml += '                      </span></span></span>';
    var isHasCancel;
    if (cancelFunc != '' && cancelFunc != ' ' && cancelFunc != null) {
        isHasCancel = true;
        dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper pop_Cancel' id='pop_Cancel'>";
        dialogHtml += "                  <span class='button_left'>";
        dialogHtml += "                      <span class='button_right'>";
        dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_cancel + '</a></span>';
        dialogHtml += '                      </span></span></span>';
    } else{
        isHasCancel = false;
    }
    dialogHtml += '            </div>';
    dialogHtml += '        </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='dialog_bottom'></div>";
    dialogHtml += '</div>';

    $('.body_bg').before(dialogHtml);
    $('#pop_confirm').bind('click', function() {
        clearDialog();
        if (typeof (okFunc) == 'function') {
            okFunc();
        }
        okFunc = null;
        cancelFunc = null;
        g_main_displayingPromptStack.pop();
        if (isHasCancel) {
            setFocusToChildWindow();
        }
    });
    $('#pop_Cancel').bind('click', function() {
        if (typeof (cancelFunc) == 'function') {
            cancelFunc();
        }
        okFunc = null;
        cancelFunc = null;
        g_main_displayingPromptStack.pop();
        setFocusToChildWindow();
    });

    reputPosition($('.dialog'), $('#div_wrapper'));

    g_main_displayingPromptStack.push('pop_confirm');

    disableTabKey();
    setTimeout(function(){
        $("#hidden_focus").show();
        $("#hidden_focus").focus();
        $("#hidden_focus").hide();
    },0);
}


function showupdateConfirmDialog(content, okFunc, laterFunc,cancelFunc,removeable) {
    if (DIALOG_UNREMOVE != removeable) {
        $('#div_wrapper').remove();
        $('.dialog').remove();
    }

    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; position:absolute; width:100%; height:100%; z-index:-1; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog'>";
    dialogHtml += "    <div class='dialog_top'></div>";
    dialogHtml += "    <div class='dialog_content'>";
    dialogHtml += "        <div class='dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + common_confirm + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='' class='dialog_close_btn'><img src='../res/dialog_close_btn.gif' title='' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='dialog_table'>" + content + '</div>';
    dialogHtml += "        <div class='dialog_table_bottom'>";
    dialogHtml += "            <div class='dialog_table_r'>";
    dialogHtml += "              <span class='button_wrapper pop_confirm' id='pop_confirm'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_ok + '</a></span>';
    dialogHtml += '                      </span></span></span>';
    if (laterFunc != '' && laterFunc != ' ' && laterFunc != null) {
        dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper pop_Cancel' id='pop_Later'>";
        dialogHtml += "                  <span class='button_left'>";
        dialogHtml += "                      <span class='button_right'>";
        dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_later + '</a></span>';
        dialogHtml += '                      </span></span></span>';
    }
    
    if (cancelFunc != '' && cancelFunc != ' ' && cancelFunc != null) {
        dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper pop_Cancel' id='pop_Cancel'>";
        dialogHtml += "                  <span class='button_left'>";
        dialogHtml += "                      <span class='button_right'>";
        dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_cancel + '</a></span>';
        dialogHtml += '                      </span></span></span>';
    }
    dialogHtml += '            </div>';
    dialogHtml += '        </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='dialog_bottom'></div>";
    dialogHtml += '</div>';

    $('.body_bg').before(dialogHtml);
    $('#pop_confirm').bind('click', function() {
        clearDialog();

        if (typeof (okFunc) == 'function') {
            okFunc();
        }
        okFunc = null;
        laterFunc = null;
        cancelFunc = null;
        g_main_displayingPromptStack.pop();
        return false;
    });
     $('#pop_Later').bind('click', function() {
        clearDialog();

        if (typeof (okFunc) == 'function') {
            laterFunc();
        }
        okFunc = null;
        laterFunc = null;
        cancelFunc = null;
        g_main_displayingPromptStack.pop();
        return false;
    });
    $('#pop_Cancel').bind('click', function() {
        if (typeof (cancelFunc) == 'function') {
            cancelFunc();
        }
        okFunc = null;
        laterFunc = null;
        cancelFunc = null;
        g_main_displayingPromptStack.pop();
        //return false;
    });

    reputPosition($('.dialog'), $('#div_wrapper'));

    g_main_displayingPromptStack.push('pop_confirm');

    disableTabKey();
}


//show info dialog
function showInfoDialog(tipsContent) {
    if ($('.info_dialog').size() == 0) {
        var hideInfo;
        clearTimeout(hideInfo);
        var dialogHtml = '';
        if ($('#div_wrapper').size() < 1) {
            dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; position:absolute; width:100%; height:100%; z-index:-1; display: block;'> </iframe></div>";
        }
        dialogHtml += "<div class='info_dialog'>";
        dialogHtml += "    <div class='info_dialog_top'></div>";
        dialogHtml += "    <div class='info_dialog_content'>";
        dialogHtml += "        <div class='info_dialog_header'>";
        dialogHtml += "            <span class='dialog_header_left'>" + common_note + "<input type='text' id='hidden_focus' value='focus'style='display:none;width:0;border:0;height:0;'></span>";
        dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='' class='info_dialog_close_btn'><img src='../res/dialog_close_btn.gif' title='' alt='' /></a></span>";
        dialogHtml += '        </div>';
        dialogHtml += "        <div class='info_dialog_table'><div class='info_content'>" + tipsContent + '</div></div>';
        dialogHtml += '    </div>';
        dialogHtml += "    <div class='info_dialog_bottom'></div>";
        dialogHtml += '</div>';

        $('.body_bg').before(dialogHtml);

        $('.info_dialog_close_btn').bind('click', function() {
            clearTimeout(hideInfo);
            $('.info_dialog').remove();
            $('#div_wrapper').remove();
            setFocusToChildWindow();
        });

        reputPosition($('.info_dialog'), $('#div_wrapper'));
        disableTabKey();
        setTimeout(function(){
            $("#hidden_focus").show();
            $("#hidden_focus").focus();
            $("#hidden_focus").hide();
        },0);
        hideInfo = setTimeout(function() {
            $('.info_dialog').remove();
            $('#div_wrapper').remove();
            setFocusToChildWindow();
        }, g_feature.dialogdisapear);
    }
}

//function for show jquery qtip
function showQtip(showTarget, content, delay) {
    var $showTarget = $('#' + showTarget);
    if ($showTarget) {
        $showTarget.qtip({
            content: content,
            position: {
                corner: {
                    tooltip: 'topMiddle',
                    target: 'bottomMiddle'
                }
            },
            show: {
                when: false,
                ready: true
            }
        });
        if (delay) {
            setTimeout(function() {
                $showTarget.qtip('destroy');
            }, delay);
        }
        else {
            setTimeout(function() {
                $showTarget.qtip('destroy');
            }, g_feature.tip_disapear);
        }
    }
    $showTarget.focus();
}

function clearDialog() {
    $('.dialog:not(#upload_dialog), .info_dialog, .login_dialog, #div_wrapper').remove();
    enableTabKey();
    if (g_is_login_opened) {
        g_is_login_opened = false;
        getDeviceStatus();
    }
}

function clearInfoDialog() {
    if (g_is_login_opened) {
        g_is_login_opened = false;
        getDeviceStatus();
    }
}

/*************Function for ie6 png**************/
function correctPNG() {
    var i = 0;
    for (i = 0; i < document.images.length; i++) {
        var img = document.images[i];
        var imgName = img.src.toUpperCase();
        if (imgName.substring(imgName.length - 3, imgName.length) == 'PNG') {
            var imgID = (img.id) ? "id='" + img.id + "' " : '';
            var imgClass = (img.className) ? "class='" + img.className + "' " : '';
            var imgTitle = (img.title) ? "title='" + img.title + "' " : "title='" + img.alt + "' ";
            var imgStyle = 'display:inline-block;' + img.style.cssText;
            if (img.align == 'left') {
                imgStyle = 'float:left;' + imgStyle;
            }

            if (img.align == 'right') {
                imgStyle = 'float:right;' + imgStyle;
            }

            if (img.parentNode.href) {
                imgStyle = 'cursor:hand;' + imgStyle;
            }

            var strNewHTML = '<span ' + imgID + imgClass + imgTitle + ' style=\"' + 'width:' + img.width + 'px; height:' + img.height + 'px;' + imgStyle + ';' + 'filter:progid:DXImageTransform.Microsoft.AlphaImageLoader' + "(src=\'" + img.src + "\', sizingMethod='scale');\"></span>";
            img.outerHTML = strNewHTML;
            i = i - 1;
        }
    }
}

/***********************Function list (begin)***********************/

/***********Get and save data (begin)**********/
// internal use only
function _createNodeStr(nodeName, nodeValue) {
    return '<' + nodeName + '>' + nodeValue + '</' + nodeName + '>';
}

// internal use only
function _recursiveObject2Xml(name, obj) {
    var xmlstr = '';
    if (typeof (obj) == 'string' || typeof (obj) == 'number') {
        xmlstr = _createNodeStr(name, obj);
    }
    else if (jQuery.isArray(obj)) {
        jQuery.each(obj, function(idx, item) {
            xmlstr += _recursiveObject2Xml(name, item);
        });
    }
    else if (typeof (obj) == 'object') {
        xmlstr += '<' + name + '>';
        jQuery.each(obj, function(propName, propVal) {
            xmlstr += _recursiveObject2Xml(propName, propVal);
        });
        xmlstr += '</' + name + '>';
    }
    return xmlstr;
}

// convert an object to xml string.
// name: root tag name of XML
// obj:  object which is to be convert to XML
function object2xml(name, obj) {
    var xmlstr = '<?xml version="1.0" encoding="UTF-8"?>';
    xmlstr += _recursiveObject2Xml(name, obj);
    return xmlstr;
}

// internal use only
function _recursiveXml2Object($xml) {
    if ($xml.children().size() > 0) {
        var _obj = {};
        $xml.children().each(function() {
            var _childObj = ($(this).children().size() > 0) ? _recursiveXml2Object($(this)) : $(this).text();
            if ($(this).siblings().size() > 0 && $(this).siblings().get(0).tagName == this.tagName) {
                if (_obj[this.tagName] == null) {
                    _obj[this.tagName] = [];
                }
                _obj[this.tagName].push(_childObj);
            }
            else {
                _obj[this.tagName] = _childObj;
            }
        });
        return _obj;
    }
    else {
        return $xml.text();
    }
}

// convert XML string to an Object.
// $xml, which is an jQuery xml object.
function xml2object($xml) {
    var obj = new Object();
    if ($xml.find('response').size() > 0) {
        var _response = _recursiveXml2Object($xml.find('response'));
        obj.type = 'response';
        obj.response = _response;
    }
    else if ($xml.find('error').size() > 0) {
        var _code = $xml.find('code').text();
        var _message = $xml.find('message').text();
        log.warn('MAIN : error code = ' + _code);
        log.warn('MAIN : error msg = ' + _message);
        obj.type = 'error';
        obj.error = {
            code: _code,
            message: _message
        };
    }
    else if ($xml.find('config').size() > 0) {
        var _config = _recursiveXml2Object($xml.find('config'));
        obj.type = 'config';
        obj.config = _config;
    }
    else {
        obj.type = 'unknown';
    }
    return obj;
}

/*************Get and save data (end)**************/
/***************Get and save data (begin)******/

// urlstr : URL of the Restful interface.
// callback_func : Callback function to handle response, this callback function
// have one parameter
// callback_func($xml) - $xml : a jQuery XML object which is successfully get
// from getAjaxData.
// options.sync
// options.timout
// options.errorCB
function getAjaxData(urlstr, callback_func, options) {
    var myurl = AJAX_HEADER + urlstr + AJAX_TAIL;
    var isAsync = true;
    var nTimeout = AJAX_TIMEOUT;
    var errorCallback = null;

    if (options) {
        if (options.sync) {
            isAsync = (options.sync == true) ? false : true;
        }
        if (options.timeout) {
            nTimeout = parseInt(options.timeout, 10);
            if (isNaN(nTimeout)) {
                nTimeout = AJAX_TIMEOUT;
            }

        }
        errorCallback = options.errorCB;
    }

    $.ajax({
        async: isAsync,
        //cache: false,
        type: 'GET',
        timeout: nTimeout,
        url: myurl,
        //dataType: ($.browser.msie) ? "text" : "xml",
        error: function(XMLHttpRequest, textStatus) {
            try {
                if (jQuery.isFunction(errorCallback)) {
                    errorCallback(XMLHttpRequest, textStatus);
                }
                log.error('MAIN : getAjaxData(' + myurl + ') error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus ' + textStatus);
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            //log.debug('MAIN : getAjaxData(' + myurl + ') sucess.');
            //log.trace(data);
            var xml;
            if (typeof data == 'string' || typeof data == 'number') {
                if (-1 != this.url.indexOf('/api/sdcard/sdcard')) {
                    data = sdResolveCannotParseChar(data);
                }
                if (!window.ActiveXObject) {
                    var parser = new DOMParser();
                    xml = parser.parseFromString(data, 'text/xml');
                }
                else {
                    //IE
                    xml = new ActiveXObject('Microsoft.XMLDOM');
                    xml.async = false;
                    xml.loadXML(data);
                }
            }
            else {
                xml = data;
            }
            if (typeof callback_func == 'function') {
                callback_func($(xml));
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
    });
}

function smsContentDeleteWrongChar(smsStr) {
    return smsStr.replace(/([\x00-\x08]|\x0b|\x0c|[\x0e-\x1f])/g, ' ');
}
function regURL ( str )
{
    var reg = "(((https|http|ftp|rtsp|mms):\/\/)|(www\\.)){1}([A-Za-z0-9\/]+\\.)+[a-z]{2,4}(\/[a-z0-9\?=\%_\&]+)*";
    var matchURL=new RegExp(reg,"ig");
    return str.replace(matchURL,function($1)
    {
        $1_href = $1.indexOf(("//").toLowerCase())==-1?"http://"+$1:$1;
        return "<a href='"+$1_href+"' style='color:blue;text-decoration:underline;' target='_blank' onclick='javascript:stopBubble(event)'>"+$1+"</a>";
    });
}
// urlstr : URL of the Restful interface.
// xml: xml string to be submit to server.
// callback_func : Callback function to handle response, this callback function
// have one parameter
// callback_func($xml) - $xml : a jQuery XML object which is successfully get
// from getAjaxData.
// options.sync
// options.timout
// options.errorCB
function saveAjaxData(urlstr, xml, callback_func, options) {
    var myurl = AJAX_HEADER + urlstr + AJAX_TAIL;
    var isAsync = true;
    var nTimeout = AJAX_TIMEOUT;
    var errorCallback = null;

    if (options) {
        if (options.sync) {
            isAsync = (options.sync == true) ? false : true;
        }
        if (options.timeout) {
            nTimeout = parseInt(options.timeout, 10);
            if (isNaN(nTimeout)) {
                nTimeout = AJAX_TIMEOUT;
            }
        }
        errorCallback = options.errorCB;
    }

    if (('home' == current_href) || ('sms' == current_href)||('quicksetup' == current_href)) {
        startLogoutTimer();
    }
    
    $.ajax({
        async: isAsync,
        //cache: false,
        type: 'POST',
        timeout: nTimeout,
        url: myurl,
        // dataType: ($.browser.msie) ? "text" : "xml",
        data: xml,
        error: function(XMLHttpRequest, textStatus) {
            try {
                if (jQuery.isFunction(errorCallback)) {
                    errorCallback(XMLHttpRequest, textStatus);
                }
                log.error('MAIN : saveAjaxData(' + myurl + ') error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus' + textStatus);
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : saveAjaxData(' + myurl + ') success.');
            log.trace(data);
            var xml;
            if (typeof data == 'string') {
                if (-1 != this.url.indexOf('/api/sms/sms-list') && -1 != data.indexOf('Messages')) {
                    data = smsContentDeleteWrongChar(data);
                }
                if (!window.ActiveXObject) {
                    var parser = new DOMParser();
                    xml = parser.parseFromString(data, 'text/xml');
                }
                else {
                    //IE
                    xml = new ActiveXObject('Microsoft.XMLDOM');
                    xml.async = false;
                    xml.loadXML(data);
                }
            }
            else {
                xml = data;
            }
            if (typeof callback_func == 'function') {
                callback_func($(xml));
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
    });
}

// return true if the AJAX response from server is <response>OK</response>
// obj: object came from $xml
function isAjaxReturnOK(obj) {
    var ret = false;
    if (obj) {
        if (typeof (obj.response) == 'string') {
            if (obj.response.toLowerCase() == 'ok') {
                ret = true;
            }
        }
    }
    return ret;
}

// options.sync
// options.timout
// options.errorCB
function getConfigData(urlstr, callback_func, options) {
    var myurl = '../' + urlstr + '';
    //var myurl = urlstr + "";
    var isAsync = true;
    var nTimeout = AJAX_TIMEOUT;
    var errorCallback = null;

    if (options) {
        if (options.sync) {
            isAsync = (options.sync == true) ? false : true;
        }
        if (options.timeout) {
            nTimeout = parseInt(options.timeout, 10);
            if (isNaN(nTimeout)) {
                nTimeout = AJAX_TIMEOUT;
            }
        }
        errorCallback = options.errorCB;
    }

    $.ajax({
        async: isAsync,
        //cache: false,
        type: 'GET',
        timeout: nTimeout,
        url: myurl,
        //dataType: ($.browser.msie) ? "text" : "xml",
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            try {
                log.debug('MAIN : getConfigData(' + myurl + ') error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus ' + textStatus);
                if (jQuery.isFunction(errorCallback)) {
                    errorCallback(XMLHttpRequest, textStatus);
                }
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : getConfigData(' + myurl + ') success.');
            log.trace(data);
            var xml;
            if (typeof data == 'string' || typeof data == 'number') {
                if (!window.ActiveXObject) {
                    var parser = new DOMParser();
                    xml = parser.parseFromString(data, 'text/xml');
                }
                else {
                    //IE
                    xml = new ActiveXObject('Microsoft.XMLDOM');
                    xml.async = false;
                    xml.loadXML(data);
                }
            }
            else {
                xml = data;
            }
            if (typeof callback_func == 'function') {
                callback_func($(xml));
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
    });
}

/*************Get and save data (end)**************/
function _createFeatureNode(str) {
    if (typeof (str) == 'undefined' || str == null) {
        return null;
    }

    var value = parseInt(str, 10);
    
    //处理str值的类型如下：数字（数字长度大于等于1）+字母的情况
    if(!isNaN(value) && value.toString().length != str.length){ 
        value = str; 
    }
    
    if (isNaN(value)) {
        // not a number
        if (str.toLowerCase().indexOf('true') > -1) {
            return true;
        }
        else if (str.toLowerCase().indexOf('false') > -1) {
            return false;
        }
        else {
            return str;
        }
    }
    else {
        // is a number
        return value;
    }
}

function _recursiveXml2Feature($xml) {
    if ($xml.children().size() > 0) {
        var _obj = {};
        $xml.children().each(function() {
            var _childObj = ($(this).children().size() > 0) ? _recursiveXml2Feature($(this)) : _createFeatureNode($(this).text());
            if ($(this).siblings().size() > 0 && $(this).siblings().get(0).tagName == this.tagName) {
                if (_obj[this.tagName] == null) {
                    _obj[this.tagName] = [];
                }
                _obj[this.tagName].push(_childObj);
            }
            else {
                _obj[this.tagName] = _childObj;
            }
        });
        return _obj;
    }
    else {
        return _createFeatureNode($xml.text());
    }
}

// convert XML string to an Object.
// $xml, which is an jQuery xml object.
function _xml2feature($xml) {
    var obj = null;
    if ($xml.find('config').size() > 0) {
        var _config = _recursiveXml2Feature($xml.find('config'));
        obj = _config;
    }
    return obj;
}

/*
 POST api/global/module-switch HTTP/1.1
 返回报文：
 <?xml version="1.0" encoding="UTF-8"?>
 <response>
 <ussd-enabled>0</ussd-enabled>
 <online-update-enabled>1</online-update-enabled>
 <sms-enabled>1</sms-enabled>
 <sdcard-enabled >0</sdcard-enabled>
 <wifi-enabled>1</wifi-enabled>
 </response>
 */
function getGlobalFeatures() {

    getConfigData('config/global/config.xml', function($xml) {
        g_feature = _xml2feature($xml);
        g_PageUrlTree = g_feature.menu;
        g_helpurl = g_feature.helpurl;
        g_needToLogin = g_feature.login;
        g_needFooter = g_feature.footer;
        
        if(typeof(g_feature.ipv6_enabled) == 'undefined'){
            g_feature_ipv6 = false;
        } else {
            g_feature_ipv6 = g_feature.ipv6_enabled == 1? true:false;
        }
        
    }, {
        sync: true
    });

    getConfigData('config/global/module-switch.xml', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            g_moduleswitch = ret.response;
            g_module = {
                'ussd_enabled' : g_moduleswitch.ussd_enabled == '1' ? true : false,
                'sdcard_enabled' : g_moduleswitch.sdcard_enabled == '1' ? true : false,
                'advanced_enabled' : g_moduleswitch.advanced_enabled == '1' ? true : false,
                'connection_enabled' : g_moduleswitch.connection_enabled == '1' ? true : false,
                'maintenance_enabled' : g_moduleswitch.maintenance_enabled == '1' ? true : false,
                'online_update_enabled' : g_moduleswitch.bbou_enabled == '1' ? true : false,
                'wifi_enabled' : g_moduleswitch.wifi_enabled == '1' ? true : false,
                'sms_enabled' : g_moduleswitch.sms_enabled == '1' ? true : false,
                'autoapn_enabled' : g_feature.autoapn_enabled == '1' ? true : false,
                'statistic_enabled' : g_moduleswitch.statistic_enabled == '1' ? true : false,
                'checklogin_enabled' : g_feature.login == '1' ? true : false,
                'help_enabled' : g_moduleswitch.help_enabled == '1' ? true : false,
                'ap_station_enabled' : g_feature.ap_station_enabled == '1' ? true : false,
                'multi_ssid_enabled' : g_feature.multi_ssid_enabled == '1' ? true : false,
                'quicksetup_enable' : g_moduleswitch.quicksetup_enable == '1' ? true : false
            };

            g_needHelp = g_module.help_enabled;
        }
    }, {
        sync: true
    });

}

/********************Function formatFloat*********************/
function formatFloat(src, pos) {
    return Math.round(src * Math.pow(10, pos)) / Math.pow(10, pos);
}

/*************************Function for current connection time in connection
 * page******************************/
function getCurrentTime(time) {
    var final_time = '';
    var times = parseInt(time, 10);

    //format time like : 02:15:38

    var hours = parseInt((times / 3600), 10);
    if (hours > 9) {
        final_time += hours + common_colon;
    }
    else if (hours >= 0) {
        final_time += '0' + hours + common_colon;
    }

    times = times - hours * 3600;

    var minutes = parseInt(times / 60, 10);
    if (minutes > 9) {
        final_time += minutes + common_colon;
    }
    else if (minutes > 0) {
        final_time += '0' + minutes + common_colon;
    }
    else if (minutes == 0) {
        final_time += '00' + common_colon;
    }
    times = times - minutes * 60;

    //handle second display
    if (times > 9) {
        final_time += times;
    }
    else if (times > 0) {
        final_time += '0' + times;
    }
    else if (times == 0) {
        final_time += '00';
    }

    return final_time;
}

function getCurrentTimeArray(time) {
    var times_array = new Array();
    var times = parseInt(time, 10);
    var days = parseInt((times / 3600) / 24, 10);
    times_array.push(days);
    times = times - days * 3600 * 24;
    var hours = parseInt(times / 3600, 10);
    times_array.push(hours);
    times = times - hours * 3600;
    var minutes = parseInt(times / 60, 10);
    times_array.push(minutes);
    var secondes = times - minutes * 60;
    times_array.push(secondes);
    return times_array;
}

/****************************Function list (end)*******************/

function clickMenu(menu) {
    var getEls = $('.' + menu + ' li ');

    $.each(getEls, function(i) {
        $(this).click(function() {
            if ($(this).hasClass('sub') && $(this).hasClass('click')) {
                $(this).removeClass('click');
            }
            else if ($(this).hasClass('nosub')) {
                return true;
            }
            else {
                $(getEls).removeClass('click');
                $(this).addClass('click');
            }
        });
        $(this).mouseover(function() {
            $(this).addClass('menu_hover');
        });
        $(this).mouseout(function() {
            $(this).removeClass('menu_hover');
        });
    });
}

//Judgement if the profile can be edit
function button_enable(button_id, enable) {
    var parent = $('#' + button_id);

    if (enable == '1') {
        parent.removeClass('disable_btn');

    }
    else if (enable == '0') {
        parent.addClass('disable_btn');

    }
}

// refresh current page
function refresh() {
    window.location.reload();
}

// goto page without history
function gotoPageWithoutHistory(url) {
    log.debug('MAIN : gotoPageWithoutHistory(' + url + ')');
    window.location.replace(url);
}

// goto page with history
function gotoPageWithHistory(url) {
    log.debug('MAIN : gotoPageWithHistory(' + url + ')');
    window.location.href = url;
}

function showErrorUnderTextbox(idOfTextbox, errormsg, label_id) {

    var errorLabel = '';
    if (label_id != null && label_id != '' && label_id != ' ') {
        errorLabel = "<div class='error_message'><label id='" + label_id + "'>" + errormsg + '</label><div>';
    }
    else {
        errorLabel = "<div class='error_message'><label>" + errormsg + '</label><div>';
    }
    $('#' + idOfTextbox).after(errorLabel);
}

function showErrorUnderTextbox_Parent(idOfTextbox, errormsg, label_id) {

    var errorLabel = '';
    if (label_id != null && label_id != '' && label_id != ' ') {
        errorLabel = "<div class='error_message'><label id='" + label_id + "'>" + errormsg + '</label><div>';
    }
    else {
        errorLabel = "<div class='error_message'><label>" + errormsg + '</label><div>';
    }
    parent.$('#' + idOfTextbox).after(errorLabel);
}

function clearAllErrorLabel() {
    $('.error_message').remove();
}

function clearAllErrorLabel_Parent() {
    parent.$('.error_message').remove();
}

function showWaitingDialog(tipTitle, tipContent, callback_func) {

    $('#div_wrapper').remove();
    var tab = "<table colspacing='0' cellspacing='0' id='wait_table' class='wait_table'>" + "<tr><td class='wait_table_top'></td></tr>" + "<tr><td class='wait_table_header'><label class='wait_title'>" + tipTitle + "</label><span class='wait_dialog_btn' id='wait_dialog_btn' ><img src='../res/dialog_close_btn.gif' alt='' /></span></td></tr>" + "<tr><td class='wait_table_content'><div class='wait_wrapper'><div class='wait_image'><img src='../res/waiting.gif' alt=''/></div><div class='wait_str'>" + tipContent + '</div></div></td></tr>' + "<tr><td class='wait_table_bottom'></td></tr>" + '</table>';
    var wait_div = "<div id='div_wrapper'></div>" + tab;
    $('body').append(wait_div);
    reputPosition($('#wait_table'), $('#div_wrapper'));
    $('#wait_dialog_btn').bind('click', function() {
        closeWaitingDialog();
    });
    if (typeof (callback_func) == 'function') {
        callback_func();
    }
    disableTabKey();
}

function showWaitingDialog_network(tipTitle, tipContent, callback_func) {
    $('#div_wrapper').remove();

    var tab = "<table colspacing='0' cellspacing='0' id='wait_table' class='wait_table'>" + "<tr><td class='wait_table_top'></td></tr>" + "<tr><td class='wait_table_header'><label class='wait_title'>" + tipTitle + "</label><span class='wait_dialog_btn' id='wait_dialog_btn' ><img src='../res/dialog_close_btn.gif' alt='' /></span></td></tr>" + "<tr><td class='wait_table_content'><div class='wait_wrapper'><div class='wait_image'><img src='../res/waiting.gif' alt=''/></div><div class='wait_str'>" + tipContent + '</div></div></td></tr>' + "<tr><td class='wait_table_bottom'></td></tr>" + '</table>';
    var wait_div = "";
    	
    if($.browser.msie && parseInt($.browser.version.charAt(0), 10) === 6 ) {
        wait_div += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; width:100%; height:100%; display: block;'> </iframe></div>";
    }else{
        wait_div += "<div id='div_wrapper'></div>";
    }
    wait_div += tab;
    $('body').append(wait_div);
    reputPosition($('#wait_table'), $('#div_wrapper'));
    $('#wait_dialog_btn').bind('click', function() {
        closeWaitingDialog();
    });
    if (typeof (callback_func) == 'function') {
        callback_func();
    }
    disableTabKey();
}

//
function closeWaitingDialog(callback_func) {
    
    if ($('.info_dialog').size() == 0) {
        $('#div_wrapper').remove();
        }
    $('#wait_table').remove();
    enableTabKey();
    if (typeof (callback_func) == 'function') {
        callback_func();
    }
}

/*********************** header ************************/
function getDeviceStatus() {
    log.debug('MAIN : getDeviceStatus()');
    if (g_is_login_opened) {
        return;
    }
    getAjaxData('api/monitoring/status', function($xml) {
        log.debug('MAIN : getDeviceStatus() get monitoring success');
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            G_MonitoringStatus = ret;
            DATA_READY.statusReady = true;
            execEachFunList();
            getPlmn();
            
            getAjaxData('api/monitoring/check-notifications', function($xml) {
                G_NotificationsStatus = null;
                var ret = xml2object($xml);
                if (ret.type == 'response') {
                    G_NotificationsStatus = ret.response;
                    showNotification();
                    DATA_READY.notificationsReady = true;
                    execEachFunList();
                }
            });
        }
    });
    if (g_module.ap_station_enabled) {
        getAjaxData('api/wlan/station-information', function($xml) {
            var ret = xml2object($xml);
            G_StationStatus = ret;
            DATA_READY.apStationReady = true;
            execEachFunList();
        });
    }

    function execEachFunList() {
        if ((g_module.ap_station_enabled && false == DATA_READY.apStationReady) || false == DATA_READY.statusReady || false == DATA_READY.notificationsReady) {
            return;
        }
        log.debug('MAIN : execFunList()');
        execFunList();
        DATA_READY.statusReady = false;
        DATA_READY.notificationsReady = false;
        DATA_READY.apStationReady = false;
    }

    function execFunList() {
        $.each(STATUS_LISTENER_FUN_LIST, function(n, value) {
            log.debug('MAIN : execFunList() execute ' + value);
            eval(value);
        });
    }

    setTimeout(getDeviceStatus, g_feature.update_interval);
}

function getPlmn() {
    g_plmn_rat = "";
    if((WIRELESS_SERVICE_STATUS_AVAIABLE == G_MonitoringStatus.response.ServiceStatus)  && (MACRO_SIM_STATUS_USIM_N != G_MonitoringStatus.response.SimStatus))
    {
        switch(G_MonitoringStatus.response.CurrentNetworkType) {
            case MACRO_NET_WORK_TYPE_GSM:
            case MACRO_NET_WORK_TYPE_GPRS:
            case MACRO_NET_WORK_TYPE_EDGE:
                g_plmn_rat =  MACRO_CURRENT_NETWOORK_2G;
                break;
            case MACRO_NET_WORK_TYPE_WCDMA:
            case MACRO_NET_WORK_TYPE_HSDPA:
            case MACRO_NET_WORK_TYPE_HSUPA:
            case MACRO_NET_WORK_TYPE_HSPA:
            case MACRO_NET_WORK_TYPE_TDSCDMA:
            case MACRO_NET_WORK_TYPE_HSPA_PLUS:
            case MACRO_NET_WORK_TYPE_DC_HSPA:               
            case DSAT_SYS_SUBMODE_TDS:
            case DSAT_SYS_SUBMODE_TDS_HSDPA:
            case DSAT_SYS_SUBMODE_TDS_HSUPA:
            case DSAT_SYS_SUBMODE_TDS_HSPA:
                g_plmn_rat =  MACRO_CURRENT_NETWOORK_3G;
                break;
            case MACRO_NET_WORK_TYPE_LTE:
                g_plmn_rat =  MACRO_CURRENT_NETWOORK_4G;
                break;
            default:
                break;
        }
    }
    
    return g_plmn_rat;
}

function initialStatusIcon() {
        var corner = '';
    if (LANGUAGE_DATA.current_language == 'ar_sa') {
            corner =  {
                    tooltip: 'topLeft',
                    target: 'bottomLeft'
                };
    } else{
        corner = {
                    tooltip: 'topRight',
                    target: 'bottomLeft'
                };
        
    }
    addStatusListener('getIconStatus()');
    if (g_module.sms_enabled) {
        $('#tooltip_sms').qtip({
            content: '<b>' + common_new_message + '</b>',
            position: {
                corner:corner
            },
            style: {
                name: 'sms'
            }
        });
    }
    if (g_module.online_update_enabled) {
        $('#tooltip_update').qtip({
            content: '<b>' + common_new_version + '</b>',
            position: {
                corner:corner
            },
            style: {
                name: 'upd'
            }
        });
    }

    $('#tooltip_sim').qtip({
        content: '<b>' + STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state + '</b>',
        position: {
            corner: corner
        },
        style: {
            name: 'sim'
        }
    });

    $('#tooltip_plmn').qtip({
        content: '<b>' + STATUS_BAR_ICON_STATUS.plmn_tooltip_state + '</b>',
        position: {
            corner: corner
        },
        style: {
            name: 'signal'
        }
    });

    if (g_module.ap_station_enabled) {
        $('#tooltip_station').qtip({
            content: '<b>' + STATUS_BAR_ICON_STATUS.station_tooltip_state + '</b>',
            position: {
                corner:corner
            },
            style: {
                name: 'station'
            }
        });
    }
    $('#tooltip_wan').qtip({
        content: '<b>' + STATUS_BAR_ICON_STATUS.wan_tooltip_state + '</b>',
        position: {
            corner: corner
        },
        style: {
            name: 'wan'
        }
    });

    $('#tooltip_wifi').qtip({
        content: '<b>' + STATUS_BAR_ICON_STATUS.wifi_tooltip_state + '</b>',
        position: {
            corner: corner
        },
        style: {
            name: 'wifi'
        }
    });

    $('#tooltip_battery').qtip({
        content: '<b>' + STATUS_BAR_ICON_STATUS.battery_tooltip_state + '</b>',
        position: {
            corner: corner
        },
        style: {
            name: 'battery'
        }
    });

    //tooltip_sms_full
    $('#tooltip_sms_full').qtip({
        content: '<b>' + Label_sms_storage_full + '</b>',
        position: {
            corner: corner
        },
        style: {
            name: 'sms_full'
        }
    });

}

function getIconStatus() {
    var header_ret = G_MonitoringStatus;
    var station_ret = G_StationStatus;
    if (header_ret != null && header_ret.type == 'response') {
        header_icon_status.ConnectionStatus = header_ret.response.ConnectionStatus;
        header_icon_status.X_IPv6ConnectionStatus = header_ret.response.X_IPv6ConnectionStatus;
        header_icon_status.X_IPv4WireLineConnectionStatus = header_ret.response.X_IPv4WireLineConnectionStatus;
        header_icon_status.X_IPv6WireLineConnectionStatus = header_ret.response.X_IPv6WireLineConnectionStatus;
        /*if (header_ret.response.SignalStrength == 0) {
         header_icon_status.SignalStrength = 0;
         }
         else {
         var temp = parseInt(header_ret.response.SignalStrength, 10) / 20;
         header_icon_status.SignalStrength = (temp == 0) ? "1" : temp.toString();
         }*/

        if (typeof(header_ret.response.SignalIcon) != 'undefined' ||
        header_ret.response.SignalIcon != null) {
            header_icon_status.SignalStrength = header_ret.response.SignalIcon;
        }
        else {
            header_icon_status.SignalStrength = parseInt(header_ret.response.SignalStrength / 20, 10).toString();
        }

        header_icon_status.BatteryStatus = header_ret.response.BatteryStatus;
        header_icon_status.BatteryLevel = header_ret.response.BatteryLevel;
        header_icon_status.SimStatus = header_ret.response.SimStatus;
        header_icon_status.WifiStatus = header_ret.response.WifiStatus;
        header_icon_status.CurrentNetworkType = header_ret.response.CurrentNetworkType;
        header_icon_status.ServiceStatus = header_ret.response.ServiceStatus;
        header_icon_status.x_WireLineLinkStatus = header_ret.response.x_WireLineLinkStatus;
        //Battery Status
        switch (header_icon_status.BatteryStatus) {
            case MACRO_BATTERY_STATUS_NORMAL:
                getBatteryLevel(header_icon_status.BatteryLevel);
                break;
            case MACRO_BATTERY_STATUS_LOW:
                $('#battery_gif').css({
                    background: 'url(../res/battery_low.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.battery_tooltip_state = battery_prower_low;
                break;

            case MACRO_BATTERY_STATUS_ELECT:

                if (null != g_lastBatteryStatus && MACRO_BATTERY_STATUS_ELECT == g_lastBatteryStatus) {
                    break;
                }
                $('#battery_gif').css({
                    background: 'url(../res/battery_elect.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.battery_tooltip_state = battery_recharging;
                break;
            default:

                $('#battery_gif').css({
                    background: 'url(../res/battery_level_' + header_icon_status.BatteryLevel + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.battery_tooltip_state = common_battery;
                break;
        }
        g_lastBatteryStatus = header_icon_status.BatteryStatus;

        //WiFi Status
        switch (header_icon_status.WifiStatus) {
            case MACRO_WIFI_OFF:
                $('#wifi_gif').css({
                    background: 'url(../res/wifi_' + MACRO_WIFI_OFF + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.wifi_tooltip_state = wlan_label_wlan_off;
                break;
            case MACRO_WIFI_ON:
                $('#wifi_gif').css({
                    background: 'url(../res/wifi_' + MACRO_WIFI_ON + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.wifi_tooltip_state = wlan_label_wlan_on;
                break;
            default:
                $('#wifi_gif').css({
                    background: 'url(../res/wifi_' + MACRO_WIFI_OFF + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.wifi_tooltip_state = wlan_label_wlan_off;
                break;
        }

        //wan Status
        if((SERVICE_STATUS_AVAIABLE == header_icon_status.ServiceStatus)
            &&((MACRO_CONNECTION_CONNECTED == header_icon_status.ConnectionStatus) 
               ||(MACRO_CONNECTION_IPV6CONNECTED == header_icon_status.X_IPv6ConnectionStatus))){
                $('#wan_gif').css({
                    background: 'url(../res/wan_' + MACRO_CONNECTION_CONNECTED + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.wan_tooltip_state = dialup_label_wan_connect;
        }else if((WIRELINESERVICE_STATUS_AVAIABLE == header_icon_status.x_WireLineLinkStatus)
                &&((MACRO_WIRELINE_CONNECTION_CONNECTED == header_icon_status.X_IPv4WireLineConnectionStatus) 
                   ||(MACRO_WIRELINE_CONNECTION_IPV6CONNECTED == header_icon_status.X_IPv6WireLineConnectionStatus))){
                $('#wan_gif').css({
                    background: 'url(../res/wan_' + MACRO_CONNECTION_CONNECTED + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.wan_tooltip_state = dialup_label_wan_connect;
        }else{
                $('#wan_gif').css({
                    background: 'url(../res/wan_' + MACRO_CONNECTION_DISCONNECTED + '.gif) 0 0 no-repeat'
                });
                STATUS_BAR_ICON_STATUS.wan_tooltip_state = dialup_label_wan_disconnect;
        }
            
        //sim card or signal Status
        if (MACRO_PIN_REQUIRED == g_main_convergedStatus.SimState)
        {
            $('#sim_signal_gif').css({background: 'url(../res/sim_disable.gif) 0 0 no-repeat'});
            STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = dialup_label_pin_code_required;
        }
        else if (MACRO_PUK_REQUIRED == g_main_convergedStatus.SimState)
        {
            $('#sim_signal_gif').css({background: 'url(../res/sim_disable.gif) 0 0 no-repeat'});
            STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = dialup_label_puk_code_required;
        }
        else if (MACRO_CARD_LOCK_PIN_WRONG == g_main_convergedStatus.SimState)
        {
            $('#sim_signal_gif').css({background: 'url(../res/sim_disable.gif) 0 0 no-repeat'});
            STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = label_hint_pinlock_active;
        }
        else if ((MACRO_SIM_STATUS_USIM_N == header_icon_status.SimStatus) || (MACRO_SIM_STATUS_USIM_PS_CS_N == header_icon_status.SimStatus) || (MACRO_SIM_STATUS_USIM_NE == header_icon_status.SimStatus)) {
            $('#sim_signal_gif').css({
                background: 'url(../res/sim_disable.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = dialup_label_sim_invalid;
        }
        else if ('undefined' == header_icon_status.SimStatus) {
            $('#sim_signal_gif').css({
                background: 'url(../res/sim_disable.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = dialup_label_sim_invalid;
        }
        else if (header_icon_status.SimlockStatus) {
            $('#sim_signal_gif').css({
                background: 'url(../res/sim_disable.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = dialup_label_sim_invalid;
        }
        else {
            switch (header_icon_status.SignalStrength) {
                case MACRO_EVDO_LEVEL_ONE:
                case MACRO_EVDO_LEVEL_TWO:
                    $('#sim_signal_gif').css({
                        background: 'url(../res/signal_' + header_icon_status.SignalStrength + '.gif) 0 0 no-repeat'
                    });
                    STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = common_signal_weak;
                    break;
                case MACRO_EVDO_LEVEL_THREE:
                case MACRO_EVDO_LEVEL_FOUR:
                case MACRO_EVDO_LEVEL_FIVE:
                    $('#sim_signal_gif').css({
                        background: 'url(../res/signal_' + header_icon_status.SignalStrength + '.gif) 0 0 no-repeat'
                    });
                    STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = common_signal_strong;
                    break;
                default:
                    $('#sim_signal_gif').css({
                        background: 'url(../res/signal_' + MACRO_EVDO_LEVEL_ZERO + '.gif) 0 0 no-repeat'
                    });
                    STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state = common_sig_off;
                    break;
            }
        }

    }

    //plmn Status
    switch (g_plmn_rat) {
        case MACRO_CURRENT_NETWOORK_2G:
            rat = '2';
            $('#plmn_gif').css({
                'display' : 'block'
            });

            $('#plmn_gif').css({
                background: 'url(../res/plmn_' + rat + '.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.CurrentNetworkType = plmn_label_2g;
            break;
        case MACRO_CURRENT_NETWOORK_3G:
            rat = '3';
            $('#plmn_gif').css({
                'display' : 'block'
            });

            $('#plmn_gif').css({
                background: 'url(../res/plmn_' + rat + '.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.CurrentNetworkType = plmn_label_3g;
            break;
        case MACRO_CURRENT_NETWOORK_H:
             rat = "h";
             $("#plmn_gif").css({
             "display" : "block"
             });
    
             $("#plmn_gif").css({
             background : "url(../res/plmn_" + rat + ".gif) 0 0 no-repeat"
             });
             STATUS_BAR_ICON_STATUS.CurrentNetworkType = plmn_label_h;
         break;
         case MACRO_CURRENT_NETWOORK_4G:
             rat = "4";
             $("#plmn_gif").css({
             "display" : "block"
             });
    
             $("#plmn_gif").css({
             background : "url(../res/plmn_" + rat + ".gif) 0 0 no-repeat"
             });
             STATUS_BAR_ICON_STATUS.CurrentNetworkType = plmn_label_4g;
         break;
        default:
            $('#plmn_gif').css({
                'display' : 'none'
            });
            $('#plmn_gif').html('');
            break;
    }

    changeTooltipContent();
}

function getBatteryLevel(str) {
    switch (str) {
        case MACRO_BATTERY_LEVEL_ZERO:
        case MACRO_BATTERY_LEVEL_ONE:
        case MACRO_BATTERY_LEVEL_TWO:
        case MACRO_BATTERY_LEVEL_THREE:
        case MACRO_BATTERY_LEVEL_FOUR:

            $('#battery_gif').css({
                background: 'url(../res/battery_level_' + str + '.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.battery_tooltip_state = common_battery;
            break;
        default:

            $('#battery_gif').css({
                background: 'url(../res/battery_level_' + str + '.gif) 0 0 no-repeat'
            });
            STATUS_BAR_ICON_STATUS.battery_tooltip_state = common_battery;
            break;
    }
}

function changeTooltipContent() {
    $('.qtip-signal').find('.qtip-content').html('<b>' + STATUS_BAR_ICON_STATUS.CurrentNetworkType + '</b>');
    $('.qtip-sim').find('.qtip-content').html('<b>' + STATUS_BAR_ICON_STATUS.sim_signal_tooltip_state + '</b>');
    if (g_module.ap_station_enabled) {
        $('.qtip-station').find('.qtip-content').html('<b>' + STATUS_BAR_ICON_STATUS.station_tooltip_state + '</b>');
    }
    $('.qtip-wan').find('.qtip-content').html('<b>' + STATUS_BAR_ICON_STATUS.wan_tooltip_state + '</b>');
    $('.qtip-wifi').find('.qtip-content').html('<b>' + STATUS_BAR_ICON_STATUS.wifi_tooltip_state + '</b>');
    $('.qtip-battery').find('.qtip-content').html('<b>' + STATUS_BAR_ICON_STATUS.battery_tooltip_state + '</b>');
}

/******************************************************************************************************/
function addStatusListener(funName) {
    STATUS_LISTENER_FUN_LIST.push(funName);
    log.debug('MAIN : addStatusListener(' + funName + '), currently ' + STATUS_LISTENER_FUN_LIST.length + ' listener.');
    if (DATA_READY.statusReady && DATA_READY.notificationsReady) {
        log.debug('MAIN : DATA is Ready, execute in addStatusListener');
        eval(funName);
    }
}

function base64encode(str) {
    var out, i, len;
    var c1, c2, c3;
    len = str.length;
    i = 0;
    out = '';
    while (i < len) {
        c1 = str.charCodeAt(i++) & 0xff;
        if (i == len) {
            out += g_base64EncodeChars.charAt(c1 >> 2);
            out += g_base64EncodeChars.charAt((c1 & 0x3) << 4);
            out += '==';
            break;
        }
        c2 = str.charCodeAt(i++);
        if (i == len) {
            out += g_base64EncodeChars.charAt(c1 >> 2);
            out += g_base64EncodeChars.charAt(((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4));
            out += g_base64EncodeChars.charAt((c2 & 0xF) << 2);
            out += '=';
            break;
        }
        c3 = str.charCodeAt(i++);
        out += g_base64EncodeChars.charAt(c1 >> 2);
        out += g_base64EncodeChars.charAt(((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4));
        out += g_base64EncodeChars.charAt(((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6));
        out += g_base64EncodeChars.charAt(c3 & 0x3F);
    }
    return out;
}

function validateInput(username, password) {
    clearAllErrorLabel();
    var validate = true;
    if (username == '') {
        showErrorUnderTextbox('username', common_message_name_empty);
        $("#username").focus();
        return false;
    }
      if (!checkInputChar(username)) {
      showErrorUnderTextbox("username", settings_hint_user_name_not_exist);
      $("#username").val("");
      $("#username").focus();
      $("#password").val("");
       return false;    
     } 
    if (password == '' && username != '') {
        showErrorUnderTextbox('password', dialup_hint_password_empty);
        $('#password').focus();
        return false;
    }
    return validate;
}

//
function login(destnation) {
    var name = $.trim($('#username').val());
    var psd = base64encode($('#password').val());
    var request = {
        Username: name,
        Password: psd
    };
    var valid = validateInput(request.Username, request.Password);
    if (valid) {
        var xmlstr = object2xml('request', request);
        log.debug('xmlstr = ' + xmlstr);
        saveAjaxData('api/user/login', xmlstr, function($xml) {
            log.debug('api/user/login successed!');
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                /*
                 * show username when login successfully
                 */
                $('#username_span').text(name);
                $('#username_span').show();
                $('#logout_span').text(common_logout);

                clearDialog();

                if (typeof(destnation) != 'undefined' &&
                destnation != null) {
                    window.location.href = destnation;
                }
                else
                {
                     startLogoutTimer();
                }
                if (g_is_disconnect_clicked) {
                    index_clickDisconnectBtn();
                }
                else if (g_is_connect_clicked) {
                    index_clickConnectBtn();
                }
            }
            else {
                if (ret.type == 'error') {
                    clearAllErrorLabel();
                    if (ret.error.code == ERROR_LOGIN_PASSWORD_WRONG) {
                        showErrorUnderTextbox('password', system_hint_wrong_password);
                        $('#password').val('');
                        $('#password').focus();
                    }
                    else if (ret.error.code == ERROR_LOGIN_ALREADY_LOGIN) {
                        showErrorUnderTextbox('password', common_user_login_repeat);
                        $('#username').focus();
                    }
                    else if (ret.error.code == ERROR_LOGIN_USERNAME_WRONG) {
                        showErrorUnderTextbox('username', settings_hint_user_name_not_exist);
                        $('#username').focus();
                        $('#username').val('');
                        $('#password').val('');
                    }
                }
            }
        });
    }
}

function userOut() {
        var logOut = {
            Logout: 1
        };

        var submitData = object2xml('request', logOut);
        saveAjaxData('api/user/logout', submitData, function($xml) {
            var ret = xml2object($xml);
            if (ret.type == 'response') {
                if (isAjaxReturnOK(ret)) {
                $("#username_span").hide();
                $("#logout_span").text(common_login);
                gotoPageWithoutHistory(LOGIN_PATH);
                }
            }
        });
}

function loginout() {
    getAjaxData('api/user/state-login', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            if (ret.response.State != 0) { //logout
                window.location.href = LOGIN_PAGE_URL;
            }
            else //login
            {
                showConfirmDialog(common_warning_logout, function() {
                userOut();
                cancelLogoutTimer();
                return false;
                });
            }
        }
    }, {
        sync: true
    });
}

function showNotification() {
    if (G_NotificationsStatus != null) {
        var unreadSmsSize = G_NotificationsStatus.UnreadMessage;
        var smsStorageFull = G_NotificationsStatus.SmsStorageFull;
        var onlineUpdateStatus = G_NotificationsStatus.OnlineUpdateStatus;
        if (g_module.sms_enabled) {

            if (smsStorageFull == '1') {//sms full
                $('#sms_full').css({
                    'display' : 'block'
                });
                $('#tooltip_sms_full').html("<img src = '../res/message.gif' alt='' />");

                $('#sms_gif').css({
                    'display' : 'none'
                });
                $('#tooltip_sms').html('');
            }
            else {
                $('#sms_full').css({
                    'display' : 'none'
                });
                $('#tooltip_sms_full').html('');
                if (unreadSmsSize > 0) { //unread message
                    $('#sms_gif').css({
                        'display': 'block'
                    });
                    $('#tooltip_sms').html("<img src = '../res/unread_message.gif' alt='' />");
                }
                else {
                    $('#sms_gif').css({
                        'display': 'none'
                    });
                    $('#tooltip_sms').html('');
                }
            }
        }
        if (g_module.online_update_enabled) {
            if (onlineUpdateStatus == MACRO_NEWVERSIONFOUND || onlineUpdateStatus == MACRO_READYTOUPDATE) {
                $('#update_gif').css({
                    'display' : 'block'
                });
                $('#tooltip_update').html("<img src = '../res/update_enable.gif'>");
            }
            else {
                $('#update_gif').css({
                    'display' : 'none'
                });
                $('#tooltip_update').html("<img src = '../res/update_disable.gif'>");
            }
        }
    }
}

var USER_MANUAL_PATH = '/usermanual/';
var USER_MANUAL_FILE_NAME = "00-content.html";

function getUserManualUrl(htmlname) {
    var i = 0;
    var helpUrl = document.getElementById("help_url");
    var current_language = LANGUAGE_DATA.current_language;
    current_language = current_language.replace('_', '-');
    var manual_head = ".." + USER_MANUAL_PATH + current_language + USER_MANUAL_PATH;
    var manual_filename = null;

    if( typeof helpUrl != "undefined" && helpUrl != null) {
    	if(htmlname != "undefined" && htmlname != ""){
            for(i= 0; i<parent.g_helpurl.Item.length; i++ ) {
            	if(htmlname == parent.g_helpurl.Item[i].htmlid) {
               	    manual_filename = parent.g_helpurl.Item[i].helpurl +".html";
                    break;
            	}
            }
            if(manual_filename == null){
            	manual_filename = USER_MANUAL_FILE_NAME;
            }
    	} else {
        	manual_filename = USER_MANUAL_FILE_NAME;
    	}

    	g_user_manual_url =  manual_head + manual_filename;            
        helpUrl.href = g_user_manual_url;
    }
}

function getPrivacyUrl() {
    var current_language = LANGUAGE_DATA.current_language;
    if(current_language != "zh_cn")
    {
        current_language = "en_us";
    }
    PriacyUrl = "../policy/" + current_language + "_policy.html"
       
}

function checkUrlValidity(url, callback_func)
{ 
    $.ajax({
      async: true,
      url: url,
      type: 'GET',
      complete: function(response) {
          if (typeof callback_func == 'function') {
              var isValidity = false;
                if(response.status == 200) {
                     log.debug(url + "is validity.");
                     isValidity = true;
                } else {
                 log.debug(url + "is not validity, use en_us manual.");
                     isValidity = false;
                 }
                callback_func(isValidity);
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
 });
}

//for dialog
function reputPosition($dialogElement, $dialogDiv) {
    var newTop = 0, newLeft = 0, scTop = 0, scLeft = 0;
    if ($dialogElement) {
        newTop = (getWindowHeight() - $dialogElement.height()) / 2;
        newLeft = (getWindowWidth() - $dialogElement.width()) / 2;
        scTop = $(document).scrollTop();
        scLeft = $(document).scrollLeft();
        newTop = scTop + newTop > 0 ? scTop + newTop : 0;
        newLeft = scLeft + newLeft;
        $dialogElement.css({
            left: newLeft,
            top: newTop
        }).show();
    }
    window.onscroll = function() {
        scTop = $(document).scrollTop();
        scLeft = $(document).scrollLeft();
    };
    window.onresize = function() {
        if (!$dialogElement.is(':visible')) {
            return;
        }
        else {
            $dialogElement.css({
                left: 0,
                top: 0
            });
            if ($dialogDiv) {
                $dialogDiv.css({
                    'width' : 0,
                    'height' : 0
                });
            }
            $(document).scrollTop(scTop);
            $(document).scrollLeft(scLeft);
            reputPosition($dialogElement.last(), $dialogDiv);
        }
    };
    if ($dialogDiv) {
        var div_width = Math.max(getWindowWidth(), getDocumentWidth());
        var div_height = Math.max(getWindowHeight(), getDocumentHeight());
        $dialogDiv.css({
            'width' : div_width,
            'height' : div_height
        });
        $dialogDiv.show();
    }
    //
    function getWindowHeight() {
        return document.documentElement.clientHeight;
    }

    function getWindowWidth() {
        return document.documentElement.clientWidth;
    }

    //
    function getDocumentHeight() {
        if (!$dialogElement) {
            return document.body.scrollHeight;
        }
        else {
            return Math.max($dialogElement.position().top + $dialogElement.height(), document.documentElement.scrollHeight);
        }
    }

    function getDocumentWidth() {
        if (!$dialogElement) {
            return document.documentElement.scrollWidth;
        }
        else {
            return Math.max($dialogElement.position().left + $dialogElement.width(), document.documentElement.scrollWidth);
        }
    }

}

function onKeyup(evt) {
    if (evt.ctrlKey || evt.altKey)// Exclude key press "CRTL+key" & "ALT+key"
    {
        return;
    }

    if (13 == evt.keyCode)// Enter key
    {
        var targetID = '';
        var stackLen = g_main_displayingPromptStack.length;

        if (stackLen > 0) {
            targetID = g_main_displayingPromptStack[stackLen - 1];
        }
        else {
            if ('pincoderequired' == current_href) {
                targetID = 'pinrequired_button_apply';
            }

            if ('pukrequired' == current_href) {
                targetID = 'pukrequired_apply';
            }

            if ('simlockrequired' == current_href) {
                targetID = 'simlock_button_apply';
            }

            if ('sms' == current_href) {
                if (!$('#jump_page').is(':hidden')) {
                    if ('' != $('#jump_page').val()) {
                        targetID = 'jump_page';
                    }
                    else if ('jump_page_index' == document.activeElement.id) {
                        targetID = 'jump_page';
                    }
                    else {
                        log.error('targetID error');
                    }
                }
            }

            if ('login' == current_href) { 
                targetID = 'login'; 
            }

            if('roaming' == current_href){
                targetID = 'login';
            }
            
            if('manual' == current_href){
                targetID = 'login';
            }

        }

        if ('' != targetID) {
            $('#' + targetID).trigger('click');
        }
    }
}

function enableTabKey() {
    $('a').attr('tabindex', '');
    $('input').attr('tabindex', '');
    $('select').attr('tabindex', '');
}

function disableTabKey() {
    $('a').attr('tabindex', '-1');
    $('input').attr('tabindex', '-1');
    $('select').attr('tabindex', '-1');
}

function startLogoutTimer()
{
    clearTimeout(g_logoutTimer);
    g_logoutTimer = setTimeout(userOut, LOGOUT_TIMEOUT_MAIN);
}

function cancelLogoutTimer()
{
    clearTimeout(g_logoutTimer);
}




function stopBubble( e )
{
    if ( e && e.stopPropagation )
    {
        e.stopPropagation();
    }
    else
    {
        window.event.cancelBubble=true;
    }
}

function sdResolveCannotParseChar(xmlStr) {
    return xmlStr.replace(/(\&|\')/g, function($0, $1) {
        return {
            '&' : '&amp;' ,
            "'" : '&#39;'
        }[$1];
    }
    );
}

