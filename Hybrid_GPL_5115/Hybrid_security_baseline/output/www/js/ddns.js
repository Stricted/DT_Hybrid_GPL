var VIRTUAL_SERVER_NUM = 16;
var g_ddns_array = [];
var g_ddns_provider = [];
var g_editIndex = 0;
var g_deleteIndex = 0;
var g_status;
var g_validate = false;
var addhtml = "<div>"+
                   "<table id='show_dialog_table'>"+
                        "<tr>"+
                            "<th width='100' height='1'></th><th width='160'></th><th width='80'></th><th width='180'></th>"+
                        "</tr>"+
                        "<tr height='48'>"+
                            "<td valign='top'>"+ddns_provider+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<select id='ddns_provider' class='input_select'></select></td>"+
                            "<td valign='top' colspan='2' id='ddns_provider_td' style='display:none'>"+
                                "<input type='text' id='ddns_provider_text' class='input_style' maxlength='20'/></td>"+
                        "</tr>"+
                        "<tr height='48'>"+
                            "<td valign='top'>"+ddns_domain+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<input type='text' id='ddns_domain' class='input_style' maxlength='128'/></td>"+
                             "<td valign='top' id='ddns_common_username' >"+dialup_label_user_name+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<input type='text' id='ddns_username' class='input_style' maxlength='128'/></td>"+
                        "</tr>"+
                        "<tr height='48'>"+
                            "<td valign='top' id='ddns_common_password' >"+common_password+common_colon+"</td>"+
                            "<td valign='top'>"+
                                "<input type='password' id='ddns_password' class='input_style' maxlength='64'/></td>"+
                            "<td valign='top'>"+ddns_wanpath+common_colon+"</td>"+
                            "<td valign='top' >"+
                                "<select id='ddns_wanpath' class='sntp_server_select'>"+                                
                                "</select></td>"+
                        "</tr>"+
                    "</table>"+
                "</div>";
