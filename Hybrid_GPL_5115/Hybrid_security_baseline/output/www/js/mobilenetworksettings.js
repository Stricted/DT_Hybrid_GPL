var network_isChecked = "checked";
var network_setting_networkBands = "NetworkBands";
var network_setting_plmn = "Plmn";
var network_setting_rat = "Rat";

var g_band_display = true;

var g_plmn_list = [];
var g_count = 0;
var g_current_net = {
    numericname: null,
    rat: null
};
var SETTING_DIALUP_MACRO_INVALID_SIM_CARD = '0';
//var NETWORK_REGISTERMODE_DEFAULT = "1";
var SETTING_DIALUP_AUTO_SEARCH_NET = 0;
var SETTING_DIALUP_MANUAL_SEARCH_NET = 1;
var SETTING_DIALUP_RAT_2G = '0';
var SETTING_DIALUP_RAT_3G = '2';
var SETTING_DIALUP_RAT_4G = '7';
var SETTING_DIALUP_PLMN = null;
var DEFAULT_DEVICE_NAME = 'default';
var PLMN_USABLE = '1';
var PLMN_REGISTERED = '2';
var PLMN_FORBIDDEN = '3';
var networkband_enable = true;
var g_setting_netWork = {
    NetworkMode: null,
    NetworkBand: null,
    LTEBand: null
};
var g_networkband = {
    NetworkBands: null
};
var g_setting_register = {
    Mode: null,
    Plmn: null,
    Rat: null
};

var NETWORKMODES = {
    "00": common_auto,
    "01": plmn_label_2g,
    "02": plmn_label_3g,  
    "03": plmn_label_4g
};
var g_setting_netWorkModeList = [["00", NETWORKMODES["00"]],["01", NETWORKMODES["01"]], ["02", NETWORKMODES["02"]],["03", NETWORKMODES["03"]]]; //default mode


var gandwnochange = "40000000";
var gsmwcdmsall = "3FFFFFFF";
var ltebandall = "7FFFFFFFFFFFFFFF";

var g_arrNetworkBand = [];
g_arrNetworkBand.push([0x400000, "GSM_BAND_3"]);
g_arrNetworkBand.push([0x200000, "GSM_BAND_8"]);
g_arrNetworkBand.push([0x400000, Label_WCDMA_BAND_1]);
g_arrNetworkBand.push([0x800000, Label_WCDMA_BAND_2]);
g_arrNetworkBand.push([0x2000000, Label_WCDMA_BAND_4]);
g_arrNetworkBand.push([0x4000000, Label_WCDMA_BAND_5]);
g_arrNetworkBand.push([0x2000000000000, Label_WCDMA_BAND_8]);
g_arrNetworkBand.push([0x40000000, Label_CM_BAND_PREF_NO_CHANGE]);
g_arrNetworkBand.push([0x0001, Label_LTE_BAND_1]);
g_arrNetworkBand.push([0x00000002, Label_LTE_BAND_2]);
g_arrNetworkBand.push([0x0004, Label_LTE_BAND_3]);
g_arrNetworkBand.push([0x00000008, Label_LTE_BAND_4]);
g_arrNetworkBand.push([0x00000010, Label_LTE_BAND_5]);
g_arrNetworkBand.push([0x00000040, Label_LTE_BAND_7]);
g_arrNetworkBand.push([0x0080, Label_LTE_BAND_8]);
g_arrNetworkBand.push([0x00000800, Label_LTE_BAND_12]);
g_arrNetworkBand.push([0x00001000, Label_LTE_BAND_13]);
g_arrNetworkBand.push([0x00010000, Label_LTE_BAND_17]);
g_arrNetworkBand.push([0x00080000, Label_LTE_BAND_20]);
g_arrNetworkBand.push([0x2000000000, Label_LTE_BAND_38]);
g_arrNetworkBand.push([0x8000000000, Label_LTE_BAND_40]);
var networkBandList = [];
function setting_dialup_getNetworkBand(device_name) {
    var band_list = '';
    var default_band_list = ''; 
    var networkband_conf_file = 'config/network/networkband_' + device_name + '.xml';// device_name;

    getConfigData(networkband_conf_file, function($xml)
    {
        var ret = xml2object($xml);
        if (ret.type == 'config')
        {
            if(typeof(ret.config.NetworkBands) != "undefined" && ret.config.NetworkBands.Band) 
            {
                g_networkband.NetworkBands = ret.config.NetworkBands.Band;
                setBands();
                setTimeout(setmutiselect,100);
            }
        }
    },{
        sync : true,
        errorCB:function(){
            setting_dialup_getNetworkBand("B890-66");
        }
    });
}

