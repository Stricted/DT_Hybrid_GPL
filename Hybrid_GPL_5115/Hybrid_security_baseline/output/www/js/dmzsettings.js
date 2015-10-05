var g_dmzData;
var g_dhcpData;
var g_dmzList = [];

function updateDmzRadioStatus() {
    if ($("[name='dmz_status']:checked").val() == 0) {
        $('#dmz_ip_address').attr('disabled', 'disabled');
        $('#dmz_ip_address').val('');
    }
    else {
        $('#dmz_ip_address').removeAttr('disabled');
        if('' == $('#dmz_ip_address').val()){
            $('#dmz_ip_address').val(g_dhcpData.DhcpIPAddress.substring(0, g_dhcpData.DhcpIPAddress.lastIndexOf('.') + 1));
            $('#dmz_ip_address').focus();
            $('#dmz_ip_address').select();
        }
    }
}

function setDmzInfo(Index){
    if(0 == Index){
        $('#dmz_wanpath').val(g_dmzList[Index].WanPath);
        $("input[name='dmz_status'][value=" + g_dmzList[Index].DmzStatus + ']').attr('checked', true);
        $('#dmz_ip_address').val(g_dmzList[Index].DmzIPAddress);
    }else{
        var dmz_table_status = false;
        for(var i=0;i<g_dmzList.length;i++){
            if(g_dmzList[i].WanPath == Index){
                Index = i;
                dmz_table_status = true;
                break;
            }
        }
        if(dmz_table_status){
            $('#dmz_wanpath').val(g_dmzList[Index].WanPath);
            $("input[name='dmz_status'][value=" + g_dmzList[Index].DmzStatus + ']').attr('checked', true);
            $('#dmz_ip_address').val(g_dmzList[Index].DmzIPAddress);
        }else{
           $("input[name='dmz_status'][value='1']").attr('checked', true);
           $('#dmz_ip_address').val(g_dhcpData.DhcpIPAddress.substring(0, g_dhcpData.DhcpIPAddress.lastIndexOf('.') + 1));
        }
    }
    updateDmzRadioStatus();
}

function initPage() {
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response'){
            g_dhcpData = ret.response;
        }
    }, {sync: true});
    
    addwanpath();
    getAjaxData('api/security/dmz', function($xml){
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            if(typeof(ret.response.Dmzs) != 'undefined'){
                g_dmzData = ret.response.Dmzs.Dmz;
                if(g_dmzData){
                    g_dmzList = [];
                    if($.isArray(g_dmzData)){
                        g_dmzList= g_dmzData;
                    }else{
                        g_dmzList.push(g_dmzData);
                    }
                 }

                if(0 < g_dmzList.length){
                    setDmzInfo(0);
                   $('#dmz_wanpath').change(function(){setDmzInfo(this.value);});
                }else{
                   $("input[name='dmz_status'][value='1']").attr('checked', true);
                   $('#dmz_ip_address').val(g_dhcpData.DhcpIPAddress.substring(0, g_dhcpData.DhcpIPAddress.lastIndexOf('.') + 1));
                    updateDmzRadioStatus();
                }
            }
        }
    });
}

function addwanpath()
{ 
    getAjaxData('api/settings/ddns-wanpath', function($xml) {
    var ret = xml2object($xml);
    var newline = "";
    var wanpath;
    if(ret.type =="response"){
        if(typeof(ret.response.WanPaths)!='undefined' && typeof(ret.response.WanPaths.WanPath)!='undefined') {
            wanpath = ret.response.WanPaths.WanPath;
            $("#dmz_wanpath option").remove();
            var i=0;
            if ($.isArray(wanpath)){
                for(i=0;i<wanpath.length;i++){
                    newline += "<option value='"+wanpath[i].WanPathValue+"'>"+filterWanPathName(wanpath[i].WanPathName)+"</option>";                 
                }                  
            } 
            else {
                newline += "<option value='"+wanpath.WanPathValue+"'>"+filterWanPathName(wanpath.WanPathName)+"</option>";                  
            }
            if(""!=newline){  
                $('#dmz_wanpath').append(newline);
            } 
        }                 
    } 
    else {
        return;
    }
    },{sync: true});
}

function isSameBeforThreeAddrs(ip1,ip2){
    var addrParts1 = ip1.split(".");
    var addrParts2 = ip2.split(".");
    for (i = 0; i < 3; i++) 
    {
        if ((Number(addrParts1[i]))  != (Number(addrParts2[i])) ) 
        {
            return false;
        }
    }
    return true;
}
function validateDmz() {
    var dmzIP = $('#dmz_ip_address').val();
    var checkIPResult = isValidIpAddress(dmzIP);
    if (false == checkIPResult) {
        showErrorUnderTextbox('dmz_error', dialup_hint_ip_address_empty);
        $('#dmz_ip_address').focus();
        return false;
    }
    if (false == isSameBeforThreeAddrs(dmzIP, g_dhcpData.DhcpIPAddress))
    {
        showErrorUnderTextbox('dmz_ip_address', dialup_hint_ip_address_empty);
        $('#dmz_ip_address').focus();
        return false;
    }
    if (false == isBroadcastOrNetworkAddress(dmzIP, g_dhcpData.DhcpLanNetmask)) {
        showErrorUnderTextbox('dmz_error', dialup_hint_ip_address_empty);
        $('#dmz_ip_address').focus();
        return false;
    }
    if (dmzIP == g_dhcpData.DhcpIPAddress) {
        showErrorUnderTextbox('dmz_error', dialup_hint_ip_address_empty);
        $('#dmz_ip_address').focus();
        return false;
    }
    return true;
}

function apply() {
    clearAllErrorLabel();
    if (!isButtonEnable('apply_button'))
    {
        return;
    }
    var dmz_status = $("[name='dmz_status']:checked").val();
    var dmz_ipaddress = $('#dmz_ip_address').val();
    var dmz_wanpath = $('#dmz_wanpath').val();
    var dmzData_submit = {
        'Dmz':{
            'WanPath' : dmz_wanpath,
            'DmzStatus' : dmz_status
        }
    };
    if('1' == dmz_status){
        if(!validateDmz()){
            return;
        }
        dmzData_submit.Dmz.DmzIPAddress = dmz_ipaddress;
      }
    var xmlstr = object2xml('request', dmzData_submit);
    saveAjaxData('api/security/dmz', xmlstr, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
        }
        else {
            button_enable('apply_button', '1');
            parent.showInfoDialog(common_failed);
        }
            initPage();
    });
}
$(document).ready(function() {
    $('input').bind('change input paste cut keydown', function() {
        button_enable('apply_button', '1');
    });
    $('select').bind('change', function() {
        button_enable('apply_button', '1');
    });
    $("[name='dmz_status']").click(function() {
        updateDmzRadioStatus();
    });
    $("#apply_button").click(function() {
        apply();
    });
    button_enable('apply_button', '0');
    initPage();
});