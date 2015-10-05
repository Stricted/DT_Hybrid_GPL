var g_periodic_set = 0;
var g_certificate_set = 0;
var g_reg_username = /^[a-zA-Z][\w]{5,11}$/;
var g_reg_password = /^[\w]{5,12}$/;
var g_reg_url = "^(((https|http)://){0,1}|(www.)){1}([A-Za-z0-9/]+.)+[a-z]{2,4}(/[a-z0-9?=%_&]+)*$";
var FILE_END_NAME = "bak";
var NUM_CHECK = 4294967295;

function isPeriodicChecked(){
    if($("#periodic_inform_check").attr("checked")){
        g_periodic_set = 1;
    }else{
        g_periodic_set = 0;
    }
}

function isCertificateChecked(){
    if($("#id_enable_certificate").attr("checked")){
        g_certificate_set = 1;
        $("#show_file_upload").show();
    }else{
        g_certificate_set = 0;
        $("#show_file_upload").hide();
    }
}
/*------------------------upload certificate or not-----------------------------*/
function isUploadCert(){
    isCertificateChecked();
    if(g_certificate_set == 1){
        isEnableUpload();
        $("#id_acs_path").attr("disabled",false);
    }else{
        $("#id_acs_path").attr("disabled",true);
        button_enable('btn_acs_upload', 0);
    }
}

/*--------------------save tr069 setting info--------------------------------------*/
function saveTRSetting(){
    if(isButtonEnable("btn_acs_save")) {
        if(validateInfo()){
            var interval_name = $("#id_interval_name").val();
            var url = $("#id_acs_url").val();
            var acs_name = $("#id_username").val();
            var acs_pwd = encrypt($("#id_password").val());
            var conn_name = $("#id_conn_username").val();
            var conn_pwd = encrypt($("#id_conn_password").val());
            var conn_port = $("#id_conn_port").val();
            var trsettinginfo = {
                PeriodicInformEnable : g_periodic_set,
                PeriodicInformInterval : interval_name,
                URL : url,
                Username : acs_name,
                Password : acs_pwd,
                ConnectionRequestUsername : conn_name,
                ConnectionRequestPassword : conn_pwd,
                ConnectionRequestPort : conn_port,
                X_SSLCertEnable : g_certificate_set
            };
            var xmlinfo = object2xml("request",trsettinginfo);
            saveAjaxData("api/settings/tr069",xmlinfo,function($xml){
                var ret = xml2object($xml);
                if(isAjaxReturnOK(ret)){
                    parent.showInfoDialog(common_success);
                    getTR069Info();
                }
                else {
                    parent.showInfoDialog(common_failed);
                }
            });
        }else{
            return;
        }
    }
    else {
        return;
    }
}
/*----------------------get file path------------------------------------*/
function getFilePath(filepath){
    var path_file = filepath;
    var file_name = "";
    var file_index = 0;
    file_index = path_file.lastIndexOf("\\");
    if(file_index>0){
        file_name = path_file.substr(file_index + 1);
    }else{
        file_index = path_file.lastIndexOf("/");
        if(file_index>0){
            file_name = path_file.substr(file_index + 1);
        }else{
            return path_file;
        }
    }
    return file_name;
}

/*----------------------upload file path----------------------------------------*/

function uploadFilePath(){
    var filepath = $("#id_acs_path").val();
    if(isButtonEnable("btn_acs_upload")){
        $("#path_form").submit();
        button_enable("btn_acs_upload",0);
    }else{
       return;
    }
}

/*------------------------get tr069 info-----------------------------------------*/
function getTR069Info(){
    button_enable("btn_acs_save",0);
    if($("#id_acs_path").val() == "" || $("#id_acs_path").val() == " ") {
        button_enable("btn_acs_upload",0);
    }
    else {
        button_enable("btn_acs_upload",1);
    }
    getAjaxData("api/settings/tr069",function($xml){
        var ret = xml2object($xml);
        if(ret.type == "response"){
            if(ret.response.PeriodicInformEnable == 1){
                $("#periodic_inform_check").attr("checked",true);
            }else{
                $("#periodic_inform_check").attr("checked",false);
            }
            isPeriodicChecked();
            $("#id_interval_name").attr("value",ret.response.PeriodicInformInterval);
            $("#id_acs_url").attr("value",ret.response.URL);
            $("#id_username").attr("value",ret.response.Username);
            $("#id_password").attr("value",(ret.response.Password == '' ? '' : decrypt(ret.response.Password)));
            $("#id_conn_username").attr("value",ret.response.ConnectionRequestUsername);
            $("#id_conn_password").attr("value",(ret.response.ConnectionRequestPassword == '' ? '' : decrypt(ret.response.ConnectionRequestPassword)));
            $("#id_conn_port").attr("value",ret.response.ConnectionRequestPort);
            if(ret.response.X_SSLCertEnable == 1){
                $("#id_enable_certificate").attr("checked",true);
                $("#show_file_upload").show();
            }
            else {
                $("#id_enable_certificate").attr("checked",false);
                $("#show_file_upload").hide();
            }
            isCertificateChecked();
        }
    });
}

