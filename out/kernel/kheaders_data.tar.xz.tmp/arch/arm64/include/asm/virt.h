

#ifndef __ASM__VIRT_H
#define __ASM__VIRT_H




#define HVC_SET_VECTORS 0


#define HVC_SOFT_RESTART 1


#define HVC_RESET_VECTORS 2


#define HVC_STUB_HCALL_NR 3


#define HVC_STUB_ERR	0xbadca11

#define BOOT_CPU_MODE_EL1	(0xe11)
#define BOOT_CPU_MODE_EL2	(0xe12)

#ifndef __ASSEMBLY__

#include <asm/ptrace.h>
#include <asm/sections.h>
#include <asm/sysreg.h>
#include <asm/cpufeature.h>


extern u32 __boot_cpu_mode[2];

void __hyp_set_vectors(phys_addr_t phys_vector_base);
void __hyp_reset_vectors(void);


static inline bool is_hyp_mode_available(void)
{
	return (__boot_cpu_mode[0] == BOOT_CPU_MODE_EL2 &&
		__boot_cpu_mode[1] == BOOT_CPU_MODE_EL2);
}


static inline bool is_hyp_mode_mismatched(void)
{
	return __boot_cpu_mode[0] != __boot_cpu_mode[1];
}

static inline bool is_kernel_in_hyp_mode(void)
{
	return read_sysreg(CurrentEL) == CurrentEL_EL2;
}

static inline bool has_vhe(void)
{
	if (cpus_have_const_cap(ARM64_HAS_VIRT_HOST_EXTN))
		return true;

	return false;
}

#endif 

#endif 
