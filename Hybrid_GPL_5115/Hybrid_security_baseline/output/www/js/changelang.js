// JavaScript Document

function creatLangList() {

   if((typeof(LANGUAGE_DATA.current_language) != 'undefined' && LANGUAGE_DATA.current_language == 'ar_sa')
    ||(typeof(parent.LANGUAGE_DATA.current_language) != 'undefined' && parent.LANGUAGE_DATA.current_language == 'ar_sa')
     )
    {
        $('link').attr('href', '../css/main_Arabic.css');
    }
}

$(function() {
    creatLangList();
});