/*
Index page for hilink
Author cxcai
Initialization connection status.
Jugdment which page will be redirected follow pin, sim and connection status.
Load main menu
*/
// JavaScript Document

//
var CURRENT_NETWORK_NO_SERVICE = 0;
var SERVICE_DOMAIN_NO_SERVICE = 0;
var SERVICE_STATUS_AVAIABLE = 2;
var WIRELINESERVICE_STATUS_AVAIABLE = "1";
var MACRO_CONNECTION_IPV6CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_IPV6CONNECTED = '2';
var g_index_wlan_basic_settings = null;
var g_wlan_security_settings = null;
var g_conection_device = "1";
var g_isAutoConnect = true;
var g_wirelessAutoConnect = true;
var g_wirelineAutoConnect = true;
var g_dialup_action = {
    Action: '1'
};

var g_mouse_on_out_event = 0;
var DIALUP_MODE_MANUAL = 1;

/*
 when the user clicks connect/disconnect button, UI should stop updating
 connection
 status for just one cycle interval(default 3s).
 */
var g_freezeConnectionUpdate = false;

//Button connection or disconnection click effect
function index_clickDisconnectBtn() {
    g_is_connect_clicked = false;
    $('#disconnect_btn').removeClass('mouse_on');
    button_enable('disconnect_btn', '0');
    setConnectionLink(dialup_label_disconnecting);
    g_dialup_action.Action = '0';
    index_sendConnectionAction();
}

function index_clickConnectBtn() {
    g_is_disconnect_clicked = false;
    setConnectionLink(dialup_label_connecting);
    $('#connect_btn').removeClass('mouse_on');
    button_enable('connect_btn', '0');
    g_dialup_action.Action = '1';
    index_sendConnectionAction();
}

function index_clickCancelBtn() {
    g_dialup_action.Action = '0';
    index_sendConnectionAction();
}

function index_sendConnectionAction() {

    var dialup_xml = object2xml('request', g_dialup_action);
    g_freezeConnectionUpdate = true;
    saveAjaxData('api/dialup/dial', dialup_xml, function($xml) {
        var dialup_ret = xml2object($xml);
        if (isAjaxReturnOK(dialup_ret)) {
            log.debug('api/dialup/dial ok');
        }
        else {
            log.debug('api/dialup/dial error');
        }
    });
}

function setHtml(){
    $.cookies.set('cookie', 'setting,dialup,connections', { expiresAt: new Date().getTime()+5*60*1000 });
}
function gotowanadvance(){
    $.cookies.set('cookie', 'setting,system,wanstatus', { expiresAt: new Date().getTime()+5*60*1000 });
}
function gotolanadvance(){
    $.cookies.set('cookie', 'setting,system,lanstatus', { expiresAt: new Date().getTime()+5*60*1000 });
}
function SetHotlinks() {
    var hotlinksHtml = '';
    var hasItem = false;

    var hotlinksurl = [];
    hotlinksurl = g_feature.hotlinks.items.item;

    if (typeof(hotlinksurl) == 'undefined') {
        return;
    }

    hotlinksHtml = '<table>';
    hotlinksHtml += " <tr  id = 'hotlink_hide'>";

    if (jQuery.isArray(hotlinksurl)) {
        $.each(hotlinksurl, function(n, value) {
            if (hotlinksurl[n] != "") {
                hotlinksHtml += '<td>';
                hotlinksHtml += '<span>';
                hotlinksHtml += "   <a id='hotlinks_" + n + "' href='" + value + "'>";
                hotlinksHtml += '   </a>';
                hotlinksHtml += '</span>';
                hotlinksHtml += '</td>';
                hasItem = true;
            }
        });
    }
    else {
        hotlinksHtml += '<td>';
        hotlinksHtml += '<span>';
        hotlinksHtml += "   <a id='hotlinks_0' href='" + hotlinksurl + "'></a>";
        hotlinksHtml += '</span>';
        hotlinksHtml += '</td>';
    }

    if(!hasItem)
    {
        $('.hot_links').attr('style', 'border-top:0px;');
    }
    
    hotlinksHtml += '  </tr>';
    hotlinksHtml += '</table>';

    $('.hotlinks').html(hotlinksHtml);

    /*
     * show hotlinks icon
     */
    if (jQuery.isArray(hotlinksurl)) {
        $.each(hotlinksurl, function(n, value) {
            var temp = '#hotlinks_' + n;
            $(temp).css({
                background: 'url(../res/hotlinks_' + n + '.gif) center 0 no-repeat'
            });
        });
    }
    else {
        $('#hotlinks_0').css({
            background: 'url(../res/hotlinks_0.gif) center 0 no-repeat'
        });
    }

}

