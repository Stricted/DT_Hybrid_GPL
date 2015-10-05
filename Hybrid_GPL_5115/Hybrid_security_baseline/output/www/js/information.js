// JavaScript Document
var RedirectStatusUmts ;
var RedirectStatusEth  ;

var Label_status_Umts = 'Error';
var Label_status_Eth  = 'Error';


function initPage() {
    RedirectStatusUmts = request(0);
    RedirectStatusEth  = request(1); 
    Status_redirect(RedirectStatusUmts,RedirectStatusEth);
    $("#RedirectStatusUmts").text(Label_status_Umts);
    $("#RedirectStatusEth").text(Label_status_Eth);
}

function request(id)
{ 
    try {
        var url = window.location.href;  
        var paraString = url.substring(url.indexOf("?")+1, url.length).split("=");  
        if (paraString.length > id)
        {
            return paraString[id];
        }
    }
    catch(exception) 
    {
    }
    return "0";
}
function Status_redirect(status_Umts,status_Eth)
{    
    if(status_Umts=='0' && status_Eth=='0')
    {
        //gotoPageWithoutHistory('login.html');
        Label_status_Umts = hint_Status_redirect_Umts_Eth;
        Label_status_Eth = '';
        return;
    }
    switch(status_Umts)
    {
        case '0':
                Label_status_Umts = "Error:No Redirect";
                break;
        case '1':
                gotoPageWithoutHistory('nocard.html');
                return;
        case '2':          
                gotoPageWithoutHistory('simlock.html');
                return;
        case '3':
                gotoPageWithoutHistory('pincoderequired.html');
                return;
        case '4':
                gotoPageWithoutHistory('pukrequired.html');
                return;
        case '5':
                gotoPageWithoutHistory('no3g.html');
                return;
        case '6':
                gotoPageWithoutHistory('noprofile.html');
                return;
        case '7':
                gotoPageWithoutHistory('manual.html');
                return;
        case '8':
                gotoPageWithoutHistory('autoupdate.html');
                return;
        case '9':
                gotoPageWithoutHistory('autoupgrade.html');
                return;
        case '12':
                gotoPageWithoutHistory('errorprofile.html');
                return;
    }
    
    switch(status_Eth)
    {
        case '0':
                Label_status_Eth = "Error:No Redirect";
                break;
        case '1':
                gotoPageWithoutHistory('nonetwork.html');
                break;
        case '2':
                gotoPageWithoutHistory('errorprofile.html');
                return;
        case '3':
                gotoPageWithoutHistory('manual.html');
                return;
    }
}

$(document).ready(function() {
    initPage();
    setLangList();
    showCurrentLanguage();
});
