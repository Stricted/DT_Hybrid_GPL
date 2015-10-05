var TURN_OFF = 0;
var TURN_ON = 1;
var wifi_enable = parent.G_MonitoringStatus.response.WifiStatus;

function setHTMLByWlanBasicSetting() {
    if('admin' == parent.g_login_usertype)
    {
        $("#wlan_status_switch").show();
    }
    var monitoring_status = parent.G_MonitoringStatus;
    if(null != monitoring_status && monitoring_status.response.WifiStatus == 1) {
        button_enable('button_turn', '1');
        $('#wlan_status').text(common_on);
        $('#wlan_currentwifiuser').html(monitoring_status.response.CurrentWifiUser + '/' + monitoring_status.response.TotalWifiUser);
        $("#show_wlan_list").show();
        $("#button_turn .button_center").html(common_turn_off);
    } else {
        button_enable('button_turn', '1');
        $('#wlan_status').text(common_off); 
        $('#wlan_currentwifiuser').html(wlan_label_no_users);
        $("#show_wlan_list").hide();
        $("#button_turn .button_center").html(common_turn_on);
    }
    setTimeout(function(){setHTMLByWlanBasicSetting();},3000);
}

function getAllInfo() {   
    setHTMLByWlanBasicSetting();
    getAjaxData("api/connection/wlanstatus", function($xml) {
        var ret = xml2object($xml);
        var new_line = '';
        if(ret.type == "response") {
            if(typeof(ret.response.Hosts) != 'undefined'){
                if(ret.response.Hosts.Host) {
                    var listHosts = ret.response.Hosts.Host;
                    $("#table_wlanlist > tbody > tr:gt(0)").remove();
                    if($.isArray(listHosts)){
                        $.each(listHosts, function(i) {
                        new_line += "<tr><td>" + listHosts[i].SSID + "</td>";
                        new_line += "<td>" + listHosts[i].AssociatedDeviceMACAddress + "</td>";
                        new_line += "<td>" +  getCurrentTime(listHosts[i].X_AssociatedDeviceLeaseTime) + "</td></tr>";
                        });
                    } else {
                        new_line += "<tr><td>" + listHosts.SSID + "</td>";
                        new_line += "<td>" + listHosts.AssociatedDeviceMACAddress + "</td>";
                        new_line += "<td>" +  getCurrentTime(listHosts.X_AssociatedDeviceLeaseTime) + "</td></tr>";                    
                    }
                    $("#table_wlanlist").append(new_line);
                }else{
                    log.error("Load WLAN Status data failed");
                }
            }else{
                log.error("Load WLAN Status data failed");
            }
        } else {
            log.error("Load WLAN Status data failed");
        }
    });
}

$(document).ready( function() {
    getAllInfo();
    $("#button_refresh").click( function() {
        getAllInfo();
    });
    
    $("#button_turn").live("click",function(){ 
        var btn_text = $(this).find(".button_center").html(); 
        if(btn_text == common_turn_off) { 
            wifi_enable = TURN_OFF; 
        } 
        else { 
            wifi_enable = TURN_ON; 
        } 
        var status_set = { 
            'Wifienable': wifi_enable 
        }; 
        var req = object2xml("request",status_set); 
        saveAjaxData("api/wlan/wifienable",req,function($xml){ 
            var ret = xml2object($xml); 
            button_enable('button_turn', '0');
            if(isAjaxReturnOK(ret)){ 
            } 
            else { 
                log.error("Save basic request failed"); 
                button_enable('button_turn', '1');
            } 
        }); 
    });   
});