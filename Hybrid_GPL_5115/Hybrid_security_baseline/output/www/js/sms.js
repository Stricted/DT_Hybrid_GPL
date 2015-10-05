// JavaScript Document
var g_smsFeature = null;
var g_sms_importenabled = null;
var SMS_BOXTYPE_LOCAL_INBOX = 1;
var SMS_BOXTYPE_LOCAL_SENT  = 2;
var SMS_BOXTYPE_LOCAL_DRAFT = 3;
var SMS_BOXTYPE_LOCAL_TRASH = 4;
var SMS_BOXTYPE_SIM_INBOX   = 5;
var SMS_BOXTYPE_SIM_SENT    = 6;
var SMS_BOXTYPE_SIM_DRAFT   = 7;
var SMS_BOXTYPE_MIX_INBOX   = 8;
var SMS_BOXTYPE_MIX_SENT    = 9;
var SMS_BOXTYPE_MIX_DRAFT   = 10;
var SMS_ERROR_CODE = "113003";

var SMS_BOXTYPE_INBOX   = SMS_BOXTYPE_LOCAL_INBOX;
var SMS_BOXTYPE_SENT    = SMS_BOXTYPE_INBOX + 1;
var SMS_BOXTYPE_DRAFT   = SMS_BOXTYPE_INBOX + 2;

var SMS_LOCAL   = 0;
var SMS_SIM    = 1;
var SMS_MIX   = 2;

var g_sms_boxType = SMS_BOXTYPE_INBOX;
var g_sms_pageIndex = 1;
var g_sms_readCount = "";
var g_sms_maxphonesize = "";
var g_sms_sortType = 0;
var g_sms_ascending = 0;
var g_sms_unreadPreferred = 0;
var g_sms_recordMsgSum = 0;
var g_sms_simSum = 0;
var g_sms_localSum = 0;
var g_sms_simmax = 0;
var g_sms_localmax = 0;
var g_sms_simused = 0;
var g_sms_localused = 0;

var g_sms_totalSmsPage = 0;
var g_sms_curMsgSum = 0;
var g_sms_checkedList = "";
var g_sms_smsListArray = {
    PageIndex : g_sms_pageIndex,
    ReadCount:g_sms_readCount,
    BoxType:g_sms_boxType,
    SortType :g_sms_sortType,
    Ascending:g_sms_ascending,
    UnreadPreferred:g_sms_unreadPreferred
};
var g_sms_smsCount= {
    LocalUnread:0,
    LocalInbox:0,
    LocalOutbox:0,
    LocalDraft:0,
    LocalDeleted:0,
    SimUnread:0,
    SimInbox:0,
    SimOutbox:0,
    SimDraft:0,
    LocalMax:0,
    SimMax:0,
    LocalUsed:0,
    SimUsed:0
};

var g_sms_smsList = [];
var g_sms_inbox_store_status = "0/0";
var g_sms_outbox_store_status = "0";
var g_sms_draftbox_store_status = "0";

var g_need_refresh = true;

Date.prototype.Format = function(format) {
    var o = {
        "M+" : this.getMonth()+1,
        "d+" : this.getDate(),
        "h+" : this.getHours(),
        "m+" : this.getMinutes(),
        "s+" : this.getSeconds()
    };
    var k;
    if(/(y+)/.test(format)) {
        format = format.replace(RegExp.$1, this.getFullYear().toString().substr(4 - RegExp.$1.length));
    }

    for(k in o) {
        if(new RegExp("("+ k +")").test(format)) {
            format = format.replace(RegExp.$1, RegExp.$1.length==1 ? o[k] : ("00"+ o[k]).substr((""+ o[k]).length));
        }
    }
    return format;
};

