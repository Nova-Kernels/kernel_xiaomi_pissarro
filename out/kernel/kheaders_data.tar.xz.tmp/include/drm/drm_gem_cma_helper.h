/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __DRM_GEM_CMA_HELPER_H__
#define __DRM_GEM_CMA_HELPER_H__

#include <drm/drmP.h>
#include <drm/drm_gem.h>


struct drm_gem_cma_object {
	struct drm_gem_object base;
	dma_addr_t paddr;
	struct sg_table *sgt;

	
	void *vaddr;
};

static inline struct drm_gem_cma_object *
to_drm_gem_cma_obj(struct drm_gem_object *gem_obj)
{
	return container_of(gem_obj, struct drm_gem_cma_object, base);
}

#ifndef CONFIG_MMU
#define DRM_GEM_CMA_UNMAPPED_AREA_FOPS \
	.get_unmapped_area	= drm_gem_cma_get_unmapped_area,
#else
#define DRM_GEM_CMA_UNMAPPED_AREA_FOPS
#endif


#define DEFINE_DRM_GEM_CMA_FOPS(name) \
	static const struct file_operations name = {\
		.owner		= THIS_MODULE,\
		.open		= drm_open,\
		.release	= drm_release,\
		.unlocked_ioctl	= drm_ioctl,\
		.compat_ioctl	= drm_compat_ioctl,\
		.poll		= drm_poll,\
		.read		= drm_read,\
		.llseek		= noop_llseek,\
		.mmap		= drm_gem_cma_mmap,\
		DRM_GEM_CMA_UNMAPPED_AREA_FOPS \
	}


void drm_gem_cma_free_object(struct drm_gem_object *gem_obj);


int drm_gem_cma_dumb_create_internal(struct drm_file *file_priv,
				     struct drm_device *drm,
				     struct drm_mode_create_dumb *args);


int drm_gem_cma_dumb_create(struct drm_file *file_priv,
			    struct drm_device *drm,
			    struct drm_mode_create_dumb *args);


int drm_gem_cma_mmap(struct file *filp, struct vm_area_struct *vma);


struct drm_gem_cma_object *drm_gem_cma_create(struct drm_device *drm,
					      size_t size);

extern const struct vm_operations_struct drm_gem_cma_vm_ops;

#ifndef CONFIG_MMU
unsigned long drm_gem_cma_get_unmapped_area(struct file *filp,
					    unsigned long addr,
					    unsigned long len,
					    unsigned long pgoff,
					    unsigned long flags);
#endif

#ifdef CONFIG_DEBUG_FS
void drm_gem_cma_describe(struct drm_gem_cma_object *obj, struct seq_file *m);
#endif

struct sg_table *drm_gem_cma_prime_get_sg_table(struct drm_gem_object *obj);
struct drm_gem_object *
drm_gem_cma_prime_import_sg_table(struct drm_device *dev,
				  struct dma_buf_attachment *attach,
				  struct sg_table *sgt);
int drm_gem_cma_prime_mmap(struct drm_gem_object *obj,
			   struct vm_area_struct *vma);
void *drm_gem_cma_prime_vmap(struct drm_gem_object *obj);
void drm_gem_cma_prime_vunmap(struct drm_gem_object *obj, void *vaddr);

#endif 
