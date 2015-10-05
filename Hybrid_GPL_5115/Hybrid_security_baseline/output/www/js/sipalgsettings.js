var g_sipData = {
    H323Enable : 0,
    RTSPEnable : 0,
    PPTPEnable : 0,
    SipStatus : 0,
    SipPort : ''
};
var SIP_ALG_DEFAULT_PORT = 5060;
var SIP_ALG_PORT_NA = '';
function checked_ck() {
    if ($('#enable_sip_alg').get(0).checked == true) {
        $('#sip_port').removeAttr('disabled');
	 if('' == $('#sip_port').val()){	 
            $('#sip_port').val(SIP_ALG_DEFAULT_PORT);		
	 }
    }
    else if ($('#enable_sip_alg').get(0).checked == false) {
        $('#sip_port').attr('disabled', 'disabled');
        $('#sip_port').val(SIP_ALG_PORT_NA);
    }
}
function initPage() {
    getAjaxData('api/security/sip', function($xml) {
        var ret = xml2object($xml);
        if(ret.type == 'response'){
            $('#enable_sip_h323').get(0).checked = (ret.response.H323Enable == 1) ? true : false;
            $('#enable_sip_rtsp').get(0).checked = (ret.response.RTSPEnable == 1) ? true : false;
            $('#enable_sip_pptp').get(0).checked = (ret.response.PPTPEnable == 1) ? true : false;
            $('#enable_sip_alg').get(0).checked = (ret.response.SipStatus == 1) ? true : false;
            $('#sip_port').val(ret.response.SipPort);
            checked_ck();
        }
    });
}

var special_port= 
[
    137,138,139,445,631,21,28090,7547,37215,1900,
    37443,80,443,56001,56002,1701,500,546,547,53,
    67,1280,514,990,38000,42000
];
function is_special_port(port)
{
    var i = 0;   
    for(i = 0; i < special_port.length; i++)
    {
        if(port==special_port[i])
        {
            return true;
        }
    }
    if(port>=7070 && port<=7101)
    {
        return true;
    }
    return false;
}

function validateSIP() {
    var ret = true;
    var sipEnable = $('#enable_sip_alg').get(0).checked ? 1 : 0;
    var sipPort = $('#sip_port').val();
    if (sipEnable == 1) {
        if($.trim($('#sip_port').val()) != ''){
            if (isNaN(sipPort) || $.trim($('#sip_port').val()).indexOf('.')>-1) {
                showErrorUnderTextbox('sip_port', samba_input_invalid);
                $('#sip_port').focus();
                ret = false;
            }
            else if ((sipPort < 1) || (sipPort > 65535) || is_special_port(sipPort)) {
                showErrorUnderTextbox('sip_port', samba_input_invalid);
                $('#sip_port').focus();
                ret = false;
            }
        }
        else {
            showErrorUnderTextbox('sip_port', samba_input_empty);
            $('#sip_port').focus();
            ret = false;
        }
    }
    return ret;
}
function apply() {
    clearAllErrorLabel();
    if (!isButtonEnable('apply_button')) {
        return;
    }
    if (validateSIP() == true) {
        g_sipData = {};
        g_sipData.H323Enable = $('#enable_sip_h323').get(0).checked ? 1 : 0;
        g_sipData.RTSPEnable = $('#enable_sip_rtsp').get(0).checked ? 1 : 0;
        g_sipData.PPTPEnable = $('#enable_sip_pptp').get(0).checked ? 1 : 0;
        g_sipData.SipStatus = $('#enable_sip_alg').get(0).checked ? 1 : 0;
        if('1' == g_sipData.SipStatus){
            g_sipData.SipPort = $.trim($('#sip_port').val());
        }
        var xmlstr_sip = object2xml('request', g_sipData);
        saveAjaxData('api/security/sip', xmlstr_sip, function($xml) {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                button_enable('apply_button', '0');
                $('#sip_port').val($.trim($('#sip_port').val()));
                parent.showInfoDialog(common_success);
                initPage();
            }
            else
            {
                parent.showInfoDialog(common_failed);
                setTimeout(function(){initPage();}, 3000);
            }
        });
    }
}
$(document).ready(function() {
    initPage();
    button_enable('apply_button', '0');
    $('#apply_button').click(function() {
        apply();
    });
    $('#enable_sip_alg').click(function() {
        checked_ck();
    });
    $('input').bind('change input paste cut keydown', function() {
        button_enable('apply_button', '1');
    });
});