var AJAX_HEADER = '../';
var AJAX_TAIL = '';
var AJAX_TIMEOUT = 30000;

var g_main_displayingPromptStack = parent.g_main_displayingPromptStack;
var g_feature = parent.g_feature;
var g_module = parent.g_module;
var g_login_usertype = parent.g_login_usertype;
var g_feature_ipv6 = parent.g_feature_ipv6;
//Pin status
var MACRO_CARD_LOCK_PIN_RIGNT = '253';
var MACRO_CARD_LOCK_PIN_WRONG = '254';
var MACRO_NO_SIM_CARD = '255';
var MACRO_CPIN_FAIL = '256';
var MACRO_PIN_READY = '257';
var MACRO_PIN_DISABLE = '258';
var MACRO_PIN_VALIDATE = '259';
var MACRO_PIN_REQUIRED = '260';
var MACRO_PUK_REQUIRED = '261';
var MACRO_INIT_SIM_CARD = '262';
//Connection status
var MACRO_CONNECTION_CONNECTING = '900';
var MACRO_CONNECTION_CONNECTED = '901';
var MACRO_CONNECTION_DISCONNECTED = '902';
var MACRO_CONNECTION_DISCONNECTING = '903';
var FORBID_AUTO_CONNECT_OPEN_DEVICE = '112';
var FORBID_AUTO_CONNECT_OPEN_DEVICE_ROAMING = '113';
var FORBID_RE_CONNECT_DROPLINE = '114';
var FORBID_RE_CONNECT_DROPLINE_ROAMING = '115';
//ipv6 Connection status
var MACRO_CONNECTION_IPV6CONNECTING = '1';
var MACRO_CONNECTION_IPV6CONNECTED = '2';
var MACRO_CONNECTION_IPV6PENDINGDISCONNECT = '3';
var MACRO_CONNECTION_IPV6DISCONNECTING = '4';
var MACRO_CONNECTION_IPV6DISCONNECTED = '5';
//Wireline Connection status
var MACRO_WIRELINE_CONNECTION_CONNECTING = '1';
var MACRO_WIRELINE_CONNECTION_CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_PENDINGDISCONNECT = '3';
var MACRO_WIRELINE_CONNECTION_DISCONNECTING = '4';
var MACRO_WIRELINE_CONNECTION_DISCONNECTED = '5';
//Wireline ipv6 Connection status
var MACRO_WIRELINE_CONNECTION_IPV6CONNECTING = '1';
var MACRO_WIRELINE_CONNECTION_IPV6CONNECTED = '2';
var MACRO_WIRELINE_CONNECTION_IPV6PENDINGDISCONNECT = '3';
var MACRO_WIRELINE_CONNECTION_IPV6DISCONNECTING = '4';
var MACRO_WIRELINE_CONNECTION_IPV6DISCONNECTED = '5';
//SIM status
var MACRO_SIM_STATUS_USIM_N = '0';
var MACRO_SIM_STATUS_USIM_Y = '1';
var MACRO_SIM_STATUS_USIM_CS_N = '2';
var MACRO_SIM_STATUS_USIM_PS_N = '3';
var MACRO_SIM_STATUS_USIM_PS_CS_N = '4';
var MACRO_SIM_STATUS_ROMSIM = '240';
var MACRO_SIM_STATUS_USIM_NE = '255';
//Battery status
var MACRO_BATTERY_STATUS_NORMAL = '0';
var MACRO_BATTERY_STATUS_ELECT = '1';
var MACRO_BATTERY_STATUS_LOW = '-1';
//Battery level
var MACRO_BATTERY_LEVEL_ZERO = '0';
var MACRO_BATTERY_LEVEL_ONE = '1';
var MACRO_BATTERY_LEVEL_TWO = '2';
var MACRO_BATTERY_LEVEL_THREE = '3';
var MACRO_BATTERY_LEVEL_FOUR = '4';
//Signal status
var MACRO_EVDO_LEVEL_ZERO = '0';
var MACRO_EVDO_LEVEL_ONE = '1';
var MACRO_EVDO_LEVEL_TWO = '2';
var MACRO_EVDO_LEVEL_THREE = '3';
var MACRO_EVDO_LEVEL_FOUR = '4';
var MACRO_EVDO_LEVEL_FIVE = '5';

