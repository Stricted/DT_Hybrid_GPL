/*****************************************************************************
 *               Copyright (C) 2006, Huawei Tech. Co., Ltd.               
 *                           ALL RIGHTS RESERVED                               
 * FileName: xml_interface.h                                                          
 * Version: 1.0                                                                
 * Description:  xml interface      
 *                                                                             
 * History:                                                                    
 * 1. 2006-12-25,z42422 Create this file.                                  
******************************************************************************/

#ifndef __TSP_XML__
#define __TSP_XML__

#include "atptypes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

typedef enum tagTSP_XML_E_TYPE
{
    TSP_XML_E_TYPE_BOOLEAN,  
    TSP_XML_E_TYPE_INT,    /* 32bit */
    TSP_XML_E_TYPE_UINT,
    TSP_XML_E_TYPE_SHORT,  /* 16bit */
    TSP_XML_E_TYPE_USHORT,
    TSP_XML_E_TYPE_BYTE,   /* 8bit */
    TSP_XML_E_TYPE_UBYTE,
    TSP_XML_E_TYPE_BUTT
}TSP_XML_E_TYPE;

typedef enum tagTSP_E_ERR_XML
{
    TSP_E_ERR_XML_OK  = 0,
    TSP_E_ERR_XML_INVALID_ARG = 0x1f000701,
    TSP_E_ERR_XML_SAME_ITEM,
    TSP_E_ERR_XML_NOT_FOUND,
    TSP_E_ERR_XML_MALLOC_FAIL,
    TSP_E_ERR_XML_PARSE_FAIL,
    TSP_E_ERR_XML_SYNTAX_ERR,
    TSP_E_ERR_XML_OPENFILE_ERR,
    TSP_E_ERR_XML_WRITEFILE_ERR,    
    TSP_E_ERR_XML_READFILE_ERR,
    TSP_E_ERR_XML_SMALL_BUF
}TSP_E_ERR_XML;

typedef enum tagTSP_XML_E_CONTENT
{
    TSP_XML_E_CONTENT_ELEMENT = 0   
}TSP_XML_E_CONTENT;

typedef enum tagXML_E_DEBUGLEVEL
{
    LEVEL_E_INFO = 0,  /* ��ʾ��Ϣ */
    LEVEL_E_WARN,      /* ������Ϣ */
    LEVEL_E_ERROR      /* ������Ϣ */
}XML_E_DEBUGLEVEL;

typedef unsigned long VOS_ULONG  ;

struct _NODE ;

typedef struct _NODE* NODEPTR ;

typedef VOS_VOID (*TSP_XML_FN_DEBUG)(VOS_UINT32 ulLevel, VOS_INT8 *pszString) ;

typedef VOS_VOID* (*TSP_XML_FN_MALLOC)(VOS_ULONG size);

typedef VOS_VOID* (*TSP_XML_FN_CALLOC)(VOS_ULONG n, VOS_ULONG size);

typedef VOS_VOID (*TSP_XML_FN_FREE)(VOS_VOID *ptr);

typedef VOS_VOID* (*TSP_XML_FN_REALLOC)(VOS_VOID *ptr, VOS_ULONG size);

/* ����XMLΪDOM�� */
extern VOS_UINT32 TSP_XML_Parse (const VOS_INT8 *pszXmlBuf, NODEPTR *ppNode);

extern VOS_UINT32 TSP_XML_ParseFile(const VOS_INT8 *pszXmlFile, NODEPTR *ppNode);

/* ��ӡDOM��ΪXML�ĵ� */
extern VOS_UINT32 TSP_XML_Transform (NODEPTR pNode , VOS_INT8 **pszBuf);    

extern VOS_UINT32 TSP_XML_TransformEx (NODEPTR pstNode, VOS_INT8 **ppszBuf, const VOS_CHAR *pcXmlHeader);

extern VOS_UINT32 TSP_XML_TransformFile (NODEPTR pNode , const VOS_INT8 *pszXmlFile);    

extern VOS_UINT32 TSP_XML_TransformXMLFile (NODEPTR pNode , const VOS_INT8 *pszXmlFile);    


/* ����һ���ڵ� */
extern VOS_UINT32 TSP_XML_CreateNode
(TSP_XML_E_CONTENT enType , const VOS_INT8 *pszNs, const VOS_INT8 *pszNodeName, const VOS_INT8* pszValue,  NODEPTR *ppNode);

/*node����set����*/
extern VOS_UINT32 TSP_XML_SetNode
(NODEPTR pNode, const VOS_INT8 *pszNs, const VOS_INT8 *pszName, const VOS_INT8 *pszValue );

/* ���ڵ����������� */
extern VOS_UINT32 TSP_XML_RemoveNode (NODEPTR pNode);

/* �ͷ�һ���ڵ��� */
extern VOS_VOID TSP_XML_FreeNode (NODEPTR phNode);

/* ��ýڵ���Ӹ��� */
extern VOS_UINT32 TSP_XML_GetNodeChildNum 
 (NODEPTR phNode, VOS_UINT32 *pulChildNum);

/*node����get����: ���parent, fistchild, lastchild, nextSibling, prevSiblign, value */

