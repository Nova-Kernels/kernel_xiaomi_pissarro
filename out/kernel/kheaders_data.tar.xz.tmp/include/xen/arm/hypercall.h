

#ifndef _ASM_ARM_XEN_HYPERCALL_H
#define _ASM_ARM_XEN_HYPERCALL_H

#include <linux/bug.h>

#include <xen/interface/xen.h>
#include <xen/interface/sched.h>
#include <xen/interface/platform.h>

struct xen_dm_op_buf;

long privcmd_call(unsigned call, unsigned long a1,
		unsigned long a2, unsigned long a3,
		unsigned long a4, unsigned long a5);
int HYPERVISOR_xen_version(int cmd, void *arg);
int HYPERVISOR_console_io(int cmd, int count, char *str);
int HYPERVISOR_grant_table_op(unsigned int cmd, void *uop, unsigned int count);
int HYPERVISOR_sched_op(int cmd, void *arg);
int HYPERVISOR_event_channel_op(int cmd, void *arg);
unsigned long HYPERVISOR_hvm_op(int op, void *arg);
int HYPERVISOR_memory_op(unsigned int cmd, void *arg);
int HYPERVISOR_physdev_op(int cmd, void *arg);
int HYPERVISOR_vcpu_op(int cmd, int vcpuid, void *extra_args);
int HYPERVISOR_tmem_op(void *arg);
int HYPERVISOR_vm_assist(unsigned int cmd, unsigned int type);
int HYPERVISOR_dm_op(domid_t domid, unsigned int nr_bufs,
		     struct xen_dm_op_buf *bufs);
int HYPERVISOR_platform_op_raw(void *arg);
static inline int HYPERVISOR_platform_op(struct xen_platform_op *op)
{
	op->interface_version = XENPF_INTERFACE_VERSION;
	return HYPERVISOR_platform_op_raw(op);
}
int HYPERVISOR_multicall(struct multicall_entry *calls, uint32_t nr);

static inline int
HYPERVISOR_suspend(unsigned long start_info_mfn)
{
	struct sched_shutdown r = { .reason = SHUTDOWN_suspend };

	
	return HYPERVISOR_sched_op(SCHEDOP_shutdown, &r);
}

static inline void
MULTI_update_va_mapping(struct multicall_entry *mcl, unsigned long va,
			unsigned int new_val, unsigned long flags)
{
	BUG();
}

static inline void
MULTI_mmu_update(struct multicall_entry *mcl, struct mmu_update *req,
		 int count, int *success_count, domid_t domid)
{
	BUG();
}

#endif 