function createConnectionButton(button_dis, button_id) {
    if (!g_isAutoConnect) {
        if (1 == g_mouse_on_out_event) {
        $('#index_connection_button').html(create_button_html(button_dis, button_id, 'mouse_on'));
        }
        else {
            $('#index_connection_button').html(create_button_html(button_dis, button_id));
        }
    }
    
}
/* just in this function can change index_plmn_name and status_img */
function GetPLMN() {
    var rat = '';
    switch (g_plmn_rat) {
        case MACRO_CURRENT_NETWOORK_2G:
            rat = plmn_label_2g;
            break;
        case MACRO_CURRENT_NETWOORK_3G:
            rat = plmn_label_3g;    
            break;
        case MACRO_CURRENT_NETWOORK_H:
            rat = plmn_label_h;
            break;
        case MACRO_CURRENT_NETWOORK_4G:
            rat = plmn_label_4g;
            break;
        default:
            rat = "";
            break;
    }
    $('#status_img').html('<p>' + rat + '</p>');
    
    if(SERVICE_STATUS_AVAIABLE != G_MonitoringStatus.response.ServiceStatus)
    {
        if((G_MonitoringStatus.response.SimStatus == "0" )&& (G_MonitoringStatus.response.simlockStatus == "1")){
            // simlock active
            $('#index_plmn_name').html(dialup_hint_simlock_active);
        }else if(g_main_convergedStatus.SimState == MACRO_CARD_LOCK_PIN_WRONG){
            // pinlock active
            $('#index_plmn_name').html(label_hint_pinlock_active);
        }else if((g_conection_device == "2") 
             && (G_MonitoringStatus.response.x_WireLineLinkStatus == WIRELINESERVICE_STATUS_AVAIABLE )){
             //wireline connected, not show no network
            $('#index_plmn_name').html("");
        }else{
            // no coverage
            $('#index_plmn_name').html(dialup_hint_no_network);
        }                
        return;
    }
    var plmn_name = '';
    getAjaxData('api/net/current-plmn', function($xml) {
        var plmn_ret = xml2object($xml);
        if ('response' == plmn_ret.type) {
            if (null == plmn_ret ||
            '' == plmn_ret.response.State ||
            ' ' == plmn_ret.response.State ||
            null == plmn_ret.response.State) {
                $('#index_plmn_name').text('');
            }
            else {
                if (typeof(plmn_ret.response.ShortName) != 'undefined' &&
                plmn_ret.response.ShortName.length > 0) {

                    plmn_name = plmn_ret.response.ShortName;

                    if (G_MonitoringStatus != null &&
                    typeof(G_MonitoringStatus.response) != 'undefined' &&
                    parseInt(G_MonitoringStatus.response.RoamingStatus, 10) == 1) {
                        plmn_name += ' ';
                        plmn_name += IDS_dialup_label_roaming;
                    }
                    $('#index_plmn_name').text(plmn_name);
                }
                else if (typeof(plmn_ret.response.FullName) != 'undefined' &&
                    plmn_ret.response.FullName.length > 0) {
                    plmn_name = plmn_ret.response.FullName;

                    if (G_MonitoringStatus != null &&
                    typeof(G_MonitoringStatus.response) != 'undefined' &&
                    parseInt(G_MonitoringStatus.response.RoamingStatus, 10) == 1) {
                        plmn_name += ' ';
                        plmn_name += IDS_dialup_label_roaming;
                    }

                    $('#index_plmn_name').text(plmn_name);
                }
            }
        }
    });
}

/*
 update current operator name and generation
 */
function index_updatePLMN() {
    /*
     * if no signal, no need to get PLMN
     */

     GetPLMN();

}

