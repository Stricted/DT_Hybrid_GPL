/*
 *      sd.c Copyright (C) 1992 Drew Eckhardt
 *           Copyright (C) 1993, 1994, 1995, 1999 Eric Youngdale
 *
 *      Linux scsi disk driver
 *              Initial versions: Drew Eckhardt
 *              Subsequent revisions: Eric Youngdale
 *	Modification history:
 *       - Drew Eckhardt <drew@colorado.edu> original
 *       - Eric Youngdale <eric@andante.org> add scatter-gather, multiple 
 *         outstanding request, and other enhancements.
 *         Support loadable low-level scsi drivers.
 *       - Jirka Hanika <geo@ff.cuni.cz> support more scsi disks using 
 *         eight major numbers.
 *       - Richard Gooch <rgooch@atnf.csiro.au> support devfs.
 *	 - Torben Mathiasen <tmm@image.dk> Resource allocation fixes in 
 *	   sd_init and cleanups.
 *	 - Alex Davis <letmein@erols.com> Fix problem where partition info
 *	   not being read in sd_open. Fix problem where removable media 
 *	   could be ejected after sd_open.
 *	 - Douglas Gilbert <dgilbert@interlog.com> cleanup for lk 2.5.x
 *	 - Badari Pulavarty <pbadari@us.ibm.com>, Matthew Wilcox 
 *	   <willy@debian.org>, Kurt Garloff <garloff@suse.de>: 
 *	   Support 32k/1M disks.
 *
 *	Logging policy (needs CONFIG_SCSI_LOGGING defined):
 *	 - setting up transfer: SCSI_LOG_HLQUEUE levels 1 and 2
 *	 - end of transfer (bh + scsi_lib): SCSI_LOG_HLCOMPLETE level 1
 *	 - entering sd_ioctl: SCSI_LOG_IOCTL level 1
 *	 - entering other commands: SCSI_LOG_HLQUEUE level 3
 *	Note: when the logging level is set by the user, it must be greater
 *	than the level indicated above to trigger output.	
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/bio.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/errno.h>
#include <linux/idr.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/string_helpers.h>
#include <linux/async.h>
#include <asm/uaccess.h>
#include <asm/unaligned.h>

#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_dbg.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_driver.h>
#include <scsi/scsi_eh.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_ioctl.h>
#include <scsi/scsicam.h>

#include "sd.h"
#include "scsi_logging.h"

#include "msg/kcmsmonitormsgtypes.h"

//add by zhouyu 2010-11-25 解决相同ID的USB设备web上只能识别一个的问题
#include <linux/string.h>
#include <linux/ctype.h>
//add by zhouyu 2010-11-25 解决相同ID的USB设备web上只能识别一个的问题

/*Start of modified by lvxin 00135113 to prohibit datacard send signal to usbmount 2009年6月6日*/
#include <linux/proc_fs.h>
//extern int g_iDataCardIn;
extern atomic_t g_iDataCardIn;
extern int g_ulNumOfDataCard;
/*End of modified by lvxin 00135113 to prohibit datacard send signal to usbmount 2009年6月6日*/

#include "../../drivers/usb/storage/usb.h"
/*
Add start for USB device led, DTS2010061800769, l67944, 20100625,
*/
#if defined (CONFIG_ATP_BOARDLED)
#ifndef CONFIG_MIPS_TC3162U
#include "boardled.h"
#endif
extern int g_USB_PROBE;
#endif
/*Add end, l67944, 20100625*/	

/* <DTS2012081608784 g68080 2012-09-13 mod begin */
#if defined (CONFIG_USB_LED)
#include <asm/io.h>
extern int g_isDataCard;
extern int g_USB_PROBE;
#define MDM9215_VENDOR_ID  0x05C6
#define USB_LED_REG  0x10107004
#define USB_LED_REGCONTROL  0x10107000
#define USB_LED_ON   0x100000
#endif
/* <DTS2012081608784 g68080 2012-09-13 mod end */

#if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268)
extern int g_USB_PROBE;
extern int g_isDataCard;
#include "board.h"
#endif
/*
 * More than enough for everybody ;)  The huge number of majors
 * is a leftover from 16bit dev_t days, we don't really need that
 * much numberspace.
 */
#define SD_MAJORS	16

/*add by l63336 for send signal to user space*/
#define ATP_SUPPORT_REPORT_USER 1
#ifdef ATP_SUPPORT_REPORT_USER
static struct task_struct *user_app = NULL; 
struct proc_dir_entry *proc_pid;
char pid_str[128];
#endif
/*add by l63336 for send signal to user space*/

MODULE_AUTHOR("Eric Youngdale");
MODULE_DESCRIPTION("SCSI disk (sd) driver");
MODULE_LICENSE("GPL");

MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK0_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK1_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK2_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK3_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK4_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK5_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK6_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK7_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK8_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK9_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK10_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK11_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK12_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK13_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK14_MAJOR);
MODULE_ALIAS_BLOCKDEV_MAJOR(SCSI_DISK15_MAJOR);
MODULE_ALIAS_SCSI_DEVICE(TYPE_DISK);
MODULE_ALIAS_SCSI_DEVICE(TYPE_MOD);
MODULE_ALIAS_SCSI_DEVICE(TYPE_RBC);

#if !defined(CONFIG_DEBUG_BLOCK_EXT_DEVT)
#define SD_MINORS	16
#else
#define SD_MINORS	0
#endif

static int  sd_revalidate_disk(struct gendisk *);
static int  sd_probe(struct device *);
static int  sd_remove(struct device *);
static void sd_shutdown(struct device *);
static int sd_suspend(struct device *, pm_message_t state);
static int sd_resume(struct device *);
static void sd_rescan(struct device *);
static int sd_done(struct scsi_cmnd *);
static void sd_read_capacity(struct scsi_disk *sdkp, unsigned char *buffer);
static void scsi_disk_release(struct device *cdev);
static void sd_print_sense_hdr(struct scsi_disk *, struct scsi_sense_hdr *);
static void sd_print_result(struct scsi_disk *, int);

static DEFINE_SPINLOCK(sd_index_lock);
static DEFINE_IDA(sd_index_ida);

extern struct proc_dir_entry proc_root;

/*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
#define SHORT_STRLEN    32
#define STRLEN 128
//#define MAX_PARTITIONS 16
#define MAX_DISKS      6

struct usb_diskinfo{
    char diskname[SHORT_STRLEN];  /*disk name:sda,sdb,....*/
    char vendor[STRLEN];     /*vendor name of the usb device*/
    char model[STRLEN];    /*product name of the usb device*/
    char serial[STRLEN];     /*serial number  of the usb device*/
    char connectiontype[SHORT_STRLEN]; /*USB1.1,USB2.0,...*/
    int  scsihostid ;        /*scsi host num of this device*/  
    int  isvalid;            /* if this partion info is valid*/
    sector_t	capacity;           /*numbers of usb device partitions*/
} *usbdiskinfos;

struct proc_dir_entry *proc_physicalmedium = NULL;
struct usb_diskinfo *diskinfos = NULL;

int attacheddisks = 0;

/*get a free member*/
static 	struct usb_diskinfo * get_freediskslot()
{
    int i = 0;

	if(attacheddisks < MAX_DISKS)
	{
            for(i=0;i<MAX_DISKS;i++)
            {
                if(0 == ((struct usb_diskinfo *)diskinfos+i)->isvalid)
    			{
    			    break;
                    }
            }	
	}
    /*解决问题DTS2010113005213，连接Hub的第5个USB设备时，会将前面的USB设备挤掉线*/
        else
        {
            return NULL;
        }
    /*解决问题DTS2010113005213，连接Hub的第5个USB设备时，会将前面的USB设备挤掉线*/

	if(i < MAX_DISKS)
	{
        	attacheddisks ++;
        	return ((struct usb_diskinfo *)diskinfos+i);	
	}

    return NULL;
}

static char * replacespace(char *buf)
{
    int i = 0;
    int len = strlen(buf);

	if(len >= STRLEN)
	{
	    len = STRLEN -1;
	}	

	for(i = 0;i < len;i++)
	{
	    if(buf[i] == ' ')
	    {
	        buf[i] = '_';
	    }
	}

	return buf;
}	

static int proc_user_physicalmedium_wr (struct file *file,
                             const char *buffer,
                             unsigned long count,
                             void *data)
{
    /*no need write operation*/
    #if 0
   strcpy(szUmts_type, buffer);
    #endif
   return count;
}

static int proc_user_physicalmedium_rd (char *page, char **start, off_t off, int count, 
								int *eof, void *data)
{
    int i = 0;
    int j = 0;
    int n = 0;
    int len = 0;
    struct usb_diskinfo *disk = NULL;

    //len += sprintf (page + len, "%s","Diskname Vendor   Product         Serial-Number    ConnectionType Capacity \r\n");
    len += sprintf (page + len, "%s","Diskname Vendor       ID       Type   ConnectionType Capacity \r\n");
    for(i = 0;i < MAX_DISKS;i++)
    {
        if(1 == ((struct usb_diskinfo *)diskinfos+i)->isvalid)
        {
            disk = ((struct usb_diskinfo *)diskinfos+i);
            disk->diskname[10] = '\0';		
            len += sprintf (page + len, "%-8s ", replacespace(disk->diskname));

            disk->vendor[15] = '\0';
            len += sprintf (page + len, "%-12s ", replacespace(disk->vendor));

            disk->model[15] = '\0';
            len += sprintf (page + len, "%-8s ",replacespace(disk->model));

            len += sprintf (page + len, "%-6s ",replacespace(disk->serial));				
            len += sprintf (page + len, "%-13s ", replacespace(disk->connectiontype));
            len += sprintf (page + len, "%-6u ", disk->capacity);
            len += sprintf (page + len, "%s","\n");            
        }
    }
    return len;
}

static void proc_user_physicalmedium_create(void)
{
    proc_physicalmedium = create_proc_entry("proc_user_usbdevs", S_IFREG | S_IRUSR , &proc_root ); 
    proc_physicalmedium->write_proc = (read_proc_t *)proc_user_physicalmedium_wr;
    proc_physicalmedium->read_proc = (write_proc_t *)proc_user_physicalmedium_rd;

    /*initialize physicalmidum devs structure*/
    diskinfos = kmalloc(sizeof(struct usb_diskinfo)*MAX_DISKS, GFP_KERNEL);
    if (!diskinfos) 
    {
        return  ;
    } 
    memset(diskinfos,0,sizeof(struct usb_diskinfo)*MAX_DISKS);
    
}

static void proc_user_physicalmedium_remove(void)
{
	/* no problem if it was not registered */
	remove_proc_entry("proc_user_usbdevs", NULL /* parent dir */);
}

/*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/

/* This semaphore is used to mediate the 0->1 reference get in the
 * face of object destruction (i.e. we can't allow a get on an
 * object after last put) */
static DEFINE_MUTEX(sd_ref_mutex);

static const char *sd_cache_types[] = {
	"write through", "none", "write back",
	"write back, no read (daft)"
};

