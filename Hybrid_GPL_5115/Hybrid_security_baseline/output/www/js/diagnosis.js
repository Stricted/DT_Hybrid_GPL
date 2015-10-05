// JavaScript Document
//
var AP_STATION_CONNECTED = 2;
var g_device_array = [];
var g_device_config = {};
var diagnosis_info = {};
var MACRO_EVDO_SIGNAL_DBM = 'dBm';

function getDeviceInfo() {
        getConfigData('config/deviceinformation/diagconfig.xml', function($xml) {
            var config_ret = _xml2feature($xml);
            if ('undefined' !== config_ret && null !== config_ret)
            {
                g_device_config = config_ret;
            }
        },{
            sync: true
        });
        getAjaxData('api/device/diagnosis', function($xml) {
        var ret = xml2object($xml);
        if ('response' == ret.type) {
            diagnosis_info = ret.response;
            switch (diagnosis_info.ClipMode) {
                case '0':
                    diagnosis_info.ClipMode = clip_mode_dtmf;
                    break;
                case '10':
                    diagnosis_info.ClipMode = clip_mode_itu;
                    break;
                case '11':
                    diagnosis_info.ClipMode = clip_mode_bell;
                    break;
                case '12':
                    diagnosis_info.ClipMode = clip_mode_rpas;
                    break;
                default:
                    diagnosis_info.ClipMode = common_unknown;
                    break;
            }
            
            g_device_config.rssi = 0;
            g_device_config.rscp = 0;
            g_device_config.rsrp = 0;
            g_device_config.rsrq = 0;            
            g_device_config.tac = 0;
            g_device_config.lac = 0;
            g_device_config.rac = 0;            
            switch (diagnosis_info.sys_mode) {
                case '0':
                    diagnosis_info.sys_mode = dialup_label_no_service;
                    break;
                case '3':
                    g_device_config.rssi = 1;
                    g_device_config.lac = 1;
                    g_device_config.rac = 1;            
                    if(typeof(diagnosis_info.rssi) != 'undefined' && diagnosis_info.rssi!="")
                    {
                        diagnosis_info.rssi = diagnosis_info.rssi + MACRO_EVDO_SIGNAL_DBM;
                    }
                    diagnosis_info.sys_mode = "GSM";
                    break;
                case '5':
                    g_device_config.rscp = 1;
                    g_device_config.lac = 1;
                    g_device_config.rac = 1;            
                    if(typeof(diagnosis_info.rscp) != 'undefined' && diagnosis_info.rscp!="")
                    {
                        diagnosis_info.rscp = diagnosis_info.rscp + MACRO_EVDO_SIGNAL_DBM;
                    }
                    diagnosis_info.sys_mode = "WCDMA";
                    break;
                case '9':
                    g_device_config.rsrp = 1;
                    g_device_config.rsrq = 1;
                    g_device_config.tac = 1;
                    if(typeof(diagnosis_info.rsrp) != 'undefined' && diagnosis_info.rsrp!="")
                    {
                        diagnosis_info.rsrp = diagnosis_info.rsrp + MACRO_EVDO_SIGNAL_DBM;
                    }
                    diagnosis_info.sys_mode = "LTE";
                    break;
                case '15':
                    g_device_config.rscp = 1;
                    g_device_config.lac = 1;
                    g_device_config.rac = 1;            
                    if(typeof(diagnosis_info.rscp) != 'undefined' && diagnosis_info.rscp!="")
                    {
                        diagnosis_info.rscp = diagnosis_info.rscp + MACRO_EVDO_SIGNAL_DBM;
                    }
                    diagnosis_info.sys_mode = "TD-SCDMA";
                    break;
                default:
                    break;
            }
            
            switch (diagnosis_info.antennamode) {
                case '0':
                    diagnosis_info.antennamode = antenna_label_antenna_mode_outer;
                    break;
                case '1':
                    diagnosis_info.antennamode = antenna_label_antenna_mode_inner;
                    break;
                default:
                    break;
            }
            
            switch (diagnosis_info.SimStatus) {
                case '0':
                    diagnosis_info.sys_mode = dialup_label_no_service;
                    if(parent.G_MonitoringStatus.response.simlockStatus == "1"){
                        diagnosis_info.SimStatus = dialup_hint_simlock_active;
                    } else {
                        getAjaxData("api/pin/status", function($xml) {
                            var pinstatus_ret = xml2object($xml);
                            if ("response" == pinstatus_ret.type) {
                                switch (pinstatus_ret.response.SimState){
                                case MACRO_PIN_REQUIRED:
                                    diagnosis_info.SimStatus = dialup_label_pin_code_required;
                                    break;
                                case MACRO_PUK_REQUIRED:
                                    diagnosis_info.SimStatus = dialup_label_puk_code_required;
                                    break;
                                case MACRO_PIN_READY:
                                    diagnosis_info.SimStatus = dialup_label_sim_valid;
                                    break;
                                case MACRO_CARD_LOCK_PIN_RIGNT:
                                    diagnosis_info.SimStatus = label_hint_pinlock_active;
                                    break;
                                case MACRO_CARD_LOCK_PIN_WRONG:
                                    diagnosis_info.SimStatus = label_hint_pinlock_active;
                                    break;
                                default:
                                    diagnosis_info.SimStatus = dialup_label_invalid_card;
                                    break;
                                }
                            } else {
                                diagnosis_info.SimStatus = dialup_label_invalid_card;
                            }
                        },{sync: true});
                    }
                    break;
                case '1':
                    diagnosis_info.SimStatus = dialup_label_sim_valid;
                    break;
                case '2':
                    diagnosis_info.SimStatus = dialup_label_sim_invalid+" CS";
                    break;
                case '3':
                    diagnosis_info.SimStatus = dialup_label_sim_invalid+" PS";
                    break;
                case '4':
                    diagnosis_info.sys_mode = dialup_label_no_service;
                    diagnosis_info.SimStatus = dialup_label_sim_invalid+" CS,PS";
                    break;
                case '255':
                    diagnosis_info.sys_mode = dialup_label_no_service;
                    diagnosis_info.SimStatus = dialup_label_sim_invalid;
                    break;
                default:
                    break;
            }
            
        }
    },{
        sync: true
    });
    setDeviceDisplay();
    setTimeout(getDeviceInfo, 6000);
}


