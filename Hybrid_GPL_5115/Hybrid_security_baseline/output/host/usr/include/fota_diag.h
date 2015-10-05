#ifndef _FOTA_DIAG_H_
#define _FOTA_DIAG_H_

void FOTA_wirless_go_to_bootrom(void);
void FOTA_reset_wirless(void);
int FOTA_diag_init(unsigned int bufsize);
int FOTA_diag_end(void);
int FOTA_diag_write_pkg_buf(const char * pcBuf, unsigned int iBuflen);

#endif /* _FOTA_DIAG_H_ */

