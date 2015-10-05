#include <linux/init.h>
#include <linux/timer.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/unaligned.h>

/*BEGIN:Added by zhouqingsong/2012/3/13*/

#define   IOMUX_SEL0_ADDR		  0x13000000  			//IOMUX_SEL0基地址 
#define   USB_PWERN0              (1<<7)      			//USB_PWERN0
#define   USB_OVRCUR0             (1<<8)      			//USB_OVRCUR0
#define   USB_PWERN1              (1<<14)     			//USB_PWERN1
#define   USB_OVRCUR1             (1<<15)     			//USB_OVRCUR1

#define   PERI_PEREN_ADDR		  (0x10100000 + 0x7C)   //PERI_PEREN基地址
#define   USB_HCLK_EN             (1<<16)      		    //写1开时钟
 
#define   PERI_PEREN1_ADDR	      (0x10100000 + 0x8C)   //PERI_PEREN1基地址
#define   USB_CLK_EN              (1<<22)      			//写1开时钟

#define   PERI_PERCTRL36_ADDR      (0x10100000 + 0x12C)  //PERI_PERCTRL36基地址
#define   USB_PORT1_RST        	   (1<<18)    		     //0:复位；1：不复位
#define   USB_PORT0_RST            (1<<17)
#define   USB_CTRL_RST             (1<<16)
#define   USB_SRST                 (1<<15)    

#define   PERI_PERCTRL39_ADDR      (0x10100000 + 0x138)   //PERI_PERCTRL39基地址
#define   USB_PHY1_RST             (1<<1)    			  //0:复位；1：不复位
#define   USB_PHY0_RST             (1<<0)

typedef volatile unsigned int       hi_v_u32;

typedef struct
{
    hi_v_u32 *IOMUX_SETL0;
    hi_v_u32 *PERI_PEREN;
    hi_v_u32 *PERI_PEREN1;
	hi_v_u32 *PERI_PERCTRL36;
	hi_v_u32 *PERI_PERCTRL39;
} hi_usb_ctrl_reg_s;

static hi_usb_ctrl_reg_s  g_usb_reg_ctrl;

/*END:Added by zhouqingsong/2012/3/13*/

static atomic_t dev_open_cnt = {
	.counter = 0,
};

/* <DTS2012102001555 g68080 2012-10-20 usb控制器驱动修改 begin */
/*
USB时钟及撤消复位步骤
reg_bits_set 0x13000000 7 8 14 15 
reg_bits_set 0x1010007c 16 
reg_bits_set 0x1010008c 22 
等一下 delay(100us)
reg_bits_clr 0x1010012c 15 16 17 18                                                                                     
reg_bits_clr 0x10100138 0 1  
reg_bits_set 0x1010012c 15 
reg_bits_set 0x10100138 0 1 
等一下 delay（100us）
reg_bits_set 0x1010012c 17 18 
等一下 delay（100us）
reg_bits_set 0x1010012c 16 
*/
/* <DTS2012102001555 g68080 2012-10-20 usb控制器驱动修改 end */
void hiusb_start_hcd(void)
{
	unsigned long flags;

	local_irq_save(flags);
	if(atomic_add_return(1, &dev_open_cnt) == 1){

		int reg;

		/*BEGIN:Added by zhouqingsong/2012/3/13*/
   		g_usb_reg_ctrl.IOMUX_SETL0 = ioremap(IOMUX_SEL0_ADDR, sizeof(hi_v_u32));
		if(NULL == g_usb_reg_ctrl.IOMUX_SETL0)
		{
		   printk("ioremap error: g_usb_reg_ctrl.IOMUX_SETL0 = NULL\r\n");
		   return;
		}
		g_usb_reg_ctrl.PERI_PEREN = ioremap(PERI_PEREN_ADDR, sizeof(hi_v_u32));
		if(NULL == g_usb_reg_ctrl.PERI_PEREN)
		{
		   printk("ioremap error: g_usb_reg_ctrl.PERI_PEREN = NULL\r\n");
		   iounmap(g_usb_reg_ctrl.IOMUX_SETL0);
		   return;
		}
		g_usb_reg_ctrl.PERI_PEREN1 = ioremap(PERI_PEREN1_ADDR, sizeof(hi_v_u32));
		if(NULL == g_usb_reg_ctrl.PERI_PEREN1)
		{
		   printk("ioremap error: g_usb_reg_ctrl.PERI_PEREN1 = NULL\r\n");
		   iounmap(g_usb_reg_ctrl.IOMUX_SETL0);
		   iounmap(g_usb_reg_ctrl.PERI_PEREN);
		   return;
		}
		g_usb_reg_ctrl.PERI_PERCTRL36 = ioremap(PERI_PERCTRL36_ADDR, sizeof(hi_v_u32));
		if(NULL == g_usb_reg_ctrl.PERI_PERCTRL36)
		{
		   printk("ioremap error: g_usb_reg_ctrl.PERI_PERCTRL36 = NULL\r\n");
		   iounmap(g_usb_reg_ctrl.IOMUX_SETL0);
		   iounmap(g_usb_reg_ctrl.PERI_PEREN);
		   iounmap(g_usb_reg_ctrl.PERI_PEREN1);
		   return;
		}
		g_usb_reg_ctrl.PERI_PERCTRL39 = ioremap(PERI_PERCTRL39_ADDR, sizeof(hi_v_u32));
		if(NULL == g_usb_reg_ctrl.PERI_PERCTRL39)
		{
		   printk("ioremap error: g_usb_reg_ctrl.PERI_PERCTRL39 = NULL\r\n");
		   iounmap(g_usb_reg_ctrl.IOMUX_SETL0);
		   iounmap(g_usb_reg_ctrl.PERI_PEREN);
		   iounmap(g_usb_reg_ctrl.PERI_PEREN1);
		   iounmap(g_usb_reg_ctrl.PERI_PERCTRL36);
		   return;
		}
	
		reg = *(g_usb_reg_ctrl.IOMUX_SETL0);
		reg |= USB_PWERN0;
		reg |= USB_OVRCUR0;
		reg |= USB_PWERN1;
		reg |= USB_OVRCUR1;
		*(g_usb_reg_ctrl.IOMUX_SETL0) = reg;

/* <DTS2012102001555 g68080 2012-10-20 usb控制器驱动修改 begin */
	    reg = *(g_usb_reg_ctrl.PERI_PEREN);
		reg |= USB_HCLK_EN;
		*(g_usb_reg_ctrl.PERI_PEREN) = USB_HCLK_EN;

		reg = *(g_usb_reg_ctrl.PERI_PEREN1);
		reg |= USB_CLK_EN;
		*(g_usb_reg_ctrl.PERI_PEREN1) = USB_CLK_EN;

        udelay(100);
		
		reg = *(g_usb_reg_ctrl.PERI_PERCTRL36);
		reg &= ~USB_PORT1_RST;
		reg &= ~USB_PORT0_RST;
	    reg &= ~USB_CTRL_RST;
		reg &= ~USB_SRST;
		*(g_usb_reg_ctrl.PERI_PERCTRL36) = reg;

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL39);
		reg &= ~USB_PHY1_RST;
		reg &= ~USB_PHY0_RST;
		*(g_usb_reg_ctrl.PERI_PERCTRL39) = reg;

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL36);
		reg |= USB_SRST;
		*(g_usb_reg_ctrl.PERI_PERCTRL36) = reg;

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL39);
		reg |= USB_PHY1_RST;
		reg |= USB_PHY0_RST;
		*(g_usb_reg_ctrl.PERI_PERCTRL39) = reg;

		udelay(100);

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL36);
		reg |= USB_PORT1_RST;
		reg |= USB_PORT0_RST;
		*(g_usb_reg_ctrl.PERI_PERCTRL36) = reg;

		udelay(100);

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL36);
        reg |= USB_CTRL_RST;
		*(g_usb_reg_ctrl.PERI_PERCTRL36) = reg;

		udelay(100);
		
		printk("hiusb_start_hcd sucessful!\r\n");
