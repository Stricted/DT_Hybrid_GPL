/*局部刷新*/
var g_href = '';
var g_sub = '';
var g_name = '';
var g_time = 1000;
function getRefresh(){
    var objValue = [];
    g_href = parent.current_href;
    if($.cookies.get('cookie')!=null){
        objValue = $.cookies.get('cookie').split(",");
        if(g_href != objValue[0]){
            if(g_href == "setting"){
                g_sub = "dialup";
                g_name = "connections";
            } else if(g_href == "maintenance"){
                g_sub = "";
                g_name = "log";
            }
        } else {
            g_sub = objValue[1];
            g_name = objValue[2];
        }
    } else {
        if(g_href == "setting"){
            g_sub = "dialup";
            g_name = "connections";
        } else if(g_href == "maintenance"){
            g_sub = "";
            g_name = "log";
        }
    }
}

function showHtml(){
    if(current_href == g_href){
        $("#show_html").attr("src",g_name+".html");
        g_href = current_href;
        g_sub = g_sub;
        g_name = g_name;
    } else {
        if(current_href == "setting"){
            $("#show_html").attr("src","connections.html");
            g_href = current_href;
            g_sub = "dialup";
            g_name = "connections";
        } 
        else if(current_href == "smsinbox"){
            $("#show_html").attr("src","smsinbox.html");
            g_href = current_href;
            g_sub = "";
            g_name = "smsinbox";
        }
    }
    getUserManualUrl(g_name);

}
function showMenu(menu,name) {
    var sub = [];
    var getEls = $('.' + menu + ' li');
    $.each(getEls, function(i) {
        if($(getEls[i]).hasClass('nosubclick')){
            $(getEls[i]).removeClass('nosubclick');
            $(getEls[i]).addClass('nosub');
        }
        if($(getEls[i]).hasClass('click')){
            $(getEls[i]).removeClass('click');
        }
        if($(getEls[i]).hasClass('subClick')){
            $(getEls[i]).removeClass('subClick');
        }
        if($(getEls[i]).hasClass('sub')||$(getEls[i]).hasClass('nosub')){
            var str = getEls[i].id;
            sub.push(str);
        }
    });
    var i;
    for(i = 0;i<sub.length;i++){
        $("#"+sub[i]).click(function() {
            if($(this).hasClass('sub')){
                g_sub = this.id;
            } else {
                g_sub = "";
            }
        });
    }
    if(g_sub!=""){
        $("#"+g_sub).addClass('click');
    } 
    if ($("#"+name).hasClass('nosub')) {
        $(getEls).removeClass('subClick');
        $(getEls).removeClass('click');
        $("#"+name).addClass('nosubclick');
    }
    else if ($("#"+name).hasClass('sub')) {
        $(getEls).removeClass('click');
        $("#"+name).addClass('click');
    }
    else {
        $(getEls).removeClass('subClick');
        $("#"+name).addClass('subClick');
    }
    $("#"+name).mouseover(function() {
        $("#"+name).addClass('menu_hover');
    });
    $("#"+name).mouseout(function() {
        $("#"+name).removeClass('menu_hover');
    });
}
function changeMenu(){
    var id = '';
    if(g_href == 'setting'){
        id = 'menu_settings';
    }
    else if(g_href == 'maintenance'){
        id = 'menu_maintenance';
    }
    return id;
}
$(document).ready(function() {
    
    setTimeout(function(){
        getRefresh();
        showHtml();
        $.cookies.set('cookie', g_href+","+g_sub+","+g_name, { expiresAt: new Date().getTime()+5*60*1000 });
        showMenu(changeMenu(),g_name);
    },g_time);
});