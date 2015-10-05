var LAN_IP_FILTER_NUM = 16;

var g_source_num = 0;
var g_config_firewall = 0;
var firewall_status = null;

var g_editIndex = 0;
var g_editID = "";
var g_deleteIndex = 0;
var PORT_ONE = 1;
var PORT_LAST = 65535;

var SAVE_PROTOCOL_TCP = "TCP";
var SAVE_PROTOCOL_UDP = "UDP";
var SAVE_PROTOCOL_TCPUDP = "TCP/UDP";
var SAVE_PROTOCAL_ALL = "ALL";
var SAVE_PROTOCAL_ICMP = "ICMP";

var BLACK_FILTER_WORD = "black";
var WHITE_FILTER_WORD = "white";

var black_filter = [];
var white_filter = [];

var addhtml = "<div>"+
                   "<table id='lan_ip_filter_table'>"+
                        "<tr>"+
                            "<th width='130' height='1'></th><th width='160'></th><th width='130'></th><th width='160'></th>"+
                        "</tr>"+
                        "<tr>"+
                            "<td height='42' valign='top'>"+firewall_label_protocol+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<select id='show_protocol_select'class='input_select'>"+
                                    "<option value='"+SAVE_PROTOCOL_TCP+"'>"+SAVE_PROTOCOL_TCP+"</option>"+
                                    "<option value='"+SAVE_PROTOCOL_UDP+"'>"+SAVE_PROTOCOL_UDP+"</option>"+
                                    "<option value='"+SAVE_PROTOCOL_TCPUDP+"'>"+SAVE_PROTOCOL_TCPUDP+"</option>"+
                                    "<option value='"+SAVE_PROTOCAL_ICMP+"'>"+SAVE_PROTOCAL_ICMP+"</option>"+
                                    "<option value='"+SAVE_PROTOCAL_ALL+"'>"+SAVE_PROTOCAL_ALL+"</option>"+
                                "</select>"+
                            "</td>"+
                            "<td height='42' valign='top'>"+firewall_label_status+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<select id='show_status_select' class='input_select'>"+
                                    "<option value='1'>"+common_on+"</option>"+
                                    "<option value='0'>"+common_off+"</option>"+
                                "</select>"+
                            "</td>"+
                        "</tr>"+
                        "<tr>"+
                            "<td height='42' valign='top'>"+lan_filter_lan_ip_start+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_start_ip' class='input_style' maxlength='39'/>"+
                            "</td>"+
                            "<td height='42' valign='top'>"+lan_filter_lan_ip_end+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_end_ip' class='input_style' maxlength='39'/>"+
                            "</td>"+
                        "</tr>"+
                        "<tr>"+
                            "<td height='42' valign='top'>"+lan_filter_lan_port_start+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_start_port' class='input_style' maxlength='5'/>"+
                            "</td>"+
                            "<td height='42' valign='top'>"+lan_filter_lan_port_end+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_end_port' class='input_style' maxlength='5'/>"+
                            "</td>"+
                        "</tr>"+
                        "<tr class='wanth'>"+
                            "<td height='42' valign='top'>"+lan_filter_wan_ip_start+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_wan_start_ip' class='input_style' maxlength='39'/>"+
                            "</td>"+
                            "<td height='42' valign='top'>"+lan_filter_wan_ip_end+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_wan_end_ip' class='input_style' maxlength='39'/>"+
                            "</td>"+
                        "</tr>"+
                        "<tr class='wanth'>"+
                            "<td height='42' valign='top'>"+lan_filter_wan_port_start+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_wan_start_port' class='input_style' maxlength='5'/>"+
                            "</td>"+
                            "<td height='42' valign='top'>"+lan_filter_wan_port_end+common_colon+"</td>"+
                            "<td height='42' valign='top'>"+
                                "<input type='text' id='show_wan_end_port' class='input_style' maxlength='5'/>"+
                            "</td>"+
                        "</tr>"+
                        "<tr class='wanth'>"+
                            "<td height='20'id='lan_filter_same_rule' style='font-size:16px;color:red' valign='top' colspan='2'></td>"+
                        "</tr>"+
                   "</table>"+
              "</div>";

/*----------------------------------------------------------------------*/
function ipTranApply(ipaddr)
{
    var ipout = [];
    var ipoutstring = '';
    var idx = 0;
    if (ipaddr == null || ipaddr == undefined || ipaddr == '')
    {
        return '';
    }

    ipout = ipaddr.split('.');
    if (ipout.length != 4)
    {
        return '';
    }
    else
    {
        for (idx = 3; idx >= 0; idx--)
        {
            if (ipout[idx] == '*')
            {
                ipout[idx] = '0';
            }
            else
            {
                break;
            }
        }
    }
    ipoutstring = ipoutstring.concat(ipout[0], '.', ipout[1], '.', ipout[2], '.', ipout[3]);
    return ipoutstring;
}

