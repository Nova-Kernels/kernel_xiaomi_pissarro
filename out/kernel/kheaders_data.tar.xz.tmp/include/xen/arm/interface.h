/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _ASM_ARM_XEN_INTERFACE_H
#define _ASM_ARM_XEN_INTERFACE_H

#include <linux/types.h>

#define uint64_aligned_t uint64_t __attribute__((aligned(8)))

#define __DEFINE_GUEST_HANDLE(name, type) \
	typedef struct { union { type *p; uint64_aligned_t q; }; }  \
        __guest_handle_ ## name

#define DEFINE_GUEST_HANDLE_STRUCT(name) \
	__DEFINE_GUEST_HANDLE(name, struct name)
#define DEFINE_GUEST_HANDLE(name) __DEFINE_GUEST_HANDLE(name, name)
#define GUEST_HANDLE(name)        __guest_handle_ ## name

#define set_xen_guest_handle(hnd, val)			\
	do {						\
		if (sizeof(hnd) == 8)			\
			*(uint64_t *)&(hnd) = 0;	\
		(hnd).p = val;				\
	} while (0)

#define __HYPERVISOR_platform_op_raw __HYPERVISOR_platform_op

#ifndef __ASSEMBLY__

typedef uint64_t xen_pfn_t;
#define PRI_xen_pfn "llx"
typedef uint64_t xen_ulong_t;
#define PRI_xen_ulong "llx"
typedef int64_t xen_long_t;
#define PRI_xen_long "llx"

__DEFINE_GUEST_HANDLE(uchar, unsigned char);
__DEFINE_GUEST_HANDLE(uint,  unsigned int);
DEFINE_GUEST_HANDLE(char);
DEFINE_GUEST_HANDLE(int);
DEFINE_GUEST_HANDLE(void);
DEFINE_GUEST_HANDLE(uint64_t);
DEFINE_GUEST_HANDLE(uint32_t);
DEFINE_GUEST_HANDLE(xen_pfn_t);
DEFINE_GUEST_HANDLE(xen_ulong_t);


#define MAX_VIRT_CPUS 1

struct arch_vcpu_info { };
struct arch_shared_info { };


struct pvclock_vcpu_time_info {
	u32   version;
	u32   pad0;
	u64   tsc_timestamp;
	u64   system_time;
	u32   tsc_to_system_mul;
	s8    tsc_shift;
	u8    flags;
	u8    pad[2];
} __attribute__((__packed__)); 


struct pvclock_wall_clock {
	u32   version;
	u32   sec;
	u32   nsec;
	u32   sec_hi;
} __attribute__((__packed__));
#endif

#endif 