static ssize_t
sd_store_cache_type(struct device *dev, struct device_attribute *attr,
		    const char *buf, size_t count)
{
	int i, ct = -1, rcd, wce, sp;
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	struct scsi_device *sdp = sdkp->device;
	char buffer[64];
	char *buffer_data;
	struct scsi_mode_data data;
	struct scsi_sense_hdr sshdr;
	int len;

	if (sdp->type != TYPE_DISK)
		/* no cache control on RBC devices; theoretically they
		 * can do it, but there's probably so many exceptions
		 * it's not worth the risk */
		return -EINVAL;

	for (i = 0; i < ARRAY_SIZE(sd_cache_types); i++) {
		const int len = strlen(sd_cache_types[i]);
		if (strncmp(sd_cache_types[i], buf, len) == 0 &&
		    buf[len] == '\n') {
			ct = i;
			break;
		}
	}
	if (ct < 0)
		return -EINVAL;
	rcd = ct & 0x01 ? 1 : 0;
	wce = ct & 0x02 ? 1 : 0;
	if (scsi_mode_sense(sdp, 0x08, 8, buffer, sizeof(buffer), SD_TIMEOUT,
			    SD_MAX_RETRIES, &data, NULL))
		return -EINVAL;
	len = min_t(size_t, sizeof(buffer), data.length - data.header_length -
		  data.block_descriptor_length);
	buffer_data = buffer + data.header_length +
		data.block_descriptor_length;
	buffer_data[2] &= ~0x05;
	buffer_data[2] |= wce << 2 | rcd;
	sp = buffer_data[0] & 0x80 ? 1 : 0;

	if (scsi_mode_select(sdp, 1, sp, 8, buffer_data, len, SD_TIMEOUT,
			     SD_MAX_RETRIES, &data, &sshdr)) {
		if (scsi_sense_valid(&sshdr))
			sd_print_sense_hdr(sdkp, &sshdr);
		return -EINVAL;
	}
	revalidate_disk(sdkp->disk);
	return count;
}

static ssize_t
sd_store_manage_start_stop(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	struct scsi_device *sdp = sdkp->device;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

	sdp->manage_start_stop = simple_strtoul(buf, NULL, 10);

	return count;
}

static ssize_t
sd_store_allow_restart(struct device *dev, struct device_attribute *attr,
		       const char *buf, size_t count)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	struct scsi_device *sdp = sdkp->device;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

	if (sdp->type != TYPE_DISK)
		return -EINVAL;

	sdp->allow_restart = simple_strtoul(buf, NULL, 10);

	return count;
}

static ssize_t
sd_show_cache_type(struct device *dev, struct device_attribute *attr,
		   char *buf)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	int ct = sdkp->RCD + 2*sdkp->WCE;

	return snprintf(buf, 40, "%s\n", sd_cache_types[ct]);
}

static ssize_t
sd_show_fua(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);

	return snprintf(buf, 20, "%u\n", sdkp->DPOFUA);
}

static ssize_t
sd_show_manage_start_stop(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	struct scsi_device *sdp = sdkp->device;

	return snprintf(buf, 20, "%u\n", sdp->manage_start_stop);
}

static ssize_t
sd_show_allow_restart(struct device *dev, struct device_attribute *attr,
		      char *buf)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);

	return snprintf(buf, 40, "%d\n", sdkp->device->allow_restart);
}

static ssize_t
sd_show_protection_type(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);

	return snprintf(buf, 20, "%u\n", sdkp->protection_type);
}

static ssize_t
sd_show_app_tag_own(struct device *dev, struct device_attribute *attr,
		    char *buf)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);

	return snprintf(buf, 20, "%u\n", sdkp->ATO);
}

static struct device_attribute sd_disk_attrs[] = {
	__ATTR(cache_type, S_IRUGO|S_IWUSR, sd_show_cache_type,
	       sd_store_cache_type),
	__ATTR(FUA, S_IRUGO, sd_show_fua, NULL),
	__ATTR(allow_restart, S_IRUGO|S_IWUSR, sd_show_allow_restart,
	       sd_store_allow_restart),
	__ATTR(manage_start_stop, S_IRUGO|S_IWUSR, sd_show_manage_start_stop,
	       sd_store_manage_start_stop),
	__ATTR(protection_type, S_IRUGO, sd_show_protection_type, NULL),
	__ATTR(app_tag_own, S_IRUGO, sd_show_app_tag_own, NULL),
	__ATTR_NULL,
};

static struct class sd_disk_class = {
	.name		= "scsi_disk",
	.owner		= THIS_MODULE,
	.dev_release	= scsi_disk_release,
	.dev_attrs	= sd_disk_attrs,
};

static struct scsi_driver sd_template = {
	.owner			= THIS_MODULE,
	.gendrv = {
		.name		= "sd",
		.probe		= sd_probe,
		.remove		= sd_remove,
		.suspend	= sd_suspend,
		.resume		= sd_resume,
		.shutdown	= sd_shutdown,
	},
	.rescan			= sd_rescan,
	.done			= sd_done,
};

/*
 * Device no to disk mapping:
 * 
 *       major         disc2     disc  p1
 *   |............|.............|....|....| <- dev_t
 *    31        20 19          8 7  4 3  0
 * 
 * Inside a major, we have 16k disks, however mapped non-
 * contiguously. The first 16 disks are for major0, the next
 * ones with major1, ... Disk 256 is for major0 again, disk 272 
 * for major1, ... 
 * As we stay compatible with our numbering scheme, we can reuse 
 * the well-know SCSI majors 8, 65--71, 136--143.
 */
static int sd_major(int major_idx)
{
	switch (major_idx) {
	case 0:
		return SCSI_DISK0_MAJOR;
	case 1 ... 7:
		return SCSI_DISK1_MAJOR + major_idx - 1;
	case 8 ... 15:
		return SCSI_DISK8_MAJOR + major_idx - 8;
	default:
		BUG();
		return 0;	/* shut up gcc */
	}
}

static struct scsi_disk *__scsi_disk_get(struct gendisk *disk)
{
	struct scsi_disk *sdkp = NULL;

	if (disk->private_data) {
		sdkp = scsi_disk(disk);
		if (scsi_device_get(sdkp->device) == 0)
			get_device(&sdkp->dev);
		else
			sdkp = NULL;
	}
	return sdkp;
}

static struct scsi_disk *scsi_disk_get(struct gendisk *disk)
{
	struct scsi_disk *sdkp;

	mutex_lock(&sd_ref_mutex);
	sdkp = __scsi_disk_get(disk);
	mutex_unlock(&sd_ref_mutex);
	return sdkp;
}

static struct scsi_disk *scsi_disk_get_from_dev(struct device *dev)
{
	struct scsi_disk *sdkp;

	mutex_lock(&sd_ref_mutex);
	sdkp = dev_get_drvdata(dev);
	if (sdkp)
		sdkp = __scsi_disk_get(sdkp->disk);
	mutex_unlock(&sd_ref_mutex);
	return sdkp;
}

static void scsi_disk_put(struct scsi_disk *sdkp)
{
	struct scsi_device *sdev = sdkp->device;

	mutex_lock(&sd_ref_mutex);
	put_device(&sdkp->dev);
	scsi_device_put(sdev);
	mutex_unlock(&sd_ref_mutex);
}

/**
 *	sd_init_command - build a scsi (read or write) command from
 *	information in the request structure.
 *	@SCpnt: pointer to mid-level's per scsi command structure that
 *	contains request and into which the scsi command is written
 *
 *	Returns 1 if successful and 0 if error (or cannot be done now).
 **/
static int sd_prep_fn(struct request_queue *q, struct request *rq)
{
	struct scsi_cmnd *SCpnt;
	struct scsi_device *sdp = q->queuedata;
	struct gendisk *disk = rq->rq_disk;
	struct scsi_disk *sdkp;
	sector_t block = rq->sector;
	sector_t threshold;
	unsigned int this_count = rq->nr_sectors;
	int ret, host_dif;

	if (rq->cmd_type == REQ_TYPE_BLOCK_PC) {
		ret = scsi_setup_blk_pc_cmnd(sdp, rq);
		goto out;
	} else if (rq->cmd_type != REQ_TYPE_FS) {
		ret = BLKPREP_KILL;
		goto out;
	}
	ret = scsi_setup_fs_cmnd(sdp, rq);
	if (ret != BLKPREP_OK)
		goto out;
	SCpnt = rq->special;
	sdkp = scsi_disk(disk);

	/* from here on until we're complete, any goto out
	 * is used for a killable error condition */
	ret = BLKPREP_KILL;

	SCSI_LOG_HLQUEUE(1, scmd_printk(KERN_INFO, SCpnt,
					"sd_init_command: block=%llu, "
					"count=%d\n",
					(unsigned long long)block,
					this_count));

	if (!sdp || !scsi_device_online(sdp) ||
 	    block + rq->nr_sectors > get_capacity(disk)) {
		SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt,
						"Finishing %ld sectors\n",
						rq->nr_sectors));
		SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt,
						"Retry with 0x%p\n", SCpnt));
		goto out;
	}

	if (sdp->changed) {
		/*
		 * quietly refuse to do anything to a changed disc until 
		 * the changed bit has been reset
		 */
		/* printk("SCSI disk has been changed. Prohibiting further I/O.\n"); */
		goto out;
	}

	/*
	 * Some SD card readers can't handle multi-sector accesses which touch
	 * the last one or two hardware sectors.  Split accesses as needed.
	 */
	threshold = get_capacity(disk) - SD_LAST_BUGGY_SECTORS *
		(sdp->sector_size / 512);

	if (unlikely(sdp->last_sector_bug && block + this_count > threshold)) {
		if (block < threshold) {
			/* Access up to the threshold but not beyond */
			this_count = threshold - block;
		} else {
			/* Access only a single hardware sector */
			this_count = sdp->sector_size / 512;
		}
	}

	SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt, "block=%llu\n",
					(unsigned long long)block));

	/*
	 * If we have a 1K hardware sectorsize, prevent access to single
	 * 512 byte sectors.  In theory we could handle this - in fact
	 * the scsi cdrom driver must be able to handle this because
	 * we typically use 1K blocksizes, and cdroms typically have
	 * 2K hardware sectorsizes.  Of course, things are simpler
	 * with the cdrom, since it is read-only.  For performance
	 * reasons, the filesystems should be able to handle this
	 * and not force the scsi disk driver to use bounce buffers
	 * for this.
	 */
	if (sdp->sector_size == 1024) {
		if ((block & 1) || (rq->nr_sectors & 1)) {
			scmd_printk(KERN_ERR, SCpnt,
				    "Bad block number requested\n");
			goto out;
		} else {
			block = block >> 1;
			this_count = this_count >> 1;
		}
	}
	if (sdp->sector_size == 2048) {
		if ((block & 3) || (rq->nr_sectors & 3)) {
			scmd_printk(KERN_ERR, SCpnt,
				    "Bad block number requested\n");
			goto out;
		} else {
			block = block >> 2;
			this_count = this_count >> 2;
		}
	}
	if (sdp->sector_size == 4096) {
		if ((block & 7) || (rq->nr_sectors & 7)) {
			scmd_printk(KERN_ERR, SCpnt,
				    "Bad block number requested\n");
			goto out;
		} else {
			block = block >> 3;
			this_count = this_count >> 3;
		}
	}
	if (rq_data_dir(rq) == WRITE) {
		if (!sdp->writeable) {
			goto out;
		}
		SCpnt->cmnd[0] = WRITE_6;
		SCpnt->sc_data_direction = DMA_TO_DEVICE;

		if (blk_integrity_rq(rq) &&
		    sd_dif_prepare(rq, block, sdp->sector_size) == -EIO)
			goto out;

	} else if (rq_data_dir(rq) == READ) {
		SCpnt->cmnd[0] = READ_6;
		SCpnt->sc_data_direction = DMA_FROM_DEVICE;
	} else {
		scmd_printk(KERN_ERR, SCpnt, "Unknown command %x\n", rq->cmd_flags);
		goto out;
	}

	SCSI_LOG_HLQUEUE(2, scmd_printk(KERN_INFO, SCpnt,
					"%s %d/%ld 512 byte blocks.\n",
					(rq_data_dir(rq) == WRITE) ?
					"writing" : "reading", this_count,
					rq->nr_sectors));

	/* Set RDPROTECT/WRPROTECT if disk is formatted with DIF */
	host_dif = scsi_host_dif_capable(sdp->host, sdkp->protection_type);
	if (host_dif)
		SCpnt->cmnd[1] = 1 << 5;
	else
		SCpnt->cmnd[1] = 0;

	if (block > 0xffffffff) {
		SCpnt->cmnd[0] += READ_16 - READ_6;
		SCpnt->cmnd[1] |= blk_fua_rq(rq) ? 0x8 : 0;
		SCpnt->cmnd[2] = sizeof(block) > 4 ? (unsigned char) (block >> 56) & 0xff : 0;
		SCpnt->cmnd[3] = sizeof(block) > 4 ? (unsigned char) (block >> 48) & 0xff : 0;
		SCpnt->cmnd[4] = sizeof(block) > 4 ? (unsigned char) (block >> 40) & 0xff : 0;
		SCpnt->cmnd[5] = sizeof(block) > 4 ? (unsigned char) (block >> 32) & 0xff : 0;
		SCpnt->cmnd[6] = (unsigned char) (block >> 24) & 0xff;
		SCpnt->cmnd[7] = (unsigned char) (block >> 16) & 0xff;
		SCpnt->cmnd[8] = (unsigned char) (block >> 8) & 0xff;
		SCpnt->cmnd[9] = (unsigned char) block & 0xff;
		SCpnt->cmnd[10] = (unsigned char) (this_count >> 24) & 0xff;
		SCpnt->cmnd[11] = (unsigned char) (this_count >> 16) & 0xff;
		SCpnt->cmnd[12] = (unsigned char) (this_count >> 8) & 0xff;
		SCpnt->cmnd[13] = (unsigned char) this_count & 0xff;
		SCpnt->cmnd[14] = SCpnt->cmnd[15] = 0;
	} else if ((this_count > 0xff) || (block > 0x1fffff) ||
		   scsi_device_protection(SCpnt->device) ||
		   SCpnt->device->use_10_for_rw) {
		if (this_count > 0xffff)
			this_count = 0xffff;

		SCpnt->cmnd[0] += READ_10 - READ_6;
		SCpnt->cmnd[1] |= blk_fua_rq(rq) ? 0x8 : 0;
		SCpnt->cmnd[2] = (unsigned char) (block >> 24) & 0xff;
		SCpnt->cmnd[3] = (unsigned char) (block >> 16) & 0xff;
		SCpnt->cmnd[4] = (unsigned char) (block >> 8) & 0xff;
		SCpnt->cmnd[5] = (unsigned char) block & 0xff;
		SCpnt->cmnd[6] = SCpnt->cmnd[9] = 0;
		SCpnt->cmnd[7] = (unsigned char) (this_count >> 8) & 0xff;
		SCpnt->cmnd[8] = (unsigned char) this_count & 0xff;
	} else {
		if (unlikely(blk_fua_rq(rq))) {
			/*
			 * This happens only if this drive failed
			 * 10byte rw command with ILLEGAL_REQUEST
			 * during operation and thus turned off
			 * use_10_for_rw.
			 */
			scmd_printk(KERN_ERR, SCpnt,
				    "FUA write on READ/WRITE(6) drive\n");
			goto out;
		}

		SCpnt->cmnd[1] |= (unsigned char) ((block >> 16) & 0x1f);
		SCpnt->cmnd[2] = (unsigned char) ((block >> 8) & 0xff);
		SCpnt->cmnd[3] = (unsigned char) block & 0xff;
		SCpnt->cmnd[4] = (unsigned char) this_count;
		SCpnt->cmnd[5] = 0;
	}
	SCpnt->sdb.length = this_count * sdp->sector_size;

	/* If DIF or DIX is enabled, tell HBA how to handle request */
	if (host_dif || scsi_prot_sg_count(SCpnt))
		sd_dif_op(SCpnt, host_dif, scsi_prot_sg_count(SCpnt),
			  sdkp->protection_type);

	/*
	 * We shouldn't disconnect in the middle of a sector, so with a dumb
	 * host adapter, it's safe to assume that we can at least transfer
	 * this many bytes between each connect / disconnect.
	 */
	SCpnt->transfersize = sdp->sector_size;
	SCpnt->underflow = this_count << 9;
	SCpnt->allowed = SD_MAX_RETRIES;

	/*
	 * This indicates that the command is ready from our end to be
	 * queued.
	 */
	ret = BLKPREP_OK;
 out:
	return scsi_prep_return(q, rq, ret);
}

