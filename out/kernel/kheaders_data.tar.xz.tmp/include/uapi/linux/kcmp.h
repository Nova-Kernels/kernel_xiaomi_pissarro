/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_KCMP_H
#define _UAPI_LINUX_KCMP_H

#include <linux/types.h>


enum kcmp_type {
	KCMP_FILE,
	KCMP_VM,
	KCMP_FILES,
	KCMP_FS,
	KCMP_SIGHAND,
	KCMP_IO,
	KCMP_SYSVSEM,
	KCMP_EPOLL_TFD,

	KCMP_TYPES,
};


struct kcmp_epoll_slot {
	__u32 efd;		
	__u32 tfd;		
	__u32 toff;		
};

#endif 
