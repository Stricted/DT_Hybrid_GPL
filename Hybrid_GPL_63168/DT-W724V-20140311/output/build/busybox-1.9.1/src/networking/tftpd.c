/* 
 * $Id: tftpd.c,v 1.1 2009/03/20 04:01:32 y42304 Exp $
 * A simple tftpd server for busybox
 *
 * Copyright (C) 2001 Steven Carr <Steven_Carr@yahoo.com>
 *
 * Tries to follow RFC1350 and RFC2347.
 * Only "octet" mode supported.
 * tsize option is supported on sending files only (pxelinux support).
 * chroot jail for security.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-137 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/tftp.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>
#include <net/if.h>
#include "busybox.h"
//#include "bcmTag.h"
#include <bits/socket.h>
#include <sys/uio.h>

#ifndef OACK
#define OACK 6
#endif
#ifndef EOPTNEG
#define EOPTNEG 8
#endif

#define EFILE 9

#define ENOPUT -2
#define ENOGET -3

#if !defined(__UCLIBC__) || defined(__UCLIBC_HAS_MMU__)
#define FORK() fork()
#else
#define FORK() vfork()
#endif

#define MAXCTRLSIZE                  \
   (sizeof(struct cmsghdr) + sizeof(struct sockaddr_in) +   \
   sizeof(struct cmsghdr) + sizeof(int) + 32)

#define TFTP_BLOCKSIZE_DEFAULT 512   /* according to RFC 1350, don't change */
#define TFTP_TIMEOUT           5     /* seconds */
#define TFTPD_TIMEOUT          300   /* seconds */
#define DUP_PKT_CHECK_COUNT    3

#define MAX_IMAGE_LEN			1024*1024*100

//Enable put only
#define CONFIG_FEATURE_TFTPD_GET
#define CONFIG_FEATURE_TFTPD_PUT

typedef unsigned char byte;
typedef unsigned int UINT32;


/*note:c00176128: put the information from bcmTag.h to here  20120317 start*/
#define TAG_VER_LEN         4
#define SIG_LEN             20
#define SIG_LEN_2           14   // Original second SIG = 20 is now devided into 14 for SIG_LEN_2 and 6 for CHIP_ID
#define CHIP_ID_LEN		    6
#define BOARD_ID_LEN        16
#define FLAG_LEN            2
#define IMAGE_LEN           10
#define ADDRESS_LEN         12
#define IMAGE_VER_LEN       32
#define RESERVED_LEN        (TAG_LEN - TAG_VER_LEN - SIG_LEN - SIG_LEN_2 - CHIP_ID_LEN - BOARD_ID_LEN - \
                                (4*IMAGE_LEN) - (3*ADDRESS_LEN) - (3*FLAG_LEN) - (2*TOKEN_LEN) - IMAGE_VER_LEN)
#define TOKEN_LEN           20
#define TAG_LEN             256
#define CRC32_INIT_VALUE    0xffffffff /* Initial CRC32 checksum value */




static unsigned long Crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};













// TAG for downloadable image (kernel plus file system)
typedef struct _FILE_TAG
{
    unsigned char tagVersion[TAG_VER_LEN];       // tag version.  Will be 2 here.
    unsigned char signiture_1[SIG_LEN];          // text line for company info
    unsigned char signiture_2[SIG_LEN_2];        // additional info (can be version number)
    unsigned char chipId[CHIP_ID_LEN];			 // chip id 
    unsigned char boardId[BOARD_ID_LEN];         // board id
    unsigned char bigEndian[FLAG_LEN];           // if = 1 - big, = 0 - little endia of the host
    unsigned char totalImageLen[IMAGE_LEN];      // the sum of all the following length
    unsigned char cfeAddress[ADDRESS_LEN];       // if non zero, cfe starting address
    unsigned char cfeLen[IMAGE_LEN];             // if non zero, cfe size in clear ASCII text.
    unsigned char rootfsAddress[ADDRESS_LEN];    // if non zero, filesystem starting address
    unsigned char rootfsLen[IMAGE_LEN];          // if non zero, filesystem size in clear ASCII text.
    unsigned char kernelAddress[ADDRESS_LEN];    // if non zero, kernel starting address
    unsigned char kernelLen[IMAGE_LEN];          // if non zero, kernel size in clear ASCII text.
    unsigned char imageSequence[FLAG_LEN * 2];   // incrments everytime an image is flashed
    /* start of maintain Auto Upgrade by zhangliang 60003055 2006年5月8日" */
    //unsigned char productName[PRODUCT_NAME_LEN]; 
    //unsigned char softVersion[SOFT_VERSION_LEN]; 
    /* end of maintain Auto Upgrade by zhangliang 60003055 2006年5月8日" */
    unsigned char imageVersion[IMAGE_VER_LEN];   // image version
    
    
    unsigned char reserved[RESERVED_LEN];        // reserved for later use
    
    //unsigned char midAddress[ADDRESS_LEN];       // if non zero, cfe starting address
    //unsigned char midLen[IMAGE_LEN];             // if non zero, cfe size in clear ASCII text.
    
    unsigned char imageValidationToken[TOKEN_LEN];// image validation token - can be crc, md5, sha;  for
                                                 // now will be 4 unsigned char crc
    unsigned char tagValidationToken[TOKEN_LEN]; // validation token for tag(from signiture_1 to end of // mageValidationToken)
} FILE_TAG, *PFILE_TAG;