/**
 *	sd_open - open a scsi disk device
 *	@inode: only i_rdev member may be used
 *	@filp: only f_mode and f_flags may be used
 *
 *	Returns 0 if successful. Returns a negated errno value in case 
 *	of error.
 *
 *	Note: This can be called from a user context (e.g. fsck(1) )
 *	or from within the kernel (e.g. as a result of a mount(1) ).
 *	In the latter case @inode and @filp carry an abridged amount
 *	of information as noted above.
 **/
static int sd_open(struct block_device *bdev, fmode_t mode)
{
	struct scsi_disk *sdkp = scsi_disk_get(bdev->bd_disk);
	struct scsi_device *sdev;
	int retval;

	if (!sdkp)
		return -ENXIO;

	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp, "sd_open\n"));

	sdev = sdkp->device;

	/*
	 * If the device is in error recovery, wait until it is done.
	 * If the device is offline, then disallow any access to it.
	 */
	retval = -ENXIO;
	if (!scsi_block_when_processing_errors(sdev))
		goto error_out;

	if (sdev->removable || sdkp->write_prot)
		check_disk_change(bdev);

	/*
	 * If the drive is empty, just let the open fail.
	 */
	retval = -ENOMEDIUM;
	if (sdev->removable && !sdkp->media_present && !(mode & FMODE_NDELAY))
		goto error_out;

	/*
	 * If the device has the write protect tab set, have the open fail
	 * if the user expects to be able to write to the thing.
	 */
	retval = -EROFS;
	if (sdkp->write_prot && (mode & FMODE_WRITE))
		goto error_out;

	/*
	 * It is possible that the disk changing stuff resulted in
	 * the device being taken offline.  If this is the case,
	 * report this to the user, and don't pretend that the
	 * open actually succeeded.
	 */
	retval = -ENXIO;
	if (!scsi_device_online(sdev))
		goto error_out;

	if (!sdkp->openers++ && sdev->removable) {
		if (scsi_block_when_processing_errors(sdev))
			scsi_set_medium_removal(sdev, SCSI_REMOVAL_PREVENT);
	}

	return 0;

error_out:
	scsi_disk_put(sdkp);
	return retval;	
}

/**
 *	sd_release - invoked when the (last) close(2) is called on this
 *	scsi disk.
 *	@inode: only i_rdev member may be used
 *	@filp: only f_mode and f_flags may be used
 *
 *	Returns 0. 
 *
 *	Note: may block (uninterruptible) if error recovery is underway
 *	on this disk.
 **/
static int sd_release(struct gendisk *disk, fmode_t mode)
{
	struct scsi_disk *sdkp = scsi_disk(disk);
	struct scsi_device *sdev = sdkp->device;

	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp, "sd_release\n"));

	if (!--sdkp->openers && sdev->removable) {
		if (scsi_block_when_processing_errors(sdev))
			scsi_set_medium_removal(sdev, SCSI_REMOVAL_ALLOW);
	}

	/*
	 * XXX and what if there are packets in flight and this close()
	 * XXX is followed by a "rmmod sd_mod"?
	 */
	scsi_disk_put(sdkp);
	return 0;
}

static int sd_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	struct scsi_disk *sdkp = scsi_disk(bdev->bd_disk);
	struct scsi_device *sdp = sdkp->device;
	struct Scsi_Host *host = sdp->host;
	int diskinfo[4];

	/* default to most commonly used values */
        diskinfo[0] = 0x40;	/* 1 << 6 */
       	diskinfo[1] = 0x20;	/* 1 << 5 */
       	diskinfo[2] = sdkp->capacity >> 11;
	
	/* override with calculated, extended default, or driver values */
	if (host->hostt->bios_param)
		host->hostt->bios_param(sdp, bdev, sdkp->capacity, diskinfo);
	else
		scsicam_bios_param(bdev, sdkp->capacity, diskinfo);

	geo->heads = diskinfo[0];
	geo->sectors = diskinfo[1];
	geo->cylinders = diskinfo[2];
	return 0;
}

/**
 *	sd_ioctl - process an ioctl
 *	@inode: only i_rdev/i_bdev members may be used
 *	@filp: only f_mode and f_flags may be used
 *	@cmd: ioctl command number
 *	@arg: this is third argument given to ioctl(2) system call.
 *	Often contains a pointer.
 *
 *	Returns 0 if successful (some ioctls return postive numbers on
 *	success as well). Returns a negated errno value in case of error.
 *
 *	Note: most ioctls are forward onto the block subsystem or further
 *	down in the scsi subsystem.
 **/
static int sd_ioctl(struct block_device *bdev, fmode_t mode,
		    unsigned int cmd, unsigned long arg)
{
	struct gendisk *disk = bdev->bd_disk;
	struct scsi_device *sdp = scsi_disk(disk)->device;
	void __user *p = (void __user *)arg;
	int error;
    
	SCSI_LOG_IOCTL(1, printk("sd_ioctl: disk=%s, cmd=0x%x\n",
						disk->disk_name, cmd));

	/*
	 * If we are in the middle of error recovery, don't let anyone
	 * else try and use this device.  Also, if error recovery fails, it
	 * may try and take the device offline, in which case all further
	 * access to the device is prohibited.
	 */
	error = scsi_nonblockable_ioctl(sdp, cmd, p,
					(mode & FMODE_NDELAY) != 0);
	if (!scsi_block_when_processing_errors(sdp) || !error)
		return error;

	/*
	 * Send SCSI addressing ioctls directly to mid level, send other
	 * ioctls to block level and then onto mid level if they can't be
	 * resolved.
	 */
	switch (cmd) {
		case SCSI_IOCTL_GET_IDLUN:
		case SCSI_IOCTL_GET_BUS_NUMBER:
			return scsi_ioctl(sdp, cmd, p);
		default:
			error = scsi_cmd_ioctl(disk->queue, disk, mode, cmd, p);
			if (error != -ENOTTY)
				return error;
	}
	return scsi_ioctl(sdp, cmd, p);
}

static void set_media_not_present(struct scsi_disk *sdkp)
{
	sdkp->media_present = 0;
	sdkp->capacity = 0;
	sdkp->device->changed = 1;
}

/**
 *	sd_media_changed - check if our medium changed
 *	@disk: kernel device descriptor 
 *
 *	Returns 0 if not applicable or no change; 1 if change
 *
 *	Note: this function is invoked from the block subsystem.
 **/
static int sd_media_changed(struct gendisk *disk)
{
	struct scsi_disk *sdkp = scsi_disk(disk);
	struct scsi_device *sdp = sdkp->device;
	struct scsi_sense_hdr *sshdr = NULL;
	int retval;

	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp, "sd_media_changed\n"));

	if (!sdp->removable)
		return 0;

	/*
	 * If the device is offline, don't send any commands - just pretend as
	 * if the command failed.  If the device ever comes back online, we
	 * can deal with it then.  It is only because of unrecoverable errors
	 * that we would ever take a device offline in the first place.
	 */
	if (!scsi_device_online(sdp)) {
		set_media_not_present(sdkp);
		retval = 1;
		goto out;
	}

	/*
	 * Using TEST_UNIT_READY enables differentiation between drive with
	 * no cartridge loaded - NOT READY, drive with changed cartridge -
	 * UNIT ATTENTION, or with same cartridge - GOOD STATUS.
	 *
	 * Drives that auto spin down. eg iomega jaz 1G, will be started
	 * by sd_spinup_disk() from sd_revalidate_disk(), which happens whenever
	 * sd_revalidate() is called.
	 */
	retval = -ENODEV;

	if (scsi_block_when_processing_errors(sdp)) {
		sshdr  = kzalloc(sizeof(*sshdr), GFP_KERNEL);
		retval = scsi_test_unit_ready(sdp, SD_TIMEOUT, SD_MAX_RETRIES,
					      sshdr);
	}

	/*
	 * Unable to test, unit probably not ready.   This usually
	 * means there is no disc in the drive.  Mark as changed,
	 * and we will figure it out later once the drive is
	 * available again.
	 */
	if (retval || (scsi_sense_valid(sshdr) &&
		       /* 0x3a is medium not present */
		       sshdr->asc == 0x3a)) {
		set_media_not_present(sdkp);
		retval = 1;
		goto out;
	}

	/*
	 * For removable scsi disk we have to recognise the presence
	 * of a disk in the drive. This is kept in the struct scsi_disk
	 * struct and tested at open !  Daniel Roche (dan@lectra.fr)
	 */
	sdkp->media_present = 1;

	retval = sdp->changed;
	sdp->changed = 0;
