var g_postfix = window.location.search;
var g_pin_first_login = 0;
// JavaScript Document
var MACRO_PIN_OPERATE_VALIDATE = 0;
var MACRO_PIN_OPERATE_DISABLE = 2;
var PUK_REQUIRED_PAGE = 'pukrequired.html';
var CARD_LOCK_RIGHT_PAGE = 'cardlock.html';
var CARD_LOCK_WRONG_PAGE = 'carderror.html';


var g_username = ["admin","user"];
var g_user = 0;
var g_login_PasswordRemainTimes = 3;
var LOGINLOCK = 0;
var LOOP_TIME = 6000;

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
    button_enable('pinrequired_button_apply', '0');

    getAjaxData('api/pin/status', function($xml) {
        var pincode_validate_ret = xml2object($xml);
        if (pincode_validate_ret.type == 'response') {
            if(MACRO_PIN_REQUIRED == pincode_validate_ret.response.SimState) {
                $('#pinrequired_input_pin').removeAttr('disabled');
                $('#pinrequired_input_pin').val('');
                $('#pinrequired_input_pin').focus();
                $('#label_SimPINTimes').text(pincode_validate_ret.response.SimPinTimes);
            }else if (MACRO_PUK_REQUIRED == pincode_validate_ret.response.SimState) {
                gotoPageWithoutHistory(PUK_REQUIRED_PAGE + window.location.search);
            }else if (MACRO_PIN_READY == pincode_validate_ret.response.SimState) {
                gotoPageWithoutHistory(LOGIN_PAGE_URL + window.location.search);
            }else if (MACRO_CARD_LOCK_PIN_RIGNT == pincode_validate_ret.response.SimState) {
                gotoPageWithoutHistory(CARD_LOCK_RIGHT_PAGE + window.location.search);
            }else if (MACRO_CARD_LOCK_PIN_WRONG == pincode_validate_ret.response.SimState) {
                gotoPageWithoutHistory(CARD_LOCK_WRONG_PAGE + window.location.search);
            }else{
                gotoPageWithoutHistory("cardinit.html" + window.location.search);
            }
        }
    });
}

function validateInput() {
    $('#pinrequired_error').html('');
    var pincode = $('#pinrequired_input_pin').val();
    var patrn = /^[0-9]{4,8}$/;
    if ('' == pincode) {
        $('#pinrequired_error').html(dialup_hint_pin_code_valid_type).attr('class', 'error_message');
        $('#pinrequired_input_pin').focus();
        $('#pinrequired_input_pin').val('');
        button_enable('pinrequired_button_apply', '0');
        return false;
    }else if (!patrn.exec(pincode)) {
        $('#pinrequired_error').html(dialup_hint_pin_code_valid_type).attr('class', 'error_message');
        $('#pinrequired_input_pin').focus();
        $('#pinrequired_input_pin').val('');
        button_enable('pinrequired_button_apply', '0');
        return false;
    }else {
        return true;
    }
}

function pinreq_validate_savePin() {
    var simSavepinStatus = $('#pinrequired_checkbox_savepin').get(0).checked ? 1 : 0;
    var simSavepinPIN = encrypt($('#pinrequired_input_pin').val());
    var validate_req = {
        OperateType: MACRO_PIN_OPERATE_VALIDATE,
        CurrentPin: simSavepinPIN,
        NewPin: '',
        PukCode: ''
    };

    var savepin_code_req = {
        SimSavepinStatus: simSavepinStatus,
        SimSavepinPIN: simSavepinPIN
    };

    var validate_xml = object2xml('request', validate_req);
    button_enable('pinrequired_button_apply', '0');
    showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments, function() {});
    saveAjaxData('api/pin/operate', validate_xml, function($xml) {
        var validate_ret = xml2object($xml);
        if (isAjaxReturnOK(validate_ret)){
            if (simSavepinStatus == 1)
            {
                var savepin_xml = object2xml('request', savepin_code_req);
                saveAjaxData('api/pin/save-pin_only', savepin_xml, function($xml) {
                   }, {sync: true});
                }
                if (('.html' == g_postfix) || ('' == g_postfix)) 
                {
                    gotoPageWithoutHistory(HOME_PAGE_URL);
                }
                else 
                {
                    setTimeout(function() {
                        var retsult;
                        getAjaxData('api/monitoring/status', function($xml) {
                            retsult = xml2object($xml);
                        },{sync :true});
                        if ('1' == retsult.response.simlockStatus) {
                            gotoPageWithoutHistory('simlock.html');
                        }else {
                            gotoPageWithoutHistory(g_postfix);
                        }
                    }, LOOP_TIME);
                }
            }
        else
        {
            closeWaitingDialog();
            $('#pinrequired_input_pin').attr('disabled', true);
            showInfoDialog(common_failed);
            setTimeout(function() {
                initPage();
                return false;
            }, 3000);
        }
    });
}

function onApply() {
    $.each($('input'), function() {
        $(this).blur();
    });
    if (!validateInput()) {
         return;   
    }
    pinreq_validate_savePin();
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
            if('1' == g_pin_first_login){
                clearDialog();
                call_dialog(common_confirm, login_first_show_info, common_ok, "pop_OK", common_cancel, "pop_Cancel");
                $("#pop_OK").click(function() {
                    $.cookies.set('cookie', "setting,system,modifypassword", { expiresAt: new Date().getTime()+5*60*1000 });
                    setTimeout(function(){
                        gotoPageWithoutHistory("setting.html?pincoderequired");
                    },1);
                });
                $("#pop_Cancel,.dialog_header_right").click(function() {
                    g_main_displayingPromptStack.pop();
                    $('#pinrequired_input_pin').focus();
                });
            }else{
                clearDialog();
                g_main_displayingPromptStack.pop();
                $('#pinrequired_input_pin').focus();
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
             g_pin_first_login = ret.response.FirstWizard;
        }
    });

    $('#save_setting').html(dialup_hilink_label_save_pin_code + common_colon);
    $('#pinrequired_input_pin').focus();

    $('#pinrequired_input_pin').bind('keyup change input paste cut', function() {
        button_enable('pinrequired_button_apply', '1');
    });
    $('#pinrequired_button_apply').bind('click', function() {
        if (!isButtonEnable('pinrequired_button_apply')) {
            return;
        }
        onApply();
    });
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
