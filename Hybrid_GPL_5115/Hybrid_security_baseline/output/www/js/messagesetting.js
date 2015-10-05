// JavaScript Document
var sms_setting_config = {
    SaveMode: '',
    Validity: '',
    Sca: '',
    UseSReport: '',
    SendType: ''
};
var g_smsFeature = null;
var g_sms_userreport = null;
var sms_setting_configSaveMode_0 = '0';
var sms_setting_configSaveMode_1 = '1';
var sms_setting_configSaveMode_2 = '2';
var sms_setting_configSaveMode_3 = '3';

var sms_setting_configUseSReport_0 = '0';
var sms_setting_configUseSReport_1 = '1';
var g_sms_center_enabled = false;
var g_sms_priority_enabled = false;
var g_sms_validity_enabled = false;
/*
10752 －－ 最大值
168  －－ 1 周
24   －－－ 1天
12   －－－ 12小时
1    －－－  1小时
其他值 －－ 按接口文档规定的默认12小时显示
 */
var SMS_VALIDITY_1WEEK = '168';
var SMS_VALIDITY_1DAY = '24';
var SMS_VALIDITY_12HOURS = '12';
var SMS_VALIDITY_1HOUR = '1';
var SMS_VALIDITY_MAX_LIMIT = '10752';
var sms_setting_configSendType_0 = '0';
var sms_setting_configSendType_1 = '1';
function sms_setting_initPage() {
    getAjaxData('api/sms/config', function($xml) {
        var sms_setting_ret = xml2object($xml);
        if (sms_setting_ret.type == 'response') {
            sms_setting_config.SaveMode = sms_setting_ret.response.SaveMode;
            sms_setting_config.Validity = sms_setting_ret.response.Validity;
            sms_setting_config.Sca = sms_setting_ret.response.Sca;
            sms_setting_config.UseSReport = sms_setting_ret.response.UseSReport;
            sms_setting_config.SendType = sms_setting_ret.response.SendType;

            g_sms_userreport = (sms_setting_ret.response.UseSReport == '1') ? true : false;

            switch (sms_setting_config.SaveMode) {
                case sms_setting_configSaveMode_0:
                    parent.SMS_BOXTYPE_INBOX = SMS_BOXTYPE_LOCAL_INBOX;
                    $('#sms_savemode').val(sms_label_save_local);
                    $("#choose_mode_show").val(sms_setting_config.SaveMode);
                    break;
                case sms_setting_configSaveMode_1:
                    parent.SMS_BOXTYPE_INBOX = SMS_BOXTYPE_SIM_INBOX;
                    $('#sms_savemode').val(sms_label_save_simcard);
                    $("#choose_mode_show").val(sms_setting_config.SaveMode);
                    break;
                case sms_setting_configSaveMode_2:
                    parent.SMS_BOXTYPE_INBOX = SMS_BOXTYPE_MIX_INBOX;
                    $('#sms_savemode').val(sms_label_save_simcard_first);
                    break;
                case sms_setting_configSaveMode_3:
                    parent.SMS_BOXTYPE_INBOX = SMS_BOXTYPE_MIX_INBOX;
                    $('#sms_savemode').val(sms_label_save_local_first);
                    break;
                default:
                    parent.SMS_BOXTYPE_INBOX = SMS_BOXTYPE_LOCAL_INBOX;
                    $('#sms_savemode').val(sms_label_save_local);
                    break;
            }
            parent.SMS_BOXTYPE_SENT    = parent.SMS_BOXTYPE_INBOX + 1;
            parent.SMS_BOXTYPE_DRAFT   = parent.SMS_BOXTYPE_INBOX + 2;
            parent.g_sms_boxType = parent.SMS_BOXTYPE_INBOX;   
            parent.g_sms_smsListArray.BoxType = g_sms_boxType;
			 

            switch (sms_setting_config.Validity) {
                case SMS_VALIDITY_1WEEK:
                case SMS_VALIDITY_1DAY:
                case SMS_VALIDITY_12HOURS:
                case SMS_VALIDITY_1HOUR:
                case SMS_VALIDITY_MAX_LIMIT:
                    $('#sms_validity').val(sms_setting_config.Validity);
                    break;
                default:
                    $('#sms_validity').val(SMS_VALIDITY_12HOURS);
                    break;
            }
            
            $('#sms_sca').val(sms_setting_config.Sca);
            
            
            switch (sms_setting_config.UseSReport) {
                case sms_setting_configUseSReport_0:
                    $('input[name=usesreport]').eq(1).attr('checked', 'checked');
                    break;
                case sms_setting_configUseSReport_1:
                    $('input[name=usesreport]').eq(0).attr('checked', 'checked');
                    break;
                default:
                    $('input[name=usesreport]').eq(1).attr('checked', 'checked');
                    break;
            }

            switch (sms_setting_config.SendType) {
                case sms_setting_configSendType_0:
                    $('#sms_sendtype').val(sms_label_sendtype_send);
                    break;
                case sms_setting_configSendType_1:
                    $('#sms_sendtype').val(sms_label_sendtype_send_save);
                    break;
            }
        }
        else {
            // showInfoDialog(common_failed);
            log.error('MESSAGESETTING: get api/sms/config data error');
        }
    }, {
        errorCB: function() {
            // showInfoDialog(common_failed);
            log.error('MESSAGESETTING: get api/sms/config file failed');
        }
    });

}