function getcurrBand(){
    var networkband = "";
    var wcdma_val = 0;
    var lte_val = 0;
    $(".wcdma").each(function(){
        if(this.checked == true){
            wcdma_val += parseInt(this.value,16);
        }
        wcdma_all &= this.checked;
    });
    wcdma_val = wcdma_val.toString(16);
    for(var i=wcdma_val.length;i<16;i++)
    {
        wcdma_val = "0" + wcdma_val;
    }
    $(".lte").each(function(){
        if(this.checked == true){
            lte_val += parseInt(this.value,16);
        }
        lte_all &= this.checked;
    });
    lte_val = lte_val.toString(16);
}

function setBands() {
    $.each(g_networkband[network_setting_networkBands], function(index, value)
    {
        var str = getBandString(parseInt(value, 16));//从10进制改为16进制
        
        networkBandList[index] = [];
        networkBandList[index][0] = value;
        networkBandList[index][1] = str;
        setTimeout(function(){},1);
        log.debug('Band string = ' + str);
    });
}


function setmutiselect() {
    var networkband_gsm;
    var networkband_wcdma;
    var networkband_lte = g_setting_netWork.LTEBand;

    if(g_setting_netWork.NetworkBand.length<=8)
    {
        networkband_wcdma = parseInt(g_setting_netWork.NetworkBand,16);
    }else{
        networkband_wcdma1 = parseInt(g_setting_netWork.NetworkBand.substr(g_setting_netWork.NetworkBand.length-8),16);;
        networkband_wcdma2 = parseInt(g_setting_netWork.NetworkBand.substr(g_setting_netWork.NetworkBand.length-16,8),16);;
    } 
    $.each(networkBandList, function(n, value) {
        var wband = /WCDMA/i;
        var lteband = /LTE/i;
        var gsmband = /GSM/i;
        var wband_list = "";
        var lteband_list = ""; 
        var gsmband_list = "";    

        if (value[1].match(wband) != null)
        {
            var temp1;
            var temp2;
            if(value[0].length <= 8)
            {
              temp1 = parseInt(value[0],16);
              temp2 = 0;
            }else{
              temp1 = parseInt(value[0].substr(value[0].length-8),16);
              temp2 = parseInt(value[0].substr(0,8),16)
            }
            if((networkband_wcdma & temp1) !=0)
            {
                wband_list = '<tr><td></td><td height ="25px" width ="20px"><input class = "wcdma" type="checkbox" checked="checked" id ="'+value[1]+'" value="'+value[0]+'"/></td><td><label>'+value[1]+'</label></td>'+'</tr>';
            }else{
                wband_list = '<tr><td></td><td height ="25px" width ="20px"><input class = "wcdma" type="checkbox" id ="'+value[1]+'" value="'+value[0]+'"/></td><td><label>'+value[1]+'</label></td>'+'</tr>';
            }
        }
        else if (value[1].match(gsmband) != null){
            var temp1;
            var temp2;
            if(value[0].length <= 8)
            {
              temp1 = 0;
              temp2 = 0;
            }else{
              temp1 = parseInt(value[0].substr(0,8),16)
            }
            if((networkband_gsm & temp1) !=0)
            {
                gsmband_list = '<tr><td></td><td height ="25px" width ="20px"><input class = "gsm" type="checkbox" checked="checked" id ="'+value[1]+'" value="'+value[0]+'"/></td><td><label>'+value[1]+'</label></td>'+'</tr>';
            }else{
                gsmband_list = '<tr><td></td><td height ="25px" width ="20px"><input class = "gsm" type="checkbox" id ="'+value[1]+'" value="'+value[0]+'"/></td><td><label>'+value[1]+'</label></td>'+'</tr>';
            }
        }      
        else if (value[1].match(lteband) != null)
        {
            if((networkband_lte & parseInt(value[0],16)) != 0)
            {
                lteband_list = '<tr><td></td><td height ="25px" width ="20px"><input class = "lte" type="checkbox" checked="checked" id ="'+value[1]+'" value="'+value[0]+'"/></td><td><label>'+value[1]+'</label></td>'+'</tr>';
            }
            else{
                lteband_list = '<tr><td></td><td height ="25px" width ="20px"><input class = "lte" type="checkbox" id ="'+value[1]+'" value="'+value[0]+'"/></td><td><label>'+value[1]+'</label></td>'+'</tr>';
            }
        }
        else
        {
            //band_list = '<option class = "auto" value = ' + value[0] + '>' + value[1] + '</option>';
        }
        $('#2g_band').append(gsmband_list);
        $('#3g_band').append(wband_list);
        $('#4g_band').append(lteband_list);
        
    });
    
    $('.gsm, .wcdma, .lte').bind("click",function() {
        button_enable('network_apply', '1');
        g_setting_netWork.NetworkBand = getcurrBand();
    });
}


