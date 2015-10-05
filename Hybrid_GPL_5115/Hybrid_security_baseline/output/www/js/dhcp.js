// JavaScript Document
/***********************************************dhcp (start)**********************/

var dhcp_value = null;
var g_DHCP_SERVER_ENABLE = 1;
var g_DHCP_SERVER_DISABLE = 0;
var HAVE_TWO_DNS_SERVER = 2;
var DHCP_PING_INTERVA = 50000;
var DHCP_MIN_LEASE_TIME = 60;
var DHCP_MAX_LEASE_TIME = 604800;
var DHCP_DEFAULT_LEASE_TIME = 86400;
var DHCP_DEFAULT_HOUR = 3600;
var DHCP_DEFAULT_MINUTE = 60;
var g_dhcpLeaseTime = 0;
var g_wan_ip_address = '';
var g_quicksetup_defaultEthIndex;
var g_quicksetup_ethInfo;
var g_Eth_list;
var g_quicksetup_ethInfo;
var g_quicksetup_currentEthInfo;
var QUICKSETUP_ADDR_STATIC_STATIC = 'Static';
var is_have_dhcp_static_address = false;


function enableDHCP() {
    var dhcpStartIPAddressTmp = '';
    var dhcpEndIPAddressTmp = '';
    var dhcpLeaseTimeTmp = '';
    if ('' != dhcp_value.DhcpStartIPAddress && '0' != dhcp_value.DhcpStartIPAddress && '0.0.0.0' != dhcp_value.DhcpStartIPAddress)
    {
        dhcpStartIPAddressTmp = dhcp_value.DhcpStartIPAddress;
    }
    else
    {
       dhcpStartIPAddressTmp = dhcp_value.DhcpIPAddress.slice(0, dhcp_value.DhcpIPAddress.lastIndexOf('.')) + '.100';
    }
    if ('' != dhcp_value.DhcpEndIPAddress && '0' != dhcp_value.DhcpEndIPAddress && '0.0.0.0' != dhcp_value.DhcpEndIPAddress)
    {
        dhcpEndIPAddressTmp = dhcp_value.DhcpEndIPAddress;
    }
    else
    {
       dhcpEndIPAddressTmp = dhcp_value.DhcpIPAddress.slice(0, dhcp_value.DhcpIPAddress.lastIndexOf('.')) + '.200';
    }
    if ('' != dhcp_value.DhcpLeaseTime && '0' != dhcp_value.DhcpLeaseTime)
    {
        dhcpLeaseTimeTmp = dhcp_value.DhcpLeaseTime;
    }
    else
    {
        dhcpLeaseTimeTmp = DHCP_DEFAULT_LEASE_TIME;
    }
    var dns_server = dhcp_value.DnsServers.split(',');
    if(HAVE_TWO_DNS_SERVER == dns_server.length)
    {
        $('#dhcp_dns_server').val(dns_server[0]).attr('disabled', false);
        $('#dhcp_dns_seconds').val(dns_server[1]).attr('disabled', false);
    }
    else
    {
        $('#dhcp_dns_server').val(dns_server[0]).attr('disabled', false);
        $('#dhcp_dns_seconds').val('').attr('disabled', false);
    }
    $('#input_dhcp_startip').val(dhcpStartIPAddressTmp).attr('disabled', false);
    $('#input_dhcp_endip').val(dhcpEndIPAddressTmp).attr('disabled', false);
    var day_value = parseInt(dhcpLeaseTimeTmp/DHCP_DEFAULT_LEASE_TIME);
    var hour_value = parseInt((dhcpLeaseTimeTmp % DHCP_DEFAULT_LEASE_TIME) / DHCP_DEFAULT_HOUR);
    var minute_value = parseInt((dhcpLeaseTimeTmp % DHCP_DEFAULT_LEASE_TIME % DHCP_DEFAULT_HOUR) / DHCP_DEFAULT_MINUTE);
    var second_value = parseInt(dhcpLeaseTimeTmp % DHCP_DEFAULT_LEASE_TIME % DHCP_DEFAULT_HOUR % DHCP_DEFAULT_MINUTE);
    $('#input_time_day').val(day_value).attr('disabled', false);
    $('#input_time_hour').val(hour_value).attr('disabled', false);
    $('#input_time_minute').val(minute_value).attr('disabled', false);
    $('#input_time_second').val(second_value).attr('disabled', false);
}

