var FILTER_MODE_DENY = 2;
var FILTER_MODE_ALLOW = 1;
var BLACK_URL = "black";
var WHITE_URL = "white";
var URL_FILTER_NUM = 16;
var g_ok_flag = 0;
var g_enabled = true;
var g_deleteIndex = 0;
var index = 0;
var g_urlWhite = null;
var g_urlBlack = null;

function isvailidUrl(){
  
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    if(isValidUrlName($.trim($('#input_url').val()))==false)
    {
        showQtip('input_url', samba_input_invalid);
        return false;
    }
    return true;  
}
function isValidUrlName(url)
{
    var i=0;
    var j=0;
    var invalidArray = new Array();
    invalidArray[i++] = "www";
    invalidArray[i++] = "com";
    invalidArray[i++] = "org";
    invalidArray[i++] = "net";
    invalidArray[i++] = "edu";
    invalidArray[i++] = "www.";
    invalidArray[i++] = ".com";
    invalidArray[i++] = ".org";
    invalidArray[i++] = ".net";
    invalidArray[i++] = ".edu";

    if (url=='')
    {
        return false;
    }
    
    if (isValidAscii(url) != '')
    {
        return false;
    }

    for (i = 0; i < url.length; i++)
    {
        if (url.charAt(i) == ' ')
        {
             return false;
        }
        if (url.charAt(i) == '\\')
        {
             return false;
        }
    }

    if ((url.length < 3 && url.length > 0) || url.length > 31 )
    {
        return false;
    }
    for(j=0; j< invalidArray.length; j++)
    {
        if (url == invalidArray[j])
        {
            return false;
        } 
    }
    return true;
}
function isSameRule()
{
    $.each($('.qtip-defaults'), function() {
        $(this).remove();
    });
    var url_address = $.trim($('#input_url').val());
    var issameRule=false;
    if($("#urlfilter_mode_select").val() == FILTER_MODE_ALLOW)
    {
        $('.show_'+WHITE_URL).each(function() {
            
            if(url_address==$(this).children().eq(0).text())
            {
                issameRule=true;
                showQtip('input_url', label_same_rule_exist);
            }
        });
    }
    else if($("#urlfilter_mode_select").val() == FILTER_MODE_DENY) 
    {
         $('.show_'+BLACK_URL).each(function() {

            if(url_address==$(this).children().eq(0).text())
            {
                issameRule=true;
                showQtip('input_url', label_same_rule_exist);
            }
        });
    }  
    return issameRule;
}
function onApply() {
    var submitObject = {};
    var blackarray = [];
    var whitearray = [];
    $('.show_'+BLACK_URL).each(function() {
        blackarray.push(resolveXMLEntityReference($(this).children().eq(0).text()));
    });
    $('.show_'+WHITE_URL).each(function() {
        whitearray.push(resolveXMLEntityReference($(this).children().eq(0).text()));
    });
    submitObject = {
        UrlFilterPolicy : $("#urlfilter_mode_select").val(),
        UrlFilter : {
            urlname : ($("#urlfilter_mode_select").val() == FILTER_MODE_DENY ? blackarray : whitearray)
        }
    };
    var newXmlString = object2xml('request', submitObject);
    saveAjaxData('api/settings/urlfilter', newXmlString, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)) {
            parent.showInfoDialog(common_success);
            initPage();
        }else{
            parent.showInfoDialog(common_failed);
            button_enable("apply", 1);
        }
    });
}

function initPage(){
    $('.user_add_line').remove();
    var isEnable = "";
    getAjaxData('api/security/firewall-switch', function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            isEnable = ret.response;
        }
    },{
        sync : true
    });
    getAjaxData('api/settings/urlfilter', function($xml) {
        var ret = xml2object($xml);
        if(ret.type == "response") {
            if(typeof(ret.response.UrlFilterPolicy)!='undefined'&& typeof(ret.response.WhiteUrlFilters)!='undefined'&& typeof(ret.response.BlackUrlFilters)!='undefined') {
                var urlFilterPolicy = ret.response.UrlFilterPolicy;
                g_urlWhite = ret.response.WhiteUrlFilters.UrlFilter;
                g_urlBlack = ret.response.BlackUrlFilters.UrlFilter;
                
                $("#urlfilter_mode_select").val(urlFilterPolicy);

                
                showBlackOrWhite(urlFilterPolicy);
                addEnableOrDisenable(urlFilterPolicy);
                
                if (isEnable.FirewallMainSwitch == "0") {
                    g_enabled = false;
                    parent.showInfoDialog(IDS_security_message_url_filter_disabled);
                    setDisabled();
                    return;
                }
                if (isEnable.FirewallMainSwitch == "1" && isEnable.FirewallUrlFilterSwitch == "0") {
                    g_enabled = false;
                    parent.showInfoDialog(IDS_security_message_url_filter_disabled);
                    setDisabled();
                    return;
                }
            } else {
                return;
            }
        }
    });
    button_enable("apply", 0);

}