function ipTranGet(ipAddress, num)
{
    var ipout = [];
    var ipoutstring = '';
    var i = 0;
    if (null == ipAddress || undefined == ipAddress || '' == ipAddress)
    {
        return '';
    }

    ipout = ipAddress.split('.');
    if (ipout.length != 4)
    {
        return '';
    }
    else
    {
        for (i = 3; i > -1; i--)
        {
            if ('0' == ipout[i])
            {
                if (num > 0)
                {
                    ipout[i] = '*';
                    num--;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    ipoutstring = ipoutstring.concat(ipout[0], '.', ipout[1], '.', ipout[2], '.', ipout[3]);
    return ipoutstring;
}


function ipGetMaskNum(ipAddress)
{
    var ipout = [];
    var ipoutstring = '';
    var i = 0;
    var ipaddNum = 0;
    if (null == ipAddress || undefined == ipAddress || '' == ipAddress)
    {
        return '';
    }

    ipout = ipAddress.split('.');
    if (ipout.length != 4)
    {
        return '';
    }
    else
    {
        for (i = 3; i > -1; i--)
        {
            if (ipout[i] == '*')
            {
                ipaddNum++;
            }
            else
            {
                break;
            }
        }
    }
    return ipaddNum;
}

function sourceIpTranApplyValid(ipaddr)
{
    var ipout = [];
    var ipoutstring = '';
    var i = 0;
    if (ipaddr == null || ipaddr == undefined || ipaddr == '')
    {
        return '';
    }

    ipout = ipaddr.split('.');
    if (ipout.length != 4)
    {
        return '';
    }
    else
    {
        for (i = 3; i > -1; i--)
        {
        if (ipout[i] == '*')
        {
                    ipout[i] = '0';
        }
        else
        {
            break;
        }
        }
    }
    ipoutstring = ipoutstring.concat(ipout[0], '.', ipout[1], '.', ipout[2], '.', ipout[3]);
    return ipoutstring;
}

function isValidIpAddressForIPFilter(address) 
{
    var addrParts = address.split('.');
    if (addrParts.length != 4)
    {
        return false;
    }

    for (i = 0; i < 4; i++)
    {
        if ((addrParts[i] == '*') && (i != 3))
        {
        if (addrParts[i + 1] != '*')
        {
            return false;
        }
        }
        if (true == (isNaN(addrParts[i])) && (addrParts[i] != '*'))
        {
            return false;
        }

        if ('' == addrParts[i])
        {
            return false;
        }

        if (addrParts[i].indexOf(' ') != -1)
        {
            return false;
        }

        if ((0 == addrParts[i].indexOf('0')) && (1 != addrParts[i].length))
        {
            return false;
        }
    }

    if ((addrParts[0] <= 0 || addrParts[0] == 127 || addrParts[0] > 223) ||
    (addrParts[1] < 0 || addrParts[1] > 255) ||
    (addrParts[2] < 0 || addrParts[2] > 255) ||
    (addrParts[3] < 0 || addrParts[3] >= 255))
    {
        return false;
    }

    return true;
}

/*-----------------------------get set info from config.xml----------------------------------------*/
function lanIPFilter_getConfig() {
    getConfigData('config/firewall/config.xml', function($xml) {
        g_config_firewall = _xml2feature($xml);
        if ('0' == g_config_firewall.lanipfilter.wan)
            {
                $('.wanth').hide();
                parent.$('.wanth').hide();
            }
            else
            {
                $('.wanth').show();
                parent.$('.wanth').show();
            }
        },
    {
        sync: true
    });
}
function showCurrentFilterlist(){
    var current_filterlist = [];

    if($("#select_ip_allow_deny").val() == "1") {
        current_filterlist = white_filter;
    }
    else {
        current_filterlist = black_filter;
    } 
    if((typeof(current_filterlist) != "undefined")&& current_filterlist!='') {
        if ($.isArray(current_filterlist)) {
            $(current_filterlist).each(function(i){
                setNewLineInitPage(current_filterlist[i],i,WHITE_FILTER_WORD);
            });
        }
        else {
            setNewLineInitPage(current_filterlist,0,WHITE_FILTER_WORD);
        }
    }
    if($(".show_ip_filter_list").length >= LAN_IP_FILTER_NUM) {
        button_enable("add_item",0);
    }
    else {
        button_enable("add_item",1);
    }
}

/*-------------------------first enter this page------------------------------------*/
function initPage() {
    black_filter = [];
    white_filter = [];

    $('.user_add_line').remove();
    button_enable('apply', '0');
    getAjaxData('api/security/firewall-switch', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            firewall_status = ret.response;
        }
        else {
            return;
        }
    }, {
        sync: true
    });
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response')
        {
            parent.dhcpPageVar = ret.response;
            parent.initDhcp();
        }
        else {
            return;
        }
    });
    getAjaxData('api/security/lan-ip-filter', function($xml) {
        var ret = xml2object($xml);
        if(typeof(ret.response) == "undefined") {
            return;
        }
        if (typeof(ret.response.BlackIPFilters) != 'undefined' && typeof(ret.response.WhiteIPFilters) != 'undefined') {
            $("#select_ip_allow_deny").val(ret.response.LanIPFilterPolicy);
            if(typeof(ret.response.WhiteIPFilters.IPFilter) != "undefined") {
                if($.isArray(ret.response.WhiteIPFilters.IPFilter)){
                    white_filter = ret.response.WhiteIPFilters.IPFilter;
                }else{
                    white_filter.push(ret.response.WhiteIPFilters.IPFilter);
                }
            }
            if(typeof(ret.response.BlackIPFilters.IPFilter) != "undefined") {
                if($.isArray(ret.response.BlackIPFilters.IPFilter)){
                    black_filter = ret.response.BlackIPFilters.IPFilter;
                }else{
                    black_filter.push(ret.response.BlackIPFilters.IPFilter);
                }
            }
            showCurrentFilterlist();
        } else {
            return;
        }
        
        
        if (firewall_status != null && firewall_status.FirewallMainSwitch == 0) {
            button_enable('add_item', '0');
            $("#select_ip_allow_deny").attr("disabled",true);
            $(".button_edit_list").css("color","#666666").css("cursor","default");
            $(".button_delete_list").css("color","#666666").css("cursor","default");
            parent.showInfoDialog(IDS_security_message_firewall_disabled);
            return false;
        }
        if (firewall_status != null && firewall_status.FirewallIPFilterSwitch == 0) {
            button_enable('add_item', '0');
            $("#select_ip_allow_deny").attr("disabled",true);
            $(".button_edit_list").css("color","#666666").css("cursor","default");
            $(".button_delete_list").css("color","#666666").css("cursor","default");
            parent.showInfoDialog(IDS_security_message_ip_address_disabled);
        }
    });
}

