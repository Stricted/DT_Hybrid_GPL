// JavaScript Document
/****************************************************profilesmgr
 * (start)************************************/
var PROFILE_MAX_NUM = 150;
var PRO_AUTHMODE_AUTO = 'AUTO';
var PRO_AUTHMODE_PAP = 'PAP';
var PRO_AUTHMODE_CHAP = 'CHAP';
var PRO_AUTHMODE_NONE = 'NONE';
var G_DIALUPMODE_AUTO = 'AlwaysOn';
var G_DIALUPMODE_MANUAL = 'Manual';
var G_DIALUPMODE_ONDEMAND = 'OnDemand';
var PRO_DEFAULT_IP_ADDRESS = '0.0.0.0';
var PRO_DEFAULT_APN_NAME = '';
var MIN_IDLE_TIME = 30;
var MAX_IDLE_TIME =7200;
var MACRO_CONNECTION_CONNECTED = '901';
var MACRO_CONNECTION_IPV6CONNECTED = '2';
var MAX_WIRELESS_MTU = 1500;
var MIN_WIRELESS_IPV4_MTU = 1280;
var MIN_WIRELESS_IPV6_MTU = 1280;


var g_currentProfileAuthMode = '0';

var g_promag_info = null;
var g_promag_index = 0;
var g_promag_default_index = 0;
var g_promag_array_index = 0;
var g_promag_currentProfileName = '';
var g_promag_operation_btnID = null;
//Indicates one of  buttons ["new_profile", "edit_profile", "delete_profile"]
var g_clear_dialog = false;
var g_promag_autoAPN = false;
var g_dialupFeature = null;
//Content of popup window
var g_promag_dialogContent = "<table border='0' cellpadding='0' cellspacing='0' class='new_profile'>";
g_promag_dialogContent += "<tr height='1'><th width='200'></th><th width='370'></th></tr>";
g_promag_dialogContent += "<tr id='popup_name'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_profile_name + common_colon + '</td>';
g_promag_dialogContent += "<td><input type='text' value='' id='pro_name' maxlength='64'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_apn + common_colon + '</td>';
g_promag_dialogContent += "<td>&nbsp;</td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_apn'>";
g_promag_dialogContent += "<td height='32' class='radio'>";
g_promag_dialogContent += "<input id='radio_apn_dynamic' type='radio' value='0' name='auto_apn' style='width:20px' />";
g_promag_dialogContent += "<span>"+dialup_label_dynamic_numeric+"</span>";
g_promag_dialogContent += "<input id='radio_apn_static' type='radio' value='1' name='auto_apn' style='width:20px' />";
g_promag_dialogContent += "<span>"+dialup_label_static_numeric+"</span>"; 
g_promag_dialogContent += '</td>';
g_promag_dialogContent += "<td ><input type='text' value='' id='pro_apn' maxlength='32'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_ipaddr_label'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_ip_address + common_colon + '</td>';
g_promag_dialogContent += "<td>&nbsp;</td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_ip'>";
g_promag_dialogContent += "<td height='32' class='radio'>";
g_promag_dialogContent += "<input type='radio' value='0' name='auto_ipaddress' style='width:20px' />";
g_promag_dialogContent += "<span>"+dialup_label_dynamic_numeric+"</span>";
g_promag_dialogContent += "<input type='radio' value='1' name='auto_ipaddress' style='width:20px' />";
g_promag_dialogContent += "<span>"+dialup_label_static_numeric+"</span>"; 
g_promag_dialogContent += '</td>';
g_promag_dialogContent += "<td ><input type='text' value='' id='pro_ipaddress' maxlength='32'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_dialup_number'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_dialup_number + common_colon + '</td>';
g_promag_dialogContent += "<td><input type='text' value='' id='pro_number' maxlength='16'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_username'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_user_name + common_colon + '</td>';
g_promag_dialogContent += "<td><input type='text' value='' id='pro_username' maxlength='32'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_password'>";
g_promag_dialogContent += "<td height='32'>" + common_password + common_colon + '</td>';
g_promag_dialogContent += "<td><input type='password' value='' id='pro_password' maxlength='32'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_authen'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_authentication + common_colon + '</td>';
g_promag_dialogContent += "<td colspan='2' class='radio'>";
g_promag_dialogContent += "<select name='authentication_mode' id='pop_authmode'>";
g_promag_dialogContent += "<option value='" + PRO_AUTHMODE_AUTO+ "'>" + common_auto + '</option>';
g_promag_dialogContent += "<option value='" + PRO_AUTHMODE_PAP+ "'>" + dialup_label_pap + '</option>';
g_promag_dialogContent += "<option value='" + PRO_AUTHMODE_CHAP+ "'>" + dialup_label_chap + '</option>';
g_promag_dialogContent += "<option value='" + PRO_AUTHMODE_NONE+ "'>" + wlan_label_none + '</option>';
g_promag_dialogContent += '</select>';
g_promag_dialogContent += '</td>';
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_mtu' style='display:none'>";
g_promag_dialogContent += "<td height='32'>" + wan_mtu + common_colon + '</td>';
g_promag_dialogContent += "<td><input type='text' value='' id='pro_mtu' maxlength='4'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_connection'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_connection_mode + common_colon + '</td>';
g_promag_dialogContent += "<td>";
g_promag_dialogContent += "<select  id='pro_dialupmode'>";
g_promag_dialogContent += "<option value='" + G_DIALUPMODE_AUTO+ "'>" + common_auto + '</option>';
g_promag_dialogContent += "<option value='" + G_DIALUPMODE_MANUAL+ "'>" + common_manual + '</option>';
g_promag_dialogContent += "<option value='" + G_DIALUPMODE_ONDEMAND+ "'>" + label_dailup_option_on_demand + '</option>';
g_promag_dialogContent += '</select>';
g_promag_dialogContent += '</td>';
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_maxideltime' style='display:none'>";
g_promag_dialogContent += "<td height='32'>" + dialup_label_max_idle_time_s + common_colon + '</td>';
g_promag_dialogContent += "<td><input type='text' value='' id='pro_maxideltime' maxlength='8'></td>";
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += "<tr id='popup_iptype' style='display:none'>";
g_promag_dialogContent += "<td height='32'>" + connectiontatus_label_iptype + common_colon + '</td>';
g_promag_dialogContent += "<td><select  id='pro_iptype'>";
g_promag_dialogContent += "<option value='ipv4ipv6'>" + wan_ipv4_ipv6_text + '</option>';
g_promag_dialogContent += "<option value='ipv4'>" + wan_ipv4_text + '</option>';
g_promag_dialogContent += "<option value='ipv6'>" + wan_ipv6_text + '</option>';
g_promag_dialogContent += '</select></td>';
g_promag_dialogContent += '</tr>';
g_promag_dialogContent += '</table>';

