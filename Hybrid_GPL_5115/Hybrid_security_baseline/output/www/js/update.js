var UPDATE_STATUS_IDLE = 10;
var UPDATE_STATUS_QUERYING = 11;
var UPDATE_NEWVERSION_FOUND = 12;
var UPDATE_STATUS_QUERYING_FAILED = 13;
var UPDATE_UP_TO_DATE = 14;
var UPDATE_DOWNLOAD_FAILED = 20;
var UPDATE_DOWNLOAD_PROGRESSING = 30;
var UPDATE_DOWNLOAD_PENDING = 31;
var UPDATE_DOWNLOAD_COMPLETE = 40;
var UPDATE_READYTO_UPDATE = 50;
var UPDATE_PROGRESSING = 60;
var UPDATE_FAILED_HAVEDATA = 70;
var UPDATE_FAILED_NODATA = 80;
var UPDATE_SUCCESSFUL_HAVEDATA = 90;
var UPDATE_SUCCESSFUL_NODATA = 100;
var RELOAD_PAGE_DELAY = 600;
var UPDATE_STATUS_INTERVAL = 3000;
var DOWNLOAD_INTERVAL = 300;
var UPDATE_PERIOD_DISABLE = 0;
var UPDATE_PERIOD_MAX = 360;
var FILE_END_NAME = "bin";

var g_status = null;
var g_update_value = null;
var g_updateComponent = null;

var g_download_percent = 10;
var g_update_percent = 10;


var g_process_terminate = "";
var g_timerReboot = 1000;
var current_type = '1';
var g_newversion_found = false;

var DEFAULT_GATEWAY_IP = '192.168.1.1';

var g_need_checkNewVersion = true;
var g_dialog_hide_updatecomfirm = true;
var g_dialog_hide_update = false;
var g_dailup_state = '0';

