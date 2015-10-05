
#ifndef	__KEXPORT_H__
#define	__KEXPORT_H__


// sd5115 kernel export functions
typedef enum
{   
    HI_CHIP_ID_NONE_E = 0x0,
    HI_CHIP_ID_S_E,
    HI_CHIP_ID_H_E,
    HI_CHIP_ID_T_E,
    HI_CHIP_ID_MPW_E,
} HI_CHIP_ID_E;

HI_CHIP_ID_E hi_kernel_get_chip_id(void);
int hi_kernel_acp_on(void);
uint hi_kernel_ahb_clk_get(uint *pui_ahb_clk);

#endif      //#ifndef	__KEXPORT_H__


