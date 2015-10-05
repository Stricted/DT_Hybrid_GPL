var g_quicksetup_currentProfileIndex;
var g_quicksetup_profileData;
var g_profile_list = [];
var g_quicksetup_currentProfileData;
var g_quicksetup_wifiBasicData;

var g_quicksetup_connectionData;
var g_quicksetup_connectionDevice;
var g_quicksetup_currentEthIndex;
var g_quicksetup_defaultEthIndex;
var g_quicksetup_ethInfo;
var g_quicksetup_currentEthInfo;
var g_Eth_list = [];
var g_quicksetup_currentConType = '';
var g_quicksetup_currentMTU = '';

var g_AuFeature = null;
var ADD_PROFILE_DISABLE = 0;
var ADD_PROFILE_ENABLE = 1;

var ADD_ETH_DISABLE = 0;
var ADD_ETH_ENABLE = 1;

var MODE_RADIO_ONLY = 1;
var MODE_WAN_ONLY = 2;
var MODE_RADIO_PRIORITY = 3;

var g_add_profile_enable = ADD_PROFILE_DISABLE;
var g_add_eth_enable = ADD_ETH_DISABLE;

var PROFILE_MAX_NUM = 150;

var g_WirelineDisplay = true;

var g_default_wanless_profile = {
    'Index' : 0,
    'IsValid' : 1,
    'Name' : '',
    'ApnIsStatic' : 1,
    'ApnName' : '',
    'DailupNum' : '*99#',
    'Username' : '',
    'Password' : '',
    'AuthMode' : 'AUTO',
    'IpIsStatic' : 0,
    'IpAddress' : '',
    'ConnectMode' : 'OnDemand',
    'MaxIdelTime' : 600,
    'MTU' : 1500,
    'IPv4_Supported' : 1,
    'IPv6_Supported' : 1,
    'ReadOnly' : '0'
};
var g_default_wanline_profile = {
    Index : 0,
    IsValid : 1,
    Name : '',
    MTU : 1492,
    ConnectMode : 'AlwaysOn',
    ConnectionType : 0,
    Username : '',
    Password : '',
    AuthMode: 'AUTO',
    IdAddrType : 'DHCP',
    IdIpv6AddrType : 'SLAAC',
    IPv4_Supported : 1,
    IPv6_Supported : 1,
    ReadOnly : 0,
    DNSServers :'',
    X_IPv6DNSServers: ''
};


var quicksetup_autoAPN = null;
var g_quicksetup_saveDataOK = true;

var QUICKSETUP_WIFIAUTHMODE_AUTO = 'AUTO';
var QUICKSETUP_WIFIAUTHMODE_OPEN = 'None';
var QUICKSETUP_WIFIAUTHMODE_SHARE = 'Basic';
var QUICKSETUP_WIFIAUTHMODE_WPA_PSK = 'WPA';
var QUICKSETUP_WIFIAUTHMODE_WPA2_PSK = '11i';
var QUICKSETUP_WIFIAUTHMODE_WPA_WPA2_PSK = 'WPAand11i';
var QUICKSETUP_WIFIADVENCRYPMODE_AES = 'AESEncryption';
var QUICKSETUP_WIFIADVENCRYPMODE_TKIP = 'TKIPEncryption';
var QUICKSETUP_WIFIADVENCRYPMODE_AES_TKIP= 'TKIPandAESEncryption';
var QUICKSETUP_WIFIBASICENCRYPMODE_NONE = 'None';
var QUICKSETUP_WIFIBASICENCRYPMODE_SHARE = 'SharedAuthentication';
var QUICKSETUP_WIFIBASICENCRYPMODE_WEP = 'WEPEncryption';
var QUICKSETUP_WIFIBASICENCRYPMODE_WEP64 = 'WEP64';
var QUICKSETUP_WIFIBASICENCRYPMODE_WEP128 = 'WEP128';
var QUICKSETUP_WIFIENABLE_STATUS = 0;
var QUICKSETUP_WIFISTATUS_DISABLE = 0;

var QUICKSETUP_AUTHMODE_AUTO = 'AUTO';
var QUICKSETUP_AUTHMODE_PAP = 'PAP';
var QUICKSETUP_AUTHMODE_CHAP = 'CHAP';
var QUICKSETUP_AUTHMODE_NONE = 'NONE';
var QUICKSETUP_DIALUPMODE_AUTO = 'AlwaysOn';
var QUICKSETUP_DIALUPMODE_MANUAL = 'Manual';
var QUICKSETUP_DIALUPMODE_ONDEMAND = 'OnDemand';
var NO_PROFILE_NEW_STATUS = 1;
var PROFILE_NOEDIT_STATUS = 2;
var PROFILE_EDITNAME_STATUS = 3;
var PROFILE_EDITCONTENT_STATUS = 4;
var G_Profile_MTU = 1500;

var QUICKSETUP_CONNECTION_TYPE_PPP = 0;
var QUICKSETUP_CONNECTION_TYPE_IP = 1;
var QUICKSETUP_ADDR_STATIC_DHCP = 'DHCP';
var QUICKSETUP_ADDR_STATIC_STATIC = 'Static';
var QUICKSETUP_ADDR_STATIC_SLAAC = 'SLAAC';

var QUICKSETUP_IPTYPE_IPV4 = 'ipv4';
var QUICKSETUP_IPTYPE_IPV6 = 'ipv6';
var QUICKSETUP_IPTYPE_IPV4IPV6 = 'ipv4ipv6';
var QUICKSETUP_IPTYPE_DISABLE = 0;
var QUICKSETUP_IPTYPE_ENABLE = 1;
var QUICKSETUP_IPTYPE_IPV4VALUE = 0;
var QUICKSETUP_IPTYPE_IPV6VALUE = 0;

var DEFAULT_GATEWAY_IP = '192.168.1.1';
var DEFAULT_GATEWAY_IPv6Address = '';



/*-------------------------init connection device------------------*/
function quicksetup_initpage_connection_device() {
    getAjaxData("api/connection/connection",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            g_quicksetup_connectionDevice = ret.response;
            $("#select_connection_device").val(g_quicksetup_connectionDevice.ConnectionDevice);
            showWanProfile();
        }
        else {
            return;
        }
    });
}

/*----------------------------------init eth info----------------*/
function quickup_initpage_ethInfo() {
    $('#label_eth_mtuormru').text(label_wan_mru);
    var ehtarray = [];
    var index_num = "";
    getAjaxData("api/dialup/ETHs",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
           if(typeof(ret.response.ETHs) != 'undefined'){
                g_quicksetup_defaultEthIndex = ret.response.CurrentETH;
                g_quicksetup_ethInfo = ret.response.ETHs.ETH;
                if(g_quicksetup_ethInfo){
                    if($.isArray(g_quicksetup_ethInfo)) {
                        g_Eth_list= g_quicksetup_ethInfo;
                    }else{
                        g_Eth_list.push(g_quicksetup_ethInfo);
                    }
                 }
               $(g_Eth_list).each(function(k, v){
                   var defaultEthName = v.Name;
                    defaultEthName = resolveXMLEntityReference(defaultEthName);
                    while (defaultEthName.indexOf(' ') >= 0) {    
                        defaultEthName = defaultEthName.replace(' ', '&nbsp;');
                    }
                    if (v.Index == g_quicksetup_defaultEthIndex)
                    {
                        g_quicksetup_currentEthInfo= v;
                    }
                   $('#select_eth_name').append('<option value=' + v.Index + '>' + defaultEthName + '</option>');
               });
               if (g_Eth_list.length > 0 && g_quicksetup_defaultEthIndex >= 0)
               {
                   if(g_quicksetup_currentEthInfo.ConnectionType == 0 && g_Eth_list.length < 4){
                        if($("#select_eth_name option[value= "+0 +"]").length == 0) {
                            $("#select_eth_name").append('<option style="font-style: italic; color:gray;" value='+ 0 +'>' + dialup_button_newprofile + '</option>');
                        }
                   }
                   quicksetup_setETHInfo(g_quicksetup_defaultEthIndex);
                   $('#select_eth_name').change(function()
                   {
                       quicksetup_setETHInfo(this.value);
                   });
               }else {
                    g_add_eth_enable = ADD_ETH_ENABLE;
                    $('#select_eth_name').append('<option style="font-style: italic; color:gray;" value=' + 0 + '>' + dialup_button_newprofile + '</option>');
                    $('#wireline_profile_tr').show();
                    $('#select_wan_iptype').removeAttr('disabled');
                    $('#eth_user_name').removeAttr('disabled');
                    $('#eth_password').removeAttr('disabled');
                    $(".profile_input,.input_select").bind('change input paste cut keydown', function() {
                        button_enable('step2_next', '1');
                    });
                }
           }else{
            log.error("the ETHs.xml'ETHs node is null or error!");
           }
        }
        button_enable('step2_back', '1');
        button_enable('step2_next', '1');
    });
}

