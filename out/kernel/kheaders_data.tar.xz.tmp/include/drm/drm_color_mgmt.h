

#ifndef __DRM_COLOR_MGMT_H__
#define __DRM_COLOR_MGMT_H__

#include <linux/ctype.h>

struct drm_crtc;

uint32_t drm_color_lut_extract(uint32_t user_input, uint32_t bit_precision);

void drm_crtc_enable_color_mgmt(struct drm_crtc *crtc,
				uint degamma_lut_size,
				bool has_ctm,
				uint gamma_lut_size);

int drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
				 int gamma_size);

#endif
