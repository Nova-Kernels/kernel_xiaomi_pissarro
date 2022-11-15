/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR BSD-3-Clause) */


#ifndef _LINUX__HFI1_IOCTL_H
#define _LINUX__HFI1_IOCTL_H
#include <linux/types.h>


struct hfi1_user_info {
	
	__u32 userversion;
	__u32 pad;
	
	__u16 subctxt_cnt;
	__u16 subctxt_id;
	
	__u8 uuid[16];
};

struct hfi1_ctxt_info {
	__u64 runtime_flags;    
	__u32 rcvegr_size;      
	__u16 num_active;       
	__u16 unit;             
	__u16 ctxt;             
	__u16 subctxt;          
	__u16 rcvtids;          
	__u16 credits;          
	__u16 numa_node;        
	__u16 rec_cpu;          
	__u16 send_ctxt;        
	__u16 egrtids;          
	__u16 rcvhdrq_cnt;      
	__u16 rcvhdrq_entsize;  
	__u16 sdma_ring_size;   
};

struct hfi1_tid_info {
	
	__u64 vaddr;
	
	__u64 tidlist;
	
	__u32 tidcnt;
	
	__u32 length;
};


struct hfi1_base_info {
	
	__u32 hw_version;
	
	__u32 sw_version;
	
	__u16 jkey;
	__u16 padding1;
	
	__u32 bthqp;
	
	__u64 sc_credits_addr;
	
	__u64 pio_bufbase_sop;
	
	__u64 pio_bufbase;
	
	__u64 rcvhdr_bufbase;
	
	__u64 rcvegr_bufbase;
	
	__u64 sdma_comp_bufbase;
	
	__u64 user_regbase;
	
	__u64 events_bufbase;
	
	__u64 status_bufbase;
	
	__u64 rcvhdrtail_base;
	
	__u64 subctxt_uregbase;
	__u64 subctxt_rcvegrbuf;
	__u64 subctxt_rcvhdrbuf;
};
#endif 
