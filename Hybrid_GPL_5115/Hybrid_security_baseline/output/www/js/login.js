var g_username = ["admin","user"];
var g_user = 0;
var g_first_login = 0;
var g_login_PasswordRemainTimes = 3;
var LOGINLOCK = 0;
var MODIFYPASSWORD_URL = "setting.html";
var LOOP_TIME = 6000;

function setSelect(){
    var newline = '';
    $.each(g_username,function(i){
        newline += "<option value='" + i + "'>" + g_username[i] + "</option>";
    });
    $('#login_username').append(newline);
    $('#login_username').val(0);
}
function checkLoginLock(){
    clearAllErrorLabel();
    showErrorUnderTextbox('login_password', login_is_locked,'login_errmsg');
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
        showErrorUnderTextbox(id, samba_input_empty,'login_errmsg');
        $('#'+id).focus();
        return false;
    } else if (true == hasSpaceOrTabAtHead(input)) {
        showErrorUnderTextbox(id, input_cannot_begin_with_space,'login_errmsg');
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
    $('#login_password').blur();
    submitObject = {
        Username:username,
        Password:stringToSHA256($('#login_password').val())
    };
     var newXmlString = object2xml('request', submitObject);
     saveAjaxData('api/user/login', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if(ret.type == 'response'){
            if (isAjaxReturnOK(ret)) {
                if(g_first_login == 1) {
                    if('admin' == username&&!checkAdminPassword($('#login_password').val(),username)){
                        call_dialog(common_confirm, login_first_show_info, common_ok, "pop_OK", common_cancel, "pop_Cancel");
                        $("#pop_OK").click(function() {
                            $.cookies.set('cookie', "setting,system,modifypassword", { expiresAt: new Date().getTime()+5*60*1000 });
                            setTimeout(function(){
                                gotoPageWithoutHistory(MODIFYPASSWORD_URL);
                            },1);
                        });
                        $("#pop_Cancel,.dialog_header_right").click(function() {
                            setTimeout(function(){
                                gotoPageWithoutHistory(HOME_PAGE_URL);
                            },1);
                        });
                    }
                    else {
                        gotoPageWithoutHistory(HOME_PAGE_URL);
                    }
                } else {
                    gotoPageWithoutHistory(HOME_PAGE_URL);
                }
            }
        }else {
            if (ret.type == 'error') {
                clearAllErrorLabel();
                if (ret.error.code == ERROR_LOGIN_PASSWORD_WRONG) {
                    g_login_PasswordRemainTimes--;
                    if (LOGINLOCK  == g_login_PasswordRemainTimes){
                        checkLoginLock();
                    }
                    else {
                        showErrorUnderTextbox('login_password', system_hint_wrong_password + ' ' + dialup_label_remaining_times + common_colon + g_login_PasswordRemainTimes,'login_errmsg');
                        $('#login_password').val('');
                        $('#login_password').focus();
                    }
                }
                else if (ret.error.code == ERROR_LOGIN_ALREADY_LOGIN) {
                    showErrorUnderTextbox('login_password', common_user_login_repeat,'login_errmsg');
                    $('#login_password').focus();
                }
                else if (ret.error.code == ERROR_LOGIN_USERNAME_WRONG) {
                    showErrorUnderTextbox('login_password', settings_hint_user_name_not_exist,'login_errmsg');
                    $('#login_password').val('');
                    $('#login_password').focus();
                }
            }
        }
    });
}
function checkUser(){
    getConfigData('config/user/config.xml', function($xml) {
        var ret = _xml2feature($xml);
        g_user = ret.user_enabled;
        if(0 == g_user){
            $('#login_username').remove();
            $('#login_username_td').append('<input type="text" id="login_username" class="input_style" value="admin" disabled/>');
        } else {
            setSelect();
        }
    },{
        sync:true
    });
}
$(document).ready(function() {
    getAjaxData("api/user/first-wizard",function($xml) {
        var ret = xml2object($xml);
        if(typeof(ret.response) != 'undefined' && ret.response.FirstWizard){
             g_first_login = ret.response.FirstWizard;
        }
    });
    checkUser();
    button_enable('login', '1');
    var ret = '';
    getAjaxData('api/user/state-login', function($xml) {
        ret = xml2object($xml);
        if (ret.type == 'response') {
            g_login_PasswordRemainTimes = ret.response.PasswordRemainTimes;
        }
    }, 
    {
        sync: true
    });
    
    if (LOGINLOCK == g_login_PasswordRemainTimes)
    {
        checkLoginLock();
    }else {
        setTimeout(function(){
            $('#login_password').focus();
        },300);
    }
    g_is_login_opened = true;
    setLangList();
    showCurrentLanguage();
    getUserManualUrl();
    $('#login').click(function(){
        if(isButtonEnable('login') && isVaildValue()){
            postData();
        } else {
            return;
        }
    });
});