function getWiFiStatusAndMyNumber() {
    //$('#ConnectionStatus').text(mynumber);

}


/*
 update signal, new message icon, new update icon repeatly.
 */
function index_updatePageStatusListener() {
    linkStatusFork();
    initConnectionStatus();
    index_updatePLMN();
    setHTMLByWlanBasicSetting();
    getNetworkStatusValue();
}

function setHTMLByWlanBasicSetting() {

    var monitoring_status = G_MonitoringStatus;
    if(null != monitoring_status) {
        if(monitoring_status.response.WifiStatus == 1){
            $('#CurrentWifiStatus').text(common_on);
            $('#CurrentWifiUser').html(monitoring_status.response.CurrentWifiUser + '/' + monitoring_status.response.TotalWifiUser);
            $("#button_turn").show();
            $("#button_turn .button_center").html(common_turn_off);
        }
        else{
            $('#CurrentWifiStatus').text(common_off); 
            $('#CurrentWifiUser').html(wlan_label_no_users);
            $("#button_turn").show();
            $("#button_turn .button_center").html(common_turn_on);
        }
    }
}


function getLoginUitl(login_id, loginTrafficxml) {
    if (loginTrafficxml >= g_monitoring_dumeter_tb) {
        $('#' + login_id).text(formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_tb, 2) + ' TB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_tb && loginTrafficxml >= g_monitoring_dumeter_gb) {
        $('#' + login_id).text(formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_gb, 2) + ' GB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_gb && loginTrafficxml >= g_monitoring_dumeter_mb) {
        $('#' + login_id).text(formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_mb, 2) + ' MB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_mb && loginTrafficxml >= g_monitoring_dumeter_kb) {
        $('#' + login_id).text(formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_kb, 2) + ' KB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_kb) {
        $('#' + login_id).text(formatFloat(parseFloat(loginTrafficxml), 2) + ' B');
    }
}
/* make sure every 6s can refesh the value, acutual value or 0 */
function initConnectionStatus() {
    var g_login_function = {};

    if(((g_conection_device == "1")&&(SERVICE_STATUS_AVAIABLE != G_MonitoringStatus.response.ServiceStatus))
        ||((g_conection_device == "2") && (WIRELINESERVICE_STATUS_AVAIABLE != G_MonitoringStatus.response.x_WireLineLinkStatus)))
    {
        g_login_function.CurrentConnectTime = 0;
        g_login_function.CurrentDownload = 0;
        g_login_function.CurrentUpload = 0;
        $("#wanipv4").html("0.0.0.0");
        $("#wanipv6").html("0::0");

    }else  if (G_MonitoringStatus != null &&
        (G_MonitoringStatus.response.ConnectionStatus == MACRO_CONNECTION_CONNECTED 
        || G_MonitoringStatus.response.X_IPv6ConnectionStatus == MACRO_CONNECTION_IPV6CONNECTED 
        || G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus == MACRO_WIRELINE_CONNECTION_CONNECTED
        || G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus == MACRO_WIRELINE_CONNECTION_IPV6CONNECTED )) {

            getAjaxData('api/monitoring/traffic-statistics', function($xml) {
                var ret = xml2object($xml);
                if (ret.type == 'response') {
                    g_login_function = ret.response;
                }
                else {
                    g_login_function.CurrentConnectTime = 0;
                    g_login_function.CurrentDownload = 0;
                    g_login_function.CurrentUpload = 0;

                    log.error('Load traffic-statistics data failed');
                }
            },{sync :true});
            getAjaxData("api/connection/wanstatus", function($xml) {
                var ret = xml2object($xml);
                if(ret.type == "response"){
                    $("#wanipv4").html(ret.response.ExternalIPAddress);
                    var Macstrings = '';
                    var length = ret.response.X_IPv6Address.length;
                    if( length > 45){
                        Macstrings  = ret.response.X_IPv6Address.substring(0,45);
                        Macstrings += "<br>";
                        Macstrings += ret.response.X_IPv6Address.substring(45, length);
                    }else{
                        Macstrings = ret.response.X_IPv6Address; 
                    }
                    $("#wanipv6").html(Macstrings);

                } else {
                    log.error("Load WAN Status data failed");
                }
            });
     }else{
        g_login_function.CurrentConnectTime = 0;
        g_login_function.CurrentDownload = 0;
        g_login_function.CurrentUpload = 0;
        $("#wanipv4").html("0.0.0.0");
        $("#wanipv6").html("0::0");
    }
    var totalTimesArray = getCurrentTime(g_login_function.CurrentConnectTime);
    getLoginUitl('TotalDownload', g_login_function.CurrentDownload);
    getLoginUitl('TotalUpload', g_login_function.CurrentUpload);
    $('#TotalConnectTime').text(totalTimesArray);

}

