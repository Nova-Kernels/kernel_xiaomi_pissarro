/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef _UAPI__ASM_SIGCONTEXT_H
#define _UAPI__ASM_SIGCONTEXT_H

#include <linux/types.h>


struct sigcontext {
	__u64 fault_address;
	
	__u64 regs[31];
	__u64 sp;
	__u64 pc;
	__u64 pstate;
	
	__u8 __reserved[4096] __attribute__((__aligned__(16)));
};




struct _aarch64_ctx {
	__u32 magic;
	__u32 size;
};

#define FPSIMD_MAGIC	0x46508001

struct fpsimd_context {
	struct _aarch64_ctx head;
	__u32 fpsr;
	__u32 fpcr;
	__uint128_t vregs[32];
};


#define ESR_MAGIC	0x45535201

struct esr_context {
	struct _aarch64_ctx head;
	__u64 esr;
};


#define EXTRA_MAGIC	0x45585401

struct extra_context {
	struct _aarch64_ctx head;
	__u64 datap; 
	__u32 size; 
	__u32 __reserved[3];
};

#endif 
