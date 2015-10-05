var g_connection_info = {};
var g_profile_list_html='';
var g_eth_profile_list_html='';
var g_umts_profile;
var g_eth_profile;
var default_umts_index = 0;
var default_eth_index = 0;
var g_freezeConnectionUpdate = false; 
var g_isAutoConnect = true;
var g_isUMTSAutoConnect = true;
var g_isETHAutoConnect = true;
var g_dialup_action = {
    Action: '1'
};
var g_timeout;
var g_note_change_wan = 0;

var SETTING_PATH = "setting.html";
var SIGLE_RADIO="1";
var SIGLE_WAN="2";
var RADIO_PRI="3";
var CONN_IPV4='ipv4';
var CONN_IPV6='ipv6';
var CONN_MIX='ipv4ipv6';
var MACRO_CONNECTION_CONNECTING = '900';
var MACRO_CONNECTION_CONNECTED = '901';
var MACRO_CONNECTION_DISCONNECTED = '902';
var MACRO_CONNECTION_IPV6CONNECTING = '1';
var MACRO_CONNECTION_IPV6CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_CONNECTING = '1';
var MACRO_WIRELINE_CONNECTION_CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_IPV6CONNECTING = '1';
var MACRO_WIRELINE_CONNECTION_IPV6CONNECTED = '2';
var SERVICE_STATUS_AVAIABLE = 2;
var WIRELINESERVICE_STATUS_AVAIABLE = "1";
var SERVICE_STATUS_AVAIABLE = 2;

var g_monitoring_dumeter_kb = 1024;
var g_monitoring_dumeter_mb = 1024 * 1024;
var g_monitoring_dumeter_gb = 1024 * 1024 * 1024;
var g_monitoring_dumeter_tb = 1024 * 1024 * 1024 * 1024;


var g_WirelineDisplay = true;

function parseflow(loginTrafficxml) {
    if (loginTrafficxml >= g_monitoring_dumeter_tb) {
        return  (parent.formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_tb, 2) + ' TB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_tb && loginTrafficxml >= g_monitoring_dumeter_gb) {
        return (parent.formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_gb, 2) + ' GB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_gb && loginTrafficxml >= g_monitoring_dumeter_mb) {
        return (parent.formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_mb, 2) + ' MB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_mb && loginTrafficxml >= g_monitoring_dumeter_kb) {
        return (parent.formatFloat(parseFloat(loginTrafficxml) / g_monitoring_dumeter_kb, 2) + ' KB');
    }
    else if (loginTrafficxml < g_monitoring_dumeter_kb) {
        return (parent.formatFloat(parseFloat(loginTrafficxml), 2) + ' B');
    }
}

