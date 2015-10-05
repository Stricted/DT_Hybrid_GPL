/******************************************************************************
  版权所有  : 2013-2020，华为技术有限公司
  文 件 名  : Hw_gpio_i2c.c
  作           者  : liuye 00252599
  版           本  : 1.0
  创建日期  : 2013-09-05
  描          述  :   该文件中使用2个GPIO来模拟I2C控制器.用一个GPIO来做时
                           钟线,另外一个GPIO来做数据线.
  函数列表  : 
                        01. i2c_gpio_set_val        设置GPIO值
                        02. i2c_gpio_set_dir        设置GPIO方向,输入还是输出
                        03. gpio_i2c_start           向I2C SLAVE设备发送I2C启动的时序
                        04. gpio_i2c_stop            向I2C SLAVE设备发送I2C结束的时序
                        05. gpio_i2c_send_ack     发送ACK信号,如果是最后一个字节则发送NACK信号
                        06. gpio_get_value          获取SDA GPIO的值
                        07. gpio_i2c_receive_ack  接收ACK信号
                        08. gpio_i2c_send_byte    发送一个字节的数据
                        09. gpio_i2c_read_byte     接收一个字节的数据
                        10. gpio_i2c_write            向I2C 写数据
                        11. gpio_i2c_read             向I2C读数据
                        12. gpio_i2c_init               初始化GPIO SDA, GPIO SCL的值, 获取I2C的地址等.
  历史记录      :
   1.日    期   : 2013-09-05
     作    者   : l00252599
     修改内容   : 完成初稿
   2.日    期   : 2013-10-20
     作    者   : l00252599
     修改内容   : 增加ATMEL芯片的支持和升级
*****************************************************************************/

/* Includes. */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/string.h>

#include <linux/version.h>
#include <linux/slab.h>
#include "bhal.h"

#include <bcm_map_part.h>
#include <board.h>
#include <boardparms.h>

#define GPIO_DELAY      50
#define GPIO_SDA        8
#define GPIO_SCL        9
#define GPIO_DIR_OUTPUT 1
#define GPIO_DIR_INPUT  0
#define GPIO_DATA_HIGH  1
#define GPIO_DATA_LOW   0

UINT16 g_sdaGpio = 0;
UINT16 g_sclGpio = 0;
UINT16 g_device_id = 0;
bool bIsUpgrading = false;

#define I2C_CMD_READ_BLR_REV	    0x49
#define I2C_CMD_READ_SYS_STATE	    0x53
#define I2C_CMD_CAL_CRC			    0x43
#define I2C_CMD_READ_CRC		    0x44
#define I2C_CMD_FW_UPGRADE		    0x55
#define I2C_CMD_EXIT_BLR		    0x45
#define I2C_CMD_READ_TOUCH_KEY	    0x4b
#define I2C_CMD_LED_CTRL		    0x42
#define I2C_CMD_SYS_RST_ENTER_BLR	0x52
#define I2C_CMD_READ_APP_REV	    0x56
#define I2C_CMD_BUZZER_SOUND	    0x1e

static spinlock_t hw_gpioI2cLock;

static void i2c_gpio_set_val(unsigned int gpio, int state)
{
	if (state)
        GPIO->GPIOio |= GPIO_NUM_TO_MASK(gpio);
	else
        GPIO->GPIOio &= ~GPIO_NUM_TO_MASK(gpio);

    return;
}

/* Toggle SCL by changing the direction of the pin. */
static void i2c_gpio_set_dir(unsigned int gpio, int state)
{

	if (GPIO_DIR_OUTPUT == state)
        GPIO->GPIODir |= GPIO_NUM_TO_MASK(gpio);
	else
        GPIO->GPIODir &= ~GPIO_NUM_TO_MASK(gpio);

    return;
}

static void gpio_i2c_start(void)  
{       
   
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);    
    udelay(GPIO_DELAY);  
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);  
    
    udelay(GPIO_DELAY);  
    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);  
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);  
    udelay(GPIO_DELAY);      
            
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);    
    udelay(GPIO_DELAY);      
    
    return;
}  

void gpio_i2c_stop(void)  
{   
    
    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);    
    udelay(GPIO_DELAY);  

    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
    
    udelay(GPIO_DELAY); 
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);    
    udelay(GPIO_DELAY);  

    return;
}  

