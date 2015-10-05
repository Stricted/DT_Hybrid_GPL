// JavaScript Document
// JavaScript Document
/****************************************************restore (start)************************************/
var g_timerRestore = 1000;

function gotoLoginWhileSystemUp() {
    if (DATA_READY.statusReady) {
        log.debug('Restore : system is up.');
        gotoPageWithoutHistory('home.html');
    }else {
        log.debug('Restore : system is down.');
    }
}

function do_restore() {
    var request = {
        Control: 2
    };
    var DEFAULT_GATEWAY_IP = '';
     // get current settings gateway address
    getConfigData('config/lan/config.xml', function($xml) {
        var ret = xml2object($xml);
        if ('config' == ret.type) {
            DEFAULT_GATEWAY_IP = ret.config.dhcps.ipaddress;
        }
    }, {
        sync: true
    }
    );
    var xmlstr = object2xml('request', request);
    log.debug('xmlstr = ' + xmlstr);
    saveAjaxData('api/device/control', xmlstr, function($xml) {
        log.debug('saveAjaxData successed!');
        var xmlstr = xml2object($xml);
        if (isAjaxReturnOK(xmlstr)) {
            ping_setPingAddress(DEFAULT_GATEWAY_IP);
            setTimeout(startPing, 50000);
        }
         else
        {
            parent.closeWaitingDialog();
            parent.showInfoDialog(common_failed);
            return false;
        }
    });
    //return false;
}

function restore() {
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
    /*After send "api/device/control" cmd, server will't response webui request.
     So delay 1 second.*/
    setTimeout(do_restore, g_timerRestore);
}

$(function() {
    $('#button_restore').bind('click', function() {
        parent.showConfirmDialog(system_hint_restore, restore, function() {});
        return false;
    });
});
/****************************************************restore (end)************************************/