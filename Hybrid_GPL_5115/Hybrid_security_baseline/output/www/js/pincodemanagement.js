// JavaScript Document
/**************Pin Code Management Page Function***************/
var g_pinCofig = null;
var pin_status = {};
function isOptionEnable(option_id) {
    var flag = false;
    $('#' + option_id).parent('li').hasClass('disable_item') ? flag = false : flag = true;
    return flag;
}

function dispalyModify(){
    var button_id = '';
            clearAllErrorLabel();
            $('#pin_code').val('');
            $('#pin_code').focus();
            if (!isOptionEnable('pin_modify'))
            {
               $('#list_menu').slideUp(100);
               return false;
            }
            var modify_info = "<tr id='modify_new'><td>" + dialup_label_new_pin + common_colon + '</td>';
            modify_info += "<td><input type='password' id='newPin' name='newPin' maxlength='8' class='input_style' value=''/></td></tr>";
            modify_info += "<tr id='modify_confirm'><td>" + dialup_label_confirm_pin + common_colon + '</td>';
            modify_info += "<td><input type='password' id='confirmPin' maxlength='8' name='confirmPin' class='input_style' value=''/></td></tr>";
            $('tr').remove('#modify_new');
            $('tr').remove('#modify_confirm');
            $('tr:odd').after(modify_info);
            modify_info = '';
            $('.button_wrapper').attr('id', 'pinmanagerment_apply');
            button_id = $('.button_wrapper').attr('id');
            button_enable(button_id, '0');
            $('#pin_code').unbind();
            $('#pin_code,#newPin,#confirmPin').bind('keyup input paste cut', function() {
                    button_enable(button_id, '1');
            });
}
function pcm_exchange_modifyOrDisable() {
    $('#pin_code_select').change(function() {

        if ($('#pin_code_select').val() == common_modify)
        {
            dispalyModify();
        }
        else
        {
            clearAllErrorLabel();
            $('#pin_code').select().focus().val('');
            $('tr').remove('#modify_new');
            $('tr').remove('#modify_confirm');
            button_enable('pinmanagerment_apply', '0');
            $('#pin_code').unbind();
            $('#pin_code').bind('keyup input paste cut', function() {
                button_enable('pinmanagerment_apply', '1');
            });


        }
    });
}
function pcm_getPinStatus() {

    $("#pin_code_select option").remove();
 
    var option_items = '';
    if ((pin_status.PinOptState == MACRO_PIN_DISABLE) && (pin_status.SimState == MACRO_PIN_READY))
    {
        if (g_pinCofig.config.pin_enable == '1')
        {
            option_items += "<option>" + common_enable + "</option>";
        }
        $("#pin_code_select").append(option_items);
        setTimeout(function() {
                $('#pin_code_select').val(common_enable);
            }, 100);
    }
    else if (pin_status.PinOptState == MACRO_PIN_VALIDATE)
    {
        if (g_pinCofig.config.pin_disable == '1')
        {
            option_items += "<option>" + common_disable + "</option>";
        }
        if (g_pinCofig.config.pin_modify == '1')
        {
            option_items += "<option>" + common_modify + "</option>";
        }
        $("#pin_code_select").append(option_items);
        setTimeout(function() {
            $('#pin_code_select').val(common_disable);
        }, 100);
        if((g_pinCofig.config.pin_disable == '0') && (g_pinCofig.config.pin_modify == '1'))
        {
            dispalyModify();
            setTimeout(function() {
                $('#pin_code_select').val(common_modify);
            }, 100);
        }
    }
    else if (pin_status.PinOptState == MACRO_CPIN_FAIL)
    {
        if (g_pinCofig.config.pin_validate == '1')
        {
            option_items = "<option>" + dialup_label_validate + "</option>";
        }
        $("#pin_code_select").append(option_items);
        setTimeout(function() {
                $('#pin_code_select').val(dialup_label_validate);
            }, 100);
    }
}

