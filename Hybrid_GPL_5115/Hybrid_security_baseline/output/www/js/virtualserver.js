var g_editIndex = 0;
var MAX_NUMBER = 16;
var MAX_LENGTH = 30;
var g_save_all_data = [];
var g_deleteIndex = 0;
var g_wanpath="";

var g_save_name = [];
var g_save_wan_port = [];
var PROTOCOL_BOTH = 0;
var PROTOCOL_TCP = 6;
var PROTOCOL_UDP = 17;
var PROTOCOL_STR_BOTH = "TCP/UDP";
var PROTOCOL_STR_TCP = "TCP";
var PROTOCOL_STR_UDP = "UDP";
var addhtml = 
    "<table width='570' id='popup_table' border='0' cellpadding='0' cellspacing='0'>"+
        "<tr>"+
            "<th width='110' height='1'></th><th width='160'></th><th width='110'></th><th width='190'></th>"+
        "</tr>"+
        "<tr id='popup_name' valign='top'>"+
            "<td height='42'>" + firewall_label_name + common_colon + '</td>'+
            "<td><input type='text' id='show_pro_name' maxlength='32' class= 'input_style'></td>"+
            "<td height='42'>" + firewall_label_status + common_colon + '</td>'+
            "<td>"+
            "<select  id='show_pro_statue' class= 'input_select'>"+
            "<option value='0'>" + common_off + '</option>'+
            "<option value='1'>" + common_on + '</option>'+
            '</select>'+
            '</td>'+
        '</tr>'+
        "<tr id='popup_apn' valign='top'>"+
            "<td height='42'>" + specialapp_commonport + common_colon + '</td>'+
            "<td class='radio'>"+
            "<select id='show_authentication_mode' class= 'input_select'>"+
            "<option value='0'>" + firewall_label_select + '</option>'+
                 "<option value='53'>" + virtual_port_dns +'</option>'+
                 "<option value='21'>" + virtual_port_ftp +'</option>'+
                 "<option value='70'>" + virtual_port_gopher +'</option>'+
                 "<option value='80'>" + virtual_port_http +'</option>'+
                 "<option value='119'>" + virtual_port_nntp +'</option>'+
                 "<option value='110'>" + virtual_port_pop3 +'</option>'+
                 "<option value='1723'>" + virtual_port_pptp +'</option>'+
                 "<option value='25'>" + virtual_port_smtp +'</option>'+
                 "<option value='1080'>" + virtual_port_sock +'</option>'+
                 "<option value='23'>" + virtual_port_telnet +'</option>'+
            '</select>'+
            '</td>'+
            "<td  height='42'>" + firewall_label_wan_port + common_colon + '</td>'+
            "<td><input type='text' value='' id='show_wan_port' class= 'input_style' maxlength='5'></td>"+
        '</tr>'+
        "<tr id='popup_dialup_number' valign='top'>"+
            "<td height='42'>" + firewall_label_lan_port + common_colon + '</td>'+
            "<td><input type='text' value='' id='show_lan_port' maxlength='5' class= 'input_style'></td>"+
            "<td  height='42'>" + virtual_lan_ipadress + common_colon + '</td>'+
            "<td><input type='text' value='' id='show_lan_ipadress' maxlength='15' class= 'input_style'></td>"+
        '</tr>'+
        "<tr id='popup_authen' valign='top'>"+
            "<td height='42'>" + firewall_label_protocol + common_colon + '</td>'+
            "<td height='42'>"+
            "<select name='authentication_m op_authmode' id='show_protocol' class= 'input_select'>"+
            "<option value='"+PROTOCOL_STR_TCP+"'>" + firewall_label_tcp + '</option>'+
            "<option value='"+PROTOCOL_STR_UDP+"'>" + firewall_label_udp + '</option>'+
            "<option value='"+PROTOCOL_STR_BOTH+"'>" + firewall_label_tcp_or_udp + '</option>'+
            '</select>'+
            '</td>'+
            "<td  height='42'>"+ security_label_wan_path+common_colon + '</td>'+
            "<td  height='42'>"+
            "<select name='wan_path op_authmode' id='wan_path' class= 'input_select' style='width:188px;'>"+
            '</select>'+
            '</td>'+
        "</tr>"+
    '</table>';
