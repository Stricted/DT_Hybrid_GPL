var g_username = ["admin","user"];
var g_user = 0;
var g_login_PasswordRemainTimes = 3;
var LOGINLOCK = 0;
var LOOP_TIME = 6000;
var g_need_checkNewVersion = false;
var update_configration = null;
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
function postData(flag){
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
            if("checked"==$('#no_remainder_later').attr('checked')){
                getAjaxData("api/online-update/configuration", function($xml){
                    var ret = xml2object($xml);
                    if(ret.type=="response")
                    {
                        update_configration = ret.response;
                    }else{
                        log.dubug("get statics wrong");
                    }
                }, {
                    sync: true
                });
                update_configration.autoUpdateInterval = 0;

                var req = object2xml("request",update_configration);
                saveAjaxData("api/online-update/configuration",req,function($xml){
                    var ret = xml2object($xml);
                    if(ret.type == "response")
                    {
                        if(isAjaxReturnOK(ret))
                        {
                            var ack={
                                    userAckNewVersion:flag
                                    };
                            if(download_send_info(ack))
                            {
                                if(flag == 0){
                                    setcookie();
                                }else{
                                    closewindow();
                                }
                            } 
                        }else{
                            parent.showInfoDialog(common_failed);
                        }
                    }else{
                       log.debug("statics is wrong");
                    }
                }, {sync: true});
            }else{
                var ack={
                        userAckNewVersion:flag
                        };
                if(download_send_info(ack))
                {
                    if(flag == 0){
                        setcookie();
                    }else{
                        closewindow();
                    }
                } 
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
function download_send_info(ack)
{   
    var req = object2xml("request", ack);
    var bret = false;
    saveAjaxData("api/online-update/ack-newversion",req,function($xml){
        var ret = xml2object($xml);
        if(ret.type=='response')
        {
            if(isAjaxReturnOK(ret))
            {
                log.debug("download start"); 
                bret = true;
            }else
            {
                log.debug("download terminate");
                showInfoDialog(common_failed);
            }
        }
        },{
           sync:true
        });
    return bret;
}
function CancelRedirect()
{
    var ack={
            status:0
            };
    var req = object2xml("request", ack);
    var bret = false;
    saveAjaxData("api/online-update/CancelRedirect",req,function($xml){
        var ret = xml2object($xml);
        if(ret.type=='response')
        {
            if(isAjaxReturnOK(ret))
            {
                bret = true;  
            }else
            {
                showInfoDialog(common_failed);
            }
        }
        },{
           sync:true
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
function show_new_version_feature()
{   
    // get current changelog
    var new_version_feature = "";
    var current_changelog = "";
    getAjaxData('api/online-update/changelog', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type 
            && typeof(ret.response.changelogs) != "undefined"
            && typeof(ret.response.changelogs.changelog) != "undefined") 
            {
                    new_version_feature = ret.response.changelogs.changelog;
            }
           }, {
               sync: true
           }
    );
    if(""!=new_version_feature)
    {
        if($.isArray(new_version_feature)) 
        {
            $.each(new_version_feature, function(i) {
                current_changelog += "<br>" + new_version_feature[i];
            });
        }
        else
        {
            current_changelog="<br>"+new_version_feature;
        }
    }
    if(""!=current_changelog)
    {
        var newinfomsg =
            "<table cellpadding='0' align='center' cellspacing='0'>" +
                "<tr height='32'>" +
                "<td width='400' colspan='1'>" + update_label_newersion + '</td>' +
                '</tr>' +
                '<tr>' +
                "<td colspan='1'><hr/></td>" +
                '</tr>' +
                '<tr>' +
                "<td width='400' class = 'wps_forbidden_message'>" + current_changelog+'</td>' +
                '</tr>' +
            '</table>';
        showPswInfoDialog(newinfomsg,"");
    }    
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
            postData(0);
        }
    });
    $('#cancel').click(function(){
        if("checked"==$('#no_remainder_later').attr('checked')){
            if(isButtonEnable('cancel')&&isVaildValue()){
                postData(1);
            }
        }else{
            CancelRedirect();      
            closewindow();
        }
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
    if(g_need_checkNewVersion){
        show_new_version_feature();
    }
});