function do_reboot() {
    var request = {
        Control: 1
    };
    
    var xmlstr = object2xml('request', request);
    log.debug('xmlstr = ' + xmlstr);
    saveAjaxData('api/device/control', xmlstr, function($xml) {
        log.debug('saveAjaxData successed!');
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
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
}

function reboot() {
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
    /*After send "api/device/control" cmd, server will't response webui request.
     So delay 1 second.*/
    setTimeout(do_reboot, g_timerReboot);
}

function reloadPage()
{
    setTimeout(function() {window.location.reload();},RELOAD_PAGE_DELAY);
}

function update_displayNoNewVersionInfo()
{
    g_dialog_hide_update = true;
    parent.clearDialog();
    parent.$('.update_dialog').hide();
    parent.showInfoDialog(update_label_noversion);
    setTimeout(parent.clearDialog, UPDATE_STATUS_INTERVAL);
}
function update_displayQueryFailed()
{
    g_dialog_hide_update = true;
    parent.clearDialog();
    parent.$('.update_dialog').hide();
    parent.showInfoDialog(update_label_queryfailed);
    setTimeout(parent.clearDialog, UPDATE_STATUS_INTERVAL);
}

function update_displayNewVersionFoundInfo()
{
    g_dialog_hide_update = true;
    parent.clearDialog();
    parent.$('.update_dialog').hide();
    if(g_need_checkNewVersion)
    {
        button_enable("update_now", "1");
        g_need_checkNewVersion = false;
        g_download_percent = 10;
        download_comfirm_with_new_feature();
    }
    else
    {
        download_comfirm_with_new_feature();
    }
}

function update_send_info(ack)
{
    var req = object2xml("request", ack)
    saveAjaxData("api/online-update/comfirmupdate",req,function($xml){
        var ret = xml2object($xml);
        if(ret.type == 'response')
        {
            if(isAjaxReturnOK(ret.response))
            {
                log.debug("update start");
                parent.$('#DownloadProgressing_Cancel').hide();
                parent.$('#dialog_header_download_update').html(update_label_updating);
                update_showUpdating();
            }else{
                log.debug("update terminate");
            }
        }
        },{
        sysc :true,
        errorCB:function(){
            log.deug("update terminate");
            }
        });
}

function update_comfirm()
{
    parent.$('#update_download,#div_wrapper').hide();
    g_update_percent = 10;
    g_dialog_hide_update = false;
    parent.showupdateConfirmDialog(update_label_comfirm, function(){
        var ack={
            AckUpdate:0
            };
        parent.g_is_login_opened = true;
        update_send_info(ack);
        }, function(){
        var ack={
            AckUpdate:2
            };
        update_send_info(ack);
        
        },function(){
        var ack={
            AckUpdate:1
            };
        update_send_info(ack);  
        });
}

function download_send_info(ack)
{
    var req = object2xml("request", ack);
    saveAjaxData("api/online-update/ack-newversion",req,function($xml){
        var ret = xml2object($xml);
        if(ret.type=='response')
        {
            if(isAjaxReturnOK(ret))
            {
                if(ack.userAckNewVersion==0)
                {
                    log.debug("download start");
                    parent.showWaitingDialog(update_label_waiting,dialup_label_connecting);
                    setTimeout(function(){
                            parent.closeWaitingDialog();
                        $("#wait_table").remove();
                        update_checkUpdateStatus();
                        },2000);
                }
            }else{
                log.debug("download terminate");
            }
        }
        },{
        errorCB:function(){
            log.deug("download terminate");
            }
        });
}
function download_comfirm_with_new_feature()
{
    // get current changelog
    var new_version_feature = "";
    var current_changelog = "";
    getAjaxData('api/online-update/changelog', function($xml) {
            var ret = xml2object($xml);
            if ('response' == ret.type 
                && typeof(ret.response.changelogs) != "undefined"
                && typeof(ret.response.changelogs.changelog) != "undefined") 
            {
                new_version_feature = ret.response.changelogs.changelog;
            }
       }, {
           sync: true
       }
    );
    if(""!=new_version_feature)
    {
        current_changelog += "<br>";    
        if($.isArray(new_version_feature)) 
        {
            $.each(new_version_feature, function(i) {
                current_changelog += "<br>" + new_version_feature[i];
            });
        }
        else
        {
            current_changelog="<br>"+new_version_feature;
        }
    }

    parent.$('#update_download,#div_wrapper').hide();
    g_dialog_hide_update = true;
    parent.showConfirmDialog(update_label_newersion+current_changelog, function(){
        update_onClickUpdateNow();
        }, function(){
        var ack={
            userAckNewVersion:1
            };
        download_send_info(ack);           
        });
}
function download_comfirm()
{
    parent.$('#update_download,#div_wrapper').hide();
    g_dialog_hide_update = true;
    parent.showConfirmDialog(update_label_newersion, function(){
        var ack={
            userAckNewVersion:0
            };
        download_send_info(ack);
        }, function(){
        var ack={
            userAckNewVersion:1
            };
        download_send_info(ack);           
        });
}

function update_downloadSuccess()
{
    parent.$('#upload_success,#div_wrapper').hide();
    g_dialog_hide_update = true;
    parent.$('.update_dialog').hide();
    parent.reputPosition(parent.$('#upload_success'), parent.$('#div_wrapper'));
}

function updatefinish_timewait(){
    parent.$('.update_dialog').hide();
    parent.closeWaitingDialog();
    parent.showWaitingDialog(common_waiting, update_label_update_the_system);
    ping_setPingAddress(DEFAULT_GATEWAY_IP);
    setTimeout(function() {
        parent.closeWaitingDialog();
        parent.showWaitingDialog(common_waiting, common_success + " " + system_label_reboot + "...");
        }, 
        40000); 
    setTimeout(startPing, 50000); 
}
function update_clickSuccessCancel()
{
    var dialogLeft = $(window).width() / 2 - parent.$('.update_dialog').outerWidth() / 2;
    parent.$('#update_success_ok').bind('click', function() {
        parent.$('#upload_success,#div_wrapper').hide();
        reboot();
    });
}

function update_failed()
{
    g_dialog_hide_update = true;
    g_update_percent = 10;
    parent.$('.update_dialog').hide();
    parent.showConfirmDialog(update_label_failed, function(){
        reloadPage();
        });
}

function update_showUpdating()
{
    //show UpdateProgress
    if(true == g_dialog_hide_update)
    {
        return;
    }
    var persent = parent.formatFloat(parseInt(g_update_percent, 10) / 100, 2);
    var update_width = parent.$('.graph').width() - 8;
    update_width = parseInt(update_width * persent, 10);
    parent.$('.press').css({'width': update_width + 'px'});
    if(100 == g_update_percent)
    {
        g_update_percent = 0;
        clearTimeout(g_process_terminate);
    }
    g_update_percent += 10;
    parent.cancelLogoutTimer();
    //show Update dialog
    if (parent.$('#div_wrapper').size() < 1) {
        parent.$('.body_bg').before("<div id='div_wrapper'></div>");
    }
    parent.$('#dialog_header_download_update').html(update_label_updating);
    parent.$('#DownloadProgressing_Cancel').hide();
    parent.reputPosition(parent.$('#update_download'), parent.$('#div_wrapper'));
    parent.disableTabKey();
    g_process_terminate = setTimeout(update_showUpdating, DOWNLOAD_INTERVAL);
}

function update_showDownloading(currentProgress) {
    //show DownloadProgress
    var persent = parent.formatFloat(parseInt(currentProgress, 10) / 100, 2);
    var update_width = parent.$('.graph').width() - 8;
    update_width = parseInt(update_width * persent, 10);
    parent.$('.press').css({'width': update_width + 'px'});
    parent.cancelLogoutTimer();
    //show Download dialog
    if (parent.$('#div_wrapper').size() < 1) {
        $('.body_bg').before("<div id='div_wrapper'></div>");
    }
    parent.$('#dialog_header_download_update').html(IDS_update_label_downloading);
    parent.$('#DownloadProgressing_Cancel').hide();
    parent.reputPosition(parent.$('#update_download'), parent.$('#div_wrapper'));

    parent.disableTabKey();
}
function update_DownloadandUpdatingProcess()
{
    if(true == g_dialog_hide_update)
    {
        return;
    }
    //show DownloadProgress
    var persent = parent.formatFloat(parseInt(g_status.DownloadProgress, 10) / 100, 2);
    var update_width = parent.$('.graph').width() - 8;
    update_width = parseInt(update_width * persent, 10);
    parent.$('.press').css({'width': update_width + 'px'});
    parent.cancelLogoutTimer();
    //show Update dialog
    if (parent.$('#div_wrapper').size() < 1) {
        parent.$('.body_bg').before("<div id='div_wrapper'></div>");
    }
    parent.$('#dialog_header_download_update').html(update_label_updating);
    parent.$('#DownloadProgressing_Cancel').show();
    parent.reputPosition(parent.$('#update_download'), parent.$('#div_wrapper'));
    parent.disableTabKey();
    
}

function update_processStatus(CurrentComponentStatus)
{
    if(g_dailup_state == '0')
    {
        return;
    }
    
    switch (CurrentComponentStatus)
    {
        //The content in log debug is just for test, it can be ignored.
        case UPDATE_STATUS_IDLE:
            log.debug('online-update : update_processStatus(UPDATE_STATUS_IDLE)');
            break;
        case UPDATE_STATUS_QUERYING:
            log.debug('online-update : update_processStatus(UPDATE_STATUS_QUERYING)');
            break;
        case UPDATE_NEWVERSION_FOUND:
            if(g_need_checkNewVersion){
                update_displayNewVersionFoundInfo(); //show dialog new feature
            }
                button_enable("update_now", 1);        
                g_newversion_found = true;
                $('#search_image').hide();
                $('#newversion_information').show().text(update_label_newersion);
            return;
        case UPDATE_STATUS_QUERYING_FAILED:
            log.debug('online-update : update_processStatus(UPDATE_STATUS_QUERYING_FAILED)');
            parent.cancelLogoutTimer();
            update_displayQueryFailed();
            return;
        case UPDATE_UP_TO_DATE:
            log.debug('online-update : update_processStatus(UPDATE_UP_TO_DATE)');
            parent.cancelLogoutTimer();
            g_newversion_found = false;
            $('#search_image').hide();
            $('#newversion_information').show().text(update_label_noversion);
            //update_displayNoNewVersionInfo();//Told user there hasn't new version.
            break;
        case UPDATE_DOWNLOAD_FAILED:
            log.debug('online-update : update_processStatus(UPDATE_DOWNLOAD_FAILED)');
            parent.cancelLogoutTimer();
            update_failed();
            parent.startLogoutTimer();
            return;
        case UPDATE_DOWNLOAD_PROGRESSING:
            log.debug('online-update : update_processStatus(UPDATE_DOWNLOAD_PROGRESSING');
            update_DownloadandUpdatingProcess();
            parent.cancelLogoutTimer();
            $('#newversion_information').show().text(update_label_updating);
            break;
        case UPDATE_DOWNLOAD_PENDING:
            log.debug('online-update : update_processStatus(UPDATE_DOWNLOAD_PENDING)');
            update_displayNewVersionFoundInfo();
            return;
        case UPDATE_DOWNLOAD_COMPLETE:
            log.debug('online-update : update_processStatus(UPDATE_DOWNLOAD_COMPLETE)');
            if(!g_dialog_hide_updatecomfirm){
                update_comfirm();
                parent.cancelLogoutTimer();
            }
            break;
        case UPDATE_READYTO_UPDATE:
            log.debug('online-update : update_processStatus(UPDATE_READYTO_UPDATE)');
            break;
        case UPDATE_PROGRESSING:
            log.debug('online-update : update_processStatus(UPDATE_PROGRESSING)');
            parent.cancelLogoutTimer();
            update_DownloadandUpdatingProcess();
            parent.$('#DownloadProgressing_Cancel').hide();
            $('#newversion_information').show().text(update_label_updating);
            break;
        case UPDATE_FAILED_HAVEDATA:
            log.debug('online-update : update_processStatus(UPDATE_FAILED_HAVEDATA)');
            update_failed();
            parent.startLogoutTimer();
            return;
        case UPDATE_FAILED_NODATA:
            log.debug('online-update : update_processStatus(UPDATE_FAILED_NODATA)');
            update_failed();
            parent.startLogoutTimer();
            return;
        case UPDATE_SUCCESSFUL_HAVEDATA:
            log.debug('online-update : update_processStatus(UPDATE_SUCCESSFUL_HAVEDATA)');
            update_downloadSuccess();
            parent.cancelLogoutTimer();
            updatefinish_timewait();
            return;
        case UPDATE_SUCCESSFUL_NODATA:
            log.debug('online-update : update_processStatus(UPDATE_SUCCESSFUL_NODATA)');
            update_downloadSuccess();
            update_clickSuccessCancel();
            parent.startLogoutTimer();
            return;
        default:
            return;
    }
    
}

function update_check_newverison()
{
    var updateStatus = UPDATE_STATUS_IDLE;
    if(g_status != null){
        updateStatus = parseInt(g_status.CurrentComponentStatus, 10);
    }
    if (updateStatus == UPDATE_STATUS_QUERYING || updateStatus == UPDATE_DOWNLOAD_PROGRESSING || updateStatus == UPDATE_PROGRESSING)
    {
      return;
    }
    
    saveAjaxData('api/online-update/check-new-version',"",
        function($xml) {
            var ret = xml2object($xml);
            if (ret.type == 'response')
            {
               if(isAjaxReturnOK(ret))
               {
                    $('#search_image').show();
                    $('#newversion_information').hide();
               }else{
                    log.debug("check failed");
                    setTimeout(update_check_newverison, UPDATE_STATUS_INTERVAL);
               }
            }
        },
        {
            errorCB: function() {
                setTimeout(update_check_newverison, UPDATE_STATUS_INTERVAL);
            }
        }
    );
}

function update_checkUpdateStatus() {
    getAjaxData('api/online-update/status',
        function($xml) {
            var update_ret = xml2object($xml);
            if (update_ret.type == 'response')
            {
                g_status = update_ret.response;
                var updateStatus = parseInt(g_status.CurrentComponentStatus, 10);
                update_processStatus(updateStatus);
            }
        },
        {
            errorCB: function() {
                setTimeout(update_checkUpdateStatus, UPDATE_STATUS_INTERVAL);
            }
        }
    );
}

function update_onClickUpdateNow() {
    var updateStatus = parseInt(g_status.CurrentComponentStatus, 10);
    if (updateStatus != UPDATE_STATUS_QUERYING && updateStatus != UPDATE_DOWNLOAD_PROGRESSING && updateStatus != UPDATE_PROGRESSING)
    {
        clearAllErrorLabel();
        $('#update_now').unbind('click');
        var ack={
            userAckNewVersion:0
        };
        var req = object2xml("request", ack);
        saveAjaxData('api/online-update/ack-newversion',req,function($xml) {
            var update_ret = xml2object($xml);
            if (update_ret.type == 'response')
            {
                log.debug('update data successfull');
                g_dialog_hide_update = false;
            }
            $('#update_now').bind('click', update_onClickUpdateNow);
        },{ sync: true });
        parent.cancelLogoutTimer();
        parent.g_is_login_opened = true;
        update_DownloadandUpdatingProcess();
    }
    else
    {
        clearAllErrorLabel();
        showErrorUnderTextbox('update_now', IDS_update_updateing_try_again);
    }
}

function update_cancelDownloading() {
    g_dialog_hide_update = true;
    if (isButtonEnable_Parent('DownloadProgressing_Cancel'))
    {
        parent.$('#update_download,#div_wrapper').hide();
        saveAjaxData('api/online-update/cancel-downloading', '', function($xml) {
            var return_ret = xml2object($xml);
            if (return_ret.type == 'response')
            {
                log.debug('update data successfull');
            }
        });
    }
}

function initPage()
{
    button_enable("url_auto_apply", 0);
   
    getAjaxData("api/online-update/configuration", function($xml){
        var ret = xml2object($xml);
        if(ret.type=="response")
        {
            if(ret.response.autoUpdateInterval == 0) 
            {
                $('input[name=auto_check_update][value=0]').attr('checked', true);
            }
            else 
            {
                $('input[name=auto_check_update][value=1]').attr('checked', true);
            }
            $("#up_url").val(ret.response.updateURL);
        }else{
            log.dubug("get statics wrong");
        }
    }, {
        sync: true
    });
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
}

function isValidUrlName(url)
{
    var i=0;
    var j=0;
    var invalidArray = new Array();
    invalidArray[i++] = "www";
    invalidArray[i++] = "com";
    invalidArray[i++] = "org";
    invalidArray[i++] = "net";
    invalidArray[i++] = "edu";
    invalidArray[i++] = "www.";
    invalidArray[i++] = ".com";
    invalidArray[i++] = ".org";
    invalidArray[i++] = ".net";
    invalidArray[i++] = ".edu";

    if (url=='')
    {
        return false;
    }
    
    if (isValidAscii(url) != '')
    {
        return false;
    }

    for (i = 0; i < url.length; i++)
    {
        if (url.charAt(i) == ' ')
        {
             return false;
        }
        if (url.charAt(i) == '\\')
        {
             return false;
        }
    }

    if ((url.length < 3 && url.length > 0) || url.length > 63 )
    {
        return false;
    }
    for(j=0; j< invalidArray.length; j++)
    {
        if (url == invalidArray[j])
        {
            return false;
        } 
    }
    return true;
}

function checkUrlandPeriod(){
    if(isValidUrlName($.trim($('#up_url').val()))==false)
    {
        showErrorUnderTextbox('up_url', samba_input_invalid);
        return false;
    }
    return true;
}

function postURLandPeriod(){
    var auto_update = "1";
    if($("#auto_check_update_enable").get(0).checked) 
    {
        auto_update = "1";
    }
    else
    {
        auto_update = "0";
    }
    var val_req ={
        autoUpdateInterval:auto_update,
        updateURL:$("#up_url").val()
    };
    req = object2xml("request",val_req);
    saveAjaxData("api/online-update/configuration",req,function($xml){
        ret = xml2object($xml);
        if(ret.type == "response")
        {
            if(isAjaxReturnOK(ret))
            {
                parent.showInfoDialog(common_success);
                button_enable("url_auto_apply", 0);
            }else{
                parent.showInfoDialog(common_failed);
                initPage();
                
            }
        }else{
           log.debug("statics is wrong");
        }
    });
}

function checkBtndisable()
{
    g_dailup_state = '0';
    if("901" == parent.G_MonitoringStatus.response.ConnectionStatus 
     || '2' == parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus
     || '2'== parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus
     || '2'== parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus 
     ){
        if(('1'== current_type) && ('2' != parent.G_MonitoringStatus.response.ServiceStatus)){
            button_enable("check_for_update", 0);
            button_enable("update_now", 0); 
            $('#newversion_information').show().text(update_label_connect_failed);
        }else{
            update_checkUpdateStatus();
            button_enable("check_for_update", 1);
            if(g_newversion_found){
                button_enable("update_now", 1);        
            }else{
                button_enable("update_now", 0);        
            } 
            g_dailup_state = '1';
        }
    }else{
        button_enable("check_for_update", 0);
        button_enable("update_now", 0);
        $('#newversion_information').show().text(update_label_connect_failed);
    }
    setTimeout(checkBtndisable,3000);
}
function getConnInfo() {
     getAjaxData('api/connection/connection', function($xml) {
        var conn_ret = xml2object($xml);
        if (conn_ret.type == 'response') {
             current_type= conn_ret.response.ConnectionDevice;
        } else {
            log.error('Error, no data');
        }
    }, {
        sync: true
    });
}
/**********************************After loaded (common)************/
$(document).ready(function() {
    getAjaxData('api/online-update/status',
        function($xml) {
            var update_ret = xml2object($xml);
            if (update_ret.type == 'response')
            {
                g_status = update_ret.response;
            }
        },{sync: true}
    );
    button_enable("url_auto_apply", "0");
    button_enable("update_now", "0");
    getConnInfo();
    initPage();
    checkBtndisable();
    if(isButtonEnable("check_for_update")){
        update_check_newverison();
    }else{
        $('#check_new').text(hilink_label_connect_failed);
    }
    $('input').bind('change input paste cut keydown', function(){
        button_enable('url_auto_apply', '1');
        clearAllErrorLabel();
    });

    $("#url_auto_apply").click(function(){
        clearAllErrorLabel();
        if(isButtonEnable("url_auto_apply")&&checkUrlandPeriod())
        {
            postURLandPeriod();
        }else{
            return ;
        }
    });

    $("#update_type").bind("change",function(){
        if($("#update_type").val()== '1')
        {
            $("#on_update_method").hide();
            $("#uploadLocalFile").show();
        }else{
            $("#on_update_method").show();
            $("#uploadLocalFile").hide();
        }
    });
    $("#auto_check_update_enable,#auto_check_update_disable").bind("change",function(){
        if($("#auto_check_update_enable").get(0).checked) 
        {
            $('#auto_update_cycle').show();
            $("#up_auto").val('');
        }
        else
        {
            $('#auto_update_cycle').hide();
        }
    });
        $('#check_for_update').bind('click', function(){
            if(isButtonEnable("check_for_update"))
            {
                g_need_checkNewVersion = true;
                update_check_newverison();
            }
        });
        $('#update_now').bind('click', function(){
            if(isButtonEnable("update_now"))
            {            
                update_onClickUpdateNow();
            }
        });
    parent.$('#DownloadProgressing_Cancel').unbind().bind("click",update_cancelDownloading);
});
