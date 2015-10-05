// JavaScript Document
var PING_TIMEOUT = 5000;

var ping_objIMG = new Image();
var ping_ipAddress = 0;
var g_count = 0;
var ping_intTimerID;


function ping()
{
    ping_objIMG.src = "http://" + ping_ipAddress + "/res/battery_low.gif";
    ping_intTimerID = setTimeout(ping, PING_TIMEOUT);
}
function startPing()
{
    if ($.browser.msie) {
       ping_objIMG.onerror = function(){
           if ($.browser.version == '6.0'){
               g_count ++;
               parent.checkUrlValidity("http://" +  ping_ipAddress + "/index.html", function(isValidity) {
                    if(isValidity) {
                        gotoPageWithoutHistory("http://" +  ping_ipAddress + "/index.html");
                    }
               });
               if (g_count > 8){
                   gotoPageWithoutHistory("http://" +  ping_ipAddress + "/index.html");
               }
           }else {
               gotoPageWithoutHistory("http://" +  ping_ipAddress + "/index.html");
           }
        };
    }
    ping_objIMG.onload = function(){
        clearTimeout(ping_intTimerID);
        gotoPageWithoutHistory("http://" +  ping_ipAddress + "/index.html");
    };
    ping_objIMG.src = "http://" + ping_ipAddress + "/res/battery_low.gif";
    ping();
}

function ping_setPingAddress(ipAddress)
{
    parent.g_is_login_opened = true;
    ping_ipAddress = ipAddress;
}