function set_apn_IsStatic()
{
    if(parent.$("[name='auto_apn']:checked").val()==0)
    {
         parent.$('#pro_apn').val('');
         parent.$('#pro_apn').attr('disabled', true);
    }
    else if(parent.$("[name='auto_apn']:checked").val()==1)
    {
        parent.$('#pro_apn').removeAttr("disabled");
        parent.$('#pro_apn').focus();
    }
    else
    {
    }
}
function set_ipaddress_IsStatic()
{
    if(parent.$("[name='auto_ipaddress']:checked").val()==0)
    {
         parent.$('#pro_ipaddress').val('');
         parent.$('#pro_ipaddress').attr('disabled', true);
    }
    else if(parent.$("[name='auto_ipaddress']:checked").val()==1)
    {
        parent.$('#pro_ipaddress').removeAttr("disabled");
        parent.$('#pro_ipaddress').focus();
    }
    else
    {
    }
}

function promag_deleteProfile(event) {
    var delIndex = $('#profilelist').val();
    var minIndex = 0;

    //Reset default profile to a minimum valid one, only if the deleting profile
    // is defualt
    parent.showWaitingDialog(common_waiting,common_waiting);
    var request = {
        'Delete' : delIndex,
        'SetDefault' : minIndex,
        'Modify' : 0
    };

    var delProfile = object2xml('request', request);

    saveAjaxData('api/dialup/profiles', delProfile, function($xml) {
        parent.closeWaitingDialog();
        var delRet = xml2object($xml);
        if (isAjaxReturnOK(delRet)) {
            log.debug('Delete profile' + delIndex + 'successful!');
            parent.showInfoDialog(common_success);
            setTimeout(function() {
                top.window.location.reload();
                return false;
            }, g_feature.dialogdisapear);
        }
        else {
            log.error('Delete profile failed');
            parent.showInfoDialog(common_failed);
        }
    });
}

function promag_cancelDelete() {
    $('#div_wrapper').remove();
    $('.dialog').remove();
}

function promag_changePositionOfAPN() {
    var apn_tr_content = "<td height='32'>" + dialup_label_apn + common_colon + "</td><td class='display_info_rows'><pre></pre></td><td></td>";
    var apn_tr = "<tr id='apn_name'>" + apn_tr_content + '</tr>';
    //$("tr").remove("#apn_name");
    //$("#profile_username").before(apn_tr);
    $('#apn_name').html(apn_tr_content);
}

function promag_switchRadio(name) {
    $('input[name=radio_' + name + ']').live('click', function() {
        if ($(this).val() == '1') {
            $('input[id=pro_' + name + ']').removeAttr('disabled');
        }
        else {
            $('input[id=pro_' + name + ']').attr('disabled', 'disabled');
            if ('ipaddress' == name) {
                $('input[id=pro_' + name + ']').val(PRO_DEFAULT_IP_ADDRESS);
            }
            else {
                $('input[id=pro_' + name + ']').val(PRO_DEFAULT_APN_NAME);
            }
        }
    });
}

function promag_setTrDisplay() {
    g_dialupFeature.dialup_number_enabled ? $('#tr_dialup_number').show() : $('#tr_dialup_number').hide();
    g_dialupFeature.authentication_info_enabled ? $('#authentication_info').show() : $('#authentication_info').hide();
    g_dialupFeature.dialup_ipaddress_enabled ? $('#ip_addresss').show() : $('#ip_addresss').hide();
    g_dialupFeature.mtu_enabled ? $('#profile_mtu_tr').show() : $('#profile_mtu_tr').hide();
    g_dialupFeature.iptype ? $('#profile_iptype_tr').show() : $('#profile_iptype_tr').hide();
    if(g_dialupFeature.connection_mode){
        $('#profile_connectionmode_tr').show();
        $('#profile_maxideltime_tr').show();
    }else{
        $('#profile_connectionmode_tr').hide();
        $('#profile_connectionmode_tr').hide();
    }
}

function Profilesmgr_getCurrentProfileArrayIndex(profileArray, index) {
    var ArrayLength = profileArray.length;
    var i = 0;
    for (; i < profileArray.length; i++) {
        if (profileArray[i].Index == index) {
            return i;
        }
    }
    return 0;
}

function promag_trasfalPassword(oriPassword) {
    var profile_array_password = oriPassword.split('');
    var profile_show_password = '';
    $.each(profile_array_password, function(i) {
        profile_show_password += '*';
    });
    return profile_show_password;
}

function promag_setDefaultPor(set_default_request) {
    var default_pro_xml = object2xml('request', set_default_request);
    saveAjaxData('api/dialup/profiles', default_pro_xml, function($xml) {
        var delRet = xml2object($xml);
        if (isAjaxReturnOK(delRet)) {
            g_promag_default_index = set_default_request.SetDefault;
            top.window.location.reload();
        }
    });
}

function promag_ifDiffrentProfileName(currentProfileName) {
    var flag = false;
    if ($.isArray(g_promag_info)) {
        $.each(g_promag_info, function(i) {
            if (g_promag_info[i].Name == currentProfileName) {
                flag = false;
                return false;
            }
            else {
                flag = true;
            }
        });
    }
    else if (typeof (g_promag_info) != 'undefined') {
        //if there is no profile
        if (g_promag_info == null) {
            flag = true;
            return true;
        }

        //if there is only one profile
        if (g_promag_info.Name == currentProfileName) {
            flag = false;
            return false;
        }
        else {
            flag = true;
        }
    }
    else {
        flag = true;
    }
    return flag;
}

