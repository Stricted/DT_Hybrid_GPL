var WANLMACFILTER_FILTER_STATUS_DISABLED = 2;
var WANLMACFILTER_FILTER_STATUS_DENY = 0;

var g_ssid_array = [];
var g_array = [];
var Wifienable = parent.G_MonitoringStatus.response.WifiStatus;
var g_current_mode = 0;
var g_ssid = 0;
var g_wlanmacfilert_filterStatusArray = [
        [0, wlan_label_deny],
        [1, wlan_label_allow],
        [2, common_disable]
    ];
function initWlanMacModeSelect(){
    var i = 0;
    var options = document.getElementById('wlan_mac_mode_select').options;
    for (i=0;i<g_wlanmacfilert_filterStatusArray.length;i++) {
        var curStatus = g_wlanmacfilert_filterStatusArray[i];
        options.add(new Option(curStatus[1], curStatus[0]));
    }
}
function setSSID(ssid_index){
    ssid_index = parseInt(ssid_index,10);
    var i = 0;
    if($.isArray(g_ssid_array)){
        i = 2-parseInt(g_ssid_array[ssid_index].MacFilterPolicy,10);
    } else {
        i = 2-parseInt(g_ssid_array.MacFilterPolicy,10);
    }
    setMACMode(ssid_index,i);
}
function setMACMode(ssid_index,select_mode){
    if($.isArray(g_ssid_array)){
        if(g_ssid_array[ssid_index].WLANMACFilterEnable==0) {
            $('#wlan_mac_mode_select').val(2);
            $('#wlan_mac_list :input').attr('disabled',true);
        } else {
            $('#wlan_mac_mode_select').val(2-select_mode);
            if(select_mode) {
                $('#wlan_mac_list :input').removeAttr('disabled');
                $(g_array[ssid_index]).each(function(i){
                    if(i>=16*(select_mode-1)&&i<16*(select_mode)) {
                        $('#wlan_mac_filter_input'+i%16).val(g_array[ssid_index][i]);
                    }
                });
            } else {
                $('#wlan_mac_list :input').attr('disabled',true);
            }
            
        }
    } else {
        if(g_ssid_array.WLANMACFilterEnable==0) {
            $('#wlan_mac_mode_select').val(2);
            $('#wlan_mac_list :input').attr('disabled',true);
        } else {
            $('#wlan_mac_mode_select').val(2-select_mode);
            if(select_mode) {
                $('#wlan_mac_list :input').removeAttr('disabled');
                $(g_array[0]).each(function(i){
                    if(i>=16*(select_mode-1)&&i<16*(select_mode)) {
                        $('#wlan_mac_filter_input'+i%16).val(g_array[0][i]);
                    }
                });
            } else {
                $('#wlan_mac_list :input').attr('disabled',true);
            }
        }
    }
}
function changeMACMode(ssid_index,select_mode){
     if($.isArray(g_ssid_array)){
        $('#wlan_mac_mode_select').val(2-select_mode);
        if(select_mode) {
            $('#wlan_mac_list :input').removeAttr('disabled');
            $(g_array[ssid_index]).each(function(i){
                if(i>=16*(select_mode-1)&&i<16*(select_mode)) {
                    $('#wlan_mac_filter_input'+i%16).val(g_array[ssid_index][i]);
                }
            });
        } else {
            $('#wlan_mac_list :input').attr('disabled',true);
            var i=0;
            for(i=0;i<16;i++)
            {
                $('#wlan_mac_filter_input'+i).val("");
            }
        }
    } else {
        $('#wlan_mac_mode_select').val(2-select_mode);
        if(select_mode) {
            $('#wlan_mac_list :input').removeAttr('disabled');
            $(g_array[0]).each(function(i){
                if(i>=16*(select_mode-1)&&i<16*(select_mode)) {
                    $('#wlan_mac_filter_input'+i%16).val(g_array[0][i]);
                }
            });
        } else {
            $('#wlan_mac_list :input').attr('disabled',true);
            var i=0;
            for(i=0;i<16;i++)
            {
                $('#wlan_mac_filter_input'+i).val("");
            }
        }
    }
}
function initPage(){
    g_ssid_array.splice(0, g_ssid_array.length);
    g_array.length = 0;
    var firewall_status = '';
    $('#wlan_mac_list :input').removeAttr('value');
    getAjaxData('api/wlan/wlan-mac-filter', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type&& typeof(ret.response.SSIDs)!='undefined' && typeof(ret.response.SSIDs.SSID)!='undefined') {
            $('#wlan_mac_list :input').removeAttr('disabled');
            $('#wlan_ssid_mode_select').removeAttr('disabled');
            $('#wlan_mac_mode_select').removeAttr('disabled');
            g_ssid_array = ret.response.SSIDs.SSID;
            if($.isArray(g_ssid_array)) {
                var i = 0;
                for(i=0;i<g_ssid_array.length;i++) {
                    g_array.push(g_ssid_array[i].WLANMACFilter.SourceMACAddress);
                }
                g_current_mode = 2-parseInt(g_ssid_array[0].MacFilterPolicy,10);
            } else {
                g_array.push(g_ssid_array.WLANMACFilter.SourceMACAddress);
                g_current_mode = 2-parseInt(g_ssid_array.MacFilterPolicy,10);
            }
            g_ssid = 0;
            setSSID(0);
        } else {
            return;
        }
        initWlanSSidSelected();
    });
}
function WlanMacModeChange(){
    var ssid = parseInt($('#wlan_ssid_mode_select').val(),10);
    var wlanmacselectid = parseInt($('#wlan_mac_mode_select').val(),10);
    $('#wlan_mac_list :input').removeAttr('disabled');
    //setMACMode(g_ssid,2-wlanmacselectid);
    changeMACMode(g_ssid,2-wlanmacselectid);
    g_current_mode = 2-wlanmacselectid;
    button_enable('apply', '1');
}
function WlanSSidModeChange(){
    var ssid = $('#wlan_ssid_mode_select').val();
    var wlanmacselectid = g_ssid_array[ssid].MacFilterPolicy;
    $('#wlan_mac_list :input').removeAttr('disabled');
    $('#wlan_mac_mode_select').val(wlanmacselectid);
    $('#wlan_mac_list :input').removeAttr('value');
    setSSID(ssid);
    g_current_mode = 2-wlanmacselectid;
    g_ssid = ssid;
    button_enable('apply', '0');
}
function lanmacfilert_checkAllMac(){
    var checkResult = true;
    var emptyMacCount = 0;
    var MacAddress = [];
    var i = 0;
    var j = 0;
    clearAllErrorLabel();
    $('#wlan_mac_list :input').each(function(i) {
        if ((($.trim($(this).val()).length > 0) && (!isValidMacAddress($(this).val())))
        || (0 == $.trim($(this).val()).length && $(this).val().length > 0)) {
            $(this).focus();
            showErrorUnderTextbox('wlan_mac_filter_input' + i, wlan_hint_mac_address_invalid);
            button_enable('apply', '0');
            MacAddress = [];
            checkResult = false;
            return;
        }
        else if (0 == $(this).val().length) {
            emptyMacCount++;
        }
        else {
            MacAddress.push(i);
        }
    });
    if (16 == emptyMacCount) {       
       
       if($('#wlan_mac_mode_select').val() == WANLMACFILTER_FILTER_STATUS_DISABLED ||
          $('#wlan_mac_mode_select').val() == WANLMACFILTER_FILTER_STATUS_DENY) {
           return true;
       } else {
           parent.showInfoDialog(wlan_hint_mac_address_invalid);
           $('#wlan_mac_filter_input0').focus();
           return false;
       }
              
    }
    if(MacAddress.length > 1){
        for(i=0;i<MacAddress.length-1;i++){
            for(j=i+1;j<MacAddress.length;j++){
                if(issameRule($('#wlan_mac_filter_input'+MacAddress[i]).val(),$('#wlan_mac_filter_input'+MacAddress[j]).val())){
                    showErrorUnderTextbox('wlan_mac_filter_input' + MacAddress[j], same_mac_address);
                    $('#wlan_mac_filter_input' + MacAddress[j]).focus();
                    button_enable('apply', '0');
                    return false;
                }
            }
        }
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
function getText(){
    var i = 0;
    var arr = [];
    for(i=0;i<16;i++) {
        arr.push($('#wlan_mac_filter_input'+i).val());
    }
    return arr;
}
function postData(){
    var newXmlString = '';
    var submitObject = null;
    var SSidText = $('#wlan_ssid_mode_select option:selected').text();
    var wlanmacselectid = parseInt($('#wlan_mac_mode_select').val(),10);
    if(WANLMACFILTER_FILTER_STATUS_DISABLED == wlanmacselectid){
        submitObject = {
            SSIDName : SSidText,
            WLANMACFilterEnable : 0
        };
    } else {
        submitObject = {
            SSIDName : SSidText,
            WLANMACFilterEnable : 1,
            MacFilterPolicy : wlanmacselectid,
            WLANMACFilter : {
                SourceMACAddress : getText()
            }
        };
    }
    newXmlString = object2xml('request', submitObject);
    saveAjaxData('api/wlan/wlan-mac-filter', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            button_enable('apply', '0');
        } else {
            parent.showInfoDialog(common_failed);
        }
        initPage();
    });
}
function onApply(){
    if(lanmacfilert_checkAllMac()){
        parent.showConfirmDialog(firewall_hint_submit_list_item, postData,function(){});
    }
}
function initWlanSSidSelected(){
    $('#wlan_ssid_mode_select').children().remove();
    var options = document.getElementById('wlan_ssid_mode_select').options;
    if($.isArray(g_ssid_array)){
        $(g_ssid_array).each(function(i){
            var curStatus = g_ssid_array[i].SSIDName;
            options.add(new Option(curStatus, i));
        });
    } else {
        var curStatus = g_ssid_array.SSIDName;
        options.add(new Option(curStatus, 0));
    }
    $('#wlan_ssid_mode_select').val(0);
}

