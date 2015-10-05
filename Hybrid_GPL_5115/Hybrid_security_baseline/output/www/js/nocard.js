// JavaScript Document

$(document).ready(function() {
    if (1 == g_feature.continue_button) {
        $('#link_login_nocard').show();
    }
    else
    {
        $('#link_login_nocard').hide();
    }

    setLangList();
    showCurrentLanguage();
    g_is_login_opened = true;
    $('#link_login_nocard').click(function() {
        getAjaxData('api/user/state-login', function($xml) {
            var ret = xml2object($xml);
            if (ret.type == 'response') {
                if (ret.response.State == '0') {
                    gotoPageWithoutHistory(HOME_PAGE_URL);
                } else {
                    gotoPageWithoutHistory(LOGIN_PAGE_URL);
                }
            }
        }, {
            sync: true
        });
    });
});
