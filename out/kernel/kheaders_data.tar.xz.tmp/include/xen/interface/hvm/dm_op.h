

#ifndef __XEN_PUBLIC_HVM_DM_OP_H__
#define __XEN_PUBLIC_HVM_DM_OP_H__

struct xen_dm_op_buf {
	GUEST_HANDLE(void) h;
	xen_ulong_t size;
};
DEFINE_GUEST_HANDLE_STRUCT(xen_dm_op_buf);

#endif 
