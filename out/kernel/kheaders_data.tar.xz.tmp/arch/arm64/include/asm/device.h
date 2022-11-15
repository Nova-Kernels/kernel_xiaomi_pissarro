
#ifndef __ASM_DEVICE_H
#define __ASM_DEVICE_H

struct dev_archdata {
#ifdef CONFIG_IOMMU_API
	void *iommu;			
#endif
#ifdef CONFIG_XEN
	const struct dma_map_ops *dev_dma_ops;
#endif
	bool dma_coherent;
};

struct pdev_archdata {
};

#endif
