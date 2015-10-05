var g_username = ["admin","user"];
var g_user = 0;
var g_login_PasswordRemainTimes = 3;
var LOGINLOCK = 0;
var LOOP_TIME = 6000;

function setSelect(){
    var newline = '';
    $.each(g_username,function(i){
        newline += "<option value='" + i + "'>" + g_username[i] + "</option>";
    });
    $('#login_username').append(newline);
    $('#login_username').val(0);
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
        if (!isAjaxReturnOK(ret)) {
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
            var ack={
                    AckUpdate:0
                };
            if(update_send_info(ack))
            {
                setcookie();
            }
        }
    });
}
function checkLoginLock(){
    clearAllErrorLabel();
    showErrorUnderTextbox('login_password', login_is_locked);
    $('#login_password').attr('disabled',true);
    button_enable('download', '0');
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
                button_enable('download', '1');
                $('#login_password').val('');
                $('#login_password').focus();
            }
        }
        }, {
        sync: true
    });
}

function setcookie()
{
    $.cookies.set('cookie', 'maintenance,null,update', { expiresAt: new Date().getTime()+5*60*1000 });
    setTimeout(function(){
            gotoPageWithoutHistory("maintenance.html");
    },1);
}

function update_send_info(ack)
{
    var req = object2xml("request", ack);
    var bret = false;
    saveAjaxData("api/online-update/comfirmupdate",req,function($xml){
        var ret = xml2object($xml);
        if(ret.type == 'response')
        {
            if(isAjaxReturnOK(ret))
            {
                log.debug("update start");
                bret = true;
            }else{
                log.debug("update terminate");
                showInfoDialog(common_failed);
            }
        }
        },{
        sync :true,
        errorCB:function(){
            log.deug("update terminate");
            }
        });
    return bret;
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
function closewindow()
{
    window.opener = null; 
    window.open('', '_self'); 
    window.close();
}
$(document).ready(function() {
    checkUser();
    setLangList();
    showCurrentLanguage();
    $('#login_username').change(function(){
        button_enable('download', '1');
    });
   
    button_enable('download', '1');

    $('#download').click(function(){
        if(isButtonEnable('download')&&isVaildValue()){
            postData();
        }
    });
    $('#cancel').click(function(){
          var ack={
                    AckUpdate:1
                };
        update_send_info(ack);       
        closewindow();
    });
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
});
