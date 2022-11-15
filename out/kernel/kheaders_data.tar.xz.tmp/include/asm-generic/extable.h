/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_EXTABLE_H
#define __ASM_GENERIC_EXTABLE_H



struct exception_table_entry
{
	unsigned long insn, fixup;
};


struct pt_regs;
extern int fixup_exception(struct pt_regs *regs);

#endif