//a method includ 3 function
function setting_dialup_initVar() {
    this.getNetwork = function()
    {
        getAjaxData('api/net/networkex', function($xml)
        {
            var ret = xml2object($xml);
            if (ret.type == 'response')
            {
                var temp_netband = '';
                g_setting_netWork.NetworkMode = ret.response.NetworkMode;
                g_setting_netWork.NetworkBand = ret.response.NetworkBand;
                g_setting_netWork.LTEBand = ret.response.LTEBand;

                setTimeout(function() {
                    $('#preferred_mode_select').val(g_setting_netWork.NetworkMode);
                },1);
            }
        },
        {
            sync:true,
            errorCB: function()
            {
                log.error('MOBILENETWORKSETTING:get api/net/networkex file failed');
            }
        });
    }; //function 1 end

    this.getInfomation = function()
    {
        if(networkband_enable)
        {
            getAjaxData('api/device/information', function($xml)
            {
                var device_ret = xml2object($xml);
                var device_name = null;
                if (device_ret.type == 'response')
                {
                    device_name = getDeviceType(device_ret.response.DeviceName);
                    setting_dialup_getNetworkBand(device_name);
                }
                else
                {
                     log.error('MOBILENETWORKSETTING:get api/device/infomation data error');
                }
            },
            {
                sync:true,
                errorCB: function()
                {
                    log.error('MOBILENETWORKSETTING: get api/device/infomation file failed');
                }
            });
        }       
    };

    this.getRegister = function()
    {
        getAjaxData('api/net/register', function($xml)
        {
            var ret = xml2object($xml);
            if (ret.type == 'response')
            {
                g_setting_register.Mode = parseInt(ret.response.Mode, 10);
                g_setting_register.Plmn = ret.response.Plmn;
                g_setting_register.Rat = ret.response.Rat;
                setTimeout(function() {
                    $('#network_select').val(g_setting_register.Mode);
                    if(g_setting_register.Mode == "1")
                    {
                        button_enable("mobilensetting_apply", 1)
                    }
                    else
                    {
                        button_enable("mobilensetting_apply", 0)
                    }
                },1);
            }
            else
            {
                log.error('MOBILENETWORKSETTING:get api/net/register data error');
            }
        },
        {
            errorCB: function()
            {
                log.error('MOBILENETWORKSETTING:get api/net/register file failed');
            }
        });
    };
}

var g_myInitVar = new setting_dialup_initVar();  //new  a method

function getNetworkConfig() {
    getConfigData('config/network/networkmode.xml', function($xml) {
        var tempMode = [];
        var ret = xml2object($xml);
        if (ret.type == 'config')
        {
            if (ret.config.networkband_enable == '0')
            {
                 networkband_enable = false;
            }
            if (ret.config.NetworkModes.Mode)
            {
                ret = ret.config.NetworkModes.Mode;
                if ($.isArray(ret))
                {
                   tempMode = ret;
                }
                else
                {
                    tempMode.push(ret);
                }
                g_setting_netWorkModeList = [];
                $.each(tempMode, function(n, value)
                {
                    g_setting_netWorkModeList.push([value, NETWORKMODES[value]]);
                });
            }
        }
    },
    {
        sync: true
    });
}