//CurrentPlmn
var MACRO_CURRENT_NETWOORK_2G = '0';
var MACRO_CURRENT_NETWOORK_3G = '2';
var MACRO_CURRENT_NETWOORK_H = '5';
var MACRO_CURRENT_NETWOORK_4G = '7';

//CurrentNetworkType
var MACRO_NET_WORK_TYPE_NOSERVICE         = '0';          /* 无服务            */
var MACRO_NET_WORK_TYPE_GSM               = '1';          /* GSM模式           */
var MACRO_NET_WORK_TYPE_GPRS              = '2';          /* GPRS模式          */
var MACRO_NET_WORK_TYPE_EDGE              = '3';          /* EDGE模式          */
var MACRO_NET_WORK_TYPE_WCDMA             = '4';          /* WCDMA模式         */
var MACRO_NET_WORK_TYPE_HSDPA             = '5';          /* HSDPA模式         */
var MACRO_NET_WORK_TYPE_HSUPA             = '6';          /* HSUPA模式         */
var MACRO_NET_WORK_TYPE_HSPA              = '7';          /* HSPA模式          */
var MACRO_NET_WORK_TYPE_TDSCDMA           = '8';          /* TDSCDMA模式       */
var MACRO_NET_WORK_TYPE_HSPA_PLUS         = '9';          /* HSPA_PLUS模式     */
var MACRO_NET_WORK_TYPE_EVDO_REV_0        = '10';         /* EVDO_REV_0模式    */
var MACRO_NET_WORK_TYPE_EVDO_REV_A        = '11';         /* EVDO_REV_A模式    */
var MACRO_NET_WORK_TYPE_EVDO_REV_B        = '12';         /* EVDO_REV_A模式    */
var MACRO_NET_WORK_TYPE_1xRTT             = '13';         /* 1xRTT模式         */
var MACRO_NET_WORK_TYPE_UMB               = '14';         /* UMB模式           */
var MACRO_NET_WORK_TYPE_1xEVDV            = '15';         /* 1xEVDV模式        */
var MACRO_NET_WORK_TYPE_3xRTT             = '16';         /* 3xRTT模式         */
var MACRO_NET_WORK_TYPE_HSPA_PLUS_64QAM   = '17';         /* HSPA+64QAM模式    */
var MACRO_NET_WORK_TYPE_HSPA_PLUS_MIMO    = '18';          /* HSPA+MIMO模式     */
var MACRO_NET_WORK_TYPE_DC_HSPA           = '19';          /* DC-HSPA模式     */
var MACRO_NET_WORK_TYPE_LTE               = '20';          /*LTE 模式*/
var DSAT_SYS_SUBMODE_TDS                  = '21';          /*TD-SCDMA ģʽ  */ 
var DSAT_SYS_SUBMODE_TDS_HSDPA            = '22';          /*TDS_HSDPA ģʽ */ 
var DSAT_SYS_SUBMODE_TDS_HSUPA            = '23';          /*TDS_HSUPA ģʽ */ 
var DSAT_SYS_SUBMODE_TDS_HSPA             = '24';          /*TDS_HSPA ģʽ  */

//Wifi station connection status
var WIFI_STATION_DISCONNECTED = '0';
var WIFI_STATION_CONNECTING = '1';
var WIFI_STATION_CONNECTED = '2';
var WIFI_STATION_DISCONNECTING = '3';
//Wifi status
var MACRO_WIFI_OFF = '0';
var MACRO_WIFI_ON = '1';


//function for show jquery qtip
function showQtip(showTarget, content, delay) {
    var $showTarget = $('#' + showTarget);
    if ($showTarget) {
        $showTarget.qtip({
            content: content,
            position: {
                corner: {
                    tooltip: 'topLeft',
                    target: 'bottomMiddle'
                }
            },
            show: {
                when: false,
                ready: true
            }
        });
        if (delay) {
            setTimeout(function() {
                $showTarget.qtip('destroy');
            }, delay);
        }
        else {
            setTimeout(function() {
                $showTarget.qtip('destroy');
            }, g_feature.tip_disapear);
        }
    }
    $showTarget.focus();
}


function showErrorUnderTextbox(idOfTextbox, errormsg, label_id) {
    var errorLabel = '';
    if (label_id != null && label_id != '' && label_id != ' ') {
        errorLabel = "<div class='error_message'><label id='" + label_id + "'>" + errormsg + '</label><div>';
    }
    else {
        errorLabel = "<div class='error_message'><label>" + errormsg + '</label><div>';
    }
    $('#' + idOfTextbox).after(errorLabel);
}

