var g_cbo_time_zone;
var g_sntp_DstStart;
var g_sntp_DstEnd;
var g_sntp_DstOffset;
var g_label;

var g_ntpServers = [];
var g_ntplist = '';
var g_ntpServerNum;
var g_wanstatus;
var g_indexstring = '';
var SNTP_SERVER_OTHER = "Other";
var DEFAULT_GATEWAY_IP = '192.168.1.1';
var GATEWAY_ExternalIPv6Address = "";
var g_LanIPv6Address = "";


function genlocaltimename()
{
    var obj = $('#cboTimeZone')[0];
    var index = obj.selectedIndex;
    var val;
    var gmt_val;
    var localtimename = '';
    if(4==obj.options[index].text.indexOf(')'))
    {
        val='';
    }else{
        val=obj.options[index].text.substring(4,obj.options[index].text.indexOf(')'));
    }
    g_label = index;

    localtimename += "GMT";
    if (val == '')
    {
        val = '00:00:00';
        gmt_val = 0;
    }else{
        var split_val = val.split(':');
        if(split_val.length == 3)
        {
            gmt_val = Math.abs(parseInt(split_val[0],10))*3600+parseInt(split_val[1],10)*60+parseInt(split_val[2],10);
        }else{
            gmt_val = Math.abs(parseInt(split_val[0],10))*3600+parseInt(split_val[1],10)*60;
        }
    }
    if(val.indexOf('-')!=-1)
    {
        val = val.replace('-','\+');
    }else if(val.indexOf('+')!=-1)
    {
        val = val.replace('\+','-');
    }
    localtimename += val;
    if ($('#daylight_switch_enable').attr('checked'))
    {
        localtimename += 'DST';
        var x1= $('#offsethh').val();
        var x2 = $('#offsetmm').val();
        var x3 = $('#offsetss').val();
        var total_offset = parseInt(x1,10)*3600+parseInt(x2,10)*60+parseInt(x3,10);
        if(val.indexOf("-") != -1)
        {
            gmt_val += total_offset;
            localtimename += '-';
        }else{
            gmt_val = gmt_val > total_offset ? gmt_val - total_offset : (localtimename += '-',total_offset - gmt_val);
        }
        var result1 = Math.floor(gmt_val/3600);
        if(result1 < 10)
        {
            localtimename += '0';
        }
        localtimename += result1.toString();
        localtimename += ":";
        gmt_val %=3600;
        result1 = Math.floor(gmt_val/60);
        if(result1 < 10)
        {
            localtimename += '0';
        }
        localtimename += result1.toString();
        localtimename += ":";
        gmt_val %=60;
        result1 = gmt_val;
        if(result1 < 10)
        {
            localtimename += '0';
        }
        localtimename += result1.toString();
        
        
        /*if (x1 < 10)
        {
            localtimename += '0';
        }
        localtimename += x1;
        localtimename += ':';

        var x2 = $('#offsetmm').val();
        if (x2 < 10)
        {
            localtimename += '0';
        }
        localtimename += x2;
        localtimename += ':';

        var x3 = $('#offsetss').val();
        if (x3 < 10)
        {
            localtimename += '0';
        }
        localtimename += x3;*/
        

        localtimename += ',M';
        localtimename += $('#monthstart').val();
        localtimename += '.';

        localtimename += $('#weekstart').val();
        localtimename += '.';

        localtimename += $('#daystart').val();
        localtimename += '/';

        var x4 = $('#hhstart').val();
        if (x4 < 10)
        {
            localtimename += '0';
        }
        localtimename += x4;
        localtimename += ':';

        var x5 = $('#mmstart').val();
        if (x5 < 10)
        {
            localtimename += '0';
        }
        localtimename += x5;
        localtimename += ':';

        var x6 = $('#ssstart').val();
        if (x6 < 10)
        {
            localtimename += '0';
        }
        localtimename += x6;

        localtimename += ',M';

        localtimename += $('#monthend').val();
        localtimename += '.';

        localtimename += $('#weekend').val();
        localtimename += '.';

        localtimename += $('#dayend').val();
        localtimename += '/';

        var x7 = $('#hhend').val();
        if (x7 < 10)
        {
            localtimename += '0';
        }
        localtimename += x7;
        localtimename += ':';

        var x8 = $('#mmend').val();
        if (x8 < 10)
        {
            localtimename += '0';
        }
        localtimename += x8;
        localtimename += ':';

        var x9 = $('#ssend').val();
        if (x9 < 10)
        {
            localtimename += '0';
        }
        localtimename += x9;
    }
    return localtimename;
}


