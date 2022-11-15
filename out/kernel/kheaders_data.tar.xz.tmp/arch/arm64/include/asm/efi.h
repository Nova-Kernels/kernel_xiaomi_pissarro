/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_EFI_H
#define _ASM_EFI_H

#include <asm/boot.h>
#include <asm/cpufeature.h>
#include <asm/fpsimd.h>
#include <asm/io.h>
#include <asm/memory.h>
#include <asm/mmu_context.h>
#include <asm/neon.h>
#include <asm/ptrace.h>
#include <asm/tlbflush.h>

#ifdef CONFIG_EFI
extern void efi_init(void);
#else
#define efi_init()
#endif

int efi_create_mapping(struct mm_struct *mm, efi_memory_desc_t *md);
int efi_set_mapping_permissions(struct mm_struct *mm, efi_memory_desc_t *md);

#define arch_efi_call_virt_setup()					\
({									\
	efi_virtmap_load();						\
	__efi_fpsimd_begin();						\
})

#define arch_efi_call_virt(p, f, args...)				\
({									\
	efi_##f##_t *__f;						\
	__f = p->f;							\
	__f(args);							\
})

#define arch_efi_call_virt_teardown()					\
({									\
	__efi_fpsimd_end();						\
	efi_virtmap_unload();						\
})

#define ARCH_EFI_IRQ_FLAGS_MASK (PSR_D_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT)




#define EFI_FDT_ALIGN	SZ_2M   


#define EFI_KIMG_ALIGN	\
	(SEGMENT_ALIGN > THREAD_ALIGN ? SEGMENT_ALIGN : THREAD_ALIGN)


static inline unsigned long efi_get_max_fdt_addr(unsigned long dram_base)
{
	return ULONG_MAX;
}


static inline unsigned long efi_get_max_initrd_addr(unsigned long dram_base,
						    unsigned long image_addr)
{
	return (image_addr & ~(SZ_1G - 1UL)) + (1UL << (VA_BITS - 1));
}

#define efi_call_early(f, ...)		sys_table_arg->boottime->f(__VA_ARGS__)
#define __efi_call_early(f, ...)	f(__VA_ARGS__)
#define efi_call_runtime(f, ...)	sys_table_arg->runtime->f(__VA_ARGS__)
#define efi_is_64bit()			(true)

#define efi_call_proto(protocol, f, instance, ...)			\
	((protocol##_t *)instance)->f(instance, ##__VA_ARGS__)

#define alloc_screen_info(x...)		&screen_info

static inline void free_screen_info(efi_system_table_t *sys_table_arg,
				    struct screen_info *si)
{
}


extern struct screen_info screen_info __attribute__((__visibility__("hidden")));

static inline void efifb_setup_from_dmi(struct screen_info *si, const char *opt)
{
}

#define EFI_ALLOC_ALIGN		SZ_64K



static inline void efi_set_pgd(struct mm_struct *mm)
{
	__switch_mm(mm);

	if (system_uses_ttbr0_pan()) {
		if (mm != current->active_mm) {
			
			update_saved_ttbr0(current, mm);
			uaccess_ttbr0_enable();
			post_ttbr_update_workaround();
		} else {
			
			uaccess_ttbr0_disable();
			update_saved_ttbr0(current, current->active_mm);
		}
	}
}

void efi_virtmap_load(void);
void efi_virtmap_unload(void);

#endif 