function promag_getProfileInfo() {
    promag_setTrDisplay();
    $("#profilelist").children().remove();
    getAjaxData('api/dialup/profiles', function($xml) {
        var profile_ret = xml2object($xml);
        if (profile_ret.type == 'response') {
            if (typeof(profile_ret.response.Profiles) !='undefined' && typeof(profile_ret.response.Profiles.Profile) !='undefine'){
                g_promag_info = profile_ret.response.Profiles.Profile;
                g_promag_index = parseInt(profile_ret.response.CurrentProfile, 10);

                if (1 > g_promag_index) {
                    g_promag_index = 1;
                }
                g_promag_default_index = g_promag_index;

                var profile_password_mark = '';
                if ($.isArray(g_promag_info)) {
                    $('#Profile_ContentStatus').show();
                    g_promag_array_index = Profilesmgr_getCurrentProfileArrayIndex(g_promag_info, g_promag_index);
                    //g_promag_index-1;
                    //$("#profile_index").val(g_promag_index);
                    $('#dprofile_name').html('<pre>' + resolveXMLEntityReference(g_promag_info[g_promag_array_index].Name) + '</pre>');
                    $('#ddialup_number').html(g_promag_info[g_promag_array_index].DailupNum);
                    $('#duser_name').html('<pre>' + resolveXMLEntityReference(g_promag_info[g_promag_array_index].Username) + '</pre>');
                    $('#profile_mtu').html('<pre>' + g_promag_info[g_promag_array_index].MTU + '</pre>');
                    if ('' != g_promag_info[g_promag_array_index].Password && ' ' != g_promag_info[g_promag_array_index].Password && null != g_promag_info[g_promag_array_index].Password) {
                        profile_password_mark = promag_trasfalPassword(g_promag_info[g_promag_array_index].Password == '' ? '' : decrypt(g_promag_info[g_promag_array_index].Password));
                        $('#dpassword').html(profile_password_mark);
                    }
                    else {
                        $('#dpassword').html('');
                    }

                    if (PRO_AUTHMODE_PAP == g_promag_info[g_promag_array_index].AuthMode) {
                        $('#dauthentication').html(dialup_label_pap);
                    }
                    else if (PRO_AUTHMODE_CHAP == g_promag_info[g_promag_array_index].AuthMode) {
                        $('#dauthentication').html(dialup_label_chap);
                    }
                    else if (PRO_AUTHMODE_NONE == g_promag_info[g_promag_array_index].AuthMode) {
                        $('#dauthentication').html(wlan_label_none);
                    }
                    else {
                        $('#dauthentication').html(common_auto);
                    }

                    if(G_DIALUPMODE_ONDEMAND == g_promag_info[g_promag_array_index].ConnectMode){
                        $('#profile_connectionmode').html('<pre>' + label_dailup_option_on_demand + '</pre>');
                        $('#profile_maxideltime_tr').show();
                        $('#profile_maxideltime').html('<pre>' + g_promag_info[g_promag_array_index].MaxIdelTime + '</pre>');
                    }else if(G_DIALUPMODE_AUTO == g_promag_info[g_promag_array_index].ConnectMode){
                        $('#profile_connectionmode').html('<pre>' + common_auto + '</pre>');
                        $('#profile_maxideltime_tr').hide();
                    }else if(G_DIALUPMODE_MANUAL == g_promag_info[g_promag_array_index].ConnectMode){
                        $('#profile_connectionmode').html('<pre>' + common_manual + '</pre>');
                        $('#profile_maxideltime_tr').hide();
                    }
                    //promag_checkIfStatic(g_promag_info[g_promag_array_index]);
                    if (g_promag_info[g_promag_array_index].ApnIsStatic == '0') {
                        $('#dapn').html('<pre>' + dialup_label_dynamic + '</pre>');
                    }
                    else {
                        $('#dapn').html('<pre>' + g_promag_info[g_promag_array_index].ApnName + '</pre>');
                    }
                    //check ipaddress is and not static 
                    if(g_promag_info[g_promag_array_index].IpIsStatic =='0'){
                        $('#dip_address').html('<pre>' + dialup_label_dynamic + '</pre>');
                    }else{
                        $('#dip_address').html('<pre>' + g_promag_info[g_promag_array_index].IpAddress + '</pre>');
                    }

                    if((1 == g_promag_info[g_promag_array_index].IPv4_Supported) && (0 == g_promag_info[g_promag_array_index].IPv6_Supported)){
                        $('#profile_iptype').html('<pre>' + wan_ipv4_text + '</pre>');
                    }else if((1 == g_promag_info[g_promag_array_index].IPv6_Supported) && (0 == g_promag_info[g_promag_array_index].IPv4_Supported)){
                        $('#profile_iptype').html('<pre>' + wan_ipv6_text + '</pre>');
                    }else if((1 == g_promag_info[g_promag_array_index].IPv4_Supported) && (1 == g_promag_info[g_promag_array_index].IPv6_Supported)){
                        $('#profile_iptype').html('<pre>' + wan_ipv4_ipv6_text + '</pre>');
                    }
                    var promag_names = [];
                    $.each(g_promag_info, function(n, value) {
                        var defaultName = g_promag_info[n].Name;
                        defaultName = resolveXMLEntityReference(defaultName);
                        while (defaultName.indexOf(' ') >= 0) {
                            defaultName = defaultName.replace(' ', '&nbsp;');
                        }

                        defaultName = defaultName + (g_promag_info[n].Index == g_promag_default_index ? common_default : '');
                        promag_names.push([g_promag_info[n].Index, defaultName]);

                        var varItem = '<option value = ' + g_promag_info[n].Index + '\>' + defaultName + '</option>';
                        $('#profilelist').append(varItem);

                    });
                    setTimeout(function() {
                        $('#profilelist').val(g_promag_index);
                    }, 1);
                    //$("#profilelist").addclass("selecter_select");

                    //$("#select_service").createOptions(promag_names);
                    //$("#select_service").sVal(g_promag_default_index);
                    if (g_promag_info[g_promag_array_index].ReadOnly == '0' && !g_promag_autoAPN) {
                        button_enable('edit_profile', '1');
                        button_enable('delete_profile', '1');
                    }
                    else {
                        button_enable('edit_profile', '0');
                        button_enable('delete_profile', '0');
                    }
                    // button_enable("delete_profile", "0");

                }
                else if (typeof g_promag_info != 'undefined') {
                    $('#Profile_ContentStatus').show();
                    // button_enable("delete_profile", "0");
                    $('#profile_index').val(g_promag_index);
                    $('#dprofile_name').html('<pre>' + resolveXMLEntityReference(g_promag_info.Name) + '</pre>');
                    $('#ddialup_number').html(g_promag_info.DailupNum);
                    $('#duser_name').html('<pre>' + resolveXMLEntityReference(g_promag_info.Username) + '</pre>');
                    $('#profile_mtu').html('<pre>' + g_promag_info.MTU + '</pre>');
                    if ('' != g_promag_info.Password && ' ' != g_promag_info.Password && null != g_promag_info.Password) {
                        profile_password_mark = promag_trasfalPassword(g_promag_info.Password == '' ? '' : decrypt(g_promag_info.Password));
                        $('#dpassword').html(profile_password_mark);
                    }
                    else {
                        $('#dpassword').html('');
                    }

                    if (PRO_AUTHMODE_PAP == g_promag_info.AuthMode) {
                        $('#dauthentication').html(dialup_label_pap);
                    }
                    else if (PRO_AUTHMODE_CHAP == g_promag_info.AuthMode) {
                        $('#dauthentication').html(dialup_label_chap);
                    }
                    else if (PRO_AUTHMODE_NONE == g_promag_info.AuthMode) {
                        $('#dauthentication').html(wlan_label_none);
                    }
                    else {
                        $('#dauthentication').html(common_auto);
                    }

                    if(G_DIALUPMODE_ONDEMAND == g_promag_info.ConnectMode){
                        $('#profile_connectionmode').html('<pre>' + label_dailup_option_on_demand + '</pre>');
                        $('#profile_maxideltime_tr').show();
                        $('#profile_maxideltime').html('<pre>' + g_promag_info.MaxIdelTime + '</pre>');
                    }else if(G_DIALUPMODE_AUTO == g_promag_info.ConnectMode){
                        $('#profile_connectionmode').html('<pre>' + common_auto + '</pre>');
                        $('#profile_maxideltime_tr').hide();
                    }else if(G_DIALUPMODE_MANUAL == g_promag_info.ConnectMode){
                        $('#profile_connectionmode').html('<pre>' + common_manual + '</pre>');
                        $('#profile_maxideltime_tr').hide();
                    }

                    //promag_checkIfStatic(g_promag_info);
                    if (g_promag_info.ApnIsStatic == '0') {
                        $('#dapn').html('<pre>' + dialup_label_dynamic + '</pre>');
                    }
                    else {
                        $('#dapn').html('<pre>' + g_promag_info.ApnName + '</pre>');
                    }
                    //check ip If static
                    if(g_promag_info.IpIsStatic =='0'){
                        $('#dip_address').html('<pre>' + dialup_label_dynamic + '</pre>');
                    }else{
                        $('#dip_address').html('<pre>' + g_promag_info.IpAddress + '</pre>');
                    }
                    if((1 == g_promag_info.IPv4_Supported) && (0 == g_promag_info.IPv6_Supported)){
                        $('#profile_iptype').html('<pre>' + wan_ipv4_text + '</pre>');
                    }else if((1 == g_promag_info.IPv6_Supported) && (0 == g_promag_info.IPv4_Supported)){
                        $('#profile_iptype').html('<pre>' + wan_ipv6_text + '</pre>');
                    }else if((1 == g_promag_info.IPv4_Supported) && (1 == g_promag_info.IPv6_Supported)){
                        $('#profile_iptype').html('<pre>' + wan_ipv4_ipv6_text + '</pre>');
                    }
                    
                    var defaultName = g_promag_info.Name;
                    defaultName = resolveXMLEntityReference(defaultName);
                    while (defaultName.indexOf(' ') >= 0) {
                        defaultName = defaultName.replace(' ', '&nbsp;');
                    }
                    defaultName = defaultName + (g_promag_info.Index == g_promag_default_index ? common_default : '');
                    var varItem = '<option value = ' + g_promag_info.Index + '\>' + defaultName + '</option>';
                    $('#profilelist').append(varItem);

                    setTimeout(function() {
                        $('#profilelist').val(g_promag_info.Index);
                    }, 1);
                    if (g_promag_info.ReadOnly == '0' && !g_promag_autoAPN) {
                        button_enable('edit_profile', '1');
                        button_enable('delete_profile', '1');
                    }
                    else {
                        button_enable('edit_profile', '0');
                        button_enable('delete_profile', '0');
                    }
                }
                else {
                    $('#profilelist').val('');
                    $('#profilelist').attr('disabled', 'disabled');
                    button_enable('edit_profile', '0');
                    button_enable('delete_profile', '0');
                }
            } else {
                return;
            }
        }
        else {
            log.error('Load profiles data failed');
            $('#profilelist').attr('disabled', 'disabled');
        }
        if (g_clear_dialog) {
            setTimeout(function() {
                parent.clearDialog();
            }, 1700);
            g_clear_dialog = false;
        }
    });
}