/*----------------------set eth info---------------------------*/
function quicksetup_setETHInfo(eth_index) {
    if(eth_index == 0){
        g_quicksetup_currentEthInfo = g_default_wanline_profile;
        $('#wireline_profile_tr').show();
        g_quicksetup_currentEthIndex = 0;
    }else{
        $('#wireline_profile_tr').hide();
        $.each(g_Eth_list, function(ind, current_Ethprofile) {
            if (current_Ethprofile.Index == eth_index)
            {
                g_quicksetup_currentEthIndex = ind;
                g_quicksetup_currentEthInfo= current_Ethprofile;
                return;
            }
        });
    }
    g_quicksetup_currentConType = g_quicksetup_currentEthInfo.ConnectionType;
    g_quicksetup_currentMTU = g_quicksetup_currentEthInfo.MTU;

    if('' !=  g_quicksetup_currentEthInfo.AuthMode){
        $("#eth_authentication_mode").val(g_quicksetup_currentEthInfo.AuthMode);
    }
    if('' !=  g_quicksetup_currentEthInfo.ConnectMode){
         $("#select_dialupmode").val(g_quicksetup_currentEthInfo.ConnectMode);
    }
    if('' !=  g_quicksetup_currentEthInfo.IdAddrType){
         $("#select_ipv4_address_type").val(g_quicksetup_currentEthInfo.IdAddrType);
    }
    if('' !=  g_quicksetup_currentEthInfo.IdIpv6AddrType){
         $("#select_ipv6_address_type").val(g_quicksetup_currentEthInfo.IdIpv6AddrType);
    }
		
    $('#select_eth_name').val(g_quicksetup_currentEthInfo.Index);
    $("#input_eth_name").val(g_quicksetup_currentEthInfo.Name);
    $("#select_connection_type").val(g_quicksetup_currentEthInfo.ConnectionType);
    $('#select_wan_iptype').val(checkIpv4v6Type(g_quicksetup_currentEthInfo.IPv4_Supported,g_quicksetup_currentEthInfo.IPv6_Supported));
    $("#input_maxideltime").val(g_quicksetup_currentEthInfo.ConnectMode == QUICKSETUP_DIALUPMODE_ONDEMAND ? g_quicksetup_currentEthInfo.MaxIdelTime : 300);
    $("#eth_user_name").val(g_quicksetup_currentEthInfo.Username);
    $("#eth_password").val(g_quicksetup_currentEthInfo.Password == '' ? '' :  decrypt(g_quicksetup_currentEthInfo.Password));
    $("#input_ipv4_address").val(g_quicksetup_currentEthInfo.IdIpAddr);
    $("#input_subnet_mask").val(g_quicksetup_currentEthInfo.IdSubMask);
    $("#input_default_gateway").val(g_quicksetup_currentEthInfo.IdGateWay);
    $("#input_primary_DNS").val(getDNSServer(g_quicksetup_currentEthInfo.DNSServers,0));
    $("#input_secondary_DNS").val(getDNSServer(g_quicksetup_currentEthInfo.DNSServers,1));
    $("#input_ipv6_address").val(g_quicksetup_currentEthInfo.IdIpv6Addr);
    $("#input_ipv6_default_gateway").val(g_quicksetup_currentEthInfo.IdIpv6Gateway);
    $("#input_ipv6_IdIpv6PrefixLen").val(g_quicksetup_currentEthInfo.IdIpv6PrefixLen);
    $("#input_ipv6_primary_DNS").val(getDNSServer(g_quicksetup_currentEthInfo.X_IPv6DNSServers,0));
    $("#input_ipv6_secondary_DNS").val(getDNSServer(g_quicksetup_currentEthInfo.X_IPv6DNSServers,1));
    if (g_quicksetup_currentEthInfo.ReadOnly == 0)
    {
        $('#eth_authentication_mode').removeAttr('disabled');
        $('#select_wan_iptype').removeAttr('disabled');
        $('#eth_user_name').removeAttr('disabled');
        $('#eth_password').removeAttr('disabled');
    }
    else
    {
        $('#eth_authentication_mode').attr('disabled', 'disabled');
        $('#select_wan_iptype').attr('disabled', 'disabled');
        $('#eth_user_name').attr('disabled', 'disabled');
        $('#eth_password').attr('disabled', 'disabled');
    }
    checkConnectiontype();
    $("#input_mrt_mtu").val(g_quicksetup_currentEthInfo.MTU);
}

function checkConnectiontype(){
    //checkConnModeStatus($('#select_dialupmode').val());
    if(QUICKSETUP_CONNECTION_TYPE_PPP == $("#select_connection_type").val()){
        $('.Connection_Type_PPPClass').show();
        $('#IPv4_AddrType_tr').hide();
        $('#label_eth_mtuormru').text(label_wan_mru);
        $('#input_mrt_mtu').val(1492);
        if(4 == g_Eth_list.length){
            $('#input_eth_name').attr('disabled','disabled');
        } 
        else
        {
            g_add_eth_enable = ADD_ETH_ENABLE;
            $('#input_eth_name').removeAttr('disabled');
        }
    }else if(QUICKSETUP_CONNECTION_TYPE_IP == $("#select_connection_type").val()){
        $('.Connection_Type_PPPClass').hide();
        $('#IPv4_AddrType_tr').show();
        $('#label_eth_mtuormru').text(wan_mtu);
        $('#input_mrt_mtu').val(1500); 
        $('#input_eth_name').removeAttr('disabled');
    }
    if('' != g_quicksetup_currentConType){
        if(g_quicksetup_currentConType != $("#select_connection_type").val()){
            if(QUICKSETUP_CONNECTION_TYPE_PPP == $("#select_connection_type").val()){
                $('#input_mrt_mtu').val(1492);
            }else{
                $('#input_mrt_mtu').val(1500);
            }
        }else{
                $('#input_mrt_mtu').val(g_quicksetup_currentMTU);
        }
    }
    //check_IPType_ShowStatus();
}

function check_IPType_ShowStatus(){
    if(QUICKSETUP_CONNECTION_TYPE_PPP == $("#select_connection_type").val()){
        if(QUICKSETUP_IPTYPE_IPV4 == $('#select_wan_iptype').val()){
            $('#IPv64_AddrType_tr').hide();
        }else if((QUICKSETUP_IPTYPE_IPV6 == $('#select_wan_iptype').val())||(QUICKSETUP_IPTYPE_IPV4IPV6 == $('#select_wan_iptype').val())){
            $('#IPv64_AddrType_tr').show();
        }
    }else{
        if(QUICKSETUP_IPTYPE_IPV4 == $('#select_wan_iptype').val()){
            $('#IPv4_AddrType_tr').show();
            $('#IPv64_AddrType_tr').hide();
        }else if(QUICKSETUP_IPTYPE_IPV6== $('#select_wan_iptype').val()){
            $('#IPv64_AddrType_tr').show();
            $('#IPv4_AddrType_tr').hide();
        }else if(QUICKSETUP_IPTYPE_IPV4IPV6== $('#select_wan_iptype').val()){
            $('#IPv4_AddrType_tr').show();
            $('#IPv64_AddrType_tr').show();
        }
    }
    checkIPv4AddrType($('#select_ipv4_address_type').val());
    checkIPv6AddrType($('#select_ipv6_address_type').val());
}

function initCurrentProfile(index)
{
    if(index == 0){
        g_quicksetup_currentProfileData = g_default_wanless_profile;
        $('#wireless_profile_name').show();

    }else{
        $.each(g_profile_list, function(ind, current_profile) {
            if (current_profile.Index == index)
            {
                g_quicksetup_currentProfileData = current_profile;
                return;
            }
        });
        $('#wireless_profile_name').hide();
    }
    $('#profileName_List').val(g_quicksetup_currentProfileData.Index);
    $("#profileName_List option[text='g_quicksetup_currentProfileData.Name']").attr('selected', true);
    $('#profileName').val(g_quicksetup_currentProfileData.Name);
    $('#apn').val(g_quicksetup_currentProfileData.ApnName);
    if(g_quicksetup_currentProfileData.ApnName!='')
    {
        $('input[name=auto_apn][value=' + '1' + ']').attr('checked', true);
    }
    else
    {
        $('input[name=auto_apn][value=' + '0' + ']').attr('checked', true); 
    } 

    if (g_quicksetup_currentProfileData.ReadOnly == "1")
    {
        set_apn_IsStatic();
        $("[name='auto_apn']").attr('disabled', 'disabled'); 
        $('#apn').attr('disabled', 'disabled');
    }else{
        $("[name='auto_apn']").removeAttr('disabled');
        $('#apn').removeAttr('disabled');
        set_apn_IsStatic();
    }

}
            
function quicksetup_initPage_profile()
{
    getAjaxData('api/dialup/profiles', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response')
        {
            //bind item for profile
            if(typeof(ret.response.Profiles) != 'undefined'){
                g_quicksetup_currentProfileIndex = ret.response.CurrentProfile;
                g_quicksetup_profileData = ret.response.Profiles.Profile;
    
                if (g_quicksetup_profileData)
                {
                    if ($.isArray(g_quicksetup_profileData))
                    {
                        g_profile_list = g_quicksetup_profileData;
                    }
                    else
                    {
                        g_profile_list.push(g_quicksetup_profileData);
                    }
                }
    
                $(g_profile_list).each(function(k, v)
                {
                    var defaultName = v.Name;
                    defaultName = resolveXMLEntityReference(defaultName);
                    while (defaultName.indexOf(' ') >= 0) {    
                        defaultName = defaultName.replace(' ', '&nbsp;');
                    }
                    $('#profileName_List').append('<option value=' + v.Index + '>' + defaultName + '</option>');
                });
                $('#profileName_List').append('<option style="font-style: italic; color:gray;" value=' + 0 + '>' + dialup_button_newprofile + '</option>');

                if (g_profile_list.length > 0 && g_quicksetup_currentProfileIndex >= 0)
                {
                    initCurrentProfile(g_quicksetup_currentProfileIndex);
                    
                    setTimeout(function(){
                        set_apn_IsStatic();
                        if (g_quicksetup_currentProfileData.ReadOnly == "1")
                        {
                            $("[name='auto_apn']").attr('disabled', 'disabled'); 
                            $('#apn').attr('disabled', 'disabled');
                        }
                    }, 900);

                    $('#profileName_List').change(function()
                    {
                        initCurrentProfile(this.value);
                    });
                }
                else
                {
                    g_quicksetup_currentProfileData = g_default_wanless_profile;
                    $('#wireless_profile_name').show();
                    g_add_profile_enable = ADD_PROFILE_ENABLE;
                    $('input[name=auto_apn][value=' + '0' + ']').attr('checked', true); 
                     setTimeout(function(){     
                         $('#apn').attr('disabled', 'disabled');
                     }, 800);
                }
                $(".profile_input,.input_select").bind('change input paste cut keydown', function() {
                    button_enable('step2_next', '1');
                });
            }else{
                log.error("the profiles.xml'Profiles node is null or error!");
            }
           

        }
        button_enable('step2_back', '1');
        button_enable('step2_next', '1');
    });
}

function quicksetup_initPage_autoApn()
{
    getAjaxData('api/dialup/auto-apn', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response')
        {
            quicksetup_autoAPN = parseInt(ret.response.AutoAPN, 10);
        }
        if (0 == quicksetup_autoAPN)
        {
            $('#checkbox_profile_autoapn').get(0).checked = false;
        }
        else
        {
            quicksetup_autoAPN = 1;
            $('#checkbox_profile_autoapn').get(0).checked = true;
        }
        quicksetup_initPage_profile();
    });
}