function urlCheck(str){
    var str1 = '';
    var str2 = '';
    if(str.indexOf('/')!=-1) {
        str1 = str.substring(0,str.lastIndexOf('/')+1);
        str2 = str.substring(str.lastIndexOf('/')+1);
        if(!(str1.match("^((https|http)://)*$"))) {
            return false;
        } else {
            return isValidIpAddress(str2);
        }
    } else {
        return isValidIpAddress(str);
    }
}

function numCheck(num) {
    var i = 0;
    var c = '';
    if(num == '') {
        return false;
    }
    for(i=0;i<num.length;i++) {
        c = num.charAt(i);
        if(!(c<='9'&&c>='0')) {
            return false;
        }
    }
    num = parseInt(num,10);
    if(!(num>0&&num<=NUM_CHECK)) {
        return false;
    }
    return true;
}

/*----------------------------validate info--------------------------------------*/
function validateInfo(){
    clearAllErrorLabel();
    if(!(numCheck($.trim($("#id_interval_name").val())))){
        showErrorUnderTextbox('id_interval_name', lan_filter_errorinfo);
        $("#id_interval_name").focus();
        return false;
    }else if(!($("#id_acs_url").val().match(g_reg_url)||urlCheck($("#id_acs_url").val()))){
        showErrorUnderTextbox('id_acs_url', lan_filter_errorinfo);
        $("#id_acs_url").focus();
        return false;
    }else if(!$.trim($("#id_username").val()).match(g_reg_username)){
        showErrorUnderTextbox('id_username', lan_filter_errorinfo);
        $("#id_username").focus();
        return false;
    }else if(!$.trim($("#id_password").val()).match(g_reg_password)){
        showErrorUnderTextbox('id_password', lan_filter_errorinfo);
        $("#id_password").focus();
        return false;
    }else if(!$.trim($("#id_conn_username").val()).match(g_reg_username)){
        showErrorUnderTextbox('id_conn_username', lan_filter_errorinfo);
        $("#id_conn_username").focus();
        return false;
    }else if(!$.trim($("#id_conn_password").val()).match(g_reg_password)){
        showErrorUnderTextbox('id_conn_password', lan_filter_errorinfo);
        $("#id_conn_password").focus();
        return false;
    }else if(!isVaildPortForIPFilter($("#id_conn_port").val())){
        showErrorUnderTextbox('id_conn_port', lan_filter_errorinfo);
        $("#id_conn_port").focus();
        return false;
    }else{
        return true;
    }
}

/*-----------------------btn_acs_upload enable or not-----------------------------------*/
function isEnableUpload(){
    clearAllErrorLabel();
    if($.trim($("#id_acs_path").val()) == "") {
        button_enable("btn_acs_upload",0);
    }
    else {
        if($("#id_acs_path").val().split(".")[$("#id_acs_path").val().split(".").length - 1] == FILE_END_NAME ) {
            button_enable("btn_acs_upload",1);
        }
        else {
            showErrorUnderTextbox('id_acs_path', system_hint_file_name_empty);
            button_enable("btn_acs_upload",0);
        }
    }
}

/*-------------------ready page-----------------------*/
$(document).ready(function(){
    getTR069Info();
    button_enable("btn_acs_upload",0);
    $("#show_tr056_setting input").bind("change click blur keyup",function(){
        button_enable("btn_acs_save",1);
    });
    $("#id_acs_path").bind("change",function(){
        isEnableUpload();
    });
    $("#periodic_inform_check").bind("click",isPeriodicChecked);
    $("#id_enable_certificate").bind("click",isCertificateChecked);
    $("#btn_acs_save").bind("click",saveTRSetting);
    $("#id_acs_path").bind("click",uploadFilePath);
    $("#btn_acs_upload").bind("click",isUploadCert);
});