getConnectionStautes();
/*
 invoke while document is ready
 */
$(document).ready(function() {
    if (LANGUAGE_DATA.current_language == 'ar_sa') {
        $('#revicedsent').text(common_sent + '/' + dialup_label_received + common_colon);
    }
    else {
        $('#revicedsent').text(dialup_label_received + '/' + common_sent + common_colon);
    }
    
    if($.browser.msie) {
        if((parseInt($.browser.version.charAt(0), 10) === 6 || parseInt($.browser.version.charAt(0), 10) === 7) && G_MonitoringStatus != null && typeof(G_MonitoringStatus) != 'undefined') {
            setTimeout(index_updatePageStatusListener, 1);
        }
    }

    addStatusListener('index_updatePageStatusListener()');
    

    if (g_module.ussd_enabled) {
        $('.index_appicon_ussd').show();
    }

    if (!g_feature.connection.enable) {
        $('.login_box_info').css({
            display: 'none'
        });
    }

    if (!g_feature.connection.connectionstatus) {
        $('.connection').css({
            display: 'none'
        });
    }
    if (g_module.sdcard_enabled) {
        $('#login_sharing_div').show();
        $('#view_SDCard_btn').bind('click', function() {
            window.location.href = 'sdcardsharing.html';
        });
    }

    if (!g_module.wifi_enabled) {
        $('.wlan_status').css({
            display: 'none'
        });
    }    
    setTimeout(getAdvanceinformation, 1000);

    if (typeof(g_feature.hotlinks.enable) != 'undefined') {
        if (g_feature.hotlinks.enable) {
            SetHotlinks();
        }
        else {
            $('.hot_links').hide();
        }
    }

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

    //Button effect when mouseover and out
    $('#connect_btn, #disconnect_btn').live('mouseover', function() {

        if (!isButtonEnable('connect_btn') || !isButtonEnable('disconnect_btn')) {
            return;
        }
        else {
            $(this).addClass('mouse_on');
        }
        g_mouse_on_out_event = 1;
    });
    $('#connect_btn, #disconnect_btn').live('mouseout', function() {
        $(this).removeClass('mouse_on');
        g_mouse_on_out_event = 2;
    });
    $('#connect_btn').live('click', function() {
        if (!isButtonEnable('connect_btn')) {
            return;
        }
        g_is_connect_clicked = true;
        index_clickConnectBtn();
    });
    $('#cancel_btn').live('click', function() {
        if (!isButtonEnable('cancel_btn')) {
            return;
        }
        g_is_disconnect_clicked = false;
        g_is_connect_clicked = false;
        $('#cancel_btn').removeClass('mouse_on');
        button_enable('cancel_btn', '0');
        
        setConnectionLink(dialup_label_connecting);
        index_clickCancelBtn();
    });
    $('#disconnect_btn').live('click', function() {
        if (!isButtonEnable('disconnect_btn')) {
            return;
        }
        g_is_disconnect_clicked = true;
        index_clickDisconnectBtn();
    });
    $("#button_turn").live("click",function(){ 
        g_is_login_opened = true;
        var btn_text = $(this).find(".button_center").html(); 
        if(btn_text == common_turn_off) { 
            wifi_enable = 0; 
        } 
        else { 
            wifi_enable = 1; 
        } 
        var status_set = { 
            'Wifienable': wifi_enable 
        }; 
        var req = object2xml("request",status_set); 
        saveAjaxData("api/wlan/wifienable",req,function($xml){ 
            var ret = xml2object($xml); 
            button_enable('button_turn', '0');
            if(isAjaxReturnOK(ret)){ 
                getAjaxData('api/monitoring/status', function($xml) {
                    log.debug('MAIN : getDeviceStatus() get monitoring success');
                    var ret = xml2object($xml);
                    if (ret.type == 'response') {
                            G_MonitoringStatus = ret;
                            setHTMLByWlanBasicSetting();
                            button_enable('button_turn', '1');
                     } else { 
                        log.error("Save basic request failed"); 
                        button_enable('button_turn', '1');
                    } 
                });
            }else{
                log.error("Save basic request failed"); 
                button_enable('button_turn', '1');
            }
       });
        g_is_login_opened = false;  
    });

})
function main_getAdvertisement() {
    if (g_feature.adcontent_enable) {
        $('#ad_div').show();
        $('#ad_iframe').get(0).src = 'adcontent.html';
    }
}

