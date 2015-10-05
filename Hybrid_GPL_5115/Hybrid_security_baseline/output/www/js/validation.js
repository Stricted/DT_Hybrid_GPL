
var CHARCODE_A = 65;
var CHARCODE_F = 70;
var CHARCODE_Z = 90;
var CHARCODE_a = 97;
var CHARCODE_f = 102;
var CHARCODE_z = 122;
var CHARCODE_0 = 48;
var CHARCODE_9 = 57;
var CHARCODE_DOT = 46;
var CHARCODE_UNDERLINE = 95;
var CHARCODE_SPACE = 32;
var CHARCODE_DASH = 45;
//Port value range
var MIN_PORT_RANGE = 1;
var MAX_PORT_RANGE = 65535;
//Filter status
var FILTER_DISABLED = 0;
var FILTER_ENABLED = 1;
//Protocol status
var PROTOCOL_BOTH = 0;
var PROTOCOL_IMCP = 1;
var PROTOCOL_TCP = 6;
var PROTOCOL_UDP = 17;

/*not support char*/
var MACRO_SUPPORT_CHAR_MIN = 32;
var MACRO_SUPPORT_CHAR_MAX = 127;
var MACRO_NOT_SUPPORT_CHAR_COMMA = 44; //,
var MACRO_NOT_SUPPORT_CHAR_QUOTATION_MARK = 34; //"
var MACRO_NOT_SUPPORT_CHAR_COLON = 58; //:
var MACRO_NOT_SUPPORT_CHAR_SEMICOLON = 59; //;
var MACRO_NOT_SUPPORT_BACKSLASH_MARK = 92; //\
var MACRO_NOT_SUPPORT_CHAR_38 = 38; //&
var MACRO_NOT_SUPPORT_CHAR_37 = 37; //%
var MACRO_NOT_SUPPORT_CHAR_43 = 43; //+
var MACRO_NOT_SUPPORT_CHAR_39 = 39; //'
var MACRO_NOT_SUPPORT_CHAR_60 = 60; //<
var MACRO_NOT_SUPPORT_CHAR_62 = 62; //>
var MACRO_NOT_SUPPORT_CHAR_63 = 63; //?
var MACRO_NOT_SUPPORT_CHAR_64 = 64; //@
var MACRO_NOT_SUPPORT_CHAR_94 = 94; //^
var MACRO_NOT_SUPPORT_PERCENT_MARK = 37;  // %
var MACRO_NOT_SUPPORT_DOLLAR_MARK = 36;  //$
var MACRO_NOT_SUPPORT_WELL_MARK = 35;    //#
var MACRO_NOT_SUPPORT_EXCALMATORY_MARK = 33;   // !
var MACRO_NOT_SUPPORT_CHAR_46 = 46; //.
var MACRO_NOT_SUPPORT_CHAR_47 = 47; // /
var MACRO_NOT_SUPPORT_CHAR_96 = 96; // `
var MACRO_NOT_SUPPORT_CHAR_124 = 124; // |


function verifyIpv6str(str)
{ 
    var Num;
    var i,j;
    var finalAddrArray = new Array();
    var falseAddrArray = new Array();
    var addrArray = str.split(':');
    Num = addrArray.length;
    if (Num > 8 || Num < 1)
    {
        return falseAddrArray;
    }
    for (i = 0; i < Num; i++)
    {
        if ((addrArray[i].length > 4)
        || (addrArray[i].length < 1))
        {
            return falseAddrArray;
        }
        for (j = 0; j < addrArray[i].length; j++)
        {
            if ((addrArray[i].charAt(j) < '0')
            || (addrArray[i].charAt(j) > 'f')
            || ((addrArray[i].charAt(j) > '9') &&
            (addrArray[i].charAt(j) < 'a')))
            {
                return falseAddrArray;
            }
        }
        finalAddrArray[i] = '';
        for (j = 0; j < (4 - addrArray[i].length); j++)
        {
            finalAddrArray[i] += '0';
        }
        finalAddrArray[i] += addrArray[i];
    }
    return finalAddrArray;
}

 //verify IPv6 Addressfunction 
