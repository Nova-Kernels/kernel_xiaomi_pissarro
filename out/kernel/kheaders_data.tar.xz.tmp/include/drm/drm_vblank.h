

#ifndef _DRM_VBLANK_H_
#define _DRM_VBLANK_H_

#include <linux/seqlock.h>
#include <linux/idr.h>
#include <linux/poll.h>

#include <drm/drm_file.h>
#include <drm/drm_modes.h>
#include <uapi/drm/drm.h>

struct drm_device;
struct drm_crtc;


struct drm_pending_vblank_event {
	
	struct drm_pending_event base;
	
	unsigned int pipe;
	
	struct drm_event_vblank event;
};


struct drm_vblank_crtc {
	
	struct drm_device *dev;
	
	wait_queue_head_t queue;	
	
	struct timer_list disable_timer;

	
	seqlock_t seqlock;		

	
	u32 count;
	
	struct timeval time;

	
	atomic_t refcount;		
	
	u32 last;
	
	unsigned int inmodeset;		
	
	unsigned int pipe;
	
	int framedur_ns;
	
	int linedur_ns;

	
	struct drm_display_mode hwmode;

	
	bool enabled;
};

int drm_vblank_init(struct drm_device *dev, unsigned int num_crtcs);
u32 drm_crtc_vblank_count(struct drm_crtc *crtc);
u32 drm_crtc_vblank_count_and_time(struct drm_crtc *crtc,
				   struct timeval *vblanktime);
void drm_crtc_send_vblank_event(struct drm_crtc *crtc,
			       struct drm_pending_vblank_event *e);
void drm_crtc_arm_vblank_event(struct drm_crtc *crtc,
			      struct drm_pending_vblank_event *e);
bool drm_handle_vblank(struct drm_device *dev, unsigned int pipe);
bool drm_crtc_handle_vblank(struct drm_crtc *crtc);
int drm_crtc_vblank_get(struct drm_crtc *crtc);
void drm_crtc_vblank_put(struct drm_crtc *crtc);
void drm_wait_one_vblank(struct drm_device *dev, unsigned int pipe);
void drm_crtc_wait_one_vblank(struct drm_crtc *crtc);
void drm_crtc_vblank_off(struct drm_crtc *crtc);
void drm_crtc_vblank_reset(struct drm_crtc *crtc);
void drm_crtc_vblank_on(struct drm_crtc *crtc);
u32 drm_crtc_accurate_vblank_count(struct drm_crtc *crtc);

bool drm_calc_vbltimestamp_from_scanoutpos(struct drm_device *dev,
					   unsigned int pipe, int *max_error,
					   struct timeval *vblank_time,
					   bool in_vblank_irq);
void drm_calc_timestamping_constants(struct drm_crtc *crtc,
				     const struct drm_display_mode *mode);
wait_queue_head_t *drm_crtc_vblank_waitqueue(struct drm_crtc *crtc);
#endif
