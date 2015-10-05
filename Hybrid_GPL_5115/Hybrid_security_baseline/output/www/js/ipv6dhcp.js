var DHCP_IPV6_MANUAL = 'Normal';
var DHCP_IPV6_SUBNET = 'UseAllocatedSubnet';
var DHCP_IPV6_OTHER = 'UseAllocatedOther';
var HAVE_TWO_DNS_SERVER = '2';
var g_settinginfo = {
    DHCPServerEnable : '',
    UseAllocatedWAN : '',
    AssociatedConnection : '',
    Prefix : '',
    PrefixLength : '',
    Preferredlifetime : '',
    Validlifetime : '',
    DNSServer : '',
    DomainName : ''
};

function change_checkbox(){
    if($("#dhcp_checkbox").attr("checked")){
        $("#dhcp_select_tr").show();
        select_configure_mode();
    }
    else{
        $("#dhcp_select_tr").hide();
        $("#dhcp_manual_content").hide();
        $("#useother_note").show();
    }
    button_enable('apply_button', '1');
}

function select_configure_mode(){
    $("#useother_note").hide();
    if( DHCP_IPV6_MANUAL == $("#dhcp_select").val() ){
        $("#dhcp_manual_content").show();
    }
    else {
        $("#dhcp_manual_content").hide();
        if( DHCP_IPV6_OTHER == $("#dhcp_select").val() ){
            $("#useother_note").show();
        }
    }
}

function validate(){
    var dhcp_dns_server = $("#dhcp_dns_server").val();
    var dhcp_dns_seconds = $("#dhcp_dns_seconds").val();
    clearAllErrorLabel();
    if(!CheckPrefix($.trim($("#dhcp_prefix").val()),0)){
        showErrorUnderTextbox('dhcp_prefix', err_prefix);
        return false;
    }
    if(!validate_prefix_length()){
        showErrorUnderTextbox('dhcp_prefix_length', err_validate_prefix_length);
        return false;
    }
    if(!validate_time($("#dhcp_preferred").val())){
        showErrorUnderTextbox('dhcp_preferred', err_dhcp_preferred);
        return false;
    }
    if(!validate_time($("#dhcp_valid").val())){
        showErrorUnderTextbox('dhcp_valid', err_dhcp_valid);
        return false;
    }
    if(dhcp_dns_server == '' && dhcp_dns_seconds == ''){
        return true;
    }
    if(dhcp_dns_server == '' && dhcp_dns_seconds != ''){
        showErrorUnderTextbox('dhcp_dns_server', err_dhcp_dns_server);
        return false;
    }
    if(!CheckPrefix(dhcp_dns_server, 1)||dhcp_dns_server == dhcp_dns_seconds){
        showErrorUnderTextbox('dhcp_dns_server', err_dhcp_dns_server);
        return false;
    }
    if(dhcp_dns_seconds == ''){
        return true;
    } 
    if(!CheckPrefix(dhcp_dns_seconds, 1)){
        showErrorUnderTextbox('dhcp_dns_seconds', err_dhcp_dns_seconds_server);
        return false;
    }
    return true;
}

function validate_prefix_length(){
    var prefix_length = parseInt($("#dhcp_prefix_length").val(), 10);
    var a = 0;
    if(prefix_length >= 16 && prefix_length <= 64){
        return true;
    }
    else{
        return false;
    }
}
function validate_time(time){
    var time_var = parseInt(time, 10);
    if(time_var >= 16 && time_var <= 4294967295){
        return true;
    }else{
        return false;
    }
}