//Display radio button
function promag_checkIfStatic(ifIPStatic) {
    if (ifIPStatic.ApnIsStatic == '0') {
        $('#dapn').html('<pre>' + dialup_label_dynamic + '</pre>');
    }
    else {
        $('#dapn').html('<pre>' + ifIPStatic.ApnName + '</pre>');
    }
}

function promag_saveRequest(obj) {
    parent.clearDialog();
    parent.showWaitingDialog(common_waiting,common_waiting);
    var profile_xml = object2xml('request', obj);
    saveAjaxData('api/dialup/profiles', profile_xml, function($xml) {
        parent.closeWaitingDialog();
        var return_ret = xml2object($xml);
        if (isAjaxReturnOK(return_ret)) {
            parent.showInfoDialog(common_success);
            setTimeout(function() {
                top.window.location.reload();
                return false;
            }, g_feature.dialogdisapear);
        }
        else {
            log.error('Save data failed');
            parent.showInfoDialog(common_failed);
            setTimeout(function() {
                parent.clearDialog();
            },3000);
            g_clear_dialog = true;
        }
        promag_getProfileInfo();
    });
}

function promag_saveProfile() {

    //getModifyInfo
    var request = '';
    var profile_index = g_promag_index;
    var isvalid;
    var dnsstatus;
    var primary_dns;
    var seconde_dns;
    var ipaddress_status;
    var ipaddress;
    var mtu;
    var conn_mode;
    var maxidtime;
    var iptype_ipv4value = 0;
    var iptype_ipv6value = 0;
    if ($.isArray(g_promag_info)) {
        isvalid = g_promag_info[g_promag_array_index].IsValid;
        dnsstatus = g_promag_info[g_promag_array_index].DnsIsStatic;
        primary_dns = g_promag_info[g_promag_array_index].PrimaryDns;
        seconde_dns = g_promag_info[g_promag_array_index].SecondaryDns;
    }
    else if (typeof g_promag_info != 'undefined') {
        isvalid = g_promag_info.IsValid;
        dnsstatus = g_promag_info.DnsIsStatic;
        primary_dns = g_promag_info.PrimaryDns;
        seconde_dns = g_promag_info.SecondaryDns;
    }
    ipaddress = $.trim(parent.$('#pro_ipaddress').val());
    mtu = $.trim(parent.$('#pro_mtu').val());
    conn_mode = $.trim(parent.$('#pro_dialupmode').val());
    maxidtime = $.trim(parent.$('#pro_maxideltime').val());
    
    var profile_name = parent.$('#pro_name').val();
    var dialup_number = parent.$('#pro_number').val();
    var username = parent.$('#pro_username').val();
    var password = parent.$('#pro_password').val();
    var authen = PRO_AUTHMODE_AUTO;
    var apn_status = 0;
    var apn_name = $.trim(parent.$('#pro_apn').val());
    var iptype = parent.$('#pro_iptype').val();

    if ('' == apn_name || null == apn_name || '' == $.trim(apn_name)) {
        apn_name = PRO_DEFAULT_APN_NAME;
        apn_status = 0;
    }else {
        apn_status = 1;
    }
    
    if(ipaddress == '' || ipaddress == null ){
        ipaddress_status = 0;
        ipaddress = PRO_DEFAULT_APN_NAME;
    }else {
        ipaddress_status = 1;
    }

    if('ipv4' == iptype){
        iptype_ipv4value = 1;
        iptype_ipv6value = 0;
    }else if('ipv6' == iptype){
        iptype_ipv4value = 0;
        iptype_ipv6value = 1;
    }else if('ipv4ipv6' == iptype){
        iptype_ipv4value = 1;
        iptype_ipv6value = 1;
    }
    var validElement = {
        'profile_name' : profile_name,
        'username' : username,
        'password' : password,
        'apn_name' : apn_name,
        'ipaddress' : ipaddress,
        'MTU' : mtu,
        'ConnectMode' : conn_mode,
        'MaxIdelTime' : maxidtime,
        'iptype':iptype
    };

    var be_valid = promag_validInput(validElement);

    if (be_valid) {
        //Get the method which currently used
        var function_name = parent.$('.dialog_header_left').html();

        if (null == g_promag_operation_btnID) {
            return;
        }
        else if ('new_profile' == g_promag_operation_btnID) {
            if (!g_dialupFeature.authentication_info_enabled) {
                authen = PRO_AUTHMODE_AUTO;
            }
            else {
                authen = parent.$('#pop_authmode').val();
            }
            //New object for create new profile
            request = {
                'Delete' : 0,
                'SetDefault' : 0,
                'Modify' : 1,
                'Profile' : {
                    'Index' : '',  //original is new_index
                    'IsValid' : 1,
                    'Name' : resolveXMLEntityReference(profile_name),
                    'ApnIsStatic' : apn_status,
                    'ApnName' : apn_name,
                    'DailupNum' : dialup_number,
                    'Username' : resolveXMLEntityReference(username),
                    'Password' : encrypt(password),
                    'AuthMode' : authen,
                    'IpIsStatic' : ipaddress_status,
                    'IpAddress' : ipaddress,
                    'DnsIsStatic' : '',
                    'PrimaryDns' : '',
                    'SecondaryDns' : '',
                    'ReadOnly' : '0',
                    'MTU':mtu,
                    'ConnectMode' : conn_mode,
                    'MaxIdelTime' : (maxidtime == '') ? 0 : maxidtime,
                    'IPv4_Supported' : iptype_ipv4value,
                    'IPv6_Supported' : iptype_ipv6value
                }
            };
        }
        else if ('edit_profile' == g_promag_operation_btnID) {
            if (!g_dialupFeature.authentication_info_enabled) {
                authen = g_currentProfileAuthMode;
            }
            else {
                authen = parent.$('#pop_authmode').val();
            }

            //Create an object for modify profile
            request = {
                'Delete' : 0,
                'SetDefault' : 0,
                'Modify' : 2,
                'Profile' : {
                    'Index' : profile_index,
                    'IsValid' : isvalid,
                    'Name' : resolveXMLEntityReference(profile_name),
                    'ApnIsStatic' : apn_status,
                    'ApnName' : apn_name,
                    'DailupNum' : dialup_number,
                    'Username' : resolveXMLEntityReference(username),
                    'Password' : encrypt(password),
                    'AuthMode' : authen,
                    'IpIsStatic' : ipaddress_status,
                    'IpAddress' : ipaddress,
                    'DnsIsStatic' : dnsstatus,
                    'PrimaryDns' : primary_dns,
                    'SecondaryDns' : seconde_dns,
                    'ReadOnly' : '0',
                    'MTU':mtu,
                    'ConnectMode' : conn_mode,
                    'MaxIdelTime' : (maxidtime == '') ? 0 : maxidtime,
                    'IPv4_Supported' : iptype_ipv4value,
                    'IPv6_Supported' : iptype_ipv6value
                }
            };
        }
        promag_saveRequest(request);
        return true;
    }
    else {
        log.error('Some of input box invalid');
        return false;
    }
}

