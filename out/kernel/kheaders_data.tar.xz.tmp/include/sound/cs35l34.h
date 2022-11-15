

#ifndef __CS35L34_H
#define __CS35L34_H

struct cs35l34_platform_data {
	
	bool aif_half_drv;
	
	bool digsft_disable;
	
	bool amp_inv;
	
	unsigned int boost_peak;
	
	unsigned int boost_ind;
	
	unsigned int boost_vtge;
	
	bool gain_zc_disable;
	
	unsigned int i2s_sdinloc;
	
	bool tdm_rising_edge;
};

#endif 
