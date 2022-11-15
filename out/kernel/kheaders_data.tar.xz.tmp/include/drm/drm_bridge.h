

#ifndef __DRM_BRIDGE_H__
#define __DRM_BRIDGE_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode_object.h>
#include <drm/drm_modes.h>

struct drm_bridge;
struct drm_panel;


struct drm_bridge_funcs {
	
	int (*attach)(struct drm_bridge *bridge);

	
	void (*detach)(struct drm_bridge *bridge);

	
	enum drm_mode_status (*mode_valid)(struct drm_bridge *crtc,
					   const struct drm_display_mode *mode);

	
	bool (*mode_fixup)(struct drm_bridge *bridge,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);
	
	void (*disable)(struct drm_bridge *bridge);

	
	void (*post_disable)(struct drm_bridge *bridge);

	
	void (*mode_set)(struct drm_bridge *bridge,
			 struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode);
	
	void (*pre_enable)(struct drm_bridge *bridge);

	
	void (*enable)(struct drm_bridge *bridge);
};


struct drm_bridge {
	struct drm_device *dev;
	struct drm_encoder *encoder;
	struct drm_bridge *next;
#ifdef CONFIG_OF
	struct device_node *of_node;
#endif
	struct list_head list;

	const struct drm_bridge_funcs *funcs;
	void *driver_private;
};

int drm_bridge_add(struct drm_bridge *bridge);
void drm_bridge_remove(struct drm_bridge *bridge);
struct drm_bridge *of_drm_find_bridge(struct device_node *np);
int drm_bridge_attach(struct drm_encoder *encoder, struct drm_bridge *bridge,
		      struct drm_bridge *previous);

bool drm_bridge_mode_fixup(struct drm_bridge *bridge,
			const struct drm_display_mode *mode,
			struct drm_display_mode *adjusted_mode);
enum drm_mode_status drm_bridge_mode_valid(struct drm_bridge *bridge,
					   const struct drm_display_mode *mode);
void drm_bridge_disable(struct drm_bridge *bridge);
void drm_bridge_post_disable(struct drm_bridge *bridge);
void drm_bridge_mode_set(struct drm_bridge *bridge,
			struct drm_display_mode *mode,
			struct drm_display_mode *adjusted_mode);
void drm_bridge_pre_enable(struct drm_bridge *bridge);
void drm_bridge_enable(struct drm_bridge *bridge);

#ifdef CONFIG_DRM_PANEL_BRIDGE
struct drm_bridge *drm_panel_bridge_add(struct drm_panel *panel,
					u32 connector_type);
void drm_panel_bridge_remove(struct drm_bridge *bridge);
struct drm_bridge *devm_drm_panel_bridge_add(struct device *dev,
					     struct drm_panel *panel,
					     u32 connector_type);
#endif

#endif
