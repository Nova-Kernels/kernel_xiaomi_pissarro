/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_PROCESSOR_H
#define _LINUX_PROCESSOR_H

#include <asm/processor.h>


#ifndef spin_begin
#define spin_begin()
#endif

#ifndef spin_cpu_relax
#define spin_cpu_relax() cpu_relax()
#endif


#ifndef spin_cpu_yield
#define spin_cpu_yield() cpu_relax_yield()
#endif

#ifndef spin_end
#define spin_end()
#endif


#ifndef spin_until_cond
#define spin_until_cond(cond)					\
do {								\
	if (unlikely(!(cond))) {				\
		spin_begin();					\
		do {						\
			spin_cpu_relax();			\
		} while (!(cond));				\
		spin_end();					\
	}							\
} while (0)

#endif

#endif 
