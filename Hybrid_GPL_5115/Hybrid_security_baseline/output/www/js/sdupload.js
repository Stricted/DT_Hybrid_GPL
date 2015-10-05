// JavaScript Document
function showUploadStatus(upload_state) {
    var message = '';
    if (SD_FILE_UPLOAD_SUCCESS == upload_state) {
        message = sd_hint_upload_file_success;
    }
    else if (SD_FILE_UPLOAD_FAIL == upload_state) {
        message = sd_hint_upload_file_failed;
    }
    else if (SD_FILE_UPLOAD_FAIL_NOSPACE == upload_state) {
        message = sd_hint_not_enough_space_available;
    }
    else if (SD_FILE_UPLOAD_FAIL_NOSD == upload_state) {
        message = sd_label_no_sd_card;
    }
    else if (SD_FILE_UPLOAD_FAIL_2GB == upload_state) {
        message = sd_hint_file_size_over_2gb;
    }
    else if (SD_FILE_UPLOAD_FAILED_WRONG_FILENAME == upload_state) {
        message = sd_hint_wrong_file_name;
    }
    else if (SD_FILE_UPLOAD_FAILED_SD_REMOVED == upload_state) {
        message = sd_hint_sd_card_removed;
    }
    else {
        message = sd_hint_upload_file_failed;
    }

    initSdCardCapacity();
    showInfoDialog(message);
}

function getUploadState() {
    getAjaxData('api/sdcard/sdfilestate', function($xml) {
        var filestate_ret = xml2object($xml);
        if ('response' == filestate_ret.type) {
            var upload_state = filestate_ret.response.sduploadstatus.State;
            if (SD_FILE_UPLOADING == upload_state) {
                setTimeout(function() {getUploadState(); }, 2000);
            }
            else {
                //window.location.replace("sdcardsharing.html?result="+upload_state);
                closeWaitingDialog();
                showUploadStatus(upload_state);
                folderlists(folder_path.CurrentPath);
                parent.startLogoutTimer();
                $('#upload_dialog').remove();
            }
        }
    },
    {
        errorCB: function()
        {
            closeWaitingDialog();
            showInfoDialog(sd_hint_upload_file_failed);
            parent.startLogoutTimer();
            $('#upload_dialog').remove();
        }
    });
}

function doUploadFile() {
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
    showWaitingDialog(common_waiting, sd_label_uploading);
    $('#wait_dialog_btn').show().bind('click', function() {
        //window.location.reload();
        window.location.href = 'sdcardsharing.html?' + rfc3986_url_resolveSpecialChar(folder_path.CurrentPath).replace(/\//g, '%2F');
    });
    $('#cur_path').val('<' + folder_path.CurrentPath + '>');
    $('#page').val('sdcardsharing.html');
    $('#upload_year').val(upload_date.Year);
    $('#upload_month').val(upload_date.Month);
    $('#upload_day').val(upload_date.Day);
    $('#upload_hours').val(upload_date.Hour);
    $('#upload_minutes').val(upload_date.Min);
    $('#upload_secondes').val(upload_date.Sec);
    $('#upload_file_form').submit();
    getUploadState();
}

function cancle_upload()
{
    getAjaxData("api/sdcard/Check_file_exist");
}
function confirm_exist(obj) {
    $('#upload_dialog').css({'z-index': '-1000'}).hide();
    folder_path.CurrentPath = $('.micro_sd').children().html();
    var upload_xml = object2xml('request', obj);
    saveAjaxData('api/sdcard/Check_file_exist', upload_xml, function($xml) {
        var exist_state = xml2object($xml);
        if ('response' == exist_state.type) {
            if ('OK' == exist_state.response) {
                doUploadFile();
            }
            else {
                showInfoDialog(common_failed);
                return false;
            }
        }
        else if ('error' == exist_state.type && MACRO_FILE_EXIST == exist_state.error.code) {
            $('.dialog_close_btn').live('click',function() {
                getAjaxData("api/sdcard/Check_file_exist");
            });
            $("#pop_confirm").die("click");
            $("#pop_Cancel").die("click");
            showConfirmDialog(sd_hint_overwrite_existed_file, doUploadFile, cancle_upload, 0);
        }
        else {
            showInfoDialog(common_failed);
            return false;
        }
    });
}

function getFileName(szFile) {
    var szFileName = '';
    var fIndex = 0;

    fIndex = szFile.lastIndexOf('\\');
    if (fIndex > 0) {
        szFileName = szFile.substr(fIndex + 1);
    }
    else {
        fIndex = szFile.lastIndexOf('/');
        if (fIndex > 0) {
            szFileName = szFile.substr(fIndex + 1);
        }
        else {
            return szFile;
        }
    }

    return szFileName;
}

function uploadfile() {
    var upload_path = $('.micro_sd').children().html();
    upload_path = upload_path == '' ? '/' : upload_path;
    var upload_file = $('#fileField2').val();
    upload_file = getFileName(upload_file);
    upload_file = resolveXMLEntityReference(upload_file);
    var upload_folder = {
        CurrentPath: upload_path,
        FileName: upload_file
    };
    getAjaxData('api/sdcard/uploadflag', function($xml) {
        var res = xml2object($xml);
        if ('response' == res.type && 0 == res.response.Uploadflag)
        {
            confirm_exist(upload_folder);
        }
        else
        {
            showInfoDialog(IDS_sd_message_uploading_file_conflict);
            return false;
        }
    });
}
