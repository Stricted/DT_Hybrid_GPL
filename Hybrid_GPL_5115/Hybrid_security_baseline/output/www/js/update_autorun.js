// JavaScript Document
var g_index_pc_assistant_path = '';
var g_index_is_windows = false;
var g_index_is_mac = false;
var g_index_is_linux = false;

function getPcAssistantOperateSystem() {
    var ua = navigator.userAgent.toLowerCase();
    g_index_is_windows = (ua.indexOf('windows') != -1 || ua.indexOf('win32') != -1); //Window operate
    g_index_is_mac = (ua.indexOf('macintosh') != -1 || ua.indexOf('mac os x') != -1); //Mac operate
    g_index_is_linux = (ua.indexOf('linux') != -1);

}

function getAutorun() {
    getAjaxData('config/pcassistant/updateautorun.xml', function($xml) {
        var assistant_ret = xml2object($xml);
        if ('config' == assistant_ret.type)
        {
            if (g_index_is_mac)
            {
                g_index_pc_assistant_path = assistant_ret.config.macpath;
            }
            else if (g_index_is_linux)
            {
                g_index_pc_assistant_path = assistant_ret.config.linuxpath;
            }
            else
            {
                g_index_pc_assistant_path = assistant_ret.config.winpath;
            }
        }
        else
        {
            g_index_pc_assistant_path = '';
        }
    });
}

$(document).ready(function() {
    getAutorun();
    getPcAssistantOperateSystem();
    $('#pc_assistant_update_download').click(function() {
        if (!isButtonEnable('pc_assistant_update_download'))
        {
            return;
        }
        else
        {
            if (null != g_index_pc_assistant_path &&
                '' != g_index_pc_assistant_path &&
                ' ' != g_index_pc_assistant_path)
            {
                $('#index_istall_tray').attr('src', g_index_pc_assistant_path);
            }
            else
            {
                showInfoDialog(common_failed);
            }
        }
    });

});