function disabledDHCP() {
    $('#input_dhcp_startip').val('').attr('disabled', true);
    $('#input_dhcp_endip').val('').attr('disabled', true);
    $('#dhcp_dns_server').val('').attr('disabled', true);
    $('#dhcp_dns_seconds').val('').attr('disabled', true);
    $('#input_time_day').val('').attr('disabled', true);
    $('#input_time_hour').val('').attr('disabled', true);
    $('#input_time_minute').val('').attr('disabled', true);
    $('#input_time_second').val('').attr('disabled', true);
}

function initPageData() {
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret;
        ret = xml2object($xml);
        log.debug('type= ' + ret.type);
        if (ret.type == 'response')
        {
            dhcp_value = ret.response;
            dhcp_value.DhcpStatus = parseInt(dhcp_value.DhcpStatus, 10);
            $('#input_dhcp_ipaddr').val(dhcp_value.DhcpIPAddress);
            $('#input_dhcp_submark').val(dhcp_value.DhcpLanNetmask);
            $("input[name='radio_dhcp_status'][value=" + dhcp_value.DhcpStatus + ']').attr('checked', true);
            if (g_DHCP_SERVER_ENABLE == dhcp_value.DhcpStatus)
            {
                enableDHCP();
            }
            else
            {
                disabledDHCP();
            }
        }
    });
    g_Eth_list = [];
    getAjaxData("api/dialup/ETHs",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
           if(typeof(ret.response.ETHs) != 'undefined'){
                g_quicksetup_defaultEthIndex = ret.response.CurrentETH;
                g_quicksetup_ethInfo = ret.response.ETHs.ETH;
                if(g_quicksetup_ethInfo){
                    if($.isArray(g_quicksetup_ethInfo)) {
                        g_Eth_list= g_quicksetup_ethInfo;
                    }else{
                        g_Eth_list.push(g_quicksetup_ethInfo);
                    }
                 }
               if (g_Eth_list.length > 0 && g_quicksetup_defaultEthIndex >= 0)
               {
                   $.each(g_Eth_list, function(ind, current_Ethprofile) {
                        if (current_Ethprofile.Index == g_quicksetup_defaultEthIndex)
                        {
                            g_quicksetup_currentEthInfo= current_Ethprofile;
                            if(typeof(g_quicksetup_currentEthInfo.IdAddrType!= 'undefined') && g_quicksetup_currentEthInfo.IdAddrType == QUICKSETUP_ADDR_STATIC_STATIC)
                            {
                                g_wan_ip_address = g_quicksetup_currentEthInfo.IdIpAddr;
                            }
                            return;
                        }
                    });
               }
           }else{
            log.error("the ETHs.xml'ETHs node is null or error!");
           }
        }
        }, {
            sync:true
        });

        getAjaxData('api/dhcp/static-addr-info', function($xml){
        var ret = xml2object($xml);
        var staticaddr_status;
        if(('response' == ret.type) && (typeof(ret.response.Hosts) != 'undefined') && (ret.response.Hosts.Host))
        {
            var HostList = ret.response.Hosts.Host;
            if(HostList)
            {
                is_have_dhcp_static_address= true;
            }
        }
        }, {
            sync:true
        });
 
}

