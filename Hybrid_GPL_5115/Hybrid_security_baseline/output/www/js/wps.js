var infomsg =
"<table cellpadding='0' align='center' cellspacing='0'>" +
    "<tr height='32'>" +
        "<td width='400' colspan='2'>" + IDS_wlan_message_n_mode_forbidden_wps + '</td>' +
    '</tr>' +
    '<tr>' +
        "<td colspan='2'><hr/></td>" +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_wlan_module + common_colon + '</td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + common_disable + '</td>' +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_80211_authentication + common_colon + ' </td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_open + '/' + wlan_label_share + '</td >' +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_ssid_broadcast + common_colon + '</td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + common_disable + '</td>' +
    '</tr>' +
'</table>';

var newinfomsg =
"<table cellpadding='0' align='center' cellspacing='0'>" +
    "<tr height='32'>" +
        "<td width='400' colspan='2'>" + IDS_wlan_message_n_mode_enable_wps + '</td>' +
    '</tr>' +
    '<tr>' +
        "<td colspan='2'><hr/></td>" +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_wps_settings + common_colon + '</td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + common_enabled + '</td>' +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_wlan_module + common_colon + '</td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + common_enabled + '</td>' +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_80211_authentication + common_colon + ' </td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_wpa2_psk + '/' + wlan_label_wpa_wpa2_psk + '</td >' +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_encryption_mode + common_colon + ' </td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_aes + '/' + wlan_label_aes_tkip + '</td >' +
    '</tr>' +
    '<tr>' +
        "<td width='200' class = 'wps_forbidden_message'>" + wlan_label_ssid_broadcast + common_colon + '</td>' +
        "<td width='200' class = 'wps_forbidden_message'>" + common_enabled + '</td>' +
    '</tr>' +
'</table>';

var g_wps_dataVlaue = '';
var WPS_VERIFY_PIN_1 = 1;
var WPS_VERIFY_PIN_3 = 3;
var PBC_CONNECT_FAILED = '0';
var PBC_CONNECTING = '1';
var PBC_CONNECT_SUCCESS = '2';
var PBC_GET_STATUS_INTERVAL = 5000;
var PBC_WAITING_TIMEOUT = 180000;
var pbcGetStatusTimer = null;
var pbcWaitingTimer = null;

var WIFIAUTHMODE_OPEN = 'None';
var WIFIAUTHMODE_SHARE = 'Basic';
var WIFIBASICENCRYPMODE_NONE = 'None';
var WIFIBASICENCRYPMODE_WEP = 'WEPEncryption';

var WPS_ap_pin = 'ap-pin';
var WPS_client_pin = 'client-pin';
var WPS_apPIN = '';

var g_wps_config = '';
var WIFIAUTHMODE_WPA2_PSK = '11i';
var WIFIAUTHMODE_WPA_WPA2_PSK = 'WPAand11i';
var WIFIENCRYPTIONMODE_AES = 'AESEncryption';
var WIFIENCRYPTIONMODE_AES_TKIP = 'TKIPandAESEncryption';


getDeviceConfig();
$(document).ready(function() {
    button_enable('apply_button', '0');
    $('input').bind('change input keydown paste cut', function() {
        button_enable('apply_button', '1');
    });
    $('#apply_button').click(function(){
        if (!isButtonEnable('apply_button')) 
        {
            return;
        }
        clearAllErrorLabel();
        if (wps_validateInput())
        {
            parent.showConfirmDialog(firewall_hint_submit_list_item, wps_apply,function(){});
        }
    });
    $('#Gen_Pin').click(function(){
        if (!isButtonEnable('Gen_Pin')) 
        {
            return;
        }
        clearAllErrorLabel();
        var wps_set_dataVlaue = { 
                            GenPinMode : '1'
                            };
                
        var wps_xml = object2xml('request', wps_set_dataVlaue);
        saveAjaxData('api/wlan/genpin', wps_xml, function($xml) {         
            var ret = xml2object($xml);
            if(typeof(ret.response)!='undefined' && typeof(ret.response.WPSclientPIN)!='undefined') 
            {
                WPS_apPIN = decrypt(ret.response.WPSclientPIN);
                $('#input_WPSPin').val(WPS_apPIN);
            }
            
        });
        button_enable('apply_button', '1');
    });
    
    wps_initPage();
});
function getDeviceConfig() {
    getConfigData('config/wifi/config.xml', function($xml) {
        var config_ret = _xml2feature($xml);
        if ('undefined' !== config_ret && null !== config_ret) {
            g_wps_config = config_ret.wifiwps.wpspinpbc;
        }
    }, {
        sync: true
    });
}

