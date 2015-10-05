/*
 * WPS environment variables
 *
 * Copyright (C) 2010, Broadcom Corporation. All Rights Reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * $Id: wps_ui.h 276844 2011-08-11 14:04:11Z gracecsm $
 */

#ifndef __WPS_1905_H__
#define __WPS_1905_H__

/*
 * WPS module
 */
#define WPS_1905_ADDR			"127.0.0.1"
#define WPS_1905_PORT			40600

typedef enum wps_1905_ctl_cmd
{
    WPS_1905_CTL_WSCSTATUS = 0,
    WPS_1905_CTL_APISCONFIGURED,
    WPS_1905_CTL_GETM2,
    WPS_1905_CTL_REGISTRARCAP,
    WPS_1905_CTL_CONFIGUREAP,
    WPS_1905_CTL_STARTAUTOCONF,
    WPS_1905_CTL_STOPAUTOCONF,
    WPS_1905_CTL_WLINSTANCE,
    WPS_1905_CTL_WSCPROCESS,
    WPS_1905_CTL_CLIENT_REGISTER,
    WPS_1905_CTL_CLIENT_RESTART
} WPS_1905_CTL_CMD;

typedef struct wps_1905_message {
    WPS_1905_CTL_CMD cmd;
    int index;
    int len;
    int status;
} WPS_1905_MESSAGE;

typedef enum wps_1905_ret_status {
    WPS_ERROR_1905_SENDFAILURE=200,         /*   Failed to send response M1/M2 to 1905 */

    /*   wscprocess,getm2 */
    WPS_1905_M1HANDLE_M2DATA,
    WPS_1905_M1HANDLE_NOTREGISRAR,

    /*    handle startautoconfig */
    WPS_1905_RESHANDLE_SESSIONOVERLAPPING,
    WPS_1905_RESHANDLE_M1DATA,

    /*  wscprocess, configureap */
    WPS_1905_M2SET_DONE,    /*   1905M2 SETUP DONE */
    WPS_1905_M2SET_NOMATCHING,
    WPS_1905_M2SET_NOSESSION,       /*   NO session opened for actions */
    WPS_1905_M2SET_NOAUTHENTICATOR,    /*   NO AUTHENTICATOR INFO in M2 */

    /*   handlw wscprocess */
    WPS_1905_UNKNOWNWSCMESSAGE,

    /*   wscstatus */
    WPS_1905_WSCAP_CONFIGURED,
    WPS_1905_WSCAP_UNCONFIGURED,
    WPS_1905_WSCAP_SESSIONONGOING
} WPS_1905_RET_STATUS;




#define WPS1905MSG_DATAPTR(pMessage) ((char *)(((void *)pMessage)+sizeof(WPS_1905_MESSAGE)))
#define WPS1905CMD_PTR(pMessage) (&(pMessage->cmd))

/*
 *  * Declare the message structure.
 *   */

typedef struct wps_1905_mq_message {
    long    type;
    WPS_1905_MESSAGE msg;
} WPS_1905_MQ_MESSAGE;

#define WPS1905MQMSG_PTR(pMessage) ((char *)(((void *)pMessage)+sizeof(WPS_1905_MQ_MESSAGE)))

int wps_1905_init();
void wps_1905_deinit();
int wps_1905_process_msg(char *buf, int buflen);
int wps_1905_send_response(char *msg,uint32 len,int cmd,int status);
int wps_1905_send_response_status(char *msg,uint32 len,int status);
#endif	/* __WPS_1905_H__ */
