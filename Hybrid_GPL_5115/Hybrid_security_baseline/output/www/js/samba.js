var VIRTUAL_SERVER_NUM = 1;
var g_samba_array = [];
var g_save_name = [];
var READ_ONLY = 1;
var g_editIndex = 0;
var g_deleteIndex = 0;
var addhtml = "<div>"+
                   "<table id='show_dialog_table' width='580'>"+
                        "<tr>"+
                            "<th width='130' height='1'></th><th width='160'></th><th width='130'></th><th width='160'></th>"+
                        "</tr>"+
                        "<tr>"+
                            "<td height='80' id='username_password_hint' style='font-size:12px;' valign='top' colspan='4'>"+common_note+':'+'<br>'+label_samba_username_check+'<br>'+label_samba_password_check+"</td>"+
                        "</tr>"+                        
                        "<tr>"+
                            "<td height='48' valign='top'>"+samba_label_username+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<input type='text' id='samba_username' class='input_style' maxlength='31'/>"+
                            "</td>"+
                            "<td valign='top'>"+common_password+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<input type='password' id='samba_password' class='input_style' maxlength='31'/>"+
                            "</td>"+
                        "</tr>"+
                        "<tr>"+
                            "<td height='48' valign='top'>"+samba_label_comfirm_password+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<input type='password' id='samba_comfirm_password' class='input_style' maxlength='31'/>"+
                            "</td>"+
                            "<td height='48' valign='top'>"+samba_label_right+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<select id='samba_right' class='input_select'>"+
                                    "<option value='1'>"+sd_label_read_only+"</option>"+
                                    "<option value='2'>"+sd_label_read_write+"</option>"+
                                "</select>"+
                            "</td>"+
                        "</tr>"+
                        "<tr>"+
                            "<td height='48' valign='top'>"+samba_label_directory+common_colon+"</td>"+
                            "<td valign='top'>"+
                            "<input type='text' id='samba_drectory' class='input_style' readonly maxlength='255'/>"+
                            "</td>"+
                            "<td valign='top'>"+
                            "<span class=\'button_samba_browse_list\' id='button_browse_samba" + "'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_browse + "</span></span></span></span>"+
                            "</td><td></td>"+
                        "</tr>"+
                    "</table>"+
                "</div>";
var information = {
                    dialogname :'',
                    Username   :'',
                    Password   :'',
                    drectory   :'',
                    Privilege  :''
                  };
var g_acl_settings = {
    HTTPLanEnable : '',
    HTTPWanEnable : '',
    SSLLanEnable: '',
    SSLWanEnable: '',
    FTPLanEnable : '',
    FTPWanEnable : ''
};
function getinformation()
{
    information.dialogname = parent.$(".dialog_header_left").text();
    information.Username  = parent.$("#samba_username").val();
    information.Password  = parent.$("#samba_password").val();
    information.drectory  = parent.$("#samba_drectory").val();
    information.Privilege = parent.$("#samba_right").val();
}

