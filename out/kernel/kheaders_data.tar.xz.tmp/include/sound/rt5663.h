

#ifndef __LINUX_SND_RT5663_H
#define __LINUX_SND_RT5663_H

struct rt5663_platform_data {
	unsigned int dc_offset_l_manual;
	unsigned int dc_offset_r_manual;
	unsigned int dc_offset_l_manual_mic;
	unsigned int dc_offset_r_manual_mic;
};

#endif