/*------------------------function set newline-----------------------------*/
function setNewLineInitPage(lanIPFilterData,key) {
    var getStatus  = lanIPFilterData.LanIPFilterStatus==1?common_on:common_off;
    var newline = "";
    var oneinfo = null;
   
   if("0" == g_config_firewall.lanipfilter.wan) {
       newline = "<tr class='user_add_line show_ip_filter_list'>";
       if (lanIPFilterData.LanIPFilterLanStartAddress == lanIPFilterData.LanIPFilterLanEndAddress){
           newline += "<td id='lan_ip_start_"+key+"' class='td_ltr'>" + lanIPFilterData.LanIPFilterLanStartAddress + "</td>";
       }
       else {
           newline += "<td id='lan_ip_start_"+key+"' class='td_ltr'>" +lanIPFilterData.LanIPFilterLanStartAddress + "-<br/>" + lanIPFilterData.LanIPFilterLanEndAddress + "</td>";
       }
       if (lanIPFilterData.LanIPFilterLanStartPort == lanIPFilterData.LanIPFilterLanEndPort){
           newline += "<td id='lan_ip_port_"+key+"' class='td_ltr'>"+lanIPFilterData.LanIPFilterLanStartPort+"</td>";
       }
       else {
           newline += "<td id='lan_ip_port_"+key+"' class='td_ltr'>"+lanIPFilterData.LanIPFilterLanStartPort+"-"+lanIPFilterData.LanIPFilterLanEndPort+"</td>";
       }
       newline += "<td id='protocol_"+key+"'>"+lanIPFilterData.LanIPFilterProtocol+"</td>"+
                      "<td id='status_"+key+"'>"+getStatus+"</td>"+
                      "<td id='ip_start_"+key+"' class='user_options'>"+
                           "<a href='javascript:void(0);' id='button_edit"+ key +"' class='button_edit_list'>"+common_edit+"</a><br>"+
                           "<a href='javascript:void(0);' id='button_delete"+ key +"' class='button_delete_list'>"+common_delete+"</a>"+
                      "</td>"+
                "</tr>";
   } 
   else {
       newline = "<tr class='user_add_line show_ip_filter_list'>";
       if (lanIPFilterData.LanIPFilterLanStartAddress == lanIPFilterData.LanIPFilterLanEndAddress){
           newline += "<td id='lan_ip_start_"+key+"' class='td_ltr'>"+ lanIPFilterData.LanIPFilterLanStartAddress + "</td>";
       }
       else {
           newline += "<td id='lan_ip_start_"+key+"' class='td_ltr'>"+ lanIPFilterData.LanIPFilterLanStartAddress +"-<br/>"+ lanIPFilterData.LanIPFilterLanEndAddress +"</td>";
       }
       if (lanIPFilterData.LanIPFilterLanStartPort == lanIPFilterData.LanIPFilterLanEndPort){
           newline += "<td id='lan_ip_port_"+key+"' class='td_ltr'>"+lanIPFilterData.LanIPFilterLanStartPort+"</td>";
       }
       else {
           newline += "<td id='lan_ip_port_"+key+"' class='td_ltr'>"+lanIPFilterData.LanIPFilterLanStartPort+"-"+lanIPFilterData.LanIPFilterLanEndPort+"</td>";
       }
       if (lanIPFilterData.LanIPFilterWanStartAddress == lanIPFilterData.LanIPFilterWanEndAddress){
           newline += "<td id='wan_ip_start_"+key+"' class='td_ltr'>"+ lanIPFilterData.LanIPFilterWanStartAddress +"</td>";
       }
       else {
           newline += "<td id='wan_ip_start_"+key+"' class='td_ltr'>"+ lanIPFilterData.LanIPFilterWanStartAddress +"-<br/>"+ lanIPFilterData.LanIPFilterWanEndAddress +"</td>";
       }
       if (lanIPFilterData.LanIPFilterWanStartPort == lanIPFilterData.LanIPFilterWanEndPort) {
           newline += "<td id='wan_port_"+key+"' class='td_ltr'>"+lanIPFilterData.LanIPFilterWanStartPort+"</td>";
       }
       else {
           newline += "<td id='wan_port_"+key+"' class='td_ltr'>"+lanIPFilterData.LanIPFilterWanStartPort+"-"+lanIPFilterData.LanIPFilterWanEndPort+"</td>";
       }
       newline += "<td id='protocol_"+key+"'>"+lanIPFilterData.LanIPFilterProtocol+"</td>"+
                      "<td id='status_"+key+"'>"+getStatus+"</td>"+
                      "<td id='ip_start_"+key+"' class='user_options'>"+
                           "<a href='javascript:void(0);' id='button_edit"+ key +"' class='button_edit_list'>"+common_edit+"</a><br>"+
                           "<a href='javascript:void(0);' id='button_delete"+ key +"' class='button_delete_list'>"+common_delete+"</a>"+
                      "</td>"+
                "</tr>";
   }
   $("#service_list").append(newline);
}

function IpAddress2DecNum(address)
{
	var addrParts = address.split('.');
	var num = 0;
	for (i = 0; i < 4; i++)
	{
		num += parseInt(addrParts[i], 10) * Math.pow(256, 3 - i);
	}
	return num;
} 

