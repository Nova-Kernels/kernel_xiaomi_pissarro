/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_DEBUG_H
#define _LINUX_SCHED_DEBUG_H



struct task_struct;
struct pid_namespace;

extern void dump_cpu_task(int cpu);


extern void show_state_filter(unsigned long state_filter);

static inline void show_state(void)
{
	show_state_filter(0);
}

struct pt_regs;

extern void show_regs(struct pt_regs *);


extern void show_stack(struct task_struct *task, unsigned long *sp);

extern void sched_show_task(struct task_struct *p);

#ifdef CONFIG_SCHED_DEBUG
struct seq_file;
extern void proc_sched_show_task(struct task_struct *p,
				 struct pid_namespace *ns, struct seq_file *m);
extern void proc_sched_set_task(struct task_struct *p);
#endif


#define __sched		__attribute__((__section__(".sched.text")))


extern char __sched_text_start[], __sched_text_end[];


extern int in_sched_functions(unsigned long addr);

#endif 
