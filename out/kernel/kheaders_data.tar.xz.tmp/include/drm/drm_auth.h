

#ifndef _DRM_AUTH_H_
#define _DRM_AUTH_H_


struct drm_lock_data {
	struct drm_hw_lock *hw_lock;
	struct drm_file *file_priv;
	wait_queue_head_t lock_queue;
	unsigned long lock_time;
	spinlock_t spinlock;
	uint32_t kernel_waiters;
	uint32_t user_waiters;
	int idle_has_lock;
};


struct drm_master {
	struct kref refcount;
	struct drm_device *dev;
	
	char *unique;
	
	int unique_len;
	
	struct idr magic_map;
	struct drm_lock_data lock;
	void *driver_priv;
};

struct drm_master *drm_master_get(struct drm_master *master);
void drm_master_put(struct drm_master **master);
bool drm_is_current_master(struct drm_file *fpriv);

#endif