function isValidIpv6Address(address)
{
    var loweraddress = ''; 
    var addrParts = '';
    var startadd = [];
    var endadd = [];
    loweraddress = address.toLowerCase(); 
    addrParts = loweraddress.split('::'); 
    if (addrParts.length == 2)
    {
        if(addrParts[0] != ''){
            startadd = verifyIpv6str(addrParts[0]);
            if(startadd.length==0){
                return false;
            }
        }
        if(addrParts[1] != ''){ 
            endadd= verifyIpv6str(addrParts[1]); 
            if(endadd.length==0) {
                return false; 
            }
        }
        var alllen = startadd.length + endadd.length;
        if(alllen > 7) {
            return false; 
        }
    }else if(addrParts.length == 1) {
        startadd = verifyIpv6str(addrParts[0]);
        if(startadd.length != 8) {
            return false; 
        } 
    } else{ 
        return false;
    } 
    return true;
}

/*-------------------------validate perfix-----------------------*/
function CheckPrefix(Prefix,type)
{
    var IpAddress1 = Prefix.split(':');
    var IpAddress3 = Prefix;
    if ( 1 == type && '' == IpAddress3)
    {
        return true;
    }
    if ( true != isValidIpv6Address(IpAddress3) )
    {
        return false;
    }
    switch ( type )
    {
        case 0 :
            if ( 15 == parseInt(IpAddress1[0].substring(0, 1),16) )
            {
                switch ( parseInt(IpAddress1[0].substring(1, 2),16) )
                {
                    case 15 :
                        return false;
                    case 14 :
                        if ( 8 <= parseInt(IpAddress1[0].substring(2, 3),16) && 12 > parseInt(IpAddress1[0].substring(2, 3),16) )
                        {
                            return false;
                        }
                    break;
                    default:
                    break;
                }
            }
        break;
        case 1 :
            if ( 15 == parseInt(IpAddress1[0].substring(0, 1),16) )
            {
                switch ( parseInt(IpAddress1[0].substring(1, 2),16) )
                {
                    case 15 :
                        return false;
                    default:
                    break;
                }
            }
        break;
        default:
            return false;
    }
    return true;
} 

// verify IP Address
function isValidIpAddress(address){
    var addrParts = address.split('.');
    if (addrParts.length != 4) 
    {
        return false;
    }
    
    for (i = 0; i < 4; i++) 
    {
        if (isNaN(addrParts[i]) == true)//isNaN  if not a number return true,else return false
        {
            return false;
        }
        
        if (addrParts[i] == '') 
        {
            return false;
        }
        
        if (addrParts[i].indexOf(' ') != -1)//
        {
            return false;
        }
        
        if ((addrParts[i].indexOf('0') == 0) && (addrParts[i].length != 1)) 
        {
            return false;
        }
        
        if (addrParts[i].charAt(0) == '+' || addrParts[i].charAt(0) == '-')
        {
           return false;
        }
    }
    
    if ((addrParts[0] <= 0 || addrParts[0] == 127 || addrParts[0] > 223) ||
    (addrParts[1] < 0 || addrParts[1] > 255) ||
    (addrParts[2] < 0 || addrParts[2] > 255) ||
    (addrParts[3] <= 0 || addrParts[3] >= 255)) 
    {
        return false;
    }
    
    return true;
}

// validate Mac Address
function isValidMacAddress(macAddress){
    var c = '';
    var i = 0, j = 0;
    var OddVals = ["1", "3", "5", "7", "9", "b", "d", "f"];
    
    if ('ff:ff:ff:ff:ff:ff' == macAddress) 
    {
        return false;
    }
    
    var addrParts = macAddress.split(':');
    if (addrParts.length != 6) 
    {
        return false;
    }
    for (i = 0; i < 6; i++) 
    {
        if (addrParts[i].length != 2) 
        {
            return false;
        }
        
        for (j = 0; j < addrParts[i].length; j++) 
        {
            c = addrParts[i].toLowerCase().charAt(j);
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) 
            {
                return false;
            }
        }
    }
    
    c = addrParts[0].toLowerCase().charAt(1);
    
    for (i = 0; i < OddVals.length; i++) 
    {
        if (c == OddVals[i]) 
        {
            return false;
        }
    }
    
    return true;
}



