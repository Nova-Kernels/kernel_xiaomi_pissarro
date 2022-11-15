#ifndef __DRM_GEM_H__
#define __DRM_GEM_H__



#include <linux/kref.h>

#include <drm/drm_vma_manager.h>


struct drm_gem_object {
	
	struct kref refcount;

	
	unsigned handle_count;

	
	struct drm_device *dev;

	
	struct file *filp;

	
	struct drm_vma_offset_node vma_node;

	
	size_t size;

	
	int name;

	
	uint32_t read_domains;

	
	uint32_t write_domain;

	
	struct dma_buf *dma_buf;

	
	struct dma_buf_attachment *import_attach;
};


#define DEFINE_DRM_GEM_FOPS(name) \
	static const struct file_operations name = {\
		.owner		= THIS_MODULE,\
		.open		= drm_open,\
		.release	= drm_release,\
		.unlocked_ioctl	= drm_ioctl,\
		.compat_ioctl	= drm_compat_ioctl,\
		.poll		= drm_poll,\
		.read		= drm_read,\
		.llseek		= noop_llseek,\
		.mmap		= drm_gem_mmap,\
	}

void drm_gem_object_release(struct drm_gem_object *obj);
void drm_gem_object_free(struct kref *kref);
int drm_gem_object_init(struct drm_device *dev,
			struct drm_gem_object *obj, size_t size);
void drm_gem_private_object_init(struct drm_device *dev,
				 struct drm_gem_object *obj, size_t size);
void drm_gem_vm_open(struct vm_area_struct *vma);
void drm_gem_vm_close(struct vm_area_struct *vma);
int drm_gem_mmap_obj(struct drm_gem_object *obj, unsigned long obj_size,
		     struct vm_area_struct *vma);
int drm_gem_mmap(struct file *filp, struct vm_area_struct *vma);


static inline void drm_gem_object_get(struct drm_gem_object *obj)
{
	kref_get(&obj->refcount);
}


static inline void
__drm_gem_object_put(struct drm_gem_object *obj)
{
	kref_put(&obj->refcount, drm_gem_object_free);
}

void drm_gem_object_put_unlocked(struct drm_gem_object *obj);
void drm_gem_object_put(struct drm_gem_object *obj);


static inline void drm_gem_object_reference(struct drm_gem_object *obj)
{
	drm_gem_object_get(obj);
}


static inline void __drm_gem_object_unreference(struct drm_gem_object *obj)
{
	__drm_gem_object_put(obj);
}


static inline void
drm_gem_object_unreference_unlocked(struct drm_gem_object *obj)
{
	drm_gem_object_put_unlocked(obj);
}


static inline void drm_gem_object_unreference(struct drm_gem_object *obj)
{
	drm_gem_object_put(obj);
}

int drm_gem_handle_create(struct drm_file *file_priv,
			  struct drm_gem_object *obj,
			  u32 *handlep);
int drm_gem_handle_delete(struct drm_file *filp, u32 handle);


void drm_gem_free_mmap_offset(struct drm_gem_object *obj);
int drm_gem_create_mmap_offset(struct drm_gem_object *obj);
int drm_gem_create_mmap_offset_size(struct drm_gem_object *obj, size_t size);

struct page **drm_gem_get_pages(struct drm_gem_object *obj);
void drm_gem_put_pages(struct drm_gem_object *obj, struct page **pages,
		bool dirty, bool accessed);

struct drm_gem_object *drm_gem_object_lookup(struct drm_file *filp, u32 handle);
int drm_gem_dumb_map_offset(struct drm_file *file, struct drm_device *dev,
			    u32 handle, u64 *offset);
int drm_gem_dumb_destroy(struct drm_file *file,
			 struct drm_device *dev,
			 uint32_t handle);

#endif 