void gpio_i2c_send_ack(unsigned char ack)  
{  
    // 0 ACK   1 NACK
    if (ack)  
    {
        i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);
        udelay(GPIO_DELAY);  
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH); 
        
        udelay(GPIO_DELAY);  
        
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);  
        udelay(GPIO_DELAY);    
    }
    else   
    {
        i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW); 
        i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);    
        
        udelay(GPIO_DELAY);  
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);  
        udelay(GPIO_DELAY);  
        
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);  
        udelay(GPIO_DELAY);           
    }

    return;
}  

int gpio_get_value(unsigned short bpGpio)
{
    return((int) ((GPIO->GPIOio & GPIO_NUM_TO_MASK(bpGpio)) >>
        (bpGpio & BP_GPIO_NUM_MASK)));
}

unsigned char gpio_i2c_receive_ack(void)  
{  
  
    unsigned char rc = 0;  
    
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);    
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);  
    udelay(GPIO_DELAY);    

    if (gpio_get_value(g_sdaGpio)) 
    {  
      rc = 1;  
    } 
    
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);  
    udelay(GPIO_DELAY);    
    
    return rc;  
}  

unsigned char gpio_i2c_send_byte(unsigned char send_byte)  
{  
    unsigned char rc = 0;  
    unsigned char out_mask = 0x80;  
    unsigned char value;  
    unsigned char count = 8;  
    
    while (count > 0) 
    {                  
        value = ((send_byte & out_mask) ? 1 : 0);     
        if (value == 1) 
        {             
            i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);    
        }      
        else 
        {                                              
            i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);  
            i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);    
        }
        udelay(GPIO_DELAY);    
        
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);  
        udelay(GPIO_DELAY);    

        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);  
                    
        out_mask >>= 1;        
        count--;         
    }  
    
    udelay(GPIO_DELAY);    
    rc = gpio_i2c_receive_ack();
    
    return rc;
    
}  

void gpio_i2c_read_byte(unsigned char *buffer, unsigned char ack)  
{  
    unsigned char count = 0x08;  
    unsigned char data = 0x00;  
    unsigned char temp = 0;  
      
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);    
    
    while (count > 0) 
    {  
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);  
        udelay(GPIO_DELAY);    
        temp = gpio_get_value(g_sdaGpio);  
        data <<= 1;  
        if (temp)
        {
            data |= 0x01;
        }
        
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);  
        udelay(GPIO_DELAY);    
        count--;  
    }
    
    gpio_i2c_send_ack(ack);//0 = ACK    1 = NACK   
    *buffer = data;           
    
    return;
}  

unsigned int gpio_i2c_write_atmel(unsigned char I2C_comm, unsigned char *data, unsigned int len)  
{  
    unsigned char rc = 0;  
    unsigned char i;
    unsigned long flags = 0;

    if ((NULL == data) && (len != 0))
    {
        return 1;
    }
    
    spin_lock_irqsave(&hw_gpioI2cLock, flags);
    gpio_i2c_start();
    
    rc = gpio_i2c_send_byte((g_device_id << 1) | 0x00);  
    if (rc)
    {
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }
    
    rc = gpio_i2c_send_byte(I2C_comm);     // send command
    if (rc)
    {
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }
       
    for (i = 0; i < len; i++) 
    {  
        rc = gpio_i2c_send_byte(*data);  
        if (rc)
        {
            spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
            return 1;  
        }
        data++;  
    }  
      
    gpio_i2c_stop();
    
    if (rc) 
    {  
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;
    }  
    
    spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
    
    return 0;    
}  

unsigned int gpio_i2c_write(unsigned char *data, unsigned int len)  
{  
    unsigned char rc = 0;  
    unsigned char i;
    unsigned long flags = 0;
    
    spin_lock_irqsave(&hw_gpioI2cLock, flags);
    gpio_i2c_start();
    
    rc |= gpio_i2c_send_byte((g_device_id << 1) | 0x00);  
    if ((data == NULL) || (0 == len)) 
    {
        gpio_i2c_stop();  
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }  
          
    for (i = 0; i < len; i++) 
    {  
        rc |= gpio_i2c_send_byte(*data);  
        data++;  
    }  
      
    gpio_i2c_stop();
    
    if (rc) 
    {  
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;
    }  
    
    spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
    
    return 0;    
}  

