var DEVICE_MODE_PROJECT_MODE = 0;
var DEVICE_MODE_DEBUG_MODE = 1;

function switch_mode(value)
{
    var request = {
        mode: value    
    };
    
    var xmlstr = object2xml("request", request);
    saveAjaxData("api/device/mode", xmlstr, function($xml){
        var ret = xml2object($xml);
    }); 
}

function closeWindow()
{
    var browserName=navigator.appName;
    if (browserName=="Netscape")
    {
        window.open('','_parent','');
        window.close();
    }
    else
    {
        if (browserName=="Microsoft Internet Explorer")
        {
            window.opener = "whocares";
            window.close();
        }
    }
}