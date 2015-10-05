// JavaScript Document
/****************************************************upnp
 * (start)************************************/
var g_upnpStatus;
var g_new_status;
var g_nat_status = '0';
var DEFAULT_GATEWAY_IP = '192.168.1.1';
var DHCP_PING_INTERVA = 50000;

var g_init_NatStatus;
var g_new_NatStatus;

function initPageData() {
    getAjaxData("api/security/upnp", function($xml) {
        var upnp_ret = xml2object($xml);
        if(upnp_ret.type == "response") {
            g_upnpStatus = parseInt(upnp_ret.response.UpnpStatus, 10);
            if(g_upnpStatus == 0) {
                $("#ck_disable").get(0).checked = true;
                $("#ck_enable").get(0).checked = false;
            }
            else {
                $("#ck_disable").get(0).checked = false;
                $("#ck_enable").get(0).checked = true;
            }
        }
        else {
            log.error("Error, no data");
        }

    },{
        sync: true
    });

    getAjaxData("api/settings/nat", function($xml) {
        var nat_ret = xml2object($xml);
        if(nat_ret.type == "response") {
            if(nat_ret.response.NatStatus == 2) {
                $('input[name= nat_flag][value=2]').attr('checked', true);
                g_init_NatStatus = 2;
            }
            else {
                $('input[name= nat_flag][value=1]').attr('checked', true);
                g_init_NatStatus = 1;
            }
            g_nat_status = nat_ret.response.NatStatus;
        }
        else {
            log.error("Error, no data");
        }

    },{
        sync: true
    });

    // get current settings gateway address
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type) {
            DEFAULT_GATEWAY_IP = ret.response.DhcpIPAddress;
        }
    }, {
        sync: true
    }
    );
}

function onRadioChange() {
    if($("#ck_enable").get(0).checked) {
        g_new_status = 1;
    }
    else {
        g_new_status = 0;
    }
    if(g_new_status == g_upnpStatus) {
        button_enable("apply", "0");
    }
    else {
        button_enable("apply", "1");
    }
}

function onApply() {
    g_upnpStatus = g_new_status;
    var request = {
        UpnpStatus : g_upnpStatus
    };
    var xmlstr = object2xml("request", request);
    saveAjaxData("api/security/upnp", xmlstr, function($xml) {
        var rsp = xml2object($xml);
        if(isAjaxReturnOK(rsp)) {
            button_enable("apply", "0");
            parent.showInfoDialog(common_success);
            initPageData();
        }
        else {
            parent.showInfoDialog(common_failed);       
        }
    });
}
function onApplyNat() {  
    var request = {
        NatStatus :  $("[name='nat_flag']:checked").val()
    };
    var xmlstr = object2xml("request", request);
    saveAjaxData("api/settings/nat", xmlstr, function($xml) {
        var rsp = xml2object($xml);
        if(isAjaxReturnOK(rsp)) {
            button_enable("apply_nat", "0");
            parent.showInfoDialog(common_success);
            initPageData();
        }
        else {
            parent.showInfoDialog(common_failed);        
        }
    });
}
function onApplyNat_reboot() {  
    var request = {
        NatStatus :  $("[name='nat_flag']:checked").val()
    };
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments, function() {});
    var xmlstr = object2xml("request", request);
    saveAjaxData("api/settings/nat", xmlstr, function($xml) {     
    });
    ping_setPingAddress(DEFAULT_GATEWAY_IP);
    setTimeout(startPing, DHCP_PING_INTERVA);
}

$(document).ready( function() {
    button_enable("apply", "0");
    button_enable("apply_nat", "0");
    $("#ck_enable,#ck_disable").click( function() {
        onRadioChange();
    });
 
    $("#apply").click( function() {
        if(!isButtonEnable("apply")) 
        {
            return;
        }
        onApply();
    });
    $("#nat_enable,#nat_disable").click(function(){
       
        if($("#nat_enable").get(0).checked) 
        {
            g_new_NatStatus = 2;
        }
        else 
        {
             g_new_NatStatus = 1;
        }
        if(g_init_NatStatus == g_new_NatStatus) 
        {
             button_enable("apply_nat", "0");
        }
        else 
        {
             button_enable("apply_nat", "1");
        }
    });
  
    $("#apply_nat").click( function() {
        if(!isButtonEnable("apply_nat")) 
        {
            return;
        }
        if(( $("[name='nat_flag']:checked").val()=='2' && g_nat_status!='2')
         ||( $("[name='nat_flag']:checked").val()!='2' && g_nat_status=='2') )
        {
            parent.showConfirmDialog(dhcp_hint_operation_restart_device, onApplyNat_reboot, function() {});
        }
        else
        {
            onApplyNat();
        }
        
    });
    initPageData();
});
/****************************************************upnp
 * (end)************************************/