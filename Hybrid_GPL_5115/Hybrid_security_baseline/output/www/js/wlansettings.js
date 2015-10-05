
var WLAN_CHANNEL_AUTO = '0';
var g_wlan_basicSetting = ''; //Data of WiFi Basic Setting
var g_wlan_setcountry = ""; //Data of Wifi Basic Country
var g_wlan_rates = "";
var Wifienable = parent.G_MonitoringStatus.response.WifiStatus;

var WIFIAUTHMODE_OPEN = 'None';
var WIFIAUTHMODE_SHARE = 'Basic';
var WIFIAUTHMODE_WPA_PSK = 'WPA';
var WIFIAUTHMODE_WPA2_PSK = '11i';
var WIFIAUTHMODE_WPA_WPA2_PSK = 'WPAand11i';
var WIFIBASICENCRYPMODE_NONE = 'None';
var WIFIBASICENCRYPMODE_SHARE = 'SharedAuthentication';
var WIFIBASICENCRYPMODE_WEP = 'WEPEncryption';
var WIFIBASICENCRYPMODE_WEP64 = 'WEP64';
var WIFIBASICENCRYPMODE_WEP128 = 'WEP128';
var WIFIENCRYPTIONMODE_AES = 'AESEncryption';
var WIFIENCRYPTIONMODE_TKIP = 'TKIPEncryption';
var WIFIENCRYPTIONMODE_AES_TKIP = 'TKIPandAESEncryption';
var WIFIBASIC_11NMCS = 15;
var STRING_SET_AUTO = 'Auto';
var g_WifiMaxAssoc = 32;
var g_Current_WifiWme = 0;
var g_11NBandWith_20 = '20';
var g_11NBandWith_20_40 = '20/40';
var g_Country_JP = 'JP';

/*------------------------get channel from data------------------------------------*/
function wlanadvanced_initChannel(channel_max) {
    $('#select_WifiChannel').unbind('click');
    $('#select_WifiChannel').empty();
    $('#select_WifiChannel').append("<option value='"+0+"'>" + common_auto + '</option>');
    var i = 0;
    for (i = 1; i <= channel_max; i++)
    {
        $('#select_WifiChannel').append('<option value=' + i + '>' + i + '</option>');
    }
}

/**---------------------------------function init 11n mcs---------------------------*/
function wlanbasicsettings_initMCS() {
    $("#select_show_mcs").unbind("click");
    $("#select_show_mcs").empty();
    $("#select_show_mcs").append("<option value='33'>"+common_auto+"</option>");
    var i = 0;
    if(g_wlan_basicSetting.Wlan11NTxRxStream == "1") {
        for (i = 1; i <= WIFIBASIC_11NMCS/2; i++) {
            $("#select_show_mcs").append("<option value='"+i+"'>"+i+"</option>");
        }
    }
    else {
        for (i = 8; i <= WIFIBASIC_11NMCS; i++) {
            $("#select_show_mcs").append("<option value='"+i+"'>"+i+"</option>");
        }
    }
    
}
/*-------------------------------init country------------------------------------------*/
function wlanadvanced_initCountry() {
    $('#select_WifiChannel').unbind('click');
    $('#select_WifiCountry').empty();
    var i = 0;
    for (i = 0; i < countryArray.length; i++)
    {
        $('#select_WifiCountry').append('<option value=' + countryArray[i][0] + '>' + countryArray[i][2] + '</option>');
    }
}

/*---------------------Check 11N Band Width with Country and Channel------------------------*/
function Check_11NBandWidth_CountryChannel(){
    var Country_Array_value = ['DZ', 'AM', 'BY', 'BZ', 'BO', 'BN', 'DO', 'GT',  'KR',  'MA', 'MO', 'RU',  'UA', 'UZ', 'YE'];
    var Country_Array_text = [wlan_country_dz,wlan_country_am,wlan_country_by,wlan_country_bz,wlan_country_bo,wlan_country_bn,wlan_country_do,wlan_country_gt,wlan_country_kr,wlan_country_ma,wlan_country_mo,wlan_country_ru,wlan_country_ua,wlan_country_uz,wlan_country_ye];
    if((g_11NBandWith_20_40 == $('#select_wlan11NBWControl').val()) && 'none' != $('.show_11n_info').css('display')){
        for(var i=0;i<Country_Array_value.length;i++){
             $("#select_WifiCountry option[value="+ Country_Array_value[i] +"]").remove();
        }
    }else if(g_11NBandWith_20 == $('#select_wlan11NBWControl').val() || 'none' == $('.show_11n_info').css('display')){
        if(0==$("#select_WifiCountry option[value='DZ']").length && 0==$("#select_WifiCountry option[value='YE']").length){
            for(var i=0;i<Country_Array_value.length;i++){
                $('#select_WifiCountry').append("<option value='"+Country_Array_value[i]+"'>" + Country_Array_text[i] + '</option>');
            }
        }
    }
    check_802ModeWidth_CountryChannel();
}