/*-------------------get WFI Enable----------------------------*/
function quicksetup_initPage_wifiEnable(){
    getAjaxData('api/wlan/wifienable', function($xml) {
        var ret = xml2object($xml);
        if('response' == ret.type){
            QUICKSETUP_WIFIENABLE_STATUS = ret.response.Wifienable;
        }
    },{sync: true});
}

/*---------------init wifi page------------------------------*/
function quicksetup_initPage_wifi() {
    getAjaxData("api/wlan/basic-settings",function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            if(typeof(ret.response.Ssids) != 'undefined'){
                g_quicksetup_wifiBasicData = ret.response.Ssids.Ssid;
                if($.isArray(g_quicksetup_wifiBasicData)) {
                    $("#ssid_authentication").val(g_quicksetup_wifiBasicData[0].WifiAuthmode);
                    getModeStatus(g_quicksetup_wifiBasicData[0].WifiMode);
                    showChangeAuthMode(g_quicksetup_wifiBasicData[0]);
                    $("#ssid_wifiName").val(g_quicksetup_wifiBasicData[0].WifiSsid);
                    $("#ssid_broadcast_select").val(g_quicksetup_wifiBasicData[0].WifiHide);
                    $("#ssid_encryption_mode_basic").val(g_quicksetup_wifiBasicData[0].WifiBasicencryptionmodes);
                    $("#ssid_neworkKey1").val(g_quicksetup_wifiBasicData[0].WifiWepKey1 == '' ? '' : decrypt(g_quicksetup_wifiBasicData[0].WifiWepKey1));
                    $("#ssid_wpa_key").val(g_quicksetup_wifiBasicData[0].WifiWpapsk == '' ? '' : decrypt(g_quicksetup_wifiBasicData[0].WifiWpapsk));
                    quicksetup_authentication(g_quicksetup_wifiBasicData[0].WifiAuthmode, 'ssid');
                }
                else if(typeof(ret.response.Ssids.Ssid) == "undefined") {
                    return;
                }
                else {
                    $("#ssid_authentication").val(g_quicksetup_wifiBasicData.WifiAuthmode);
                    getModeStatus(g_quicksetup_wifiBasicData.WifiMode);
                    showChangeAuthMode(g_quicksetup_wifiBasicData);
                    $("#ssid_wifiName").val(g_quicksetup_wifiBasicData.WifiSsid);
                    $("#ssid_broadcast_select").val(g_quicksetup_wifiBasicData.WifiHide);
                    $("#ssid_encryption_mode_basic").val(g_quicksetup_wifiBasicData.WifiBasicencryptionmodes);
                    $("#ssid_neworkKey1").val(g_quicksetup_wifiBasicData.WifiWepKey1 == '' ? '' : decrypt(g_quicksetup_wifiBasicData.WifiWepKey1));
                    $("#ssid_wpa_key").val(g_quicksetup_wifiBasicData.WifiWpapsk == '' ? '' : decrypt(g_quicksetup_wifiBasicData.WifiWpapsk));
                    quicksetup_authentication(g_quicksetup_wifiBasicData.WifiAuthmode, 'ssid');
                }
            }
        }
    });
}

/***********************show 802.11n status****************************/
function getModeStatus(mode){
    if(mode == "n" || mode == "b/g/n") {
        if(mode == "n") {
            $("#ssid_authentication option[value="+ QUICKSETUP_WIFIAUTHMODE_SHARE +"]").remove();
            if(QUICKSETUP_WIFIAUTHMODE_OPEN == $('#ssid_authentication').val()){
                $("#ssid_encryption_mode_basic option[value="+ QUICKSETUP_WIFIBASICENCRYPMODE_WEP +"]").remove();
                if(0 == $('#ssid_encryption_mode_basic option[value='+QUICKSETUP_WIFIBASICENCRYPMODE_NONE+']').length){
                    $('#ssid_encryption_mode_basic').prepend("<option value="+ QUICKSETUP_WIFIBASICENCRYPMODE_NONE +">" + wlan_label_none + '</option>');
                }
                $('#ssid_network_key').hide();
            }
        }
        else {
            if($("#ssid_authentication option[value="+ QUICKSETUP_WIFIAUTHMODE_SHARE +"]").length == 0) {
                $("#ssid_authentication").prepend("<option value="+ QUICKSETUP_WIFIAUTHMODE_SHARE +">" + wlan_label_share + '</option>');
            }
        
            if($("#ssid_encryption_mode_basic option[value= "+QUICKSETUP_WIFIBASICENCRYPMODE_WEP +"]").length == 0) {
                $("#ssid_encryption_mode_basic").prepend("<option value="+ QUICKSETUP_WIFIBASICENCRYPMODE_WEP +">" + wlan_label_wep + '</option>');
            }
        }
    }
    else {
        if($("#ssid_authentication option[value="+ QUICKSETUP_WIFIAUTHMODE_SHARE +"]").length == 0) {
            $("#ssid_authentication").prepend("<option value="+ QUICKSETUP_WIFIAUTHMODE_SHARE +">" + wlan_label_share + '</option>');
        }
        
        if($("#ssid_encryption_mode_basic option[value="+ QUICKSETUP_WIFIBASICENCRYPMODE_WEP +"]").length == 0) {
            $("#ssid_encryption_mode_basic").prepend("<option value="+ QUICKSETUP_WIFIBASICENCRYPMODE_WEP +">" + wlan_label_wep + '</option>');
        }
    }
    
    if('n' != mode){
        if(0 == $('#ssid_encryption_mode_wpa option[value=' + QUICKSETUP_WIFIADVENCRYPMODE_TKIP + ']').length){
            $('#ssid_encryption_mode_wpa').append('<option value = ' + QUICKSETUP_WIFIADVENCRYPMODE_TKIP + '>' + wlan_label_tkip + '</option>');
        } 
        if(0 == $('#ssid_encryption_mode_wpa option[value=' + QUICKSETUP_WIFIADVENCRYPMODE_AES_TKIP + ']').length){
            $('#ssid_encryption_mode_wpa').append('<option value = ' + QUICKSETUP_WIFIADVENCRYPMODE_AES_TKIP + '>' + wlan_label_aes_tkip+ '</option>');
        }
    }else{
        if((QUICKSETUP_WIFIAUTHMODE_WPA_PSK == $('#ssid_authentication').val()) ||
            (QUICKSETUP_WIFIAUTHMODE_WPA2_PSK == $('#ssid_authentication').val()) ||
            (QUICKSETUP_WIFIAUTHMODE_WPA_WPA2_PSK == $('#ssid_authentication').val())){
                $('#ssid_encryption_mode_wpa option[value=' + QUICKSETUP_WIFIADVENCRYPMODE_TKIP + ']').remove();
                $('#ssid_encryption_mode_wpa option[value=' + QUICKSETUP_WIFIADVENCRYPMODE_AES_TKIP + ']').remove();
        }
    }

}

/*--------------------------------show key------------------------------*/
function getDefferentKey(key) {
    if (key == QUICKSETUP_WIFIBASICENCRYPMODE_NONE)
    {
        $('#ssid_network_key').hide();
    }
    else if (key == QUICKSETUP_WIFIBASICENCRYPMODE_WEP)
    {
        $('#ssid_network_key').show();
    }
}

/*-----------------------------wifiAuthMode change ---------------------*/
function showChangeAuthMode(wifiBasicData) {
    
    if (wifiBasicData.WifiAuthmode == QUICKSETUP_WIFIAUTHMODE_OPEN ||
        wifiBasicData.WifiAuthmode == QUICKSETUP_WIFIAUTHMODE_SHARE)
    {
        $('#div_ssid_encrypt_way1').show();
        $('#div_ssid_encrypt_way2').hide();

        if (QUICKSETUP_WIFIAUTHMODE_SHARE == wifiBasicData.WifiAuthmode)
        {
            $("#ssid_encryption_mode_basic option[value="+ QUICKSETUP_WIFIBASICENCRYPMODE_NONE +"]").remove();
        }
        else
        {
            if ($("#ssid_encryption_mode_basic option[value="+ QUICKSETUP_WIFIBASICENCRYPMODE_NONE +"]").length == 0)
            {
                 $('#ssid_encryption_mode_basic').prepend("<option value="+ QUICKSETUP_WIFIBASICENCRYPMODE_NONE +">" + wlan_label_none + '</option>');
            }
         }
         getDefferentKey(wifiBasicData.WifiBasicencryptionmodes);
    }
    else
    {
        $('#div_ssid_encrypt_way2').show();
        $('#div_ssid_encrypt_way1').hide();
    }
}

function quicksetup_initPageData() {
    quicksetup_initpage_connection_device();
    if (g_module.autoapn_enabled)
    {
        quicksetup_initPage_autoApn();
    }
    else
    {
       $('#tr_profile_autoapn').remove();
       $('#auto_apn_finish').remove();
       quicksetup_initPage_profile();
    }
    
    setTimeout(quickup_initpage_ethInfo, 500);
}

function quicksetup_onChangeAutoAPN()
{
    button_enable('step2_back', '0');
    button_enable('step2_next', '0');
    quicksetup_autoAPN = $('#checkbox_profile_autoapn').get(0).checked;
    var request = {
        AutoAPN: quicksetup_autoAPN ? 1 : 0
    };
    var xmlstr = object2xml('request', request);
    saveAjaxData('api/dialup/auto-apn', xmlstr, function($xml) {
        quicksetup_initPage_autoApn();
    });
}