function getPinEnable() {
    getConfigData('config/pincode/config.xml', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'config')
        {
            g_pinCofig = ret;
        }
    },{sync: true});
}

function pcm_validateOriPin() {
    //Original pin code
    var button_id = 'pinmanagerment_apply';
    var pin_val = $('#pin_code').val();
    var patrn = /^[0-9]{4,8}$/;
    if (pin_val == '' || pin_val == ' ' || pin_val == null)
    {
        showErrorUnderTextbox('pin_code', dialup_hint_pin_code_valid_type, 'ori_pin_wrong');
        $('#pin_code').val('');
        $('#pin_code').focus();
        button_enable(button_id, '0');
        return false;
    }
    else if (!patrn.exec(pin_val))
    {
        showErrorUnderTextbox('pin_code', dialup_hint_pin_code_valid_type, 'ori_pin_wrong');
        $('#pin_code').val('');
        $('#pin_code').focus();
        button_enable(button_id, '0');
        return false;
    }
    else
    {
        button_enable(button_id, '1');
        return true;
    }
}

function pcm_validateNewPin() {
    var button_id = 'pinmanagerment_apply';
    var new_pin = $('#newPin');
    var pin_val = new_pin.val();
    var patrn = /^[0-9]{4,8}$/;
    if (pin_val == '' || pin_val == ' ' || pin_val == null)
    {
        showErrorUnderTextbox('newPin', dialup_hint_pin_code_valid_type, 'new_pin_wrong');
        new_pin.val('');
        new_pin.focus();
        button_enable(button_id, '0');
        return false;
    }
    else if (!patrn.exec(pin_val))
    {
        showErrorUnderTextbox('newPin', dialup_hint_pin_code_valid_type, 'new_pin_wrong');
        new_pin.val('');
        new_pin.focus();
        button_enable(button_id, '0');
        return false;
    }
    else
    {
        button_enable(button_id, '1');
        return true;
    }
}

function pcm_validateConfirmPin() {
    var button_id = 'pinmanagerment_apply';
    var confirmPin = $('#confirmPin');
    var pin_val = confirmPin.val();
    var patrn = /^[0-9]{4,8}$/;
    if (pin_val == '' || pin_val == ' ' || pin_val == null)
    {
        showErrorUnderTextbox('confirmPin', dialup_hint_pin_code_valid_type, 'confirm_pin_wrong');
        confirmPin.val('');
        confirmPin.focus();
        button_enable(button_id, '0');
        return false;
    }
    else if (!patrn.exec(pin_val))
    {
        showErrorUnderTextbox('confirmPin', dialup_hint_pin_code_valid_type, 'confirm_pin_wrong');
        confirmPin.val('');
        confirmPin.focus();
        button_enable(button_id, '0');
        return false;
    }
    else if (pin_val != $('#newPin').val())
    {
        showErrorUnderTextbox('confirmPin', dialup_hint_confirm_and_new_same, 'confirm_pin_wrong');
        confirmPin.val('');
        confirmPin.focus();
        button_enable(button_id, '0');
        return false;
    }
    else
    {
        button_enable(button_id, '1');
        return true;
    }
}

function pcm_opreatFailed() {
    getAjaxData('api/pin/status', function($xml) {
        var pinstatus_ret = xml2object($xml);
        if (pinstatus_ret.type == 'response')
        {
            pin_status.SimState = pinstatus_ret.response.SimState;
            pin_status.PinOptState = pinstatus_ret.response.PinOptState;
            pin_status.SimPinTimes = pinstatus_ret.response.SimPinTimes;
            pin_status.SimPukTimes = pinstatus_ret.response.SimPukTimes;

            if (MACRO_PUK_REQUIRED == pinstatus_ret.response.SimState)
            {
                log.debug('REDIRECT : SimState == MACRO_PUK_REQUIRED, redirect to pukrequired.html');
                gotoPageWithoutHistory('pukrequired.html' + "?setting");

            }
            else if(MACRO_INIT_SIM_CARD == pinstatus_ret.response.SimState)
            {
                gotoPageWithoutHistory("cardinit.html" + "?setting");
            }
            else
            {
                $('#pin_code').val('');
                $('#newPin').val('');
                $('#confirmPin').val('');
                $('#remainingtime').html(pin_status.SimPinTimes);
                $('#pin_code').blur();
                setTimeout(function() {$('#pin_code').focus();}, 2500);
            }
        }
    });
}