function isVaildVirtual(index){
    parent.clearAllErrorLabel();
    var sambaComfirmPassword = parent.$('#samba_comfirm_password').val();
    var sambaDrectory = $.trim(parent.$('#samba_drectory').val());
    var sambaPassword = parent.$('#samba_password').val();
    var i = 0;
    var j = 0;

    if (parent.$(".dialog_header_left").text() == common_edit){
        for (i in g_save_name){
            if (i != index && g_save_name[i] == parent.$('#samba_username').val()) {
                showErrorUnderTextbox_Parent('samba_username', samba_input_invalid);
                parent.$('#samba_username').focus();
                return false;
            }
        }
    }
    if (parent.$(".dialog_header_left").text() == common_add) {
        for (j in g_save_name) {
            if (g_save_name[j] == parent.$('#samba_username').val()){
                showErrorUnderTextbox_Parent('samba_username', samba_input_invalid);
                parent.$('#samba_username').focus();
                return false;
            }
        }
    }
    if(!(checkSambaName('samba_username')&&checkSambaPwd('samba_password')&&checkSambaPwd('samba_comfirm_password'))){
        return false;
    }
    if (true == hasSpaceOrTabAtHead(sambaPassword)) {
        showErrorUnderTextbox_Parent('samba_password', input_cannot_begin_with_space);
        parent.$('#samba_password').focus();
        return false;
    } else if(true == hasSpaceOrTabAtHead(sambaComfirmPassword)){
        showErrorUnderTextbox_Parent('samba_comfirm_password', input_cannot_begin_with_space);
        parent.$('#samba_comfirm_password').focus();
        return false;
    }
    if(sambaComfirmPassword != sambaPassword){
        showErrorUnderTextbox_Parent("samba_comfirm_password",samba_same_password);
        parent.$('#samba_comfirm_password').focus();
        return false;
    }
    var samba_path = $.trim(parent.$("#samba_drectory").val());    
    if(getPathlength(samba_path)>64)
    {
        showErrorUnderTextbox_Parent("samba_drectory", hint_directory_path_length_invalid);
        parent.$('#samba_drectory').focus();
        return false;
    }   
    return true;
}
function getPathlength(str)
{
    var cArr = str.match(/[^\x00-\xff]/ig);
    return str.length + 2*(cArr==null?0:cArr.length);
}
function checkDrectory(id){
    var i = 1;
    var pathDir = $.trim(parent.$('#'+id).val()); 
    var pathParts = pathDir.split('/');
    if('/'==pathDir)
    {
        return true;
    }
    if(('/' ==pathDir.charAt(0)) || ('/' == pathDir.charAt(pathDir.length-1))){
        showErrorUnderTextbox_Parent(id, hint_directory_path_chart_invalid);
        parent.$('#'+id).focus();
        return false;
    }
    for(i=0;i<pathParts.length;i++){
        if (''==pathParts[i]||!checkInputChar(pathParts[i])) {
            showErrorUnderTextbox_Parent(id, samba_input_invalid);
            parent.$('#'+id).focus();
            return false;
        }
    }
    return true;
}
function checkSambaName(){
    //var sambaname = $.trim(parent.$('#samba_username').val());
    var sambaname = parent.$('#samba_username').val();
    if(sambaname=='root'  ||sambaname=='nobody' ||sambaname=='support'
     ||sambaname=='admin' ||sambaname=='user'   ||sambaname=='anonymous'
     ||sambaname=='web'   ||sambaname=='upnp'   ||sambaname=='cwmp'
     ||sambaname=='telnetd'||sambaname=='console'||sambaname=='dhcps'
     )
    {
        showErrorUnderTextbox_Parent('samba_username', samba_input_invalid_username.replace('%s', $.trim(parent.$('#samba_username').val())));
        parent.$('#samba_username').focus();
        return false;
    }
    if ('' != sambaname) {
        if (!checkSambaUsername(sambaname)) {
            showErrorUnderTextbox_Parent('samba_username', samba_input_invalid);
            parent.$('#samba_username').focus();
            return false;
        } else {
            return true;
        }
    } else {
        showErrorUnderTextbox_Parent('samba_username', samba_input_empty);
        parent.$('#samba_username').focus();
        return false;
    }
}

