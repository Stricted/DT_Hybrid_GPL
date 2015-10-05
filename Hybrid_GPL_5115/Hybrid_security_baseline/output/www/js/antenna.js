function onApply(){
    var antenna_mode = $("#select_antenna_mode").val();
    var data = { 
        antennamode: antenna_mode 
        };
    var newXmlSetString = object2xml('request', data);
    saveAjaxData("api/settings/antenna", newXmlSetString, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            log.debug("modify antenna mode successed");
            button_enable("apply", "0");
            parent.showInfoDialog(common_success);
            initPage();
        }
        else {
            log.error("modify antenna mode failed");
            parent.showInfoDialog(common_failed);
        }
    });
}

function initPage(){
    getAjaxData('api/settings/antenna', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response') {
            $('#select_antenna_mode').val(ret.response.antennamode);
        }
    });
    button_enable("apply", 0);
}

function onSelectAntannaMode(){
    button_enable("apply", 1);
}

$(document).ready(function(){
    initPage();
    $("#apply").click(function(){
        if(isButtonEnable('apply')){
            onApply();
        }
    });
    $("#select_antenna_mode").bind("change", onSelectAntannaMode);
});