function isValidSubnetMask(mask){
    var i = 0;
    var num = 0;
    var zeroBitPos = 0, oneBitPos = 0;
    var zeroBitExisted = false;
    
    if ('0.0.0.0' == mask) 
    {
        return false;
    }
    
    if ('255.255.255.255' == mask) 
    {
        return false;
    }
    
    var maskParts = mask.split('.');
    if (maskParts.length != 4) 
    {
        return false;
    }
    
    for (i = 0; i < 4; i++) 
    {
        if (true == isNaN(maskParts[i])) 
        {
            return false;
        }
        if ('' == maskParts[i]) 
        {
            return false;
        }
        if (maskParts[i].indexOf(' ') != -1) 
        {
            return false;
        }
        
        if ((0 == maskParts[i].indexOf('0')) && (maskParts[i].length != 1)) 
        {
            return false;
        }
        
        if (maskParts[i].charAt(0) == '+' || maskParts[i].charAt(0) == '-')
        {
           return false;
        }
        
        num = parseInt(maskParts[i],10);
        if (num < 0 || num > 255) 
        {
            return false;
        }
        if (true == zeroBitExisted && 0 != num) 
        {
            return false;
        }
        zeroBitPos = getLeftMostZeroBitPos(num);
        oneBitPos = getRightMostOneBitPos(num);
        if (zeroBitPos < oneBitPos) 
        {
            return false;
        }
        if (zeroBitPos < 8) 
        {
            zeroBitExisted = true;
        }
    }
    
    return true;
}

function isBroadcastOrMulticastIp(ipAddress)
{
    var ip = '';
    ip = inetAton(ipAddress);
    ip = ip >>> 0;
    if((0xffffffff == ip) || (ip >= 0xe0000000 && ip <= 0xefffffff))
    {
        return true;
    }
    return false;
}

function isPrivateIp(ipAddress)
{
    var ip = '';
    ip = inetAton(ipAddress);
    ip = ip >>> 0;
    if((ip >= 0xa0000000 && ip <= 0xa0ffffff) 
        || (ip >= 0xac100000 && ip <= 0xac1fffff) 
        || (ip >= 0xc0a80000 && ip <= 0xc0a8ffff))
    {
        return true;
    }
    return false;
}

// function for split port
function portPartsParse(_port){
    var portArray = [];
    _port = $.trim(_port);
    if (_port.indexOf('-') == -1) 
    {
        portArray = [_port, _port];
    }
    else 
    {
        _port = _port.split('-');
        portArray = [$.trim(_port[0]), $.trim(_port[1])];
    }
    return portArray;
}

// function for join port
function portJoin(startPort, endPort){
    var _port = '';
    var startPt = $.trim(startPort);
    var endPt = $.trim(endPort);
    
    if ( "" == startPt || "" == endPt || startPt == endPt ) 
    {
        _port = startPort;
    }
    else 
    {
        _port = startPort + "-" + endPort;
    }
    return _port;
}

function inetAton(a){
    var n = [];
    
    n = a.split(/\./);
    if (n.length != 4) 
    {
        return 0;
    }
    return ((n[0] << 24) | (n[1] << 16) | (n[2] << 8) | n[3]);
}

function getLeftMostZeroBitPos(num){
    var i = 0;
    var numArr = [128, 64, 32, 16, 8, 4, 2, 1];
    
    for (i = 0; i < numArr.length; i++) 
    {
        if ((num & numArr[i]) == 0) 
        {
            return i;
        }
    }
    
    return numArr.length;
}

function getRightMostOneBitPos(num){
    var i = 0;
    var numArr = [1, 2, 4, 8, 16, 32, 64, 128];
    
    for (i = 0; i < numArr.length; i++) 
    {
        if (((num & numArr[i]) >> i) == 1) 
        {
            return (numArr.length - i - 1);
        }
    }
    
    return -1;
}

function compareStartIpAndEndIp(startIp, endIp){
    var ipStart = startIp.split(".");
    var ipEnd = endIp.split(".");
    
    for (i = 0; i < 4; i++) 
    {
        if (parseInt(ipEnd[i],10) > parseInt(ipStart[i],10)) 
        {
            break;
        }
        else if (!((parseInt(ipEnd[i],10) == parseInt(ipStart[i],10)) && 3 != i)) 
        {
            return false;
        }
    }
    
    return true;
}

function isSameSubnetAddrs(ip1, ip2, mask){
    var addrParts1 = ip1.split(".");
    var addrParts2 = ip2.split(".");
    var maskParts = mask.split(".");
    for (i = 0; i < 4; i++) 
    {
        if (((Number(addrParts1[i])) & (Number(maskParts[i]))) != ((Number(addrParts2[i])) & (Number(maskParts[i])))) 
        {
            return false;
        }
    }
    return true;
}