/*note:c00176128: put the information from bcmTag.h to here  20120317  end*/





int 	g_peer = -1;
char 	filedir[256] = {0};


//function prototype declaration
void handler_sigterm(int signum);
int tftpd_options(char *options, int opt_len, char **argv, int max_arg);
void tftpd_nak (int peer, int error);
int tftpd_ack (int peer, int block);
int tftpd_oack (int peer, int count, char **list);
int tftpd_data (int peer, int block, char *data, int size);
int tftpd_getdata(int peer, int block, char *data, int size);
int tftpd_getack(int peer, int block);
void tftpd_send (int peer, struct tftphdr *first_pkt, int pkt_len, int buffersize);
void tftpd_receive (int peer, struct tftphdr *first_pkt, int pkt_len, int buffersize);
static int tftpd_daemon (char *directory, char *address, int port);
void get_directory(int argc,char **argv);
void tftpd_main (int argc, char **argv);
UINT32 getCrc32(byte *pdata, UINT32 size, UINT32 crc);
int verifyTag(PFILE_TAG pTag);
static int initSocket(int port, int type, int backlog);
int systemNoHang (char *command);


void handler_sigterm(int signum)
{
	exit(0);					
}


/*
 * Handle initial connection protocol.
 *     +-------+---~~---+---+---~~---+---+---~~---+---+---~~---+---+-->  >-------+---+---~~---+---+
 *     |  opc  |filename| 0 |  mode  | 0 |  opt1  | 0 | value1 | 0 | <  <  optN  | 0 | valueN | 0 |
 *     +-------+---~~---+---+---~~---+---+---~~---+---+---~~---+---+-->  >-------+---+---~~---+---+
 *             ^--->
 */

/* The options are zero terminated, retrieve a list of pointers to the first character of each option */
int tftpd_options (char *options, int opt_len, char **argv, int max_arg)
{
    int x;
    int y;
    argv[0] = options;
    for (y = 1, x = 0; (y < max_arg) && (x < (opt_len - 1)); x++)
    {
        if (0 == options[x])
        {
            if (0 == options[x + 1])
                return y;
            argv[y] = &options[x + 1];
            y++;
        }
    }
    return y;
}


/*
 * Send a nak packet (error message).
 * Error code passed in is one of the
 * standard TFTP codes, or a UNIX errno
 * offset by 100.
 */
