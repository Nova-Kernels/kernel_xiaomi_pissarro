

#ifndef __LINUX_PCI_EPF_H
#define __LINUX_PCI_EPF_H

#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/pci.h>

struct pci_epf;

enum pci_barno {
	BAR_0,
	BAR_1,
	BAR_2,
	BAR_3,
	BAR_4,
	BAR_5,
};


struct pci_epf_header {
	u16	vendorid;
	u16	deviceid;
	u8	revid;
	u8	progif_code;
	u8	subclass_code;
	u8	baseclass_code;
	u8	cache_line_size;
	u16	subsys_vendor_id;
	u16	subsys_id;
	enum pci_interrupt_pin interrupt_pin;
};


struct pci_epf_ops {
	int	(*bind)(struct pci_epf *epf);
	void	(*unbind)(struct pci_epf *epf);
	void	(*linkup)(struct pci_epf *epf);
};


struct pci_epf_driver {
	int	(*probe)(struct pci_epf *epf);
	int	(*remove)(struct pci_epf *epf);

	struct device_driver	driver;
	struct pci_epf_ops	*ops;
	struct module		*owner;
	struct config_group	*group;
	const struct pci_epf_device_id	*id_table;
};

#define to_pci_epf_driver(drv) (container_of((drv), struct pci_epf_driver, \
				driver))


struct pci_epf_bar {
	dma_addr_t	phys_addr;
	size_t		size;
};


struct pci_epf {
	struct device		dev;
	const char		*name;
	struct pci_epf_header	*header;
	struct pci_epf_bar	bar[6];
	u8			msi_interrupts;
	u8			func_no;

	struct pci_epc		*epc;
	struct pci_epf_driver	*driver;
	struct list_head	list;
};

#define to_pci_epf(epf_dev) container_of((epf_dev), struct pci_epf, dev)

#define pci_epf_register_driver(driver)    \
		__pci_epf_register_driver((driver), THIS_MODULE)

static inline void epf_set_drvdata(struct pci_epf *epf, void *data)
{
	dev_set_drvdata(&epf->dev, data);
}

static inline void *epf_get_drvdata(struct pci_epf *epf)
{
	return dev_get_drvdata(&epf->dev);
}

const struct pci_epf_device_id *
pci_epf_match_device(const struct pci_epf_device_id *id, struct pci_epf *epf);
struct pci_epf *pci_epf_create(const char *name);
void pci_epf_destroy(struct pci_epf *epf);
int __pci_epf_register_driver(struct pci_epf_driver *driver,
			      struct module *owner);
void pci_epf_unregister_driver(struct pci_epf_driver *driver);
void *pci_epf_alloc_space(struct pci_epf *epf, size_t size, enum pci_barno bar);
void pci_epf_free_space(struct pci_epf *epf, void *addr, enum pci_barno bar);
int pci_epf_bind(struct pci_epf *epf);
void pci_epf_unbind(struct pci_epf *epf);
void pci_epf_linkup(struct pci_epf *epf);
#endif 
