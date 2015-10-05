// JavaScript Document
/**************Constant for check SIM card, PIN, [PUK]***************/
//sim
var MACRO_SAVE_PIN_ENABLED = "1";
var MACRO_SAVE_PIN_DISABLED = "0";
var SIM_PIN_TIMES_DEFAULT = "3";
var SIM_SAVE_PIN_SCID = "0";
var SIM_SAVE_PIN_DEFAULT = "0";
var SIM_PUK_TIMES_DEFAULT = "10";
var MACRO_SIM_LOCK_ENABLE = "1";
var MACRO_SIM_REMAIN_TIMES = "10";
var MACRO_SIM_PLOCK_ENABLE = "0";
var MACRO_NETOWRK_SERVICE_AVAILABILITY = 2;
/***********************/

//Create object for store the Ajax response from simcard and pincode
var simlock_status = {
    SimLockEnable: "",
    SimLockRemainTimes: "",
    pSimLockEnable: "",
    pSimLockRemainTimes: ""
};

var pin_status = {
    SimState: "",
    PinOptState: "",
    SimPinTimes: "",
    SimPukTimes: ""
};

function redirectOnCondition(prefix, postfix) {
    //get SIM status
    var ret = false;
    var url_prefix = "";
    if ("string" == typeof(prefix)) {
        url_prefix = prefix;
    }
    if("string" == typeof(postfix)) {
        postfix = "?" + postfix;
    }
    else {
        postfix="";
    }
    //get Simlock status
    getAjaxData('api/monitoring/status', function($xml) {
        var currentStatus = xml2object($xml);
        if(MACRO_NO_SIM_CARD == currentStatus.response.SimStatus)
        {
            gotoPageWithoutHistory(url_prefix + "nocard.html" + postfix);
            ret = true;
        }
        else if(currentStatus.response.SimStatus==0 && currentStatus.response.simlockStatus==1)
        {
            gotoPageWithoutHistory(url_prefix + "simlock.html" + postfix);
            ret = true;
        }
    },{sync :true});
    if (ret) {
        return true;
    }
    //get PIN status
    getAjaxData("api/pin/status", function($xml) {
        var pinstatus_ret = xml2object($xml);
        if ("response" == pinstatus_ret.type) {
            pin_status.SimState = pinstatus_ret.response.SimState;
            pin_status.PinOptState = pinstatus_ret.response.PinOptState;
            pin_status.SimPinTimes = pinstatus_ret.response.SimPinTimes;
            pin_status.SimPukTimes = pinstatus_ret.response.SimPukTimes;

            log.debug("REDIRECT : SimState is " + pin_status.SimState);
            //Judgement pin status
            if (MACRO_NO_SIM_CARD == pin_status.SimState) {
                log.debug("REDIRECT : SimState == MACRO_NO_SIM_CARD, redirect to nocard.html");
                gotoPageWithoutHistory(url_prefix + "nocard.html" + postfix);
                ret = true;
            }
            else if (MACRO_CPIN_FAIL == pin_status.SimState) {
                log.debug("REDIRECT : SimState == MACRO_CPIN_FAIL, redirect to nocard.html");
                gotoPageWithoutHistory(url_prefix + "nocard.html");
                ret = true;
            }
            else if (MACRO_PIN_REQUIRED == pin_status.SimState) {
                log.debug("REDIRECT : SimState == MACRO_PIN_REQUIRED, redirect to pincoderequired.html");

                if ((typeof(G_StateLogin)!="undefined") && G_StateLogin) {
                    gotoPageWithoutHistory(url_prefix + "pincodemanagement.html");
                    ret = true;
                }
                else {
                    gotoPageWithoutHistory(url_prefix + "pincoderequired.html" + postfix);
                    ret = true;
                }
            }
            else if (MACRO_PUK_REQUIRED == pin_status.SimState) {
                log.debug("REDIRECT : SimState == MACRO_PUK_REQUIRED, redirect to pukrequired.html");

                if ((typeof(G_StateLogin)!="undefined") && G_StateLogin) {
                    gotoPageWithHistory(url_prefix + "pukrequired_login.html");
                    ret = true;
                }
                else {
                    gotoPageWithoutHistory(url_prefix + "pukrequired.html" + postfix);
                    ret = true;
                }
            }
            else if (MACRO_CARD_LOCK_PIN_WRONG == pin_status.SimState) {
                log.debug("REDIRECT : SimState == MACRO_NO_SIM_CARD, redirect to carderror.html");
                gotoPageWithoutHistory(url_prefix + "carderror.html" + postfix);
                ret = true;
            }
            else if (MACRO_INIT_SIM_CARD == pin_status.SimState)
            {
                log.debug("REDIRECT : SimState == MACRO_NO_SIM_CARD, redirect to cardinit.html");
                gotoPageWithoutHistory(url_prefix + "cardinit.html" + postfix);
                ret = true;
            }
        }
        else {
            log.error("REDIRECT : Load api/pin/status file failed");
        }
    }, {
        sync : true
    }
    );

    if (ret) {
        return true;
    }
}
function redirectOnConditionforindex(gatewayAddr) {
    //get redirect status
    var ret = false;
    getAjaxData("api/monitoring/redirect", function($xml) {
    
        var Redirectstatus_ret = xml2object($xml);
        var RedirectStatusUmts = '0';
        var RedirectStatusEth  = '0';
        if ("response" == Redirectstatus_ret.type 
            && typeof(Redirectstatus_ret.response.RedirectStatusUmts) != 'undefined'
            && typeof(Redirectstatus_ret.response.RedirectStatusEth) != 'undefined'
            )
        {                           
            RedirectStatusUmts = Redirectstatus_ret.response.RedirectStatusUmts;
            RedirectStatusEth  = Redirectstatus_ret.response.RedirectStatusEth;
           
            log.debug("REDIRECT : RedirectStatusUmts is " + RedirectStatusUmts);
            log.debug("REDIRECT : RedirectStatusEth is " + RedirectStatusEth);
            
            gotoPageWithoutHistory("http://"+gatewayAddr+"/information.html"+'?'+RedirectStatusUmts+'='+RedirectStatusEth);

            ret = true;
        }
        else 
        {
            log.error("REDIRECT : Load api/monitoring/redirect file failed");
        }
    }, {
        sync : true
    }
    );

    return ret;   
}
