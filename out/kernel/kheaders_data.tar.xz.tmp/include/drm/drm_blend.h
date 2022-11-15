

#ifndef __DRM_BLEND_H__
#define __DRM_BLEND_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <drm/drm_mode.h>

struct drm_device;
struct drm_atomic_state;
struct drm_plane;

static inline bool drm_rotation_90_or_270(unsigned int rotation)
{
	return rotation & (DRM_MODE_ROTATE_90 | DRM_MODE_ROTATE_270);
}

int drm_plane_create_rotation_property(struct drm_plane *plane,
				       unsigned int rotation,
				       unsigned int supported_rotations);
unsigned int drm_rotation_simplify(unsigned int rotation,
				   unsigned int supported_rotations);

int drm_plane_create_zpos_property(struct drm_plane *plane,
				   unsigned int zpos,
				   unsigned int min, unsigned int max);
int drm_plane_create_zpos_immutable_property(struct drm_plane *plane,
					     unsigned int zpos);
int drm_atomic_normalize_zpos(struct drm_device *dev,
			      struct drm_atomic_state *state);
#endif
