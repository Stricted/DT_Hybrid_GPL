var g_username_enable = null;
var g_flag = null;
var LOGINLOCK = 0;
var g_login_PasswordRemainTimes = 3;
var g_time = 30000;

var g_postfix = top.location.search;
if (g_postfix != null) {
    var length = g_postfix.length;
    var start = g_postfix.indexOf('?');
    g_postfix = g_postfix.substring(start + 1, length) + '.html';
}else {
    g_postfix = '';
}

function getPasswordRemainTimes(){
    var ret = '';
    getAjaxData('api/user/state-login', function($xml) {
        ret = xml2object($xml);
        if (ret.type == 'response') {
            g_login_PasswordRemainTimes = ret.response.PasswordRemainTimes;
        }},{
        sync: true
        }
    );
}

function checkcurrPwd(pwd)
{
    var ischeck = false;
    var submitObject = {
        Username:parent.g_login_usertype,
        Password:stringToSHA256(pwd)
    };
    var newXmlString = object2xml("request", submitObject);
    saveAjaxData('api/user/login', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response" && isAjaxReturnOK(ret))
        {
            ischeck = true;
        }
    },{
        sync:true
    });
    return ischeck;
}
function vilidatePassword() {
    clearAllErrorLabel();
    var currenPassword = $('#current_password').val();
    var newPassword = $('#new_password').val();
    var confirmPassword = $('#confirm_password').val();
    var username = $('#username_select').val();
    if ('' == currenPassword)
    {
        showErrorUnderTextbox('current_password', system_hint_current_password_empty);
        $('#current_password').focus();
        return false;
    }else if(!checkcurrPwd(currenPassword)){
        showErrorUnderTextbox('current_password', system_hint_wrong_password);
        $('#current_password').focus();
        return false;
    }
    if ('' == newPassword)
    {
        showErrorUnderTextbox('new_password', system_hint_enter_new_password);
        $('#new_password').focus();
        return false;
    }
    if ('' == confirmPassword)
    {
        showErrorUnderTextbox('confirm_password', system_hint_enter_confirm_password);
        $('#confirm_password').focus();
        return false;
    }
    if(hasSpaceOrTabAtHead(newPassword))
    {
        showErrorUnderTextbox('new_password', input_cannot_begin_with_space);
        $('#newPassword').focus();
        return false;
    }
    if (newPassword != confirmPassword)
    {
        showErrorUnderTextbox('confirm_password', system_hint_new_confirm_password_same);
        $('#confirm_password').focus();
        return false;
    }
    
    if(!isAsciiOrSpaceString(newPassword)){
        showErrorUnderTextbox('new_password', system_hint_current_pwd_valid_char);
        $('#new_password').focus();
        return false;
    }
    if(username == 'admin'){
        if(!checkAdminPassword(newPassword, username)){
            parent.showConfirmDialog(admin_check_title+'<br>'+user_check_info_1+'<br>'+admin_check_info_2+'<br>'+lable_admin_password_reverse+'<br>'+"<div class='info_title'><h4>"+ system_hint_new_confirm_password_not_safe +"</h4></div>", postmodifyPassword, function() {});
            return false;
        }
    }
    else if(username == 'user'){      
        if(!checkUserPassword(newPassword)){
            parent.showConfirmDialog(admin_check_title+'<br>'+user_check_info_1+'<br>'+admin_check_info_2+'<br>'+"<div class='info_title'><h4>"+ system_hint_new_confirm_password_not_safe +"</h4></div>", postmodifyPassword, function() {});
            return false;
        }
    }
    return true;
}
function postmodifyPassword()
{
    var username = '';
    if(g_flag == 1){
        username = $('#username_select').val();
    }else {
        username = 'admin';
    }
    var currentPassword = $('#current_password').val();
    var newPassword = $('#new_password').val();
    currentPassword = stringToSHA256(currentPassword);
    newPassword = stringToSHA256(newPassword);
    var request = {
        Username: username,
        CurrentPassword: currentPassword,
        NewPassword: newPassword
        };
    var xmlstr = object2xml('request', request);
    saveAjaxData('api/user/password', xmlstr, function($xml) {
    var ret = xml2object($xml);
    if (isAjaxReturnOK(ret))
    {
        if(('' != g_postfix) && ('.html' != g_postfix)){
            gotoPageWithoutHistory(g_postfix);
        }else{
            button_enable('apply_button', '0');
            $('#current_password').val('');
            $('#new_password').val('');
            $('#confirm_password').val('');
            parent.showInfoDialog(common_success);
            setTimeout(function(){parent.userOut();}, 1000);
        }
    }
    else if (ret.type == 'error')
    {   
        getPasswordRemainTimes();
        if (LOGINLOCK  == g_login_PasswordRemainTimes){
            gotoPageWithoutHistory('login.html');
        }
        else {
            showErrorUnderTextbox('current_password', system_hint_wrong_password + ' ' + dialup_label_remaining_times + common_colon + g_login_PasswordRemainTimes);
            button_enable('apply_button', '0');
            $('#current_password').val('');
            $('#new_password').val('');
            $('#confirm_password').val('');
            $('#current_password').focus();
            }
    }   
    });

}

function apply() {
    if (!isButtonEnable('apply_button'))
    {
        return;
    }

    var bValid = vilidatePassword();
    if (bValid)
    {
        postmodifyPassword()
    }
}

function configDataDisplay(){
    if( g_username_enable != null && typeof(g_username_enable) != 'undefined' && g_username_enable.user_name_enabled == '1')
    {
        $('#tr_username_enable').show();
    }
    else
    {   g_flag = 1;
        $('#tr_username_enable').hide();
    }
}
    

$(document).ready(function() {
    $('#apply_button').click(function() {
        apply();
    });
    button_enable('apply_button', '0');

    $('input').bind('change input paste cut keydown', function() {
        button_enable('apply_button', '1');
    });

    getConfigData('config/user/config.xml', function($xml) {
        g_username_enable = _xml2feature($xml);
    },{
        sync : true
    });

    configDataDisplay();
    setTimeout(function(){
        $('#current_password').focus();
    },1000);
});