/*------------------show turn on button---------------*/
function showBtnOrContent() {
    if(Wifienable == 1) {
        initWlanMacModeSelect();
        initPage();
        $("#show_turn_on_wlan").hide();
        $("#show_waln_mac_content").show();
    }
    else {
        $("#show_turn_on_wlan").show();
        $("#show_waln_mac_content").hide();
    }
}

$(document).ready(function() {
    showBtnOrContent();
    $('#apply').click(function() {
        if (isButtonEnable('apply')) {
            onApply();
        }
    });
    $('#wlan_mac_mode_select').bind('change',function(){
        WlanMacModeChange();
    });
    $('#wlan_ssid_mode_select').bind('change',function(){
        WlanSSidModeChange();
    });
    $('#wlan_mac_list :input').bind('change input paste cut keydown',function(){
        button_enable('apply', '1');
    });
    
    //click turn on button
    $("#wlan_turn_on").click(function() {
        Wifienable = 1;
        var ret = {
                'Wifienable': Wifienable
            }
        var req = object2xml("request",ret);
        saveAjaxData("api/wlan/wifienable",req,function($xml) {
            var ret = xml2object($xml);
            if(isAjaxReturnOK(ret)) {
                button_enable('wlan_turn_on', '0');
                initWlanMacModeSelect();
                initPage();
                $("#show_turn_on_wlan").hide();
                $("#show_waln_mac_content").show();
            }
            else {
                return;
            }
        });
    });
});