function isVaildSpecialPort_virtualserver(port, showTarget){
    var portParts = $.trim(port);
    var c = '';
    var i = 0;
    if ('' == portParts){
        showErrorUnderTextbox_Parent(showTarget,samba_input_empty);
        parent.$('#'+showTarget).focus();
        return false;
    }
    if ((portParts.indexOf('0') == 0) && (portParts.length != 1)){
        showErrorUnderTextbox_Parent(showTarget,samba_input_invalid);
        parent.$('#'+showTarget).focus();
        return false;
    }
    for (i = 0; i < portParts.length; i++){
        c = portParts.toLowerCase().charAt(i);
        if (!((c >= '0') && (c <= '9'))){
            showErrorUnderTextbox_Parent(showTarget,samba_input_invalid);
            parent.$('#'+showTarget).focus();
            return false;
        }
    }
    if ((portParts < MIN_PORT_RANGE) || (portParts > MAX_PORT_RANGE)){
        showErrorUnderTextbox_Parent(showTarget,samba_input_invalid);
        parent.$('#'+showTarget).focus();
        return false;
    }
    return true;
}
function checkIPAndPort(){
    parent.clearAllErrorLabel();
    checkPortShowMsg();
    var virtualServerIPName = $.trim(parent.$('#show_pro_name').val());
    var virtualServerIPAddress = $.trim(parent.$('#show_lan_ipadress').val());
    var virtualServerLanPort = $.trim(parent.$('#show_lan_port').val());
    var virtualServerWanPort = $.trim(parent.$('#show_wan_port').val());
    if ('' != virtualServerIPName){
        if (!checkInputChar(virtualServerIPName)){
            showErrorUnderTextbox_Parent("show_pro_name",samba_input_invalid);
            parent.$('#show_pro_name').focus();
            return false;
        }
        if(parent.$(".dialog_header_left").text() == common_edit) {
            for (var i in g_save_name){
                if (i != g_editIndex && g_save_name[i] == virtualServerIPName){
                    showErrorUnderTextbox_Parent("show_pro_name",label_same_name_exist);
                    parent.$('#show_pro_name').focus();
                    return false;
                }
            }
        }
        else {
            for (var i in g_save_name){
                if (g_save_name[i] == virtualServerIPName){
                    showErrorUnderTextbox_Parent("show_pro_name",label_same_name_exist);
                    parent.$('#show_pro_name').focus();
                    return false;
                }
            }
        }
    }
    else{
        showErrorUnderTextbox_Parent("show_pro_name",common_message_name_empty);
        parent.$('#show_pro_name').focus();
        return false;
    }
    if ('' != virtualServerWanPort){
        if (!isVaildSpecialPort_virtualserver(virtualServerWanPort, 'show_wan_port')){  
            parent.$('#show_wan_port').focus(); 
            return false;
        }
        if(parent.$(".dialog_header_left").text() == common_edit) {
            for (var i in g_save_wan_port){
                if (i != g_editIndex && g_save_wan_port[i] == virtualServerWanPort){
                    showErrorUnderTextbox_Parent("show_wan_port",Label_wan_port_exist);
                    parent.$('#show_wan_port').focus();
                    return false;
                }
            }
        }
        else {
            for (var i in g_save_wan_port){
                if (g_save_wan_port[i] == virtualServerWanPort){
                    showErrorUnderTextbox_Parent("show_wan_port",Label_wan_port_exist);
                    parent.$('#show_wan_port').focus();
                    return false;
                }
            }
        }
    }
    else{
        showErrorUnderTextbox_Parent("show_wan_port",samba_input_empty);
        parent.$('#show_wan_port').focus(); 
        return false;
    }
    if (virtualServerLanPort != ''){
        if (!isVaildSpecialPort_virtualserver(virtualServerLanPort, 'show_lan_port')){
            parent.$('#show_lan_port').focus(); 
            return false;
        }
    }
    else{
        showErrorUnderTextbox_Parent("show_lan_port",samba_input_empty);
        parent.$('#show_lan_port').focus();
        return false;
    }
    if('' != virtualServerIPAddress){
        if( !isValidIpAddress(virtualServerIPAddress) ||
            !isSameSubnetAddrs(virtualServerIPAddress, parent.dhcpLanIPAddress, parent.dhcpLanNetmask) ||
            !isBroadcastOrNetworkAddress(virtualServerIPAddress, parent.dhcpLanNetmask) ||
            virtualServerIPAddress == parent.dhcpLanIPAddress){
            showErrorUnderTextbox_Parent("show_lan_ipadress",samba_input_invalid);
            parent.$('#show_lan_ipadress').focus(); 
            return false;
        }
    }
    else {
        showErrorUnderTextbox_Parent("show_lan_ipadress",samba_input_empty);
        parent.$('#show_lan_ipadress').focus(); 
        return false;
    }
    return true;
}

