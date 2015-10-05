/*
 *  linux/include/asm-arm/hardware/dw_serial.h
 *
 *   *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __DW_SERIAL_H__
#define __DW_SERIAL_H__
/* -------------------------------------------------------------------------------
 *  From DW UART Block Specification
 * -------------------------------------------------------------------------------
 *  UART Register Offsets.
 */
 
#define DW_UART_RBR          0x00	 /*  Receive buff. */
#define DW_UART_THR          0x00	 /*  Transmit Holding  */
#define DW_UART_DLL          0x00	 /*  Divisor Latch Low */

#define DW_UART_DLH          0x04  /*  Divisor Latch High */
#define DW_UART_IER          0x04  /*  int enable*/

#define DW_UART_IIR          0x08  /*  interrupt indentification REG*/
#define DW_UART_FCR          0x08  /*  FIFO control*/


#define DW_UART_LCR          0x0C  /*  Line control reg */

#define DW_UART_MCR          0x10  /* Modem Control reg*/
        
#define DW_UART_LSR          0x14  /*  Line  statue  */
#define DW_UART_MSR          0x14  /*  Modem statue */

#define DW_UART_USR          0x7C  /*  Uart statues */

#define DW_UART_HTX          0xA4  /*  Halt Tx*/

/*
*
* DW_UART_LCR bit field
*
*/
#define DW_UART_DLAB       (1 << 7)
#define DW_UART_BREAK      (1 << 6)
#define DW_UART_EPS        (1 << 4)
#define DW_UART_PEN        (1 << 3)
#define DW_UART_STOP       (1 << 2)
#define DW_UART_8bit       0x3 
#define DW_UART_7bit       0x2
#define DW_UART_6bit       0x1
#define DW_UART_5bit       0x0
#define DW_UART_DATALEN_MASK 0x03
 
/*
*
* DW_UART_IER bit field
*
*/ 
#define PTIME       (1<<7)
#define EDSSI       (1<<3)
#define ELSI        (1<<2)
#define ETBEI       (1<<1)
#define ERBFI       (1) 

/*
*
* DW_UART_LSR bit field
*
*/
#define DW_RFE        (1<<7)
#define DW_TEMT       (1<<6)
#define DW_THRE       (1<<5)
#define DW_BI         (1<<4)
#define DW_FE         (1<<3)
#define DW_PE         (1<<2)
#define DW_OE         (1<<1)
#define DW_DR         (1)

#define UARTdw_DR_OE		(1 << 9)
#define UARTdw_DR_BE		(1 << 12)
#define UARTdw_DR_PE		(1 << 10)
#define UARTdw_DR_FE		(1 << 11)

#define DW_RSR_ANY (DW_OE|DW_PE|DW_FE|DW_BI)
#define DW_DUMMY_RSR_RX  
/*
*
* DW_UART_MCR bit field
*
*/

#define DW_MC_AFCE      (1<<5)
#define DW_MC_LOOP      (1<<4)
#define DW_MC_OUT2      (1<<3)
#define DW_MC_OUT1      (1<<2)
#define DW_MC_RTS       (1<<1)
#define DW_MC_DTR       (1 )





/*
*
* DW_UART_MSR bit field
*
*/

#define DW_DCD          (1<<7)
#define DW_RI           (1<<6)
#define DW_DSR          (1<<5)
#define DW_CTS          (1<<4)


#define DW_MSR_ANY (DW_DCD|DW_DSR|DW_CTS)


/*
*
* DW_UART_IIR bit field
*
*/
#define DW_RECEIVERR        (0x06)    /* HIGHEST   */
#define DW_RECEIVEAVA       (0x04)    /* SECOND  RECEIVE date */
#define DW_RECTIMEOUT       (0x0C)    /* SECOND    */
#define DW_TRANSEMP         (0x02)    /* THIRD transmit hold reg empty */
#define DW_NOINTERRUPT      (0x01)    /* NO interrupt pending */
#define DW_MODEMSTA         (0)       /* FOURTH modem int    */
#define DW_BUSY             (0x7)     /* Fifth write while line control busy*/

/*
*
* DW_UART_FCR bit field
*
*/
#define RECFIFO1_2          (0x02<<6)    /* */
#define TXFIFO1_2           (0x03<<4)       /*  */
#define FIFOENA             (1)  

/*
*
* DW_UART_USR bit field
*
*/
#define DW_UART_BUSY        0x01
#define DW_XFIFO_NOT_FULL   0x02
#define DW_XFIFO_EMP        0x04
#define DW_RFIFO_NOT_EMP    0x08
#define DW_RFIFO_FULL       0x10

#endif /* __DW_SERIAL_H__ */
