/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : atcmdadapter.h
  作    者  : x00104602
  版    本  : 1.0
  创建日期  : 2008-5-3
  描    述  : ATCMD命令适配头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2008-5-3
     作    者   : x00104602
     修改内容   : 完成初稿

*********************************************************************************/

#ifndef __ATCMDADAPTER_H__
#define __ATCMDADAPTER_H__

#include "at_lib_fmc.h"

typedef int (cmd_func_t)(char **argv);

//int execATCMD(msg_q *msg_buf);
//int execATCMD_cimi(msg_q *msg_buf);
//int execATCMD_board(msg_q *msg_buf);
//int execATCMD_factory(msg_q *msg_buf);
//int execATCMD_swver(msg_q *msg_buf);



extern int at_cmdhwversion(char **argv);
extern int at_cmdcimi(char **argv);
extern int at_cmdboard(char **argv);
extern int at_cmdfactory(char **argv);
extern int at_cmdswver(char **argv);
extern int at_cmdcgdcont(char **argv);
extern int at_cmdsysinfo(char **argv);
extern int at_cmdsyscfg(char **argv);
extern int at_cmdcops(char **argv);
extern int at_cmdcpwd(char **argv);
extern int at_cmdclck(char **argv);
extern int at_cmdcpin(char **argv);
extern int at_cmdnewcpin(char **argv);
extern int at_cmdcmee(char **argv);
extern int at_cmdcfun(char **argv);
extern int at_cmdcardmode(char **argv);
extern int at_cmdcsq(char **argv);
extern int at_cmdsn(char **argv);
extern int at_cmdhsdpa(char **argv);
extern int at_cmdcpas(char **argv);
extern int at_cmdata(char **argv);
extern int at_cmdatd(char **argv);
extern int at_cmdchup(char **argv);
extern int at_cmdclip(char **argv);

extern int at_cmddtmf(char **argv);

extern int at_cmdddsetex(char **argv);

extern int at_cmdscid(char **argv);
/*cs_20080828  chenkeyun          begin*/
extern int at_cmdccfc(char **argv);

extern int at_cmdccwa(char **argv);

extern int at_cmdchld(char **argv);

extern int at_cmdcssn(char **argv);
/*cs_20080828  chenkeyun          end*/

/*Start of added for support STC hspa by lvxin00135113@20090812*/
extern int at_cmdsysmode(char **argv);
extern int at_cmdqcsysmode(char **argv);
extern int at_cmdqcpinstat(char **argv);
extern int at_cmdcgclass(char **argv);
extern int at_cmdcpinc(char **argv);
extern int at_cmdcreg(char **argv);

/*<AP7D04492  w00126555 20100330 add begin*/
#ifdef SUPPORT_ATP_PRESS_DIAL
extern int at_cmdpppmode(char **argv);
#endif
/*AP7D04492  w00126555 20100330 add end>*/

//#if defined(SUPPORT_ATP_WITHMODEM)
extern int at_cmdgetsn(char **argv);
extern int at_cmdstartmmi(char **argv);
extern int at_cmdsetantenna(char **argv);
/*<mmi l129990 20101117 add begin*/
extern int at_cmdmmirouter(char **argv);
/*mmi l129990 20101117 add end>*/
extern int at_cmdRestoreAMSS(char **argv);/*AP7D04599 CODE.EQUIPCMD wanggang 2010-04-27*/
//#endif
/*End of added for support STC hspa by lvxin00135113@20090812*/
/* <DTS2013020703926 z81004485 2013/2/18 begin  */
extern int at_cmdiccid(char **argv);
/* DTS2013020703926 z81004485 2013/2/18 end> */
#endif /* __ATCMDADAPTER_H__ */
