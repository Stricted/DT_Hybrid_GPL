var g_key = "gatewaylte";

function addImeiIntoKey() {
    getAjaxData("api/device/information",function($xml) {
        var ret = xml2object($xml);
        if(typeof(ret.response) != "undefined") {
            if(ret.response.Imei==''||ret.response.Imei=='Unknown')
            {
                g_key = g_key + '000000';
            }
            else
            {
                g_key = g_key + ret.response.Imei.substring(ret.response.Imei.length-6,ret.response.Imei.length);
            }           
        }
    },{
        sync : true
    });
}

addImeiIntoKey(); 
/*---------------S-box:加密运算的S盒--------------------*/
var sBox =
[   /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */ 
    99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,
    202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,
    183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,
    4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,
    9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,
    83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,
    208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,
    81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,
    205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,
    96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,
    224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,
    231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,
    186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,
    112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,
    225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,
    140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22
];

/*-----------------invBOX:解密运算的S盒---------------------------*/
var invsBox = 
[/*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */  
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb, /*0*/ 
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb, /*1*/
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e, /*2*/ 
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25, /*3*/ 
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92, /*4*/ 
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84, /*5*/ 
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06, /*6*/ 
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b, /*7*/
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73, /*8*/ 
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e, /*9*/
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b, /*a*/
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4, /*b*/ 
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f, /*c*/ 
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef, /*d*/ 
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61, /*e*/ 
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d  /*f*/
];

//密钥扩展运算
function KeyExpansion(key) {
    var i = 0;
    var j = 0;
    var k = 0;
    var c = 0;
    var r = 0;
    var rc = [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36];
    var w = [];

    for(i=0;i<11;i++){
        var w_r = [];
        for(j=0;j<4;j++) {
            var w_d = [];
            for(k=0;k<4;k++) {
                w_d[k] = 0;
            }
            w_r[j] = w_d;
        }
        w[i] = w_r;
    }
        
    for(r=0; r<4; r++)
    {
        var w_c = [];
        for(c=0; c<4; c++)
        {
            w[0][r][c] = key[r+c*4];
        }
    }
    
    for(i=1; i<=10; i++)
    {
        for(j=0; j<4; j++)
        {
            var t = [];
            for(r=0; r<4; r++)
            {
                t[r] = j ? w[i][r][j-1] : w[i-1][r][3];
            }
            if(j == 0)
            {
                var temp = t[0];
                for(r=0; r<3; r++)
                {
                    t[r] = sBox[t[(r+1)%4]];
                }
                t[3] = sBox[temp];
                t[0] ^= rc[i-1];
            }
            for(r=0; r<4; r++)
            {
                w[i][r][j] = w[i-1][r][j] ^ t[r];
            }    
        }
    }
    return w;
}

//轮密钥加运算
function AddRoundKey(state,k) {
    var c = 0;
    var r = 0;
    for(c=0; c<4; c++)
    {
        for(r=0; r<4; r++)
        {
            state[r][c] ^= k[r][c];
        }
    }
    return state;
}

//S盒运算
function SubBytes(state) {
    var c = 0;
    var r = 0;
    for(r=0; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            state[r][c] = sBox[state[r][c]];
        }
    }
    return state;
}
//S盒逆运算
function InvSubBytes(state)
{
    var c = 0;
    var r = 0;
    for(r=0; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            state[r][c] = invsBox[state[r][c]];
        }
    }
    return state;
}

//行移位变换运算
function ShiftRows(state) {
    var t = [];
    var r = 0;
    var c = 0;
    for(r=1; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            t[c] = state[r][(c+r)%4];
        }
        for(c=0; c<4; c++)
        {
            state[r][c] = t[c];
        }
    }
    return state;
}
//行移位变换逆运算
function InvShiftRows(state)
{
    var t = [];
    var c = 0;
    var r = 0;
    for(r=1; r<4; r++)
    {
        for(c=0; c<4; c++)
        {
            t[c] = state[r][(c-r+4)%4];
        }
        for(c=0; c<4; c++)
        {
            state[r][c] = t[c];
        }
    }
    return state;
}

//列混淆运算
function FFmul(a,b) {
    var bw = [];
    var res = 0;
    var i =0;
    bw[0] = b;
    for(i=1; i<4; i++)
    {
        bw[i] = (bw[i-1]<<1)%256;
        if(bw[i-1]&0x80)
        {
            bw[i]^=0x1b;
        }
    }
    for(i=0; i<4; i++)
    {
        if((a>>i)&0x01)
        {
            res ^= bw[i];
        }
    }
    return res;
}

function MixColumns(state) {
    var t = [];
    var c = 0;
    var r = 0;
    for(c=0; c< 4; c++)
    {
        for(r=0; r<4; r++)
        {
            t[r] = state[r][c];
        }
        for(r=0; r<4; r++)
        {
            state[r][c] = FFmul(0x02, t[r])
                        ^ FFmul(0x03, t[(r+1)%4])
                        ^ FFmul(0x01, t[(r+2)%4])
                        ^ FFmul(0x01, t[(r+3)%4]);
        }
    }
    return state;
}

