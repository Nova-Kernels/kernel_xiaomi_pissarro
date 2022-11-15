

#ifndef __LINUX_W1_H
#define __LINUX_W1_H

#include <linux/device.h>


struct w1_reg_num {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u64	family:8,
		id:48,
		crc:8;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u64	crc:8,
		id:48,
		family:8;
#else
#error "Please fix <asm/byteorder.h>"
#endif
};

#ifdef __KERNEL__

#define W1_MAXNAMELEN		32

#define W1_SEARCH		0xF0
#define W1_ALARM_SEARCH		0xEC
#define W1_CONVERT_TEMP		0x44
#define W1_SKIP_ROM		0xCC
#define W1_COPY_SCRATCHPAD	0x48
#define W1_WRITE_SCRATCHPAD	0x4E
#define W1_READ_SCRATCHPAD	0xBE
#define W1_READ_ROM		0x33
#define W1_READ_PSUPPLY		0xB4
#define W1_MATCH_ROM		0x55
#define W1_RESUME_CMD		0xA5


struct w1_slave {
	struct module		*owner;
	unsigned char		name[W1_MAXNAMELEN];
	struct list_head	w1_slave_entry;
	struct w1_reg_num	reg_num;
	atomic_t		refcnt;
	int			ttl;
	unsigned long		flags;

	struct w1_master	*master;
	struct w1_family	*family;
	void			*family_data;
	struct device		dev;
	struct device		*hwmon;
};

typedef void (*w1_slave_found_callback)(struct w1_master *, u64);


struct w1_bus_master {
	void		*data;

	u8		(*read_bit)(void *);

	void		(*write_bit)(void *, u8);

	u8		(*touch_bit)(void *, u8);

	u8		(*read_byte)(void *);

	void		(*write_byte)(void *, u8);

	u8		(*read_block)(void *, u8 *, int);

	void		(*write_block)(void *, const u8 *, int);

	u8		(*triplet)(void *, u8);

	u8		(*reset_bus)(void *);

	u8		(*set_pullup)(void *, int);

	void		(*search)(void *, struct w1_master *,
		u8, w1_slave_found_callback);
};


enum w1_master_flags {
	W1_ABORT_SEARCH = 0,
	W1_WARN_MAX_COUNT = 1,
};


struct w1_master {
	struct list_head	w1_master_entry;
	struct module		*owner;
	unsigned char		name[W1_MAXNAMELEN];
	
	struct mutex		list_mutex;
	struct list_head	slist;
	struct list_head	async_list;
	int			max_slave_count, slave_count;
	unsigned long		attempts;
	int			slave_ttl;
	int			initialized;
	u32			id;
	int			search_count;
	
	u64			search_id;

	atomic_t		refcnt;

	void			*priv;

	
	int			enable_pullup;
	
	int			pullup_duration;

	long			flags;

	struct task_struct	*thread;
	struct mutex		mutex;
	struct mutex		bus_mutex;

	struct device_driver	*driver;
	struct device		dev;

	struct w1_bus_master	*bus_master;

	u32			seq;
};

int w1_add_master_device(struct w1_bus_master *master);
void w1_remove_master_device(struct w1_bus_master *master);


struct w1_family_ops {
	int  (*add_slave)(struct w1_slave *sl);
	void (*remove_slave)(struct w1_slave *sl);
	const struct attribute_group **groups;
	const struct hwmon_chip_info *chip_info;
};


struct w1_family {
	struct list_head	family_entry;
	u8			fid;

	struct w1_family_ops	*fops;

	atomic_t		refcnt;
};

int w1_register_family(struct w1_family *family);
void w1_unregister_family(struct w1_family *family);


#define module_w1_family(__w1_family) \
	module_driver(__w1_family, w1_register_family, \
			w1_unregister_family)

u8 w1_triplet(struct w1_master *dev, int bdir);
void w1_write_8(struct w1_master *, u8);
u8 w1_read_8(struct w1_master *);
int w1_reset_bus(struct w1_master *);
u8 w1_calc_crc8(u8 *, int);
void w1_write_block(struct w1_master *, const u8 *, int);
void w1_touch_block(struct w1_master *, u8 *, int);
u8 w1_read_block(struct w1_master *, u8 *, int);
int w1_reset_select_slave(struct w1_slave *sl);
int w1_reset_resume_command(struct w1_master *);
void w1_next_pullup(struct w1_master *, int);

static inline struct w1_slave* dev_to_w1_slave(struct device *dev)
{
	return container_of(dev, struct w1_slave, dev);
}

static inline struct w1_slave* kobj_to_w1_slave(struct kobject *kobj)
{
	return dev_to_w1_slave(container_of(kobj, struct device, kobj));
}

static inline struct w1_master* dev_to_w1_master(struct device *dev)
{
	return container_of(dev, struct w1_master, dev);
}

#endif 

#endif 
