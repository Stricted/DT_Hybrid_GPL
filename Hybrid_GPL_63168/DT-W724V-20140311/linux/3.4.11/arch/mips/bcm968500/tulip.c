#if defined(CONFIG_BCM_KF_MIPS_BCM9685XX)

#include "bl_os_wraper.h"
#include "bl_lilac_soc.h"
#include "bl_lilac_brd.h"
#include "bl_lilac_spi.h"
#include "tulip_fpga.h"

int bl_lilac_tulip_restart(void)
{
    BL_SPI_SLAVE_CFG slave_cfg;

    unsigned char reg = TULIP_RESET; // 7=leds, 8=restart
    unsigned char val = TULIP_RESET_VALUE;
    
    if(bl_spi_bus_init(1))
    {
        printk("restart failed to initialize SPI!\n");
        return 1;
    }
    slave_cfg.bus = 1;
    slave_cfg.cs = TULIP_SPI_CHIP_SELECT;
    slave_cfg.sleep_period = 300;
    slave_cfg.polarity = 0;
    slave_cfg.lsb_msb = 0;
    slave_cfg.edge = 0;
    slave_cfg.clock = TULIP_SPI_CLOCK;
    slave_cfg.handler = NULL;
    if(bl_spi_slave_init(&slave_cfg))
    {
        printk("restart failed to initialize SPI slave!\n");
        return 1;
    }
    
    bl_claim_bus(&slave_cfg);
    /* set a suitable access type in the address */
    SET_FPGA_ACCESS_TYPE_WRITE(reg);
    if (!bl_spi_xfer(&slave_cfg, 1, 0, &reg, BL_SPI_XFER_BEGIN))
        bl_spi_xfer(&slave_cfg, 1, 0, &val, BL_SPI_XFER_END);
    
    bl_release_bus(&slave_cfg);

    return BL_LILAC_SOC_OK;
}
#endif /* CONFIG_BCM_KF_MIPS_BCM9685XX */

