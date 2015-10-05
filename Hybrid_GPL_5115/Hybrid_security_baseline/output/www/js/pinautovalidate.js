// JavaScript Document
/**************************Function for PIN code auto validation page********************************/
var MACRO_SIM_LOCK_ENABLE = '1';
var MACRO_PIN_OPERATE_VALIDATE = 0;
var MACRO_PUK_UNLOCK_OPERATE = 4;
var send_request = {
    OperateType: MACRO_PUK_UNLOCK_OPERATE,
    CurrentPin: '',
    NewPin: '',
    PukCode: ''
};


var pin_status = {
    SimState: '',
    PinOptState: '',
    SimPinTimes: '',
    SimPukTimes: ''
};

var pinautovalidate_getPinData = {
    SimSavepinStatus: '',
    SimSavepinScid: ''
};

var pinautovalidate_savePinData = {
    SimSavepinStatus: '',
    SimSavepinPIN: ''
};
var remaining_times = 0;

function checkButtonEnable()
{
    if (($('#auto_pincode').val()).length >= 4 || 0 == $('input:checked').val()) {
            button_enable('validate_apply', '1');
    }
    else {
        button_enable('validate_apply', '0');
    }
}

function updatePinInputEnable(PinOptState)
{
        if (MACRO_PIN_DISABLE == PinOptState)  //Pin operation disabled
        {
            $('#auto_pincode').attr('disabled', 'disabled').val('');
            $("input[name='auto_validation']").attr('disabled', 'disabled');
        }
        else if ($('input:checked').val() == 1)
        {
            $('#auto_pincode').removeAttr('disabled');
            $('#auto_pincode').val('');
            $('#auto_pincode').focus();
        }
        else
        {
            $('#auto_pincode').attr('disabled', 'disabled').val('');
        }
}


function validateEnabled() {
    $('.autovalidate_enable').attr('checked', 'checked');
    $('#current_status').html(dialup_label_auto_validation_enabled);
}

function validateDisabled() {
    $('.autovalidate_disable').attr('checked', 'checked');
    $('#current_status').html(dialup_label_auto_validation_disabled);
}

function validateFormat() {
    if ($('input:checked').val() == 0)
    {
        return true;
    }
    var auto_pincode = $('#auto_pincode').val();
    var patrn = /^[0-9]{4,8}$/;
    if (auto_pincode == '' || auto_pincode == ' ' || auto_pincode == null)
    {
        showErrorUnderTextbox('auto_pincode', dialup_hint_current_pin_empty);
        return false;
    }
    else if (!patrn.exec(auto_pincode))
    {
        showErrorUnderTextbox('auto_pincode', dialup_hint_pin_code_valid_type);
        $(this).val('');
        return false;
    }
    return true;
}

function checkPukRequired()
{
    var pinstatus_ret;
    getAjaxData('api/pin/status', function($xml) {
        pinstatus_ret = xml2object($xml);
    },
    {
        sync: true
    }
    );
   if (pinstatus_ret.type == 'response')
    {
        pin_status.SimState = pinstatus_ret.response.SimState;
        pin_status.PinOptState = pinstatus_ret.response.PinOptState;
        pin_status.SimPinTimes = pinstatus_ret.response.SimPinTimes;
        pin_status.SimPukTimes = pinstatus_ret.response.SimPukTimes;

        if (pin_status.SimState == MACRO_NO_SIM_CARD)
        {
            gotoPageWithoutHistory("nocard.html");
        }
        else if (MACRO_PUK_REQUIRED == pin_status.SimState)
        {
            log.debug('REDIRECT : SimState == MACRO_PUK_REQUIRED, redirect to pukrequired.html');
            gotoPageWithoutHistory('pukrequired.html' + "?setting");
        }
        else if (MACRO_CARD_LOCK_PIN_RIGNT == pin_status.SimState) 
        {
            gotoPageWithoutHistory('cardlock.html');
        }
        else if (MACRO_CARD_LOCK_PIN_WRONG == pin_status.SimState) 
        {
            gotoPageWithoutHistory('carderror.html');
        }
        else if(MACRO_INIT_SIM_CARD == pin_status.SimState)
        {
            gotoPageWithoutHistory("cardinit.html" + "?setting");
        }
        else if(MACRO_CPIN_FAIL == pin_status.SimState)
        {
            gotoPageWithoutHistory("nocard.html");
        }
        else
        {
            $('.main_content').show();
            $('#remaining_times').html(pin_status.SimPinTimes);
             getAjaxData('api/pin/save-pin', function($xml) {
                var ret = xml2object($xml);
                if (ret.type == 'response')
                {
                    pinautovalidate_getPinData = ret.response;
                    if (pinautovalidate_getPinData.SimSavepinStatus == 1)
                    {
                        validateEnabled();
                    }
                    else
                    {
                        validateDisabled();
                    }
                    updatePinInputEnable(pin_status.PinOptState);
                }
              },
            {
                sync: true
            });
         }
     }
}



function initPage()
{
    if(MACRO_NO_SIM_CARD == parent.G_MonitoringStatus.response.SimStatus)
    {
        gotoPageWithoutHistory("nocard.html");
        return true;
    }
    else if(parent.G_MonitoringStatus.response.SimStatus==0 && parent.G_MonitoringStatus.response.simlockStatus==1)
    {
        gotoPageWithoutHistory("simlock.html");
        return true;
    }
    checkPukRequired();
    button_enable('validate_apply', '0');


}

function onApply()
{
    if (!isButtonEnable('validate_apply'))
    {
        return;
    }
    clearAllErrorLabel();

    if (validateFormat())
    {
        pinautovalidate_savePinData.SimSavepinStatus = $('input:checked').val();
        pinautovalidate_savePinData.SimSavepinPIN = encrypt($('#auto_pincode').val());
        var xmlstr = object2xml('request', pinautovalidate_savePinData);
        saveAjaxData('api/pin/save-pin', xmlstr, function($xml) {
            var request_ret = xml2object($xml);
            if (isAjaxReturnOK(request_ret))
            {
                parent.showInfoDialog(common_success);
            }
            else
            {
                parent.showInfoDialog(common_failed);
            }
            setTimeout(function() {
                    initPage();
            }, g_feature.dialogdisapear);
        });
           button_enable('validate_apply', '0');
    }
}
function main_executeBeforeDocumentReady() {
    //get Simlock status
    var currentStatus = parent.G_MonitoringStatus; 
    if (   currentStatus != null 
        && typeof(currentStatus.response) != 'undefined'
        && typeof(currentStatus.response.SimStatus) != 'undefined'
        && typeof(currentStatus.response.simlockStatus) != 'undefined'
        )
    {
    }
    else
    {
        //get Simlock status
        getAjaxData('api/monitoring/status', function($xml) {
            currentStatus = xml2object($xml);
        },{sync :true});
    
    }   
    if(currentStatus.response.SimStatus == MACRO_NO_SIM_CARD)
    {
        gotoPageWithoutHistory("nocard.html");
    }   
    else if(currentStatus.response.SimStatus==0 && currentStatus.response.simlockStatus==1)
    {
        gotoPageWithoutHistory("simlock.html?setting");
    }   
}

main_executeBeforeDocumentReady();
$(document).ready(function() {
    $('#validate_apply').bind('click', onApply);
    $('input[type=radio]').bind('click', updatePinInputEnable);
    $('input').bind('change input paste cut keydown keyup', function() {
        checkButtonEnable();
    });
    initPage();
});
