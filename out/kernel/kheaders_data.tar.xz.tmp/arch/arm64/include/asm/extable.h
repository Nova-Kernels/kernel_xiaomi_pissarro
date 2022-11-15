/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_EXTABLE_H
#define __ASM_EXTABLE_H



struct exception_table_entry
{
	int insn, fixup;
};

#define ARCH_HAS_RELATIVE_EXTABLE

extern int fixup_exception(struct pt_regs *regs);
#endif
