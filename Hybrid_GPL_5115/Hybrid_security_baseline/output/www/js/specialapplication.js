// JavaScript Document
var comret = '';
var SPECIAL_APP_NUM = 16;
var Statu_DISABLED = 0;
var Statu_ENABLED = 1;
var dialogTitle = null;
var EditIndex = 0;

var PRTT_FTP=21;
var PRTT_IRC=6667;
var PRTT_TFTP=69;
var PRTT_SNMP_IN=161;
var PRTT_SNMP_OUT=162;
var PRTT_TALK_IN=517;
var PRTT_TALK_OUT=518;
var PRTT_H323=1720;
var PRTT_PPTP=1723;
var PRTT_HTTP=80;
var CWMP_PORT=7547;
var PORT_NUM=256;

var g_deleteIndex = 0;
var g_wanpath = "";
var g_edit_id = "";
var save_all_info = [];
var g_save_name = [];
var g_trigger_ports = [];
var g_open_ports = [];
var StatusArray = [
    [Statu_DISABLED, common_off],
    [Statu_ENABLED, common_on]
];

var PROTOCOL_BOTH_STR = 'TCP/UDP';
var PROTOCOL_TCP_STR = 'TCP';
var PROTOCOL_UDP_STR = 'UDP';
var portArray = [
    [PROTOCOL_BOTH_STR, firewall_label_tcp_or_udp],
    [PROTOCOL_TCP_STR, firewall_label_tcp],
    [PROTOCOL_UDP_STR, firewall_label_udp]
];
var addhtml = "<div id='special_appcontent'>"+
                  "<table id='special_apptext' style='width:580px'>"+
                     "<tr>"+
                        "<th width='130' height='1'></th><th width='160'></th><th width='130'></th><th width='160'></th>"+
                     "</tr>"+
                     "<tr>"+
                        "<td valign='top'>"+specialapp_commonport+common_colon+"</td>"+
                        "<td valign='top'>"+
                        "<select  class='input_select' id='commonport_select' valign='top'>"+
                        "</select>"+
                        "</td>"+
                        "<td valign='top'>"+common_name+common_colon+"</td>"+
                        "<td valign='top'><input type='text' class='input_style' id='input_name' maxlength='32'></td>"+
                     "</tr>"+
                     "<tr>"+
                         "<td valign='top'>"+firewall_label_status+common_colon+"</td>"+
                          "<td valign='top'>"+
                          "<select class='input_select' id='status_option' valign='top'>"+
                          "<option value='0' id='app_status0'>"+common_off+"</option>"+
                          "<option value='1' id='app_status1'>"+common_on+"</option>"+
                          "</select>"+
                          "</td>"+
                          "<td valign='top'>"+firewall_label_trigger_protocol+common_colon+"</td>"+
                          "<td valign='top'>"+
                          "<select class='input_select' id='trigger_protocol' valign='top'>"+
                          "<option value='" + PROTOCOL_BOTH_STR + "' id='triggerprotocol_val0'>"+firewall_label_tcp_or_udp+"</option>"+
                          "<option value='" + PROTOCOL_TCP_STR + "' id='triggerprotocol_val1'>"+firewall_label_tcp+"</option>"+
                          "<option value='" + PROTOCOL_UDP_STR + "' id='triggerprotocol_val2'>"+firewall_label_udp+"</option>"+
                          "</select>"+
                          "</td>"+
                     "</tr>"+
                     "<tr>"+
                          "<td valign='top'>"+specialapp_trigger_port_start+common_colon+"</td>"+
                          "<td valign='top'><input type='text' class='input_style' id='input_triportstart' maxlength='5'></td>"+
                          "<td valign='top'>"+specialapp_trigger_port_end+common_colon+"</td>"+
                          "<td valign='top'><input type='text' class='input_style' id='input_triportend' maxlength='5'></td>"+
                      "</tr>"+
                      "<tr>"+
                          "<td valign='top'>"+specialapp_open_port_start+common_colon+"</td>"+
                          "<td valign='top'><input type='text' class='input_style' id='input_portstart' maxlength='5'></td>"+
                          "<td valign='top'>"+specialapp_open_port_end+common_colon+"</td>"+
                          "<td valign='top' colspan='3'><input type='text' class='input_style' id='input_portend' maxlength='5'></td>"+
                      "</tr>"+
                      "<tr>"+
                          "<td valign='top'>"+security_label_wan_path+common_colon+"</td>"+
                          "<td valign='top' colspan='3'><select id='wan_path' class='input_style' style='width:180px;'></select></td>"+
                      "</tr>"+
                   "</table>"+
               "</div>";
 