function quicksetup_authentication(mode, ssid) {
    
    if (mode == QUICKSETUP_WIFIAUTHMODE_OPEN ||
    mode == QUICKSETUP_WIFIAUTHMODE_SHARE)
    {
        $('#div_' + ssid + '_encrypt_way1').show();
        $('#div_' + ssid + '_encrypt_way2').hide();

        if (QUICKSETUP_WIFIAUTHMODE_SHARE == mode)
        {
            $('#' + ssid + "_encryption_mode_basic option[value="+QUICKSETUP_WIFIBASICENCRYPMODE_NONE+"]").remove();
            $('#' + ssid + '_encryption_mode_basic').val(QUICKSETUP_WIFIBASICENCRYPMODE_WEP);
        }
        else
        {
            if ($('#' + ssid + "_encryption_mode_basic option[value="+QUICKSETUP_WIFIBASICENCRYPMODE_NONE+"]").length == 0)
            {
                $('#' + ssid + '_encryption_mode_basic').prepend("<option value="+QUICKSETUP_WIFIBASICENCRYPMODE_NONE+">" + wlan_label_none + '</option>');
            }
        }

            getDefferentKey($('#ssid_encryption_mode_basic').val());
    }
    else{
        $('#checkbox_password').show();
        $('#div_' + ssid + '_encrypt_way2').show();
        $('#div_' + ssid + '_encrypt_way1').hide();
        $('#' + ssid + '_encryption_mode_wpa').val(QUICKSETUP_WIFIADVENCRYPMODE_AES_TKIP);
        
        if(QUICKSETUP_WIFIAUTHMODE_WPA_PSK == mode){
                $("#ssid_encryption_mode_wpa").val(g_quicksetup_wifiBasicData[0].WifiWpaencryptionmodes);
        }
        else if(QUICKSETUP_WIFIAUTHMODE_WPA2_PSK == mode){
                $("#ssid_encryption_mode_wpa").val(g_quicksetup_wifiBasicData[0].IEEE11iEncryptionModes);
        }
        else if(QUICKSETUP_WIFIAUTHMODE_WPA_WPA2_PSK == mode){
                $("#ssid_encryption_mode_wpa").val(g_quicksetup_wifiBasicData[0].X_MixedEncryptionModes);
        }

    }
}

/*-------------------------validate eth data--------------------------*/
function quicksetup_validateETHsData() {
    var username = $.trim($("#eth_user_name").val());
    var password = $("#eth_password").val();
    var ethname = $.trim($('#input_eth_name').val());
    var conntype = $('#select_connection_type').val();
    
    // check profile name
    if(!checkNameOrPassword('input_eth_name')){
        return false;
    }
    
    if((1<g_Eth_list.length) && (QUICKSETUP_CONNECTION_TYPE_PPP != $('#select_connection_type').val())){
        showErrorUnderTextbox('eth_set_info', hint_ethprofile_edit_invalid);
        $('#select_connection_type').focus();
        return false;
    }
    if (0 == $('#select_eth_name').val()) {
        for(var i =0; i< g_Eth_list.length; i++){
            if(g_Eth_list[i].Name == ethname){
                showErrorUnderTextbox('input_eth_name', dialup_hint_profile_name_has_exist);
                $('#input_eth_name').focus();
                $('#input_eth_name').select();
                return false;
            }
        }
    }
    
    // check username
    if(QUICKSETUP_CONNECTION_TYPE_PPP == conntype){
            if('' != username && !checkNameOrPassword('eth_user_name')){
                return false;
            }
            if('' == username)
            {
                showErrorUnderTextbox('eth_user_name', wan_valid_username);
                $('#eth_user_name').focus();
                return false;
            }
            // check password
            if('' != password && !checkNameOrPassword('eth_password')){
                return false;
            }           
            if('' == password)
            {
                showErrorUnderTextbox('eth_password', wan_valid_username);
                $('#eth_password').focus();
                return false;
            }
        }
    return true;
}

function quicksetup_validateSsid(ssid) {
    var name = $.trim($('#' + ssid + '_wifiName').val());
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
            c == CHARCODE_SPACE)
            )
            {
                errMsg = wlan_hint_ssid_valid_char;
                break;
            }
        }
    }

    if (common_ok != errMsg)
    {
        showErrorUnderTextbox(ssid + '_wifiName', errMsg);
        $('#' + ssid + '_wifiName').focus();
        $('#' + ssid + '_wifiName').select();
        return false;
    }
    else
    {
        return true;
    }
}

function quicksetup_validateProfile() {
    var profileName = $.trim($('#profileName').val());

    // check profile name
    if(0 == profileName.length)
    {
        showErrorUnderTextbox('profileName', IDS_dialup_hint_profile_name_null);
        $('#profileName').focus();
        return false;
    }
    if (profileName.length > 64 || profileName.length < 1)
    {
        showErrorUnderTextbox('profileName', dialup_hint_profile_name_valid_char);
        $('#profileName').focus();
        $('#profileName').select();
        return false;
    }
    if ('' == $.trim(profileName)) {
        showErrorUnderTextbox('profileName', samba_input_empty);
        $('#profileName').focus();
        $('#profileName').select();
        return false;
    }
    if (!checkInputChar_wanName(profileName))
    {
        showErrorUnderTextbox('profileName', wan_valid_username);
        $('#profileName').focus();
        $('#profileName').select();
        return false;
    }
    var flag = false;
    if (0 == g_quicksetup_currentProfileData.Index) {
        $(g_profile_list).each(function(k, v){
            var newprofile = profileName;
            if(v.Name == newprofile){
                flag = true;
            }
        });
    }
    if(flag == true){
        showErrorUnderTextbox('profileName', dialup_hint_profile_name_has_exist);
        $('#profileName').focus();
        $('#profileName').select();
        return false;
    }
    
    var apn = $.trim($('#apn').val());
    // check apn
    if (apn.length != 0)
    {
        if (!checkValidApn(apn))
        {
            showErrorUnderTextbox('apn', dialup_hint_apn_valid_check);
            $('#apn').focus();
            $('#apn').select();
            return false;
        }
    }
    return true;
}