function wps_singleSSID_getConfig() {
    getAjaxData('api/wlan/basic-settings', function($xml) {
        var ret = xml2object($xml);
        if (0 == ret.response.WifiEnable || 0 == ret.response.WifiHide) {
            setTimeout(function() {
                showInfoDialog(infomsg);
            }, 500);
            if (!g_wps_config) {
                $('#input_WPSPin').attr('disabled', true);
            } else {
                $('#wps_pin_radio').attr('disabled', true);
                $('#wps_pbc_radio').attr('disabled', true);
            }

            return;
        }
        getAjaxData('api/wlan/security-settings', function($xml) {
            var ret = xml2object($xml);
            if (('AUTO' == ret.response.WifiAuthmode || 'OPEN' == ret.response.WifiAuthmode || 'SHARE' == ret.response.WifiAuthmode) &&
            ('NONE' == ret.response.WifiBasicencryptionmodes || 'WEP' == ret.response.WifiBasicencryptionmodes)) {
                setTimeout(function() {
                    showInfoDialog(infomsg);
                }, 500);
                if (!g_wps_config) {
                    $('#input_WPSPin').attr('disabled', true);
                } else {
                    $('#wps_pin_radio').attr('disabled', true);
                    $('#wps_pbc_radio').attr('disabled', true);
                }

                return;
            }
        }, {
            sync: true
        });
    }, {
        sync: true
    });
}

