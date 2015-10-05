var ok_flag = 0;
var ok_flag_route = 0;
var ROUTE_LAN = 'br0';
var editIndex_route = null;

var g_deleteIndex_route = 0;
var g_connection_device = "1";

var DEFAULT_GATEWAY_IP = '192.168.1.1';

var listRoute ;
var listipv6Route;
var listDhcp;

var InterfaceArray = [];
var g_wanPath_value_v4 = '';
var g_wanPath_value_v6 = '';
var g_ipv4index = 0;
var g_ipv6index = 0;

$(".button_delete_list_route").live("click", function(){
    if($(".add_item_control_route:hidden").size() > 0 && $("#edit_item_ok_route").size() < 1){
        g_deleteIndex_route = $(".button_delete_list_route").index(this);
        parent.call_dialog(common_delete, firewall_hint_delete_list_item, common_ok, "pop_OK", common_cancel, "pop_Cancel",save_popok,cancel_pop);
    }
});

/*------------------pop ok function-------------------*/
function save_popok() {
    deleteFilter(g_deleteIndex_route, $(".user_add_line_route"));
    parent.clearDialog();
    button_enable("apply_route", "1");
    button_enable("add_item_route", "1");
}

/*--------------------pop cancel function---------------*/
function cancel_pop() {
    parent.clearDialog();
}
function addFilter_v4route(insertNode){
    var addLine = null;
    var index = arguments[arguments.length-1];
    addLine = "<tr class=\"user_add_line\">";

    addLine += "<td id= 'ipv4des_"+ index  +"'>" + arguments[1] + "</td>";
    addLine += "<td id= 'ipv4mask_"+ index  +"'>" + arguments[2] + "</td>";
    addLine += "<td id= 'ipv4gateway_"+ index  +"'>" + arguments[3] + "</td>";
    addLine += "<td id= 'ipv4wanpath_"+ index  +"'>" + arguments[4] + "</td>";
    addLine += "<td id= 'ipv4status_"+ index  +"'>" + arguments[5] + "</td>";
    addLine += "<td class='user_options'><span id ='edit_"+ index +"' class=\"button_edit_list\">" + common_edit + "</span>&nbsp;&nbsp;<span id ='delete_"+ index +"' class=\"button_delete_list\">" + common_delete + "</span></td></tr>";
    
    var currentTrTotal = $(insertNode).size();
    $(insertNode).eq(currentTrTotal - 2).after(addLine);
}

function addFilter_route(insertNode){
    var addLine = null;
    var index = arguments[arguments.length -1];

    addLine = "<tr class=\"user_add_line_route\">";
    addLine += "<td id= 'ipv6des_"+ index  +"'>" + arguments[1] + "</td>";
    addLine += "<td id= 'ipv6prefis_"+ index  +"'>" + arguments[2] + "</td>";
    addLine += "<td id= 'ipv6gateway_"+ index  +"'>" + arguments[3] + "</td>";
    addLine += "<td id= 'ipv6interf_"+ index  +"'>" + arguments[4] + "</td>";
    addLine += "<td id= 'ipv6status_"+ index +"'>" + arguments[5] + "</td>";

    addLine += "<td class='user_options'><span id ='edit_"+ index +"' class=\"button_edit_list_route\">" + common_edit + "</span>&nbsp;&nbsp;<span id ='delete_"+ index +"' class=\"button_delete_list_route\">" + common_delete + "</span></td></tr>";
    var currentTrTotal = $(insertNode).size();
    $(insertNode).eq(currentTrTotal - 2).after(addLine);
}

function showAddItemControl_route(){
    $(".add_item_control_route").show();
}

function hideAddItemControl_route(){
    $(".add_item_control_route").hide();
}
function getValue(element_id)
{
    return $('#'+element_id).val();
}

function checkPrefixLenNum(id){
    var flag = true;
    var sizeParts = $('#'+id).val().split("");
    if($('#'+id).val()<0||$('#'+id).val()>128){
        flag = false;
    } else{
        $(sizeParts).each(function(i){
            if(!(sizeParts[i]<='9'&&sizeParts[i]>='0')){
                flag = false;
            }
        });
    }
    return flag;
}

function checkInvalidDestinationIpv6Addr(addr){
    var ipv6address = addr.toLowerCase();
    if(('::' == addr) || ('::1' == addr) || ('ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff' == ipv6address) ||
        ('ff01' == ipv6address.substring(0,4)) ||('ff02' == ipv6address.substring(0,4)) ||
        ('ff05' == ipv6address.substring(0,4)) || ('ff08' == ipv6address.substring(0,4)) || 
        ('ff0e' == ipv6address.substring(0,4))){
           return false;
    }
    return true;
}

