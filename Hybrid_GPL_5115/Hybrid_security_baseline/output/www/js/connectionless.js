// JavaScript Document
var MANUAL_CONNECTION = '1';
var INVALID_SIM_CARD = '255';
var SIM_CARD_READY = '257';
var PIN_CODE_REQUIRED = '260';
var PUK_CODE_REQUIRED = '261';
var NET_ROAMING_STATUS = '1';

var monitoring_status = null;

var network_type = new Array(dialup_label_no_service, dialup_label_gsm, dialup_label_gprs, dialup_label_edge, dialup_label_wcdma, dialup_label_hsdpa, dialup_label_hsupa, dialup_label_hspa, dialup_label_scdma, dialup_label_hspa_plus, dialup_label_evdo0, dialup_label_evdoa, dialup_label_evdob);

addStatusListener('getStatusFromUpdatingIcon()');
$(document).ready(function() {

    $('#btn_continue').live('click', function() {
        onContinue();
        return false;
    });

    $('#manual_connect').live('click', function() {
        getAjaxData('api/monitoring/status', function($xml) {
            var ret = xml2object($xml);
            G_MonitoringStatus = ret;
            if (2 == G_MonitoringStatus.response.ServiceStatus &&
                MACRO_EVDO_LEVEL_ONE == G_MonitoringStatus.response.RoamingStatus)
            {
                showConfirmDialog(connection_roaming_status_on, connect);
            }
            else
            {
                connect();
            }
        });
        return false;
    });
});

function getStatusFromUpdatingIcon()
{
    monitoring_status = G_MonitoringStatus.response;
    setHTML();
}

function setHTML() {
    var network_type_index = parseInt(monitoring_status.CurrentNetworkType, 10);
    $('#wan_network_type').html(network_type[network_type_index]);

    var manual_conected = '';
    var connection_status = '';

    var nConnectionStatus = parseInt(monitoring_status.ConnectionStatus, 10);
    var nServiceStatus = parseInt(monitoring_status.ServiceStatus, 10);

    //
    var roaming_status = monitoring_status.RoamingStatus;


    // no service
    if (nServiceStatus != 2)
    {
        $('#no_server_description h3').html(connection_service_no_coverage);
        $('#wan_network_type').html(dialup_label_no_service);
        connection_status = "<div class='manual_connection_status'>" + dialup_label_disconnected + '</div>';
        manual_conected = "<div id='manual_wrapper'><div class='manual_button'>" + create_button_html(common_continue, 'btn_continue') + '</div></div>';
        $('#connection_status').html(connection_status);
        $('#connected_disconnected').html(manual_conected);
        button_enable('manual_connect', '1');
    }
    // connected
    else if (nConnectionStatus == 901)
    {
        $('#no_server_description').hide();
        connection_status = "<div class='manual_connection_status'>" + dialup_label_connected + '</div>';
        manual_conected = "<div id='manual_wrapper'><div class='manual_button'>" + create_button_html(common_continue, 'btn_continue') + '</div></div>';
        $('#connection_status').html(connection_status);
        $('#connected_disconnected').html(manual_conected);
        button_enable('manual_connect', '1');
    }
    // disconnect
    else if (nConnectionStatus == 902)
    {
        if (NET_ROAMING_STATUS == roaming_status)
        {
            $('#no_server_description h3').html(connection_roaming_status_on);
        }
        else
        {
            $('#no_server_description h3').html(connection_device_connectionless);
        }
        connection_status = "<div class='manual_connection_status'>" + dialup_label_disconnected + '</div>';
        $('#connection_status').html(connection_status);
        getAjaxData('api/dialup/connection', function($xml) {
            var connection_ret = xml2object($xml);
            if (typeof connection_ret == 'undefined' || MANUAL_CONNECTION == connection_ret.response.ConnectMode || ('0' == connection_ret.response.RoamAutoConnectEnable && '1' == roaming_status))
            {
                manual_conected = "<div id='manual_wrapper'><div class='manual_button'>" + create_button_html(common_connect, 'manual_connect') + '</div></div>';
                $('#connected_disconnected').html(manual_conected);
                button_enable('manual_connect', '1');
            }
        });
    }
    // connecting
    else if (nConnectionStatus == 900)
    {
        $('#no_server_description').hide();
        connection_status = "<div class='manual_connection_status'>" + dialup_label_connecting + '</div>';
        $('#connection_status').html(connection_status);
        if ($('#manual_connect') != null && $('#manual_connect') != 'undefined')
        {
            button_enable('manual_connect', '0');
        }
    }
}


function connect() {
    var action = {
        Action: '1'
    };

    var action_xml = object2xml('request', action);
    showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments, function() {
        setTimeout(refresh, FEATURE_UI_WAITING.dialog_disapear);
    });
    saveAjaxData('api/dialup/dial', action_xml, function($xml) {
        var return_ret = xml2object($xml);
        if ('response' == return_ret.type && 'OK' == return_ret.response)
        {
            // history.go(0);
        }
        else
        {
            log.error('Connection failed');
        }
    });
}

function onContinue()
{
    gotoPageWithoutHistory('login.html');
}
