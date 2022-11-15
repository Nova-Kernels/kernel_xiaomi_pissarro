

#ifndef __LINUX_SND_RT5645_H
#define __LINUX_SND_RT5645_H

struct rt5645_platform_data {
	
	bool in2_diff;

	unsigned int dmic1_data_pin;
	
	unsigned int dmic2_data_pin;
	

	unsigned int jd_mode;
	
	bool level_trigger_irq;
	
	bool inv_jd1_1;
};

#endif
