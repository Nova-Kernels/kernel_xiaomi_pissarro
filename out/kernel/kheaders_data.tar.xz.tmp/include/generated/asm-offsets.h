#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__


#define TSK_ACTIVE_MM 1312 

#define TSK_TI_FLAGS 0 
#define TSK_TI_PREEMPT 24 
#define TSK_TI_ADDR_LIMIT 8 
#define TSK_TI_TTBR0 16 
#define TSK_STACK 56 
#define TI_CPU_EXCP 40 
#define TI_REGS_ON_EXCP 32 

#define THREAD_CPU_CONTEXT 2720 

#define S_X0 0 
#define S_X1 8 
#define S_X2 16 
#define S_X3 24 
#define S_X4 32 
#define S_X5 40 
#define S_X6 48 
#define S_X7 56 
#define S_X8 64 
#define S_X10 80 
#define S_X12 96 
#define S_X14 112 
#define S_X16 128 
#define S_X18 144 
#define S_X20 160 
#define S_X22 176 
#define S_X24 192 
#define S_X26 208 
#define S_X28 224 
#define S_LR 240 
#define S_SP 248 
#define S_COMPAT_SP 104 
#define S_PSTATE 264 
#define S_PC 256 
#define S_ORIG_X0 272 
#define S_SYSCALLNO 280 
#define S_ORIG_ADDR_LIMIT 288 
#define S_STACKFRAME 304 
#define S_FRAME_SIZE 320 

#define MM_CONTEXT_ID 736 

#define VMA_VM_MM 64 
#define VMA_VM_FLAGS 80 

#define VM_EXEC 4 

#define PAGE_SZ 4096 

#define DMA_BIDIRECTIONAL 0 
#define DMA_TO_DEVICE 1 
#define DMA_FROM_DEVICE 2 

#define CLOCK_REALTIME 0 
#define CLOCK_MONOTONIC 1 
#define CLOCK_MONOTONIC_RAW 4 
#define CLOCK_REALTIME_RES 100 
#define CLOCK_REALTIME_COARSE 5 
#define CLOCK_MONOTONIC_COARSE 6 
#define CLOCK_COARSE_RES 4000000 
#define NSEC_PER_SEC 1000000000 

#define VDSO_CS_CYCLE_LAST 0 
#define VDSO_RAW_TIME_SEC 8 
#define VDSO_RAW_TIME_NSEC 16 
#define VDSO_XTIME_CLK_SEC 24 
#define VDSO_XTIME_CLK_NSEC 32 
#define VDSO_XTIME_CRS_SEC 40 
#define VDSO_XTIME_CRS_NSEC 48 
#define VDSO_WTM_CLK_SEC 56 
#define VDSO_WTM_CLK_NSEC 64 
#define VDSO_TB_SEQ_COUNT 72 
#define VDSO_CS_MONO_MULT 76 
#define VDSO_CS_RAW_MULT 84 
#define VDSO_CS_SHIFT 80 
#define VDSO_TZ_MINWEST 88 
#define VDSO_TZ_DSTTIME 92 
#define VDSO_USE_SYSCALL 96 

#define TVAL_TV_SEC 0 
#define TVAL_TV_USEC 8 
#define TSPEC_TV_SEC 0 
#define TSPEC_TV_NSEC 8 

#define TZ_MINWEST 0 
#define TZ_DSTTIME 4 

#define CPU_BOOT_STACK 0 
#define CPU_BOOT_TASK 8 

#define CPU_SUSPEND_SZ 112 
#define CPU_CTX_SP 104 
#define MPIDR_HASH_MASK 0 
#define MPIDR_HASH_SHIFTS 8 
#define SLEEP_STACK_DATA_SYSTEM_REGS 0 
#define SLEEP_STACK_DATA_CALLEE_REGS 112 
#define ARM_SMCCC_RES_X0_OFFS 0 
#define ARM_SMCCC_RES_X2_OFFS 16 
#define ARM_SMCCC_QUIRK_ID_OFFS 0 
#define ARM_SMCCC_QUIRK_STATE_OFFS 8 

#define HIBERN_PBE_ORIG 8 
#define HIBERN_PBE_ADDR 0 
#define HIBERN_PBE_NEXT 16 
#define ARM64_FTR_SYSVAL 24 

#define TRAMP_VALIAS -279198056448 

#endif
