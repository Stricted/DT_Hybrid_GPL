var ok_flag = 0;
var STATICADDRESS_RULES_MAX = 8;
var ENABLE = 1;
var DISABLE = 0;
var StatusArray = [
    [DISABLE,common_disable],
    [ENABLE,common_enable]
]; 
var HostList = {};
var listDhcp = {};
var listHosts= {};
var editIndex;
var g_dhcpstatic_index = 0;


function addFilter_dhcprouter(insertNode){
    var addLine = null;
    var index = arguments[arguments.length-1];

    addLine = "<tr class=\"user_add_line\">";
    addLine += "<td id= 'route_mac"+ index  +"'>" + arguments[1] + "</td>";
    addLine += "<td id= 'route_ip"+ index  +"'>" + arguments[2] + "</td>";
    addLine += "<td id= 'route_status"+ index  +"'>" + arguments[3] + "</td>";
    addLine += "<td class='user_options'><span id ='router_edit"+ index +"' class=\"button_edit_list\">" + common_edit + "</span>&nbsp;&nbsp;<span id ='route_delete"+ index +"' class=\"button_delete_list\">" + common_delete + "</span></td></tr>";
    
    var currentTrTotal = $(insertNode).size();
    $(insertNode).eq(currentTrTotal - 2).after(addLine);
}


function isVaildValue(){
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    if(!isValidMacAddress($('#MACAddress').val())){
        showQtip('MACAddress', wlan_hint_mac_address_invalid+wlan_label_mac_address_example);
        return false;
    }

    if(!isValidIpAddress(($('#StaticIPAddress').val()))){
        showQtip('StaticIPAddress', dialup_hint_ip_address_empty);
        return false;
    }
    
    if(!(isSameSubnetAddrs($('#StaticIPAddress').val(),listDhcp.DhcpIPAddress,listDhcp.DhcpLanNetmask))){
        showQtip('StaticIPAddress', static_address_invalid);
        return false;
    }

    if(!IsIpInRange($('#StaticIPAddress').val(),listDhcp.DhcpStartIPAddress,listDhcp.DhcpEndIPAddress)){
        showQtip('StaticIPAddress', dhcp_staticaddress_invalid);
        return false;
    }
    
    if($.isArray(listHosts)){
        for(var i=0;i < listHosts.length;i++){
            if(listHosts[i].IPAddress==$('#StaticIPAddress').val()){   
                showQtip('StaticIPAddress', static_address_Used);
                return false;
            }  
        }
    } 
    else {
        if(listHosts.IPAddress==$('#StaticIPAddress').val()){         
            showQtip('StaticIPAddress', static_address_Used);
            return false;
        }                    
    }
    return true;
}

function isSameRule(){
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    var isSR_Status = false;
    $('.user_add_line').each(function(i) {
        var MacAddr = $(this).children().eq(0).text();
        var IpAddr = $(this).children().eq(1).text();
        if(MacAddr == $('#MACAddress').val()){
            isSR_Status = true;
            showQtip('MACAddress', same_mac_address);
        }else if(IpAddr==  $('#StaticIPAddress').val()){
            isSR_Status = true;
            showQtip('StaticIPAddress', static_address_reiteration);
        }
    });
    return isSR_Status;
}
function initPage(){
    button_enable('apply', '0');
    $('.user_add_line').remove();

    getAjaxData('api/dhcp/settings',function($xml){
        var ret_dhcp = xml2object($xml);
        listDhcp = ret_dhcp.response;
     });
    g_dhcpstatic_index = 0;
    getAjaxData('api/dhcp/static-addr-info', function($xml){
            var ret = xml2object($xml);
            var staticaddr_status;
            if(('response' == ret.type) && (typeof(ret.response.Hosts) != 'undefined') && (ret.response.Hosts.Host)){
                HostList = ret.response.Hosts.Host;
                if(HostList){
                    if($.isArray(HostList)){
                        $(HostList).each(function(i){
                                staticaddr_status =  getDArrayElement(StatusArray,HostList[i].HostEnabled,'value');
                                addFilter_dhcprouter(
                                    $('#service_list tr'),
                                    HostList[i].HostHw,
                                    HostList[i].HostIp,
                                    staticaddr_status,
                                    g_dhcpstatic_index ++
                                );
                            });
                        }else{
                            staticaddr_status =  getDArrayElement(StatusArray,HostList.HostEnabled,'value');
                            addFilter_dhcprouter(
                                $('#service_list tr'),
                                HostList.HostHw,
                                HostList.HostIp,
                                staticaddr_status,
                                g_dhcpstatic_index ++
                            );
                        }
                    }
                }
        if ($('.user_add_line').length >= STATICADDRESS_RULES_MAX)
        {
            button_enable('add_item', '0');
        }
    });
    //get lan status
    getAjaxData("api/connection/lanstatus", function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response"){
            if(ret.response.Hosts.Host){
                listHosts = ret.response.Hosts.Host;
            }  
        } else {
            log.error("Load LAN Status data failed");
        }
    });
}

