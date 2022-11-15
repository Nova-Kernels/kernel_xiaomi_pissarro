
#ifndef __ASM_MODULE_H
#define __ASM_MODULE_H

#include <asm-generic/module.h>

#define MODULE_ARCH_VERMAGIC	"aarch64"

#ifdef CONFIG_ARM64_MODULE_PLTS
struct mod_plt_sec {
	struct elf64_shdr	*plt;
	int			plt_num_entries;
	int			plt_max_entries;
};

struct mod_arch_specific {
	struct mod_plt_sec	core;
	struct mod_plt_sec	init;

	
	struct plt_entry 	*ftrace_trampoline;
};
#endif

u64 module_emit_plt_entry(struct module *mod, void *loc, const Elf64_Rela *rela,
			  Elf64_Sym *sym);

#ifdef CONFIG_RANDOMIZE_BASE
extern u64 module_alloc_base;
#else
#define module_alloc_base	((u64)_etext - MODULES_VSIZE)
#endif

struct plt_entry {
	
	__le32	mov0;	
	__le32	mov1;	
	__le32	mov2;	
	__le32	br;	
};

static inline struct plt_entry get_plt_entry(u64 val)
{
	
	return (struct plt_entry){
		cpu_to_le32(0x92800010 | (((~val      ) & 0xffff)) << 5),
		cpu_to_le32(0xf2a00010 | ((( val >> 16) & 0xffff)) << 5),
		cpu_to_le32(0xf2c00010 | ((( val >> 32) & 0xffff)) << 5),
		cpu_to_le32(0xd61f0200)
	};
}

static inline bool plt_entries_equal(const struct plt_entry *a,
				     const struct plt_entry *b)
{
	return a->mov0 == b->mov0 &&
	       a->mov1 == b->mov1 &&
	       a->mov2 == b->mov2;
}

#endif 