function writecurrtime(sntpcurrtime)
{
    var currtime2;
    var currtime = sntpcurrtime.split('T');
    currtime2 = currtime[1].split('Z');
    $('#sntp_currenttime').append((currtime[0] + " " + currtime2[0]));
}

function writemonth()
{
    dw("<option value='1'>" + sntp_label_jan + "</option>");
    dw("<option value='2'>" + sntp_label_feb + "</option>");
    dw("<option value='3'>" + sntp_label_mar + "</option>");
    dw("<option value='4'>" + sntp_label_apr + "</option>");
    dw("<option value='5'>" + sntp_label_may + "</option>");
    dw("<option value='6'>" + sntp_label_jun + "</option>");
    dw("<option value='7'>" + sntp_label_jul + "</option>");
    dw("<option value='8'>" + sntp_label_aug + "</option>");
    dw("<option value='9'>" + sntp_label_sep + "</option>");
    dw("<option value='10'>" + sntp_label_oct + "</option>");
    dw("<option value='11'>" + sntp_label_nov + "</option>");
    dw("<option value='12'>" + sntp_label_dec + "</option>");
}

function writeweek()
{
    var i;
    for(i=0;i<5;i++)
    {
        var j = i+1;
        dw("<option value='"+ j + "'>" + eval('sntp_label_week_' +i) + "</option>");
    }
}

function writeday()
{
    var i;
    for(i = 0;i < 7;i++)
    {
        dw("<option value='" + i + "'>" + eval('sntp_label_day_' + i) + "</option>");
    }

}

function writehh(hh)
{
    var i;
    for(i = 0; i < hh; i++ )
    {
        dw("<option value=" + i + ">" + i + "</option>");
    }
}

function writemm()
{
    var i;
    for(i = 0; i < 60; i++ )
    {
        dw("<option value=" + i + ">" + i + "</option>");
    }
}
function writess()
{
    var i;
    for(i = 0; i < 60; i++ )
    {
        dw("<option value=" + i + ">" + i + "</option>");
    }
}

function writeTimeZoneList()
{
    var i;
    for(i=0;i<76;i++)
    {
        dw("<option value='" + i + "'>" + eval('sntp_label_timeZones_' + i) + "</option>");
    }
}

function sntpInit(optionlist)
{ 
    var textbox=optionlist.charAt(optionlist.length-1);
    if($.browser.msie) { 
        if(parseInt($.browser.version.charAt(0), 10) === 8) {
            var top_value = $('#ntpServer'+textbox).position().top; 
            $('#ntpServerOther'+textbox).css({'position':'relative','top':(top_value-4)}); 
        } 
    }
    if($('#'+optionlist).val() == SNTP_SERVER_OTHER)
    {
        $('#ntpServerOther'+textbox).removeAttr('disabled');
        $('#ntpServerOther'+textbox).css('display','');
        $('#ntpServerOther'+textbox).focus();
    }
    else
    {
        $('#'+textbox).val("");
        $('#ntpServerOther'+textbox).attr('value','');
        $('#ntpServerOther'+textbox).attr('disabled','1');
        $('#ntpServerOther'+textbox).css('display','none');
    }
}

function sntpChange(optionlist)
{ 
    var textbox=optionlist.charAt(optionlist.length-1);
    if($('#'+optionlist).val() == SNTP_SERVER_OTHER)
    {
        $('#ntpServerOther'+textbox).removeAttr('disabled');
        $('#ntpServerOther'+textbox).css('display','');
        $('#ntpServerOther'+textbox).focus();
    }
    else
    {
        $('#'+textbox).val("");
        $('#ntpServerOther'+textbox).attr('value','');
        $('#ntpServerOther'+textbox).attr('disabled','1');
        $('#ntpServerOther'+textbox).css('display','none');
    }
}

