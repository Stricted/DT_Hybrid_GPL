
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
    clickMenu('menu_maintenance');
}
function clickMenu(menu) {
    var getEls = $('.' + menu + ' li ');
    $.each(getEls, function(i) {
        $(this).click(function() {
            $(getEls).addClass('nosub');
             if ($(this).hasClass('nosub')) {
                $(getEls).removeClass('nosubclick');
                $(getEls).removeClass('subClick');
                $(this).addClass('nosubclick');
                return true;
            }
        });
   });
}