//    save user selected network (include mode and band)
function setting_dialup_saveVar() {
    if (!isButtonEnable('network_apply'))
    {
        return;
    }
    g_setting_netWork.NetworkMode = $('#preferred_mode_select').val() ;
    if(!networkband_enable)
    {
       if(parseInt(g_setting_netWork.NetworkBand,16)==0){
        g_setting_netWork.NetworkBand = gsmwcdmsall;
       }
       if(parseInt(g_setting_netWork.LTEBand,16)==0){
        g_setting_netWork.LTEBand = ltebandall;
       }
    }
    if(parseInt(g_setting_netWork.NetworkBand,16)==0 ||parseInt(g_setting_netWork.LTEBand,16)==0 )
    {
        parent.showInfoDialog(connection_label_bandnull_error);
        return;
    }
    parent.showWaitingDialog(common_waiting,common_waiting);
    var xmlstr = object2xml('request', g_setting_netWork);
    saveAjaxData('api/net/networkex', xmlstr, function($xml)
    {
        parent.closeWaitingDialog();
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret))
        {
            button_enable('network_apply', '0');
            parent.showInfoDialog(common_success);
        }
        else
        {
            parent.showInfoDialog(common_failed);
            log.error('MOBILENETWORKSETTING:post api/net/network data error');
            button_enable('network_apply', '1');
        }
    },
    {
        errorCB: function()
        {
            parent.closeWaitingDialog();
            parent.showInfoDialog(common_failed);
            log.error('MOBILENETWORKSETTING:post api/net/networkex filet failed');
        }
    });
}


function set_bandcheck(){
    if(!networkband_enable)
    {
        // hide band selectclom
        g_setting_netWork.NetworkBand = gsmwcdmsall;
        g_setting_netWork.LTEBand = ltebandall;
        return ;
    }else{
        //display band selectclom

    }
}

//
function setting_dialup_initSelectOption() {
    var mode_list = '';
    $.each(g_setting_netWorkModeList, function(n, value) {
        mode_list = '<option value = ' + value[0] + '>' + value[1] + '</option>';
        $('#preferred_mode_select').append(mode_list);
        });
    $('#preferred_mode_select').change(function() {
        if($('#preferred_mode_select').val() == g_setting_netWork.NetworkMode)
        {
            button_enable('network_apply', '0');
            return;
        }
        set_bandcheck();
        button_enable('network_apply', '1');
    });
    var netWork_list = '';
    var searchNetWorkModeList = [[SETTING_DIALUP_MANUAL_SEARCH_NET, common_manual], [SETTING_DIALUP_AUTO_SEARCH_NET, common_auto]];
    $.each(searchNetWorkModeList, function(n, value) {
    netWork_list = '<option value = ' + value[0] + '>' + value[1] + '</option>';
    $('#network_select').append(netWork_list);
    });

    $('#network_select').change(function() {
        match_plmn_name();
        button_enable('mobilensetting_apply', '1');
        setting_dialup_menuNetMode(parseInt(this.value, 10));
    });
}

//
function BitwiseAndLarge(val1, val2) {
    var shift = 0, result = 0;
    var mask = ~ ((~ 0) << 30); // Gives us a bit mask like 01111..1 (30 ones)
    var divisor = 1 << 30; // To work with the bit mask, we need to clear bits at a time
    while ((val1 != 0) && (val2 != 0))
    {
        var rs = (mask & val1) & (mask & val2);
        val1 = Math.floor(val1 / divisor); // val1 >>> 30
        val2 = Math.floor(val2 / divisor); // val2 >>> 30
        var i = shift++;
        for (i; i--;)
        {
            rs *= divisor; // rs << 30
        }
        result += rs;
    }
    return result;
}

