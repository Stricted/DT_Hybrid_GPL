var wan_PPP_Routed = "PPP_Routed";
var wan_IP_Routed = "IP_Routed";
var IP_TYPE_IPV4 = "ipv4";
var IP_TYPE_IPV6 = "ipv6"
var IP_TYPE_IPV4IPV6 = "ipv4ipv6";
var PPP_AUTHMODE_AUTO = 'AUTO';
var PPP_AUTHMODE_PAP = 'PAP';
var PPP_AUTHMODE_CHAP = 'CHAP';
var PPP_DIALUPMODE_AUTO = 'AlwaysOn';
var PPP_DIALUPMODE_MANUAL = 'Manual';
var PPP_DIALUPMODE_ONDEMAND = 'OnDemand';
var MIN_IDLE_TIME = 30;
var MAX_IDLE_TIME = 7200;
var IPADDRESSTYPE_DHCP = 'DHCP';
var IPADDRESSTYPE_STATIC = 'Static';
var IPADDRESSTYPE_SLAAC = 'SLAAC';
var connectionType = [0,1];
var IP_TYPE_DISABLE = 0;
var IP_TYPE_ENABLE = 1;
var connectionTypeText = [wan_PPP_Routed_text,wan_IP_Routed_text];
var IPType = [IP_TYPE_IPV4,IP_TYPE_IPV6,IP_TYPE_IPV4IPV6];
var TPTypeText = [wan_ipv4_text,wan_ipv6_text,wan_ipv4_ipv6_text];
var IPv4addressType = [IPADDRESSTYPE_DHCP,IPADDRESSTYPE_STATIC];
var IPv4addressTypeText = [dhcp_label_dhcp,dialup_label_static];
var IPv6addressType = [IPADDRESSTYPE_DHCP,IPADDRESSTYPE_SLAAC,IPADDRESSTYPE_STATIC];
var IPv6addressTypeText = [dhcp_label_dhcp,slaac_label_slaac,dialup_label_static];
var authenticationMode = [PPP_AUTHMODE_AUTO,PPP_AUTHMODE_PAP,PPP_AUTHMODE_CHAP];
var authenticationText = [common_auto,dialup_label_pap,dialup_label_chap];
var pppdialupMode = [PPP_DIALUPMODE_AUTO,PPP_DIALUPMODE_MANUAL,PPP_DIALUPMODE_ONDEMAND];
var pppdialupText = [common_auto,common_manual,label_dailup_option_on_demand];
var g_eth_array = [];
var g_eth_name_array = [];
var g_eth_index_array = [];
var g_wan_default_index = 0;
var g_wan_array_index = 0;
var g_wan_currentProfileNmae = "";
var g_wan_currentConnType = 0;
var g_wan_currentMTU = "";
var g_editIndex = 0;
var WAN_MAX_NUM = 10;
var DEFAULT_GATEWAY_IP = '192.168.1.1';
var DEFAULT_GATEWAY_IPv6Address = '';
var g_addhtml = "<table id='show_dialog_table' class='dialog_table_style'>"+
                    "<tr height='30' valign='top'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+dialup_label_profile_name+common_colon+"</td>"+
                        "<td width='142'><input type='text' id='eth_name_dialog' class='input_style' maxlength='64'/></td>"+
                        "<td width='50'></td>"+
                        "<td width='150'><span id='label_eth_mtuormru'></span>"+common_colon+"</td>"+
                        "<td colspan='1'><input id='wan_size_dialog' type='text' class='input_style' maxlength='4'></td>"+
                        "<td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_connection_type+common_colon+"</td>"+
                        "<td width='142'><select id='connection_type_dialog' class='input_select'></select></td>"+
                        "<td width='50'></td>"+
                        "<td width='150' style='display:none;' class='css_authentication_mode'>"+set_quick_authentication_mode+common_colon+"</td>"+
                        "<td colspan='1' style='display:none;' class='css_authentication_mode'><select id='authentication_mode_dialog' class='input_select'></select></td><td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='ipv4_type_dialog_tr'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+connectiontatus_label_iptype+common_colon+"</td>"+
                        "<td colspan='1'><select id='ip_type_dialog' class='input_select'></select></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='ppp_dialupmode_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+dialup_label_connection_mode+common_colon+"</td>"+
                        "<td width='142'><select id='ppp_dialupmode' class='input_select'></select></td>"+
                        "<td width='50'></td>"+
                        "<td width='150' style='display:none;' class='css_maxideltime'>"+dialup_label_max_idle_time_s+common_colon+"</td>"+
                        "<td colspan='1' style='display:none;' class='css_maxideltime'><input type='text' id='ppp_maxideltime' value='300' class='input_style'maxlength='8' /></td><td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='ipv4_address_type_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_ipv4_address_type+common_colon+"</td>"+
                        "<td colspan='1'><select id='ipv4_address_type_dialog' class='input_select'></select></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='ipv4_address_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_ipv4_address+common_colon+"</td>"+
                        "<td width='142'><input id='ipv4_address_dialog' class='input_style' type='text' maxlength='15'/></td>"+
                        "<td width='50'></td>"+
                        "<td width='150'>"+dhcp_label_subnet_mask+common_colon+"</td>"+
                        "<td colspan='1'><input id='subnet_mask_dialog' class='input_style' type='text' maxlength='15'/></td><td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='default_gateway_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wanstatus_label_defaultgateway+common_colon+"</td>"+
                        "<td width='142'><input id='default_gateway_dialog' class='input_style' type='text' maxlength='15'/></td>"+
                        "<td width='50'></td>"+
                        "<td width='150'>"+wan_primary_DNS+common_colon+"</td>"+
                        "<td colspan='1'><input id='primary_DNS_dialog' class='input_style' type='text' maxlength='15'/></td><td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='secondary_DNS_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_secondary_DNS+common_colon+"</td>"+
                        "<td colspan='1'><input id='secondary_DNS_dialog' class='input_style' type='text' maxlength='15'/></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' class='wan_ipv6' id='ipv6_address_type_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_ipv6_address_type+common_colon+"</td>"+
                        "<td colspan='1'><select id='ipv6_address_type_dialog' class='input_select'></select></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' class='wan_ipv6' id='ipv6_address_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_ipv6_address+common_colon+"</td>"+
                        "<td width='142'><input id='ipv6_address_dialog' class='input_style' type='text' maxlength='64'/></td>"+
                        "<td width='50'>/<input id='ipv6_IdIpv6PrefixLen_dialog' class='input_style_small' type='text' maxlength='3'/></td>"+
                        "<td width='150'>"+wan_ipv6_default_gateway+common_colon+"</td>"+
                        "<td width='142'><input id='ipv6_default_gateway_dialog' class='input_style' type='text' maxlength='64'/></td>"+
                        "<td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' class='wan_ipv6' id='ipv6_primary_DNS_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+wan_ipv6_primary_DNS+common_colon+"</td>"+
                        "<td width='142'><input id='ipv6_primary_DNS_dialog' class='input_style' type='text' maxlength='64'/></td>"+
                        "<td width='50'></td>"+
                        "<td width='150'>"+wan_ipv6_secondary_DNS+common_colon+"</td>"+
                        "<td colspan='1'><input id='ipv6_secondary_DNS_dialog' class='input_style' type='text' maxlength='64'/></td><td width='20'></td>"+
                    "</tr>"+
                    "<tr height='30' valign='top' id='user_name_dialog_tr' style='display: none;'>"+
                        "<td width='20'></td>"+
                        "<td width='160'>"+dialup_label_user_name+common_colon+"</td>"+
                        "<td width='142'><input id='user_name_dialog' class='input_style' type='text' maxlength='64'/></td>"+
                        "<td width='50'></td>"+
                        "<td width='150'>"+common_password+common_colon+"</td>"+
                        "<td colspan='1'><input id='password_dialog' class='input_style' type='password' maxlength='64'/></td><td width='20'></td>"+
                    "</tr>"+
                "</table>";