function cancel_profile() {
    parent.clearDialog();
}


function promag_setPopupTrDisplay() {
    g_dialupFeature.dialup_number_enabled ? parent.$('#popup_dialup_number').show() : parent.$('#popup_dialup_number').hide();
    g_dialupFeature.authentication_info_enabled ? parent.$('#popup_authen').show() : parent.$('#popup_authen').hide();
    g_dialupFeature.dialup_ipaddress_enabled ? parent.$('#popup_ip,#popup_ipaddr_label').show() : parent.$('#popup_ip,#popup_ipaddr_label').hide();
    g_dialupFeature.mtu_enabled ? parent.$('#popup_mtu').show() : parent.$('#popup_mtu').hide();
    g_dialupFeature.iptype ? parent.$('#popup_iptype').show() : parent.$('#popup_iptype').hide();
    if(g_dialupFeature.connection_mode){
        parent.$('#popup_connection').show();
        if(G_DIALUPMODE_ONDEMAND== parent.$('#pro_dialupmode').val()){
              parent.$('#popup_maxideltime').show();  
        }
    }else{
          parent.$('#popup_connection').hide();
          parent.$('#popup_maxideltime').hide();  
    }
}

function promag_validInput(obj) {
    $.each(parent.$('.pro_wrong'), function(i) {
        $(this).remove();
    });
    $.each(parent.$('input'), function() {
        $(this).blur();
    });
    var flag = true;
    var wrongHtml = "<tr class='pro_wrong'>";
    wrongHtml += '<td>&nbsp;</td>';
    wrongHtml += "<td colspan='2' class='pro_wrong_td' id='temp_wrong'>&nbsp;</td>";
    wrongHtml += '</tr>';
    //Valid profile name
    if (obj.profile_name == '' || obj.profile_name == null) {
        flag = false;
        parent.$('#pro_name').focus();
        parent.$('#popup_name').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_name_wrong');
        parent.$('#pro_name_wrong').html('*' + IDS_dialup_hint_profile_name_null);
    }
    else if ('' == $.trim(obj.profile_name)) {
        flag = false;
        parent.$('#pro_name').focus();
        parent.$('#popup_name').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_name_wrong');
        parent.$('#pro_name_wrong').html('*' + samba_input_empty);
    }
    else if (obj.profile_name.length > 64 || obj.profile_name.length < 1)
    {
        flag = false;
        parent.$('#pro_name').focus();
        parent.$('#popup_name').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_name_wrong');
        parent.$('#pro_name_wrong').html('*' + dialup_hilink_hint_profile_name_invalidate);
    }
    else if (!checkInputChar_wanName(obj.profile_name)) {
        flag = false;
        parent.$('#pro_name').focus();
        parent.$('#popup_name').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_name_wrong');
        parent.$('#pro_name_wrong').html('*' + wan_valid_username);
    }
    else {
        if ('new_profile' == g_promag_operation_btnID) {
            if (!promag_ifDiffrentProfileName(obj.profile_name)) {
                flag = false;
            }

        }
        else if ('edit_profile' == g_promag_operation_btnID) {
            if (obj.profile_name != g_promag_currentProfileName) {
                if (!promag_ifDiffrentProfileName(obj.profile_name)) {
                    flag = false;
                }
            }
            else {
                parent.$('#pro_name_wrong').html('');
            }
        }

        if (!flag) {
            flag = false;
            parent.$('#pro_name').focus();
            parent.$('#popup_name').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_name_wrong');
            parent.$('#pro_name_wrong').html('*' + dialup_hint_profile_name_has_exist);
        }
        else {

            parent.$('#pro_name_wrong').html('');
        }
    }

    //Valid apn name
    if ('' != obj.apn_name && false == checkValidApn(obj.apn_name)) {
        flag = false;
        parent.$('#pro_apn').focus();
        parent.$('#popup_apn').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_apn_wrong');
        parent.$('#pro_apn_wrong').html('*' + dialup_hint_apn_valid_check);
    }
    else {
        if(parent.$("[name='auto_apn']:checked").val()==1 && obj.apn_name == '')
        {
            flag = false;
            parent.$('#pro_apn').focus();
            parent.$('#popup_apn').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_apn_wrong');
            parent.$('#pro_apn_wrong').html('*' + dialup_hint_apn_valid_check);
        }
        else
        {
            parent.$('#pro_apn_wrong').html('');
        }
       
    }
    // valid ipaddress
    if(!isValidIpAddress(obj.ipaddress) && obj.ipaddress != ''){
        flag = false;
        parent.$('#pro_ipaddress').focus();
        parent.$('#popup_ip').after(wrongHtml);
        parent.$('#temp_wrong').attr('id','pro_ip_wrong');
        parent.$('#pro_ip_wrong').html('*'+staticroute_hint_destinationNetworkAddress_invalid);
    }else{
        if(parent.$("[name='auto_ipaddress']:checked").val()==1 && obj.ipaddress == '')
        {
            flag = false;
            parent.$('#pro_ipaddress').focus();
            parent.$('#popup_ip').after(wrongHtml);
            parent.$('#temp_wrong').attr('id','pro_ip_wrong');
            parent.$('#pro_ip_wrong').html('*'+staticroute_hint_destinationNetworkAddress_invalid);  
        }
        else
        {
            parent.$('#pro_ip_wrong').html('');
        }
    }
    
    //Valid Username
    if ('' != obj.username) {
        if ('' == $.trim(obj.username)) {
            flag = false;
            parent.$('#pro_username').focus();
            parent.$('#popup_username').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_username_wrong');
            parent.$('#pro_username_wrong').html('*' + samba_input_empty);
        }
        else if(!checkInputChar_wanName(obj.username)){
            flag = false;
            parent.$('#pro_username').focus();
            parent.$('#popup_username').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_username_wrong');
            parent.$('#pro_username_wrong').html('*' + wan_valid_username);
        }
    }
    else {
        parent.$('#pro_username_wrong').html('');
    }

    //Vaild user password
    if ('' != obj.password) {
        if ('' == $.trim(obj.password)) {
            flag = false;
            parent.$('#pro_password').focus();
            parent.$('#popup_password').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_password_wrong');
            parent.$('#pro_password_wrong').html('*' + samba_input_empty);
        }
        else if(!checkInputChar_wanName(obj.password))
        {
            flag = false;
            parent.$('#pro_password').focus();
            parent.$('#popup_password').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_password_wrong');
            parent.$('#pro_password_wrong').html('*' + wan_valid_username);
        }
    }
    else {
        parent.$('#pro_password_wrong').html('');
    }


    if(true == isNaN(obj.MTU)){
        flag = false;
        parent.$('#pro_mtu').focus();
        parent.$('#popup_mtu').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_mtu_wrong');
        parent.$('#pro_mtu_wrong').html('*' + samba_input_invalid);
    }else if(('ipv4' ==obj.iptype)&&(MIN_WIRELESS_IPV4_MTU > obj.MTU || MAX_WIRELESS_MTU < obj.MTU) ){
        flag = false;
        parent.$('#pro_mtu').focus();
        parent.$('#popup_mtu').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_mtu_wrong');
        parent.$('#pro_mtu_wrong').html('*' + hint_ethprofile_mtu_invalid);
    }else if(('ipv6' ==obj.iptype||'ipv4ipv6' ==obj.iptype)&&(MIN_WIRELESS_IPV6_MTU > obj.MTU || MAX_WIRELESS_MTU < obj.MTU) ){
        flag = false;
        parent.$('#pro_mtu').focus();
        parent.$('#popup_mtu').after(wrongHtml);
        parent.$('#temp_wrong').attr('id', 'pro_mtu_wrong');
        parent.$('#pro_mtu_wrong').html('*' + hint_profile_ipv6_mtu_invalid);
    }else {
        parent.$('#pro_mtu_wrong').html('');
    }

    if(G_DIALUPMODE_ONDEMAND == obj.ConnectMode){
        if(true == isNaN(obj.MaxIdelTime)){
            flag = false;
            parent.$('#pro_maxideltime').focus();
            parent.$('#popup_maxideltime').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_mode_wrong');
            parent.$('#pro_mode_wrong').html('*' + dialup_hint_max_idle_time_number);
        }else if((obj.MaxIdelTime.indexOf('0') == 0) && (obj.MaxIdelTime != 0)){
            flag = false;
            parent.$('#pro_maxideltime').focus();
            parent.$('#popup_maxideltime').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_mode_wrong');
            parent.$('#pro_mode_wrong').html('*' + dialup_hint_max_idle_time_invalid);
        }else if(obj.MaxIdelTime.indexOf('.') != -1){
            flag = false;
            parent.$('#pro_maxideltime').focus();
            parent.$('#popup_maxideltime').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_mode_wrong');
            parent.$('#pro_mode_wrong').html('*' + dialup_hint_max_idle_time_integer);
        }else if((obj.MaxIdelTime < MIN_IDLE_TIME) || (obj.MaxIdelTime > MAX_IDLE_TIME)){
            flag = false;
            parent.$('#pro_maxideltime').focus();
            parent.$('#popup_maxideltime').after(wrongHtml);
            parent.$('#temp_wrong').attr('id', 'pro_mode_wrong');
            parent.$('#pro_mode_wrong').html('*' + dialup_hint_max_idle_time_range);
        }
    }

    return flag;
}