function writeNtpServer()
{
    var j;
    for(j=0;j<g_ntpServerNum;j++)
    {
        g_ntplist+=('<tr><td class="sntp_timer_server_td_text"><label class="sntp_tag" id="label_sntp_SNTPSer'+(j+1)+'">'+sntp_time_server+(j+1)+common_colon+'</label></td>');
        g_ntplist+=('<td class="sntp_timer_server_td_input"><select class="sntp_server_select" id="ntpServer'+(j+1)+'">');
        g_ntplist+=('<option value="">'+wlan_label_none+'</option>');
        var i;
        for(i = 0; i < g_ntpServers.length; i++ )
        {
            g_ntplist += ("<option value=" + g_ntpServers[i] + ">" + g_ntpServers[i] + "</option>");
        }
        g_ntplist += ('<option value="' + SNTP_SERVER_OTHER + '">' + sntp_label_Other + '</option>');
        g_ntplist += ('</select>&nbsp;&nbsp;&nbsp');
        g_ntplist += ('<input class="sntp_server_input" style="display:none" disabled="1" id="ntpServerOther' + ( j + 1 ) + '" maxlength="128" type="text"></td></tr>');
    }
}

function writesynstatus(status)
{
    switch(status)
        {
            case 'Disabled':
                $('#sntp_status').append(common_disable);
                break;
            case 'Unsynchronized':
                $('#sntp_status').append(sntp_label_status_unsyn);
                break;
            case 'Synchronized':
                $('#sntp_status').append(sntp_label_status_syn);
                break;
            case 'Error_FailedToSynchronize':
                $('#sntp_status').append(sntp_label_status_err);
                break;
            default:
                $('#sntp_status').append(common_disable);
                break;
        }
}


//split timezone
function Splittimezone(timezonestring)
{
    try
    {
        var time;
        if(timezonestring.indexOf('<DST>')>=0)
        {
            time = timezonestring.split('<DST>');
        }else
        {
            time = timezonestring.split('DST');
        }
        var time1=time[1].split(',');
        g_sntp_DstStart=time1[1];
        g_sntp_DstEnd=time1[2];
        g_sntp_DstOffset=time1[0];
    }catch(e){
        g_sntp_DstStart=0;
        g_sntp_DstEnd=0;
        g_sntp_DstOffset=0;
    }
}

//set timezone offset
function settimezoneoffset(timezonestring)
{
    var index = 0;
    var indexstring ;
    if(timezonestring.indexOf('<DST>')>=0)
    {
        g_cbo_time_zone = timezonestring.split('<DST>')[0];
    }
    else if(timezonestring.indexOf('DST')>=0)
    {
        g_cbo_time_zone = timezonestring.split('DST')[0];
    }else
    {
        g_cbo_time_zone = timezonestring;
    }
    if(g_cbo_time_zone.length&&g_cbo_time_zone.indexOf('GMT')>=0)
    {
        if(g_cbo_time_zone.indexOf('<GMT>')>=0)
        {
            indexstring = g_cbo_time_zone.substr(5);
        }else
        {
            indexstring = g_cbo_time_zone.substr(3);
        }
        g_indexstring = indexstring;
        
        if('00:00:00'==indexstring||'+00:00:00'==indexstring||'-00:00:00'==indexstring||''==indexstring)
        {
            index = 25;
        }else{
            if(indexstring.indexOf("+") != -1)
            {
                indexstring = indexstring.replace('\+','-');
            }else if(indexstring.indexOf("-") != -1)
            {
                indexstring = indexstring.replace('-','\+');
            }
            var i;
            for(i=0;i<76;i++)
            {
                if(eval('sntp_label_timeZones_'+i).indexOf(indexstring)>=0)
                {
                    index = i;
                    break;
                }
            }
        }
        
    }
    return index;
}