void tftpd_nak (int peer, int error)
{
    char buf[TFTP_BLOCKSIZE_DEFAULT + 4];
    struct tftphdr *pkt;

    pkt = (struct tftphdr *) buf;
    pkt->th_opcode = htons ((u_short) ERROR);
    pkt->th_code = htons ((u_short) error);

    switch (error)
    {
        case ENOPUT:
            strcpy (pkt->th_msg, "Put not supported");
            pkt->th_code = htons (EUNDEF);
            break;
        case ENOGET:
            strcpy (pkt->th_msg, "Get not supported");
            pkt->th_code = htons (EUNDEF);
            break;
        case EUNDEF:
            strcpy (pkt->th_msg, "Undefined error code");
            break;
        case ENOTFOUND:
            strcpy (pkt->th_msg, "File not found");
            break;
        case EACCESS:
            strcpy (pkt->th_msg, "Access violation");
            break;
        case ENOSPACE:
            strcpy (pkt->th_msg, "Disk full or allocation exceeded");
            break;
        case EBADOP:
            strcpy (pkt->th_msg, "Illegal TFTP operation");
            break;
        case EBADID:
            strcpy (pkt->th_msg, "Unknown transfer ID");
            break;
        case EEXISTS:
            strcpy (pkt->th_msg, "File already exists");
            break;
        case ENOUSER:
            strcpy (pkt->th_msg, "No such user");
            break;
        case EOPTNEG:
            strcpy (pkt->th_msg, "Failure to negotiate RFC2347 options");
            break;
        case EFILE:
            strcpy (pkt->th_msg, "Image file is illegal");
            break;
        default:
            strcpy (pkt->th_msg, strerror (error - 100));
            pkt->th_code = htons (EUNDEF);
        break;
    }

    send (peer, buf, strlen (pkt->th_msg) + 5, 0);
}


/*
 * Send a ack packet 
 */
int tftpd_ack (int peer, int block)
{
    struct tftphdr pkt;

    pkt.th_opcode = htons (ACK);
    pkt.th_block = htons (block);

    if (send (peer, &pkt, sizeof(pkt), 0) != sizeof(pkt))
    {
        printf("tftpd_ack send\n");
        return 0;
    }
    return 1;
}


/*
 * send an oack
 */
int tftpd_oack (int peer, int count, char **list)
{
    char buf[TFTP_BLOCKSIZE_DEFAULT + 4];
    struct tftphdr *pkt;
    int x;
    char *ptr;

    pkt = (struct tftphdr *)buf;
    pkt->th_opcode = htons (OACK);
    ptr = pkt->th_stuff;

    for (x = 0; x<count; x++)
    {
        ptr = strrchr (strcpy (ptr, list[x]), '\0') + 1;
    }

    if ( send (peer, buf, (ptr-buf), 0) != (ptr-buf))
    {
        printf("tftpd_oack send\n");
        return 0;
    }

    return 1;
}


/*
 * send data
 */
int tftpd_data (int peer, int block, char *data, int size)
{
    struct tftphdr *pkt;
    char buf[TFTP_BLOCKSIZE_DEFAULT + 4];

    pkt = (struct tftphdr *)buf;
    pkt->th_opcode = htons (DATA);
    pkt->th_block  = htons(block);

    memcpy(pkt->th_data, data, size);

    if (send (peer, &buf, size + 4, 0)!= (size + 4))
    {
        printf("tftpd_data send\n");
        return 0;
    }
    
    return 1;
}

int tftpd_getdata(int peer, int block, char *data, int size)
{
    struct tftphdr *pkt;
    struct timeval tv;
    fd_set rfds;
    int len = -1;
    int timeout_counter = 4;

    pkt = (struct tftphdr *)data;

    do 
    {   
        tv.tv_sec = TFTP_TIMEOUT;
        tv.tv_usec = 0;
        FD_ZERO (&rfds);
        FD_SET (peer, &rfds);
        switch (select (FD_SETSIZE, &rfds, NULL, NULL, &tv))
        {
            case 1:      /* data ready */
                len = recv (peer, data, size, 0);
                if (len < 0)
                {
                    printf("failed to read (data)\n");
                    return 0;
                }
                if (len == 0)
                {
                    printf("tftp len is 0\n");
                    break;
                }
                pkt->th_opcode = ntohs (pkt->th_opcode);
                pkt->th_block = ntohs (pkt->th_block);
                if (ERROR == pkt->th_opcode)
                {
                    printf(pkt->th_data);
                    return 0;
                }
                if ((pkt->th_opcode == DATA) && (pkt->th_block != block))
                {
                    //synchronize (peer);
                }
                break;
            case 0:      /* timeout */
                timeout_counter--;
                if (0 == timeout_counter)
                {
                    printf ("last timeout\n");
                    return 0;
                }
                break;
            default:      /* error */
                printf("select failed\n");
                break;
        }
    }while (!(pkt->th_opcode == DATA) && (pkt->th_block == block));
    return len;
}