function checkNum(id){
    var flag = true;
    var sizeParts = parent.$('#'+id).val().split("");
    if(parent.$('#'+id).val()<1||parent.$('#'+id).val()>1500){
        flag = false;
    } else{
        $(sizeParts).each(function(i){
            if(!(sizeParts[i]<='9'&&sizeParts[i]>='0')){
                flag = false;
            }
        });
    }
    return flag;
}

function promag_getNewIndex() {
    var index_array = [];
    var new_index = '';

    //Get all indexs.
    if ('undefined' == typeof (g_promag_info)) {
        index_array.push(0);
    }
    else {
        $('.list').each(function(k) {
            index_array.push(parseInt($(this).attr('id'), 10));
        });
    }

    //Make an new g_promag_index according to currently g_promag_index
    var i = 0;
    for (i; i < index_array.length; i++) {
        if (index_array[i + 1] - index_array[i] == 1) {
            new_index = parseInt(index_array[i + 1], 10) + 1;
        }
        else {
            new_index = parseInt(index_array[i], 10) + 1;
            break;
        }
    }
    return new_index;
}

function promag_getAutoAPN() {
    getAjaxData('api/dialup/auto-apn', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            g_promag_autoAPN = parseInt(ret.response.AutoAPN, 10) == 1;
        }
        if (g_promag_autoAPN) {
            $('input[name=redio_autoapn]').eq(0).attr('checked', 'checked');
            button_enable('edit_profile', '0');
            button_enable('delete_profile', '0');
            button_enable('new_profile', '1');
            $('#profilelist').attr('disabled', 'disabled');
            $('#profiles_info tr:gt(0) td').css({
                color: '#BFBFBF'
            });
        }
        else {
            $('input[name=redio_autoapn]').eq(1).attr('checked', 'checked');
            button_enable('new_profile', '1');
            $('#profilelist').removeAttr('disabled');
            $('#profiles_info tr:gt(0) td').css({
                color: 'black'
            });
            var aProfile = null;
            if (g_promag_info) {
                if ($.isArray(g_promag_info)) {
                    aProfile = g_promag_info[g_promag_array_index];
                }
                else {
                    aProfile = g_promag_info;
                }
                if (aProfile.ReadOnly == '0') {
                    button_enable('edit_profile', '1');
                    button_enable('delete_profile', '1');
                }
                else {
                    button_enable('edit_profile', '0');
                    button_enable('delete_profile', '0');
                }
            }
            else {
                button_enable('edit_profile', '0');
                button_enable('delete_profile', '0');
            }
            // $("#select_service").removeAttr("disabled");
            // $("#select_service").addClass("input_select");
        }
    });
}