function quicksetup_validateNeworkKeyPwd(password) {
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

function quicksetup_validateWepPwd(password) {
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
            errMsg = wlan_hint_wps_psk_valid_char;
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

function quicksetup_ValidateWifiSecurity(ssid) {
    var ifw = $('#' + ssid + '_authentication').val();
    if (ifw == QUICKSETUP_WIFIAUTHMODE_WPA_PSK ||
    ifw == QUICKSETUP_WIFIAUTHMODE_WPA2_PSK ||
    ifw == QUICKSETUP_WIFIAUTHMODE_WPA_WPA2_PSK)
    {
        if (!quicksetup_validateWepPwd(ssid + '_wpa_key'))
        {
            return false;
        }
    }
    else if (ifw == QUICKSETUP_WIFIAUTHMODE_OPEN ||
    ifw == QUICKSETUP_WIFIAUTHMODE_SHARE)
    {
        if ($('#' + ssid + '_encryption_mode_basic').val() == QUICKSETUP_WIFIBASICENCRYPMODE_NONE)
        {
            return true;
        }
        else
        {
            if (!quicksetup_validateNeworkKeyPwd(ssid + '_neworkKey1'))
            {
                return false;
            }
        }
    }

    return true;
}

function quicksetup_showStepLast() {
    if (g_module.autoapn_enabled)
    {
        $('#label_auto_apn').html((1 == quicksetup_autoAPN) ? common_enable : common_disable);
    }

    $('#label_apn').html(('' == $('#apn').val())?dialup_label_dynamic_numeric:$('#apn').val());
    
    $('#label_profile_name').html('<pre>' + resolveXMLEntityReference($('#profileName').val()) + '</pre>');
    
    //show eth lable info
    $("#eth_lable_name").html('<pre>' + resolveXMLEntityReference($("#input_eth_name").val()) + '</pre>');
    $('#eth_lable_conntype').html($('#select_connection_type option:selected').text());
    $('#eth_lable_mtuormru').html($('#select_connection_type').val()==QUICKSETUP_CONNECTION_TYPE_PPP?label_wan_mru:wan_mtu);
    $('#eth_lable_mtuormru_value').html($('#input_mrt_mtu').val());
    if(QUICKSETUP_CONNECTION_TYPE_PPP == $('#select_connection_type').val()){
        $('#eth_lable_user_name_ex').show();
        $('#eth_show_info_ipv4_type').hide();
        $('#eth_lable_authentication_mode').html($("#eth_authentication_mode option:selected").text());
        $('#eth_lable_connmode').html($('#select_dialupmode option:selected').text());
        $("#eth_lable_user_name").html('<pre>' + resolveXMLEntityReference($("#eth_user_name").val()) + '</pre>');
    }else if(QUICKSETUP_CONNECTION_TYPE_IP == $('#select_connection_type').val()){
        $('#eth_lable_addrtype').html($('#select_ipv4_address_type option:selected').text());
        $('#eth_lable_authentication_mode_ex').hide();
        $('#eth_lable_connmode_ex').hide();
        $('#eth_lable_user_name_ex').hide();
    }


    if(QUICKSETUP_WIFISTATUS_DISABLE != QUICKSETUP_WIFIENABLE_STATUS){
            //set ssid info
            $("#ssid_label_name").html($("#ssid_wifiName").val());
            $("#ssid_label_authentication").html($("#ssid_authentication option:selected").text());
            $("#ssid_label_ssid_broadcast").html($("#ssid_broadcast_select option:selected").text());
            if((QUICKSETUP_WIFIAUTHMODE_OPEN == $('#ssid_authentication').val()) || 
                ( QUICKSETUP_WIFIAUTHMODE_SHARE== $('#ssid_authentication').val())){
                $("#ssid_label_encryption_mode").html($("#ssid_encryption_mode_basic option:selected").text());
            }
            else{
                $("#ssid_label_encryption_mode").html($("#ssid_encryption_mode_wpa option:selected").text());
            }
        }else{
             $('#wlan_content_status').hide();
        }
}

function quicksetup_settings(step) {
    //startLogoutTimer();
    clearAllErrorLabel();
    var isValid = true;
    var i = 0;
    var ssids = ['ssid'];

    if (step == 0)
    {
        isValid = true;
    }
    else if (step == 1)
    {
        isValid = true;
        if((($("#select_connection_device").val() == MODE_RADIO_ONLY) &&('' == $('#profileName').val()))|| 
            (($("#select_connection_device").val() == MODE_WAN_ONLY) && ("" == $("#input_eth_name").val()))|| 
            ($("#select_connection_device").val() == MODE_RADIO_PRIORITY && ('' == $('#profileName').val() || "" == $("#input_eth_name").val())))
        {
            button_enable('step2_next', '0');
        }
        else{
            button_enable('step2_next', '1');
        } 
    }else if (step == 2)
    {
        if (g_module.ap_station_enabled)
        {
            if ((g_module.autoapn_enabled && 1 == quicksetup_autoAPN) ||
            (quicksetup_validateProfile() && $("#select_connection_device").val() == MODE_RADIO_ONLY))
            {
                isValid = true;
            }
            else if((g_module.autoapn_enabled && 1 == quicksetup_autoAPN) || (quicksetup_validateETHsData() && $("#select_connection_device").val() == MODE_WAN_ONLY)) {
                isValid = true;
            }
            else if((g_module.autoapn_enabled && 1 == quicksetup_autoAPN) || (quicksetup_validateETHsData() && quicksetup_validateProfile() && $("#select_connection_device").val() == MODE_RADIO_PRIORITY)) {
                isValid = true;
            }
            else
            {
                isValid = false;
                return;
            }
        }
        else{
            if((MODE_RADIO_ONLY == $("#select_connection_device").val()) && quicksetup_validateProfile() ){
                isValid = true;
            }else if((MODE_WAN_ONLY == $("#select_connection_device").val()) && quicksetup_validateETHsData()){
                isValid = true;
            }else{
                isValid = false;
                return;
            }
        }
    }
    else if (step == 3)
    {
        isValid = true;
        ssids = ['ssid'];
        for (i = 0; i < ssids.length; ++i)
        {
            if (!quicksetup_validateSsid(ssids[i]))
            {
                isValid = false;
                return;
            }
        }

    }
    else if (step == 4)
    {
        if(QUICKSETUP_WIFISTATUS_DISABLE != QUICKSETUP_WIFIENABLE_STATUS){
        
            if($('#wifi_config_enable').attr('checked'))
            {
                ssids = ['ssid'];
                for (i = 0; i < ssids.length; ++i){
                    if (!quicksetup_ValidateWifiSecurity(ssids[i])){
                    isValid = false;
                    return;
                    }
                }  
            }

        if (!g_module.multi_ssid_enabled){
            var auth = $('#ssid_authentication').val();
            if (QUICKSETUP_WIFIAUTHMODE_OPEN == auth ||QUICKSETUP_WIFIAUTHMODE_SHARE == auth){
                if ($('#ssid_encryption_mode_basic').val() == QUICKSETUP_WIFIAUTHMODE_AUTO){
                    showConfirmDialog(wlan_hint_use_encryption, quicksetup_showStepLast);
                    return;
                }else{
                    quicksetup_showStepLast();
                }
            }else{
                quicksetup_showStepLast();
            }
        }else{
            var auth1 = $('#ssid_authentication').val();
            if ((QUICKSETUP_WIFIAUTHMODE_OPEN == auth1 ||QUICKSETUP_WIFIAUTHMODE_SHARE == auth1)){
                if ($('#ssid_encryption_mode_basic').val() == QUICKSETUP_WIFIBASICENCRYPMODE_NONE){
                    showConfirmDialog(wlan_hint_use_encryption, quicksetup_showStepLast);
                    return;
                }else{
                    quicksetup_showStepLast();
                    }
                }else{
                    quicksetup_showStepLast();
                    }
            }
        }else{
            if(($("#select_connection_device").val() == MODE_RADIO_ONLY)&&quicksetup_validateProfile()){
                isValid = true;
            }else if(($("#select_connection_device").val() == MODE_WAN_ONLY)&& quicksetup_validateETHsData()){
                isValid = true;
            }else{
                isValid = false;
                return;
            }
            quicksetup_showStepLast();
        }
    }


    if (isValid == true)
    {
        for (i = 0; i <= 4; i++)
        {
            $('#quicksetup' + i).css('display', 'none');
        }
        $('#quicksetup' + step).css('display', 'block');
    }

}

/*--------------------------get wifi basic data-----------------------*/
function getWifiBaiscDataInfo(wifiBasicInfo){
    wifiBasicInfo.WifiSsid = $.trim($('#ssid_wifiName').val());
    wifiBasicInfo.WifiHide = $('#ssid_broadcast_select').val();
    if(wifiBasicInfo.WifiHide == '0')
    {
        wifiBasicInfo.WifiWpsenbl = '0';
    }
    wifiBasicInfo.WifiAuthmode = $('#ssid_authentication').val();
    wifiBasicInfo.WifiWpaencryptionmodes = $('#ssid_encryption_mode_wpa').val();
    wifiBasicInfo.WifiBasicencryptionmodes = $('#ssid_encryption_mode_basic').val();
switch(wifiBasicInfo.WifiAuthmode){
    case QUICKSETUP_WIFIAUTHMODE_OPEN:
        wifiBasicInfo.WifiBasicencryptionmodes = $("#ssid_encryption_mode_basic").val();
        wifiBasicInfo.BasicAuthenticationMode = QUICKSETUP_WIFIBASICENCRYPMODE_NONE;
        if(QUICKSETUP_WIFIBASICENCRYPMODE_WEP == $("#ssid_encryption_mode_basic").val()){
            wifiBasicInfo.WifiWepKey1 = encrypt($('#ssid_neworkKey1').val());
            wifiBasicInfo.WifiWepKeyIndex = 1;
        }
        break;
    case QUICKSETUP_WIFIAUTHMODE_SHARE:
        wifiBasicInfo.WifiBasicencryptionmodes = $("#ssid_encryption_mode_basic").val();
        wifiBasicInfo.BasicAuthenticationMode = QUICKSETUP_WIFIBASICENCRYPMODE_SHARE;
        if(QUICKSETUP_WIFIBASICENCRYPMODE_WEP == $("#ssid_encryption_mode_basic").val()){
            wifiBasicInfo.WifiWepKey1 = encrypt($('#ssid_neworkKey1').val());
            wifiBasicInfo.WifiWepKeyIndex = 1;
        }
        break;
     case QUICKSETUP_WIFIAUTHMODE_WPA_PSK:
        wifiBasicInfo.WifiWpaencryptionmodes = $("#ssid_encryption_mode_wpa").val();
        wifiBasicInfo.WifiWpapsk = encrypt($("#ssid_wpa_key").val());
        break;
     case QUICKSETUP_WIFIAUTHMODE_WPA2_PSK:
        wifiBasicInfo.IEEE11iEncryptionModes = $("#ssid_encryption_mode_wpa").val();
        wifiBasicInfo.WifiWpapsk = encrypt($("#ssid_wpa_key").val());
        break;
     case QUICKSETUP_WIFIAUTHMODE_WPA_WPA2_PSK:
        wifiBasicInfo.X_MixedEncryptionModes = $("#ssid_encryption_mode_wpa").val();
        wifiBasicInfo.WifiWpapsk = encrypt($("#ssid_wpa_key").val());
        break;
    }
}

/*-----------------------------------------post wifi data-------------------*/
function quicksetup_postData_wifi() {
    var WifiBasicDate = null;
    if($.isArray(g_quicksetup_wifiBasicData)) {
        getWifiBaiscDataInfo(g_quicksetup_wifiBasicData[0]);
        WifiBasicDate = g_quicksetup_wifiBasicData[0];
    }
    else {
        getWifiBaiscDataInfo(g_quicksetup_wifiBasicData);
        WifiBasicDate = g_quicksetup_wifiBasicData;
    }
    var basic = {
            Ssid : WifiBasicDate
    };
    
    var xmlstr_security = object2xml('request', basic);
    saveAjaxData('api/wlan/basic-settings', xmlstr_security, function($xml) {
        var ret = xml2object($xml);
        if (!isAjaxReturnOK(ret))
        {
            g_quicksetup_saveDataOK = false;
        }
        if(g_quicksetup_saveDataOK)
        {
           closeWaitingDialog();
           showInfoDialog(common_success);
           setTimeout(function() {
           gotoPageWithHistory('home.html');
           }, 2000);
        }
        else
        {
           closeWaitingDialog();
           showInfoDialog(common_failed);
           g_quicksetup_saveDataOK = true;
        }
    });
}

function quicksetup_postData() {
    
    
    // wifi ending
    if(MODE_RADIO_ONLY == $("#select_connection_device").val()) {
        postProfileData();
    }
    else if (MODE_WAN_ONLY == $("#select_connection_device").val()) {
        postETHData();
    }
    if(QUICKSETUP_WIFISTATUS_DISABLE != QUICKSETUP_WIFIENABLE_STATUS){
        if($('#wifi_config_enable').attr('checked'))
        {
            // wifi beginning
            quicksetup_postData_wifi();
        }     
    }

    

}

/*-----------------------------post profile data-------------------*/
function postProfileData() {
    var quicksetup_currentProfileData;
    var edit_or_add;
    var getNewProfile_index;
    var profileName = $.trim($('#profileName').val());

    if($('#profileName_List').val() == 0){
        quicksetup_currentProfileData = g_default_wanless_profile;
        edit_or_add = 1; 
        quicksetup_currentProfileData.Name = profileName;
    }else{
        $.each(g_profile_list, function(ind, current_profile) {
            if (current_profile.Index == $('#profileName_List').val())
            {
                quicksetup_currentProfileData = current_profile;
                getNewProfile_index = current_profile.Index;
            }
        });
        edit_or_add = 2;
    }
    if (!g_module.autoapn_enabled || 0 == quicksetup_autoAPN)
    {
        var request_profile = {};
        var apnstatic = 0;
        var apnname = '';
        
        if('' != $.trim($("#apn").val())){
            quicksetup_currentProfileData.ApnIsStatic = 1;
            quicksetup_currentProfileData.ApnName = $("#apn").val();
        }
        request_profile = {
                Delete: 0,
                SetDefault: 0,
                Modify: edit_or_add,
                'Profile' : quicksetup_currentProfileData
            };
            var xmlstr_profiles = object2xml('request', request_profile);
            saveAjaxData('api/dialup/profiles', xmlstr_profiles,function($xml){
                var ret = xml2object($xml);
                if(isAjaxReturnOK(ret)){
                    if(edit_or_add == 1){
                        getAjaxData('api/dialup/profiles', function($xml){
                            var ret = xml2object($xml);
                            if ((ret.type == 'response') && typeof(ret.response.Profiles) != 'undefined'){
                                var ProfileDate = ret.response.Profiles.Profile;
                                var ProfileList = [];
                                if($.isArray(ProfileDate)){
                                    ProfileList = ProfileDate;
                                }else{
                                    ProfileList.push(ProfileDate);
                                }
                                for(var i=0;i<ProfileList.length;i++){
                                     if(profileName == ProfileList[i].Name){
                                            getNewProfile_index = ProfileList[i].Index;
                                     }
                                }
                            }
                         },{sync: true});
                    }
                    setPublicConnectionfun($('#select_connection_device').val(),getNewProfile_index,0);
                }else{
                   g_quicksetup_saveDataOK = false;
                   if((QUICKSETUP_WIFISTATUS_DISABLE == QUICKSETUP_WIFIENABLE_STATUS) 
                    || (!($('#wifi_config_enable').attr('checked')))){
                        show_setup_error();     
                   }
                }
            },{sync: true});
            return;
    }
    
}

function getProfileArrayStatus(ProNameList,ProName){
    if(1 <= ProNameList.length){
        for(var i =0;i<ProNameList.length;i++){
            if(ProNameList[i].Name == ProName){
                if(($('#authentication').val()==ProNameList[i].AuthMode) &&
                    ($('#apn').val()==ProNameList[i].ApnName) &&
                    ($('#select_connection_mode').val()==ProNameList[i].ConnectMode) &&
                    ($('#iptype').val()==checkIpv4v6Type(ProNameList[i].IPv4_Supported,ProNameList[i].IPv6_Supported))){
                        return PROFILE_NOEDIT_STATUS;
                    }else{
                        return PROFILE_EDITCONTENT_STATUS;
                    }
            }
        }
        return PROFILE_EDITNAME_STATUS;
    }
    return NO_PROFILE_NEW_STATUS;
}

function setPublicConnectionfun(connDevice,setWirelessIdex,setWirelessIndex){
    var request_connection={
        ConnectionDevice : connDevice,
        setDefaultProfile : setWirelessIdex,
        setDefaultETH : setWirelessIndex
    };
    var xmlstr_connection = object2xml('request', request_connection);
    saveAjaxData('api/connection/connection', xmlstr_connection,function($xml){
       var ret = xml2object($xml);
       if (!isAjaxReturnOK(ret))
       {
           g_quicksetup_saveDataOK = false;
       }
        if((QUICKSETUP_WIFISTATUS_DISABLE == QUICKSETUP_WIFIENABLE_STATUS) || 
            (!$('#wifi_config_enable').attr('checked')))
        {
            if(g_quicksetup_saveDataOK)
            {
                closeWaitingDialog();
                showInfoDialog(common_success);
                setTimeout(function() {
                gotoPageWithHistory('home.html');
                }, 2000);
            }
            else
            {
                closeWaitingDialog();
                showInfoDialog(common_failed);
                g_quicksetup_saveDataOK = true;
            }
        }
    },{sync: true});
}

function show_setup_error()
{
    closeWaitingDialog();
    showInfoDialog(common_failed);
    g_quicksetup_saveDataOK = true;
}

function quicksetup_finish() {
    quicksetup_postData();
}

function quicksetup_bindButtonClick() {
    $('#step1').click(function() {
        quicksetup_settings(1);
        //checkConnectiontype();
    });

    $('#step2_back').click(function() {
        if (isButtonEnable('step2_back'))
        {
            quicksetup_settings(0);
        }
    });

    $('#step2_next').click(function() {
        if (isButtonEnable('step2_next'))
        {   
            if(0 == $('#profileName_List').val())
            {
                if (($.isArray(g_profile_list)) && (g_profile_list.length >= PROFILE_MAX_NUM)) 
                {
                    showInfoDialog(dialup_hint_max_profile_number.replace('%d', PROFILE_MAX_NUM));
                    return;
                }
            }
        
            if(QUICKSETUP_WIFISTATUS_DISABLE != QUICKSETUP_WIFIENABLE_STATUS){
                quicksetup_settings(2);
            }else{
                quicksetup_settings(4);
            }
        }
    });

    $('#step3_back').click(function() {
        quicksetup_settings(1);
    });

    $('#step3_next').click(function() {
        if($('#wifi_config_enable').attr('checked'))
        {
             quicksetup_settings(3);
        }       
        else
        {
             quicksetup_settings(4);
        }
    });

    $('#step4_back').click(function() {
        quicksetup_settings(2);
    });

    $('#step4_next').click(function() {
        quicksetup_settings(4);
    });

    $('#step5_back').click(function() {
        if(QUICKSETUP_WIFISTATUS_DISABLE != QUICKSETUP_WIFIENABLE_STATUS){
            if($('#wifi_config_enable').attr('checked'))
            {
                quicksetup_settings(3);
            }else{
                quicksetup_settings(2);
            }
        }else{
            quicksetup_settings(1);
        }
    });

    $('#step5_next').click(function() {
        quicksetup_settings(5);
    });

    $('#step6_back').click(function() {
        quicksetup_settings(4);
    });

    $('#step_finish').click(function() {
        showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
        setTimeout(quicksetup_finish,1000);
    });
}

function configDataDisplay() {
    if(g_AuFeature.authentication_info_enabled =='1')
    {
        $('#tr_authentication').show();
        $('#last_Wireless_tr_authentication').show();
    }
    else
    {
        $('#tr_authentication').hide();
        $('#last_Wireless_tr_authentication').hide();
    }
    if(g_AuFeature.dialup_number_enabled =='1')
    {
        $('#dialup_number').show();
        $('#last_dialup_number').show();
    }
    else
    {
        $('#dialup_number').hide();
        $('#last_dialup_number').hide();    
    }
    
    if(g_AuFeature.dialup_ipaddress_enabled =='1')
    {
        $('#quicksetup_iptype').show();
        $('#quicksetup_ipaddress').show();
        $('#quicksetup_ipaddress_ex').show();
        $('#last_dialup_ip_address').show();
    }
    else
    {
        $('#quicksetup_iptype').hide();
        $('#quicksetup_ipaddress').hide();
        $('#quicksetup_ipaddress_ex').hide();
        $('#last_dialup_ip_address').hide();
    }

    if('1' == g_AuFeature.connection_mode){
        $('.quicksetup_connmode').show();
    }else{
        $('.quicksetup_connmode').hide();
    }

    if('1' == g_AuFeature.iptype){
        $('#iptype_tr').show();
        $('#last_iptype_Wireless').show();
    }else{
        $('#iptype_tr').hide();
        $('#last_iptype_Wireless').hide();
    }
}

/*--------------------------function show profile or wan---------------------------*/
function showWanProfile() {
    if($("#select_connection_device").val() == MODE_RADIO_ONLY) {
        $("#profile_status_Wireless").show();
        $("#profile_status_Wireline").hide();
        $("#show_eth_div").hide();
        $("#up_Settings").show();
    }
    else if($("#select_connection_device").val() == MODE_WAN_ONLY) {
        $("#up_Settings").hide();
        $("#show_eth_div").show();
        $("#show_eth_div").css({'border-top':'1px','margin-top':'0px'});
        $("#profile_status_Wireless").hide();
        $("#profile_status_Wireline").show();
    }
    else {
        $("#show_eth_div").css({'border-top':'1px solid #DEDEDF','margin-top':'20px'});
        $("#show_eth_div").show();
        $("#up_Settings").show();
        $("#profile_status_Wireless").show();
        $("#profile_status_Wireline").show();
    }
}

function getEthProfileArrayStatus(ProNameList,ProName,conn_type){
    if(1 <= ProNameList.length){
        for(var i =0;i<ProNameList.length;i++){
            if((ProNameList[i].Name == ProName) || (ProNameList.length == 1 && g_Eth_list[i].ConnectionType == QUICKSETUP_CONNECTION_TYPE_IP && conn_type == QUICKSETUP_CONNECTION_TYPE_PPP) || (conn_type == QUICKSETUP_CONNECTION_TYPE_IP)){
                return PROFILE_EDITCONTENT_STATUS;
            }
        }
        return PROFILE_EDITNAME_STATUS;
    }
    return NO_PROFILE_NEW_STATUS;
}

/*--------------------------------post data--------------------------------------*/
function postETHData() {
    var eth_post_info = "";
    var username = $.trim($("#eth_user_name").val());
    var password = $("#eth_password").val();
    var ethname = $.trim($('#input_eth_name').val());
    var conntype = $('#select_connection_type').val();
    var iptype = $('#select_wan_iptype').val();
    var authenticationmode = $('#eth_authentication_mode').val();
    var mtuormru = $('#input_mrt_mtu').val();
    var dialupmode = $('#select_dialupmode').val();
    var maxideltime = $('#input_maxideltime').val();
    var ipv4addrtype = $('#select_ipv4_address_type').val();
    var ipv4addr = $('#input_ipv4_address').val();
    var subnetmask =$('#input_subnet_mask').val();
    var defaultgateway = $('#input_default_gateway').val();
    var primarydns = $('#input_primary_DNS').val();
    var secondarydns = $('#input_secondary_DNS').val();
    var ipv6addrtype = $('#select_ipv6_address_type').val();
    var ipv6addr = $('#input_ipv6_address').val();
    var ipv6defaultgateway = $('#input_ipv6_default_gateway').val();
    var ipv6IdIpv6PrefixLen = $('#input_ipv6_IdIpv6PrefixLen').val();
    var ipv6primarydns = $('#input_ipv6_primary_DNS').val();
    var ipv6secondarydns = $('#input_ipv6_secondary_DNS').val();    
    checkIpv4v6Value(iptype);
    var eth_post_date = {
            Index : '',
            IsValid : 1,
            Name : resolveXMLEntityReference(ethname),
            MTU : mtuormru,
            ConnectionType : conntype,
            IPv4_Supported : QUICKSETUP_IPTYPE_IPV4VALUE,
            IPv6_Supported : QUICKSETUP_IPTYPE_IPV6VALUE,
            ReadOnly : 0
    };

    if((QUICKSETUP_IPTYPE_IPV6 == iptype || QUICKSETUP_IPTYPE_IPV4IPV6 == iptype) && QUICKSETUP_ADDR_STATIC_STATIC== ipv6addrtype){
        eth_post_date.IdIpv6AddrType = ipv6addrtype;
        eth_post_date.IdIpv6Addr = ipv6addr;
        eth_post_date.IdIpv6PrefixLen = (ipv6IdIpv6PrefixLen == '') ? 0 : ipv6IdIpv6PrefixLen;
        eth_post_date.IdIpv6Gateway = ipv6defaultgateway;
        eth_post_date.X_IPv6DNSServers = ipv6secondarydns==''?ipv6primarydns:(ipv6primarydns+','+ipv6secondarydns);
    }else if('' != iptype){
        eth_post_date.IdIpv6AddrType = ipv6addrtype;
    }
    
    if(QUICKSETUP_CONNECTION_TYPE_PPP == conntype){
        eth_post_date.ConnectMode = dialupmode;
        eth_post_date.MaxIdelTime = (maxideltime == '') ? 0: maxideltime;
        eth_post_date.Username = resolveXMLEntityReference(username);
        eth_post_date.Password = encrypt(password);
        eth_post_date.AuthMode = authenticationmode;
    }else if(QUICKSETUP_CONNECTION_TYPE_IP == conntype){
        if((QUICKSETUP_IPTYPE_IPV4 == iptype || QUICKSETUP_IPTYPE_IPV4IPV6 == iptype) && QUICKSETUP_ADDR_STATIC_STATIC == ipv4addrtype){
            eth_post_date.IdAddrType = ipv4addrtype;
            eth_post_date.IdIpAddr = ipv4addr;
            eth_post_date.IdSubMask = subnetmask;
            eth_post_date.IdGateWay = defaultgateway;
            eth_post_date.DNSServers = secondarydns==''?primarydns:(primarydns+','+secondarydns);
        }else if('' != iptype){
            eth_post_date.IdAddrType = ipv4addrtype;
        }
    }
    if(PROFILE_EDITCONTENT_STATUS == getEthProfileArrayStatus(g_Eth_list,$('#input_eth_name').val(),conntype)){
        var ethPro_deleteindex = 0;
        var ethPro_modify = 2;
        if(1 == g_Eth_list.length && g_Eth_list[g_quicksetup_currentEthIndex].ConnectionType != conntype){
            ethPro_deleteindex = g_Eth_list[g_quicksetup_currentEthIndex].Index;
            ethPro_modify = 1;
        }
        eth_post_date.Index = g_Eth_list[g_quicksetup_currentEthIndex].Index;
        eth_post_info = {
            Delete : ethPro_deleteindex,
            SetDefault :0,
            Modify :ethPro_modify,
            ETH : eth_post_date
        };
        var xmlstr_profiles = object2xml('request', eth_post_info);
        saveAjaxData('api/dialup/ETHs', xmlstr_profiles,function($xml){
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret))
            {       
                if(g_quicksetup_currentConType != $("#select_connection_type").val()){
                    getAjaxData('api/dialup/ETHs', function($xml){
                        var ret1 = xml2object($xml);
                        if ((ret1.type == 'response') && typeof(ret1.response.ETHs) != 'undefined'){
                            var ProfileDate = ret1.response.ETHs.ETH;
                            var ProfileList = [];
                            if($.isArray(ProfileDate)){
                                ProfileList = ProfileDate;
                            }else{
                                ProfileList.push(ProfileDate);
                            }
                            for(var i=0;i<ProfileList.length;i++){
                                if(ethname == ProfileList[i].Name){
                                    getNewProfile_index = ProfileList[i].Index;
                                }
                            }
                    }
                    setPublicConnectionfun($('#select_connection_device').val(),0,getNewProfile_index);
                    return;
                },{sync: true});
               }else
               {
                  setPublicConnectionfun($('#select_connection_device').val(),0,$('#select_eth_name').val());
               }
            }
            else{
               g_quicksetup_saveDataOK = false;
               if((QUICKSETUP_WIFISTATUS_DISABLE == QUICKSETUP_WIFIENABLE_STATUS) 
               || (!($('#wifi_config_enable').attr('checked')))){
                    show_setup_error();     
               }
            }
        },{sync: true});
    }else if((PROFILE_EDITNAME_STATUS == getEthProfileArrayStatus(g_Eth_list,$('#input_eth_name').val(),conntype)) ||
                    (NO_PROFILE_NEW_STATUS== getEthProfileArrayStatus(g_Eth_list,$('#input_eth_name').val(),conntype))){
        eth_post_info = {
            Delete : 0,
            SetDefault :0,
            Modify :1,
            ETH : eth_post_date
        };
        var getNewProfile_index = 0;
        var xmlstr_profiles = object2xml('request', eth_post_info);
        saveAjaxData('api/dialup/ETHs', xmlstr_profiles, function($xml){
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret))
            {
                getAjaxData('api/dialup/ETHs', function($xml){
                    var ret = xml2object($xml);
                    if ((ret.type == 'response') && typeof(ret.response.ETHs) != 'undefined'){
                        var ProfileDate = ret.response.ETHs.ETH;
                        var ProfileList = [];
                        if($.isArray(ProfileDate)){
                            ProfileList = ProfileDate;
                        }else{
                            ProfileList.push(ProfileDate);
                        }
                        for(var i=0;i<ProfileList.length;i++){
                             if(ethname == ProfileList[i].Name){
                                    getNewProfile_index = ProfileList[i].Index;
                             }
                        }
                    }
                    setPublicConnectionfun($('#select_connection_device').val(),0,getNewProfile_index);
                    return;
                },{sync: true});
            }
            else
           {
               g_quicksetup_saveDataOK = false;
               if((QUICKSETUP_WIFISTATUS_DISABLE == QUICKSETUP_WIFIENABLE_STATUS) 
               || (!($('#wifi_config_enable').attr('checked')))){
                    show_setup_error();     
               }
           }
        },{sync: true});

    }

}
function get_WirelineDisplay_flag()
{  
    getConfigData('config/global/config.xml', function($xml) {
        var ret = _xml2feature($xml);       
        if(typeof(ret.wirelinewan_enable) != 'undefined')
        {
            g_WirelineDisplay = ret.wirelinewan_enable == 1? true:false;
        }   
    }, {
            sync: true
    });
}


