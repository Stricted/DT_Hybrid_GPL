var hexchars = "0123456789abcdef";
var k = [];
var w = [];
var H0 = "";
var H1 = "";
var H2 = "";
var H3 = "";
var H4 = "";
var H5 = "";
var H6 = "";
var H7 = "";
var K = [ "428a2f98","71374491","b5c0fbcf","e9b5dba5","3956c25b","59f111f1","923f82a4","ab1c5ed5",
         "d807aa98","12835b01","243185be","550c7dc3","72be5d74","80deb1fe","9bdc06a7","c19bf174",
         "e49b69c1","efbe4786","0fc19dc6","240ca1cc","2de92c6f","4a7484aa","5cb0a9dc","76f988da",
         "983e5152","a831c66d","b00327c8","bf597fc7","c6e00bf3","d5a79147","06ca6351","14292967",
         "27b70a85","2e1b2138","4d2c6dfc","53380d13","650a7354","766a0abb","81c2c92e","92722c85",
         "a2bfe8a1","a81a664b","c24b8b70","c76c51a3","d192e819","d6990624","f40e3585","106aa070",
         "19a4c116","1e376c08","2748774c","34b0bcb5","391c0cb3","4ed8aa4a","5b9cca4f","682e6ff3",
         "748f82ee","78a5636f","84c87814","8cc70208","90befffa","a4506ceb","bef9a3f7","c67178f2"];

function hexToBinary(str) {
    var st = "";
    var str2 = "";
    var i = 0;
    for(i = 0;i < str.length; i++) {
        switch(str.charAt(i)) {
            case '0':
                st = "0000";
                break;
            case '1':
                st = "0001";
                break;
            case '2':
                st = "0010";
                break;
            case '3':
                st = "0011";
                break;
            case '4':
                st = "0100";
                break;
            case '5':
                st = "0101";
                break;
            case '6':
                st = "0110";
                break;
            case '7':
                st = "0111";
                break;
            case '8':
                st = "1000";
                break;
            case '9':
                st = "1001";
                break;
            case 'a':
                st = "1010";
                break;
            case 'b':
                st = "1011";
                break;
            case 'c':
                st = "1100";
                break;
            case 'd':
                st = "1101";
                break;
            case 'e':
                st = "1110";
                break;
            case 'f':
                st = "1111";
                break;
            default :
                break;
        }
        str2 += st;
    }
    return str2;
}

//二进制到十六进制
function binaryToHex(str) { 
    var out = ""; 
    len = str.length; 
    num = parseInt(len/4, 10); 
    switch(len%4) { 
        case 1: str = "000" + str; 
            num ++; 
            break; 
        case 2: str = "00" + str; 
            num ++; 
            break; 
        case 3: str = "00" + str; 
            num ++; 
            break; 
        default: 
        break; 
    } 
    var i = 0; 
    while(i < num) { 
        var str_4 = str.substring(4*i, 4*(i+1)); 
        var int_4 = parseInt(str_4.charAt(0), 10) *8 + parseInt(str_4.charAt(1), 10) *4 + parseInt(str_4.charAt(2), 10) *2 + parseInt(str_4.charAt(3), 10); 
        out += hexchars.charAt(int_4); 
        i++; 
    } 
    return out;
}

 //从字符串(ASCII)到二进制
function stringToBinary(str) {
    var str2 = "";
    var i =0;
    for(i = 0;i < str.length;i++) {
        str2 += fill_zero(str.charCodeAt(i).toString(2),8);
    }
    return str2;
}

//按位填充函数
function fill_zero(str,n) {
    var str1 = "";
    var i = 0;
    if(str.length < n) {
        for(i=0;i < n-str.length;i++) {
            str1 += "0";
        }
    }
    return str1+str;//Integer.toHexString(Integer.valueOf(str2+s,2))+" ";//换成十六进制;
}

