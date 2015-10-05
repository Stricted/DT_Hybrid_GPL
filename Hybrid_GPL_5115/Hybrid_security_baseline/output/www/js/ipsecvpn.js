var vpnsList = [];
var g_add_edit='';
var g_auth_file = [0,0,0];
var check_VPNName = /^[a-zA-Z0-9]{5,15}$/;
var NUMSTR='0123456789';
var LOWABC='abcdefghijklmnopqrstuvwxyz';
var HIGHABC='ABCDEFGHIJKLMNOPQRSTUVWXYZ';
var check_ManualAuthenticationKey = /^[1-9][0-9]{5,29}$/;
var IPSECMODE_TUNNEL = 'tunnel';
var IPSECMODE_TRANSPORT = 'transport';
var IPSECVPN_MANUAL='manual';
var IPSECVPN_AUTO='auto';
var IPSECVPN_AH='ah';
var IPSECVPN_ESP='esp';
var ADDTITLE = -1;
var AUTHMODE_PRE_SHARED_KEY='1';
var AUTHMODE_CERTIFICATE='2';
var RADIO_CHECKED = 1;
var MAX_VPN_COUNT = 4;
var FILE_END_NAME = "bak";

var DEL_STATUS = 0;
var ADD_STATUS = 1;
var EDIT_STATUS = 2;

var manual_mode_md = "hmac-md5";
var manual_mode_sha = "hmac-sha1";
var manual_mode_3descbc = "3des-cbc";
var manual_mode_descbc = "des-cbc";
var manual_mode_aescbc = "aes-cbc";
var manual_mode_null = "null";
var atuo_mode_3des = "3des";
var atuo_mode_des = "des";
var atuo_mode_aes = "aes";
var atuo_mode_all = "all";
var auto_mode_md5 = "md5";
var auto_mode_sha1 = "sha1";
var auto_mode_phdhgroup1 = "768bit";
var auto_mode_phdhgroup2 = "1024bit";
var auto_mode_phdhgroup3 = "1536bit";
var auto_mode_phdhgroup4 = "2048bit";
var auto_mode_phdhgroup5 = "4096bit";

