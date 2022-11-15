

#ifndef _DRM_PCI_H_
#define _DRM_PCI_H_

#include <linux/pci.h>

struct drm_dma_handle;
struct drm_device;
struct drm_driver;
struct drm_master;

struct drm_dma_handle *drm_pci_alloc(struct drm_device *dev, size_t size,
				     size_t align);
void drm_pci_free(struct drm_device *dev, struct drm_dma_handle * dmah);

int drm_legacy_pci_init(struct drm_driver *driver, struct pci_driver *pdriver);
void drm_legacy_pci_exit(struct drm_driver *driver, struct pci_driver *pdriver);
#ifdef CONFIG_PCI
int drm_get_pci_dev(struct pci_dev *pdev,
		    const struct pci_device_id *ent,
		    struct drm_driver *driver);
#else
static inline int drm_get_pci_dev(struct pci_dev *pdev,
				  const struct pci_device_id *ent,
				  struct drm_driver *driver)
{
	return -ENOSYS;
}
#endif

#define DRM_PCIE_SPEED_25 1
#define DRM_PCIE_SPEED_50 2
#define DRM_PCIE_SPEED_80 4

int drm_pcie_get_speed_cap_mask(struct drm_device *dev, u32 *speed_mask);
int drm_pcie_get_max_link_width(struct drm_device *dev, u32 *mlw);

#endif 