function setcookie(id){
    $.cookies.set('cookie', "setting,dialup,"+id, { expiresAt: new Date().getTime()+5*60*1000 });
}
function initConnstatus()
{
    if(typeof g_timeout != "undefined"){
        clearTimeout(g_timeout);
    }
    setTimeout(linkStatusFork,1500);
    g_timeout = setTimeout(initConnstatus,6000);
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

function createConnectionButton(button_dis, button_id) {
    $('#conn_btn').html("");
    if (!g_isAutoConnect) {
        $('#conn_btn').html(create_button_html(button_dis, button_id));
    }  
}

function getConnInfo() {
     getAjaxData('api/connection/connection', function($xml) {
        var conn_ret = xml2object($xml);
        if (conn_ret.type == 'response') {
            g_connection_info = conn_ret.response;
        } else {
            log.error('Error, no data');
        }
    }, {
        sync: true
    });    
    getAjaxData('api/dialup/profiles', function($xml) {
        var conn_profile = xml2object($xml);
        g_profile_list_html = '';
        if (conn_profile.type == 'response') {
            if(typeof(conn_profile.response.Profiles)!="undefined" && conn_profile.response.Profiles.Profile!='' && typeof(conn_profile.response.Profiles.Profile)!="undefined")
            {
                var temp = $(conn_profile.response.Profiles.Profile); 
                g_umts_profile = conn_profile.response.Profiles.Profile;
                default_umts_index = conn_profile.response.CurrentProfile;
                var i;
                for(i = 0;temp.length >0 && i< temp.length; i++)
               {
                    var defaultName = resolveXMLEntityReference(temp[i].Name);
                    while (defaultName.indexOf(' ') >= 0) {
                            defaultName = defaultName.replace(' ', '&nbsp;');
                    }
                    if(conn_profile.response.CurrentProfile == temp[i].Index)
                    {
                        g_profile_list_html += '<option selected="selected" value="'+temp[i].Index+'">'+defaultName+common_default+'</option>';
                        g_isUMTSAutoConnect = temp[i].ConnectMode == 'Manual' ? false:true;
                    }
                    else
                    {
                        g_profile_list_html += '<option value="'+temp[i].Index+'">'+defaultName+'</option>';
                    }
                }
            }
        }
        else 
        {
            log.error('Error, no data');
        }
        if(g_profile_list_html == ''){
            g_isUMTSAutoConnect = true;
        }
        $('#conn_profiles option').remove();
        $('#conn_profiles').append(g_profile_list_html);
    },
    {
        sync: true
    });

    getAjaxData('api/dialup/ETHs', function($xml) {
        var conn_profile = xml2object($xml);
        g_eth_profile_list_html = '';
        if (conn_profile.type == 'response') {
            if(typeof(conn_profile.response.ETHs)!="undefined" && conn_profile.response.ETHs.ETH!='' && typeof(conn_profile.response.ETHs.ETH)!="undefined")
            {
                var temp = $(conn_profile.response.ETHs.ETH);
                g_eth_profile = conn_profile.response.ETHs.ETH;
                default_eth_index = conn_profile.response.CurrentETH;
                var i;
                for(i = 0; temp.length > 0 && i < temp.length; i++)
                {
                    var ethName = resolveXMLEntityReference(temp[i].Name);
                    while (ethName.indexOf(' ') >= 0) {
                        ethName = ethName.replace(' ', '&nbsp;');
                    }
                    if(conn_profile.response.CurrentETH == temp[i].Index)
                    {
                        g_eth_profile_list_html += '<option selected="selected" value="'+temp[i].Index+'">'+ethName+common_default+'</option>';
                        g_isETHAutoConnect = (temp[i].ConnectionType == "0" && temp[i].ConnectMode == 'Manual') ? false :true;
                    }
                    else
                    {
                        g_eth_profile_list_html += '<option value="'+temp[i].Index+'">'+ethName+'</option>';
                    }
                }   
            }
        }
        else
        {
            log.error('Error, no data');
        }
        if(g_eth_profile_list_html == ''){
            g_isETHAutoConnect = false;
        }
        $('#conn_eth_profiles option').remove();
        $('#conn_eth_profiles').append(g_eth_profile_list_html);
    }, 
    {
        sync: true
    });
}


function setProDisplay(ConnectionDevice)
{
    //$('.connectionstatus_table').css('display','');
    switch(ConnectionDevice)
    {    
        case SIGLE_WAN:
            $("#conn_device")[0].selectedIndex=1;
            $('#conn_eth').css('display','');
            $('#conn_radio').css('display','none');
            if(g_eth_profile_list_html=='')
            {
                parent.showConfirmDialog(connectiontatus_label_eth_noprofile, function(){
                    setcookie('wan');
                    gotoPageWithoutHistory(SETTING_PATH);
                }, function(){  button_enable('app_btn', '0');});
            }
            break;
        case RADIO_PRI:
            $("#conn_device")[0].selectedIndex=2;
            $('#conn_eth').css('display','');
            $('#conn_radio').css('display','');
            if(g_eth_profile_list_html==''&&g_profile_list_html=='')
            {
                parent.showConfirmDialog(connectiontatus_label_eth_noprofile, function(){ 
                    setcookie('profilesmgr');
                    gotoPageWithoutHistory(SETTING_PATH); 
                }, function(){  button_enable('app_btn', '0');});
            }
            break;
        default:
            $("#conn_device")[0].selectedIndex=0;
            $('#conn_eth').css('display','none');
            $('#conn_radio').css('display','');
            if(g_profile_list_html=='')
            {
                parent.showConfirmDialog(connectiontatus_label_eth_noprofile, function(){ 
                    setcookie('profilesmgr');
                    gotoPageWithoutHistory(SETTING_PATH);
                }, function(){  button_enable('app_btn', '0');});
            }
            break;        
    }
}

function get_WirelineDisplay_flag()
{  
    getConfigData('config/global/config.xml', function($xml) {
        var ret = _xml2feature($xml);       
        if(typeof(ret.wirelinewan_enable) != 'undefined')
        {
            g_WirelineDisplay = ret.wirelinewan_enable == 1? true:false;
        }   
    }, {
            sync: true
    });
}

function initPage()
{
    getConnInfo();
    setProDisplay(g_connection_info.ConnectionDevice);
    initConnstatus();
    //displayUserInfo();
    button_enable('app_btn', '0');
}

function postdata()
{
    var requestdata ={
       ConnectionDevice:$("#conn_device").val(),
       setDefaultProfile: $("#conn_device").val() != SIGLE_WAN ? $("#conn_profiles").val() : 0,
       setDefaultETH:$("#conn_device").val() != SIGLE_RADIO ? $("#conn_eth_profiles").val():0
    };
    parent.showWaitingDialog(common_waiting,common_waiting);
    var req =object2xml('request', requestdata);
    saveAjaxData('api/connection/connection', req, function($xml){
        parent.closeWaitingDialog();
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret))
        {
            parent.showInfoDialog(common_success);
        }else{
            parent.showInfoDialog(common_failed);
        }
        initPage();
       });
}

