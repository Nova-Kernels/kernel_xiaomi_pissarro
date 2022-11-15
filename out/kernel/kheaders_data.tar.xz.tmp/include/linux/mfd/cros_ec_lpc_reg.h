

#ifndef __LINUX_MFD_CROS_EC_REG_H
#define __LINUX_MFD_CROS_EC_REG_H


u8 cros_ec_lpc_read_bytes(unsigned int offset, unsigned int length, u8 *dest);


u8 cros_ec_lpc_write_bytes(unsigned int offset, unsigned int length, u8 *msg);


void cros_ec_lpc_reg_init(void);


void cros_ec_lpc_reg_destroy(void);

#endif 
