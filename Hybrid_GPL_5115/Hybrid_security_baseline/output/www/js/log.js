var MIN_PORT = 1;
var MAX_PORT = 65535;
var DEFAULT_PORT = 514;
var log_enable = 1;
var log_disable = 0;
var dms_status = '';
function checkLogEnable(){
    if($('#enable_log').attr('checked')){
        $('#log_settings').show();
        checkRadioStatus();
    } else {
        $('#log_settings').hide();
    }
}

function checkRadioStatus(){
    if(log_enable == $("[name='mode']:checked").val()){
        $('#log_primary_address').val(dms_status.PrimaryServerAddress).attr('disabled', false);
        $('#log_primary_port').val(dms_status.PrimaryServerPort).attr('disabled', false);
        $('#log_second_address').val(dms_status.SecondServerAddress).attr('disabled', false);
        $('#log_second_port').val(dms_status.SecondServerPort).attr('disabled', false);
    }else{
        $('#log_primary_address').val('').attr('disabled', true);
        $('#log_primary_port').val('').attr('disabled', true);
        $('#log_second_address').val('').attr('disabled', true);
        $('#log_second_port').val('').attr('disabled', true);
    }
}

function initPage(){
    getAjaxData('api/settings/log-info', function($xml) {
        var dms_ret = xml2object($xml);
        if (dms_ret.type == 'response') {
            dms_status = dms_ret.response;
            $('#enable_log').get(0).checked = (log_enable == dms_status.LogEnable);
            $('#log_level').attr("value",dms_status.LogLevel);
            $("input[name='mode'][value=" + dms_status.LogServer + ']').attr('checked', true);
            checkLogEnable();
        } else {
            return;
        }
    });
}
function getData(){
    var submitObject = null;
    if($('#enable_log').attr('checked')){
        if("1"==$("[name='mode']:checked").val()){
            submitObject = {
                LogEnable : log_enable,
                LogLevel : $('#log_level').attr("value"),
                LogServer : $("[name='mode']:checked").val(),
                PrimaryServerAddress : $('#log_primary_address').attr("value"),
                PrimaryServerPort : $.trim($('#log_primary_port').attr("value")),
                SecondServerAddress : $('#log_second_address').attr("value"),
                SecondServerPort : $.trim($('#log_second_port').attr("value"))
            };
        }
        else{
            submitObject = {
                LogEnable : log_enable,
                LogLevel : $('#log_level').attr("value"),
                LogServer : $("[name='mode']:checked").val()
            };
        } 
    }
    else {
        submitObject = {
            LogEnable : log_disable
        };
    }
    return submitObject;
}

function Amss_Reboot()
{
    var status = "1";
    if($('#enable_log').attr('checked'))
    {
       if("1"==$("[name='mode']:checked").val())
       {
            status = "0";
       }
    }
    var submitObject_Amss_Reboot = 
    {
        Status : status
    };
    var newXmlString = object2xml('request', submitObject_Amss_Reboot);
    saveAjaxData("api/settings/amss-reboot",newXmlString,function($xml){   
    });
}
function postData(){
    Amss_Reboot();
    var newXmlString = object2xml('request', getData());
    saveAjaxData('api/settings/log-info', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            initPage();
            checkLogEnable();
            button_enable('apply', '0');
        }else{
            parent.showInfoDialog(common_failed);
        }
    });
}

function inputnullcheck(){
        if(log_enable ==  $("[name='mode']:checked").val()){
          if(($.trim($('#log_primary_address').val()) == '')
                &&($.trim($('#log_second_address').val()) == '' )){
                    if($.trim($('#log_primary_address').val()) == '')
                    {
                         showErrorUnderTextbox("log_primary_address", dialup_hint_ip_address_empty);
                          $('#log_primary_address').focus();
                     }else if($.trim($('#log_second_address').val()) == '')
                     {
                         showErrorUnderTextbox("log_second_address", dialup_hint_ip_address_empty);
                         $('#log_second_address').focus();
                      }
                    return false;
               }
    }
    return true;
}

function inputPortCheck(id){
    if(log_enable ==  $("[name='mode']:checked").val()){
        if("" == $.trim($('#'+id).val())){
            $('#'+id).attr("value",DEFAULT_PORT);
            return true;
        }
        if (-1 != $('#'+id).val().indexOf('.')){
            showErrorUnderTextbox(id, log_port_integer);
            $('#'+id).focus();
            return false;
        }
        else if (true == isNaN($('#'+id).val())){
            showErrorUnderTextbox(id, log_port_number);
            $('#'+id).focus();
            return false;
        }
        else if ((0 == $('#'+id).val().indexOf('0')) && (0 != $('#'+id).val())){
            showErrorUnderTextbox(id, log_port_invalid);
            $('#'+id).focus();
            return false;
        }
        else if (($('#'+id).val() < MIN_PORT) || ($('#'+id).val() > MAX_PORT)){
            showErrorUnderTextbox(id, log_port_range);
            $('#'+id).focus();
            return false;
        }
    }
    return true;
}
function inputAddressCheck(id){
    var str_ip = $('#'+id).val();
    if("" == str_ip){
        return true;
    }
    var str = /^((https|http|ftp|rtsp|mms)?:\/\/)?(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?(([0-9]{1,3}\.){3}[0-9]{1,3}|([0-9a-zA-Z_!~*'()-]+\.)*([0-9a-zA-Z][0-9A-Za-z-]{0,61})?[0-9A-Za-z]\.[a-z]{2,6})(:[0-9]{1,4})?((\/?)|(\/[0-9A-Za-z_!~*'().;?:@&=+$,%#-]+)+\/?)$/;
    if(isIpFormat(str_ip)){
        if (!isValidIpAddress(str_ip) || ( $('#log_primary_address').val() == $('#log_second_address').val())){
            showErrorUnderTextbox(id, dialup_hint_ip_address_empty);
            $('#'+id).focus();
            return false;
        }
    }else{
        if(!str.test(str_ip)){
            showErrorUnderTextbox(id, dialup_hint_ip_address_empty);
            $('#'+id).focus();
            return false;
        }
    }
    return true;
}
function onApply(){
          return inputnullcheck()&&inputAddressCheck('log_primary_address')&&inputPortCheck('log_primary_port')&&inputAddressCheck('log_second_address')&&inputPortCheck('log_second_port');
}
$(document).ready(function() {
    initPage();
    $('input[type=radio]').click(function(){
        checkRadioStatus();
        button_enable('apply', '1');
    });
    $('#enable_log').click(function() {
        checkLogEnable();
        button_enable('apply', '1');
    });
    $('#apply').click(function(){
        if (!isButtonEnable('apply')) {
            return;
        }
        clearAllErrorLabel();
        if($('#enable_log').attr('checked') && !onApply()){
            return;
        }
        parent.showConfirmDialog(firewall_hint_submit_list_item, postData);
    });
    $('.input_style,#log_level,#log_server').bind('change input paste cut keydown',function(){
        button_enable("apply", 1);
    });
});