//set SntpDststart
function setSntpDststart(){
    try
    {
        var start1;
        var start2;
        var start3;
        if(g_sntp_DstStart.indexOf('/') != -1)
        {
            start1 = g_sntp_DstStart.split('/');
            if(start1[0].indexOf('.') != -1)
            {
                start2 = start1[0].split('.');
                $('#monthstart').val(parseInt(start2[0].substr(1,start2[0].length-1),10));
                $('#weekstart').val(parseInt(start2[1],10));
                $('#daystart').val(parseInt(start2[2],10));
            }
            if(start1[1].indexOf(':') != -1)
            {
                start3 = start1[1].split(':');
                $('#hhstart').val(parseInt(start3[0],10));
                $('#mmstart').val(parseInt(start3[1],10));
                $('#ssstart').val(parseInt(start3[2],10));
            }
            }
    }
    catch(e)
    {
        $('#monthstart').val(sntp_label_jan);
        $('#weekstart').val(sntp_label_week_0);
        $('#daystart').val(sntp_label_day_0);
        $('#hhstart').val(0);
        $('#mmstart').val(0);
        $('#ssstart').val(0);
    }
}


//set SntpDstOffset
function setSntpDstend(){
    try
    {
        var end1;
        var end2;
        var end3;
        if(g_sntp_DstEnd.indexOf('/') != -1)
        {
            end1 = g_sntp_DstEnd.split('/');
            if(end1[0].indexOf('.') != -1)
            {
                end2 = end1[0].split('.');
                $('#monthend').val(parseInt(end2[0].substr(1,end2[0].length-1),10));
                $('#weekend').val(parseInt(end2[1],10));
                $('#dayend').val(parseInt(end2[2],10));
            }
            if(end1[1].indexOf(':') != -1)
            {
                end3 = end1[1].split(':');
                $('#hhend').val(parseInt(end3[0],10));
                $('#mmend').val(parseInt(end3[1],10));
                $('#ssend').val(parseInt(end3[2],10));
            }
        }
    }
    catch(e)
    {
        $('#monthend').val(sntp_label_jan);
        $('#weekend').val(sntp_label_week_0);
        $('#dayend').val(sntp_label_day_0);
        $('#hhend').val(0);
        $('#mmend').val(0);
        $('#ssend').val(0);
    }
}


//set SntpDstOffset
function setSntpDstOffset(){
    try
    {
        var offset;
        var indexstring = g_indexstring.split(':');
        var gmt_val = 0;
        var offset_val = 0;
        if(indexstring.length == 3)
        {
            gmt_val = Math.abs(parseInt(indexstring[0],10))*3600 + parseInt(indexstring[1],10)*60 + parseInt(indexstring[2],10);
        }else{
            gmt_val = Math.abs(parseInt(indexstring[0],10))*3600 + parseInt(indexstring[1],10)*60;
        }
        offset = g_sntp_DstOffset.split(':');
        offset_val = Math.abs(parseInt(offset[0],10))*3600 + parseInt(offset[1],10)*60 + parseInt(offset[2],10);
        if(g_sntp_DstOffset.indexOf('-') != -1 && g_indexstring.indexOf('-') == -1)
        {
            offset_val += gmt_val;
        }else{
            if(offset_val > gmt_val){
                offset_val -= gmt_val;
            }else{
                offset_val = gmt_val - offset_val;
            }
        }
        
        
        $('#offsethh').val(Math.floor(offset_val/3600));
        offset_val %=3600;
        $('#offsetmm').val(Math.floor(offset_val/60));
        offset_val %=60;
        $('#offsetss').val(offset_val);
    }
    catch(e)
    {
        $('#offsethh').val(0);
        $('#offsetmm').val(0);
        $('#offsetss').val(0);
       }
}


//server
function ServerInit(NTPServer,index)
{
    if(NTPServer.length)
      {
          var i;
          for( i = 0; i < g_ntpServers.length; i++ )
            {
                if(NTPServer == g_ntpServers[i] )
                {
                    $('#ntpServer'+index).val(g_ntpServers[i]);
                    break;
                }
            }
            if(i==g_ntpServers.length)
            {
                $('#ntpServer'+index).val(SNTP_SERVER_OTHER);
                $('#ntpServerOther'+index).val(NTPServer);
            }
       }
    else
    {
        $('#ntpServer'+index).val('');
    }
    sntpInit('ntpServer'+index);
}