/*------------check 802.11g&Country is Japan----------------*/
function check_802ModeWidth_CountryChannel(){
    if(g_Country_JP == $('#select_WifiCountry').val()){
        if('b' != $('#select_WifiMode').val())
        {
            $("#select_WifiChannel option[value='14']").remove();  
        }
        else if(0==$("#select_WifiChannel option[value='14']").length){
            $('#select_WifiChannel').append("<option value='14'>" + 14 + '</option>');
        }
    }
}
/*----------------------------get ssid index------------------------------*/
function getSsidIndex(ssidObject){
    $("#select_show_ssid_index").unbind("click");
    $("#select_show_ssid_index").empty();
    if($.isArray(ssidObject)){
        $.each(ssidObject,function(n,value){
            $("#select_show_ssid_index").append("<option value = "+n+">"+wlan_label_ssid+value.WifiIndex+"</option>");
        });
    }
    else {
        $("#select_show_ssid_index").append("<option value = 0>"+wlan_label_ssid+ssidObject.WifiIndex+"</option>");
    }
}

/*-------------------------------set network ley---------------------------------------------*/
function wlanbasicsettings_networkKey(key,ssidinfo) {
    $('#ssid_neworkKey1').val(ssidinfo.WifiWepKey1 == '' ?  '' : decrypt(ssidinfo.WifiWepKey1));
    $('#ssid_neworkKey2').val(ssidinfo.WifiWepKey2 == '' ?  '' : decrypt(ssidinfo.WifiWepKey2));
    $('#ssid_neworkKey3').val(ssidinfo.WifiWepKey3 == '' ?  '' : decrypt(ssidinfo.WifiWepKey3));
    $('#ssid_neworkKey4').val(ssidinfo.WifiWepKey4 == '' ?  '' : decrypt(ssidinfo.WifiWepKey4));
    if (key == WIFIBASICENCRYPMODE_NONE)
    {
        $('#ssid_network_key').hide();
        if (($('#ssid_encryption_mode_basic').val() == WIFIBASICENCRYPMODE_NONE &&
            $('#ssid_authentication').val() == WIFIAUTHMODE_OPEN))
        {
            $('#checkbox_password').hide();
        }
    }
    else if (key == WIFIBASICENCRYPMODE_WEP)
    {
        $('#ssid_network_key').show();
        $('#ssid_current_network_key').val(ssidinfo.WifiWepKeyIndex);
        $('#checkbox_password').show();
    }
}

/*---------------------------------set Authmode and change mode-------------------------------------------*/
function changeAndGetMode(mode,g_wlan_basicSettingData) {
    if(WIFIAUTHMODE_OPEN == mode  ||
       WIFIAUTHMODE_SHARE == mode ) 
    {
       $('#div_ssid_encrypt_way1').show();
       $('#div_ssid_encrypt_way2').hide();
       $('#ssid_authentication').val(mode);
       if (WIFIAUTHMODE_SHARE == mode)
       {
            $('#ssid_encryption_mode_basic option[value='+ WIFIBASICENCRYPMODE_NONE +']').remove();
            if($("#ssid_encryption_mode_basic option[value= "+WIFIBASICENCRYPMODE_WEP +"]").length == 0) {
                $("#ssid_encryption_mode_basic").prepend("<option value="+ WIFIBASICENCRYPMODE_WEP +">" + wlan_label_wep + '</option>');
            }
       }
       else
       {
            if ($('#ssid_encryption_mode_basic option[value='+WIFIBASICENCRYPMODE_NONE+']').length == 0)
            {
                $('#ssid_encryption_mode_basic').prepend("<option value="+ WIFIBASICENCRYPMODE_NONE +">" + wlan_label_none + '</option>');
            }
            if($("#ssid_encryption_mode_basic option[value= "+WIFIBASICENCRYPMODE_WEP +"]").length == 0) {
                $("#ssid_encryption_mode_basic").prepend("<option value="+ WIFIBASICENCRYPMODE_WEP +">" + wlan_label_wep + '</option>');
            }
            setTimeout(function() {
                 $('#ssid_encryption_mode_basic').val(g_wlan_basicSettingData.WifiBasicencryptionmodes);
            },1);            
       }
       if($('#select_WifiMode').val()== "n")
       {
            if($("#ssid_encryption_mode_basic option[value= "+WIFIBASICENCRYPMODE_WEP +"]").length != 0)
            {
                $('#ssid_encryption_mode_basic option[value='+ WIFIBASICENCRYPMODE_WEP +']').remove();
            }       
       }
       
       setTimeout(function(){
           wlanbasicsettings_networkKey($('#ssid_encryption_mode_basic').val(), g_wlan_basicSettingData);
       }, 1);
       
       $('#ssid_caution').show();
    }
    else {
        $('#checkbox_password').show();
        $('#div_ssid_encrypt_way2').show();
        $('#div_ssid_encrypt_way1').hide();
        $("#ssid_wpa_key").val(g_wlan_basicSettingData.WifiWpapsk == '' ?  '' : decrypt(g_wlan_basicSettingData.WifiWpapsk));
        $('#ssid_authentication').val(mode);
        if(WIFIAUTHMODE_WPA_PSK == mode){
                $("#ssid_encryption_mode_wpa").val(g_wlan_basicSettingData.WifiWpaencryptionmodes);
        }
        else if(WIFIAUTHMODE_WPA2_PSK == mode){
                $("#ssid_encryption_mode_wpa").val(g_wlan_basicSettingData.IEEE11iEncryptionModes);
        }
        else if(WIFIAUTHMODE_WPA_WPA2_PSK == mode){
                $("#ssid_encryption_mode_wpa").val(g_wlan_basicSettingData.X_MixedEncryptionModes);
        }
    }
}
function change_WifiRate(mode)
{
    g_wlan_rates = "";
    var i = 0;
    $("#select_WifiRate").children().remove();
    
    if(mode == "b") {
        g_wlan_rates = [STRING_SET_AUTO, '1', '2', '5.5', '11'];
    }
    else if(mode == "g") {
        g_wlan_rates = [STRING_SET_AUTO, '1', '2', '5.5', '6', '9', '12', '18', '24', '36', '48', '54'];
    }
    else if(mode == "b/g/n") {
        g_wlan_rates = [STRING_SET_AUTO, '6.5', '13', '19.5', '26', '39', '52', '58.5', '65', '78', '104', '117', '130','300'];
    }
    else {
        g_wlan_rates = [STRING_SET_AUTO, '1', '2', '5.5', '6', '9', '11', '12', '18', '24', '36', '48', '54'];
    }
    
    for(i = 0;i<g_wlan_rates.length;i++) {
        if (0 == i)
        {
             $("#select_WifiRate").append("<option value='"+g_wlan_rates[i]+"'>"+ common_auto +"</option>");
        }
        else
        {
            $("#select_WifiRate").append("<option value='"+g_wlan_rates[i]+"'>"+g_wlan_rates[i]+"</option>");
        }
    }
}

