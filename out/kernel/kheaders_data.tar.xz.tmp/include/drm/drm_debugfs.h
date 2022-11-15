

#ifndef _DRM_DEBUGFS_H_
#define _DRM_DEBUGFS_H_


struct drm_info_list {
	
	const char *name;
	
	int (*show)(struct seq_file*, void*);
	
	u32 driver_features;
	
	void *data;
};


struct drm_info_node {
	
	struct drm_minor *minor;
	
	const struct drm_info_list *info_ent;
	
	struct list_head list;
	struct dentry *dent;
};

#if defined(CONFIG_DEBUG_FS)
int drm_debugfs_create_files(const struct drm_info_list *files,
			     int count, struct dentry *root,
			     struct drm_minor *minor);
int drm_debugfs_remove_files(const struct drm_info_list *files,
			     int count, struct drm_minor *minor);
#else
static inline int drm_debugfs_create_files(const struct drm_info_list *files,
					   int count, struct dentry *root,
					   struct drm_minor *minor)
{
	return 0;
}

static inline int drm_debugfs_remove_files(const struct drm_info_list *files,
					   int count, struct drm_minor *minor)
{
	return 0;
}
#endif

#endif 