function checkPortShowMsg(){
    parent.clearAllErrorLabel();
    var wanPortValue = parent.$('#show_wan_port').val();
    if('21' == wanPortValue){
        showErrorUnderTextbox_Parent("popup_table",virtualserver_ftp_port_help);
    }else if('443' == wanPortValue){
        showErrorUnderTextbox_Parent("popup_table",virtualserver_https_port_help);
    }
}

function initPage(){
    $("#service_list td").parent().remove();
    button_enable('apply', '0');
    g_save_name.length = 0;
    g_save_wan_port.length =0;
    g_save_all_data.splice(0, g_save_all_data.length);
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response'){
            parent.dhcpPageVar = ret.response;
            parent.initDhcp();
        }
    });
    getAjaxData('api/security/virtual-servers', function($xml) {
        var ret = xml2object($xml);
        if(typeof(ret.response.Servers) != 'undefined' && ret.response.Servers.Server){
            var servers = ret.response.Servers.Server;
            if(servers){
            button_enable('add_item', '1');
            if($.isArray(servers)){
                $(servers).each(function(i){
                    var getStatus  = servers[i].VirtualServerStatus==1? common_on : common_off;
                    var getProtocol = servers[i].VirtualServerProtocol;
                    var newline =   "<tr class='user_add_line'>"+
                                        "<td id='serveripname"+ i +"'>"+servers[i].VirtualServerIPName+"</td>"+
                                        "<td id='wan_port"+ i +"' class='td_ltr'>"+servers[i].VirtualServerWanPort+"</td>"+
                                        "<td id='lan_ip"+ i +"' class='td_ltr'>"+servers[i].VirtualServerIPAddress+"</td>"+
                                        "<td id='lan_port"+ i +"' class='td_ltr'>"+servers[i].VirtualServerLanPort+"</td>"+
                                        "<td id='protocol"+ i +"'>"+getProtocol+"</td>"+
                                        "<td id='status"+ i +"'>"+getStatus+"</td>"+
                                        "<td id='ip_start"+ i +"' class='user_options'>"+
                                            "<a href='javascript:void(0);' id='button_edit"+ i +"' class='button_edit_list'>"+common_edit+"</a>"+
                                            '<br>'+
                                            "<a href='javascript:void(0);' id='button_delete"+ i +"' class='button_delete_list'>"+common_delete+"</a>"+
                                        "</td>"+
                                    "</tr>";
                    $("#service_list").append(newline);
                    g_save_all_data.push([i,servers[i]]);
                    g_save_name.push(servers[i].VirtualServerIPName);
                    g_save_wan_port.push(servers[i].VirtualServerWanPort);
                });
                if(g_save_all_data.length == MAX_NUMBER){
                    button_enable('add_item',0);
                }

            }
            else{
                var getStatus  = servers.VirtualServerStatus == 1? common_on : common_off;
                var getProtocol = servers.VirtualServerProtocol;
                var newline = "<tr class='user_add_line'>"+
                                  "<td id='serveripname0'>"+ servers.VirtualServerIPName +"</td>"+
                                  "<td id='wan_port0' class='td_ltr'>"+ servers.VirtualServerWanPort +"</td>"+
                                  "<td id='lan_ip0' class='td_ltr'>"+ servers.VirtualServerIPAddress +"</td>"+
                                  "<td id='lan_port0' class='td_ltr'>"+ servers.VirtualServerLanPort +"</td>"+
                                  "<td id='protocol0'>"+ getProtocol +"</td>"+
                                  "<td id='status0'>"+ getStatus +"</td>"+
                                  "<td id='ip_start0' class='user_options'>"+
                                      "<a href='javascript:void(0);' id='button_edit0' class='button_edit_list'>"+common_edit+"</a>"+
                                      '<br>'+
                                      "<a href='javascript:void(0);' id='button_delete0' class='button_delete_list'>"+common_delete+"</a>"+
                                  "</td>"+
                              "</tr>";
                $("#service_list").append(newline);
                g_save_all_data.push([0,servers]);
                g_save_name.push(servers.VirtualServerIPName);
                g_save_wan_port.push(servers.VirtualServerWanPort); 
                }
             }
        }
    });
    addwanpath()
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
            g_wanpath = '';
            wanpath = ret.response.WanPaths.WanPath;  
            var i=0;
            if ($.isArray(wanpath))
            {
                for(i=0;i<wanpath.length;i++)
                {
                    g_wanpath += "<option value='"+wanpath[i].WanPathValue+"'>"+filterWanPathName(wanpath[i].WanPathName)+"</option>";                 
                }                  
            } 
            else 
            {
                g_wanpath += "<option value='"+wanpath.WanPathValue+"'>"+filterWanPathName(wanpath.WanPathName)+"</option>";                  
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


function clickAdd(){
    parent.call_dialog(common_add,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_pop_ok,clickCancel,"show_pro_name");
    parent.$("#wan_path").append(g_wanpath);
    parent.$('#wan_path').css({'width': 'auto'});
}
function clickCancel(){
    parent.clearDialog();
    g_main_displayingPromptStack.pop();
}

function clickDel(){
    if(g_save_all_data.length >= 0){
        g_deleteIndex = $(".button_delete_list").index(this);
        $(this).blur();
        parent.showConfirmDialog(firewall_hint_delete_list_item, delete_pop_ok, clickCancel);
    }
}
function selectChange(){
    var change = parent.$("#show_authentication_mode").val();
    if(0 != change){
        parent.$("#show_wan_port").attr("value",change);
        parent.$("#show_lan_port").attr("value",change);
    } else {
        parent.$("#show_wan_port").attr("value",'');
        parent.$("#show_lan_port").attr("value",'');
    }
}
function onApply(){
    var submitObject = null;
    if(0 != g_save_all_data.length){
        var array = [];
        for(i = 0;i<g_save_all_data.length;i++){
            var filter = {
                VirtualServerIPName : g_save_all_data[i][1].VirtualServerIPName,
                VirtualServerStatus : g_save_all_data[i][1].VirtualServerStatus,
                VirtualServerWanPort : g_save_all_data[i][1].VirtualServerWanPort,
                VirtualServerLanPort : g_save_all_data[i][1].VirtualServerLanPort,
                VirtualServerIPAddress : g_save_all_data[i][1].VirtualServerIPAddress,
                VirtualServerProtocol : g_save_all_data[i][1].VirtualServerProtocol,
                WanPath:g_save_all_data[i][1].WanPath
            };
            array.push(filter);
        }
        submitObject = {
            Servers : {
                Server : array
            }
        };
    }
    else {
        submitObject = {
            Servers : ''
        };
    }
    var newXmlString = object2xml('request', submitObject);
    saveAjaxData("api/security/virtual-servers",newXmlString,function($xml){
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            button_enable('apply', '0');
            initPage();
        }else{
            parent.showInfoDialog(common_failed);
        }
    });
}