function setDisabled(){
    $("#urlfilter_mode_select").attr("disabled",true);
    $(".button_edit_list").css("color","#666666").css("cursor","default");
    $(".button_delete_list").css("color","#666666").css("cursor","default");
    button_enable("add_item", 0);
}


function onFilterModeChange() {
    showBlackOrWhite($("#urlfilter_mode_select").val());
    addEnableOrDisenable($("#urlfilter_mode_select").val());
    button_enable("apply", 1);
}

/*------------------add enble or not----------------*/
function addEnableOrDisenable(BlackValue) {
    if($(".show_"+(BlackValue == FILTER_MODE_DENY ? BLACK_URL : WHITE_URL)).size() >= URL_FILTER_NUM ) {
           button_enable('add_item', '0');
    }
    else {
        button_enable('add_item', '1');
    }
}

/*---------------------------------show black or white--------------------*/
function showBlackOrWhite(type) {
    index = 0;
    $('.user_add_line').remove();

    if((parseInt(type,10) == FILTER_MODE_ALLOW)&&(typeof(g_urlWhite) != "undefined")) {
        setURLInfo(g_urlWhite.urlname,WHITE_URL);
    }
    else if((parseInt(type,10) == FILTER_MODE_DENY) && (typeof(g_urlBlack) != "undefined") ){
        setURLInfo(g_urlBlack.urlname,BLACK_URL);
    }
}

/*--------------------------set url info-----------------------*/
function setURLInfo(URLData,key) {
    var lastFilter = "";
    if(URLData){
        if ($.isArray(URLData)) {
            $(URLData).each(function(i) {
                var url = URLData[i];
                addFilter($('#service_list tr'),resolveXMLEntityReference(url),key,index);
                index ++;
            });
            lastFilter = URLData[URLData.length-1];
        }
        else if(typeof(URLData) == "undefined") {
            return;
        }
        else {
            var url = URLData;
            addFilter($('#service_list tr'),resolveXMLEntityReference(url),key,index);
            index ++;
            lastFilter = URLData.urlname;
        }
        $('#input_url').val(lastFilter);
    }
}

function addFilter(insertNode){
    var addLine = null;
    addLine = "<tr class='user_add_line show_"+arguments[2]+"'>";
    addLine += "<td id='url_"+ arguments[3] +"' class='td_ltr'>" + arguments[1] + "</td>";
    addLine += "<td class='user_options'><span id='url_edit"+ arguments[3] +"' class=\"button_edit_list\">" + common_edit + "</span>&nbsp;&nbsp;<span id='url_delete"+ arguments[3] +"' class=\"button_delete_list\">" + common_delete + "</span></td></tr>";
    
    var currentTrTotal = $(insertNode).size();
    $(insertNode).eq(currentTrTotal - 2).after(addLine);
}

/*-----------------------pop ok function--------------*/
function save_popok() {
    $(".user_add_line").eq(g_deleteIndex).remove();
    parent.clearDialog();
    
    button_enable("apply", "1");
    button_enable("add_item", "1");
    $("#urlfilter_mode_select").attr("disabled",false);
    addEnableOrDisenable($("#urlfilter_mode_select").val());
}

/*-----------------------pop cancel function ----------*/
function cancel_pop() {
    parent.clearDialog();
    $("#apply").focus();
}