int tftpd_getack(int peer, int block)
{
    char data[TFTP_BLOCKSIZE_DEFAULT + 4];
    struct tftphdr *pkt;
    struct timeval tv;
    fd_set rfds;
    int timeout_counter = 1;
    int len;

    pkt = (struct tftphdr *)data;

    do 
    {   
        tv.tv_sec = TFTP_TIMEOUT;
        tv.tv_usec = 0;
        FD_ZERO (&rfds);
        FD_SET (peer, &rfds);
        switch (select (FD_SETSIZE, &rfds, NULL, NULL, &tv))
        {
            case 1:      /* data ready */
                len = recv (peer, data, TFTP_BLOCKSIZE_DEFAULT + 4, 0);
                if (len < 0)
                {
                    printf("failed to read (data)\n");
                    return 0;
                }

                pkt->th_opcode = ntohs (pkt->th_opcode);
                pkt->th_block = ntohs (pkt->th_block);

                if (ERROR == pkt->th_opcode)
                {
                    printf(pkt->th_data);
                    return 0;
                }

                if ((pkt->th_opcode == ACK) && (pkt->th_block != block))
                {
                    //synchronize (peer);
                }
                break;
            case 0:      /* timeout */
                timeout_counter--;
                if (timeout_counter == 0)
                {
                    printf("last timeout\n");
                    return 0;
                }
                break;
            default:      /* error */
                printf ("select failed\n");
                break;
        }
    }while (!((pkt->th_opcode == ACK) && (pkt->th_block == block)) );

    return (1 == 1);
}


#ifndef CONFIG_FEATURE_TFTPD_GET
void tftpd_send (int peer, struct tftphdr *tp, int n, int buffersize)
{
	tftpd_nak (peer, ENOGET);
    close (peer);
    //exit(0);
	return;
}

#else
void tftpd_send (int peer, struct tftphdr *first_pkt, int pkt_len, int buffersize)
{
    FILE *file = NULL;
    char buffer[TFTP_BLOCKSIZE_DEFAULT+4];
    char *list[64]; /* list of pointers to options and values */
    int listcount;
    char *reply_list[64];
    int reply_listcount = 0;
    char tsize_ret[32];
	char filename[128] = {0};
    int block, inbytes, x;
    int need_last_empty_block = 0;
    int dupCount = 0;

    listcount = tftpd_options (first_pkt->th_stuff, pkt_len, list ,64);
	
	

    if (strcasecmp(list[1], "octet") != 0)
    {
        tftpd_nak(peer,EBADOP);
        close(peer);
        sleep(1);
        //exit(0);
		return;
    }
	
	snprintf(filename,sizeof(filename),"%s/%s",filedir,list[0]);


	
    file = fopen (filename, "r");
    if (NULL == file)
    {
		tftpd_nak (peer, ENOTFOUND);
        close(peer);
        sleep(1);
		return;
    }
    fseek (file, 0, SEEK_END);
    sprintf(tsize_ret, "%lu", ftell (file));
    fseek (file, 0, SEEK_SET);

    if (0 == atoi(tsize_ret)%TFTP_BLOCKSIZE_DEFAULT)
    {
        need_last_empty_block = 1;
    }
	
    /* 0=filename, 1=mode, 2=option, 3=option_value ... */
    block = 1;
    reply_listcount = 0;

    /* Look through the options for the ones we support */
    for (x = 2; x < listcount; x++)
    {
        if (0 == strcasecmp(list[x],"tsize")) /* only one option supported so far */
        {
            reply_list[reply_listcount] = list[x];      
            reply_listcount++;
            reply_list[reply_listcount] = tsize_ret; /* point to the real value */
            reply_listcount++;
        }
    }

    /* If there are any options, send an OACK instead of an ACK */
    if (reply_listcount > 0)
    {
        dupCount = DUP_PKT_CHECK_COUNT;
        do
        {
            if (0 == dupCount)
            {
                fclose(file);
                close(peer);
                return;
            }

            tftpd_oack(peer, reply_listcount, reply_list);

            dupCount--;
        }
        while (!tftpd_getack(peer,0));
    }   


    /* Send the file! */
    while ((inbytes = fread(buffer, 1, TFTP_BLOCKSIZE_DEFAULT, file)) > 0)
    {
        dupCount = DUP_PKT_CHECK_COUNT;
        do
        {
            if (0 == dupCount)
            {
                fclose(file);
                close(peer);
                return;
            }

            tftpd_data(peer, block, buffer, inbytes);

            dupCount--;
        }
        while (!tftpd_getack(peer, block));
        block++;
    }

    if (need_last_empty_block)
    {
        dupCount = DUP_PKT_CHECK_COUNT;
        do
        {
            if (0 == dupCount)
            {
                fclose(file);
                close(peer);
                return;
            }

            tftpd_data(peer, block, buffer, 0);

            dupCount--;
        }
        while (!tftpd_getack(peer, block));
        block++;
    }

    fclose(file);
    close(peer);
    //exit (0);
	return;
}