/*--------------------------set 11n or rate show--------------------------------*/
function showRateOr11N(mode) {
   
    if(mode == "n" || mode == "b/g/n") {
        if(mode == "n") {
            $("#ssid_authentication option[value="+ WIFIAUTHMODE_SHARE +"]").remove();
            if(WIFIAUTHMODE_OPEN == $('#ssid_authentication').val()){
                if($("#ssid_encryption_mode_basic option[value= "+WIFIBASICENCRYPMODE_WEP +"]").length != 0)
                {
                    $("#ssid_encryption_mode_basic option[value="+ WIFIBASICENCRYPMODE_WEP +"]").remove();
                }
                if(0 == $('#ssid_encryption_mode_basic option[value='+WIFIBASICENCRYPMODE_NONE+']').length){
                    $('#ssid_encryption_mode_basic').prepend("<option value="+ WIFIBASICENCRYPMODE_NONE +">" + wlan_label_none + '</option>');
                }
                $('#ssid_network_key').hide();
                $('#checkbox_password').hide();
            }
        }
        else {
            if($("#ssid_authentication option[value="+ WIFIAUTHMODE_SHARE +"]").length == 0) {
                $("#ssid_authentication").prepend("<option value="+ WIFIAUTHMODE_SHARE +">" + wlan_label_share + '</option>');
            }
        
            if($("#ssid_encryption_mode_basic option[value= "+WIFIBASICENCRYPMODE_WEP +"]").length == 0) {
                $("#ssid_encryption_mode_basic").prepend("<option value="+ WIFIBASICENCRYPMODE_WEP +">" + wlan_label_wep + '</option>');
            }
        }
        $(".show_11n_info").show();
        $(".show_rate_info").hide();
        $("#select_show_wmm").val(1);
        $("#select_show_wmm").attr("disabled",true);
    }
    else {
        $(".show_11n_info").hide();
        $(".show_rate_info").show();
        $("#select_show_wmm").val(g_Current_WifiWme);
        $("#select_show_wmm").removeAttr("disabled");
        if($("#ssid_authentication option[value="+ WIFIAUTHMODE_SHARE +"]").length == 0) {
            $("#ssid_authentication").prepend("<option value="+ WIFIAUTHMODE_SHARE +">" + wlan_label_share + '</option>');
        }
        
        if($("#ssid_encryption_mode_basic option[value="+ WIFIBASICENCRYPMODE_WEP +"]").length == 0) {
            $("#ssid_encryption_mode_basic").prepend("<option value="+ WIFIBASICENCRYPMODE_WEP +">" + wlan_label_wep + '</option>');
        }
    }
    Check_11NBandWidth_CountryChannel();
    showEncryptionMode(mode);

    if('none' != $(".show_rate_info").css("display"))
    {
        change_WifiRate(mode);
    }
}