$(document).ready(function() {
    $('#select_data_connecion').append("<option value='0'>" + dialup_label_3g_only + '</option>');
    $('#select_data_connecion').append("<option value='1'>" + setting_label_wifi_only + '</option>');
    $('#select_data_connecion').append("<option value='2'>" + setting_label_wifi_prefer + '</option>');

    $('#select_connection_type').append("<option value='" + QUICKSETUP_CONNECTION_TYPE_PPP+ "'>" + wan_PPP_Routed_text + '</option>');
    $('#select_connection_type').append("<option value='" + QUICKSETUP_CONNECTION_TYPE_IP+ "'>" + wan_IP_Routed_text + '</option>');

    $('#select_connection_mode,#select_dialupmode').append("<option value='" + QUICKSETUP_DIALUPMODE_AUTO+ "'>" + common_auto + '</option>');
    $('#select_connection_mode,#select_dialupmode').append("<option value='" + QUICKSETUP_DIALUPMODE_MANUAL+ "'>" + common_manual + '</option>');
    $('#select_connection_mode,#select_dialupmode').append("<option value='" + QUICKSETUP_DIALUPMODE_ONDEMAND+ "'>" + label_dailup_option_on_demand + '</option>');
    
    $('#authentication,#eth_authentication_mode').append("<option value='" + QUICKSETUP_AUTHMODE_AUTO+ "'>" + common_auto + '</option>');
    $('#authentication,#eth_authentication_mode').append("<option value='" + QUICKSETUP_AUTHMODE_PAP+ "'>" + dialup_label_pap + '</option>');
    $('#authentication,#eth_authentication_mode').append("<option value='" + QUICKSETUP_AUTHMODE_CHAP+ "'>" + dialup_label_chap + '</option>');
    $('#authentication').append("<option value='" + QUICKSETUP_AUTHMODE_NONE+ "'>" + wlan_label_none + '</option>');

     $('#select_ipv4_address_type,#select_ipv6_address_type').append("<option value='" + QUICKSETUP_ADDR_STATIC_DHCP+ "'>" + dhcp_label_dhcp + '</option>');
     $('#select_ipv6_address_type').append("<option value='" + QUICKSETUP_ADDR_STATIC_SLAAC+ "'>" + slaac_label_slaac + '</option>');
     $('#select_ipv4_address_type,#select_ipv6_address_type').append("<option value='" + QUICKSETUP_ADDR_STATIC_STATIC+ "'>" + dialup_label_static + '</option>');

     $('#iptype,#select_wan_iptype').append("<option value='"+QUICKSETUP_IPTYPE_IPV4+"'>" + wan_ipv4_text + '</option>');
     $('#iptype,#select_wan_iptype').append("<option value='"+QUICKSETUP_IPTYPE_IPV6+"'>" + wan_ipv6_text+ '</option>');
     $('#iptype,#select_wan_iptype').append("<option value='"+QUICKSETUP_IPTYPE_IPV4IPV6+"'>" + wan_ipv4_ipv6_text + '</option>');

    $('#ssid_broadcast_select').append("<option value='0'>" + common_disable + '</option>');
    $('#ssid_broadcast_select').append("<option value='1'>" + common_enable + '</option>');


    $("#ssid_authentication option[value="+QUICKSETUP_WIFIAUTHMODE_OPEN+"]").text(wlan_label_open);
    $("#ssid_authentication option[value="+QUICKSETUP_WIFIAUTHMODE_SHARE+"]").text(wlan_label_share);
    $("#ssid_authentication option[value="+QUICKSETUP_WIFIAUTHMODE_WPA_PSK+"]").text(wlan_label_wpa_psk);
    $("#ssid_authentication option[value="+QUICKSETUP_WIFIAUTHMODE_WPA2_PSK+"]").text(wlan_label_wpa2_psk);
    $("#ssid_authentication option[value="+QUICKSETUP_WIFIAUTHMODE_WPA_WPA2_PSK+"]").text(wlan_label_wpa_wpa2_psk);

    $("#ssid_encryption_mode_wpa option[value="+QUICKSETUP_WIFIADVENCRYPMODE_AES+"]").text(wlan_label_aes);
    $("#ssid_encryption_mode_wpa option[value="+QUICKSETUP_WIFIADVENCRYPMODE_TKIP+"]").text(wlan_label_tkip);
    $("#ssid_encryption_mode_wpa option[value="+QUICKSETUP_WIFIADVENCRYPMODE_AES_TKIP+"]").text(wlan_label_aes_tkip);
    
    //device change listener
    $("#select_connection_device").change(function() {
        showWanProfile();
    });
    
    //eth change listener
/*    
    $("#select_eth_name").change(function() {
        quicksetup_setETHInfo($("#select_eth_name").val());
    });
*/
    //eth Encryption mode change
    $("#ssid_encryption_mode_basic").change(function() {
        getDefferentKey($("#ssid_encryption_mode_basic").val());
    });
    
    if (g_module.ap_station_enabled)
    {
        $('.no_station').remove();
    }
    else
    {
        $('#quicksetup5_data_connection').remove();
        $('.have_station').remove();
    }

    if (!g_module.autoapn_enabled)
    {
        $('#id_tr_autoapn').remove();
    }
    getConfigData('config/dialup/config.xml', function($xml) {
        g_AuFeature = _xml2feature($xml);
    }, {
        sync: true
    });
    //configDataDisplay();
    // init qtip
    $('#tooptips_ssid').qtip({
        content: '<b>' + dialup_label_connection_mode + '</b>:' + common_auto + ',' + common_manual,
        position: {
            corner: {
                tooltip: 'rightMiddle',
                target: 'leftMiddle'
            }
        }
    });
    $('#tooltips_encryption_mode').qtip({
        content: '<b>' + wlan_label_encryption_mode + '</b>:' + wlan_label_none + ',' + wlan_label_wep,
        position: {
            corner: {
                tooltip: 'rightMiddle',
                target: 'leftMiddle'
            }
        }
    });

    // init page data
    quicksetup_initPageData();
    
    quicksetup_initPage_wifiEnable();
    if(QUICKSETUP_WIFISTATUS_DISABLE != QUICKSETUP_WIFIENABLE_STATUS){
        quicksetup_initPage_wifi();
    }

    //bind button click
    quicksetup_bindButtonClick();
    
    

    $('#ssid_authentication').change(function() {
        var ssid = 'ssid';
        quicksetup_authentication(this.value, ssid);
    });
   
    $('#select_connection_type').change(function(){checkConnectiontype()});
    $('.wlan_list').hide();
    $('#wlan_content_status').hide();
    //show wifi config or not
    $("#wifi_config_enable").click(function(){

         var cbValue = $('#wifi_config_enable').attr('checked');
         if(cbValue)
         {
            $('.wlan_list').show();
            $('#wlan_content_status').show();
         }
         else
         {
            $('.wlan_list').hide();
            $('#wlan_content_status').hide();
         }
     });
    get_WirelineDisplay_flag();
    if(false==g_WirelineDisplay)
    {
        $('#connectiondevice').hide();
        $("#select_connection_device").val(1);
    }
});

