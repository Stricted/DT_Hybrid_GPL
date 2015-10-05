var chooseContent = "<div id='file_path_wrapper' class='file_path_wrapper'><p>" + sd_hint_select_a_folder + "</p><div id='file_tree_wrapper' class='file_tree_wrapper'><div id='file_tree_div' class='file_tree_div'></div></div></div>";
var folder_path = {
    CurrentPath: ''
};
var check_browse_num = 0;
var FILE_LIST_TYPE_FOLDER = '0';
var choice = [];    //the add list on the page
var ADD_NUM = 10;   //the max number on the page
function sd_resolveEntityReference(str){
    return str.replace(/(\%26|\%27|\%20|\%23|\%25|\%3B|\'|\&)/g,
                          function($0, $1)
                          {
                              return{
                                '%26' : '&amp;' ,
                                '%27' : '&#39;' ,
                                '%20' : ' ' ,
                                '%23' : '#' ,
                                '%25' : '%' ,
                                '%3B' : ';' ,
                                "'" : '&#39;' ,
                                '&' : '&amp;'
                              }[$1];
                          }
                         );
}

function initPage(){
    var i = 0;
    var k = 0;
    var dms_status = '';
    var choice_old = [];
    getAjaxData('api/settings/dms', function($xml) {
        var dms_ret = xml2object($xml);
        if (dms_ret.type == 'response') {
            dms_status = dms_ret.response;
            $('#enable_dms').get(0).checked = (dms_status.DMS_enable == 1);
            $('#auto_infomation').get(0).checked = (dms_status.AutoDetectUT == 1);
            if(typeof(dms_status.Directorys)!='undefined' && typeof(dms_status.Directorys.Directory)!='undefined'){
                if($.isArray(dms_status.Directorys.Directory)){
                    for(i=0;i<dms_status.Directorys.Directory.length;i++){
                        choice_old.push(dms_status.Directorys.Directory[i]);
                    }
                } else {
                    choice_old.push(dms_status.Directorys.Directory);
                }
            } else {
                return;
            }
        }else{
            log.error("Load dms Status data failed");
        }
    },{
        sync: true
    });
    addFilter($('#share_directory_list tr'), choice_old);
    button_enable('apply', '0');
}

function checkEnableDms(){
    var i = 0;
    if($('#enable_dms').attr('checked')){
        for(i=0;i<choice.length;i++){
            $('#share_directory'+choice[i]).attr('disabled',false);
            $('#share_directory_input'+choice[i]).attr('disabled',false);
            button_enable('button_browse'+choice[i], '1');
            button_enable('button_delete'+choice[i], '1');
            button_enable('button_add'+choice[i], '1');
        }
        if(1 == choice.length && $('#share_directory_input'+choice[0]).val() == "/"){
            button_enable('button_delete'+choice[0], '0');
        }
        if(choice.length == ADD_NUM){
            button_enable('button_add'+choice[ADD_NUM-1], '0');
        }
    } else {
        for(i=0;i<choice.length;i++){
            $('#share_directory'+choice[i]).attr('disabled',true);
            $('#share_directory_input'+choice[i]).attr('disabled',true);
            button_enable('button_browse'+choice[i], '0');
            button_enable('button_delete'+choice[i], '0');
            button_enable('button_add'+choice[i], '0');
        }
    }
}

function checkDirectory()
{
    clearAllErrorLabel();
    for(var i=0;i<choice.length;i++){
        if($('#share_directory_input'+choice[i]).val() != ""){
            var directory_i = $('#share_directory_input'+choice[i]).val().indexOf('/mnt/')==0?$('#share_directory_input'+choice[i]).val():"/mnt"+$('#share_directory_input'+choice[i]).val();
            for(var j=0;j<choice.length;j++){
                if( i != j ){
                    if($('#share_directory_input'+choice[j]).val() != ""){
                        var directory_j = $('#share_directory_input'+choice[j]).val().indexOf('/mnt/')==0?$('#share_directory_input'+choice[j]).val():"/mnt"+$('#share_directory_input'+choice[j]).val();
                        if(directory_i == directory_j){
                            showErrorUnderTextbox('share_directory_input'+choice[j],err_dms_directory);
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

function getData(){
    var DMSArray = [];
    var i = 0;
    var j = 0;
    var submitObject = null;
    var dms_enable = 0;
    var dms_auto = 0;
    if($('#enable_dms').attr('checked')){
        dms_enable = 1;
    } else {
        dms_enable = 0;
    }
    if($('#auto_infomation').attr('checked')){
        dms_auto = 1;
    } else {
        dms_auto = 0;
    }
    for(i=0;i<choice.length;i++){
        var dms_active = 0;
        var filter = null;
        if($('#share_directory'+choice[i]).attr('checked')){
            dms_active = 1;
        } else {
            dms_active = 0;
        }
        if(choice.length == 1 && $('#share_directory_input'+choice[0]).val()=='/'){
            filter = {
                Active :dms_active,
                InstanceID: 1,
                ContentDirectory:'',
                ContentDirectoryUTF:''
            };
            DMSArray.push(filter);
        }
        if($('#share_directory_input'+choice[i]).val()!='/'){
            if(dms_auto == 0){
                filter = {
                    Active :dms_active,
                    InstanceID: i+1-j,
                    ContentDirectory:$('#share_directory_input'+choice[i]).val(),
                    ContentDirectoryUTF:''
                };
            } else {
                filter = {
                    Active :dms_active,
                    InstanceID: i+1-j,
                    ContentDirectory:'',
                    ContentDirectoryUTF:$('#share_directory_input'+choice[i]).val()
                };
            }
            j = 0;
            DMSArray.push(filter);
        } else {
            j++;
        }
    }
    submitObject = {
        DMS_enable:dms_enable,
        AutoDetectUT:dms_auto,
        Directorys: {
            Directory:DMSArray
        }
    };
    return submitObject;
}

function postData(){
    if(checkDirectory()){
        var newXmlString = object2xml('request', getData());
        saveAjaxData('api/settings/dms', newXmlString, function($xml) {
            var ret = xml2object($xml);
            if (isAjaxReturnOK(ret)) {
                var i = 0;
                for(i=0;i<choice.length;){
                    deleteFilter(0, $(".user_dms_add_line"));
                    choice.splice(0,1);
                }
                parent.showInfoDialog(common_success);
                initPage();
                checkEnableDms();
                button_enable('apply', '0');
            }else{
                parent.showInfoDialog(common_failed);
                button_enable('apply', '1');
            }
        });
    }
    else
    {
        checkEnableDms();
    }
}

$(document).ready(function() {
    initPage();
    $('#enable_dms').click(function() {
        checkEnableDms();
        button_enable('apply', '1');
    });
    $('#apply').click(function() {
        if (!isButtonEnable('apply')) {
            return;
        }
        button_enable('apply', '0');
        for(i=0;i<choice.length;i++){
            $('#share_directory'+choice[i]).attr('disabled',true);
            $('#share_directory_input'+choice[i]).attr('disabled',true);
            button_enable('button_browse'+choice[i], '0');
            button_enable('button_delete'+choice[i], '0');
            button_enable('button_add'+choice[i], '0');
        }
        postData();
    });
    $('#auto_infomation').bind('click', function() {
        button_enable('apply', '1');
    });
    checkEnableDms();
});