function initPage()
{
    button_enable('apply', '0');
    $('.user_add_line').remove();
    InterfaceArray = [];
    InterfaceArray.push([ROUTE_LAN,static_route_select_check_lan]);
    getAjaxData('api/settings/ddns-wanpath', function($xml) {
        var ret = xml2object($xml);
            if(typeof(ret.response.WanPaths)!='undefined' && typeof(ret.response.WanPaths.WanPath)!='undefined'){
                var WanPathArray = [];
                if($.isArray(ret.response.WanPaths.WanPath)){
                    WanPathArray = ret.response.WanPaths.WanPath;
                }else{
                    WanPathArray.push(ret.response.WanPaths.WanPath);
                }
                for(var i=0;i<WanPathArray.length;i++){
                    InterfaceArray.push([WanPathArray[i].WanPathValue,filterWanPathName(WanPathArray[i].WanPathName)]);
                    if('up' == WanPathArray[i].WanPathStatusV4){
                        g_wanPath_value_v4 = WanPathArray[i].WanPathValue;
                    }
                    if('up' == WanPathArray[i].WanPathStatusV6){
                        g_wanPath_value_v6 = WanPathArray[i].WanPathValue;
                    }
                }
            }
        },{sync:true});
    getAjaxData('api/dhcp/settings',function($xml){
        var ret_dhcp = xml2object($xml);
        listDhcp = ret_dhcp.response;
     });
    getAjaxData('api/connection/connection', function($xml) {
        var conn_ret = xml2object($xml);
        if (conn_ret.type == 'response') {
            g_connection_device = conn_ret.response.ConnectionDevice;
        } else {
            log.error('Error, no data');
        }
    },{
        sync:true
    });    
    getAjaxData('api/settings/staticroute',function($xml){
        g_ipv4index  = 0;

        var ret = xml2object($xml);
        WAN_Enable = ret.response.Enable;
        if(typeof(ret.response.routes) != 'undefined' && ret.response.routes.route){
            listRoute = ret.response.routes.route;      
            var InterfaceStatus;
            var isActiveStatus;
            if(listRoute)
            {
                if(MACRO_SUPPORT_CHAR_MIN <= listRoute.length)
                {
                    button_enable('add_item', '0');
                }
    
                if($.isArray(listRoute))
                {
                    $(listRoute).each(function(i){
                        if('' == listRoute[i].Interface){
                            isActiveStatus = label_inactive;
                        }else if((g_wanPath_value_v4 == listRoute[i].Interface) ||(ROUTE_LAN == listRoute[i].Interface)){
                            isActiveStatus = label_active;
                        }else{
                            isActiveStatus = label_inactive;
                        }
                        InterfaceStatus = getDArrayElement(InterfaceArray, listRoute[i].Interface, 'value');
                        addFilter_v4route(
                            $('#service_list tr'),
                            listRoute[i].DestIPAddress,
                            listRoute[i].DestSubnetMask,
                            listRoute[i].GatewayIPAddress,
                            InterfaceStatus,
                            isActiveStatus,
                            g_ipv4index ++
                        );
                  });
                }
                else
                {
                    if('' == listRoute.Interface){
                        isActiveStatus = label_inactive;
                    }else if((g_wanPath_value_v4== listRoute.Interface) || (ROUTE_LAN == listRoute.Interface)){
                        isActiveStatus = label_active;
                    }else{
                        isActiveStatus = label_inactive;
                    }
                    InterfaceStatus = getDArrayElement(InterfaceArray, listRoute.Interface, 'value');
                    addFilter_v4route(
                        $('#service_list tr'),
                        listRoute.DestIPAddress,
                        listRoute.DestSubnetMask,
                        listRoute.GatewayIPAddress,
                        InterfaceStatus,
                        isActiveStatus,
                        g_ipv4index ++
                   );
                }
            }
        }       
    });
      
    // get current settings gateway address
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type) 
        {
            DEFAULT_GATEWAY_IP = ret.response.DhcpIPAddress;
        }
    }, {
            sync: true
        }
    );
    $('#checkbox_staticroute_interface').attr('disabled', true);
}

