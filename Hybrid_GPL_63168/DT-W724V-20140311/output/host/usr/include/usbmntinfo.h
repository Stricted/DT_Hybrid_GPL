#ifndef _USBMNTINFO_H_
#define _USBMNTINFO_H_

#define MAX_USB_COUNT 16
#define MAX_NAME_SIZE 256
#define MAX_TYPE_SIZE 64

#define USB_MOUNTINFO_PATH "/var/usbmountinfo"
#define     ATP_USB_MOUNT_NTFS_USERMAPPING_FILE         "/var/NTFS_UserMapping"

#include <stdio.h>

typedef struct tagUSBMountInfo
{
    VOS_CHAR driveName[MAX_NAME_SIZE];
    VOS_CHAR drivePath[MAX_NAME_SIZE];
    VOS_CHAR driveType[MAX_TYPE_SIZE];
}USBMountInfo;

static VOS_VOID ParseUsbMountInfo( const char* pFileName, USBMountInfo usb[MAX_USB_COUNT] )
{
    VOS_CHAR infotmp[1024];
    VOS_CHAR* pcTmp = NULL;
    VOS_UINT32 ulIndex = 0;
    FILE* pf = fopen( pFileName, "rb" );
    if (NULL == pf)
        return;

    while(NULL != fgets( infotmp, sizeof(infotmp), pf ))
    {
        if (0 != strncasecmp( infotmp, "/dev/sd", strlen("/dev/sd")))
            continue;
        
        //解析
        pcTmp = strtok( infotmp, " " );
        if (pcTmp == NULL )
            continue;
        snprintf( usb[ulIndex].driveName, sizeof(usb[ulIndex].driveName), "%s", pcTmp );

        strtok( NULL, " " );
        pcTmp = strtok( NULL, " " );
        if (pcTmp == NULL )
            continue;
        
        snprintf( usb[ulIndex].drivePath, sizeof(usb[ulIndex].drivePath), "%s", pcTmp );
        strtok( NULL, " " );
        pcTmp = strtok( NULL, " " );
        if (pcTmp == NULL )
            continue;
        
        snprintf( usb[ulIndex].driveType, sizeof(usb[ulIndex].driveType), "%s", pcTmp );
        ulIndex ++;
        if (ulIndex == MAX_USB_COUNT)
            break;
    }
    fclose(pf);
}


static VOS_VOID UmountUsbBlock(USBMountInfo usb[MAX_USB_COUNT])
{
    VOS_UINT32 ulIndex = 0;
    VOS_CHAR strCmd[256];
    for ( ulIndex = 0; ulIndex < MAX_USB_COUNT; ++ ulIndex )
    {
        if ( strlen( usb[ulIndex].driveName) == 0 )
        {
            continue;
        }
        snprintf( strCmd, sizeof(strCmd), "umount %s", usb[ulIndex].driveName);
        ATP_Exec( strCmd );
    }
}

static VOS_VOID MountUsbBlock( USBMountInfo usb[MAX_USB_COUNT] )
{
    VOS_UINT32 ulIndex = 0;
    VOS_CHAR strCmd[256];
    for ( ulIndex = 0; ulIndex < MAX_USB_COUNT; ++ ulIndex )
    {
        if ( strlen( usb[ulIndex].driveName) == 0 )
        {
            continue;
        }
        if (!strcasecmp( usb[ulIndex].driveType, "vfat" ) )
        {
            snprintf( strCmd, sizeof(strCmd), "mount -t vfat %s %s -o iocharset=utf8,shortname=winnt", 
                usb[ulIndex].driveName, usb[ulIndex].drivePath );
        }
#ifdef SUPPORT_ATP_NTFS
        else if (!strcasecmp( usb[ulIndex].driveType, "fuseblk" ) )
        {
            snprintf( strCmd, sizeof(strCmd), "ntfs-3g %s %s -o force,usermapping="ATP_USB_MOUNT_NTFS_USERMAPPING_FILE, 
                usb[ulIndex].driveName, usb[ulIndex].drivePath );
        }
#endif
#ifdef SUPPORT_ATP_KERNEL_HFS_FS
        else if( !strcasecmp( usb[ulIndex].driveType, "hfs" ) )
        {
            snprintf( strCmd, sizeof(strCmd), "mount -t hfs %s %s -o codepage=cp437,iocharset=utf8,ro", 
                usb[ulIndex].driveName, usb[ulIndex].drivePath );
        }
#endif
#ifdef SUPPORT_ATP_KERNEL_HFSPLUS_FS
        else if( !strcasecmp( usb[ulIndex].driveType, "hfsplus" ) )
        {
            snprintf( strCmd, sizeof(strCmd), "mount -t hfsplus %s %s -o nls=utf8,rw", 
                usb[ulIndex].driveName, usb[ulIndex].drivePath );
        }
#endif
#if  defined(SUPPORT_ATP_KERNEL_EXT3_FS)||defined(SUPPORT_ATP_KERNEL_EXT2_FS)
        else if( !strcasecmp( usb[ulIndex].driveType, "ext3" ) )
        {
            snprintf( strCmd, sizeof(strCmd), "mount -t auto %s %s -o rw", 
                usb[ulIndex].driveName, usb[ulIndex].drivePath );
        }
#endif        
        ATP_Exec( strCmd );
    }
}

static VOS_VOID MountUsb()
{
    USBMountInfo usb[MAX_USB_COUNT];
    memset( usb, 0, sizeof(usb));
    //解析文件
    ParseUsbMountInfo( USB_MOUNTINFO_PATH,  usb);
    MountUsbBlock( usb );
}

static VOS_VOID UmountUsb()
{
    USBMountInfo usb[MAX_USB_COUNT];
    memset( usb, 0, sizeof(usb));
    //解析文件
    ParseUsbMountInfo( USB_MOUNTINFO_PATH,  usb);
    UmountUsbBlock( usb );
}

#endif