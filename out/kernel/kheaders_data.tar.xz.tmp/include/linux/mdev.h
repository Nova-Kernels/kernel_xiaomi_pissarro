

#ifndef MDEV_H
#define MDEV_H

struct mdev_device;


struct mdev_parent_ops {
	struct module   *owner;
	const struct attribute_group **dev_attr_groups;
	const struct attribute_group **mdev_attr_groups;
	struct attribute_group **supported_type_groups;

	int     (*create)(struct kobject *kobj, struct mdev_device *mdev);
	int     (*remove)(struct mdev_device *mdev);
	int     (*open)(struct mdev_device *mdev);
	void    (*release)(struct mdev_device *mdev);
	ssize_t (*read)(struct mdev_device *mdev, char __user *buf,
			size_t count, loff_t *ppos);
	ssize_t (*write)(struct mdev_device *mdev, const char __user *buf,
			 size_t count, loff_t *ppos);
	long	(*ioctl)(struct mdev_device *mdev, unsigned int cmd,
			 unsigned long arg);
	int	(*mmap)(struct mdev_device *mdev, struct vm_area_struct *vma);
};


struct mdev_type_attribute {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, struct device *dev, char *buf);
	ssize_t (*store)(struct kobject *kobj, struct device *dev,
			 const char *buf, size_t count);
};

#define MDEV_TYPE_ATTR(_name, _mode, _show, _store)		\
struct mdev_type_attribute mdev_type_attr_##_name =		\
	__ATTR(_name, _mode, _show, _store)
#define MDEV_TYPE_ATTR_RW(_name) \
	struct mdev_type_attribute mdev_type_attr_##_name = __ATTR_RW(_name)
#define MDEV_TYPE_ATTR_RO(_name) \
	struct mdev_type_attribute mdev_type_attr_##_name = __ATTR_RO(_name)
#define MDEV_TYPE_ATTR_WO(_name) \
	struct mdev_type_attribute mdev_type_attr_##_name = __ATTR_WO(_name)


struct mdev_driver {
	const char *name;
	int  (*probe)(struct device *dev);
	void (*remove)(struct device *dev);
	struct device_driver driver;
};

#define to_mdev_driver(drv)	container_of(drv, struct mdev_driver, driver)

extern void *mdev_get_drvdata(struct mdev_device *mdev);
extern void mdev_set_drvdata(struct mdev_device *mdev, void *data);
extern uuid_le mdev_uuid(struct mdev_device *mdev);

extern struct bus_type mdev_bus_type;

extern int  mdev_register_device(struct device *dev,
				 const struct mdev_parent_ops *ops);
extern void mdev_unregister_device(struct device *dev);

extern int  mdev_register_driver(struct mdev_driver *drv, struct module *owner);
extern void mdev_unregister_driver(struct mdev_driver *drv);

extern struct device *mdev_parent_dev(struct mdev_device *mdev);
extern struct device *mdev_dev(struct mdev_device *mdev);
extern struct mdev_device *mdev_from_dev(struct device *dev);

#endif 
