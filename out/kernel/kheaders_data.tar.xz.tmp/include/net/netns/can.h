/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __NETNS_CAN_H__
#define __NETNS_CAN_H__

#include <linux/spinlock.h>

struct dev_rcv_lists;
struct s_stats;
struct s_pstats;

struct netns_can {
#if IS_ENABLED(CONFIG_PROC_FS)
	struct proc_dir_entry *proc_dir;
	struct proc_dir_entry *pde_version;
	struct proc_dir_entry *pde_stats;
	struct proc_dir_entry *pde_reset_stats;
	struct proc_dir_entry *pde_rcvlist_all;
	struct proc_dir_entry *pde_rcvlist_fil;
	struct proc_dir_entry *pde_rcvlist_inv;
	struct proc_dir_entry *pde_rcvlist_sff;
	struct proc_dir_entry *pde_rcvlist_eff;
	struct proc_dir_entry *pde_rcvlist_err;
	struct proc_dir_entry *bcmproc_dir;
#endif

	
	struct dev_rcv_lists *can_rx_alldev_list;
	spinlock_t can_rcvlists_lock;
	struct timer_list can_stattimer;
	struct s_stats *can_stats;	
	struct s_pstats *can_pstats;	

	
	struct hlist_head cgw_list;
};

#endif 
