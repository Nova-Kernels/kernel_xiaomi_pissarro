/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_SCHED_TYPES_H
#define _UAPI_LINUX_SCHED_TYPES_H

#include <linux/types.h>

struct sched_param {
	int sched_priority;
};

#define SCHED_ATTR_SIZE_VER0	48	


struct sched_attr {
	__u32 size;

	__u32 sched_policy;
	__u64 sched_flags;

	
	__s32 sched_nice;

	
	__u32 sched_priority;

	
	__u64 sched_runtime;
	__u64 sched_deadline;
	__u64 sched_period;
};

#endif 
