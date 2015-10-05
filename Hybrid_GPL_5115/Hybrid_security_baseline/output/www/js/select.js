//the structure of selectbox
/*<div class="selecter">
<div class="selecter_select" >
<input type="text" value="Text1" />
</div>
<div class="selecter_options">
<div class="options_wrapper">
<span >Text1</span>
<span >Text2</span>
</div>
</div>
</div>*/
/*$(obj).createSelect( parameter );       //type of parameter is Object or void       default:  {onlyread:true,direction_up:false,maxlength:40,maxheight:200}
$(obj).createOption( parameter );       //type of parameter is Array                element in parameter is Array that like key-Value,   example:[[value1,Text1],[value2,Text2]]
$(obj).sVal( parameter );               //type of parameter is String or void
$(obj).sText( parameter );              //type of parameter is String or void
$(obj).onchange();                      //a listen function for the option be click
$(obj).disabled();                      //set the selectbox disabled
$(obj).sfocus();                        //input focus
$(obj).sselect();                       //input select
//obj is a container of the selectbox
*/

//
var g_selectBoxs = {};                        // to store the select options k-v

$.fn.createSelect = function() {
    log.debug("SELECTER(createSelect): set_param: "+arguments[0]);
    new createSelect(this,arguments[0]|| {});
    var newObj = this.attr("id");
    g_selectBoxs[newObj] = {
        curVal:null,
        arrVal:[]
    };
}
//
$.fn.createOptions = function() {
    arguments[0] = arguments[0]||[];
    log.debug("SELECTER(createOptions): OPTIONS-VAL: "+arguments[0]);
    new createOptions(this,arguments[0]);
    var newObj = this.attr("id");
    g_selectBoxs[newObj].arrVal = arguments[0];
}
//
$.fn.sVal = function() {
    log.debug("SELECTER(sVal): Val: "+arguments[0]);
    var newObj = this.attr("id");
    if(arguments.length>0) {
        var newVal = arguments[0];
        var kv = [newVal,newVal];//default val;
        $.each(g_selectBoxs[newObj].arrVal, function(n,v) {
            if(newVal == v[0]) {
            	kv = v;
                return kv;
            }
        });
        g_selectBoxs[newObj].curVal = kv[0];
        this.find("input").val(kv[1]);
    }
    else {
        return g_selectBoxs[newObj].curVal;
    }
}
//
$.fn.sText = function() {
    log.debug("SELECTER(sText): Text: "+arguments[0]);
    if(arguments.length>0) {
        this.find("input").val(arguments[0]);
    }
    else {
        return this.find("input").val()||null;
    }
}
//
$.fn.onchange = function(callBackFn) {
    $(this).find("span").bind("change", function(event,k,v) {
        callBackFn(k,v);
    })
}
//
$.fn.disabled = function() {
    this.find(".selecter_options").hide();
    this.find(".selecter_select").attr("disabled",arguments[0]).children().attr("disabled",arguments[0]);
}
//
$.fn.sfocus = function() {
    this.find("input").focus();
}
//
$.fn.sselect = function() {
    this.find("input").select();
}
//

function createSelect(parent,init_set) {
    var onlyread = typeof(init_set.onlyread) == "undefined";
    var direction_up = init_set.direction_up||false;
    var maxlength = init_set.maxlength||40;
    var maxheight = init_set.maxheight||600;
    //var disabled = init_set.disabled||false;
    //
    var init = true;
    //
    create_select();
    //
    function create_select() {
        var selecter = document.createElement("div");
        $(selecter).attr("class","selecter");
        var selecter_select = document.createElement("div");
        $(selecter_select).attr("class","selecter_select");
        var selecter_input = document.createElement("input");
        $(selecter_input).attr("type","text");
        $(selecter_input).attr("readonly",onlyread).css({
            "cursor":onlyread?"pointer":"default"
        });
        $(selecter_input).attr("maxlength", maxlength);
        selecter_select.appendChild(selecter_input);
        selecter_select.onclick = function(event) {
            if("true" == $(this).attr("disabled")) {
                return false;
            }

            parent.find(".selecter_options").toggle(0, function() {
                if(init) {
                    newPositionByDirection(selecter_select,direction_up,maxheight);
                    init = false;
                }
            });
        }
        selecter_input.onchange = function() {
            parent.sVal(this.value);
        }
        //
        $(document).click( function(event) {
            if ($(event.target).attr("class")=="selecter_select"
            || ($(event.target).attr("type")=="text" && onlyread)) {
                return;
            }
            else {
                $(selecter_select).next().hide();
            }
        });
        //
        selecter.appendChild(selecter_select);
        parent.html("").append($(selecter));
        log.debug("SELECTER: SELECT-HTML: "+parent.html());
    }

}

//
function createOptions(parent,options) {
    parent.find(".selecter_options").remove();
    if(options.length<=0) {
        return;
    }
    var selecter_options = document.createElement("div");
    $(selecter_options).attr("class","selecter_options");
    var options_wrapper = document.createElement("div");
    options_wrapper.setAttribute("class","options_wrapper");
    $(selecter_options).hide();
    $(selecter_options).append($(options_wrapper));
    parent.children().append($(selecter_options));
    //
    $.each(options, function(index,val) {
        var span = document.createElement("span");
        $(span).text(val[1]);
        $(span).bind("click", function() {
            parent.sVal(val[0]);
            parent.sText(val[1]);
            parent.find(".selecter_options").hide();
            $(this).trigger("change",val);
        });
        span.onmouseover = function() {
            $(this).addClass("selecter_options_hover");
        }
        span.onmouseout = function() {
            $(this).removeClass("selecter_options_hover");
        }
        options_wrapper.appendChild(span);
    });
    log.debug("SELECTER: SELECT-OPTIONS-HTML: "+selecter_options.parentNode.innerHTML);
}

//
function newPositionByDirection(select_div,up,maxheight) {
    var width = $(select_div).width();
    $(select_div).next().css({
        "width":width,
        "z-index":"-1"
    });
    $(select_div).find("input").width(width-18);
    //
    var span_borderTop = up?1:0;
    var span_borderBottom = up?0:1;
    $(select_div).next().find("span")
    .css({
        "border-top-width":span_borderTop,
        "border-bottom-width":span_borderBottom
    });
    //
    var height = $(select_div).next().find("div").height();
    var overflow = height>maxheight;
    if(overflow) {
        $(select_div).next().css({
            "height":maxheight,
            "overflow-y":"scroll",
            "overflow-x":"hidden"
        })
        .find("span").css({
            "padding-right":3,
            "width":width-20
        });
    }
    else {
        $(select_div).next().css({
            "border-top-width":0,
            "border-bottom-width":0
        });
    }
    if(up) {

        var position = $(select_div).position();
        var newTop = position.top - $(select_div).next().height();
        $(select_div).next().css({
            "top":newTop
        });
    }
    $(select_div).next().css({
        "z-index":"3"
    });
}