//initpage
function initPage() {
    button_enable('apply_button', '0');

    $('#sys_switch_enable').attr('checked','false');
    $('#sntp_settings').css("display","none");
    
    $('#sys_switch').attr('checked','false');
    $('#daylight_switch').css("display","none");

    getAjaxData('config/sntp/config', function($xml) {
            var ret=xml2object($xml);
            if(ret.type=='config')
                {
                    var res = ret.config;
                    var i;
                    if(typeof(res.ntpServers) != 'undefined' && res.ntpServers.ntpServer){
                        for(i=0;i<res.ntpServers.ntpServer.length;i++)
                        {
                            g_ntpServers.push(res.ntpServers.ntpServer[i]);
                        }
                    }
                    g_ntpServerNum=res.ntpServerNum;
                    writeNtpServer();
                    $('#sntp_server_list').prepend(g_ntplist);
                }
        },{
        sync:true
        });
    getAjaxData('api/connection/wanstatus', function($xml) {
            var ret=xml2object($xml);
            if(ret.type=='response')
            {
               g_wanstatus = ret.response;
               if(typeof(ret.response.X_IPv6Address)!='undefined')
               {
                   GATEWAY_ExternalIPv6Address = ret.response.X_IPv6Address;
                   if(GATEWAY_ExternalIPv6Address.indexOf("/")!=-1)
                   {
                        GATEWAY_ExternalIPv6Address = GATEWAY_ExternalIPv6Address.substring(0, GATEWAY_ExternalIPv6Address.indexOf("/"));
                   }
               }
            }
        });

      //get lan ipv6
      getAjaxData("api/settings/hostinfo",function($xml) 
      {
          var ret = xml2object($xml);
          if(ret.type == "response" && typeof(ret.response.HostIPv6Address)!='undefined') 
          {
             g_LanIPv6Address = ret.response.HostIPv6Address;
          }
      }, {
         sync : true
      });
    
    getAjaxData('api/settings/sntp', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            var res=ret.response;
            if(typeof(res.CurrentLocalTime)!='undefined' && ''!=res.CurrentLocalTime)
            {
                writecurrtime(res.CurrentLocalTime);
            }else{
                $('#sntp_currenttime').append("");
            }
            if(typeof(res.status)!='undefined' && ''!=res.status)
            {
                writesynstatus(res.status);
            }else{
                $('#sntp_status').append(common_disable);
            }

            ServerInit(res.NTPServer1,1);
            ServerInit(res.NTPServer2,2);
            ServerInit(res.NTPServer3,3);
            ServerInit(res.NTPServer4,4);
            ServerInit(res.NTPServer5,5);

            var timezon_selectedIndex = settimezoneoffset(res.LocalTimeZoneName); 
            if(res.X_Label>=0&&res.X_Label<75){
                $('#cboTimeZone')[0].selectedIndex=res.X_Label;
                }
            else{
                $('#cboTimeZone')[0].selectedIndex = timezon_selectedIndex;
                }
            if(res.LocalTimeZoneName.indexOf('DST')>=0)
                {
                    $('#daylight_switch_enable').attr("checked",1);
                    $('#daylight_switch').css('display','');
                    Splittimezone(res.LocalTimeZoneName);
                    setSntpDststart();
                    setSntpDstend();
                    setSntpDstOffset();
                }
            else{
                    $('#daylight_switch_enable').removeAttr('checked');
                    $('#daylight_switch').css('display','none');
                }
                
            if(typeof(res.enable)!="undefined" && res.enable!="" && res.enable=='1')
                {
                    $('#sntp_settings').css("display","");
                }
            else
                {
                    $('#sys_switch_enable').removeAttr('checked');
                    $('#sntp_settings').css("display","none");
                }
        }
    },{
    sync:true
    });
    
    // get current settings gateway address
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type) {
            DEFAULT_GATEWAY_IP = ret.response.DhcpIPAddress;
        }
    }, {
        sync: true
    }
    );
}

