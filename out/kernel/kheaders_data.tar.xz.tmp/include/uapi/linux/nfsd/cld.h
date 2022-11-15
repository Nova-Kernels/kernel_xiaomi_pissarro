/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */


#ifndef _NFSD_CLD_H
#define _NFSD_CLD_H

#include <linux/types.h>


#define CLD_UPCALL_VERSION 1


#define NFS4_OPAQUE_LIMIT 1024

enum cld_command {
	Cld_Create,		
	Cld_Remove,		
	Cld_Check,		
	Cld_GraceDone,		
};


struct cld_name {
	__u16		cn_len;				
	unsigned char	cn_id[NFS4_OPAQUE_LIMIT];	
} __attribute__((packed));


struct cld_msg {
	__u8		cm_vers;		
	__u8		cm_cmd;			
	__s16		cm_status;		
	__u32		cm_xid;			
	union {
		__s64		cm_gracetime;	
		struct cld_name	cm_name;
	} __attribute__((packed)) cm_u;
} __attribute__((packed));

#endif 