/*---------------- 802.11 mode is 'n',show Encryption Mode-----------------------------*/
function showEncryptionMode(mode){

    if('none' == $("#div_ssid_encrypt_way2").css("display"))
    {
        return; 
    }
    if('n' != mode){
        if(0 == $('#ssid_encryption_mode_wpa option[value=' + WIFIENCRYPTIONMODE_TKIP + ']').length){
            $('#ssid_encryption_mode_wpa').append('<option value = ' + WIFIENCRYPTIONMODE_TKIP + '>' + wlan_label_tkip + '</option>');
        } 
        if(0 == $('#ssid_encryption_mode_wpa option[value=' + WIFIENCRYPTIONMODE_AES_TKIP + ']').length){
            $('#ssid_encryption_mode_wpa').append('<option value = ' + WIFIENCRYPTIONMODE_AES_TKIP + '>' + wlan_label_aes_tkip+ '</option>');
        }
    }else{
        if((WIFIAUTHMODE_WPA_PSK == $('#ssid_authentication').val()) ||
            (WIFIAUTHMODE_WPA2_PSK == $('#ssid_authentication').val()) ||
            (WIFIAUTHMODE_WPA_WPA2_PSK == $('#ssid_authentication').val())){
                $('#ssid_encryption_mode_wpa option[value=' + WIFIENCRYPTIONMODE_TKIP + ']').remove();
                $('#ssid_encryption_mode_wpa option[value=' + WIFIENCRYPTIONMODE_AES_TKIP + ']').remove();
        }
    }
}

/*------------------------------set info-------------------------------*/
function setData (showObject) {
    g_Current_WifiWme = showObject.WifiWme;
    var i = 0;
    for (i = 0; i < countryArray.length; i++)
    {
        if (countryArray[i][0] == showObject.WifiCountry)
        {
            wlanadvanced_initChannel(countryArray[i][1]);
            break;
        }
    }
    
    if(showObject.WifiMode == "n" || showObject.WifiMode == "b/g/n") {
        $(".show_11n_info").show();
        $(".show_rate_info").hide();
        $("#select_show_wmm").attr("disabled",true);
    }
    else {
        $(".show_11n_info").hide();
        $(".show_rate_info").show();
    }
    $('#select_WifiCountry').val(showObject.WifiCountry);
    $("#select_WifiChannel").val(showObject.WifiChannel);
    $("#select_mhz").val(showObject.WifiTxPwrPcnt);
    $('#select_show_txprower').val(showObject.TxEnhance);
    $("#select_wlan11NBWControl").val(showObject.X_Wlan11NBWControl);
    $("#select_interval").val(showObject.Wifi11NGuardInterval);
    $('#select_WifiMode').val(showObject.WifiMode);
    changeAndGetMode(showObject.WifiAuthmode,showObject);
    showRateOr11N(showObject.WifiMode);
    setTimeout(function() {
        $('#ssid_encryption_mode_basic').val(showObject.WifiBasicencryptionmodes);
    },1);    
    $('input[name= ssid_wifiBroadcast][value=' + showObject.WifiHide + ']').attr('checked', true);
    $('input[name= ssid_wpsenble][value=' + showObject.WifiWpsenbl + ']').attr('checked', true);
    $('#ssid_wifiIsolate').val(showObject.WifiIsolate);
    $('#ssid_wifiName').val(showObject.WifiSsid);
    $("#input_show_max_limit").val(showObject.WifiMaxAssoc);
    $("#select_show_wmm").val(showObject.WifiWme);
    setTimeout(function() {
        $("#select_show_mcs").val(showObject.Wifi11NMcs);
    },1);
    $("#ssid_current_network_key").val(showObject.WifiWepKeyIndex);
    setTimeout(function() {
        $("#select_WifiRate").val(showObject.WifiRate);
    },1);
    $('#check_wpa_psk').get(0).checked = false;
    if(0 != $('#select_show_ssid_index').val()){
        $('#check_wifi_enable').attr('checked', (showObject.SSIDEnable==1? true:false));
    }
}

/*----------------------------------init page-----------------------------------*/
function wifibasicsettings_initpage() {
    $('#select_WifiCountry').show();
    wlanadvanced_initCountry();
    $(".show_wifi_enable_checkbox").hide();
    
    getAjaxData('api/wlan/basic-settings',function($xml){
        var ret = xml2object($xml);
        if('response' == ret.type){
            g_wlan_basicSetting = ret.response.Ssids.Ssid;
            getSsidIndex(g_wlan_basicSetting);
            wlanbasicsettings_initMCS();
            if(!$.isArray(g_wlan_basicSetting)) {
                setData(g_wlan_basicSetting);
            }
            else {
                setData(g_wlan_basicSetting[0]);
            }
            wlanbasicsettings_showPassword();
            enable_wps_set();
        }
        else{
                log.error('Get api/wlan/basic-settings error.');
        }
   },{
        sync : true
   });
}

