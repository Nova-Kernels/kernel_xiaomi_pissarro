

#ifndef _MISC_CXLLIB_H
#define _MISC_CXLLIB_H

#include <linux/pci.h>
#include <asm/reg.h>




bool cxllib_slot_is_supported(struct pci_dev *dev, unsigned long flags);



#define CXL_XSL_CONFIG_VERSION1		1
struct cxllib_xsl_config {
	u32	version;     
	u32	log_bar_size;
	u64	bar_addr;    
	u64	dsnctl;      
	u64	dra;         
};

int cxllib_get_xsl_config(struct pci_dev *dev, struct cxllib_xsl_config *cfg);



enum cxllib_mode {
	CXL_MODE_CXL,
	CXL_MODE_PCI,
};

#define CXL_MODE_NO_DMA       0
#define CXL_MODE_DMA_TVT0     1
#define CXL_MODE_DMA_TVT1     2

int cxllib_switch_phb_mode(struct pci_dev *dev, enum cxllib_mode mode,
			unsigned long flags);



int cxllib_set_device_dma(struct pci_dev *dev, unsigned long flags);



struct cxllib_pe_attributes {
	u64 sr;
	u32 lpid;
	u32 tid;
	u32 pid;
};
#define CXL_TRANSLATED_MODE 0
#define CXL_REAL_MODE 1

int cxllib_get_PE_attributes(struct task_struct *task,
	     unsigned long translation_mode, struct cxllib_pe_attributes *attr);



int cxllib_handle_fault(struct mm_struct *mm, u64 addr, u64 size, u64 flags);


#endif 
