var g_array_init_address = [];
var g_current_mode = 0;
function setDisabled(){
    $('#lanmacid_mac_list :input').attr('disabled',true);
    $('#lanmacid_mode_select').attr('disabled',true);
}

function initText(filter_mode){
    filter_mode = parseInt(filter_mode, 10);
    $(g_array_init_address).each(function(i){
        if(g_current_mode != filter_mode && i<16) {
            g_array_init_address.splice((g_current_mode-1)*16+i, 1, $('#lanmacid_lanmacfilter_input'+i).val());
        }
        if(i >= 16*(filter_mode-1) && i < 16*filter_mode) {
            $('#lanmacid_lanmacfilter_input' + i%16).val(g_array_init_address[i]);
        }
    });
    g_current_mode = filter_mode;
}

function initPage() {
    g_array_init_address.length = 0;
    getAjaxData('api/security/firewall-switch', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            firewall_status = ret.response;
        } else {
            return;
        }
    }, {
        sync: true
    });
    getAjaxData('api/security/lan-mac-filter', function($xml) {
        var profile_ret = xml2object($xml);
        if (profile_ret.type == 'response') {
            if(typeof(profile_ret.response.MacFilterPolicy)!='undefined'&& typeof(profile_ret.response.MacFilter)!='undefined'&& typeof(profile_ret.response.MacFilter.SourceMACAddress)!='undefined'){
                g_array_init_address = profile_ret.response.MacFilter.SourceMACAddress;
                $('#lanmacid_mode_select').val(profile_ret.response.MacFilterPolicy);
                g_current_mode = parseInt(profile_ret.response.MacFilterPolicy,10);
                initText(profile_ret.response.MacFilterPolicy);
            } else {
                return;
            }
        }
        if (firewall_status != null && 0 == firewall_status.FirewallMainSwitch) {
            setDisabled();
            parent.showInfoDialog(firewall_label_lan_mac_address_filter_enable);
            return false;
        }
        if (firewall_status != null && 0 == firewall_status.FirewallLanMacFilterSwitch) {
            setDisabled();
            parent.showInfoDialog(IDS_security_message_lan_mac_address_disabled);
            return false;
        }
    });
    
}

function setData(temp){
    var i;
    if(temp.length != 0){
        for(i=0;i<temp[0].length;i++){
            $('#lanmacid_lanmacfilter_input'+i).val(temp[0][i].SourceMACAddress);
        }   
    }
}

$(document).ready(function() {
    button_enable('lanmacfilter_apply', '0');
    // init macfilter status options
    initPage();
    
    $('#lanmacid_mode_select').bind('change', function() {
        var selectedStatus = $('#lanmacid_mode_select').val();
        $('.error_message').remove();
        initText(selectedStatus);
        button_enable('lanmacfilter_apply', '1');
    });
    
    $('#lanmacid_mac_list :input').bind('input change cut paste keydown', function() {
        button_enable('lanmacfilter_apply', '1');
    });
    
    $('#lanmacfilter_apply').click(function() {
        if (!isButtonEnable('lanmacfilter_apply')) {
            return;
        }
        clearAllErrorLabel();
        if(lanmacfilert_checkAllMac()){
            postData();
        }
    });
});

function lanmacfilert_checkAllMac(){
    var checkResult = true;
    var emptyMacCount = 0;
    var MacAddress = [];
    var i ;
    var j ;
    $('#lanmacid_mac_list :input').each(function(i) {
        if ((($.trim($(this).val()).length > 0) && (!lanmacfilter_isValidMacAddress($(this).val())))
        || (0 == $.trim($(this).val()).length && $(this).val().length > 0)) {
            $(this).focus();
            showErrorUnderTextbox('lanmacid_lanmacfilter_input' + i, wlan_hint_mac_address_invalid);
            button_enable('lanmacfilter_apply', '0');
            MacAddress = [];
            checkResult = false;
            return checkResult;
        }
        else if (0 == $(this).val().length) {
            emptyMacCount++;
        }
        else {
            MacAddress.push(i);
        }
    });
    /*if (16 == emptyMacCount) {
        parent.showInfoDialog(wlan_hint_mac_address_invalid);
        $('#lanmacid_lanmacfilter_input0').focus();
        checkResult = false;
        return checkResult;
    }*/
    if(MacAddress.length > 1){
        for(i=0;i<MacAddress.length-1;i++){
            for(j=i+1;j<MacAddress.length;j++){
                if(issameRule($('#lanmacid_lanmacfilter_input'+MacAddress[i]).val(), $('#lanmacid_lanmacfilter_input'+MacAddress[j]).val())){
                    showErrorUnderTextbox('lanmacid_lanmacfilter_input' + MacAddress[j], same_mac_address);
                    $('#lanmacid_lanmacfilter_input' + MacAddress[j]).focus();
                    button_enable('lanmacfilter_apply', '0');
                    checkResult = false;
                    return checkResult;
                }
            }
        }
    }
    if(checkResult){
        MacAddress = [];
    }
    return checkResult;
}
function issameRule(strVariable1,strVariable2)
{
    if($.trim(strVariable1.toUpperCase())==$.trim(strVariable2.toUpperCase()))
    {
        return true;
    }
    return false;
}

function getData(){
    var lanMACFilterArray = [];
    var i;
    var submitObject;
    for(i=0;i<16;i++){
        lanMACFilterArray.push($('#lanmacid_lanmacfilter_input'+i).val());
    }
    submitObject = {
        MacFilterPolicy:$('#lanmacid_mode_select').val(),
        MacFilter: {
            SourceMACAddress:lanMACFilterArray
        }
    };
    return submitObject;
}

function postData() {
    var address;
    var newXmlString = object2xml('request', getData());
    saveAjaxData('api/security/lan-mac-filter', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            button_enable("lanmacfilter_apply", '0');
        }else{
            parent.showInfoDialog(common_failed);
        }
        initPage();
    });
}

function lanmacfilter_isValidMacAddress(macAddress) {
    macAddress = macAddress.toLowerCase();
    var c = 0;
    var i = 0, j = 0;

    if ('ff:ff:ff:ff:ff:ff' == macAddress || '00:00:00:00:00:00' == macAddress) {
        return false;
    }

    var addrParts = macAddress.split(':');
    if (addrParts.length != 6) {
        return false;
    }
    for (i = 0; i < 6; i++) {
        if (addrParts[i].length != 2) {
            return false;
        }

        for (j = 0; j < addrParts[i].length; j++) {
            c = addrParts[i].charAt(j);
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                return false;
            }
        }
    }

    c = parseInt(addrParts[0].charAt(1), 16);
    if (c % 2)
    {
        return false;
    }

    return true;
}