function postData() {
    dhcp_value = {};
    dhcp_value.DhcpIPAddress = $('#input_dhcp_ipaddr').val();
    dhcp_value.DhcpLanNetmask = $('#input_dhcp_submark').val();
    dhcp_value.DhcpStatus = $("input[name='radio_dhcp_status']:checked").val();
    dhcp_value.DnsStatus = 0;
    if(g_DHCP_SERVER_ENABLE == dhcp_value.DhcpStatus)
        {
            dhcp_value.DhcpStartIPAddress = $('#input_dhcp_startip').val();
            dhcp_value.DhcpEndIPAddress = $('#input_dhcp_endip').val();
            dhcp_value.DhcpLeaseTime = g_dhcpLeaseTime;
            if(($('#dhcp_dns_server').val()=="" || $('#dhcp_dns_server').val()==null) && 
              ($('#dhcp_dns_seconds').val()=="" || $('#dhcp_dns_seconds').val()==null) )
            {
               dhcp_value.DnsServers = '';
            }
            else if($('#dhcp_dns_seconds').val()=="" || $('#dhcp_dns_seconds').val()==null) 
            {
               dhcp_value.DnsServers = $('#dhcp_dns_server').val();
            }
            else
            {
               dhcp_value.DnsServers = $('#dhcp_dns_server').val()+","+$('#dhcp_dns_seconds').val();
            }
        }
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments, function() {});
    var xmlstr = object2xml('request', dhcp_value);   
    saveAjaxData('api/dhcp/settings', xmlstr, function($xml) {
        var ret = xml2object($xml);
        if (!isAjaxReturnOK(ret)) {
            parent.closeWaitingDialog();
            parent.showInfoDialog(common_failed);
            return;
        }
    });  
    ping_setPingAddress($.trim(dhcp_value.DhcpIPAddress));
    setTimeout(startPing, DHCP_PING_INTERVA);
}

