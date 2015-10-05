#include "bhal.h"
#include "atpconfig.h"
#include "msgapi.h"
#include <board.h>
#include <boardparms.h>

#ifdef SUPPORT_ATP_WLAN
int WlanWorkStatus = WLAN_DISABLE;
EXPORT_SYMBOL(WlanWorkStatus);
#endif

extern void ATP_LED_CtrlLed(BOARD_LED_NAME enLedName, BOARD_LED_STATE enLedState);

unsigned int ATP_LED_ChangeStatus(BSP_E_LED_NAME enLedName, unsigned int ulStatus)
{
    switch(enLedName)
    {
        case BHAL_LED_Internet:
        if (ATP_LED_WAN_CONNECTING_STATUS == ulStatus)
        {
            //PPP/DHCP negotiation
            ATP_LED_CtrlLed(BHAL_LED_Internet, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
        }
        else if (ATP_LED_WAN_CONNECTED_STATUS == ulStatus)
        {
            //PPP/DHCP
            ATP_LED_CtrlLed(BHAL_LED_Internet, BHAL_LED_STATE_ON);
        }
        else if (ATP_LED_WAN_DISCONNECTED_STATUS == ulStatus)
        {
            //authentication failed
            ATP_LED_CtrlLed(BHAL_LED_Internet, BHAL_LED_STATE_OFF);
        }
        else    
        {
            //Tx/Tx traffic on line
            ATP_LED_CtrlLed(BHAL_LED_Internet, BHAL_LED_STATE_FASTBLINK_CONTINUES);
        }
        break;

#ifdef SUPPORT_ATP_VOICE_DT
        case BHAL_LED_Voip1:
        if (ATP_VOICE_VOIP_NEGOTIATION_STATUS == ulStatus)
        {
            //negotiation or voip traffic presence
            ATP_LED_CtrlLed(BHAL_LED_Voip1, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
        }
        else if (ATP_VOICE_VOIP_REGISTERER_UP_STATUS == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Voip1, BHAL_LED_STATE_ON);
        }
        else if (ATP_VOICE_VOIP_TRAFFIC_IN_STATUS == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Voip1, BHAL_LED_STATE_FASTBLINK_CONTINUES);
        }
        else if (ATP_VOICE_VOIP_REGISTERER_DISABLED_STATUS == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Voip1, BHAL_LED_STATE_RED);
        }
        else
        {
            ATP_LED_CtrlLed(BHAL_LED_Voip1, BHAL_LED_STATE_OFF);
        }
        break;

        case BHAL_LED_DectSearch: // Wait fix: Should use ulStatus defined in Dect application.
        if (BHAL_LED_STATE_ON == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_DectSearch, BHAL_LED_STATE_ON);
        }
        else if (BHAL_LED_STATE_OFF == ulStatus)
    	{
    	    ATP_LED_CtrlLed(BHAL_LED_DectSearch, BHAL_LED_STATE_OFF);
    	}
    	else if (BHAL_LED_STATE_SLOWBLINK_CONTINUES == ulStatus)
    	{
    	    ATP_LED_CtrlLed(BHAL_LED_DectSearch, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
    	}
    	else
    	{
    	    ATP_LED_CtrlLed(BHAL_LED_DectSearch, BHAL_LED_STATE_OFF);
    	}
    	break;
#endif

#ifdef SUPPORT_ATP_XDSL
        case BHAL_LED_Wan: //DSL LED
        if (ATP_DSL_STATUS_UP == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Wan, BHAL_LED_STATE_ON);
        }
        else if ((ATP_DSL_STATUS_TRAINING == ulStatus) || (ATP_DSL_STATUS_INIT == ulStatus))
        {
            ATP_LED_CtrlLed(BHAL_LED_Wan, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
        }
        else if (ATP_DSL_STATUS_DOWN == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Wan, BHAL_LED_STATE_OFF);
        }
        else
        {
            ATP_LED_CtrlLed(BHAL_LED_Wan, BHAL_LED_STATE_OFF);
        }
        break;
#endif

