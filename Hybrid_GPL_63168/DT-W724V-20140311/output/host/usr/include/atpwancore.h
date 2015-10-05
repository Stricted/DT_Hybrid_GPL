#ifndef __ATP_WANCORE_H__
#define __ATP_WANCORE_H__


typedef enum tagATP_WANCORE_MSGTYPES_EN
{
    ATP_MSG_WANCORE_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_WANCORE),
    ATP_MSG_WANCORE_WANCONN_CHANGE,
} ATP_WANCORE_MSGTYPES_EN;

/* 连接触发，需要与DHCP/PPPOE客户保持一致 */
typedef enum
{
    /* 等待 */
    WAN_TRIGGER_IDLE = 0,
    /* 断线 */
    WAN_TRIGGER_DISCONNECTING = 1,
    /* 手动 */
    WAN_TRIGGER_MANUAL = 2,
    /* 自动 */
    WAN_TRIGGER_ALWAYSON = 3,
    /* 按需拨号 */
    WAN_TRIGGER_ONDEMAND = 4, 
}WAN_TRIGGER_EN;


/* WAN连接状态 */
typedef enum
{
    /* 未初始化 */
    ATP_WAN_STATUS_UNCONFIGURED,
    /* 正在连接 */
    ATP_WAN_STATUS_CONNECTING,
    /* 正在认证 */
    ATP_WAN_STATUS_AUTHENTICATING,
    /* 已连接 */
    ATP_WAN_STATUS_CONNECTED,
    /* 未连接 */
    ATP_WAN_STATUS_PENDING,
    /* 正在断开 */
    ATP_WAN_STATUS_DISCONNECTING,
    /* 已断开 */
    ATP_WAN_STATUS_DISCONNECTED, 
}ATP_WAN_STATUS_EN;


#endif