function getBandString(band) {
    var bandstr = '';
    // band is any
    if (band == 0x6800000 || band == 0x140FFFF)
    {
        bandstr = Label_CM_BAND_PREF_ANY;
    }
    // band is automatic
    else if (band == 0x680380)
    {
        bandstr = common_auto;
    }
    else
    {
        jQuery.each(g_arrNetworkBand, function(i, n)
        {
            if (BitwiseAndLarge(n[0], band) > 0)
            {
                if (bandstr.length > 0)
                {
                    bandstr += '/';
                }
            bandstr += n[1];
            }
        });
    }
    return bandstr;
}
function setting_dialup_registerNetMode(mode) {
    if(!isButtonEnable("mobilensetting_apply")) {
        return;
    }
    if (SETTING_DIALUP_AUTO_SEARCH_NET == mode)
    {
        g_setting_register.Plmn = '';
        g_setting_register.Rat = '';
        var xmlstr = object2xml('request', g_setting_register);
        saveAjaxData('api/net/register', xmlstr, function($xml)
        {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret))
            {
                parent.showInfoDialog(common_settings_successfull);
            }
            else
            {
                parent.showInfoDialog(common_failed);
            }
            g_myInitVar.getRegister();
        });
        return;
    }
    parent.g_is_login_opened = true;
    parent.cancelLogoutTimer();

    parent.showWaitingDialog_network(common_waiting, IDS_dialup_label_searching_network);
    getAjaxData('api/net/plmn-list', function($xml)
    {
        var ret = xml2object($xml);
        g_plmn_list = [];
        if (ret.type == 'response')
        {
            if (ret.response.Networks.Network)
            {
                if ($.isArray(ret.response.Networks.Network))
                {
                    g_plmn_list = ret.response.Networks.Network;
                }
                else
                {
                    g_plmn_list.push(ret.response.Networks.Network);
                }
            }
            g_count = 0;
            setting_dialup_showPlmnList(g_plmn_list);
            parent.clearInfoDialog();
            parent.startLogoutTimer();
        }else if((ret.type == 'error') && (ret.error.code == '3')){
            g_count ++;
            if(g_count >= 8){
                g_count = 0;
                parent.closeWaitingDialog();
                parent.showInfoDialog(common_failed);
            }else {
                setTimeout(function(){
                    parent.closeWaitingDialog();
                    setting_dialup_registerNetMode(g_setting_register.Mode);
                    },6000);
            }
        }else{
            g_count = 0;
            parent.startLogoutTimer();
            parent.closeWaitingDialog();
            parent.showInfoDialog(common_failed);
            log.error('MOBILENETWORKSETTING:get api/net/plmn-list data error');
            button_enable('mobilensetting_apply', '1');
        }
    },
    {
        timeout: 300000,
        errorCB: function(XMLHttpRequest, textStatus)
        {
            parent.startLogoutTimer();
            parent.closeWaitingDialog();
            var errorInfo = ('timeout' == textStatus) ? common_timeout : common_failed;
            parent.showInfoDialog(errorInfo);
            log.error('MOBILENETWORKSETTING:get api/net/plmn-list file failed');
            button_enable('mobilensetting_apply', '1');
        }
    });
    }
    //
function setting_dialup_setNetMode(numeric, rat) {
    g_setting_register[network_setting_plmn] = numeric;
    g_setting_register[network_setting_rat] = rat;
}
//

function setting_dialup_menuNetMode(mode) {
    switch (mode)
    {
        case SETTING_DIALUP_AUTO_SEARCH_NET:
            g_setting_register.Mode = mode;
            $('#network_select').val(mode);
            break;
        case SETTING_DIALUP_MANUAL_SEARCH_NET:
            g_setting_register.Mode = mode;
            $('#network_select').val(mode);
            break;
        default:
            break;
    }
}
//
function setting_dialup_searchAndRegister(mode) {
    if (mode == SETTING_DIALUP_MANUAL_SEARCH_NET)  // to search
    {
        parent.cancelLogoutTimer();
        g_setting_register.Plmn = '';
        g_setting_register.Rat = '';
        parent.showWaitingDialog_network(common_waiting, common_searching);
    }

    if (mode == null)                            // to g_setting_register
    {
        parent.showWaitingDialog_network(common_waiting, setting_label_registering_network);
    }

    xmlstr = object2xml('request', g_setting_register);
    saveAjaxData('api/net/register', xmlstr, function($xml)
    {
        var ret = xml2object($xml);
        if (ret.type == 'response') //setting success
        {
            //(auto / manual)
            if (isAjaxReturnOK(ret))//(true)//
            {
                switch (mode)
                {
                    case SETTING_DIALUP_MANUAL_SEARCH_NET:      //amnual
                        setting_dialup_registerNetMode();
                        break;
                    case SETTING_DIALUP_AUTO_SEARCH_NET:        //auto
                        parent.showInfoDialog(common_success);
                        break;
                    default:
                        parent.startLogoutTimer();
                        parent.closeWaitingDialog();
                        parent.showInfoDialog(common_success);         //register
                        //myInitVar.getRegister();
                        break;
                }
                button_enable('mobilensetting_apply', '1');
            }//return ok
            else // setting falid
            {
                parent.startLogoutTimer();
                parent.closeWaitingDialog();
                parent.showInfoDialog(common_failed);
                g_myInitVar.getRegister();
                log.error('MOBILENETWORKSETTING:post api/net/register not return OK');
            }
        }
        else  //seetting error
        {
            parent.startLogoutTimer();
            parent.closeWaitingDialog();
            parent.showInfoDialog(common_failed);
            g_myInitVar.getRegister();
            log.error('MOBILENETWORKSETTING:post api/net/register data error');
        }
    },
    {
        timeout: 120000,  // 2 minutes
        //sync:true,
        errorCB: function(XMLHttpRequest, textStatus)
        {
            parent.startLogoutTimer();
            parent.closeWaitingDialog();
            var errorInfo = ('timeout' == textStatus) ? common_timeout : common_failed;
            parent.showInfoDialog(errorInfo);
            g_myInitVar.getRegister();
            log.error('MOBILENETWORKSETTING:post api/net/register file failed');
        }
    });//saveAjaxData end
}

