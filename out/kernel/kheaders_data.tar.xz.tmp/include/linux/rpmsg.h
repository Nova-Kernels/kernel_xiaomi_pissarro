

#ifndef _LINUX_RPMSG_H
#define _LINUX_RPMSG_H

#include <linux/types.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mod_devicetable.h>
#include <linux/kref.h>
#include <linux/mutex.h>
#include <linux/poll.h>

#define RPMSG_ADDR_ANY		0xFFFFFFFF

struct rpmsg_device;
struct rpmsg_endpoint;
struct rpmsg_device_ops;
struct rpmsg_endpoint_ops;


struct rpmsg_channel_info {
	char name[RPMSG_NAME_SIZE];
	u32 src;
	u32 dst;
};


struct rpmsg_device {
	struct device dev;
	struct rpmsg_device_id id;
	char *driver_override;
	u32 src;
	u32 dst;
	struct rpmsg_endpoint *ept;
	bool announce;

	const struct rpmsg_device_ops *ops;
};

typedef int (*rpmsg_rx_cb_t)(struct rpmsg_device *, void *, int, void *, u32);


struct rpmsg_endpoint {
	struct rpmsg_device *rpdev;
	struct kref refcount;
	rpmsg_rx_cb_t cb;
	struct mutex cb_lock;
	u32 addr;
	void *priv;

	const struct rpmsg_endpoint_ops *ops;
};


struct rpmsg_driver {
	struct device_driver drv;
	const struct rpmsg_device_id *id_table;
	int (*probe)(struct rpmsg_device *dev);
	void (*remove)(struct rpmsg_device *dev);
	int (*callback)(struct rpmsg_device *, void *, int, void *, u32);
};

#if IS_ENABLED(CONFIG_RPMSG)

int register_rpmsg_device(struct rpmsg_device *dev);
void unregister_rpmsg_device(struct rpmsg_device *dev);
int __register_rpmsg_driver(struct rpmsg_driver *drv, struct module *owner);
void unregister_rpmsg_driver(struct rpmsg_driver *drv);
void rpmsg_destroy_ept(struct rpmsg_endpoint *);
struct rpmsg_endpoint *rpmsg_create_ept(struct rpmsg_device *,
					rpmsg_rx_cb_t cb, void *priv,
					struct rpmsg_channel_info chinfo);

int rpmsg_send(struct rpmsg_endpoint *ept, void *data, int len);
int rpmsg_sendto(struct rpmsg_endpoint *ept, void *data, int len, u32 dst);
int rpmsg_send_offchannel(struct rpmsg_endpoint *ept, u32 src, u32 dst,
			  void *data, int len);

int rpmsg_trysend(struct rpmsg_endpoint *ept, void *data, int len);
int rpmsg_trysendto(struct rpmsg_endpoint *ept, void *data, int len, u32 dst);
int rpmsg_trysend_offchannel(struct rpmsg_endpoint *ept, u32 src, u32 dst,
			     void *data, int len);

unsigned int rpmsg_poll(struct rpmsg_endpoint *ept, struct file *filp,
			poll_table *wait);

#else

static inline int register_rpmsg_device(struct rpmsg_device *dev)
{
	return -ENXIO;
}

static inline void unregister_rpmsg_device(struct rpmsg_device *dev)
{
	
	WARN_ON(1);
}

static inline int __register_rpmsg_driver(struct rpmsg_driver *drv,
					  struct module *owner)
{
	
	WARN_ON(1);

	return -ENXIO;
}

static inline void unregister_rpmsg_driver(struct rpmsg_driver *drv)
{
	
	WARN_ON(1);
}

static inline void rpmsg_destroy_ept(struct rpmsg_endpoint *ept)
{
	
	WARN_ON(1);
}

static inline struct rpmsg_endpoint *rpmsg_create_ept(struct rpmsg_device *rpdev,
						      rpmsg_rx_cb_t cb,
						      void *priv,
						      struct rpmsg_channel_info chinfo)
{
	
	WARN_ON(1);

	return ERR_PTR(-ENXIO);
}

static inline int rpmsg_send(struct rpmsg_endpoint *ept, void *data, int len)
{
	
	WARN_ON(1);

	return -ENXIO;
}

static inline int rpmsg_sendto(struct rpmsg_endpoint *ept, void *data, int len,
			       u32 dst)
{
	
	WARN_ON(1);

	return -ENXIO;

}

static inline int rpmsg_send_offchannel(struct rpmsg_endpoint *ept, u32 src,
					u32 dst, void *data, int len)
{
	
	WARN_ON(1);

	return -ENXIO;
}

static inline int rpmsg_trysend(struct rpmsg_endpoint *ept, void *data, int len)
{
	
	WARN_ON(1);

	return -ENXIO;
}

static inline int rpmsg_trysendto(struct rpmsg_endpoint *ept, void *data,
				  int len, u32 dst)
{
	
	WARN_ON(1);

	return -ENXIO;
}

static inline int rpmsg_trysend_offchannel(struct rpmsg_endpoint *ept, u32 src,
					   u32 dst, void *data, int len)
{
	
	WARN_ON(1);

	return -ENXIO;
}

static inline unsigned int rpmsg_poll(struct rpmsg_endpoint *ept,
				      struct file *filp, poll_table *wait)
{
	
	WARN_ON(1);

	return 0;
}

#endif 


#define register_rpmsg_driver(drv) \
	__register_rpmsg_driver(drv, THIS_MODULE)


#define module_rpmsg_driver(__rpmsg_driver) \
	module_driver(__rpmsg_driver, register_rpmsg_driver, \
			unregister_rpmsg_driver)

#endif 
