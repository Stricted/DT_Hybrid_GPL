var network_type_value = null;
var service_status_value = null;
var service_status_valid = 2;
var g_connection_info;
function getServicestatusvalue(service_status){
    switch(service_status){
        case '0':
            service_status_value = common_no_service;
            return;
        case '1':
            service_status_value = system_label_circuit_switched_capable;
            return;
        case '2':
            service_status_value = system_label_packet_switch_capable;
            return;
        case '3':
            service_status_value = system_label_circuit_packet_switched_capable;
            return;
        case '4':
            service_status_value = common_searching;
            return;
        default:
            service_status_value = common_unknown;
            return;
    }
}
//此入参为系统子模式
function getNetworkStatusValue(network_status)
{
    switch(network_status){
        case MACRO_NET_WORK_TYPE_GSM:
            network_type_value = dialup_label_gsm;
            return;
        case MACRO_NET_WORK_TYPE_GPRS:
            network_type_value = dialup_label_gprs;
            return;
        case MACRO_NET_WORK_TYPE_EDGE:
            network_type_value = dialup_label_edge;
            return;    
        case MACRO_NET_WORK_TYPE_WCDMA:
            network_type_value = dialup_label_wcdma;
            return;
        case MACRO_NET_WORK_TYPE_HSDPA:
            network_type_value = dialup_label_hsdpa;
            return;
        case MACRO_NET_WORK_TYPE_HSUPA:
            network_type_value = dialup_label_hsupa;
            return;
        case MACRO_NET_WORK_TYPE_HSPA:
            network_type_value = dialup_label_hspa;
            return;
        case MACRO_NET_WORK_TYPE_TDSCDMA:
            network_type_value = "TD-SCDMA";
            return;
        case MACRO_NET_WORK_TYPE_HSPA_PLUS:
            network_type_value = dialup_label_hspa_plus;
            return;
        case MACRO_NET_WORK_TYPE_DC_HSPA:
            network_type_value = dialup_label_DC_HSPA;
            return;
        case MACRO_NET_WORK_TYPE_LTE:
            network_type_value = dialup_label_lte;
            return;
        /* TDS子模式 */
        case DSAT_SYS_SUBMODE_TDS:
            network_type_value = "TD-SCDMA";
            return;
        case DSAT_SYS_SUBMODE_TDS_HSDPA:
            network_type_value = "TDS-HSDPA";
            return;
        case DSAT_SYS_SUBMODE_TDS_HSUPA:
            network_type_value = "TDS-HSUPA";
            return;
        case DSAT_SYS_SUBMODE_TDS_HSPA:
            network_type_value = "TDS-HSPA";
            return;
        default:
            network_type_value = dialup_label_no_service;
            return;
        }
}
function getAllInfo() {

    var monitor_status = parent.G_MonitoringStatus;

    if((("2" == monitor_status.response.ServiceStatus) 
        &&(monitor_status.response.ConnectionStatus == "901" 
        || monitor_status.response.X_IPv6ConnectionStatus == "2" ))
        || (monitor_status.response.X_IPv4WireLineConnectionStatus == "2"
        || monitor_status.response.X_IPv6WireLineConnectionStatus == "2" ))
      {
            getAjaxData("api/connection/wanstatus", function($xml) {
                var ret = xml2object($xml);
                if(ret.type == "response"){
                    $("#wan_ipv4_ipaddress").html(ret.response.ExternalIPAddress);
                    $("#wan_ipv4_primarydnsserver").html(ret.response.DNSServers.split(",")[0]);
                    $("#wan_ipv4_secondarydnsserver").html(ret.response.DNSServers.split(",")[1]);
                    $("#wan_ipv6_ipaddress").html(ret.response.X_IPv6Address);
                    $("#wan_defaultgateway").html(ret.response.X_IPv6DefaultGateway);
                    $("#wan_ipv6_primarydnsserver").html(ret.response.X_IPv6DNSServers.split(",")[0]);
                    $("#wan_ipv6_secondarydnsserver").html(ret.response.X_IPv6DNSServers.split(",")[1]);
                } else {
                    log.error("Load WAN Status data failed");
                }
            });
     }
     getAjaxData('api/connection/connection', function($xml) {
        var conn_ret = xml2object($xml);
        if (conn_ret.type == 'response') {
            g_connection_info = conn_ret.response;
        } else {
            log.error('Error, no data');
        }
        if(typeof g_connection_info !="undefined" && g_connection_info.ConnectionDevice == "2")
        {
            $("#wireless_status").hide();
        }else{
            $("#wireless_status").show();
        }
    }, {
        sync: true
    });    
}

function getWanStatusInfo()
{
    monitor_status = parent.G_MonitoringStatus;
    if(monitor_status)
        {
          if(service_status_valid==monitor_status.response.ServiceStatus){
                getServicestatusvalue(monitor_status.response.CurrentServiceDomain);
                getNetworkStatusValue(monitor_status.response.CurrentNetworkType);
                $("#wan_networktype").html(network_type_value);
                $("#wan_servicestatus").html(service_status_value);
            }
          else{
                $("#wan_networktype").html(dialup_label_no_service);
                $("#wan_servicestatus").html(common_unknown);
            }
        }
    else
        {
            log.error("Load Monitoring Status data failed");
        }
}

$(document).ready( function() {
    if (!parent.g_feature_ipv6) {
        $("#wanstatus_ipv6_title").hide();
        $("#wanstatus_ipv6_table").hide();
    } else {
        $("#wanstatus_ipv6_title").show();
        $("#wanstatus_ipv6_table").show();
    }
    getAllInfo();
    getWanStatusInfo();
});