unsigned int gpio_i2c_read( unsigned char *buffer, unsigned int len) 
{
    unsigned char rc = 0;  
    unsigned char i;  
    unsigned long flags = 0;

    spin_lock_irqsave(&hw_gpioI2cLock, flags);

    gpio_i2c_start();
    rc |= gpio_i2c_send_byte(g_device_id << 1);  

    for (i = 0; i < len; i++) 
    {  
        // 当是最后一个字节后发送NACK信号.
        gpio_i2c_read_byte(buffer++, !(len - i - 1));
    } 

    gpio_i2c_stop();   
    if (rc) 
    {  
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        
        return 1;  
    }

    spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
    return 0;    

}

int Touch_Read_Blr_Rev(unsigned char *rev)
{
    return gpio_i2c_read_atmel(I2C_CMD_READ_BLR_REV, rev, 1);
}

int Touch_Read_Sys_State(unsigned char *state)
{
    return gpio_i2c_read_atmel(I2C_CMD_READ_SYS_STATE, state, 1);
}

int Touch_Calculate_CRC(void)
{
    return gpio_i2c_write_atmel(I2C_CMD_CAL_CRC, NULL, 0);
}

int Touch_Read_CRC(unsigned char *crc)
{
    return gpio_i2c_read_atmel(I2C_CMD_READ_CRC, crc, 2);
}

int Touch_FW_Upgrade(unsigned char *data, unsigned int num)
{
    return gpio_i2c_write_atmel(I2C_CMD_FW_UPGRADE, data, num);
}

int Touch_Exit_Blr(void)
{
    return gpio_i2c_write_atmel(I2C_CMD_EXIT_BLR, NULL, 0);
}

int Touch_Buzzer_Sound()
{
    unsigned char acBuzzer = I2C_CMD_BUZZER_SOUND;
    
    if (bIsUpgrading)
    {
        return 0;
    }
    else
    {
        return gpio_i2c_write_atmel(I2C_CMD_LED_CTRL, &acBuzzer, 1);
    }
}

int Touch_Read_Touch_Key(unsigned char *key_value)
{
    if (bIsUpgrading)
    {
        *key_value = 0;
        return 0;
    }
    else
    {
        return gpio_i2c_read_atmel(I2C_CMD_READ_TOUCH_KEY, key_value, 1);
    }
}

int Touch_Sys_Rst_Enter_Blr(void)
{
    return gpio_i2c_write_atmel(I2C_CMD_SYS_RST_ENTER_BLR, NULL, 0);
}

int Touch_Read_App_Rev(unsigned char *rev)
{
    return gpio_i2c_read_atmel(I2C_CMD_READ_APP_REV, rev, 1);
}

int Touch_i2c_Reset(void)
{
    unsigned short resetGpio = 0;

    if (BpGetI2cResetGpio(&resetGpio) == BP_SUCCESS)
    {
        GPIO->GPIODir |= GPIO_NUM_TO_MASK(resetGpio);
        GPIO->GPIOio &= ~GPIO_NUM_TO_MASK(resetGpio);
        mdelay(10);
        GPIO->GPIOio |= GPIO_NUM_TO_MASK(resetGpio);
        return 0;
    }

    return 1;
}

unsigned int gpio_i2c_read_atmel(unsigned char I2C_comm, unsigned char *buffer, unsigned int len)  
{  
    unsigned char rc = 0;  
    unsigned char i;  
    unsigned long flags = 0;
    
    spin_lock_irqsave(&hw_gpioI2cLock, flags);

    gpio_i2c_start();
    
    rc = gpio_i2c_send_byte((g_device_id << 1) | 0x0);  
    if (rc)
    {
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }
    
    rc = gpio_i2c_send_byte(I2C_comm);
    if (rc)
    {
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }

    gpio_i2c_start();       // repeat start
    rc = gpio_i2c_send_byte((g_device_id << 1) | 0x01);  
    if (rc)
    {
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }
    
    for (i = 0; i < len; i++) 
    {  
        // 当是最后一个字节后发送NACK信号.
        gpio_i2c_read_byte(buffer++, !(len - i - 1));
    } 
 
    gpio_i2c_stop();   
    if (rc) 
    {  
        spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
        return 1;  
    }
    
    spin_unlock_irqrestore(&hw_gpioI2cLock, flags);
    return 0;    
}  

unsigned short calcrc16(unsigned char *i2cUpgBuf, unsigned int i2cUpgLen)
{
    unsigned short crc = 0;
    unsigned char data;
    unsigned int i, j;
    unsigned char *pTembuf = NULL;

    pTembuf = i2cUpgBuf;

    for (i = 0; i < i2cUpgLen; i++)
    {
        data = *pTembuf++;  
        crc = crc ^ (unsigned short) data << 8;
        j = 8;
        do
        {
            if (crc & 0x8000) {
                crc = crc << 1 ^ 0x1021;
            }
            else {
                crc = crc << 1;
            }
        } while(--j);
    }

    return (crc);
}