function promag_onChangeAutoAPN() {
    g_promag_autoAPN = $(this).get(0).value == '0';
    var request = {
        AutoAPN: g_promag_autoAPN ? 1 : 0
    };
    var xmlstr = object2xml('request', request);
    saveAjaxData('api/dialup/auto-apn', xmlstr, function($xml) {
        promag_getAutoAPN();
    });
}

function setDialupModeStatus(){
    if(G_DIALUPMODE_ONDEMAND== parent.$('#pro_dialupmode').val()){
        parent.$('#popup_maxideltime').show();
    }else{
        parent.$('#popup_maxideltime').hide();
    }
}

function editProfile(){
    var currentProfileData = null;
    parent.call_dialog(common_edit, g_promag_dialogContent, common_save, 'pop_Save', common_cancel, 'pop_Cancel',promag_saveProfile,cancel_profile);
    parent.$("[name='auto_apn']").die().live('click', set_apn_IsStatic);
    parent.$("[name='auto_ipaddress']").die().live('click', set_ipaddress_IsStatic);
    if ($.isArray(g_promag_info)) {
        currentProfileData = g_promag_info[g_promag_array_index];
    }
    else if (typeof (g_promag_info) != 'undefined') {
        currentProfileData = g_promag_info;
    }
    else {
        log.error('no profile to be edit.');
        return;
    }
    g_currentProfileAuthMode = currentProfileData.AuthMode;
    g_promag_currentProfileName = currentProfileData.Name;
    parent.$('#pro_name').val(currentProfileData.Name);
    parent.$('#pro_number').val(currentProfileData.DailupNum);
    parent.$('#pro_username').val(currentProfileData.Username);
    parent.$('#pro_mtu').val(currentProfileData.MTU);
    parent.$('#pro_password').val(currentProfileData.Password == '' ? '' : decrypt(currentProfileData.Password));
    
    if (PRO_AUTHMODE_PAP == currentProfileData.AuthMode) {
        parent.$('#pop_authmode').val(PRO_AUTHMODE_PAP);
    }
    else if (PRO_AUTHMODE_CHAP == currentProfileData.AuthMode) {
        parent.$('#pop_authmode').val(PRO_AUTHMODE_CHAP);
    }
    else if (PRO_AUTHMODE_NONE == currentProfileData.AuthMode) {
        parent.$('#pop_authmode').val(PRO_AUTHMODE_NONE);
    }
    else {
        parent.$('#pop_authmode').val(PRO_AUTHMODE_AUTO);
    }
    
    if(G_DIALUPMODE_ONDEMAND == currentProfileData.ConnectMode){
        parent.$('#pro_dialupmode').val(G_DIALUPMODE_ONDEMAND);
    }else if(G_DIALUPMODE_AUTO == currentProfileData.ConnectMode){
        parent.$('#pro_dialupmode').val(G_DIALUPMODE_AUTO);
    }else if(G_DIALUPMODE_MANUAL == currentProfileData.ConnectMode){
        parent.$('#pro_dialupmode').val(G_DIALUPMODE_MANUAL);
    }
    parent.$('#pro_maxideltime').val(currentProfileData.MaxIdelTime);
    parent.$('#pro_apn').val(currentProfileData.ApnName);
    if(currentProfileData.ApnName!='')
    {
        parent.$('input[name=auto_apn][value=' + '1' + ']').attr('checked', true);
    }
    else
    {
        parent.$('input[name=auto_apn][value=' + '0' + ']').attr('checked', true);
    }  
    set_apn_IsStatic();
    parent.$('#pro_ipaddress').val(currentProfileData.IpAddress);
    if(currentProfileData.IpAddress!='')
    {
        parent.$('input[name=auto_ipaddress][value=' + '1' + ']').attr('checked', true);
    }
    else
    {
        parent.$('input[name=auto_ipaddress][value=' + '0' + ']').attr('checked', true);
    }   
    set_ipaddress_IsStatic();
    if((1 == currentProfileData.IPv4_Supported) && (0 == currentProfileData.IPv6_Supported)){
        parent.$('#pro_iptype').val('ipv4');
    }else if((1 == currentProfileData.IPv6_Supported) && (0 == currentProfileData.IPv4_Supported)){
        parent.$('#pro_iptype').val('ipv6');
    }else if((1 == currentProfileData.IPv6_Supported) && (1 == currentProfileData.IPv4_Supported)){
        parent.$('#pro_iptype').val('ipv4ipv6');
    }
    parent.$('#pro_name').select();
    
    promag_setPopupTrDisplay();
}