/*-----------------------------------show different info when index changed----------------------------------------*/
function getDifferentData(value) {
    setData(g_wlan_basicSetting[parseInt(value,10)]);
    if(value == "0") {
        $(".show_wifi_enable_checkbox").hide();
    }
    else {
        $(".show_wifi_enable_checkbox").show();
    }
}

/*--------------------------show password when checked-----------------------------------*/
function wlanbasicsettings_showPassword() {
    var cbValue = $('#check_wpa_psk').attr('checked');
    var strType = cbValue ? 'text' : 'password';
    $.each($('input[name=ssid_key_name]'), function(i) {
        $("<input id='" + $(this).attr('id') + "' name='ssid_key_name' type='" + strType + "' class='input_style' maxlength='"+$(this).attr('maxlength')+"' value='" + $(this).val() + "' />")
            .replaceAll($('#' + $(this).attr('id')));
    });
}

/*-------------------------checkName function---------------------------*/
function wlanbasicsettings_checkName() {
    var name = $.trim($('#ssid_wifiName').val());
    var errMsg = common_ok;
    var i = 0;

    if (0 == name.length)
    {
        errMsg = wlan_hint_ssid_empty;
    }
    else if (hasSpaceOrTabAtHead(name))
    {
        errMsg = input_cannot_begin_with_space;
    }
    else if (32 < name.length)
    {
        errMsg = wizard_help_name_ssid;
    }
    else
    {
        for (i = 0; i < name.length; i++)
        {
            var c = name.charCodeAt(i);
            if (!((c >= CHARCODE_0 && c <= CHARCODE_9) ||
            (c >= CHARCODE_A && c <= CHARCODE_Z) ||
            (c >= CHARCODE_a && c <= CHARCODE_z) ||
            c == CHARCODE_DASH ||
            c == CHARCODE_UNDERLINE ||
            c == CHARCODE_DOT ||
            c == CHARCODE_SPACE
            ))
            {
                errMsg = wlan_hint_ssid_valid_char;
                break;
            }
        }
    }

    if (common_ok != errMsg)
    {
        showErrorUnderTextbox('ssid_wifiName', errMsg);
        $('#ssid_wifiName').focus();
        $('#ssid_wifiName').select();
        return false;
    }
    else
    {
        return true;
    }
}

/*--------------------------check netpwd function--------------------------*/
function wlanbasicsettings_checkNetworkKeyPwd(password) {
    var pwdVal = $('#' + password).val();
    var errMsg = null;
    var ret = false;

    if (0 == pwdVal.length)
    {
        errMsg = dialup_hint_password_empty;
    }
    else if (hasSpaceOrTabAtHead(pwdVal))
    {
        errMsg = input_cannot_begin_with_space;
    }
    else if (10 == pwdVal.length || 26 == pwdVal.length)
    {
        if (!isHexString(pwdVal))
        {
            errMsg = wlan_hint_64_or_128_bit_key;
        }
        else
        {
            ret = true;
        }
    }
    else if (5 == pwdVal.length || 13 == pwdVal.length)
    {
        if (!checkInputChar(pwdVal))
        {
            errMsg = wlan_hint_wep_key_valid_type;
        }
        else
        {
            ret = true;
        }
    }
    else
    {
        errMsg = wlan_hint_64_or_128_bit_key;
    }

    if (!ret)
    {
        showErrorUnderTextbox(password, errMsg);
        $('#' + password).focus();
        $('#' + password).select();
    }

    return ret;
}

/*---------------------------check wap pwd function-----------------------------*/
function wlanbasicsettings_checkWapPwd(password) {
    var pwdVal = $('#' + password).val();
    var errMsg = null;
    var ret = false;

    if (0 == pwdVal.length)
    {
        errMsg = dialup_hint_password_empty;
    }
    else if (hasSpaceOrTabAtHead(pwdVal))
    {
        errMsg = input_cannot_begin_with_space;
    }
    else if (64 == pwdVal.length)
    {
        if (!isHexString(pwdVal))
        {
            errMsg = wlan_hint_wps_psk_valid_type;
        }
        else
        {
            ret = true;
        }
    }
    else if (pwdVal.length >= 8 && pwdVal.length <= 63)
    {
        if (!checkInputChar(pwdVal))
        {
            errMsg = wlan_hint_wps_psk_valid_char;
        }
        else
        {
            ret = true;
        }
    }
    else
    {
        errMsg = wlan_hint_wps_psk_valid_type;
    }

    if (!ret)
    {
        showErrorUnderTextbox(password, errMsg);
        $('#' + password).focus();
        $('#' + password).select();
    }

    return ret;
}

