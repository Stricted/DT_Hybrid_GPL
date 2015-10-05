// JavaScript Document
/****************************************************restore (start)************************************/
function doSubmit() {
    document.forms[0].submit();
    setTimeout(function(){check_bak_state();},1000);
}
    
var g_bak_state = 1;
function check_bak_state()
{
    getAjaxData('api/device/control', function($xml) {
    var bak_state = xml2object($xml);
        if (bak_state.type == 'response') {
            if(bak_state.response.Control == 0)
            {
                parent.closeWaitingDialog();
                parent.showInfoDialog(common_failed);
                g_bak_state = 0;
            }
            else
            {
                setTimeout(function(){check_bak_state();},1000);
            }
        }
    });
}
function gotoLoginWhileSystemUp() {
    log.debug('configruation : system is up.');
    gotoPageWithoutHistory('home.html');
}

function restore() {
    g_bak_state = 1;
    parent.cancelLogoutTimer();
    var current_date = new Date();
    var upload_date = {
        Year: current_date.getFullYear(),
        Month: current_date.getMonth() + 1,
        Day: current_date.getDate(),
        Hour: current_date.getHours(),
        Min: current_date.getMinutes(),
        Sec: current_date.getSeconds()
    };
    $('#cur_path').val($('#restore_file').val());
    $('#page').val('configuration.html');
    $('#upload_year').val(upload_date.Year);
    $('#upload_month').val(upload_date.Month);
    $('#upload_day').val(upload_date.Day);
    $('#upload_hours').val(upload_date.Hour);
    $('#upload_minutes').val(upload_date.Min);
    $('#upload_secondes').val(upload_date.Sec);
    $('#randomNum').val(Math.round(Math.random() * 10000));
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
    setTimeout(function() {
        if(g_bak_state == 1)
        {
            parent.closeWaitingDialog();
            gotoLoginWhileSystemUp();
        }
    }, 60000);
    setTimeout(doSubmit, 20);
}

$(function() {
    
    var query_string = getQueryStringByName('result');
    if ('error' == query_string) {
        parent.showInfoDialog(common_failed);
    }else if ('success' == query_string) {
        parent.cancelLogoutTimer();
        parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
        setTimeout(function() {
            gotoLoginWhileSystemUp();
        }, 30000);
    }

    $('#save_button').bind('click', function() {
        if (!isButtonEnable('save_button')) {
            return;
        }

        //Object for store oprate number.
        var cfg_backup_request = {
            Control: 3
        };

        parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);

        var cfg_backup_xml = object2xml('request', cfg_backup_request);
        saveAjaxData('api/device/control', cfg_backup_xml, function($xml) {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                parent.closeWaitingDialog();
                window.location.href = '../../nvram.bak';
            } else {
                parent.closeWaitingDialog();
                parent.showInfoDialog(common_failed);
            }
        });
    });
    button_enable('restore_button', '0');
    $('#restore_file').bind('change', function() {
        button_enable('restore_button', '0');
        var filename = this.value;
        var reg = /\.bak$/i;
        if (0 == filename.length) {
            clearAllErrorLabel();
            $('#restore_button').unbind('click');
        }else if (reg.test(filename)) {
            button_enable('restore_button', '1');
            clearAllErrorLabel();
            $('#restore_button').bind('click', function() {
                parent.showConfirmDialog(dhcp_hint_operation_restart_device, restore, function() {});
            });
        }else {
            clearAllErrorLabel();
            button_enable('restore_button', '0');
            showErrorUnderTextbox('restore_file', system_hint_file_name_empty);
            $('#restore_button').unbind('click');
        }
    });
});
/****************************************************restore (end)************************************/