var g_postfix = window.location.search;
var simPageVar = {
    SIMPin: ''
};

var SIM_STATUS_LOCKED = 1;

function main_beforeready() {
    if (g_postfix != null) {
        var length = g_postfix.length;
        var start = g_postfix.indexOf('?');
        g_postfix = g_postfix.substring(start + 1, length) + '.html';
    }else {
        g_postfix = '';
    }
}

main_beforeready();

function initPage() {
    button_enable('simlock_button_apply', '0');

    getAjaxData('api/pin/simlock', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            if (SIM_STATUS_LOCKED == ret.response.SimLockEnable) {
                $('#label_SimlockTimes').text(ret.response.SimLockRemainTimes);
                if (parseInt(ret.response.SimLockRemainTimes, 10) < 1) {
                    $('#input_simunlock').attr('disabled', 'disabled');
                }else {
                    $('#input_simunlock').removeAttr('disabled');
                    $('#input_simunlock').val('');
                    $('#input_simunlock').focus();
                }
            }else {
                gotoPageWithoutHistory(HOME_PAGE_URL + window.location.search);
            }
        }
    });
}

function validateInput() {
    var pin_val = $('#input_simunlock').val();
    var patrn = /^[0-9]{8}$/;
    if (pin_val == '' || pin_val == ' ' || pin_val == null) {
        $('#simunlock_error').html(dialup_hint_password_empty).attr('class', 'error_message');
        $('#input_simunlock').focus();
        $('#input_simunlock').val('');
        button_enable('simlock_button_apply', '0');
        return false;
    }else if (!patrn.exec(pin_val)) {
        $('#simunlock_error').html(dialup_hint_simlock_code_valid_type).attr('class', 'error_message');
        $('#input_simunlock').focus();
        $('#input_simunlock').val('');
        button_enable('simlock_button_apply', '0');
        return false;
    }else {
        $('#simunlock_error').html('');
        return true;
    }
}

function onApply() {
    $.each($('input'), function() {
        $(this).blur();
    });
    if (!isButtonEnable('simlock_button_apply')) {
        return;
    }

    if (validateInput()) {
        var sim_pin = encrypt($('#input_simunlock').val());
        var request = {
            SimLockCode: sim_pin
        };
        var wps_xml = object2xml('request', request);

        button_enable('simlock_button_apply', '0');
        saveAjaxData('api/pin/verify-simlock', wps_xml, function($xml) {
            var return_ret = xml2object($xml);
            if (isAjaxReturnOK(return_ret)) {
                log.debug('SIMLOCKREQUIRED : simlock validate success.');
                if (('.html' == g_postfix) || ('' == g_postfix)) {
                    gotoPageWithoutHistory(HOME_PAGE_URL);
                }else {
                    gotoPageWithoutHistory(g_postfix);
                }
            }else {
                showInfoDialog(sim_lock_validate_failed);
                setTimeout(function() {
                    initPage();
                    return false;
                }, 1500);
            }
        });
    }
}

$(document).ready(function() {

    $('#input_simunlock').focus();

    $('#input_simunlock').bind('keyup change input paste cut keydown', function() {
        if (($(this).val()).length >= 8) {
            button_enable('simlock_button_apply', '1');
        }else {
            button_enable('simlock_button_apply', '0');
        }
    });
    $('#simlock_button_apply').bind('click', onApply);

    if (1 == g_feature.continue_button)
    {
        $('#link_login').show();
    }
    else
    {
        $('#link_login').hide();
    }

    initPage();
    setLangList();
    showCurrentLanguage();
    $('#link_login').click(function() {
         window.location = LOGIN_PAGE_URL;
    });
});