#endif


#ifndef CONFIG_FEATURE_TFTPD_PUT
void tftpd_receive (int peer, struct tftphdr *tp, int n, int buffersize)
{
    tftpd_nak (peer, ENOPUT);
    close (peer);
    //exit(0);
	return;
}

#else
void tftpd_receive (int peer, struct tftphdr *first_pkt, int pkt_len, int buffersize)
{
    FILE *file = NULL;
    char buffer[TFTP_BLOCKSIZE_DEFAULT + 4];
    struct tftphdr *pkt;
    int block, inbytes;
    char *list[64];
    int listcount;
    int byteRd = 0;
    char tag[TAG_LEN];
    int uploadSize = 0;
    int i = 0;   
    int ulRet = 0;
    int g_nok = FALSE;

    pkt = (struct tftphdr *)buffer;
    listcount = tftpd_options (first_pkt->th_stuff, pkt_len, list ,64);

    printf ("mode = %s, file = %s\n", list[1], list[0]);
    if (0 == strcasecmp(list[1], "octet"))
    {
        block = 0;

        file = fopen ("/var/image", "w+");
        if (NULL == file)
        {
            printf("open file failed\n");
        }

        do
        {
            if (!tftpd_ack(peer, block))
            {
                g_nok = TRUE;
                break;
            }
            block++;

            /* If duplicate pkt, (for slow ack on 38R board) discard it */
            for (i = 0; i < DUP_PKT_CHECK_COUNT; i++)
            {
                inbytes = tftpd_getdata(peer, block, buffer, TFTP_BLOCKSIZE_DEFAULT + 4);
                if (block == (int) (*(short*)(buffer+2)))
                {
                    break;
                }

            }
            
            if (inbytes <= 0)
            {
                g_nok = TRUE;
                break;
            }
            byteRd = inbytes - 4;
            
            if (1 == block) 
            {
                /* Not enough data for a valid first packet */
                if (byteRd < TFTP_BLOCKSIZE_DEFAULT)   
                {
                    uploadSize = byteRd;
                    g_nok = TRUE;
                    tftpd_nak(peer, EFILE);
                    break;
                }
                         
                /* The first data of .bin file is the FILE_TAG structure
                   and the FILE_TAG length is 256 */
                memcpy(tag, pkt->th_msg, TAG_LEN);
                PFILE_TAG pTag = (PFILE_TAG) tag;
                if (0 == verifyTag(pTag)) 
                {
                    printf("Image file is .bin file.\n");
                }
                else 
                {   
                    printf("Image file is illegal.\n");
                    g_nok = TRUE;
                    tftpd_nak(peer, EFILE);
                    break;
                }                 
            } 

            if (fwrite(pkt->th_msg, 1, byteRd, file) != byteRd)
            {
                printf("write file failed\n");
                g_nok = TRUE;
                break;	
            }
      
            uploadSize += byteRd;
        }
        while((TFTP_BLOCKSIZE_DEFAULT + 4) == inbytes);
        if (uploadSize > MAX_IMAGE_LEN + TOKEN_LEN) 
        {       
            printf("Image file size is illegal\n");
            g_nok = TRUE;
            tftpd_nak(peer, EFILE);
        }
        fclose(file);

        tftpd_ack(peer,block); //final acknowledge
        printf("Upload file size: %d\n", uploadSize);

        if (FALSE == g_nok)
        {
            printf("Waiting for upgrade software..........\n");
            
            /* Use command to upgrade software*/
            ulRet = systemNoHang("upg -b -f /var/image -m 20001 &");
            if(0 != ulRet)    
            {
                printf("Execute upgrade command failed \n");  
            } 
        }

    }
    else
    {
        printf("Only support 'octet' mode.\n");
        tftpd_nak(peer, EBADOP);
    }    
}
#endif

