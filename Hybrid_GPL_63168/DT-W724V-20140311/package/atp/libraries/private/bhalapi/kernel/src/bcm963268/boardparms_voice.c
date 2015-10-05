/*
    Copyright 2000-2010 Broadcom Corporation

    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the “GPL”), available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:

        As a special exception, the copyright holders of this software give
        you permission to link this software with independent modules, and to
        copy and distribute the resulting executable under terms of your
        choice, provided that you also meet, for each linked independent
        module, the terms and conditions of the license of that module. 
        An independent module is a module which is not derived from this
        software.  The special exception does not apply to any modifications
        of the software.

    Notwithstanding the above, under no circumstances may you combine this
    software in any way with any other Broadcom software provided under a
    license other than the GPL, without Broadcom's express prior written
    consent.
*/                       

/**************************************************************************
* File Name  : boardparms_voice.c
*
* Description: This file contains the implementation for the BCM63xx board
*              parameter voice access functions.
*
***************************************************************************/
#include "atpconfig.h"

#ifdef SUPPORT_ATP_HYBRID
/* ---- Include Files ---------------------------------------------------- */

#include "boardparms_voice.h"
#include <bcm_map_part.h>

#if !defined(_CFE_)
#include <linux/kernel.h>
#endif

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/*
 * -------------------------- Daughter Boards ------------------------------
 */



VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88506 =
{
   VOICECFG_LE88506_STR,     /* szBoardId */
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      /* Always end the device list with BP_NULL_DEVICE */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88264_TH =
{
   VOICECFG_LE88264_TH_STR,   /*Daughter Card ID*/
   2,   /*Number of FXS Lines*/
   0,   /*Number of FXO Lines*/
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_88264,
         BP_SPI_SS_B1, /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1,   /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
             }
         }
      },

      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK_TH,
   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32261 =
{
   VOICECFG_SI32261_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {  /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,   /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1,  /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
         /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
         /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88266 =
{
   VOICECFG_LE88266_STR,   /* Daughter Board ID */
   2,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {   /* Voice Device 0 */
      {
         /* Device Type */
         BP_VD_ZARLINK_88266,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
         /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
         /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_MASTERSLAVE_FB,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32267 =
{
   VOICECFG_SI32267_STR,   /*Daughter board ID */
   2,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32267,
         BP_SPI_SS_NOT_REQUIRED,   /* ISI SPI CS handled internally. It is mapped by the zsiChipMap list. */
         BP_RESET_FXS1,
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,
   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI3217x =
{
   VOICECFG_SI3217X_STR,   /*Daughter Card ID */
   2,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32176,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_SILABS_32178,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            }
         }
      },

      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   1,   /* FXO number is 1 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_89116,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_89316,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88266x2_LE89010 =
{
   VOICECFG_LE88266x2_LE89010_STR,   /* Daughter Card ID */
   4,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_88266,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_88266,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },
      {
         /* Device Type 3 */
         BP_VD_ZARLINK_89010,
         BP_SPI_SS_B3,  /* Device uses SPI_SS_B3 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXO,  /* Device uses FXO reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on Le89010 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_MASTERSLAVE_FB,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260x2 =
{
   VOICECFG_SI32260x2_STR,   /*Daughter Card ID*/
   4,   /*Number of FXS Lines*/
   0,   /*Number of FXO Lines*/
   {   /* voiceDevice0 Parameters */
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2*/
         BP_VD_SILABS_32261,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88536_ZSI =
{
   VOICECFG_LE88536_ZSI_STR,   /* Daughter Board ID */
   2,   /* Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {   /* Voice Device 0 Parameters */
      {
         BP_VD_ZARLINK_88536,   /* Device Type */
         BP_SPI_SS_NOT_REQUIRED,   /* ZSI SPI CS handled internally. It is mapped using the zsiMapList. */
         BP_RESET_FXS1,
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_ZL88601 =
{
   VOICECFG_VE886_HUAWEI_STR,     /* szBoardId */
   6,             /* numFxsLines */
   0,             /* numFxoLines */

   {
      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
            /* Channel 1 on device 1 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               7,
               7            
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,
         BP_NOT_DEFINED,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_NOT_DEFINED, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
		 
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
    /* voiceDevice3 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_88601,
         BP_NOT_DEFINED,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_NOT_DEFINED, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice4 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,
         BP_NOT_DEFINED,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_NOT_DEFINED, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },


    /* voiceDevice5 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_88601,
         BP_NOT_DEFINED,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_NOT_DEFINED, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};


VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_ZL88701 =
{
   VOICECFG_ZL88701_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_88701,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_89010,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B3 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXO,  /* Device uses FXO reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on Le89010 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE89116 =
{
   VOICECFG_LE89116_STR,   /* Daughter Card ID */
   1,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_89116,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
            }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI3217x_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* Daughter Card ID */
   2,   /* Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device type */
         BP_VD_SILABS_32176,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 Reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },
      {
         /* Device Type */
         BP_VD_SILABS_32178,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32176 =
{
   VOICECFG_SI32176_STR,   /* Daughter Board ID */
   1,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32176,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_VE890HV =
{
   VOICECFG_VE890HV_STR,   /*Daughter Card ID */
   2,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /* Device type */
         BP_VD_ZARLINK_89136,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      {
         /* Device type 2 */
         BP_VD_ZARLINK_89336,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE89316 =
{
   VOICECFG_LE89316_STR,   /* Daughter Card ID */
   1,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_89316,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )

};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32178 =
{
   VOICECFG_SI32178_STR,   /* Daughter Board ID */
   1,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32178,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },

   /* SLIC Device Profile */
    BP_VD_FLYBACK,
    /* General-purpose flags */
    ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_NOSLIC =
{
   VOICECFG_NOSLIC_STR, /*Daughter Board ID */
   0,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      BP_NULL_DEVICE_MACRO_NEW
   },

   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32267_NTR =
{
   VOICECFG_SI32267_NTR_STR,   /* Daughter Board ID */
   2,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32267,
         BP_SPI_SS_NOT_REQUIRED, /* ISI SPI CS handled internally. It is mapped by the zsiChipMap list. */
         BP_RESET_FXS1,          /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,
   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260x2_SI3050 =
{
   VOICECFG_SI32260x2_SI3050_STR,   /*Daughter card ID */
   4,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },
      {
         /* Device Type 3 */
         BP_VD_SILABS_3050,
         BP_SPI_SS_B3,  /* Device uses SPI_SS_B3 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXO,  /* Device uses FXO reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on Si3050 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88266x2 =
{
   VOICECFG_LE88266x2_STR,   /* Daughter card ID */
   4,   /*Number of FXS Lines */
   0,   /* Number of FXO Lines*/
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_88266,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_88266,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },

   /* SLIC Device Profile */
   BP_VD_MASTERSLAVE_FB,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_6368MVWG =
{
   VOICECFG_6368MVWG_STR,   /*Daughter board ID */
   2,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /*Voice device 0 */
         BP_VD_ZARLINK_89116, /*Device type */
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      {
         /*Voice Device 1 */
         BP_VD_ZARLINK_89316,
         BP_SPI_SS_B2,           /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not need reset. */

         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9530 =
{
   VOICECFG_LE9530_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {  /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9530,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9530_WB =
{
   VOICECFG_LE9530_WB_STR,	/* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9530,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI3239 =
{
   VOICECFG_SI3239_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3239,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32392 =
{
   VOICECFG_SI32392_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32392,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9530_LE88506 = {
   VOICECFG_LE9530_LE88506_STR,   /* Daughter board ID*/
   4,   /*Num FXS Lines */
   0,   /*Num FXO Lines */
   {   /*voiceDevice0 Params */
      {
         /* Device Type */
         BP_VD_ZARLINK_88506,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
            {
               /* Channel 0 on device */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  0,
                  0
               },
               /* Test a single channel on 88506 */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  1,
                  1
               }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_9530,
         BP_SPI_SS_B1,           /*The 9530 chips are actually internal, device ID is always 0. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         /* Channel description */
            {
               /* Channel 0 on device */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  BP_TIMESLOT_INVALID,
                  BP_TIMESLOT_INVALID
               },
               /* Channel 1 on device */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  BP_TIMESLOT_INVALID,
                  BP_TIMESLOT_INVALID
               }
            }
      },

     /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

/* End of Daughter Card Definitions */

#if defined(_BCM96328_) || defined(CONFIG_BCM96328)

VOICE_BOARD_PARMS_NEW g_voice_bcm96328AVNGR =
{
   "96328avngr",   /*Base Board ID*/
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_2, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_6, BP_DEDICATED_PIN }    /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_29_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED, /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED, /* DECT reset GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Dect is not supported on this board. */

   /* List of daughter boards supported by this base board. */
   {
      &g_voiceBoard_SI32176,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE89116,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_LE88264_TH,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_SI32267,
      0
   }
};

/* List of base boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[] =
{
   &g_voice_bcm96328AVNGR,
   0
};

#endif

#if defined(_BCM96362_) || defined(CONFIG_BCM96362)

VOICE_BOARD_PARMS_NEW g_voice_bcm96362RPVT = {
   "96362RPVT",   /*Base Board ID*/
   {
      { SPI_DEV_0, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_2, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_28_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_31_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_26_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_29_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_35_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   /* Dect is supported. */
   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter cards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96362RPVT_2u = {
   "96362RPVT_2U",   /*Base Board ID*/
   {
      { SPI_DEV_0, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_2, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_28_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_31_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_26_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_29_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_35_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   /* Dect is supported. */
   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter cards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96362ADVN2XH =
{
   "96362ADVN2xh",   /*Base Board ID*/
   {
      { SPI_DEV_0, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_1, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_2, BP_GPIO_9_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_28_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_31_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_26_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_29_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_44_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88506,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96362RAVNGR2 =
{
   "96362RAVNGR2",   /*Base Board ID*/
   {
      { SPI_DEV_0, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_1, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_2, BP_GPIO_9_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_28_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_31_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_DEFINED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_29_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_35_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   /* Dect is supported*/
   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96362ADVNGR2 =
{
   "96362ADVNgr2",   /*Base Board ID*/
   {
      { SPI_DEV_0, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_1, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_2, BP_GPIO_9_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_28_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_31_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_DEFINED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_29_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_44_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_VE890_INVBOOST,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96362RADVN2XH =
{
   "96362RADVN2XH",   /*Base Board ID*/
   {
      { SPI_DEV_0, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_1, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin. */
      { SPI_DEV_2, BP_GPIO_9_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_28_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_31_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_26_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_29_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_44_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88506,
      0
   }
};

/* List of base boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm96362RPVT,
   &g_voice_bcm96362RPVT_2u,
   &g_voice_bcm96362ADVN2XH,
   &g_voice_bcm96362RAVNGR2,
   &g_voice_bcm96362ADVNGR2,
   &g_voice_bcm96362RADVN2XH,
   0
};

#endif

#if defined(_BCM963268_) || defined(CONFIG_BCM963268)

VOICE_BOARD_PARMS_NEW g_voice_bcm963268HW_hybrid_VE886 =
{
   "Hybrid",  /*Base Board ID */
   {
      { SPI_DEV_1, BP_GPIO_16_AL }, /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },    /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_3, BP_GPIO_16_AL }  /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_14_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_14_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_14_AL   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },
   
   BP_NOT_DEFINED,    /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */
   
   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_ZL88601,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963268HW_w724v_VE886 =
{
   "W724v",  /*Base Board ID */
   {
      { SPI_DEV_1, BP_GPIO_16_AL }, /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },    /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_3, BP_GPIO_16_AL }  /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_14_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_14_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_14_AL   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },
   
   BP_NOT_DEFINED,    /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */
   
   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_ZL88601,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963268SV =
{
   "963268SV1",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN }, /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },    /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_3, BP_DEDICATED_PIN }  /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_14_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,    /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963268V30A =
{
   "963268V30A",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_5, BP_GPIO_17_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with a dthe GPIO_17_AL pin. */
   },
   {
      BP_GPIO_14_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_50_AL     /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_51_AL,    /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88264_TH,
      &g_voiceBoard_SI32260x2,
      &g_voiceBoard_ZL88601,
	  &g_voiceBoard_ZL88701,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168MBV_17A_302 =
{
   "963168MBV17A302",
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_10_AL     /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168MBV_30A_302 =
{
   "963168MBV30A302",   /* Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_10_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168MBV_17A =
{
   "963168MBV_17A",   /* Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_23_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,    /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168MBV_30A =
{
   "963168MBV_30A",   /* Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_23_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168XH =
{
   "963168XH",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_21_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168XH5 =
{
   "963168XH5",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_21_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_35_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168MP =
{
   "963168MP",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_19_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168VX =
{
   "963168VX",   /* Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_5, BP_GPIO_17_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_17_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_9_AL     /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,      /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   /*Dect not supported on this board. */
   BP_DECT_NOT_SUPPORTED,

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168VX_P400 =
{
   "963168VX_P400",   /* Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_5, BP_GPIO_17_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_17_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_9_AL     /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset GPIO pin. */
   BP_GPIO_8_AH,      /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   /*Dect not supported on this board. */
   BP_DECT_NOT_SUPPORTED,

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963268BU =
{
   "963268BU",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_5, BP_GPIO_17_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_17_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_18_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_19_AL,   /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,  /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963268BU_P300 =
{
   "963268BU_P300",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_5, BP_GPIO_17_AL }       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_17_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_18_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_GPIO_19_AL,    /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   BP_DECT_INTERNAL,  /* Internal DECT Config */
   /* Supported Daughter Cards */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168XN5 =
{
   "963168XN5",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B4 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B7 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_18_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_8_AL     /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_DEFINED,    /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* DECT not supported in this board */
   /* Supported Daughter Cards */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88264_TH,
      &g_voiceBoard_SI32260x2,
      &g_voiceBoard_ZL88601,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168WFAR =
{
   "963168WFAR",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B4 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B7 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_10_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_DEFINED,    /* DECT reset GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* DECT not supported in this board */
   /* Supported Daughter Cards */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88264_TH,
      &g_voiceBoard_SI32260x2,
      &g_voiceBoard_ZL88601,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm963168AC5 =
{
   "963168AC5",   /*Base Board ID */
   {
      { SPI_DEV_1, BP_DEDICATED_PIN },   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      { SPI_DEV_4, BP_GPIO_16_AL },      /* This SPI Device ID is mapped to SPI_SS_B4 with the GPIO_16_AL pin. */
      { SPI_DEV_7, BP_GPIO_9_AL }        /* This SPI Device ID is mapped to SPI_SS_B7 with the GPIO_9_AL pin. */
   },
   {
      BP_GPIO_14_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_21_AL    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_DEFINED,    /* DECT reset GPIO pin. */
   BP_GPIO_39_AH,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* DECT not supported in this board */
   /* Supported Daughter Cards */
   {
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88266,
      0
   }
};

/* List of base boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm963268HW_hybrid_VE886,   
   &g_voice_bcm963268HW_w724v_VE886,
   &g_voice_bcm963268BU_P300,
   &g_voice_bcm963268BU,
   &g_voice_bcm963168VX,
   &g_voice_bcm963168VX_P400,
   &g_voice_bcm963268SV,
   &g_voice_bcm963268V30A,
   &g_voice_bcm963168MBV_17A,
   &g_voice_bcm963168MBV_30A,
   &g_voice_bcm963168XH,
   &g_voice_bcm963168XH5,
   &g_voice_bcm963168MP,
   &g_voice_bcm963168VX,
   &g_voice_bcm963168MBV_17A_302,
   &g_voice_bcm963168MBV_30A_302,
   &g_voice_bcm963168XN5,
   &g_voice_bcm963168WFAR,
   &g_voice_bcm963168AC5,
   0
};

#endif

#if defined(_BCM96368_) || defined(CONFIG_BCM96368)

VOICE_BOARD_PARMS_NEW g_voice_bcm96368MVWG =
{
   "96368MVWG",
   {
      {SPI_DEV_2, BP_GPIO_28_AL},   /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_28_AL pin. */
      {SPI_DEV_3, BP_GPIO_29_AL},   /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_29_AL pin. */
      {SPI_DEV_4, BP_NOT_CONNECTED} /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_10_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_10_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset GPIO pin. */
   BP_GPIO_3_AH,      /* Relay 1 GPIO pin. */
   BP_GPIO_13_AH,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter cards. */
   {
      &g_voiceBoard_6368MVWG,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96368MB2G =
{
   "96368MB2G",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_2, BP_GPIO_28_AL},      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_3, BP_GPIO_29_AL}       /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_29_AL pin. */
   },

   {
      BP_GPIO_3_AL,      /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_16_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 3 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE89116,
      &g_voiceBoard_LE89316,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_SI32176,
      &g_voiceBoard_SI32178,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96368MVNgr =
{
   "96368MVNgr",   /*Board ID */
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_2, BP_GPIO_28_AL},     /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_3, BP_GPIO_29_AL}      /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_29_AL pin. */
   },
   {
      BP_GPIO_16_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_17_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_4_AL     /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_DEFINED,   /* DECT reset GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   /* Channel description */
   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32176,
      &g_voiceBoard_SI32178,
      &g_voiceBoard_SI3217x_NOFXO,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE89116,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE89316,
      &g_voiceBoard_VE890HV,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88264_TH,
      0
   }
};

/* List of base boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm96368MVWG,
   &g_voice_bcm96368MVNgr,
   &g_voice_bcm96368MB2G,
   0
};

#endif

#if defined(_BCM96816_) || defined(CONFIG_BCM96816)

VOICE_BOARD_PARMS_NEW g_voice_bcm96816SV =
{
   "96816SV",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN}, /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_2, BP_NOT_CONNECTED}, /* No connection for this SPI Device. */
      {SPI_DEV_3, BP_NOT_CONNECTED}  /* No connection for this SPI Device. */
   },
   {
      BP_NOT_DEFINED,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,   /* FXS2 reset pin, no connection on this board. */
      BP_NOT_CONNECTED   /* FXO reset pin, no connection on this board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3239,
      0
   }
};

/* List of base boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm96816SV,
   0
};

#endif

#if defined(_BCM96818_) || defined(CONFIG_BCM96818)

VOICE_BOARD_PARMS_NEW g_voice_bcm96818G_RG =
{
   "96818G_RG",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_2,  BP_GPIO_28_AL},    /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_3,  BP_GPIO_29_AL}     /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_29_AL pin. */
   },
   {
      BP_GPIO_14_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_16_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin, 6818G_RG doesn't support DECT */
   BP_NOT_DEFINED,   /* Relay1 control pin, 6818G_RG doesn't support relay */
   BP_NOT_DEFINED,   /* Repay2 control pin, 6818G_RG doesn't support relay */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_LE9530,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88506,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96818GR_ONT =
{
   "96818GR_ONT",
   {
      {SPI_DEV_1, BP_NOT_DEFINED},   /*Only uses built-in Le9530 daughter board */
      {SPI_DEV_2, BP_NOT_DEFINED},
      {SPI_DEV_3, BP_NOT_DEFINED}
   },
   {
      BP_NOT_DEFINED,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_DEFINED,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXSO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_LE9530,
      0
   }
};


VOICE_BOARD_PARMS_NEW g_voice_bcm96818SV =
{
   "96818SV",
   {
      {SPI_DEV_1,  BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_2, BP_GPIO_28_AL},      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_3,  BP_GPIO_29_AL}      /* This SPI Device ID is mapped to SPI_SS_B3 with the GPIO_29_AL pin. */
   },
   {
      BP_GPIO_11_AL,   /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_12_AL,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_GPIO_13_AL   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_LE88266x2_LE89010,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96818EGG =
{
   "96818EGG",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},   /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_2, BP_GPIO_16_AL},      /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      {SPI_DEV_3, BP_NOT_CONNECTED}    /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_28_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_5_AL,      /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI32267,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96818GRRG =
{
   "96818GRRG",
   {
      {SPI_DEV_3, BP_GPIO_21_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_21_AL pin. */
      {SPI_DEV_2, BP_GPIO_28_AL},     /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_7, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_16_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_SI32392,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_VE890_INVBOOST,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_SI32267,
      &g_voiceBoard_ZL88601,	  
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96818GRRG_BOSA =
{
   "96818GRRG_BOSA",
   {
      {SPI_DEV_3, BP_GPIO_21_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_21_AL pin. */
      {SPI_DEV_2, BP_GPIO_28_AL},     /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_7, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_16_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* The list of supported daughter boards. */
   {
       &g_voiceBoard_SI3239,
       &g_voiceBoard_SI32392,
       &g_voiceBoard_LE88506,
       &g_voiceBoard_VE890_INVBOOST,
       &g_voiceBoard_LE88536_ZSI,
       &g_voiceBoard_LE88266,
       &g_voiceBoard_SI32261,
       &g_voiceBoard_SI32267,
       0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96818GRRG2L_BOSA =
{
   "96818GRRG2LBOSA",
   {
      {SPI_DEV_3, BP_GPIO_21_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_21_AL pin. */
      {SPI_DEV_2, BP_GPIO_28_AL},     /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_28_AL pin. */
      {SPI_DEV_7, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin. */
   },
   {
      BP_GPIO_16_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_15_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,   /* DECT reset pin */
   BP_NOT_DEFINED,     /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,     /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* The list of supported daughter boards. */
   {
       &g_voiceBoard_LE88506,
       &g_voiceBoard_VE890_INVBOOST,
       &g_voiceBoard_LE88536_ZSI,
       &g_voiceBoard_LE88266,
       &g_voiceBoard_SI32261,
       &g_voiceBoard_SI32267,
       &g_voiceBoard_ZL88601,
       0
   }
};

/* The list of daughter boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm96818G_RG,
   &g_voice_bcm96818SV,
   &g_voice_bcm96818EGG,
   &g_voice_bcm96818GRRG,
   &g_voice_bcm96818GRRG_BOSA,
   &g_voice_bcm96818GRRG2L_BOSA,
   &g_voice_bcm96818GR_ONT,
   0
};

#endif

#if defined(_BCM96828_) || defined(CONFIG_BCM96828)

VOICE_BOARD_PARMS_NEW g_voice_bcm96828EGG =
{
   "96828EGG",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN },  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. */
      {SPI_DEV_4, BP_GPIO_16_AL },     /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      {SPI_DEV_3, BP_NOT_CONNECTED }     /* This SPI Device ID is not connected */
   },
   {
      BP_GPIO_17_AL,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_22_AL,    /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED  /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED, /* DECT reset pin */
   BP_NOT_DEFINED,   /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,   /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE9530_LE88506,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE9530_WB,
      &g_voiceBoard_ZL88601,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96828HGW =
{
   "96828HGW",
   {
      { SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin. This is the mapping for P202 and later versions of this board. */
      { SPI_DEV_4, BP_GPIO_16_AL},    /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_16_AL pin. */
      { SPI_DEV_3, BP_NOT_CONNECTED}  /* This SPI Device ID is not connected. */
   },
   {
      BP_GPIO_17_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_22_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE9530_LE88506,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE9530_WB,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96821GF_SFU_F11 =
{
   "96821GF_SFU_F11",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin.  */
      {SPI_DEV_2, BP_NOT_CONNECTED},  /* This SPI Device ID is not connected. */
      {SPI_DEV_3, BP_NOT_CONNECTED}   /* This SPI Device ID is not connected. */
   },
   {
      BP_NOT_DEFINED,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,  /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter cards */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_SI32392,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96821GF_SFU_G11 =
{
   "96821GF_SFU_G11",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin.  */
      {SPI_DEV_2, BP_NOT_CONNECTED},  /* This SPI Device ID is not connected.  */
      {SPI_DEV_3, BP_NOT_CONNECTED}   /* This SPI Device ID is not connected.  */
      },
   {
      BP_NOT_DEFINED,    /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,  /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter cards. */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_SI32392,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96821GF_SFU_F21 =
{
   "96821GF_SFU_F21",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin.  */
      {SPI_DEV_2, BP_NOT_CONNECTED},  /* This SPI Device ID is not connected.  */
      {SPI_DEV_3, BP_NOT_CONNECTED}   /* This SPI Device ID is not connected.  */
   },
   {
      BP_NOT_DEFINED,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_SI32392,
    0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96821GF_SFU_G21 =
{
   "96821GF_SFU_G21",
   {
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B1 with a dedicated pin.  */
      {SPI_DEV_2, BP_NOT_CONNECTED},  /* This SPI Device ID is not connected.  */
      {SPI_DEV_3, BP_NOT_CONNECTED}   /* This SPI Device ID is not connected.  */
   },
   {
      BP_NOT_DEFINED,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,   /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED    /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED, /* Internal DECT configuration */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_SI32392,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96828HGW_F42 =
{
   "96828HGW_F42",
   {
      { SPI_DEV_4, BP_GPIO_16_AL},    /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_16_AL pin.  */
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      BP_GPIO_17_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_22_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE9530_WB,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96828HGW_G42 =
{
   "96828HGW_G42",
   {
      {SPI_DEV_4, BP_GPIO_16_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_16_AL pin.  */
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      BP_GPIO_17_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_22_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE9530_WB,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96828HGW_F21 =
{
   "96828HGW_F21",
   {
      {SPI_DEV_4, BP_GPIO_16_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_16_AL pin.  */
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      BP_GPIO_17_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_22_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE9530_WB,
      0
    }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96828HGW_G21 =
{
   "96828HGW_G21",
   {
      {SPI_DEV_4, BP_GPIO_16_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_16_AL pin.  */
      {SPI_DEV_1, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      BP_GPIO_17_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_22_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_SI32267,
      &g_voiceBoard_LE88536_ZSI,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_SI32261,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE9530,
      &g_voiceBoard_LE9530_WB,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm96828SV =
{
   "96828SV",
   {
      {SPI_DEV_4, BP_GPIO_16_AL},     /* This SPI Device ID is mapped to SPI_SS_B1 with the GPIO_16_AL pin.  */
      {SPI_DEV_5, BP_GPIO_17_AL},     /* This SPI Device ID is mapped to SPI_SS_B2 with the GPIO_17_AL pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      BP_GPIO_36_AL,     /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_GPIO_37_AL,     /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards */
   {
      &g_voiceBoard_SI3239,
      &g_voiceBoard_SI32392,
      &g_voiceBoard_SI3217x,
      &g_voiceBoard_LE88506,
      &g_voiceBoard_LE88266,
      &g_voiceBoard_LE88536_ZSI,
      0
   }
};

/* List of base boards in this family of chips. */
static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm96828HGW,
   &g_voice_bcm96828EGG,
   &g_voice_bcm96821GF_SFU_F11,
   &g_voice_bcm96821GF_SFU_G11,
   &g_voice_bcm96821GF_SFU_F21,
   &g_voice_bcm96821GF_SFU_G21,
   &g_voice_bcm96828HGW_F42,
   &g_voice_bcm96828HGW_G42,
   &g_voice_bcm96828HGW_F21,
   &g_voice_bcm96828HGW_G21,
   &g_voice_bcm96828SV,
   0
};

#endif

#if defined(_BCM968500_) || defined(CONFIG_BCM968500)
VOICE_BOARD_PARMS_NEW g_voice_bcm968500LILAC =
{
   "968500CHERRY",
   {
      {SPI_DEV_0, 40},                /* This SPI Device ID is mapped to GPIO 40 pin.  */
      {SPI_DEV_5, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      30,                /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,  /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_LE88266,
      0
   }
};

VOICE_BOARD_PARMS_NEW g_voice_bcm968571_E8C =
{
   "968751LUPINE2",
   {
      {SPI_DEV_0, 40},                /* This SPI Device ID is mapped to GPIO 40 pin.  */
      {SPI_DEV_5, BP_DEDICATED_PIN},  /* This SPI Device ID is mapped to SPI_SS_B2 with a dedicated pin.  */
      {SPI_DEV_3, BP_DEDICATED_PIN}   /* This SPI Device ID is mapped to SPI_SS_B3 with a dedicated pin.  */
   },
   {
      32,                /* FXS1 reset pin, mapped to the FXS1 reset pin on daughter board. */
      BP_NOT_CONNECTED,  /* FXS2 reset pin, mapped to the FXS2 reset pin on daughter board. */
      BP_NOT_CONNECTED   /* FXO reset pin, mapped to the FXO reset pin on daughter board. */
   },

   BP_NOT_CONNECTED,  /* DECT reset pin */
   BP_NOT_DEFINED,    /* Relay 1 GPIO pin. */
   BP_NOT_DEFINED,    /* Relay 2 GPIO pin. */

   BP_DECT_NOT_SUPPORTED,  /* Internal DECT Config */

   /* List of supported daughter boards. */
   {
      &g_voiceBoard_LE88266,
      0
   }
};

static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   &g_voice_bcm968500LILAC,
   &g_voice_bcm968571_E8C,
   0
};

#endif

#if !defined(_BCM963268_) && !defined(CONFIG_BCM963268) && !defined(_BCM96362_) && !defined(CONFIG_BCM96362) && !defined(_BCM96328_) && !defined(CONFIG_BCM96328) && !defined(_BCM96368_) && !defined(CONFIG_BCM96368) && !defined(_BCM96816_) && !defined(CONFIG_BCM96816) && !defined(_BCM96828_) && !defined(CONFIG_BCM96828) && !defined(_BCM96818_) && !defined(CONFIG_BCM96818) && !defined(_BCM968500_) && !defined(CONFIG_BCM968500)

static PVOICE_BOARD_PARMS_NEW g_VoiceBoardParms_new[]=
{
   0
};

#endif

static VOICE_BOARD_PARMS voiceBoard_Current;
static PVOICE_BOARD_PARMS_NEW g_pCurrentVoiceBp = 0;
static PVOICE_DAUGHTER_BOARD_PARMS g_pCurrentVoiceDc = 0;
static int g_BpDectPopulated = 1;

static void bpmemcpy( void* dstptr, const void* srcptr, int size );
static char * bpstrcpy( char* dest, const char* src );

static void bpmemcpy( void* dstptr, const void* srcptr, int size )
{
   char* dstp = dstptr;
   const char* srcp = srcptr;
   int i;
   for( i=0; i < size; i++ )
   {
      *dstp++ = *srcp++;
   }
}

static char * bpstrcpy( char* dest, const char* src)
{
   while(*src)
   {
      *dest++ = *src++;
   }

   *dest = '\0';

   return dest;
}

/*****************************************************************************
 * Name:          BpGetZSISpiDevID()
 *
 * Description:     This function returns the SPI Device ID for the ZSI daughter
*                   boards based on the current chip.
 *
 * Parameters:    Nothing
 *
 * Returns:       SPI Dev ID for ZSI Daughter Boards
 *
 *****************************************************************************/
unsigned int BpGetZSISpiDevID( void )
{
#ifdef ZSI_SPI_DEV_ID
   return ZSI_SPI_DEV_ID;
#else
   return BP_NOT_DEFINED;
#endif
}

/*****************************************************************************
 * Name:          BpSetDectPopulatedData()
 *
 * Description:     This function sets the g_BpDectPopulated variable. It is
 *                used for the user to specify in the board parameters if the
 *                board DECT is populated or not (1 for populated, 0 for not).
 *
 * Parameters:    int BpData - The data that g_BpDectPopulated will be set to.
 *
 * Returns:       Nothing
 *
 *****************************************************************************/
void BpSetDectPopulatedData( int BpData )
{
   g_BpDectPopulated = BpData;
}

/*****************************************************************************
 * Name: 	      BpGetVoiceDectNum()
 *
 * Description:	  This function returns the number of DECT channels. It is
 *                used to fill the old board parameters structure with DECT
 *                data. If the user specifies that DECT is not populated on the
 *                board, it will return that there are zero DECT lines.
 *
 * Parameters:    ppBpVoice - The voice board parameters structure.
 *
 * Returns:       The number of DECT lines in passed-in board.
 *
 *****************************************************************************/
unsigned int BpGetVoiceDectNum(PVOICE_BOARD_PARMS_NEW *ppBpVoice)
{
   unsigned int iCnt=0;

   while ( (*ppBpVoice)->dectChDes[iCnt].status != BP_VOICE_CHANNEL_NONE )
   {
      iCnt++;
   }

   if(BpDectPopulated() == BP_DECT_NOT_POPULATED)
   {
      return 0;
   }

   return iCnt;
}


/*****************************************************************************
 * Name: 	      BpDectPopulated()
 *
 * Description:	  This function is used to determine if DECT is populated on
 * 				  the board.
 *
 * Parameters:    None
 *
 * Returns:       BP_DECT_POPULATED if DECT is populated, otherwise it will
 *                return BP_DECT_NOT_POPULATED.
 *
 *****************************************************************************/
int BpDectPopulated( void )
{
   return (g_BpDectPopulated ? BP_DECT_POPULATED:BP_DECT_NOT_POPULATED);
}

/*****************************************************************************
 * Name:          BpGetVoiceParms()
 *
 * Description:     Finds the voice parameters based on the daughter board and
 *                base board IDs and fills the old parameters structure with
 *                information.
 *
 * Parameters:    pszBoardId - The daughter board ID that is being used.
 *                voiceParms - The old voice parameters structure that must be
 *                             filled with data from the new structure.
 *                pszBaseBoardId - The base board ID that is being used.
 *
 * Returns:       If the board is not found, returns BP_BOARD_ID_NOT_FOUND.
 *                If everything goes properly, returns BP_SUCCESS.
 *
 *****************************************************************************/
int BpGetVoiceParms( char* pszBoardId, VOICE_BOARD_PARMS* voiceParms, char* pszBaseBoardId )
{
   int nRet = BP_BOARD_ID_NOT_FOUND;
   int relayCount = 0;
   int i = 0;
   int nDeviceCount = 0;
   PVOICE_BOARD_PARMS_NEW *ppBp;
   PVOICE_DAUGHTER_BOARD_PARMS *ppDc;
   VOICE_BOARD_PARMS currentVoiceParms;

   /* Must first go through the list of base boards and find one that matches */
   for( ppBp = g_VoiceBoardParms_new; *ppBp; ppBp++ )
   {
      if( (0 == bpstrcmp((*ppBp)->szBaseBoardId, pszBaseBoardId)))
      {
         /* Found the matching board */
         break;
      }
   }

   if(!(*ppBp))
   {
      /* No matching base board found */
      return nRet;
   }

   /* Found matching base board; now, must iterate through supported daughter cards to find a match. */
   for(ppDc = (*ppBp)->pDcParms; *ppDc; ppDc++)
   {
      if( (0 == bpstrcmp((*ppDc)->szBoardId, pszBoardId)))
      {
         /*Succesfully found base board + daughter card combination
         Must now fill the currentVoiceParms structure with data and copy to voiceParms
         First set base board and daughter board strings */
         bpmemcpy(currentVoiceParms.szBoardId, (*ppDc)->szBoardId, BP_BOARD_ID_LEN);
         bpmemcpy(currentVoiceParms.szBaseBoardId, (*ppBp)->szBaseBoardId, BP_BOARD_ID_LEN);

         /*Set the FXS and FXO line numbers. */
         currentVoiceParms.numFxsLines = (*ppDc)->numFxsLines;
         currentVoiceParms.numFxoLines = (*ppDc)->numFxoLines;

         /*Set the number of DECT Lines. */
         currentVoiceParms.numDectLines = BpGetVoiceDectNum(ppBp);

         /*This prevents the total number of channels from being greater than 7. */
         if(currentVoiceParms.numFxsLines + currentVoiceParms.numFxoLines + currentVoiceParms.numDectLines > 7)
         {
            if(currentVoiceParms.numDectLines == 4)
            {
               /* If there are four DECT lines and it is exceeding limit, can
                * cut two of the DECT lines for board/daughter card combinations
                * with 4 FXS lines such as 963268V30A with Si32260x2.*/
               currentVoiceParms.numDectLines = 2;
            }
            else
            {
               return BP_MAX_CHANNELS_EXCEEDED; /* Return a failure. */
            }
         }

         /*Set the relay GPIO pins*/
         currentVoiceParms.pstnRelayCtrl.relayGpio[0] = (*ppBp)->relay1CtrlGpio;
         currentVoiceParms.pstnRelayCtrl.relayGpio[1] = (*ppBp)->relay2CtrlGpio;

         if((*ppBp)->relay1CtrlGpio != BP_NOT_DEFINED)
         {
            relayCount++;
         }

         if((*ppBp)->relay2CtrlGpio != BP_NOT_DEFINED)
         {
            relayCount++;
         }

         currentVoiceParms.numFailoverRelayPins = relayCount;

         /*Set DECT UART to Not Defined always for now. */
         currentVoiceParms.dectUartControl.dectUartGpioTx = BP_NOT_DEFINED;
         currentVoiceParms.dectUartControl.dectUartGpioRx = BP_NOT_DEFINED;

         /* Set the device profile */
         currentVoiceParms.deviceProfile = (*ppDc)->deviceProfile;

         /*Set the flags*/
         currentVoiceParms.flags = (*ppDc)->flags;

         /*Set DECT*/
         if(currentVoiceParms.numDectLines)
         {
            currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType = currentVoiceParms.voiceDevice[nDeviceCount+1].voiceDeviceType = BP_VD_IDECT1;
            currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = currentVoiceParms.voiceDevice[nDeviceCount+1].spiCtrl.spiDevId = 0;
            currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = currentVoiceParms.voiceDevice[nDeviceCount+1].spiCtrl.spiGpio = BP_NOT_DEFINED;
            currentVoiceParms.voiceDevice[nDeviceCount].requiresReset = currentVoiceParms.voiceDevice[nDeviceCount+1].requiresReset = 1;
            currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = currentVoiceParms.voiceDevice[nDeviceCount+1].resetGpio = (*ppBp)->dectRstGpio;

            switch(currentVoiceParms.numDectLines)
            {
               case 1:
               case 2:
               {
                  bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount].channel[nDeviceCount],&(*ppBp)->dectChDes[0],sizeof(BP_VOICE_CHANNEL)*currentVoiceParms.numDectLines);
                  nDeviceCount++;
               }
               break;

               case 3:
               case 4:
               {
                  bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount].channel[nDeviceCount], &(*ppBp)->dectChDes[0], sizeof(BP_VOICE_CHANNEL)*2);
                  bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount+1].channel[nDeviceCount],&(*ppBp)->dectChDes[1],sizeof(BP_VOICE_CHANNEL)*2);
                  nDeviceCount+=2;
               }
               break;
               default:
               {
                  /* Return a failure */
                  return BP_MAX_CHANNELS_EXCEEDED;
               }
               break;
            }
         }

         while(i < BP_MAX_VOICE_DEVICES && (*ppDc)->voiceDevice[i].nDeviceType != BP_VD_NONE)
         {
            /*Loop through the voice devices and copy to currentVoiceParms*/

            currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType = (*ppDc)->voiceDevice[i].nDeviceType;

            if((*ppDc)->voiceDevice[i].nRstPin == BP_RESET_NOT_REQUIRED || (*ppBp)->fxsoRstGpio[(*ppDc)->voiceDevice[i].nRstPin] == BP_RESET_NOT_REQUIRED)
            {
               currentVoiceParms.voiceDevice[nDeviceCount].requiresReset = 0;
               currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = BP_NOT_DEFINED;
            }
            else
            {
               currentVoiceParms.voiceDevice[nDeviceCount].requiresReset = 1;
               currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = (*ppBp)->fxsoRstGpio[(*ppDc)->voiceDevice[i].nRstPin];
            }

            currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = (*ppBp)->voiceSpiPortCs[(*ppDc)->voiceDevice[i].nSPI_SS_Bx].numSpiPort;
            currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio  = ((*ppBp)->voiceSpiPortCs[(*ppDc)->voiceDevice[i].nSPI_SS_Bx].numGpio == BP_DEDICATED_PIN) ? BP_NOT_DEFINED : (*ppBp)->voiceSpiPortCs[(*ppDc)->voiceDevice[i].nSPI_SS_Bx].numGpio;

            /* Handle the ZSI devices */
            if((*ppDc)->voiceDevice[i].nSPI_SS_Bx == BP_SPI_SS_NOT_REQUIRED)
            {
               /* Current device is a ZSI device. */
               currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = BpGetZSISpiDevID();

               if(currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId == BP_NOT_DEFINED)
               {
                  /* Failure - Tried to use a ZSI/ISI chip on a board which does not support it*/
                  return BP_NO_ZSI_ON_BOARD_ERR;
               }

               currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = BP_NOT_DEFINED;
            }

            /* Handle Le9530 and Si3239, which are internal devices */
            switch(currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType)
            {
               case BP_VD_ZARLINK_9530:
               {
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = 0;
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = BP_NOT_DEFINED;
               }
               break;
               case BP_VD_SILABS_3239:
               {
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = 1;
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = BP_NOT_DEFINED;
               }
               default:
               break;
            }

            bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount].channel[0], &((*ppDc)->voiceDevice[i].channel[0]), sizeof(BP_VOICE_CHANNEL)*2);

            i++;
            nDeviceCount++;
         }

         /*Add a NULL Device*/
         currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType=BP_VD_NONE;

         voiceBoard_Current = currentVoiceParms;
         g_pCurrentVoiceBp = *ppBp;
         g_pCurrentVoiceDc = *ppDc;
         bpmemcpy( voiceParms, &currentVoiceParms, sizeof(VOICE_BOARD_PARMS) );
         nRet = BP_SUCCESS;

         break;
      }
   }

   return( nRet );
}

/**************************************************************************
* Name       : BpSetVoiceBoardId
*
* Description: This function find the BOARD_PARAMETERS structure for the
*              specified board id string and assigns it to a global, static
*              variable.
*
* Parameters : [IN] pszBoardId - Board id string that is saved into NVRAM.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_FOUND - Error, board id input string does not
*                  have a board parameters configuration record.
***************************************************************************/
int BpSetVoiceBoardId( char *pszBoardId )
{
   int nRet = BP_BOARD_ID_NOT_FOUND;
   PVOICE_DAUGHTER_BOARD_PARMS *ppDc;
   PVOICE_BOARD_PARMS_NEW *ppBpVoice;

   for( ppBpVoice = g_VoiceBoardParms_new; *ppBpVoice; ppBpVoice++ )
   {
      for(ppDc = (*ppBpVoice)->pDcParms; *ppDc; ppDc++)
      {
         if( 0 == bpstrcmp((*ppDc)->szBoardId, pszBoardId) )
         {
            bpmemcpy(voiceBoard_Current.szBoardId,pszBoardId,BP_BOARD_ID_LEN);
            g_pCurrentVoiceDc = *ppDc;
            nRet = BP_SUCCESS;

            return nRet;
         }
      }
    }

    return( nRet );
} /* BpSetVoiceBoardId */


/**************************************************************************
* Name       : BpGetVoiceBoardId
*
* Description: This function returns the current board id strings.
*
* Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
*                  string is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
***************************************************************************/

int BpGetVoiceBoardId( char *pszBoardId )
{
   int i;

   if (g_pCurrentVoiceDc == 0)
   {
      return -1;
   }

   for (i = 0; i < BP_BOARD_ID_LEN; i++)
   {
      pszBoardId[i] = g_pCurrentVoiceDc->szBoardId[i];
   }

   return 0;
}


/**************************************************************************
* Name       : BpGetVoiceBoardIds
*
* Description: This function returns all of the supported voice board id strings.
*
* Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
*                  strings are returned in.  Each id starts at BP_BOARD_ID_LEN
*                  boundary.
*              [IN] nBoardIdsSize - Number of BP_BOARD_ID_LEN elements that
*                  were allocated in pszBoardIds.
*              [IN] pszBaseBoardId - Name of base Board ID to associate Voice
*                  Board ID with.
*
* Returns    : Number of board id strings returned.
***************************************************************************/
int BpGetVoiceBoardIds( char *pszBoardIds, int nBoardIdsSize, char *pszBaseBoardId )
{
   PVOICE_BOARD_PARMS_NEW *ppBp;
   PVOICE_DAUGHTER_BOARD_PARMS *ppDc;
   int i;
   char *src;
   char *dest;

   for( i = 0, ppBp = g_VoiceBoardParms_new; *ppBp; ppBp++)
   {
      if( 0 == bpstrcmp((*ppBp)->szBaseBoardId, pszBaseBoardId))
      {
         break;
      }
   }

   if(!(*ppBp))
   {
      return 0;
   }

   for( i = 0, ppDc = (*ppBp)->pDcParms; *ppDc && nBoardIdsSize; ppDc++)
   {
      dest = pszBoardIds;

      src = (*ppDc)->szBoardId;

      bpstrcpy(dest, src);

      i++;
      pszBoardIds += BP_BOARD_ID_LEN;
      nBoardIdsSize--;
   }

   return( i );
} /* BpGetVoiceBoardIds */

/**************************************************************************
* Name       : BpGetVoiceDectType
*
* Description: This function returns whether or not Dect is supported on a given board.
*
* Parameters : [IN] pszBoardId - Name of the base Board ID
*
* Returns    : Status indicating if the base board supports dect.
***************************************************************************/
int BpGetVoiceDectType( char *pszBoardId )
{
   PVOICE_BOARD_PARMS_NEW *ppBp;
   int dectStatus = BP_VOICE_NO_DECT;

   for(ppBp = g_VoiceBoardParms_new; *ppBp; ppBp++)
   {
      if( 0 == bpstrcmp((*ppBp)->szBaseBoardId, pszBoardId))
      {
         break;
      }
   }

   if(!(*ppBp))
   {
        return dectStatus;
   }

   if((*ppBp)->dectChDes[0].status != BP_VOICE_CHANNEL_NONE)
   {
      dectStatus = BP_VOICE_INT_DECT;
   }

   return dectStatus;
}

#else
/* ---- Include Files ---------------------------------------------------- */

#include <linux/kernel.h> 
#include "boardparms_voice.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* Always end the device list in VOICE_BOARD_PARMS with this macro */
#define BP_NULL_DEVICE_MACRO     \
{                                \
   BP_VD_NONE,                   \
   {  0, BP_NOT_DEFINED },       \
   0,                            \
   BP_NOT_DEFINED,               \
   {                             \
      { BP_VOICE_CHANNEL_INACTIVE, BP_VCTYPE_NONE, BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_NARROWBAND, BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS, BP_VOICE_CHANNEL_ENDIAN_BIG, BP_TIMESLOT_INVALID, BP_TIMESLOT_INVALID }, \
      { BP_VOICE_CHANNEL_INACTIVE, BP_VCTYPE_NONE, BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_NARROWBAND, BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS, BP_VOICE_CHANNEL_ENDIAN_BIG, BP_TIMESLOT_INVALID, BP_TIMESLOT_INVALID }, \
   }                             \
}

#define BP_NULL_CHANNEL_MACRO             \
{  BP_VOICE_CHANNEL_INACTIVE,             \
   BP_VCTYPE_NONE,                        \
   BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,    \
   BP_VOICE_CHANNEL_NARROWBAND,           \
   BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,   \
   BP_VOICE_CHANNEL_ENDIAN_BIG,           \
   BP_TIMESLOT_INVALID,                   \
   BP_TIMESLOT_INVALID                    \
},


#if defined(_BCM96328_) || defined(CONFIG_BCM96328)

VOICE_BOARD_PARMS voiceBoard_96328AVNG_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_96328AVNG_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_96328AVNG_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_96328AVNG_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,    

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_SI32261 =
{
   VOICECFG_SI32261_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }    
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            6,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,   

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_LE88276_NTR =
{
   VOICECFG_LE88276_NTR_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,    

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_VE890HV_Partial =
{
   VOICECFG_VE890HV_PARTIAL_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,                /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Reset pin */
         BP_GPIO_30_AL,    

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNG_VE890HV =
{
   VOICECFG_VE890HV_STR,   /* szBoardId */
   "96328avng",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89136,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,                /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Reset pin */
         BP_GPIO_30_AL,    

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_31_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,     

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "96328avngr",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96328avngr",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,       /* Reset line shared to one line only for AVNGR board */

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_SI32261 =
{
   VOICECFG_SI32261_STR,   /* szBoardId */
   "96328avngr",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "96328avngr",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            6,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,   

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "96328avngr",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_LE88536_TH =
{
   VOICECFG_LE88536_TH_STR,   /* szBoardId */
   "96328avngr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96328AVNGR_LE88264_TH =
{
   VOICECFG_LE88264_TH_STR,   /* szBoardId */
   "96328avngr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88264,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "963293epon",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "963293epon",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963293epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963293epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963293EPON_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963293epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_QCUK,   

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963293EPON_SI3217X_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* szBoardId */
   "963293epon",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,       /* Reset line shared to one line only for AVNGR board */

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* No DAA channel on 63293epon board */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_GPHY_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "963293epon_gphy",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_GPHY_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "963293epon_gphy",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_GPHY_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963293epon_gphy",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963293EPON_GPHY_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963293epon_gphy",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963293EPON_GPHY_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963293epon_gphy",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_QCUK,   

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963293EPON_GPHY_SI3217X_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* szBoardId */
   "963293epon_gphy",
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,       /* Reset line shared to one line only for AVNGR board */

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* No DAA channel on 63293epon board */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

static PVOICE_BOARD_PARMS g_VoiceBoardParms[] = 
{
   &voiceBoard_96328AVNG_LE88276,
   &voiceBoard_96328AVNG_SI3226,
   &voiceBoard_96328AVNG_VE890,
   &voiceBoard_96328AVNG_VE890_INVBOOST,
   &voiceBoard_96328AVNG_SI3217X,
   &voiceBoard_96328AVNG_LE88506,
#ifdef SI32261ENABLE
   &voiceBoard_96328AVNG_SI32261,
#endif   
#ifdef SI32267ENABLE
   &voiceBoard_96328AVNG_SI32267,
#endif
   &voiceBoard_96328AVNG_LE88276_NTR,
   &voiceBoard_96328AVNG_VE890HV_Partial,
   &voiceBoard_96328AVNG_VE890HV,

   &voiceBoard_96328AVNGR_SI32176,
   &voiceBoard_96328AVNGR_SI3217X,
   &voiceBoard_96328AVNGR_LE89116,
   &voiceBoard_96328AVNGR_LE88536_TH,
   &voiceBoard_96328AVNGR_LE88264_TH,
   &voiceBoard_96328AVNGR_SI32261,
   &voiceBoard_96328AVNGR_SI32267,

   &voiceBoard_963293EPON_SI32176,
   &voiceBoard_963293EPON_SI3217X_NOFXO,
   &voiceBoard_963293EPON_LE89116,
   &voiceBoard_963293EPON_LE88506,
   &voiceBoard_963293EPON_LE88276,
   &voiceBoard_963293EPON_SI3226,

   &voiceBoard_963293EPON_GPHY_SI32176,
   &voiceBoard_963293EPON_GPHY_LE89116,
   &voiceBoard_963293EPON_GPHY_LE88506,
   &voiceBoard_963293EPON_GPHY_LE88276,
   &voiceBoard_963293EPON_GPHY_SI3226,
   &voiceBoard_963293EPON_GPHY_SI3217X_NOFXO,
   0
};

#endif

#if defined(_BCM96362_) || defined(CONFIG_BCM96362)

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_NOSLIC =
{
   VOICECFG_NOSLIC_STR,   /* szBoardId */
   "96362ADVNgr",
   0,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_NOT_DEFINED,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96362ADVNgr",
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "96362ADVNgr",
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI32178 =
{
   VOICECFG_SI32178_STR,   /* szBoardId */
   "96362ADVNgr",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI3217X_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* szBoardId */
   "96362ADVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 32176 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96362ADVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* szBoardId */
   "96362ADVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "96362ADVNgr",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_LE89316 =
{
   VOICECFG_LE89316_STR,   /* szBoardId */
   "96362ADVNgr",  
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96362ADVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_LE88536_TH =
{
   VOICECFG_LE88536_TH_STR,   /* szBoardId */
   "96362ADVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_LE88264_TH =
{
   VOICECFG_LE88264_TH_STR,   /* szBoardId */
   "96362ADVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88264,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96362ADVNgr",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96362ADVNgr",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI32261 =
{
   VOICECFG_SI32261_STR,   /* szBoardId */
   "96362ADVNgr",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "96362ADVNgr",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_VE890HVP =
{
   VOICECFG_VE890HV_PARTIAL_STR,   /* szBoardId */
   "96362ADVNgr",    
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,               /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Reset pin */
         BP_GPIO_28_AL,   /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNgr_VE890HV =
{
   VOICECFG_VE890HV_STR,   /* szBoardId */
   "96362ADVNgr",    
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89136,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNGR2_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96362ADVNgr2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNGR2_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96362ADVNgr2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNGR2_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96362ADVNgr2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNGR2_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96362ADVNgr2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVNGR2_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96362ADVNgr2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVN2XH_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96362ADVN2xh",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVN2XH_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96362ADVN2xh",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVN2XH_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96362ADVN2xh",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVN2XH_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96362ADVN2xh",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362ADVN2XH_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96362ADVN2xh",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_30_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_44_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "963629epon",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_SI3217X_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* szBoardId */
   "963629epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 32176 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "963629epon",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963629epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963629epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,    

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963629epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_QCUK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_SI32261 =
{
   VOICECFG_SI32261_STR,   /* szBoardId */
   "963629epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96362EPON_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "963629epon",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_29_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,   

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96361XF_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96361XF",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96361XF_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96361XF",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_31_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96361XF_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96361XF",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96361XF_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96361XF",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96361XF_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96361XF",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_28_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

static PVOICE_BOARD_PARMS g_VoiceBoardParms[] = 
{
   &voiceBoard_96362ADVNgr_NOSLIC,
   &voiceBoard_96362ADVNgr_SI3217X,
   &voiceBoard_96362ADVNgr_SI32176,
   &voiceBoard_96362ADVNgr_SI32178,
   &voiceBoard_96362ADVNgr_SI3217X_NOFXO,   
   &voiceBoard_96362ADVNgr_VE890,
   &voiceBoard_96362ADVNgr_VE890_INVBOOST,
   &voiceBoard_96362ADVNgr_LE89116,
   &voiceBoard_96362ADVNgr_LE89316,
   &voiceBoard_96362ADVNgr_LE88506,
   &voiceBoard_96362ADVNgr_LE88536_TH,
   &voiceBoard_96362ADVNgr_LE88264_TH,
   &voiceBoard_96362ADVNgr_LE88276,
   &voiceBoard_96362ADVNgr_SI3226,   
#ifdef SI32261ENABLE
   &voiceBoard_96362ADVNgr_SI32261,
#endif
#ifdef SI32267ENABLE
   &voiceBoard_96362ADVNgr_SI32267,
#endif
   &voiceBoard_96362ADVNgr_VE890HVP,
   &voiceBoard_96362ADVNgr_VE890HV,
   
   &voiceBoard_96362ADVNGR2_SI3217X,
   &voiceBoard_96362ADVNGR2_VE890,
   &voiceBoard_96362ADVNGR2_LE88506,
   &voiceBoard_96362ADVNGR2_LE88276,
   &voiceBoard_96362ADVNGR2_SI3226,

   &voiceBoard_96362ADVN2XH_SI3217X,
   &voiceBoard_96362ADVN2XH_VE890,
   &voiceBoard_96362ADVN2XH_LE88506,
   &voiceBoard_96362ADVN2XH_LE88276,
   &voiceBoard_96362ADVN2XH_SI3226,
      
   /* EPON should not have any FXO related configs */
   &voiceBoard_96362EPON_SI32176,
   &voiceBoard_96362EPON_SI3217X_NOFXO,
   &voiceBoard_96362EPON_LE89116,
   &voiceBoard_96362EPON_LE88506,
   &voiceBoard_96362EPON_LE88276,
   &voiceBoard_96362EPON_SI3226,
#ifdef SI32261ENABLE
   &voiceBoard_96362EPON_SI32261,
#endif
#ifdef SI32267ENABLE
   &voiceBoard_96362EPON_SI32267,
#endif

   &voiceBoard_96361XF_SI3217X,
   &voiceBoard_96361XF_VE890,
   &voiceBoard_96361XF_LE88506,
   &voiceBoard_96361XF_LE88276,
   &voiceBoard_96361XF_SI3226,
        
   0
};

#endif

#if defined(_BCM963268_) || defined(CONFIG_BCM963268)


VOICE_BOARD_PARMS voiceBoard_963268_W724v_VE890 =
{
   VOICECFG_VE890_HUAWEI_STR,   /* szBoardId */
   "W724v",  
   6,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
       /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               7,
               7
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
    /* voiceDevice3 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
      /* voiceDevice4 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

    /* voiceDevice5 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268_W724v_VE886 =
{
   VOICECFG_VE886_HUAWEI_STR,   /* szBoardId */
   "W724v",  
   6,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               7,
               7            
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
    /* voiceDevice3 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice4 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },


    /* voiceDevice5 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};
/* START ADD: l81004871 FOR Adapter HYBRID 2013-06-17 */
VOICE_BOARD_PARMS voiceBoard_963268_Hybrid_VE890 =
{
   VOICECFG_VE890_HUAWEI_STR,   /* szBoardId */
   "Hybrid",  
   6,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
       /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               7,
               7
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
    /* voiceDevice3 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
      /* voiceDevice4 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

    /* voiceDevice5 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268_Hybrid_VE886 =
{
   VOICECFG_VE886_HUAWEI_STR,   /* szBoardId */
   "Hybrid",  
   6,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               7,
               7            
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },
      
    /* voiceDevice3 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice4 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },


    /* voiceDevice5 parameters */
    {
         /* Device type */
         BP_VD_ZARLINK_88601,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};
/* END ADD: l81004871 FOR Adapter HYBRID 2013-06-17 */

VOICE_BOARD_PARMS voiceBoard_963268SV_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "963268SV1",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268SV_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "963268SV1",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268_HG2xx_VE890 =
{
   VOICECFG_VE890_HUAWEI_STR,   /* szBoardId */
   "963268_hg658b",  
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_18_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268SV_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963268SV1",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268SV_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963268SV1",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268SV_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963268SV1",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963268SV_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "963268SV1",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268SV_LE88536_ZSI =
{
   VOICECFG_LE88536_ZSI_STR,   /* szBoardId */
   "963268SV1",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963268MBV_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "963268MBV",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_LE88536_ZSI =
{
   VOICECFG_LE88536_ZSI_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268MBV_LE88276_NTR =
{
   VOICECFG_LE88276_NTR_STR,   /* szBoardId */
   "963268MBV",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_35_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963168VX_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "963168VX",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963168VX_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "963168VX",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963168VX_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963168VX",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963168VX_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963168VX",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963168VX_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963168VX",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963168VX_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "963168VX",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963168VX_LE88536_ZSI =
{
   VOICECFG_LE88536_ZSI_STR,   /* szBoardId */
   "963168VX",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            4,
            /* SPI GPIO */
            BP_GPIO_16_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_15_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_963268BU_SI32267 =
{
   VOICECFG_SI32267_STR,   /* szBoardId */
   "963268BU",    
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32267,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,

   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_LE88536_ZSI =
{
   VOICECFG_LE88536_ZSI_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST, 

   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_963268BU_SI32261 =
{
   VOICECFG_SI32261_STR,   /* szBoardId */
   "963268BU",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   4,             /* numDectLines */
   1,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_19_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      
      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_39_AH,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

static PVOICE_BOARD_PARMS g_VoiceBoardParms[] = 
{
   &voiceBoard_963268_W724v_VE886,
   &voiceBoard_963268_W724v_VE890,
   &voiceBoard_963268_Hybrid_VE886,
   &voiceBoard_963268_Hybrid_VE890,   
   &voiceBoard_963268SV_SI3217X,
   &voiceBoard_963268SV_VE890,
   &voiceBoard_963268_HG2xx_VE890,
   &voiceBoard_963268SV_LE88506,
   &voiceBoard_963268SV_LE88276,
   &voiceBoard_963268SV_SI3226,
   &voiceBoard_963268SV_SI32267,
   &voiceBoard_963268SV_LE88536_ZSI,
   
   &voiceBoard_963268MBV_SI3217X,
   &voiceBoard_963268MBV_VE890,
   &voiceBoard_963268MBV_LE88506,
   &voiceBoard_963268MBV_LE88276,
   &voiceBoard_963268MBV_SI3226,
   &voiceBoard_963268MBV_SI32267,
   &voiceBoard_963268MBV_LE88536_ZSI,
   &voiceBoard_963268MBV_LE88276_NTR,
   
   &voiceBoard_963168VX_SI3217X,
   &voiceBoard_963168VX_VE890,
   &voiceBoard_963168VX_LE88506,
   &voiceBoard_963168VX_LE88276,
   &voiceBoard_963168VX_SI3226,
   &voiceBoard_963168VX_SI32267,
   &voiceBoard_963168VX_LE88536_ZSI,

   &voiceBoard_963268BU_SI3217X,
   &voiceBoard_963268BU_VE890,
   &voiceBoard_963268BU_LE88506,
   &voiceBoard_963268BU_LE88276,
   &voiceBoard_963268BU_SI3226,
   &voiceBoard_963268BU_SI32267,
   &voiceBoard_963268BU_LE88536_ZSI,
   &voiceBoard_963268BU_SI32261,

   0
};

#endif


#if defined(_BCM96368_) || defined(CONFIG_BCM96368)

VOICE_BOARD_PARMS voiceBoard_96368MVWG =
{
   VOICECFG_6368MVWG_STR,   /* szBoardId */
   "96368MVWG",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_10_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_GPIO_29_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_GPIO_10_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               5,
               5
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               6,
               6
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_3_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96368MBG",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

{
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "96368MBG",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_LE89316 =
{
   VOICECFG_LE89316_STR,   /* szBoardId */
   "96368MBG",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96368MBG",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "96368MBG",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_SI32178 =
{
   VOICECFG_SI32178_STR,   /* szBoardId */
   "96368MBG",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96368MBG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96368MBG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MBG_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96368MBG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_3_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "96368MVNgr",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_SI32178 =
{
   VOICECFG_SI32178_STR,   /* szBoardId */
   "96368MVNgr",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_SI3217X_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "96368MVNgr",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_LE89316 =
{
   VOICECFG_LE89316_STR,   /* szBoardId */
   "96368MVNgr",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_VE890HV_Partial =
{
   VOICECFG_VE890HV_PARTIAL_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,                /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Reset pin */
         BP_GPIO_16_AL,    /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_VE890HV =
{
   VOICECFG_VE890HV_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89136,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_LE88536_TH =
{
   VOICECFG_LE88536_TH_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgr_LE88264_TH =
{
   VOICECFG_LE88264_TH_STR,   /* szBoardId */
   "96368MVNgr",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88264,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_SI32176 =
{
   VOICECFG_SI32176_STR,   /* szBoardId */
   "96368MVNgrP2",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_SI32178 =
{
   VOICECFG_SI32178_STR,   /* szBoardId */
   "96368MVNgrP2",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_SI3217X_NOFXO =
{
   VOICECFG_SI3217X_NOFXO_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_LE89116 =
{
   VOICECFG_LE89116_STR,   /* szBoardId */
   "96368MVNgrP2",   
   1,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_LE89316 =
{
   VOICECFG_LE89316_STR,   /* szBoardId */
   "96368MVNgrP2",   
   1,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_VE890HV_Partial =
{
   VOICECFG_VE890HV_PARTIAL_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,                /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Reset pin */
         BP_GPIO_16_AL,    /* This setting is specific to Zarlink reference card Le71HR8923G for shared 1 reset line. */

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_VE890HV =
{
   VOICECFG_VE890HV_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89136,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89336,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_LE88536_TH =
{
   VOICECFG_LE88536_TH_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88536,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_INVBOOST_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96368MVNgrP2_LE88264_TH =
{
   VOICECFG_LE88264_TH_STR,   /* szBoardId */
   "96368MVNgrP2",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88264,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK_TH, 

   /* General-purpose flags */
   ( BP_FLAG_THALASSA_SUPPORT )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_VE890 =
{
   VOICECFG_VE890_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89316,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_LE88266x2_LE89010 =
{
   VOICECFG_LE88266x2_LE89010_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   4,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88266,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88266,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89010,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_GPIO_29_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_4_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on Le89010 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_MASTERSLAVE_FB,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_LE88266x2 =
{
   VOICECFG_LE88266x2_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   4,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88266,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88266,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_MASTERSLAVE_FB,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_SI3217X =
{
   VOICECFG_SI3217X_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   2,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32176,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32178,

         /* SPI control */
         {  
            /* SPI dev id */
            2,
            /* SPI GPIO */
            BP_GPIO_28_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_SI32261 =
{
   VOICECFG_SI32261_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_SI32260x2_SI3050 =
{
   VOICECFG_SI32260x2_SI3050_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   4,             /* numFxsLines */
   1,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,  /* Should be Si32260, but use Si32261 for now */

         /* SPI control */
         {  
            /* SPI dev id */
            1,   
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,  /* Should be Si32260, but use Si32261 for now */

         /* SPI control */
         {  
            /* SPI dev id */
            2,   
            /* SPI GPIO */
            BP_GPIO_28_AL,  
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* voiceDevice3 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3050,

         /* SPI control */
         {  
            /* SPI dev id */
            3,
            /* SPI GPIO */
            BP_GPIO_29_AL,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_4_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on Si3050 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96368RMVNg_SI32260x2 =
{
   VOICECFG_SI32260x2_STR,   /* szBoardId */
   "96368RMVNg NOR",   
   4,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   2,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_IDECT1,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_DECT,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,  /* Should be Si32260, but use Si32261 for now */

         /* SPI control */
         {  
            /* SPI dev id */
            1, 
            /* SPI GPIO */
            BP_NOT_DEFINED, 
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_16_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32261,  /* Should be Si32260, but use Si32261 for now */

         /* SPI control */
         {  
            /* SPI dev id */
            2, 
            /* SPI GPIO */
            BP_GPIO_28_AL, 
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_17_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_GPIO_8_AH,  BP_GPIO_13_AH } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FB_TSS,

   /* General-purpose flags */
   ( 0 )

};

static PVOICE_BOARD_PARMS g_VoiceBoardParms[] = 
{
   &voiceBoard_96368MVWG,
   
   &voiceBoard_96368MBG_LE88276,
   &voiceBoard_96368MBG_LE88506,
   &voiceBoard_96368MBG_VE890,
   &voiceBoard_96368MBG_LE89116,
   &voiceBoard_96368MBG_LE89316,
   &voiceBoard_96368MBG_SI3217X,
   &voiceBoard_96368MBG_SI32176,
   &voiceBoard_96368MBG_SI32178,
   &voiceBoard_96368MBG_SI3226,
   
   &voiceBoard_96368MVNgr_SI3217X,
   &voiceBoard_96368MVNgr_SI32176,
   &voiceBoard_96368MVNgr_SI32178,
   &voiceBoard_96368MVNgr_SI3217X_NOFXO,
   &voiceBoard_96368MVNgr_SI3226,   
   &voiceBoard_96368MVNgr_VE890,
   &voiceBoard_96368MVNgr_VE890_INVBOOST,
   &voiceBoard_96368MVNgr_LE89116,          
   &voiceBoard_96368MVNgr_LE89316,
   &voiceBoard_96368MVNgr_VE890HV_Partial,
   &voiceBoard_96368MVNgr_VE890HV,        
   &voiceBoard_96368MVNgr_LE88506,
   &voiceBoard_96368MVNgr_LE88276,
   &voiceBoard_96368MVNgr_LE88536_TH,
   &voiceBoard_96368MVNgr_LE88264_TH, 

   &voiceBoard_96368MVNgrP2_SI3217X,
   &voiceBoard_96368MVNgrP2_SI32176,
   &voiceBoard_96368MVNgrP2_SI32178,
   &voiceBoard_96368MVNgrP2_SI3217X_NOFXO,
   &voiceBoard_96368MVNgrP2_SI3226,   
   &voiceBoard_96368MVNgrP2_VE890,
   &voiceBoard_96368MVNgrP2_VE890_INVBOOST,
   &voiceBoard_96368MVNgrP2_LE89116,          
   &voiceBoard_96368MVNgrP2_LE89316,
   &voiceBoard_96368MVNgrP2_VE890HV_Partial,
   &voiceBoard_96368MVNgrP2_VE890HV,        
   &voiceBoard_96368MVNgrP2_LE88506,
   &voiceBoard_96368MVNgrP2_LE88276,
   &voiceBoard_96368MVNgrP2_LE88536_TH,
   &voiceBoard_96368MVNgrP2_LE88264_TH,
   
   &voiceBoard_96368RMVNg_VE890,
   &voiceBoard_96368RMVNg_LE88266x2_LE89010,
   &voiceBoard_96368RMVNg_LE88266x2,
   &voiceBoard_96368RMVNg_SI3217X,
   &voiceBoard_96368RMVNg_SI32261,
   &voiceBoard_96368RMVNg_SI32260x2_SI3050,
   &voiceBoard_96368RMVNg_SI32260x2,

   0
};

#endif

#if defined(_BCM96816_) || defined(CONFIG_BCM96816)

VOICE_BOARD_PARMS voiceBoard_96816SV_SI3239 =
{
   VOICECFG_SI3239_STR,   /* szBoardId */
   "96816SV",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3239,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_NOT_DEFINED,

   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_LE9530 =
{
   VOICECFG_LE9530_STR,   /* szBoardId */
   "96816PVWM",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_9530,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_NOT_DEFINED,

   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96816PVWM",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96816PVWM",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96816PVWM",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_LE9530_LE88276 =
{
   VOICECFG_LE9530_LE88276_STR,   /* szBoardId */
   "96816PVWM",   
   4,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_9530,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,   

   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_LE9530_LE88506 =
{
   VOICECFG_LE9530_LE88506_STR,   /* szBoardId */
   "96816PVWM",   
   4,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Test a single channel on 88506 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_9530,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_FLYBACK,    

   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )

};


VOICE_BOARD_PARMS voiceBoard_96816PVWM_LE9530_SI3226 =
{
   VOICECFG_LE9530_SI3226_STR,   /* szBoardId */
   "96816PVWM",   
   4,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* voiceDevice1 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_9530,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },
   
   /* SLIC Device Profile */
   BP_VD_QCUK,   

   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )

};


VOICE_BOARD_PARMS voiceBoard_96818RG_LE9530 =
{
   VOICECFG_LE9530_STR,   /* szBoardId */
   "96818RG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_9530,

         /* SPI control */
         {  
            /* SPI dev id */
            0,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         0,

         /* Reset pin */
         BP_NOT_DEFINED,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_NOT_DEFINED,

   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )

};

VOICE_BOARD_PARMS voiceBoard_96818RG_LE88276 =
{
   VOICECFG_LE88276_STR,   /* szBoardId */
   "96818RG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88276,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK, 

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96818RG_LE88506 =
{
   VOICECFG_LE88506_STR,   /* szBoardId */
   "96818RG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,

   /* General-purpose flags */
   ( 0 )

};

VOICE_BOARD_PARMS voiceBoard_96818RG_SI3226 =
{
   VOICECFG_SI3226_STR,   /* szBoardId */
   "96818RG",   
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   0,             /* numDectLines */
   0,             /* numFailoverRelayPins */

   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3226,

         /* SPI control */
         {  
            /* SPI dev id */
            1,
            /* SPI GPIO */
            BP_NOT_DEFINED,
         },

         /* Reset required (1 for yes, 0 for no) */
         1,

         /* Reset pin */
         BP_GPIO_14_AL,

         /* Channel description */
         {
            /* Channel 0 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with BP_NULL_DEVICE_MACRO */
      BP_NULL_DEVICE_MACRO,
   },

   /* Relay control pins */
   { { BP_NOT_DEFINED,  BP_NOT_DEFINED } },

   /* DECT UART control pins */
   { BP_NOT_DEFINED,  BP_NOT_DEFINED },

   /* SLIC Device Profile */
   BP_VD_QCUK,

   /* General-purpose flags */
   ( 0 )

};

static PVOICE_BOARD_PARMS g_VoiceBoardParms[] =
{
   &voiceBoard_96816SV_SI3239,
   &voiceBoard_96816PVWM_LE88276,
   &voiceBoard_96816PVWM_SI3226,
   &voiceBoard_96816PVWM_LE88506,
   &voiceBoard_96816PVWM_LE9530,
   &voiceBoard_96816PVWM_LE9530_LE88276,
   &voiceBoard_96816PVWM_LE9530_LE88506,
   &voiceBoard_96816PVWM_LE9530_SI3226,
   &voiceBoard_96818RG_LE9530,
   &voiceBoard_96818RG_LE88276,
   &voiceBoard_96818RG_LE88506,
   &voiceBoard_96818RG_SI3226,
   0
};
#endif


static PVOICE_BOARD_PARMS g_pCurrentVoiceBp = 0;

static void bpmemcpy( void* dstptr, const void* srcptr, int size );
static void bpmemcpy( void* dstptr, const void* srcptr, int size )
{
   char* dstp = dstptr;
   const char* srcp = srcptr;
   int i;
   for( i=0; i < size; i++ )
   {
      *dstp++ = *srcp++;
   }
} 

int BpGetVoiceParms( char* pszBoardId, VOICE_BOARD_PARMS* voiceParms, char* pszBaseBoardId )
{
   int nRet = BP_BOARD_ID_NOT_FOUND;
   PVOICE_BOARD_PARMS *ppBp;

   for( ppBp = g_VoiceBoardParms; *ppBp; ppBp++ )
   {
     if( (0 == bpstrcmp((*ppBp)->szBoardId, pszBoardId)) && (0 == bpstrcmp((*ppBp)->szBaseBoardId, pszBaseBoardId) ))
     {
         g_pCurrentVoiceBp = *ppBp;
         bpmemcpy( voiceParms, g_pCurrentVoiceBp, sizeof(VOICE_BOARD_PARMS) );
         nRet = BP_SUCCESS;
         break;
     }
   }

   return( nRet );
}


/**************************************************************************
* Name       : BpSetVoiceBoardId
*
* Description: This function find the BOARD_PARAMETERS structure for the
*              specified board id string and assigns it to a global, static
*              variable.
*
* Parameters : [IN] pszBoardId - Board id string that is saved into NVRAM.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_FOUND - Error, board id input string does not
*                  have a board parameters configuration record.
***************************************************************************/
int BpSetVoiceBoardId( char *pszBoardId )
{
    int nRet = BP_BOARD_ID_NOT_FOUND;
    PVOICE_BOARD_PARMS *ppBp;

    for( ppBp = g_VoiceBoardParms; *ppBp; ppBp++ )
    {
        if( 0 == bpstrcmp((*ppBp)->szBoardId, pszBoardId) )
        {
            g_pCurrentVoiceBp = *ppBp;
            nRet = BP_SUCCESS;
            break;
        }
    }

    return( nRet );
} /* BpSetVoiceBoardId */


/**************************************************************************
* Name       : BpGetVoiceBoardId
*
* Description: This function returns the current board id strings.
*
* Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
*                  string is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
***************************************************************************/
int BpGetVoiceBoardId( char *pszBoardId )
{
    int i;

    if (g_pCurrentVoiceBp == 0){
        return -1;}

    for (i = 0; i < BP_BOARD_ID_LEN; i++)
        pszBoardId[i] = g_pCurrentVoiceBp->szBoardId[i];

    return 0;
}


/**************************************************************************
* Name       : BpGetVoiceBoardIds
*
* Description: This function returns all of the supported voice board id strings.
*
* Parameters : [OUT] pszBoardIds - Address of a buffer that the board id
*                  strings are returned in.  Each id starts at BP_BOARD_ID_LEN
*                  boundary.
*              [IN] nBoardIdsSize - Number of BP_BOARD_ID_LEN elements that
*                  were allocated in pszBoardIds.
*              [IN] pszBaseBoardId - Name of base Board ID to associate Voice
*                  Board ID with.
*
* Returns    : Number of board id strings returned.
***************************************************************************/
int BpGetVoiceBoardIds( char *pszBoardIds, int nBoardIdsSize, char *pszBaseBoardId )
{
    PVOICE_BOARD_PARMS *ppBp;
    int i;
    char *src;
    char *dest;

    for( i = 0, ppBp = g_VoiceBoardParms; *ppBp && nBoardIdsSize; ppBp++ )
    {
        if( 0 == bpstrcmp((*ppBp)->szBaseBoardId, pszBaseBoardId) )
        {
			//printf ("match (*ppBp)->szBaseBoardId=%s, pszBaseBoardId=%s\n",(*ppBp)->szBaseBoardId, pszBaseBoardId);   
            dest = pszBoardIds;
            src = (*ppBp)->szBoardId;
            while( *src )
                *dest++ = *src++;
            *dest = '\0';
            i++;
            pszBoardIds += BP_BOARD_ID_LEN;
            nBoardIdsSize--;
        }
    }

    return( i );
} /* BpGetVoiceBoardIds */
#endif