function checkConnModeStatus(mode){
    if(QUICKSETUP_DIALUPMODE_ONDEMAND == mode){
        $('#quicksetup_ethmaxideltime').show();
    }else{
        $('#quicksetup_ethmaxideltime').hide();
    }
}

function checkIPv4AddrType(mode){
    if((QUICKSETUP_ADDR_STATIC_STATIC == mode)&&('none' != $("#IPv4_AddrType_tr").css("display"))){
        $('.IPv4_AddrType_class').show();
    }else{
        $('.IPv4_AddrType_class').hide();
    }
}

function checkIPv6AddrType(mode){
    if((QUICKSETUP_ADDR_STATIC_STATIC == mode)&&('none' != $("#IPv64_AddrType_tr").css("display"))){
        $('.IPv6_AddrType_class').show();
    }else{
        $('.IPv6_AddrType_class').hide();
    }
}

function getDisplayStatus(element_id)
{
    if($("#"+element_id).css('display')!='none'){
            return true;
    }else{
            return false;       
    }
}
function isLanipv4(ipv4)
{
    var matchString = DEFAULT_GATEWAY_IP.substring(0, DEFAULT_GATEWAY_IP.lastIndexOf("."));
    var matchString1 = ipv4.substring(0, ipv4.lastIndexOf("."));
    if(matchString == matchString1)
    {
       return true;
    } 
    return false;
}