function initpage(){
    save_all_info = [];
    g_save_name.length = 0;
    g_trigger_ports = [];
    g_open_ports = [];
    $("#service_list td").parent().remove();
    button_enable('apply', '0');
    getAjaxData('api/security/special-applications',function($xml){
        var ret = xml2object($xml);
        if(ret.type == 'response'){   
            if(typeof(ret.response.LanPorts) != 'undefined' && ret.response.LanPorts.LanPort){
                ret = $(ret.response.LanPorts.LanPort);
                if (ret.length >= SPECIAL_APP_NUM)
                {
                    button_enable('add_item', '0');
                }
                if(ret.length > 0)
                {
                     $(ret).each(function(i){
                         var newline = "<tr class='user_add_line' id='addlist"+i+"'>"+
                                  "<td width='90px' id='triggername"+i+"'>"+ret[i].SpecialApplicationTriggerName+"</td>"+
                                  "<td id='triggerstatus"+i+"'>"+getDArrayElement(StatusArray,ret[i].SpecialApplicationTriggerStatus, 'value')+"</td>"+
                                  "<td id='triport"+i+"' class='td_ltr'>"+portJoin(ret[i].SpecialApplicationTriggerPort,ret[i].SpecialApplicationTriggerPortEnd)+"</td>"+
                                  "<td id='triggerpro"+i+"'>"+getDArrayElement(portArray,ret[i].SpecialApplicationTriggerProtocol, 'value')+"</td>"+
                                  "<td id='openport"+i+"' class='td_ltr'>"+portJoin(ret[i].SpecialApplicationStartOpenPort0,ret[i].SpecialApplicationEndOpenPort0)+"</td>"+
                                  "<td  class='user_options' id='add_id_"+i+"'>"+
                                  "<a href='javascript:void(0);' id='button_edit"+ i +"' class='button_edit_list' >"+common_edit+"</a><br>"+
                                  "<a href='javascript:void(0);' id='button_delete"+ i +"' class='button_delete_list'>"+common_delete+"</a>"+
                                  "</td>"+
                                  "</tr>";
                         $("#service_list").append(newline); 
                         save_all_info.push([i,ret[i]]);  
                         g_save_name.push(ret[i].SpecialApplicationTriggerName);
                         g_trigger_ports.push([ret[i].SpecialApplicationTriggerPort,ret[i].SpecialApplicationTriggerPortEnd]);
                         g_open_ports.push([ret[i].SpecialApplicationStartOpenPort0,ret[i].SpecialApplicationEndOpenPort0]);
                    });
                }
              }
         }
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
function splitPortString(_port) {
    portarray = _port.split(';');
    return portarray;
}

function validateInput(){  
    var reg = /^[\w\s]{1,32}$/;
    parent.clearAllErrorLabel();
    if(!parent.$('#input_name').val().match(reg))
    {
        showErrorUnderTextbox_Parent('input_name', lan_filter_errorinfo);
        parent.$('#input_name').focus();
        return false;
    }
    if (dialogTitle == common_edit){
        for (var i in g_save_name){
            if (i != EditIndex && g_save_name[i] == parent.$('#input_name').val()){
                showErrorUnderTextbox_Parent('input_name', label_same_name_exist);
                parent.$('#input_name').focus();
                return false;
            }
        }
    }
    if (dialogTitle == common_add){
        for (var i in g_save_name){
            if (g_save_name[i] == parent.$('#input_name').val()){
                showErrorUnderTextbox_Parent('input_name', label_same_name_exist);
                parent.$('#input_name').focus();
                return false;
            }
        }
    }
    if(!isVaildPortForIPFilter(parent.$('#input_triportstart').val()))
    {
        showErrorUnderTextbox_Parent('input_triportstart', special_application_triporterrorinfo);
        parent.$('#input_triportstart').focus();
        return false;
    }
    if(!isVaildPortForIPFilter(parent.$('#input_triportend').val()))
    {
        showErrorUnderTextbox_Parent('input_triportend', special_application_triporterrorinfo);
        parent.$('#input_triportend').focus();
        return false;
    }
    if(parseInt(parent.$('#input_triportstart').val(),10)>parseInt(parent.$('#input_triportend').val(),10))
    {
        showErrorUnderTextbox_Parent('input_triportstart', lan_port_number_reverse);
        parent.$('#input_triportstart').focus();
        return false;
    }

    var tPortS = parseInt(parent.$('#input_triportstart').val(),10);
    var tPortE = parseInt(parent.$('#input_triportend').val(),10);
    if(((tPortS <= PRTT_FTP) && (tPortE >= PRTT_FTP))         
            || ((tPortS <= PRTT_TFTP) && (tPortE >= PRTT_TFTP))            
            || ((tPortS <= PRTT_H323) && (tPortE >= PRTT_H323))
            || ((tPortS <= PRTT_IRC) && (tPortE >= PRTT_IRC))
            || ((tPortS <= PRTT_SNMP_IN) && (tPortE >= PRTT_SNMP_IN))
            || ((tPortS <= PRTT_SNMP_OUT) && (tPortE >= PRTT_SNMP_OUT))
            || ((tPortS <= PRTT_TALK_IN) && (tPortE >= PRTT_TALK_IN))
            || ((tPortS <= PRTT_TALK_OUT) && (tPortE >= PRTT_TALK_OUT))
            || ((tPortS <= PRTT_PPTP) && (tPortE >= PRTT_PPTP))
            || ((tPortS <= PRTT_HTTP) && (tPortE >= PRTT_HTTP)))
    {
        showErrorUnderTextbox_Parent('input_triportstart', lan_filter_triporterrorinfo);
        parent.$('#input_triportstart').focus();
        return false;
    }

    if(!isVaildPortForIPFilter(parent.$('#input_portstart').val()))
    {
        showErrorUnderTextbox_Parent('input_portstart', special_application_triporterrorinfo);
        parent.$('#input_portstart').focus();
        return false;
    }
    if(!isVaildPortForIPFilter(parent.$('#input_portend').val()))
    {
        showErrorUnderTextbox_Parent('input_portend', special_application_triporterrorinfo);
        parent.$('#input_portend').focus();
        return false;
    }
    if(parseInt(parent.$('#input_portstart').val(),10) > parseInt(parent.$('#input_portend').val(),10))
    {
        showErrorUnderTextbox_Parent('input_portstart', lan_port_number_reverse);
        parent.$('#input_portstart').focus();
        return false;
    }
    
    if((parseInt(parent.$('#input_portstart').val(),10) <= CWMP_PORT) && (parseInt(parent.$('#input_portend').val(),10) >= CWMP_PORT))
    {
        showErrorUnderTextbox_Parent('input_portstart', lan_filter_openporterrorinfo);
        parent.$('#input_portstart').focus();
        return false;
    }
    return true;
}

function portnum_check()
{
    var tPortS = parseInt(parent.$('#input_triportstart').val(),10);
    var tPortE = parseInt(parent.$('#input_triportend').val(),10);
    var oPortS = parseInt(parent.$('#input_portstart').val(),10);
    var oPortE = parseInt(parent.$('#input_portend').val(),10);
    
    var temptport = 0;
    var tempoport = 0;
    var totaltrigport = 1 + tPortE - tPortS;
    var totalopenport = 1 + oPortE - oPortS;
    var i=0;
    if (dialogTitle == common_edit)
    {
            while(save_all_info[i] != null)
            {
                if(i != EditIndex)
                {
                    temptport = 1 + parseInt(save_all_info[i][1].SpecialApplicationTriggerPortEnd) - parseInt(save_all_info[i][1].SpecialApplicationTriggerPort);
                    totaltrigport += temptport;
                    tempoport = 1 + parseInt(save_all_info[i][1].SpecialApplicationEndOpenPort0) - parseInt(save_all_info[i][1].SpecialApplicationStartOpenPort0);
                    totalopenport += tempoport;
                }
                i++;
            }
    }
    else
    {
            while(save_all_info[i] != null)
            {            
                temptport = 1 + parseInt(save_all_info[i][1].SpecialApplicationTriggerPortEnd) - parseInt(save_all_info[i][1].SpecialApplicationTriggerPort);
                totaltrigport += temptport;
                tempoport = 1 + parseInt(save_all_info[i][1].SpecialApplicationEndOpenPort0) - parseInt(save_all_info[i][1].SpecialApplicationStartOpenPort0);
                totalopenport += tempoport;
                i++;
            }
    }

    if(totaltrigport > PORT_NUM)
    {
        showErrorUnderTextbox_Parent('input_triportstart', lan_filter_triport_diff_errorinfo);
        parent.$('#input_triportstart').focus();
        return false;
    }
    
    if(totalopenport > PORT_NUM)
    {
        showErrorUnderTextbox_Parent('input_portstart', lan_filter_openport_diff_errorinfo);
        parent.$('#input_portstart').focus();
        return false;
    }
    
    return true;    
}

function port_begin_end_check()
{
    var tPortS = parseInt(parent.$('#input_triportstart').val(),10);
    var tPortE = parseInt(parent.$('#input_triportend').val(),10);
    var oPortS = parseInt(parent.$('#input_portstart').val(),10);
    var oPortE = parseInt(parent.$('#input_portend').val(),10);
    
    var j = 0;
    if (dialogTitle == common_edit)
    {
        for (var i in g_trigger_ports){
            for(j = tPortS; j <=tPortE; j++)
            {
                if (i != EditIndex && g_trigger_ports[i][0] <= j && g_trigger_ports[i][1] >= j){
                    showErrorUnderTextbox_Parent('input_triportstart', lan_port_used);
                    parent.$('#input_triportstart').focus();
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }
        
        for (var i in g_open_ports){
            for(j = oPortS; j <=oPortE; j++)
            {
                if (i != EditIndex && g_open_ports[i][0] <= j && g_open_ports[i][1] >= j){
                    showErrorUnderTextbox_Parent('input_portstart', lan_port_used);
                    parent.$('#input_portstart').focus();
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    else
    {
        for (var i in g_trigger_ports){
            for(j = tPortS; j <=tPortE; j++)
            {
                if (g_trigger_ports[i][0] <= j && g_trigger_ports[i][1] >= j){
                    showErrorUnderTextbox_Parent('input_triportstart', lan_port_used);
                    parent.$('#input_triportstart').focus();
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }
        
        for (var i in g_open_ports){
            for(j = oPortS; j <=oPortE; j++)
            {
                if (g_open_ports[i][0] <= j && g_open_ports[i][1] >= j){
                    showErrorUnderTextbox_Parent('input_portstart', lan_port_used);
                    parent.$('#input_portstart').focus();
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    return true;
}

function listToDialog(EditIndex){
    parent.$('#input_name').val(save_all_info[EditIndex][1].SpecialApplicationTriggerName) ;
    parent.$('#status_option').val(save_all_info[EditIndex][1].SpecialApplicationTriggerStatus);
    parent.$('#trigger_protocol').val(save_all_info[EditIndex][1].SpecialApplicationTriggerProtocol) ;
    parent.$('#input_triportstart').val(save_all_info[EditIndex][1].SpecialApplicationTriggerPort) ;
    parent.$('#input_triportend').val(save_all_info[EditIndex][1].SpecialApplicationTriggerPortEnd)  ;
    parent.$('#input_portstart').val(save_all_info[EditIndex][1].SpecialApplicationStartOpenPort0) ;
    parent.$('#input_portend').val(save_all_info[EditIndex][1].SpecialApplicationEndOpenPort0);
    if(save_all_info[EditIndex][1].WanPath!='')
    {  
        setTimeout(function(){
            parent.$('#wan_path').val(save_all_info[EditIndex][1].WanPath);
        },1);
    }
}

function postData() {
    var submitObject = {};
    var lanPortArray = [];
    $('.user_add_line').each(function(i) {

        var LanPort = {
            SpecialApplicationTriggerName: $(this).children().eq(0).text(),
            SpecialApplicationTriggerStatus: getDArrayElement(StatusArray, $(this).children().eq(1).text(), 'key'),
            SpecialApplicationTriggerPort: $(this).children().eq(2).text().split("-")[0],
            SpecialApplicationTriggerPortEnd: typeof($(this).children().eq(2).text().split("-")[1]) == 'undefined' ? $(this).children().eq(2).text().split("-")[0] : $(this).children().eq(2).text().split("-")[1],
            SpecialApplicationTriggerProtocol: getDArrayElement(portArray, $(this).children().eq(3).text(), 'key'),
            SpecialApplicationStartOpenPort0: $(this).children().eq(4).text().split("-")[0],
            SpecialApplicationEndOpenPort0: typeof($(this).children().eq(4).text().split("-")[1]) == 'undefined' ? $(this).children().eq(4).text().split("-")[0] : $(this).children().eq(4).text().split("-")[1],
            WanPath:save_all_info[i][1].WanPath
        };
       lanPortArray.push(LanPort);
    });
    submitObject = {
        LanPorts: {
            LanPort: lanPortArray
        }            
    };
    var submitData = object2xml('request', submitObject);
    saveAjaxData('api/security/special-applications', submitData, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            button_enable('apply', '0');
            initpage();
        } else {
            parent.showInfoDialog(common_failed);
        }
    });
}
 /* -------------request commonport users'name from the config.xml----------------------------------------------  */
function get_Option_configname(){
    getConfigData('config/specialapp/config.xml', function($xml) {
        var commonport_ret = xml2object($xml);                    
        if (commonport_ret.type == 'config') {
            comret = commonport_ret.config.commonport;
            if($.isArray(comret)){
                for(i=0;i<comret.length;i++){
                    if(comret[i].name == "" || comret[i].name == " " || comret[i].name == null ){
                        
                        parent.$("#commonport_select").append("<option value="+i+">"+firewall_label_select+"</option>");
                    }else{
                        parent.$("#commonport_select").append("<option value="+i+">"+comret[i].name+"</option>");
                    }
                }               
            } else {
              return ;
            }               
        } else {
            log.error('Load language data failed');
        }
    });
}
/*---------------------infor the selectdata for users'change from the commonport'value is changged---------------*/
function optionCommonValue(){
    var commonport_select = 0;
    if($.isArray(comret)){
        commonport_select = parseInt(parent.$('#commonport_select').val(),10);
        parent.$('#input_name').val(comret[commonport_select].name);
        parent.$('#status_option').val(comret[commonport_select].Status);
        parent.$('#trigger_protocol').val(comret[commonport_select].TriggerProtocol);
        parent.$('#input_triportstart').val(comret[commonport_select].TriggerPortStart);
        parent.$('#input_triportend').val(comret[commonport_select].TriggerPortEnd);
        parent.$('#input_portstart').val(comret[commonport_select].OpenPortStart);
        parent.$('#input_portend').val(comret[commonport_select].OpenPortEnd);
    }else{
        commonport_select = parseInt(parent.$('#commonport_select').val(),10);
        parent.$('#input_name').val(comret.name);
        parent.$('#status_option').val(comret.Status);
        parent.$('#trigger_protocol').val(comret.TriggerProtocol);
        parent.$('#input_triportstart').val(comret.TriggerPortStart);
        parent.$('#input_triportend').val(comret.TriggerPortEnd);
        parent.$('#input_portstart').val(comret.OpenPortStart);
        parent.$('#input_portend').val(comret.OpenPortEnd);
    }
}
            
function saveToList(edit_id,EditIndex){               
    if(dialogTitle == common_edit){
        var filterinfo = {
            SpecialApplicationTriggerName : parent.$('#input_name').val(),
            SpecialApplicationTriggerStatus : parent.$('#status_option').val(),
            SpecialApplicationTriggerPort: parent.$('#input_triportstart').val(),
            SpecialApplicationTriggerPortEnd: parent.$('#input_triportend').val(),             
            TriggerPort: portJoin(parent.$('#input_triportstart').val(),parent.$('#input_triportend').val()),
            SpecialApplicationTriggerProtocol: parent.$('#trigger_protocol').val(),
            SpecialApplicationStartOpenPort0: parent.$('#input_portstart').val(),
            SpecialApplicationEndOpenPort0: parent.$('#input_portend').val(),
            openPort: portJoin(parent.$('#input_portstart').val(),parent.$('#input_portend').val()),
            WanPath:parent.$('#wan_path').val()
        };
       $('#triggername'+edit_id).text(parent.$('#input_name').val());
       $('#triggerstatus'+edit_id).text(getDArrayElement(StatusArray,parent.$('#status_option').val(), 'value'));
       $('#triport'+edit_id).text(portJoin(parent.$('#input_triportstart').val(),parent.$('#input_triportend').val()));
       $('#triggerpro'+edit_id).text(getDArrayElement(portArray,parent.$('#trigger_protocol').val(), 'value'));
       $('#openport'+edit_id).text(portJoin(parent.$('#input_portstart').val(),parent.$('#input_portend').val()));
       save_all_info.splice(EditIndex,1,[EditIndex,filterinfo]);
       g_save_name[EditIndex] = parent.$('#input_name').val();
       g_trigger_ports.splice(EditIndex,1,[filterinfo.SpecialApplicationTriggerPort,filterinfo.SpecialApplicationTriggerPortEnd]);
       g_open_ports.splice(EditIndex,1,[filterinfo.SpecialApplicationStartOpenPort0,filterinfo.SpecialApplicationEndOpenPort0]);
    } else if(dialogTitle == common_add){
        var new_index = 0;
        if(save_all_info.length == 0){
           new_index = 0;
        }else{
           new_index = save_all_info[save_all_info.length-1][0]+1;
        }
          
        var  newline = "<tr class='user_add_line' id='addlist"+new_index+"'>"+
                       "<td width='90px' id='triggername"+new_index+"'>"+parent.$("#input_name").val()+"</td>"+
                       "<td id='triggerstatus"+new_index+"'>"+getDArrayElement(StatusArray,parent.$('#status_option').val(), 'value')+"</td>"+
                       "<td id='triport"+new_index+"' class='td_ltr'>"+portJoin(parent.$('#input_triportstart').val(),parent.$('#input_triportend').val())+"</td>"+
                       "<td id='triggerpro"+new_index+"'>"+getDArrayElement(portArray,parent.$('#trigger_protocol').val(), 'value')+"</td>"+
                       "<td id='openport"+new_index+"' class='td_ltr'>"+ portJoin(parent.$('#input_portstart').val(),parent.$('#input_portend').val())+"</td>"+
                       "<td  class='user_options' id='add_id_"+new_index+"'>"+
                       "<a href='javascript:void(0);' id='button_edit"+ new_index +"' class='button_edit_list'>"+common_edit+"</a><br>"+
                       "<a href='javascript:void(0);' id='button_delete"+ new_index +"' class='button_delete_list'>"+common_delete+"</a>"+
                       "</td>"+
                       "</tr>";
        $("#service_list").append(newline); 
        var filterinfoAdd = {
            SpecialApplicationTriggerName : parent.$('#input_name').val(),
            SpecialApplicationTriggerStatus : parent.$('#status_option').val(),
            SpecialApplicationTriggerPort: parent.$('#input_triportstart').val(),
            SpecialApplicationTriggerPortEnd: parent.$('#input_triportend').val(),             
            TriggerPort: portJoin(parent.$('#input_triportstart').val(),parent.$('#input_triportend').val()),
            SpecialApplicationTriggerProtocol: parent.$('#trigger_protocol').val(),
            SpecialApplicationStartOpenPort0: parent.$('#input_portstart').val(),
            SpecialApplicationEndOpenPort0: parent.$('#input_portend').val(),
            openPort: portJoin(parent.$('#input_portstart').val(),parent.$('#input_portend').val()),
            WanPath:parent.$('#wan_path').val()
         };
        save_all_info.push([new_index, filterinfoAdd]); 
        g_save_name.push(filterinfoAdd.SpecialApplicationTriggerName);
        g_trigger_ports.push([filterinfoAdd.SpecialApplicationTriggerPort, filterinfoAdd.SpecialApplicationTriggerPortEnd]);
        g_open_ports.push([filterinfoAdd.SpecialApplicationStartOpenPort0, filterinfoAdd.SpecialApplicationEndOpenPort0]);
    }         
}

/*-----------------------------save function---------------*/
function save_pop_click() {
    if(validateInput() && portnum_check() && port_begin_end_check()){
        saveToList(g_edit_id,EditIndex);
        parent.clearDialog();
        button_enable('apply', '1'); 
        if(dialogTitle = common_add) {
            if(save_all_info.length < SPECIAL_APP_NUM){
                button_enable('add_item', '1');
            } else {
                button_enable('add_item', '0');
            }
        }
        return true;
    }
    else{
        return false;
    }
}

/*-----------------------------cancel function--------------------*/
function cancel_pop_click() {
    parent.clearDialog();
}

/*----------------------------delete button popok click-----------------*/
function delete_pop_click() {
    $(".user_add_line").eq(g_deleteIndex).remove();
    parent.clearDialog();
    save_all_info.splice(g_deleteIndex,1);
    g_save_name.splice(g_deleteIndex,1);
    g_trigger_ports.splice(g_deleteIndex,1);
    g_open_ports.splice(g_deleteIndex,1);
    button_enable('apply', '1'); 
}

$(document).ready(function() {  
    g_edit_id = "";
    button_enable('add_item', '1');
    initpage();
    $('.button_delete_list').live("click",function(){
        g_deleteIndex = $(".button_delete_list").index(this);
        parent.showConfirmDialog(firewall_hint_delete_list_item, delete_pop_click,cancel_pop_click);
        $(".button_delete_list").blur();
        button_enable('add_item', '1');
    });
    //show add item control
    $('#add_item').click(function() {  
        if ($('.user_add_line').length >= SPECIAL_APP_NUM){
            button_enable('add_item', '0');
        } else {  
            parent.call_dialog(common_add, addhtml, common_save, "button1_save", common_cancel, "button2_cancel",save_pop_click,cancel_pop_click,"input_name");
            parent.$("#wan_path").append(g_wanpath);
            parent.$('#wan_path').css({'width': 'auto'});
            get_Option_configname();
           /* create common port is selected by a click */
            parent.$('#commonport_select').bind('change',function(){
                optionCommonValue();
            });
            dialogTitle = common_add;
       }
    });
    
    $('.button_edit_list').live('click',function(){      
        EditIndex = $(".button_edit_list").index(this);
        g_edit_id = $(this).parent().attr("id").split("_")[2];
        parent.call_dialog(common_edit, addhtml, common_save, "button1_save", common_cancel, "button2_cancel",save_pop_click,cancel_pop_click,"input_name");
        parent.$("#wan_path").append(g_wanpath);
        parent.$('#wan_path').css({'width': 'auto'});
        get_Option_configname();
        listToDialog(EditIndex);
        dialogTitle = common_edit;
        parent.$('#commonport_select').bind('change',function(){
            optionCommonValue();
        });
    });

    $('#apply').click(function() {
       if (isButtonEnable('apply')) {
           parent.showConfirmDialog(firewall_hint_submit_list_item, postData);
       }
    });
});