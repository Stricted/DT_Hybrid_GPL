// JavaScript Document
var g_device_array = [];
var g_device_config = {};
var g_device_info = {};

function getDeviceInfo() {
    var GATEWAY_ExternalIPAddress ='';
    var GATEWAY_ExternalIPv6Address ='';
    if((("2" == parent.G_MonitoringStatus.response.ServiceStatus) 
        &&(parent.G_MonitoringStatus.response.ConnectionStatus == "901" 
        || parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus == "2" ))
        || (parent.G_MonitoringStatus.response.X_IPv4WireLineConnectionStatus == "2"
        || parent.G_MonitoringStatus.response.X_IPv6WireLineConnectionStatus == "2" ))
      {
            getAjaxData('api/connection/wanstatus', function($xml) {
                    var ret=xml2object($xml);
                    if(ret.type=='response' && typeof(ret.response.ExternalIPAddress)!='undefined'
                       && typeof(ret.response.X_IPv6Address)!='undefined')
                    {
                       GATEWAY_ExternalIPAddress = ret.response.ExternalIPAddress;
                       GATEWAY_ExternalIPv6Address = ret.response.X_IPv6Address;
                    }
            }, {
                sync : true
            });
       } else {
                   GATEWAY_ExternalIPAddress = "";
                   GATEWAY_ExternalIPv6Address = "";
       }
        
    getAjaxData('api/device/information', function($xml) {
        var device_ret = xml2object($xml);
        if (device_ret.type == 'response') {
            g_device_info = device_ret.response;
            g_device_info.WanIPAddress = GATEWAY_ExternalIPAddress;
            g_device_info.WanIPAddress_ipv6 = GATEWAY_ExternalIPv6Address;
        } else {
            log.error('Error, no data');
        }
    }, {
        sync: true
    });
}

function getDeviceConfig() {
    getConfigData('config/deviceinformation/config.xml', function($xml) {
        var config_ret = _xml2feature($xml);
        if ('undefined' !== config_ret && null !== config_ret)
        {
            g_device_config = config_ret;
        }
    }, {
        sync: true
    });
}

function createListForDevice(_name, _value) {
    var tdName = '';
    var values = common_unknown;
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

        case 'WebUIVersion':
            tdName = system_label_webui_version;
            break;
        case 'MacAddress1':
        if (g_device_info.MacAddress2 == '') {
            tdName = wlan_label_mac_address;

           } else {
                  tdName = IDS_common_mac_address_x.replace('%d', 1);
          }
            break;
        case 'MacAddress2':
        if (g_device_info.MacAddress2 != '') {
                tdName = IDS_common_mac_address_x.replace('%d', 2);

            } else {
                   return row;
            }
            break;
        case 'Iccid':
            tdName = system_label_iccid;
            break;
        case 'Msisdn':
            tdName = system_label_my_number;
            break;
        case 'ProductFamily':
            tdName = system_label_product_family;
            break;
        case 'Classify':
            tdName = system_label_classify;
            break;
        case 'WanIPAddress':
            tdName = system_label_wanip_address_ipv4;
            break;
         case 'WanIPAddress_ipv6':
            tdName = system_label_wanip_address_ipv6;
            break;
        case 'RouterHWVersion':
            tdName = system_label_routerhardwareversion;
            break;
        case 'RouterSWVersion':
            tdName = system_label_routersoftwareversion;
            break;
        default:
            break;
    }
    row = '<tr><td>' + tdName + common_colon + "</td><td class='info_value' id ='device_" + _name  + "'>" + (_value == '' ? values : _value) + '</td></tr>';
    return row;
}
//Switch device info to display refrence device configuration xml
function setDeviceDisplay(_device_config, _device_info) {
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

getDeviceConfig();

$(document).ready(function() {
    getDeviceInfo(); 
    setDeviceDisplay(g_device_config, g_device_info);
});