function initPage_route()
{
    button_enable('apply_route', '0');
    $('.user_add_line_route').remove();
    g_ipv6index  = 0;

    getAjaxData('api/settings/ipv6staticroute',function($xml){
        var ret = xml2object($xml);
        if(typeof(ret.response.ipv6routes) != 'undefined'){
            listipv6Route = ret.response.ipv6routes.ipv6route;
            var InterfaceStatus;
            if(listipv6Route)
            {
                if(MACRO_SUPPORT_CHAR_MIN <= listipv6Route.length)
                {
                    button_enable('add_item_route', '0');
                }
    
                if($.isArray(listipv6Route))
                {
                    $(listipv6Route).each(function(i){
                    InterfaceStatus = getDArrayElement(InterfaceArray, listipv6Route[i].Interface, 'value');
                    if (InterfaceStatus != "") {
                        addFilter_route(
                            $('#service_list_route tr'),
                            listipv6Route[i].DestIPAddress,
                            listipv6Route[i].PrefixLength,
                            listipv6Route[i].GatewayIPAddress,
                            InterfaceStatus,
                            listipv6Route[i].Status=='Active'?label_active:label_inactive,
                            g_ipv6index
                        );
                        g_ipv6index ++;
                    }
                });
                }
                else
                {
                    InterfaceStatus = getDArrayElement(InterfaceArray, listipv6Route.Interface, 'value');
                    if (InterfaceStatus != "") {
                        addFilter_route(
                            $('#service_list_route tr'),
                            listipv6Route.DestIPAddress,
                            listipv6Route.PrefixLength,
                            listipv6Route.GatewayIPAddress,
                            InterfaceStatus,
                            listipv6Route.Status=='Active'?label_active:label_inactive,
                            0
                        );
                        g_ipv6index ++;
                    }
                }
            }
            
        } 
       });
     $('#checkbox_ipv6staticroute_interface').attr('disabled', true);

}

function isValidIpAddress(address){
    var addrParts = address.split('.');
    if (addrParts.length != 4) 
    {
        return false;
    }
    
    for (i = 0; i < 4; i++) 
    {
        if (isNaN(addrParts[i]) == true)
        {
            return false;
        }
        
        if (addrParts[i] == '') 
        {
            return false;
        }
        
        if (addrParts[i].indexOf(' ') != -1)
        {
            return false;
        }
        
        if ((addrParts[i].indexOf('0') == 0) && (addrParts[i].length != 1)) 
        {
            return false;
        }
        
       if (addrParts[i].charAt(0) == '+' ||addrParts[i].charAt(0) == '-')
        {
           return false;
        }
    }
    
    if ((addrParts[0] <= 0 || addrParts[0] == 127 || addrParts[0] > 223) ||
    (addrParts[1] < 0 || addrParts[1] > 255) ||
    (addrParts[2] < 0 || addrParts[2] > 255) ||
    (addrParts[3] < 0 || addrParts[3] >= 255)) 
    {
        return false;
    }
    
    return true;
}
        
function getMostRightPosOf1(str)
{
    for (i = str.length - 1; i >= 0; i--)
    {
        if (str.charAt(i) == '1')
        {
            break;
        }
    }
    return i;
}
        
function getBinaryString(str)
{
    var numArr = [128, 64, 32, 16, 8, 4, 2, 1];
    var addrParts = str.split('.');
    if (addrParts.length < 3)
    {
        return "00000000000000000000000000000000";
    }
    var binstr = '';
    for (i = 0; i < 4; i++)
    {
        var num = parseInt(addrParts[i])
        
        for ( j = 0; j < numArr.length; j++ )
        {
            if ( (num & numArr[j]) != 0 )
            {
                binstr += '1';
            }
            else
            {
                binstr += '0';
            }    
        }
    }
    return binstr;
}
        