/*---------------------------check wifi security function-----------------------------*/
function wlanbasicsettings_checkWifiSecurity() {
    var strNetworkKey = $('#ssid_current_network_key').val();
    var authMethod = $('#ssid_authentication').val();
    var bscEncptMode = $('#ssid_encryption_mode_basic').val();

    if (authMethod == WIFIAUTHMODE_WPA_PSK ||
    authMethod == WIFIAUTHMODE_WPA2_PSK ||
    authMethod == WIFIAUTHMODE_WPA_WPA2_PSK)
    {
        return wlanbasicsettings_checkWapPwd('ssid_wpa_key');
    }
    else
    {
        if (WIFIBASICENCRYPMODE_NONE != bscEncptMode)
        {
            if ((!wlanbasicsettings_checkNetworkKeyPwd("ssid_neworkKey1"))
                ||(!wlanbasicsettings_checkNetworkKeyPwd("ssid_neworkKey2"))
                ||(!wlanbasicsettings_checkNetworkKeyPwd("ssid_neworkKey3"))
                || (!wlanbasicsettings_checkNetworkKeyPwd("ssid_neworkKey4")))
            {
                return false;
            }
        }
    }

    return true;
}
function parseInt_ex(value,n)
{
    if(isNaN(parseInt(value, n)))
    {
        return 0;
    }
    else
    {
        return parseInt(value, n);    
    }
}

/*---------------------------check wifi Max Associations Limit-----------------------------*/

function wlanbasicsettings_checkmaxassociationslimit()
{
    var isusedwifi = 0;
    var i = 0;
    var wlan_index = parseInt($("#select_show_ssid_index").val(),10);
    var current_WifiMaxAssoc = parseInt_ex($("#input_show_max_limit").val());
    var SSIDEnable = 1;

    if($.trim($("#input_show_max_limit").val())==''||isNaN($("#input_show_max_limit").val()))
    {
        showErrorUnderTextbox('input_show_max_limit', wlanbasicsettings_max_association_check_value);
        $('#input_show_max_limit').focus();
        return false;
    }
    if(current_WifiMaxAssoc==0)
    {
        showErrorUnderTextbox('input_show_max_limit', wlanbasicsettings_max_association_value_invalid);
        $('#input_show_max_limit').focus();
        return false;
    }

    if($("#select_show_ssid_index").val() == "0") 
    {
        SSIDEnable = 1;
    }
    else
    {
        SSIDEnable = $("#check_wifi_enable").get(0).checked ? 1 : 0;
    }
    if (SSIDEnable==0)
    {
        return true;
    }
    getAjaxData('api/wlan/basic-settings',function($xml){
        var ret = xml2object($xml);
        if('response' == ret.type){
            var wlan_basicSetting = ret.response.Ssids.Ssid;         
            if(!$.isArray(wlan_basicSetting)) 
            {
                isusedwifi = 0;                          
            }
            else 
            {
                for (i=0; i<wlan_basicSetting.length; i++)
                {
                    if ( (i!=wlan_index) && (wlan_basicSetting[i].SSIDEnable=='1' || wlan_basicSetting[i].WifiIndex=='1') )
                    {
                        isusedwifi = isusedwifi + parseInt_ex(wlan_basicSetting[i].WifiMaxAssoc,10);
                    }    
                }    
            }
        }
        else
        {
            log.error('Get api/wlan/basic-settings error.');
        }
   },{
        sync : true
   });

   
    if (current_WifiMaxAssoc + isusedwifi > g_WifiMaxAssoc)
    {
        parent.showInfoDialog(wlanbasicsettings_max_association_limit.replace('%d', (g_WifiMaxAssoc-isusedwifi)));
        $('#input_show_max_limit').focus();
        return false;
    }
    
    return true;
}
function getDisplayStatus(element_id)
{
    if($("#"+element_id).css('display')!='none')
    {
        return true;
    }
    else
    {
        return false;       
    }
}