static struct in_addr getLanIp(void)
{
    int socketfd;
    struct ifreq lan;

    memset(&lan, 0, sizeof(lan));
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        printf("Error openning socket when getting LAN info\n");
    }
    else  
    {
        strcpy(lan.ifr_name, "br0");
        if (ioctl(socketfd,SIOCGIFADDR,&lan) < 0) 
        {
            printf("Error getting LAN IP address\n");
        }
    }
    close(socketfd);
    return ((struct sockaddr_in *)&(lan.ifr_addr))->sin_addr;
}


static int tftpd_daemon (char *directory, char *address, int port)
{
    struct tftphdr *tp;
    struct sockaddr_in from;
    struct sockaddr_in myaddr;
    struct sockaddr_in bindaddr;
    int fd = -1;
    int rv;
    int n;

    char buf[TFTP_BLOCKSIZE_DEFAULT + 4];
    struct iovec iov = { buf, sizeof buf };
    struct cmsghdr *cmsg;
    char *ctrl = (char *)xmalloc(MAXCTRLSIZE);
    struct msghdr msg = { (void*)&from, sizeof from, &iov, 1, (void*)ctrl, MAXCTRLSIZE, 0};
    struct in_pktinfo *info = NULL;
	
	struct stat st;
	

    signal(SIGTERM, handler_sigterm);

    if((fd = initSocket(69, SOCK_DGRAM, 0)) < 0)
    {
        printf("tftpd: initSocket failed. port=69\n");
        return 0;
    }
	

    
        fd_set         readset;

        memset(buf,0,TFTP_BLOCKSIZE_DEFAULT + 4);
        memset (&myaddr, 0, sizeof (myaddr));
	
		FD_ZERO (&readset);
        FD_SET (fd, &readset);
        

	while (1)
    {	
		if(fstat(fd,&st)<0)
		{
			perror("fstat");
		}
		
        /* Never time out, we're in standalone mode */
        rv = select (fd+1, &readset, NULL, NULL, NULL);
		//FD_CLR(fd, &readset);
		

        if (rv < 0)
        {
            if (errno != EINTR) 
            {
                //printf("*** error select < 0\n");
				perror("select");
                return 0;
            }
    	    continue;
        }

		
        n = recvmsg (fd, &msg, MSG_WAITALL);
        if (n <= 0)
        {            
            /* Received message with zero length, reloop */
            if (0 == n) 
            {
                continue;
            }
            else
            {         
                printf("*** error recvmsg < 0\n");
                return 0;
            }
        }
        
		
        for(cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL;cmsg = CMSG_NXTHDR(&msg,cmsg))
        {
            if (IP_PKTINFO == cmsg->cmsg_type)
            {
                info = (struct in_pktinfo *)CMSG_DATA(cmsg);
                break;
            }
        }

        /* Process the request */
        myaddr.sin_family = AF_INET;
        myaddr.sin_port = htons (0);  
        myaddr.sin_addr = getLanIp();

        if (myaddr.sin_addr.s_addr != info->ipi_spec_dst.s_addr)
        {
            memcpy(&myaddr.sin_addr, &info->ipi_spec_dst, sizeof myaddr.sin_addr);
        }

	
		/* Get a socket.  This has to be done before the chroot() (/dev goes away) */
		g_peer = socket (AF_INET, SOCK_DGRAM, 0);
		if (g_peer < 0)
		{
			printf("socket < 0\n");
			return 0;
		}
		if (chroot ("."))
		{
			printf("chroot = 0\n");
			return 0;
		}
		from.sin_family = AF_INET;

		/* Process the request */
		if (bind (g_peer, (struct sockaddr *) &myaddr, sizeof myaddr) < 0)
		{
			printf("daemon-child bind\n");
			return 0;
		}

		if (connect (g_peer, (struct sockaddr *) &from, sizeof from) < 0)
		{
			printf("daemon-child connect\n");
			return 0;
		}
		tp = (struct tftphdr *) buf;

		tp->th_opcode = ntohs (tp->th_opcode);

		switch(tp->th_opcode)
		{
		  case RRQ:
			 tftpd_send (g_peer, tp, n, TFTP_BLOCKSIZE_DEFAULT);
			 break;
		  case WRQ:
			 tftpd_receive (g_peer, tp, n, TFTP_BLOCKSIZE_DEFAULT);
			 break;
		}
	}
    return 1;
}