function displayWirelessStatus()
{
   switch(parent.G_MonitoringStatus.response.ConnectionStatus){
        case MACRO_CONNECTION_CONNECTED:
            $('#connection_status').text(dialup_label_connected);
            break;
        case MACRO_CONNECTION_CONNECTING:
            $('#connection_status').text(dialup_label_connecting);
            break;
        default:
            $('#connection_status').text(dialup_label_disconnected);
            break;
    }

    switch(parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus){
        case MACRO_CONNECTION_IPV6CONNECTED:
            $('#connection_ipv6_status').text(dialup_label_connected);
            break;
        case MACRO_CONNECTION_IPV6CONNECTING:
            $('#connection_ipv6_status').text(dialup_label_connecting);
            break;
        default:
            $('#connection_ipv6_status').text(dialup_label_disconnected);
            break;
    }
}

function displayWirelineStatus()
{
   switch(parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus){
        case MACRO_WIRELINE_CONNECTION_CONNECTED:
            $('#connection_status').text(dialup_label_connected);
            break;
        case MACRO_WIRELINE_CONNECTION_CONNECTING:
            $('#connection_status').text(dialup_label_connecting);
            break;
        default:
            $('#connection_status').text(dialup_label_disconnected);
            break;
    }

    switch(parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus){
        case MACRO_WIRELINE_CONNECTION_IPV6CONNECTED:
            $('#connection_ipv6_status').text(dialup_label_connected);
            break;
        case MACRO_WIRELINE_CONNECTION_IPV6CONNECTING:
            $('#connection_ipv6_status').text(dialup_label_connecting);
            break;
        default:
            $('#connection_ipv6_status').text(dialup_label_disconnected);
            break;
    }
}

function displayFlow()
{
    if (g_module.ap_station_enabled &&
    parent.G_StationStatus != null &&
    WIFI_STATION_CONNECTED == parent.G_StationStatus.response.ConnectStatus) {
        $('#TotalDownload').text(parseflow(parent.G_StationStatus.response.RxFlux));
        $('#TotalUpload').text(parseflow(parent.G_StationStatus.response.TxFlux));
        $('#TotalConnectTime').text(getCurrentTime(parent.G_StationStatus.response.CurrentTime));
    }
    else {
        getAjaxData('api/monitoring/traffic-statistics', function($xml) {
            var ret = xml2object($xml);
            if (ret.type == 'response') {
                var res = ret.response;
                if (parent.G_MonitoringStatus == null || 
                    (parent.G_MonitoringStatus.response.ConnectionStatus != MACRO_CONNECTION_CONNECTED 
                    && parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus != MACRO_CONNECTION_IPV6CONNECTED 
                    && parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus != MACRO_WIRELINE_CONNECTION_CONNECTED
                    && parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus != MACRO_WIRELINE_CONNECTION_IPV6CONNECTED )) {
                    res.CurrentConnectTime = 0;
                    res.CurrentDownload = 0;
                    res.CurrentUpload = 0;
                }
                $('#TotalDownload').text(parseflow(res.CurrentDownload));
                $('#TotalUpload').text(parseflow(res.CurrentUpload));
                $('#TotalConnectTime').text(getCurrentTime(res.CurrentConnectTime));
            }
            else {
                log.error('Load traffic-statistics data failed');
            }
        });
    }
}

function displayUserInfo(){
    if(g_login_usertype == 'admin')
    {
        $('.connstatus_admin_table').css('display','');
    }
    if (!g_feature_ipv6) {
        $('#conn_ipv6_status').hide();
        $('#conn_ipv6_tr').hide();
    } else {
        $('#conn_ipv6_status').show();
    }

    $('#X_IPv6ConnectionStatus').css('display','');
    $('#ConnectionStatus').css('display','');
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
    var currentStatus = parent.G_MonitoringStatus.response;
    // while ServiceStatus is not available, user can not click Connect Button
    if (currentStatus.x_WireLineLinkStatus != WIRELINESERVICE_STATUS_AVAIABLE) {
        createConnectionButton(common_connect, 'connect_btn');
        button_enable('connect_btn', '0');
        return;
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
                createConnectionButton(common_connect, 'connect_btn');     
            }
            button_enable('connect_btn', '0');
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
            createConnectionButton(common_disconnect, 'disconnect_btn');
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
            createConnectionButton(common_disconnect, 'disconnect_btn');
            button_enable('disconnect_btn', '0');
            break;

        case "55":// disconnected
            createConnectionButton(common_connect, 'connect_btn');
            break;
        default:
            // Connection failed. Please try again later. If the problem
            // persists, please contact your Service Provider.
            createConnectionButton(common_connect, 'connect_btn');
            break;
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
    var currentStatus = parent.G_MonitoringStatus.response;
    
    // while ServiceStatus is not available, user can not click Connect Button
    if (currentStatus.ServiceStatus != SERVICE_STATUS_AVAIABLE) {
        createConnectionButton(common_connect, 'connect_btn');
        button_enable('connect_btn', '0');
        return;
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
                createConnectionButton(common_connect, 'connect_btn');     
            }
            button_enable('connect_btn', '0');
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
            createConnectionButton(common_disconnect, 'disconnect_btn');
            break;
        case "9023":// disconnecting brown
        case "9024":// disconnecting brown
        case "9033":// disconnecting brown
        case "9034":// disconnecting brown
        case "9035":// disconnecting brown
            // disconnecting
            createConnectionButton(common_disconnect, 'disconnect_btn');
            button_enable('disconnect_btn', '0');
            break;
        case "9025":// disconnected 
            createConnectionButton(common_connect, 'connect_btn');
            break;
        default:
            // Connection failed. Please try again later. If the problem
            // persists, please contact your Service Provider.
            createConnectionButton(common_connect, 'connect_btn');
            break;
    }
}

