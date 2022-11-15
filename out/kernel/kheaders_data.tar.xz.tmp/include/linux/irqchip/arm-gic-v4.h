

#ifndef __LINUX_IRQCHIP_ARM_GIC_V4_H
#define __LINUX_IRQCHIP_ARM_GIC_V4_H

struct its_vpe;


struct its_vm {
	struct fwnode_handle	*fwnode;
	struct irq_domain	*domain;
	struct page		*vprop_page;
	struct its_vpe		**vpes;
	int			nr_vpes;
	irq_hw_number_t		db_lpi_base;
	unsigned long		*db_bitmap;
	int			nr_db_lpis;
};


struct its_vpe {
	struct page 		*vpt_page;
	struct its_vm		*its_vm;
	
	int			irq;
	irq_hw_number_t		vpe_db_lpi;
	
	int			vpe_proxy_event;
	
	u16			col_idx;
	
	u16			vpe_id;
	
	bool			idai;
	
	bool			pending_last;
};


struct its_vlpi_map {
	struct its_vm		*vm;
	struct its_vpe		*vpe;
	u32			vintid;
	bool			db_enabled;
};

enum its_vcpu_info_cmd_type {
	MAP_VLPI,
	GET_VLPI,
	PROP_UPDATE_VLPI,
	PROP_UPDATE_AND_INV_VLPI,
	SCHEDULE_VPE,
	DESCHEDULE_VPE,
	INVALL_VPE,
};

struct its_cmd_info {
	enum its_vcpu_info_cmd_type	cmd_type;
	union {
		struct its_vlpi_map	*map;
		u8			config;
	};
};

int its_alloc_vcpu_irqs(struct its_vm *vm);
void its_free_vcpu_irqs(struct its_vm *vm);
int its_schedule_vpe(struct its_vpe *vpe, bool on);
int its_invall_vpe(struct its_vpe *vpe);
int its_map_vlpi(int irq, struct its_vlpi_map *map);
int its_get_vlpi(int irq, struct its_vlpi_map *map);
int its_unmap_vlpi(int irq);
int its_prop_update_vlpi(int irq, u8 config, bool inv);

int its_init_v4(struct irq_domain *domain, const struct irq_domain_ops *ops);

#endif