function isLanipv4(ipv4)
{
    var matchString = DEFAULT_GATEWAY_IP.substring(0, DEFAULT_GATEWAY_IP.lastIndexOf("."));
    var matchString1 = ipv4.substring(0, ipv4.lastIndexOf("."));
    if(matchString == matchString1)
    {
       return true;
    } 
    return false;
}
function checkIPv4(id){
    if(parent.$("#"+id).val()==''){
        showErrorUnderTextbox_Parent(id, samba_input_empty);
        parent.$("#"+id).focus();
        return false;
    } else {
        if(!isValidIpAddress(parent.$("#"+id).val())){
            showErrorUnderTextbox_Parent(id, samba_input_invalid);
            parent.$("#"+id).focus();
            return false;
        }
        if(isLanipv4(parent.$("#"+id).val()))
        {
            showErrorUnderTextbox_Parent(id, label_not_input_LAN_address);
            parent.$("#"+id).focus();
            return false;
        }   
    }
    return true;
}
function checkIPv4SubnetMask(id){
    if(parent.$("#" + id).val() == ''){
        showErrorUnderTextbox_Parent(id, samba_input_empty);
        parent.$("#" + id).focus();
        return false;
    } else {
        if(!isValidSubnetMask(parent.$("#" + id).val())){
            showErrorUnderTextbox_Parent(id, samba_input_invalid);
            parent.$("#" + id).focus();
            return false;
        }
    }
    return true;
}