//api/net/current-plmn
function setting_dialup_getCurrPlmn() {
    getAjaxData('api/net/current-plmn', function($xml)
    {
        var ret = xml2object($xml);
        if (ret.type == 'response')
        {
            g_current_net.numericname = ret.response.Numeric;
            g_current_net.rat = ret.response.Rat;
        }
        else
        {
            //showInfoDialog(common_failed);
            log.error('MOBILENETWORKSETTING:get api/net/current-plmn data error');
        }
    },
    {
        sync:true,
        errorCB: function()
        {
            log.error('MOBILENETWORKSETTING:get api/net/current-plmn file failed');
        }
    });
}

//pop ok button click
function popok_button() {
    if(g_plmn_list.length > 0) {
        if (!isButtonEnable('pop_OK'))
        {
            return;
        }
        parent.$('#div_wrapper,.dialog').hide();
        var index = parent.$('#plmn_list :checked').val();
        setting_dialup_setNetMode(g_plmn_list[index].Numeric, g_plmn_list[index].Rat);
        setting_dialup_searchAndRegister(null);
    }
    else {
        parent.$('#div_wrapper,.dialog').hide();
        parent.startLogoutTimer();
        g_myInitVar.getRegister();
    }
}

//cancel click
function cancel_button() {
    parent.$('#div_wrapper,.dialog').hide();
    button_enable('mobilensetting_apply', '1');
    parent.startLogoutTimer();
}

function setting_dialup_showPlmnList(plmnList) {
    parent.closeWaitingDialog();
    parent.call_dialog(setting_label_listing_network, "<table id='plmn_list' class='plmn_list'></table>", common_ok, 'pop_OK', common_cancel, 'pop_Cancel',popok_button,cancel_button);
    var plmn_li_list = [];
    if (plmnList.length > 0)
    {
        var ifChecked = '';
        $.each(plmnList, function(n, value)
        {
            var plmnState = null;
            var Rat_Status = null;
            switch (value.State)
            {
                case PLMN_USABLE:
                    plmnState = plmn_label_usable;
                    break;
                case PLMN_REGISTERED:
                    plmnState = plmn_label_registered;
                    break;
                case PLMN_FORBIDDEN:
                    plmnState = plmn_label_forbidden;
                    break;
                default:
                    plmnState = common_unknown;
                    break;
            }

            switch(value.Rat)
            {
                case SETTING_DIALUP_RAT_2G:
                    Rat_Status = plmn_label_2g;
                    break;
                case SETTING_DIALUP_RAT_3G:
                    Rat_Status = plmn_label_3g;
                    break;
                case SETTING_DIALUP_RAT_4G:
                    Rat_Status = plmn_label_4g;
                    break;
                default:
                    Rat_Status = common_unknown;
                    break;
            }
            
            if (plmnState == plmn_label_registered)
            {
                ifChecked = network_isChecked;
            }
            else
            {
                ifChecked = '';
            }
            plmn_li_list += "<tr><td ><input type = 'radio' name='netMode' value = '" + n + "' " + ifChecked + " id='netMode_" + n + "'></td>"
                        + "<td ><span id ='netMode_" + n + "'>" + value.ShortName + ' '
                        + "</span><span id ='RatMode_" + n + "'>" + Rat_Status + ' '
                        + "</span><span id ='UseMode_" + n + "'>(" + plmnState +")</label></td></tr>";
        });
    }
    else
    {
        plmn_li_list = '<tr><td>' + setting_label_no_network + '</td></tr>';
        parent.$('#pop_Cancel').hide();
        button_enable('mobilensetting_apply', '1');
    }
    parent.$('#plmn_list').append(plmn_li_list);
    
    parent.$("#netMode_0").attr("checked",true);
    
    parent.$('.dialog_close_btn').bind('click', function()
    {
        parent.$('#div_wrapper,.dialog').hide();
        button_enable('mobilensetting_apply', '1');
        parent.startLogoutTimer();
    });
    //reputPosition(parent.$('#sms_dialog'), parent.$('#div_wrapper'));
}