function checkSambaPwd(id){
    //var sambapwd = $.trim(parent.$('#'+id).val());
    var sambapwd = parent.$('#'+id).val();
    if (typeof(sambapwd) == 'undefined')
    {
        return;
    }
    if ('' != sambapwd) {
        if (!checkSambaPassword(sambapwd)) {
            showErrorUnderTextbox_Parent(id,samba_input_invalid);
            parent.$('#'+id).focus();
            return false;
        } else {
            return true;
        }
    } else {
        showErrorUnderTextbox_Parent(id, samba_input_empty);
        parent.$('#'+id).focus();
        return false;
    }
}
function checkSambaUsername(str)
{
    var i = 0;
    for(i = 0; i < str.length; i++)
    {
        var pChar = parseInt(str.charAt(i),10);
        if(!(((pChar >= 0) && (pChar <= 9 )) || ((str.charAt(i) >= 'a') && (str.charAt(i) <= 'z'))||((str.charAt(i) >= 'A') && (str.charAt(i) <= 'Z'))))
        {     
            return false;
        }
    }
    return true;
}
function checkSambaPassword(str)
{
    if (!checkInputChar_wanName(str))
    {
        return false;
    }
    var special = "&\\\'\"> ";
    for(var i=0;i< str.length;i++)
    {       
        if(special.indexOf(str.charAt(i)) >=0 )
        {
            return false;
        }   
    }
    return true;
}
function onApply_enable(){
    var submitObject_enable = {
        SMBEnable : ($('#enable_samba').attr('checked')?1:0)
    };
    var ret_enable = '';
    var newXmlString_enable = object2xml('request', submitObject_enable);
    var b_setSMBEnable=false;
    saveAjaxData("api/settings/sambaenable",newXmlString_enable,function($xml){
        ret_enable = xml2object($xml);
        if (isAjaxReturnOK(ret_enable)) {
            b_setSMBEnable = true;
        }else{
            parent.showInfoDialog(common_failed);
            return;
        }
    },{
        sync: true
    });

    if(false==b_setSMBEnable)
    {
        return;
    }
    relate_aclFTPLan();
    
    var submitObject_ftpserver_enable = {
        Enable : ($('#enable_ftpserver').attr('checked')?1:0)
    };
    var ret_enable_ftpserver = '';
    var newXmlString_enable_ftpserver = object2xml('request', submitObject_ftpserver_enable);
    saveAjaxData("api/settings/ftpserverenable",newXmlString_enable_ftpserver,function($xml){
        ret_enable_ftpserver = xml2object($xml);
        if (isAjaxReturnOK(ret_enable_ftpserver)) {
            parent.showInfoDialog(common_success);
            button_enable('apply_enable', '0');
        }else{
            parent.showInfoDialog(common_failed);
            return;
        }
    });
    
}
function relate_aclFTPLan()
{
    g_acl_settings.FTPLanEnable = $('#enable_ftpserver').attr('checked')?1:0 ;
    var acl_object = {
        AclServices : g_acl_settings
    };
    var acl_xml = object2xml("request", acl_object);
    saveAjaxData("api/settings/aclstatus",acl_xml,function($xml){
    });
}
function onApply(){  
    var submitObject = null;
    var i = 0;
    if(0!=g_samba_array.length){
        var array = [];
        for(i=0;i<g_samba_array.length;i++){
        var filter = {
                    Username : g_samba_array[i][1].Username,
                    Password : encrypt(g_samba_array[i][1].Password),
                    Path : g_samba_array[i][1].Path,
                    Privilege : g_samba_array[i][1].Privilege
            };
            array.push(filter);
            }
            submitObject = {
                sambas : {
                    samba : array
                }
            };
        }
        else {
            submitObject = '';
        }
        var newXmlString = object2xml('request', submitObject);
        saveAjaxData("api/settings/samba",newXmlString,function($xml){
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                parent.showInfoDialog(common_success);
                button_enable('apply', '0');
            }else{
                parent.showInfoDialog(common_failed);
            }
        });
}
function clearDialog(){
    $('.dialog:not(#upload_dialog), .info_dialog, .login_dialog, #div_wrapper').remove();
    $('a').attr('tabindex', '');
    $('input').attr('tabindex', '');
    $('select').attr('tabindex', '');
}
function showDialog(name){
    getAjaxData('api/settings/sambausb', function($xml) {
        var ret = xml2object($xml);
        var samba_usb = '';
        var newline = "";
        if(ret.type == "response"&& typeof(ret.response.usbs)!='undefined'){
            samba_usb = ret.response;
            if(''!=samba_usb && typeof(samba_usb.usbs.usb)!='undefined'){
                var i = 0;
                var usb_count = 0;
                if ($.isArray(samba_usb.usbs.usb)) {
                    for(i=0;i<samba_usb.usbs.usb.length;i++){
                        if(1 == samba_usb.usbs.usb[i].Status){
                            if(0 == usb_count){
                                newline += "<option value='"+usb_count+"' selected>"+samba_usb.usbs.usb[i].DiskInfo+"</option>";
                            } else {
                                newline += "<option value='"+usb_count+"'>"+samba_usb.usbs.usb[i].DiskInfo+"</option>";
                            }
                            usb_count++;
                        }
                    }
                } else {
                    if(1 == samba_usb.usbs.usb.Status){
                        newline += "<option value='"+usb_count+"' selected>"+samba_usb.usbs.usb.DiskInfo+"</option>";
                    }
                }
                if(""!=newline){
                    parent.call_dialog(name,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_func,cancel_func,"samba_username");
                    parent.$('#samba_device').append(newline);
                    setTimeout(function() {
                        parent.$('#samba_device').val(0);
                    },1);
                    parent.$('#samba_password').blur(function() {
                        checkpasswordstrength();                        
                     });
                    parent.$('#samba_comfirm_password').blur(function() {                        
                        checkpasswordstrength();
                     });
                } else {
                    parent.showConfirmDialog(samba_no_usb_tips,clearDialog);
                }
            } else {
                parent.showConfirmDialog(samba_no_usb_tips,clearDialog);
            }
        } else {
            return;
        }
    },{
        sync: true
    });
}
function checkpasswordstrength()
{
    parent.clearAllErrorLabel();
    var samba_password = parent.$('#samba_password').val();
    var samba_comfirm_password = parent.$('#samba_comfirm_password').val();
    if (typeof(samba_password) == 'undefined' || typeof(samba_comfirm_password) == 'undefined')
    {
        return;
    }
    if (samba_password!='')
    {
        if (!checkSambaPassword(samba_password)) 
        {
            showErrorUnderTextbox_Parent('samba_password',samba_input_invalid);
        } 
        else
        {
            var strength = checkPasswordStrength(samba_password);
            if (samba_password.length<8 || strength<2)
            {
                showErrorUnderTextbox_Parent('samba_password', label_input_strength_password_weak);
            } 
        }    
    }
    if (samba_comfirm_password!='')
    {
        if(samba_comfirm_password != samba_password)
        {                          
            showErrorUnderTextbox_Parent("samba_comfirm_password",samba_same_password);                     
        }
    }    
}
/*---------delete pop ok function --------------*/
function delete_pop() {
    $(".user_add_line").eq(g_deleteIndex).remove();
    parent.clearDialog();
    button_enable("apply", "1");
    button_enable("add_item", "1");
    g_samba_array.splice(g_deleteIndex,1);
    g_save_name.splice(g_deleteIndex,1);
    if(g_samba_array.length >= VIRTUAL_SERVER_NUM)
    {
        button_enable('add_item', '0');
    }
}

