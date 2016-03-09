
#ifndef _DELTDIAG_H_
#define _DELTDIAG_H_

/*  DELT COMMANDS */

#define DELTSNRDN		1
#define DELTQLNDN		2
#define DELTHLINDNI		3
#define DELTHLINDNR		4
#define DELTBITSDN		5
#define DELTACTATP		6
#define DELTACTPSD		7
#define DELTGAINDN		8
#define DELTSNRM		9
#define DELTATTNDR		10


/*  DELT STRUCTURES */

typedef struct {
	unsigned int iToneNum ;
	int iValue ;
    int iActSize;
} ADSLCMD_DIAG_VAL_ST;

 
typedef struct {
	unsigned int uiStatSize ;
	ADSLCMD_DIAG_VAL_ST * pstDiagVal;
} ADSLCMD_DIAG_TEST_DATA_ST;


/*  DELT STATUS SIZES  */
#define DELTSNRDN_SZ		(512 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTQLNDN_SZ		(512 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTHLINDNI_SZ		(513 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTHLINDNR_SZ		(513 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTBITSDN_SZ		(1024 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTACTATP_SZ		(2 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTACTPSD_SZ		(2 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTGAINDN_SZ		(512 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTSNRM_SZ			(2 * sizeof(ADSLCMD_DIAG_VAL_ST))
#define DELTATTNDR_SZ		(2 * sizeof(ADSLCMD_DIAG_VAL_ST))


/*
 *	Useful notes
 *
 *	-> "tone_num" is not used for deltactatp 
 *
 *	-> For delthlindni and delthlindnr
 *		*)	status returned is "scale" plus 512 pairs of "tone_num" and "value"
 *			hence size is 513 pairs. "scale" is returned in the first "value" field
 *			of the first (index 0) pair so "tone_num" is invalid for first pair.
 *			Rest index 1 to 512 "tone_num" and "value" id passed.
 *
 */

/*
 *	How to use
 *
 *	Example:
 *
 *		int fd ;
 *		delthlindni* hlindni ;
 *
 *		fd = open(deltdiag, O_RDONLY)
 *
 *		if(hlindni.val = malloc(DELTHLINDNI_SZ))
 *			hlindni.stat_size = DELTHLINDNI_SZ ;
 *		else
 *			exit(1) ;
 *
 *		ioctl(fd, DELTHLINDNI, hlindni) ;
 *
 */
 
#endif