extern NODEPTR TSP_XML_GetNodeParent(NODEPTR phNode);

extern NODEPTR TSP_XML_GetNodeFirstChild (NODEPTR phNode);

extern NODEPTR TSP_XML_GetNodeLastChild (NODEPTR phNode);

extern NODEPTR TSP_XML_GetNodeNextSibling (NODEPTR phNode);

extern NODEPTR TSP_XML_GetNodePrevSibling (NODEPTR phNode);

extern VOS_UINT32 TSP_XML_GetNodeValue
     (NODEPTR pNode, TSP_XML_E_CONTENT *penType, const VOS_INT8 **ppszNs, const  VOS_INT8 ** ppszName, const VOS_INT8 ** ppszValue);

/*����get����*/
extern VOS_UINT32 TSP_XML_GetAttr 
(NODEPTR pNode,  const VOS_INT8 *pszNs, const VOS_INT8 *pszAttrName, const VOS_INT8 **ppszAttrValue);

extern VOS_UINT32 TSP_XML_GetFirstAttr 
(NODEPTR phNode, const VOS_INT8 **ppszNs, const VOS_INT8 **ppszAttr, const VOS_INT8 **ppszValue);

extern VOS_UINT32 TSP_XML_GetNextSiblingAttr 
(NODEPTR phNode, const VOS_INT8 *pszNs , const VOS_INT8 *pszAttrName, 
const VOS_INT8 **ppszNs, const VOS_INT8 **ppszAttr, const VOS_INT8 **ppszValue);

extern VOS_UINT32 TSP_XML_AddFirstChildNode(NODEPTR pFather, NODEPTR pChild);

extern VOS_UINT32 TSP_XML_AppendChildNode(NODEPTR pFather, NODEPTR pChild);

extern VOS_UINT32 TSP_XML_AddNextSiblingNode(NODEPTR pNode, NODEPTR pNextSibling);

extern VOS_UINT32 TSP_XML_AddPrevSiblingNode(NODEPTR pNode, NODEPTR pPrevSibling);

/* attr ����add���� */
extern VOS_UINT32 TSP_XML_AddAttr
 (NODEPTR pNode, const VOS_INT8 *pszNs, const VOS_INT8 *pszAttrName, const VOS_INT8 *pszAttrValue);

extern VOS_UINT32 TSP_XML_SetAttr
 (NODEPTR pNode, const VOS_INT8 *pszNs,
 const VOS_INT8 *pszAttrName, const VOS_INT8 *pszAttrValue);

extern VOS_UINT32 TSP_XML_RemoveAttr(NODEPTR pNode, const VOS_INT8* pszAttrNs, const VOS_INT8 *pszAttrName);

/* ��չ�ӿڣ�����Ӧ��ʹ�� */
/* �����������ü���ѯ�ڵ�������Ե�ֵ */
extern VOS_UINT32 TSP_XML_SetNodeValueEx(NODEPTR pNode, TSP_XML_E_TYPE enType, VOS_VOID *pValue);

extern VOS_UINT32 TSP_XML_GetNodeValueEx(NODEPTR pNode, TSP_XML_E_TYPE enType, VOS_VOID *pValue);

extern VOS_UINT32 TSP_XML_SetAttrValueEx
    (NODEPTR pNode, const VOS_INT8 *pszNs, const VOS_INT8* pszAttrname, TSP_XML_E_TYPE enType, VOS_VOID *pValue);

extern VOS_UINT32 TSP_XML_GetAttrValueEx
    (NODEPTR pNode, const VOS_INT8 *pszNs, const VOS_INT8* pszAttrname, TSP_XML_E_TYPE enType, VOS_VOID *pValue);

/* clone�� */
extern VOS_UINT32 TSP_XML_CloneNode (NODEPTR pNode, NODEPTR *ppNewNode);

/* clone�����ڵ� */
extern VOS_UINT32 TSP_XML_CloneNodeSelf (NODEPTR pNode, NODEPTR *ppNewNode);

/* ����URL��ýڵ� */
extern VOS_UINT32 TSP_XML_GetNodeByUrl
 (NODEPTR pNode, const VOS_INT8 *pszUrl, NODEPTR *ppNode);

extern NODEPTR TSP_XML_Iterator(NODEPTR pNode);

/* ���xml����汾�� */
extern VOS_UINT32 TSP_XML_GetVersion(VOS_INT8 *pszVersion, VOS_UINT32 ulLen);

/* ���õ�����Ϣ������� */
extern VOS_UINT32 TSP_XML_RegDebugFunc( TSP_XML_FN_DEBUG pfnDebug );

extern VOS_VOID TSP_XML_FreeMem(VOS_VOID *ptr);

/* ע���ڴ������غ���,���ڶ��ڴ����������Ҫ����û� */
extern VOS_UINT32 TSP_XML_RegMemFunc
  ( TSP_XML_FN_MALLOC pfnMalloc,  TSP_XML_FN_CALLOC pfnCalloc, 
    TSP_XML_FN_FREE  pfnFree,  TSP_XML_FN_REALLOC pfnRealloc);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __TSP_XML__ */

