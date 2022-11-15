

#ifndef _DRM_IOCTL_H_
#define _DRM_IOCTL_H_

#include <linux/types.h>
#include <linux/bitops.h>

#include <asm/ioctl.h>

struct drm_device;
struct drm_file;
struct file;


typedef int drm_ioctl_t(struct drm_device *dev, void *data,
			struct drm_file *file_priv);


typedef int drm_ioctl_compat_t(struct file *filp, unsigned int cmd,
			       unsigned long arg);

#define DRM_IOCTL_NR(n)                _IOC_NR(n)
#define DRM_MAJOR       226


enum drm_ioctl_flags {
	
	DRM_AUTH		= BIT(0),
	
	DRM_MASTER		= BIT(1),
	
	DRM_ROOT_ONLY		= BIT(2),
	
	DRM_CONTROL_ALLOW	= BIT(3),
	
	DRM_UNLOCKED		= BIT(4),
	
	DRM_RENDER_ALLOW	= BIT(5),
};


struct drm_ioctl_desc {
	unsigned int cmd;
	enum drm_ioctl_flags flags;
	drm_ioctl_t *func;
	const char *name;
};


#define DRM_IOCTL_DEF_DRV(ioctl, _func, _flags)				\
	[DRM_IOCTL_NR(DRM_IOCTL_##ioctl) - DRM_COMMAND_BASE] = {	\
		.cmd = DRM_IOCTL_##ioctl,				\
		.func = _func,						\
		.flags = _flags,					\
		.name = #ioctl						\
	}

int drm_ioctl_permit(u32 flags, struct drm_file *file_priv);
long drm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
long drm_ioctl_kernel(struct file *, drm_ioctl_t, void *, u32);
#ifdef CONFIG_COMPAT
long drm_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#else

#define drm_compat_ioctl NULL
#endif
bool drm_ioctl_flags(unsigned int nr, unsigned int *flags);

int drm_noop(struct drm_device *dev, void *data,
	     struct drm_file *file_priv);
int drm_invalid_op(struct drm_device *dev, void *data,
		   struct drm_file *file_priv);

#endif 
