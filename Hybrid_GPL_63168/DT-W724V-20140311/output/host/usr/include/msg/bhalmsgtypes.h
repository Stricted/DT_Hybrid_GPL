#ifndef __ATP_BHAL_MSGTYPES_H__
#define __ATP_BHAL_MSGTYPES_H__



/*内核和用户态通信的netlink协议类型*/




typedef struct tBhalIoctParms
{
    char *pcString;
    char *pcBuf;
    unsigned int   ulType;
    unsigned int   ulStrLen;
    unsigned int   ulOffset;  
    int   lResult; 
} tBHAL_IOCTL_PARMS_ST;




#endif // End of __ATP_BHAL_MSGTYPES_H__

