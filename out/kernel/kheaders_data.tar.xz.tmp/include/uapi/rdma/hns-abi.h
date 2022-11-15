/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR BSD-2-Clause) */


#ifndef HNS_ABI_USER_H
#define HNS_ABI_USER_H

#include <linux/types.h>

struct hns_roce_ib_create_cq {
	__u64   buf_addr;
};

struct hns_roce_ib_create_qp {
	__u64	buf_addr;
	__u64   db_addr;
	__u8    log_sq_bb_count;
	__u8    log_sq_stride;
	__u8    sq_no_prefetch;
	__u8    reserved[5];
};

struct hns_roce_ib_alloc_ucontext_resp {
	__u32	qp_tab_size;
};
#endif 
