

#ifndef _DRM_IRQ_H_
#define _DRM_IRQ_H_

struct drm_device;

int drm_irq_install(struct drm_device *dev, int irq);
int drm_irq_uninstall(struct drm_device *dev);

#endif
