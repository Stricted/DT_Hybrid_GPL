var g_firewallSwitchData = [];
var g_firewallAttackData = [];
$(document).ready(function() {
    button_enable('apply_button', '0');
    button_enable('attack_button', '0');
    $("input[name='firewallswitch']").click(function() {
        button_enable('apply_button', '1');
        checked_ck();
    });
    $("input[name='firewallattack']").click(function() {
        button_enable('attack_button', '1');
    });

    $("#apply_button").click(function() {
        if (isButtonEnable('apply_button')) 
        {
            if(true==$('#checkbox_FirewallMainSwitch').get(0).checked )
            {
                parent.showConfirmDialog(firewall_hint_submit_list_item, apply, function(){});
            }
            else
            {
                parent.showConfirmDialog(firewall_hint_submit_list_item_disable_firewall, apply, function(){});
            }
        }   
    });

    $('#attack_button').click(function(){
        if (isButtonEnable('attack_button')){
            parent.showConfirmDialog(firewall_hint_submit_list_item, Apply_Attack, function(){});
        }
    });
    initPage();
    initPage_attack();
});

function initPage() {
    getAjaxData('api/security/firewall-switch', function($xml) {
        var ret = xml2object($xml);
        if (ret.type == 'response')
        {
            g_firewallSwitchData = ret.response;
            $('#checkbox_FirewallMainSwitch').get(0).checked = (g_firewallSwitchData.FirewallMainSwitch == 1);
            $('#checkbox_FirewallIPFilterSwitch').get(0).checked = (g_firewallSwitchData.FirewallIPFilterSwitch == 1);
            $('#checkbox_FirewallWanPortPingSwitch').get(0).checked = (g_firewallSwitchData.FirewallWanPortPingSwitch == 0);
            $('#checkbox_FirewallLanMacFilterSwitch').get(0).checked = (g_firewallSwitchData.FirewallLanMacFilterSwitch == 1);
            $('#checkbox_FirewallUrlFilterSwitch').get(0).checked = (g_firewallSwitchData.FirewallUrlFilterSwitch == 1);
            checked_ck();
        }
    });
}

function initPage_attack(){
    getAjaxData('api/security/AttackProtecting', function($xml){
        var ret = xml2object($xml);
        if (ret.type == 'response'){
            g_firewallAttackData = ret.response;
            $('#checkbox_FirewallICMP').get(0).checked = (g_firewallAttackData.IcmpFlooding == 1);
            $('#checkbox_FirewallSYN').get(0).checked = (g_firewallAttackData.SynFlooding == 1);
            $('#checkbox_FirewallARP').get(0).checked = (g_firewallAttackData.ArpAttack == 1);
        }
    });
}

function apply() {
    g_firewallSwitchData.FirewallMainSwitch = $('#checkbox_FirewallMainSwitch').get(0).checked ? 1 : 0;
    g_firewallSwitchData.FirewallIPFilterSwitch = $('#checkbox_FirewallIPFilterSwitch').get(0).checked ? 1 : 0;
    g_firewallSwitchData.FirewallWanPortPingSwitch = $('#checkbox_FirewallWanPortPingSwitch').get(0).checked ? 0 : 1;
    g_firewallSwitchData.FirewallLanMacFilterSwitch = $('#checkbox_FirewallLanMacFilterSwitch').get(0).checked ? 1 : 0;
    g_firewallSwitchData.FirewallUrlFilterSwitch = $('#checkbox_FirewallUrlFilterSwitch').get(0).checked ? 1 : 0;
    var xmlstr_security = object2xml('request', g_firewallSwitchData);
    saveAjaxData('api/security/firewall-switch', xmlstr_security, function($xml) {
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret))
        {
            button_enable('apply_button', '0');
            parent.showInfoDialog(common_success);
            initPage();
        }
        else
        {
            parent.showInfoDialog(common_failed);
        }
    });
}

function Apply_Attack(){
    g_firewallAttackData.IcmpFlooding = $('#checkbox_FirewallICMP').get(0).checked ? 1 : 0 ;
    g_firewallAttackData.SynFlooding = $('#checkbox_FirewallSYN').get(0).checked ? 1 : 0;
    g_firewallAttackData.ArpAttack = $('#checkbox_FirewallARP').get(0).checked ? 1 : 0;
    var attackdata_xml = object2xml('request', g_firewallAttackData);
    saveAjaxData('api/security/AttackProtecting', attackdata_xml, function($xml){
        var ret = xml2object($xml);
        if (isAjaxReturnOK(ret)){
            button_enable('attack_button', '0');
            parent.showInfoDialog(common_success);
            initPage_attack();
        }else{
            parent.showInfoDialog(common_failed);
        }
    });
}

function checked_ck() {
    if ($('#checkbox_FirewallMainSwitch').attr('checked') == "checked")
    {
        $('#checkbox_FirewallIPFilterSwitch').removeAttr('disabled');
        $('#checkbox_FirewallWanPortPingSwitch').removeAttr('disabled');
        $('#checkbox_FirewallLanMacFilterSwitch').removeAttr('disabled');
        $('#checkbox_FirewallUrlFilterSwitch').removeAttr('disabled');
        $('#label_enable').css('color', '#000000');
        $('#label_disable').css('color', '#000000');
    }
    else
    {
        $('#checkbox_FirewallIPFilterSwitch').attr('disabled', 'disabled');
        $('#checkbox_FirewallWanPortPingSwitch').attr('disabled', 'disabled');
        $('#checkbox_FirewallLanMacFilterSwitch').attr('disabled', 'disabled');
        $('#checkbox_FirewallUrlFilterSwitch').attr('disabled','disabled');
        $('#label_enable').css('color', '#baaaaa');
        $('#label_disable').css('color', '#baaaaa');
    }
}