function isBetweenStartIPAndEndIP(ipAddress, startIP, endIP) {
    var num = 0;
    var numStart = 0;
    var numEnd = 0;
    var maskParts = ipAddress.split('.');
    var maskStartIPParts = startIP.split('.');
    var maskEndIPParts = endIP.split('.');

    num = parseInt(maskParts[3], 10);
    numStart = parseInt(maskStartIPParts[3], 10);
    numEnd = parseInt(maskEndIPParts[3], 10);
    if ((num >= numStart) && (num <= numEnd))
    {
        return true;
    }
        else
    {
        return false;
    }
}
function isValidTimeValue() {
    var dhcpLeaseTime_day = $('#input_time_day').val();
    var dhcpLeaseTime_hour = $('#input_time_hour').val();
    var dhcpLeaseTime_minute = $('#input_time_minute').val();
    var dhcpLeaseTime_second = $('#input_time_second').val();
    
    if (-1 != dhcpLeaseTime_day.indexOf('.'))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_integer);
        $('#input_time_day').focus();
        return false;
    }
    else if (true == isNaN(dhcpLeaseTime_day))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_number);
        $('#input_time_day').focus();
        return false;
    }
    else if ((0 == dhcpLeaseTime_day.indexOf('0')) && (0 != dhcpLeaseTime_day))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_invalid);
        $('#input_time_day').focus();
        return false;
    }
    else if ( (0 > dhcpLeaseTime_day) || (7 < dhcpLeaseTime_day))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_valid);
        $('#input_time_day').focus();
        return false;
    }
    
    if (-1 != dhcpLeaseTime_hour.indexOf('.'))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_integer);
        $('#input_time_hour').focus();
        return false;
    }
    else if (true == isNaN(dhcpLeaseTime_hour))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_number);
        $('#input_time_hour').focus();
        return false;
    }
    else if ((0 == dhcpLeaseTime_hour.indexOf('0')) && (0 != dhcpLeaseTime_hour))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_invalid);
        $('#input_time_hour').focus();
        return false;
    }
    else if ( (0 > dhcpLeaseTime_hour) || (23 < dhcpLeaseTime_hour))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_valid);
        $('#input_time_hour').focus();
        return false;
    }
    
    if (-1 != dhcpLeaseTime_minute.indexOf('.'))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_integer);
        $('#input_time_minute').focus();
        return false;
    }
    else if (true == isNaN(dhcpLeaseTime_minute))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_number);
        $('#input_time_minute').focus();
        return false;
    }
    else if ((0 == dhcpLeaseTime_minute.indexOf('0')) && (0 != dhcpLeaseTime_minute))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_invalid);
        $('#input_time_minute').focus();
        return false;
    }
    else if ( (0 > dhcpLeaseTime_minute) || (59 < dhcpLeaseTime_minute))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_valid);
        $('#input_time_minute').focus();
        return false;
    }
    
    if (-1 != dhcpLeaseTime_second.indexOf('.'))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_integer);
        $('#input_time_second').focus();
        return false;
    }
    else if (true == isNaN(dhcpLeaseTime_second))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_number);
        $('#input_time_second').focus();
        return false;
    }
    else if ((0 == dhcpLeaseTime_second.indexOf('0')) && (0 != dhcpLeaseTime_second))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_invalid);
        $('#input_time_second').focus();
        return false;
    }
    else if ( (0 > dhcpLeaseTime_second) || (59 < dhcpLeaseTime_second))
    {
        showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_valid);
        $('#input_time_second').focus();
        return false;
    }
    
    return true;
}
function verifyUserInput() {
    var dhcpIPAddresss = $('#input_dhcp_ipaddr').val();
    var dhcpLanNetmask = $('#input_dhcp_submark').val();
    var dhcpStatus = $("input[name='radio_dhcp_status']:checked").val();
    var dhcpStartIPAddress = $('#input_dhcp_startip').val();
    var dhcpEndIPAddress = $('#input_dhcp_endip').val();
    var dhcpLeaseTime = 0;
    var dhcp_dns_server = $("#dhcp_dns_server").val();
    var dhcp_dns_seconds = $("#dhcp_dns_seconds").val();

    clearAllErrorLabel();
    if (!isValidIpAddress(dhcpIPAddresss))
    {
        showErrorUnderTextbox('input_dhcp_ipaddr', dialup_hint_ip_address_empty);
        $('#input_dhcp_ipaddr').focus();
        return false;
    }

    if (!isValidSubnetMask(dhcpLanNetmask))
    {
        showErrorUnderTextbox('input_dhcp_submark', dhcp_hint_subnet_mask_invalid);
        $('#input_dhcp_submark').focus();
        return false;
    }

    if (!isBroadcastOrNetworkAddress(dhcpIPAddresss, dhcpLanNetmask))
    {
        showErrorUnderTextbox('input_dhcp_ipaddr', dialup_hint_ip_address_empty);
        $('#input_dhcp_ipaddr').focus();
        return false;
    }
//enable DHCP server
    if (1 == dhcpStatus)
    {
        if ((!isValidIpAddress(dhcpStartIPAddress)) || (dhcpIPAddresss == dhcpStartIPAddress))
        {
            showErrorUnderTextbox('input_dhcp_startip', dhcp_hint_start_ip_address_invalid);
            $('#input_dhcp_startip').focus();
            return false;
        }
        if (!isBroadcastOrNetworkAddress(dhcpStartIPAddress, dhcpLanNetmask))
        {
            showErrorUnderTextbox('input_dhcp_startip', dhcp_hint_start_ip_address_invalid);
            $('#input_dhcp_startip').focus();
            return false;
        }
        if ((!isValidIpAddress(dhcpEndIPAddress)) || (dhcpIPAddresss == dhcpEndIPAddress))
        {
            showErrorUnderTextbox('input_dhcp_endip', dhcp_hint_end_ip_address_invalid);
            $('#input_dhcp_endip').focus();
            return false;
        }
        if (!isBroadcastOrNetworkAddress(dhcpEndIPAddress, dhcpLanNetmask))
        {
            showErrorUnderTextbox('input_dhcp_endip', dhcp_hint_end_ip_address_invalid);
            $('#input_dhcp_endip').focus();
            return false;
        }
        if (!isSameSubnetAddrs(dhcpStartIPAddress, dhcpIPAddresss, dhcpLanNetmask))
        {
            showErrorUnderTextbox('input_dhcp_startip', dhcp_hint_start_ip_address_same_subnet);
            $('#input_dhcp_startip').focus();
            return false;
        }
        if (!isSameSubnetAddrs(dhcpEndIPAddress, dhcpIPAddresss, dhcpLanNetmask))
        {
            showErrorUnderTextbox('input_dhcp_endip', dhcp_hint_end_ip_address_same_subnet);
            $('#input_dhcp_endip').focus();
            return false;
        }
        if (!compareStartIpAndEndIp(dhcpStartIPAddress, dhcpEndIPAddress))
        {
            showErrorUnderTextbox('input_dhcp_endip', dhcp_hint_end_ip_greater_start_ip);
            $('#input_dhcp_endip').focus();
            return false;
        }

        if (!isValidTimeValue())
        {
            return false;
        }
        else
        {
            g_dhcpLeaseTime = parseInt(DHCP_DEFAULT_LEASE_TIME*$('#input_time_day').val()) + parseInt(DHCP_DEFAULT_HOUR*$('#input_time_hour').val()) + parseInt(DHCP_DEFAULT_MINUTE*$('#input_time_minute').val()) + parseInt($('#input_time_second').val());
            g_dhcpLeaseTime = g_dhcpLeaseTime.toString(10);
            dhcpLeaseTime = g_dhcpLeaseTime
        }
        
        if (-1 != dhcpLeaseTime.indexOf('.'))
        {
            showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_integer);
            $('#input_time_day').focus();
            return false;
        }
        else if (true == isNaN(dhcpLeaseTime))
        {
            showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_number);
            $('#input_time_day').focus();
            return false;
        }
        else if ((0 == dhcpLeaseTime.indexOf('0')) && (0 != dhcpLeaseTime))
        {
            showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_invalid);
            $('#input_time_day').focus();
            return false;
        }
        else if ((dhcpLeaseTime < DHCP_MIN_LEASE_TIME) || (dhcpLeaseTime > DHCP_MAX_LEASE_TIME))
        {
            showErrorUnderTextbox('input_time_hide', dhcp_hint_dhcp_lease_time_valid);
            $('#input_time_day').focus();
            return false;
        }
        else if (isBetweenStartIPAndEndIP(dhcpIPAddresss, dhcpStartIPAddress, dhcpEndIPAddress))
        {
            showErrorUnderTextbox('input_dhcp_ipaddr', dhcp_message_ip_address_in_start_end);
            $('#input_dhcp_ipaddr').focus();
            return false;
        }
        if(dhcp_dns_server == '' && dhcp_dns_seconds == '')
        {
             return true;
        }
        if(dhcp_dns_server == '' && dhcp_dns_seconds != ''){
            showErrorUnderTextbox('dhcp_dns_server', err_dhcp_dns_server);
            return false;
        }
        if(!isValidIpAddress(dhcp_dns_server)){
            showErrorUnderTextbox('dhcp_dns_server', err_dhcp_dns_server);
            return false;
        }
        if(dhcp_dns_seconds == ''){
            return true;
        } 
        if(!isValidIpAddress(dhcp_dns_seconds, 1)){
            showErrorUnderTextbox('dhcp_dns_seconds', err_dhcp_dns_seconds_server);
            return false;
        }
    }
    return true;
}
function is_wanip_address(ipv4)
{
    var matchString = g_wan_ip_address.substring(0, g_wan_ip_address.lastIndexOf("."));
    var matchString1 = ipv4.substring(0, ipv4.lastIndexOf("."));
    if(matchString == matchString1)
    {
       return true;
    } 
    return false;
}