var ip_dialog_content = "<div id='show_ip_dialog'>"+
                            "<table id='vpn_dialog'>"+
                                "<tr><td class='vpn_text' >"+set_ipvpn_connection+common_colon+"</td><td><input type='checkbox' id='conn_enable' style='width:20px;'/>&nbsp;"+common_enable+"</td></tr>"+
                                "<tr><td class='vpn_text' >"+set_ipvpn_setname+common_colon+"</td><td class='vpn_input' ><input type='text'  id='show_set_name' maxlength='64' class='input_style'/>"+"</td></tr>"+
                                "<tr><td class='vpn_text' >"+set_ipvpn_remote_ip+common_colon+"</td><td class='vpn_input' ><input type='text' id='show_set_remote' class='input_style' maxlength='31'/>"+"</td></tr>"+                                
                                "<tr><td class='vpn_text' >"+set_ipvpn_key+common_colon+"</td>"+
                                      "<td width='50%'><input type='radio' name='show_set_key' checked='checked' id='set_key_manual' checked='checked' value='manual' style='width:20px;'/>&nbsp;"+common_manual+
                                           "<input type='radio' name='show_set_key' id='set_key_auto' value='auto' style='width:20px;'/>&nbsp;"+common_auto+"</td>"+
                                "</tr>"+
                                "<tr id='protocol'>"+
                                   "<td class='vpn_text' >"+firewall_label_protocol+common_colon+"</td>"+
                                   "<td class='vpn_input' >"+
                                    "<select id='show_set_protocol' class='input_select'>"+
                                       "<option value='ah' selected='selected'>"+set_ipvpn_protocol_mode_ah+"</option>"+
                                       "<option value='esp'>"+set_ipvpn_protocol_mode_esp+"</option>"+
                                    "</select>"+
                                   "</td>"+
                                "</tr>"+
                                "<tr class='manual_manualauther' style='display:none'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_manual_auther+common_colon+"</td>"+
                                   "<td class='vpn_input'>"+
                                      "<select id='show_set_auther' class='input_select'>"+
                                          "<option value='hmac-md5' >"+manual_mode_md+"</option>"+
                                          "<option value='hmac-sha1' >"+manual_mode_sha+"</option>"+
                                      "</select>"+
                                    "</td>"+
                                "</tr>"+
                                "<tr class='manual_manualauther' style='display:none'>"+
                                    "<td class='vpn_text' >"+set_ipvpn_manual_auther_key+common_colon+"</td>"+
                                    "<td class='vpn_input'><input type='password' id='show_ipvpn_manual_key' valign='top' class='input_style' maxlength='31'/></td>"+
                                "</tr>"+
                                "<tr class='manual_manualencry' style='display:none'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_manual_encry+common_colon+"</td>"+
                                   "<td class='vpn_input'>"+
                                      "<select id='show_set_encry' class='input_select'>"+
                                          "<option value='3des-cbc' >"+manual_mode_3descbc+"</option>"+
                                          "<option value='des-cbc' >"+manual_mode_descbc+"</option>"+
                                          "<option value='aes-cbc' >"+manual_mode_aescbc+"</option>"+
                                          "<option value='null' >"+manual_mode_null+"</option>"+
                                      "</select>"+
                                    "</td>"+
                                "</tr>"+
                                "<tr class='manual_manualencry' style='display:none'>"+
                                    "<td class='vpn_text' >"+set_ipvpn_manual_encry_key+common_colon+"</td>"+
                                    "<td class='vpn_input'><input type='password' id='show_ipvpn_manual_encry_key' valign='top' class='input_style' maxlength='31'/></td>"+
                                "</tr>"+
                                "<tr id ='ipsec_mode'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_ip_mode+common_colon+"</td>"+
                                   "<td class='vpn_input'>"+
                                      "<select id='show_set_ip_mode' class='input_select'>"+
                                        "<option value='tunnel' id='tunnel'>"+set_ipvpn_ip_mode_tunnel+"</option>"+
                                        "<option value='transport' id='transport' selected='selected'>"+set_ipvpn_ip_mode_transport+"</option>"+
                                      "</select>"+
                                   "</td>"+
                                 "</tr>"+
                                 "<tr class='ipsecMode' style='display:none'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_datasrc+common_colon+"</td>"+
                                   "<td class='vpn_input'><input type='text' id='datasrc' valign='top' class='input_style' maxlength='31'/></td>"+
                               "</tr>"+
                               "<tr class='ipsecMode' style='display:none'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_datadst+common_colon+"</td>"+
                                   "<td class='vpn_input'><input type='text' id='datadst' valign='top' class='input_style' maxlength='31'/></td></td>"+
                               "</tr>"+
                               "<tr class='ipsecMode' style='display:none'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_datasrcmask+common_colon+"</td>"+
                                   "<td class='vpn_input'><input type='text' id='datasrcmask' valign='top' class='input_style' maxlength='31'/></td> </td>"+
                               "</tr>"+
                               "<tr class='ipsecMode' style='display:none'>"+
                                   "<td class='vpn_text' >"+set_ipvpn_datadstmask+common_colon+"</td>"+
                                   "<td class='vpn_input'><input type='text' id='datadstmask' valign='top' class='input_style' maxlength='31'/></td> </td>"+
                               "</tr>"+
                                 " <tr>"+"<td class='vpn_text' >"+set_ipvpn_local_port+common_colon+"</td>"+
                                     "<td class='vpn_input' ><input type='text' id='show_ipvpn_local_port' valign='top' class='input_style' maxlength='31'/></td>"+
                                 "</tr>"+
                                 "<tr id='remote_port'>"+
                                     "<td class='vpn_text'>"+set_ipvpn_remote_port+common_colon+"</td>"+
                                     "<td class='vpn_input'><input type='text' id='show_ipvpn_remote_port' valign='top' class='input_style' maxlength='31'/></td>"+
                                 "</tr>"+
                                 "<tr  class='manual' style='display:none'>"+
                                    "<td class='vpn_text'>"+set_ipvpn_manual_spi+common_colon+"</td>"+
                                    "<td class='vpn_input'><input type='text' id='show_ipvpn_manual_spi' valign='top' class='input_style' maxlength='31'/></td>"+
                                "</tr>"+
                                "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text' >"+dialup_label_mode+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='mode' class='input_select'>"+
                                         "<option value='aggressive' >"+set_ipvpn_atuo_mode_aggr+"</option>"+
                                         "<option value='main' >"+set_ipvpn_atuo_mode_main+"</option>"+
                                         "<option value='both' >"+firewall_label_both+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text' >"+set_ipvpn_atuo_nattstate+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='nattstate' class='input_select'>"+
                                         "<option value='enable' >"+common_enable+"</option>"+
                                         "<option value='disable' >"+common_disable+"</option>"+
                                         "<option value='force' >"+set_ipvpn_atuo_nattstate_force+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                 "<td class='vpn_text' >"+set_ipvpn_atuo_dpdtime+common_colon+"</td>"+
                                 "<td class='vpn_input'><input type='text' id='dpdtime' valign='top' class='input_style' maxlength='31'/></td> "+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                 "<td class='vpn_text'>"+set_ipvpn_auto_identifierlocal+common_colon+"</td>"+
                                 "<td class='vpn_input'><input type='text' id='identifierlocal' valign='top' class='input_style' maxlength='31'/></td> "+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text'>"+set_ipvpn_atuo_ph1encalg+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='ph1encalg' class='input_select'>"+
                                         "<option value='3des' >"+atuo_mode_3des+"</option>"+
                                         "<option value='des' >"+atuo_mode_des+"</option>"+
                                         "<option value='aes' >"+atuo_mode_aes+"</option>"+
                                         "<option value='null' >"+atuo_mode_all+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text'>"+set_ipvpn_atuo_ph1authalg+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='ph1authalg' class='input_select'>"+
                                         "<option value='md5' >"+auto_mode_md5+"</option>"+
                                         "<option value='sha1' >"+auto_mode_sha1+"</option>"+
                                         "<option value='all' >"+atuo_mode_all+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text' >"+set_ipvpn_atuo_ph1dhgroup+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='ph1dhgroup' class='input_select'>"+
                                         "<option value='768bit' >"+auto_mode_phdhgroup1+"</option>"+
                                         "<option value='1024bit' >"+auto_mode_phdhgroup2+"</option>"+
                                         "<option value='1536bit' >"+auto_mode_phdhgroup3+"</option>"+
                                         "<option value='2048bit' >"+auto_mode_phdhgroup4+"</option>"+
                                         "<option value='4096bit' >"+auto_mode_phdhgroup5+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                 "<td class='vpn_text' >"+set_ipvpn_auto_ph1lifetime+common_colon+"</td>"+
                                 "<td class='vpn_input'><input type='text' id='ph1lifetime' valign='top' class='input_style' maxlength='31'/></td> "+
                             "</tr>"+
                             "<tr class='autohtml' id='tr_ph2encalg' style='display:none' style='display:none'>"+
                                     "<td class='vpn_text' >"+set_ipvpn_atuo_ph2encalg+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='ph2encalg' class='input_select'>"+
                                         "<option value='3des' >"+atuo_mode_3des+"</option>"+
                                         "<option value='des' >"+atuo_mode_des+"</option>"+
                                         "<option value='aes' >"+atuo_mode_aes+"</option>"+
                                         "<option value='all' >"+atuo_mode_all+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text' >"+set_ipvpn_atuo_ph2authalg+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='ph2authalg' class='input_select'>"+
                                         "<option value='hmac-md5' >"+manual_mode_md+"</option>"+
                                         "<option value='hmac-sha1' >"+manual_mode_sha+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                     "<td class='vpn_text' >"+set_ipvpn_atuo_ph2dhgroup+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='ph2dhgroup' class='input_select'>"+
                                         "<option value='768bit' >"+auto_mode_phdhgroup1+"</option>"+
                                         "<option value='1024bit' >"+auto_mode_phdhgroup2+"</option>"+
                                         "<option value='1536bit' >"+auto_mode_phdhgroup3+"</option>"+
                                         "<option value='2048bit' >"+auto_mode_phdhgroup4+"</option>"+
                                         "<option value='4096bit' >"+auto_mode_phdhgroup5+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' style='display:none'>"+
                                 "<td class='vpn_text' >"+set_ipvpn_auto_ph2lifetime+common_colon+"</td>"+
                                 "<td class='vpn_input'><input type='text' id='ph2lifetime' valign='top' class='input_style' maxlength='31'/></td>"+
                             "</tr>"+
                             "<tr class='autohtml' id='tr_authmode' style='display:none'>"+
                                     "<td class='vpn_text' >"+set_quick_authentication_mode+common_colon+"</td>"+
                                     "<td class='vpn_input'>"+
                                     "<select id='authmode' class='input_select'>"+
                                         "<option value='1' >"+set_ipvpn_auto_presharedkey+"</option>"+
                                         "<option value='2' >"+set_ipvpn_auto_certificate+"</option>"+
                                     "</select>"+
                                 "</td>"+
                             "</tr>"+
                             "<tr class='autohtml' id='tr_presharedkey' style='display:none'>"+
                                 "<td class='vpn_text' >"+set_ipvpn_auto_presharedkey+common_colon+"</td>"+
                                 "<td class='vpn_input'><input type='password' id='presharedkey' valign='top' class='input_style' maxlength='31'/></td>"+
                             "</tr>"+
                             "</table>"+
                             "<div id='show_certificate' style='display:none'>"+
                                 "<iframe name='iframe_upload_file' style='display:none;'></iframe>"+
                                "<table class='certificatehtml'><form name='vpn_file' id='upload_file_form' method='post' action='/api/certificate/fileupload' enctype='multipart/form-data' encoding='multipart/form-data' target='iframe_upload_file'>"+
                                "<tr><td class='vpn_text_form'>"+set_ipvpn_auto_publicname+common_colon+"</td>"+"<td class='vpn_input_form'><input type='file' name='uploadfile' id='fileField0' size='20' class='upload_file' contentEditable=false onfocus='setInputValue();'  value=''/></td></tr>"+
                                "<tr><td class='vpn_text_form'>"+set_ipvpn_atuo_privatename+common_colon+"</td>"+"<td class='vpn_input_form'><input type='file' name='uploadfile' id='fileField1' size='20' class='upload_file' contentEditable=false onfocus='setInputValue();'  value=''/></td></tr>"+
                                "<tr><td class='vpn_text_form'>"+set_ipvpn_auto_rpublicname+common_colon+"</td>"+"<td class='vpn_input_form'><input type='file' name='uploadfile' id='fileField2' size='20' class='upload_file' contentEditable=false onfocus='setInputValue();'  value=''/></td></tr>"+
                                '</form></table>'+
                             "</div>"+
                        "</div>";