//按位异或
function bit_df_or(str1,str2) {
    var str = "";
    var i = 0;
    for(i=0;i<str1.length;i++) {
        if(str1.charAt(i)==str2.charAt(i)) {
            str += "0";
        } else {
            str += "1";
        }
    }
    return str;
}
//按位同或
function bit_sa_or(str1,str2) {
    var str = "";
    var i = 0;
    for(i=0;i<str1.length;i++) {
        if(str1.charAt(i)==str2.charAt(i)) {
            str += "1";
        } else {
            str += "0";
        }           
    }
    return str;
}
//按位与
function bit_and(str1,str2) {
    var str = "";
    var i = 0;
    for(i=0;i<str1.length;i++) {
        if(str1.charAt(i)=='0'||str2.charAt(i)=='0') {
            str += "0";
        } else {
            str += "1"; 
        }            
    }
    return str;
}
//按位或
function bit_or(str1,str2) {
    var str = "";
    var i = 0;
    for(i = 0;i<str1.length;i++) {
        if(str1.charAt(i)=='1'||str2.charAt(i)=='1') {
            str += "1";
        } else {
            str += "0";
        }            
    }
    return str;
}
//按位非
function bit_non(str1) {
    var str = "";
    var i = 0;
    for(i=0;i<str1.length;i++) {
        if(str1.charAt(i)=='0') {
            str += "1";
        } else {
            str += "0";
        }          
    }
    return str;
}
//按位循环左移n位
function rotl(str,n) {
    var str1 = str.substring(0,n);
    var str2 = str.substring(n);
    return str2+str1;
}
//按位循环右移n位
function rotr(str,n) {
    var str1 = str.substring(str.length-n);
    var str2 = str.substring(0,str.length-n);
    return str1+str2;
}
//按位右移n位
function shr(str,n) {
    var fillZero = [];
    var str2 = "";
    var i = 0;
    for(i = 0;i < n; i++) {
        fillZero[i]  = "0";
        str2 += fillZero[i];
    }
    var str1 = str.substring(0,str.length-n);
    return str2+str1;
}

function ch(str1,str2,str3) {
    return bit_df_or(bit_and(str1,str2),bit_and(bit_non(str1),str3));
}
    
function maj(str1,str2,str3) {
    return bit_df_or(bit_df_or(bit_and(str1,str2),bit_and(str1,str3)),bit_and(str2,str3));
}
    
function big_sigma_zero(str1) {
    return bit_df_or(bit_df_or(rotr(str1,2),rotr(str1,13)),rotr(str1,22));
}
    
function big_sigma_one(str1) {
    return bit_df_or(bit_df_or(rotr(str1,6),rotr(str1,11)),rotr(str1,25));
}

function small_sigma_zero(str1) {
    return bit_df_or(bit_df_or(rotr(str1,7),rotr(str1,18)),shr(str1,3));
}

function small_sigma_one(str1) {
    return bit_df_or(bit_df_or(rotr(str1,17),rotr(str1,19)),shr(str1,10));
}

//按位相加
function binaryplus(str1,str2) {
    var cstr = "";
    var cstr_str = "";
    var flag = 0;
    var i = 0;
    for(i = str1.length-1;i>=0;i--) {
        cstr =((str1.charAt(i)-'0')+((str2.charAt(i)-'0'))+flag)%2;
        if(((str1.charAt(i)-'0')+(str2.charAt(i)-'0')+flag)>=2) {
            flag = 1;
        }
        else {
            flag = 0;
        }
        cstr_str = cstr + cstr_str;
    }
    return cstr_str;
}

//计算T1
function T1(str_h,str_e,str_ch,str_w,str_k) {
    return binaryplus(binaryplus(binaryplus(str_h,big_sigma_one(str_e)),binaryplus(str_ch,str_w)),str_k);
}
//计算T2
function T2(str_a,str_maj) {
    return binaryplus(big_sigma_zero(str_a),str_maj);
}

