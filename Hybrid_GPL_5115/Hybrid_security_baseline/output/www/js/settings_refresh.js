
function changeHtml(nameHtml){

    if(parent.G_MonitoringStatus.response.loginState != "1"){
       gotoPageWithoutHistory(LOGIN_PAGE_URL); 
    }
    var href = '';
    var name = '';
    if(current_href == g_href){
        $("#show_html").attr("src",nameHtml+".html");
        href = current_href;
        g_sub = g_sub;
        name = nameHtml;
    } else {
        if(current_href == "setting"){
            $("#show_html").attr("src","connections.html");
            href = current_href;
            g_sub = "dialup";
            name = "connections";
        }
        else if(current_href == "maintenance"){
            $("#show_html").attr("src","log.html");
            href = current_href;
            g_sub = "";
            name = "logview";
        }
    }
    parent.getUserManualUrl(name);
    
    $.cookies.set('cookie', href+","+g_sub+","+name, { expiresAt: new Date().getTime()+5*60*1000 });
    if(false == parent.g_setting_click)
    {
    	clickMenu('menu_settings');
    }
}
function clickMenu(menu) {
    parent.g_setting_click = true;
    var getEls = $('.' + menu + ' li');
    $.each(getEls, function(i) {
        for(var i=0;i<getEls.length;i++){
            if($(getEls[i]).hasClass('nosubclick')){
                $(getEls[i]).removeClass('nosubclick');
                $(getEls[i]).addClass('nosub');
            }
        }
        $(this).click(function() {
            if ($(this).hasClass('sub') && $(this).hasClass('click')) {
                $(this).removeClass('click');
                return false;
            }
            else if ($(this).hasClass('nosub')) {
                $(getEls).removeClass('subClick');
                $(getEls).removeClass('click');
                $(this).addClass('nosubclick');
                return false;
            }
            else if ($(this).hasClass('sub')){
                $(getEls).removeClass('click');
                $(this).addClass('click');
                return false;
            }
            else {
                $(getEls).removeClass('nosubclick');
                $(getEls).removeClass('subClick');
                $(this).addClass('subClick');
                return false;
            }
        });
        $(this).mouseover(function() {
            $(this).addClass('menu_hover');
        });
        $(this).mouseout(function() {
            $(this).removeClass('menu_hover');
        });
    });
}