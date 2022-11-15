

#ifndef _DRM_NOTIFIER_MI_H_
#define _DRM_NOTIFIER_MI_H_

#include <linux/notifier.h>


#define MI_DISP_DPMS_EVENT             0x01

#define MI_DISP_DPMS_EARLY_EVENT       0x02
#define MI_DISP_CHANGE_FPS	  0xF628

enum {
	
	MI_DISP_DPMS_ON   = 0,
	MI_DISP_DPMS_LP1       = 1,
	MI_DISP_DPMS_LP2       = 2,
	MI_DISP_DPMS_STANDBY   = 3,
	MI_DISP_DPMS_SUSPEND   = 4,
	
	MI_DISP_DPMS_POWERDOWN = 5,
};

enum mi_disp_id {
	MI_DISPLAY_PRIMARY = 0,
	MI_DISPLAY_SECONDARY,
	MI_DISPLAY_MAX,
};

struct mi_disp_notifier {
	int disp_id;
	void *data;
};

int mi_disp_register_client(struct notifier_block *nb);
int mi_disp_unregister_client(struct notifier_block *nb);
int mi_disp_notifier_call_chain(unsigned long val, void *v);

#endif 