function calculate_sha_256(A,B,C,D,E,F,G,H) {
    var temp1 = "";
    var temp2 = "";
    var sha_str = "";
    var i = 0;
    for(i = 0;i<64;i++) {
        temp1=T1(H,E,ch(E,F,G),w[i],k[i]);
        temp2=binaryplus(temp1,T2(A,maj(A,B,C)));
        H = G;
        G = F;
        F = E;
        E = binaryplus(D,temp1);
        D = C;
        C = B;
        B = A;
        A = temp2;
    }
    H0 = binaryToHex(binaryplus(A,hexToBinary(H0)));
    H1 = binaryToHex(binaryplus(B,hexToBinary(H1)));
    H2 = binaryToHex(binaryplus(C,hexToBinary(H2)));
    H3 = binaryToHex(binaryplus(D,hexToBinary(H3)));
    H4 = binaryToHex(binaryplus(E,hexToBinary(H4)));
    H5 = binaryToHex(binaryplus(F,hexToBinary(H5)));
    H6 = binaryToHex(binaryplus(G,hexToBinary(H6)));
    H7 = binaryToHex(binaryplus(H,hexToBinary(H7)));
}

function stringToSHA256(str) {
    H0 = "6a09e667";
    H1 = "bb67ae85";
    H2 = "3c6ef372";
    H3 = "a54ff53a";
    H4 = "510e527f";
    H5 = "9b05688c";
    H6 = "1f83d9ab";
    H7 = "5be0cd19";
    var msg_binary = "";
    var msg_str2 = "";
    var A = "";
    var B = "";
    var C = "";
    var D = "";
    var E = "";
    var F = "";
    var G = "";
    var H = "";
    var i = 0;
    var n = 0;
    var group_num = 1;//组数
    var mod = 0;//最后一组的原始长度
    for (i = 0; i < 64; i++) {
        k[i] = hexToBinary(K[i]);
    }
    msg_binary = stringToBinary(str);
    var SIZE = msg_binary.length;
    mod = SIZE%512;

    if(mod < 448) {
        group_num = parseInt(SIZE/512, 10) + 1;
    }
    else{
        group_num = parseInt(SIZE/512, 10) + 2;
    }
     
    var char_group = [];
    for(i = 0;i < SIZE;i++) {//填充位数
       char_group[i] = msg_binary.charAt(i);
    }
     
     //将SIZE转为2进制，得到结果
    var size_str = SIZE.toString(2);

    char_group[SIZE] = '1';
    for(i = SIZE+1;i<512*group_num-size_str.length;i++) {
        char_group[i] = '0';
    }
    for(i = 512*group_num-size_str.length;i<512*group_num;i++) {
        char_group[i] = size_str.charAt(i-512*group_num+size_str.length);
    }

    for(i = 0;i<512*group_num;i++) {
        msg_str2 += char_group[i];
    }
    for(n = 0;n<group_num;n++){
        var str3 = "";//存放每一个分组的512位
        str3 = msg_str2.substring(n*512,(n+1)*512);//取每一组的512为
        for(i = 0;i<16;i++) {
            w[i] = str3.substring(i*32,(i+1)*32);
        }
        for(i = 16;i<64;i++) {
            var a = binaryplus(small_sigma_one(w[i-2]),w[i-7]);
            var b = binaryplus(small_sigma_zero(w[i-15]),w[i-16]);
            w[i] = binaryplus(a,b); 
        }

        A = hexToBinary(H0);
        B = hexToBinary(H1);
        C = hexToBinary(H2);
        D = hexToBinary(H3);
        E = hexToBinary(H4);
        F = hexToBinary(H5);
        G = hexToBinary(H6);
        H = hexToBinary(H7);
        
        calculate_sha_256(A,B,C,D,E,F,G,H);
    }
    return H0+H1+H2+H3+H4+H5+H6+H7;
}