function createListForDevice(_name, _value) {
    var tdName = '';
    var values = "--";
    var row = '';
    switch (_name) {
        case 'DeviceName':
            tdName = system_label_device_name;
            break;

        case 'SerialNumber':
            tdName = system_label_serial_number;
            break;

        case 'Imei':
            tdName = system_label_imei;
            break;
        case 'Imsi':
            tdName = device_information_imsi;
            break;
        case 'HardwareVersion':
            tdName = system_label_hardware_version;
            break;
        case 'SoftwareVersion':
            tdName = system_label_software_version;
            break;
        case 'SoftwareBuildTime':
            tdName = system_label_software_buildtime;
            break;
        case 'Iccid':
            tdName = system_label_iccid;
            break;
        case 'Msisdn':
            tdName = system_label_my_number;
            break;
        case 'dbandwidth':
            tdName = system_label_bandwidth;
            break;
        case 'dservice_status':
            tdName = system_label_service_status;
            break;
        case 'antennamode':
            tdName = antenna_label_antenna_mode;
            break;
        case 'ClipMode':
            tdName = clip_select_mode;
            break;
        case 'SimStatus':
            tdName = dialup_label_sim_card_status;
            break;
        case 'SimPinStatus':
            tdName = dialup_label_sim_card_status;
            break;
        case 'NetworkBand':
            tdName = label_diagnosis_band_class;
            break;
        case 'CurrentNetworkType':
            tdName = label_network_mode;
            break;
        case 'sys_mode':
            tdName = label_wireless_mode;
            break;
        case 'ServiceStatus':
            tdName = system_label_service_status;
            break;
        case 'bcch':
            tdName = "BCCH";
            break;
        case 'cell_id':
            tdName = label_diagnosis_cellid;
            break;
        case 'lac':
            tdName = label_diagnosis_lacid;
            break;
        case 'rac':
            tdName = label_diagnosis_racid;
            break;
        case 'tac':
            tdName = "TAC";
            break;
        //2G信号强度
        case 'rssi':
            tdName = label_diagnosis_rssi;
            break;
        //3G信号强度    
        case 'rscp':
            tdName = label_diagnosis_rscp;
            break;    
        //4G信号强度    
        case 'rsrp':
            tdName = "RSRP";
            break;
        case 'rsrq':
            tdName = "RSRQ";
            break;
        default:
            break;
    }
    row = '<tr><td>' + tdName + common_colon + "</td><td class='info_value' id= 'diag_" + _name + "'>" + (_value == '' ? values : _value) + '</td></tr>';
    return row;
}
//Switch device info to display refrence device configuration xml
function setDeviceDisplay() {
    var _device_config = g_device_config;
    var _device_info = diagnosis_info;
    var list_content = '';
    var p = '';
    for (p in _device_info) {
        if (_device_config[p.toLowerCase()]) {
            if (typeof(_device_info[p]) != 'undefinded') {
                list_content += createListForDevice(p, _device_info[p]);
            }
            else {
                log.error("device dosen't exsited");
            }
        }
    }
    $('.diviceInfo_table').html(list_content);
}



getDeviceInfo();

$(document).ready(function() {
    setDeviceDisplay();
});