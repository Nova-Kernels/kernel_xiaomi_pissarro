

#ifndef __CS35L35_H
#define __CS35L35_H

struct classh_cfg {
	
	bool classh_bst_override;
	bool classh_algo_enable;
	int classh_bst_max_limit;
	int classh_mem_depth;
	int classh_release_rate;
	int classh_headroom;
	int classh_wk_fet_disable;
	int classh_wk_fet_delay;
	int classh_wk_fet_thld;
	int classh_vpch_auto;
	int classh_vpch_rate;
	int classh_vpch_man;
};

struct monitor_cfg {
	
	bool is_present;
	bool imon_specs;
	bool vmon_specs;
	bool vpmon_specs;
	bool vbstmon_specs;
	bool vpbrstat_specs;
	bool zerofill_specs;
	u8 imon_dpth;
	u8 imon_loc;
	u8 imon_frm;
	u8 imon_scale;
	u8 vmon_dpth;
	u8 vmon_loc;
	u8 vmon_frm;
	u8 vpmon_dpth;
	u8 vpmon_loc;
	u8 vpmon_frm;
	u8 vbstmon_dpth;
	u8 vbstmon_loc;
	u8 vbstmon_frm;
	u8 vpbrstat_dpth;
	u8 vpbrstat_loc;
	u8 vpbrstat_frm;
	u8 zerofill_dpth;
	u8 zerofill_loc;
	u8 zerofill_frm;
};

struct cs35l35_platform_data {

	
	bool stereo;
	
	int sp_drv_str;
	
	int sp_drv_unused;
	
	bool bst_pdn_fet_on;
	
	int bst_vctl;
	
	int bst_ipk;
	
	bool gain_zc;
	
	int aud_channel;
	
	int adv_channel;
	
	bool shared_bst;
	
	bool ext_bst;
	
	int boost_ind;
	
	struct classh_cfg classh_algo;
	
	struct monitor_cfg mon_cfg;
};

#endif 