/*-------------------------------delete pop ok function------------*/
function delete_pop_ok() {
    $(".user_add_line").eq(g_deleteIndex).remove();
    parent.clearDialog();
    button_enable("apply", "1");
    button_enable("add_item", "1");
    g_save_all_data.splice(g_deleteIndex,1);
    g_save_name.splice(g_deleteIndex,1);
    g_save_wan_port.splice(g_deleteIndex,1); 
    button_enable('add_item',1);
}

/*-------------------------------pop ok function-----------------*/
function save_pop_ok() {
    if(checkIPAndPort()){
        if(parent.$(".dialog_header_left").text() == common_edit) {
            $('#protocol' + g_save_all_data[g_editIndex][0]).text(parent.$("#show_protocol").val());
            $("#serveripname" + g_save_all_data[g_editIndex][0]).text(parent.$("#show_pro_name").val());
            $("#wan_port" + g_save_all_data[g_editIndex][0]).text(parent.$("#show_wan_port").val());
            $("#lan_ip" + g_save_all_data[g_editIndex][0]).text(parent.$("#show_lan_ipadress").val());
            $("#lan_port" + g_save_all_data[g_editIndex][0]).text(parent.$("#show_lan_port").val());
            $("#status" + g_save_all_data[g_editIndex][0]).text(parent.$("#show_pro_statue").val() == "1" ? common_on : common_off);
            var editinfo = {
                VirtualServerIPName : parent.$("#show_pro_name").val(),
                VirtualServerIPAddress : parent.$("#show_lan_ipadress").val(),
                VirtualServerLanPort : parent.$("#show_lan_port").val(),
                VirtualServerWanPort : parent.$("#show_wan_port").val(),
                VirtualServerProtocol : parent.$("#show_protocol").val(),
                VirtualServerStatus : parent.$("#show_pro_statue").val(),
                WanPath:parent.$("#wan_path").val()
            };
            g_save_all_data[g_editIndex][1] = editinfo;
            g_save_name.splice(g_editIndex,1,parent.$("#show_pro_name").val());
            g_save_wan_port.splice(g_editIndex,1,parent.$("#show_wan_port").val()); 
        }
        else{
            var number = 0;
            if(0 != g_save_all_data.length){
                number = g_save_all_data[g_save_all_data.length-1][0]+1;
            }
            var addline = "<tr class='user_add_line'>"+
                 "<td id='serveripname"+ number +"'>"+parent.$("#show_pro_name").val()+"</td>"+
                 "<td id='wan_port"+ number +"' class='td_ltr'>"+parent.$("#show_wan_port").val()+"</td>"+
                 "<td id='lan_ip"+ number +"' class='td_ltr'>"+parent.$("#show_lan_ipadress").val()+
                 "<td id='lan_port"+ number +"' class='td_ltr'>"+parent.$("#show_lan_port").val()+"</td>"+
                 "<td id='protocol"+ number +"'>"+parent.$("#show_protocol option:selected").text()+"</td>"+
                 "<td id='status"+ number +"'>"+parent.$("#show_pro_statue option:selected").text()+"</td>"+
                 "<td id='ip_start"+ number +"' class='user_options'>"+
                    "<a href='javascript:void(0);' id='button_edit"+ number +"' class='button_edit_list'>"+common_edit+"</a>"+
                      '<br>'+
                    "<a href='javascript:void(0);' id='button_delete"+ number +"' class='button_delete_list'>"+common_delete+"</a>"+
                 "</td>"+
              "</tr>";
            var addinfo = {
                VirtualServerIPName : parent.$("#show_pro_name").val(),
                VirtualServerIPAddress : parent.$("#show_lan_ipadress").val(),
                VirtualServerLanPort : parent.$("#show_lan_port").val(),
                VirtualServerWanPort : parent.$("#show_wan_port").val(),
                VirtualServerProtocol : parent.$("#show_protocol").val(),
                VirtualServerStatus : parent.$("#show_pro_statue").val(),
                WanPath:parent.$("#wan_path").val()
            };
            g_save_all_data.push([number,addinfo]);
            g_save_name.push(addinfo.VirtualServerIPName);
            g_save_wan_port.push(addinfo.VirtualServerWanPort); 
            $("#service_list").append(addline);
            if(g_save_all_data.length == MAX_NUMBER){
                button_enable('add_item',0);
            } 
        }
        button_enable("apply",1);
        parent.clearDialog();
        return true;
    }else{
        return false;
    }
}

