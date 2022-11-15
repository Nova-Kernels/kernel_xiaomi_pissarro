/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_T10_PI_H
#define _LINUX_T10_PI_H

#include <linux/types.h>
#include <linux/blkdev.h>


enum t10_dif_type {
	T10_PI_TYPE0_PROTECTION = 0x0,
	T10_PI_TYPE1_PROTECTION = 0x1,
	T10_PI_TYPE2_PROTECTION = 0x2,
	T10_PI_TYPE3_PROTECTION = 0x3,
};


struct t10_pi_tuple {
	__be16 guard_tag;	
	__be16 app_tag;		
	__be32 ref_tag;		
};

#define T10_PI_APP_ESCAPE cpu_to_be16(0xffff)
#define T10_PI_REF_ESCAPE cpu_to_be32(0xffffffff)

extern const struct blk_integrity_profile t10_pi_type1_crc;
extern const struct blk_integrity_profile t10_pi_type1_ip;
extern const struct blk_integrity_profile t10_pi_type3_crc;
extern const struct blk_integrity_profile t10_pi_type3_ip;

#endif
