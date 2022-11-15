



#ifndef __DRM_CRTC_HELPER_H__
#define __DRM_CRTC_HELPER_H__

#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/idr.h>

#include <linux/fb.h>

#include <drm/drm_crtc.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_modeset_helper.h>

void drm_helper_disable_unused_functions(struct drm_device *dev);
int drm_crtc_helper_set_config(struct drm_mode_set *set,
			       struct drm_modeset_acquire_ctx *ctx);
bool drm_crtc_helper_set_mode(struct drm_crtc *crtc,
			      struct drm_display_mode *mode,
			      int x, int y,
			      struct drm_framebuffer *old_fb);
bool drm_helper_crtc_in_use(struct drm_crtc *crtc);
bool drm_helper_encoder_in_use(struct drm_encoder *encoder);

int drm_helper_connector_dpms(struct drm_connector *connector, int mode);

void drm_helper_resume_force_mode(struct drm_device *dev);

int drm_helper_crtc_mode_set(struct drm_crtc *crtc, struct drm_display_mode *mode,
			     struct drm_display_mode *adjusted_mode, int x, int y,
			     struct drm_framebuffer *old_fb);
int drm_helper_crtc_mode_set_base(struct drm_crtc *crtc, int x, int y,
				  struct drm_framebuffer *old_fb);


int drm_helper_probe_single_connector_modes(struct drm_connector
					    *connector, uint32_t maxX,
					    uint32_t maxY);
int drm_helper_probe_detect(struct drm_connector *connector,
			    struct drm_modeset_acquire_ctx *ctx,
			    bool force);
void drm_kms_helper_poll_init(struct drm_device *dev);
void drm_kms_helper_poll_fini(struct drm_device *dev);
bool drm_helper_hpd_irq_event(struct drm_device *dev);
void drm_kms_helper_hotplug_event(struct drm_device *dev);

void drm_kms_helper_poll_disable(struct drm_device *dev);
void drm_kms_helper_poll_enable(struct drm_device *dev);
bool drm_kms_helper_is_poll_worker(void);

#endif
