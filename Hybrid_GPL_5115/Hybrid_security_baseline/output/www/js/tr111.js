var g_stun_set = 0;
var MAX_PERIOD= 2147483647;

function isStunChecked(){
    if($("#tr111_stun_check").attr("checked")){
        g_stun_set = 1;
    }else{
        g_stun_set = 0;
    }
}

/*----------------save tr111 setting info------------------*/
function validate (){
    var min = $.trim($("#id_interval_min").val());
    var max = $.trim($("#id_interval_max").val());
    var port = $("#id_port").val();
    var username = $("#id_username").val();
    var password = $("#id_password").val();

    clearAllErrorLabel();
    if(!validate_interval(min))
    {
        showErrorUnderTextbox('id_interval_min', trlll_err_overvalue);
        return false;
    }

    if(!validate_interval(max))
    {
        showErrorUnderTextbox('id_interval_max', trlll_err_overvalue);
        return false;
    }
    if(parseInt(min,10) > parseInt(max,10)){
        parent.showInfoDialog(tr111_err_minbiger);
        return false;
    }
    if(!validate_address())
    {
        showErrorUnderTextbox('id_address', tr111_err_address);
        return false;
    }
    if(!validate_interval(port))
    {
        showErrorUnderTextbox('id_port', tr111_err_port);
        return false;
    }

    if(!validate_string(username))
    {
        showErrorUnderTextbox('id_username', tr111_err_username);
        return false;
    }

    if(!validate_string(password))
    {
        showErrorUnderTextbox('id_password', tr111_err_password);
        return false;
    }
    return true;
}

function saveTR111Setting() {
    if(validate ())
    {
        var min = $("#id_interval_min").val();
        var max = $("#id_interval_max").val();
        var address = $("#id_address").val();
        var port = $("#id_port").val();
        var username = $("#id_username").val();
        var password = encrypt($("#id_password").val());
        var trsettinginfo = {
            STUNEnable : g_stun_set,
            STUNMinimumKeepAlivePeriod : min,
            STUNMaximumKeepAlivePeriod : max,
            STUNServerAddress : address,
            STUNServerPort : port,
            STUNUsername : username,
            STUNPassword : password
        };
        var xmlinfo = object2xml("request",trsettinginfo);
        saveAjaxData("api/settings/tr111",xmlinfo,function($xml){
            var ret = xml2object($xml);
            if(isAjaxReturnOK(ret)){
                button_enable('tr111_apply', '0');
                parent.showInfoDialog(common_success);
            }
            else{
                parent.showInfoDialog(common_failed);
                getTR111Info();
            }
        });
    }
    else
    {
        return;
    }
}
/*-----------------------pro tr111info---------------*/
function getTR111Info(){
    getAjaxData("api/settings/tr111", function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response")
        {
            g_stun_set = ret.response.STUNEnable;
            $('#tr111_stun_check').get(0).checked=(g_stun_set == 1)?true:false;
            $('#id_interval_min').val(ret.response.STUNMinimumKeepAlivePeriod);
            $('#id_interval_max').val(ret.response.STUNMaximumKeepAlivePeriod);
            $('#id_address').val(ret.response.STUNServerAddress);
            $('#id_port').val(ret.response.STUNServerPort);
            $('#id_username').val(ret.response.STUNUsername);
            $('#id_password').val(ret.response.STUNPassword == '' ? '' : decrypt(ret.response.STUNPassword));
        }
    });
}

function isValidIpAddress(address) 
{
    var addrParts = address.split('.');
    if (addrParts.length != 4)
    {
        return false;
    }

    for (i = 0; i < 4; i++)
    {
        if ((addrParts[i] == '*') && (i != 3))
        {
        if (addrParts[i + 1] != '*')
        {
            return false;
        }
        }
        if (true == (isNaN(addrParts[i])) && (addrParts[i] != '*'))
        {
            return false;
        }

        if ('' == addrParts[i])
        {
            return false;
        }

        if (addrParts[i].indexOf(' ') != -1)
        {
            return false;
        }

        if ((0 == addrParts[i].indexOf('0')) && (1 != addrParts[i].length))
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
    
function validate_interval(integer){
    var int_rule = /^[0-9]{1,}$/;
    if('' == integer)
    {
        return false;
    }
    if(!int_rule.exec(integer)){
        return false;
    }
    if(parseInt(integer,10) > MAX_PERIOD){
        return false;
    }
    return true;
}

function validate_address(){
    var address =  $.trim($("#id_address").val());
    if(''==address){
        return false;
    }
    if (!isValidIpAddress(address)){
        return false;
    }
    return true;
}

function validate_string(str){
    var str_rule = /^[a-zA-Z0-9_]{1,}$/;
    if('' == str){
        return false;
    }
    if(!str_rule.exec(str)){
        return false;
    }
    return true;
}

$(document).ready(function(){
    $("#tr111_stun_check").bind("click",isStunChecked);
    $("#tr111_apply").click(function(){
        if (isButtonEnable('tr111_apply')) {
            saveTR111Setting();
        }
    });
    getTR111Info();
    button_enable('tr111_apply', '0');
    $('input').bind('change input paste cut keydown', function() {
        button_enable('tr111_apply', '1');
    });
});