function showErrorUnderTextbox_Parent(idOfTextbox, errormsg, label_id) {

    var errorLabel = '';
    if (label_id != null && label_id != '' && label_id != ' ') {
        errorLabel = "<div class='error_message'><label id='" + label_id + "'>" + errormsg + '</label><div>';
    }
    else {
        errorLabel = "<div class='error_message'><label>" + errormsg + '</label><div>';
    }
    parent.$('#' + idOfTextbox).after(errorLabel);
}

function clearAllErrorLabel() {
    $('.error_message').remove();
}

function clearAllErrorLabel_Parent() {
    parent.$('.error_message').remove();
}

function getQueryStringByName(item) {
    var svalue = location.search.match(new RegExp('[\?\&]' + item + '=([^\&]*)(\&?)', 'i'));
    return svalue ? svalue[1] : svalue;
}


var log = log4javascript.getNullLogger();
var _logEnable = getQueryStringByName('log');

function dw(text) {
    document.write(text);
}

function create_button(content, button_id, buttonClassName) {
    if (buttonClassName != '' && buttonClassName != ' ' && buttonClassName != null) {
        button = "<span class='button_wrapper " + buttonClassName + "' id='" + button_id + "'>";
    }
    else {
        button = "<span class='button_wrapper' id='" + button_id + "'>";
    }
    button += "<span class='button_left'>" + "<span class='button_right'>" + "<span class='button_center'>" + content + '</span></span></span></span>';
    document.write(button);
}

function create_button_html(content, button_id, buttonClassName) {
    if (buttonClassName != '' && buttonClassName != ' ' && buttonClassName != null) {
        button = "<span class='button_wrapper " + buttonClassName + "' id='" + button_id + "'>";
    }
    else {
        button = "<span class='button_wrapper' id='" + button_id + "'>";
    }
    button += "<span class='button_left'>" + "<span class='button_right'>" + "<span class='button_center'>" + content + '</span></span></span></span>';
    return button;
}

function button_enable(button_id, enable) {
    var parent = $('#' + button_id);

    if (enable == '1') {
        parent.removeClass('disable_btn');

    }
    else if (enable == '0') {
        parent.addClass('disable_btn');

    }
}

function isButtonEnable(button_id) {
    var disable = true;
    var $button = $('#' + button_id);
    if ($button) {
        if ($button.children()) {
            disable = $button.hasClass('disable_btn');
            //enable = $button.children().hasClass("button_left");
        }
    }
    return !disable;
}
function isButtonEnable_Parent(button_id) {
    var disable = true;
    var $button = parent.$('#' + button_id);
    if ($button) {
        if ($button.children()) {
            disable = $button.hasClass('disable_btn');
            //enable = $button.children().hasClass("button_left");
        }
    }
    return !disable;
}

function _createNodeStr(nodeName, nodeValue) {
    return '<' + nodeName + '>' + nodeValue + '</' + nodeName + '>';
}

// internal use only
function _recursiveObject2Xml(name, obj) {
    var xmlstr = '';
    if (typeof (obj) == 'string' || typeof (obj) == 'number') {
        xmlstr = _createNodeStr(name, obj);
    }
    else if (jQuery.isArray(obj)) {
        jQuery.each(obj, function(idx, item) {
            xmlstr += _recursiveObject2Xml(name, item);
        });
    }
    else if (typeof (obj) == 'object') {
        xmlstr += '<' + name + '>';
        jQuery.each(obj, function(propName, propVal) {
            xmlstr += _recursiveObject2Xml(propName, propVal);
        });
        xmlstr += '</' + name + '>';
    }
    return xmlstr;
}

// convert an object to xml string.
// name: root tag name of XML
// obj:  object which is to be convert to XML
function object2xml(name, obj) {
    var xmlstr = '<?xml version="1.0" encoding="UTF-8"?>';
    xmlstr += _recursiveObject2Xml(name, obj);
    return xmlstr;
}

// internal use only
function _recursiveXml2Object($xml) {
    if ($xml.children().size() > 0) {
        var _obj = {};
        $xml.children().each(function() {
            var _childObj = ($(this).children().size() > 0) ? _recursiveXml2Object($(this)) : $(this).text();
            if ($(this).siblings().size() > 0 && $(this).siblings().get(0).tagName == this.tagName) {
                if (_obj[this.tagName] == null) {
                    _obj[this.tagName] = [];
                }
                _obj[this.tagName].push(_childObj);
            }
            else {
                _obj[this.tagName] = _childObj;
            }
        });
        return _obj;
    }
    else {
        return $xml.text();
    }
}