function isLanipv6(ipv6)
{ 
    var Prefix = parent.$('#ipv6_IdIpv6PrefixLen_dialog').val();   
    if(Prefix<=0||Prefix>128||isNaN(Prefix))
    {
        return false;
    }
    //fd00:1020:3040:5000::1/64,fe80::1/64
    if(DEFAULT_GATEWAY_IPv6Address!='')
    {
        var IPv6AddressParts = DEFAULT_GATEWAY_IPv6Address.split(',');
        var i = 0;
        for(i=0;i<IPv6AddressParts.length;i++)
        {
            var matchString = IPv6AddressParts[i].substring(0, IPv6AddressParts[i].lastIndexOf("/"));
            if(isSameipv6Prefix(matchString,ipv6,Prefix))
            {
                return true;
            }                     
        }
    }        
    return false;
}
function checkIPv6(id){
    if(parent.$("#" + id).val() == ''){
        showErrorUnderTextbox_Parent(id, samba_input_empty);
        parent.$("#" + id).focus();
        return false;
    } 
    else {
        if(!isValidIpv6Address(parent.$("#" + id).val())){
            showErrorUnderTextbox_Parent(id, samba_input_invalid);
            parent.$("#" + id).focus();
            return false;
        }
        if(isLanipv6(parent.$("#" + id).val()))
        {
            showErrorUnderTextbox_Parent(id, label_not_input_LAN_address);
            parent.$("#" + id).focus();
            return false;
        }
    }
    return true;
}
function isVaildVirtual(){
    parent.clearAllErrorLabel();
    var ProfileName = parent.$("#eth_name_dialog").val();
    if(parent.$(".dialog_header_left").text() == common_edit){
        if(g_wan_currentProfileNmae != ProfileName){
            if(checkRepeatedlyProfileName(g_eth_array,ProfileName)){
                showErrorUnderTextbox_Parent('eth_name_dialog', dialup_hint_profile_name_has_exist);
                parent.$('#eth_name_dialog').focus();
                return false;
            }
        }
    }else{
            if(checkRepeatedlyProfileName(g_eth_array,ProfileName)){
                showErrorUnderTextbox_Parent('eth_name_dialog', dialup_hint_profile_name_has_exist);
                parent.$('#eth_name_dialog').focus();
                return false;
            }
    }
    
    var Size = parent.$('#wan_size_dialog').val();
    if(!checkNameOrPassword('eth_name_dialog')){
        return false;
    }
    if(''==Size){
        showErrorUnderTextbox_Parent('wan_size_dialog', samba_input_empty);
        parent.$('#wan_size_dialog').focus();
        return false;
    }else{
        if(true == isNaN(Size)){
            showErrorUnderTextbox_Parent('wan_size_dialog', samba_input_invalid);
            parent.$('#wan_size_dialog').focus();
            return false;
        }
        if((1280>Size || 1492<Size)&&(connectionType[0] == parent.$('#connection_type_dialog').val())){
            showErrorUnderTextbox_Parent('wan_size_dialog', hint_ethprofile_mru_invalid);
            parent.$('#wan_size_dialog').focus();
            return false;
        }
        if((1280>Size || 1500<Size)&&(connectionType[1] == parent.$('#connection_type_dialog').val())){
            showErrorUnderTextbox_Parent('wan_size_dialog', hint_ethprofile_mtu_invalid);
            parent.$('#wan_size_dialog').focus();
            return false;
        }
    }

    if ('OnDemand'== parent.$('#ppp_dialupmode').val()){
        var idletime = parent.$('#ppp_maxideltime').val();
        if (true == isNaN(idletime)) {
            showErrorUnderTextbox_Parent('ppp_maxideltime', dialup_hint_max_idle_time_number);
            parent.$('#ppp_maxideltime').focus();
            return false;
        }else if ((idletime.indexOf('0') == 0) && (idletime != 0)) {
            showErrorUnderTextbox_Parent('ppp_maxideltime', dialup_hint_max_idle_time_invalid);
            parent.$('#ppp_maxideltime').focus();
            return false;
        }else if (idletime.indexOf('.') != -1) {
            showErrorUnderTextbox_Parent('ppp_maxideltime', dialup_hint_max_idle_time_integer);
            parent.$('#ppp_maxideltime').focus();
            return false;
        }else if ((idletime < MIN_IDLE_TIME) || (idletime > MAX_IDLE_TIME)) {
            showErrorUnderTextbox_Parent('ppp_maxideltime', dialup_hint_max_idle_time_range);
            parent.$('#ppp_maxideltime').focus();
            return false;
        }
    }
    
    if(getDisplayStatus("ipv4_address_dialog_tr") && !checkIPv4('ipv4_address_dialog')){
        return false;
    }
    if(getDisplayStatus("ipv4_address_dialog_tr") && !checkIPv4SubnetMask('subnet_mask_dialog')){
        return false;
    }
    if(getDisplayStatus("default_gateway_dialog_tr") && !checkIPv4('default_gateway_dialog')){
        return false;
    }
    if(getDisplayStatus("default_gateway_dialog_tr") && !checkIPv4('primary_DNS_dialog')){
        return false;
    }
    if(parent.$("#secondary_DNS_dialog").val()!='' && getDisplayStatus("secondary_DNS_dialog_tr") && !checkIPv4('secondary_DNS_dialog')){
        return false;
    }
    if(getDisplayStatus("ipv6_address_dialog_tr") && !checkIPv6('ipv6_address_dialog')){
        return false;
    }
    if(getDisplayStatus("ipv6_address_dialog_tr") && !checkIPv6('ipv6_default_gateway_dialog')){
        return false;
    }
    if(getDisplayStatus("ipv6_address_dialog_tr") && ''==parent.$('#ipv6_IdIpv6PrefixLen_dialog').val()){
        showErrorUnderTextbox_Parent('ipv6_IdIpv6PrefixLen_dialog', samba_input_empty);
        parent.$('#ipv6_IdIpv6PrefixLen_dialog').focus();
        return false;
    }
    else if(getDisplayStatus("ipv6_address_dialog_tr") && !checkPrefixLenNum('ipv6_IdIpv6PrefixLen_dialog')){
        showErrorUnderTextbox_Parent('ipv6_IdIpv6PrefixLen_dialog', samba_input_invalid);
        parent.$('#ipv6_IdIpv6PrefixLen_dialog').focus();
        return false;
    }
    if(getDisplayStatus("ipv6_primary_DNS_dialog_tr") && !checkIPv6('ipv6_primary_DNS_dialog')){
        return false;
    }
    if(parent.$("#ipv6_secondary_DNS_dialog").val()!='' && getDisplayStatus("ipv6_primary_DNS_dialog_tr") && !checkIPv6('ipv6_secondary_DNS_dialog')){
        return false;
    }
    if(('' != parent.$("#user_name_dialog").val()) && (getDisplayStatus("user_name_dialog_tr")) && (!checkNameOrPassword('user_name_dialog'))){
        return false;
    }
   if(('' == parent.$("#user_name_dialog").val()) && (getDisplayStatus("user_name_dialog_tr")) ){
        showErrorUnderTextbox_Parent('user_name_dialog', samba_input_invalid);
        parent.$('#user_name_dialog').focus();
        return false;
    }
    if(('' != parent.$("#password_dialog").val()) && (getDisplayStatus("user_name_dialog_tr")) && (!checkNameOrPassword('password_dialog'))){
        return false;
    }
   if(('' == parent.$("#password_dialog").val()) && (getDisplayStatus("user_name_dialog_tr")) ){
        showErrorUnderTextbox_Parent('password_dialog', samba_input_invalid);
        parent.$('#password_dialog').focus();
        return false;
    }
    if((('1' < g_eth_array.length) || ((parent.$(".dialog_header_left").text() == dialup_button_newprofile) && ('1' == g_eth_array.length))) && 
        ('0' != parent.$('#connection_type_dialog').val())){
        showErrorUnderTextbox_Parent('show_dialog_table',hint_ethprofile_edit_invalid);
        return false;
    }
    return true;
}