function isMatchedIpAndMask(ip, mask)
{
    var locIp = getBinaryString(ip);
    var locMask = getBinaryString(mask);
    
    if (locIp.length != locMask.length)
    {
        return false;
    } 
    var most_right_pos_1 = getMostRightPosOf1(locMask);
    
    if (most_right_pos_1 == -1)
    {
        if (loaIp == '00000000000000000000000000000000')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
       
    for (var i = most_right_pos_1 + 1; i < locIp.length; i++)
    {
        if (locIp.charAt(i) != '0')
        {
            return false;
        }
    }
    
    return true;
}
        
function isVaildValue()
{
    var i = 0;
    var index1 = getValue('input_destination_network_address').lastIndexOf('.');
    var index2 = listDhcp.DhcpIPAddress.lastIndexOf('.');
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
   
    if(!isValidIpAddress(getValue('input_destination_network_address')))
    {
        showQtip('input_destination_network_address', staticroute_hint_destinationNetworkAddress_invalid);
        return false;
    }
        
    if((listDhcp.DhcpIPAddress == getValue('input_destination_network_address')) ||
        ('0' == getValue('input_destination_network_address').substr(index1 + 1) && 
        listDhcp.DhcpIPAddress.substr(0,index1) == getValue('input_destination_network_address').substr(0,index2)))
    {
        showQtip('input_destination_network_address', staticroute_hint_destinationNetworkAddress_invalid);
        return false;
    }

    if(!isValidSubnetMask(getValue('input_subnetmask')) && getValue('input_subnetmask') != '255.255.255.255')
    {
        showQtip('input_subnetmask', staticroute_hint_subnetMask_invalid);
        return false;
    }

    if((getValue('input_destination_network_address') == getValue('input_subnetmask')) ||
        (!isMatchedIpAndMask(getValue('input_destination_network_address'), getValue('input_subnetmask'))))
    {
        showQtip('input_subnetmask', staticroute_hint_addrAddSMask_invalid);
        return false;
    }

    if(("checked" != $('#checkbox_staticroute_interface').attr('checked')) && ("checked" != $('#checkbox_staticroute_default').attr('checked')))
    {
        showQtip('checkbox_staticroute_interface', staticroute_hint_checkbox_alldisable);
        return false;
    }

    if("checked" == $('#checkbox_staticroute_default').attr('checked'))
    {
        if(!(isValidIpAddress(getValue('input_staticroute_default'))))
        {
            showQtip('input_staticroute_default', staticroute_hint_default_invalid);
            return false;
        }
        if(ROUTE_LAN == $('#select_staticroute_interface').val())
        {
            var gateway_address = getValue('input_staticroute_default');
            var matchString = DEFAULT_GATEWAY_IP.substring(0, DEFAULT_GATEWAY_IP.lastIndexOf("."));
            var compareString = gateway_address.substring(0, gateway_address.lastIndexOf("."));
            if(matchString != compareString)
            {
                showQtip('input_staticroute_default', staticroute_hint_default_invalid_LAN);
                return false;
            }  
        }
    }

    if(("checked" == $('#checkbox_staticroute_interface').attr('checked')))
    {
        if((ROUTE_LAN !=$('#select_staticroute_interface').val()) && 
            (MACRO_CONNECTION_CONNECTED != parent.G_MonitoringStatus.response.ConnectionStatus) &&
            (MACRO_WIRELINE_CONNECTION_CONNECTED != parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus))
        {
            showQtip('select_staticroute_interface', staticroute_hint_waninterface_invalid);
            return false;
        }
        if((ROUTE_LAN !=$('#select_staticroute_interface').val()) && 
            (g_wanPath_value_v4 != $('#select_staticroute_interface').val()))
        {
            showQtip('select_staticroute_interface', staticroute_hint_waninterface_invalid);
            return false;
        }
    }
    return true;

}

function isVaildValue_route()
{
    var i = 0;
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
   
    if(!(verifyIpv6str(getValue('input_destination_network_address_ipv6'))) || 
         !(isValidIpv6Address( getValue('input_destination_network_address_ipv6'))) ||
         !(checkInvalidDestinationIpv6Addr(getValue('input_destination_network_address_ipv6'))))
    {
        showQtip('input_destination_network_address_ipv6', staticroute_hint_destinationNetworkAddress_invalid);
        return false;
    }

    if(!checkPrefixLenNum('input_prefix_length_ipv6')){
        showQtip('input_prefix_length_ipv6', hint_ipv6_prefix_length_invalid);
        return false;
    }

    if(("checked" != $('#checkbox_ipv6staticroute_interface').attr('checked')) && ("checked" != $('#checkbox_ipv6staticroute_default').attr('checked')))
    {
        showQtip('checkbox_ipv6staticroute_interface', staticroute_hint_checkbox_alldisable);
        return false;
    }
    
    if("checked" == $('#checkbox_ipv6staticroute_default').attr('checked'))
    {
        if(!(isValidIpv6Address(getValue('input_staticroute_default_ipv6'))) || 
        !(verifyIpv6str(getValue('input_staticroute_default_ipv6'))))
        {
            showQtip('input_staticroute_default_ipv6', staticroute_hint_default_invalid);
            return false;
        }
        if(ROUTE_LAN == $('#select_staticroute_interface_ipv6').val())
        {
            var gateway_address_ipv6 = getValue('input_staticroute_default_ipv6');
            var matchString = "fe80::";
            if ((gateway_address_ipv6.toLowerCase()).indexOf(matchString)!=0)
            {
                showQtip('input_staticroute_default_ipv6', staticroute_hint_default_invalid_LAN);
                return false;
            }  
        }
    }

    if(("checked" == $('#checkbox_ipv6staticroute_interface').attr('checked')))
    {
        if((ROUTE_LAN !=$('#select_staticroute_interface_ipv6').val()) && 
            (MACRO_CONNECTION_IPV6CONNECTED != parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus) &&
            (MACRO_WIRELINE_CONNECTION_IPV6CONNECTED != parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus))
        {
            showQtip('select_staticroute_interface_ipv6', staticroute_hint_waninterface_invalid);
            return false;
        }
        if((ROUTE_LAN !=$('#select_staticroute_interface_ipv6').val()) && 
            (g_wanPath_value_v6 != $('#select_staticroute_interface_ipv6').val()))
        {
            showQtip('select_staticroute_interface_ipv6', staticroute_hint_waninterface_invalid);
            return false;
        }
    }

    return true;

}
function isSameRule()
{      
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    var network_address = getValue('input_destination_network_address');
    var issameRule = false;
    $('.user_add_line').each(function(i) {
        if($(this).children().eq(0).text()==network_address)
        {
            issameRule=true;
            showQtip('input_destination_network_address', staticroute_hint_destinationNetworkAddress_empty);
        }
    });
    return issameRule;
}

function isSameRule_route()
{
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    var network_address = getValue('input_destination_network_address_ipv6');
    var issameRule = false;
    $('.user_add_line_route').each(function(i) {
        if($(this).children().eq(0).text()==network_address)
        {
            issameRule=true;
            showQtip('input_destination_network_address_ipv6', staticroute_hint_destinationNetworkAddress_empty);
        }
    });
    return issameRule;
}
function isSame_ipv6prefix_route()
{
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    var dnetworkAddress_route = $.trim($('#input_destination_network_address_ipv6').val());
    var PerfixLength = $.trim($('#input_prefix_length_ipv6').val());
    var issameprefix = false;
    $('.user_add_line_route').each(function(i) {
        if(checkSameipv6Prefix(dnetworkAddress_route,$(this).children().eq(0).text(),PerfixLength,$(this).children().eq(1).text()))
        {
            issameprefix = true;
            showQtip('input_destination_network_address_ipv6', input_destination_network_address_ipv6_prefix_check);
        }
    });
    return issameprefix;
}
function checkSameipv6Prefix(ipv61,ipv62,Prefix1,Prefix2)
{
    if (Prefix1!=Prefix2)
    {
        return false;
    }
    if(ipv61==''||ipv62=='')
    {
        return false;
    }
    if(Prefix1<=0||Prefix1>128)
    {
        return false;
    } 
    return isSameipv6Prefix(ipv61,ipv62,Prefix1);
}

function LoadStaticCheckbox()
{
    var interface_value = $('#select_staticroute_interface').val();
    if(ROUTE_LAN != interface_value)
    {
        $('#checkbox_staticroute_default').attr('checked', false);
        $('#input_staticroute_default').attr('disabled', true);
        $('#input_staticroute_default').attr('value', '');
    }
    else
    {
        $('#checkbox_staticroute_default').attr('checked', true);
        $('#input_staticroute_default').attr('disabled', false);
    }

    $('#checkbox_staticroute_default').attr('disabled', true);
    if("" == $('#input_staticroute_default').val())
    {
        $('#input_staticroute_default').attr('value', '');
    }
}

function LoadIPv6StaticCheckbox()
{
    var interface_value = $('#select_staticroute_interface_ipv6').val();
    if(ROUTE_LAN != interface_value)
    {
        $('#checkbox_ipv6staticroute_default').attr('checked', false);
        $('#input_staticroute_default_ipv6').attr('disabled', true);
        $('#input_staticroute_default_ipv6').attr('value', '');
    }
    else
    {
        $('#checkbox_ipv6staticroute_default').attr('checked', true);
        $('#input_staticroute_default_ipv6').attr('disabled', false);
    }
    $('#checkbox_ipv6staticroute_default').attr('disabled', true);
    if("" == $('#input_staticroute_default_ipv6').val()) {
        $('#input_staticroute_default_ipv6').attr('value', '');
    }
}

$(document).ready(function(){
    initPage();
    StaticRoute_ready();
    LoadStaticCheckbox();
    
    if (parent.g_feature_ipv6)
    {
        initPage_route();
        ipv6StaticRoute_ready();
        LoadIPv6StaticCheckbox();
        $('#staticroute_ipv6_title').show();
        $('#staticroute_ipv6_content').show();
    }
    else {
        $('#staticroute_ipv6_title').hide();
        $('#staticroute_ipv6_content').hide();
    }
});

function StaticRoute_ready()
{
    var currentAllVal = null;
    var editInterface = null;
    $('.button_edit_list').live('click', function() {
        if (($(".add_item_control:hidden").size() > 0) && ($('#edit_item_ok').size() < 1)) {
            editIndex = $('.button_edit_list').index(this);
            currentAllVal = $('.user_add_line').eq(editIndex).html();
            var editStaticroute = $(this).parent().siblings();
            var editSrouteDNaddr = editStaticroute.eq(0);
            var editSrouteSubnetMask = editStaticroute.eq(1);
            var editSrouteDgateway = editStaticroute.eq(2);
            editInterface = editStaticroute.eq(3);

            var htmlInterface = editInterface.html();

            editSrouteDNaddr.html('<input type="text" value="' + editSrouteDNaddr.html() + '" id="input_destination_network_address" maxlength="15" /></td>');
            editSrouteSubnetMask.html('<input type="text" value="' + editSrouteSubnetMask.html() + '" id="input_subnetmask" maxlength="15"></td>');
            editSrouteDgateway.html('<input type="text" value="' + editSrouteDgateway.html() + '" id="input_staticroute_default" maxlength="15"></td>');

            if(g_connection_device == "2")
            {
                createSelect(editInterface, 'select_staticroute_interface', InterfaceArray);
                $('#select_staticroute_interface').val(getDArrayElement(InterfaceArray, htmlInterface, 'key'));
            }else{
                createSelect(editInterface, 'select_staticroute_interface', InterfaceArray);
                $('#select_staticroute_interface').val(getDArrayElement(InterfaceArray, htmlInterface, 'key'));
            }

           

            $(this).parent().html('<a id="edit_item_ok" href="javascript:void(0);">' + common_ok +
            '</a>&nbsp;&nbsp;<a id="edit_item_cancel" href="javascript:void(0);">' + common_cancel + '</a>');
            LoadStaticCheckbox();
            hideAddItemControl();

            $('.user_add_line input').eq(0).focus();

            button_enable('apply', '0');
            button_enable('add_item', '0');
            $('#checkbox_staticroute_interface').attr('disabled', false);
            }
        });


    $('#edit_item_ok').live('click', function() {
            if (isVaildValue()&&!isSameRule()) {
                var edit_dnetworkAddress = $.trim($('#input_destination_network_address').val());
                var edit_SubnetMask = $.trim($('#input_subnetmask').val());
                var edit_defaultGetway = $.trim($('#input_staticroute_default').val());
                var edit_interfaceOption = $('#select_staticroute_interface option:selected').text();

                hideAddItemControl();
                var editStaticRoute = $(this).parent().siblings();
                editStaticRoute.eq(0).html(edit_dnetworkAddress);
                editStaticRoute.eq(1).html(edit_SubnetMask);
                editStaticRoute.eq(2).html(edit_defaultGetway);
                editStaticRoute.eq(3).html(edit_interfaceOption);

                $(this).parent().html('<span class=\"button_edit_list\">' + common_edit +
                '</span>&nbsp;&nbsp;<span class=\"button_delete_list\">' + common_delete + '</span>');

                currentAllVal = $('.user_add_line').eq(editIndex).html();
                button_enable('apply', '1');
                button_enable('add_item', '1');
                ok_flag = 1;
                if ($('.user_add_line').length >= 16)
                {
                button_enable('add_item', '0');
                }
                $('#checkbox_staticroute_interface').attr('disabled', true);
            }
        });
        
    $('#edit_item_cancel').live('click', function() {
        $('.user_add_line').eq(editIndex).html(currentAllVal);
        $('.qtip').qtip('destroy');
        if (!isButtonEnable('add_item'))
        {
        button_enable('add_item', '1');
        button_enable('apply', '1');
        if(1 == ok_flag)
        {
          ok_flag = 0;
        }
        }
        $('#checkbox_staticroute_interface').attr('disabled', true);
    });

    $('#add_item_cancel').live('click', function() {
        if(0 < $('.user_add_line').length)
        {
            button_enable('apply', '1');
        }
        $('#checkbox_staticroute_interface').attr('disabled', true);
        hideAddItemControl();
        return false;
    });

    $('#add_item_ok').live('click', function() {
        if (isVaildValue()&&!isSameRule())
        {
            var add_dnetworkAddress = $.trim($('#input_destination_network_address').val());
            var add_SubnetMask = $.trim($('#input_subnetmask').val());
            var add_defaultGetway = $.trim($('#input_staticroute_default').val());
            var add_interfaceOption = $('#select_staticroute_interface option:selected').text();

            hideAddItemControl();

            addFilter_v4route(
                $('#service_list tr'), 
                add_dnetworkAddress, 
                add_SubnetMask, 
                add_defaultGetway, 
                add_interfaceOption,
                '',
                g_ipv4index ++
            );

            button_enable('apply', '1');

            if (MACRO_SUPPORT_CHAR_MIN <= $('.user_add_line').length)
            {
                button_enable('add_item', '0');
            }
            $('#checkbox_staticroute_interface').attr('disabled', true);
        }
        return false;
    });

    $('#add_item').click(function() {
        if (isButtonEnable('add_item'))
        {
            LoadStaticCheckbox();
            showAddItemControl();
            $('.add_item_control input').eq(0).focus();
            button_enable('apply', '0');
            $('#checkbox_staticroute_interface').attr('disabled', false);
        }
    });

    $('#apply').click(function() {
        if (isButtonEnable('apply')) {
            parent.showConfirmDialog(firewall_hint_submit_list_item, postData);
        }        
    });
    if(g_connection_device == "2")
    {
        initSelectOption('select_staticroute_interface', InterfaceArray);
    }else{
        initSelectOption('select_staticroute_interface', InterfaceArray);
    }
    
    $('#checkbox_staticroute_interface').attr('checked', true);
    $('#checkbox_staticroute_default').attr('checked', false);
    $('#checkbox_staticroute_default').attr('disabled', true);


    $('#select_staticroute_interface').live('change',function(){
        LoadStaticCheckbox();
    });

}

function ipv6StaticRoute_ready()
{
    var currentAllVal_route = null;
    var editInterface_route = null;
    $('.button_edit_list_route').live('click', function() {
        if (($(".add_item_control_route:hidden").size() > 0) && ($('#edit_item_ok_route').size() < 1)) {
            editIndex_route = $('.button_edit_list_route').index(this);
            currentAllVal_route = $('.user_add_line_route').eq(editIndex_route).html();
            var editStaticroute_route = $(this).parent().siblings();
            var editSrouteDNaddr_route = editStaticroute_route.eq(0);
            var editSroutePerfixLength = editStaticroute_route.eq(1);
            var editSrouteDgateway_route = editStaticroute_route.eq(2);
            editInterface_route = editStaticroute_route.eq(3);

            var htmlInterface_route = editInterface_route.html();

            editSrouteDNaddr_route.html('<input type="text" value="' + editSrouteDNaddr_route.html() + '" id="input_destination_network_address_ipv6" maxlength="39"/></td>');
            editSroutePerfixLength.html('<input type="text" value="' + editSroutePerfixLength.html() + '" id="input_prefix_length_ipv6" maxlength="3" /></td>');
            editSrouteDgateway_route.html('<input type="text" value="' + editSrouteDgateway_route.html() + '" id="input_staticroute_default_ipv6" maxlength="39" /></td>');

            if(g_connection_device=="2")
            {
                createSelect(editInterface_route, 'select_staticroute_interface_ipv6', InterfaceArray);
                $('#select_staticroute_interface_ipv6').val(getDArrayElement(InterfaceArray, htmlInterface_route, 'key'));
            }else{
                createSelect(editInterface_route, 'select_staticroute_interface_ipv6', InterfaceArray);
                $('#select_staticroute_interface_ipv6').val(getDArrayElement(InterfaceArray, htmlInterface_route, 'key'));
            }
            

            $(this).parent().html('<a id="edit_item_ok_route" href="javascript:void(0);">' + common_ok +
            '</a>&nbsp;&nbsp;<a id="edit_item_cancel_route" href="javascript:void(0);">' + common_cancel + '</a>');
            LoadIPv6StaticCheckbox();
            hideAddItemControl_route();

            $('.user_add_line_route input').eq(0).focus();

            button_enable('apply_route', '0');
            button_enable('add_item_route', '0');
            $('#checkbox_ipv6staticroute_interface').attr('disabled', false);
        }
    });
    
    $('#edit_item_ok_route').live('click', function() {
        if (isVaildValue_route()&&!isSameRule_route()&&!isSame_ipv6prefix_route()) {
            var edit_dnetworkAddress_route = $.trim($('#input_destination_network_address_ipv6').val());
            var edit_PerfixLength = $.trim($('#input_prefix_length_ipv6').val());
            var edit_defaultGetway_route = $.trim($('#input_staticroute_default_ipv6').val());
            var edit_interfaceOption_route = $('#select_staticroute_interface_ipv6 option:selected').text();

            hideAddItemControl_route();
            var editStaticRoute_route = $(this).parent().siblings();
            editStaticRoute_route.eq(0).html(edit_dnetworkAddress_route);
            editStaticRoute_route.eq(1).html(edit_PerfixLength);
            editStaticRoute_route.eq(2).html(edit_defaultGetway_route);
            editStaticRoute_route.eq(3).html(edit_interfaceOption_route);

            $(this).parent().html('<span class=\"button_edit_list_route\">' + common_edit +
            '</span>&nbsp;&nbsp;<span class=\"button_delete_list_route\">' + common_delete + '</span>');

            currentAllVal_route = $('.user_add_line_route').eq(editIndex_route).html();
            button_enable('apply_route', '1');
            button_enable('add_item_route', '1');
            ok_flag_route = 1;
            if (MACRO_SUPPORT_CHAR_MIN <= $('.user_add_line_route').length)
            {
                button_enable('add_item_route', '0');
            }
            $('#checkbox_ipv6staticroute_interface').attr('disabled', true);
        }
    });


    $('#edit_item_cancel_route').live('click', function() {
        $('.user_add_line_route').eq(editIndex_route).html(currentAllVal_route);
        $('.qtip').qtip('destroy');
        if (!isButtonEnable('add_item_route'))
        {
            button_enable('add_item_route', '1');
            button_enable('apply_route', '1');
            if(1 == ok_flag_route)
            {
                ok_flag_route= 0;
            }
        }
        $('#checkbox_ipv6staticroute_interface').attr('disabled', true);
    });

    $('#add_item_cancel_route').live('click', function() {
        if(0 < $('.user_add_line_route').length)
        {
            button_enable('apply_route', '1');
        }
        $('#checkbox_ipv6staticroute_interface').attr('disabled', true);
        hideAddItemControl_route();
        return false;
    });

    $('#add_item_ok_route').live('click', function() {
        if (isVaildValue_route()&&!isSameRule_route()&&!isSame_ipv6prefix_route())
        {
            var add_dnetworkAddress_route = $.trim($('#input_destination_network_address_ipv6').val());
            var add_PerfixLength = $.trim($('#input_prefix_length_ipv6').val());
            var add_defaultGetway_route = $.trim($('#input_staticroute_default_ipv6').val());
            var add_interfaceOption_route = $('#select_staticroute_interface_ipv6 option:selected').text();

            hideAddItemControl_route();

            addFilter_route(
            $('#service_list_route tr'), 
                add_dnetworkAddress_route, 
                add_PerfixLength, 
                add_defaultGetway_route, 
                add_interfaceOption_route,
                '',
                g_ipv6index ++
            );

            button_enable('apply_route', '1');

            if (MACRO_SUPPORT_CHAR_MIN <= $('.user_add_line_route').length)
            {
                button_enable('add_item_route', '0');
            }
            $('#checkbox_ipv6staticroute_interface').attr('disabled', true);
        }
        return false;
    });

    $('#add_item_route').click(function() {
        if (isButtonEnable('add_item_route'))
        {
            LoadIPv6StaticCheckbox();
            showAddItemControl_route();
            $('.add_item_control_route input').eq(0).focus();
            button_enable('apply_route', '0');
            $('#checkbox_ipv6staticroute_interface').attr('disabled', false);
        }
    });

    $('#apply_route').click(function() {
        if (isButtonEnable('apply_route')) {
            parent.showConfirmDialog(firewall_hint_submit_list_item, postData_route);
        }
    });
    if(g_connection_device == "2")
    {
        initSelectOption('select_staticroute_interface_ipv6', InterfaceArray);
    }else{
        initSelectOption('select_staticroute_interface_ipv6', InterfaceArray);
    }
    $('#checkbox_ipv6staticroute_interface').attr('checked', true);
    $('#checkbox_ipv6staticroute_default').attr('checked', false);
    $('#checkbox_ipv6staticroute_default').attr('disabled', true);


    $('#select_staticroute_interface_ipv6').live('change',function(){
        LoadIPv6StaticCheckbox();
    });

}

function postData() {
    var submitObject = {};
    var StaticRouteArray = [];
    var srInterfaceArray = g_connection_device == "2" ? InterfaceArray : InterfaceArray; 

    $('.user_add_line').each(function(i) {
        var dnetworkAddress = $(this).children().eq(0).text();
        var subnetMask = $(this).children().eq(1).text();
        var defaultGetway = $(this).children().eq(2).text();
        var interfaceOption = getDArrayElement(srInterfaceArray, $(this).children().eq(3).text(), 'key');;
        
        var SRoutes = {
            DestIPAddress: dnetworkAddress,
            DestSubnetMask: subnetMask,
            GatewayIPAddress: defaultGetway,
            Interface: interfaceOption,
            Enable:1
        };
        StaticRouteArray.push(SRoutes);
    });

    submitObject = {
        routes: {
            route: StaticRouteArray
        }
    };
    var submitData = object2xml('request', submitObject);

    saveAjaxData('api/settings/staticroute', submitData, function($xml) {
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

function postData_route() {
    var submitObject = {};
    var ipv6StaticRouteArray = [];
    var ipv6SrInterfaceArray = g_connection_device == "2" ? InterfaceArray : InterfaceArray;

    $('.user_add_line_route').each(function(i) {
        var dnetworkAddress_route = $(this).children().eq(0).text();
        var perfixLength = $(this).children().eq(1).text();
        var defaultGetway_route = $(this).children().eq(2).text();
        var interfaceOption_route = getDArrayElement(ipv6SrInterfaceArray, $(this).children().eq(3).text(), 'key');

        var ipv6SRoutes = {
            DestIPAddress: dnetworkAddress_route,
            PrefixLength: perfixLength,
            GatewayIPAddress: defaultGetway_route,
            Interface: interfaceOption_route,
            Enable:1
        };
        ipv6StaticRouteArray.push(ipv6SRoutes);
    });

    submitObject = {
        ipv6routes: {
            ipv6route: ipv6StaticRouteArray
        }
    };
    var submitData = object2xml('request', submitObject);

    saveAjaxData('api/settings/ipv6staticroute', submitData, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            button_enable('apply_route', '0');
        }
        else {
            parent.showInfoDialog(common_failed);
        }
        initPage_route();
    });
}


