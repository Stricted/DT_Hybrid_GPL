/***********************************************************************
 *
 *  Copyright (c) 2013  Broadcom Corporation
 *  All Rights Reserved
 *
 * <:label-BRCM:2013:proprietary:standard
 * 
 *  This program is the proprietary software of Broadcom Corporation and/or its
 *  licensors, and may only be used, duplicated, modified or distributed pursuant
 *  to the terms and conditions of a separate, written license agreement executed
 *  between you and Broadcom (an "Authorized License").  Except as set forth in
 *  an Authorized License, Broadcom grants no license (express or implied), right
 *  to use, or waiver of any kind with respect to the Software, and Broadcom
 *  expressly reserves all rights in and to the Software and all intellectual
 *  property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 *  NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 *  BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 * 
 *  Except as expressly set forth in the Authorized License,
 * 
 *  1. This program, including its structure, sequence and organization,
 *     constitutes the valuable trade secrets of Broadcom, and you shall use
 *     all reasonable efforts to protect the confidentiality thereof, and to
 *     use this information only in connection with your use of Broadcom
 *     integrated circuit products.
 * 
 *  2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *     AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 *     WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 *     RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
 *     ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
 *     FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
 *     COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
 *     TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
 *     PERFORMANCE OF THE SOFTWARE.
 * 
 *  3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
 *     ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 *     INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
 *     WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
 *     IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
 *     OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
 *     SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
 *     SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
 *     LIMITED REMEDY.
 * :>
 *
 ************************************************************************/
/**  @mainpage Broadcom Wireless Configuration System Module
 *
 *   @section Introduction
 *
 *   This document covers Broadcom WLAN Configuration and System
 *   Module(WCSM).
 *
 *  @file wlcsm_lib_api.h
 *  @brief shared wlcsm header file for API
 *
 *  this header file includes all general functions and structure definations
 *  used for this wlcsm module.
 *
 * */

#ifndef __WLCSM_API_H__
#define __WLCSM_API_H__

/** @brief NVRAM variable types enum */
typedef enum t_wlcsm_nvram_var_type {
    
    WLCSM_NVRAM_VAR_INT,                        /**< varible is integer value */
    WLCSM_NVRAM_VAR_STRING                     /**< variable is string value */

} t_WLCSM_NVRAM_VAR_TYPE;

/** @brief NVRAM variable struct to tell the name and type */
typedef struct t_wlcsm_nvram_var {

    char *name;                            /**< variable name */
    int type;              		  /**< varaible type */
    char *value;

} t_WLCSM_NVRAM_VAR;


/** @brief holder structure for a few nvram variables which share the same prefix.  */

typedef struct t_wlcsm_nvram_names_set {
    char prefix[32];
    struct t_wlcsm_nvram_var vars[];
} t_WLCSM_NVRAM_NAMES_SET;


/** @brief CSM events which can fired to invoking application
 *
 * CSM implements notification mechanism to have invoking applice be aware
 * of interesting events which it may has to response.
 *
 */

typedef enum t_wlcsm_event_type {

    /** @brief event triggered when nvram item set or unset
     *
     *  the event handler for this will include three arguments:
     *	___
     * 	1. arg1: char * name
     *	2. arg2: char * value to be changed [NULL when unset]
     *	3. arg3: char * oldvalue
     */
    WLCSM_EVT_NVRAM_CHANGED,
    /** @brief  event when nvram is commited
     *
     * the event handler's three arguments will all be NULL
     * as no any value is required for this event.
     *
     * */
    WLCSM_EVT_NVRAM_COMMITTED,
    WLCSM_EVT_LAST                     

} t_WLCSM_EVENT_TYPE;


/** @brief event hook function prototype for specific event
 *
 * For all specific event, invoking function can define hook function to
 * respond to certain event, the functions always carry three arguments regardless
 * of event type. Check each event for the meaning of reach arguments.
 *
 */
typedef void (*WLCSM_EVENT_HOOK_FUNC)(const char *arg1,const char *arg2,char* arg3);

/** @brief event hook function prototype for responding to __ALL__ event
 *
 * For responding to all events, the arguements of the function will be variable
 * in hook function, it has to be analyzed event by event,
 * refer to ::wlcsm_register_event_hook for example.
 */
typedef void (*WLCSM_EVENT_HOOK_GENERICFUNC)(t_WLCSM_EVENT_TYPE event,...);