function wps_multiSSID_getConfig() {
    getAjaxData('api/wlan/multi-security-settings', function($xml) {
        var ret = xml2object($xml);
        if (0 == ret.response.WifiEnable) {
            setTimeout(function() {
                showInfoDialog(infomsg);
            }, 500);
            $('#input_WPSPin').attr('disabled', true);
            return;
        }
        getAjaxData('api/wlan/multi-basic-settings', function($xml) {
            var ret = xml2object($xml);
            var wps_basicData = ret.response.Ssids.Ssid;
            if (('AUTO' == wps_basicData[0].WifiAuthmode || 'OPEN' == wps_basicData[0].WifiAuthmode || 'SHARE' == wps_basicData[0].WifiAuthmode) 
            &&('NONE' == wps_basicData[0].WifiBasicencryptionmodes || 'WEP' == wps_basicData[0].WifiBasicencryptionmodes) || (0 == wps_basicData[0].WifiBroadcast)) {
                setTimeout(function() {
                    showInfoDialog(infomsg);
                }, 500);
                if (!g_wps_config) {
                    $('#input_WPSPin').attr('disabled', true);
                } else {
                    $('#wps_pin_radio').attr('disabled', true);
                    $('#wps_pbc_radio').attr('disabled', true);
                }
                return;
            }
        }, {
            sync: true
        });
    }, {
        sync: true
    });

}
function disable_wps()
{
    if (!g_wps_config) {
        $('#input_WPSPin').attr('disabled', true);
        button_enable('Gen_Pin', '0');
        $('#wps_mode_select').attr('disabled', true);
    } else {
        $('#wps_pin_radio').attr('disabled', true);
        $('#wps_pbc_radio').attr('disabled', true);
    }
}
function wps_SSID_getConfigs()
{
    getAjaxData('api/wlan/wifienable', function($xml) {
       var ret = xml2object($xml);
       if (0 == ret.response.Wifienable) {
           setTimeout(function() {
               parent.showConfirmDialog(newinfomsg,function(){return;});
           }, 500);
           disable_wps();
           return;
         }
          getAjaxData('api/wlan/basic-settings', function($xml) 
          {
              var ret = xml2object($xml);
              var wlan_basicData = ret.response.Ssids.Ssid;
              if($.isArray(wlan_basicData))
              {                 
                if(wlan_basicData[0].WifiWpsenbl=='0')
                {
                        setTimeout(function() {
                                    parent.showConfirmDialog(newinfomsg,function(){return;});
                                }, 500);
                        disable_wps();
                        return;
                }                                                 
            }
            else
            {    
               if(wlan_basicData.WifiWpsenbl=='0')
                {
                        setTimeout(function() {
                                    parent.showConfirmDialog(newinfomsg,function(){return;});
                                }, 500);
                        disable_wps();
                        return;
                }
            }   
          }, {
              sync: true
          });
      }, {
         sync: true
   });
}
function change_wpspin_type()
{
    if($("#wps_mode_select").val() == WPS_ap_pin)
    {
        $("<input id='input_WPSPin'" + " type='" + "text" + "' maxlength='"+$('#input_WPSPin').attr('maxlength')+"' style='" + $('#input_WPSPin').attr('style') +"' disabled='" + "disabled" + "' />")
        .replaceAll($('#input_WPSPin'));
    }
    else
    {
        $("<input id='input_WPSPin'" + " type='" + "password" + "' maxlength='"+$('#input_WPSPin').attr('maxlength')+"' style='" + $('#input_WPSPin').attr('style') +"' />")
        .replaceAll($('#input_WPSPin'));
    }
    $('#input_WPSPin').unbind().bind('change input keydown paste cut', function() {
        button_enable('apply_button', '1');
    });
}
function wps_initPage() {
    if (!g_wps_config) {
        $('#wps_pin').show();
        $('#wps_pbc').remove(); 
        var newline = "";     
        newline += "<option value='"+WPS_ap_pin+"'>"+wlan_wps_mode_ap_pin+"</option>";
        newline += "<option value='"+WPS_client_pin+"'>"+wlan_wps_mode_client_pin+"</option>";
        $('#wps_mode_select').empty();
        $('#wps_mode_select').append(newline);
        $("#wps_mode_select").bind('change',function() {
            button_enable('apply_button', '1');
            change_wpspin_type();
            if($("#wps_mode_select").val() == WPS_ap_pin)
            {
                $('#input_WPSPin').val(WPS_apPIN);
                $('#Gen_Pin').show();
                //$('#input_WPSPin').attr('disabled', true);
            }
            else if($("#wps_mode_select").val() == WPS_client_pin)
            {
                $('#input_WPSPin').val('');
                $('#Gen_Pin').hide();
                //$("#input_WPSPin").removeAttr("disabled");
            }
            else
            {
            }
            });      
    } else {
        $('#wps_pin').remove();
        $('#wps_pbc').show();
        getAjaxData('api/wlan/wps-pbc', function($xml) {
            var ret = xml2object($xml);
            if (PBC_CONNECT_FAILED == ret.response.State) {
                $('#wps_pin_radio').attr('checked', true);
            } else if (PBC_CONNECT_SUCCESS == ret.response.State) {
                $('#wps_pbc_radio').attr('checked', true);
                wps_pbc_radio();
            }
        });
    }
    getAjaxData('api/wlan/wps', function($xml) {
        var ret = xml2object($xml);
        g_wps_dataVlaue = ret.response;
        $('#wps_mode_select').val(g_wps_dataVlaue.WPSMode);
        WPS_apPIN = decrypt(g_wps_dataVlaue.WPSclientPIN);
        change_wpspin_type();
        if(g_wps_dataVlaue.WPSMode == WPS_ap_pin)
        {
            $('#input_WPSPin').val(WPS_apPIN);
            $('#Gen_Pin').show();
            $('#input_WPSPin').attr('disabled', true);
        }
    });
    wps_SSID_getConfigs();
}

function wps_verifyPIN(pin) {
    var accum = 0;
    pin = pin >>> 0;
    accum = accum >>> 0;
    accum += WPS_VERIFY_PIN_3 * ((pin / 10000000 | 0) % 10);
    accum += WPS_VERIFY_PIN_1 * ((pin / 1000000 | 0) % 10);
    accum += WPS_VERIFY_PIN_3 * ((pin / 100000 | 0) % 10);
    accum += WPS_VERIFY_PIN_1 * ((pin / 10000 | 0) % 10);
    accum += WPS_VERIFY_PIN_3 * ((pin / 1000 | 0) % 10);
    accum += WPS_VERIFY_PIN_1 * ((pin / 100 | 0) % 10);
    accum += WPS_VERIFY_PIN_3 * ((pin / 10 | 0) % 10);
    accum += WPS_VERIFY_PIN_1 * ((pin | 0) % 10);

    return (0 == (accum % 10));
}

