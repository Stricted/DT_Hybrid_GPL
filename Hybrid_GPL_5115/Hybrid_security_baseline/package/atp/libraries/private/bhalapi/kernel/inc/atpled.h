/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : atpled.h
  ��    ��  : l69021
  ��    ��  : 1.0
  ��������  : 2007-7-13
  ��    ��  : 
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-7-13
     ��    ��   : l69021
     �޸�����   : ��ɳ���

*********************************************************************************/

#if !defined(_ATP_LED_H)
#define _ATP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bhal.h" 
#include "bhalplat.h"

#define LED_MAX_NUMBER              15          /* ����֧�ֵ�LED��Ŀ */

#define LED_125MS                   (HZ / 8)    /* ~125 ms */
#define LED_FAST_BLINK_COUNT        0           /* ~125ms */
#define LED_SLOW_BLINK_COUNT        1           /* ~250ms */

#ifndef _ATPLED_GPIO_
#define _ATPLED_GPIO_
#define GPIO_1_NUM        1
#define GPIO_2_NUM        2
#define GPIO_3_NUM        3
#define GPIO_4_NUM        4
#define GPIO_5_NUM        5
#define GPIO_6_NUM        6
#define GPIO_7_NUM        7
#define GPIO_8_NUM        8

#endif

#define GPIO_STUN_GPIO_OUTPUT_BIT (3)
#define WPS_GPIO_SHIFT_BIT 20


#define GPIO_0_NUM_NETCTL 0        /*Internet Led Control 0 by L67530*/
#define GPIO_1_NUM_NETCTL 1        /*Internet Led Control 1 by L67530*/
#define GPIO_5_NUM_RESET 5
#define GPIO_6_NUM_WLCTL 6


typedef struct ledinfom
{
    long lLedNumber;
    long lBlinkCountDown; 
     BSP_E_LED_STATE enLedState;
     BSP_E_LED_STATE enSavedLedState;  // used in blink once for restore to the orignal ledState  
} LED_INFO_ST;


/********************* Public Interface Prototypes. ***************************/
void ATP_LED_InitLeds(void);
void ATP_SetBoardID();

#ifdef __cplusplus
}
#endif

#endif /* _ATP_LED_H */

