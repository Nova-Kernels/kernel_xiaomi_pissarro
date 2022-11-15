

#ifndef __DRM_MODE_CONFIG_H__
#define __DRM_MODE_CONFIG_H__

#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/idr.h>
#include <linux/workqueue.h>

#include <drm/drm_modeset_lock.h>

struct drm_file;
struct drm_device;
struct drm_atomic_state;
struct drm_mode_fb_cmd2;
struct drm_format_info;


struct drm_mode_config_funcs {
	
	struct drm_framebuffer *(*fb_create)(struct drm_device *dev,
					     struct drm_file *file_priv,
					     const struct drm_mode_fb_cmd2 *mode_cmd);

	
	const struct drm_format_info *(*get_format_info)(const struct drm_mode_fb_cmd2 *mode_cmd);

	
	void (*output_poll_changed)(struct drm_device *dev);

	
	int (*atomic_check)(struct drm_device *dev,
			    struct drm_atomic_state *state);

	
	int (*atomic_commit)(struct drm_device *dev,
			     struct drm_atomic_state *state,
			     bool nonblock);

	
	struct drm_atomic_state *(*atomic_state_alloc)(struct drm_device *dev);

	
	void (*atomic_state_clear)(struct drm_atomic_state *state);

	
	void (*atomic_state_free)(struct drm_atomic_state *state);
};


struct drm_mode_config {
	
	struct mutex mutex;

	
	struct drm_modeset_lock connection_mutex;

	
	struct drm_modeset_acquire_ctx *acquire_ctx;

	
	struct mutex idr_mutex;

	
	struct idr crtc_idr;

	
	struct idr tile_idr;

	
	struct mutex fb_lock;
	
	int num_fb;
	
	struct list_head fb_list;

	
	spinlock_t connector_list_lock;
	
	int num_connector;
	
	struct ida connector_ida;
	
	struct list_head connector_list;
	
	int num_encoder;
	
	struct list_head encoder_list;

	
	int num_overlay_plane;
	
	int num_total_plane;
	
	struct list_head plane_list;

	
	int num_crtc;
	
	struct list_head crtc_list;

	
	struct list_head property_list;

	int min_width, min_height;
	int max_width, max_height;
	const struct drm_mode_config_funcs *funcs;
	resource_size_t fb_base;

	
	bool poll_enabled;
	bool poll_running;
	bool delayed_event;
	struct delayed_work output_poll_work;

	
	struct mutex blob_lock;

	
	struct list_head property_blob_list;

	

	
	struct drm_property *edid_property;
	
	struct drm_property *dpms_property;
	
	struct drm_property *path_property;
	
	struct drm_property *tile_property;
	
	struct drm_property *link_status_property;
	
	struct drm_property *plane_type_property;
	
	struct drm_property *prop_src_x;
	
	struct drm_property *prop_src_y;
	
	struct drm_property *prop_src_w;
	
	struct drm_property *prop_src_h;
	
	struct drm_property *prop_crtc_x;
	
	struct drm_property *prop_crtc_y;
	
	struct drm_property *prop_crtc_w;
	
	struct drm_property *prop_crtc_h;
	
	struct drm_property *prop_fb_id;
	
	struct drm_property *prop_in_fence_fd;
	
	struct drm_property *prop_out_fence_ptr;
	
	struct drm_property *prop_crtc_id;
	
	struct drm_property *prop_active;
	
	struct drm_property *prop_mode_id;

	
	struct drm_property *dvi_i_subconnector_property;
	
	struct drm_property *dvi_i_select_subconnector_property;

	
	struct drm_property *tv_subconnector_property;
	
	struct drm_property *tv_select_subconnector_property;
	
	struct drm_property *tv_mode_property;
	
	struct drm_property *tv_left_margin_property;
	
	struct drm_property *tv_right_margin_property;
	
	struct drm_property *tv_top_margin_property;
	
	struct drm_property *tv_bottom_margin_property;
	
	struct drm_property *tv_brightness_property;
	
	struct drm_property *tv_contrast_property;
	
	struct drm_property *tv_flicker_reduction_property;
	
	struct drm_property *tv_overscan_property;
	
	struct drm_property *tv_saturation_property;
	
	struct drm_property *tv_hue_property;

	
	struct drm_property *scaling_mode_property;
	
	struct drm_property *aspect_ratio_property;
	
	struct drm_property *degamma_lut_property;
	
	struct drm_property *degamma_lut_size_property;
	
	struct drm_property *ctm_property;
	
	struct drm_property *gamma_lut_property;
	
	struct drm_property *gamma_lut_size_property;

	
	struct drm_property *suggested_x_property;
	
	struct drm_property *suggested_y_property;

	
	struct drm_property *writeback_fb_id_property;

	
	struct drm_property *writeback_pixel_formats_property;
	
	struct drm_property *writeback_out_fence_ptr_property;

	
	uint32_t preferred_depth, prefer_shadow;

	
	bool async_page_flip;

	
	bool allow_fb_modifiers;

	
	struct drm_property *modifiers_property;

	
	uint32_t cursor_width, cursor_height;

	const struct drm_mode_config_helper_funcs *helper_private;
};

void drm_mode_config_init(struct drm_device *dev);
void drm_mode_config_reset(struct drm_device *dev);
void drm_mode_config_cleanup(struct drm_device *dev);

#endif
