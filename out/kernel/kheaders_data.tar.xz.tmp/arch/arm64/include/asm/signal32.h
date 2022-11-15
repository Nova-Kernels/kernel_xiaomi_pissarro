
#ifndef __ASM_SIGNAL32_H
#define __ASM_SIGNAL32_H

#ifdef __KERNEL__
#ifdef CONFIG_COMPAT
#include <linux/compat.h>

#define AARCH32_KERN_SIGRET_CODE_OFFSET	0x500

int compat_setup_frame(int usig, struct ksignal *ksig, sigset_t *set,
		       struct pt_regs *regs);
int compat_setup_rt_frame(int usig, struct ksignal *ksig, sigset_t *set,
			  struct pt_regs *regs);

void compat_setup_restart_syscall(struct pt_regs *regs);
#else

static inline int compat_setup_frame(int usid, struct ksignal *ksig,
				     sigset_t *set, struct pt_regs *regs)
{
	return -ENOSYS;
}

static inline int compat_setup_rt_frame(int usig, struct ksignal *ksig, sigset_t *set,
					struct pt_regs *regs)
{
	return -ENOSYS;
}

static inline void compat_setup_restart_syscall(struct pt_regs *regs)
{
}
#endif 
#endif 
#endif 
