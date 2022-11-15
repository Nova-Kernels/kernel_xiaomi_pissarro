/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MSG_H
#define _LINUX_MSG_H

#include <linux/list.h>
#include <linux/time64.h>
#include <uapi/linux/msg.h>


struct msg_msg {
	struct list_head m_list;
	long m_type;
	size_t m_ts;		
	struct msg_msgseg *next;
	void *security;
	
};


struct msg_queue {
	struct kern_ipc_perm q_perm;
	time64_t q_stime;		
	time64_t q_rtime;		
	time64_t q_ctime;		
	unsigned long q_cbytes;		
	unsigned long q_qnum;		
	unsigned long q_qbytes;		
	pid_t q_lspid;			
	pid_t q_lrpid;			

	struct list_head q_messages;
	struct list_head q_receivers;
	struct list_head q_senders;
} __randomize_layout;

#endif 