function checkIPv4Addr(id){
    if('' == $("#"+id).val()){
        showErrorUnderTextbox(id, samba_input_empty);
        $("#"+id).focus();
        return false;
    } else {
        if(!isValidIpAddress($("#"+id).val())){
            showErrorUnderTextbox(id, samba_input_invalid);
            $("#"+id).focus();
            return false;
        }
        if(isLanipv4($("#"+id).val()))
        {
            showErrorUnderTextbox(id, label_not_input_LAN_address);
            $("#"+id).focus();
            return false;
        }
    }
    return true;
}
function isLanipv6(ipv6)
{   
    var Prefix = $('#input_ipv6_IdIpv6PrefixLen').val();
    if(Prefix<=0||Prefix>128||isNaN(Prefix))
    {
        return false;
    }
    //fd00:1020:3040:5000::1/64,fe80::1/64
    if(DEFAULT_GATEWAY_IPv6Address!='')
    {
        var IPv6AddressParts = DEFAULT_GATEWAY_IPv6Address.split(',');
        var i = 0;
        for(i=0;i<IPv6AddressParts.length;i++)
        {
            var matchString = IPv6AddressParts[i].substring(0, IPv6AddressParts[i].lastIndexOf("/"));
            if(isSameipv6Prefix(matchString,ipv6,Prefix))
            {
                return true;
            }                     
        }
    }        
    return false;
}

function checkIPv6Addr(id){
    if('' == $("#" + id).val()){
        showErrorUnderTextbox(id, samba_input_empty);
        $("#" + id).focus();
        return false;
    } 
    else {
        if(!isValidIpv6Address($("#" + id).val())){
            showErrorUnderTextbox(id, samba_input_invalid);
            $("#" + id).focus();
            return false;
        }
        if(isLanipv6($("#" + id).val()))
        {
            showErrorUnderTextbox(id, label_not_input_LAN_address);
            $("#" + id).focus();
            return false;
        }
    }
    return true;
}

function checkPrefixLenNum(id){
    var flag = true;
    var sizeParts = $('#'+id).val().split("");
    if($('#'+id).val()<0||$('#'+id).val()>128){
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


function getDNSServer(DNSServers,num){
    if( DNSServers == 'undefined' ||DNSServers == ''){
        return '';
    }

    var DNSParts = DNSServers.split(',');
    if(0==num){
        return DNSParts[num];
    } else if(2==DNSParts.length){
        return DNSParts[num];
    } else {
        return '';
    }
}

function checkIpv4v6Type(ipv4value,ipv6value){
    if(QUICKSETUP_IPTYPE_ENABLE == ipv4value && QUICKSETUP_IPTYPE_DISABLE == ipv6value){
        return QUICKSETUP_IPTYPE_IPV4;
    }else if(QUICKSETUP_IPTYPE_ENABLE == ipv6value && QUICKSETUP_IPTYPE_DISABLE == ipv4value){
        return QUICKSETUP_IPTYPE_IPV6;
    }else if(QUICKSETUP_IPTYPE_ENABLE == ipv4value && QUICKSETUP_IPTYPE_ENABLE == ipv6value){
        return QUICKSETUP_IPTYPE_IPV4IPV6;    
    }
}

function checkIpv4v6Value(type){
        if(QUICKSETUP_IPTYPE_IPV4 == type){
            QUICKSETUP_IPTYPE_IPV4VALUE = QUICKSETUP_IPTYPE_ENABLE;
            QUICKSETUP_IPTYPE_IPV6VALUE = QUICKSETUP_IPTYPE_DISABLE;
        }else if(QUICKSETUP_IPTYPE_IPV6 == type){
            QUICKSETUP_IPTYPE_IPV4VALUE = QUICKSETUP_IPTYPE_DISABLE;
            QUICKSETUP_IPTYPE_IPV6VALUE = QUICKSETUP_IPTYPE_ENABLE;
        }else if(QUICKSETUP_IPTYPE_IPV4IPV6== type){
            QUICKSETUP_IPTYPE_IPV4VALUE = QUICKSETUP_IPTYPE_ENABLE;
            QUICKSETUP_IPTYPE_IPV6VALUE = QUICKSETUP_IPTYPE_ENABLE;
        }
}

function checkNameOrPassword(id){
    if ('' != $.trim($('#'+id).val())) {
            if (!checkInputChar_wanName($.trim($('#'+id).val()))) {
                showErrorUnderTextbox(id, wan_valid_username);
                $('#'+id).select();
                $('#'+id).focus();
                return false;
            } else {
                return true;
            }
    } else {
        showErrorUnderTextbox(id, samba_input_empty);
        $('#'+id).select();
        $('#'+id).focus();
        return false;
    }
}
function set_apn_IsStatic()
{
    if($("[name='auto_apn']:checked").val()==0)
    {
         $('#apn').val('');
         $('#apn').attr('disabled', 'disabled');
    }
    else if($("[name='auto_apn']:checked").val()==1)
    {
        $('#apn').removeAttr("disabled");
        $('#apn').focus();
    }
    else
    {
    }
}
function set_ipaddress_IsStatic()
{
    if($("[name='auto_ipaddress']:checked").val()==0)
    {
        $('#ipAddress').val('');
        $('#ipAddress').attr('disabled', true);
    }
    else if($("[name='auto_ipaddress']:checked").val()==1)
    {
        $('#ipAddress').removeAttr("disabled");
        $('#ipAddress').focus();
    }
    else
    {
    }
}

