// JavaScript Document

function writehh(hh)
{
    var i;
    for(i = 0; i < hh; i++ )
    {
        dw("<option value=" + i + ">" + i + "</option>");
    }
}

function writemm()
{
    var i;
    for(i = 0; i < 60; i++ )
    {
        dw("<option value=" + i + ">" + i + "</option>");
    }
}

function writeday()
{
    var i;
    for(i = 0;i < 7;i++)
    {
        dw("<option value='" + i + "'>" + eval('sntp_label_day_' + i) + "</option>");
    }

}
function writedate()
{
    var i;
    for(i = 1; i <=31; i++ )
    {
        dw("<option value=" + i + ">" + i + "</option>");
    }

}
function onCheckChange()
{
    $('#auto_reboot_week').removeAttr('disabled');
    $('#auto_reboot_days').removeAttr('disabled');
    if($('#autoreboot_day').attr('checked') == "checked")
    {     
        $('#auto_reboot_week').attr('disabled',true);  
        $('#auto_reboot_days').attr('disabled',true);  
    }   
    if($('#autoreboot_week').attr('checked') == "checked")
    {
        $('#auto_reboot_week').removeAttr('disabled');
        $('#auto_reboot_days').attr('disabled',true); 
    } 
    if($('#autoreboot_month').attr('checked') == "checked")
    {             
        $('#auto_reboot_week').attr('disabled',true);  
        $('#auto_reboot_days').removeAttr('disabled');
    }
}
function onRadioChange() 
{
    if($("#auto_reboot_enable").get(0).checked) 
    {
        $('#auto_reboot_settings').show(); 
    }
    else 
    {
        $('#auto_reboot_settings').hide();
    }
    button_enable("button_apply", "1");
}
function writecurrtime(sntpcurrtime)
{
    var currtime2;
    var currtime = sntpcurrtime.split('T');
    currtime2 = currtime[1].split('Z');
    $('#sntp_currenttime').html((sntp_label_currenttime + common_colon+currtime[0] + " " + currtime2[0]));
}

function initPage()
{ 
    getAjaxData("api/settings/AutoReboot", function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            if(ret.response.enable == 1) 
            {
                $('input[name=auto_reboot][value=1]').attr('checked', true);              
            }
            else 
            {
                $('input[name=auto_reboot][value=0]').attr('checked', true);
            }
            $("#autoreboot_day").removeAttr('checked');        
            $("#autoreboot_week").removeAttr('checked');
            $("#autoreboot_month").removeAttr('checked');
                
            if(typeof(ret.response.hour) != "undefined")
            {
                $('#auto_reboot_hour').val(ret.response.hour);
            }
            if(typeof(ret.response.minute) != "undefined")
            {
                $('#auto_reboot_minute').val(ret.response.minute);
            }
                
            if(ret.response.policy==1)
            {
                $("#autoreboot_day").attr("checked",true);
            }
            else if(ret.response.policy==2)
            {
                $("#autoreboot_week").attr("checked",true);
                if(typeof(ret.response.week) != "undefined")
                {
                    $('#auto_reboot_week').val(ret.response.week);
                }                  
            }
            else
            {
                $("#autoreboot_month").attr("checked",true);
                if(typeof(ret.response.day) != "undefined")
                {
                    $('#auto_reboot_days').val(ret.response.day);
                }
            }
            onRadioChange();
            onCheckChange();
        }
        else {
            log.error("Error, no data");
        }

    },{
        sync: true
    });
    
    getAjaxData('api/settings/sntp', function($xml) {
           var ret = xml2object($xml);
           if (ret.type == 'response') {
               var res=ret.response;
               if(typeof(res.CurrentLocalTime)!='undefined' && ''!=res.CurrentLocalTime)
               {
                   writecurrtime(res.CurrentLocalTime);
               }else
               {
                   $('#sntp_currenttime').html("");
               }              
           }
       },{
            sync:true
       });

    button_enable("button_apply", "0");
}

function onApply()
{
    var postdate = {};
    postdate.enable=$("input[name='auto_reboot']:checked").val();
    if($("#auto_reboot_enable").get(0).checked) 
    {
        postdate.hour = $('#auto_reboot_hour').val();
        postdate.minute = $('#auto_reboot_minute').val();
           
        if($('#autoreboot_day').attr('checked') == "checked")
        {
            postdate.policy = 1;
        }
        else if($('#autoreboot_week').attr('checked') == "checked")
        {
            postdate.policy = 2;  
            postdate.week = $('#auto_reboot_week').val();
        }
        else
        {
            postdate.policy = 3; 
            postdate.day = $('#auto_reboot_days').val();
        }
    }
    var xmlstr = object2xml("request", postdate);
    saveAjaxData("api/settings/AutoReboot", xmlstr, function($xml) {
        var bret = xml2object($xml);
        if(isAjaxReturnOK(bret)) {
            parent.showInfoDialog(common_success);         
        }
        else {
            parent.showInfoDialog(common_failed);        
        }
        initPage();
    });
}

$(document).ready(function() {

    $('#button_apply').bind('click', function() {
        if(!isButtonEnable("button_apply")) 
        {
            return;
        }
        parent.showConfirmDialog(firewall_hint_submit_list_item, onApply, function() {});
        return false;
    }); 
    $("#auto_reboot_enable,#auto_reboot_disable").click( function() {
        onRadioChange();
    });
    $('#autoreboot_day').click(function(){
        if($('#autoreboot_day').attr('checked') == "checked")
        {
            $("#autoreboot_week").removeAttr('checked');
            $("#autoreboot_month").removeAttr('checked');
        }
        onCheckChange();
    });
    $('#autoreboot_week').click(function(){
        if($('#autoreboot_week').attr('checked') == "checked")
        {
            $("#autoreboot_day").removeAttr('checked');
            $("#autoreboot_month").removeAttr('checked');
        }
        onCheckChange();
    });
    $('#autoreboot_month').click(function(){
        if($('#autoreboot_month').attr('checked') == "checked")
        {
            $("#autoreboot_week").removeAttr('checked');
            $("#autoreboot_day").removeAttr('checked');
        }
        onCheckChange();
    });    
   
    $('input,select').die().live('keydown click change', function() {
        button_enable('button_apply', '1');
    }); 
    initPage(); 
});