function wps_validateInput() {
    var pin_val = $.trim($('#input_WPSPin').val());
    var patrn = /^[0-9]{8}$/;
    if (pin_val == '' || pin_val == ' ' || pin_val == null) {
        showErrorUnderTextbox('wps_pin_error', wlan_hint_wps_pin_valid_value);
        $('#input_WPSPin').focus();
        return false;
    }

    if (!patrn.exec(pin_val)) {
        showErrorUnderTextbox('wps_pin_error', wlan_hint_wps_pin_valid_value);
        $('#input_WPSPin').focus().select();
        return false;
    }
    if($("#wps_mode_select").val() == WPS_client_pin)
    {
        return true;
    }
    pin_val = parseInt(pin_val, 10);
    if (!wps_verifyPIN(pin_val)) {
        showErrorUnderTextbox('wps_pin_error', IDS_wps_pin_code_verify_error);
        $('#input_WPSPin').focus().select();
        return false;
    }

    return true;
}

/*
 * E355需要的方法
 */

function wps_pin_radio() {
    $('#input_WPSPin').removeAttr('disabled');
}

function wps_pbc_radio() {
    $('#input_WPSPin').attr('disabled', 'disabled');
}

function wps_pbc_connect_status() {
    getAjaxData('api/wlan/wps-pbc', function($xml) {
        var ret = xml2object($xml);
        if (PBC_CONNECT_FAILED == ret.response.State) {
            clearTimeout(pbcWaitingTimer);
            parent.closeWaitingDialog();
            parent.showInfoDialog(pbc_label_connect_failed);
        } else if (PBC_CONNECT_SUCCESS == ret.response.State) {
            clearTimeout(pbcWaitingTimer);
            parent.closeWaitingDialog();
            button_enable('apply_button', '0');
            parent.showInfoDialog(pbc_label_connect_success);
        } else {
            pbcGetStatusTimer = setTimeout(wps_pbc_connect_status, PBC_GET_STATUS_INTERVAL);
            return;
        }
    });
}

function saveWpsPin() {
    if (wps_validateInput()) {        
        var wps_set_dataVlaue;
        if($("#wps_mode_select").val() == WPS_client_pin)
        {
            wps_set_dataVlaue = { 
                          WPSMode :     WPS_client_pin,
                          WPSapPIN:encrypt($.trim($('#input_WPSPin').val()))
                          }; 
        }
        else if($("#wps_mode_select").val() == WPS_ap_pin)
        {
            wps_set_dataVlaue = { 
                          WPSMode : WPS_ap_pin,
                          WPSclientPIN: encrypt($.trim($('#input_WPSPin').val()))
                          };
			WPS_apPIN = $.trim($('#input_WPSPin').val());

        }
        else
        {
        }          
        var wps_xml = object2xml('request', wps_set_dataVlaue);
        saveAjaxData('api/wlan/wps', wps_xml, function($xml) {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                button_enable('apply_button', '0');
                parent.showInfoDialog(common_success);
                $('#input_WPSPin').val($.trim($('#input_WPSPin').val()));
            } else {
                wps_initPage();
            }
        });
    }
}

function wps_apply() {
    if (!isButtonEnable('apply_button')) {
        return;
    }

    clearAllErrorLabel();

    if (!g_wps_config) {
        saveWpsPin();
    } else if ($('#wps_pin_radio').get(0).checked) {
        saveWpsPin();
    } else if ($('#wps_pbc_radio').get(0).checked) {
        var pbcRequest = {
            WPSMode: '1'
        };
        var pbc_xml = object2xml('request', pbcRequest);
        saveAjaxData('api/wlan/wps-pbc', pbc_xml, function($xml) {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                parent.showWaitingDialog(common_waiting, pbc_label_connecting_network);
                pbcWaitingTimer = setTimeout(function() {
                    clearTimeout(pbcGetStatusTimer);
                    parent.closeWaitingDialog();
                    parent.showInfoDialog(pbc_label_connect_failed);
                },
                PBC_WAITING_TIMEOUT);
                pbcGetStatusTimer = setTimeout(function(){wps_pbc_connect_status();}, PBC_GET_STATUS_INTERVAL);
                button_enable('apply_button', 0);
            } else {
                parent.showInfoDialog(pbc_label_connect_failed);
            }
        });
    }
}