function clickCancel(){
    clearDialog();
}
function getPath(path,num){
    var pathParts = path.split('/');
    if(0 == num){
        return pathParts[1];
    } else {
        var i = 2;
        var samba_path = "";
        for(i=2;i<pathParts.length;i++){
            if(i==2)
            {
                 samba_path += (pathParts[i]);
            }
            else
            {
                samba_path += ("/"+pathParts[i]);
            }            
        }
        return samba_path;
    }
}
function initPage(){
    $("#service_list td").parent().remove();
    g_samba_array = [];
    g_save_name.length = 0;
    button_enable('apply', '0');
    getAjaxData('api/settings/sambaenable', function($xml) {
        var ret = xml2object($xml);
        if(ret.type =="response"){
            var sambaenable = ret.response.SMBEnable;
            $('#enable_samba').attr('checked',1==sambaenable?true:false);
        }
    },{
        sync: true
    });
    getAjaxData('api/settings/ftpserverenable', function($xml) {
        var ret = xml2object($xml);
        if(ret.type =="response"){
            var ftpserverenable = ret.response.Enable;
            $('#enable_ftpserver').attr('checked',1==ftpserverenable?true:false);
        }
    },{
        sync: true
    });
    getAjaxData("api/settings/aclstatus",function($xml){       
        var res = xml2object($xml);
        if('response' == res.type)
        {
            g_acl_settings = res.response.AclServices;       
        }
    },{
        sync: true
    });
    getAjaxData('api/settings/samba', function($xml) {
        var ret = xml2object($xml);
        var samba = "";
        if(ret.type =="response"){
            if(typeof(ret.response.sambas)!='undefined'||typeof(ret.response.sambas.samba)!='undefined') {
                samba = ret.response.sambas.samba;
                if (samba) {
                    if ($.isArray(samba)) {
                        $(samba).each(function(i) {
                            var newline = "<tr class='user_add_line'>"+
                                             "<td id='samba_username"+i+"'>"+samba[i].Username+"</td>"+
                                             "<td id='samba_drectory"+i+"'>"+samba[i].Path+"</td>"+
                                             "<td id='samba_right"+i+"'>"+(samba[i].Privilege==READ_ONLY?sd_label_read_only:sd_label_read_write)+"</td>"+
                                             "<td id='samba_options"+i+"' class='user_options'>"+
                                                "<a href='javascript:void(0);' id='button_edit"+i+"' class='button_edit_list'>"+common_edit+"</a>"+
                                                "<br><a href='javascript:void(0);' id='button_delete"+i+"' class='button_delete_list'>"+common_delete+"</a>"+
                                             "</td>"+
                                          "</tr>";
                            $("#service_list").append(newline);
                            if(samba[i].Password != '') {
                                samba[i].Password = decrypt(samba[i].Password);
                            }
                            g_samba_array.push([i,samba[i]]);
                            g_save_name.push(samba[i].Username);
                        });
                    }else {
                        var newline = "<tr class='user_add_line'>"+
                                             "<td id='samba_username0'>"+samba.Username+"</td>"+
                                             "<td id='samba_drectory0'>"+samba.Path+"</td>"+
                                             "<td id='samba_right0'>"+(samba.Privilege==READ_ONLY?sd_label_read_only:sd_label_read_write)+"</td>"+
                                             "<td id='samba_options0' class='user_options'>"+
                                                "<a href='javascript:void(0);' id='button_edit0' class='button_edit_list'>"+common_edit+"</a>"+
                                                "<br><a href='javascript:void(0);' id='button_delete0' class='button_delete_list'>"+common_delete+"</a>"+
                                             "</td>"+
                                          "</tr>";
                        $("#service_list").append(newline);
                        if(samba.Password != '') {
                            samba.Password = decrypt(samba.Password);
                        }
                        g_samba_array.push([0,samba]);
                        g_save_name.push(samba.Username);
                    }
                }
            } else {
                return;
            }
        }
    },{
        sync: true
    });
    if(g_samba_array.length >= VIRTUAL_SERVER_NUM)
    {
        button_enable('add_item', '0');
    }
}
/*-------------------edit save function-------------------*/
function save_func() {
    if(isVaildVirtual(g_editIndex)){          
        if (parent.$(".dialog_header_left").text() == common_edit) {
            $("#samba_right"+g_samba_array[g_editIndex][0]).text(parent.$("#samba_right").val()==READ_ONLY?sd_label_read_only:sd_label_read_write);
            $("#samba_username"+g_samba_array[g_editIndex][0]).text(parent.$("#samba_username").val());
            $("#samba_drectory"+g_samba_array[g_editIndex][0]).text(parent.$("#samba_drectory").val());
            var editinfo = {
                Username : parent.$("#samba_username").val(),
                Password : parent.$("#samba_password").val(),
                Path : (parent.$("#samba_drectory").val()=='')?'/':parent.$("#samba_drectory").val(),
                Privilege : parent.$("#samba_right").val()
            };
            g_samba_array.splice(g_editIndex,1,[g_samba_array[g_editIndex][0],editinfo]);
            g_save_name[g_editIndex] = editinfo.Username;
        }
        else {
            var addline = "";
            if(0 != g_samba_array.length){
                var new_index = g_samba_array[g_samba_array.length-1][0]+1;
                addline = "<tr class='user_add_line'>"+
                                  "<td id='samba_username"+ new_index +"'>"+parent.$("#samba_username").val()+"</td>"+
                                  "<td id='samba_drectory"+ new_index +"'>"+parent.$("#samba_drectory").val()+"</td>"+
                                  "<td id='samba_right"+ new_index +"'>"+parent.$("#samba_right option:selected").text()+"</td>"+
                                  "<td id='samba_options"+ new_index +"' class='user_options'>"+
                                      "<a href='javascript:void(0);' id='button_edit"+ new_index +"' class='button_edit_list'>"+common_edit+"</a>"+
                                      "<br><a href='javascript:void(0);' id='button_delete"+ new_index +"' class='button_delete_list'>"+common_delete+"</a>"+
                                  "</td>"+
                              "</tr>";
            } else {
                addline = "<tr class='user_add_line'>"+
                                  "<td id='samba_username0'>"+parent.$("#samba_username").val()+"</td>"+
                                  "<td id='samba_drectory0'>"+parent.$("#samba_drectory").val()+"</td>"+
                                  "<td id='samba_right0'>"+parent.$("#samba_right option:selected").text()+"</td>"+
                                  "<td id='samba_options0' class='user_options'>"+
                                      "<a href='javascript:void(0);' id='button_edit0' class='button_edit_list'>"+common_edit+"</a>"+
                                      "<br><a href='javascript:void(0);' id='button_delete0' class='button_delete_list'>"+common_delete+"</a>"+
                                  "</td>"+
                              "</tr>";
            }
            var addinfo = {
                Username : parent.$("#samba_username").val(),
                Password : parent.$("#samba_password").val(),
                Path : (parent.$("#samba_drectory").val()=='')?'/':parent.$("#samba_drectory").val(),
                Privilege : parent.$("#samba_right").val()
            };
            if(0 != g_samba_array.length){
                g_samba_array.push([g_samba_array[g_samba_array.length-1][0]+1,addinfo]);
            } else {
                g_samba_array.push([0,addinfo]);
            }
            g_save_name.push(addinfo.Username);
            $("#service_list").append(addline);
            
        } 
        if(g_samba_array.length >= VIRTUAL_SERVER_NUM){
                button_enable('add_item', '0');
        }
        button_enable("apply",'1');
        parent.clearDialog();
        return true;
    }
    else {
        return false;
    }
}