/*begin check the ip is in the ip range, w00166520*/
function IsIpInRange(ipAddress, startip, endip)
{
    var ip = '';
    var MinIp = '';
    var MaxIp = '';
    ip = inetAton(ipAddress);
    ip = ip >>> 0;
    MinIp = inetAton(startip);
    MinIp = MinIp >>> 0;
    MaxIp = inetAton(endip);
    MaxIp = MaxIp >>> 0;
    if(ip >= MinIp && ip <= MaxIp){
        return true;
    }
    return false;
}
/*end check the ip is in the ip range, w00166520*/

function isBroadcastOrNetworkAddress(ipAddress, netmask){
    var ip = '';
    var mask = '';
    var netaddr = '';
    var broadaddr = '';
    
    ip = inetAton(ipAddress);
    mask = inetAton(netmask);
    netaddr = ip & mask;
    broadaddr = netaddr | ~ mask;
    
    if (netaddr == ip || ip == broadaddr) 
    {
        return false;
    }
    return true;
}

function isVaildSpecialPort(port, showTarget){
    var portParts = $.trim(port);
    var c = '';
    var i = 0;
    
    if ('' == portParts)
    {
        //showQtip(showTarget, firewall_hint_port_empty);
        return false;
    }
    if ((portParts.indexOf('0') == 0) && (portParts.length != 1)) 
    {
        //showQtip(showTarget, firewall_hint_port_empty);
        return false;
    }
    
    for (i = 0; i < portParts.length; i++) 
    {
        c = portParts.toLowerCase().charAt(i);
        if (!((c >= '0') && (c <= '9'))) 
        {
            //showQtip(showTarget, firewall_hint_port_empty);
            return false;
        }
    }
    
    if ((portParts < MIN_PORT_RANGE) || (portParts > MAX_PORT_RANGE)) 
    {
        //showQtip(showTarget, firewall_hint_port_number_valid_char);
        return false;
    }
    
    return true;
}

function isVaildPortForIPFilter(port, showTarget){
    var portParts = $.trim(port);    
    if ('' == portParts)
    {
        //showQtip(showTarget, firewall_hint_port_empty);
        return false;
    }
    if (port < MIN_PORT_RANGE || port > MAX_PORT_RANGE) 
    {
        //showQtip(showTarget, firewall_hint_port_number_valid_char);
        return false;
    }
    var splitPort = portPartsParse(port);
    
    var i = 0;
    for (i = 0; i < splitPort.length; i++) 
    {
        if (isNaN(splitPort[i])) 
        {
            //showQtip(showTarget, firewall_hint_port_empty);
            return false;
        }
    }
    portParts = $.trim(port);
    if (portParts.indexOf('-') == -1) 
    {
        if (!isVaildSpecialPort(portParts, showTarget)) 
        {
            return false;
        }
    }
    else 
    {
        portParts = port.split('-');
        if (portParts.length == 2) 
        {
            for (i = 0; i < 2; i++) 
            {
                if (isNaN(portParts[i]) == true) 
                {
                    //showQtip(showTarget, firewall_hint_port_empty);
                    return false;
                }
                
                if (portParts[i] == '') 
                {
                    //showQtip(showTarget, firewall_hint_port_empty);
                    return false;
                }
                
                if (!isVaildSpecialPort(portParts[i], showTarget)) 
                {
                    return false;
                }
            }
            if (parseInt(portParts[0], 10) > parseInt(portParts[1], 10)) 
            {
                //showQtip(showTarget, firewall_hint_start_greater_end_port);
                return false;
            }
        }
        else 
        {
            //showQtip(showTarget, firewall_hint_port_empty);
            return false;
        }
    }
    return true;
}

function isHexString(str){
    for (i = 0; i < str.length; i++) 
    {
        var c = str.charCodeAt(i);
        if ( !( (c >= CHARCODE_0 && c <= CHARCODE_9)
               ||(c >= CHARCODE_A && c <= CHARCODE_F)
               ||(c >= CHARCODE_a && c <= CHARCODE_f)
             )
        ) 
        {       
            return false;
        }
    }
    return true;
}

