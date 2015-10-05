#ifndef __ATP_CADAVER_MSGTYPES_H__
#define __ATP_CADAVER_MSGTYPES_H__

#define ATP_CBBID_CADAVER_NAME       "cadaver"

enum ATP_CADAVER_MSGTYPES_EN
{
    ATP_MSG_CADAVER_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_CADAVER),
    ATP_CADAVER_PROCESS_STARTUP,            /* cadaver process startup successfully           */ 
    ATP_WEBDAV_EXIT_PROCESS,                /* ֪ͨ��ǰ��webdav�����˳�                       */
    ATP_WEBDAV_SET_PROXY,                   /* ֪ͨ��ǰ��webdav���ô���������               */
    ATP_WEBDAV_UNSET_PROXY,                 /* ֪ͨ��ǰ��webdavȡ������������               */
    ATP_WEBDAV_LOGON_NETWORKSTORAGE,        /* Ϊ��ǰ�û���¼������mediencenter               */
    ATP_WEBDAV_LOGOUT_NETWORKSTORAGE,       /* ȡ����ǰ��mediencenter֮�����������           */
    ATP_WEBDAV_CONNECT_CHECK,               /* �жϵ�ǰ��mediencenter֮�����������״̬       */
    ATP_WEBDAV_CHECK_STORAGE_SPACE,         /* ��鵱ǰ�洢�ռ���ÿռ��С�����ÿռ��С     */
    ATP_WEBDAV_CHECK_FOLDER_SPACE,          /* ���ĳ���ļ��еĴ�С                           */
    ATP_WEBDAV_GET_DIR_ALL_STATE,           /* ��ȡĳ���ļ�����ȫ���ļ����ļ�����Ϣ           */
    ATP_WEBDAV_GET_DIR_ALLSUB_DIR_STATE,    /* ��ȡĳ���ļ�����ȫ�����ļ�����Ϣ               */
    ATP_WEBDAV_GET_FILE_STATE,              /* ��ȡĳ���ļ�״̬                               */
    ATP_WEBDAV_CREATE_DIR,                  /* ����һ���ļ���                                 */
    ATP_WEBDAV_DELETE_DIR,                  /* ɾ��ĳ���ļ���                                 */
    ATP_WEBDAV_MV_DIR,                      /* �ƶ�ĳ���ļ���                                 */
    ATP_WEBDAV_COPY_DIR,                    /* ��ĳ���ļ��и�������һ��                       */
    ATP_WEBDAV_RENAME_DIR,                  /* ����ĳ���ļ�������                             */
    ATP_WEBDAV_CREATE_FILE,                 /* ����ĳ���ļ�                                   */
    ATP_WEBDAV_OPEN_FILE,                   /* ��ĳ���ļ�                                   */
    ATP_WEBDAV_WRITE_FILE,                  /* ������д��ĳ���ļ�                             */
    ATP_WEBDAV_CLOSE_FILE,                  /* �ر�ĳ���ļ�                                   */
    ATP_WEBDAV_DELETE_FILE,                 /* ɾ��ĳ���ļ�                                   */
    ATP_WEBDAV_MV_FILE,                     /* �ƶ�ĳ���ļ�                                   */
    ATP_WEBDAV_COPY_FILE,                   /* ��ĳ���ļ���������һ��                         */
    ATP_WEBDAV_RENAME_FILE,                 /* ��ĳ���ļ�����                                 */
    ATP_WEBDAV_UPLOAD_FILE,                 /* �ϴ�ĳ���ļ�                                   */
    ATP_WEBDAV_DOWNLOAD_FILE,               /* ����ĳ���ļ�                                   */
    ATP_WEBDAV_MODIFY_OPTIONS,              /* �޸�cadaverѡ��                                */
    ATP_WEBDAV_PROPFIND,                    /* ��ȡĳ��propertyֵ                             */
    ATP_WEBDAV_PROPSET,                     /* ����ĳ��propertyֵ                             */
    ATP_WEBDAV_LOCK,                        /* Ϊĳ��resource����                             */
    ATP_WEBDAV_UNLOCK,                      /* Ϊĳ��resourceȥ��                             */
    ATP_WEBDAV_GETMTIME,                    /* ��ȡĳ��resource������޸�ʱ���               */
    ATP_WEBDAV_SETMTIME,                    /* ����ĳ��resource������޸�ʱ���               */
} ;
#endif // End of __ATP_CADAVER_MSGTYPES_H__