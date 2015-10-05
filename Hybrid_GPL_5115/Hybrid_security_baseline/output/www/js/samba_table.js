var deleteIndex = 0;
// delete item
var chooseContent = "<div id='file_path_wrapper' class='file_path_wrapper'><p>" + sd_hint_select_a_folder + "</p><div id='file_tree_wrapper' class='file_tree_wrapper'><div id='file_tree_div' class='file_tree_div'></div></div></div>";
var folder_path = {
    CurrentPath: ''
};
var samba_path = '/';
var FILE_LIST_TYPE_FOLDER = '0';
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
function deleteFilter(deleteIndex, item){
    item.eq(deleteIndex).remove();
}    

parent.$(".button_samba_browse_list").die().live("click", function(){
    if (!isButtonEnable('button_browse_samba'))
    {
        return;
    }
    samba_path = '/';
    getinformation();
    parent.clearDialog();
    parent.call_dialog(sd_hint_select_a_folder, chooseContent, common_ok, 'path_dialog_ok', common_cancel, 'path_dialog_cancel',browse_pop_ok,browse_pop_cancel);
    displayCustomerTree('file_tree_div', '/');
    button_enable('path_dialog_ok', '0');
    button_enable('apply', '1');
});
/*-----------------function browse ok---------------*/
function browse_pop_ok() {
    if (!isButtonEnable('path_dialog_ok')) {
        return;
    }
    var folder_path = parent.$('a.focusfolder').closest('span').attr('id');
    if (typeof(folder_path) != 'undefined') {
        folder_path = folder_path.substring(folder_path.indexOf('_') + 1);
        folder_path = folder_path.replace(/\*/g, '/');
        folder_path = '/' +folder_path;   
        samba_path = folder_path;     
    } else {      
    }
    g_main_displayingPromptStack.pop();  
    parent.clearDialog();
    restore_dialog();
}
/*--------------------function pop cancel--------------*/
function browse_pop_cancel() {
    g_main_displayingPromptStack.pop();
    parent.clearDialog();
    restore_dialog();
}
function restore_dialog()
{
    showDialog(information.dialogname);
    parent.$("#samba_username").attr("value",information.Username);
    parent.$("#samba_drectory").attr("value",samba_path);
    parent.$("#samba_right").attr("value",information.Privilege);
    parent.$("#samba_password").attr("value",information.Password);
    parent.$("#samba_comfirm_password").attr("value",information.Password);
    checkpasswordstrength();
}
//Folder of folder tree clicked
parent.$('.sub_folder_li').die().live('click', function() {
    button_enable('path_dialog_ok', '1');
});
//Folder tree list click
parent.$('div.hitarea').die().live('click', function() {
    var list_id = $(this).closest('li').attr('id');
    var list_path = $(this).next().attr('id');
    list_path = list_path.substring(list_path.indexOf('_') + 1);
    list_path = list_path.replace(/\&/g, '&amp;');
    if (parent.$('#' + list_id).hasClass('expandable') && !parent.$('#' + list_id).hasClass('loaded_sublist')){
        displayCustomerTree(list_id, list_path);
        createSubFolderTree(list_id);
        if (parent.$('#' + list_id).children('div').hasClass('noSubFolder'))
        {
            parent.$('#' + list_id).children('span.folder_path_line').css({'background': 'none'});
        }
        else
        {
            parent.$('#' + list_id).children('span.folder_path_line').css({'background': 'url(../res/treeview-default-line.gif) 0 0 repeat-y'});
        }
    }else if (parent.$('#' + list_id).hasClass('expandable') && parent.$('#' + list_id).hasClass('loaded_sublist')){
        showSubFolderTree(list_id);
        parent.$('#' + list_id).children('span.folder_path_line').css({'background': 'url(../res/treeview-default-line.gif) 0 0 repeat-y'});
    }else{
        hideSubFolderTree(list_id);
        parent.$('#' + list_id).children('span.folder_path_line').css({'background': 'none'});
    }
});
//Mouseover folder tree
parent.$('a.folder_name').die().live('mouseover', function() {
    $(this).addClass('folder_hover');
});
parent.$('a.folder_name').live('mouseout', function() {
    $(this).removeClass('folder_hover');
});
//Mouse click folder tree
parent.$('a.folder_name').live('click', function() {
    if ($(this).hasClass('focusfolder') == false) {
        parent.$('a.focusfolder').removeClass('focusfolder');
        $(this).addClass('focusfolder');
    }
});
//Mouse over and mouse out on folder
parent.$('.sd_img_file').die().live('mouseover', function() {
    $(this).children().addClass('menu_hover');
});
parent.$('.sd_img_file').live('mouseout', function() {
    $(this).children().removeClass('menu_hover');
});
//Mouse over and mouse out on file
parent.$('.sd_img').die().live('mouseover', function() {
    $(this).children().addClass('menu_hover');
});
parent.$('.sd_img').live('mouseout', function() {
    $(this).children().removeClass('menu_hover');
});
//Click on folder
parent.$('.sd_img_file').live('click', function() {
    var current_path = $(this).children().children().html();
    var parent_path = $('.micro_sd').children().html();
    if ('' == parent_path || '/' == parent_path){
        current_path = parent_path + current_path;
    }else{
        current_path = parent_path + '/' + current_path;
    }
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
    button_enable('common_delete', '0');
    folderlists(current_path);
});
//Click up level button
parent.$('#sd_label_up').click(function() {
    if (!isButtonEnable('sd_label_up')){
        return;
    }
    var uplevel_file_name = parent.$('.micro_sd').children().html();
    uplevel_file_name = uplevel_file_name.substring(0, uplevel_file_name.lastIndexOf('/'));
    uplevel_file_name = ('' == uplevel_file_name) ? '/' : uplevel_file_name;
    parent.showWaitingDialog(common_waiting, sd_hint_wait_a_few_moments);
    button_enable('common_delete', '0');
    folderlists(uplevel_file_name);
});
//function for add item
//addFilter(insertNode,tdText1,tdText2,tdText3,...);
function addFilter(insertNode,DirectoryInstance){
    var i = 1;
    var j = 0;
    var addLine = "";
    if(0!=choice.length){
        choice.splice(0,choice.length);
    }
    for ( i = 1; i < arguments.length; i++ ){
        if(0==DirectoryInstance.length){
            addLine += "<tr height='35' class=\'user_dms_add_line\'>"+
                   "<td width='5%'>"+
                       "<input class=\'dms_checkbox_list\' name='share_directory' id='share_directory1' type='checkbox' checked />"+
                   "<td width='56%'>"+
                       "<input id='share_directory_input1' name='share_directory_input1' type='text' value=''  class=\'dms_input\' readonly size='20'/>"+
                   "</td>"+
                   "<td width='15%'>"+
                       "<span class=\'button_dms_browse_list\' id='button_browse1'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_browse + "</span></span></span></span>"+
                   "</td>"+
                   "<td width='14%'>"+
                       "<span class=\'button_dms_delete_list\' id='button_delete1'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_delete + "</span></span></span></span>"+
                   "</td>"+
                   "<td width='10%'>"+
                       "<span class=\'button_dms_add_list\' id='button_add1'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_add + "</span></span></span></span>"+
                   "</td>"+
               "</tr>";
               choice.push(1);
        } else {
            for(j=0;j<DirectoryInstance.length;j++){
                addLine += "<tr height='35' class=\'user_dms_add_line\'>"+
                               "<td width='5%'>";
                if(1 == DirectoryInstance[j].Active){
                    addLine +=     "<input class=\'dms_checkbox_list\' name='share_directory' id='share_directory" + (j+1) + "' type='checkbox' checked />";
                } else {
                    addLine +=     "<input class=\'dms_checkbox_list\' name='share_directory' id='share_directory" + (j+1) + "' type='checkbox'/>";
                }
                    addLine += "</td>"+
                               "<td width='56%'>";
                if($('#auto_infomation').attr('checked')){
                    addLine +=     "<input id='share_directory_input" + (j+1) + "' name='share_directory_input"+ (j+1) +"' type='text' value='"+sdResolveCannotParseChar(DirectoryInstance[j].ContentDirectoryUTF)+"'  class=\'dms_input\' readonly size='20'/>";
                } else {
                    addLine +=     "<input id='share_directory_input" + (j+1) + "' name='share_directory_input"+ (j+1) +"' type='text' value='"+DirectoryInstance[j].ContentDirectory+"'  class=\'dms_input\' readonly size='20'/>";
                }
                    addLine += "</td>"+
                               "<td width='15%'>"+
                                   "<span class=\'button_dms_browse_list\' id='button_browse" + (j+1) + "'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_browse + "</span></span></span></span>"+
                               "</td>"+
                               "<td width='14%'>"+
                                   "<span class=\'button_dms_delete_list\' id='button_delete" + (j+1) + "'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_delete + "</span></span></span></span>"+
                               "</td>"+
                               "<td width='10%'>";
                if(j == DirectoryInstance.length - 1){
                    addLine += "<span class=\'button_dms_add_list\' id='button_add" + (j+1) + "'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_add + "</span></span></span></span>";
                } else {
                    addLine += "<span class=\'button_dms_add_list\' id='button_add" + (j+1) + "' style='display:none;'><span class='button_left'><span class='button_right'><span class='button_center'>" + common_add + "</span></span></span></span>";
                }
                addLine +=     "</td>"+
                           "</tr>";
                choice.push(j+1);
            }
        }
    }
    $('#share_directory_list').append(addLine);
    if(1 == choice.length){
        button_enable('button_delete'+choice[0], '0');
    }
}