out:
	if (retval != sdkp->previous_state)
		sdev_evt_send_simple(sdp, SDEV_EVT_MEDIA_CHANGE, GFP_KERNEL);
	sdkp->previous_state = retval;
	kfree(sshdr);
	return retval;
}

static int sd_sync_cache(struct scsi_disk *sdkp)
{
	int retries, res;
	struct scsi_device *sdp = sdkp->device;
	struct scsi_sense_hdr sshdr;

	if (!scsi_device_online(sdp))
		return -ENODEV;


	for (retries = 3; retries > 0; --retries) {
		unsigned char cmd[10] = { 0 };

		cmd[0] = SYNCHRONIZE_CACHE;
		/*
		 * Leave the rest of the command zero to indicate
		 * flush everything.
		 */
		res = scsi_execute_req(sdp, cmd, DMA_NONE, NULL, 0, &sshdr,
				       SD_TIMEOUT, SD_MAX_RETRIES, NULL);
		if (res == 0)
			break;
	}

	if (res) {
		sd_print_result(sdkp, res);
		if (driver_byte(res) & DRIVER_SENSE)
			sd_print_sense_hdr(sdkp, &sshdr);
	}

	if (res)
		return -EIO;
	return 0;
}

static void sd_prepare_flush(struct request_queue *q, struct request *rq)
{
	rq->cmd_type = REQ_TYPE_BLOCK_PC;
	rq->timeout = SD_TIMEOUT;
	rq->cmd[0] = SYNCHRONIZE_CACHE;
	rq->cmd_len = 10;
}

static void sd_rescan(struct device *dev)
{
	struct scsi_disk *sdkp = scsi_disk_get_from_dev(dev);

	if (sdkp) {
		revalidate_disk(sdkp->disk);
		scsi_disk_put(sdkp);
	}
}


#ifdef CONFIG_COMPAT
/* 
 * This gets directly called from VFS. When the ioctl 
 * is not recognized we go back to the other translation paths. 
 */
static int sd_compat_ioctl(struct block_device *bdev, fmode_t mode,
			   unsigned int cmd, unsigned long arg)
{
	struct scsi_device *sdev = scsi_disk(bdev->bd_disk)->device;

	/*
	 * If we are in the middle of error recovery, don't let anyone
	 * else try and use this device.  Also, if error recovery fails, it
	 * may try and take the device offline, in which case all further
	 * access to the device is prohibited.
	 */
	if (!scsi_block_when_processing_errors(sdev))
		return -ENODEV;
	       
	if (sdev->host->hostt->compat_ioctl) {
		int ret;

		ret = sdev->host->hostt->compat_ioctl(sdev, cmd, (void __user *)arg);

		return ret;
	}

	/* 
	 * Let the static ioctl translation table take care of it.
	 */
	return -ENOIOCTLCMD; 
}
#endif

static struct block_device_operations sd_fops = {
	.owner			= THIS_MODULE,
	.open			= sd_open,
	.release		= sd_release,
	.locked_ioctl		= sd_ioctl,
	.getgeo			= sd_getgeo,
#ifdef CONFIG_COMPAT
	.compat_ioctl		= sd_compat_ioctl,
#endif
	.media_changed		= sd_media_changed,
	.revalidate_disk	= sd_revalidate_disk,
};

static unsigned int sd_completed_bytes(struct scsi_cmnd *scmd)
{
	u64 start_lba = scmd->request->sector;
	u64 end_lba = scmd->request->sector + (scsi_bufflen(scmd) / 512);
	u64 bad_lba;
	int info_valid;

	if (!blk_fs_request(scmd->request))
		return 0;

	info_valid = scsi_get_sense_info_fld(scmd->sense_buffer,
					     SCSI_SENSE_BUFFERSIZE,
					     &bad_lba);
	if (!info_valid)
		return 0;

	if (scsi_bufflen(scmd) <= scmd->device->sector_size)
		return 0;

	if (scmd->device->sector_size < 512) {
		/* only legitimate sector_size here is 256 */
		start_lba <<= 1;
		end_lba <<= 1;
	} else {
		/* be careful ... don't want any overflows */
		u64 factor = scmd->device->sector_size / 512;
		do_div(start_lba, factor);
		do_div(end_lba, factor);
	}

	/* The bad lba was reported incorrectly, we have no idea where
	 * the error is.
	 */
	if (bad_lba < start_lba  || bad_lba >= end_lba)
		return 0;

	/* This computation should always be done in terms of
	 * the resolution of the device's medium.
	 */
	return (bad_lba - start_lba) * scmd->device->sector_size;
}

/**
 *	sd_done - bottom half handler: called when the lower level
 *	driver has completed (successfully or otherwise) a scsi command.
 *	@SCpnt: mid-level's per command structure.
 *
 *	Note: potentially run from within an ISR. Must not block.
 **/
static int sd_done(struct scsi_cmnd *SCpnt)
{
	int result = SCpnt->result;
	unsigned int good_bytes = result ? 0 : scsi_bufflen(SCpnt);
	struct scsi_sense_hdr sshdr;
	int sense_valid = 0;
	int sense_deferred = 0;

	if (result) {
		sense_valid = scsi_command_normalize_sense(SCpnt, &sshdr);
		if (sense_valid)
			sense_deferred = scsi_sense_is_deferred(&sshdr);
	}
#ifdef CONFIG_SCSI_LOGGING
	SCSI_LOG_HLCOMPLETE(1, scsi_print_result(SCpnt));
	if (sense_valid) {
		SCSI_LOG_HLCOMPLETE(1, scmd_printk(KERN_INFO, SCpnt,
						   "sd_done: sb[respc,sk,asc,"
						   "ascq]=%x,%x,%x,%x\n",
						   sshdr.response_code,
						   sshdr.sense_key, sshdr.asc,
						   sshdr.ascq));
	}
#endif
	if (driver_byte(result) != DRIVER_SENSE &&
	    (!sense_valid || sense_deferred))
		goto out;

	switch (sshdr.sense_key) {
	case HARDWARE_ERROR:
	case MEDIUM_ERROR:
		good_bytes = sd_completed_bytes(SCpnt);
		break;
	case RECOVERED_ERROR:
		good_bytes = scsi_bufflen(SCpnt);
		break;
	case NO_SENSE:
		/* This indicates a false check condition, so ignore it.  An
		 * unknown amount of data was transferred so treat it as an
		 * error.
		 */
		scsi_print_sense("sd", SCpnt);
		SCpnt->result = 0;
		memset(SCpnt->sense_buffer, 0, SCSI_SENSE_BUFFERSIZE);
		break;
	case ABORTED_COMMAND:
		if (sshdr.asc == 0x10) { /* DIF: Disk detected corruption */
			scsi_print_result(SCpnt);
			scsi_print_sense("sd", SCpnt);
			good_bytes = sd_completed_bytes(SCpnt);
		}
		break;
	case ILLEGAL_REQUEST:
		if (sshdr.asc == 0x10) { /* DIX: HBA detected corruption */
			scsi_print_result(SCpnt);
			scsi_print_sense("sd", SCpnt);
			good_bytes = sd_completed_bytes(SCpnt);
		}
		break;
	default:
		break;
	}
 out:
	if (rq_data_dir(SCpnt->request) == READ && scsi_prot_sg_count(SCpnt))
		sd_dif_complete(SCpnt, good_bytes);

	return good_bytes;
}

static int media_not_present(struct scsi_disk *sdkp,
			     struct scsi_sense_hdr *sshdr)
{

	if (!scsi_sense_valid(sshdr))
		return 0;
	/* not invoked for commands that could return deferred errors */
	if (sshdr->sense_key != NOT_READY &&
	    sshdr->sense_key != UNIT_ATTENTION)
		return 0;
	if (sshdr->asc != 0x3A) /* medium not present */
		return 0;

	set_media_not_present(sdkp);
	return 1;
}

/*
 * spinup disk - called only in sd_revalidate_disk()
 */
static void
sd_spinup_disk(struct scsi_disk *sdkp)
{
	unsigned char cmd[10];
	unsigned long spintime_expire = 0;
	int retries, spintime;
	unsigned int the_result;
	struct scsi_sense_hdr sshdr;
	int sense_valid = 0;

	spintime = 0;

	/* Spin up drives, as required.  Only do this at boot time */
	/* Spinup needs to be done for module loads too. */
	do {
		retries = 0;

		do {
			cmd[0] = TEST_UNIT_READY;
			memset((void *) &cmd[1], 0, 9);

			the_result = scsi_execute_req(sdkp->device, cmd,
						      DMA_NONE, NULL, 0,
						      &sshdr, SD_TIMEOUT,
						      SD_MAX_RETRIES, NULL);

			/*
			 * If the drive has indicated to us that it
			 * doesn't have any media in it, don't bother
			 * with any more polling.
			 */
			if (media_not_present(sdkp, &sshdr))
				return;

			if (the_result)
				sense_valid = scsi_sense_valid(&sshdr);
			retries++;
		} while (retries < 3 && 
			 (!scsi_status_is_good(the_result) ||
			  ((driver_byte(the_result) & DRIVER_SENSE) &&
			  sense_valid && sshdr.sense_key == UNIT_ATTENTION)));

		if ((driver_byte(the_result) & DRIVER_SENSE) == 0) {
			/* no sense, TUR either succeeded or failed
			 * with a status error */
			if(!spintime && !scsi_status_is_good(the_result)) {
				sd_printk(KERN_NOTICE, sdkp, "Unit Not Ready\n");
				sd_print_result(sdkp, the_result);
			}
			break;
		}
					
		/*
		 * The device does not want the automatic start to be issued.
		 */
		if (sdkp->device->no_start_on_add)
			break;

		if (sense_valid && sshdr.sense_key == NOT_READY) {
			if (sshdr.asc == 4 && sshdr.ascq == 3)
				break;	/* manual intervention required */
			if (sshdr.asc == 4 && sshdr.ascq == 0xb)
				break;	/* standby */
			if (sshdr.asc == 4 && sshdr.ascq == 0xc)
				break;	/* unavailable */
			/*
			 * Issue command to spin up drive when not ready
			 */
			if (!spintime) {
				sd_printk(KERN_NOTICE, sdkp, "Spinning up disk...");
				cmd[0] = START_STOP;
				cmd[1] = 1;	/* Return immediately */
				memset((void *) &cmd[2], 0, 8);
				cmd[4] = 1;	/* Start spin cycle */
				if (sdkp->device->start_stop_pwr_cond)
					cmd[4] |= 1 << 4;
				scsi_execute_req(sdkp->device, cmd, DMA_NONE,
						 NULL, 0, &sshdr,
						 SD_TIMEOUT, SD_MAX_RETRIES,
						 NULL);
				spintime_expire = jiffies + 100 * HZ;
				spintime = 1;
			}
			/* Wait 1 second for next try */
			msleep(1000);
			printk(".");

		/*
		 * Wait for USB flash devices with slow firmware.
		 * Yes, this sense key/ASC combination shouldn't
		 * occur here.  It's characteristic of these devices.
		 */
		} else if (sense_valid &&
				sshdr.sense_key == UNIT_ATTENTION &&
				sshdr.asc == 0x28) {
			if (!spintime) {
				spintime_expire = jiffies + 5 * HZ;
				spintime = 1;
			}
			/* Wait 1 second for next try */
			msleep(1000);
		} else {
			/* we don't understand the sense code, so it's
			 * probably pointless to loop */
			if(!spintime) {
				sd_printk(KERN_NOTICE, sdkp, "Unit Not Ready\n");
				sd_print_sense_hdr(sdkp, &sshdr);
			}
			break;
		}
				
	} while (spintime && time_before_eq(jiffies, spintime_expire));

	if (spintime) {
		if (scsi_status_is_good(the_result))
			printk("ready\n");
		else
			printk("not responding...\n");
	}
}


