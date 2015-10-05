var CLIP_MODE_DTMF = '0';
var CLIP_MODE_FSK_ITU = '10';
var CLIP_MODE_FSK_BELL = '11';
var CLIP_MODE_FSK_RPAS = '12';

function onApply(){
    var clip_mode = $("#select_clip_mode").val();
    var data = { 
        ClipMode: clip_mode 
        };
    var xmlclipmode = object2xml('request', data);
    saveAjaxData("api/settings/clip", xmlclipmode, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            button_enable("apply", "0");
            parent.showInfoDialog(common_success);
            initPage();
        }
        else {
            parent.showInfoDialog(common_failed);
        }
    });
}

function initPage(){
    getAjaxData("api/settings/clip",function($xml){    
    var ret =  xml2object($xml);
    if(ret.type == "response")
    { 
        switch(ret.response.ClipMode){
            case CLIP_MODE_DTMF:
            case CLIP_MODE_FSK_ITU:
            case CLIP_MODE_FSK_BELL:
            case CLIP_MODE_FSK_RPAS:
                $("#select_clip_mode").val(ret.response.ClipMode);
                break;
           default:
                break;
        }
    }
    });
    button_enable("apply", 0);
}

function onModeChange(){
    button_enable("apply", 1);
}

$(document).ready(function(){
    initPage();
    $("#apply").click(function(){
        if(isButtonEnable('apply')){
            onApply();
        }
    });
    $("#select_clip_mode").bind("change", onModeChange);
});