function setInputValue()
{
    var isOpera = (navigator.userAgent.indexOf('Opera') >= 0);
    if(isOpera)
    {
        document.getElementById("sd_file_input").focus();
        document.getElementById("sd_file_input").value=document.getElementById("uploadfile").value;
    }
}

/*-------------------ip_dialog_content variety function--------------------------*/
function set_ipsecvpn_key(){
    var manualclick=1;
    var autoclick=1;
    $('#set_key_manual').click(function(){
        if(manualclick){
            $('.autohtml').hide();
            
            $('#show_certificate').hide();
            $('#vpn_Upload').hide();
            $(".manual").show();
            $(".manual_manualauther").show();
            if($("#show_set_protocol").val() == IPSECVPN_AH){
                $(".manual_manualencry").hide();
            }
            else {
                $(".manual_manualencry").show();
            }
            if($("#show_set_ip_mode").val() == IPSECMODE_TUNNEL) {
                $('.ipsecMode').show();
            }
            else {
                $('.ipsecMode').hide();
            }
            manualclick--;
            if(!autoclick)
            {
                autoclick++;
            }
        }
    });
    $('#set_key_auto').click(function(){
        if(autoclick){
            $(".manual_manualauther").hide();
            $(".manual_manualencry").hide();
            $('.manual').hide();
            
            $(".autohtml").show();
            authModeChange();
            if($("#show_set_protocol").val() != IPSECVPN_AH) {
                $("#tr_ph2encalg").show();
            }
            else {
                $("#tr_ph2encalg").hide();
            }
            
            if($("#show_set_ip_mode").val() == IPSECMODE_TUNNEL) {
                $('.ipsecMode').show();
            }
            else {
                $('.ipsecMode').hide();
            }
            
            if($("#authmode").val() == "1") {
                $("#tr_presharedkey").show();
                $('#show_certificate').hide();
                $('#vpn_Upload').hide();
            }
            else {
                $("#tr_presharedkey").hide();
                $('#show_certificate').show();
                $('#vpn_Upload').show();
            }
            if(!manualclick)
            {
                manualclick++;
            }
            autoclick--;
        }
    });
}

function protocolChange(setchoice,_value)
{
    if(setchoice == IPSECVPN_AUTO)
    {
        if(_value == IPSECVPN_AH)
        {
            $('#tr_ph2encalg').css('display','none');
        }else{
            $('#tr_ph2encalg').css('display','');
        }
    }else{
        if(_value == IPSECVPN_AH)
        {
           $('.manual_manualauther').show();
           $(".manual_manualencry").hide();
        }else{
            $('.manual_manualauther').show();
            $(".manual_manualencry").show();
        }
    }
}

function ipsecModeChange(_value)
{
    if(_value == IPSECMODE_TUNNEL)
    {
        $('.ipsecMode').show();
    }else{
        $('.ipsecMode').hide();
    }
}

function authModeChange()
{
    $('#authmode').bind('change',function(){
        if(this.value == '1' )
        {
            $('#tr_presharedkey').css('display','');
            $('#vpn_Upload').css('display','none');
            $('#show_certificate').hide();
        }else{
            $('#tr_presharedkey').css('display','none');
            $('#vpn_Upload').css('display','');
            button_enable('vpn_Upload', 0);
            $("#show_certificate").show();
        }
    });
}

