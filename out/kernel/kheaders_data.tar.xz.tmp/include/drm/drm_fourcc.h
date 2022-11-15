
#ifndef __DRM_FOURCC_H__
#define __DRM_FOURCC_H__

#include <linux/types.h>
#include <uapi/drm/drm_fourcc.h>


#ifdef __BIG_ENDIAN
# define DRM_FORMAT_HOST_XRGB1555     (DRM_FORMAT_XRGB1555         |	\
				       DRM_FORMAT_BIG_ENDIAN)
# define DRM_FORMAT_HOST_RGB565       (DRM_FORMAT_RGB565           |	\
				       DRM_FORMAT_BIG_ENDIAN)
# define DRM_FORMAT_HOST_XRGB8888     DRM_FORMAT_BGRX8888
# define DRM_FORMAT_HOST_ARGB8888     DRM_FORMAT_BGRA8888
#else
# define DRM_FORMAT_HOST_XRGB1555     DRM_FORMAT_XRGB1555
# define DRM_FORMAT_HOST_RGB565       DRM_FORMAT_RGB565
# define DRM_FORMAT_HOST_XRGB8888     DRM_FORMAT_XRGB8888
# define DRM_FORMAT_HOST_ARGB8888     DRM_FORMAT_ARGB8888
#endif

struct drm_device;
struct drm_mode_fb_cmd2;


struct drm_format_info {
	u32 format;
	u8 depth;
	u8 num_planes;
	u8 cpp[3];
	u8 hsub;
	u8 vsub;
};


struct drm_format_name_buf {
	char str[32];
};

const struct drm_format_info *__drm_format_info(u32 format);
const struct drm_format_info *drm_format_info(u32 format);
const struct drm_format_info *
drm_get_format_info(struct drm_device *dev,
		    const struct drm_mode_fb_cmd2 *mode_cmd);
uint32_t drm_mode_legacy_fb_format(uint32_t bpp, uint32_t depth);
int drm_format_num_planes(uint32_t format);
int drm_format_plane_cpp(uint32_t format, int plane);
int drm_format_horz_chroma_subsampling(uint32_t format);
int drm_format_vert_chroma_subsampling(uint32_t format);
int drm_format_plane_width(int width, uint32_t format, int plane);
int drm_format_plane_height(int height, uint32_t format, int plane);
const char *drm_get_format_name(uint32_t format, struct drm_format_name_buf *buf);

#endif 