//post data
function postData()
{
try{
    var syncenable=0;
    var localtimename = "";
    var ntpServerlist = [];
    var request = {};
    if("checked"==$('#sys_switch_enable').attr('checked'))
        {
          syncenable = 1;
          var i;
          for(i=1;i<6;i++)
          {
            if(SNTP_SERVER_OTHER==$('#ntpServer'+i).val())
              {
                   ntpServerlist.push($('#ntpServerOther'+i).val());
              }else{
                   ntpServerlist.push($('#ntpServer'+i).val());
              }
          }
          localtimename = genlocaltimename();

        request =
            {
                enable:syncenable,
                NTPServer1:resolveXMLEntityReference(ntpServerlist[0]),
                NTPServer2:resolveXMLEntityReference(ntpServerlist[1]),
                NTPServer3:resolveXMLEntityReference(ntpServerlist[2]),
                NTPServer4:resolveXMLEntityReference(ntpServerlist[3]),
                NTPServer5:resolveXMLEntityReference(ntpServerlist[4]),
                LocalTimeZoneName:localtimename,
                X_Label:"checked"==$('#sys_switch_enable').attr('checked')?g_label:-1
            };
        }else{
        request =
            {
                enable:syncenable
        };
    }
    
    var ret=object2xml('request',request);
    log.debug(ret);
    saveAjaxData('api/settings/sntp', ret, function($xml){
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret))
        {
            parent.showInfoDialog(sntp_dialog_ok);
            button_enable('apply_button', '0');
        }
        else
        {
           parent.showInfoDialog(common_failed);
           setTimeout(function(){
               window.location.reload(false);
           },3000);
        }
        });
        }
catch(e){
    }
}
function validateServerInput(getewayIp,address)
{     
    if (isIpFormat(address))
    {
        var matchString = getewayIp.substring(0, getewayIp.lastIndexOf("."));
        if(address.indexOf(matchString) != -1)
        {
            return false;
        }     
        if (isValidIpAddress(address) == false)
        {
            return false;
        }
    }
    else
    {
        if(isTValidName(address)==false)
        {
            return false;
        }
    }
    return true;
}
//verifyInput()
function isTValidName(name) 
{
    if(name.length=0||name.length>=128)
    {
        return false;
    }
    if(name.charAt(name.length-1)=='-' || name.charAt(0)=='-' )
    {
        return false;
    }
    return check_input_char_SNTP(name);
}

function check_input_char_SNTP(str)
{
    var i;
    var char_i;
    var num_char_i;
    if(str == "")
    {
        return true;
    }   
    for(i = 0; i < str.length; i++)
    {
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        if ((num_char_i > MACRO_SUPPORT_CHAR_MAX) || (num_char_i < MACRO_SUPPORT_CHAR_MIN))
        {
            return false;
        }
        else if((MACRO_NOT_SUPPORT_EXCALMATORY_MARK == num_char_i)
            ||(MACRO_NOT_SUPPORT_WELL_MARK == num_char_i)
            ||(MACRO_NOT_SUPPORT_DOLLAR_MARK == num_char_i)
            ||(MACRO_NOT_SUPPORT_PERCENT_MARK == num_char_i)
            ||(MACRO_NOT_SUPPORT_CHAR_64 == num_char_i)
            ||(MACRO_NOT_SUPPORT_CHAR_38 == num_char_i)
            ||(MACRO_NOT_SUPPORT_CHAR_94 == num_char_i)
            )
        {
            return false;
        }
        else
        {
            continue;
        }
    }
    return true;
}
function isLanWanipv6Address(address)
{   
      //check LAN ipv6 ,eg :fd00:1020:3040:5000::1/64,fe80::1/64
      if(g_LanIPv6Address!='')
      {
         var IPv6AddressParts = g_LanIPv6Address.split(',');
         var i = 0;
         for(i=0;i<IPv6AddressParts.length;i++)
         {
              var matchString = IPv6AddressParts[i].substring(0, IPv6AddressParts[i].lastIndexOf("/"));
              if(address==matchString)
              {
                  return true;
              }
         }
       } 
       //check WAN ipv6
       if(GATEWAY_ExternalIPv6Address==address)
       {
            return true;
       }

       return false;
}


