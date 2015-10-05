var g_postfix = window.location.search;

// JavaScript Document
var MACRO_PUK_UNLOCK_OPERATE = 4;
var send_request = {
    OperateType: MACRO_PUK_UNLOCK_OPERATE,
    CurrentPin: '',
    NewPin: '',
    PukCode: ''
};
var g_username = ["admin","user"];
var g_user = 0;
var g_login_PasswordRemainTimes = 3;
var LOGINLOCK = 0;
var LOOP_TIME = 6000;


var g_pukcode_flag = false;
var g_newpin_flag = false;
var g_retypenewpin_flag = false;
var g_puk_first_login = 0;
function main_beforeready() {
    if (g_postfix != null)
    {
        var length = g_postfix.length;
        var start = g_postfix.indexOf('?');
        g_postfix = g_postfix.substring(start + 1, length) + '.html';
    } else {
        g_postfix = '';
    }

}

main_beforeready();

function initPage() {

    button_enable('pukrequired_apply', '0');

    getAjaxData('api/pin/status', function($xml) {
        var status_ret = xml2object($xml);
        if (status_ret.type == 'response') {
            if (MACRO_PUK_REQUIRED == status_ret.response.SimState) {
                $('#remain_times').text(status_ret.response.SimPukTimes);
                if (1 > parseInt(status_ret.response.SimPukTimes, 10)) {
                    $('#input_puk').attr('disabled', 'disabled');
                    $('#input_newpin').attr('disabled', 'disabled');
                    $('#input_confirmPin').attr('disabled', 'disabled');
                    $('#puk_blocked').show();
                    $('#puk_blocked').html(dialup_help_puk_locked);
                }else {
                    $('#input_puk').removeAttr('disabled');
                    $('#input_newpin').removeAttr('disabled');
                    $('#input_confirmPin').removeAttr('disabled');
                    $('#input_puk').val('');                          //clean input control
                    $('#input_newpin').val('');
                    $('#input_confirmPin').val('');
                    $('#input_puk').focus();                          //set the focus
                }
            }
            else if (MACRO_CARD_LOCK_PIN_RIGNT == status_ret.response.SimState) 
            {
                gotoPageWithoutHistory('cardlock.html');
            }
            else if (MACRO_CARD_LOCK_PIN_WRONG == status_ret.response.SimState) 
            {
                gotoPageWithoutHistory('carderror.html');
            }
            else if(MACRO_INIT_SIM_CARD == status_ret.response.SimState)
            {
                gotoPageWithoutHistory("cardinit.html" + window.location.search);
            }
            else {
                gotoPageWithoutHistory(LOGIN_PAGE_URL + window.location.search);
            }
        }
    });
}

