var g_deleteIndex = 0;

// delete item
function deleteFilter(g_deleteIndex, item){
    item.eq(g_deleteIndex).remove();
}
//if there hasn't any add or delete button it won't work
$(".button_delete_list").live("click", function(){
    if($(".add_item_control:hidden").size() > 0 && $("#edit_item_ok").size() < 1){
        g_deleteIndex = $(".button_delete_list").index(this);
        parent.call_dialog(common_delete, firewall_hint_delete_list_item, common_ok, "pop_OK", common_cancel, "pop_Cancel",pop_click,pop_cancel_click);
    }
});

/*--------------pop ok function---------------*/
function pop_click() {
    deleteFilter(g_deleteIndex, $(".user_add_line"));
    parent.clearDialog();
    button_enable("apply", "1");
    button_enable("add_item", "1");
}

/*------------------pop cancel click-------------*/
function pop_cancel_click() {
    parent.clearDialog();
}

//function for add item
//addFilter(insertNode,tdText1,tdText2,tdText3,...);
function addFilter(insertNode){
    var addLine = null;
    var i = 1;
    addLine = "<tr class=\"user_add_line\">";
    for ( i = 1; i < arguments.length; i++ ) 
    {
        addLine += "<td>" + arguments[i] + "</td>";
    }
    addLine += "<td class='user_options'><span class=\"button_edit_list\">" + common_edit + "</span>&nbsp;&nbsp;<span class=\"button_delete_list\">" + common_delete + "</span></td></tr>";
    
    var currentTrTotal = $(insertNode).size();
    $(insertNode).eq(currentTrTotal - 2).after(addLine);
}

//show add item control
function showAddItemControl(){
    $(".add_item_control").show();
}

//hide add item control
function hideAddItemControl(){
    $(".add_item_control").hide();
}

function getDArrayElement( obj, item, which ){
    var val = "";
    if ( obj && $.isArray(obj) ) 
    {
        $(obj).each( function(i){
            if( "value" == which )
            {                
                if ( obj[i][0] == item ) 
                {
                    val = obj[i][1];
                }
            }
            else if( "key" == which )
            {
                if ( obj[i][1] == item ) 
                {
                    val = obj[i][0];
                }
            }
            else
            {                
            }
        });
    }
    
    return val;
}

function createSelect( where, id, option ){
    where.html("<select id="+ id +"></select>");
    
    if( $.isArray(option) )
    {
        $(option).each( function(n){
            $("#"+ id).append("<option value=" + option[n][0]+ ">"+ option[n][1] + "</option>");             
        });
    }   
}

function initSelectOption( ids, dArray ){
    var i=0; 
    for (i=0; i < dArray.length; i++) {
        if( $.isArray(ids) )
        {
            $(ids).each( function(n){
                $("#" + ids[n]).append("<option value=" + dArray[i][0]+ ">"+ dArray[i][1] + "</option>");                 
            });
        }
        else
        {
            $("#" + ids).append("<option value=" + dArray[i][0]+ ">"+ dArray[i][1] + "</option>");             
        }
    }
}
