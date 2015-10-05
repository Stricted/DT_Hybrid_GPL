#ifndef __ATP_CADAVER_MSGTYPES_H__
#define __ATP_CADAVER_MSGTYPES_H__

#define ATP_CBBID_CADAVER_NAME       "cadaver"

enum ATP_CADAVER_MSGTYPES_EN
{
    ATP_MSG_CADAVER_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CADAVER),
    ATP_CADAVER_PROCESS_STARTUP,            /* cadaver process startup successfully           */ 
    ATP_WEBDAV_EXIT_PROCESS,                /* 通知当前的webdav进程退出                       */
    ATP_WEBDAV_SET_PROXY,                   /* 通知当前的webdav采用代理建立连接               */
    ATP_WEBDAV_UNSET_PROXY,                 /* 通知当前的webdav取消代理建立连接               */
    ATP_WEBDAV_LOGON_NETWORKSTORAGE,        /* 为当前用户登录连接至mediencenter               */
    ATP_WEBDAV_LOGOUT_NETWORKSTORAGE,       /* 取消当前与mediencenter之间的网络连接           */
    ATP_WEBDAV_CONNECT_CHECK,               /* 判断当前与mediencenter之间的网络连接状态       */
    ATP_WEBDAV_CHECK_STORAGE_SPACE,         /* 检查当前存储空间可用空间大小及已用空间大小     */
    ATP_WEBDAV_CHECK_FOLDER_SPACE,          /* 检查某个文件夹的大小                           */
    ATP_WEBDAV_GET_DIR_ALL_STATE,           /* 获取某个文件夹下全部文件及文件夹信息           */
    ATP_WEBDAV_GET_DIR_ALLSUB_DIR_STATE,    /* 获取某个文件夹下全部子文件夹信息               */
    ATP_WEBDAV_GET_FILE_STATE,              /* 获取某个文件状态                               */
    ATP_WEBDAV_CREATE_DIR,                  /* 创建一个文件夹                                 */
    ATP_WEBDAV_DELETE_DIR,                  /* 删除某个文件夹                                 */
    ATP_WEBDAV_MV_DIR,                      /* 移动某个文件夹                                 */
    ATP_WEBDAV_COPY_DIR,                    /* 将某个文件夹复制至另一处                       */
    ATP_WEBDAV_RENAME_DIR,                  /* 更改某个文件夹名称                             */
    ATP_WEBDAV_CREATE_FILE,                 /* 创建某个文件                                   */
    ATP_WEBDAV_OPEN_FILE,                   /* 打开某个文件                                   */
    ATP_WEBDAV_WRITE_FILE,                  /* 将数据写入某个文件                             */
    ATP_WEBDAV_CLOSE_FILE,                  /* 关闭某个文件                                   */
    ATP_WEBDAV_DELETE_FILE,                 /* 删除某个文件                                   */
    ATP_WEBDAV_MV_FILE,                     /* 移动某个文件                                   */
    ATP_WEBDAV_COPY_FILE,                   /* 将某个文件复制至另一处                         */
    ATP_WEBDAV_RENAME_FILE,                 /* 将某个文件改名                                 */
    ATP_WEBDAV_UPLOAD_FILE,                 /* 上传某个文件                                   */
    ATP_WEBDAV_DOWNLOAD_FILE,               /* 下载某个文件                                   */
    ATP_WEBDAV_MODIFY_OPTIONS,              /* 修改cadaver选项                                */
    ATP_WEBDAV_PROPFIND,                    /* 获取某个property值                             */
    ATP_WEBDAV_PROPSET,                     /* 设置某个property值                             */
    ATP_WEBDAV_LOCK,                        /* 为某个resource上锁                             */
    ATP_WEBDAV_UNLOCK,                      /* 为某个resource去锁                             */
    ATP_WEBDAV_GETMTIME,                    /* 获取某个resource的最后修改时间戳               */
    ATP_WEBDAV_SETMTIME,                    /* 设置某个resource的最后修改时间戳               */
} ;
#endif // End of __ATP_CADAVER_MSGTYPES_H__