function isAsciiString(str){
    for (i = 0; i < str.length; i++) 
    {
        var c = str.charCodeAt(i);
        if (c <= 32 || c >= 127) 
        {
        
            return false;
        }
    }
    return true;
}


function checkInputChar(str){
    var i = 0;
    var char_i = '';
    var num_char_i = '';
    
    if (str == "") 
    {
        return true;
    }
    
    for (i = 0; i < str.length; i++) 
    {
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        if ((num_char_i > MACRO_SUPPORT_CHAR_MAX) || (num_char_i < MACRO_SUPPORT_CHAR_MIN)) 
        {
            return false;
        }
        else if ((MACRO_NOT_SUPPORT_CHAR_COMMA == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_QUOTATION_MARK == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_COLON == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_SEMICOLON == num_char_i) ||
        (MACRO_NOT_SUPPORT_BACKSLASH_MARK == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_38 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_37 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_43 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_39 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_60 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_62 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_63 == num_char_i)) 
        {
            return false;
        }
    }
    return true;
}

function checkInputChar_wanPasword(str){
    var i = 0;
    var char_i = '';
    var num_char_i = '';
    
    if (str == "") 
    {
        return true;
    }
    
    for (i = 0; i < str.length; i++) 
    {
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        if ((num_char_i > MACRO_SUPPORT_CHAR_MAX) || (num_char_i < MACRO_SUPPORT_CHAR_MIN)) 
        {
            return false;
        }
        else if ((MACRO_NOT_SUPPORT_CHAR_QUOTATION_MARK == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_39 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_COMMA == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_46 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_47 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_COLON == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_SEMICOLON == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_60 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_62 == num_char_i) ||
        (MACRO_NOT_SUPPORT_BACKSLASH_MARK == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_96 == num_char_i) ||
        (MACRO_NOT_SUPPORT_CHAR_124 == num_char_i)) 
        {
            return false;
        }
    }
    return true;
}

function checkInputChar_wanName(str){
    var i = 0;
    var char_i = '';
    var num_char_i = '';
    
    if (str == "") {
        return true;
    }
    
    for (i = 0; i < str.length; i++) {
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        if ((num_char_i > MACRO_SUPPORT_CHAR_MAX) || (num_char_i < MACRO_SUPPORT_CHAR_MIN)) {
            return false;
        }
    }
    return true;
}