/*-------------------first enter this page--------------------------*/
function getIPsecVPN(){
    $("#vpn_show input").parent().parent().remove();
    if($("#vpn_show :radio[name='isCheck']:checked").size() == RADIO_CHECKED){
        button_enable("ipvpn_del_btn",1);
        button_enable("ipvpn_edit_btn",1);
    }else{
        button_enable("ipvpn_del_btn",0);
        button_enable("ipvpn_edit_btn",0);
    }
    getAjaxData("api/settings/vpn",function($xml){
        var ret = xml2object($xml);
        var newline ="";
        if(ret.type == "response"){
            if(typeof(ret.response.X_VPN_IPsecs) != 'undefined' && ret.response.X_VPN_IPsecs.X_VPN_IPsec ){
                vpnsList = ret.response.X_VPN_IPsecs.X_VPN_IPsec;
                if(vpnsList.length != 0){
                    if(vpnsList.length >= MAX_VPN_COUNT){
                        button_enable("ipvpn_add_btn",0);
                    }else{
                        button_enable("ipvpn_add_btn",1);
                    }
                    if($.isArray(vpnsList)){
                        var i = 0;
                        for(i;i<vpnsList.length;i++){
                            newline="<tr>"+
                                        "<td><input type='radio' id="+vpnsList[i].Index+" name='isCheck'/></td>"+
                                        "<td>"+vpnsList[i].vpnname+"</td>"+
                                        "<td>"+vpnsList[i].RemoteIPAddress+"</td>"+
                                        "<td>"+vpnsList[i].Protocol+"</td>"+
                                        "<td>"+vpnsList[i].Enable+"</td>"+
                                        "<td>"+vpnsList[i].KeyMode+"</td>"+
                                    "</tr>";
                            $("#vpn_show").append(newline);
                        }
                    }else{
                        newline="<tr>"+
                                    "<td><input type='checkbox' id='"+vpnsList.Index+"' name='isCheck'/></td>"+
                                    "<td>"+vpnsList.vpnname+"</td>"+
                                    "<td>"+vpnsList.RemoteIPAddress+"</td>"+
                                    "<td>"+vpnsList.Protocol+"</td>"+
                                    "<td>"+vpnsList.Enable+"</td>"+
                                    "<td>"+vpnsList.KeyMode+"</td>"+
                                "</tr>";
                        $("#vpn_show").append(newline);
                    }
                }else{
                    $("#ipvpn_del_btn").hide();
                    $("#ipvpn_edit_btn").hide();
                }
                
            }else{
                log.error("the vpn.xml is not X_VPN_IPsec'data or null!");
            }
            
            
        }else{
            log.error("the vpn.xml is not response'type or null!");
        }
    }); 
}

function checkSameName()
{
    if(g_add_edit==ADDTITLE)
    {
        var i=0;
        for(;i<vpnsList.length;i++)
        {
            if($("#show_set_name").val()==vpnsList[i].vpnname)
            {
                return false;
            }
        }
    }else{
        var j;
        for(j = 0;j < vpnsList.length;j++)
        {
            if(g_add_edit!=j&&$("#show_set_name").val()==vpnsList[j].vpnname)
            {
                return false;
            }
        }
    }
    return true;
}

function checkSameIP()
{
    if(g_add_edit==ADDTITLE)
    {
        var i=0;
        for(;i<vpnsList.length;i++)
        {
            if($("#show_set_remote").val()==vpnsList[i].RemoteIPAddress)
            {
                return false;
            }
        }
    }else{
        var j;
        for(j = 0;j < vpnsList.length;j++)
        {
            if(g_add_edit!=j&&$("#show_set_remote").val()==vpnsList[j].RemoteIPAddress)
            {
                return false;
            }
        }
    }
    return true;
}

function checkSamestr(str)
{
    var s=str.charAt(0);
    var i=1;
    var temp='';
    for(;i<str.length;i++)
    {
        temp=str.charAt(i);
        if(temp!=s)
        {
            return false;
        }
    }
    return true;
}

function checkSafestr(str,compstr)
{
    var temp;
    var i;
    for(i=0;i<str.length;i++)
    {
        temp=str.charCodeAt(i);
        if(temp<32||temp>127)
        {
            return true;
        }else{
            temp=str.charAt(i);
            if(compstr.indexOf(temp)==-1)
            {
               return true;
            }
        }
    }
    return false;
}

function checkSameNet(ip1,mask1,ip2,mask2)
{
    var ippart1 = ip1.split('.');
    var ippart2 = ip2.split('.');
    var maskpart1 = mask1.split('.');
    var maskpart2 = mask2.split('.');
    var i;
    for(i=0;i<4;i++)
    {
        var temp1 = parseInt(ippart1[i],10) & parseInt(maskpart1[i],10);
        var temp2 = parseInt(ippart2[i],10) & parseInt(maskpart2[i],10);
        if(temp1!=temp2)
        {
            return false;
        }
    }
    return true;
}

function erroralert(idofvalue,alerttext){
    var addhtml = "<tr class='alert_error'><td> </td>"+
                  '<td>'+alerttext+'</td></tr>';
    var get_tr = $('#'+idofvalue).parent().parent();
    get_tr.after(addhtml);
    
}

