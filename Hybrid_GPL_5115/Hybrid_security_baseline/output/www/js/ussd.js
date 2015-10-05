var USSD_RETURN_RESULT = 2001;
var USSD_OUT_TIME = 2003;
var USSD_HAVE_USSD_DIALOG = 1;
var USSD_HAVE_NOT_USSD_DIALOG = 0;
var g_ussd_config = "";
var g_ussd_btnStr = "";
var g_ussd_currentFun = null;
var g_ussd_chargeLimitText = "*0123456789#";
var g_ussd_generalLimitText = "*0123456789#";
var g_ussd_generalCommandList = new Array();
var g_ussd_timeout = false;
var g_ussd_timer_maxDialog = null;

redirectOnCondition(null, "ussd");
$(document).ready( function() {
	ussd_initVar();
	ussd_releaseUssdDialog();
	setInterval( function() {
		ussd_getDialogStatus();
	},g_feature.update_interval);
});
//
function ussd_initVar() {
	getConfigData('config/ussd/ussd.xml', function($xml) {
		var ret = xml2object($xml);
		if(ret.type == "config") {
			g_ussd_config = ret.config.USSD;
			ussd_initPage();
		} else {
			log.error("USSD: get data configurations/ussd.xml error");
		}
	}, {
		errorCB: function() {
			log.error("USSD:load configurations/ussd.xml failed ");
		}
	});
}

//create fun list by configuration
function ussd_initPage() {
	$.each(g_ussd_config, function(n,value) {
		switch(n) {
			case "ActivateInternetService":
				ussd_activateInternetService(value);
				$("#fun_activateInternetService").show();
				break;
			case "BalanceInquiry":
				ussd_balanceInquiry(value);
				$("#fun_balanceInquiry").show();
				break;
			case "Charge":
				ussd_charge(value);
				$("#fun_charge").show();
				break;
			case "General":
				ussd_general(value);
				$("#fun_general").show();
				break;
			default:
				log.error("USSD:g_ussd_config error");
				break;
		}
	});
}

// fun ActivateInternetService
function ussd_activateInternetService(value) {
	var config_list = new Array();
	if(value.Items) {
		if(value.Items.Item) {
			if($.isArray(value.Items.Item)) {
				config_list = value.Items.Item;
			} else {
				config_list.push(value.Items.Item);
			}
		}
	}
	if(0 == config_list.length){
		return;
	}

    $("#activate_internet_service_title").text(eval(value.Title));
    $("#activate_internet_service_description").html(regURL(eval(value.Description)));
	var subTr = "";

	/*
	 * create select
	 */
	var selectHtml = "";
	selectHtml = "<select id='activate_internet_service_select'>";

	$.each(config_list, function(n,value) {
		selectHtml +="<option value='"+$.trim(value.Command)+"'>" + eval(value.Subject).replace("%d", "1") +"</option>";
	});
	selectHtml +="</select>";

	/*
	 * create button
	 */
	var btnHTML = "";
	btnHTML = "<span class='button_wrapper' id='ActivateInternetServiceBtn'>";
	btnHTML += "<span class='button_left'>";
	btnHTML += "<span class='button_right'>";
	btnHTML += "<span class='button_center'>" + eval(config_list[0].Action);
	btnHTML +=  "</span></span></span></span>";

	subTr +="<tr><td height='36'><label>"+selectHtml+"<label></td>"
	+"<td class=\"align_right\">"+btnHTML+"</td></tr>";

	$("#activate_internet_service table").append(subTr);

	/*
	 * define button action
	 */
	$("#ActivateInternetServiceBtn").live("click", function() {
		ussd_sendCommand('ActivateInternetService',
		$("#activate_internet_service_select").val(),
		'CodeType');
	});
}

//fun BalanceInquiry
function ussd_balanceInquiry(value) {
	$("#balance_inquiry_title").text(eval(value.Title));
	$("#balance_inquiry_description").text(eval(value.Description));
	ussd_creatBtn("BalanceInquiry",$.trim(value.Command),value.Action,"BalanceInquiry");
	$("#balance_inquiry_action").append(g_ussd_btnStr);
}

//fun Charge
function ussd_charge(value) {
	$("#charge_title").text(eval(value.Title));
	$("#charge_description").text(eval(value.Description));
	g_ussd_chargeLimitText = $.trim(value.LimitText);
	$("#charge_command").val(value.Command);
	ussd_creatBtn("Charge",$.trim(value.Command),value.Action,"Charge");
	$("#charge_action").append(g_ussd_btnStr);
}