function checkPrefixLenNum(id){
    var flag = true;
    var sizeParts = parent.$('#'+id).val().split("");
    if(parent.$('#'+id).val()<0||parent.$('#'+id).val()>128){
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
function checkNameOrPassword(id){
    if ('' != $.trim(parent.$('#'+id).val())) {
            if (!checkInputChar_wanName($.trim(parent.$('#'+id).val()))) {
                showErrorUnderTextbox_Parent(id, wan_valid_username);
                parent.$('#'+id).focus();
                return false;
            }else {
                return true;
            }
    } else {
        showErrorUnderTextbox_Parent(id, samba_input_empty);
        parent.$('#'+id).focus();
        return false;
    }
}

function setPassword(password){
    var count = password.length;
    var new_psw = "";
    var i = 0 ;
    for(;i < count; i ++){
        new_psw += "*";
    }
    return new_psw;
}
function setIPv4AddrTr(IdAddrType){
    if(IPADDRESSTYPE_STATIC== IdAddrType && 'none' != $("#ipv4_address_type_tr").css('display')){
        $("#ipv4_address_tr").show();
        $("#subnet_mask_tr").show();
        $("#default_gateway_tr").show();
        $("#primary_DNS_tr").show();
        $("#secondary_DNS_tr").show();
    } else {
        $("#ipv4_address_tr").hide();
        $("#subnet_mask_tr").hide();
        $("#default_gateway_tr").hide();
        $("#primary_DNS_tr").hide();
        $("#secondary_DNS_tr").hide();
    }
}
function setIPv6AddrTr(IdIpv6AddrType){
    if(IPADDRESSTYPE_STATIC == IdIpv6AddrType && 'none' != $("#ipv6_address_type_tr").css('display')){
        $("#ipv6_address_tr").show();
        $("#ipv6_default_gateway_tr").show();
        $("#ipv6_primary_DNS_tr").show();
        $("#ipv6_secondary_DNS_tr").show();
    } else {
        $("#ipv6_address_tr").hide();
        $("#ipv6_default_gateway_tr").hide();
        $("#ipv6_primary_DNS_tr").hide();
        $("#ipv6_secondary_DNS_tr").hide();
    }
}
function setIPType(ConnectionType,IdAddrType,IdIpv6AddrType,ConnectMode,IpType){
    if(parent.g_feature_ipv6){
        $('#ipv6_address_type_tr').show();
    } else {
        $('#ipv6_address_type_tr').hide();
    }
    if(ConnectionType==1){
        $('#label_wan_mtuormru').text(wan_mtu);
        $('#user_name_tr').hide();
        $('#wan_password_tr').hide();
        $('.css_dialupmode').hide();
        $('#authentication_mode_tr').hide();
        if(wan_ipv4_text == IpType){
            $('#ipv4_address_type_tr').show();
            $('#ipv6_address_type_tr').hide();
        }else if(wan_ipv6_text == IpType){
            $('#ipv4_address_type_tr').hide();
            $('#ipv6_address_type_tr').show();
        }else if(wan_ipv4_ipv6_text == IpType){
            $('#ipv4_address_type_tr').show();
            $('#ipv6_address_type_tr').show();            
        }
    } else {
        $('#label_wan_mtuormru').text(label_wan_mru);
        $('#ipv4_address_type_tr').hide();
        $('#user_name_tr').show();
        $('#wan_password_tr').show();
        $('.css_dialupmode').show();
        $('#authentication_mode_tr').show();
        if(PPP_DIALUPMODE_AUTO == ConnectMode){
            $('#eth_connectionmode').html('<pre>' +common_auto + '</pre>');
            $('#eth_maxideltime_tr').hide();
        }else if(PPP_DIALUPMODE_MANUAL == ConnectMode){
            $('#eth_connectionmode').html('<pre>' + common_manual + '</pre>');
            $('#eth_maxideltime_tr').hide();
        }else if(PPP_DIALUPMODE_ONDEMAND == ConnectMode){
            $('#eth_connectionmode').html('<pre>' + label_dailup_option_on_demand + '</pre>');
            $('#eth_maxideltime_tr').show();
        }else{
            $('.css_dialupmode').hide();
        }

        if(wan_ipv4_text == IpType){
            $('#ipv6_address_type_tr').hide();
        }else if(wan_ipv6_text == IpType || wan_ipv4_ipv6_text == IpType){
            $('#ipv6_address_type_tr').show();
        }
    }
    setIPv6AddrTr(IdIpv6AddrType);
    setIPv4AddrTr(IdAddrType);
}
function getDNSServer(DNSServers,num){
    var DNSParts = DNSServers.split(',');
    if(0==num){
        return DNSParts[num];
    } else if(2==DNSParts.length){
        return DNSParts[num];
    } else {
        return '';
    }
}
function setPage(array){
    var ipaddressType_text;
    var AuthMode_text;
    var IpType_text;
    $('#connection_type').html('<pre>' + (0== array.ConnectionType ? wan_PPP_Routed_text : wan_IP_Routed_text) + '</pre>');
    if((IP_TYPE_ENABLE == array.IPv4_Supported)&&(IP_TYPE_DISABLE == array.IPv6_Supported)){
        IpType_text = wan_ipv4_text;
    }else if((IP_TYPE_ENABLE == array.IPv6_Supported)&&(IP_TYPE_DISABLE == array.IPv4_Supported)){
        IpType_text = wan_ipv6_text;
    }else if((IP_TYPE_ENABLE == array.IPv6_Supported)&&(IP_TYPE_ENABLE == array.IPv4_Supported)){
        IpType_text = wan_ipv4_ipv6_text;
    }
    $('#ip_type').html('<pre>' + IpType_text + '</pre>');
    $('#wan_mtu').html('<pre>' + array.MTU + '</pre>');
    $('#ipv4_address_type').html('<pre>' + (IPADDRESSTYPE_STATIC == array.IdAddrType?dialup_label_static:dhcp_label_dhcp) + '</pre>');
    $('#eth_maxideltime').html('<pre>' + array.MaxIdelTime + '</pre>');
    
    if(IPADDRESSTYPE_DHCP == array.IdIpv6AddrType){
        ipaddressType_text = dhcp_label_dhcp;
    }else if(IPADDRESSTYPE_SLAAC == array.IdIpv6AddrType){
        ipaddressType_text = slaac_label_slaac;
    }else if(IPADDRESSTYPE_STATIC == array.IdIpv6AddrType){
        ipaddressType_text = dialup_label_static;
    }
    $('#ipv6_address_type').html('<pre>' + ipaddressType_text + '</pre>');

    if(PPP_AUTHMODE_AUTO == array.AuthMode){
        AuthMode_text = common_auto;
    }else if(PPP_AUTHMODE_PAP == array.AuthMode){
        AuthMode_text = dialup_label_pap;
    }else if(PPP_AUTHMODE_CHAP == array.AuthMode){
        AuthMode_text = dialup_label_chap;
    }
    $('#authentication_mode').html('<pre>' + AuthMode_text + '</pre>');
    
    $('#user_name').html('<pre>' + resolveXMLEntityReference(array.Username) + '</pre>');
    $('#wan_password').html('<pre>' + setPassword(array.Password) + '</pre>');
    $("#ipv4_address").html('<pre>' + array.IdIpAddr + '</pre>');
    $("#subnet_mask").html('<pre>' + array.IdSubMask + '</pre>');
    $("#default_gateway").html('<pre>' + array.IdGateWay + '</pre>');
    $("#primary_DNS").html('<pre>' + getDNSServer(array.DNSServers,0) + '</pre>');
    $("#secondary_DNS").html('<pre>' + getDNSServer(array.DNSServers,1) + '</pre>');
    $("#ipv6_address").html('<pre>' + array.IdIpv6Addr + '/' + sd_label_size + common_colon + array.IdIpv6PrefixLen + '</pre>');
    $("#ipv6_default_gateway").html('<pre>' + array.IdIpv6Gateway + '</pre>');
    $("#ipv6_primary_DNS").html('<pre>' + getDNSServer(array.X_IPv6DNSServers,0) + '</pre>');
    $("#ipv6_secondary_DNS").html('<pre>' + getDNSServer(array.X_IPv6DNSServers,1) + '</pre>');
    setIPType(array.ConnectionType,array.IdAddrType,array.IdIpv6AddrType,array.ConnectMode,IpType_text);
    if(0 == array.ReadOnly){
        button_enable("edit_profile",'1');
        button_enable("delete_profile",'1');
    } else{
        button_enable("edit_profile",'0');
        button_enable("delete_profile",'0');
    } 
    
    if(!parent.g_feature_ipv6){
        $('#ipv6_address_type_tr').hide();
        $("#ipv6_address_tr").hide();
        $("#ipv6_default_gateway_tr").hide();
        $("#ipv6_primary_DNS_tr").hide();
        $("#ipv6_secondary_DNS_tr").hide();
    }
}
function setSelect(id,array){
    var html = '';
    parent.$('#'+id).children().remove();
    $(array).each(function(i) {
        html += "<option value='"+i+"'>"+array[i]+"</option>";
    });
    parent.$('#'+id).append(html);
}
function setStringSelect(id,array1,array2){
    var html = '';
    parent.$('#'+id).children().remove();
    $(array1).each(function(i) {
        html += "<option value='"+array1[i]+"'>"+array2[i]+"</option>";
    });
    parent.$('#'+id).append(html);
}
function setETHNameSelect(id,arrayIndex,arrayName,num){
    var html = '';
    $('#'+id).children().remove();
    $(arrayName).each(function(i) {
        var ethName = resolveXMLEntityReference(arrayName[i]);
        while (ethName.indexOf(' ') >= 0) {
            ethName = ethName.replace(' ', '&nbsp;');
        }
        if(arrayIndex[i] == num){
            html += "<option value='"+ arrayIndex[i] +"'>"+ethName+ common_default +"</option>";
        } else {
            html += "<option value='"+ arrayIndex[i] +"'>"+ethName+"</option>";
        }
    });
    $('#'+id).append(html);
}

function eth_getCurrentProfileArrayIndex(profileArray,index){
    var i = 0;
    for (; i < profileArray.length; i++) {
        if (profileArray[i].Index == index) {
                return i;
        }
    }
    return 0;
}

function initPage(){
    g_eth_array.splice(0, g_eth_array.length);
    g_eth_name_array.splice(0, g_eth_name_array.length);
    g_eth_index_array.splice(0,g_eth_index_array.length);

    getAjaxData('api/dialup/ETHs', function($xml) {
        var wan = "";
        var i =0;
        var ret = xml2object($xml);
        if(ret.type =="response" && typeof(ret.response.ETHs)!='undefined' && typeof(ret.response.ETHs.ETH)!='undefined'){
            $('#Profile_ContentStatus').show();
            wan = ret.response.ETHs;
            g_wan_default_index = parseInt(ret.response.CurrentETH, 10);
            if (wan) {
                if(wan.ETH != '') {
                    if ($.isArray(wan.ETH)) {
                        $(wan.ETH).each(function(i) {
                            if(wan.ETH[i].Password != '') {
                                wan.ETH[i].Password = (wan.ETH[i].Password == '' ? '' : decrypt(wan.ETH[i].Password));
                            }
                            g_eth_array.push(wan.ETH[i]);
                            g_eth_name_array.push(wan.ETH[i].Name);
                            g_eth_index_array.push(wan.ETH[i].Index);
                        });
                    }
                    else {
                        if(wan.ETH.Password != '') {
                            wan.ETH.Password = (wan.ETH.Password == '' ? '' : decrypt(wan.ETH.Password));
                        }
                        g_eth_array.push(wan.ETH);
                        g_eth_name_array.push(wan.ETH.Name);
                        g_eth_index_array.push(wan.ETH.Index);
                    }
                    $('#eth_name').attr('disabled',false);
                    setETHNameSelect('eth_name',g_eth_index_array,g_eth_name_array,g_wan_default_index);
                    setTimeout(function(){$('#eth_name').val(g_wan_default_index);},1);
                    g_wan_array_index = eth_getCurrentProfileArrayIndex(g_eth_array, g_wan_default_index);
                    setPage(g_eth_array[g_wan_array_index]);
                    
                    if((connectionType[1] == g_eth_array[0].ConnectionType) ||(('4'== g_eth_array.length) &&
                        (connectionType[0] == g_eth_array[0].ConnectionType))){
                        button_enable("new_profile",'0');
                    }
                }
            }
        } else {
            return;
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
    });

    //get ipv6
    getAjaxData("api/settings/hostinfo",function($xml) 
    {
        var ret = xml2object($xml);
        if(ret.type == "response" && typeof(ret.response.HostIPv6Address)!='undefined') 
        {
            DEFAULT_GATEWAY_IPv6Address = ret.response.HostIPv6Address;
        }
    }, {
        sync : true
    });
    
}
function setConnectionType(){
    if(parent.g_feature_ipv6){
        parent.$("#ipv6_address_type_dialog_tr").show();
    } else {
           parent.$("#ipv6_address_type_dialog_tr").hide();
    }
    if(0==parent.$("#connection_type_dialog").val()){
        parent.$("#ipv4_address_type_dialog_tr").hide();
        parent.$("#user_name_dialog_tr").show();
        parent.$("#ppp_dialupmode_tr").show();
        parent.$(".css_authentication_mode").show();
        if(dialup_button_newprofile == parent.$(".dialog_header_left").text()){
            parent.$("#wan_size_dialog").val(1492);
        }
        parent.$("#label_eth_mtuormru").text(label_wan_mru);
    } else {
        parent.$("#user_name_dialog_tr").hide();
        parent.$("#ppp_dialupmode_tr").hide();
        parent.$(".css_authentication_mode").hide();
        if(dialup_button_newprofile == parent.$(".dialog_header_left").text()){
            parent.$("#wan_size_dialog").val(1500);
        }
        parent.$("#label_eth_mtuormru").text(wan_mtu);
    }

     if(common_edit == parent.$(".dialog_header_left").text()){
            if(g_wan_currentConnType != parent.$("#connection_type_dialog").val()){
                if(0 == parent.$("#connection_type_dialog").val()){
                    parent.$("#wan_size_dialog").val(1492);
                }else{
                    parent.$("#wan_size_dialog").val(1500);
                }
            }else{
                parent.$("#wan_size_dialog").val(g_wan_currentMTU);
            }
     }
    check_IPType_ShowStatus();
}

function showIpv4AddrStatus(){
    if(('Static' == parent.$("#ipv4_address_type_dialog").val()) && ('none' != parent.$("#ipv4_address_type_dialog_tr").css("display"))){
        parent.$("#ipv4_address_dialog_tr").show();
        parent.$("#default_gateway_dialog_tr").show();
        parent.$("#secondary_DNS_dialog_tr").show();
    } else {
        parent.$("#ipv4_address_dialog_tr").hide();
        parent.$("#default_gateway_dialog_tr").hide();
        parent.$("#secondary_DNS_dialog_tr").hide();
    }
}

function showIpv6AddrStatus(){
    if(('Static' == parent.$("#ipv6_address_type_dialog").val()) && ('none' != parent.$("#ipv6_address_type_dialog_tr").css("display"))){
        parent.$("#ipv6_address_dialog_tr").show();
        parent.$("#ipv6_primary_DNS_dialog_tr").show();
    } else {
        parent.$("#ipv6_address_dialog_tr").hide();
        parent.$("#ipv6_primary_DNS_dialog_tr").hide();
    }
}

function check_IPType_ShowStatus(){
    if(0==parent.$("#connection_type_dialog").val()){
        if('ipv4' == parent.$('#ip_type_dialog').val()){
            parent.$("#ipv6_address_type_dialog_tr").hide();
        }else if('ipv6' == parent.$('#ip_type_dialog').val() || 'ipv4ipv6' == parent.$('#ip_type_dialog').val()){
            parent.$("#ipv6_address_type_dialog_tr").show();
        }
    }else{
        if('ipv4' == parent.$('#ip_type_dialog').val()){
            parent.$("#ipv4_address_type_dialog_tr").show();
            parent.$("#ipv6_address_type_dialog_tr").hide();
        }else if('ipv6' == parent.$('#ip_type_dialog').val()){
            parent.$("#ipv4_address_type_dialog_tr").hide();
            parent.$("#ipv6_address_type_dialog_tr").show();
        }else if('ipv4ipv6' == parent.$('#ip_type_dialog').val()){
            parent.$("#ipv4_address_type_dialog_tr").show();
            parent.$("#ipv6_address_type_dialog_tr").show();
        }
    }

        showIpv4AddrStatus();
        showIpv6AddrStatus();
}
function showDialog(name){
    parent.call_dialog_wan(name, g_addhtml, common_save, 'wan_Save', common_cancel, 'wan_Cancel',save_eths,cancel_eths);
    //setSelect('wan_connection_dialog',wanConnection);
    setStringSelect('ipv4_address_type_dialog',IPv4addressType,IPv4addressTypeText);
    setStringSelect('ipv6_address_type_dialog',IPv6addressType,IPv6addressTypeText);
    setStringSelect('authentication_mode_dialog',authenticationMode,authenticationText);
    setStringSelect('connection_type_dialog',connectionType,connectionTypeText);
    setStringSelect('ip_type_dialog',IPType,TPTypeText);
    setStringSelect('ppp_dialupmode', pppdialupMode, pppdialupText);
    setConnectionType();
}
function setDialog(array){
    g_wan_currentProfileNmae = array.Name;
    g_wan_currentConnType = array.ConnectionType;
    g_wan_currentMTU = array.MTU;
    parent.$("#eth_name_dialog").val(array.Name);
    parent.$("#wan_size_dialog").val(array.MTU);


    if('' !=array.ConnectMode) {
          parent.$("#ppp_dialupmode").val(array.ConnectMode);
    }
		
    if((IP_TYPE_ENABLE == array.IPv4_Supported) && (IP_TYPE_DISABLE == array.IPv6_Supported)){
        parent.$('#ip_type_dialog').val(IP_TYPE_IPV4);
    }else if((IP_TYPE_ENABLE == array.IPv6_Supported) && (IP_TYPE_DISABLE == array.IPv4_Supported)){
        parent.$('#ip_type_dialog').val(IP_TYPE_IPV6);
    }else if((IP_TYPE_ENABLE == array.IPv4_Supported) && (IP_TYPE_ENABLE == array.IPv6_Supported)){
        parent.$('#ip_type_dialog').val(IP_TYPE_IPV4IPV6);
    }
    if(PPP_DIALUPMODE_ONDEMAND == array.ConnectMode){
        parent.$('.css_maxideltime').show();
        parent.$('#ppp_maxideltime').val(array.MaxIdelTime);
    }else{
        parent.$('.css_maxideltime').hide();
    }
	
    if('' !=array.AuthMode) {
        parent.$("#authentication_mode_dialog").val(array.AuthMode);
    }
    if('' !=array.IdAddrType){
        parent.$("#ipv4_address_type_dialog").val(array.IdAddrType);
    }
    if('' !=array.IdIpv6AddrType){
        parent.$("#ipv6_address_type_dialog").val(array.IdIpv6AddrType);
    }
   
    parent.$("#connection_type_dialog").val(array.ConnectionType);
    parent.$("#user_name_dialog").val(array.Username);
    parent.$("#password_dialog").val(array.Password);
    parent.$("#ipv4_address_dialog").val(array.IdIpAddr);
    parent.$("#subnet_mask_dialog").val(array.IdSubMask);
    parent.$("#default_gateway_dialog").val(array.IdGateWay);
    parent.$("#primary_DNS_dialog").val(getDNSServer(array.DNSServers,0));
    parent.$("#secondary_DNS_dialog").val(getDNSServer(array.DNSServers,1));
    parent.$("#ipv6_address_dialog").val(array.IdIpv6Addr);
    parent.$("#ipv6_default_gateway_dialog").val(array.IdIpv6Gateway);
    parent.$("#ipv6_IdIpv6PrefixLen_dialog").val(array.IdIpv6PrefixLen);
    parent.$("#ipv6_primary_DNS_dialog").val(getDNSServer(array.X_IPv6DNSServers,0));
    parent.$("#ipv6_secondary_DNS_dialog").val(getDNSServer(array.X_IPv6DNSServers,1));
    setConnectionType();
}
function wan_deleteProfile() {
    var delIndex = parseInt($('#eth_name').val(),10);
    var minIndex = 0;
    if (delIndex == g_wan_default_index) {
        parent.showWaitingDialog(common_waiting,common_waiting);
    }
    var request = {
        Delete : delIndex,
        SetDefault : minIndex,
        Modify : 0
    };
    var delProfile = object2xml('request', request);
    saveAjaxData('api/dialup/ETHs', delProfile, function($xml) {
        if(0 != parent.$('#wait_table').length){
            parent.closeWaitingDialog();
        }
        var delRet = xml2object($xml);
        if (isAjaxReturnOK(delRet)) {
            parent.showInfoDialog(common_success);
                top.window.location.reload();
        }
        else {
            parent.showInfoDialog(common_failed);
        }
        top.window.location.reload();
    });
}
function wan_cancelDelete() {
    parent.$('#div_wrapper').remove();
    parent.$('.dialog').remove();
}
function wan_saveRequest(obj) {
    parent.clearDialog();
    parent.showWaitingDialog(common_waiting,common_waiting);
    var wan_xml = object2xml('request', obj);
    saveAjaxData('api/dialup/ETHs', wan_xml, function($xml) {
        var return_ret = xml2object($xml);
        parent.closeWaitingDialog();
        if (!isAjaxReturnOK(return_ret)) {
            parent.showInfoDialog(common_failed);
        } else {
            parent.showInfoDialog(common_success);
            setTimeout(function() {
                top.window.location.reload();
                return false;
            }, g_feature.dialogdisapear);
        }
    });
}
function ipv6Enable(ethSave){
    if(!parent.g_feature_ipv6){
        ethSave.IdIpv6AddrType = '';
        ethSave.IdIpv6Addr = '';
        ethSave.IdIpv6PrefixLen = '';
        ethSave.IdIpv6Gateway = '';
        ethSave.X_IPv6DNSServers = '';
    }
}
function wan_setDefaultPor(set_default_request) {
    var default_wan_xml = object2xml('request', set_default_request);
    saveAjaxData('api/dialup/ETHs', default_wan_xml, function($xml) {
        var delRet = xml2object($xml);
        if (isAjaxReturnOK(delRet)) {
            g_wan_default_index = set_default_request.SetDefault;
            setETHNameSelect('eth_name',g_eth_index_array,g_eth_name_array,g_wan_default_index);
            $('#eth_name').val(g_wan_default_index);
            
        }
    });
}

//save eths function
function save_eths(){
    if(isVaildVirtual()){
        var ethNameDialog = parent.$("#eth_name_dialog").val();
        var wanMtuDialog = parent.$("#wan_size_dialog").val();
        var pppdialupModeDialog = parent.$("#ppp_dialupmode").val();
        var ppp_maxideltimeDialog = parent.$("#ppp_maxideltime").val();
        var authenticationModeDialog = parent.$("#authentication_mode_dialog").val();
        var connectionTypeDialog = parent.$("#connection_type_dialog").val();
        var iptype = parent.$('#ip_type_dialog').val();
        var usernameDialog = getDisplayStatus("user_name_dialog_tr") ? parent.$("#user_name_dialog").val() : '';
        var passwordDialog = getDisplayStatus("user_name_dialog_tr") ? encrypt(parent.$("#password_dialog").val()) : '';
        var ipv4AddressTypeDialog = getDisplayStatus("ipv4_address_type_dialog_tr") ? parent.$("#ipv4_address_type_dialog").val() : '';
        var ipv6AddressTypeDialog = getDisplayStatus("ipv6_address_type_dialog_tr") ? parent.$("#ipv6_address_type_dialog").val() : '';
        var ipv4AddressDialog = getDisplayStatus("ipv4_address_dialog_tr") ? parent.$("#ipv4_address_dialog").val() : '';
        var subnetMaskDialog = getDisplayStatus("ipv4_address_dialog_tr") ? parent.$("#subnet_mask_dialog").val() : '';
        var defaultGatewayDialog = getDisplayStatus("default_gateway_dialog_tr") ? parent.$("#default_gateway_dialog").val() : '';
        var primaryDNSDialog = getDisplayStatus("default_gateway_dialog_tr") ? parent.$("#primary_DNS_dialog").val() : '';
        var secondaryDNSDialog = getDisplayStatus("secondary_DNS_dialog_tr") ? parent.$("#secondary_DNS_dialog").val() : '';
        var ipv6AddressDialog = getDisplayStatus("ipv6_address_dialog_tr") ? parent.$("#ipv6_address_dialog").val() : '';
        var ipv6DefaultGatewayDialog = getDisplayStatus("ipv6_address_dialog_tr") ? parent.$("#ipv6_default_gateway_dialog").val() : '';
        var ipv6IdIpv6PrefixLenDialog = getDisplayStatus("ipv6_address_dialog_tr") ? parent.$("#ipv6_IdIpv6PrefixLen_dialog").val() : '';
        var ipv6PrimaryDNSDialog = getDisplayStatus("ipv6_primary_DNS_dialog_tr") ? parent.$("#ipv6_primary_DNS_dialog").val() : '';
        var ipv6SecondaryDNSDialog = getDisplayStatus("ipv6_primary_DNS_dialog_tr") ? parent.$("#ipv6_secondary_DNS_dialog").val() : '';
        
        var ethSave = {
            Index : '',
            IsValid : 1,
            Name : resolveXMLEntityReference(ethNameDialog),
            MTU : wanMtuDialog,
            ConnectionType : connectionTypeDialog,
            ReadOnly : 0
        };

        if(('ipv6' == iptype || 'ipv4ipv6' == iptype) && ('Static' == ipv6AddressTypeDialog)){
            ethSave.IdIpv6AddrType = ipv6AddressTypeDialog;
            ethSave.IdIpv6Addr = ipv6AddressDialog;
            ethSave.IdIpv6PrefixLen = (ipv6IdIpv6PrefixLenDialog == '') ? 0 : ipv6IdIpv6PrefixLenDialog;
            ethSave.IdIpv6Gateway = ipv6DefaultGatewayDialog;
            ethSave.X_IPv6DNSServers = ipv6SecondaryDNSDialog==''?ipv6PrimaryDNSDialog:(ipv6PrimaryDNSDialog+','+ipv6SecondaryDNSDialog);
        }else if('' != ipv6AddressTypeDialog){
            ethSave.IdIpv6AddrType = ipv6AddressTypeDialog;
        }

        if('ipv4' == iptype){
            ethSave.IPv4_Supported = 1;
            ethSave.IPv6_Supported = 0;
        }else if('ipv6' == iptype){
            ethSave.IPv4_Supported = 0;
            ethSave.IPv6_Supported = 1;
        }else if('ipv4ipv6' == iptype){
            ethSave.IPv4_Supported = 1;
            ethSave.IPv6_Supported = 1;
        }
        
        if(0 == connectionTypeDialog){
            ethSave.ConnectMode = pppdialupModeDialog;
            ethSave.Username = resolveXMLEntityReference(usernameDialog);
            ethSave.Password = passwordDialog;
            ethSave.AuthMode = authenticationModeDialog;
            if('OnDemand' == pppdialupModeDialog){
                ethSave.MaxIdelTime = (ppp_maxideltimeDialog == '') ? 0: ppp_maxideltimeDialog;                
            }
        }else if(1 == connectionTypeDialog){
            if(('ipv4' == iptype || 'ipv4ipv6' == iptype) && ('Static' == ipv4AddressTypeDialog)){
                ethSave.IdAddrType = ipv4AddressTypeDialog;
                ethSave.IdIpAddr = ipv4AddressDialog;
                ethSave.IdSubMask = subnetMaskDialog;
                ethSave.IdGateWay = defaultGatewayDialog;
                ethSave.DNSServers = secondaryDNSDialog==''?primaryDNSDialog:(primaryDNSDialog+','+secondaryDNSDialog);
              }else if('' != ipv4AddressTypeDialog){
                ethSave.IdAddrType = ipv4AddressTypeDialog;
              }
        }
        ipv6Enable(ethSave);
        if(parent.$(".dialog_header_left").text() == common_edit) {
            var ethPro_deleteindex = 0;
            var ethPro_modify = 2;
            if(('1' == g_eth_array.length) && (parent.$('#connection_type_dialog').val() != g_eth_array[g_wan_array_index].ConnectionType)){
                ethPro_deleteindex = g_eth_array[g_wan_array_index].Index;
                ethPro_modify = 1;
            }
            ethSave.Index = g_eth_array[g_wan_array_index].Index;
            var submitObject = {
                Delete : ethPro_deleteindex,
                SetDefault :0,
                Modify :ethPro_modify,
                ETH : ethSave
            };
            if(('1' == g_eth_array.length) && (parent.$('#connection_type_dialog').val() != g_eth_array[g_wan_array_index].ConnectionType)){
                parent.showConfirmDialog(lable_wandelete_note + " " + lable_continue_anyway, function(){wan_saveRequest(submitObject);}, wan_cancelDelete);
            }else{
                wan_saveRequest(submitObject);
                return true;
            }
        }
        else {
            var submitObjectAdd = {
                Delete : 0,
                SetDefault :0,
                Modify :1,
                ETH : ethSave
            };
            wan_saveRequest(submitObjectAdd);
            return true;
        }
    }
    else {
        return false;
    }
}

//click cancel button
function cancel_eths() {
    parent.clearDialog();
}

function setpppDialupModeStatus(){
    if('OnDemand' == parent.$('#ppp_dialupmode').val()){
        parent.$('.css_maxideltime').show();
    }else{
        parent.$('.css_maxideltime').hide();
    }
}

function getDisplayStatus(element_id)
{
    if(parent.$("#"+element_id).css('display')!='none'){
            return true;
    }else{
            return false;       
    }
}

function checkRepeatedlyProfileName(ProfileArray,CurrentProfilename){
    for(var i=0;i<ProfileArray.length;i++){
        if (ProfileArray[i].Name == CurrentProfilename) {
            return true;
        }
    }
    return false;
}

function edit_profilefun(){
        showDialog(common_edit);
        var defaultIndex = $('#eth_name').val();
        g_wan_array_index = eth_getCurrentProfileArrayIndex(g_eth_array, defaultIndex);
        setTimeout(function(){setDialog(g_eth_array[g_wan_array_index]);}, 1);
        parent.$('#eth_name_dialog').focus();
}
$(document).ready(function(){
    initPage();
    $('#eth_name').change(function(){
        var defaultIndex = $('#eth_name').val();
        g_wan_array_index = eth_getCurrentProfileArrayIndex(g_eth_array, defaultIndex);
        setPage(g_eth_array[g_wan_array_index]);
    });
    $("#new_profile").click(function(){
        if (!isButtonEnable('new_profile')) {
            return;
        }
        showDialog(dialup_button_newprofile);
        parent.$('#eth_name_dialog').focus();
    });
    
    $("#edit_profile").click(function(){
        if (!isButtonEnable('edit_profile')) {
            return;
        }
        if((g_wan_default_index == $('#eth_name').val()) &&
            ((MACRO_WIRELINE_CONNECTION_CONNECTED == parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus) ||
            (MACRO_WIRELINE_CONNECTION_IPV6CONNECTED == parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus))){
                parent.showConfirmDialog(lable_disconnect_network + " " + lable_continue_anyway, edit_profilefun, wan_cancelDelete);
            }else{
                edit_profilefun();
            }
    });
    $("#delete_profile").click(function(){
        if (!isButtonEnable('delete_profile')) {
            return;
        }
        if((g_wan_default_index == $('#eth_name').val()) &&
            ((MACRO_WIRELINE_CONNECTION_CONNECTED == parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus) ||
             (MACRO_WIRELINE_CONNECTION_IPV6CONNECTED == parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus))){
                parent.showConfirmDialog(lable_disconnect_network + " " + lable_wandelete_note + " " + lable_continue_anyway, wan_deleteProfile, wan_cancelDelete);
        }else{
                parent.showConfirmDialog(lable_wandelete_note + " " + lable_continue_anyway, wan_deleteProfile, wan_cancelDelete);
        }
    });
    parent.$("#ipv4_address_type_dialog").die().live('change',showIpv4AddrStatus);
    parent.$("#ipv6_address_type_dialog").die().live('change',showIpv6AddrStatus);
    parent.$("#connection_type_dialog").die().live('change',setConnectionType);
    parent.$('#ip_type_dialog').die().live('change',check_IPType_ShowStatus);
    parent.$('#ppp_dialupmode').die().live('change', setpppDialupModeStatus);
});