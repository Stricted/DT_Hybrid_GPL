#ifndef __ATP_BHAL_MSGTYPES_H__
#define __ATP_BHAL_MSGTYPES_H__



/*�ں˺��û�̬ͨ�ŵ�netlinkЭ������*/




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

