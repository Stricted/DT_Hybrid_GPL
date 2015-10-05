// JavaScript Document
var g_postfix = window.location.search;
var LOOP_TIME = 3000;

function main_beforeready() {
    if (g_postfix != null) {
        var length = g_postfix.length;
        var start = g_postfix.indexOf('?');
        g_postfix = g_postfix.substring(start + 1, length) + '.html';
    }else {
        g_postfix = '';
    }

}
function check_init_status()
{

    getAjaxData('api/monitoring/status', function($xml) {
        var retsult = xml2object($xml);
        if ('1' == retsult.response.simlockStatus) {
            gotoPageWithoutHistory("simlock.html");
        }
    },{sync :true});
    
    getAjaxData("api/pin/status", function($xml) {
    var ret = xml2object($xml);
    if ('response' == ret.type)
    {
        if (MACRO_NO_SIM_CARD == ret.response.SimState)
        {
            gotoPageWithoutHistory("nocard.html"+ window.location.search);
        }        
        else if (MACRO_PUK_REQUIRED == ret.response.SimState)
        {
            gotoPageWithoutHistory('pukrequired.html'+ window.location.search);
        }
        else if (MACRO_PIN_REQUIRED == ret.response.SimState)
        {
            gotoPageWithoutHistory('pincoderequired.html'+ window.location.search);
        }
        else if (MACRO_CARD_LOCK_PIN_WRONG == ret.response.SimState) 
        {
            gotoPageWithoutHistory('carderror.html'+ window.location.search);
        }
        else if (MACRO_INIT_SIM_CARD == ret.response.SimState)
        {
            setTimeout(check_init_status, LOOP_TIME);   
        }
        else
        {
            succeed_deal(); 
        }          
    }
    else
    {
        setTimeout(check_init_status, LOOP_TIME);  
    }
    });
}
function succeed_deal()
{
    if (('.html' == g_postfix) || ('' == g_postfix)) 
    {
        gotoPageWithoutHistory(LOGIN_PAGE_URL);
    }
    else 
    {
        gotoPageWithoutHistory(g_postfix);
    }
}
main_beforeready();
$(document).ready(function() {
    $('#card_init_leave').show();
    check_init_status();
    setLangList();
    showCurrentLanguage();
    g_is_login_opened = true;
    $('#card_init_leave').click(function() {        
        if (('.html' == g_postfix) || ('' == g_postfix)) 
        {
            gotoPageWithoutHistory(LOGIN_PAGE_URL);
        }
        else 
        {
            gotoPageWithoutHistory(HOME_PAGE_URL);
        }
    });
});
