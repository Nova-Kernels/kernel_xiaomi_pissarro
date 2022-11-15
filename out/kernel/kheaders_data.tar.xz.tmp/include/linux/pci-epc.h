

#ifndef __LINUX_PCI_EPC_H
#define __LINUX_PCI_EPC_H

#include <linux/pci-epf.h>

struct pci_epc;

enum pci_epc_irq_type {
	PCI_EPC_IRQ_UNKNOWN,
	PCI_EPC_IRQ_LEGACY,
	PCI_EPC_IRQ_MSI,
};


struct pci_epc_ops {
	int	(*write_header)(struct pci_epc *pci_epc,
				struct pci_epf_header *hdr);
	int	(*set_bar)(struct pci_epc *epc, enum pci_barno bar,
			   dma_addr_t bar_phys, size_t size, int flags);
	void	(*clear_bar)(struct pci_epc *epc, enum pci_barno bar);
	int	(*map_addr)(struct pci_epc *epc, phys_addr_t addr,
			    u64 pci_addr, size_t size);
	void	(*unmap_addr)(struct pci_epc *epc, phys_addr_t addr);
	int	(*set_msi)(struct pci_epc *epc, u8 interrupts);
	int	(*get_msi)(struct pci_epc *epc);
	int	(*raise_irq)(struct pci_epc *pci_epc,
			     enum pci_epc_irq_type type, u8 interrupt_num);
	int	(*start)(struct pci_epc *epc);
	void	(*stop)(struct pci_epc *epc);
	struct module *owner;
};


struct pci_epc_mem {
	phys_addr_t	phys_base;
	size_t		size;
	unsigned long	*bitmap;
	size_t		page_size;
	int		pages;
	
	struct mutex	lock;
};


struct pci_epc {
	struct device			dev;
	struct list_head		pci_epf;
	const struct pci_epc_ops	*ops;
	struct pci_epc_mem		*mem;
	u8				max_functions;
	struct config_group		*group;
	
	spinlock_t			lock;
};

#define to_pci_epc(device) container_of((device), struct pci_epc, dev)

#define pci_epc_create(dev, ops)    \
		__pci_epc_create((dev), (ops), THIS_MODULE)
#define devm_pci_epc_create(dev, ops)    \
		__devm_pci_epc_create((dev), (ops), THIS_MODULE)

#define pci_epc_mem_init(epc, phys_addr, size)	\
		__pci_epc_mem_init((epc), (phys_addr), (size), PAGE_SIZE)

static inline void epc_set_drvdata(struct pci_epc *epc, void *data)
{
	dev_set_drvdata(&epc->dev, data);
}

static inline void *epc_get_drvdata(struct pci_epc *epc)
{
	return dev_get_drvdata(&epc->dev);
}

struct pci_epc *
__devm_pci_epc_create(struct device *dev, const struct pci_epc_ops *ops,
		      struct module *owner);
struct pci_epc *
__pci_epc_create(struct device *dev, const struct pci_epc_ops *ops,
		 struct module *owner);
void devm_pci_epc_destroy(struct device *dev, struct pci_epc *epc);
void pci_epc_destroy(struct pci_epc *epc);
int pci_epc_add_epf(struct pci_epc *epc, struct pci_epf *epf);
void pci_epc_linkup(struct pci_epc *epc);
void pci_epc_remove_epf(struct pci_epc *epc, struct pci_epf *epf);
int pci_epc_write_header(struct pci_epc *epc, struct pci_epf_header *hdr);
int pci_epc_set_bar(struct pci_epc *epc, enum pci_barno bar,
		    dma_addr_t bar_phys, size_t size, int flags);
void pci_epc_clear_bar(struct pci_epc *epc, int bar);
int pci_epc_map_addr(struct pci_epc *epc, phys_addr_t phys_addr,
		     u64 pci_addr, size_t size);
void pci_epc_unmap_addr(struct pci_epc *epc, phys_addr_t phys_addr);
int pci_epc_set_msi(struct pci_epc *epc, u8 interrupts);
int pci_epc_get_msi(struct pci_epc *epc);
int pci_epc_raise_irq(struct pci_epc *epc, enum pci_epc_irq_type type,
		      u8 interrupt_num);
int pci_epc_start(struct pci_epc *epc);
void pci_epc_stop(struct pci_epc *epc);
struct pci_epc *pci_epc_get(const char *epc_name);
void pci_epc_put(struct pci_epc *epc);

int __pci_epc_mem_init(struct pci_epc *epc, phys_addr_t phys_addr, size_t size,
		       size_t page_size);
void pci_epc_mem_exit(struct pci_epc *epc);
void __iomem *pci_epc_mem_alloc_addr(struct pci_epc *epc,
				     phys_addr_t *phys_addr, size_t size);
void pci_epc_mem_free_addr(struct pci_epc *epc, phys_addr_t phys_addr,
			   void __iomem *virt_addr, size_t size);
#endif 