unsigned int gpio_i2c_upg(unsigned char *i2cUpgBuf, unsigned int i2cUpgLen)
{
	int i, j, k, status;
	unsigned char checksum = 0;
	unsigned char remain_bytes = 0;
    unsigned char data_buf[33];
    unsigned short crc = 0;
    unsigned short usI2cCrc = 0;    
    unsigned char *ptempupgBuf = NULL;
    
	i = i2cUpgLen;
	j = 0;
    
    if ((NULL == i2cUpgBuf) || (0 == i2cUpgLen))
    {
        printk("i2cUpgError \r\n");
        return 1;
    }

    bIsUpgrading = true;
    
    ptempupgBuf = i2cUpgBuf;
    crc = calcrc16(ptempupgBuf, i2cUpgLen);

    Touch_i2c_Reset();
    mdelay(400);

	printk("get bootloader rev...\r\n");
	status = Touch_Read_Blr_Rev(&checksum);
	// check read result
	if (status == 0)
	{
		printk("bootloader rev: %x\r\n", checksum);
	}
	else
	{
		printk("bootloader information read:\tFAIL\r\n");
        bIsUpgrading = false;
        return 1;
	}
	
	while (i > 0) 
    {
		remain_bytes = i > 32 ? 32 : i;
        memcpy(data_buf, i2cUpgBuf, remain_bytes);
        i2cUpgBuf += remain_bytes;
		checksum = 0;
		for (k = 0; k < remain_bytes; k++) 
        {
			checksum += data_buf[k];
		}
		data_buf[remain_bytes] = 0xff - checksum;
		
		// perform a write access
		status = Touch_FW_Upgrade(data_buf, (unsigned int)(remain_bytes + 1));
		// check read result
		if (status != 0)
        {
			printk("Write page %d:FAIL\r\n", j);
		}
		mdelay(20);
		i -= remain_bytes;
		j += remain_bytes;
	}
	
	mdelay(10);

	status = Touch_Calculate_CRC();
	// check read result
	if (status == 0) {
		printk("CRC Calculation:\tPASS\r\n");
	}
	else {
		printk("CRC Calculation:\tFAIL\r\n");
	}
	mdelay(400);
	
	status = Touch_Read_CRC(&usI2cCrc);
	// check read result
	if (status == 0)
	{
		printk("CRC read %x:\n", usI2cCrc);
	}
	else
	{
		printk("CRC read:\tFAIL\r\n");
	}


    usI2cCrc = ((((usI2cCrc) >> 8) & 0xFF) | (((usI2cCrc)&0xFF) << 8));
    if (crc != usI2cCrc)
    {
        printk("check crc error. crc: [%x] readcrc: [%x]\r\n", crc, usI2cCrc);
        bIsUpgrading = false;
        return 1;
    }

	printk("exit bootloader...\r\n");
	//exit boot
	status = Touch_Exit_Blr();
	// check read result
	if (status == 0) 
    {
		printk("Boot exit:\tPASS\r\n");
	}
	else
    {
		printk("Boot exit:\tFAIL\r\n");
	}

    bIsUpgrading = false;
    
    return 0;
}


unsigned int gpio_i2c_init(VOID)
{
    unsigned int iRet = 0;
    int status = 0;
    
    spin_lock_init(&hw_gpioI2cLock);

    iRet += BpGetI2cSdaGpio(&g_sdaGpio);
    iRet += BpGetI2cSclGpio(&g_sclGpio);
    iRet += BpGetI2cDeviceId(&g_device_id);
    
    if (0 != iRet)
    {
        return 1;
    }
    
    printk("sdaGpio: [%d] sclGpio: [%d] g_device_id: [%d]\r\n", g_sdaGpio, g_sclGpio, g_device_id);

    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_HIGH);  
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);    
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);  
    i2c_gpio_set_dir(g_sclGpio, GPIO_DIR_OUTPUT);  

    status = Touch_Exit_Blr();
    // check read result
    if (status == 0) 
    {
        printk("Boot exit:\tPASS\r\n");
    }
    else 
    {
        printk("Boot exit:\tFAIL\r\n");
    }
    return 0;
}