/*************************************************************//**
 * @brief get nvram value
 *
 *  get nvram item value by name
 *
 **/
char *wlcsm_nvram_get(const char *name);

/*************************************************************//**
 * @brief set nvram value
 *
 *  set nvram item value by name
 *
 **/
int  wlcsm_nvram_set(const char *name,const char *value);


/*************************************************************//**
 * @brief unset nvram value
 *
 * unset nvram item value by name
 *
 **/
int  wlcsm_nvram_unset(const char *name);


/*************************************************************//**
 * @brief getall nvram value
 *
 *  retrieve all nvram item values into buf with maximum length of count
 *
 **/
int wlcsm_nvram_getall(char *buf, int count);

/*************************************************************//** 
 * @brief commit all nvram values into storage
 *
 * once commit, event will send over to APPs which are listening on the event, APPs should respond
 * to this event with corresponding actions.
 *
 **/
int wlcsm_nvram_commit (void);

/*************************************************************//**
 * @brief set trace debug
 *
 * This API will take string input such as:
 * 	+log [enable user space debugging log]
 * 	-err
 * 	k+log [kernel debugging level]
 ***************************************************************/
void wlcsm_set_trace_level(char *tl);

/*************************************************************//**
 * @brief set userpace trace debug
 *
 ***************************************************************/
void wlcsm_set_trace(unsigned int level);

/*************************************************************//**
 * @brief enable listenging thread for event handling
 *
 * When an app often conduct nvram read/write operation, to improve performance
 * invoke this API at the beginning of the APP will enable local copy of nvram
 * value and it will get updated in time. If the application has registered event
 * , this API is being invoked automatically. [so do not need to call it again]
 *
 ***************************************************************/
int wlcsm_init();

/*************************************************************//**
 * @brief resource clean up api
 *
 * This API should be used as pair with ::wlcsm_init when app quite.
 **************************************************************/
int wlcsm_shutdown();


/*************************************************************//**
 * @brief API for app to register hook for specific event
 *
 * For some applications, they maybe only interested in certain events,but not
 * all events. For example, some application will be only interested in nvram
 * change event, in this case, the applications can use this function to register
 * funtion to respond to this specific event.
 *
 * ~~~~~
 *
 * void nvram_changed(char *name, char *value, char *oldvalue) {
 *        printf("nvram changed noticed:%s,from %s to %s \r\n",namme,oldvalue,value );
 * }
 *
 * wlcsm_register_event_hook(WLCSM_EVT_NVRAM_CHANGED,nvram_changed);
 * ~~~~~
 *
 * How to respond to this event is solely invoking application's responsibility.But take
 * notice that this function is invoked when the event happens, the hook function should
 * not occupy too long for processing.
 *
 * @return void
 ***************************************************************/
void wlcsm_register_event_hook(t_WLCSM_EVENT_TYPE type,WLCSM_EVENT_HOOK_FUNC hook);

/*************************************************************//**
 * @brief API for app to register hook to respond __ALL__ event.
 *
 * In rare case, one application is interested in all the events. In such a case, one
 * applicatoin can use this API to register its event handler.
 * @anchor allevent
 * ~~~~
 *	void wlcsm_event_handler(t_WLCSM_EVENT_TYPE type,...)
 *	{
 *	    va_list arglist;
 *
 *	    va_start(arglist,type);
 *
 *	    switch ( type ) {
 *
 *	    case WLCSM_EVT_NVRAM_CHANGED: {
 *
 *		char * name=va_arg(arglist,char *);
 *		char * value=va_arg(arglist,char *);
 *		char * oldvalue=va_arg(arglist,char *);
 *		printf("change received name:%s,value:%s,oldvalue:%s\r\n",name,value,oldvalue);
 *	    }
 *	    break;
 *
 *	    case WLCSM_EVT_NVRAM_COMMITTED:
 *		printf("nvram commited, restart or not? \r\n");
 *		break;
 *
 *	    default:
 *		break;
 *	    }
 *
 *	    va_end(arglist);
 *	}
 *
 *     wlcsm_register_event_generic_hook(wlcsm_event_handler);
 *~~~~
 * @return void
 ***************************************************************/
void wlcsm_register_event_generic_hook(WLCSM_EVENT_HOOK_GENERICFUNC hook);
#endif
