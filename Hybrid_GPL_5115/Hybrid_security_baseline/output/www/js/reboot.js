// JavaScript Document
var g_timerReboot = 1000;

function gotoLoginWhileSystemUp() {

    if (DATA_READY.statusReady) {
        log.debug('Reboot : system is up.');
        gotoPageWithoutHistory('login.html');
    }else {
        log.debug('Reboot : system is down.');
    }
}

function do_reboot() {
    var request = {
        Control: 1
    };
    var DEFAULT_GATEWAY_IP = '';
     // get current settings gateway address
    getAjaxData('api/dhcp/settings', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type) {
            DEFAULT_GATEWAY_IP = ret.response.DhcpIPAddress;
        }
    }, {
        sync: true
    }
    );
    var xmlstr = object2xml('request', request);
    log.debug('xmlstr = ' + xmlstr);
    saveAjaxData('api/device/control', xmlstr, function($xml) {
        log.debug('saveAjaxData successed!');
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            ping_setPingAddress(DEFAULT_GATEWAY_IP);
            setTimeout(startPing, 30000);
        }
        else
        {
            parent.closeWaitingDialog();
            parent.showInfoDialog(common_failed);
            return false;
        }
    });
}

function reboot() {
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
    /*After send "api/device/control" cmd, server will't response webui request.
     So delay 1 second.*/
    setTimeout(do_reboot, g_timerReboot);
}

$(function() {
    $('#button_reboot').bind('click', function() {
        parent.showConfirmDialog(system_hint_reboot, reboot, function() {});
        return false;
    });
});