function formatIpv6address(ipaddress)
{
	if(ipaddress.indexOf("::") > -1 )
	{
		if(ipaddress.indexOf("::") == 0)
		{
			if("::" == ipaddress)
			{
				ipaddress = "0:0:0:0:0:0:0:0";
				return ipaddress;
			}
			else
			{
				var i = 0;
				var count_colon = 0;
				var length_ip = ipaddress.length;
				for(i = 0; i < length_ip; i++)
				{
					if(":" == ipaddress.charAt(i))
					{
						count_colon ++;
					}
				}
				var new_colon_str = "";
				if(7 == count_colon)
				{
					new_colon_str = "0:0:";
				}
				else if(6 == count_colon)
				{
					new_colon_str = "0:0:0:";
				}
				else if(5 == count_colon)
				{
					new_colon_str = "0:0:0:0:";
				}
				else if(4 == count_colon)
				{
					new_colon_str = "0:0:0:0:0:";
				}
				else if(3 == count_colon)
				{
					new_colon_str = "0:0:0:0:0:0:";
				}
				else if(2 == count_colon)
				{
					new_colon_str = "0:0:0:0:0:0:0:";
				}
				return ipaddress.replace("::", new_colon_str);
			}
		}
		else
		{
			var i = 0;
			var count_colon = 0;
			var length_ip = ipaddress.length;
			for(i = 0; i < length_ip; i++)
			{
				if(":" == ipaddress.charAt(i))
				{
					count_colon ++;
				}
			}
			var new_colon_str = "";
			if(7 == count_colon)
			{
				new_colon_str = ":0:";
			}
			else if(6 == count_colon)
			{
				new_colon_str = ":0:0:";
			}
			else if(5 == count_colon)
			{
				new_colon_str = ":0:0:0:";
			}
			else if(4 == count_colon)
			{
				new_colon_str = ":0:0:0:0:";
			}
			else if(3 == count_colon)
			{
				new_colon_str = ":0:0:0:0:0:";
			}
			else if(2 == count_colon)
			{
				new_colon_str = ":0:0:0:0:0:0:";
			}
			return ipaddress.replace("::", new_colon_str);
		}
	}
	else
	{
		return ipaddress;
	}
}
function Ipv6Compare(start, end)
{
	var i = 0;
	var format_start_ip = formatIpv6address(start);
	var format_end_ip = formatIpv6address(end);
	var start_ip_array = format_start_ip.split(":");
	var end_ip_array = format_end_ip.split(":");
	var start_value = 0;
	var end_value = 0;
	
	for(i = 0; i < 8; i++)
	{
		start_value = parseInt(start_ip_array[i], 16);
		end_value = parseInt(end_ip_array[i], 16);
		if(start_value == end_value)
		{
			continue;
		}
		else if(start_value > end_value)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}

function checkMutiIp(id){
    var val= $.trim(parent.$('#'+id).val()).toLowerCase();
    if((0 == val.indexOf("ff"))||(0 == val.indexOf("::")))
    {
        showErrorUnderTextbox_Parent(id,lan_filter_errorinfo);
        parent.$('#'+id).focus();
        return false;
    }
    return true;
}
/*---------------------------------validate ip and port-------------------------------------*/
function checkIPAndPort(){
    parent.clearAllErrorLabel();
    if($.trim(parent.$("#show_start_ip").val()) == "" && $.trim(parent.$("#show_end_ip").val()) == "" 
        && $.trim(parent.$("#show_start_port").val()) == "" && $.trim(parent.$("#show_end_port").val()) == "" && $.trim(parent.$("#show_wan_start_ip").val()) == "" 
        && $.trim(parent.$("#show_wan_end_ip").val()) == "" && $.trim(parent.$("#show_wan_start_port").val()) == "" && $.trim(parent.$("#show_wan_end_port").val()) == "") 
    {
        showErrorUnderTextbox_Parent("show_start_ip",lan_filter_errorinfo);
        parent.$('#show_start_ip').focus();
        return false;
    }
    //sourse ip can not be Multicast address
    if(false == checkMutiIp("show_start_ip")){
            return false;
    }
    if(!isValidIpv6Address(parent.$("#show_start_ip").val()) && $.trim(parent.$("#show_start_ip").val()) != "") {
        if(!isValidIpAddressForIPFilter(parent.$("#show_start_ip").val()) 
            || parent.$("#show_start_ip").val() == parent.dhcpLanIPAddress 
            || !isSameSubnetAddrs(sourceIpTranApplyValid(parent.$("#show_start_ip").val()), parent.dhcpLanIPAddress, parent.dhcpLanNetmask))
        {
            showErrorUnderTextbox_Parent("show_start_ip",lan_filter_errorinfo);
            parent.$('#show_start_ip').focus();
            return false;
        }
    }
    //sourse ip can not be Multicast address
    if(false == checkMutiIp("show_end_ip")){
            return false;
    }
    if ($.trim(parent.$("#show_end_ip").val()) != "") {
        if(isValidIpv6Address(parent.$("#show_start_ip").val())) {
            if(!isValidIpv6Address(parent.$("#show_end_ip").val())) {
                showErrorUnderTextbox_Parent("show_end_ip",lan_filter_errorinfo);
                parent.$('#show_end_ip').focus();
                return false;
            }
        }
        else {
            if((!isValidIpAddressForIPFilter(parent.$("#show_end_ip").val())
                || parent.$("#show_end_ip").val() == parent.dhcpLanIPAddress || !isSameSubnetAddrs(sourceIpTranApplyValid(parent.$("#show_end_ip").val()), parent.dhcpLanIPAddress, parent.dhcpLanNetmask)))
            {
                showErrorUnderTextbox_Parent("show_end_ip",lan_filter_errorinfo);
                parent.$('#show_end_ip').focus();
                return false;
            }
        }
    } 
    
    if($.trim(parent.$("#show_start_ip").val()) != "" && $.trim(parent.$("#show_end_ip").val()) != "")
    {
    	var start_ip = $.trim(parent.$("#show_start_ip").val());
    	var end_ip = $.trim(parent.$("#show_end_ip").val());
    	if(isValidIpAddressForIPFilter(start_ip) && isValidIpAddressForIPFilter(end_ip))
    	{
    		if(IpAddress2DecNum(start_ip) > IpAddress2DecNum(end_ip))
    		{
	            showErrorUnderTextbox_Parent("show_end_ip",lan_ip_reverse);
	            parent.$('#show_end_ip').focus();
	            return false;
    		}
    	}
    	else if(isValidIpv6Address(start_ip) && isValidIpv6Address(end_ip))
    	{
    		if(!Ipv6Compare(start_ip, end_ip))
    		{
	            showErrorUnderTextbox_Parent("show_end_ip",lan_ip_reverse);
	            parent.$('#show_end_ip').focus();
	            return false;
    		}
    	}
    	else
    	{
            showErrorUnderTextbox_Parent("show_end_ip",lan_filter_errorinfo);
            parent.$('#show_end_ip').focus();
            return false;
    	}
    }
    
    if (!isVaildPortForIPFilter(parent.$("#show_start_port").val()) && $.trim(parent.$("#show_start_port").val()) != "")
    {        
        showErrorUnderTextbox_Parent("show_start_port",lan_filter_errorinfo); 
        parent.$('#show_start_port').focus();   
        return false;  
    }
    if($.trim(parent.$("#show_end_port").val()) != "") {
        if (!isVaildPortForIPFilter(parent.$("#show_end_port").val()) || parseInt(parent.$("#show_end_port").val(),10)<parseInt(parent.$("#show_start_port").val(),10))
        {       
            showErrorUnderTextbox_Parent("show_end_port",lan_filter_errorinfo); 
            parent.$('#show_end_port').focus();       
            return false;  
        }
        if($.trim(parent.$("#show_start_port").val()) == "") {
            showErrorUnderTextbox_Parent("show_start_port",lan_filter_errorinfo); 
            parent.$('#show_start_port').focus();   
            return false;  
        }
    }
    if("0" != g_config_firewall.lanipfilter.wan) {
        if(!isValidIpv6Address(parent.$("#show_wan_start_ip").val()) && $.trim(parent.$("#show_wan_start_ip").val()) != "") {
            if (!isValidIpAddress(parent.$("#show_wan_start_ip").val())
            || parent.$("#show_wan_start_ip").val() == parent.dhcpLanIPAddress ){
            showErrorUnderTextbox_Parent("show_wan_start_ip",lan_filter_errorinfo);
            parent.$('#show_wan_start_ip').focus();      
            return false;  
            }
        }
        if(false == checkMutiIp("show_wan_end_ip")){
            return false;
        }
        if($.trim(parent.$("#show_wan_end_ip").val()) != "") {
            if(isValidIpv6Address(parent.$("#show_wan_start_ip").val())) {
                if(!isValidIpv6Address(parent.$("#show_wan_end_ip").val())) {
                    showErrorUnderTextbox_Parent("show_wan_end_ip",lan_filter_errorinfo);
                    parent.$('#show_wan_end_ip').focus();     
                    return false; 
                }
            }
            else {
                if (!isValidIpAddress(parent.$("#show_wan_end_ip").val())
                    || parent.$("#show_wan_end_ip").val() == parent.dhcpLanIPAddress) 
                {      
                    showErrorUnderTextbox_Parent("show_wan_end_ip",lan_filter_errorinfo);
                    parent.$('#show_wan_end_ip').focus();     
                    return false;  
                }
            }
        }
        if(false == checkMutiIp("show_wan_start_ip")){
            return false;
        }
        
	    if($.trim(parent.$("#show_wan_start_ip").val()) != "" && $.trim(parent.$("#show_wan_end_ip").val()) != "")
	    {
	    	var start_wan_ip = $.trim(parent.$("#show_wan_start_ip").val());
	    	var end_wan_ip = $.trim(parent.$("#show_wan_end_ip").val());
	    	if(isValidIpAddressForIPFilter(start_wan_ip) && isValidIpAddressForIPFilter(end_wan_ip))
	    	{
	    		if(IpAddress2DecNum(start_wan_ip) > IpAddress2DecNum(end_wan_ip))
	    		{
		            showErrorUnderTextbox_Parent("show_wan_end_ip",lan_ip_reverse);
		            parent.$('#show_wan_end_ip').focus();
		            return false;
	    		}
	    	}
	    	else if(isValidIpv6Address(start_wan_ip) && isValidIpv6Address(end_wan_ip))
	    	{
	    		if(!Ipv6Compare(start_wan_ip, end_wan_ip))
	    		{
		            showErrorUnderTextbox_Parent("show_wan_end_ip",lan_ip_reverse);
		            parent.$('#show_wan_end_ip').focus();
		            return false;
	    		}
	    	}
	    	else
	    	{
	            showErrorUnderTextbox_Parent("show_wan_end_ip",lan_filter_errorinfo);
	            parent.$('#show_wan_end_ip').focus();
	            return false;
	    	}
	    }
	    
        if (!isVaildPortForIPFilter(parent.$("#show_wan_start_port").val()) && $.trim(parent.$("#show_wan_start_port").val()) != "")
        {        
            showErrorUnderTextbox_Parent("show_wan_start_port",lan_filter_errorinfo); 
            parent.$('#show_wan_start_port').focus();    
            return false;  
        }
        if($.trim(parent.$("#show_wan_end_port").val()) != "") {
             if (!isVaildPortForIPFilter(parent.$("#show_wan_end_port").val()) || parseInt(parent.$("#show_wan_end_port").val(),10)<parseInt(parent.$("#show_wan_start_port").val(),10))
            {       
                showErrorUnderTextbox_Parent("show_wan_end_port",lan_filter_errorinfo);
                parent.$('#show_wan_end_port').focus();       
                return false;
            }
            if($.trim(parent.$("#show_wan_start_port").val()) == "") {
                showErrorUnderTextbox_Parent("show_wan_start_port",lan_filter_errorinfo); 
                parent.$('#show_wan_start_port').focus();    
                return false;  
            }
        }
        
        var start_ip_lan = $.trim(parent.$("#show_start_ip").val());
    	var end_ip_lan = $.trim(parent.$("#show_end_ip").val());
    	var start_ip_wan = $.trim(parent.$("#show_wan_start_ip").val());
    	var end_ip_wan = $.trim(parent.$("#show_wan_end_ip").val());
    	if(((isValidIpAddressForIPFilter(start_ip_lan) || isValidIpAddressForIPFilter(end_ip_lan)) && (isValidIpv6Address(start_ip_wan) || isValidIpv6Address(end_ip_wan)))
    		|| ((isValidIpv6Address(start_ip_lan) || isValidIpv6Address(end_ip_lan)) && (isValidIpAddressForIPFilter(start_ip_wan) || isValidIpAddressForIPFilter(end_ip_wan))))
    	{
            showErrorUnderTextbox_Parent("show_wan_start_ip", lan_wan_ip_same_format);
            parent.$('#show_wan_start_ip').focus();
            return false;
    	}
    }
    return true;
}
function check_allicmp_protocol_port()
{   
    parent.clearAllErrorLabel();
    if(parent.$("#show_protocol_select").val()==SAVE_PROTOCAL_ALL || parent.$("#show_protocol_select").val()==SAVE_PROTOCAL_ICMP)
    {
        if($.trim(parent.$("#show_start_port").val()) != "")
        {
            showErrorUnderTextbox_Parent("show_start_port",lan_filter_conf_port_errorinfo); 
            parent.$('#show_start_port').focus();    
            return false; 
        }
        if($.trim(parent.$("#show_end_port").val()) != "")
        {
            showErrorUnderTextbox_Parent("show_end_port",lan_filter_conf_port_errorinfo); 
            parent.$('#show_end_port').focus();    
            return false; 
        }
        if($.trim(parent.$("#show_wan_start_port").val()) != "")
        {
            showErrorUnderTextbox_Parent("show_wan_start_port",lan_filter_conf_port_errorinfo); 
            parent.$('#show_wan_start_port').focus();    
            return false; 
        }
        if($.trim(parent.$("#show_wan_end_port").val()) != "")
        {
            showErrorUnderTextbox_Parent("show_wan_end_port",lan_filter_conf_port_errorinfo); 
            parent.$('#show_wan_end_port').focus();    
            return false; 
        }      
    }
    return true;
}
function isSameRule()
{
    var issameRule=false;
    var editinfo = null;
    var check_data_list = null;
    
    
    if($("#select_ip_allow_deny").val() == "1"){
        check_data_list = white_filter;
    }else{
        check_data_list = black_filter;
    }
    
  
    if('1' == g_config_firewall.lanipfilter.wan) 
    {
        editinfo = {
                LanIPFilterLanStartAddress : parent.$("#show_start_ip").val(),
                LanIPFilterLanEndAddress : $.trim(parent.$("#show_end_ip").val()) == "" ? parent.$("#show_start_ip").val() : parent.$("#show_end_ip").val(),
                LanIPFilterLanStartPort : parent.$("#show_start_port").val(),
                LanIPFilterLanEndPort : $.trim(parent.$("#show_end_port").val()) == "" ? parent.$("#show_start_port").val() : parent.$("#show_end_port").val(),
                LanIPFilterProtocol : parent.$("#show_protocol_select").val(),
                LanIPFilterStatus : parent.$("#show_status_select").val(),
                LanIPFilterWanStartAddress : parent.$("#show_wan_start_ip").val(),
                LanIPFilterWanEndAddress : $.trim(parent.$("#show_wan_end_ip").val()) == "" ? parent.$("#show_wan_start_ip").val() : parent.$("#show_wan_end_ip").val(),
                LanIPFilterWanStartPort : parent.$("#show_wan_start_port").val(),
                LanIPFilterWanEndPort : $.trim(parent.$("#show_wan_end_port").val()) == "" ? parent.$("#show_wan_start_port").val() : parent.$("#show_wan_end_port").val()
                };
    }else{
        editinfo = {
            LanIPFilterLanStartAddress : parent.$("#show_start_ip").val(),
            LanIPFilterLanEndAddress : $.trim(parent.$("#show_end_ip").val()) == "" ? parent.$("#show_start_ip").val() : parent.$("#show_end_ip").val(),
            LanIPFilterLanStartPort : parent.$("#show_start_port").val(),
            LanIPFilterLanEndPort : $.trim(parent.$("#show_end_port").val()) == "" ? parent.$("#show_start_port").val() : parent.$("#show_end_port").val(),
            LanIPFilterProtocol : parent.$("#show_protocol_select").val(),
            LanIPFilterStatus : parent.$("#show_status_select").val()
        };
    }
    for (var i in check_data_list)
    {
        if (i == g_editIndex && (parent.$(".dialog_header_left").text() == common_edit))
        {
              continue;
        }else{
            if(check_data_list[i].LanIPFilterLanStartAddress == editinfo.LanIPFilterLanStartAddress
            && check_data_list[i].LanIPFilterLanEndAddress == editinfo.LanIPFilterLanEndAddress
            && check_data_list[i].LanIPFilterLanStartPort == editinfo.LanIPFilterLanStartPort
            && check_data_list[i].LanIPFilterLanEndPort == editinfo.LanIPFilterLanEndPort
            && check_data_list[i].LanIPFilterProtocol == editinfo.LanIPFilterProtocol)
            {
                if('1' == g_config_firewall.lanipfilter.wan
                    && check_data_list[i].LanIPFilterWanStartAddress == editinfo.LanIPFilterWanStartAddress
                    && check_data_list[i].LanIPFilterWanEndAddress == editinfo.LanIPFilterWanEndAddress
                    && check_data_list[i].LanIPFilterWanStartPort == editinfo.LanIPFilterWanStartPort
                    && check_data_list[i].LanIPFilterWanEndPort == editinfo.LanIPFilterWanEndPort)
                    {
                        parent.$("#lan_filter_same_rule").text(label_same_rule_exist);
                        return true;
                }else{
                    parent.$("#lan_filter_same_rule").text(label_same_rule_exist);
                    return true;
                }
            }
        }
    }
    if($("#select_ip_allow_deny").val() == "1"){
        if(parent.$(".dialog_header_left").text() != common_edit){
            white_filter.push(editinfo);
        }else{
            white_filter.splice(g_editIndex,1,editinfo);
        }
    }else{
       if(parent.$(".dialog_header_left").text() != common_edit){
            black_filter.push(editinfo);
        }else{
            black_filter.splice(g_editIndex,1,editinfo);
        }
    }
                

    return issameRule;
}

/*--------------------------add btn click--------------------------------------*/
function clickAdd(){
  if(isButtonEnable("add_item")){
    if($(".show_ip_filter_list").length < LAN_IP_FILTER_NUM){
          if('0' == g_config_firewall.lanipfilter.wan) {
            parent.call_dialog(common_add,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_pop_ok,clickCancel);
            parent.$('.wanth').hide();
            parent.$("#show_dialog_table").css("height","150px"); 
          }
          else {
            parent.call_dialog(common_add,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_pop_ok,clickCancel);
            $('.wanth').show();
            parent.$('.wanth').show();
          }       
      }
      else {
        return;
      } 
  }
    else{
      return;
    }
}

/*---------------------------------apply btn --------------------------------------*/
function clickApply(){
    var submitObject = {};
    var apparray = [];
    
    if('0' == g_config_firewall.lanipfilter.wan) {
        $(".show_ip_filter_list").each(function(i) {
            var ipinfo = setApplyData($(this).children(),0);
            apparray.push(ipinfo);
        });
    }
    else {
        $(".show_ip_filter_list").each(function(i) {
            var ipinfo = setApplyData($(this).children(),1);
            apparray.push(ipinfo);
        });
    }
    submitObject = {
        LanIPFilterPolicy : $("#select_ip_allow_deny").val(),
        IPFilters : {
            IPFilter : apparray
        }
    };
    var sub = object2xml("request",submitObject);
    saveAjaxData("api/security/lan-ip-filter",sub,function($xml){
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

function setApplyData(data,wanEnable) {
    var returnInfo = null;
    if(wanEnable == 0) {
        returnInfo = {
            LanIPFilterProtocol: data.eq(4).text(),
            LanIPFilterStatus: data.eq(5).text() == common_on?1:0,
            LanIPFilterLanStartAddress: isValidIpv6Address(data.eq(0).text().split("-")[0]) ? data.eq(0).text().split("-")[0] : ipTranApply(data.eq(0).text().split("-")[0]),
            LanIPFilterLanEndAddress: isValidIpv6Address(data.eq(0).text().split("-")[1] == null ? data.eq(0).text().split("-")[0] : data.eq(0).text().split("-")[1]) ? data.eq(0).text().split("-")[1] == null ? data.eq(0).text().split("-")[0] : data.eq(0).text().split("-")[1] : ipTranApply(data.eq(0).text().split("-")[1] == null ? data.eq(0).text().split("-")[0] : data.eq(0).text().split("-")[1]),
            LanIPFilterLanStartPort: data.eq(1).text().split("-")[0],
            LanIPFilterLanEndPort: data.eq(1).text().split("-")[1] == null?data.eq(1).text().split("-")[0]:data.eq(1).text().split("-")[1],
            LanIPFilterSrcStartIPMask: ipGetMaskNum(data.eq(0).text())
        };
    }
    else if(wanEnable == 1) {
        returnInfo = {
            LanIPFilterProtocol: data.eq(4).text(),
            LanIPFilterStatus: data.eq(5).text() == common_on?1:0,
            LanIPFilterLanStartAddress: isValidIpv6Address(data.eq(0).text().split("-")[0]) ? data.eq(0).text().split("-")[0] : ipTranApply(data.eq(0).text().split("-")[0]),
            LanIPFilterLanEndAddress: isValidIpv6Address(data.eq(0).text().split("-")[1] == null ? data.eq(0).text().split("-")[0] : data.eq(0).text().split("-")[1]) ? data.eq(0).text().split("-")[1] == null ? data.eq(0).text().split("-")[0] : data.eq(0).text().split("-")[1] : ipTranApply(data.eq(0).text().split("-")[1] == null ? data.eq(0).text().split("-")[0] : data.eq(0).text().split("-")[1]),
            LanIPFilterLanStartPort: data.eq(1).text().split("-")[0],
            LanIPFilterLanEndPort: data.eq(1).text().split("-")[1] == null?data.eq(1).text().split("-")[0]:data.eq(1).text().split("-")[1],
            LanIPFilterWanStartAddress: isValidIpv6Address(data.eq(2).text().split("-")[0]) ? data.eq(2).text().split("-")[0] : ipTranApply(data.eq(2).text().split("-")[0]),
            LanIPFilterWanEndAddress: isValidIpv6Address(data.eq(2).text().split("-")[1] == null ? data.eq(2).text().split("-")[0] : data.eq(2).text().split("-")[1]) ? data.eq(2).text().split("-")[1] == null ? data.eq(2).text().split("-")[0] : data.eq(2).text().split("-")[1] : ipTranApply(data.eq(2).text().split("-")[1] == null ? data.eq(2).text().split("-")[0] : data.eq(2).text().split("-")[1]),
            LanIPFilterWanStartPort: data.eq(3).text().split("-")[0],
            LanIPFilterWanEndPort: data.eq(3).text().split("-")[1] == null?data.eq(3).text().split("-")[0]:data.eq(3).text().split("-")[1],
            LanIPFilterSrcStartIPMask: ipGetMaskNum(data.eq(0).text()),
            LanIPFilterDestStartIPMask: ipGetMaskNum(data.eq(2).text())
        };
    }
    return returnInfo;
}


/*---------------------------------delete btn click------------------------------------------------*/
function clickDel(){
    if (firewall_status != null && firewall_status.FirewallIPFilterSwitch == 0) {
        return;
    }
    if (firewall_status != null && firewall_status.FirewallMainSwitch == 0){
        return;
    }
    g_deleteIndex = $(".button_delete_list").index(this);
    parent.showConfirmDialog(firewall_hint_delete_list_item, delete_pop_ok,clickCancel);
    $(".button_delete_list").blur();
}

/*--------------------delete pop ok function------------------------------*/
function delete_pop_ok() {
    $(".user_add_line").eq(g_deleteIndex).remove();
    parent.clearDialog();
    button_enable("apply", "1");
    if($("#select_ip_allow_deny").val() == "1"){
        white_filter.splice(g_deleteIndex,1);
    }else{
        black_filter.splice(g_deleteIndex,1);
    }
    if($(".show_ip_filter_list").length < LAN_IP_FILTER_NUM) {
       button_enable("add_item",1);
    }
}

/*------------------------click cancel btn--------------------------------*/
function clickCancel(){
    parent.clearDialog();
}

/*---------------------------save pop ok function--------------------------*/
function save_pop_ok() {
    if(check_allicmp_protocol_port()&&checkIPAndPort() && !isSameRule()){
        if (parent.$(".dialog_header_left").text() == common_edit) {
            $('#protocol_'+g_editID.split("_")[2]).text(parent.$("#show_protocol_select option:selected").text());
            if (parent.$("#show_start_ip").val() == parent.$("#show_end_ip").val() || $.trim(parent.$("#show_end_ip").val()) == ""){
                $("#lan_ip_start_"+g_editID.split("_")[2]).html(parent.$("#show_start_ip").val());
            }
            else {
                $("#lan_ip_start_"+g_editID.split("_")[2]).html(parent.$("#show_start_ip").val()+"-<br/>"+parent.$("#show_end_ip").val());
            }
            if (parent.$("#show_start_port").val() == parent.$("#show_end_port").val() || $.trim(parent.$("#show_end_port").val()) == "") {
                $("#lan_ip_port_"+g_editID.split("_")[2]).text(parent.$("#show_start_port").val());
            }
            else {
                $("#lan_ip_port_"+g_editID.split("_")[2]).text(parent.$("#show_start_port").val()+"-"+parent.$("#show_end_port").val());
            }
            $("#status_"+g_editID.split("_")[2]).text(parent.$("#show_status_select").val()=="1"?common_on:common_off);
            if('1' == g_config_firewall.lanipfilter.wan)
                if (parent.$("#show_wan_start_ip").val() == parent.$("#show_wan_end_ip").val() || $.trim(parent.$("#show_wan_end_ip").val()) == ""){
                    $("#wan_ip_start_"+g_editID.split("_")[2]).html(parent.$("#show_wan_start_ip").val());
                }
                else {
                    $("#wan_ip_start_"+g_editID.split("_")[2]).html(parent.$("#show_wan_start_ip").val()+"-<br/>"+parent.$("#show_wan_end_ip").val());
                }
                if (parent.$("#show_wan_start_port").val() == parent.$("#show_wan_end_port").val() || $.trim(parent.$("#show_wan_end_port").val()) == "") {
                    $("#wan_port_"+g_editID.split("_")[2]).text(parent.$("#show_wan_start_port").val());
                }
                else {
                    $("#wan_port_"+g_editID.split("_")[2]).text(parent.$("#show_wan_start_port").val()+"-"+parent.$("#show_wan_end_port").val());
                }
            }
        else {
            var number = 0;
            if($("#select_ip_allow_deny").val() == "1"){
                if(white_filter.length == 0) {
                    number = 0;
                }
                else {
                    number = white_filter.length;
                }
            }else{
                if(black_filter.length == 0) {
                    number = 0;
                }
                else {
                    number = black_filter.length;
                }
            }
            var addline = "";
            if('0' == g_config_firewall.lanipfilter.wan) {
                addline = "<tr class='user_add_line show_ip_filter_list'>";
                if (parent.$("#show_start_ip").val() == parent.$("#show_end_ip").val() || $.trim(parent.$("#show_end_ip").val()) == "") {
                    addline += "<td id='lan_ip_start_"+number+"' class='td_ltr'>"+parent.$("#show_start_ip").val()+"</td>";
                }
                else {
                    addline += "<td id='lan_ip_start_"+number+"' class='td_ltr'>"+parent.$("#show_start_ip").val()+"-<br/>"+parent.$("#show_end_ip").val()+"</td>";
                }
                if (parent.$("#show_start_port").val() == parent.$("#show_end_port").val() || $.trim(parent.$("#show_end_port").val()) == "") {
                    addline += "<td id='lan_ip_port_"+number+"' class='td_ltr'>"+parent.$("#show_start_port").val()+"</td>";
                }
                else {
                    addline += "<td id='lan_ip_port_"+number+"' class='td_ltr'>"+parent.$("#show_start_port").val()+"-"+parent.$("#show_end_port").val()+"</td>";
                }
                addline += "<td id='protocol_"+number+"'>"+parent.$("#show_protocol_select option:selected").text()+"</td>"+
                                 "<td id='status_"+number+"'>"+parent.$("#show_status_select option:selected").text()+"</td>"+
                                 "<td class='user_options' id='ip_start_"+number+"'>"+
                                    "<a href='javascript:void(0);' id='button_edit"+ number +"' class='button_edit_list'>"+common_edit+"</a>"+
                                    "<a href='javascript:void(0);' id='button_delete"+ number +"' class='button_delete_list'>"+common_delete+"</a>"+
                                 "</td>"+
                            "</tr>";
            }
            else {
                addline = "<tr class='user_add_line show_ip_filter_list'>";
                if (parent.$("#show_start_ip").val() == parent.$("#show_end_ip").val() || $.trim(parent.$("#show_end_ip").val()) == "") {
                    addline += "<td id='lan_ip_start_"+number+"' class='td_ltr'>"+parent.$("#show_start_ip").val()+"</td>";
                }
                else {
                    addline += "<td id='lan_ip_start_"+number+"' class='td_ltr'>"+parent.$("#show_start_ip").val()+"-<br/>"+parent.$("#show_end_ip").val()+"</td>";
                }
                if (parent.$("#show_start_port").val() == parent.$("#show_end_port").val() || $.trim(parent.$("#show_end_port").val()) == "") {
                    addline += "<td id='lan_ip_port_"+number+"' class='td_ltr'>"+parent.$("#show_start_port").val()+"</td>";
                }
                else {
                    addline += "<td id='lan_ip_port_"+number+"' class='td_ltr'>"+parent.$("#show_start_port").val()+"-"+parent.$("#show_end_port").val()+"</td>";
                }
                if (parent.$("#show_wan_start_ip").val() == parent.$("#show_wan_end_ip").val() || $.trim(parent.$("#show_wan_end_ip").val()) == "") {
                    addline += "<td id='wan_ip_start_"+number+"' class='td_ltr'>"+parent.$("#show_wan_start_ip").val()+"</td>";
                }
                else {
                    addline += "<td id='wan_ip_start_"+number+"' class='td_ltr'>"+parent.$("#show_wan_start_ip").val()+"-<br/>"+parent.$("#show_wan_end_ip").val()+"</td>";
                }
                if (parent.$("#show_wan_start_port").val() == parent.$("#show_wan_end_port").val() || $.trim(parent.$("#show_wan_end_port").val()) == "") {
                    addline += "<td id='wan_port_"+number+"' class='td_ltr'>"+parent.$("#show_wan_start_port").val()+"</td>";
                }
                else {
                    addline += "<td id='wan_port_"+number+"' class='td_ltr'>"+parent.$("#show_wan_start_port").val()+"-"+parent.$("#show_wan_end_port").val()+"</td>";
                }
                addline += "<td id='protocol_"+number+"'>"+parent.$("#show_protocol_select option:selected").text()+"</td>"+
                                 "<td id='status_"+number+"'>"+parent.$("#show_status_select option:selected").text()+"</td>"+
                                 "<td class='user_options' id='ip_start_"+number+"'>"+
                                    "<a href='javascript:void(0);' id='button_edit"+ number +"' class='button_edit_list'>"+common_edit+"</a><br>"+
                                    "<a href='javascript:void(0);' id='button_delete"+ number +"' class='button_delete_list'>"+common_delete+"</a>"+
                                 "</td>"+
                            "</tr>";
                
            }
            $("#service_list").append(addline);
            if($(".show_ip_filter_list").length >= LAN_IP_FILTER_NUM) {
                button_enable("add_item",0);
            }
        }     
        button_enable("apply",1);
        parent.clearDialog();
        return true;
    }
    else {
        return false;
    }
}


/*----------------------------ready----------------------------------------*/
$(document).ready(function() {
    lanIPFilter_getConfig();
    initPage();
    $("#add_item").live("click",clickAdd);
    $(".button_delete_list").live("click",clickDel);
	//协议为ALL时，LAN和WAN port选项置灰
	parent.$("#show_protocol_select").die().live("change",function(){
		if((parent.$("#show_protocol_select").val()== SAVE_PROTOCAL_ALL) )
		{
		      //select protocol all ，disableLAN start port,LAN end port, WAN start port,WAN end port	
			parent.$("#show_start_port").attr('disabled',true);	
			parent.$("#show_end_port").attr('disabled',true); 
			parent.$("#show_wan_start_port").attr('disabled',true); 
			parent.$("#show_wan_end_port").attr('disabled',true); 
		}	 
		else{
			parent.$("#show_start_port").removeAttr('disabled');		
			parent.$("#show_end_port").removeAttr('disabled');		
			parent.$("#show_wan_start_port").removeAttr('disabled');
			parent.$("#show_wan_end_port").removeAttr('disabled');
			}
	 });
    $('#apply').click(function() {
        if (!isButtonEnable('apply')) {
            return;
        }
        parent.showConfirmDialog(firewall_hint_submit_list_item, clickApply);
    });   

    $('.button_edit_list').live('click', function() {
        if (firewall_status != null && firewall_status.FirewallIPFilterSwitch == 0) {
            return;
        }
        if (firewall_status != null && firewall_status.FirewallMainSwitch == 0){
            return;
        }
        
        g_editIndex = $('.button_edit_list').index(this);
        g_editID = $(this).parent().attr("id");  

            var current_rule = null;
            if($("#select_ip_allow_deny").val() == "1"){
                current_rule = white_filter[g_editIndex];
            }else{
                current_rule = black_filter[g_editIndex];
            }

        if ('0' == g_config_firewall.lanipfilter.wan)
        {
            parent.call_dialog(common_edit,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_pop_ok,clickCancel,"show_start_ip");
            $('.wanth').hide();
            parent.$('.wanth').hide();
            parent.$("#show_dialog_table").css("height","150px");         
        }
        else
        {
            parent.call_dialog(common_edit,addhtml,common_save,"show_edit_ok",common_cancel,"show_edit_cancel",save_pop_ok,clickCancel,"show_start_ip"); 
            $('.wanth').show();
            parent.$('.wanth').show();
            parent.$("#show_wan_start_ip").attr("value",current_rule.LanIPFilterWanStartAddress);
            parent.$("#show_wan_end_ip").attr("value",current_rule.LanIPFilterWanEndAddress);
            parent.$("#show_wan_start_port").attr("value",current_rule.LanIPFilterWanStartPort);
            parent.$("#show_wan_end_port").attr("value",current_rule.LanIPFilterWanEndPort); 
        }
        parent.$("#show_protocol_select").attr("value",current_rule.LanIPFilterProtocol);
        parent.$("#show_status_select").attr("value",current_rule.LanIPFilterStatus);
        parent.$("#show_start_ip").attr("value",current_rule.LanIPFilterLanStartAddress);
        parent.$("#show_end_ip").attr("value",current_rule.LanIPFilterLanEndAddress);
        parent.$("#show_start_port").attr("value",current_rule.LanIPFilterLanStartPort);
        parent.$("#show_end_port").attr("value",current_rule.LanIPFilterLanEndPort);      
    });
    $("#select_ip_allow_deny").change(function() {
        button_enable('apply',1);
        $(".user_add_line").remove();
        showCurrentFilterlist();
    });
});

