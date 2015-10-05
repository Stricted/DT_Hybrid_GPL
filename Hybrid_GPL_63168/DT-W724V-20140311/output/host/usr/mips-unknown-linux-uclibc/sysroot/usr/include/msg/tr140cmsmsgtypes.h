/*!
  \brief 需要发送的消息
  \param[in] ATP_MODULE_USBSTORAGECMS: 
  \return
  \retval
*/
#ifndef __ATP_TR140CMS_MSGTYPES_H__
#define __ATP_TR140CMS_MSGTYPES_H__

enum ATP_TR140CMS_MSGTYPES_EN
{
    ATP_MSG_TR140CMS_START = ATP_MSG_CATEGORY_DEF(ATP_MODULE_USBSTORAGECMS),

    ATP_MSG_TR140CMS_UNMOUNT,

};

#endif
