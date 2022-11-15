

#ifndef __DRM_PRIME_H__
#define __DRM_PRIME_H__

#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/scatterlist.h>



struct drm_prime_file_private {

	struct mutex lock;
	struct rb_root dmabufs;
	struct rb_root handles;
};

struct device;

struct dma_buf_export_info;
struct dma_buf;

struct drm_device;
struct drm_gem_object;
struct drm_file;

struct device;

struct dma_buf *drm_gem_prime_export(struct drm_device *dev,
				     struct drm_gem_object *obj,
				     int flags);
int drm_gem_prime_handle_to_fd(struct drm_device *dev,
			       struct drm_file *file_priv, uint32_t handle, uint32_t flags,
			       int *prime_fd);
int drm_gem_prime_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma);
struct drm_gem_object *drm_gem_prime_import(struct drm_device *dev,
					    struct dma_buf *dma_buf);

struct drm_gem_object *drm_gem_prime_import_dev(struct drm_device *dev,
						struct dma_buf *dma_buf,
						struct device *attach_dev);

int drm_gem_prime_fd_to_handle(struct drm_device *dev,
			       struct drm_file *file_priv, int prime_fd, uint32_t *handle);
struct dma_buf *drm_gem_dmabuf_export(struct drm_device *dev,
				      struct dma_buf_export_info *exp_info);
void drm_gem_dmabuf_release(struct dma_buf *dma_buf);

int drm_prime_sg_to_page_addr_arrays(struct sg_table *sgt, struct page **pages,
				     dma_addr_t *addrs, int max_pages);
struct sg_table *drm_prime_pages_to_sg(struct page **pages, unsigned int nr_pages);
void drm_prime_gem_destroy(struct drm_gem_object *obj, struct sg_table *sg);


#endif 
