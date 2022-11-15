/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TRACE_H
#define _LINUX_TRACE_H

#ifdef CONFIG_TRACING

struct trace_export {
	struct trace_export __rcu	*next;
	void (*write)(const void *, unsigned int);
};

int register_ftrace_export(struct trace_export *export);
int unregister_ftrace_export(struct trace_export *export);

#endif	

#endif	
