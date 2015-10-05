#ifndef __SENSOR_API_H__
#define __SENSOR_API_H__

#include "atptypes.h"
#include "msgapi.h"
#include "httpapi.h"
#include "xmlapi.h"
#include "cfmapi.h"
#include "cmo_def.h"


#ifdef ATP_DEBUG
#define ATP_IPDR_SENSOR_DEBUG(format, args...)       {printf("FILE: %s LINE: %d: ", __FILE__, __LINE__);printf(format, ##args);}
#else
#define ATP_IPDR_SENSOR_DEBUG(x...)
#endif

#define ATP_IPDR_WAIT_TIME  (1000*30)

//error code
typedef enum tagATP_IPDR_ERR_CODE_EN
{
    ATP_IPDR_ERR_MEM,     //内存错误
    ATP_IPDR_ERR_GET_OBJ,    //获取对象失败
    ATP_IPDR_ERR_GET_VALUE,   //获取参数失败
    ATP_IPDR_ERR_BUILD_HEAD,   //创建消息头失败
    ATP_IPDR_ERR_GET_KEYLIST,   //domain to keylist error
    ATP_IPDR_ERR_PARA,
    ATP_IPDR_ERR_APPAND_REFERENCE,
} ATP_IPDR_ERR_CODE_EN;

typedef struct tagATP_IPDR_OBJ_GETMSG_ST
{
    struct tagATP_IPDR_OBJ_GETMSG_ST	*pstNext;
    
    VOS_UINT32      ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
    VOS_UINT32      ulObjOID;
    VOS_VOID*       pvParaListMsg;
} ATP_IPDR_OBJ_GETMSG_ST;

//typedef sensor struct
typedef struct tagATP_IPDR_SENSOR_ST
{
    VOS_UINT32      ulKeyList[ATP_CFM_MAX_TREE_DEPTH];
    VOS_UINT32      ulOID; 
    VOS_CHAR*            pcEncodingTypes;    //XML 、XDR
    VOS_UINT32           ulReportingInterval;   //上报间隔
    ATP_IPDR_OBJ_GETMSG_ST *pstObjGetMsgList;
} ATP_IPDR_SENSOR_ST;


//接口函数
//创建一个Sensor
ATP_IPDR_SENSOR_ST* ATP_IPDR_SensorCreate(const VOS_UINT32 ulKeyList[ATP_CFM_MAX_TREE_DEPTH],    VOS_UINT32 ulOID);

//从数据库中加载数据，并获取obj 或parameter的reference
VOS_UINT32  ATP_IPDR_SensorReloadParameters(ATP_IPDR_SENSOR_ST*  pstSensor);

//解析对象或参数的值
VOS_UINT32 ATP_IPDR_SensorIterateValues(const ATP_IPDR_SENSOR_ST* pstSensor, VOS_VOID* pvArg, PFCfmDBIterator pfIterator);

//destroy sensor
VOS_VOID ATP_IPDR_SensorDestroy(ATP_IPDR_SENSOR_ST *pstSensor);

//通过sensor获取参数信息
VOS_UINT32 ATP_IPDR_SensorPrintValues(const ATP_IPDR_SENSOR_ST*  pstSensor);


#endif   /*__SENSOR_API_H__*/