$(document).ready(function() {
    initPage();
    $("#apply").click(function(){
        if (isButtonEnable('apply')) {
            parent.showConfirmDialog(firewall_hint_submit_list_item, onApply);
        }
        else {
            return;
        }
    });
    $("#add_item").live("click",function() {
        if(!isButtonEnable('add_item')){
            return;
        }
        clickAdd();
    });
    parent.$("#show_authentication_mode").die().live('change',function(){
        selectChange();
        checkPortShowMsg();
    });
    parent.$('#show_wan_port').die().live('blur',function(){checkPortShowMsg();});
    $(".button_delete_list").live("click",clickDel);  
    $('.button_edit_list').live('click', function() {
        parent.call_dialog(common_edit,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_pop_ok,clickCancel,"show_pro_name");
        parent.$("#wan_path").append(g_wanpath);
        parent.$('#wan_path').css({'width': 'auto'});
        g_editIndex = $('.button_edit_list').index(this);
        
        parent.$("#show_pro_name").attr("value",g_save_all_data[g_editIndex][1].VirtualServerIPName);
        parent.$("#show_wan_port").attr("value",g_save_all_data[g_editIndex][1].VirtualServerWanPort);
        parent.$("#show_lan_ipadress").attr("value",g_save_all_data[g_editIndex][1].VirtualServerIPAddress);
        parent.$("#show_lan_port").attr("value",g_save_all_data[g_editIndex][1].VirtualServerLanPort);
        parent.$("#show_protocol").val(g_save_all_data[g_editIndex][1].VirtualServerProtocol);
        parent.$("#show_pro_statue").val(g_save_all_data[g_editIndex][1].VirtualServerStatus);

        if(g_save_all_data[g_editIndex][1].WanPath!='')
        {
            setTimeout(function(){
                    parent.$("#wan_path").val(g_save_all_data[g_editIndex][1].WanPath);
            },1);
        }
    });
});