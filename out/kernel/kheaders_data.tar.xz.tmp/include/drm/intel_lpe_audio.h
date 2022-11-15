

#ifndef _INTEL_LPE_AUDIO_H_
#define _INTEL_LPE_AUDIO_H_

#include <linux/types.h>
#include <linux/spinlock_types.h>

struct platform_device;

#define HDMI_MAX_ELD_BYTES	128

struct intel_hdmi_lpe_audio_port_pdata {
	u8 eld[HDMI_MAX_ELD_BYTES];
	int port;
	int pipe;
	int ls_clock;
	bool dp_output;
};

struct intel_hdmi_lpe_audio_pdata {
	struct intel_hdmi_lpe_audio_port_pdata port[3]; 
	int num_ports;
	int num_pipes;

	void (*notify_audio_lpe)(struct platform_device *pdev, int port); 
	spinlock_t lpe_audio_slock;
};

#endif 
