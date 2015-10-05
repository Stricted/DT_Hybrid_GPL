/***************************************************************************
 * File Name  : flash_api.h
 *
 * Description: This file contains definitions and prototypes for a public
 *              flash device interface and an internal flash device interface.
 ***************************************************************************/

#if !defined(_ATP_FLASH_H)
#define _ATP_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "atpflashlayout.h"

/* Flash storage address information that is determined by the flash driver. */
typedef struct flashaddrinfo
{
    unsigned long ulFlashAreaStartOffset;
    unsigned long ulFlashAreaLen;    
}ATP_FLASH_ADDR_INFO_ST;

/********************* Public Interface Prototypes. ***************************/
void ATP_FLASH_InitFlash(void);
//void ATP_FLASH_GetAddrInfo(ATP_FLASH_ADDR_INFO_ST * pstFlashAddrInfo);

/*we change "BSP_E_FLASH_AREA" to "unsigned long" so we didn't need use "bhal.h"*/

int ATP_FLASH_ReadFlashArea(unsigned long enCfgAreaType,void *string,unsigned long strLen, unsigned long offset);
int ATP_FLASH_WriteFlashArea(unsigned long enCfgAreaType,void *string,unsigned long strLen, unsigned long offset);

//int ATP_FLASH_ClearConfigFile(void);
//int ATP_FLASH_SetImage(int flash_start_addr, char * string, int size);
int ATP_FLASH_GetSize(void);
int ATP_FLASH_GetBlockSize(unsigned long ulSrcAddr);

//unsigned long ATP_FLASH_ReadFlash(void * toaddr, unsigned long fromaddr, unsigned long len);
int ATP_FLASH_EraseFlash(unsigned long ulFlashAddr, unsigned long ulLen);

int ATP_FLASH_ReadFlash(unsigned long ulFlashAddr,void *pszDataBuffer,unsigned long ulLen);
int ATP_FLASH_WriteFlash(unsigned long ulFlashAddr, const void * pDataBuffer, unsigned long ulLen);
/*Start of Change by l63336 for NANDFLASH 2010-8-30*/
#ifdef SUPPORT_ATP_NANDFLASH
//int ATP_FLASH_EraseFlashArea(unsigned long enCfgAreaType);
int ATP_FLASHCBB_EraseBlock(unsigned long ulAddress, unsigned long ulLen);
int ATP_FLASH_ReadFlash_CountError(unsigned long ulFlashAddr,void *pszDataBuffer, unsigned long ulLen,unsigned long *ulCount);
#endif
/*End of Change by l63336 for NANDFLASH 2010-8-30*/

unsigned long ATP_FLASH_GetFlashOffset(unsigned long enCfgAreaType);
unsigned long ATP_FLASH_GetFlashAreaSize(unsigned long enCfgAreaType);


//void ATP_FLASH_Test();

#ifdef __cplusplus
}
#endif

#endif /* _ATP_FLASH_H */