function setConnectionLink(connectType) {   
    $('#index_connection_status').html(connectType + "<div id='menu_connection_settings' style='display:none'><a href='setting.html' onclick='"+'setHtml()'+"'>" + hilink_label_auto_connection_link + '</a></div>');
    if('admin' == g_login_usertype){
        $('#menu_connection_settings').show();
    }
}


function getConnectionStautes() {
    getAjaxData('api/connection/connection', function($xml) {
        var conn_ret = xml2object($xml);
        if (conn_ret.type == 'response') {
            g_conection_device = conn_ret.response.ConnectionDevice;
        } else {
            log.error('Error, no data');
        }
    }, {
        sync: true
    });
    var wirelineConnectMode;
    var wirelessConnectMode;
    getAjaxData('api/dialup/connection', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type) {
            wirelessConnectMode = ret.response.ConnectMode;
            wirelineConnectMode = ret.response.Wireline_ConnectMode;
            g_wirelessAutoConnect = (DIALUP_MODE_MANUAL == wirelessConnectMode) ? false:true;
            g_wirelineAutoConnect = (DIALUP_MODE_MANUAL == wirelineConnectMode) ? false:true;
        }
    },{
       sync:true
    });
}

function index_updateWirelineConnectionStatus(){
    /*
     when the user clicks connect/disconnect button, UI should stop updating
     connection
     status for just one cycle interval(default 3s).
     */
    if (g_freezeConnectionUpdate) {
        g_freezeConnectionUpdate = false;
        return;
    }
    var currentStatus = G_MonitoringStatus.response;
    var signal_strength = '';
    if (typeof(currentStatus.SignalIcon) != 'undefined' || currentStatus.SignalIcon != null) {
        signal_strength = '0' + currentStatus.SignalIcon;
    }else {
        signal_strength = '0' + parseInt(currentStatus.SignalStrength / 20, 10).toString();
    }

    /*
     * singal strength
     */
     if((MACRO_PIN_REQUIRED == g_main_convergedStatus.SimState) || (MACRO_PUK_REQUIRED == g_main_convergedStatus.SimState)||
        (MACRO_SIM_STATUS_USIM_N == currentStatus.SimStatus) || (MACRO_SIM_STATUS_USIM_PS_CS_N == currentStatus.SimStatus)||
        (MACRO_SIM_STATUS_USIM_NE == currentStatus.SimStatus) || (header_icon_status.SimlockStatus)){
         $('#status_img').css({background: 'none'});
     }else{
        $('#status_img').css({background: 'url(../res/icon_signal_' + signal_strength + '.gif) 0 0 no-repeat'});
    }
    var totalStatus =currentStatus.X_IPv4WireLineConnectionStatus.concat(currentStatus.X_IPv6WireLineConnectionStatus);
    switch (totalStatus) {
        case "11"://connecting brown
        case "13"://connecting brown
        case "14"://connecting brown
        case "15"://connecting brown
        case "31"://connecting brown
        case "41"://connecting brown
        case "51"://connecting brown
         // connecting
            if ($('#connect_btn').size() == 0) {
                $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
                
            }
            setConnectionLink(dialup_label_connecting);
            button_enable('connect_btn', '0');
            $('#connect_btn').removeClass('mouse_on');
            break;

        case "12"://connected light
        case "21"://connected light
        case "22"://connected light
        case "23"://connected light
        case "24"://connected light
        case "25"://connected light
        case "32"://connected light
        case "42"://connected light
        case "52"://connected light
            // connected
            setConnectionLink(dialup_label_connected);
            $('#index_connection_button').html(createConnectionButton(common_disconnect, 'disconnect_btn'));
            break;

        case "33"://disconnecting brown
        case "34"://disconnecting brown
        case "35"://disconnecting brown
        case "43"://disconnecting brown
        case "44"://disconnecting brown
        case "45"://disconnecting brown
        case "53"://disconnecting brown
        case "54"://disconnecting brown
            // disconnecting
            setConnectionLink(dialup_label_disconnecting);
            $('#index_connection_button').html(createConnectionButton(common_disconnect, 'disconnect_btn'));
            button_enable('disconnect_btn', '0');
            break;

        case "55":// disconnected
            if (currentStatus.x_WireLineLinkStatus != WIRELINESERVICE_STATUS_AVAIABLE){
                // no coverage
                setConnectionLink(hilink_label_wireline_connect_failed);
                $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
            }
            else {
                // disconnected
                setConnectionLink(dialup_label_disconnected);
                $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
            }
            break;
        default:
            // Connection failed. Please try again later. If the problem
            // persists, please contact your Service Provider.
            setConnectionLink(hilink_label_connect_failed_common_tip);
            $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
            break;
    }

    // while ServiceStatus is not available, user can not click Connect Button
    if (currentStatus.x_WireLineLinkStatus != WIRELINESERVICE_STATUS_AVAIABLE) {
        button_enable('connect_btn', '0');
    }
}