function sms_setting_validateSca() {
    if(!g_sms_center_enabled) {
        return true;
    }
    var sca_content = $.trim($('#sms_sca').val());
    var patrn = /^([0-9]|\*|\#|\+){1,20}$/;
    if(0 == sca_content.indexOf("+"))
    {
        patrn = /^([0-9]|\*|\#|\+){1,21}$/;
    }
    if ('' == sca_content || null == sca_content || !patrn.exec(sca_content)) {
        showErrorUnderTextbox('sms_sca_error', sms_hint_sms_center_number_invalid);
        $('#sms_sca').focus().select();
        return false;
    }
    return true;
}

function sms_setting_apply() {
    if (!isButtonEnable('button_apply')) {
        return;
    }
    clearAllErrorLabel();
    if (sms_setting_validateSca()) {
        var savemodemodify = (sms_setting_config.SaveMode == $("#choose_mode_show").val());
        sms_setting_config.Validity = $('#sms_validity').val();
        sms_setting_config.Sca = $('#sms_sca').val();
        sms_setting_config.SaveMode = "1";
        sms_setting_config.UseSReport = (g_sms_userreport == true) ? '1' : '0';

        switch ($('#sms_sendtype').val()) {
            case sms_label_sendtype_send:
                sms_setting_config.SendType = sms_setting_configSendType_0;
                break;
            case sms_label_sendtype_send_save:
                sms_setting_config.SendType = sms_setting_configSendType_1;
                break;
        }
        var sms_setting_xml = object2xml('request', sms_setting_config);
        button_enable('button_apply', '0');
        if(!savemodemodify){
            parent.showWaitingDialog(common_waiting, sms_error_label1);
        }
        saveAjaxData('api/sms/config', sms_setting_xml, function($xml) {
            parent.closeWaitingDialog();
            var sms_setting_ret = xml2object($xml);
            if (isAjaxReturnOK(sms_setting_ret)) {
                parent.showInfoDialog(common_success);
            }
            else {
                button_enable('button_apply', '1');
                parent.showInfoDialog(common_failed);
                log.error('MESSAGESETTING: post api/sms/cofig file failed');

            }
            sms_setting_initPage();
        }, {
            errorCB: function() {
                parent.showInfoDialog(common_failed);
                log.error('MESSAGESETTING: post api/sms/cofig file failed');
            }
        });
    }
}

function sms_onChangeUserReport() {
    g_sms_userreport = $(this).get(0).value == '0';
}

function sms_changevalidity(val) {
    var value = val;

    return;
}

function sms_createvalidity() {

    var smsvalidityHTML = '';

    smsvalidityHTML = '<option value=' + SMS_VALIDITY_MAX_LIMIT + '>' + common_maximum + '</option>';
    smsvalidityHTML += '<option value=' + SMS_VALIDITY_1WEEK + '>' + common_week.replace('%d', 1) + '</option>';
    smsvalidityHTML += '<option value=' + SMS_VALIDITY_1DAY + '>' + common_day.replace('%d', 1) + '</option>';
    smsvalidityHTML += '<option value=' + SMS_VALIDITY_12HOURS + '>' + '12 ' + dialup_label_hours + '</option>';
    smsvalidityHTML += '<option value=' + SMS_VALIDITY_1HOUR + '>' + '1 ' + dialup_label_hour + '</option>';

    $('#sms_validity').append(smsvalidityHTML);


    $('#sms_validity').change(function() {
        sms_changevalidity($('#sms_validity').val());
    });
}
function sms_setDisplay() {
    getConfigData("config/sms/config.xml", function($xml) {
        g_smsFeature = _xml2feature($xml);
        g_sms_center_enabled = g_smsFeature.sms_center_enabled == "1" ? true : false;
        g_sms_validity_enabled = g_smsFeature.sms_validity_enabled == "1" ? true : false;
        if(g_sms_center_enabled) {
            $('#sms_center_number_tr').show();
        }
        else {
            $('#sms_center_number_tr').hide();
        }
        
        if(g_sms_validity_enabled) {
            $('#sms_validity_tr').show();
        }
        else {
            $('#sms_validity_tr').hide();
        }
        
    }, {
        sync : true
    });
}

$(document).ready(function() {
    button_enable('button_apply', '0');
    $('#button_apply').click(function() {
        sms_setting_apply();
    });
    
    $("#choose_mode_show").change(function() {
        button_enable('button_apply', '1');
    });
    
    $('input').bind('change input paste cut keydown', function() {
        button_enable('button_apply', '1');
    });
    $('select').bind('change input paste cut keydown', function() {
        button_enable('button_apply', '1');
    });
    $('.input_select').click(function() {
        button_enable('button_apply', '1');
    });
    var savemode_list_li = "<li><a href='javascript: void();'>" + sms_label_save_local + "</a></li><li><a href='javascript: void();'>" + sms_label_save_simcard + "</a></li><li><a href='javascript: void();'>" + sms_label_save_simcard_first + "</a></li><li><a href='javascript: void();'>" + sms_label_save_local_first + '</a></li>';

    $('#save_mode_list').html(savemode_list_li);

    var senttype_list_li = "<li><a href='javascript: void();'>" + sms_label_sendtype_send + "</a></li><li><a href='javascript: void();'>" + sms_label_sendtype_send_save + '</a></li>';

    $('#senttype_list').html(senttype_list_li);

    $('input[name=usesreport]').bind('click', sms_onChangeUserReport);

    sms_createvalidity();
    sms_setting_initPage();
    sms_setDisplay();

});