#ifndef __ATCMDMSGTYPES_H__
#define __ATCMDMSGTYPES_H__

#define ATP_CBBID_ATCMD_NAME    "atcmd"  
/* <DTS2013083001384 xiazhongyue 20130830 begin */
//!\brief CFM error code
/*!
 *    atcmdµÄ´íÎóÂë
 */
typedef enum tagATP_E_ERR_ATCMD
{
    ATP_E_ERR_ATCMD_PARA_INVALID = ATP_MNT_ERR_CODE_DEF(ATP_TEAM_MNT, ATP_MODULE_ATCMD),
  
    ATP_E_ERR_ATCMD_MAX
}ATP_E_ERR_ATCMD;

/*  DTS2013083001384 xiazhongyue 20130830 end > */
#endif