/* <DTS2012102001555 g68080 2012-10-20 usb控制器驱动修改 end */
		/*END:Added by zhouqingsong/2012/3/13*/
	}
	local_irq_restore(flags);
}
EXPORT_SYMBOL(hiusb_start_hcd);

void hiusb_stop_hcd(void)
{
	unsigned long flags;

	local_irq_save(flags);
	if(atomic_sub_return(1, &dev_open_cnt) == 0){

        /*BEGIN:Added by zhouqingsong/2012/3/13*/
		int reg;
		
		reg = *(g_usb_reg_ctrl.PERI_PEREN);
		reg &=~USB_HCLK_EN;
		*(g_usb_reg_ctrl.PERI_PEREN) = reg;

		reg = *(g_usb_reg_ctrl.PERI_PEREN1);
		reg &=~ USB_CLK_EN;
		*(g_usb_reg_ctrl.PERI_PEREN1) = reg;

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL36);
		reg &=~USB_PORT1_RST;
		reg &=~ USB_PORT0_RST;
	    reg &=~ USB_CTRL_RST;
		reg &=~ USB_SRST;
		*(g_usb_reg_ctrl.PERI_PERCTRL36) = reg;

		reg = *(g_usb_reg_ctrl.PERI_PERCTRL39);
		reg &=~ USB_PHY1_RST;
		reg &=~ USB_PHY0_RST;
		*(g_usb_reg_ctrl.PERI_PERCTRL39) = reg;

		reg = *(g_usb_reg_ctrl.IOMUX_SETL0);
		reg &=~USB_PWERN0;
		reg &=~USB_OVRCUR0;
		reg &=~USB_PWERN1;
		reg &=~USB_OVRCUR1;
		*(g_usb_reg_ctrl.IOMUX_SETL0) = reg;

		iounmap(g_usb_reg_ctrl.IOMUX_SETL0);
		iounmap(g_usb_reg_ctrl.PERI_PEREN);
		iounmap(g_usb_reg_ctrl.PERI_PEREN1);
		iounmap(g_usb_reg_ctrl.PERI_PERCTRL36);
		iounmap(g_usb_reg_ctrl.PERI_PERCTRL39);
		
/* <DTS2012102001555 g68080 2012-10-20 usb控制器驱动修改 begin */
		printk("hiusb_stop_hcd sucessful!\r\n");
/* <DTS2012102001555 g68080 2012-10-20 usb控制器驱动修改 end */
		/*END:Added by zhouqingsong/2012/3/13*/
	}
	local_irq_restore(flags);
}
EXPORT_SYMBOL(hiusb_stop_hcd);