#ifdef SUPPORT_ATP_WLAN
        case BHAL_LED_Wlan:
        if (BHAL_LED_STATE_ON == ulStatus)
        {   
            ATP_LED_CtrlLed(BHAL_LED_Wlan, BHAL_LED_STATE_ON);
        }
        else if (BHAL_LED_STATE_OFF == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Wlan, BHAL_LED_STATE_OFF);
        }
        else
        {
            ATP_LED_CtrlLed(BHAL_LED_Wlan, BHAL_LED_STATE_OFF);
        }
        break;

        case BHAL_LED_Wps:
    	if(BHAL_LED_STATE_SLOWBLINK_CONTINUES == ulStatus)
    	{
    		ATP_LED_CtrlLed(BHAL_LED_Wps, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
    	}
		else if (BHAL_LED_WPS_WLAN_STATE_SLOWBLINK_CONTINUES == ulStatus)
		{
			ATP_LED_CtrlLed(BHAL_LED_Wps, BHAL_LED_WPS_WLAN_STATE_SLOWBLINK_CONTINUES);
		}
		else if (BHAL_LED_WPS_VOIP_STATE_SLOWBLINK_CONTINUES == ulStatus)
		{
			ATP_LED_CtrlLed(BHAL_LED_Wps, BHAL_LED_WPS_VOIP_STATE_SLOWBLINK_CONTINUES);
		}
		else if (BHAL_LED_WPS_WLAN_VOIP_STATE_SLOWBLINK_CONTINUES == ulStatus)
		{
			ATP_LED_CtrlLed(BHAL_LED_Wps, BHAL_LED_WPS_WLAN_VOIP_STATE_SLOWBLINK_CONTINUES);
		}
    	else if(BHAL_LED_STATE_OFF == ulStatus)
    	{
    	    ATP_LED_CtrlLed(BHAL_LED_Wps, BHAL_LED_STATE_OFF);
    	}
		else
    	{
    	    ATP_LED_CtrlLed(BHAL_LED_Wps, BHAL_LED_STATE_OFF);
    	}
    	break;
#endif

#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
		case BHAL_LED_Service1:
		if (BHAL_LED_STATE_SERVICE_SLOWBLINK_CONTINUES == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Service1, BHAL_LED_STATE_SERVICE_SLOWBLINK_CONTINUES);
        }
        else if (BHAL_LED_STATE_ON == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Service1, BHAL_LED_STATE_ON);
        }
		else
    	{
    		ATP_LED_CtrlLed(BHAL_LED_Service1, BHAL_LED_STATE_OFF);
    	}
        break;
		
		case BHAL_LED_Service2:
        if (BHAL_LED_STATE_BITTEWARTEN_SLOWBLINK_CONTINUES == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Service2, BHAL_LED_STATE_BITTEWARTEN_SLOWBLINK_CONTINUES);
        }
        else if (BHAL_LED_STATE_ON == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Service2, BHAL_LED_STATE_ON);
        }
		else
    	{
    		ATP_LED_CtrlLed(BHAL_LED_Service2, BHAL_LED_STATE_OFF);
    	}
        break;
#endif
#if 1
		case BHAL_LED_Powerfail:
		if (BHAL_LED_STATE_SLOWBLINK_CONTINUES == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Powerfail, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
        }
        else if (BHAL_LED_STATE_ON == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Powerfail, BHAL_LED_STATE_ON);
        }
		else
    	{
    		ATP_LED_CtrlLed(BHAL_LED_Powerfail, BHAL_LED_STATE_OFF);
    	}
        break;
#endif
#ifdef SUPPORT_ATP_DT_W724V_UPG_LED
        case BHAL_LED_Power:
		if (BHAL_LED_STATE_SLOWBLINK_CONTINUES == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Power, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
        }
        else if (BHAL_LED_STATE_ON == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_Power, BHAL_LED_STATE_ON);
        }
        else
        {
            ATP_LED_CtrlLed(BHAL_LED_Power, BHAL_LED_STATE_OFF);
        }
        break;
#endif

        case BHAL_LED_LineLteAll:
        if (BHAL_LED_STATE_SLOWBLINK_CONTINUES == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_LineLte1, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
            ATP_LED_CtrlLed(BHAL_LED_LineLte2, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
            ATP_LED_CtrlLed(BHAL_LED_LineLte3, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
            ATP_LED_CtrlLed(BHAL_LED_LineLte4, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
            ATP_LED_CtrlLed(BHAL_LED_LineLte5, BHAL_LED_STATE_SLOWBLINK_CONTINUES);
        }
        else if (BHAL_LED_STATE_OFF == ulStatus)
        {
            ATP_LED_CtrlLed(BHAL_LED_LineLte1, BHAL_LED_STATE_OFF);
            ATP_LED_CtrlLed(BHAL_LED_LineLte2, BHAL_LED_STATE_OFF);
            ATP_LED_CtrlLed(BHAL_LED_LineLte3, BHAL_LED_STATE_OFF);
            ATP_LED_CtrlLed(BHAL_LED_LineLte4, BHAL_LED_STATE_OFF);
            ATP_LED_CtrlLed(BHAL_LED_LineLte5, BHAL_LED_STATE_OFF);
        }
        else
        {
            ATP_LED_CtrlLed(BHAL_LED_LineLte1, BHAL_LED_STATE_ON);
            ATP_LED_CtrlLed(BHAL_LED_LineLte2, BHAL_LED_STATE_ON);
            ATP_LED_CtrlLed(BHAL_LED_LineLte3, BHAL_LED_STATE_ON);
            ATP_LED_CtrlLed(BHAL_LED_LineLte4, BHAL_LED_STATE_ON);
            ATP_LED_CtrlLed(BHAL_LED_LineLte5, BHAL_LED_STATE_ON);
        }
        break;

        default:
        {
            ATP_LED_CtrlLed((BOARD_LED_NAME)enLedName, (BOARD_LED_STATE)ulStatus);
            break;
        }
    }
    return 0;
}


