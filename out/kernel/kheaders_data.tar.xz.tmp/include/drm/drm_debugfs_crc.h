
#ifndef __DRM_DEBUGFS_CRC_H__
#define __DRM_DEBUGFS_CRC_H__

#define DRM_MAX_CRC_NR		10


struct drm_crtc_crc_entry {
	bool has_frame_counter;
	uint32_t frame;
	uint32_t crcs[DRM_MAX_CRC_NR];
};

#define DRM_CRC_ENTRIES_NR	128


struct drm_crtc_crc {
	spinlock_t lock;
	const char *source;
	bool opened, overflow;
	struct drm_crtc_crc_entry *entries;
	int head, tail;
	size_t values_cnt;
	wait_queue_head_t wq;
};

#if defined(CONFIG_DEBUG_FS)
int drm_crtc_add_crc_entry(struct drm_crtc *crtc, bool has_frame,
			   uint32_t frame, uint32_t *crcs);
#else
static inline int drm_crtc_add_crc_entry(struct drm_crtc *crtc, bool has_frame,
					 uint32_t frame, uint32_t *crcs)
{
	return -EINVAL;
}
#endif 

#endif 
