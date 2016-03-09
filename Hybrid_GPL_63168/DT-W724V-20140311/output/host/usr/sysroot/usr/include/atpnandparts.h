/*****************************************************************************
 *               Copyright (C) 2011, Huawei Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
******************************************************************************/
/**\file atpnandparts.h
  *
  *\copyright 2007-2020，华为技术有限公司
  *\author s00175839
  *\date  2012-6-5
*/
#ifndef __ATP_NAND_PARTS_H__
#define __ATP_NAND_PARTS_H__

int atp_setup_mtd_partitions(
                                    struct mtd_info *mtd,
                                    const unsigned char *pucBootParam, 
                                    unsigned int ulMainOrSlave, /* 0 main 1 slave*/
                                    struct mtd_partition **ppstParts,
                                    unsigned int *pulMtdNum);
#endif

