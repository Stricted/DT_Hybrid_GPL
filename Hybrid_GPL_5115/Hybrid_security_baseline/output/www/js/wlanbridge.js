var LAZY = 0;
var BRIDGE = 1;
var DISABLE = 2;
var READY = 0;
var USING = 1;
var Wifienable = parent.G_MonitoringStatus.response.WifiStatus;

function clearInput(){
    var i=0;
    for(;i<4;i++){
        $('#input_peerMacAddress'+i).val("");
        $('#bridge_statusvalue'+i).text("");
    }
}
function initPage(){
    $('input').eq(0).focus();
    clearInput();
    button_enable('apply_button','0');
    getAjaxData("api/wlan/bridge-settings",function($xml){
        var obj = xml2object($xml);
        if(obj.type == 'response'){
            $('#bridge_select').val(obj.response.Bridgeval);
            $('.bridges_text').hide();
            if(obj.response.Bridgeval == BRIDGE){
                $('.bridges_text').show();
                if(typeof(obj.response.Bridges) != 'undefined' && typeof(obj.response.Bridges.Bridge) != 'undefined'){
                    var instances= $(obj.response.Bridges.Bridge);
                    if(instances.length>0)
                    {
                        var i;
                        for(i=0;i<instances.length;i++)
                        {
                            var j = instances[i].InstanceID - 1;
                            $('#input_peerMacAddress'+j).val($.trim(instances[i].X_WlanBridgeMacAddress));
                        }
                    }
                }
            }
        }else{
            $('#bridge_select').val(LAZY);
            $('.bridges_text').hide();
        }
    });
}

function verifybridge(){
    var i=0;
    for(; i<4;i++)
    {
        if($('#input_peerMacAddress'+i).val() != "")
        {
            if(!isValidMacAddress($('#input_peerMacAddress'+i).val()))
            {
                 showErrorUnderTextbox('input_peerMacAddress'+i, wlan_hint_mac_address_invalid);
                 return false; 
             }
             var j = i+1;
             for(;j<4;j++)
             {
                  if(issameRule($('#input_peerMacAddress'+i).val(),$('#input_peerMacAddress'+j).val()))
                  {
                       showErrorUnderTextbox('input_peerMacAddress'+j, same_mac_address);
                       return false;
                   }
               }
        }
    }
    return true;
}

function issameRule(strVariable1,strVariable2)
{
    if($.trim(strVariable1.toUpperCase())==$.trim(strVariable2.toUpperCase()))
    {
        return true;
    }
    return false;
}


function postvalue(){
    var req = '';
    var bridgeRestrictionval = '';
    var instanceID = 0;
    var wlanBridgeMacAddress = '';
    var InstanceArray = [];
    bridgeRestrictionval = parseInt($('#bridge_select').val(),10);
    if(bridgeRestrictionval == LAZY || bridgeRestrictionval == DISABLE){
        req = {
        Bridgeval : bridgeRestrictionval
        };
    }else if(bridgeRestrictionval == BRIDGE){
        for(i=0;i<4;i++)
        {
             if($('#input_peerMacAddress'+i).val()!=""){
                 var fiter = {
                     InstanceID : i + 1,
                     X_WlanBridgeMacAddress : $('#input_peerMacAddress'+i).val(),
                     X_WlanBridgeStatus : ''
                 };
                 InstanceArray.push(fiter);
             }
         }
            req = {
                Bridgeval : bridgeRestrictionval,
                Bridges : {
                    Bridge : InstanceArray
                }
        }; 
    }
    var requestStr = object2xml('request', req);
    saveAjaxData('api/wlan/bridge-settings',requestStr, function($xml){
        var ret = xml2object($xml);
        if(isAjaxReturnOK(ret)){
            parent.showInfoDialog(common_success);
            button_enable('apply_button', 0);
        } else {
            parent.showInfoDialog(common_failed);
            initPage();          
        }
    });
}

function wlanbridge_apply()
{
    if (!isButtonEnable('apply_button'))
    {
        return;
    }
    clearAllErrorLabel();
    if($('#bridge_select').val()!=BRIDGE||verifybridge())
    {
        parent.showConfirmDialog(firewall_hint_submit_list_item, postvalue, function() {});
    }
    button_enable('apply_button', '0');
}

/*--------------------------show content or turn on button------------------*/
function showBtnOrContent() {
    if(Wifienable == 1) {
        initPage();
        $("#show_turn_on_wlan").hide();
        $("#show_briage_main_content").show();
    }
    else {
        $("#show_turn_on_wlan").show();
        $("#show_briage_main_content").hide();
    }
}


$(document).ready(function(){
    showBtnOrContent();
    $("#bridge_select").bind('change',function(){
        button_enable('apply_button', '1'); 
        switch(parseInt($('#bridge_select').val(),10)){
            case LAZY : 
            case DISABLE : 
                $('.bridges_text').hide(); 
                break;
            case BRIDGE : 
                $('.bridges_text').show(); 
                break;
            default :
                $('.bridges_text').hide();
                break;
        }
    });
    $("input").bind('change input paste cut keydown',function(){
        button_enable('apply_button', '1');
        clearAllErrorLabel();
    });
    $('#apply_button').bind('click',function(){
        wlanbridge_apply();
    });
    //click turn button
    $("#wlan_turn_on").click(function() {
        Wifienable = 1;
        var ret = {
                Wifienable : Wifienable
            }
        var req = object2xml("request",ret);
        saveAjaxData("api/wlan/wifienable",req,function($xml) {
            var ret = xml2object($xml);
            if(isAjaxReturnOK(ret)) {
                button_enable('wlan_turn_on', '0');
                initPage();
                $("#show_turn_on_wlan").hide();
                $("#show_briage_main_content").show();
            }
            else {
                return;
            }
        });
    });
});