/*------------------------------get data what user set--------------------------------*/
function getDataSet(newBasicSetting) {
    newBasicSetting.WifiCountry = $("#select_WifiCountry").val();
    newBasicSetting.WifiChannel = $("#select_WifiChannel").val();
    newBasicSetting.WifiMode = $("#select_WifiMode").val();
    newBasicSetting.WifiTxPwrPcnt = $("#select_mhz").val();
    newBasicSetting.TxEnhance = $("#select_show_txprower").val();
    newBasicSetting.X_Wlan11NBWControl = $("#select_wlan11NBWControl").val();
    newBasicSetting.Wifi11NGuardInterval = $("#select_interval").val();
    newBasicSetting.WifiSsid = $.trim($("#ssid_wifiName").val());
    if($("#select_show_ssid_index").val() == "0") {
        newBasicSetting.SSIDEnable = 1;
    }
    else {
        newBasicSetting.SSIDEnable = $("#check_wifi_enable").get(0).checked ? 1 : 0;
    }    
    newBasicSetting.WifiWme = $("#select_show_wmm").val();
    if('none' != $(".show_rate_info").css("display"))
    {
        newBasicSetting.WifiRate = $("#select_WifiRate").val();
    }else{
        newBasicSetting.WifiRate = STRING_SET_AUTO;
    }
    newBasicSetting.WifiHide = $("[name='ssid_wifiBroadcast']:checked").val();
    if(getDisplayStatus('div_wpsenble'))
    {
        newBasicSetting.WifiWpsenbl = $("[name='ssid_wpsenble']:checked").val();
    }
    else
    {
        newBasicSetting.WifiWpsenbl = '0';
    }
    newBasicSetting.Wifi11NMcs = $("#select_show_mcs").val();
    newBasicSetting.WifiMaxAssoc = $("#input_show_max_limit").val();
    newBasicSetting.WifiIsolate = $("#ssid_wifiIsolate").val();
    newBasicSetting.WifiAuthmode = $("#ssid_authentication").val();

    switch($("#ssid_authentication").val()){
        case WIFIAUTHMODE_OPEN:
            newBasicSetting.WifiBasicencryptionmodes = $("#ssid_encryption_mode_basic").val();
            newBasicSetting.BasicAuthenticationMode = WIFIBASICENCRYPMODE_NONE;
            if(WIFIBASICENCRYPMODE_WEP == $("#ssid_encryption_mode_basic").val()){
                newBasicSetting.WifiWepKey1 = encrypt($('#ssid_neworkKey1').val());
                newBasicSetting.WifiWepKey2 = encrypt($('#ssid_neworkKey2').val());
                newBasicSetting.WifiWepKey3 = encrypt($('#ssid_neworkKey3').val());
                newBasicSetting.WifiWepKey4 = encrypt($('#ssid_neworkKey4').val());
                newBasicSetting.WifiWepKeyIndex = $('#ssid_current_network_key').val();
            }
            break;
        case WIFIAUTHMODE_SHARE:
            newBasicSetting.WifiBasicencryptionmodes = $("#ssid_encryption_mode_basic").val();
            newBasicSetting.BasicAuthenticationMode = WIFIBASICENCRYPMODE_SHARE;
            if(WIFIBASICENCRYPMODE_WEP == $("#ssid_encryption_mode_basic").val()){
                newBasicSetting.WifiWepKey1 = encrypt($('#ssid_neworkKey1').val());
                newBasicSetting.WifiWepKey2 = encrypt($('#ssid_neworkKey2').val());
                newBasicSetting.WifiWepKey3 = encrypt($('#ssid_neworkKey3').val());
                newBasicSetting.WifiWepKey4 = encrypt($('#ssid_neworkKey4').val());
                newBasicSetting.WifiWepKeyIndex = $('#ssid_current_network_key').val();
            }
            break;
         case WIFIAUTHMODE_WPA_PSK:
            newBasicSetting.WifiWpaencryptionmodes = $("#ssid_encryption_mode_wpa").val();
            newBasicSetting.WifiWpapsk = encrypt($("#ssid_wpa_key").val());
            break;
         case WIFIAUTHMODE_WPA2_PSK:
            newBasicSetting.IEEE11iEncryptionModes = $("#ssid_encryption_mode_wpa").val();
            newBasicSetting.WifiWpapsk = encrypt($("#ssid_wpa_key").val());
            break;
         case WIFIAUTHMODE_WPA_WPA2_PSK:
            newBasicSetting.X_MixedEncryptionModes = $("#ssid_encryption_mode_wpa").val();
            newBasicSetting.WifiWpapsk = encrypt($("#ssid_wpa_key").val());
            break;
    }
    return newBasicSetting;
}

/*---------------------post wlan setting data-------------------------------*/
function postWlanSettingData() {
    var wlan_set = g_wlan_basicSetting[parseInt($("#select_show_ssid_index").val(),10)];
    var data = {};
    var wlan_basic = getDataSet(wlan_set);
        data = {
                Ssid : wlan_basic
                 };
    var req = object2xml("request",data);
    saveAjaxData("api/wlan/basic-settings",req,function($xml) {
        var ret = xml2object($xml);
        if(isAjaxReturnOK(ret)) {
            parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
            button_enable('apply_button', 0);
            log.debug("Save wlan basic setting info successful"); 
            setTimeout(function() {
                wifibasicsettings_initpage();
                parent.closeWaitingDialog();
                parent.showInfoDialog(common_success);
                return false;
            }, 3000);
        }
        else {
            parent.showInfoDialog(common_failed);
            //wifibasicsettings_initpage();
            log.error("Save wlan basic setting info request failed");
        }
    });
}

