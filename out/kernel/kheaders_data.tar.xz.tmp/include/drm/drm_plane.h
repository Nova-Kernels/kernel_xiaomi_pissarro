

#ifndef __DRM_PLANE_H__
#define __DRM_PLANE_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode_object.h>

struct drm_crtc;
struct drm_printer;
struct drm_modeset_acquire_ctx;


struct drm_plane_state {
	struct drm_plane *plane;

	
	struct drm_crtc *crtc;

	
	struct drm_framebuffer *fb;

	
	struct dma_fence *fence;

	

	int32_t crtc_x;
	
	int32_t crtc_y;

	uint32_t crtc_w, crtc_h;

	
	uint32_t src_x, src_y;
	uint32_t src_h, src_w;

	
	unsigned int rotation;

	
	unsigned int zpos;
	unsigned int normalized_zpos;

	
	struct drm_rect src, dst;

	
	bool visible;

	struct drm_atomic_state *state;
};

static inline struct drm_rect
drm_plane_state_src(const struct drm_plane_state *state)
{
	struct drm_rect src = {
		.x1 = state->src_x,
		.y1 = state->src_y,
		.x2 = state->src_x + state->src_w,
		.y2 = state->src_y + state->src_h,
	};
	return src;
}

static inline struct drm_rect
drm_plane_state_dest(const struct drm_plane_state *state)
{
	struct drm_rect dest = {
		.x1 = state->crtc_x,
		.y1 = state->crtc_y,
		.x2 = state->crtc_x + state->crtc_w,
		.y2 = state->crtc_y + state->crtc_h,
	};
	return dest;
}


struct drm_plane_funcs {
	
	int (*update_plane)(struct drm_plane *plane,
			    struct drm_crtc *crtc, struct drm_framebuffer *fb,
			    int crtc_x, int crtc_y,
			    unsigned int crtc_w, unsigned int crtc_h,
			    uint32_t src_x, uint32_t src_y,
			    uint32_t src_w, uint32_t src_h,
			    struct drm_modeset_acquire_ctx *ctx);

	
	int (*disable_plane)(struct drm_plane *plane,
			     struct drm_modeset_acquire_ctx *ctx);

	
	void (*destroy)(struct drm_plane *plane);

	
	void (*reset)(struct drm_plane *plane);

	
	int (*set_property)(struct drm_plane *plane,
			    struct drm_property *property, uint64_t val);

	
	struct drm_plane_state *(*atomic_duplicate_state)(struct drm_plane *plane);

	
	void (*atomic_destroy_state)(struct drm_plane *plane,
				     struct drm_plane_state *state);

	
	int (*atomic_set_property)(struct drm_plane *plane,
				   struct drm_plane_state *state,
				   struct drm_property *property,
				   uint64_t val);

	
	int (*atomic_get_property)(struct drm_plane *plane,
				   const struct drm_plane_state *state,
				   struct drm_property *property,
				   uint64_t *val);
	
	int (*late_register)(struct drm_plane *plane);

	
	void (*early_unregister)(struct drm_plane *plane);

	
	void (*atomic_print_state)(struct drm_printer *p,
				   const struct drm_plane_state *state);

	
	bool (*format_mod_supported)(struct drm_plane *plane, uint32_t format,
				     uint64_t modifier);
};


enum drm_plane_type {
	
	DRM_PLANE_TYPE_OVERLAY,

	
	DRM_PLANE_TYPE_PRIMARY,

	
	DRM_PLANE_TYPE_CURSOR,
};



struct drm_plane {
	struct drm_device *dev;
	struct list_head head;

	char *name;

	
	struct drm_modeset_lock mutex;

	struct drm_mode_object base;

	uint32_t possible_crtcs;
	uint32_t *format_types;
	unsigned int format_count;
	bool format_default;

	uint64_t *modifiers;
	unsigned int modifier_count;

	struct drm_crtc *crtc;
	struct drm_framebuffer *fb;

	struct drm_framebuffer *old_fb;

	const struct drm_plane_funcs *funcs;

	struct drm_object_properties properties;

	enum drm_plane_type type;

	
	unsigned index;

	const struct drm_plane_helper_funcs *helper_private;

	
	struct drm_plane_state *state;

	struct drm_property *zpos_property;
	struct drm_property *rotation_property;
};

#define obj_to_plane(x) container_of(x, struct drm_plane, base)

__printf(9, 10)
int drm_universal_plane_init(struct drm_device *dev,
			     struct drm_plane *plane,
			     uint32_t possible_crtcs,
			     const struct drm_plane_funcs *funcs,
			     const uint32_t *formats,
			     unsigned int format_count,
			     const uint64_t *format_modifiers,
			     enum drm_plane_type type,
			     const char *name, ...);
int drm_plane_init(struct drm_device *dev,
		   struct drm_plane *plane,
		   uint32_t possible_crtcs,
		   const struct drm_plane_funcs *funcs,
		   const uint32_t *formats, unsigned int format_count,
		   bool is_primary);
void drm_plane_cleanup(struct drm_plane *plane);


static inline unsigned int drm_plane_index(struct drm_plane *plane)
{
	return plane->index;
}
struct drm_plane * drm_plane_from_index(struct drm_device *dev, int idx);
void drm_plane_force_disable(struct drm_plane *plane);

int drm_mode_plane_set_obj_prop(struct drm_plane *plane,
				       struct drm_property *property,
				       uint64_t value);


static inline struct drm_plane *drm_plane_find(struct drm_device *dev,
		struct drm_file *file_priv,
		uint32_t id)
{
	struct drm_mode_object *mo;
	mo = drm_mode_object_find(dev, file_priv, id, DRM_MODE_OBJECT_PLANE);
	return mo ? obj_to_plane(mo) : NULL;
}


#define drm_for_each_plane_mask(plane, dev, plane_mask) \
	list_for_each_entry((plane), &(dev)->mode_config.plane_list, head) \
		for_each_if ((plane_mask) & (1 << drm_plane_index(plane)))


#define drm_for_each_legacy_plane(plane, dev) \
	list_for_each_entry(plane, &(dev)->mode_config.plane_list, head) \
		for_each_if (plane->type == DRM_PLANE_TYPE_OVERLAY)


#define drm_for_each_plane(plane, dev) \
	list_for_each_entry(plane, &(dev)->mode_config.plane_list, head)


#endif
