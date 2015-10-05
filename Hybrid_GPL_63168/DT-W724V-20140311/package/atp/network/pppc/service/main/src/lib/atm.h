/* atm.h - Functions useful for ATM applications */
 
/* Written 1995-2000 by Werner Almesberger, EPFL-LRC/ICA */
 

#ifndef _ATM_H
#define _ATM_H

#include <stdint.h>
#include <sys/socket.h>
#include <linux/atm.h>


/*
 * For versions of glibc < 2.1
 */

#ifndef AF_ATMPVC
#define AF_ATMPVC	8
#endif

#ifndef AF_ATMSVC
#define AF_ATMSVC	20
#endif

#ifndef PF_ATMPVC
#define PF_ATMPVC	AF_ATMPVC
#endif

#ifndef PF_ATMSVC
#define PF_ATMSVC	AF_ATMSVC
#endif

#ifndef SOL_ATM
#define SOL_ATM		264
#endif

#ifndef SOL_AAL
#define SOL_AAL		265
#endif

#ifdef SUPPORT_ATP_PPP_RALINK_ATM
struct atp_atm_trafprm {
	unsigned char	traffic_class;	/* traffic class (ATM_UBR, ...) */
	int		max_pcr;	/* maximum PCR in cells per second */
	int		pcr;		/* desired PCR in cells per second */
	int		scr;	 	/* desired SCR in cells per second */
	int		min_pcr;	/* minimum PCR in cells per second */
	int		max_cdv;	/* maximum CDV in microseconds */
	int		max_sdu;	/* maximum SDU in bytes */
	int     mbs;        /* maximum burst size (MBS) in cells */ 
    /* extra params for ABR */
    unsigned int 	icr;         	/* Initial Cell Rate (24-bit) */
    unsigned int	tbe;		/* Transient Buffer Exposure (24-bit) */ 
    unsigned int 	frtt : 24;	/* Fixed Round Trip Time (24-bit) */
    unsigned int 	rif  : 4;       /* Rate Increment Factor (4-bit) */
    unsigned int 	rdf  : 4;       /* Rate Decrease Factor (4-bit) */
    unsigned int nrm_pres  :1;      /* nrm present bit */
    unsigned int trm_pres  :1;     	/* rm present bit */
    unsigned int adtf_pres :1;     	/* adtf present bit */
    unsigned int cdf_pres  :1;    	/* cdf present bit*/
    unsigned int nrm       :3;     	/* Max # of Cells for each forward RM cell (3-bit) */
    unsigned int trm       :3;    	/* Time between forward RM cells (3-bit) */    
    unsigned int adtf      :10;     /* ACR Decrease Time Factor (10-bit) */
    unsigned int cdf       :3;      /* Cutoff Decrease Factor (3-bit) */
    unsigned int spare     :9;      /* spare bits */ 
};

#else
struct atp_atm_trafprm {
	unsigned char	traffic_class;	/* traffic class (ATM_UBR, ...) */
	int		max_pcr;	/* maximum PCR in cells per second */
	int		pcr;		/* desired PCR in cells per second */
	int		min_pcr;	/* minimum PCR in cells per second */
	int		max_cdv;	/* maximum CDV in microseconds */
	int		max_sdu;	/* maximum SDU in bytes */
/*add by l63336 ÊÊÅäBROADCOM ATM Çý¶¯*/	
#ifndef BROADCOM_CHIP
        int             scr;          
        int             mbs;          
        int             cdv;           
#endif
/*add by l63336 ÊÊÅäBROADCOM ATM Çý¶¯*/	
        /* extra params for ABR */
        unsigned int 	icr;         	/* Initial Cell Rate (24-bit) */
        unsigned int	tbe;		/* Transient Buffer Exposure (24-bit) */ 
        unsigned int 	frtt : 24;	/* Fixed Round Trip Time (24-bit) */
        unsigned int 	rif  : 4;       /* Rate Increment Factor (4-bit) */
        unsigned int 	rdf  : 4;       /* Rate Decrease Factor (4-bit) */
        unsigned int nrm_pres  :1;      /* nrm present bit */
        unsigned int trm_pres  :1;     	/* rm present bit */
        unsigned int adtf_pres :1;     	/* adtf present bit */
        unsigned int cdf_pres  :1;    	/* cdf present bit*/
        unsigned int nrm       :3;     	/* Max # of Cells for each forward RM cell (3-bit) */
        unsigned int trm       :3;    	/* Time between forward RM cells (3-bit) */    
	unsigned int adtf      :10;     /* ACR Decrease Time Factor (10-bit) */
	unsigned int cdf       :3;      /* Cutoff Decrease Factor (3-bit) */
        unsigned int spare     :9;      /* spare bits */ 
};
#endif

