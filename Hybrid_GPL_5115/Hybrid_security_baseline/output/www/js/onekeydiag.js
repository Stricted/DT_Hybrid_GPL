// JavaScript Document
//
var AP_STATION_CONNECTED = 2;
var MACRO_EVDO_SIGNAL_DBM = 'dBm';
var g_info = {
SimStatus:null,
serviceStates : null,
SignalStrength:null,
wirelessStatus :null,
connmode:null,
connectStates :null,
wifi:null,
currentUser:null
};

function getNetworkStatus(status)
{
    switch (status.SignalIcon) {
        case MACRO_EVDO_LEVEL_ONE:
        case MACRO_EVDO_LEVEL_TWO:
            g_info.SignalStrength = common_signal_weak;
            break;
        case MACRO_EVDO_LEVEL_THREE:
        case MACRO_EVDO_LEVEL_FOUR:
        case MACRO_EVDO_LEVEL_FIVE:
            g_info.SignalStrength = common_signal_strong;
            break;
        default:
            g_info.SignalStrength = common_sig_off;
            break;
    }
    
    switch (status.x_WireLineLinkStatus) {
        case '0':
            g_info.wirelessStatus = label_inactive;
            break;
        case '1':
            g_info.wirelessStatus = label_active;
            break;
        default:
            break;
    }
    if(status.ServiceStatus != "2"){
        g_info.serviceStates = dialup_label_no_service;
    }else{
        switch(status.CurrentNetworkType){
            case MACRO_NET_WORK_TYPE_GSM:
                g_info.serviceStates = dialup_label_gsm;
                break;
            case MACRO_NET_WORK_TYPE_GPRS:
                g_info.serviceStates = dialup_label_gprs;
                break;
            case MACRO_NET_WORK_TYPE_EDGE:
                g_info.serviceStates = dialup_label_edge;
                break;    
            case MACRO_NET_WORK_TYPE_WCDMA:
                g_info.serviceStates = dialup_label_wcdma;
                break;
            case MACRO_NET_WORK_TYPE_HSDPA:
                g_info.serviceStates = dialup_label_hsdpa;
                break;
            case MACRO_NET_WORK_TYPE_HSUPA:
                g_info.serviceStates = dialup_label_hsupa;
                break;
            case MACRO_NET_WORK_TYPE_HSPA:
                g_info.serviceStates = dialup_label_hspa;
                break;
            case MACRO_NET_WORK_TYPE_TDSCDMA:
                g_info.serviceStates = "TD-SCDMA";
                break;
            case MACRO_NET_WORK_TYPE_HSPA_PLUS:
                g_info.serviceStates = dialup_label_hspa_plus;
                break;
            case MACRO_NET_WORK_TYPE_DC_HSPA:
                g_info.serviceStates = dialup_label_DC_HSPA;
                break;
            case MACRO_NET_WORK_TYPE_LTE:
                g_info.serviceStates = dialup_label_lte;
                break;
            case DSAT_SYS_SUBMODE_TDS:
                g_info.serviceStates = "TD-SCDMA";
                break;
            case DSAT_SYS_SUBMODE_TDS_HSDPA:
                g_info.serviceStates = "TDS-HSDPA";
                break;
            case DSAT_SYS_SUBMODE_TDS_HSUPA:
                g_info.serviceStates = "TDS-HSUPA";
                break;
            case DSAT_SYS_SUBMODE_TDS_HSPA:
                g_info.serviceStates = "TDS-HSPA";
                break;
            default:
                g_info.serviceStates = dialup_label_no_service;
                break;
            }
    }
    switch (status.SimStatus) {
        case '0':
            if(status.simlockStatus == "1"){
                g_info.SimStatus = dialup_hint_simlock_active;
            } else {
                getAjaxData("api/pin/status", function($xml) {
                    var pinstatus_ret = xml2object($xml);
                    if ("response" == pinstatus_ret.type) {
                        switch (pinstatus_ret.response.SimState){
                        case MACRO_PIN_REQUIRED:
                            g_info.SimStatus = dialup_label_pin_code_required;
                            break;
                        case MACRO_PUK_REQUIRED:
                            g_info.SimStatus = dialup_label_puk_code_required;
                            break;
                        case MACRO_PIN_READY:
                            g_info.SimStatus = dialup_label_sim_valid;
                            break;
                        case MACRO_CARD_LOCK_PIN_RIGNT:
                            g_info.SimStatus = label_hint_pinlock_active;
                            break;
                        case MACRO_CARD_LOCK_PIN_WRONG:
                            g_info.SimStatus = label_hint_pinlock_active;
                            break;
                        default:
                            g_info.SimStatus = dialup_label_invalid_card;
                            break;
                        }
                    } else {
                        g_info.SimStatus = dialup_label_invalid_card;
                    }
                },{sync: true});
            }
            break;
        case '1':
            g_info.SimStatus = dialup_label_sim_valid;
            break;
        case '2':
            g_info.SimStatus = dialup_label_sim_invalid +" CS";
            break;
        case '3':
            g_info.SimStatus = dialup_label_sim_invalid +" PS";
            break;
        case '4':
            g_info.SimStatus = dialup_label_sim_invalid +" CS,PS";
            break;
        case '255':
            g_info.SimStatus = dialup_label_invalid_card;
        default:
            break;
    }
    $('#search_network').hide();
    $('#network').show();
    $('#sim_card').html(g_info.SimStatus);
    $('#service_states').html(g_info.serviceStates);
    $('#signal_dbm').html(g_info.SignalStrength);
    $('#wireline_state').html(g_info.wirelessStatus);

}