function postData(){
    var submitObject = {};
    var StaticAddrArray = [];
    $('.user_add_line').each(function(i) {
        var MacAddress = $(this).children().eq(0).text();
        var IpAddress = $(this).children().eq(1).text();
        var Status = getDArrayElement(StatusArray, $(this).children().eq(2).text(), 'key');

        var SAddrs ={
            HostHw : MacAddress,
            HostIp : IpAddress,
            HostEnabled : Status
        };

        StaticAddrArray.push(SAddrs);
    });

    submitObject = {
        Hosts: {
            Host: StaticAddrArray
        }
    };
    var submitData = object2xml('request', submitObject);

    saveAjaxData('api/dhcp/static-addr-info', submitData, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            button_enable('apply', '0');
        }
        else {
            parent.showInfoDialog(common_failed);
        }
        initPage();
    });
}

$(document).ready( function() {
    initPage();
    var currentAllVal = null;
    var editStatus = null;
    $('.button_edit_list').live('click', function() {
        if (($(".add_item_control:hidden").size() > 0) && ($('#edit_item_ok').size() < 1)) {
            editIndex = $('.button_edit_list').index(this);
            currentAllVal = $('.user_add_line').eq(editIndex).html();
            var editStaticAddr = $(this).parent().siblings();
            var editMACAddr = editStaticAddr.eq(0);
            var editIPAddr = editStaticAddr.eq(1);
            editStatus = editStaticAddr.eq(2);
            var htmlStatus = editStatus.html();

            editMACAddr.html('<input type="text" value="' + editMACAddr.html() + '" id="MACAddress" maxlength="17" /></td>');
            editIPAddr.html('<input type="text" value="' + editIPAddr.html() + '" id="StaticIPAddress" maxlength="15" /></td>');
            createSelect(editStatus,'StaticStatus',StatusArray);
            $('#StaticStatus').val(getDArrayElement(StatusArray, htmlStatus, 'key'));

            $(this).parent().html('<a id="edit_item_ok" href="javascript:void(0);">' + common_ok +
            '</a>&nbsp;&nbsp;<a id="edit_item_cancel" href="javascript:void(0);">' + common_cancel + '</a>');

            hideAddItemControl();
            button_enable('apply', '0');
            button_enable('add_item', '0');
        }  
    });

    $('#edit_item_ok').live('click', function(){
        if(isVaildValue() && !isSameRule()){
            var edit_macaddr = $.trim($('#MACAddress').val());
            var edit_ipaddr = $.trim($('#StaticIPAddress').val());
            var edit_status = $('#StaticStatus option:selected').text();

            hideAddItemControl();
            var editStaticAddr = $(this).parent().siblings();
            editStaticAddr.eq(0).html(edit_macaddr);
            editStaticAddr.eq(1).html(edit_ipaddr);
            editStaticAddr.eq(2).html(edit_status);

            $(this).parent().html('<span class=\"button_edit_list\">' + common_edit +
            '</span>&nbsp;&nbsp;<span class=\"button_delete_list\">' + common_delete + '</span>');
            currentAllVal = $('.user_add_line').eq(editIndex).html();
            button_enable('apply', '1');
            button_enable('add_item', '1');
            ok_flag = 1;
            if ($('.user_add_line').length >= STATICADDRESS_RULES_MAX){
                button_enable('add_item', '0');
            }
        }
    });

    $('#edit_item_cancel').live('click', function() {
        $('.user_add_line').eq(editIndex).html(currentAllVal);
        $('.qtip').qtip('destroy');
        if (!isButtonEnable('add_item'))
        {
            button_enable('add_item', '1');
            if(1 == ok_flag)
            {
                button_enable('apply', '1');
                ok_flag = 0;
            }
        }
    });

    $('#add_item_cancel').live('click', function() {
        hideAddItemControl();
        return false;
    });

    $('#add_item_ok').live('click', function() {
        if (isVaildValue() && !isSameRule())
        {
            var add_macaddr = $.trim($('#MACAddress').val());
            var add_ipaddr = $.trim($('#StaticIPAddress').val());
            var add_status = $('#StaticStatus option:selected').text();

            hideAddItemControl();

            addFilter_dhcprouter(
                $('#service_list tr'), 
                add_macaddr, 
                add_ipaddr, 
                add_status,
                g_dhcpstatic_index ++
            );

            button_enable('apply', '1');

            if (STATICADDRESS_RULES_MAX <= $('.user_add_line').length)
            {
                button_enable('add_item', '0');
            }
        }
        return false;
    });

    $('#add_item').click(function() {
        if (isButtonEnable('add_item'))
        {
            showAddItemControl();
            $('.add_item_control input').eq(0).focus();
            button_enable('apply', '0');
        }
    });

    $('#apply').click(function() {
        if (isButtonEnable('apply')) {
            parent.showConfirmDialog(firewall_hint_submit_list_item, postData);
        }        
    });

    initSelectOption('StaticStatus', StatusArray);
 });