function call_dialog_sms(dialogTitle, content, button1_text, button1_id,okFunc) {
    $('#div_wrapper').remove();
    $('.dialog').remove();

    var dialogHtml = '';
    if ($('#div_wrapper').size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; width:100%; height:100%; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog' id='sms_dialog'>";
    dialogHtml += "    <div class='sms_dialog_top'></div>";
    dialogHtml += "    <div class='sms_dialog_content'>";
    dialogHtml += "        <div class='sms_dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + dialogTitle + '</span>';
    dialogHtml += "            <span class='dialog_header_right'><a href='javascript:void(0);' title='close' class='dialog_close_btn'><img src='../res/dialog_close_btn.gif' alt='' /></a></span>";
    dialogHtml += '        </div>';
    dialogHtml += "        <div class='sms_dialog_table'><input type='text' id='hidden_focus' style='display:none' value='focus'>" + content + '</div>';
    dialogHtml += "        <div class='sms_dialog_table_bottom'>";
    dialogHtml += "            <div class='dialog_table_r'>";

    dialogHtml += "              <span class='button_wrapper " + button1_id + "' id='" + button1_id + "'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + button1_text + '</a></span>';
    dialogHtml += '                      </span></span></span>';

    dialogHtml += '            </div>';
    dialogHtml += '        </div>';
    dialogHtml += '    </div>';
    dialogHtml += "    <div class='sms_dialog_bottom'></div>";
    dialogHtml += '</div>';

    $('.body_bg').before(dialogHtml);

    reputPosition($('.dialog'), $('#div_wrapper'));
    

    setTimeout(function(){
        $('#hidden_focus').show();
        $('#hidden_focus').focus();
        $('#hidden_focus').hide();
    },0);
    $('#'+button1_id).bind('click', function() {
        var b = true;
        if (typeof (okFunc) == 'function') {
            b = okFunc();
        }
        if(b){
            okFunc = null;
            g_main_displayingPromptStack.pop();
            setFocusToChildWindow();
        }
        return false;
    });
    

    g_main_displayingPromptStack.push(button1_id);

    $('a').attr('tabindex', '-1');
    $('select').attr('tabindex', '-1');
}

//to show the sms store state and show the sms list
function sms_initPage() {
    if(!isButtonEnable("ref_msg_btn")) {
        return;
    }
    sms_getBoxCount();
    sms_initBoxCount();
    if(g_sms_curMsgSum > 0) {
        button_enable("ref_msg_btn","0");
        sms_initSMS();
    }else{
        g_sms_curMsgSum = 0;
        g_sms_recordMsgSum = 0;
        if(g_sms_smsListArray.BoxType == SMS_BOXTYPE_SIM_SENT || g_sms_smsListArray.BoxType == SMS_BOXTYPE_SIM_DRAFT)
        {
            $("#sms_simorlocal_date").hide();
        }else{
            $("#sms_simorlocal_date").show();
        }
    }
    g_need_refresh = true;
}

function sms_initSMS()
{
    sms_getBoxData();
    g_sms_recordMsgSum = g_sms_curMsgSum;
}

function sms_isValidatePageIndex(pageIndex) {
    var pattern = new RegExp("^\\+?[1-9][0-9]*$");
    var result = pattern.test(pageIndex);
    if(result) {
        if(pageIndex>g_sms_totalSmsPage) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
// bind emement event
function sms_btnAddEvent() {
    $("#sms_table :checkbox").live("click", function() {
        var $allCheckBox = $("#sms_table :checkbox");
        var checkedCount ,allChecked;
        if(this == $allCheckBox[0]) {
            $allCheckBox.attr("checked",this.checked);
        } else {
            checkedCount = $allCheckBox.filter(":not(:first)").filter(":checked").length;
            allChecked = checkedCount+1 == $allCheckBox.length;
            $allCheckBox[0].checked = allChecked;
        }
        checkedCount = $allCheckBox.filter(":not(:first)").filter(":checked").length;
        button_enable("del_msg_btn",checkedCount>0);
    });
    //
    $("#del_msg_btn").bind("click", function() {
        if(!isButtonEnable("del_msg_btn")) {
            return;
        }
        showConfirmDialog(common_confirm_delete_list_item, function() {
            sms_smsDelete();
        }, function() {
        });
    });
    /*$("#ref_msg_btn").bind("click",function()
     {
     sms_initPage();
     $("#jump_page_index").val("");
     });*/
    $("#jump_page").bind("click", function() {
        var pageIndex = $("#jump_page_index").val();

        if(sms_isValidatePageIndex(pageIndex)) {
            sms_pageNav(pageIndex);
        } else {
            showQtip("jump_page_index", sms_hint_wrong_page_num);
            $("#jump_page_index").select();
        }
    });

    //
    if(false == g_sms_importenabled) {
        $("#import_btn").remove();
    } else {
        $("#import_btn").bind("click", function() {
            sms_importMessage();
        });
    }
}

//get the current sms type
function sms_getBoxType(name) {
    $("#frame_show").hide();
    var msgType = {
        "smsinbox":SMS_BOXTYPE_INBOX,
        "smssent":SMS_BOXTYPE_SENT,
        "smsdrafts":SMS_BOXTYPE_DRAFT
    };
    g_sms_boxType = msgType[name];
    g_sms_smsListArray.BoxType = g_sms_boxType;
    g_sms_pageIndex = 1;
    sms_initPage();
}

//
function sms_getBoxCount() {
    getAjaxData("api/sms/sms-count", function($xml) {
        //parent.closeWaitingDialog();
        var ret = xml2object($xml);
        if (ret.type == "response") {
            g_sms_smsCount = ret.response;
        } 
        /*else if(ret.type== "error"){
            parent.clearInfoDialog();
            var errcode = ret.error.code;
            var message = "";
            switch(errcode)
            {
                case SMS_ERROR_CODE:
                    message = sms_error_label1;
                    break;
                default:
                    message = sms_error_label2;
                    break;
            }
            parent.showWaitingDialog(common_waiting, message);
        }*/
        else{
            log.error("SMS: get api/sms/sms-count data error");
        }
        
        switch(g_sms_boxType) {
            case SMS_BOXTYPE_INBOX:
                $("#cur_box_type").text(sms_label_inbox);
                g_sms_curMsgSum = (g_sms_boxType == SMS_BOXTYPE_LOCAL_INBOX)? g_sms_smsCount.LocalInbox : g_sms_smsCount.SimInbox;
                break;
            case SMS_BOXTYPE_SENT:
                $("#cur_box_type").text(sms_label_sent);
                g_sms_curMsgSum = (g_sms_boxType == SMS_BOXTYPE_LOCAL_SENT)? g_sms_smsCount.LocalOutbox : g_sms_smsCount.SimOutbox;
                break;
            case SMS_BOXTYPE_DRAFT:
                $("#cur_box_type").text(sms_label_drafts);
                g_sms_curMsgSum = (g_sms_boxType == SMS_BOXTYPE_LOCAL_DRAFT)? g_sms_smsCount.LocalDraft : g_sms_smsCount.SimDraft;
                break;
            default:
                g_sms_curMsgSum = g_sms_smsCount.LocalInbox;
                break;
        }
        g_sms_localSum = parseInt(g_sms_smsCount.LocalInbox, 10)
        +parseInt(g_sms_smsCount.LocalOutbox,10)
        +parseInt(g_sms_smsCount.LocalDraft,10)
        +parseInt(g_sms_smsCount.LocalDeleted,10);
        g_sms_simSum = parseInt(g_sms_smsCount.SimInbox, 10)
        +parseInt(g_sms_smsCount.SimOutbox,10)
        +parseInt(g_sms_smsCount.SimDraft,10);
        g_sms_simmax = parseInt_ex(g_sms_smsCount.SimMax,10);
        g_sms_localmax = parseInt_ex(g_sms_smsCount.LocalMax,10); 
        g_sms_simused = parseInt_ex(g_sms_smsCount.SimUsed,10);
        g_sms_localused = parseInt_ex(g_sms_smsCount.LocalUsed,10);
        var storage_status_string = '';
        if(g_sms_boxType == SMS_BOXTYPE_LOCAL_INBOX || g_sms_boxType == SMS_BOXTYPE_LOCAL_SENT || g_sms_boxType == SMS_BOXTYPE_LOCAL_DRAFT)
        {
            storage_status_string = g_sms_localused.toString()+'/'+g_sms_localmax.toString();
        }
        else
        {
            storage_status_string = g_sms_simused.toString()+'/'+g_sms_simmax.toString();
        }
        $("#sms_storage_status").text(sms_label_sms_storage_status+common_colon+storage_status_string);
        if(g_sms_localSum >= g_sms_smsCount.LocalMax && g_sms_smsCount.LocalMax > 0) {
            $("#fullMessage_tip_div").show();
        } else {
            $("#fullMessage_tip_div").hide();
        }
    }, {
        sync : true,
        errorCB: function() {
            //showInfoDialog(common_error);
            log.error("SMS: get api/sms/sms-count file failed");
        }
    });
}
function parseInt_ex(value,n)
{
    if(isNaN(parseInt(value, n)))
    {
        return 0;
    }
    else
    {
        return parseInt(value, n);    
    }
}
/*calculate inbox sms count and if there is sms in inbox or sentbox or draftbox ,then show the list
 * else give 'no message' tip
 */
function sms_initBoxCount() {
    var unread = 0,
    inbox = 0,
    outbox = 0,
    draftbox = 0;
    switch(SMS_BOXTYPE_INBOX)
        {
            case SMS_BOXTYPE_SIM_INBOX:
                unread = g_sms_smsCount.SimUnread;
                inbox = g_sms_smsCount.SimInbox;
                outbox = g_sms_smsCount.SimOutbox;
                draftbox = g_sms_smsCount.SimDraft;
                break;
            case SMS_BOXTYPE_MIX_INBOX:
                unread = parseInt(g_sms_smsCount.SimUnread,10) + parseInt(g_sms_smsCount.LocalUnread,10);
                inbox = parseInt(g_sms_smsCount.SimInbox,10) + parseInt(g_sms_smsCount.LocalInbox,10);
                outbox = parseInt(g_sms_smsCount.SimOutbox,10) + parseInt(g_sms_smsCount.LocalOutbox,10);
                draftbox = parseInt(g_sms_smsCount.SimDraft,10) + parseInt(g_sms_smsCount.LocalDraft,10);
                break;
            default:
                unread = g_sms_smsCount.LocalUnread;
                inbox = g_sms_smsCount.LocalInbox;
                outbox = g_sms_smsCount.LocalOutbox;
                draftbox = g_sms_smsCount.LocalDraft;
                break;
                
        }

    g_sms_inbox_store_status = unread+"/"+inbox;
    g_sms_outbox_store_status = outbox;
    g_sms_draftbox_store_status = draftbox;
    $("#label_inbox_status").text(g_sms_inbox_store_status);
    $("#label_sent_status").text(g_sms_outbox_store_status);
    $("#label_drafts_status").text(g_sms_draftbox_store_status);
    //
    if(g_sms_curMsgSum >0) {
        g_sms_totalSmsPage = Math.ceil(g_sms_curMsgSum/g_sms_smsListArray.ReadCount);
        g_sms_pageIndex = Math.min(g_sms_pageIndex,g_sms_totalSmsPage);
        var curSmsPage = g_sms_pageIndex+"/"+g_sms_totalSmsPage;
        $("#curSmsPage").text(curSmsPage);
        sms_createPageNav();
        $(".sms_pagination div").show();
    } else {
        $("#sms_check_all").removeAttr("checked");
        $(".sms_list_tr").remove();
        button_enable("del_msg_btn","0");
        //button_enable("ref_msg_btn","0");
        $(".sms_pagination div").hide();
        $("#page_num").html("");
        $("#curSmsPage").html("");
    }

}

//get current box(inbox/sendbox/draftbox) sms
function sms_getBoxData() {
    g_sms_smsListArray.PageIndex = g_sms_pageIndex;
    var msgCondition = object2xml("request",g_sms_smsListArray);
    saveAjaxData('api/sms/sms-list',msgCondition, function($xml) {
        var ret = xml2object($xml);
        if (ret.type == "response") {
            if(ret.response.Messages.Message) {
                g_sms_smsList = [];
                if($.isArray(ret.response.Messages.Message)) {
                    g_sms_smsList = ret.response.Messages.Message;
                } else {
                    g_sms_smsList.push(ret.response.Messages.Message);
                }
                sms_initBoxList();
            } else {
                // showInfoDialog(common_failed);
                log.error("SMS: get api/sms/sms-list data error");
            }
        } else {
            // showInfoDialog(common_failed);
            log.error("SMS: get api/sms/sms-list data error");
        }
    }, {
        errorCB: function() {
            // showInfoDialog(common_failed);
            log.error("SMS: get api/sms/sms-list file failed");
        }
    });
}

//crate dynamic table to show the sms list
function sms_initBoxList() {
    var boxListStr = "";
    var smsReadState = null;
    var tempContent = "";
    var now = new Date().Format("yyyy-MM-dd hh:mm:ss");
    //
    var $preTr= $("#boxList_title"),
    $row = null,
    $cell_1 = null,$cell_2 = null,$cell_3 = null,$cell_4 = null,
    $pre = null;
    //
    $(".sms_list_tr").remove();
    $.each(g_sms_smsList, function(n,message) {
        switch(parseInt(message.SmsType,10)) {
            case 7:
                message.Content = sms_label_setting_usesreport  + common_colon + " " + common_success;
                break;
            case 8:
                message.Content = sms_label_setting_usesreport  + common_colon + " " + common_failed;
                break;
            default:
                break;
        }
        var tempContent1 = resolveXMLEntityReference(message.Content);
        tempContent = regURL(tempContent1);
        smsReadState = "read_state"+message.Smstat;
        var msgDate = "";
        if(g_sms_smsListArray.BoxType != SMS_BOXTYPE_SIM_SENT && g_sms_smsListArray.BoxType != SMS_BOXTYPE_SIM_DRAFT)
        {
            msgDate = message.Date;
            msgDate = new Date(msgDate.replace(/-/g,"/")).Format("yyyy-MM-dd hh:mm:ss");
        }
        
        /*if(msgDate.substring(0,10)==now.substring(0,10))
         {
         msgDate = msgDate.substring(11);
         }
         else
         {
         msgDate = msgDate.substring(0,10);
         }*/

        $row  = $("<tr class='sms_list_tr'></tr>");
        //
        $cell_1 = $("<td width='50' class='sms_td'></td>");
        $cell_1.append("<input id= 'smscheck_" + n + "' type='checkbox' name='checkbox' value='"+message.Index+"' />");
        $row.append($cell_1);
        //
        $cell_2 = $("<td width='140' class='td_pl'></td>");
        $cell_2.html("<table border='0' cellpadding='0' class='sms_msg' cellspacing='0'>"
        +"<tr>"
        +"<td><span id= 'sms_readstate" + n + "' class='msg_icon "+smsReadState+" '>&nbsp;</span></td>"
        +"<td id= 'number_" + n + "' class='sms_phone_number'>"+message.Phone+"</td>"
        +"</tr>"
        +"</table>");
        $row.append($cell_2);
        //
        $cell_3 = $("<td width='308' class='td_pl'></td>");
        $pre = $("<pre id= 'content_" + n + "' class='sms_content'></pre>");
        if(tempContent.length != message.Content.length){
            $pre.html( tempContent );
        }else{
            $pre.text( tempContent );
        }
        $cell_3.html($pre);
        $row.append($cell_3);
        //
        if(g_sms_smsListArray.BoxType != SMS_BOXTYPE_SIM_SENT && g_sms_smsListArray.BoxType != SMS_BOXTYPE_SIM_DRAFT)
        {
             $("#sms_simorlocal_date").show();
             $cell_4 = $("<td width='150' class='td_pl'></td>");
             $cell_4.append("<label id= 'localdate_" + n + "'>"+msgDate+"</label>");
             $row.append($cell_4);
        }else{
             $("#sms_simorlocal_date").hide();
        }
       
        //
        $preTr.after($row);
        $preTr = $row;

    });
    $("#sms_check_all").removeAttr("checked");
    button_enable("del_msg_btn",'0');
    button_enable("ref_msg_btn","1");
}

//create the page navigate menu
function sms_createPageNav() {
    var page_number = "";
    var aContent = 0;
    //to begin or end href
    var pageBeginHref = "",
    pageLastHref = "";
    pageBeginHref = (g_sms_pageIndex == 1)?"javascript:void(0);" : "javascript:sms_pageNav('first')";
    pageLastHref = (g_sms_pageIndex == g_sms_totalSmsPage)?"javascript:void(0);" : "javascript:sms_pageNav('last')";
    $("#pageBegin").attr("href",pageBeginHref);
    $("#pageLast").attr("href",pageLastHref);
    //to previous or next page
    var prePageHref = "",
    nextPageHref = "";
    prePageHref = (g_sms_pageIndex==1)? "javascript:void(0);" : "javascript:sms_pageNav('prePage')";
    nextPageHref = (g_sms_pageIndex == g_sms_totalSmsPage)? "javascript:void(0);" : "javascript:sms_pageNav('nextPage')";
    $("#prePage").attr("href",prePageHref);
    $("#nextPage").attr("href",nextPageHref);
    //
    var beginPage , endPage ,pageSize = 8;
    g_sms_pageIndex = parseInt(g_sms_pageIndex,10);
    if(g_sms_totalSmsPage > pageSize) {
        if(g_sms_pageIndex +pageSize/2 >=g_sms_totalSmsPage) {
            endPage = g_sms_totalSmsPage;
            beginPage = endPage - pageSize;
        } else if(g_sms_pageIndex <=pageSize/2) {
            beginPage = 1;
            endPage = beginPage +pageSize;
        } else {
            beginPage = g_sms_pageIndex>4?g_sms_pageIndex-4:1;
            endPage = g_sms_pageIndex +4> g_sms_totalSmsPage? g_sms_totalSmsPage:g_sms_pageIndex +4;
        }
    } else {
        beginPage = 1;
        endPage = g_sms_totalSmsPage;
    }

    for(i=beginPage;i<=endPage;i++) {
        aHref = i==g_sms_pageIndex? " href=\"javascript:void(0);\"" : " href=\"javascript:sms_pageNav('"+i+"')\" style=\"text-decoration:underline\"";
        page_number += "<a "+aHref+">"+i+"</a>";
    }
    $("#page_num").html(page_number);
}

//to change current page index then refresh the sms list
function sms_pageNav(to) {
    switch(to) {
        case "first":
            g_sms_pageIndex = 1;
            break;
        case "last":
            g_sms_pageIndex = g_sms_totalSmsPage;
            break;
        case "prePage":
            g_sms_pageIndex --;
            break;
        case "nextPage":
            g_sms_pageIndex ++;
            break;
        default:
            g_sms_pageIndex = to;
            break;
    }
    //window.location.href="#";
    $(document).scrollTop(0);
    sms_initPage();
    $("#jump_page_index").val("");
}

//function delete user checked sms
function sms_smsDelete() {
    parent.cancelLogoutTimer();
    g_sms_checkedList = "";// be checked
    var beChecked = $("#sms_table :checkbox:gt(0):checked");
    beChecked.each( function() {
        g_sms_checkedList+="<Index>"+this.value+"</Index>";
    });
    var xmlstr = object2xml("request", g_sms_checkedList);
    saveAjaxData("api/sms/delete-sms",xmlstr, function($xml) {
        var ret = xml2object($xml);
        parent.startLogoutTimer();
        if(isAjaxReturnOK(ret)) {
            sms_initPage();
        }
    });
}

//
function sms_importMessage() {
    if(g_sms_simSum == 0 ) {
        showInfoDialog(strid_sms_hint_import_none);
        return;
    }
    if(g_sms_localSum >= g_sms_smsCount.LocalMax) {
        showInfoDialog(strid_sms_hint_no_room);
        return;
    }
    $("#pop_confirm").die("click");
    $("#pop_Cancel").die("click");
    $(".dialog_close_btn").die("click");
    $(".wait_dialog_btn").die("click");
    $("#pop_OK").die("click");
    showConfirmDialog(strid_sms_hint_import_conform, function() {

        $(".dialog").remove();
        showWaitingDialog(common_waiting, STRID_sms_message_importing_messages.replace("%d", g_sms_simSum));
        var backupSim = {
            IsMove:0,
            Date : new Date().Format("yyyy-MM-dd hh:mm:ss")
        };

        var beforeLocalSum = g_sms_localSum;
        var xmlstr = object2xml("request",backupSim);
        saveAjaxData("api/sms/backup-sim",xmlstr, function($xml) {
            var ret = xml2object($xml);
            closeWaitingDialog();
            if( "" != ret.response.FailNumber && "undefined" != ret.response.FailNumber &&
                "" != ret.response.sucNumber && "undefined" != ret.response.sucNumber) {
                    
                sms_getBoxCount();
                
                var failNumber =  parseInt(ret.response.FailNumber, 10);
                var sucNumber = parseInt(ret.response.SucNumber, 10);
                showInfoDialog(sucNumber+" "+common_succeed +common_comma+ failNumber+" "+common_failed);
            } else {
                showInfoDialog(common_failed);
            }
        }, {
            //import 40 sms,about 40s,so the max timeout is 60000
            timeout : 60000,
            errorCB: function() {
                closeWaitingDialog();
                showInfoDialog(common_failed);
            }
        });
    }, function() {
    });
}

function sms_showWaitingDialog(tipTitle, tipContent, callback_func) {
    $("#div_wrapper").remove();
    var tab = "<table colspacing='0' cellspacing='0' id='wait_table' class='wait_table'>" +
    "<tr><td class='wait_table_top'></td></tr>" +
    "<tr><td class='wait_table_header'><label class='wait_title'>" +
    tipTitle +
    // "</label><span class='wait_dialog_btn' id='wait_dialog_btn' ><img src='../res/dialog_close_btn.gif' alt='' /></span></td></tr>" +
    "<tr><td class='wait_table_content'><div class='wait_wrapper'><div class='wait_image'><img src='../res/waiting.gif' alt=''/></div><div class='wait_str'>" +
    tipContent +
    "</div></div></td>" +
    "</tr>" +
    "<tr>" +
    // "<td class='wait_table_content_sms'>"+
    // "<span class='button_wrapper pop_Cancel' id='pop_Cancel'>"+
    // "                  <span class='button_left'>"+
    // "                      <span class='button_right'>"+
    // "                            <span class='button_center'>" + common_cancel + "</span>"+
    // "                      </span></span></span>"+
    // "</td>"+
    "</tr>"+
    "<tr><td class='wait_table_bottom'></td></tr>" +
    "</table>";
    var wait_div = "<div id='div_wrapper'></div>" + tab;
    $("body").append(wait_div);
    reputPosition($("#wait_table"),$("#div_wrapper"));

    $("#wait_dialog_btn").bind("click", function() {
        closeWaitingDialog();
    });
    $("#pop_Cancel").live("click", function() {
        if (typeof(callback_func) == "function") {
            callback_func();
        }
    });
}

//-------------------------------------------------------------------------------------------------------------
function sms_showSmsDialog() {
    $("#div_wrapper").remove();
    $("#sms_dialog").remove();

    g_content = null;
    var dialogHtml = "";
    if ($("#div_wrapper").size() < 1) {
        dialogHtml += "<div id='div_wrapper'><iframe   id='ifream_dialog'  src= '' frameborder= '0' style= 'background:#bcbcbc; width:100%; height:100%; display: block;'> </iframe></div>";
    }
    dialogHtml += "<div class='dialog' id='sms_dialog'>";
    dialogHtml += "    <div class='sms_dialog_top'></div>";
    dialogHtml += "    <div class='sms_dialog_content'>";
    dialogHtml += "        <div class='sms_dialog_header'>";
    dialogHtml += "            <span class='dialog_header_left'>" + sms_label_message + "</span>";
    dialogHtml += "            <span class='dialog_header_right'><a class='dialog_close_btn' title='' href='javascript:void(0);'><img src='../res/dialog_close_btn.gif' title='' alt='' /></a></span>";
    dialogHtml += "        </div>";
    dialogHtml += "        <div class='sms_dialog_table'>";
    dialogHtml += "               <div class='sms_message'>";
    dialogHtml += "               <p>" + sms_label_recipients + common_colon + "</p>";
    dialogHtml += "               <div id='sms_number_wrapper'>";
    dialogHtml += "                   <input type='text' id='recipients_number' name='recipients_number' value='' />";
    dialogHtml += "               </div>";
    dialogHtml += "               <p>" + sms_label_content + common_colon + "</p>";
    dialogHtml += "               <textarea id='message_content'></textarea>";
    dialogHtml += "               <p id='sms_count'></p>";
    dialogHtml += "               </div>";
    dialogHtml += "        </div>";
    dialogHtml += "        <div class='sms_dialog_table_bottom'>";
    dialogHtml += "            <div class='dialog_table_r'>";

    dialogHtml += "              <span class='button_wrapper pop_send' id='pop_send'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_send + "</a></span>";
    dialogHtml += "                      </span></span></span>";

    dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper pop_save_to_drafts' id='pop_save_to_drafts'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + sms_label_save_to_drafts + "</a></span>";
    dialogHtml += "                      </span></span></span>";

    dialogHtml += "&nbsp;&nbsp;&nbsp;&nbsp;<span class='button_wrapper pop_Cancel pop_common_cancel' id='pop_common_cancel'>";
    dialogHtml += "                  <span class='button_left'>";
    dialogHtml += "                      <span class='button_right'>";
    dialogHtml += "                            <span class='button_center'><a href='javascript:void(0);' title=''>" + common_cancel + "</a></span>";
    dialogHtml += "                      </span></span></span>";

    dialogHtml += "            </div>";
    dialogHtml += "        </div>";
    dialogHtml += "    </div>";
    dialogHtml += "    <div class='sms_dialog_bottom'></div>";
    dialogHtml += "</div>";

    $(".body_bg").before(dialogHtml);
    if( 0 == g_smsFeature.smscharlang || "undefined" == typeof(g_smsFeature.smscharlang) ) {
        $("#sms_count").html("160(1)");
    } else {
        $("#sms_count").html("155(1)");
    }

    reputPosition($("#sms_dialog"),$("#div_wrapper"));
    
    disableTabKey();
    $("#recipients_number").attr("tabindex", "0");
}

function sms_newMessage() {
    g_sms_length = 0;
    parent.cancelLogoutTimer();
    sms_showSmsDialog();
    var buttonHtml = create_button_html(common_send,"pop_send", "pop_send") + "&nbsp;&nbsp;&nbsp;&nbsp;"
    + create_button_html(sms_label_save_to_drafts, "pop_save_to_drafts", "pop_save_to_drafts") + "&nbsp;&nbsp;&nbsp;&nbsp;"
    + create_button_html(common_cancel,"pop_Cancel","pop_Cancel");
    $(".dialog_table_r").html(buttonHtml);
    $("#recipients_number").focus().select();
}

// set sms read
function setSmsRead(currentSmsIndex) {
    $(g_sms_smsList).each( function(i) {
        if(g_sms_smsList[i].Index == currentSmsIndex) {
            if(g_sms_smsList[i].Smstat == 0) {
                var submitXmlObject = {
                    Index:currentSmsIndex
                };
                var submitData = object2xml("request", submitXmlObject);
                saveAjaxData("api/sms/set-read", submitData, function($xml) {
                    var ret = xml2object($xml);
                    if(isAjaxReturnOK(ret)) {
                        sms_initPage();
                    }
                });
            }
        }
    });
}

//-------------------------------------------------------------------------------------------------
var PAGE_HANDLE_FORWARD = '0';
var PAGE_HANDLE_REPLY = '1';
var PAGE_HANDLE_READ = '2';
var PAGE_HANDLE_CREATE = '3';
var PAGE_HANDLE_NULL = '4';
var SMS_TEXT_MODE_UCS2 =  0;
var SMS_TEXT_MODE_7BIT =  1;
var ASCII_CODE    = 127;
var SMS_UCS2_MAX_SIZE = 268;//70+64+67+67;
var SMS_7BIT_MAX_SIZE = 612;//160+146+153+153;
var g_content = null;
var g_text_mode = SMS_TEXT_MODE_7BIT;
var g_sms_length = 0;
var g_sms_num = 1;
var g_ucs2_num = 0;
var g_station;
var GSM_7BIT_NUM = 128;
var SMS_STR_NUM = 620;
var EXTENSION_ASCII = 9;

var g_ext_7bit_tab = [
[20, 0x005E], [40, 0x007B], [41, 0x007D],
[47, 0x005C], [60, 0x005B], [61, 0x007E],
[62, 0x005D], [64, 0x007C], [101, 0x20AC]
];

var g_ext_7bit_tab_turkish = [
[13, 0x001D], [20, 0x005E],
[40, 0x007B], [41, 0x007D], [47, 0x005C], [60, 0x005B],
[61, 0x007E], [62, 0x005D], [64, 0x007C], [71, 0x011E],
[73, 0x0130], [83, 0x015E], [99, 0x00E7], [101, 0x20AC],
[103, 0x011F], [105, 0x0131], [115, 0x015F]
];

var g_ext_7bit_tab_spanish = [
[9, 0x00E7],  [20, 0x005E],
[40, 0x007B], [41, 0x007D], [47, 0x005C], [60, 0x005B],
[61, 0x007E], [62, 0x005D], [64, 0x007C], [65, 0x00C1],
[73, 0x00CD], [79, 0x00D3], [85, 0x00DA], [97, 0x00E1],
[101, 0x20AC], [105, 0x00ED], [111, 0x00F3], [117, 0x00FA]
];

var g_ext_7bit_tab_Portuguese = [
[5, 0x00EA], [9, 0x00E7],   [11, 0x00D4],
[12, 0x00F4], [14, 0x00C1], [15, 0x00E1],[18, 0x03A6],
[19, 0x0393], [20, 0x005E], [21, 0x03A9], [22, 0x03A0],
[23, 0x03A8], [24, 0x03A3], [25, 0x0398],
[31, 0x00CA], [40, 0x007B], [41, 0x007D], [47, 0x005C],
[60, 0x005B], [61, 0x007E], [62, 0x005D], [64, 0x007C],
[65, 0x00C0], [73, 0x00CD], [79, 0x00D3], [85, 0x00DA],
[91, 0x00C3], [92, 0x00D5], [97, 0x00C2], [101, 0x20AC],
[105, 0x00ED], [111, 0x00F3], [117, 0x00FA], [123, 0x00E3],
[124, 0x00F5], [127, 0x00E2]
];
var extension_char = 27;
var ENTER_CHAR = 10;
var CR_CHAR = 13;
var arrayGSM_7bit =
[
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F
];
var arrayGSM_7DefaultTable =
[
0x0040, 0x00A3, 0x0024, 0x00A5, 0x00E8, 0x00E9, 0x00F9, 0x00EC, 0x00F2, 0x00C7, 0x000A, 0x00D8, 0x00F8, 0x000D, 0x00C5, 0x00E5,
0x0394, 0x005F, 0x03A6, 0x0393, 0x039B, 0x03A9, 0x03A0, 0x03A8, 0x03A3, 0x0398, 0x039E, 0x001B, 0x00C6, 0x00E6, 0x00DF, 0x00C9,
0x0020, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
0x00A1, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00C4, 0x00D6, 0x00D1, 0x00DC, 0x00A7,
0x00BF, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00E4, 0x00F6, 0x00F1, 0x00FC, 0x00E0
];

var arrayGSM_7ExtTable =
[
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005E, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0x0020, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0x007B, 0x007D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005C,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005B, 0x007E, 0x005D, 0xFFFF,
0x007C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20AC, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
];

/* Turkish National Language Single Shift Table */
var arrayGSM_7TurkishExtTable  =
[
/* 0       1       2       3      4       5       6       7       8       9       10      11      12      13      14      15 */
/* 0 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x000A, 0xFFFF, 0xFFFF, 0x001D, 0xFFFF, 0xFFFF,
/* 1 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005E, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0020, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 2 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x007B, 0x007D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005C,
/* 3 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005B, 0x007E, 0x005D, 0xFFFF,
/* 4 */0x007C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x011E, 0xFFFF, 0x0130, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 5 */0xFFFF, 0xFFFF, 0xFFFF, 0x015E, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 6 */0xFFFF, 0xFFFF, 0xFFFF, 0x00E7, 0xFFFF, 0x20AC, 0xFFFF, 0x011F, 0xFFFF, 0x0131, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 7 */0xFFFF, 0xFFFF, 0xFFFF, 0x015F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
];

/* Portuguese National Language Single Shift Table */
var arrayGSM_7PortugueseExtTable =
[
/* 0       1       2       3      4       5       6       7       8       9       10      11      12      13      14      15 */
/* 0 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00EA, 0xFFFF, 0xFFFF, 0xFFFF, 0x00E7, 0x000A, 0x00D4, 0x00F4, 0xFFFF, 0x00C1, 0x00E1,
/* 1 */0xFFFF, 0xFFFF, 0x03A6, 0x0393, 0x005E, 0x03A9, 0x03A0, 0x03A8, 0x03A3, 0x0398, 0xFFFF, 0x0020, 0xFFFF, 0xFFFF, 0xFFFF, 0x00CA,
/* 2 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x007B, 0x007D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005C,
/* 3 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005B, 0x007E, 0x005D, 0xFFFF,
/* 4 */0x007C, 0x00C0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00CD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00D3,
/* 5 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00DA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00C3, 0x00D5, 0xFFFF, 0xFFFF, 0xFFFF,
/* 6 */0xFFFF, 0x00C2, 0xFFFF, 0xFFFF, 0xFFFF, 0x20AC, 0xFFFF, 0xFFFF, 0xFFFF, 0x00ED, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00F3,
/* 7 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00E3, 0x00F5, 0xFFFF, 0xFFFF, 0x00E2
];

/* Spanish National Language Single Shift Table */
var arrayGSM_7SpanishExtTable =
[
/* 0       1       2       3      4       5       6       7       8       9       10      11      12      13      14      15 */
/* 0 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00E7, 0x000A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 1 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005E, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0020, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 2 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x007B, 0x007D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005C,
/* 3 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x005B, 0x007E, 0x005D, 0xFFFF,
/* 4 */0x007C, 0x00C1, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00CD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00D3,
/* 5 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00DA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
/* 6 */0xFFFF, 0x00E1, 0xFFFF, 0xFFFF, 0xFFFF, 0x20AC, 0xFFFF, 0xFFFF, 0xFFFF, 0x00ED, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00F3,
/* 7 */0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
];

function sms_isPhoneNumber(str){
    var bRet = true;

    if((str.length > 20)||(str.length<=0)) {
        bRet = false;
        return bRet;
    }
    
    var rgExp = /^[+]{0,1}[*#0123456789]{1,20}$/;
    if (!(str.match(rgExp))) 
    {
        bRet = false;
    }
    
    return bRet;
}
//check phone number validation
function sms_isSendNumber(str) {
    var bRet = true;
    var i = 0;
    var char_i;
    var num_char_i;
    if($.isArray(str)) {
        $(str).each( function(i) {
            bRet = sms_isPhoneNumber(str[i]);
            
        });
    } else {
        bRet = sms_isPhoneNumber(str);
    }
    return bRet;
}

function sms_sendNumberCheck() {
    clearAllErrorLabel();
    var i = 0;
    var checkResult = true;
    var numbers;
    numbers = arguments[0];
    // maximum number
    if(numbers.length > g_sms_maxphonesize) {
        showErrorUnderTextbox("recipients_number", sms_hint_maximum_number.replace("%d",g_sms_maxphonesize));//numbers more than 10;
        $('#recipients_number').focus().select();
        checkResult = false;
    }

    for(i = 0;i<numbers.length;i++) {
        numbers[i] = $.trim(numbers[i]);
        if(false == sms_isSendNumber(numbers[i])) {
            showErrorUnderTextbox("recipients_number", sms_hint_mobile_number_format);
            $('#recipients_number').focus().select();
            checkResult = false;
            return false;
        }
    }
    return checkResult;
}

function check_extension_ascii_for_char_number(str) {
    var i = 0;
    var char_i;
    var char_i_code;
    var k = 0;
    var extension_ascii_num = 0;
    var charLenAtFirstSMSEnd = 1;

    var ext_tab = g_ext_7bit_tab;
    var normal_max_len = 160;
    var long_max_len = 153;

    switch( g_smsFeature.smscharlang ) {
        case 0:
            ext_tab = g_ext_7bit_tab;
            break;
        case 1:
            ext_tab = g_ext_7bit_tab_turkish;
            break;
        case 2:
            ext_tab = g_ext_7bit_tab_spanish;
            break;
        case 3:
            ext_tab = g_ext_7bit_tab_Portuguese;
            break;
        default:
            break;
    }

    if( 0 == g_smsFeature.smscharlang || "undefined" == typeof(g_smsFeature.smscharlang) ) {
        normal_max_len = 160;
        long_max_len = 153;
    } else {
        normal_max_len = 155;
        long_max_len = 149;
    }

    for(i=0; i<str.length; i++) {
        var charLen = 1;
        char_i = str.charAt(i);
        char_i_code = char_i.charCodeAt();
        for(charLen=1,k = 0;k< ext_tab.length;k++) {
            if(char_i_code == ext_tab[k][1]) {
                charLen = 2;
                break;
            }
        }
        if(1 == charLen) {
            extension_ascii_num++;
        } else {
            if(1 == charLenAtFirstSMSEnd) {
                if( (long_max_len-1) == extension_ascii_num ) {
                    extension_ascii_num+=2;
                    charLenAtFirstSMSEnd=2;
                } else if(( (long_max_len*2-1) == extension_ascii_num)
                || ( (long_max_len*3-1) == extension_ascii_num )
                || ( (long_max_len*4-1) == extension_ascii_num)) {
                    extension_ascii_num+=3;
                } else {
                    extension_ascii_num+=2;
                }
            } else {
                if(( (long_max_len-1)*2 == extension_ascii_num)
                || (((long_max_len-1)*3+1) == extension_ascii_num)
                || (((long_max_len-1)*4+2) == extension_ascii_num)) {
                    extension_ascii_num+=3;
                } else {
                    extension_ascii_num+=2;
                }
            }
        }
    }
    if(extension_ascii_num > normal_max_len && 2 == charLenAtFirstSMSEnd) {
        extension_ascii_num++;
    }
    return extension_ascii_num;
}

function ucs2_number_check(str) {
    var i = 0;
    var char_i;
    var num_char_i;

    var j = 0;
    var flag;
    var ucs2_num_temp=0;
    var ext_Table = arrayGSM_7ExtTable;

    if (str.length ==0) {
        return 0;
    }

    switch( g_smsFeature.smscharlang ) {
        case 0:
            ext_Table = arrayGSM_7ExtTable;
            break;
        case 1:
            ext_Table = arrayGSM_7TurkishExtTable;
            break;
        case 2:
            ext_Table = arrayGSM_7SpanishExtTable;
            break;
        case 3:
            ext_Table = arrayGSM_7PortugueseExtTable;
            break;
        default:
            break;
    }

    for(i=0; i<str.length; i++) {
        flag = 0;
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        for(j = 0; j < GSM_7BIT_NUM; j++) {
            if( num_char_i == arrayGSM_7DefaultTable[j]
            ||( num_char_i == ext_Table[j] )
            ) {
                flag = 1;
                break;
            }
        }
        if(0 == flag) {
            ucs2_num_temp++;
        }
    }

    return ucs2_num_temp;
}
/*
function check_enter_number_in_sms_content(str) {
    var i = 0;
    var char_i;
    var num_char_i;
    var enter_number = 0;

    for(i = 0; i < str.length; i++) {
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        if( num_char_i == ENTER_CHAR) {
            if( i == 0 ) {
                enter_number++;
            } else {
                char_i = str.charAt(i - 1);
                num_char_i = char_i.charCodeAt();
                if( num_char_i != CR_CHAR ) {
                    enter_number++;
                }
            }
        }
    }
    return enter_number;
}
*/
function sms_showContentCount(str) {
    g_sms_length = str.length;
    //g_sms_length += check_enter_number_in_sms_content(str);

    document.getElementById("sms_count").innerHTML = "(" + g_sms_length + ")";
}

function sms_numberCheck(str) {
    var sms_left_length;
    var sms_num;
    var temp_length;
    var temp_enter_number;
    var normal_max_len = 160;
    var long_max_len = 153;
    var err_info = null;

    temp_length = str.length;
    //temp_enter_number = check_enter_number_in_sms_content(str);
    //temp_length += temp_enter_number;
    if(SMS_TEXT_MODE_UCS2 == g_text_mode) {
        normal_max_len = 70;
        long_max_len = 67;

        if(temp_length > SMS_UCS2_MAX_SIZE) {
            err_info = sms_hint_max_ucs2_characters_268;
        }
    } else //7bit;
    {
        temp_length = check_extension_ascii_for_char_number(str);

        if( 0 == g_smsFeature.smscharlang || "undefined" == typeof(g_smsFeature.smscharlang) ) {
            normal_max_len = 160;
            long_max_len = 153;
            if(temp_length > SMS_7BIT_MAX_SIZE) {
                err_info = sms_hint_max_ascii_characters_612;
            }
        } else {
            normal_max_len = 155;
            long_max_len = 149;

            if(temp_length > long_max_len*4 ) {
                err_info = sms_hint_max_ascii_characters_596;
            }
        }

    }

    if( null != err_info ) {
        clearAllErrorLabel();
        showErrorUnderTextbox("sms_count", err_info);
        g_sms_length = temp_length;
        button_enable("pop_send", "0");
        button_enable("pop_save_to_drafts", "0");
    } else {
        button_enable("pop_send", "1");
        button_enable("pop_save_to_drafts", "1");
        clearAllErrorLabel();
    }

    if( temp_length <= normal_max_len ) {
        document.getElementById("sms_count").innerHTML = normal_max_len - temp_length + "(" + 1 + ")";
        sms_num = 1;
        if(temp_length <= 0) {
            g_content = str.substring(0);
        }
    } else if( (temp_length > normal_max_len ) && (temp_length <= long_max_len*4) ) {
        sms_num = parseInt(temp_length/long_max_len, 10)+1;

        if( 0 == (temp_length%long_max_len) ) {
            sms_num -= 1;
        }

        document.getElementById("sms_count").innerHTML = long_max_len*sms_num - temp_length + "(" + sms_num + ")";
    } else {
        var tmp =  parseInt((temp_length - long_max_len*4)/long_max_len, 10);
        var tmp2 = Math.floor(tmp);
        var tmp3 = (long_max_len*4 +(tmp2+1)*long_max_len) - temp_length;
        document.getElementById("sms_count").innerHTML =tmp3 + "(" + (tmp2+4+1)+ ")";
    }

    g_sms_num = sms_num;
    g_sms_length = temp_length;
}
/*
function ucs2_number_change(str) {
    var i=0;
    var cmp_len_min =0;
    var diff_begin =0;
    var new_end =0;
    var old_end = 0;
    var ucs2_num_tmp=0;
    var new_ucs2_num =0;
    var old_ucs2_num =0;
    var char_i;
    var char_i_code;
    var char_i_old;
    var char_i_code_old;

    if (g_content == null) {
        g_ucs2_num =  ucs2_number_check(str);
        return;
    }

    if (str==null) {
        g_ucs2_num =0;
        return;
    }

    cmp_len_min = Math.min(str.length, g_content.length);
    if (cmp_len_min == 0) {
        g_ucs2_num =  ucs2_number_check(str);
        return;
    }

    for (i=0;i<cmp_len_min;i++) {
        char_i = str.charAt(i);
        char_i_code = char_i.charCodeAt();

        char_i_old = g_content.charAt(i);
        char_i_code_old = char_i_old.charCodeAt();

        if (char_i_code_old != char_i_code) {
            diff_begin = i;
            break;
        }
    }

    if (i == cmp_len_min) {
        diff_begin = cmp_len_min;
    }
    if (diff_begin == g_content.length) {
        if (g_content.length == str.length ) {
            return;
        } else {
            ucs2_num_tmp = ucs2_number_check(str.substring(diff_begin,str.length));
            g_ucs2_num = g_ucs2_num + ucs2_num_tmp;
            return ;
        }
    }
    if (diff_begin == str.length) {
        ucs2_num_tmp = ucs2_number_check(g_content.substring(diff_begin,g_content.length));
        g_ucs2_num = g_ucs2_num - ucs2_num_tmp;
        return ;
    }

    for (i = 0;i< cmp_len_min;i++) {
        

        char_i = str.charAt(str.length-1 -i);
        char_i_code = char_i.charCodeAt();

        char_i_old = g_content.charAt(g_content.length-1-i);
        char_i_code_old = char_i_old.charCodeAt();

        if (char_i_code_old != char_i_code) {
            new_end = str.length-1-i;
            old_end = g_content.length -1-i;
            break;
        }
    }

    if (new_end < diff_begin) {
        new_ucs2_num = 0;
    } else {
        new_ucs2_num = ucs2_number_check(str.substring(diff_begin,new_end +1));
    }

    if (old_end < diff_begin) {
        old_ucs2_num = 0;
    } else {
        old_ucs2_num = ucs2_number_check(g_content.substring(diff_begin,old_end+1));
    }
    g_ucs2_num = g_ucs2_num + (new_ucs2_num -old_ucs2_num );

    return ;
}
*/
function sms_contentDiffUCS2Num( str ) {
    var idx        = 0;
    var oldEndPos    = 0;
    var newEndPos  = 0;
    var minLen  = 0;
    var diffLen  = 0;
    var diffPos  = 0;
    var diffNum = 0;
    var diffOldNum   = 0;
    var diffNewNum = 0;

    //if input some char at first time
    if ( null == g_content  || 0 == g_content.length  ) {
        g_ucs2_num =  ucs2_number_check(str);
        return;
    }

    //if delete all content, reset g_ucs2_num
    if ( null == str || 0 == str.length ) {
        g_ucs2_num =0;
        return;
    }

    minLen = Math.min( str.length, g_content.length );
    

    //find different char position
    for( diffPos = 0; diffPos < minLen; ++diffPos ) {
        if( str.charAt( diffPos ).charCodeAt() != g_content.charAt( diffPos ).charCodeAt() ) {
            break;
        }
    }


    if( diffPos == minLen ) //add or delete char at tail of sms content
    {
        diffLen = str.length - g_content.length;
        if( diffLen > 0 ) //add
        {
            diffNum = ucs2_number_check( str.substring( diffPos ) );
        } else if( diffLen < 0 )// delete
        {
            diffNum = (-1) * ucs2_number_check( g_content.substring( diffPos ) );
        } else {
        }
    } else // add or delete char at middle or header of sms content
    {
        for( idx = 0, oldEndPos = g_content.length-1,newEndPos = str.length-1; idx < minLen && oldEndPos > diffPos && newEndPos > diffPos; ++idx, --oldEndPos,--newEndPos )
        {
            if( str.charAt( newEndPos ).charCodeAt() != g_content.charAt( oldEndPos ).charCodeAt() )
            {
                break;
            }
        }
        
        diffOldNum = ucs2_number_check( g_content.substring( diffPos, oldEndPos+1 ) );    
        diffNewNum = ucs2_number_check( str.substring( diffPos, newEndPos+1 ) );    
        
        diffNum = diffNewNum - diffOldNum;
    }

    g_ucs2_num += diffNum;
}

function sms_contentChange(str) {
    if( $.browser.msie )
    {
        sms_contentDiffUCS2Num( str );
    }
    else
    {
        g_ucs2_num =  ucs2_number_check(str);
    }
    
    if (g_ucs2_num >0) {
        g_text_mode = SMS_TEXT_MODE_UCS2;
    } else {
        g_text_mode = SMS_TEXT_MODE_7BIT;
    }
    sms_numberCheck(str);
    g_content = str;

}

function sms_contentCheck() {
    var checkResult = true;
    if(SMS_TEXT_MODE_UCS2 == g_text_mode) {
        if(g_sms_length > SMS_UCS2_MAX_SIZE) {
            clearAllErrorLabel();
            showErrorUnderTextbox("sms_count", IDS_sms_hint_content_too_long); /*The max UCS2 characters of SMS message is 268 */
            $("#message_content").select();
            checkResult = false;
        }
    } else {
        g_text_mode =  SMS_TEXT_MODE_7BIT;
        if(g_sms_length > SMS_7BIT_MAX_SIZE) {
            clearAllErrorLabel();
            showErrorUnderTextbox("sms_count", IDS_sms_hint_content_too_long);/*The max ASCII characters of SMS message is 612 */
            $("#message_content").select();
            checkResult = false;
        }
    }
    return checkResult;
}

/*-----------------------------------*/
function changeHtml(nameHtml){

    if(parent.G_MonitoringStatus.response.loginState != "1"){
       gotoPageWithoutHistory(LOGIN_PAGE_URL); 
    }
	var href = 'sms';
	var sub = "";
	var name = nameHtml;
    if('messagesettings' == nameHtml)
    {
	    $("#frame_show").show();
	    $("#right_show").hide();
	    $("#show_html").attr("src",nameHtml+".html");
		name = "messagesettings";
		$.cookies.set('cookie', href+","+sub+","+name, { expiresAt: new Date().getTime()+5*60*1000 });
	    clickMenu('menu_sms');
    }
    else
    {
	    $("#frame_show").hide();
	    $("#right_show").show();
        if('inbox' == nameHtml)
        {
        	sms_getBoxType('smsinbox');
        }
        if('sent' == nameHtml)
        {
        	sms_getBoxType('smssent');
        }
        if('drafts' == nameHtml)
        {
        	sms_getBoxType('smsdrafts');
        }
        $.cookies.set('cookie', href+","+sub+","+name, { expiresAt: new Date().getTime()+5*60*1000 });
    }
    parent.getUserManualUrl(nameHtml);
}
function clickMenu(menu) {
    var getEls = $('.' + menu + ' li ');
    $.each(getEls, function(i) {
        $(this).click(function() {
            $(getEls).addClass('nosub');
             if ($(this).hasClass('nosub')) {
                $(getEls).removeClass('nosubclick');
                $(getEls).removeClass('subClick');
                $(getEls).removeClass('click');
                $(this).addClass('nosubclick');
                return true;
            }
        });
   });
}
/*-----------------------------------*/



function main_executeBeforeDocumentReady() {
    getConfigData("config/sms/config.xml", function($xml) {
        g_smsFeature = _xml2feature($xml);
        g_sms_smsListArray.ReadCount = g_smsFeature.pagesize > 50 ? 50 : (g_smsFeature.pagesize);
        g_sms_maxphonesize = g_smsFeature.maxphone;
        g_sms_importenabled = g_smsFeature.import_enabled == "1"?true:false;
    }, {
        sync : true
    });

    getAjaxData("api/sms/config", function($xml){
        var  ret = xml2object($xml);
        var Save_Mode_Sms = parseInt(ret.response.SaveMode,10);
        switch(Save_Mode_Sms)
            {
                case SMS_SIM:
                    SMS_BOXTYPE_INBOX   = SMS_BOXTYPE_SIM_INBOX;
                    break;
                case SMS_MIX:
                    SMS_BOXTYPE_INBOX   = SMS_BOXTYPE_MIX_INBOX;
                    break;
                default:
                    SMS_BOXTYPE_INBOX   = SMS_BOXTYPE_LOCAL_INBOX;
                    break;
            }
        
        SMS_BOXTYPE_SENT    = SMS_BOXTYPE_INBOX + 1;
        SMS_BOXTYPE_DRAFT   = SMS_BOXTYPE_INBOX + 2;
        g_sms_boxType = SMS_BOXTYPE_INBOX;    
        g_sms_smsListArray.BoxType = g_sms_boxType;
    },
    {
        sync : true
    });

    redirectOnCondition(null, "sms");
    main_beforeready();
}
var g_postfix = window.location.search;
function main_beforeready() {
    if (g_postfix != null) {
        var length = g_postfix.length;
        var start = g_postfix.indexOf('?');
        g_postfix = g_postfix.substring(start + 1, length);
    }else {
        g_postfix = '';
    }
}


main_executeBeforeDocumentReady();
//-------------------------------------------------------------------------------------------------
$(document).ready( function() {
    sms_getBoxType("smsinbox");

    //sms_initPage();
    // to check new messages
    setInterval( function() {
        if(g_need_refresh){
            sms_getBoxCount();
            sms_initBoxCount();
            if((g_sms_curMsgSum > 0) && (g_sms_recordMsgSum != g_sms_curMsgSum)) {
                sms_initSMS();
            }
        }
    }, parent.g_feature.update_interval);
    sms_btnAddEvent();

    var smsIndex = -1;
    var scaIndex = null;

    $("#message").live("click", function() {
        sms_newMessage();
        smsIndex = -1;
    });
    // show message
    function sms_showMessage(viewIndex) {
        
        sms_showSmsDialog();
        //smsIndex = $(".sms_td :input[type=checkbox]").eq(viewIndex).val();
        smsIndex = g_sms_smsList[viewIndex].Index;
        var buttonHtml = "";
        var isReadOnly = false;
        if(g_sms_boxType == SMS_BOXTYPE_INBOX) {
            buttonHtml += create_button_html(common_reply,"pop_reply", "pop_reply") + "&nbsp;&nbsp;&nbsp;&nbsp;";
        }
        if(g_sms_boxType == SMS_BOXTYPE_DRAFT) {
            $("#recipients_number").focus().select();
            buttonHtml += create_button_html(common_send,"pop_send", "pop_send") + "&nbsp;&nbsp;&nbsp;&nbsp;";
            buttonHtml += create_button_html(sms_label_save_to_drafts, "pop_save_to_drafts", "pop_save_to_drafts") + "&nbsp;&nbsp;&nbsp;&nbsp;";
        }
        if(g_sms_boxType != SMS_BOXTYPE_DRAFT) {
            isReadOnly = true;
            $("#recipients_number").attr("readonly","readonly");
            $("#message_content").attr("readonly","readonly");
            buttonHtml +=  create_button_html(common_forward, "pop_forward", "pop_forward") + "&nbsp;&nbsp;&nbsp;&nbsp;";
        }
        buttonHtml +=  create_button_html(common_cancel,"pop_Cancel","pop_Cancel");
        $(".dialog_table_r").html(buttonHtml);
        $(g_sms_smsList).each( function(i) {
            if(g_sms_smsList[i].Index == smsIndex) {
                $("#recipients_number").val(g_sms_smsList[i].Phone);
                $("#message_content").val(g_sms_smsList[i].Content);
                if(isReadOnly) {
                    sms_showContentCount( $("#message_content").val() );
                } else {
                    sms_contentChange($("#message_content").val());
                }

                scaIndex = i;
            }
        });
        setSmsRead(smsIndex);
    }
    
    $(".sms_list_tr").live("click", function( event ) {
            if ( "sms_td" != $(event.target).attr("class")  && "sms_td" != $(event.target).parent().attr("class")  ) 
            {
                sms_showMessage( $(".sms_list_tr").index(this) );
            }
    });
    
    // show reply dialog
    $("#pop_reply").live("click", function() {
        var replyNumber = $("#recipients_number").val();
        var buttonHtml = create_button_html(common_send,"pop_send", "pop_send") + "&nbsp;&nbsp;&nbsp;&nbsp;"
        + create_button_html(sms_label_save_to_drafts, "pop_save_to_drafts", "pop_save_to_drafts") + "&nbsp;&nbsp;&nbsp;&nbsp;"
        + create_button_html(common_cancel,"pop_Cancel","pop_Cancel");
        parent.cancelLogoutTimer();
        sms_showSmsDialog();
        $(".dialog_table_r").html(buttonHtml);
        $("#recipients_number").val(replyNumber);
        sms_contentChange($("#message_content").val());
        $("#message_content").focus().select();
    });
    // show forward dialog
    $("#pop_forward").live("click", function() {
        var forwardContent = $("#message_content").val();
        var buttonHtml = create_button_html(common_send,"pop_send", "pop_send") + "&nbsp;&nbsp;&nbsp;&nbsp;"
        + create_button_html(sms_label_save_to_drafts, "pop_save_to_drafts", "pop_save_to_drafts") + "&nbsp;&nbsp;&nbsp;&nbsp;"
        + create_button_html(common_cancel,"pop_Cancel","pop_Cancel");
        parent.cancelLogoutTimer();
        sms_showSmsDialog();
        $(".dialog_table_r").html(buttonHtml);
        $("#message_content").val(forwardContent);
        sms_contentChange($("#message_content").val());
        $("#recipients_number").focus().select();
    });
    // function for create sms & save sms
    function sms_createSms(btnValue) {
        clearAllErrorLabel();
        var strPhoneNumber = $.trim($("#recipients_number").val());
        var messageContent = $("#message_content").val();

        // if last chacracter o phone number is ';', remove it
        if (strPhoneNumber.length > 0) {
            if (strPhoneNumber.lastIndexOf(";") == (strPhoneNumber.length - 1)) {
                strPhoneNumber = strPhoneNumber.substring(0, strPhoneNumber.length - 1);
                $("#recipients_number").val(strPhoneNumber);
            }
        }
        if($("#message_content").val() == "") {
            g_sms_num = 1;
        }

        var PhoneArray = strPhoneNumber.split(";");
        var strSMSContent = $.trim($("#message_content").val());
        if(((btnValue == "Save" && (0 == strPhoneNumber.length && 0 != strSMSContent.length )) || sms_sendNumberCheck(PhoneArray)) && sms_contentCheck()) {
            var scaValue = "";
            messageContent =  resolveXMLEntityReference(messageContent);

            var index = -1;
            if(SMS_BOXTYPE_DRAFT == g_sms_boxType && smsIndex>-1) {
                index = smsIndex;
                smsIndex = -1;
            }
            var now = new Date().Format("yyyy-MM-dd hh:mm:ss");
            var submitXmlObject = {
                Index:index,
                Phones: {
                    Phone:PhoneArray
                },
                Sca:scaValue,
                Content:messageContent,
                Length:messageContent.length,
                Reserved:g_text_mode,
                Date:now
            };
            smsIndex = -1;
            var submitData = object2xml("request", submitXmlObject);
            if(btnValue == "Save") {
                clearDialog();
                saveAjaxData("api/sms/save-sms", submitData, function($xml) {
                    var ret = xml2object($xml);
                    if(isAjaxReturnOK(ret)) {
                        if(SMS_BOXTYPE_DRAFT == g_sms_boxType) {
                            sms_initPage();
                        }
                    }
                    else if(ret.type == 'error'){
                        if(ret.error.code == 113002){
                              showInfoDialog(sd_hint_not_enough_space_available);
                        }
                        else if(ret.error.code == 113001){
                             showInfoDialog(Label_center_number_error);
                        }
                        else{
                            showInfoDialog(common_failed);
                        }
                    } else {
                        showInfoDialog(common_failed);
                    }
                });
            } else {
                var refreshStatus;
                sms_showWaitingDialog(common_waiting,
                sms_hint_sending);//+"&nbsp;"+"1/" + g_sms_num*(PhoneArray.length));
                g_need_refresh = false;
                $(".wait_dialog_btn").show();
                $("#sms_dialog").remove();

                saveAjaxData("api/sms/send-sms", submitData, function($xml) {
                    var ret = xml2object($xml);
                    if(!isAjaxReturnOK(ret)) 
                    {
                        if(ret.type == 'error')
                        {
                            if(ret.error.code == 113002)
                            {
                                $("#wait_table").remove();
                                showInfoDialog(sd_hint_not_enough_space_available_send_sms);
                                return;
                            }
                            else if(ret.error.code == 113001)
                            {
                                $("#wait_table").remove();
                                showInfoDialog(Label_center_number_error);
                                return;
                            }
                        }
                    }
                    //-------------------------------------------------------------------
                    function getSendSmsStatus() {
                        getAjaxData("api/sms/send-status", function($xml) {
                            var ret = xml2object($xml);
                            ret = ret.response;
                            var sendTotalCount = ret.TotalCount;
                            var currentSendIndex = ret.CurIndex;
                            var currentSendPhone = ret.Phone;
                            var sendSuccessPhones = ret.SucPhone;
                            var sendFailPhones = ret.FailPhone;
                            var statusContent = sms_hint_sending + "&nbsp;" + currentSendIndex + "/" + sendTotalCount;

                            $(".wait_table_content .wait_str").html(statusContent);
                            if(currentSendPhone == "") {
                                $("#wait_table").remove();
                                g_need_refresh = true;
                                clearTimeout(refreshStatus);
                                var successedArray = sendSuccessPhones.split(",");
                                var successedTotal = successedArray.length;
                                var failedArray = sendFailPhones.split(",");
                                var failedTotal = failedArray.length;
                                if(successedArray[successedTotal-1] == "") {
                                    successedArray.pop();
                                    successedTotal>0 ? successedTotal -= 1 : successedTotal = 0;
                                    $(".send_success_info").hide();
                                }
                                if(failedArray[failedTotal-1] == "") {
                                    failedArray.pop();
                                    failedTotal>0 ? failedTotal -= 1 : failedTotal = 0;
                                    $(".send_failed_info").hide();
                                }

                                var successDialogHtml = "<table id='sms_success_info'><tr><td><h1><span>"+
                                successedTotal + "&nbsp;" + common_successfully + ",</span><span>" + failedTotal + "&nbsp;" + common_failed + "</span></h1></td>"+
                                "</tr><tr><td height='27'></td></tr>";
                                if(successedTotal >0) {
                                    successDialogHtml += "<tr class='send_success_info'><td><table><tr><td rowspan='2' width='30' valign='top'><img src='../res/icon_sms_send_success.gif' alt='' /></td>"+
                                    "<td>" + sms_label_the_message_sent_successed + common_colon +
                                    "</td></tr><tr><td class='success_phone_number'>" + sendSuccessPhones.split(",").join("; ") + "</td></tr></table></td></tr><tr class='send_failed_info'><td height='21'></td></tr>";
                                }

                                if(failedTotal >0) {
                                    successDialogHtml += "<tr class='send_failed_info'><td><table><tr><td rowspan='2' width='30' valign='top'><img src='../res/icon_sms_send_failed.gif' alt='' /></td>"+
                                    "<td>" + sms_label_the_message_sent_failed + common_colon +
                                    "</td></tr><tr><td  class='failed_phone_number'>" + sendFailPhones.split(",").join("; ") + "</td></tr></table></td></tr>";
                                }
                                successDialogHtml += "</table>";
                                call_dialog_sms(common_note, successDialogHtml, common_ok, "pop_OK",function(){                        
                                    $("#wait_div, #div_wrapper").remove();
                                    $(".dialog").remove();
                                    parent.startLogoutTimer();
                                });
                                $(".dialog_table").css("text-align","left");
                                if(( SMS_BOXTYPE_SENT  == g_sms_boxType && successedTotal>0)
                                ||( SMS_BOXTYPE_DRAFT == g_sms_boxType )) {
                                    button_enable("ref_msg_btn","1");
                                    sms_initPage(); //if send failed save to draft
                                }
                            }
                            else
                            {
                                 clearTimeout(refreshStatus);
                                 refreshStatus = setTimeout(getSendSmsStatus, parent.g_feature.update_interval);
                            }
                        },{
                            timeout: 60000,  // 1 minutes
                            //sync:true,
                            errorCB: function(XMLHttpRequest, textStatus)
                            {
                                $("#wait_table").remove();
                                g_need_refresh = true;
                                var errorInfo = ('timeout' == textStatus) ? common_timeout : common_failed;
                                showInfoDialog(errorInfo);                                                         
                            }
                        });
                    }

                    //----------------------------------------------------------------------
                    refreshStatus = setTimeout(getSendSmsStatus, parent.g_feature.update_interval);

                    function cancelSendSms() {
                        var cancelSend = 1;
                        var submitData = object2xml("request", cancelSend);

                        saveAjaxData("api/sms/cancel-send", submitData, function($xml) {
                            var ret = xml2object($xml);
                            clearTimeout(refreshStatus);
                            $("#wait_table, #div_wrapper").remove();
                            if(isAjaxReturnOK(ret)) {
                                getSendSmsStatus();
                            }
                        });
                    }

                    $("#pop_Cancel").bind("click", function() {
                        if(!isButtonEnable("pop_Cancel")) {
                            return;
                        }
                        button_enable("pop_Cancel","0");
                        cancelSendSms();
                    });
                    $(".wait_dialog_btn").unbind("click").live("click", function() {
                        parent.startLogoutTimer();
                        cancelSendSms();
                    });
                    $(".dialog_close_btn").live("click", function() {
                        $("#wait_div, #div_wrapper").remove();
                        $(".dialog").remove();
                        parent.startLogoutTimer();
                    });
                },{
                        timeout: 60000,  // 1 minutes
                        //sync:true,
                        errorCB: function(XMLHttpRequest, textStatus)
                        {
                                $("#wait_table").remove();
                                g_need_refresh = true;
                                var errorInfo = ('timeout' == textStatus) ? common_timeout : common_failed;
                                showInfoDialog(errorInfo);                                                         
                            }
                        });
            }
            scaIndex = null;
        } else {
            button_enable("pop_save_to_drafts","1");
        }
    }

    // send sms
    $("#pop_send").live("click", function() {
        if(!isButtonEnable("pop_send")) {
            return;
        }
        sms_createSms("Sent");
    });
    // clear sca value
    $(".pop_Cancel, .dialog_close_btn").live("click", function() {
        scaIndex = null;
        clearAllErrorLabel();
        parent.startLogoutTimer();
    });
    // delete sms
    $("#pop_delete").live("click", function() {
        if(isButtonEnable("pop_delete")) {
            parent.startLogoutTimer();
            button_enable("pop_delete","0");
            clearDialog();
            var submitXmlObject = {
                Index:smsIndex
            };
            var submitData = object2xml("request", submitXmlObject);
            saveAjaxData("api/sms/delete-sms", submitData, function($xml) {
                sms_initPage();
            });
        }
    });
    $("#pop_save_to_drafts").live("click", function() {
        if(!isButtonEnable("pop_save_to_drafts")) {
            return;
        }
        parent.startLogoutTimer();
        button_enable("pop_save_to_drafts","0");
        sms_createSms("Save");
    });
    $("#message_content").live("keydown keypress keyup focus change", function(event) {
        if( $("#message_content").attr("readonly") ) {
            return;
        }
        if(("keydown" == event.type || "keypress" == event.type || "keyup" == event.type)
        && (37 == event.keyCode || 38 == event.keyCode || 39 == event.keyCode || 40 == event.keyCode)) {
            return;
        }
        sms_numberCheck($("#message_content").val());
        sms_contentChange($("#message_content").val());
    });

var g_href = '';
var g_sub = '';
var g_name = '';

function getRefresh(){
    var objValue = [];
    g_href = parent.current_href;
    if($.cookies.get('cookie')!=null){
        objValue = $.cookies.get('cookie').split(",");
        if(g_href != objValue[0]){
            if(g_href == "sms"){
                g_sub = "";
                g_name = "inbox";
            } 
        } else {
            g_sub = objValue[1];
            g_name = objValue[2];
        }
    } else {
        if(g_href == "sms"){
            g_sub = "";
            g_name = "inbox";
        } 
    }
    
}
    if(g_postfix != null && g_postfix != "" && g_postfix == "inbox")
    {
        $.cookies.set('cookie', "sms,,inbox", { expiresAt: new Date().getTime()+5*60*1000 });
    }
    setTimeout(function(){
        getRefresh();
        changeHtml(g_name);
    },1000);    

});