function getWifiStatusValue(Status)
{
    if(Status.WifiStatus == 1){
        g_info.wifi = common_on; 
        g_info.currentUser = Status.CurrentWifiUser + '/' + Status.TotalWifiUser; 

        if(Status.CurrentWifiUser > 0){
            getAjaxData("api/connection/wlanstatus", function($xml) {
                var ret = xml2object($xml);
                var wlanHosts = [];
                var new_line = '';
                if((ret.type == "response") && (typeof(ret.response.Hosts) != 'undefined') && (ret.response.Hosts.Host))
                {
                    if($.isArray(ret.response.Hosts.Host)){
                        wlanHosts = ret.response.Hosts.Host;
                    }else{
                        wlanHosts.push(ret.response.Hosts.Host);
                    }
                    $("#table_lanlist > tbody > tr:gt(0)").remove();
                    $.each(wlanHosts, function(i) {
                        wlanHosts[i].SSID =  'WIFI_'+ (i + 1) + '_' + wlanHosts[i].SSID ;
                        new_line += "<tr><td>" + wlanHosts[i].SSID + "</td>";
                        new_line += "<td>" + wlanHosts[i].AssociatedDeviceMACAddress + "</td></tr>";
                    });
                    $("#table_lanlist").append(new_line);
                } else {
                    log.error("Load WLAN Status data failed");
                }
            },{ sync: true });
            $('#table_lanlist').show();
        }

    }
    else{
        g_info.wifi = common_off; 
        g_info.currentUser = wlan_label_no_users; 
    }
    $('#search_wifi').hide();
    $('#wifi').show();
    $('#wifi_states').html(g_info.wifi);
    $('#current_user').html(g_info.currentUser);

}
function getConnectionStates(Status){
    var connectmode = null;
    var connect_mode = null;
    var connectstate = null;
    var iptype = null;
    getAjaxData('api/connection/connection', function($xml) {
        var conn_ret = xml2object($xml);
        if (conn_ret.type == 'response') {
            connectmode = conn_ret.response.ConnectionDevice;
            if(connectmode == "1"){
                connect_mode = label_set_quick_connection_wireless;
            }else if (connectmode == "2"){
                connect_mode = label_set_quick_connection_wireline;
            }
        } else {
            log.error('Error, no data');
        }
    }, {
        sync: true
    });

    if(("2" == Status.ServiceStatus) && (connectmode == "1")){
        if((MACRO_CONNECTION_IPV6CONNECTED == Status.X_IPv6ConnectionStatus)||
        (MACRO_CONNECTION_CONNECTED == Status.ConnectionStatus)){
            connectstate = dialup_label_connected;
        }else{
            connectstate = dialup_label_disconnected;
        }
    }else if((connectmode == "2")&&((MACRO_WIRELINE_CONNECTION_CONNECTED == Status.X_IPv4WireLineConnectionStatus) ||
            (MACRO_WIRELINE_CONNECTION_IPV6CONNECTED == Status.X_IPv6WireLineConnectionStatus))){
            connectstate = dialup_label_connected;
    } else {
        connectstate = dialup_label_disconnected;
    }
    
    $('#search_connection').hide();
    $('#connection').show();
    $('#connection_mode').html(connect_mode);
    
    $('#connect_states').html(connectstate);
}
function getDeviceInfo() {
    $('#network').hide();
    $('#connection').hide();
    $('#wifi,#table_lanlist').hide();

    if(("checked" != $('#network_check').attr('checked'))
     &&("checked" != $('#connection_check').attr('checked'))
     &&("checked" != $('#wifi_check').attr('checked'))){
       return ;
    }
    if("checked" == $('#network_check').attr('checked')){
        $('#search_network').show();
    }
    if("checked" == $('#connection_check').attr('checked')){
        $('#search_connection').show();
    }
    if("checked" == $('#wifi_check').attr('checked')){
        $('#search_wifi').show();
    }
    getAjaxData('api/monitoring/status', function($xml) {
    var ret = xml2object($xml);
        if ('response' == ret.type) {
            if("checked" == $('#network_check').attr('checked')){
                getNetworkStatus(ret.response);
            }
            if("checked" == $('#connection_check').attr('checked')){
                getConnectionStates(ret.response);
            }
            if("checked" == $('#wifi_check').attr('checked')){
                getWifiStatusValue(ret.response);
            }
        } else {
            $('#network').hide();
            $('#connection').hide();
            $('#wifi;#table_lanlist').hide();
            parent.showInfoDialog(common_failed);
        }
    },{
        sync: true
    });

}





$(document).ready(function() {
    button_enable('button_apply', '0');
    $('#network_check,#connection_check,#wifi_check').bind("click",function(){
        button_enable('button_apply', '1');
    });
    $('#button_apply').click(function(){
        if (isButtonEnable('button_apply')){
            button_enable('button_apply', '0');
            getDeviceInfo();
        }
    });

});