function index_updateWirelessConnectionStatus(){
    /*
     when the user clicks connect/disconnect button, UI should stop updating
     connection
     status for just one cycle interval(default 3s).
     */
    if (g_freezeConnectionUpdate) {
        g_freezeConnectionUpdate = false;
        return;
    }
    var currentStatus = G_MonitoringStatus.response;
    var signal_strength = '';
    if (typeof(currentStatus.SignalIcon) != 'undefined' || currentStatus.SignalIcon != null) {
        signal_strength = '0' + currentStatus.SignalIcon;
    }else {
        signal_strength = '0' + parseInt(currentStatus.SignalStrength / 20, 10).toString();
    }
    /*
     * singal strength
     */
     if((MACRO_PIN_REQUIRED == g_main_convergedStatus.SimState) || (MACRO_PUK_REQUIRED == g_main_convergedStatus.SimState)||
        (MACRO_SIM_STATUS_USIM_N == currentStatus.SimStatus) || (MACRO_SIM_STATUS_USIM_PS_CS_N == currentStatus.SimStatus)||
        (MACRO_SIM_STATUS_USIM_NE == currentStatus.SimStatus) || (header_icon_status.SimlockStatus)){
         $('#status_img').css({background: 'none'});
     }else{
        $('#status_img').css({background: 'url(../res/icon_signal_' + signal_strength + '.gif) 0 0 no-repeat'});
    }

    // while ServiceStatus is not available, user can not click Connect Button
    if(currentStatus.ServiceStatus != SERVICE_STATUS_AVAIABLE){
        setConnectionLink(hilink_label_connect_failed);
        $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));

        button_enable('connect_btn', '0');
        return ;
    }
    var currnet_network_notice= '';
    if((currentStatus.ServiceStatus == SERVICE_STATUS_AVAIABLE)
        &&(currentStatus.CurrentNetworkType == MACRO_NET_WORK_TYPE_GSM
          ||currentStatus.CurrentNetworkType == MACRO_NET_WORK_TYPE_GPRS
          ||currentStatus.CurrentNetworkType == MACRO_NET_WORK_TYPE_EDGE))
        {
            currnet_network_notice = '. ' + dialup_label_network_weak;
        }
    
    var totalStatus =currentStatus.ConnectionStatus.concat(currentStatus.X_IPv6ConnectionStatus);
    switch (totalStatus) {
        case "9001"://connecting brown
        case "9003"://connecting brown
        case "9004"://connecting brown
        case "9005"://connecting brown
        case "9021"://connecting brown
        case "9031"://connecting brown
            // connecting
            if ($('#connect_btn').size() == 0) {
                $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
                
            }
            setConnectionLink(dialup_label_connecting + currnet_network_notice);
            button_enable('connect_btn', '0');
            $('#connect_btn').removeClass('mouse_on');
            break;
        case "9002"://connected light
        case "9011"://connected light
        case "9012"://connected light
        case "9013"://connected light
        case "9014"://connected light
        case "9015"://connected light
        case "9022"://connected light
        case "9032"://connected light
            // connected
            setConnectionLink(dialup_label_connected + currnet_network_notice);
            $('#index_connection_button').html(createConnectionButton(common_disconnect, 'disconnect_btn'));
            break;
        case "9023":// disconnecting brown
        case "9024":// disconnecting brown
        case "9033":// disconnecting brown
        case "9034":// disconnecting brown
        case "9035":// disconnecting brown
            // disconnecting
            setConnectionLink(dialup_label_disconnecting + currnet_network_notice);
            $('#index_connection_button').html(createConnectionButton(common_disconnect, 'disconnect_btn'));
            button_enable('disconnect_btn', '0');
            break;
        case "9025":// disconnected 
            // disconnected
            setConnectionLink(dialup_label_disconnected + currnet_network_notice);
            $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
            break;
        default:
            // Connection failed. Please try again later. If the problem
            // persists, please contact your Service Provider.
            setConnectionLink(hilink_label_connect_failed_common_tip);
            $('#index_connection_button').html(createConnectionButton(common_connect, 'connect_btn'));
            break;
    }

}

