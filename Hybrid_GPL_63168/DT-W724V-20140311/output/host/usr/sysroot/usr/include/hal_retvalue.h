/********************************************************************************
*               Copyright (C) 2006, Huawei Tech. Co., Ltd.              
*                           ALL RIGHTS RESERVED                              
* 版    本: 1.0                                                       
* 文件功能: 根据《底层软件开发过程规范V1.0》要求，为底层软件开发提供的统一的函数错误返
*          回码集合。错误返回码可以根据需求进行添加，修改较大时注意更新相应规范。                              
* 作    者：固网终端驱动与OS技术开发团队                                                        
* 修改历史:                                                                                                                                                 
* 1. 2006-3-1,固网终端驱动与OS技术开发团队 创建该文件                                                                                                                                                                                        
********************************************************************************/

#ifndef __HAL_RETVALUE_H__
#define __HAL_RETVALUE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE   1
#endif

#define ERR_NO(x)	(0-x)
       
typedef enum
{
    HAL_E_RETVAL_OK                      = 0,            /* 函数正确返回 */
    HAL_E_RETVAL_EDEFAULT                = ERR_NO(1),    /* 默认错误 */
    HAL_E_RETVAL_EFILE_OPEN              = ERR_NO(101),  /* 文件打开失败 */
    HAL_E_RETVAL_EFILE_LEN               = ERR_NO(102),  /* 文件大小错误 */
    HAL_E_RETVAL_EFILE_READ              = ERR_NO(103),  /* 读取文件失败 */
    HAL_E_RETVAL_EFILE_WRITE             = ERR_NO(104),  /* 写入文件失败 */
    HAL_E_RETVAL_EFILE_SEEK              = ERR_NO(105),  /* 文件指针定位失败 */
    HAL_E_RETVAL_EPBUF_NULL              = ERR_NO(201),  /* 空地址 */
    HAL_E_RETVAL_EMEM_GET                = ERR_NO(202),  /* 动态申请内存失败 */
    HAL_E_RETVAL_EBUF_OVERLEN            = ERR_NO(203),  /* 写入缓冲区超长 */
    HAL_E_RETVAL_EBUF_TOOSMALL           = ERR_NO(204),  /* 读取缓冲区太小 */
    HAL_E_RETVAL_EFLASH_READ             = ERR_NO(301),  /* 读FLASH错误*/
    HAL_E_RETVAL_EFLASH_WRITE            = ERR_NO(302),  /* 写FLASH错误*/
    HAL_E_RETVAL_EFLASH_ERASE            = ERR_NO(303),  /* 擦除Flash Block失败*/
    HAL_E_RETVAL_EFLASH_ADDRER           = ERR_NO(304),  /* 地址越界*/   
    HAL_E_RETVAL_EFLASH_BLOCK_BEGIN_ADDR = ERR_NO(305),  /* 不是块的首地址 */   
    HAL_E_RETVAL_EFLASH_BLOCK_END_ADDR   = ERR_NO(305),  /* 不是块的尾地址 */ 
    HAL_E_RETVAL_EFLASH_NOT_EXIST        = ERR_NO(306),  /* FLASH不存在 */    
    HAL_E_RETVAL_EFLASH_NOT_READY        = ERR_NO(307),  /* FLASH正忙 */    
    HAL_E_RETVAL_EFLASH_NOT_SUPPORT_CFI  = ERR_NO(308),  /* 不支持CFI */    
    HAL_E_RETVAL_EFLASH_PARAMETER        = ERR_NO(309),  /* 参数非法 */
    HAL_E_RETVAL_EDEFLATE_DATA           = ERR_NO(401),  /* 数据压缩失败 */
    HAL_E_RETVAL_EINFLATE_DATA           = ERR_NO(402),  /* 数据解压失败 */
    
}HAL_E_FUNCRETVAL;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* __HAL_RETVALUE_H__ */