//fun General
function ussd_general(value) {
	$("#general_title").text(eval(value.Title));
	$("#general_description").text(eval(value.Description));
	g_ussd_generalLimitText = $.trim(value.LimitText);
	var config_list = new Array();
	if(value.Menu) {
		if(value.Menu.MenuItem) {
			if($.isArray(value.Menu.MenuItem)) {
				config_list = value.Menu.MenuItem;
			} else {
				config_list.push(value.Menu.MenuItem);
			}
		}
	}
	$.each(config_list, function(n,subValue) {
		subValue.Command = $.trim(subValue.Command);//.substring(0,40);
		subValue.Name = $.trim(subValue.Name);
		if(subValue.Command.length > 0 && subValue.Command.length<=40) {
			if(subValue.Name.length == 0) {
				subValue.Name = subValue.Command;
			}
			g_ussd_generalCommandList.push([subValue.Command,subValue.Name]);
		}
	});
	$("#general_command_select").createSelect({
		maxlength:40,
		direction_up:true,
		onlyread:false,
		maxheight:240
	});
	$("#general_command_select").createOptions(g_ussd_generalCommandList);

	//
	ussd_creatBtn("General",value.Command,value.Action,"general_btn");
	$("#general_action").append(g_ussd_btnStr);
}

//
function ussd_creatBtn(funName,content,action,btnId) {
	g_ussd_btnStr ="<span class='button_wrapper' id='"+btnId+"' onclick = \"javascript:ussd_sendCommand('"+funName+"','"+content+"','CodeType');return false;\">"
	+"<span class='button_left'>"
	+"<span class='button_right'>"
	+"<span class='button_center'>" + eval(action) + "</span></span></span></span>";
}

function ussd_checkChargeCommand(Content) {
	var reg = null;
	if(g_ussd_chargeLimitText != "") {
		try {
			reg = new RegExp(g_ussd_chargeLimitText);
			if(reg.exec(Content) != Content) {
				showErrorUnderTextbox("charge_hint",ussd_label_hint_wrong_command);
				$("#charge_command").focus().select();
				$(".button_wrapper").removeClass("disable_btn");
				return false;
			}
		} catch(exception) {
			log.error(exception);
			showErrorUnderTextbox("charge_hint",ussd_label_hint_wrong_command);
			$("#charge_command").focus().select();
			$(".button_wrapper").removeClass("disable_btn");
			return false;
		}

	}
	return true;
}

function ussd_checkGeneralCommand(Content) {
	var reg = null;
	if(g_ussd_generalLimitText != "") {
		try {
			reg = new RegExp(g_ussd_generalLimitText);
			if(reg.exec(Content) != Content) {
				showErrorUnderTextbox("general_hint",ussd_label_hint_wrong_command);
				$("#general_command_select").select();
				$(".button_wrapper").removeClass("disable_btn");
				return false;
			}

		} catch(exception) {
			log.error(exception);
			showErrorUnderTextbox("general_hint",ussd_label_hint_wrong_command);
			$("#general_command_select").select();
			$(".button_wrapper").removeClass("disable_btn");
			return false;
		}
	}
	return true;
}

//
function ussd_sendCommand(funName,Content,CodeType) {
	if(!isButtonEnable("general_btn")) {
		return;
	} else {
		$(".button_wrapper").addClass("disable_btn");
	}
	clearAllErrorLabel();
	switch(funName) {
		case "Charge":
			Content = $.trim($("#charge_command").val());
			Content = Content.replace(/[\[\]]/g,'');
			if(Content == null || Content == "") {
				showErrorUnderTextbox("charge_hint",ussd_label_hint_wrong_command);
				$("#charge_command").focus().select();
				$(".button_wrapper").removeClass("disable_btn");
				return;
			} else {
				if(false == ussd_checkChargeCommand(Content)){
					return;
				}
			}
			break;
		case "General":
			Content = $.trim($("#general_command_select").sVal());
			//
			if(Content == null || Content == "") {
				showErrorUnderTextbox("general_hint",ussd_label_hint_wrong_command);
				$("#general_command_select").select();
				$(".button_wrapper").removeClass("disable_btn");
				return;
			} else {
				if(false == ussd_checkGeneralCommand(Content)){
					return;
				}
			}
			g_ussd_currentFun = funName;
			Content = resolveXMLEntityReference(Content);
			$("#general_command_select").sVal(null);
			$("#general_result_table").append("<tr><td class='general_status'>"+common_sent+common_colon+"</td><td class='general_content'>"+Content+"</td></tr>");
			var lastTrH = $("#general_result_table tr:last").height();
			var tableH = $("#general_result_table").height();
			$("#general_result").scrollTop(tableH-lastTrH).scrollLeft(0);
			break;
		case "BalanceInquiry":
			g_ussd_currentFun = funName;
			break;
		default:
			break;
	}
	//
	var send = {
		content:Content,
		codeType:CodeType
	};
	var ussdSend = object2xml("request",send);
	showWaitingDialog(common_waiting, IDS_ussd_label_wait_response);
	$("#wait_dialog_btn").show().bind("click", function() {
		g_ussd_timeout = true;
		clearTimeout(g_ussd_timer_maxDialog);
		ussd_releaseUssdDialog();
		g_ussd_currentFun=null;
	});
	saveAjaxData('api/ussd/send',ussdSend, function($xml) {
		var ret = xml2object($xml);
		if(ret.type == "response") {
			if(isAjaxReturnOK(ret)) {
				g_ussd_timeout = false;
				g_ussd_timer_maxDialog = setTimeout( function() {
					g_ussd_timeout = true;
					closeWaitingDialog();
					showInfoDialog(common_timeout);
					ussd_releaseUssdDialog();
					g_ussd_currentFun=null;

				},2*60*1000);
				//
				ussd_getResult();
			} else {
				sendCommandFailed();
			}
		} else  //failed
		{
			sendCommandFailed();
		}
	}, {
		errorCB: function(XMLHttpRequest,textStatus) {
			sendCommandFailed();
		}
	});
	//
	function sendCommandFailed() {
		closeWaitingDialog();
		$(".button_wrapper").removeClass("disable_btn");
		showInfoDialog(common_failed);
		g_ussd_currentFun = null;
	}

}

