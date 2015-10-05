

$(document).ready(function() {
    setLangList();
    showCurrentLanguage();
    g_is_login_opened = true;
    getUserManualUrl();
    $('#link_login_cardlock').click(function() {
        window.location = HOME_PAGE_URL;
    });
});
