var wlanlistHosts = [];

function getAllInfo() {
    //get macaddress
    getAjaxData("api/connection/macaddress",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            var Macobj = null;
            var Macstrings = '';
            Macobj = ret.response.WLANMACAddress.split(",");
            Macstrings = Macobj.join("<br>");

            $("#lan_mac_address").html(ret.response.MACAddress);
            $("#wlan_mac_address").html(Macstrings);
        }
        else {
            log.error("Load macaddress data failed");
        }
    });
    
    //get dhcp
    getAjaxData("api/dhcp/settings",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            $("#ipv4_ip_address").html(ret.response.DhcpIPAddress);
            $("#subnet_mask").html(ret.response.DhcpLanNetmask);
            $("#ipv4_dhcp_server").html(ret.response.DhcpStatus == 1 ? common_enabled:common_disabled);
            $("#ipv4_start_ip_address").html(ret.response.DhcpStartIPAddress);
            $("#ipv4_end_ip_address").html(ret.response.DhcpEndIPAddress);

            var DHCP_MIN_LEASE_TIME = 60;
            var DHCP_MAX_LEASE_TIME = 604800;
            var DHCP_DEFAULT_LEASE_TIME = 86400;
            var DHCP_DEFAULT_HOUR = 3600;
            var DHCP_DEFAULT_MINUTE = 60;
            var day_value = parseInt(ret.response.DhcpLeaseTime/DHCP_DEFAULT_LEASE_TIME);
            var hour_value = parseInt((ret.response.DhcpLeaseTime % DHCP_DEFAULT_LEASE_TIME) / DHCP_DEFAULT_HOUR);
            var minute_value = parseInt((ret.response.DhcpLeaseTime % DHCP_DEFAULT_LEASE_TIME % DHCP_DEFAULT_HOUR) / DHCP_DEFAULT_MINUTE);
            var second_value = parseInt(ret.response.DhcpLeaseTime % DHCP_DEFAULT_LEASE_TIME % DHCP_DEFAULT_HOUR % DHCP_DEFAULT_MINUTE);			
            $("#ipv4_dhcp_lease_time").html(day_value+time_unit_day+' '+hour_value+time_unit_hour+' '+minute_value+time_unit_minute+' '+second_value+time_unit_second_all);
        }
        else {
            log.error("Load dhcp data failed");
        }
    });
    
    //get ipv6
    getAjaxData("api/settings/hostinfo",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            $("#ipv6_ip_address").html(ret.response.HostIPv6Address);
        }
        else {
            log.error("Load ipv6 data failed");
        }
    });
    
    //get slaac
    getAjaxData("api/settings/slaac",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            $("#Prefix").html(ret.response.Prefix);
            $("#PrefixLength").html(ret.response.PrefixLength);
            $("#Preferredlifetime").html(ret.response.PreferredLifeTime+' '+time_unit_second);
            $("#Validlifetime").html(ret.response.ValidLifeTime+' '+time_unit_second);
        }
        else {
            log.error("Load slaac data failed");
        }
    });
    //add internel time between remove and wirte newline
    $("#table_lanlist > tbody > tr:gt(0)").remove();
 
    getAjaxData("api/connection/wlanstatus", function($xml) {
        var ret = xml2object($xml);
        wlanlistHosts = [];
        if(ret.type == "response") {
            if(typeof(ret.response.Hosts) != 'undefined'){
                if(ret.response.Hosts.Host) {
                    if($.isArray(ret.response.Hosts.Host)){
                        wlanlistHosts = ret.response.Hosts.Host;
                    }else{
                        wlanlistHosts.push(ret.response.Hosts.Host);
                    }
                }else{
                    log.error("Load WLAN Status data failed");
                }
            }else{
                log.error("Load WLAN Status data failed");
            }
        } else {
            log.error("Load WLAN Status data failed");
        }
    },{ sync: true });

    //get lan status
    getAjaxData("api/connection/lanstatus", function($xml) {
        var ret = xml2object($xml);
        var new_line = '';
        if(ret.type == "response"){
            if(ret.response.Hosts.Host){
                var listHosts = ret.response.Hosts.Host;
                if($.isArray(listHosts)){
                    $.each(listHosts, function(i) {
                        $.each(wlanlistHosts, function(j) {
                            if(wlanlistHosts[j].AssociatedDeviceMACAddress == listHosts[i].MACAddress)
                            {
                                listHosts[i].HostName =  'WIFI_'+ (j + 1) + '_' + wlanlistHosts[j].SSID ;
                            }
                        });
                    new_line += "<tr><td id= 'lan_hostname"+ i  +"'>" + listHosts[i].HostName + "</td>";
                    new_line += "<td id= 'lan_mac"+ i  +"'>" + listHosts[i].MACAddress + "</td>";
                    new_line += "<td id= 'lan_ip"+ i  +"'>" + listHosts[i].IPAddress + '<br/>' + listHosts[i].X_IPv6Address + "</td>";
                    new_line += "<td id= 'lan_TimeRe"+ i  +"'>" +  (listHosts[i].LeaseTimeRemaining==0?dialup_label_static:getCurrentTime(listHosts[i].LeaseTimeRemaining)) + "</td></tr>";
                    });
                } else {
                    $.each(wlanlistHosts, function(j) {
                        if(wlanlistHosts[j].AssociatedDeviceMACAddress == listHosts.MACAddress)
                        {
                            listHosts.HostName = 'WIFI_'+ (j + 1) + '_' + wlanlistHosts[j].SSID ;
                        }
                    });
                    new_line += "<tr><td id= 'lan_hostname0'>" + listHosts.HostName + "</td>";
                    new_line += "<td id= 'lan_mac0'>" + listHosts.MACAddress + "</td>";
                    new_line += "<td id= 'lan_ip0'>" + listHosts.IPAddress + '<br/>' + listHosts.X_IPv6Address + "</td>";
                    new_line += "<td id= 'lan_TimeRe0'>" +  (listHosts.LeaseTimeRemaining==0?dialup_label_static:getCurrentTime(listHosts.LeaseTimeRemaining)) + "</td></tr>";
                }
                $("#table_lanlist").append(new_line);
            }  
        } else {
            log.error("Load LAN Status data failed");
        }
    });

}

$(document).ready( function() {
    if (!parent.g_feature_ipv6) {
        $("#lanstatus_ipv6_title").hide();
        $("#lanstatus_ipv6_table").hide();
    } else {
        $("#lanstatus_ipv6_title").show();
        $("#lanstatus_ipv6_table").show();
    }
    getAllInfo();
    $("#button_refresh").click( function(){
        getAllInfo();
    });
});