function clearErroralert() {
    $('.alert_error').remove();
}
/*------------------------input listener-------------------------------------*/
function validateInput(){
    clearErroralert();
    if($("#show_set_name").val() == "" || !$("#show_set_name").val().match(check_VPNName)|| $("#show_set_name").val().length>15||!checkSameName()){
        erroralert('show_set_name', set_ipvpn_error_info_name);
        $("#show_set_name").focus();
        return false;
    }
    
    if($("#show_set_remote").val() == "" || !isValidIpAddress($("#show_set_remote").val())||!checkSameIP()){
        erroralert('show_set_remote', set_ipvpn_error_info_idaddress);
        $("#show_set_remote").focus();
        return false;
    }

    if($('#set_key_manual').attr('checked'))
    {
        if(manual_mode_md==$('#show_set_auther').val()&&$('#show_ipvpn_manual_key').val().length!=16)
        {
            erroralert('show_ipvpn_manual_key', set_ipvpn_error_info_mannual_key);
            return false;
        }

        if(manual_mode_sha==$('#show_set_auther').val()&&$('#show_ipvpn_manual_key').val().length!=20)
        {
            erroralert('show_ipvpn_manual_key', set_ipvpn_error_info_mannual_key);
            return false;
        }
        if(IPSECVPN_ESP==$('#show_set_protocol').val().toLowerCase())
        {
            if(manual_mode_3descbc==$('#show_set_encry').val()&&$('#show_ipvpn_manual_encry_key').val().length!=24)
            {
                erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                return false;
            }
            if(manual_mode_3descbc==$('#show_set_encry').val()&&$('#show_ipvpn_manual_encry_key').val().length==24)
            {
                var str1=$('#show_ipvpn_manual_encry_key').val().substr(0,8);
                var str2=$('#show_ipvpn_manual_encry_key').val().substr(8,8);
                var str3=$('#show_ipvpn_manual_encry_key').val().substr(16,8);
                if((str1==str2)||(str2==str3)||(str3==str1))
                {
                    erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                    return false;
                }
                if(checkSamestr(str1)||checkSamestr(str3)||checkSamestr(str3))
                {
                    erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                    return false;
                }
                if(!checkSafestr(str1, NUMSTR)||!checkSafestr(str1, LOWABC)||!checkSafestr(str1, HIGHABC))
                {
                    erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                    return false;
                }
                if(!checkSafestr(str2, NUMSTR)||!checkSafestr(str2, LOWABC)||!checkSafestr(str2, HIGHABC))
                {
                    erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                    return false;
                }
                if(!checkSafestr(str3, NUMSTR)||!checkSafestr(str3, LOWABC)||!checkSafestr(str3, HIGHABC))
                {
                    erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                    return false;
                }
               }
        
            if(manual_mode_descbc==$('#show_set_encry').val()&&$('#show_ipvpn_manual_encry_key').val().length!=8)
            {
                erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                return false;
            }

            if(manual_mode_aescbc==$('#show_set_encry').val()&&$('#show_ipvpn_manual_encry_key').val().length!=16&&$('#show_ipvpn_manual_encry_key').val().length!=24&&$('#show_ipvpn_manual_encry_key').val().length!=32)
            {
                erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                return false;
            }
            if(manual_mode_null==$('#show_set_encry').val()&&$('#show_ipvpn_manual_encry_key').val()!='')
            {
                erroralert('show_ipvpn_manual_encry_key', set_ipvpn_error_info_mannual_key);
                return false;
            }
        }
        if($('#show_ipvpn_manual_spi').val()==''||$('#show_ipvpn_manual_spi').val().length>8
            || checkSafestr($('#show_ipvpn_manual_spi').val().toLowerCase(), '0123456789abcdef'))
        {
            erroralert('show_ipvpn_manual_spi', set_ipvpn_error_info_mannual_key);
            return false;
        }
        var temp = parseInt($('#show_ipvpn_manual_spi').val(), 16);
        if ((temp < 0x101) || (temp > 0xfffffffe))
        {
            erroralert('show_ipvpn_manual_spi', set_ipvpn_error_info_mannual_key);
            return false;
        }
    }

    if($('#show_set_ip_mode').val()==IPSECMODE_TUNNEL)
    {
        if(!isValidIpAddress($('#datasrc').val()))
        {
            erroralert('datasrc', set_ipvpn_error_info_idaddress);
            return false;
        }
        if(!isValidIpAddress($('#datadst').val()))
        {
            erroralert('datadst', set_ipvpn_error_info_idaddress);
            return false;
        }
        if(!isValidSubnetMask($('#datasrcmask').val()))
        {
            erroralert('datasrcmask', set_ipvpn_error_info_idaddress);
            return false;
        }
        if(!isValidSubnetMask($('#datadstmask').val()))
        {
            erroralert('datadstmask', set_ipvpn_error_info_idaddress);
            return false;
        }
        if(checkSameNet($('#datasrc').val(), $('#datasrcmask').val(), $('#datadst').val(), $('#datadstmask').val()))
        {
            erroralert('datasrc', set_ipvpn_error_info_port);
            return false;
        }
        if($('#datasrc').val()==$('#datadst').val())
        {
            erroralert('datasrc', set_ipvpn_error_info_port);
            return false;
        }
        //wan ip confilct with this is not fixed
    }
    
    if(!isVaildPortForIPFilter($("#show_ipvpn_local_port").val())){
        erroralert('show_ipvpn_local_port', set_ipvpn_error_info_port);
        $("#show_ipvpn_local_port").focus();
        return false;
    }
    if(!isVaildPortForIPFilter($("#show_ipvpn_remote_port").val())){
        erroralert('show_ipvpn_remote_port', set_ipvpn_error_info_port);
        $("#show_ipvpn_remote_port").focus();
        return false;
    }
    
    if($('#set_key_auto').attr('checked'))
    {
        if($('#dpdtime').val()!=''&&(isNaN($('#dpdtime').val())||parseInt($('#dpdtime').val(),10)<0))
        {
            erroralert('dpdtime', set_ipvpn_error_info_mannual_key);
            return false;
        }

        if($('#ph1lifetime').val()!=''&&(isNaN($('#ph1lifetime').val())||parseInt($('#ph1lifetime').val(),10)<0))
        {
            erroralert('ph1lifetime', set_ipvpn_error_info_mannual_key);
            return false;
        }

        if($('#ph2lifetime').val()!=''&&(isNaN($('#ph2lifetime').val())||parseInt($('#ph2lifetime').val(),10)<0))
        {
            erroralert('ph1lifetime', set_ipvpn_error_info_mannual_key);
            return false;
        }
        if($('#authmode').val()==AUTHMODE_PRE_SHARED_KEY)
        {
            if($('#presharedkey').val()=='')
            {
                erroralert('presharedkey', set_ipvpn_error_info_mannual_key);
                return false;
            }
        }
    }

    return true;
}