function getProfileInfo(){
    getAjaxData("api/settings/ipv6dhcp", function($xml) {
        var profile_ret = xml2object($xml);
        if(profile_ret.type == "response")
        {

            $("#dhcp_prefix").val(profile_ret.response.Prefix);
            $("#dhcp_select").val(profile_ret.response.UseAllocatedWAN);
            $("#dhcp_prefix_length").val(profile_ret.response.PrefixLength);
            $("#dhcp_preferred").val(profile_ret.response.Preferredlifetime);
            $("#dhcp_valid").val( profile_ret.response.Validlifetime);

            g_settinginfo.DHCPServerEnable = profile_ret.response.DHCPServerEnable;
            g_settinginfo.UseAllocatedWAN = profile_ret.response.UseAllocatedWAN;
            g_settinginfo.AssociatedConnection = profile_ret.response.AssociatedConnection;
            g_settinginfo.Prefix = profile_ret.response.Prefix;
            g_settinginfo.PrefixLength = profile_ret.response.PrefixLength;
            g_settinginfo.Preferredlifetime = profile_ret.response.Preferredlifetime;
            g_settinginfo.Validlifetime = profile_ret.response.Validlifetime;
            g_settinginfo.DomainName = profile_ret.response.DomainName;
            g_settinginfo.DNSServer =  profile_ret.response.DNSServer;

            var dhcp_dns_server = profile_ret.response.DNSServer.split(',');
            if(HAVE_TWO_DNS_SERVER == dhcp_dns_server.length)
            {
                $("#dhcp_dns_server").val(dhcp_dns_server[0]);
                $("#dhcp_dns_seconds").val(dhcp_dns_server[1]);
            }
            else
            {
                $("#dhcp_dns_server").val(dhcp_dns_server[0]);
            }
            
            if(g_settinginfo.DHCPServerEnable ==0){
                $("#dhcp_manual_content").hide();
                $("#dhcp_select_tr").hide();
                $("#dhcp_checkbox").attr("checked",false);
                $("#useother_note").show();
            }
            else {
                $("#dhcp_checkbox").attr("checked",true);
                $("#dhcp_select_tr").show();
                select_configure_mode();
            }
        }
    });
}

function saveSetting() {
    g_settinginfo = {};
    g_settinginfo.DHCPServerEnable = $("#dhcp_checkbox").attr("checked")? 1 : 0;
    if(1 == g_settinginfo.DHCPServerEnable)
    {
        g_settinginfo.UseAllocatedWAN = $("#dhcp_select").val();
        if(DHCP_IPV6_MANUAL == $("#dhcp_select").val())
        {
            g_settinginfo.Prefix = $("#dhcp_prefix").val();
            g_settinginfo.PrefixLength = $("#dhcp_prefix_length").val();
            g_settinginfo.Preferredlifetime = $("#dhcp_preferred").val();
            g_settinginfo.Validlifetime = $("#dhcp_valid").val();
            if(($("#dhcp_dns_seconds").val()=='' || $("#dhcp_dns_seconds").val()==null) 
                && ($("#dhcp_dns_server").val() == "" || $("#dhcp_dns_server").val() == null)){
                g_settinginfo.DNSServer = "";
            }
            else if ($("#dhcp_dns_seconds").val()==''||$("#dhcp_dns_seconds").val()==null){
                g_settinginfo.DNSServer = $("#dhcp_dns_server").val();
            }
            else {
                g_settinginfo.DNSServer = $("#dhcp_dns_server").val()+','+$("#dhcp_dns_seconds").val();
            }
        }
    }
    
    var xmlinfo = object2xml("request",g_settinginfo);
    saveAjaxData("api/settings/ipv6dhcp",xmlinfo,function($xml){
        var ret = xml2object($xml);
        if(isAjaxReturnOK(ret)){
            parent.showInfoDialog(common_success);
            button_enable('apply_button', '0');
        }
        else {
            parent.showInfoDialog(common_failed);
        }
        getProfileInfo();
    });
}

$(document).ready(function(){
    getProfileInfo();
    $("#dhcp_checkbox").click(change_checkbox);
    $("#dhcp_select").change(function(){
        select_configure_mode();
        button_enable('apply_button', '1');
    });
    button_enable('apply_button', '0');
    $("input").bind("change input paste cut keydown",function(){
        button_enable('apply_button', '1');
    });
    $("#apply_button").click(function(){
        if (isButtonEnable('apply_button')){
            if (('1' == $("#dhcp_checkbox").attr("checked")) && (DHCP_IPV6_MANUAL == $("#dhcp_select").val()) && (!validate())) {
                return;
            }
            saveSetting();
            //parent.showConfirmDialog(dhcp_hint_operation_restart_device, function(){saveSetting();});
        }
    });
});