$(document).ready( function() {
    initPage();
    $("#urlfilter_mode_select").bind('change',onFilterModeChange);
    $("#apply").click(function(){
        if(isButtonEnable('apply')){
            button_enable("apply", 0);
            parent.showConfirmDialog(firewall_hint_submit_list_item, onApply);
        }
    });
    var editIndex = 0;
    var currentAllVal = null;
    $('.button_edit_list').live('click', function() {
        if (($(".add_item_control:hidden").size() > 0) && ($('#edit_item_ok').size() < 1)&&g_enabled) {
            $("#urlfilter_mode_select").attr("disabled",true);
            editIndex = $('.button_edit_list').index(this);
            currentAllVal = $('.user_add_line').eq(editIndex).html();
            var editLanIpFilter = $(this).parent().siblings();
            var editLanIpAddr = editLanIpFilter.eq(0);
            editLanIpAddr.html('<input type="text" value="' + editLanIpAddr.html() + '" id="input_url" maxlength="31"></td>');
            $(this).parent().html('<a id="edit_item_ok" href="javascript:void(0);">' + common_ok +
            '</a>&nbsp;&nbsp;<a id="edit_item_cancel" href="javascript:void(0);">' + common_cancel + '</a>');
            $(".add_item_control").hide();
            $('.user_add_line input').eq(0).focus();
            $('#edit_item_cancel').live('click', function() {
                $('.user_add_line').eq(editIndex).html(currentAllVal);
                $('.qtip').qtip('destroy');
                $("#urlfilter_mode_select").attr("disabled",false);
                button_enable('apply', '1');
                if (!isButtonEnable('add_item'))
                {
                    button_enable('add_item', '1');
                    if(1 == g_ok_flag)
                    {
                        button_enable('apply', '1');
                        g_ok_flag = 0;
                    }
                }
            });
            $('#add_item').live('click', function() {
                if (isButtonEnable('add_item'))
                {
                    $('.user_add_line').eq(editIndex).html(currentAllVal);
                    $('.qtip').qtip('destroy');
                }    
            });
            button_enable('apply', '0');
            button_enable('add_item', '0');            
        }
    });
    
    $('#edit_item_ok').live('click', function() {
        if (isvailidUrl()&& !isSameRule()){
            var url = $.trim($('#input_url').val());
            $(".add_item_control").hide();
            var editURLFilter = $(this).parent().siblings();
            editURLFilter.eq(0).html(resolveXMLEntityReference(url));
            $(this).parent().html('<span class=\"button_edit_list\">' + common_edit +
            '</span>&nbsp;&nbsp;<span class=\"button_delete_list\">' + common_delete + '</span>');
            currentAllVal = $('.user_add_line').eq(editIndex).html();
            button_enable('apply', '1');
            button_enable('add_item', '1');
            $("#urlfilter_mode_select").attr("disabled",false);
            g_ok_flag = 1;
            addEnableOrDisenable($("#urlfilter_mode_select").val());
        }
    });

    $('#add_item_ok').live('click', function() {
        if (isvailidUrl() && !isSameRule()){
            var url = $.trim($('#input_url').val());
            $(".add_item_control").hide();
            if($("#urlfilter_mode_select").val() == FILTER_MODE_ALLOW) {
                addFilter($('#service_list tr'), resolveXMLEntityReference(url),WHITE_URL,index);
                index++;
            }
            else if($("#urlfilter_mode_select").val() == FILTER_MODE_DENY) {
                addFilter($('#service_list tr'), resolveXMLEntityReference(url),BLACK_URL,index);
                index++;
            }
            button_enable('apply', '1');
            $("#urlfilter_mode_select").attr("disabled",false);
            addEnableOrDisenable($("#urlfilter_mode_select").val());
        }
    });

    //hide add item control
    $('#add_item_cancel').live('click', function() {
        $(".add_item_control").hide();
        $("#urlfilter_mode_select").attr("disabled",false);
        button_enable("apply", 1);
    });
    
    //show add item control
    $('#add_item').click(function() {
        if (isButtonEnable('add_item')){
            $(".add_item_control").show();
            $('.add_item_control input').eq(0).focus();
            button_enable('apply', '0');
            $("#urlfilter_mode_select").attr("disabled",true);
        }
    });
    $(".button_delete_list").live("click", function(){
        if($(".add_item_control:hidden").size() > 0 && $("#edit_item_ok").size() < 1&&g_enabled){
            g_deleteIndex = $(".button_delete_list").index(this);
            setTimeout(function() {
                parent.showConfirmDialog(firewall_hint_delete_list_item, save_popok, cancel_pop);
            },1);
            
        }
    });
});