function InvMixColumns(state)
{
    var t = [0,0,0,0];
    var r,c;
    for(c=0; c< 4; c++)
    {
        for(r=0; r<4; r++)
        {
            t[r] = state[r][c];
        }
        for(r=0; r<4; r++)
        {
            state[r][c] = FFmul(0x0e, t[r])
                        ^ FFmul(0x0b, t[(r+1)%4])
                        ^ FFmul(0x0d, t[(r+2)%4])
                        ^ FFmul(0x09, t[(r+3)%4]);
        }
    }
    return state;
}

//处理密钥不是16位数据的情况
function setKey() {
    var temp_length = g_key.length;
    var temp_key = [];
    var i = 0;
    if(16 <= temp_length)
    {
        for(i=0;i<16;i++) {
            temp_key[i] = g_key.charCodeAt(i);
        }
    }
    else
    {
        for(i=0;i<temp_length;i++) {
            temp_key[i] = g_key.charCodeAt(i);
        }
        for(i=temp_length;i<16;i++) {
            temp_key[i] = 0x00;
        }
    }
    return KeyExpansion(temp_key);
}

//加密算法
function encrypt(input) {
    if(input=='')
    {
        return input;
    }
    var w = setKey();
    var end_temp = "";
    var length_num = 0;
    var m = 0;
    var j = 0;
    var k = 0;
    var c = 0;
    var r = 0;
    var i = 0;
    if(input.length % 16 == 0) {
        length_num = input.length / 16;
    }
    else {
        length_num = parseInt(input.length/16,10) + 1;
    }
    
    
    for(m = 1;m <= length_num;m++) {
        var temp_input = "";
        if(m != 1) {
            temp_input = input.substring((m-1)*16,m*16);
        }
        else {
            temp_input = input;
        }

        var temp = [];
        for(j=0;j<16;j++) {
            if(typeof(temp_input.charCodeAt(j)) == "undefined" || isNaN(temp_input.charCodeAt(j))){
                temp[j] = 0x00;
            }
            else {
                temp[j] = temp_input.charCodeAt(j);
            }
            
        }

        var state = [];    
            for(j=0;j<4;j++) {
                var state_c = [];
                for(k=0;k<4;k++) {
                    state_c[k] = 0;
                }
                state[j] = state_c;
            }
            
        for(r=0; r<4; r++)
        {
            for(c=0; c<4 ;c++)
            {
                state[r][c] = temp[c*4+r];
            }
        }
        var add_state = AddRoundKey(state,w[0]);
        
        for(i=1;i<=10;i++) {
            var sub_state = SubBytes(add_state);
            add_state = ShiftRows(sub_state);
            
            if(i!=10) {
                add_state = MixColumns(add_state);
            }
            add_state = AddRoundKey(add_state,w[i]);
        }
        
        for(r=0; r<4; r++)
        {
            for(c=0; c<4 ;c++)
            {
                temp[c*4+r] = add_state[r][c];
            }
        }
        
        for(i=0;i<temp.length;i++) {
            if(temp[i] <= 15) {
                end_temp = end_temp + '0' + temp[i].toString(16);
            }
            else {
                end_temp += temp[i].toString(16);
            }
        }
    }
    return end_temp.toUpperCase();
}

function StringToNumber(word){
    var input_arr = [];
    var i = 0;
    for(i=0;i<word.length;i+=2){
        var str = '0x' + word.charAt(i) + word.charAt(i+1);
        input_arr.push(parseInt(str,16));
    }
    return input_arr;
}
function decrypt_part(input)
{
    input = StringToNumber(input);
    var w = setKey();
    var state = [];
    var i,r,c;

    for(r=0; r<4; r++)
    {
        var temp = [];
        for(c=0; c<4 ;c++)
        {
            temp[c] = input[c*4+r];
        }
        state[r] = temp;
    }

    state = AddRoundKey(state, w[10]);
    for(i=9; i>=0; i--)
    {
        state = InvShiftRows(state);
        state = InvSubBytes(state);
        state = AddRoundKey(state, w[i]);
        if(i)
        {
            state = InvMixColumns(state);
        }
    }
    var retstrlen = 0;
    for(r=0; r<4; r++)
    {
        for(c=0; c<4 ;c++)
        {
            if(state[r][c]!=0){
                input[c*4+r] = String.fromCharCode(state[r][c]);
                retstrlen++;
            } else {
                input[c*4+r] = state[r][c];
            }
        }
    }

    var word = '';
    $(input).each(function(k){
        //if(0 != input[k]){
            word += input[k];
        //}
    });
    return word.substring(0, retstrlen);
}
//解密算法
function decrypt(input){
    if(input=='')
    {
        return input;
    }  
    var i=0;
    var result='';
    var input_part='';
    for(i=0;i<input.length/32;i++)
    {  
       input_part = input.substring(32*i,32*i+32);
       result = result + decrypt_part(input_part);     
    } 
    return result;
}