/*----------------------------apply data when click add --------------------------*/
function saveDataFromApply(){
    if(validateInput()){
        
        var vpn_index = "";
        var vpn_modify = "";
        if($(".dialog_header_left").text() == set_ipvpn_add_ipsec) {
            vpn_index = vpnsList.length + 1;
            vpn_modify = ADD_STATUS;
        }
        else if($(".dialog_header_left").text() == set_ipvpn_edit_ipsec) {
            vpn_index = $("#vpn_show :radio[name='isCheck']:checked").attr("id");
            vpn_modify = EDIT_STATUS;
        }
        
        //VPNConnection Enabled
        var vpnconn = 0;
        if($("#conn_enable").attr("checked")){
            vpnconn = 1;
        }else{
            vpnconn = 0;
        }
        //VPN name
        var vpnName = "";
        vpnName = $("#show_set_name").val();
        //Remote ip address
        var vpnIPAdd = "";
        vpnIPAdd = $("#show_set_remote").val();
        //key mode
        var vpnKeyMode = "";
        if($("#set_key_manual").attr("checked")){
            vpnKeyMode = IPSECVPN_MANUAL;
        }else{
            vpnKeyMode = IPSECVPN_AUTO;
        }

        //LocalPort
        var vpnLocal = "";
        vpnLocal = $("#show_ipvpn_local_port").val();
    
        //RemotePort
        var vpnRemotePort = "";
        vpnRemotePort = $("#show_ipvpn_remote_port").val();

        //IPsecMode
        //DataSource
        //DataDestination
        //SubnetMaskOfDataSource
        //SubnetMaskOfDataDestination
        var vpnIPMode = "";
        var vpnDatasrc = "";
        var vpnDatadst = "";
        var vpnDatasrcmask = "";
        var vpnDatadstmask = "";
        vpnIPMode = $("#show_set_ip_mode").val();
        if(vpnIPMode == IPSECMODE_TUNNEL){
            vpnDatasrc = $("#datasrc").val();
            vpnDatadst = $("#datadst").val();
            vpnDatasrcmask = $("#datasrcmask").val();
            vpnDatadstmask = $("#datadstmask").val();
            
        }
        
        //Protocol
        var vpnPro = $("#show_set_protocol").val();

        //ManualAuthenticationAlgorithm
        var vpnMAA = "";
        //ManualAuthenticationKey
        var vpnMAK = "";
        //ManualEncryptionAlgorithm
        //ManualEncryptionKey
        var vpnMEA = "";
        var vpnMEK = "";
        //ManualSPI
        var vpnManualSPI = "";
        if(vpnKeyMode==IPSECVPN_MANUAL)
        {
            vpnMAA=$("#show_set_auther").val();
            vpnMAK= encrypt($("#show_ipvpn_manual_key").val());
            if(vpnPro==IPSECVPN_ESP)
            {
                vpnMEA=$("#show_set_encry").val();
                vpnMEK=encrypt($("#show_ipvpn_manual_encry_key").val());
            }
            vpnManualSPI = $("#show_ipvpn_manual_spi").val();
        }
        //WanInterface
        var wanInterface = "";
        //Mode
        var vpn_mode = "";
        //NATTState
        var vpn_NATTState ="";
        //DPDTime
        var vpn_DPDTime = "";
        //LocalIdentifier
        var vpn_LocalIdentifier = '';
        //Phase1EncryptionAlgorithm
        var vpn_PEA = "";
        //Phase1AuthenticationAlgorithm
        var vpn_PAA = "";
        //Phase1DHGroup
        var vpn_Phase1DHGroup = "";
        //Phase1LifeCycle
        var vpn_Phase1LifeCycle = "";
        //Phase2EncryptionAlgorithm
        var vpn_Phase2EncryptionAlgorithm = "";
        //Phase2AuthenticationAlgorithm
        var vpn_P2AA = "";
        //Phase2Group
        var vpn_P2G = "";
        //Phase2LifeCycle
        var vpn_p2life = "";
        //AuthenticationMode
        var vpn_AM = "";
        //PreSharedKey
        var vpn_PS = "";
        if(vpnKeyMode == IPSECVPN_AUTO)
        {
            vpn_mode = $("#mode").val();
            vpn_NATTState = $("#nattstate").val();
            vpn_DPDTime=$("#dpdtime").val();
            vpn_LocalIdentifier = $('#identifierlocal').val();
            vpn_PEA=$("#ph1encalg").val();
            vpn_PAA=$("#ph1authalg").val();
            vpn_Phase1DHGroup=$("#ph1dhgroup").val();
            vpn_Phase1LifeCycle=$("#ph1lifetime").val();
            if(vpnPro==IPSECVPN_ESP){
                vpn_Phase2EncryptionAlgorithm=$("#ph2encalg").val();
            }
            vpn_P2AA=$("#ph2authalg").val();
            vpn_P2G=$("#ph2dhgroup").val();
            vpn_p2life=$("#ph2lifetime").val();
            
            vpn_AM=$("#authmode").val();
            if(vpn_AM == AUTHMODE_PRE_SHARED_KEY)
            {
                vpn_PS = $("#presharedkey").val();
            }
        }

        //save data 
        var data_info = {
            Index         : vpn_index,
            Enable        : vpnconn,
            vpnname       : vpnName,
            RemoteIPAddress          : vpnIPAdd,
            KeyMode       : vpnKeyMode,
            Protocol      : vpnPro,
            ManualAuthAlg : vpnMAA,
            ManualAuthKey : vpnMAK,
            ManualEncAlg  : vpnMEA,
            ManualEncKey  : vpnMEK,
            IPsecMode     : vpnIPMode,
            LocalPort     : vpnLocal,
            RemotePort    : vpnRemotePort,
            ManualSpi     : vpnManualSPI,
            DataSrc       : vpnDatasrc,
            DataDst       : vpnDatadst,
            DataSrcMask   : vpnDatasrcmask,
            DataDstMask   : vpnDatadstmask,
            WanInterface  :wanInterface,
            Mode          : vpn_mode,
            NatTState     : vpn_NATTState,
            DpdTime       : vpn_DPDTime,
            LocalIdentifier  : vpn_LocalIdentifier,
            Ph1EncAlg     : vpn_PEA,
            Ph1AuthAlg    : vpn_PAA,
            Ph1DhGroup    : vpn_Phase1DHGroup,
            Ph1LifeTime   : vpn_Phase1LifeCycle,
            Ph2EncAlg     : vpn_Phase2EncryptionAlgorithm,
            Ph2AuthAlg    : vpn_P2AA,
            Ph2PfGroup    : vpn_P2G,
            Ph2LifeTime   : vpn_p2life,
            AuthMode      : vpn_AM,
            PreSharedKey  : vpn_PS
        };
        var host = {
            X_VPN_IPsec : data_info
            };
        var hosts = {
                Delete       : DEL_STATUS,
                Modify       : vpn_modify,
                X_VPN_IPsecs : host
            };
        
        var req = object2xml("request",hosts);
        saveAjaxData("api/settings/vpn",req,function($xml){
            var ret = xml2object($xml);
            if(isAjaxReturnOK(ret)){
                clearDialog();
                showInfoDialog(common_success);
                setTimeout(function(){
                    window.location.reload();
                },2000);
                
            }else{
                clearDialog();
                showInfoDialog(common_failed);
            }
            getIPsecVPN();
        });
    }else{
        return;
    }
}

