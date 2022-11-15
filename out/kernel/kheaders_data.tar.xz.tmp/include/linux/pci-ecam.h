
#ifndef DRIVERS_PCI_ECAM_H
#define DRIVERS_PCI_ECAM_H

#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>


struct pci_config_window;
struct pci_ecam_ops {
	unsigned int			bus_shift;
	struct pci_ops			pci_ops;
	int				(*init)(struct pci_config_window *);
};


struct pci_config_window {
	struct resource			res;
	struct resource			busr;
	void				*priv;
	struct pci_ecam_ops		*ops;
	union {
		void __iomem		*win;	
		void __iomem		**winp; 
	};
	struct device			*parent;
};


struct pci_config_window *pci_ecam_create(struct device *dev,
		struct resource *cfgres, struct resource *busr,
		struct pci_ecam_ops *ops);
void pci_ecam_free(struct pci_config_window *cfg);


void __iomem *pci_ecam_map_bus(struct pci_bus *bus, unsigned int devfn,
			       int where);

extern struct pci_ecam_ops pci_generic_ecam_ops;

#if defined(CONFIG_ACPI) && defined(CONFIG_PCI_QUIRKS)
extern struct pci_ecam_ops pci_32b_ops;		
extern struct pci_ecam_ops hisi_pcie_ops;	
extern struct pci_ecam_ops thunder_pem_ecam_ops; 
extern struct pci_ecam_ops pci_thunder_ecam_ops; 
extern struct pci_ecam_ops xgene_v1_pcie_ecam_ops; 
extern struct pci_ecam_ops xgene_v2_pcie_ecam_ops; 
#endif

#ifdef CONFIG_PCI_HOST_COMMON

int pci_host_common_probe(struct platform_device *pdev,
			  struct pci_ecam_ops *ops);
#endif
#endif