/*
 * Determine whether disk supports Data Integrity Field.
 */
void sd_read_protection_type(struct scsi_disk *sdkp, unsigned char *buffer)
{
	struct scsi_device *sdp = sdkp->device;
	u8 type;

	if (scsi_device_protection(sdp) == 0 || (buffer[12] & 1) == 0)
		type = 0;
	else
		type = ((buffer[12] >> 1) & 7) + 1; /* P_TYPE 0 = Type 1 */

	sdkp->protection_type = type;

	switch (type) {
	case SD_DIF_TYPE0_PROTECTION:
	case SD_DIF_TYPE1_PROTECTION:
	case SD_DIF_TYPE3_PROTECTION:
		break;

	case SD_DIF_TYPE2_PROTECTION:
		sd_printk(KERN_ERR, sdkp, "formatted with DIF Type 2 "	\
			  "protection which is currently unsupported. "	\
			  "Disabling disk!\n");
		goto disable;

	default:
		sd_printk(KERN_ERR, sdkp, "formatted with unknown "	\
			  "protection type %d. Disabling disk!\n", type);
		goto disable;
	}

	return;

disable:
	sdkp->capacity = 0;
}

static void read_capacity_error(struct scsi_disk *sdkp, struct scsi_device *sdp,
			struct scsi_sense_hdr *sshdr, int sense_valid,
			int the_result)
{
	sd_print_result(sdkp, the_result);
	if (driver_byte(the_result) & DRIVER_SENSE)
		sd_print_sense_hdr(sdkp, sshdr);
	else
		sd_printk(KERN_NOTICE, sdkp, "Sense not available.\n");

	/*
	 * Set dirty bit for removable devices if not ready -
	 * sometimes drives will not report this properly.
	 */
	if (sdp->removable &&
	    sense_valid && sshdr->sense_key == NOT_READY)
		sdp->changed = 1;

/*
	 * We used to set media_present to 0 here to indicate no media
	 * in the drive, but some drives fail read capacity even with
	 * media present, so we can't do that.
 */
	sdkp->capacity = 0; /* unknown mapped to zero - as usual */
}

#define RC16_LEN 32
#if RC16_LEN > SD_BUF_SIZE
#error RC16_LEN must not be more than SD_BUF_SIZE
#endif

static int read_capacity_16(struct scsi_disk *sdkp, struct scsi_device *sdp,
						unsigned char *buffer)
{
	unsigned char cmd[16];
	struct scsi_sense_hdr sshdr;
	int sense_valid = 0;
	int the_result;
	int retries = 3;
	unsigned long long lba;
	unsigned sector_size;

	do {
		memset(cmd, 0, 16);
			cmd[0] = SERVICE_ACTION_IN;
			cmd[1] = SAI_READ_CAPACITY_16;
		cmd[13] = RC16_LEN;
		memset(buffer, 0, RC16_LEN);

		the_result = scsi_execute_req(sdp, cmd, DMA_FROM_DEVICE,
					buffer, RC16_LEN, &sshdr,
					SD_TIMEOUT, SD_MAX_RETRIES, NULL);

		if (media_not_present(sdkp, &sshdr))
			return -ENODEV;

		if (the_result) {
			sense_valid = scsi_sense_valid(&sshdr);
			if (sense_valid &&
			    sshdr.sense_key == ILLEGAL_REQUEST &&
			    (sshdr.asc == 0x20 || sshdr.asc == 0x24) &&
			    sshdr.ascq == 0x00)
				/* Invalid Command Operation Code or
				 * Invalid Field in CDB, just retry
				 * silently with RC10 */
				return -EINVAL;
		}
		retries--;

	} while (the_result && retries);

	if (the_result) {
		sd_printk(KERN_NOTICE, sdkp, "READ CAPACITY(16) failed\n");
		read_capacity_error(sdkp, sdp, &sshdr, sense_valid, the_result);
		return -EINVAL;
	}

	sector_size = get_unaligned_be32(&buffer[8]);
	lba = get_unaligned_be64(&buffer[0]);

	sd_read_protection_type(sdkp, buffer);

	if ((sizeof(sdkp->capacity) == 4) && (lba >= 0xffffffffULL)) {
		sd_printk(KERN_ERR, sdkp, "Too big for this kernel. Use a "
			"kernel compiled with support for large block "
			"devices.\n");
		sdkp->capacity = 0;
		return -EOVERFLOW;
	}

	sdkp->capacity = lba + 1;
	return sector_size;
}

static int read_capacity_10(struct scsi_disk *sdkp, struct scsi_device *sdp,
						unsigned char *buffer)
{
	unsigned char cmd[16];
	struct scsi_sense_hdr sshdr;
	int sense_valid = 0;
	int the_result;
	int retries = 3;
	sector_t lba;
	unsigned sector_size;

	do {
			cmd[0] = READ_CAPACITY;
		memset(&cmd[1], 0, 9);
		memset(buffer, 0, 8);
		
		the_result = scsi_execute_req(sdp, cmd, DMA_FROM_DEVICE,
					buffer, 8, &sshdr,
					      SD_TIMEOUT, SD_MAX_RETRIES, NULL);

		if (media_not_present(sdkp, &sshdr))
			return -ENODEV;

		if (the_result)
			sense_valid = scsi_sense_valid(&sshdr);
		retries--;

	} while (the_result && retries);

	if (the_result) {
		sd_printk(KERN_NOTICE, sdkp, "READ CAPACITY failed\n");
		read_capacity_error(sdkp, sdp, &sshdr, sense_valid, the_result);
		return -EINVAL;
	}

	sector_size = get_unaligned_be32(&buffer[4]);
	lba = get_unaligned_be32(&buffer[0]);

	if ((sizeof(sdkp->capacity) == 4) && (lba == 0xffffffff)) {
		sd_printk(KERN_ERR, sdkp, "Too big for this kernel. Use a "
			"kernel compiled with support for large block "
			"devices.\n");
		sdkp->capacity = 0;
		return -EOVERFLOW;
	}

	sdkp->capacity = lba + 1;
	return sector_size;
}

static int sd_try_rc16_first(struct scsi_device *sdp)
{
	if (sdp->scsi_level > SCSI_SPC_2)
		return 1;
	if (scsi_device_protection(sdp))
		return 1;
	return 0;
	}	
	
/*
 * read disk capacity
 */
static void
sd_read_capacity(struct scsi_disk *sdkp, unsigned char *buffer)
{
	int sector_size;
	struct scsi_device *sdp = sdkp->device;
	sector_t old_capacity = sdkp->capacity;

	if (sd_try_rc16_first(sdp)) {
		sector_size = read_capacity_16(sdkp, sdp, buffer);
		if (sector_size == -EOVERFLOW)
			goto got_data;
		if (sector_size == -ENODEV)
			return;
		if (sector_size < 0)
			sector_size = read_capacity_10(sdkp, sdp, buffer);
		if (sector_size < 0)
			return;
	} else {
		sector_size = read_capacity_10(sdkp, sdp, buffer);
		if (sector_size == -EOVERFLOW)
			goto got_data;
		if (sector_size < 0)
			return;
		if ((sizeof(sdkp->capacity) > 4) &&
		    (sdkp->capacity > 0xffffffffULL)) {
			int old_sector_size = sector_size;
				sd_printk(KERN_NOTICE, sdkp, "Very big device. "
					  "Trying to use READ CAPACITY(16).\n");
			sector_size = read_capacity_16(sdkp, sdp, buffer);
			if (sector_size < 0) {
				sd_printk(KERN_NOTICE, sdkp,
					"Using 0xffffffff as device size\n");
				sdkp->capacity = 1 + (sector_t) 0xffffffff;
				sector_size = old_sector_size;
				goto got_data;
			}
		}
	}	

	/* Some devices are known to return the total number of blocks,
	 * not the highest block number.  Some devices have versions
	 * which do this and others which do not.  Some devices we might
	 * suspect of doing this but we don't know for certain.
	 *
	 * If we know the reported capacity is wrong, decrement it.  If
	 * we can only guess, then assume the number of blocks is even
	 * (usually true but not always) and err on the side of lowering
	 * the capacity.
	 */
	if (sdp->fix_capacity ||
	    (sdp->guess_capacity && (sdkp->capacity & 0x01))) {
		sd_printk(KERN_INFO, sdkp, "Adjusting the sector count "
				"from its reported value: %llu\n",
				(unsigned long long) sdkp->capacity);
				--sdkp->capacity;
	}

got_data:
	if (sector_size == 0) {
		sector_size = 512;
		sd_printk(KERN_NOTICE, sdkp, "Sector size 0 reported, "
			  "assuming 512.\n");
	}

	if (sector_size != 512 &&
	    sector_size != 1024 &&
	    sector_size != 2048 &&
	    sector_size != 4096 &&
	    sector_size != 256) {
		sd_printk(KERN_NOTICE, sdkp, "Unsupported sector size %d.\n",
			  sector_size);
		/*
		 * The user might want to re-format the drive with
		 * a supported sectorsize.  Once this happens, it
		 * would be relatively trivial to set the thing up.
		 * For this reason, we leave the thing in the table.
		 */
		sdkp->capacity = 0;
		/*
		 * set a bogus sector size so the normal read/write
		 * logic in the block layer will eventually refuse any
		 * request on this device without tripping over power
		 * of two sector size assumptions
		 */
		sector_size = 512;
	}
	blk_queue_hardsect_size(sdp->request_queue, sector_size);

	{
		char cap_str_2[10], cap_str_10[10];
		u64 sz = (u64)sdkp->capacity << ilog2(sector_size);

		string_get_size(sz, STRING_UNITS_2, cap_str_2,
				sizeof(cap_str_2));
		string_get_size(sz, STRING_UNITS_10, cap_str_10,
				sizeof(cap_str_10));

		if (sdkp->first_scan || old_capacity != sdkp->capacity)
		sd_printk(KERN_NOTICE, sdkp,
			  "%llu %d-byte hardware sectors: (%s/%s)\n",
			  (unsigned long long)sdkp->capacity,
			  sector_size, cap_str_10, cap_str_2);
	}

	/* Rescale capacity to 512-byte units */
	if (sector_size == 4096)
		sdkp->capacity <<= 3;
	else if (sector_size == 2048)
		sdkp->capacity <<= 2;
	else if (sector_size == 1024)
		sdkp->capacity <<= 1;
	else if (sector_size == 256)
		sdkp->capacity >>= 1;

	sdkp->device->sector_size = sector_size;

}

/* called with buffer of length 512 */
static inline int
sd_do_mode_sense(struct scsi_device *sdp, int dbd, int modepage,
		 unsigned char *buffer, int len, struct scsi_mode_data *data,
		 struct scsi_sense_hdr *sshdr)
{
	return scsi_mode_sense(sdp, dbd, modepage, buffer, len,
			       SD_TIMEOUT, SD_MAX_RETRIES, data,
			       sshdr);
}

/*
 * read write protect setting, if possible - called only in sd_revalidate_disk()
 * called with buffer of length SD_BUF_SIZE
 */