var g_dot='.';
function vpn_uploadstatus_delay(){
    setTimeout(function(){
        if(g_dot.length==5)
        {
           g_dot = '.';
        }else{
           g_dot += '.';
        }
        $('#vpn_uploading').text(sd_label_uploading+g_dot);
        vpn_uploadstatus_delay();
    },300);
}

/*--------------------------function for set value when edit clicked--------------------------------*/
function initdialog(title,list)
{
    if(title == ADDTITLE)
    {
        $(".manual").show();
        $(".manual_manualauther").show();
    }
    else
    {
        $("#conn_enable").attr("checked",list.Enable);
        $("#show_set_name").val(list.vpnname);
        $("#show_set_remote").val(list.RemoteIPAddress);
        $("#show_ipvpn_local_port").val(list.LocalPort);
        $("#show_ipvpn_remote_port").val(list.RemotePort);
        $("#show_set_protocol").val(list.Protocol.toLowerCase());
        $("#show_set_ip_mode").val(list.IPsecMode.toLowerCase());
        $("#datasrc").val(list.DataSrc);
        $("#datadst").val(list.DataDst);
        $("#datasrcmask").val(list.DataSrcMask);
        $("#datadstmask").val(list.DataDstMask);
            
        $('#show_set_auther').val(list.ManualAuthAlg);
        $('#show_ipvpn_manual_key').val(list.ManualAuthKey == '' ? '' : decrypt(list.ManualAuthKey));
        $('#show_ipvpn_manual_spi').val(list.ManualSpi);
        $('#show_set_encry').val(list.ManualEncAlg);
        $('#show_ipvpn_manual_encry_key').val(list.ManualEncKey == '' ? '' : decrypt(list.ManualEncKey));
        $("#mode").val(list.Mode);
        $("#nattstate").val(list.NatTState);
        $("#dpdtime").val(list.DpdTime);
        $("#identifierlocal").val(list.LocalIdentifier);
        $("#ph1encalg").val(list.Ph1EncAlg);
        $("#ph1authalg").val(list.Ph1AuthAlg);
        $("#ph1dhgroup").val(list.Ph1DhGroup);
        $("#ph1lifetime").val(list.Ph1LifeTime);
        $("#ph2authalg").val(list.Ph2AuthAlg);
        $("#ph2encalg").val(list.Ph2EncAlg);
        $("#ph2dhgroup").val(list.Ph2DhGroup);
        $("#ph2lifetime").val(list.Ph2LifeTime);
        $("#authmode").val(list.AuthMode);
        $('#presharedkey').val(list.PreSharedKey);
        if(list.IPsecMode.toLowerCase()==IPSECMODE_TUNNEL)
        {
            $(".ipsecMode").show();
        }
        if(list.KeyMode.toLowerCase() == IPSECVPN_MANUAL){
            $(".manual_manualauther").show();
            $(".manual").show();
            $(".autohtml").hide();
            $('#show_certificate').hide();
            if(list.Protocol.toLowerCase()==IPSECVPN_ESP)
            {
                $(".manual_manualencry").show();
                $("#set_key_manual").attr("checked",true);
            }    
        }else{
            $(".manual").hide();
            $(".manual_manualauther").hide();
            $(".manual_manualencry").hide();
            $("#set_key_auto").attr("checked",true);
            $(".autohtml").show();
            if(list.Protocol.toLowerCase()==IPSECVPN_ESP){
                $("#tr_ph2encalg").show();
            }
            
            authModeChange();
            if(list.AuthMode==AUTHMODE_PRE_SHARED_KEY)
            {
                $('#tr_presharedkey').show();
                $('#vpn_Upload').hide();
                $('#show_certificate').hide();
            }else{
                $('#tr_presharedkey').hide();
                $('#vpn_Upload').show(); 
                $("#show_certificate").show();
            }
        }
    }
    $('#show_set_ip_mode').bind('change',function(){
        ipsecModeChange(this.value);
        });
    $('#show_set_protocol').bind('change',function(){
        var setchoice= $('#set_key_manual').attr('checked')>$('#set_key_auto').attr('checked')? IPSECVPN_MANUAL : IPSECVPN_AUTO;
        protocolChange(setchoice,this.value);
        });
    $('#vpn_Upload').click(function(){
        if(g_auth_file[0]&&g_auth_file[1]&&g_auth_file[2]){
            if(isButtonEnable('vpn_Upload'))
            {
               $('#upload_file_form').submit();
               button_enable('vpn_Upload', 0);
               button_enable('ipvpn_apply_btn', 0);
               $(".dialog_table_r").prepend(uploadstatus);
               setTimeout(clearDialog,3000);
               g_auth_file[0] = 0;
               g_auth_file[1] = 0;
               g_auth_file[2] = 0;
               vpn_uploadstatus_delay();
            }
        }else{
            var i = (g_auth_file[0] == 0 ? "0" : ( g_auth_file[1] == 0 ? "1" : "2" ));
            showErrorUnderTextbox('fileField'+i, system_hint_file_name_empty);
        }
        });
    set_ipsecvpn_key();
    button_enable("ipvpn_apply_btn",1);
}