function createSubFolderTree(obj_id) {
   if (parent.$('#' + obj_id).hasClass('lastExpandable')){
       parent.$('#' + obj_id).addClass('collapsable');
       parent.$('#' + obj_id).removeClass('expandable');
       parent.$('#' + obj_id).addClass('lastCollapsable');
       parent.$('#' + obj_id).removeClass('lastExpandable');
       parent.$('#' + obj_id).children('div').addClass('collapsable-hitarea');
       parent.$('#' + obj_id).children('div').removeClass('expandable-hitarea');
       parent.$('#' + obj_id).addClass('loaded_sublist');
   }else{
       parent.$('#' + obj_id).addClass('collapsable');
       parent.$('#' + obj_id).removeClass('expandable');
       parent.$('#' + obj_id).children('div').addClass('collapsable-hitarea');
       parent.$('#' + obj_id).children('div').removeClass('expandable-hitarea');
       parent.$('#' + obj_id).addClass('loaded_sublist');
   }
}

function showSubFolderTree(obj_id) {
    if (parent.$('#' + obj_id).hasClass('lastExpandable')){
        parent.$('#' + obj_id).addClass('collapsable');
        parent.$('#' + obj_id).removeClass('expandable');
        parent.$('#' + obj_id).addClass('lastCollapsable');
        parent.$('#' + obj_id).removeClass('lastExpandable');
        parent.$('#' + obj_id).children('div').addClass('collapsable-hitarea');
        parent.$('#' + obj_id).children('div').removeClass('expandable-hitarea');
        parent.$('#' + obj_id).children('ul').css({'display': 'block'});
    }else{
        parent.$('#' + obj_id).addClass('collapsable');
        parent.$('#' + obj_id).removeClass('expandable');
        parent.$('#' + obj_id).children('div').addClass('collapsable-hitarea');
        parent.$('#' + obj_id).children('div').removeClass('expandable-hitarea');
        parent.$('#' + obj_id).children('ul').css({'display': 'block'});
    }
}