function isVaildVirtual(){
    clearAllErrorLabel_Parent();
    var ddns_provider = $.trim(parent.$('#ddns_provider option:selected').text());
    var ddns_domain = $.trim(parent.$('#ddns_domain').val());
    if(sntp_label_Other == ddns_provider&&!checkNameOrPassword('ddns_provider_text')){
        return false;
    }
    if(''==ddns_domain){
        showErrorUnderTextbox_Parent('ddns_domain', samba_input_empty);
        parent.$('#ddns_domain').focus();
        return false;
    } else if(!checkNameOrPassword('ddns_domain')){
        return false;   
    }
     var firstPointIndex = parent.$("#ddns_domain").val().indexOf('.');
     var realHostName = parent.$("#ddns_domain").val().substring(0,firstPointIndex);
     var realDomainName = parent.$("#ddns_domain").val().substring(firstPointIndex+1,parent.$("#ddns_domain").val().length);
     if(''==realHostName ||''==realDomainName)
      {
      	      showErrorUnderTextbox_Parent('ddns_domain', ddns_domain_invalid);      	
	      parent.$('#ddns_domain').focus();
             return false;
      }
			
    if(!(checkNameOrPassword('ddns_username')&&checkNameOrPassword('ddns_password'))){
        return false;
    } else if (true == hasSpaceOrTabAtHead(parent.$('#ddns_password').val())) {
        showErrorUnderTextbox_Parent('ddns_password', input_cannot_begin_with_space);
        parent.$('#ddns_password').focus();
        return false;
    }
    return true;
}
function isVaildDomainHost(val){ 
    for (j = 0; j < val.length; j++) { 
        var c = val.charAt(j); 
        if (!((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')||(c >= '0' && c <= '9')||c == '-')) { 
            return false;
        }
    } 
    return true;
}
function isvailidUrl(){
    var regg = "^(((https|http):\/\/)|(www\\.)){1}([A-Za-z0-9\/]+\\.)+[a-z]{2,4}(\/[a-z0-9\?=\%_\&]+)*$";
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    if($.trim(parent.$('#ddns_domain').val()).match(regg)){
        return true;
    } else{
        return false;
    }
}
function checkNameOrPassword(id){
    if ('' != $.trim(parent.$('#'+id).val())) {
        if (!checkInputChar_wanName($.trim(parent.$('#'+id).val()))) {
            showErrorUnderTextbox_Parent(id, samba_input_invalid);
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

function onApply(){
    var submitObject = null;
    var i = 0;
    var array = [];

    var protocol = '';
    var server = '';
    
    for(i=0;i<g_ddns_array.length;i++){
        
        if(g_ddns_array[i][1].Provider=='DynDNS.org')
        {
            protocol = 'dyndns';
            server = 'DynDNS.org';
        }
        else if(g_ddns_array[i][1].Provider=='TZO')
        {
            protocol = 'TZO';
            server = 'cgi.tzo.com';
        }
        else if(g_ddns_array[i][1].Provider=='no-ip.com')
        {
            protocol = 'no-ip';
            server = 'dynupdate.no-ip.com';
        }
        else if(g_ddns_array[i][1].Provider=='selfHOST.de')
        {
            protocol = 'selfHOST';
            server = 'carol.selfhost.de';
        }
        else
        {
        }      
        var filter = {
            enable : g_ddns_array[i][1].enable,
            Provider : g_ddns_array[i][1].Provider,
            UserName : $.trim(resolveXMLEntityReference(g_ddns_array[i][1].UserName)),
            Password : encrypt(g_ddns_array[i][1].Password),
            HostName : $.trim(resolveXMLEntityReference(g_ddns_array[i][1].HostName)),
            ServicePort : '80',
            Protocol : protocol,
            Server : server,
            DomainName :$.trim(resolveXMLEntityReference( g_ddns_array[i][1].DomainName)),
            WanPath : g_ddns_array[i][1].WanPath
        };
        array.push(filter);
    }
    submitObject = {
        ddnss : {
            ddns : array
        }
    };
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);

    var newXmlString = object2xml('request', submitObject);
    saveAjaxData("api/settings/ddns",newXmlString,function($xml){
        var ret = xml2object($xml);
        parent.closeWaitingDialog();

        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            initPage();
        } else {
            parent.showInfoDialog(common_failed);
        }
    },{timeout:60000});
}
function isSameDDNSRule(){
    var hostName = $.trim(parent.$('#ddns_domain').val());
    var issameRule = false;
    $('.user_add_line').each(function(i) {
        if((parent.$(".dialog_header_left").text() == common_edit) && (i == g_editIndex)){
            return;
        }
        if($.trim($(this).children().eq(1).text()) == hostName){
            issameRule=true;
            showErrorUnderTextbox_Parent('ddns_domain', label_same_name_exist);
        }
    });
    return issameRule;
}

function saveDialog(){
    if(isVaildVirtual() && !isSameDDNSRule()){
        if (parent.$(".dialog_header_left").text() == common_edit) {
            $("#ddns_provider"+g_ddns_array[g_editIndex][0]).text(providerCheck());
            $("#ddns_domain"+g_ddns_array[g_editIndex][0]).text(parent.$("#ddns_domain").val());
            $("#ddns_username"+g_ddns_array[g_editIndex][0]).text(parent.$("#ddns_username").val());

	    var firstPointIndex = parent.$("#ddns_domain").val().indexOf('.');
           var realHostName = parent.$("#ddns_domain").val().substring(0,firstPointIndex);
	    var realDomainName = parent.$("#ddns_domain").val().substring(firstPointIndex+1,parent.$("#ddns_domain").val().length);
		
            var editinfo = {
                enable : '1',
                Provider : providerCheck(),
                UserName : $.trim(parent.$("#ddns_username").val()),
                Password : parent.$("#ddns_password").val(),
                HostName :$.trim(realHostName),
                ServicePort : '',
                Protocol : '',
                Server : '',
                DomainName :$.trim(realDomainName),
                WanPath :parent.$("#ddns_wanpath").val()
            };
            g_ddns_array.splice(g_editIndex,1,[g_ddns_array[g_editIndex][0],editinfo]);
        }
        else {
            var addline = "";
            if(0 != g_ddns_array.length){
                var new_index = g_ddns_array[g_ddns_array.length-1][0]+1;
                addline = "<tr class='user_add_line'>"+
                                  "<td id='ddns_provider" + new_index + "'>"+providerCheck()+"</td>"+
                                  "<td id='ddns_domain"+ new_index + "'>"+resolveXMLEntityReference(parent.$("#ddns_domain").val())+"</td>"+
                                  "<td id='ddns_username"+ new_index +"'>"+resolveXMLEntityReference(parent.$("#ddns_username").val())+"</td>"+
                                  "<td id='ddns_status"+ new_index +"'>"+ddnsStatus_stop+"</td>"+
                                  "<td id='ddns_options"+ new_index +"' class='user_options'>"+
                                      "<a href='javascript:void(0);' id='button_edit"+ new_index +"' class='button_edit_list'>"+common_edit+"</a>"+
                                      "<br><a href='javascript:void(0);' id='button_delete"+ new_index +"' class='button_delete_list'>"+common_delete+"</a>"+
                                  "</td>"+
                              "</tr>";
            } else {
                addline = "<tr class='user_add_line'>"+
                                  "<td id='ddns_provider0'>"+providerCheck()+"</td>"+
                                  "<td id='ddns_domain0'>"+resolveXMLEntityReference(parent.$("#ddns_domain").val())+"</td>"+
                                  "<td id='ddns_username0'>"+resolveXMLEntityReference(parent.$("#ddns_username").val())+"</td>"+
                                  "<td id='ddns_status0'>"+ddnsStatus_stop+"</td>"+
                                  "<td id='ddns_options0' class='user_options'>"+
                                      "<a href='javascript:void(0);' id='button_edit0' class='button_edit_list'>"+common_edit+"</a>"+
                                      "<br><a href='javascript:void(0);' id='button_delete0' class='button_delete_list'>"+common_delete+"</a>"+
                                  "</td>"+
                              "</tr>";
            }

	    var firstPointIndex = parent.$("#ddns_domain").val().indexOf('.');
	    var realHostName = parent.$("#ddns_domain").val().substring(0,firstPointIndex);
	    var realDomainName = parent.$("#ddns_domain").val().substring(firstPointIndex+1,parent.$("#ddns_domain").val().length);
		 
            var addinfo = {
                enable : '1',
                Provider : providerCheck(),
                UserName : $.trim(parent.$("#ddns_username").val()),
                Password : parent.$("#ddns_password").val(),
                HostName : $.trim(realHostName),
                ServicePort : '',
                Protocol : '',
                Server : '',
                DomainName :$.trim(realDomainName),
                WanPath : parent.$("#ddns_wanpath").val()
            };
            if(0 != g_ddns_array.length){
                g_ddns_array.push([g_ddns_array[g_ddns_array.length-1][0]+1,addinfo]);
            } else {
                g_ddns_array.push([0,addinfo]);
            }
            $("#service_list").append(addline);
            if(g_ddns_array.length >= VIRTUAL_SERVER_NUM){
                button_enable('add_item', '0');
            }
        }
        button_enable("apply",'1');
        parent.clearDialog();
        return true;
    }
    else {
        return false;
    }
}
function showDialog(name){
    g_ddns_provider.splice(0, g_ddns_provider.length);
    getAjaxData('config/ddns/config.xml', function($xml) {
        var ret = xml2object($xml);
        var ddns_provider = '';
        var newline = "";
        if(ret.type == "config"){
            ddns_provider = ret.config;
        }
        if(''!=ddns_provider){
            var i = 0;
            if ($.isArray(ddns_provider.provider)) {
                for(i=0;i<ddns_provider.provider.length;i++){
                    newline += "<option value='"+i+"'>"+ddns_provider.provider[i]+"</option>";
                    g_ddns_provider.push(ddns_provider.provider[i]);
                }
            } else {
                newline += "<option value='0' selected>"+ddns_provider.provider+"</option>";
                g_ddns_provider.push(ddns_provider.provider);
            }
            if(""!=newline){
                parent.call_dialog(name,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",saveDialog,cancel,"ddns_domain");
                parent.$('#ddns_provider').append(newline);
            } else {
                parent.call_dialog(name,common_failed,common_ok,"no_provider_ok");
            }
        } else {
            parent.call_dialog(name,common_failed,common_ok,"no_provider_ok");
        }
    },{
        sync: true
    });
    addwanpath();
}
function addwanpath()
{ 
    getAjaxData('api/settings/ddns-wanpath', function($xml) {
    var ret = xml2object($xml);
    var newline = "";
    var wanpath;
    if(ret.type =="response")
    {
        if(typeof(ret.response.WanPaths)!='undefined' && typeof(ret.response.WanPaths.WanPath)!='undefined') 
        {
            wanpath = ret.response.WanPaths.WanPath;  
            var i=0;
            if ($.isArray(wanpath))
            {
                for(i=0;i<wanpath.length;i++)
                {
                    newline += "<option value='"+wanpath[i].WanPathValue+"'>"+filterWanPathName(wanpath[i].WanPathName)+"</option>";                 
                }                  
            } 
            else 
            {
                newline += "<option value='"+wanpath.WanPathValue+"'>"+filterWanPathName(wanpath.WanPathName)+"</option>";                  
            }
            if(""!=newline)
            {  
                parent.$('#ddns_wanpath').append(newline);
                parent.$('#ddns_wanpath').css({'width': 'auto'});
            } 
            else 
            {
            }
        }                 
    } 
    else 
    {
        return;
    }
    }
    ,{
        sync: true
    });
}
function deleteData(){
    $(".user_add_line").eq(g_deleteIndex).remove();
    parent.clearDialog();
    button_enable("apply", "1");
    button_enable("add_item", "1");
    g_ddns_array.splice(g_deleteIndex,1);
}
function deleteCanael(){
    parent.clearDialog();
}
function clickDel(){
    if(g_ddns_array.length > 0){
        g_deleteIndex = $(".button_delete_list").index(this);
        parent.showConfirmDialog(firewall_hint_delete_list_item, deleteData, deleteCanael);
        $(".button_delete_list").blur();
    }
}

function providerCheck(){
    if(sntp_label_Other==parent.$('#ddns_provider option:selected').text()){
        return parent.$('#ddns_provider_text').val();
    } else {
        return parent.$('#ddns_provider option:selected').text();
    }
}
function initPage(){
    $("#service_list td").parent().remove();
    g_ddns_array.splice(0, g_ddns_array.length);
    button_enable('apply', '0');
    getddnsStatus();
    getAjaxData('api/settings/ddns', function($xml) {
        var ret = xml2object($xml);
        var ddns = "";
        if(ret.type =="response"){
            if(typeof(ret.response.ddnss)!='undefined' && typeof(ret.response.ddnss.ddns)!='undefined') {
                ddns = ret.response.ddnss.ddns;
                if (ddns) {
                    if (ddns.length >= VIRTUAL_SERVER_NUM)
                    {
                        button_enable('add_item', '0');
                    }
                    if ($.isArray(ddns)) {
                        if(typeof(g_status)!='undefined' && $.isArray(g_status) && ddns.length == g_status.length)
                        {
                            g_validate = true;
                        }
                        if(ddns.length != 0){
                            $(ddns).each(function(i) {
                                var newline = "<tr class='user_add_line'>"+
                                                 "<td id='ddns_provider"+i+"'>"+ddns[i].Provider+"</td>"+
                                                 "<td id='ddns_domain"+i+"'>"+(ddns[i].HostName+"."+ddns[i].DomainName)+"</td>"+
                                                 "<td id='ddns_username"+i+"'>"+ddns[i].UserName+"</td>"+
                                                 "<td id='ddns_status"+i+"'>"+(getddnsstatus_array(i))+"</td>"+
                                                 "<td id='ddns_options"+i+"' class='user_options'>"+
                                                    "<a href='javascript:void(0);' id='button_edit"+i+"' class='button_edit_list'>"+common_edit+"</a>"+
                                                    "<br><a href='javascript:void(0);' id='button_delete"+i+"' class='button_delete_list'>"+common_delete+"</a>"+
                                                 "</td>"+
                                              "</tr>";
                                $("#service_list").append(newline);
                                if(ddns[i].Password != '') {
                                    ddns[i].Password = decrypt(ddns[i].Password);
                                }
                                g_ddns_array.push([i,ddns[i]]);
                            });
                        }
                    }else {
                        if(typeof(g_status)!='undefined' && !$.isArray(g_status))
                        {
                            g_validate = true;
                        }
                        var newline = "<tr class='user_add_line'>"+
                                         "<td id='ddns_provider0'>"+ddns.Provider+"</td>"+
                                         "<td id='ddns_domain0'>"+(ddns.HostName+"."+ddns.DomainName)+"</td>"+
                                         "<td id='ddns_username0'>"+ddns.UserName+"</td>"+
                                         "<td id='ddns_status0'>"+(getddnsstatus())+"</td>"+
                                         "<td id='ddns_options0' class='user_options'>"+
                                            "<a href='javascript:void(0);' id='button_edit0' class='button_edit_list'>"+common_edit+"</a>"+
                                            "<br><a href='javascript:void(0);' id='button_delete0' class='button_delete_list'>"+common_delete+"</a>"+
                                         "</td>"+
                                      "</tr>";
                        $("#service_list").append(newline);
                        if(ddns.Password != '') {
                            ddns.Password = decrypt(ddns.Password);
                        }
                        g_ddns_array.push([0,ddns]);
                    }
                }
            } else {
                return;
            }
        }
    });
}
function getddnsstatus_array(index)
{
    if(false==g_validate)
    {
        return '';
    }
    return getstatusstr(g_status[index]);
}
function getddnsstatus()
{
    if(false==g_validate)
    {
        return '';
    }
    return getstatusstr(g_status);
}
function getstatusstr(value)
{  
    if (value==0)
    {
        return ddnsStatus_stop;
    }
    else if (value==1)
    {
        return ddnsStatus_updating;
    }
    else if (value==2)
    {
        return ddnsStatus_update_success;
    }
    else if (value==3)
    {
        return ddnsStatus_update_fail;
    }
    else
    {  
        return '';
    }
    return '';
}
function getddnsStatus()
{
    getAjaxData('api/settings/ddns-status', function($xml) {
        var ret = xml2object($xml);
            if(ret.type =="response"){
                if(typeof(ret.response.status)!='undefined' && typeof(ret.response.status.ddnsStatus)!='undefined') {
                    g_status = ret.response.status.ddnsStatus;                                         
                } else {
                    return;
                }
            }
        },{
            sync: true
        });   
}
function setProvider(g_editIndex){
    $(g_ddns_provider).each(function(i){
        if(g_ddns_provider[i] == g_ddns_array[g_editIndex][1].Provider){
            setTimeout(function(){
                parent.$('#ddns_provider').val(i);
                ddnsProviderChanged();
            },0);
            return;
        }
    });
}
function cancel(){
    parent.clearDialog();
    $("#apply").focus();
}


function ddnsProviderChanged()
{   
    if(sntp_label_Other==parent.$('#ddns_provider option:selected').text()){
        parent.$('#ddns_provider_td').show();
        parent.$('#ddns_provider_text').focus();
    } else {
        parent.$('#ddns_provider_td').hide();
        parent.$('#ddns_domain').focus();
    }

     if( 'TZO' == parent.$('#ddns_provider option:selected').text() )
     {
         parent.$("#ddns_common_password"). html(common_key+common_colon);
         parent.$("#ddns_common_username"). html(IDS_label_email_address+common_colon);
     }else{
         parent.$("#ddns_common_password"). html(common_password+common_colon);
         parent.$("#ddns_common_username"). html(dialup_label_user_name+common_colon);
     }
}


$(document).ready(function() {
    initPage();
    $("#no_provider_ok").live("click",parent.clearDialog);
    $(".button_delete_list").live("click",clickDel);
    $('#apply').click(function() {
        if (!isButtonEnable('apply')) {
            return;
        }
        parent.showConfirmDialog(firewall_hint_submit_list_item, onApply, function() {});
    });
    $('#add_item').click(function() {
        if(isButtonEnable('add_item')){
            showDialog(common_add);
            parent.$("#ddns_host").focus();
        }
    });
    $('.button_edit_list').live('click', function() {
        $('.button_edit_list').blur(); 
        showDialog(common_edit);
        g_editIndex = $('.button_edit_list').index(this);
        if(0 != g_ddns_array.length&&g_editIndex < g_ddns_array.length){
            setProvider(g_editIndex);
            parent.$("#ddns_password").val(g_ddns_array[g_editIndex][1].Password);
            parent.$("#ddns_domain").val(g_ddns_array[g_editIndex][1].HostName+'.'+g_ddns_array[g_editIndex][1].DomainName);
            parent.$("#ddns_username").val(g_ddns_array[g_editIndex][1].UserName);
            if(g_ddns_array[g_editIndex][1].WanPath!=''){
            setTimeout(function(){
                parent.$("#ddns_wanpath").val(g_ddns_array[g_editIndex][1].WanPath);
                },1);
            }
        }
    });

   parent.$("#ddns_provider").die().live('change',ddnsProviderChanged);   
});