/*---------------------------click add and show dialog------------------------------------*/
function showAddDialog(){
    if(vpnsList.length < 4){
        call_vpn_dialog(set_ipvpn_add_ipsec,ip_dialog_content,common_apply,"ipvpn_apply_btn");
        g_add_edit=-1;
        initdialog(g_add_edit,'');
    }else{
        return;
    }
    
}
/*************Function for vpn append dialog**************/

var uploadstatus = "&nbsp;&nbsp;&nbsp;&nbsp;<span id='vpn_uploading' style='text-align:center;font-weight:bold;' >"+sd_label_uploading+"</span>";

function call_vpn_dialog(dialogTitle, content, button1_text, button1_id) {
    $('#div_wrapper').remove();
    $('.dialog').remove();

    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; width:100%; height:100%; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog' id='sms_dialog'>";
    dialogHtml += "    <div class='sms_dialog_top'></div>";
    dialogHtml += "    <div class='sms_dialog_content'>";
    dialogHtml += "        <div class='sms_dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + dialogTitle + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='close' class='dialog_close_btn'><img src='../res/dialog_close_btn.gif' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='vpn_dialog_table'>" + content + '</div>';
    dialogHtml += "        <div class='vpn_dialog_table_bottom'>";
    dialogHtml += "            <div class='dialog_table_r'>";

    dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper disable_btn' id='vpn_Upload' style='display:none'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + sd_label_upload + "</a></span>";
    dialogHtml += '                      </span></span></span>';
    dialogHtml += '              <span class="button_space"></span>';
    dialogHtml += "              <span class='button_wrapper " + button1_id + "' id='" + button1_id + "'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + button1_text + '</a></span>';
    dialogHtml += '                      </span></span></span>';

    dialogHtml += '            </div>';
    dialogHtml += '        </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='vpn_dialog_bottom'></div>";
    dialogHtml += '</div>';

    $('.body_bg').before(dialogHtml);

    reputPosition($('.dialog'), $('#div_wrapper'));

    g_main_displayingPromptStack.push(button1_id);

    $('a').attr('tabindex', '-1');
    $('select').attr('tabindex', '-1');
}

/*---------------------------click edit sha show dialog----------------------------------*/
function showEditDialog(){
    call_vpn_dialog(set_ipvpn_edit_ipsec,ip_dialog_content,common_apply,"ipvpn_apply_btn");
    button_enable("ipvpn_apply_btn",1);
    g_add_edit = $("#vpn_show :radio[name='isCheck']:checked").attr("id") - 1;
    if($.isArray(vpnsList)){
        initdialog(g_add_edit,vpnsList[g_add_edit]);
    }else{
        initdialog(g_add_edit,vpnsList);
    }

}

/*----------------------------show delete button enable---------------------------------------*/
function delInfo(){
    call_dialog(common_delete, firewall_hint_delete_list_item, common_ok, "pop_OK", common_cancel, "pop_Cancel");
    $("#pop_OK").click(function(){
        clearDialog();
        var delID = $("#vpn_show :radio[name='isCheck']:checked").attr("id");
        var hosts = {
            Delete : delID,
            Modify : DEL_STATUS
        };
        var req = object2xml("request",hosts);
        saveAjaxData("api/settings/vpn",req,function($xml){
            var ret = xml2object($xml);
            if(isAjaxReturnOK(ret)){
                showInfoDialog(common_success);
                getIPsecVPN();
                button_enable("ipvpn_del_btn",0);
                button_enable("ipvpn_edit_btn",0);
            }else{
                showInfoDialog(common_failed);
                $("#vpn_show :radio[name='isCheck']:checked").attr("checked",false);
                button_enable("ipvpn_del_btn",0);
                button_enable("ipvpn_edit_btn",0);
            }
        }); 
    }); 
}


/*---------------------ready-------------------------------*/
$(document).ready(function(){
    getIPsecVPN();  
    $("#ipvpn_add_btn").live("click",showAddDialog);
    $("#ipvpn_edit_btn").click(function(){
        if(isButtonEnable('ipvpn_edit_btn')){
            showEditDialog();    
        }
    });
    $("#ipvpn_del_btn").click(function(){
        if(isButtonEnable("ipvpn_del_btn")){
            delInfo();
        }
    });
    $("#ipvpn_apply_btn").live("click",saveDataFromApply);
    $("#vpn_show :radio").live("click",function(){
        if($("#vpn_show :radio[name='isCheck']:checked").size() == RADIO_CHECKED){
            button_enable("ipvpn_del_btn",1);
            button_enable("ipvpn_edit_btn",1);
        }else{
            button_enable("ipvpn_del_btn",0);
            button_enable("ipvpn_edit_btn",0);
        }
    });
    $("#fileField0").live("change",function(){
        clearAllErrorLabel();
        if( $('#fileField0').val().length == 0 || FILE_END_NAME != $('#fileField0').val().split(".")[$('#fileField0').val().split(".").length - 1])
        {
            showErrorUnderTextbox('fileField0', system_hint_file_name_empty);
        }else{
            g_auth_file[0] = 1;
            button_enable('vpn_Upload', 1);
        }
    });
    $("#fileField1").live("change",function(){
        clearAllErrorLabel();
        if( $('#fileField1').val().length == 0 || FILE_END_NAME != $('#fileField1').val().split(".")[$('#fileField1').val().split(".").length - 1])
        {
            showErrorUnderTextbox('fileField1', system_hint_file_name_empty);
        }else{
            g_auth_file[1] = 1;
            button_enable('vpn_Upload', 1);
        }
    });
    $("#fileField2").live("change",function(){
        clearAllErrorLabel();
        if( $('#fileField2').val().length == 0 || FILE_END_NAME != $('#fileField2').val().split(".")[$('#fileField2').val().split(".").length - 1])
        {
            showErrorUnderTextbox('fileField2', system_hint_file_name_empty);
        }else{
            g_auth_file[2] = 1;
            button_enable('vpn_Upload', 1);
        }
    });
    
    $('select,input:radio').live('change',clearErroralert);
});