function pcm_postOperate() {
    $.each($('input'), function() {
        $(this).blur();
    });
    var newPin = '';
    var currentPin = '';
    var operateType = '';


    if ($('#pin_code').val() != '' && $('#pin_code').val() != ' ' && $('#pin_code').val() != null)
    {
        currentPin = encrypt($('#pin_code').val());
    }
    if ($('#newPin').val() != '' && $('#newPin').val() != ' ' && $('#newPin').val() != null)
    {
        newPin = encrypt($('#newPin').val());
    }

    switch ($('#pin_code_select').val())
    {
        case common_modify:
            operateType = '3';
            break;
        case common_disable:
            operateType = '2';
            break;
        case common_enable:
            operateType = '1';
            break;
        case dialup_label_validate:
            operateType = '0';
            break;
        default:
            operateType = '4';
            break;
    }

        var request = {
        OperateType: operateType,
        CurrentPin: currentPin,
        NewPin: newPin,
        PukCode: ''
    };

    var requestStr = object2xml('request', request);
    saveAjaxData('api/pin/operate', requestStr, function($xml) {
        var return_ret = xml2object($xml);
        if (isAjaxReturnOK(return_ret))
        {
            parent.showInfoDialog(common_success);
            pcm_initPage();
        }
        else
        {
            // pin code validate failed
            parent.showInfoDialog(pin_code_validate_failed);
            pcm_opreatFailed();
            $('#pin_code').focus();
        }
    });
    button_enable('pinmanagerment_apply', '0');
}

function pcm_initPage()
{
    getAjaxData('api/pin/status', function($xml) {
        var pinstatus_ret = xml2object($xml);
        if (pinstatus_ret.type == 'response')
        {
            pin_status = pinstatus_ret.response;
        }
    },{sync: true});
    if (MACRO_CARD_LOCK_PIN_RIGNT == pin_status.SimState) {
        gotoPageWithoutHistory('cardlock.html');
    }
    
    $('#pin_code').select().focus().val('');
    $('#remainingtime').html(pin_status.SimPinTimes);
    $('tr').remove('#modify_new');
    $('tr').remove('#modify_confirm');

    pcm_getPinStatus();

    $('#pin_code').unbind();
    $('#pin_code').bind('keyup input paste cut', function() {
            button_enable('pinmanagerment_apply', '1');
        });
}

redirectOnCondition(null, 'setting');
getPinEnable();

$(document).ready(function() {
    
    pcm_initPage();

    $('#pinmanagerment_apply').die().live('click', function() {
        if(!isButtonEnable("pinmanagerment_apply")) 
        {
            return;
        }
        clearAllErrorLabel();
        var oriPinValidate = false;
        var newPinValidate = false;
        var confirmPinValidate = false;

        oriPinValidate = pcm_validateOriPin();
        if ($('#pin_code_select').val() == common_modify)
        {
            if (oriPinValidate){
                newPinValidate = pcm_validateNewPin();
            }
            if (newPinValidate)
            {
                confirmPinValidate = pcm_validateConfirmPin();
            }
        }else{
            var newPinValidate = true;
            var confirmPinValidate = true;
        }
        if (oriPinValidate && newPinValidate && confirmPinValidate)
        {
            pcm_postOperate();
        }
        else
        {
            button_enable('pinmanagerment_apply', '0');
        }

    });
    
    pcm_exchange_modifyOrDisable();

    $('#pop_confirm').die().live('click', function() {
        refresh();
    });

});