function validateInput() {
    $('#puk_error').html('');
    $('#newpin_error').html('');
    $('#confirmPin_error').html('');
    var pukcode = $('#input_puk').val();
    var newpin = $('#input_newpin').val();
    var retypepin = $('#input_confirmPin').val();
    var pukpatrn = /^[0-9]{8}$/;
    var pinpatrn = /^[0-9]{4,8}$/;

    if ('' == pukcode) {
        $('#puk_error').html(dialup_hint_puk_code_valid_type).attr('class', 'error_message');
        $('#input_puk').focus();
        $('#input_puk').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else if (!pukpatrn.exec(pukcode)) {
        $('#puk_error').html(dialup_hint_puk_code_valid_type).attr('class', 'error_message');
        $('#input_puk').focus();
        $('#input_puk').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else {

    }

    if ('' == newpin) {
        $('#newpin_error').html(dialup_hint_pin_code_valid_type).attr('class', 'error_message');
        $('#input_newpin').focus();
        $('#input_newpin').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else if (!pinpatrn.exec(newpin)) {
        $('#newpin_error').html(dialup_hint_pin_code_valid_type).attr('class', 'error_message');
        $('#input_newpin').focus();
        $('#input_newpin').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else {

    }

    if ('' == retypepin) {
        $('#confirmPin_error').html(dialup_hint_pin_code_valid_type).attr('class', 'error_message');
        $('#input_confirmPin').focus();
        $('#input_confirmPin').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else if (!pinpatrn.exec(retypepin)) {
        $('#confirmPin_error').html(dialup_hint_pin_code_valid_type).attr('class', 'error_message');
        $('#input_confirmPin').focus();
        $('#input_confirmPin').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else if (newpin != retypepin) {
        $('#confirmPin_error').html(dialup_hint_confirm_and_new_same).attr('class', 'error_message');
        $('#input_newpin').focus();
        $('#input_newpin').val('');
        $('#input_confirmPin').val('');
        button_enable('pukrequired_apply', '0');
        return false;
    }else {

    }
    return true;
}

function onApply() {
    $.each($('input'), function() {
        $(this).blur();
    });
    if (!isButtonEnable('pukrequired_apply')) {
        return;
    }

    if (validateInput()) {
        var pukcode = $('#input_puk').val();
        var newpin = $('#input_newpin').val();
        
        send_request.PukCode = encrypt(pukcode);
        send_request.CurrentPin = encrypt(newpin);
        send_request.NewPin = encrypt(newpin);

        var unlockpuk_xml = object2xml('request', send_request);
        button_enable('pukrequired_apply', '0');
        saveAjaxData('api/pin/operate', unlockpuk_xml, function($xml) {
            var return_ret = xml2object($xml);
            if (isAjaxReturnOK(return_ret)) {
                log.debug('PUKREQUIRED : puk validate success.');
                showInfoDialog(common_success);
                if (('.html' == g_postfix) || ('' == g_postfix)) 
                {
                    gotoPageWithoutHistory(HOME_PAGE_URL);
                }
                else 
                {
                    gotoPageWithoutHistory("cardinit.html" + window.location.search);
                }

            }else {
                showInfoDialog(puk_code_validate_failed);
                setTimeout(function() {
                    initPage();
                    return false;
                }, 3000);
            }
        });
    }
}
function isVaildValue(){
    clearAllErrorLabel();
    if(checkText('login_password')){
        return true;
    } else {
        return false;
    }
}

function checkText(id){
    var input = $('#'+id).val();
    if(''==input){
        showErrorUnderTextbox(id, samba_input_empty);
        $('#'+id).focus();
        return false;
    } else if(!checkInputChar(input)) {
        showErrorUnderTextbox(id, samba_input_invalid);
        $("#"+id).val("");
        $('#'+id).focus();
        return false;
    } else if (true == hasSpaceOrTabAtHead(input)) {
        showErrorUnderTextbox(id, samba_input_invalid);
        $("#"+id).val("");
        $('#'+id).focus();
        return false;
    }
    return true;
}

function postData(){
    var submitObject = null;
    var username = '';
    if(0 == g_user){
        username = $('#login_username').val();
    } else {
        username = $('#login_username option:selected').text();
    }
    submitObject = {
        Username:username,
        Password:stringToSHA256($('#login_password').val())
    };
     var newXmlString = object2xml('request', submitObject);
     saveAjaxData('api/user/login', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if (!isAjaxReturnOK(ret)) 
        {     
            if (ret.type == 'error') {
                clearAllErrorLabel();
                if (ret.error.code == ERROR_LOGIN_PASSWORD_WRONG) {
                    g_login_PasswordRemainTimes--;
                    if (LOGINLOCK  == g_login_PasswordRemainTimes){
                        checkLoginLock();
                    }
                    else {
                        showErrorUnderTextbox('login_password', system_hint_wrong_password + ' ' + dialup_label_remaining_times + common_colon + g_login_PasswordRemainTimes);
                        $('#login_password').val('');
                        $('#login_password').focus();
                    }
                }
                else if (ret.error.code == ERROR_LOGIN_ALREADY_LOGIN) {
                    showErrorUnderTextbox('login_password', common_user_login_repeat);
                    $('#login_password').focus();
                }
                else if (ret.error.code == ERROR_LOGIN_USERNAME_WRONG) {
                    showErrorUnderTextbox('login_password', settings_hint_user_name_not_exist);
                    $('#login_password').val('');
                    $('#login_password').focus();
                }
            }
            else
            {
                showErrorUnderTextbox('login_password', system_hint_wrong_password);
                $('#login_password').val('');
                $('#login_password').focus();
            }
        } else {
            addImeiIntoKey(); 
            if('1' == g_puk_first_login){
                clearDialog();
                call_dialog(common_confirm, login_first_show_info, common_ok, "pop_OK", common_cancel, "pop_Cancel");
                $("#pop_OK").click(function() {
                    $.cookies.set('cookie', "setting,system,modifypassword", { expiresAt: new Date().getTime()+5*60*1000 });
                    setTimeout(function(){
                        gotoPageWithoutHistory("setting.html?pukrequired");
                    },1);
                });
                $("#pop_Cancel,.dialog_header_right").click(function() {
                    g_main_displayingPromptStack.pop();
                    $('#input_puk').focus();
                });
            }else{
                clearDialog();
                g_main_displayingPromptStack.pop();
                $('#input_puk').focus();
            }
        }
    });
}
function checkLoginLock(){
    clearAllErrorLabel();
    showErrorUnderTextbox('login_password', login_is_locked);
    $('#login_password').attr('disabled',true);
    button_enable('login', '0');  
    setTimeout(cycCheckLoginLock, LOOP_TIME);
}
function cycCheckLoginLock(){
    var ret = '';
    getAjaxData('api/user/state-login', function($xml) {
        ret = xml2object($xml);
        if (ret.type == 'response') {
            g_login_PasswordRemainTimes = ret.response.PasswordRemainTimes;
            if (LOGINLOCK == g_login_PasswordRemainTimes) {
                setTimeout(cycCheckLoginLock, LOOP_TIME);
            }
            else {
                clearAllErrorLabel();
                $('#login_password').attr('disabled',false);
                button_enable('login', '1');  
                $('#login_password').val('');
                $('#login_password').focus();
            }
        }
        }, {
        sync: true
    });
}

$(document).ready(function() {
    getAjaxData("api/user/first-wizard",function($xml) {
        var ret = xml2object($xml);
        if(typeof(ret.response) != 'undefined' && ret.response.FirstWizard){
             g_puk_first_login = ret.response.FirstWizard;
        }
    });

    $('#input_puk').focus();
    $('input').bind('keyup change input paste cut keydown', function() {
        if (($('#input_puk').val()).length >= 4 && ($('#input_newpin').val()).length >= 4 && ($('#input_confirmPin').val()).length >= 4) {
            button_enable('pukrequired_apply', '1');
        }else {
            button_enable('pukrequired_apply', '0');
        }
    });
    $('#pukrequired_apply').bind('click', onApply);
    if (1 == g_feature.continue_button) {
        $('#link_login').show();
    }
    else
    {
        $('#link_login').hide();
    }
    g_is_login_opened = true;
    initPage();
    setLangList();
    showCurrentLanguage();
    $('#link_login').click(function() {
        gotoPageWithoutHistory(HOME_PAGE_URL);
    });
    var g_need_login = false;
    getAjaxData('api/user/state-login', function($xml) {
            var ret = xml2object($xml);
            if ((typeof(ret.response.State) != 'undefined') && (ret.response.State != 0))
            {
                g_need_login = true;
            }
            if (ret.type == 'response') {
                g_login_PasswordRemainTimes = ret.response.PasswordRemainTimes;
            }
        }, {
            sync: true
        });

    if(true==g_need_login)
    {
        showloginDialog();
        $('#login_username').attr('disabled',true);
        button_enable('login', '0');
        $('#login_username').change(function(){
            button_enable('login', '1');
        });           
        button_enable('login', '1');     
        $('#login').click(function(){
            if(isButtonEnable('login')&&isVaildValue()){
                postData();
            }
        });
    
        if (LOGINLOCK == g_login_PasswordRemainTimes)
        {
            checkLoginLock();
        }
        else
        {
            setTimeout(function(){
                $('#login_password').focus();
            },300);
        }
    }
});
