
#ifndef __RPMB_H__
#define __RPMB_H__

#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <uapi/linux/rpmb.h>
#include <linux/kref.h>


enum rpmb_type {
	RPMB_TYPE_ANY = 0,
	RPMB_TYPE_EMMC,
	RPMB_TYPE_UFS,
	RPMB_TYPE_SIM,
	RPMB_TYPE_MAX = RPMB_TYPE_SIM
};

extern struct class rpmb_class;


struct rpmb_cmd {
	u32 flags;
	u32 nframes;
	struct rpmb_frame *frames __aligned(8);
};


struct rpmb_data {
	u16 req_type;
	struct rpmb_cmd icmd;
	struct rpmb_cmd ocmd;
};


struct rpmb_ops {
	int (*cmd_seq)(struct device *dev, struct rpmb_cmd *cmds, u32 ncmds);
	enum rpmb_type type;
	const u8 *dev_id;
	size_t dev_id_len;
	u16 reliable_wr_cnt;
};


struct rpmb_dev {
	struct mutex lock; 
	struct device dev;
	int    id;
#ifdef CONFIG_RPMB_INTF_DEV
	struct cdev cdev;
	unsigned long status;
#endif 
	const struct rpmb_ops *ops;
};

#define to_rpmb_dev(x) container_of((x), struct rpmb_dev, dev)

#if IS_ENABLED(CONFIG_RPMB)
struct rpmb_dev *rpmb_dev_get(struct rpmb_dev *rdev);
void rpmb_dev_put(struct rpmb_dev *rdev);
struct rpmb_dev *rpmb_dev_find_by_device(struct device *parent);
struct rpmb_dev *rpmb_dev_get_by_type(enum rpmb_type type);
struct rpmb_dev *rpmb_dev_register(struct device *dev,
				   const struct rpmb_ops *ops);
struct rpmb_dev *rpmb_dev_find_device(void *data,
			int (*match)(struct device *dev, const void *data));
int rpmb_dev_unregister(struct device *dev);
int rpmb_cmd_seq(struct rpmb_dev *rdev, struct rpmb_cmd *cmds, u32 ncmds);
int rpmb_cmd_req(struct rpmb_dev *rdev, struct rpmb_data *data);
u16 rpmb_get_rw_size(struct rpmb_dev *rdev);


#else
static inline struct rpmb_dev *rpmb_dev_get(struct rpmb_dev *rdev)
{
	return NULL;
}

static inline void rpmb_dev_put(struct rpmb_dev *rdev) { }

static inline struct rpmb_dev *rpmb_dev_find_by_device(struct device *parent)
{
	return NULL;
}

static inline
struct rpmb_dev *rpmb_dev_get_by_type(enum rpmb_type type)
{
	return NULL;
}

static inline struct rpmb_dev *
rpmb_dev_register(struct device *dev, const struct rpmb_ops *ops)
{
	return NULL;
}

static inline int rpmb_dev_unregister(struct device *dev)
{
	return 0;
}

static inline int rpmb_cmd_seq(struct rpmb_dev *rdev,
			       struct rpmb_cmd *cmds, u32 ncmds)
{
	return 0;
}

static inline int rpmb_cmd_req(struct rpmb_dev *rdev, struct rpmb_data *data)
{
	return 0;
}

#endif 

#endif 
