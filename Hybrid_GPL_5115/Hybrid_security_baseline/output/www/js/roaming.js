var g_username = ["admin","user"];
var g_user = 0;
function setSelect(){
    var newline = '';
    $.each(g_username,function(i){
        newline += "<option value='" + i + "'>" + g_username[i] + "</option>";
    });
    $('#login_username').append(newline);
    $('#login_username').val(0);
}
function initPage(){
    setLangList();
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
            showInfoDialog(common_failed);
        } else {
            gotoPageWithoutHistory(HOME_PAGE_URL);
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
    checkUser();
    initPage();
    $('#login_username').change(function(){
        button_enable('login', '1');
    });
    $('#login_password').bind('change input paste cut keydown',function(){
        button_enable('login', '1');
    });
    $('#login').click(function(){
        if(isButtonEnable('login')&&isVaildValue()){
            postData();
        }
    });
    $('#login_password').focus();
});