function is_wan_ip()
{
    var dhcpIPAddresss = $('#input_dhcp_ipaddr').val();
    var dhcpStartIPAddress = $('#input_dhcp_startip').val();
    var dhcpEndIPAddress = $('#input_dhcp_endip').val();
    if(g_wan_ip_address!='')
    {
        if(is_wanip_address(dhcpIPAddresss))
        {
            showErrorUnderTextbox('input_dhcp_ipaddr', label_dhcp_not_WAN_ip_address);
            $('#input_dhcp_ipaddr').focus();
            return true;
        }
        if(is_wanip_address(dhcpStartIPAddress))
        {
            showErrorUnderTextbox('input_dhcp_startip', label_dhcp_not_WAN_ip_address);
            $('#input_dhcp_ipaddr').focus();
            return true;
        }
        if(is_wanip_address(dhcpStartIPAddress))
        {
            showErrorUnderTextbox('input_dhcp_endip', label_dhcp_not_WAN_ip_address);
            $('#input_dhcp_ipaddr').focus();
            return true;
        }       
    }   
    return false;
}
function onApply() {
    if (!isButtonEnable('apply'))
    {
        return;
    }
    if (verifyUserInput() && !is_wan_ip())
    {
        if(is_have_dhcp_static_address)
        { 
            parent.showConfirmDialog(dhcp_hint_operation_restart_device_disable_static_address, postData, function() {});
        }
        else
        {
            parent.showConfirmDialog(dhcp_hint_operation_restart_device, postData, function() {});
        }
    }
}


$(document).ready(function() {
    $('input').bind('change input paste cut keydown', function() {
        button_enable('apply', '1');
    });
    button_enable('apply', '0');

    $('#apply').click(function() {
        onApply();
    });

    initPageData();
});
