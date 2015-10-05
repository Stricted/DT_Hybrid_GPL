#include "boardparms.h"

unsigned int UtilGetChipRev(void);
char *UtilGetChipName(char *buf, int len);
int UtilGetChipIsPinCompatible(void); 
void Get968500EmacMode(ETHERNET_MAC_INFO *pE, int port, unsigned long *mac_type, unsigned long *port_flags);
int UtilGetLanLedGpio(int n, unsigned short *gpio_num);
int UtilGetLan1LedGpio(unsigned short *gpio_num);
int UtilGetLan2LedGpio(unsigned short *gpio_num);
int UtilGetLan3LedGpio(unsigned short *gpio_num);
int UtilGetLan4LedGpio(unsigned short *gpio_num);
#if defined (CONFIG_BCM96838) || defined(_BCM96838_)
unsigned int gpio_get_dir(unsigned int gpio_num);
void gpio_set_dir(unsigned int gpio_num, unsigned int dir);
unsigned int gpio_get_data(unsigned int gpio_num);
void gpio_set_data(unsigned int gpio_num, unsigned int data);                                                
void set_pinmux(unsigned int pin_num, unsigned int mux_num);

#endif

