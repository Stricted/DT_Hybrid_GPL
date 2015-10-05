/* test vectors from p1ovect1.txt */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#define VOS_OK                 (0)
#define VOS_NOK                 (1)
#define VOS_ERROR              (-1)
#define VOS_TRUE               1L 
#define VOS_FALSE              0L

typedef void                    VOS_VOID;

typedef unsigned char           VOS_UINT8;
typedef char                    VOS_INT8;
typedef char                    VOS_CHAR;

typedef unsigned short          VOS_UINT16;
typedef signed short            VOS_INT16;

typedef unsigned int           VOS_UINT32;
typedef signed int             VOS_INT32;
typedef unsigned int           VOS_BOOL;

int RSA_Encrypt(char *pszSource_file, char *pszObject_file, char *pszKeyfile);


VOS_UINT32 RSA_read_publickey(char *pszSource_file, RSA  **ppRsa);



int RSA_Encrypt(char *pszSource_file, char *pszObject_file, char *pszKeyfile)
{
	FILE *pfPlain = NULL, *pfCipher = NULL;
	VOS_CHAR *szPlainBlock = NULL;
	VOS_CHAR *szCipherBlock = NULL;
	RSA  *pRsa = NULL;
	VOS_INT32  iret = 0;
	VOS_UINT32 flen = 0;
	VOS_INT32 icount = 0;
	VOS_UINT32  ulRet = VOS_OK;
	
	if(!pszSource_file || !pszObject_file || !pszKeyfile)
	{
		return 0;	
	}
    if((pfPlain = fopen(pszSource_file, "rb")) == NULL)
    {
        printf ("==============\nopen file [ %s ] error!\n==============\n", pszSource_file);
        return 0;
    }
    if((pfCipher = fopen(pszObject_file, "wb")) == NULL)
    {
        printf ("==============\nopen file [ %s ] error!\n==============\n", pszObject_file);
        return 0;
    }
    //pRsa = RSA_new();
    /* 读取密钥 */
    
    if(VOS_OK != RSA_read_publickey(pszKeyfile, &pRsa))    
    {
    	printf("key error! \n");
    	return 0;	
    }
    

	flen = RSA_size(pRsa);	
	szPlainBlock = (char*)malloc(flen + 1);
	szCipherBlock = (char*)malloc(flen + 1);

	if(!szPlainBlock || !szCipherBlock)
	{
		if(szPlainBlock)
		{
			free(szPlainBlock);
			szPlainBlock = NULL;
		}
		if(szCipherBlock)
		{
			free(szCipherBlock);
			szCipherBlock = NULL;
		}	
	    fclose(pfPlain);
	    fclose(pfCipher);	
		RSA_free(pRsa);
		return VOS_NOK;
	}

	memset(szPlainBlock, 0, flen + 1);
	memset(szCipherBlock, 0, flen + 1);
    
    
    while(!feof(pfPlain))
    {
        /* 每次读flen个字节，并返回成功读取的字节数 */
        if((icount = fread(szPlainBlock, sizeof(char), flen, pfPlain)) == flen)
        {
            iret = RSA_public_encrypt(flen, szPlainBlock, szCipherBlock, pRsa, RSA_NO_PADDING);
            if (iret < 0)
            {            	
				printf("iret = [ %d ]\n", iret);
                printf("Encrypt failed! \n");
				ulRet = VOS_NOK;
				goto RSAEN_END;				
            }
            fwrite(szCipherBlock, sizeof(char), flen, pfCipher);
        }
    }

    if(icount)
    {
        /* 填充 */
        memset(szPlainBlock + icount, '\0', flen - icount);
        /* 最后一个字符保存包括最后一个字符在内的所填充的字符数量 */
        szPlainBlock[flen - 1] = flen - icount;
        iret = RSA_public_encrypt(flen, szPlainBlock, szCipherBlock, pRsa, RSA_NO_PADDING);
        if (iret < 0)
        {   
            printf("Encrypt failed! \n");
			ulRet = VOS_NOK;
			goto RSAEN_END;			
        }
        fwrite(szCipherBlock, sizeof(char), flen, pfCipher);
    }

	ulRet = VOS_OK;
	
RSAEN_END:	
    free(szPlainBlock);
    free(szCipherBlock);
    
    szPlainBlock=NULL;
    szCipherBlock=NULL;
    
    fclose(pfPlain);
    fclose(pfCipher);

    RSA_free(pRsa);
        
    return ulRet;

}




VOS_UINT32 RSA_read_publickey(char *pszSource_file, RSA  **ppRsa)
{
	FILE *pfkey;
	
	if(NULL == pszSource_file || NULL == ppRsa)
	{
		printf("RSA_read_publickey null\r\n");
		return VOS_NOK;	
	}
	if((pfkey = fopen(pszSource_file, "rb")) == NULL)
	{	    
		printf("RSA_read_publickey rb err\r\n");	
	    return VOS_NOK;
	}

	*ppRsa = PEM_read_RSAPublicKey(pfkey, NULL, NULL, NULL);

	fclose(pfkey);
	if(NULL == *ppRsa)	
	{
		printf("get public key null");
		return VOS_NOK;
	}
	
    return VOS_OK;
}

int main(int argc, char *argv[])
{

    char szflag[32] ={0};
    char szSource_file[256] = {0};
    char szObject_file[256] = {0};
    char szKey_file[256] = {0};    
    char *acAfEn;
    int  iLen;

   
    if(argc != 5)
    {
        printf("\nUsage: makeDES <flag> <Source file> <Object file> <key>\n");
        printf("flag        -- 标识，为1时进行加密操作，为2时进行解密操作\n");
        printf("Source file -- 源文件\n");
        printf("Object file -- 目标文件\n");
        return 0;
    }


    snprintf(szflag, sizeof(szflag), "%s", argv[1]);
    snprintf(szSource_file, sizeof(szSource_file), "%s", argv[2]);
    snprintf(szObject_file, sizeof(szObject_file), "%s", argv[3]);
    snprintf(szKey_file, sizeof(szKey_file), "%s", argv[4]);    

    if(strcmp(szflag, "1") != 0)
    {
        return 0;
    }
 
    //printf("Source file: %s\n",szSource_file);
    //printf("Object file: %s\n",szObject_file);

    if(strcmp(szflag, "1") == 0)
    {
        RSA_Encrypt(szSource_file, szObject_file, szKey_file);
    }


    return 1;
}
/*end of this file*/