static void
sd_read_write_protect_flag(struct scsi_disk *sdkp, unsigned char *buffer)
{
	int res;
	struct scsi_device *sdp = sdkp->device;
	struct scsi_mode_data data;
	int old_wp = sdkp->write_prot;

	set_disk_ro(sdkp->disk, 0);
	if (sdp->skip_ms_page_3f) {
		sd_printk(KERN_NOTICE, sdkp, "Assuming Write Enabled\n");
		return;
	}

	if (sdp->use_192_bytes_for_3f) {
		res = sd_do_mode_sense(sdp, 0, 0x3F, buffer, 192, &data, NULL);
	} else {
		/*
		 * First attempt: ask for all pages (0x3F), but only 4 bytes.
		 * We have to start carefully: some devices hang if we ask
		 * for more than is available.
		 */
		res = sd_do_mode_sense(sdp, 0, 0x3F, buffer, 4, &data, NULL);

		/*
		 * Second attempt: ask for page 0 When only page 0 is
		 * implemented, a request for page 3F may return Sense Key
		 * 5: Illegal Request, Sense Code 24: Invalid field in
		 * CDB.
		 */
		if (!scsi_status_is_good(res))
			res = sd_do_mode_sense(sdp, 0, 0, buffer, 4, &data, NULL);

		/*
		 * Third attempt: ask 255 bytes, as we did earlier.
		 */
		if (!scsi_status_is_good(res))
			res = sd_do_mode_sense(sdp, 0, 0x3F, buffer, 255,
					       &data, NULL);
	}

	if (!scsi_status_is_good(res)) {
		sd_printk(KERN_WARNING, sdkp,
			  "Test WP failed, assume Write Enabled\n");
	} else {
		sdkp->write_prot = ((data.device_specific & 0x80) != 0);
		set_disk_ro(sdkp->disk, sdkp->write_prot);
		if (sdkp->first_scan || old_wp != sdkp->write_prot) {
		sd_printk(KERN_NOTICE, sdkp, "Write Protect is %s\n",
			  sdkp->write_prot ? "on" : "off");
		sd_printk(KERN_DEBUG, sdkp,
			  "Mode Sense: %02x %02x %02x %02x\n",
			  buffer[0], buffer[1], buffer[2], buffer[3]);
	}
}
}

/*
 * sd_read_cache_type - called only from sd_revalidate_disk()
 * called with buffer of length SD_BUF_SIZE
 */
static void
sd_read_cache_type(struct scsi_disk *sdkp, unsigned char *buffer)
{
	int len = 0, res;
	struct scsi_device *sdp = sdkp->device;

	int dbd;
	int modepage;
	struct scsi_mode_data data;
	struct scsi_sense_hdr sshdr;
	int old_wce = sdkp->WCE;
	int old_rcd = sdkp->RCD;
	int old_dpofua = sdkp->DPOFUA;

	if (sdp->skip_ms_page_8)
		goto defaults;

	if (sdp->type == TYPE_RBC) {
		modepage = 6;
		dbd = 8;
	} else {
		modepage = 8;
		dbd = 0;
	}

	/* cautiously ask */
	res = sd_do_mode_sense(sdp, dbd, modepage, buffer, 4, &data, &sshdr);

	if (!scsi_status_is_good(res))
		goto bad_sense;

	if (!data.header_length) {
		modepage = 6;
		sd_printk(KERN_ERR, sdkp, "Missing header in MODE_SENSE response\n");
	}

	/* that went OK, now ask for the proper length */
	len = data.length;

	/*
	 * We're only interested in the first three bytes, actually.
	 * But the data cache page is defined for the first 20.
	 */
	if (len < 3)
		goto bad_sense;
	if (len > 20)
		len = 20;

	/* Take headers and block descriptors into account */
	len += data.header_length + data.block_descriptor_length;
	if (len > SD_BUF_SIZE)
		goto bad_sense;

	/* Get the data */
	res = sd_do_mode_sense(sdp, dbd, modepage, buffer, len, &data, &sshdr);

	if (scsi_status_is_good(res)) {
		int offset = data.header_length + data.block_descriptor_length;

		if (offset >= SD_BUF_SIZE - 2) {
			sd_printk(KERN_ERR, sdkp, "Malformed MODE SENSE response\n");
			goto defaults;
		}

		if ((buffer[offset] & 0x3f) != modepage) {
			sd_printk(KERN_ERR, sdkp, "Got wrong page\n");
			goto defaults;
		}

		if (modepage == 8) {
			sdkp->WCE = ((buffer[offset + 2] & 0x04) != 0);
			sdkp->RCD = ((buffer[offset + 2] & 0x01) != 0);
		} else {
			sdkp->WCE = ((buffer[offset + 2] & 0x01) == 0);
			sdkp->RCD = 0;
		}

		sdkp->DPOFUA = (data.device_specific & 0x10) != 0;
		if (sdkp->DPOFUA && !sdkp->device->use_10_for_rw) {
			sd_printk(KERN_NOTICE, sdkp,
				  "Uses READ/WRITE(6), disabling FUA\n");
			sdkp->DPOFUA = 0;
		}

		if (sdkp->first_scan || old_wce != sdkp->WCE ||
		    old_rcd != sdkp->RCD || old_dpofua != sdkp->DPOFUA)
		sd_printk(KERN_NOTICE, sdkp,
		       "Write cache: %s, read cache: %s, %s\n",
		       sdkp->WCE ? "enabled" : "disabled",
		       sdkp->RCD ? "disabled" : "enabled",
		       sdkp->DPOFUA ? "supports DPO and FUA"
		       : "doesn't support DPO or FUA");

		return;
	}

bad_sense:
	if (scsi_sense_valid(&sshdr) &&
	    sshdr.sense_key == ILLEGAL_REQUEST &&
	    sshdr.asc == 0x24 && sshdr.ascq == 0x0)
		/* Invalid field in CDB */
		sd_printk(KERN_NOTICE, sdkp, "Cache data unavailable\n");
	else
		sd_printk(KERN_ERR, sdkp, "Asking for cache data failed\n");

defaults:
	sd_printk(KERN_ERR, sdkp, "Assuming drive cache: write through\n");
	sdkp->WCE = 0;
	sdkp->RCD = 0;
	sdkp->DPOFUA = 0;
}

/*
 * The ATO bit indicates whether the DIF application tag is available
 * for use by the operating system.
 */
void sd_read_app_tag_own(struct scsi_disk *sdkp, unsigned char *buffer)
{
	int res, offset;
	struct scsi_device *sdp = sdkp->device;
	struct scsi_mode_data data;
	struct scsi_sense_hdr sshdr;

	if (sdp->type != TYPE_DISK)
		return;

	if (sdkp->protection_type == 0)
		return;

	res = scsi_mode_sense(sdp, 1, 0x0a, buffer, 36, SD_TIMEOUT,
			      SD_MAX_RETRIES, &data, &sshdr);

	if (!scsi_status_is_good(res) || !data.header_length ||
	    data.length < 6) {
		sd_printk(KERN_WARNING, sdkp,
			  "getting Control mode page failed, assume no ATO\n");

		if (scsi_sense_valid(&sshdr))
			sd_print_sense_hdr(sdkp, &sshdr);

		return;
	}

	offset = data.header_length + data.block_descriptor_length;

	if ((buffer[offset] & 0x3f) != 0x0a) {
		sd_printk(KERN_ERR, sdkp, "ATO Got wrong page\n");
		return;
	}

	if ((buffer[offset + 5] & 0x80) == 0)
		return;

	sdkp->ATO = 1;

	return;
}

/**
 *	sd_revalidate_disk - called the first time a new disk is seen,
 *	performs disk spin up, read_capacity, etc.
 *	@disk: struct gendisk we care about
 **/
static int sd_revalidate_disk(struct gendisk *disk)
{
	struct scsi_disk *sdkp = scsi_disk(disk);
	struct scsi_device *sdp = sdkp->device;
	unsigned char *buffer;
	unsigned ordered;

	SCSI_LOG_HLQUEUE(3, sd_printk(KERN_INFO, sdkp,
				      "sd_revalidate_disk\n"));

	/*
	 * If the device is offline, don't try and read capacity or any
	 * of the other niceties.
	 */
	if (!scsi_device_online(sdp))
		goto out;

	buffer = kmalloc(SD_BUF_SIZE, GFP_KERNEL);
	if (!buffer) {
		sd_printk(KERN_WARNING, sdkp, "sd_revalidate_disk: Memory "
			  "allocation failure.\n");
		goto out;
	}

	sd_spinup_disk(sdkp);

	/*
	 * Without media there is no reason to ask; moreover, some devices
	 * react badly if we do.
	 */
	if (sdkp->media_present) {
		sd_read_capacity(sdkp, buffer);
		sd_read_write_protect_flag(sdkp, buffer);
		sd_read_cache_type(sdkp, buffer);
		sd_read_app_tag_own(sdkp, buffer);
	}

	sdkp->first_scan = 0;

	/*
	 * We now have all cache related info, determine how we deal
	 * with ordered requests.  Note that as the current SCSI
	 * dispatch function can alter request order, we cannot use
	 * QUEUE_ORDERED_TAG_* even when ordered tag is supported.
	 */
	if (sdkp->WCE)
		ordered = sdkp->DPOFUA
			? QUEUE_ORDERED_DRAIN_FUA : QUEUE_ORDERED_DRAIN_FLUSH;
	else
		ordered = QUEUE_ORDERED_DRAIN;

	blk_queue_ordered(sdkp->disk->queue, ordered, sd_prepare_flush);

	set_capacity(disk, sdkp->capacity);
	kfree(buffer);

 out:
	return 0;
}

/**
 *	sd_format_disk_name - format disk name
 *	@prefix: name prefix - ie. "sd" for SCSI disks
 *	@index: index of the disk to format name for
 *	@buf: output buffer
 *	@buflen: length of the output buffer
 *
 *	SCSI disk names starts at sda.  The 26th device is sdz and the
 *	27th is sdaa.  The last one for two lettered suffix is sdzz
 *	which is followed by sdaaa.
 *
 *	This is basically 26 base counting with one extra 'nil' entry
 *	at the beggining from the second digit on and can be
 *	determined using similar method as 26 base conversion with the
 *	index shifted -1 after each digit is computed.
 *
 *	CONTEXT:
 *	Don't care.
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
static int sd_format_disk_name(char *prefix, int index, char *buf, int buflen)
{
	const int base = 'z' - 'a' + 1;
	char *begin = buf + strlen(prefix);
	char *end = buf + buflen;
	char *p;
	int unit;

	p = end - 1;
	*p = '\0';
	unit = base;
	do {
		if (p == begin)
			return -EINVAL;
		*--p = 'a' + (index % unit);
		index = (index / unit) - 1;
	} while (index >= 0);

	memmove(begin, p, end - p);
	memcpy(buf, prefix, strlen(prefix));

	return 0;
}

/*
 * The asynchronous part of sd_probe
 */
static void sd_probe_async(void *data, async_cookie_t cookie)
{
	struct scsi_disk *sdkp = data;
	struct scsi_device *sdp;
	struct gendisk *gd;
	u32 index;
	struct device *dev;
/*add by l63336 for send signal to user space*/
    long pid;
/*add by l63336 for send signal to user space*/
	sdp = sdkp->device;
	gd = sdkp->disk;
	index = sdkp->index;
	dev = &sdp->sdev_gendev;

	if (!sdp->request_queue->rq_timeout) {
		if (sdp->type != TYPE_MOD)
			blk_queue_rq_timeout(sdp->request_queue, SD_TIMEOUT);
		else
			blk_queue_rq_timeout(sdp->request_queue,
					     SD_MOD_TIMEOUT);
	}

	device_initialize(&sdkp->dev);
	sdkp->dev.parent = &sdp->sdev_gendev;
	sdkp->dev.class = &sd_disk_class;
	dev_set_name(&sdkp->dev, dev_name(&sdp->sdev_gendev));

	if (device_add(&sdkp->dev))
		goto out_free_index;

	get_device(&sdp->sdev_gendev);

	if (index < SD_MAX_DISKS) {
		gd->major = sd_major((index & 0xf0) >> 4);
		gd->first_minor = ((index & 0xf) << 4) | (index & 0xfff00);
		gd->minors = SD_MINORS;
	}
	gd->fops = &sd_fops;
	gd->private_data = &sdkp->driver;
	gd->queue = sdkp->device->request_queue;

	/* defaults, until the device tells us otherwise */
	sdp->sector_size = 512;
	sdkp->capacity = 0;
	sdkp->media_present = 1;
	sdkp->write_prot = 0;
	sdkp->WCE = 0;
	sdkp->RCD = 0;
	sdkp->ATO = 0;
	sdkp->first_scan = 1;

	sd_revalidate_disk(gd);

	blk_queue_prep_rq(sdp->request_queue, sd_prep_fn);

	gd->driverfs_dev = &sdp->sdev_gendev;
	gd->flags = GENHD_FL_EXT_DEVT | GENHD_FL_DRIVERFS;
	if (sdp->removable)
		gd->flags |= GENHD_FL_REMOVABLE;

	dev_set_drvdata(dev, sdkp);
	add_disk(gd);
	sd_dif_config_host(sdkp);

	sd_printk(KERN_NOTICE, sdkp, "Attached SCSI %sdisk\n",
		  sdp->removable ? "removable " : "");

/*add by l63336 for send signal to user space*/
#ifdef ATP_SUPPORT_REPORT_USER
#if 0
            pid = simple_strtol(pid_str, NULL, 10); 
            if ( pid != 0 )
            {
                //printk("PID from scsi module, %d\n", pid);
                user_app = find_task_by_vpid(pid);
                /* send sigusr1 */
                if(user_app)
                {
                    send_sig(SIGUSR1, user_app, 0);
                }
            }
#endif
            syswatch_nl_send( ATP_MSG_MONITOR_EVT_USBSTORAGE_PLUGIN, NULL, 0 );
#endif
/*add by l63336 for send signal to user space*/

	return;

 out_free_index:
	ida_remove(&sd_index_ida, index);
}

