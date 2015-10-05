var g_logsetting_moduleLength = 0;
var g_logsetting_registerModule = '';
var g_logsetting_registerlog = '';
var g_logsetting_maskBChar = '';
var g_logsetting_maskBStr = '';
var g_logsetting_maskBStrLength = 0;
var g_logsetting_comlresslogfile = '';
var LOGSETTING_MASK_5 = 5;
var LOGSETTING_CHECKBO0X_1 = '1';
var LOGSETTING_CHECKBO0X_2 = '2';
var LOGSETTING_CHECKBO0X_3 = '3';
var LOGSETTING_CHECKBO0X_0 = '0';
$(document).ready(function() {

    initPage();
    button_enable('apply_button', '0');

    $('input').live('click', function() {
        button_enable('apply_button', '1');
    });

    /*$("#export_log_to_pc").click(function(){
        button_enable("apply_button", "0");
        var export_log = {
            content: ""
        };
        var xmlstr_module = object2xml("request", export_log);
        saveAjaxData("api/device/compresslogfile", xmlstr_module, function($xml){
            var ret = xml2object($xml);
            if (!isAjaxReturnOK(ret))
            {
                var iframe_src = ret.response.LogPath;
                $("#export_file_dialog").attr("src", iframe_src);
                button_enable("apply_button", "1");
            }
        });
    });*/

    $('#export_file_to_pc').click(function() {
        if (!isButtonEnable('export_file_to_pc'))
        {
            return;
        }
        getLogFilePath();
    });

});
function initPage() {
    getAjaxData('api/device/logsetting', function($xml) {
        var ret = xml2object($xml);
        var i = 0;
        g_logsetting_registerlog = ret.response;
        g_logsetting_moduleLength = g_logsetting_registerlog.Modules.Module.length;
        g_logsetting_registerModule = g_logsetting_registerlog.Modules.Module;
        var where = g_logsetting_registerlog.Where;
        switch (where)
        {

            case LOGSETTING_CHECKBO0X_1:
                    $('#file').get(0).checked = false;
                    $('#stoud').get(0).checked = true;
                    break;
            case LOGSETTING_CHECKBO0X_2:
                    $('#file').get(0).checked = true;
                    $('#stoud').get(0).checked = false;
                    break;
            case LOGSETTING_CHECKBO0X_3:
                    $('#file').get(0).checked = true;
                    $('#stoud').get(0).checked = true;
                    break;
            default:
                    $('#file').get(0).checked = false;
                    $('#stoud').get(0).checked = false;
                    break;
        }
        $('#logfilter').html('');
        for (i = 0; i < g_logsetting_moduleLength; i++)
        {
            var maskBAddBit = '';
            var name = g_logsetting_registerModule[i].desc;
            $('#logfilter').append('<tr><td>' + (i + 1) + "</td><td height='32' width='200' align='center'>" + name + "</td><td width='400' class='f_color' align='center'>Fatal<input type='checkbox' id='mask0_" + i + "' class='log_click'/>" +
                "Error<input type='checkbox' id='mask1_" + i + "' class='log_click'/>" +
                "Warn<input type='checkbox' id='mask2_" + i + "' class='log_click'/>" +
                "Info<input type='checkbox' id='mask3_" + i + "' class='log_click'/>" +
                "Debug<input type='checkbox' id='mask4_" + i + "' class='log_click'/></td> </tr>");
            g_logsetting_maskBStr = parseInt(g_logsetting_registerModule[i].mask, 10).toString(2);
            g_logsetting_maskBStrLength = g_logsetting_maskBStr.length;
            if (g_logsetting_maskBStrLength < LOGSETTING_MASK_5)
            {
                var j = 0;
                for (j = 0; j < LOGSETTING_MASK_5 - g_logsetting_maskBStrLength; j++)
                {
                    maskBAddBit += 0;
                }

            }
            g_logsetting_maskBStr = g_logsetting_maskBStr + maskBAddBit;
            g_logsetting_maskBChar = g_logsetting_maskBStr.split('');
            var k = 0;
            for (k = 0; k < g_logsetting_maskBChar.length; k++)
            {
                $('#mask' + k + '_' + i).get(0).checked = (g_logsetting_maskBChar[k] == 1) ? true : false;
                //alert($("#mask"+k).get(0).checked );
            }

        }

    });
}

function desableAll() {
    button_enable('apply_button', '1');
    var i = 0;
    for (i = 0; i < g_logsetting_moduleLength; i++)
    {
            $('#mask0_' + i).get(0).checked = false;
            $('#mask1_' + i).get(0).checked = false;
            $('#mask2_' + i).get(0).checked = false;
            $('#mask3_' + i).get(0).checked = false;
            $('#mask4_' + i).get(0).checked = false;
    }
}

function onApply()
{

    if ($('#file').get(0).checked == false && $('#stoud').get(0).checked == true)
    {
        g_logsetting_registerlog.Where = LOGSETTING_CHECKBO0X_1;
    }
    else if ($('#file').get(0).checked == true && $('#stoud').get(0).checked == false)
    {
        g_logsetting_registerlog.Where = LOGSETTING_CHECKBO0X_2;
    }
    else if ($('#file').get(0).checked == true && $('#stoud').get(0).checked == true)
    {
        g_logsetting_registerlog.Where = LOGSETTING_CHECKBO0X_3;
    }
    else if ($('#file').get(0).checked == false && $('#stoud').get(0).checked == false)
    {
        g_logsetting_registerlog.Where = LOGSETTING_CHECKBO0X_0;
    }
    var i = 0;
    for (i = 0; i < g_logsetting_moduleLength; i++) {
        var m = '';
        var k = 0;
        for (k = g_logsetting_maskBChar.length - 1; k > -1; k--)
        {
            m += $('#mask' + k + '_' + i).get(0).checked == true ? 1 : 0;
        }

       g_logsetting_registerModule[i].mask = parseInt(m, 2).toString(10);
    }
    button_enable('apply_button', '0');
    var xmlstr_module = object2xml('response', g_logsetting_registerlog);
    saveAjaxData('api/device/logsetting', xmlstr_module, function($xml) {
        var ret = xml2object($xml);
        var saveState = ret.response;
        if (!isAjaxReturnOK(ret))
        {
            button_enable('apply_button', '0');
            showInfoDialog(common_success);
        }
        else
        {
            initPage();
        }
    });
}

function getLogFilePath() {
    getAjaxData('api/device/compresslogfile', function($xml) {
        var file_ret = xml2object($xml);
        if ('response' == file_ret.type)
        {
            if (null != file_ret.response.LogPath &&
            '' != file_ret.response.LogPath &&
            ' ' != file_ret.response.LogPath)
            {
                $('#fileName').val(file_ret.response.LogPath);
                //window.open(file_ret.response.LogPath,'_self');
                var w = window.open("","_self");
                w.location.href=file_ret.response.LogPath;
            }
            else
            {
            }
        }
        else
        {
        }
    },{
        sync: true
    });
}