function hideSubFolderTree(obj_id) {
    if (parent.$('#' + obj_id).hasClass('lastCollapsable')){
        parent.$('#' + obj_id).addClass('expandable');
        parent.$('#' + obj_id).removeClass('collapsable');
        parent.$('#' + obj_id).addClass('lastExpandable');
        parent.$('#' + obj_id).removeClass('lastCollapsable');
        parent.$('#' + obj_id).children('div').addClass('expandable-hitarea');
        parent.$('#' + obj_id).children('div').removeClass('collapsable-hitarea');
        parent.$('#' + obj_id).children('ul').css({'display': 'none'});
    }else{
        parent.$('#' + obj_id).addClass('expandable');
        parent.$('#' + obj_id).removeClass('collapsable');
        parent.$('#' + obj_id).children('div').addClass('expandable-hitarea');
        parent.$('#' + obj_id).children('div').removeClass('collapsable-hitarea');
        parent.$('#' + obj_id).children('ul').css({'display': 'none'});
    }
}

function displayCustomerTree(parent_id , root_path) {
    var current_path = root_path;
    var filter_path = '';
    var newList = '<ul>';
    if ('/' == current_path) {
        filter_path = '/';
    } else {
        filter_path = '/' + current_path.replace(/\*/g, '/');
    }
    folder_path.CurrentPath = filter_path;
    var send_xml = object2xml('request', folder_path);
    saveAjaxData('api/settings/dms-sdfile', send_xml, function($xml) {
        var return_ret = xml2object($xml);
        if ('response' == return_ret.type) {
            var folder_lists = return_ret.response.FileList.File;
            if ($.isArray(folder_lists)) {
                var folder_count = 0;
                var folder_list_count = 0;
                $.each(folder_lists, function(i) {
                    if (FILE_LIST_TYPE_FOLDER == folder_lists[i].Type) {
                        folder_count++;
                    }
                });
                $.each(folder_lists, function(i) {
                    if (FILE_LIST_TYPE_FOLDER == folder_lists[i].Type) {
                        var folder_parent_id = '';
                        var folder_parent_path = '';
                        var folder_name = '';

                        if ('/' == root_path) {
                            folder_name = sd_resolveEntityReference(folder_lists[i].Name);
                            folder_parent_id = parent_id + i;
                            folder_parent_path = folder_name;
                        } else {
                            folder_name = sd_resolveEntityReference(folder_lists[i].Name);
                            folder_parent_id = parent_id + i;
                            folder_parent_path = current_path + '\*' + folder_name;

                        }
                        folder_parent_path = folder_parent_path.replace(/\'/g, '&#39;');
                        if ((folder_count - 1) > folder_list_count) {
                            newList += "<li id='" + folder_parent_id + "' class='sub_folder_li expandable'><div id='list_flag\*" + folder_parent_id + "' class='hitarea expandable-hitarea'></div><span  id='folder_" + folder_parent_path + "' class='folder_path_line'><a class='folder_name' href='javascript: void(0)'><pre>" + folder_name + '</pre></a></span></li>';
                        } else {
                            newList += "<li id='" + folder_parent_id + "' class='sub_folder_li expandable lastExpandable'><div id='list_flag\*" + folder_parent_id + "' class='hitarea expandable-hitarea lastExpandable-hitarea'></div><span id='folder_" + folder_parent_path + "' class='folder_path_line'><a class='folder_name' href='javascript: void(0)'><pre>" + folder_name + '</pre></a></span></li>';
                        }

                        folder_list_count++;
                    }
                });
                if ('<ul>' == newList) {
                    if (parent.$('#' + parent_id).children('div').hasClass('lastExpandable-hitarea')) {
                        parent.$('#' + parent_id).children('div').removeClass('hitarea').removeClass('lastExpandable-hitarea').removeClass('collapsable-hitarea').addClass('noSubFolder_last');
                        parent.$('#' + parent_id).children('span.folder_path_line').css({'background': 'none'});
                    } else {
                        parent.$('#' + parent_id).removeClass('expandable').removeClass('collapsable').children('div').removeClass('hitarea').removeClass('expandable-hitarea').removeClass('collapsable-hitarea').addClass('noSubFolder');
                        parent.$('#' + parent_id).children('span.folder_path_line').css({'background': 'none'});
                    }
                } else {
                    parent.$('#' + parent_id).append(newList);
                }
            } else if (typeof(folder_lists) != 'undefined') {
                if (FILE_LIST_TYPE_FOLDER == folder_lists.Type) {
                    var folder_parent_id = '';
                    var folder_parent_path = '';
                    var folder_name = '';

                    if ('/' == root_path) {
                        folder_name = sd_resolveEntityReference(folder_lists.Name);
                        folder_parent_id = parent_id + '0';
                        folder_parent_path = folder_name;
                    } else {
                        folder_name = sd_resolveEntityReference(folder_lists.Name);
                        folder_parent_id = parent_id + '0';
                        folder_parent_path = current_path + '\*' + folder_name;
                    }
                    folder_parent_path = folder_parent_path.replace(/\'/g, '&#39;');
                    newList += "<li id='" + folder_parent_id + "' class='sub_folder_li expandable lastExpandable'><div id='list_flag\*" + folder_parent_id + "' class='hitarea expandable-hitarea lastExpandable-hitarea'></div><span id='folder_" + folder_parent_path + "' class='folder_path_line'><a class='folder_name' href='javascript: void(0)'><pre>" + folder_name + '</pre></a></span></li>';
                    parent.$('#' + parent_id).append(newList);
                } else {
                    if (parent.$('#' + parent_id).children('div').hasClass('lastExpandable-hitarea')) {
                        parent.$('#' + parent_id).children('div').removeClass('hitarea').removeClass('lastExpandable-hitarea').removeClass('collapsable-hitarea').addClass('noSubFolder_last');
                        parent.$('#' + parent_id).children('span.folder_path_line').css({'background': 'none'});
                    } else {
                        parent.$('#' + parent_id).removeClass('expandable').removeClass('collapsable').children('div').removeClass('hitarea').removeClass('expandable-hitarea').removeClass('collapsable-hitarea').addClass('noSubFolder');
                        parent.$('#' + parent_id).children('span.folder_path_line').css({'background': 'none'});
                    }
                }
            } else {
                if (parent.$('#' + parent_id).children('div').hasClass('lastExpandable-hitarea')) {
                    parent.$('#' + parent_id).children('div').removeClass().addClass('noSubFolder_last');
                    parent.$('#' + parent_id).children('span.folder_path_line').css({'background': 'none'});
                } else {
                    parent.$('#' + parent_id).removeClass('expandable').removeClass('collapsable').children('div').removeClass().addClass('noSubFolder');
                    parent.$('#' + parent_id).children('span.folder_path_line').css({'background': 'none'});
                }
            }
        }
    }, {
        timeout:3000000
    });
}