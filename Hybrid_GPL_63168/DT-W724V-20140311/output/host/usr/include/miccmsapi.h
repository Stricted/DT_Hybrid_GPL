
/****************************************************************************
 * File Name:     miccmsapi.h
 * Author:        ATP team
 * Date:           2011/12/17
 * Descripton:   mic cms inner interface declarations
 *****************************************************************************/

#ifndef _MIC_CMS_H
#define _MIC_CMS_H

#include "atptypes.h"




#if defined(__cplusplus)
extern "C" {
#endif


VOS_UINT32  MicSubDevAttachedDoNotify();


#ifdef ATP_DEBUG
#define ATP_MICCMS_DEBUG(format, args...)           {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_MICCMS_DEBUG(x...)
#endif


#if defined(__cplusplus)
}
#endif


#endif