struct atp_atm_qos {
	struct atp_atm_trafprm txtp;	/* parameters in TX direction */
	struct atp_atm_trafprm rxtp __ATM_API_ALIGN;
					/* parameters in RX direction */
	unsigned char aal __ATM_API_ALIGN;
};

#define SO_ATP_ATMQOS	__SO_ENCODE(SOL_ATM,2,struct atp_atm_qos)
			    /* Quality of Service setting */

#define HOSTS_ATM "/etc/hosts.atm"

/* text2atm flags */
#define T2A_PVC		  1	/* address is PVC */
#define T2A_SVC		  2	/* address is SVC */
#define T2A_UNSPEC	  4	/* allow unspecified parts in PVC address */
#define T2A_WILDCARD	  8	/* allow wildcards in PVC or SVC address */
#define T2A_NNI		 16	/* allow NNI VPI range (PVC) */
#define T2A_NAME	 32	/* allow name resolution */
#define T2A_REMOTE	 64	/* OBSOLETE */
#define T2A_LOCAL	128	/* don't use ANS */

/* atm2text flags */
#define A2T_PRETTY	 1	/* add syntactic sugar */
#define A2T_NAME	 2	/* attempt name lookup */
#define A2T_REMOTE	 4	/* OBSOLETE */
#define A2T_LOCAL	 8	/* don't use ANS */

/* atm_equal flags */
#define AXE_WILDCARD	 1	/* allow wildcard match */
#define AXE_PRVOPT	 2	/* private part of SVC address is optional */

/* text2qos flags */
#define T2Q_DEFAULTS	 1	/* structure contains default values */

/* text2sap flags */
#define T2S_NAME	 1	/* attempt name lookup */
#define T2S_LOCAL	 2	/* we may support NIS or such in the future */

/* sap2text flags */
#define S2T_NAME	 1	/* attempt name lookup */
#define S2T_LOCAL	 2	/* we may support NIS or such in the future */

/* sap_equal flags */
#define SXE_COMPATIBLE	 1	/* check for compatibility instead of identity*/
#define SXE_NEGOTIATION	 2	/* allow negotiation; requires SXE_COMPATIBLE;
				   assumes "a" defines the available
				   capabilities */
#define SXE_RESULT	 4	/* return selected SAP */

#define MAX_ATM_ADDR_LEN (2*ATM_ESA_LEN+ATM_E164_LEN+5)
				/* 4 dots, 1 plus */
#define MAX_ATM_NAME_LEN 256	/* wild guess */
#define MAX_ATM_QOS_LEN 116	/* 5+4+2*(3+3*(7+9)+2)+1 */
#define MAX_ATM_SAP_LEN	255	/* BHLI(27)+1+3*BLLI(L2=33,L3=41,+1)+2 */


int text2atm(const char *text,struct sockaddr *addr,int length,int flags);
int atm2text(char *buffer,int length,const struct sockaddr *addr,int flags);
int atm_equal(const struct sockaddr *a,const struct sockaddr *b,int len,
  int flags);

int sdu2cell(int s,int sizes,const int *sdu_size,int *num_sdu);

int text2qos(const char *text,struct atm_qos *qos,int flags);
int qos2text(char *buffer,int length,const struct atm_qos *qos,int flags);
int qos_equal(const struct atm_qos *a,const struct atm_qos *b);

int text2sap(const char *text,struct atm_sap *sap,int flags);
int sap2text(char *buffer,int length,const struct atm_sap *sap,int flags);
int sap_equal(const struct atm_sap *a,const struct atm_sap *b,int flags,...);

int __t2q_get_rate(const char **text,int up);
int __atmlib_fetch(const char **pos,...); /* internal use only */

#endif
