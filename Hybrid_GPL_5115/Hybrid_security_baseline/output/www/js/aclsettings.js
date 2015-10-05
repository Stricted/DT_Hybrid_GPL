var ENABLE = '1';

var g_acl_settings = {
    HTTPLanEnable : '',
    HTTPWanEnable : '',
    SSLLanEnable: '',
    SSLWanEnable: '',
    FTPLanEnable : '',
    FTPWanEnable : ''
};

function getstatus()
{
    getAjaxData("api/settings/aclstatus",function($xml){       
        var res = xml2object($xml);
        if('response' == res.type)
        {
            var acl_settings = res.response.AclServices;
            $('#httplan' ).get(0).checked = (acl_settings.HTTPLanEnable == ENABLE) ? true : false;
            $('#httpwan' ).get(0).checked = false;
            $('#ssllan' ).get(0).checked = (acl_settings.SSLLanEnable == ENABLE) ? true : false;
            $('#sslwan' ).get(0).checked = (acl_settings.SSLWanEnable == ENABLE) ? true : false;		
            $('#ftplan' ).get(0).checked = (acl_settings.FTPLanEnable == ENABLE) ? true : false;
            $('#ftpwan' ).get(0).checked = (acl_settings.FTPWanEnable == ENABLE) ? true : false;
        }
    });
}
function relate_ftpserver()
{
    var submitObject_ftpserver_enable = {
        Enable : $('#ftplan').get(0).checked ? 1 : 0
    };
    var ret_enable_ftpserver = '';
    var newXmlString_enable_ftpserver = object2xml('request', submitObject_ftpserver_enable);
    saveAjaxData("api/settings/ftpserverenable",newXmlString_enable_ftpserver,function($xml){   
    });
}

function setacls()
{
    g_acl_settings.HTTPLanEnable = $('#httplan').get(0).checked ? 1 : 0;
    g_acl_settings.HTTPWanEnable = 0;
    g_acl_settings.SSLLanEnable = $('#ssllan').get(0).checked ? 1 : 0;
    g_acl_settings.SSLWanEnable = $('#sslwan').get(0).checked ? 1 : 0;
    g_acl_settings.FTPLanEnable = $('#ftplan').get(0).checked ? 1 : 0;
    g_acl_settings.FTPWanEnable = $('#ftpwan').get(0).checked ? 1 : 0;

    relate_ftpserver();
    
    var acl_object = {
        AclServices : g_acl_settings
    };
    var acl_xml = object2xml("request", acl_object);
    saveAjaxData("api/settings/aclstatus",acl_xml,function($xml){
        var rsp =  xml2object($xml);
        if(isAjaxReturnOK(rsp)) {
            button_enable('apply', "0");
            parent.showInfoDialog(common_success);
            getstatus();
        }
        else {
            parent.showInfoDialog(common_failed);
        }
    });
}

$(document).ready(function(){
    $('#apply').click(function(){
        if(isButtonEnable('apply')){
            setacls();
        }
    });
    button_enable('apply', '0');
    $('input:checkbox').click(function() {
        button_enable('apply', '1');
    });
    getstatus();
});