void get_directory(int argc,char **argv)
{
	char dir[256] = {0};
	
	while((argv)[0] )
	{
		if(0==strcmp(argv[0],"-s") && argv[1])
		{
			snprintf(dir,sizeof(dir),"%s",argv[1]);
		}
		argv++;
	}
	
	if(0 == strlen(dir))
	{
		/*get current directory*/
		getcwd(dir, sizeof(dir)-1);
	}
	
	snprintf(filedir,sizeof(filedir),"%s",dir);
}

void tftpd_main (int argc, char **argv)
{
    int result;
    char *address = NULL;      /* address to listen to */
    int port = 69;
	
	get_directory(argc,argv);

	
	result = tftpd_daemon ((char*)"", address, port);
}


UINT32 getCrc32(byte *pdata, UINT32 size, UINT32 crc) 
{
    while (size-- > 0)
        crc = (crc >> 8) ^ Crc32_table[(crc ^ *pdata++) & 0xff];

    return crc;
}


int verifyTag(PFILE_TAG pTag)
{
    UINT32 crc;
    int status = 0;

    /* Check tag validate token */
    crc = CRC32_INIT_VALUE;
    crc = getCrc32((char*) pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    crc = htonl(crc);

    if (crc != (UINT32)(*(UINT32*)(pTag->tagValidationToken)))
    {
        printf("Illegal image ! Tag crc failed.\n");
        status = -1;
    }

    return status;
}


static int initSocket(int port, int type, int backlog) 
{
    struct sockaddr_in   serverAddr;
    int                  sFd;
    int                  optVal;

    /* Create a TCP or UDP based socket */
    if ((sFd = socket(AF_INET, type, 0)) < 0)
    {
        printf("initSocket: socket failed. port=%d\n", port);
        return (errno);
    }

    /* Set socket options */
    optVal = 1;
    if (setsockopt(sFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) < 0)
    {
        printf("initSocket: setsockopt failed. port=%d fd=%d\n", port, sFd);
        close(sFd);
        return (errno);
    }

    if (SOCK_DGRAM == type)
    {
        /* Set option for getting the to ip address. */
        if (setsockopt(sFd, IPPROTO_IP, IP_PKTINFO, &optVal, sizeof(optVal)) < 0)
        {
            printf("initSocket: setsockopt failed. port=%d fd=%d\n", port, sFd);
            close(sFd);
            return (errno);
        }
    }

    /* Set up the local address */
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr  = htonl(INADDR_ANY);

    /* Bind socket to local address */
    if (bind(sFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("initSocket: bind failed. port=%d fd=%d\n", port, sFd);
        close(sFd);
        return (errno);
    }

    if (SOCK_STREAM == type)
    {
        /* Enable connection to SOCK_STREAM socket */
        if (listen(sFd, backlog) < 0)
        {
            printf("initSocket: listen failed. port=%d fd=%d\n", port, sFd);
            close(sFd);
            return (errno);
        }
    }
    return (sFd);
}


extern char **environ;
int systemNoHang (char *command)
{
    int pid = 0, counter = 0, ret = 0;
    int status = 0;

    if (NULL == command)
    {
        return -1;
    }

    pid = fork();
    if (-1 == pid)
    {
        return -1;
    }

    if (0 == pid)
    {
        char *argv[4];
        argv[0] = "sh";
        argv[1] = "-c";
        argv[2] = command;
        argv[3] = 0;
        execve("/bin/sh", argv, environ);
        //exit(127);
		return;
    }

    do
    {
        /* Check the child is exited or not without hang */
        ret = waitpid(pid, &status, WNOHANG | WUNTRACED);
        switch ( ret )
        {
            case -1:   // error occurs then return -1
            {
                return -1;
            }
            case 0:    // child does not exited yet
            {
                usleep(20);
                if ( ++counter > 20000 )
                {
                    kill(pid, SIGTERM);
                    return -1;
                }
                break;
            }
            default:   // child does exit successfully
            {
                return status;
            }
        }
    } while (1);
}

