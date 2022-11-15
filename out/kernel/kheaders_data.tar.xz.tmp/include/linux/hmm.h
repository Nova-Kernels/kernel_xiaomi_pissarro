

#ifndef LINUX_HMM_H
#define LINUX_HMM_H

#include <linux/kconfig.h>

#if IS_ENABLED(CONFIG_HMM)

#include <linux/device.h>
#include <linux/migrate.h>
#include <linux/memremap.h>
#include <linux/completion.h>

struct hmm;


typedef unsigned long hmm_pfn_t;

#define HMM_PFN_VALID (1 << 0)
#define HMM_PFN_READ (1 << 1)
#define HMM_PFN_WRITE (1 << 2)
#define HMM_PFN_ERROR (1 << 3)
#define HMM_PFN_EMPTY (1 << 4)
#define HMM_PFN_SPECIAL (1 << 5)
#define HMM_PFN_DEVICE_UNADDRESSABLE (1 << 6)
#define HMM_PFN_SHIFT 7


static inline struct page *hmm_pfn_t_to_page(hmm_pfn_t pfn)
{
	if (!(pfn & HMM_PFN_VALID))
		return NULL;
	return pfn_to_page(pfn >> HMM_PFN_SHIFT);
}


static inline unsigned long hmm_pfn_t_to_pfn(hmm_pfn_t pfn)
{
	if (!(pfn & HMM_PFN_VALID))
		return -1UL;
	return (pfn >> HMM_PFN_SHIFT);
}


static inline hmm_pfn_t hmm_pfn_t_from_page(struct page *page)
{
	return (page_to_pfn(page) << HMM_PFN_SHIFT) | HMM_PFN_VALID;
}


static inline hmm_pfn_t hmm_pfn_t_from_pfn(unsigned long pfn)
{
	return (pfn << HMM_PFN_SHIFT) | HMM_PFN_VALID;
}


#if IS_ENABLED(CONFIG_HMM_MIRROR)


struct hmm_mirror;


enum hmm_update_type {
	HMM_UPDATE_INVALIDATE,
};


struct hmm_mirror_ops {
	
	void (*sync_cpu_device_pagetables)(struct hmm_mirror *mirror,
					   enum hmm_update_type update_type,
					   unsigned long start,
					   unsigned long end);
};


struct hmm_mirror {
	struct hmm			*hmm;
	const struct hmm_mirror_ops	*ops;
	struct list_head		list;
};

int hmm_mirror_register(struct hmm_mirror *mirror, struct mm_struct *mm);
void hmm_mirror_unregister(struct hmm_mirror *mirror);



struct hmm_range {
	struct list_head	list;
	unsigned long		start;
	unsigned long		end;
	hmm_pfn_t		*pfns;
	bool			valid;
};


int hmm_vma_get_pfns(struct vm_area_struct *vma,
		     struct hmm_range *range,
		     unsigned long start,
		     unsigned long end,
		     hmm_pfn_t *pfns);
bool hmm_vma_range_done(struct vm_area_struct *vma, struct hmm_range *range);



int hmm_vma_fault(struct vm_area_struct *vma,
		  struct hmm_range *range,
		  unsigned long start,
		  unsigned long end,
		  hmm_pfn_t *pfns,
		  bool write,
		  bool block);
#endif 


#if IS_ENABLED(CONFIG_DEVICE_PRIVATE) ||  IS_ENABLED(CONFIG_DEVICE_PUBLIC)
struct hmm_devmem;

struct page *hmm_vma_alloc_locked_page(struct vm_area_struct *vma,
				       unsigned long addr);


struct hmm_devmem_ops {
	
	void (*free)(struct hmm_devmem *devmem, struct page *page);
	
	int (*fault)(struct hmm_devmem *devmem,
		     struct vm_area_struct *vma,
		     unsigned long addr,
		     const struct page *page,
		     unsigned int flags,
		     pmd_t *pmdp);
};


struct hmm_devmem {
	struct completion		completion;
	unsigned long			pfn_first;
	unsigned long			pfn_last;
	struct resource			*resource;
	struct device			*device;
	struct dev_pagemap		pagemap;
	const struct hmm_devmem_ops	*ops;
	struct percpu_ref		ref;
};


struct hmm_devmem *hmm_devmem_add(const struct hmm_devmem_ops *ops,
				  struct device *device,
				  unsigned long size);
struct hmm_devmem *hmm_devmem_add_resource(const struct hmm_devmem_ops *ops,
					   struct device *device,
					   struct resource *res);


static inline void hmm_devmem_page_set_drvdata(struct page *page,
					       unsigned long data)
{
	unsigned long *drvdata = (unsigned long *)&page->pgmap;

	drvdata[1] = data;
}


static inline unsigned long hmm_devmem_page_get_drvdata(struct page *page)
{
	unsigned long *drvdata = (unsigned long *)&page->pgmap;

	return drvdata[1];
}



struct hmm_device {
	struct device		device;
	unsigned int		minor;
};


struct hmm_device *hmm_device_new(void *drvdata);
void hmm_device_put(struct hmm_device *hmm_device);
#endif 
#endif 


#if IS_ENABLED(CONFIG_HMM_MIRROR)
void hmm_mm_destroy(struct mm_struct *mm);

static inline void hmm_mm_init(struct mm_struct *mm)
{
	mm->hmm = NULL;
}
#else 
static inline void hmm_mm_destroy(struct mm_struct *mm) {}
static inline void hmm_mm_init(struct mm_struct *mm) {}
#endif 


#else 
static inline void hmm_mm_destroy(struct mm_struct *mm) {}
static inline void hmm_mm_init(struct mm_struct *mm) {}
#endif 
