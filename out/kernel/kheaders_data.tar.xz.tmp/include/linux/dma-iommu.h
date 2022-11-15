
#ifndef __DMA_IOMMU_H
#define __DMA_IOMMU_H

#ifdef __KERNEL__
#include <linux/types.h>
#include <asm/errno.h>

#ifdef CONFIG_IOMMU_DMA
#include <linux/dma-mapping.h>
#include <linux/iommu.h>
#include <linux/msi.h>

int iommu_dma_init(void);


int iommu_get_dma_cookie(struct iommu_domain *domain);
int iommu_get_msi_cookie(struct iommu_domain *domain, dma_addr_t base);
void iommu_put_dma_cookie(struct iommu_domain *domain);


int iommu_dma_init_domain(struct iommu_domain *domain, dma_addr_t base,
		u64 size, struct device *dev);


int dma_info_to_prot(enum dma_data_direction dir, bool coherent,
		     unsigned long attrs);


struct page **iommu_dma_alloc(struct device *dev, size_t size, gfp_t gfp,
		unsigned long attrs, int prot, dma_addr_t *handle,
		void (*flush_page)(struct device *, const void *, phys_addr_t));
void iommu_dma_free(struct device *dev, struct page **pages, size_t size,
		dma_addr_t *handle);
struct page **
iommu_dma_alloc_fix_iova(struct device *dev, size_t size, gfp_t gfp,
		unsigned long attrs, int prot, dma_addr_t handle,
		void (*flush_page)(struct device *, const void *, phys_addr_t));
void iommu_dma_free_from_reserved_range(struct device *dev,
		struct page **pages, size_t size, dma_addr_t *handle);

int iommu_dma_mmap(struct page **pages, size_t size, struct vm_area_struct *vma);

dma_addr_t iommu_dma_map_page(struct device *dev, struct page *page,
		unsigned long offset, size_t size, int prot);
int iommu_dma_map_sg(struct device *dev, struct scatterlist *sg,
		int nents, int prot);


void iommu_dma_unmap_page(struct device *dev, dma_addr_t handle, size_t size,
		enum dma_data_direction dir, unsigned long attrs);
void iommu_dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nents,
		enum dma_data_direction dir, unsigned long attrs);
dma_addr_t iommu_dma_map_resource(struct device *dev, phys_addr_t phys,
		size_t size, enum dma_data_direction dir, unsigned long attrs);
void iommu_dma_unmap_resource(struct device *dev, dma_addr_t handle,
		size_t size, enum dma_data_direction dir, unsigned long attrs);
int iommu_dma_mapping_error(struct device *dev, dma_addr_t dma_addr);


void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg);
void iommu_dma_get_resv_regions(struct device *dev, struct list_head *list);
#ifdef CONFIG_MTK_IOMMU_V2
void iommu_dma_dump_iovad(struct iommu_domain *domain,
		unsigned long target);
int iommu_dma_get_iovad_info(struct device *dev,
	unsigned long *base, unsigned long *max);
#endif
#else

struct iommu_domain;
struct msi_msg;
struct device;

static inline int iommu_dma_init(void)
{
	return 0;
}

static inline int iommu_get_dma_cookie(struct iommu_domain *domain)
{
	return -ENODEV;
}

static inline int iommu_get_msi_cookie(struct iommu_domain *domain, dma_addr_t base)
{
	return -ENODEV;
}

static inline void iommu_put_dma_cookie(struct iommu_domain *domain)
{
}

static inline void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg)
{
}

static inline void iommu_dma_get_resv_regions(struct device *dev, struct list_head *list)
{
}

#endif	
#endif	
#endif	
