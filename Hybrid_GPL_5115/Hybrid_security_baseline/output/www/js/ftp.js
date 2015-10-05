var FTP_STATUS_0 = '0';
var FTP_STATUS_1 = '1';
var FTP_STATUS_2 = '2';
var FTP_STATUS_3 = '3';
var FTP_STATUS_4 = '4';
var FTP_STATUS_5 = '5';
var CHARTLENGHTMAX_20 = '20';
var CHARTLENGHTMAX_64 = '64';
var CHART_6 = '6';
var CHART_1 = '1';
var CHART_65535 = '65535';
function getFirstStatusValue(listValue){
    $("#ftp_username").val(listValue.UserName);
    $("#ftp_downloadurl").val(listValue.DownLoadUrl);
    if('' != listValue.Port){
        $("#ftp_port").val(listValue.Port);
    }
    //$("#ftp_localpath").val(listValue.LocalPath);
}

function getHistoryStatusValue(status){
    var Label_status;
    switch(status){
        case FTP_STATUS_0:
                Label_status = ftp_status_success;
                break;
        case FTP_STATUS_1:
                Label_status = ftp_status_downloading;
                break;
        case FTP_STATUS_2:
                Label_status = ftp_status_failde_ae;
                break;
        case FTP_STATUS_3:
                Label_status = ftp_status_failde_tmd;
                break;
        case FTP_STATUS_4:
                Label_status = ftp_status_failde_nsl;
                break;
        case FTP_STATUS_5:
                Label_status = ftp_status_failde_de;
                break;
    }
    return Label_status;
}
function initPage(){
    getAjaxData('api/settings/ftpclient', function($xml){
        var ret = xml2object($xml);
        var new_line = '';
        if(('response' == ret.type) && (typeof(ret.response.ftps) != 'undefined') && ret.response.ftps.ftp){
               var ftpList =   ret.response.ftps.ftp;
               $("#table_download_hostory > tbody > tr:gt(0)").remove();
               if($.isArray(ftpList)){
                    getFirstStatusValue(ftpList[0]);
                    $.each(ftpList, function(i) {
                    new_line += "<tr><td>" + ftpList[i].UserName + "</td>";
                    new_line += "<td>" + ftpList[i].Port + "</td>";
                    new_line += "<td>" + ftpList[i].DownLoadUrl + "</td>";
                    new_line += "<td>" + ftpList[i].LocalPath + "</td>";
                    new_line += "<td>" + getHistoryStatusValue(ftpList[i].Status) + "</td></tr>";
                    });
               }else{
                    getFirstStatusValue(ftpList);
                    new_line += "<tr><td>" + ftpList.UserName + "</td>";
                    new_line += "<td>" + ftpList.Port + "</td>";
                    new_line += "<td>" + ftpList.DownLoadUrl + "</td>";
                    new_line += "<td>" + ftpList.LocalPath + "</td>";
                    new_line += "<td>" + getHistoryStatusValue(ftpList.Status) + "</td></tr>";
               }
               $("#table_download_hostory").append(new_line);
        }
    });
}

function getUsbshowDevice(){
    getAjaxData('api/settings/sambausb', function($xml) {
        var ret = xml2object($xml);
        var usb_deviceList = '';
        var newline = '';
        var usb_count = 0;
        if(('response' == ret.type) && ( 'undefined' != typeof(ret.response.usbs)) && ret.response.usbs.usb){
                usb_deviceList = ret.response.usbs.usb;
                var i = 0;
                if ($.isArray(usb_deviceList)) {
                    for(i=0;i<usb_deviceList.length;i++){
                        if(1 == usb_deviceList[i].Status){
                            if(0 == usb_count){
                                newline += "<option value='"+usb_count+"' selected>"+usb_deviceList[i].DiskInfo+"</option>";
                            } else {
                                newline += "<option value='"+usb_count+"'>"+usb_deviceList[i].DiskInfo+"</option>";
                            }
                            usb_count++;
                        }
                    }
                } else {
                    if(1 == usb_deviceList.Status){
                        newline += "<option value='"+usb_count+"' selected>"+usb_deviceList.DiskInfo+"</option>";
                    }
                }
                button_enable("apply",'1');
        } else {
                newline += "<option value='"+usb_count+"' selected>"+ label_nousb_device +"</option>";
                button_enable("apply",'0');
        }

        if(""!=newline){
            $('#ftp_device').append(newline);
            setTimeout(function() {$('#ftp_device').val(0)},100);
        }
    },{
        sync: true
    });
}

function isVaildValue(){
    clearAllErrorLabel();
    if(CHARTLENGHTMAX_20 < $('#ftp_username').val().length){
        showErrorUnderTextbox('ftp_username', hint_username_invalid);
        $('#ftp_username').focus();
        return false;
    }
    if(CHARTLENGHTMAX_20 < $('#ftp_password').val().length){
        showErrorUnderTextbox('ftp_password', hint_password_invalid);
        $('#ftp_password').focus();
        return false;
    }
    if(CHARTLENGHTMAX_64 < $('#ftp_localpath').val().length){
        showErrorUnderTextbox('ftp_localpath', hint_localpath_lenght_invalid);
        $('#ftp_localpath').focus();
        return false;
    }
    if(('/' == $('#ftp_localpath').val().charAt(0)) || ('/' == $('#ftp_localpath').val().charAt($('#ftp_localpath').val().length-1))){
        showErrorUnderTextbox('ftp_localpath', hint_localpath_chart_invalid);
        $('#ftp_localpath').focus();
        return false;
    }
    if(('' == $('#ftp_downloadurl').val().length) || (CHART_6 >= $('#ftp_downloadurl').val().length) ||
        ('ftp://' != $('#ftp_downloadurl').val().substr(0,6)) ||
        ('/' == $('#ftp_downloadurl').val().charAt($('#ftp_downloadurl').val().length-1))){
        showErrorUnderTextbox('ftp_downloadurl', hint_downloadurl_invalid);
        $('#ftp_downloadurl').focus();
        return false;
    }
    if(('' == $('#ftp_port').val()) || isNaN($('#ftp_port').val())){
        showErrorUnderTextbox('ftp_port', hint_port_invalid);
        $('#ftp_port').focus();
        return false;
    }
    if((CHART_1 > $('#ftp_port').val()) || (CHART_65535< $('#ftp_port').val())){
        showErrorUnderTextbox('ftp_port', hint_port_text_invalid);
        $('#ftp_port').focus();
        return false;
    }
    return true;
}

function onApply(){
    var submitObject = null;
    var localpath_value = '';
    if('' != $('#ftp_localpath').val()){
        localpath_value = $('#ftp_localpath').val() + '/';
    }
        submitObject = {
            ftps : {
                ftp : {
                    UserName : $('#ftp_username').val(),
                    PassWord : $('#ftp_password').val(),
                    Port : $('#ftp_port').val(),
                    DownLoadUrl : $('#ftp_downloadurl').val(),
                    LocalPath : $("#ftp_device option:selected").text() + '/' + localpath_value
                }
            }
        }
        var newXmlString = object2xml('request', submitObject);
        saveAjaxData('api/settings/ftpclient-download',newXmlString,function($xml){
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)){
                parent.showInfoDialog(common_success);
            }else{
                parent.showInfoDialog(common_failed);
            }
                initPage();
        });
}
$(document).ready(function() {
    button_enable("apply",'0');
    getUsbshowDevice();
    initPage();
    $('#apply').click(function() {
        if (!isButtonEnable('apply')) {
            return;
        }
        if(isVaildValue()){
            parent.showConfirmDialog(firewall_hint_submit_list_item, onApply);
        }
    });
});