$(document).ready(function() {
    var list = '';
    var wlan_td_content = '';

    getConfigData('config/dialup/config.xml', function($xml) {
        g_dialupFeature = _xml2feature($xml);
    }, {
        sync: true
    });

    if (parent.g_module.autoapn_enabled) {
        promag_getAutoAPN();
        $('#tr_profile_autoapn').show();
    }

    // add tooltips
    promag_getProfileInfo();

    $('#new_profile').click(function(event) {
        if (!isButtonEnable('new_profile')) {
            return;
        }

        if (($.isArray(g_promag_info)) && (g_promag_info.length >= PROFILE_MAX_NUM)) {
            parent.showInfoDialog(dialup_hint_max_profile_number.replace('%d', PROFILE_MAX_NUM));
            return;
        }
        g_promag_operation_btnID = event.currentTarget.id;

        //Load popup window
        parent.call_dialog(dialup_button_newprofile, g_promag_dialogContent, common_save, 'pop_Save', common_cancel, 'pop_Cancel',promag_saveProfile,cancel_profile);
        parent.$("[name='auto_apn']").die().live('click', set_apn_IsStatic);
        parent.$("[name='auto_ipaddress']").die().live('click', set_ipaddress_IsStatic);
        promag_setPopupTrDisplay();

        parent.$('#pro_name').val('');
        parent.$('#pro_number').val(g_dialupFeature.dialup_number);
        parent.$('#pro_username').val('');
        parent.$('#pro_password').val('');
        parent.$('#pro_authentication').val('');
        parent.$('#pro_apn').val('');
        parent.$('input[name=auto_apn][value=' + '0' + ']').attr('checked', true);
        parent.$('input[name=auto_ipaddress][value=' + '0' + ']').attr('checked', true);
        set_apn_IsStatic();
        set_ipaddress_IsStatic();
        parent.$('#pro_ipaddress').val('');
        parent.$('#pro_mtu').val(g_dialupFeature.mtu_size);
        parent.$('#pro_maxideltime').val(g_dialupFeature.max_idle_time);
        parent.$('#pro_name').focus();
    });
    $('#edit_profile').click(function(event) {
        if (!isButtonEnable('edit_profile')) {
            return;
        }
        g_promag_operation_btnID = event.currentTarget.id;
        if((g_promag_default_index == $('#profilelist').val()) && ("2" == parent.G_MonitoringStatus.response.ServiceStatus) &&
            ((MACRO_CONNECTION_IPV6CONNECTED == parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus) ||
            (MACRO_CONNECTION_CONNECTED == parent.G_MonitoringStatus.response.ConnectionStatus))){
            parent.showConfirmDialog(lable_disconnect_network + " " + lable_continue_anyway, editProfile, promag_cancelDelete);
        }else{
            editProfile();
        }
    });
    //Send the status of apn and ipaddress for popup window
    //promag_switchRadio("apn");
    //promag_switchRadio("ipaddress");

    $('#delete_profile').click(function(event) {
        if (!isButtonEnable('delete_profile')) {
            return;
        }
        if((g_promag_default_index == $('#profilelist').val()) && ("2" == parent.G_MonitoringStatus.response.ServiceStatus) &&
            ((MACRO_CONNECTION_IPV6CONNECTED == parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus) ||
            (MACRO_CONNECTION_CONNECTED == parent.G_MonitoringStatus.response.ConnectionStatus))){
            parent.showConfirmDialog(lable_disconnect_network + " " + lable_continue_anyway, promag_deleteProfile, promag_cancelDelete);
        }else{
            parent.showConfirmDialog(dialup_hilink_confirm_delete, promag_deleteProfile, promag_cancelDelete);
        }
    });
    $('input[name=redio_autoapn]').bind('click', promag_onChangeAutoAPN);

    $('#profilelist').change(function() {
        g_promag_index = $('#profilelist').val();
        g_promag_array_index = Profilesmgr_getCurrentProfileArrayIndex(g_promag_info, g_promag_index);
        //g_promag_index-1;
        $('#dprofile_name').html('<pre>' + resolveXMLEntityReference(g_promag_info[g_promag_array_index].Name) + '</pre>');
        $('#ddialup_number').html(g_promag_info[g_promag_array_index].DailupNum);
        $('#duser_name').html('<pre>' + resolveXMLEntityReference(g_promag_info[g_promag_array_index].Username) + '</pre>');
        $('#profile_mtu').html('<pre>' + g_promag_info[g_promag_array_index].MTU + '</pre>');

        if ('' != g_promag_info[g_promag_array_index].Password && ' ' != g_promag_info[g_promag_array_index].Password && null != g_promag_info[g_promag_array_index].Password) {
            var profile_password_mark = '';
            profile_password_mark = promag_trasfalPassword(g_promag_info[g_promag_array_index].Password == '' ? '' : decrypt(g_promag_info[g_promag_array_index].Password));
            $('#dpassword').html(profile_password_mark);
        }
        else {
            $('#dpassword').html('');
        }

        if (PRO_AUTHMODE_PAP == g_promag_info[g_promag_array_index].AuthMode) {
            $('#dauthentication').html(dialup_label_pap);
        }
        else if (PRO_AUTHMODE_CHAP == g_promag_info[g_promag_array_index].AuthMode) {
            $('#dauthentication').html(dialup_label_chap);
        }
        else if (PRO_AUTHMODE_NONE == g_promag_info[g_promag_array_index].AuthMode) {
            $('#dauthentication').html(wlan_label_none);
        }
        else {
             $('#dauthentication').html(common_auto);
        }
        if(G_DIALUPMODE_ONDEMAND == g_promag_info[g_promag_array_index].ConnectMode){
            $('#profile_connectionmode').html('<pre>' + label_dailup_option_on_demand + '</pre>');
            $('#profile_maxideltime_tr').show();
            $('#profile_maxideltime').html('<pre>' + g_promag_info[g_promag_array_index].MaxIdelTime + '</pre>');
        }else if(G_DIALUPMODE_AUTO == g_promag_info[g_promag_array_index].ConnectMode){
            $('#profile_connectionmode').html('<pre>' + common_auto + '</pre>');
            $('#profile_maxideltime_tr').hide();
        }else if(G_DIALUPMODE_MANUAL == g_promag_info[g_promag_array_index].ConnectMode){
            $('#profile_connectionmode').html('<pre>' + common_manual + '</pre>');
            $('#profile_maxideltime_tr').hide();
        }
        
        //promag_checkIfStatic(g_promag_info[g_promag_array_index]);
        if (g_promag_info[g_promag_array_index].ApnIsStatic == '0'){
            $('#dapn').html('<pre>' + dialup_label_dynamic + '</pre>');
        }
        else {
            $('#dapn').html('<pre>' + g_promag_info[g_promag_array_index].ApnName + '</pre>');
        }
        //check ip if static
        if(g_promag_info[g_promag_array_index].IpIsStatic =='0'){
            $('#dip_address').html('<pre>' + dialup_label_dynamic + '</pre>');
        }else{
            $('#dip_address').html('<pre>' + g_promag_info[g_promag_array_index].IpAddress + '</pre>');
        }

        if((1 == g_promag_info[g_promag_array_index].IPv4_Supported) && (0 == g_promag_info[g_promag_array_index].IPv6_Supported)){
            $('#profile_iptype').html('<pre>' + wan_ipv4_text + '</pre>');
        }else if((1 == g_promag_info[g_promag_array_index].IPv6_Supported) && (0 == g_promag_info[g_promag_array_index].IPv4_Supported)){
            $('#profile_iptype').html('<pre>' + wan_ipv6_text + '</pre>');
        }else if((1 == g_promag_info[g_promag_array_index].IPv4_Supported) && (1 == g_promag_info[g_promag_array_index].IPv6_Supported)){
            $('#profile_iptype').html('<pre>' + wan_ipv4_ipv6_text + '</pre>');
        } 
        if (g_promag_info[g_promag_array_index].ReadOnly == '0') 
        {
            button_enable('edit_profile', '1');
            button_enable('delete_profile', '1');
        }
        else {
            button_enable('edit_profile', '0');
            button_enable('delete_profile', '0');
        }
    });
    parent.$('#pro_dialupmode').die().live('change', function() {
        setDialupModeStatus();
    });
});