/**
 *	sd_probe - called during driver initialization and whenever a
 *	new scsi device is attached to the system. It is called once
 *	for each scsi device (not just disks) present.
 *	@dev: pointer to device object
 *
 *	Returns 0 if successful (or not interested in this scsi device 
 *	(e.g. scanner)); 1 when there is an error.
 *
 *	Note: this function is invoked from the scsi mid-level.
 *	This function sets up the mapping between a given 
 *	<host,channel,id,lun> (found in sdp) and new device name 
 *	(e.g. /dev/sda). More precisely it is the block device major 
 *	and minor number that is chosen here.
 *
 *	Assume sd_attach is not re-entrant (for time being)
 *	Also think about sd_attach() and sd_remove() running coincidentally.
 **/
static int sd_probe(struct device *dev)
{
	struct scsi_device *sdp = to_scsi_device(dev);
	struct scsi_disk *sdkp;
	struct gendisk *gd;
	u32 index;
	int error;
    
      int giVendor;
      int giProduct;
      int giInterfaceClass;
      
    //add by zhouyu 2010-11-25 解决相同ID的USB设备web上只能识别一个的问题
    #if 0
        int len = 0;
        char buffer[512] = {0};/*buffer用来存放USB进程文件中的所有信息，512的长读足够存放4个规范的USB信息*/
        unsigned long ulDstAddr = (unsigned long)buffer;
        struct file *fp = NULL;
    #endif
        char DeviceId[16] = {0};
        char serialnum[5] = {0};
        char sero[5] = {0};
        char *ser = NULL;
        int    length = 0;
        int    n = 0 , i = 0;
    //add by zhouyu 2010-11-25 解决相同ID的USB设备web上只能识别一个的问题

    /*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.start*/
        unsigned char *buffer = NULL;
    /*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.end*/
       
	error = -ENODEV;
	if (sdp->type != TYPE_DISK && sdp->type != TYPE_MOD && sdp->type != TYPE_RBC)
		goto out;

	SCSI_LOG_HLQUEUE(3, sdev_printk(KERN_INFO, sdp,
					"sd_attach\n"));

	error = -ENOMEM;
	sdkp = kzalloc(sizeof(*sdkp), GFP_KERNEL);
	if (!sdkp)
		goto out;

/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.start*/
	  buffer = kmalloc(SD_BUF_SIZE, GFP_KERNEL);
    	  if (!buffer) {
    		goto out_free;
    	  }
/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.end*/

	gd = alloc_disk(SD_MINORS);
	if (!gd)
		goto out_free;

	do {
		if (!ida_pre_get(&sd_index_ida, GFP_KERNEL))
			goto out_put;

		spin_lock(&sd_index_lock);
		error = ida_get_new(&sd_index_ida, &index);
		spin_unlock(&sd_index_lock);
	} while (error == -EAGAIN);

	if (error)
		goto out_put;

	error = sd_format_disk_name("sd", index, gd->disk_name, DISK_NAME_LEN);
	if (error)
		goto out_free_index;

	sdkp->device = sdp;
	sdkp->driver = &sd_template;
	sdkp->disk = gd;
	sdkp->index = index;
	sdkp->openers = 0;
	sdkp->previous_state = 1;
        
	async_schedule(sd_probe_async, sdkp);

/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.start*/        
      sd_read_capacity(sdkp, buffer);
/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.end*/      
    
    /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
    {

        {
            struct usb_diskinfo *disk = NULL;
            struct Scsi_Host *host = sdp->host;
        	struct us_data *us = host_to_us(host);
        	char *string = NULL;
        	struct gendisk *sgp = gd;
        	char buf[BDEVNAME_SIZE];
        	 
        	/* Don't show non-partitionable removeable devices or empty devices */
        	/*if (!get_capacity(sgp) ||
        			(sgp->minors == 1 && (sgp->flags & GENHD_FL_REMOVABLE)))
        		goto next1;
        	if (sgp->flags & GENHD_FL_SUPPRESS_PARTITION_INFO)
        		goto next1;*/
            disk = get_freediskslot();
        	if(!disk)
        	{
        	    goto next1;
        	}	

              giVendor = le16_to_cpu(us->pusb_dev->descriptor.idVendor);
              giProduct = le16_to_cpu(us->pusb_dev->descriptor.iProduct);
              giInterfaceClass = us->pusb_intf->altsetting->desc.bInterfaceClass;

	       
/* <DTS2012081608784 g68080 2012-09-13 mod begin */
/* <DTS2012110605009 g68080 2012-11-16 mod begin */
#if 0           
            if (MDM9215_VENDOR_ID == giVendor)
            {
                    return;
		    }
#endif
/* <DTS2012110605009 g68080 2012-11-16 mod end */
/* <DTS2012081608784 g68080 2012-09-13 mod end */

          //add by zhouyu 2010-11-25 解决相同ID的USB设备web上只能识别一个的问题
              //序列号存在，ID的结构为vendor+serial后四位
              if (us->pusb_dev->serial)
              {
                  //记录该序列号的指针
                  ser = us->pusb_dev->serial;
                  length = strlen(us->pusb_dev->serial);
                  
                  //取序列号的后四位
                  //如果读出来的序列号长度小于4，则补0补够4位
                  if(length < 4)
                  {
                      n = 4 - length;
                      sprintf(serialnum ,"%s" , us->pusb_dev->serial);
                      strncpy(sero , "0000" , n);
                      strcat(serialnum , sero);
                  }
                  else
                  {
                      sprintf(serialnum ,"%s" , (ser + length - 4));
                  }
                  sprintf(DeviceId , "%04x%s" , giVendor , serialnum);
              }
              else
              {
               //序列号不存在，ID的结构为vendor+product
                  sprintf(DeviceId , "%04x%04x",giVendor,giProduct);
              }

          //delete by zhouyu 20101213统一LAN方式，经处理后的ID仍然相同则不再作加1处理
          #if 0
              //在往"/proc/proc_user_usbdevs"文件写信息之前先查一次是否要写的ID已经存在
              //如果存在就把当前的设备ID加1后再写入
              fp = filp_open("/proc/proc_user_usbdevs", O_RDONLY, 0);
              if (!IS_ERR(fp) && fp->f_op && fp->f_op->read)
              {
                    //忽略第一行标题行
                    fp->f_pos = 62;

                    if((len = (int) fp->f_op->read(fp, (void *) ulDstAddr, 512,&fp->f_pos)) > 0)
                    {
                        //循环四次，最多处理四个相同ID的USB设备
                        for(i = 0 ; i < 4 ; i++)
                        {
                            if(NULL != strstr(buffer , DeviceId))
                            {
                                if(0xffff == giVendor)
                                    giVendor = 0x0;
                                giVendor = giVendor + 1;

                                if (us->pusb_dev->serial)
                                {
                                    sprintf(DeviceId , "%04x%s" , giVendor , serialnum);
                                }
                                else
                                {
                                    sprintf(DeviceId , "%04x%04x",giVendor,giProduct);
                                }
                            }       
                        }
                    }
                    else
                    {
                        printk("Failed to read config file '%s'\n", "/proc/proc_user_usbdevs");
                    }      
              }
              else
              {
                    printk("fail to open file '/proc/proc_user_usbdevs'!\n");
              }

              filp_close(fp, NULL);
          #endif
          //delete by zhouyu 20101213统一LAN方式，经处理后的ID仍然相同则不再作加1处理

            //BT项目，为了和lan信息统一，把ID中的字母规范为大写字母
              for(i = 0 ; i < 8 ; i++)
                DeviceId[i] = toupper(DeviceId[i]);
            //BT项目，为了和lan信息统一，把ID中的字母规范为大写字母
              
              //最终写入proc文件的ID       
              sprintf(disk->model , "%s",DeviceId);
              printk("     device id: %s\n", disk->model);
          //add by zhouyu 2010-11-25 解决相同ID的USB设备web上只能识别一个的问题
              
             /*diskname,sda,sdb,...*/            
            	printk("       diskname: %s\n", gd->disk_name);
        	snprintf(disk->diskname,STRLEN-1,gd->disk_name);
                
        	/* print the controller name */
        	printk("   Host scsi%d: usb-storage\n", host->host_no);
            disk->scsihostid = host->host_no;

        	/* print product, vendor, and serial number strings */
        	if (NULL != us->pusb_dev->manufacturer)
        		string = us->pusb_dev->manufacturer;
        	else if (us->unusual_dev->vendorName)
        		string = us->unusual_dev->vendorName;
        	else
        		string = "Unknown";
        	printk("       Vendor: %s\n", string);
#if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268)
			if(strstr(string,"HUAWEI"))
			{
				g_isDataCard = 1;
			}
			else
			{
				g_isDataCard = 0;	
			}
#endif			
        	snprintf(disk->vendor,STRLEN-1,string);
            
              sprintf(disk->serial , "%d",giInterfaceClass);
        	/*if (us->pusb_dev->product)
        		string = us->pusb_dev->product;
        	else if (us->unusual_dev->productName)
        		string = us->unusual_dev->productName;
        	else
        		string = "Unknown";
        	printk("      Product: %s\n", string);
        	snprintf(disk->model,STRLEN-1,string);
        	
        	if (us->pusb_dev->serial)
        		string = us->pusb_dev->serial;
        	else
        		string = "None";
        	printk("Serial Number: %s\n", string);
        	snprintf(disk->serial,STRLEN-1,string);*/
            
             if(USB_SPEED_HIGH == us->pusb_dev->speed )
                    //snprintf(disk->connectiontype,SHORT_STRLEN - 1,"USB2.0,USB1.1");
                    snprintf(disk->connectiontype,SHORT_STRLEN - 1,"USB2.0");
             else 
                    snprintf(disk->connectiontype,SHORT_STRLEN - 1,"USB1.1");
             printk("connectiontype: %s\n", disk->connectiontype);
        	
    		disk->isvalid = 1;

/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.start*/ 
#if 0
        	disk->capacity = (int)get_capacity(sgp) >> 11;
#else
            disk->capacity = (((unsigned long long)(sdkp->capacity)*(sdkp->device->sector_size)/1024))/1024;//以M为单位
#endif
/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.end*/ 
       }

next1:
{
#ifdef ATP_SUPPORT_REPORT_USER
#if 0
    long pid = simple_strtol(pid_str, NULL, 10); 
    if ( pid != 0 )
    {
        //printk("PID from scsi module, %d\n", pid);
        user_app = find_task_by_vpid(pid);
        /* send sigusr1 */
        if(user_app)
        {
            send_sig(SIGUSR1, user_app, 0);
        }
    }
#endif
    syswatch_nl_send( ATP_MSG_MONITOR_EVT_USBSTORAGE_PLUGIN, NULL, 0 );
#endif
}	

/*add end*/
}
/*
Note:这里把各个芯片方案的USB点灯统一，包括U盘、数据卡、打印机等等，
如果没有特殊情况，请按照这个方案进行添加修改，尽量避免用各个芯片宏隔，以便维护管理和移植。
Add start for USB device led, DTS2010061800769, l67944, 20100625,
*/	
#ifdef CONFIG_ATP_BOARDLED 
    led_state_change(LED_NAME_USB,LED_STATE_ON);
	g_USB_PROBE = 1;
    printk("\n\n\n\nsd_probe: USB insert!!\n\n\n\n");
#endif
/*Add end, l67944, 20100625*/
#if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268)
	g_USB_PROBE = 1;
    printk("\n\n\n\nsd_probe: USB insert!!\n\n\n\n");
#endif

/* <DTS2012081608784 g68080 2012-09-13 mod begin */
#if defined (CONFIG_USB_LED)
    {
      unsigned int *pui_mem = NULL;
	  pui_mem=ioremap(USB_LED_REG, 4);
	  int val;
	  val=readl(pui_mem);
 	  val |=USB_LED_ON;
	  writel(val,pui_mem);

      iounmap(pui_mem);

	  pui_mem=ioremap(USB_LED_REGCONTROL, 4);
	  val=readl(pui_mem);
	  val |=USB_LED_ON;
	  writel(val,pui_mem);

	  iounmap(pui_mem);
	
      g_USB_PROBE = 1;
    }
#endif
/* <DTS2012081608784 g68080 2012-09-13 mod end */

    /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/

/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.start*/
       if(buffer != NULL)
            kfree(buffer);
       buffer = NULL;
/*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.end*/ 

	return 0;

 out_free_index:
	spin_lock(&sd_index_lock);
	ida_remove(&sd_index_ida, index);
	spin_unlock(&sd_index_lock);
 out_put:
	put_disk(gd);
 out_free:
	kfree(sdkp);

  /*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.start*/
       if(buffer != NULL)
            kfree(buffer);
       buffer = NULL;
  /*added by w00190448@20120714 解决读取u盘容量问题。之前一直是读取的0.end*/ 
 out:
	return error;
}