function setting_dialup_connectionStatus() {
    var simstate = parent.G_MonitoringStatus.response.SimStatus;
    
    var connectionIsUnFree = (
            MACRO_SIM_STATUS_USIM_N == simstate
            ||MACRO_SIM_STATUS_USIM_NE == simstate
            );
    if (connectionIsUnFree)
    {
        $('#network_select').attr('disabled', true);
        button_enable('mobilensetting_apply', '0');
    }
    else
    {
        $('#network_select').attr('disabled', false);
        parent.$('#pop_OK').removeClass('disable_btn');
    }
    
    setTimeout(function() {
        setting_dialup_connectionStatus();
    },parent.g_feature.update_interval);
}


function getDeviceType(deviceName)
{
    var szTmp = deviceName.split('-');
    var deviceType = null;
    deviceType = deviceName.match(/[a-zA-Z]+\d+/);
    if (2 == szTmp.length)
    {
        deviceType = deviceType + '-' + szTmp[1];
    }
    return deviceType;
}

function match_plmn_name() {
    getAjaxData('api/net/current-plmn', function($xml) {
        var currently_plmn = null;
        var current_plmn_ret = xml2object($xml);
        if (current_plmn_ret.type == 'response')
        {
            currently_plmn = current_plmn_ret;
        }
    },
    {
        sync: true
    });
}

//redirectOnCondition(null, 'setting');
$(document).ready(function() {
    button_enable("network_apply",0);
    button_enable('mobilensetting_apply','0');
    getNetworkConfig();
    setting_dialup_initSelectOption();

    g_myInitVar.getNetwork();
    g_myInitVar.getInfomation();
    g_myInitVar.getRegister();

    setting_dialup_connectionStatus();
    set_bandcheck();


    $("#mobilensetting_apply").bind("click",function(){
        
        if(!isButtonEnable("mobilensetting_apply")) {
            return;
        }
        var dialupStatus = parent.G_MonitoringStatus.response.ConnectionStatus;
        var ipv6dialupStatus = parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus;
        var connectionIsUnFree = (          
            MACRO_CONNECTION_CONNECTED == dialupStatus ||           
            MACRO_CONNECTION_IPV6CONNECTED == ipv6dialupStatus    
            );
        if (connectionIsUnFree && (parent.G_MonitoringStatus.response.ServiceStatus == "2"))
        {
            parent.showConfirmDialog(label_hint_operation_terminate_connection, 
                function() {setting_dialup_registerNetMode(g_setting_register.Mode);},
                function() {}
            );
        }
        else
        {
            setting_dialup_registerNetMode(g_setting_register.Mode);
        }        
        
    });
    $('#network_apply').bind('click', function()
    {       
        if (!isButtonEnable('network_apply'))
        {
            return;
        }
        var dialupStatus = parent.G_MonitoringStatus.response.ConnectionStatus;
        var ipv6dialupStatus = parent.G_MonitoringStatus.response.X_IPv6ConnectionStatus;
        var connectionIsUnFree = (          
            MACRO_CONNECTION_CONNECTED == dialupStatus ||           
            MACRO_CONNECTION_IPV6CONNECTED == ipv6dialupStatus    
            );
        if (connectionIsUnFree && (parent.G_MonitoringStatus.response.ServiceStatus == "2"))
        {
            parent.showConfirmDialog(label_hint_operation_terminate_connection, setting_dialup_saveVar, function() {});
        }
        else
        {
            setting_dialup_saveVar();
        }               
        
    });
    $("#wcdma_all_band").click(function(){
        if(this.checked == true){
            $(".wcdma").attr("checked",true);
        }else{
            $(".wcdma").attr("checked",false);
        }
        button_enable("network_apply", 1);
        g_setting_netWork.NetworkBand = getcurrBand();
    });
    $("#lte_all_band").click(function(){
        if(this.checked == true){
            $(".lte").attr("checked",true);
        }else{
            $(".lte").attr("checked",false);
        }
        button_enable("network_apply", 1);
        g_setting_netWork.NetworkBand = getcurrBand();
    });
});