function linkStatusFork()
{
    if(g_conection_device == "2")
    {
        g_isAutoConnect = g_wirelineAutoConnect;
        index_updateWirelineConnectionStatus();
    }else{
        g_isAutoConnect = g_wirelessAutoConnect;
        index_updateWirelessConnectionStatus();
    }
}

function getNetworkStatusValue()
{
    var network_status = G_MonitoringStatus.response.CurrentNetworkType;
    switch(network_status){
        case MACRO_NET_WORK_TYPE_GSM:
            network_type_value = dialup_label_gsm;
            break;
        case MACRO_NET_WORK_TYPE_GPRS:
            network_type_value = dialup_label_gprs;
            break;
        case MACRO_NET_WORK_TYPE_EDGE:
            network_type_value = dialup_label_edge;
            break;    
        case MACRO_NET_WORK_TYPE_WCDMA:
            network_type_value = dialup_label_wcdma;
            break;
        case MACRO_NET_WORK_TYPE_HSDPA:
            network_type_value = dialup_label_hsdpa;
            break;
        case MACRO_NET_WORK_TYPE_HSUPA:
            network_type_value = dialup_label_hsupa;
            break;
        case MACRO_NET_WORK_TYPE_HSPA:
            network_type_value = dialup_label_hspa;
            break;
        case MACRO_NET_WORK_TYPE_HSPA_PLUS:
            network_type_value = dialup_label_hspa_plus;
            break;
        case MACRO_NET_WORK_TYPE_DC_HSPA:
            network_type_value = dialup_label_DC_HSPA;
            break;
        case MACRO_NET_WORK_TYPE_LTE:
            network_type_value = dialup_label_lte;
            break;
        default:
            network_type_value = dialup_label_no_service;
            break;
        }
    $("#wan_networktype").html(network_type_value);
      
}

function getAdvanceinformation()
{
    getAjaxData("api/dhcp/settings",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            $("#lanipv4").html(ret.response.DhcpIPAddress);
        }
        else {
            log.error("Load dhcp data failed");
        }
    });

    getAjaxData("api/settings/hostinfo",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            var Macstrings = '';
            var length = ret.response.HostIPv6Address.length;
            if( length > 45){
                Macstrings  = ret.response.HostIPv6Address.substring(0,45);
                Macstrings += "<br>";
                Macstrings += ret.response.HostIPv6Address.substring(45, length);
            }else{
                Macstrings = ret.response.HostIPv6Address; 
            }
            $("#lanipv6").html(Macstrings);
        }
        else {
            log.error("Load ipv6 data failed");
        }
    });

}
