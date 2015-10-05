
#ifndef	__HI_IRQS_H__
#define	__HI_IRQS_H__
#define HISI_GIC_IRQ_START 32	
#define INTNR_COMMTX0					HISI_GIC_IRQ_START+32	
#define INTNR_COMMRX0					HISI_GIC_IRQ_START+33
#define INTNR_L2CACHE_INT_COMB			HISI_GIC_IRQ_START+34	
#define INTNR_L2CACHE_CHK0_INT			HISI_GIC_IRQ_START+35	
#define INTNR_L2CACHE_CHK1_INT			HISI_GIC_IRQ_START+36
#define INTNR_ZSP	                    HISI_GIC_IRQ_START+37
#define INTNR_USB_EHCI	                HISI_GIC_IRQ_START+38
#define INTNR_USB_OHCI	                HISI_GIC_IRQ_START+39
#define INTNR_SFC				        HISI_GIC_IRQ_START+40
#define INTNR_NANDC_S				    HISI_GIC_IRQ_START+41
#define INTNR_SSP					    HISI_GIC_IRQ_START+42
#define INTNR_I2C_S					    HISI_GIC_IRQ_START+43
#define INTNR_I2C_M					    HISI_GIC_IRQ_START+44
#define INTNR_UART0			            HISI_GIC_IRQ_START+45
#define INTNR_UART1			            HISI_GIC_IRQ_START+46
#define INTNR_TIMER_0_1			        HISI_GIC_IRQ_START+47	/* Timer 0 and 1 */
#define INTNR_TIMER_2_3                	HISI_GIC_IRQ_START+48	/* Timer 2 and 3 */
#define INTNR_GPIO_0                    HISI_GIC_IRQ_START+49	/* GPIO 0 */
#define INTNR_GPIO_1                    HISI_GIC_IRQ_START+50	/* GPIO 1 */
#define INTNR_GPIO_2	                HISI_GIC_IRQ_START+51	/* GPIO 2 */
#define INTNR_GPIO_3                    HISI_GIC_IRQ_START+52	/* GPIO 3 */
#define INTNR_GPIO_4                    HISI_GIC_IRQ_START+53	/* GPIO 4 */
#define INTNR_GPIO_5	                HISI_GIC_IRQ_START+54	/* GPIO 5 */
#define INTNR_GPIO_6                    HISI_GIC_IRQ_START+55	/* GPIO 6 */
#define INTNR_GPIO_7                    HISI_GIC_IRQ_START+56	/* GPIO 7 */
#define INTNR_SDIO				        HISI_GIC_IRQ_START+57
#define INTNR_DMAC	                    HISI_GIC_IRQ_START+58
#define INTNR_PCIE0_A	                HISI_GIC_IRQ_START+59
#define INTNR_PCIE0_B	                HISI_GIC_IRQ_START+60
#define INTNR_PCIE0_C	                HISI_GIC_IRQ_START+61
#define INTNR_PCIE0_D	                HISI_GIC_IRQ_START+62
#define INTNR_PCIE1_A	                HISI_GIC_IRQ_START+63
#define INTNR_PCIE1_B	                HISI_GIC_IRQ_START+64
#define INTNR_PCIE1_C	                HISI_GIC_IRQ_START+65
#define INTNR_PCIE1_D	                HISI_GIC_IRQ_START+66
#define INTNR_PMU                       HISI_GIC_IRQ_START+67
#define INTNR_SCI                       HISI_GIC_IRQ_START+68
#define INTNR_PCIE0_LINK_DOWN	        HISI_GIC_IRQ_START+69
#define INTNR_PCIE1_LINK_DOWN	        HISI_GIC_IRQ_START+70
#define INTNR_PCIE0_PM	                HISI_GIC_IRQ_START+71
#define INTNR_PCIE1_PM	                HISI_GIC_IRQ_START+72

#define NR_IRQS						    HISI_GIC_IRQ_START+128  //128
#define MAX_GIC_NR      1


#endif