// convert XML string to an Object.
// $xml, which is an jQuery xml object.
function xml2object($xml) {
    var obj = new Object();
    if ($xml.find('response').size() > 0) {
        var _response = _recursiveXml2Object($xml.find('response'));
        obj.type = 'response';
        obj.response = _response;
    }
    else if ($xml.find('error').size() > 0) {
        var _code = $xml.find('code').text();
        var _message = $xml.find('message').text();
        log.warn('MAIN : error code = ' + _code);
        log.warn('MAIN : error msg = ' + _message);
        obj.type = 'error';
        obj.error = {
            code: _code,
            message: _message
        };
    }
    else if ($xml.find('config').size() > 0) {
        var _config = _recursiveXml2Object($xml.find('config'));
        obj.type = 'config';
        obj.config = _config;
    }
    else {
        obj.type = 'unknown';
    }
    return obj;
}

/*************Get and save data (end)**************/
/***************Get and save data (begin)******/

// urlstr : URL of the Restful interface.
// callback_func : Callback function to handle response, this callback function
// have one parameter
// callback_func($xml) - $xml : a jQuery XML object which is successfully get
// from getAjaxData.
// options.sync
// options.timout
// options.errorCB
function getAjaxData(urlstr, callback_func, options) {
    var myurl = AJAX_HEADER + urlstr + AJAX_TAIL;
    var isAsync = true;
    var nTimeout = AJAX_TIMEOUT;
    var errorCallback = null;

    if (options) {
        if (options.sync) {
            isAsync = (options.sync == true) ? false : true;
        }
        if (options.timeout) {
            nTimeout = parseInt(options.timeout, 10);
            if (isNaN(nTimeout)) {
                nTimeout = AJAX_TIMEOUT;
            }

        }
        errorCallback = options.errorCB;
    }

    $.ajax({
        async: isAsync,
        //cache: false,
        type: 'GET',
        timeout: nTimeout,
        url: myurl,
        //dataType: ($.browser.msie) ? "text" : "xml",
        error: function(XMLHttpRequest, textStatus) {
            try {
                if (jQuery.isFunction(errorCallback)) {
                    errorCallback(XMLHttpRequest, textStatus);
                }
                log.error('MAIN : getAjaxData(' + myurl + ') error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus ' + textStatus);
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : getAjaxData(' + myurl + ') sucess.');
            log.trace(data);
            var xml;
            if (typeof data == 'string' || typeof data == 'number') {
                if (-1 != this.url.indexOf('/api/sdcard/sdcard')) {
                    data = sdResolveCannotParseChar(data);
                }
                if (!window.ActiveXObject) {
                    var parser = new DOMParser();
                    xml = parser.parseFromString(data, 'text/xml');
                }
                else {
                    //IE
                    xml = new ActiveXObject('Microsoft.XMLDOM');
                    xml.async = false;
                    xml.loadXML(data);
                }
            }
            else {
                xml = data;
            }
            if (typeof callback_func == 'function') {
                callback_func($(xml));
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
    });
}

function sdResolveCannotParseChar(xmlStr) {
    return xmlStr.replace(/(\&|\')/g, function($0, $1) {
        return {
            '&' : '&amp;' ,
            "'" : '&#39;'
        }[$1];
    }
    );
}

function smsContentDeleteWrongChar(smsStr) {
    return smsStr.replace(/([\x00-\x08]|\x0b|\x0c|[\x0e-\x1f])/g, ' ');
}

// urlstr : URL of the Restful interface.
// xml: xml string to be submit to server.
// callback_func : Callback function to handle response, this callback function
// have one parameter
// callback_func($xml) - $xml : a jQuery XML object which is successfully get
// from getAjaxData.
// options.sync
// options.timout
// options.errorCB
function saveAjaxData(urlstr, xml, callback_func, options) {
    var myurl = AJAX_HEADER + urlstr + AJAX_TAIL;
    var isAsync = true;
    var nTimeout = AJAX_TIMEOUT;
    var errorCallback = null;

    if (options) {
        if (options.sync) {
            isAsync = (options.sync == true) ? false : true;
        }
        if (options.timeout) {
            nTimeout = parseInt(options.timeout, 10);
            if (isNaN(nTimeout)) {
                nTimeout = AJAX_TIMEOUT;
            }
        }
        errorCallback = options.errorCB;
    }

    parent.startLogoutTimer();

    $.ajax({
        async: isAsync,
        //cache: false,
        type: 'POST',
        timeout: nTimeout,
        url: myurl,
        // dataType: ($.browser.msie) ? "text" : "xml",
        data: xml,
        error: function(XMLHttpRequest, textStatus) {
            try {
                if (jQuery.isFunction(errorCallback)) {
                    errorCallback(XMLHttpRequest, textStatus);
                }
                log.error('MAIN : saveAjaxData(' + myurl + ') error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus' + textStatus);
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : saveAjaxData(' + myurl + ') success.');
            log.trace(data);
            var xml;
            if (typeof data == 'string') {
                if (-1 != this.url.indexOf('/api/sms/sms-list') && -1 != data.indexOf('Messages')) {
                    data = smsContentDeleteWrongChar(data);
                }
                if(-1 != this.url.indexOf('/api/settings/dms-sdfile') && -1 != data.indexOf('Name') )
                {
                    data = sdResolveCannotParseChar(data);
                }
                if (!window.ActiveXObject) {
                    var parser = new DOMParser();
                    xml = parser.parseFromString(data, 'text/xml');
                }
                else {
                    //IE
                    xml = new ActiveXObject('Microsoft.XMLDOM');
                    xml.async = false;
                    xml.loadXML(data);
                }
            }
            else {
                xml = data;
            }
            if (typeof callback_func == 'function') {
                callback_func($(xml));
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
    });
}

// return true if the AJAX response from server is <response>OK</response>
// obj: object came from $xml
function isAjaxReturnOK(obj) {
    var ret = false;
    if (obj) {
        if (typeof (obj.response) == 'string') {
            if (obj.response.toLowerCase() == 'ok') {
                ret = true;
            }
        }
    }
    return ret;
}

// options.sync
// options.timout
// options.errorCB
function getConfigData(urlstr, callback_func, options) {
    var myurl = '../' + urlstr + '';
    //var myurl = urlstr + "";
    var isAsync = true;
    var nTimeout = AJAX_TIMEOUT;
    var errorCallback = null;

    if (options) {
        if (options.sync) {
            isAsync = (options.sync == true) ? false : true;
        }
        if (options.timeout) {
            nTimeout = parseInt(options.timeout, 10);
            if (isNaN(nTimeout)) {
                nTimeout = AJAX_TIMEOUT;
            }
        }
        errorCallback = options.errorCB;
    }

    $.ajax({
        async: isAsync,
        //cache: false,
        type: 'GET',
        timeout: nTimeout,
        url: myurl,
        //dataType: ($.browser.msie) ? "text" : "xml",
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            try {
                log.debug('MAIN : getConfigData(' + myurl + ') error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus ' + textStatus);
                if (jQuery.isFunction(errorCallback)) {
                    errorCallback(XMLHttpRequest, textStatus);
                }
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : getConfigData(' + myurl + ') success.');
            log.trace(data);
            var xml;
            if (typeof data == 'string' || typeof data == 'number') {
                if (!window.ActiveXObject) {
                    var parser = new DOMParser();
                    xml = parser.parseFromString(data, 'text/xml');
                }
                else {
                    //IE
                    xml = new ActiveXObject('Microsoft.XMLDOM');
                    xml.async = false;
                    xml.loadXML(data);
                }
            }
            else {
                xml = data;
            }
            if (typeof callback_func == 'function') {
                callback_func($(xml));
            }
            else {
                log.error('callback_func is undefined or not a function');
            }
        }
    });
}

/*************Get and save data (end)**************/
function _createFeatureNode(str) {
    if (typeof (str) == 'undefined' || str == null) {
        return null;
    }

    var value = parseInt(str, 10);
    
    //处理str值的类型如下：数字（数字长度大于等于1）+字母的情况
    if(!isNaN(value) && value.toString().length != str.length){ 
        value = str; 
    }
    
    if (isNaN(value)) {
        // not a number
        if (str.toLowerCase().indexOf('true') > -1) {
            return true;
        }
        else if (str.toLowerCase().indexOf('false') > -1) {
            return false;
        }
        else {
            return str;
        }
    }
    else {
        // is a number
        return value;
    }
}

function _recursiveXml2Feature($xml) {
    if ($xml.children().size() > 0) {
        var _obj = {};
        $xml.children().each(function() {
            var _childObj = ($(this).children().size() > 0) ? _recursiveXml2Feature($(this)) : _createFeatureNode($(this).text());
            if ($(this).siblings().size() > 0 && $(this).siblings().get(0).tagName == this.tagName) {
                if (_obj[this.tagName] == null) {
                    _obj[this.tagName] = [];
                }
                _obj[this.tagName].push(_childObj);
            }
            else {
                _obj[this.tagName] = _childObj;
            }
        });
        return _obj;
    }
    else {
        return _createFeatureNode($xml.text());
    }
}

// convert XML string to an Object.
// $xml, which is an jQuery xml object.
function _xml2feature($xml) {
    var obj = null;
    if ($xml.find('config').size() > 0) {
        var _config = _recursiveXml2Feature($xml.find('config'));
        obj = _config;
    }
    return obj;
}

function loadLangFile() {
    var langFile = '../language/lang_' + parent.LANGUAGE_DATA.current_language + '.js';

    $.ajax({
        async: false,
        //cache: false,
        type: 'GET',
        timeout: '3000',
        url: langFile,
        dataType: 'script',
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            try {
                log.error('MAIN : loadLangFile() error.');
                log.error('MAIN : XMLHttpRequest.readyState = ' + XMLHttpRequest.readyState);
                log.error('MAIN : XMLHttpRequest.status = ' + XMLHttpRequest.status);
                log.error('MAIN : textStatus' + textStatus);
                log.error('MAIN : errorThrown' + errorThrown);
            }
            catch (exception) {
                log.error(exception);
            }
        },
        success: function(data) {
            log.debug('MAIN : loadLangFile() success.');
        }
    });
}

$(document).ready(function() {
        $('#login_wrapper').show();
        $(document).keyup(onKeyup);
});

function getCurrentTime(time) {
    var final_time = '';
    var times = parseInt(time, 10);

    //format time like : 02:15:38

    var hours = parseInt((times / 3600), 10);
    if (hours > 9) {
        final_time += hours + common_colon;
    }
    else if (hours >= 0) {
        final_time += '0' + hours + common_colon;
    }

    times = times - hours * 3600;

    var minutes = parseInt(times / 60, 10);
    if (minutes > 9) {
        final_time += minutes + common_colon;
    }
    else if (minutes > 0) {
        final_time += '0' + minutes + common_colon;
    }
    else if (minutes == 0) {
        final_time += '00' + common_colon;
    }
    times = times - minutes * 60;

    //handle second display
    if (times > 9) {
        final_time += times;
    }
    else if (times > 0) {
        final_time += '0' + times;
    }
    else if (times == 0) {
        final_time += '00';
    }

    return final_time;
}

function stopBubble( e )
{
    if ( e && e.stopPropagation )
    {
        e.stopPropagation();
    }
    else
    {
        window.event.cancelBubble=true;
    }
}

function regURL ( str )
{
    var reg = "(((https|http|ftp|rtsp|mms):\/\/)|(www\\.)){1}([A-Za-z0-9\/]+\\.)+[a-z]{2,4}(\/[a-z0-9\?=\%_\&]+)*";
    var matchURL=new RegExp(reg,"ig");
    return str.replace(matchURL,function($1)
    {
        $1_href = $1.indexOf(("//").toLowerCase())==-1?"http://"+$1:$1;
        return "<a href='"+$1_href+"' style='color:blue;text-decoration:underline;' target='_blank' onclick='javascript:stopBubble(event)'>"+$1+"</a>";
    });
}

function onKeyup(evt) {
    if (evt.ctrlKey || evt.altKey)// Exclude key press "CRTL+key" & "ALT+key"
    {
        return;
    }   
    if (13 == evt.keyCode)// Enter key
    {
        var targetID = '';
        var stackLen = g_main_displayingPromptStack.length;

        var current_href = parent.$("#show_html").attr("src");
        current_href = current_href.substring(0, current_href.lastIndexOf('.'));
    
        if (stackLen > 0) {
            targetID = g_main_displayingPromptStack[stackLen - 1];
        }
        else {
            if ('autoconnection' == current_href) {
                targetID = 'autocennect_apply';
            }

            if ('mobilenetworksettings' == current_href) {
                targetID = 'mobilensetting_apply';
            }

            if ('pincodemanagement' == current_href) {
                targetID = $('.button_wrapper').attr('id');
            }

            if ('mobileconnection' == current_href) {
                targetID = 'apply';
            }

            if ('pincodeautovalidate' == current_href) {
                targetID = 'validate_apply';
            }

            if ('modifypassword' == current_href) {
                targetID = 'apply_button';
            }

            if ('wps' == current_href) {
                targetID = 'apply_button';
            }

            if ('dhcp' == current_href) {
                targetID = 'apply';
            }

            if ('sipalgsettings' == current_href) {
                targetID = 'apply_button';
            }

            if ('mobilenetworksettings' == current_href) {
                targetID = 'mobilensetting_apply';
            }

            if ('wlanbasicsettings' == current_href) {
                targetID = 'apply_button';
            }

            if ('wlanadvanced' == current_href) {
                targetID = 'apply_button';
            }

            if ('wlanmacfilter' == current_href) {
                targetID = 'apply';
            }

            if ('firewallswitch' == current_href) {
                targetID = 'apply_button';
            }

            if ('dmzsettings' == current_href) {
                targetID = 'apply_button';
            }

            if ('upnp' == current_href) {
                targetID = 'apply';
            }

            if ('update' == current_href) {
                if (!$('#success_ok').is(':hidden')) {
                    targetID = 'success_ok';
                }

                if (!$('#pop_Cancel').is(':hidden')) {
                    targetID = 'pop_Cancel';
                }
            }
            
            if ('sdcardsharing' == current_href) {
                targetID = g_sd_mode_id;
            }

            if ('lanipfilter' == current_href) { 
                targetID = 'apply'; 
            }

            if ('lanmacfilter' == current_href) { 
                targetID = 'lanmacfilter_apply'; 
            }

            if ('urlfilter' == current_href) { 
                targetID = 'apply'; 
            }

            if ('ddns' == current_href) { 
                targetID = 'apply'; 
            }

            if ('samba' == current_href) { 
                targetID = 'apply'; 
            }

            if ('dms' == current_href) { 
                targetID = 'apply'; 
            }

            if ('log' == current_href) { 
                targetID = 'apply'; 
            }

            if ('antenna' == current_href) { 
                targetID = 'apply'; 
            }

            if ('clip' == current_href) { 
                targetID = 'apply'; 
            }

            if ('wlansettings' == current_href) {
                targetID = 'apply_button';
            }

            if("tr069" == current_href) {
                targetID = "btn_acs_save";
            }

            if("tr111" == current_href) {
                targetID = "tr111_apply";
            }

            if("logview" == current_href) {
                targetID = "apply";
            }

            if("connectionstatus" == current_href) {
                targetID = "apply_button";
            }

            if("sntp" == current_href) {
                targetID = "apply_button";
            }

            if ('wlanbridge' == current_href){
                targetID = 'apply_button';
            }

            if ('virtualserver' == current_href){
                targetID = 'apply';
            }

            if ('nat' == current_href){
                targetID = 'apply';
            }

            if ('aclsettings' == current_href){
                targetID = 'apply';
            }

            if ('ipv6dhcp' == current_href){
                targetID = 'apply_button';
            }

            if ('slaac' == current_href){
                targetID = 'apply_button';
            }

            if('specialapplication' == current_href){
                targetID = 'apply';
             }

            if('roaming' == current_href){
                targetID = 'login';
            }
            
            if('manual' == current_href){
                targetID = 'login';
            }

        }

        if ('' != targetID) {
            $('#' + targetID).trigger('click');
        }
    }
}

// goto page without history
function gotoPageWithoutHistory(url) {
    log.debug('MAIN : gotoPageWithoutHistory(' + url + ')');
    parent.window.location.replace(url);
}

// goto page with history
function gotoPageWithHistory(url) {
    log.debug('MAIN : gotoPageWithHistory(' + url + ')');
    parent.window.location.href = url;
}

function creatLangList() {

   if(typeof(parent.LANGUAGE_DATA.current_language) != 'undefined' && parent.LANGUAGE_DATA.current_language == 'ar_sa')
    {
        $('link').attr('href', '../css/main_Arabic.css');
    }
}

loadLangFile();

$(window).load(function() {
    $("#all_content").show();
    creatLangList();
    if (parent.g_needToLogin)
    {
        parent.startLogoutTimer();
    }
});