/*----------------------cancel function--------------*/
function cancel_func() {
    parent.clearDialog();
}

$(document).ready(function() {
    initPage();
    $(".button_delete_list").live("click",function(){
        g_deleteIndex = $(".button_delete_list").index(this);
        getAjaxData('api/settings/sambausb', function($xml) 
        {
            var ret = xml2object($xml);
            var samba_usb = '';
            if(ret.type == "response"&& typeof(ret.response.usbs)!='undefined'){
                samba_usb = ret.response;
                if(''!=samba_usb && typeof(samba_usb.usbs.usb)!='undefined'){
                    if(g_samba_array.length > 0){                       
                        parent.showConfirmDialog(samba_hint_delete_list_item, delete_pop, clickCancel);
                        $(".button_delete_list").blur();
                    }
                } else {
                    parent.showConfirmDialog(samba_no_usb_tips,clearDialog);
                }
            } else {
                return;
            }
            },{
                sync: true
        });
    });
    $('#enable_samba').click(function() {
        button_enable('apply_enable', '1');
    });
    $('#enable_ftpserver').click(function() {
        button_enable('apply_enable', '1');
    });
    $('#apply_enable').click(function() {
        if (!isButtonEnable('apply_enable')) {
            return;
        }
        parent.showConfirmDialog(firewall_hint_submit_list_item, onApply_enable,function(){});
    });
    $('#apply').click(function() {
        if (!isButtonEnable('apply')) {
            return;
        }
        parent.showConfirmDialog(firewall_hint_submit_list_item, onApply,function(){});
    });
    $('#add_item').click(function() {
        if(isButtonEnable('add_item')){
            showDialog(common_add);
            parent.$("#samba_drectory").val('/');
        }
    });
    $('#samba_add_item').bind('change',function(){
        button_enable('apply', '1');
    });
    $('.button_edit_list').live('click', function() {
        $('.button_edit_list').blur();
        showDialog(common_edit);
        g_editIndex = $('.button_edit_list').index(this);
        if(0 != g_samba_array.length&&g_editIndex < g_samba_array.length){
            parent.$("#samba_username").attr("value",g_samba_array[g_editIndex][1].Username);
            parent.$("#samba_drectory").attr("value",$("#samba_drectory"+g_samba_array[g_editIndex][0]).text());
            parent.$("#samba_right").attr("value",g_samba_array[g_editIndex][1].Privilege);
            parent.$("#samba_password").attr("value",g_samba_array[g_editIndex][1].Password);
            parent.$("#samba_comfirm_password").attr("value",g_samba_array[g_editIndex][1].Password);
            
           checkpasswordstrength();
        }
    });
});