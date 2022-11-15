/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_CLOCK_H
#define _LINUX_SCHED_CLOCK_H

#include <linux/smp.h>


extern unsigned long long notrace sched_clock(void);


extern unsigned long long notrace sched_clock_get_cyc(
	unsigned long long *cyc_ret);


extern u64 running_clock(void);
extern u64 sched_clock_cpu(int cpu);


extern void sched_clock_init(void);

#ifndef CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
static inline void sched_clock_tick(void)
{
}

static inline void clear_sched_clock_stable(void)
{
}

static inline void sched_clock_idle_sleep_event(void)
{
}

static inline void sched_clock_idle_wakeup_event(void)
{
}

static inline u64 cpu_clock(int cpu)
{
	return sched_clock();
}

static inline u64 local_clock(void)
{
	return sched_clock();
}
#else
extern int sched_clock_stable(void);
extern void clear_sched_clock_stable(void);


extern u64 __sched_clock_offset;

extern void sched_clock_tick(void);
extern void sched_clock_tick_stable(void);
extern void sched_clock_idle_sleep_event(void);
extern void sched_clock_idle_wakeup_event(void);


static inline u64 cpu_clock(int cpu)
{
	return sched_clock_cpu(cpu);
}

static inline u64 local_clock(void)
{
	return sched_clock_cpu(raw_smp_processor_id());
}
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

extern void enable_sched_clock_irqtime(void);
extern void disable_sched_clock_irqtime(void);
#else
static inline void enable_sched_clock_irqtime(void) {}
static inline void disable_sched_clock_irqtime(void) {}
#endif

#endif 