function linkStatusFork()
{
    //displayFlow();
    if(g_connection_info.ConnectionDevice == SIGLE_WAN)
    {
        g_isAutoConnect = g_isETHAutoConnect;
        displayWirelineStatus();
        index_updateWirelineConnectionStatus();    
    }else{
        g_isAutoConnect = g_isUMTSAutoConnect;
        displayWirelessStatus();
        index_updateWirelessConnectionStatus();
    }
    
}


$(document).ready(function() {
    initPage();

    $('#connectiondevice,#conn_profiles,#conn_eth_profiles').bind('change',function(){
        g_note_change_wan = 0;
        if($("#conn_device").val() != g_connection_info.ConnectionDevice){
            button_enable('app_btn', '1');
            g_note_change_wan = 1;
        } else if(($("#conn_device").val() == SIGLE_WAN) && (default_eth_index != $("#conn_eth_profiles").val())){
            button_enable('app_btn', '1');
            g_note_change_wan = 1;
        } else if (($("#conn_device").val() == SIGLE_RADIO) && (default_umts_index != $("#conn_profiles").val())){
            button_enable('app_btn', '1');
        } else {
            button_enable('app_btn', '0');
        }
    });
    $("#conn_device").bind('change', function(){
        setProDisplay($("#conn_device").val());
    });
    $('#app_btn').html(create_button_html(common_apply, "apply_button"));
    button_enable('app_btn', '0');
    $('#app_btn').click(function(){
        if(!isButtonEnable('app_btn'))
        {
            return;
        }
        if(($("#conn_profiles").val()==''&&($('#conn_device').val() == SIGLE_RADIO ||$('#conn_device').val() == SIGLE_WAN))||
            ($("#conn_profiles").val()==''&&$("#conn_eth_profiles").val()&&$('#conn_device').val()==RADIO_PRI))
        {
            parent.showInfoDialog(connectiontatus_label_noprofile);
            return;
        }

        if((("2" == parent.G_MonitoringStatus.response.ServiceStatus) && 
            ((MACRO_CONNECTION_CONNECTED == parent.G_MonitoringStatus.response.ConnectionStatus) ||(MACRO_CONNECTION_IPV6CONNECTED == parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus))) ||
            (MACRO_WIRELINE_CONNECTION_CONNECTED == parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus) ||
            (MACRO_WIRELINE_CONNECTION_IPV6CONNECTED == parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus)){
                  if(g_note_change_wan == 1){
                      parent.showConfirmDialog(lable_disconnect_network + " " + lable_wanchange_note + " " + lable_continue_anyway, postdata,function(){ return; });
                  } else {
                      parent.showConfirmDialog(lable_disconnect_network + " " + lable_continue_anyway, postdata,function(){ return; });
                  }
            }else{
                postdata();
            }
        });
    $("#connect_btn").live("click",function(){
        if(isButtonEnable("connect_btn"))
        {
             g_dialup_action.Action = "1";
            if(!g_freezeConnectionUpdate)
            {
                index_sendConnectionAction();
                button_enable(this.id, "0");
            }
        }
        });
    $("#disconnect_btn").live("click",function(){
        if(isButtonEnable("disconnect_btn"))
        {
             g_dialup_action.Action = "0";
            if(!g_freezeConnectionUpdate)
            {
                index_sendConnectionAction();
                button_enable(this.id, "0");
            }
        }
        });
    get_WirelineDisplay_flag();
    if(false==g_WirelineDisplay)
    {
        $('#connectiondevice').hide();
        $("#conn_device").val(1);
    }
});
