var g_save_loginfo = "";

var g_display_level = "";
var LEVEL_PREFIX = "log_view_level_";

var log_enable = 1;
var log_disable = 0;
var g_log_info = '';


function getNewLog() {
    getAjaxData('api/settings/log-info', function($xml) {
        var dms_ret = xml2object($xml);
        if (dms_ret.type == 'response') {
            g_log_info = dms_ret.response;
            $('#enable_log').get(0).checked = (log_enable == g_log_info.LogEnable);
            $('#log_level').attr("value",g_log_info.LogLevel);
        } 
    },{
        sync : true
    });
    
    getAjaxData("api/settings/log",function($xml){
        var res = xml2object($xml);
        if(res.type == 'response'){
            if(typeof(res.response.DisplayLevel) != 'undefined') {
                g_save_loginfo += res.response.DisplayLevel + ":" + res.response.log +"\n";
                g_display_level = res.response.DisplayLevel;
                $("#show_log_area").val(g_save_loginfo);
            }
        }
        
    },{
        sync : true
    });
}

/*---------------------get config data---------------------------*/
function getConfigLevel(){
    getConfigData("config/log/config.xml",function($xml){
        var ret = xml2object($xml);
        var html = "";
        var i = 0;
        for(;i<ret.config.Level.length;i++){
            html += "<option value = '"+ret.config.Level[i]+"'>"+eval(LEVEL_PREFIX+ret.config.Level[i])+"</option>"; 
        }
        $("#level_display_select").append(html);
    },{
        sync : true
    });
}

function getData(){
    var submitObject = null;
    if($('#enable_log').attr('checked')){
        submitObject = {
            LogEnable : log_enable,
            LogLevel : $('#log_level').attr("value"),
            LogServer : 0
         };
    }
    else {
        submitObject = {
            LogEnable : log_disable
        };
    }
    return submitObject;
}


/*------------------function ready---------------------*/
$(document).ready(function(){
    getConfigLevel();
    button_enable("apply",0);
        
    $("#level_display_select,#log_level,#enable_log").bind("change",function(){
        button_enable("apply",1);
    });
    getNewLog();
    if(g_display_level != "") {
        $("#level_display_select").val(g_display_level);
    }
    
    $("#apply").click(function(){
        if(isButtonEnable("apply")){
            var status = "1";
            if($('#enable_log').attr('checked'))
            {
                status = "0";
            }
            var submitObject_Amss_Reboot = 
            {
                Status : status
            };
            var newXmlString = object2xml('request', submitObject_Amss_Reboot);
            saveAjaxData("api/settings/amss-reboot",newXmlString,function($xml){   
            });
            var b_setLoginfo = false;
            var newXmlString = object2xml('request', getData());
            saveAjaxData('api/settings/log-info', newXmlString, function($xml) {
                var ret = xml2object($xml);
                if (isAjaxReturnOK(ret)) {
                    b_setLoginfo = true;       
                }else{
                    parent.showInfoDialog(common_failed);
                }
            },{
                sync: true
            });

            if(false==b_setLoginfo)
            {
               return;
            } 
            var apply_level =  {
                Display : $("#level_display_select").val()
            };
            var req = object2xml("request",apply_level);
            saveAjaxData("api/settings/displaylevel",req,function($xml){
                var ret = xml2object($xml);
                if(isAjaxReturnOK(ret)){
                    button_enable("apply",0);
                    parent.showInfoDialog(common_success);
                    g_save_loginfo = "";
                    getNewLog();
                }
                else {
                    parent.showInfoDialog(common_failed);
                }
            },{
                sync: true
            });
        }
    });
});