/**
 *	sd_remove - called whenever a scsi disk (previously recognized by
 *	sd_probe) is detached from the system. It is called (potentially
 *	multiple times) during sd module unload.
 *	@sdp: pointer to mid level scsi device object
 *
 *	Note: this function is invoked from the scsi mid-level.
 *	This function potentially frees up a device name (e.g. /dev/sdc)
 *	that could be re-used by a subsequent sd_probe().
 *	This function is not called when the built-in sd driver is "exit-ed".
 **/
static int sd_remove(struct device *dev)
{
	struct scsi_disk *sdkp = dev_get_drvdata(dev);
    
    /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
    {
        int n = 0;
        struct scsi_device *sdp = to_scsi_device(dev);
        struct Scsi_Host *host = sdp->host;

        for(n = 0;n < MAX_DISKS; n++)
        {
            if(!strcmp(sdkp->disk->disk_name,((struct usb_diskinfo *)diskinfos+n)->diskname))
            {
                printk("\r\nDISK(%s) is removed from diskinfos",((struct usb_diskinfo *)diskinfos+n)->vendor);
                ((struct usb_diskinfo *)diskinfos+n)->isvalid = 0; 
                attacheddisks--;
                if(	attacheddisks < 0)
                {
                    attacheddisks = 0;
                }	
            }	
        }

    }
    /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
	
/* <DTS2012091806613 g68080 2012-09-21 mod begin */
#if defined (CONFIG_USB_LED)
    /* < DTS2012102308899 l00205975 2012/10/30 begin  */
    /* 由于无线模块会多上报一个无用的storage device， 这里判断到剩下一个的时候就灭灯 */
    if(0 >= attacheddisks)    /* <DTS2012110605009 g68080 2012-11-16 mod 无线取消无用SD卡上报 */
    {
        unsigned int *pui_mem = NULL;
        pui_mem=ioremap(USB_LED_REG, 4);
        int val;
        val=readl(pui_mem);
        val |= USB_LED_ON;
        writel(val,pui_mem);

	iounmap(pui_mem);

       pui_mem=ioremap(USB_LED_REGCONTROL, 4);
	val=readl(pui_mem);
        val &=~USB_LED_ON;
        writel(val,pui_mem);

        iounmap(pui_mem);

        g_USB_PROBE = 0;
    }
    /* DTS2012102308899 l00205975 2012/10/30 end> */ 
#endif
/* <DTS2012091806613 g68080 2012-09-21 mod end */
    
	device_del(&sdkp->dev);
	del_gendisk(sdkp->disk);
	sd_shutdown(dev);

	mutex_lock(&sd_ref_mutex);
	dev_set_drvdata(dev, NULL);
	put_device(&sdkp->dev);
	mutex_unlock(&sd_ref_mutex);

	return 0;
}

/**
 *	scsi_disk_release - Called to free the scsi_disk structure
 *	@dev: pointer to embedded class device
 *
 *	sd_ref_mutex must be held entering this routine.  Because it is
 *	called on last put, you should always use the scsi_disk_get()
 *	scsi_disk_put() helpers which manipulate the semaphore directly
 *	and never do a direct put_device.
 **/
static void scsi_disk_release(struct device *dev)
{
	struct scsi_disk *sdkp = to_scsi_disk(dev);
	struct gendisk *disk = sdkp->disk;
	
	spin_lock(&sd_index_lock);
	ida_remove(&sd_index_ida, sdkp->index);
	spin_unlock(&sd_index_lock);

	disk->private_data = NULL;
	put_disk(disk);
	put_device(&sdkp->device->sdev_gendev);

	kfree(sdkp);
}

static int sd_start_stop_device(struct scsi_disk *sdkp, int start)
{
	unsigned char cmd[6] = { START_STOP };	/* START_VALID */
	struct scsi_sense_hdr sshdr;
	struct scsi_device *sdp = sdkp->device;
	int res;

	if (start)
		cmd[4] |= 1;	/* START */

	if (sdp->start_stop_pwr_cond)
		cmd[4] |= start ? 1 << 4 : 3 << 4;	/* Active or Standby */

	if (!scsi_device_online(sdp))
		return -ENODEV;

	res = scsi_execute_req(sdp, cmd, DMA_NONE, NULL, 0, &sshdr,
			       SD_TIMEOUT, SD_MAX_RETRIES, NULL);
	if (res) {
		sd_printk(KERN_WARNING, sdkp, "START_STOP FAILED\n");
		sd_print_result(sdkp, res);
		if (driver_byte(res) & DRIVER_SENSE)
			sd_print_sense_hdr(sdkp, &sshdr);
	}

	return res;
}

/*
 * Send a SYNCHRONIZE CACHE instruction down to the device through
 * the normal SCSI command structure.  Wait for the command to
 * complete.
 */
static void sd_shutdown(struct device *dev)
{
	struct scsi_disk *sdkp = scsi_disk_get_from_dev(dev);

	if (!sdkp)
		return;         /* this can happen */

	if (sdkp->WCE) {
		sd_printk(KERN_NOTICE, sdkp, "Synchronizing SCSI cache\n");
		sd_sync_cache(sdkp);
	}

	if (system_state != SYSTEM_RESTART && sdkp->device->manage_start_stop) {
		sd_printk(KERN_NOTICE, sdkp, "Stopping disk\n");
		sd_start_stop_device(sdkp, 0);
	}

	scsi_disk_put(sdkp);
}

static int sd_suspend(struct device *dev, pm_message_t mesg)
{
	struct scsi_disk *sdkp = scsi_disk_get_from_dev(dev);
	int ret = 0;

	if (!sdkp)
		return 0;	/* this can happen */

	if (sdkp->WCE) {
		sd_printk(KERN_NOTICE, sdkp, "Synchronizing SCSI cache\n");
		ret = sd_sync_cache(sdkp);
		if (ret)
			goto done;
	}

	if ((mesg.event & PM_EVENT_SLEEP) && sdkp->device->manage_start_stop) {
		sd_printk(KERN_NOTICE, sdkp, "Stopping disk\n");
		ret = sd_start_stop_device(sdkp, 0);
	}

done:
	scsi_disk_put(sdkp);
	return ret;
}

static int sd_resume(struct device *dev)
{
	struct scsi_disk *sdkp = scsi_disk_get_from_dev(dev);
	int ret = 0;

	if (!sdkp->device->manage_start_stop)
		goto done;

	sd_printk(KERN_NOTICE, sdkp, "Starting disk\n");
	ret = sd_start_stop_device(sdkp, 1);

done:
	scsi_disk_put(sdkp);
	return ret;
}
/*add by l63336 for send signal to user space*/
#ifdef ATP_SUPPORT_REPORT_USER
/* when some one writes to proc file this function will be invoked */
static int proc_user_pid_wr (struct file *file,
                             const char *buffer,
                             unsigned long count,
                             void *data)
{
   strcpy(pid_str, buffer);
   return count;
}

/* when some one reads proc file this function will be invoked */
static int proc_user_pid_rd (char *page, char **start,
                              off_t off, int count,
                              int *eof, void *data)
{
    int len = 0;

    len += sprintf (page + len, "%s",pid_str);
    return len;
}
#endif

/*add by l63336 for send signal to user space*/

/**
 *	init_sd - entry point for this driver (both when built in or when
 *	a module).
 *
 *	Note: this function registers this driver with the scsi mid-level.
 **/
static int __init init_sd(void)
{
	int majors = 0, i, err;

	SCSI_LOG_HLQUEUE(3, printk("init_sd: sd driver entry point\n"));
    
    /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
    proc_user_physicalmedium_create();
    /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
    
/*add by l63336 for send signal to user space*/
#ifdef ATP_SUPPORT_REPORT_USER
      proc_pid = create_proc_entry("proc_user_pid", S_IFREG | S_IRUSR , &proc_root ); 
      proc_pid->write_proc = proc_user_pid_wr;
      proc_pid->read_proc = proc_user_pid_rd;
#endif      
/*add by l63336 for send signal to user space*/

	for (i = 0; i < SD_MAJORS; i++)
		if (register_blkdev(sd_major(i), "sd") == 0)
			majors++;

	if (!majors)
		return -ENODEV;

	err = class_register(&sd_disk_class);
	if (err)
		goto err_out;

	err = scsi_register_driver(&sd_template.gendrv);
	if (err)
		goto err_out_class;

	return 0;

err_out_class:
	class_unregister(&sd_disk_class);
err_out:
	for (i = 0; i < SD_MAJORS; i++)
		unregister_blkdev(sd_major(i), "sd");
	return err;
}

/**
 *	exit_sd - exit point for this driver (when it is a module).
 *
 *	Note: this function unregisters this driver from the scsi mid-level.
 **/
static void __exit exit_sd(void)
{
	int i;

	SCSI_LOG_HLQUEUE(3, printk("exit_sd: exiting sd driver\n"));

     /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
     proc_user_physicalmedium_remove(); 
     /*Added by lvxin00135113@20091217 实现DT需求，实现TR140规范*/
     
	scsi_unregister_driver(&sd_template.gendrv);
	class_unregister(&sd_disk_class);

	for (i = 0; i < SD_MAJORS; i++)
		unregister_blkdev(sd_major(i), "sd");
}

module_init(init_sd);
module_exit(exit_sd);

static void sd_print_sense_hdr(struct scsi_disk *sdkp,
			       struct scsi_sense_hdr *sshdr)
{
	sd_printk(KERN_INFO, sdkp, "");
	scsi_show_sense_hdr(sshdr);
	sd_printk(KERN_INFO, sdkp, "");
	scsi_show_extd_sense(sshdr->asc, sshdr->ascq);
}

static void sd_print_result(struct scsi_disk *sdkp, int result)
{
	sd_printk(KERN_INFO, sdkp, "");
	scsi_show_result(result);
}