function showWlanOrBtn(){
    if(Wifienable == 1) {
        //init page
        wifibasicsettings_initpage();
        $("#show_turn_on_wlan").hide();
        $("#show_wlan_settings_info").show();
    }
    else {
        $("#show_turn_on_wlan").show();
        $("#show_wlan_settings_info").hide();
    }
}
function enable_wps_set()
{
    $('#div_wpsenble').hide();
    if($("#select_show_ssid_index").val() == "0" )
    {   
        switch($("#ssid_authentication").val()){
            case WIFIAUTHMODE_WPA2_PSK:
                var IEEE11iEncryptionModes = $("#ssid_encryption_mode_wpa").val();
                if(IEEE11iEncryptionModes==WIFIENCRYPTIONMODE_AES||IEEE11iEncryptionModes==WIFIENCRYPTIONMODE_AES_TKIP)
                {
                    if($("[name='ssid_wifiBroadcast']:checked").val()==1)
                    {
                        $('#div_wpsenble').show(); 
                    }
                }          
                break;
            case WIFIAUTHMODE_WPA_WPA2_PSK:
                var X_MixedEncryptionModes = $("#ssid_encryption_mode_wpa").val();
                if(X_MixedEncryptionModes==WIFIENCRYPTIONMODE_AES||X_MixedEncryptionModes==WIFIENCRYPTIONMODE_AES_TKIP)
                {
                    if($("[name='ssid_wifiBroadcast']:checked").val()==1)
                    {
                        $('#div_wpsenble').show(); 
                    }
                }
                break;
            }          
    }
}
/*------------------------------ready----------------------------------*/
$(document).ready(function() {

    showWlanOrBtn();
    button_enable('apply_button', 0);
    $('#check_wpa_psk').get(0).checked = false;
    
    $('input,select').die().live('keydown click change', function() {
        button_enable('apply_button', '1');
    });
    
    //change ssid inde an show info
    $("#select_show_ssid_index").die().live("change",function() {
        getDifferentData(this.value);
        wlanbasicsettings_showPassword();
        enable_wps_set();
    });
    //show pwd or not
    $("#check_wpa_psk").click(wlanbasicsettings_showPassword);
    //change authentication
    $("#ssid_authentication").die().live("change",function() {
        var basicsetting = g_wlan_basicSetting[parseInt($("#select_show_ssid_index").val(),10)];
        changeAndGetMode(this.value,basicsetting);
        showEncryptionMode($('#select_WifiMode').val());
        enable_wps_set();
    });
    //change Encryption 
    $("#ssid_encryption_mode_wpa").die().live("change",function() {
        enable_wps_set();
    });
      
    $("#ssid_encryption_mode_basic").die().live("change",function() {
        var basicsetting =g_wlan_basicSetting[parseInt($("#select_show_ssid_index").val(),10)];
        wlanbasicsettings_networkKey(this.value,basicsetting);
    });
    
    //change ths wifimode
    $("#select_WifiMode").die().live("change",function() {showRateOr11N(this.value);});
    //change country and show channel
    $("#select_WifiCountry,select_WifiCountry_for_Idevice").die().live("change",function() {
        for (i = 0; i < countryArray.length; i++){
            if (countryArray[i][0] == this.value){
                wlanadvanced_initChannel(countryArray[i][1]);
                break;
            }
        }
        
        check_802ModeWidth_CountryChannel();
    });
    //click apply nad post data
    $("#apply_button").click(function() {
        clearAllErrorLabel();
        if (!isButtonEnable('apply_button')){
            return;
        }
        if(wlanbasicsettings_checkName() && wlanbasicsettings_checkWifiSecurity() && wlanbasicsettings_checkmaxassociationslimit()) {
            if( ( WIFIAUTHMODE_OPEN==$("#ssid_authentication").val()
                ||WIFIAUTHMODE_SHARE==$("#ssid_authentication").val())
                &&( WIFIBASICENCRYPMODE_NONE==$('#ssid_encryption_mode_basic').val() 
                ||WIFIBASICENCRYPMODE_WEP == $("#ssid_encryption_mode_basic").val()))
            {
                parent.showConfirmDialog(label_hint_encryption_mode_not_safe,postWlanSettingData,function(){}); 
            }
            else
            {
                if('none' != $("#div_ssid_encrypt_way2").css("display"))
                {                                      
                    if (checkPasswordStrength($('#ssid_wpa_key').val())<2)
                    {                                    
                        parent.showConfirmDialog(wlan_hint_password_notsafe_wlan,postWlanSettingData,function(){});
                    }
                    else
                    {
                        parent.showConfirmDialog(wlan_hint_disconnect_wlan,postWlanSettingData,function(){}); 
                    }                  
                }
                else
                {
                     parent.showConfirmDialog(wlan_hint_disconnect_wlan,postWlanSettingData,function(){}); 
                }
            }         
        }
        else {
            return;
        }
    });
    
    //click turn on btn
    $("#wlan_turn_on").click(function() {
        Wifienable = 1;
        var ret = {
                Wifienable : Wifienable
            }
        var req = object2xml("request",ret);
        saveAjaxData("api/wlan/wifienable",req,function($xml) {
            var ret = xml2object($xml);
            if(isAjaxReturnOK(ret)) {
                //init page
                button_enable('wlan_turn_on', '0');
                wifibasicsettings_initpage();
                $("#show_turn_on_wlan").hide();
                $("#show_wlan_settings_info").show();
            }
            else {
                return;
            }
        });
    });
    //check 11N Band Width
    $("#select_wlan11NBWControl").die().live("change",function() {Check_11NBandWidth_CountryChannel();});
});
