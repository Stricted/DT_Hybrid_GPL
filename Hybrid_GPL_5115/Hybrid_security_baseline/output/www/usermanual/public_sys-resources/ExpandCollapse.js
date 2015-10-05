
var expandClassName = 'dropdownexpand';
var collapseClassName = 'dropdowncollapse';

/*
* 展开或折叠一个节点
*/
function ExpandorCollapseNode(obj)
{
    obj = obj.parentNode;
    if(obj.className == expandClassName) obj.className = collapseClassName;
    else obj.className = expandClassName;
}

/*
* 展开或折叠所有节点
*/

function ExpandorCollapseAllNodes(obj)
{
	var expandOrCollapse = obj.getAttribute("title");

	if(expandOrCollapse == "collapse")
	{
		obj.setAttribute("title","expand");
		obj.className = "dropdownAllButtonexpand";
		obj.innerHTML = "expand all";
	}
	else
	{
		obj.setAttribute("title","collapse");
		obj.className = "dropdownAllButtoncollapse";
		obj.innerHTML = "collapse all";
	}
	
    var allElements = document.getElementsByTagName('*');
	for (var i = 0; i < allElements.length; i++)
	{
		var el = allElements[i];
		if(el.className.indexOf(expandClassName) != -1 || el.className.indexOf(collapseClassName) != -1)
		{
			if(expandOrCollapse=="collapse") el.className = collapseClassName;
			else el.className = expandClassName;  
		}
	}
}


var log = new Object();
log.debug = function(param){};

// document.write
function dw(text){
    document.write(text);
}