function resolveXMLEntityReference(xmlStr)
{
    return xmlStr.replace( /(\<|\>|\&|\'|\")/g,
                          function($0, $1)
                          {
                              return{
                                  "<" : "&lt;"
                                , ">" : "&gt;"
                                , "&" : "&amp;"
                                , "'" : "&#39;"
                                , "\"": "&quot;"        
                              }[$1];
                          }
                         );
}


/******************************************************************
*   Description:        
*           Trim whitespace characters like space, tab, etc. at head or tail of string 
*   Return:
*           Trimed string.
*   Remark:
*           Calls trimWhitespace() as a member function of String.
*           
*******************************************************************/
String.prototype.trimWhitespace = function()
{
    return this.replace(/(^\s*)|(\s*$)/g,"");
};

function hasSpaceOrTabAtHead(str)
{
    if(0 == str.indexOf(" ") || 0 == str.indexOf("\t"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

function hasZeroAtHead(str)
{
    if(0 == str.indexOf("0"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

function isAsciiOrSpaceString(str){
    for (i = 0; i < str.length; i++) 
    {
        var c = str.charCodeAt(i);
        if (c < 32 || c >= 127) 
        {
        
            return false;
        }
    }
    return true;
}

function checkUserPassword(password){
    var i = 0;
    var j = 255;
    var k = 255;
    var m = 255;
    var n = 255;
    if(hasSpaceOrTabAtHead(password)||password.length<6||hasZeroAtHead(password)){
        return false;
    }
    for(i=0;i<password.length;i++){
        var word = password.charAt(i);
        if(password.charCodeAt(i)<32||password.charCodeAt(i)>=127){
            return false;
        }
        if(word>='a'&&word<='z'){
            j = 1;
        }
        else if(word>='A'&&word<='Z'){
            k = 2;
        }
        else if(word>='0'&&word<='9'){
            m = 4;
        }
        else {
            n = 8;
        }
        var add = j&k&m&n;
        if(add == 0){
            return true;
        }
    }
    return false;
}

function checkAdminPassword(password,username){
    var i = 0;
    var j = 0;
    var k = 0;
    var m = 0;
    var n = 0;
    var checkArray = "`~!@#$%^&*()-_=\+\\|[{}];:\'\",<.>/? ";

    if(hasSpaceOrTabAtHead(password)||password.length<6){
        return false;
    }
    if(!isAsciiOrSpaceString(password)){
        return false;
    }
    if(password == username){
        return false;
    }
    if(password.length == username.length)
    {
        for(i=0; i<password.length; i++){
          if(password.charAt(i) != username.charAt(password.length-i-1)){
            j = 1;
            break;
          }
        }
        if(j == 0){
            return false;
        }
        j = 0;
    }
    for(i=0;i<password.length;i++){
        var word = password.charAt(i);
        if(word>='a'&&word<='z'){
            j = 1;
        }
        else if(word>='A'&&word<='Z'){
            k = 1;
        }
        else if(word>='0'&&word<='9'){
            m = 1;
        }
        else if(checkArray.indexOf(word)!=-1){
            n = 1;
        }
        else {
        }
    }
    var add = j+k+m+n;
    if((add >= 2) && (add <= 4)){
        return true;
    }
    return false;
}
function checkPasswordStrength(password)
{
    var special = "`~!@#$%^&*()-_=\+\\|[{}];:\'\",<.>/? ";
    var count = 0;
    var pwdType1 = 0;
    var pwdType2 = 0;
    var pwdType3 = 0;
    var pwdType4 = 0;
    for(var i=0;i< password.length;i++)
    {  
        if(password.charAt(i) <= 'Z' && password.charAt(i) >= 'A')
        {
            if( 0 == pwdType1)
            {
                count +=1;
                pwdType1 = 1;
            }
            continue;
        }
        if(password.charAt(i) <= 'z' && password.charAt(i) >= 'a')
        {
            if( 0 == pwdType2)
            {
                count +=1;
                pwdType2 = 1;
            }
            continue;
        }
        if(password.charAt(i) <= '9' && password.charAt(i) >= '0')
        {
            if( 0 == pwdType3)
            {
                count +=1;
                pwdType3 = 1;
            }
            continue;
        }
        if(special.indexOf(password.charAt(i)) >=0 )
        {
            if( 0 == pwdType4)
            {
                count +=1;
                pwdType4 = 1;
            }
            continue;
        }
   
    }
    return count;
}

function checkWirelesPassword(password){
    var i = 0;
    var j = 255;
    var k = 255;
    var m = 255;
    var n = 255;
    var checkArray = 'LlIiOo';
    if(hasSpaceOrTabAtHead(password)||password.length<6||hasZeroAtHead(password)){
        return false;
    }
    if(!isAsciiOrSpaceString(password)){
        return false;
    }
    for(i=0;i<password.length;i++){
        var word = password.charAt(i);
        if(checkArray.indexOf(word)!=-1){
            return false;
        }
        if(word>='a'&&word<='z'){
            j = 1;
        }
        else if(word>='A'&&word<='Z'){
            k = 2;
        }
        else if(word>='0'&&word<='9'){
            if(i>0){
                if(word == password.charAt(i-1)){
                    return false;
                }
            }
            m = 4;
        }
        else {
            n = 8;
        }
    }
    var add = j&k&m&n;
    if(add == 0){
        return true;
    }
    return false;
}

function isInteger(value)
{   
    if (/^(\+|-)?\d+$/.test(value)) 
    {
          return true;
    } 
    else 
    {
        return false;
    }
}

function isPlusInteger(value)
{
    if (isInteger(value) && parseInt(value) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

function isIpFormat(str)
{
    var addrParts = str.split('.');
    if (addrParts.length != 4 ) 
        return false;

    for(var i=0;i<addrParts.length;i++)
    {
        if (isPlusInteger(addrParts[i]) == false) 
        {
            return false;
        }
    }

    return true;
}
/*add for support ajax by lijiechun*/
function isValidAscii(val)
{
    for ( var i = 0 ; i < val.length ; i++ )
    {
        var ch = val.charAt(i);
        if ( ch < ' ' || ch > '~' )
        {
            return ch;
        }
    }
    return '';
}
function checkValidApn(apn)
{
    var i = 0;
    
    for (i=0;i<apn.length;i++)
    {
        var word = apn.charAt(i);
        
        if (word=='.' || word=='-')
        {
        }
        else if (word>='a'&&word<='z')
        {        
        }
        else if (word>='A'&&word<='Z')
        {    
        }
        else if (word>='0'&&word<='9')
        {   
        }
        else 
        {
            return false;
        }
    }
    return true;
}
function formatIpv6address(ipaddress)
{
    if(ipaddress.indexOf("::") > -1 )
    {
        if(ipaddress.indexOf("::") == 0)
        {
            if("::" == ipaddress)
            {
                ipaddress = "0:0:0:0:0:0:0:0";
                return ipaddress;
            }
            else
            {
                var i = 0;
                var count_colon = 0;
                var length_ip = ipaddress.length;
                for(i = 0; i < length_ip; i++)
                {
                    if(":" == ipaddress.charAt(i))
                    {
                        count_colon ++;
                    }
                }
                var new_colon_str = "";
                if(7 == count_colon)
                {
                    new_colon_str = "0:0:";
                }
                else if(6 == count_colon)
                {
                    new_colon_str = "0:0:0:";
                }
                else if(5 == count_colon)
                {
                    new_colon_str = "0:0:0:0:";
                }
                else if(4 == count_colon)
                {
                    new_colon_str = "0:0:0:0:0:";
                }
                else if(3 == count_colon)
                {
                    new_colon_str = "0:0:0:0:0:0:";
                }
                else if(2 == count_colon)
                {
                    new_colon_str = "0:0:0:0:0:0:0:";
                }
                return ipaddress.replace("::", new_colon_str);
            }
        }
        else
        {
            var i = 0;
            var count_colon = 0;
            var length_ip = ipaddress.length;
            for(i = 0; i < length_ip; i++)
            {
                if(":" == ipaddress.charAt(i))
                {
                    count_colon ++;
                }
            }
            var new_colon_str = "";
            if(7 == count_colon)
            {
                new_colon_str = ":0:";
            }
            else if(6 == count_colon)
            {
                new_colon_str = ":0:0:";
            }
            else if(5 == count_colon)
            {
                new_colon_str = ":0:0:0:";
            }
            else if(4 == count_colon)
            {
                new_colon_str = ":0:0:0:0:";
            }
            else if(3 == count_colon)
            {
                new_colon_str = ":0:0:0:0:0:";
            }
            else if(2 == count_colon)
            {
                new_colon_str = ":0:0:0:0:0:0:";
            }
            return ipaddress.replace("::", new_colon_str);
        }
    }
    else
    {
        return ipaddress;
    }
}

function isSameipv6Prefix(ipv61,ipv62,Prefix1)
{  
    if(ipv61==''||ipv62=='')
    {
        return false;
    }
    if(Prefix1<=0||Prefix1>128)
    {
        return false;
    }
    var bret = true;
    var address_ipv6_1 = formatIpv6address(ipv61);
    var address_ipv6_2 = formatIpv6address(ipv62);
    var i = 0;
    
    var ipv6_array_1 = address_ipv6_1.split(":");
    var ipv6_array_2 = address_ipv6_2.split(":");
    var ipv6_value_1 = 0;
    var ipv6_value_2 = 0;

    for(i = 0; i < Prefix1/16; i++)
    {
        ipv6_value_1 = parseInt(ipv6_array_1[i], 16);
        ipv6_value_2 = parseInt(ipv6_array_2[i], 16);
        if(ipv6_value_1 != ipv6_value_2)
        {
            bret = false;
            break;
        }
    }
    if ((bret==true) && (Prefix1%16!=0))
    {
        var temp = '';
        for(i = 0; i < 16; i++)
        {    
            if(i<Prefix1%16)
            {
                temp = temp+'1';
            }
            else
            {
                temp = temp+'0';
            }            
        }
        var a = parseInt(ipv6_array_1[Math.floor(Prefix1/16)], 16);
        var b = parseInt(ipv6_array_2[Math.floor(Prefix1/16)], 16);
        var compareobj = parseInt(temp, 2);
        if (a&compareobj!=b&compareobj)
        {
            bret = false;
        }
    }
    return bret;
}
function filterWanPathName(WanPathName)
{
     return WanPathName.replace("VOICE_TR069_", "");
}


