

#ifndef _DRM_DRV_H_
#define _DRM_DRV_H_

#include <linux/list.h>
#include <linux/irqreturn.h>

#include <drm/drm_device.h>

struct drm_file;
struct drm_gem_object;
struct drm_master;
struct drm_minor;
struct dma_buf_attachment;
struct drm_display_mode;
struct drm_mode_create_dumb;


#define DRIVER_USE_AGP			0x1
#define DRIVER_LEGACY			0x2
#define DRIVER_PCI_DMA			0x8
#define DRIVER_SG			0x10
#define DRIVER_HAVE_DMA			0x20
#define DRIVER_HAVE_IRQ			0x40
#define DRIVER_IRQ_SHARED		0x80
#define DRIVER_GEM			0x1000
#define DRIVER_MODESET			0x2000
#define DRIVER_PRIME			0x4000
#define DRIVER_RENDER			0x8000
#define DRIVER_ATOMIC			0x10000
#define DRIVER_KMS_LEGACY_CONTEXT	0x20000
#define DRIVER_SYNCOBJ                  0x40000
#define DRIVER_PREFER_XBGR_30BPP        0x80000


struct drm_driver {
	
	int (*load) (struct drm_device *, unsigned long flags);

	
	int (*open) (struct drm_device *, struct drm_file *);

	
	void (*postclose) (struct drm_device *, struct drm_file *);

	
	void (*lastclose) (struct drm_device *);

	
	void (*unload) (struct drm_device *);

	
	void (*release) (struct drm_device *);

	
	u32 (*get_vblank_counter) (struct drm_device *dev, unsigned int pipe);

	
	int (*enable_vblank) (struct drm_device *dev, unsigned int pipe);

	
	void (*disable_vblank) (struct drm_device *dev, unsigned int pipe);

	
	bool (*get_scanout_position) (struct drm_device *dev, unsigned int pipe,
				      bool in_vblank_irq, int *vpos, int *hpos,
				      ktime_t *stime, ktime_t *etime,
				      const struct drm_display_mode *mode);

	
	bool (*get_vblank_timestamp) (struct drm_device *dev, unsigned int pipe,
				     int *max_error,
				     struct timeval *vblank_time,
				     bool in_vblank_irq);

	
	irqreturn_t(*irq_handler) (int irq, void *arg);

	
	void (*irq_preinstall) (struct drm_device *dev);

	
	int (*irq_postinstall) (struct drm_device *dev);

	
	void (*irq_uninstall) (struct drm_device *dev);

	
	int (*master_create)(struct drm_device *dev, struct drm_master *master);

	
	void (*master_destroy)(struct drm_device *dev, struct drm_master *master);

	
	int (*master_set)(struct drm_device *dev, struct drm_file *file_priv,
			  bool from_open);
	
	void (*master_drop)(struct drm_device *dev, struct drm_file *file_priv);

	
	int (*debugfs_init)(struct drm_minor *minor);

	
	void (*gem_free_object) (struct drm_gem_object *obj);

	
	void (*gem_free_object_unlocked) (struct drm_gem_object *obj);

	
	int (*gem_open_object) (struct drm_gem_object *, struct drm_file *);

	
	void (*gem_close_object) (struct drm_gem_object *, struct drm_file *);

	
	struct drm_gem_object *(*gem_create_object)(struct drm_device *dev,
						    size_t size);

	
	
	int (*prime_handle_to_fd)(struct drm_device *dev, struct drm_file *file_priv,
				uint32_t handle, uint32_t flags, int *prime_fd);
	
	int (*prime_fd_to_handle)(struct drm_device *dev, struct drm_file *file_priv,
				int prime_fd, uint32_t *handle);
	
	struct dma_buf * (*gem_prime_export)(struct drm_device *dev,
				struct drm_gem_object *obj, int flags);
	
	struct drm_gem_object * (*gem_prime_import)(struct drm_device *dev,
				struct dma_buf *dma_buf);
	int (*gem_prime_pin)(struct drm_gem_object *obj);
	void (*gem_prime_unpin)(struct drm_gem_object *obj);
	struct reservation_object * (*gem_prime_res_obj)(
				struct drm_gem_object *obj);
	struct sg_table *(*gem_prime_get_sg_table)(struct drm_gem_object *obj);
	struct drm_gem_object *(*gem_prime_import_sg_table)(
				struct drm_device *dev,
				struct dma_buf_attachment *attach,
				struct sg_table *sgt);
	void *(*gem_prime_vmap)(struct drm_gem_object *obj);
	void (*gem_prime_vunmap)(struct drm_gem_object *obj, void *vaddr);
	int (*gem_prime_mmap)(struct drm_gem_object *obj,
				struct vm_area_struct *vma);

	
	int (*dumb_create)(struct drm_file *file_priv,
			   struct drm_device *dev,
			   struct drm_mode_create_dumb *args);
	
	int (*dumb_map_offset)(struct drm_file *file_priv,
			       struct drm_device *dev, uint32_t handle,
			       uint64_t *offset);
	
	int (*dumb_destroy)(struct drm_file *file_priv,
			    struct drm_device *dev,
			    uint32_t handle);

	
	const struct vm_operations_struct *gem_vm_ops;

	
	int major;
	
	int minor;
	
	int patchlevel;
	
	char *name;
	
	char *desc;
	
	char *date;

	
	u32 driver_features;

	

	const struct drm_ioctl_desc *ioctls;
	
	int num_ioctls;

	
	const struct file_operations *fops;

	
	

	
	struct list_head legacy_dev_list;
	int (*firstopen) (struct drm_device *);
	void (*preclose) (struct drm_device *, struct drm_file *file_priv);
	int (*dma_ioctl) (struct drm_device *dev, void *data, struct drm_file *file_priv);
	int (*dma_quiescent) (struct drm_device *);
	int (*context_dtor) (struct drm_device *dev, int context);
	int dev_priv_size;
};

__printf(6, 7)
void drm_dev_printk(const struct device *dev, const char *level,
		    unsigned int category, const char *function_name,
		    const char *prefix, const char *format, ...);
__printf(3, 4)
void drm_printk(const char *level, unsigned int category,
		const char *format, ...);
extern unsigned int drm_debug;

int drm_dev_init(struct drm_device *dev,
		 struct drm_driver *driver,
		 struct device *parent);
void drm_dev_fini(struct drm_device *dev);

struct drm_device *drm_dev_alloc(struct drm_driver *driver,
				 struct device *parent);
int drm_dev_register(struct drm_device *dev, unsigned long flags);
void drm_dev_unregister(struct drm_device *dev);

void drm_dev_ref(struct drm_device *dev);
void drm_dev_unref(struct drm_device *dev);
void drm_put_dev(struct drm_device *dev);
void drm_dev_unplug(struct drm_device *dev);


static inline int drm_dev_is_unplugged(struct drm_device *dev)
{
	int ret = atomic_read(&dev->unplugged);
	smp_rmb();
	return ret;
}


int drm_dev_set_unique(struct drm_device *dev, const char *name);


#endif