function verifyInput()
{
    var isvalid=true;
    var total = 0;
    $('.sntp_server_input').each(function (){
        if(this.value!=''){
            isvalid =(isvalid && g_wanstatus.ExternalIPAddress != $.trim(this.value) 
                              && validateServerInput(DEFAULT_GATEWAY_IP,$.trim(this.value))
                              && !isLanWanipv6Address($.trim(this.value)));
            if(!isvalid){
                showErrorUnderTextbox(this.id, sntp_hint_input_err,'serveradderr');
                return isvalid;
            }
        }
    });
    var i;
    for(i = 1 ; i < 6 && $('#sys_switch_enable').attr('checked') == "checked"; i++ ){
        if((SNTP_SERVER_OTHER == $('#ntpServer'+i).val() && $.trim($('#ntpServerOther'+i).val()) == '') 
            || $('#ntpServer'+i).val() ==''){
            total++;
        }
    }
    if(total == 5){
        parent.showInfoDialog(sntp_dialog_ntpserver_error);
        return false;
    }

    return isvalid;
}

function check_daylight_saving_time(){
    if(parseInt($('#monthstart').val(),10) > parseInt($('#monthend').val(),10)){
        return false;
    }else if(parseInt($('#monthstart').val(),10) < parseInt($('#monthend').val(),10)){
        return true;
    }
    
    if(parseInt($('#weekstart').val(),10) > parseInt($('#weekend').val(),10)){
        return false;
    }else if(parseInt($('#weekstart').val(),10) < parseInt($('#weekend').val(),10)){
        return true;
    }
    
    if(parseInt($('#daystart').val(),10) > parseInt($('#dayend').val(),10)){
        return false;
    }else if(parseInt($('#daystart').val(),10) < parseInt($('#dayend').val(),10)){
        return true;
    }
    
    if(parseInt($('#hhstart').val(),10) > parseInt($('#hhend').val(),10)){
        return false;
    }else if(parseInt($('#hhstart').val(),10) < parseInt($('#hhend').val(),10)){
        return true;
    }
    
    if(parseInt($('#mmstart').val(),10) > parseInt($('#mmend').val(),10)){
        return false;
    }else if(parseInt($('#mmstart').val(),10) < parseInt($('#mmend').val(),10)){
        return true;
    }
    
    if(parseInt($('#ssstart').val(),10) > parseInt($('#ssend').val(),10)){
        return false;
    }else if(parseInt($('#ssstart').val(),10) < parseInt($('#ssend').val(),10)){
        return true;
    }
    
    return true;
}

//apply
function sntp_apply()
{
    if(!isButtonEnable('apply_button'))
      {
        return;
      }
    clearAllErrorLabel();
     if (verifyInput())
    {
        parent.showConfirmDialog(firewall_hint_submit_list_item, postData);
        button_enable('apply_button', '0');
    }
}

//load
$(document).ready(function(){
    initPage();
    $('#apply_button').bind('click',function(){
        sntp_apply();
        });

    $('select').bind('change',function(){
        button_enable('apply_button', '1');
        clearAllErrorLabel();
        });
    
    $('input').bind('change input paste cut keydown', function(){
            button_enable('apply_button', '1');
            clearAllErrorLabel();
        });
    $('.sntp_server_select').bind('change',function(){
        sntpChange(this.id);
        });
    $('#sys_switch_enable').click(function(){
        if($('#sys_switch_enable').attr('checked') != "checked")
        {
            $("#sntp_settings").css('display','none');
        }
        else
        {
            $("#sntp_settings").css('display','');
        }
    });
    
    $('#daylight_switch_enable').click(function(){
        if($('#daylight_switch_enable').attr('checked') != "checked")
        {
            $("#daylight_switch").css('display','none');
        }
        else
        {
            $("#daylight_switch").css('display','');
        }
    });
});