//
function ussd_getResult() {
	if(g_ussd_timeout == true) {
		return;
	}
	//
	getAjaxData("api/ussd/get", function($xml) {
		if(g_ussd_timeout == true) {
			return;
		}
		var ret = xml2object($xml);
		if (ret.type == "response") {
			ret = $.trim(ret.response.content);
			if(ret.length== 0) {
				ret = common_failed;
			}
			switch(g_ussd_currentFun) {
				case "General":
					ret = ret.replace(/[\n|\r]/g,"<br/>");
					var lastTrH = $("#general_result_table tr:last").height();
					$("#general_result_table").append("<tr class='general_result'><td class='general_status'>"+dialup_label_received+common_colon+"</td><td class='general_content'><pre>"+ret+"</pre></td></tr>");
					var tableH = $("#general_result_table").height();
					lastTrH += $("#general_result_table tr:last").height();
					$("#general_result").scrollTop(tableH-lastTrH).scrollLeft(0);
					break;
				case "BalanceInquiry":
					//$("#balance_inquiry_result").text(ret);
					$("#balance_inquiry_result").append("<pre>"+ret+"</pre>");
					break;
				default :
					clearTimeout(g_ussd_timer_maxDialog);
					closeWaitingDialog();
					g_ussd_currentFun = null;
					$(".button_wrapper").removeClass("disable_btn");
					showInfoDialog(ret);
					return;
			}
			//
			clearTimeout(g_ussd_timer_maxDialog);
			closeWaitingDialog();
			g_ussd_currentFun = null;
			$(".button_wrapper").removeClass("disable_btn");
		} else {
			if(ret.error.code == "111019") {//no response from network
				setTimeout(ussd_getResult,g_feature.update_interval);
			} else if(ret.error.code == "111020") {//network timeout error
				clearTimeout(g_ussd_timer_maxDialog);
				closeWaitingDialog();
				ussd_releaseUssdDialog();
				showInfoDialog(common_timeout);
				g_ussd_currentFun = null;
			} else {
				clearTimeout(g_ussd_timer_maxDialog);
				closeWaitingDialog();
				g_ussd_currentFun = null;
				showInfoDialog(common_failed);
				log.error("USSD:get api/ussd/get data error");
			}
		}

	}, {
		errorCB: function() {
			clearTimeout(g_ussd_timer_maxDialog);
			closeWaitingDialog();
			showInfoDialog(common_failed);
			g_ussd_currentFun = null;
			log.error("USSD:get api/ussd/get file failed");
		}
	});
}

// to check if there is ussd dialog
function ussd_getDialogStatus(haveDialogFun,noDialogFun) {
	getAjaxData('api/ussd/status', function($xml) {
		var ret = xml2object($xml);
		if (ret.type == "response" ) {
			if(ret.response.result == USSD_HAVE_USSD_DIALOG ) {
				$(".button_wrapper").addClass("disable_btn");
				if(typeof(haveDialogFun) == "function" ) {
					haveDialogFun();
				}

			} else {
				$(".button_wrapper").removeClass("disable_btn");
				if(typeof(noDialogFun) == "function" ) {
					noDialogFun();
				}
			}
		}
	});
}

//release USSD dialog   (api/ussd/release)
function  ussd_releaseUssdDialog() {
	getAjaxData("api/ussd/release", function($xml) {
		var ret = xml2object($xml);
		if(isAjaxReturnOK(ret)) {
			g_ussd_currentFun= null;
			$(".button_wrapper").removeClass("disable_btn");
		} else {
			//showInfoDialog(common_failed);
			log.debug("USSD:<release> not return response");
		}
	}, {
		//sync:true,
		errorCB: function() {
			log.debug("USSD:<release> file is not find");
		}
	});
}