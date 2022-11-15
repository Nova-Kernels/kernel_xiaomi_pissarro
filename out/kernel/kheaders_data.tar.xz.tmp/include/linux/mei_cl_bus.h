/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MEI_CL_BUS_H
#define _LINUX_MEI_CL_BUS_H

#include <linux/device.h>
#include <linux/uuid.h>
#include <linux/mod_devicetable.h>

struct mei_cl_device;
struct mei_device;

typedef void (*mei_cldev_cb_t)(struct mei_cl_device *cldev);


struct mei_cl_device {
	struct list_head bus_list;
	struct mei_device *bus;
	struct device dev;

	struct mei_me_client *me_cl;
	struct mei_cl *cl;
	char name[MEI_CL_NAME_SIZE];

	struct work_struct rx_work;
	mei_cldev_cb_t rx_cb;
	struct work_struct notif_work;
	mei_cldev_cb_t notif_cb;

	unsigned int do_match:1;
	unsigned int is_added:1;

	void *priv_data;
};

struct mei_cl_driver {
	struct device_driver driver;
	const char *name;

	const struct mei_cl_device_id *id_table;

	int (*probe)(struct mei_cl_device *cldev,
		     const struct mei_cl_device_id *id);
	int (*remove)(struct mei_cl_device *cldev);
};

int __mei_cldev_driver_register(struct mei_cl_driver *cldrv,
				struct module *owner);
#define mei_cldev_driver_register(cldrv)             \
	__mei_cldev_driver_register(cldrv, THIS_MODULE)

void mei_cldev_driver_unregister(struct mei_cl_driver *cldrv);


#define module_mei_cl_driver(__mei_cldrv) \
	module_driver(__mei_cldrv, \
		      mei_cldev_driver_register,\
		      mei_cldev_driver_unregister)

ssize_t mei_cldev_send(struct mei_cl_device *cldev, u8 *buf, size_t length);
ssize_t mei_cldev_recv(struct mei_cl_device *cldev, u8 *buf, size_t length);
ssize_t mei_cldev_recv_nonblock(struct mei_cl_device *cldev, u8 *buf,
				size_t length);

int mei_cldev_register_rx_cb(struct mei_cl_device *cldev, mei_cldev_cb_t rx_cb);
int mei_cldev_register_notif_cb(struct mei_cl_device *cldev,
				mei_cldev_cb_t notif_cb);

const uuid_le *mei_cldev_uuid(const struct mei_cl_device *cldev);
u8 mei_cldev_ver(const struct mei_cl_device *cldev);

void *mei_cldev_get_drvdata(const struct mei_cl_device *cldev);
void mei_cldev_set_drvdata(struct mei_cl_device *cldev, void *data);

int mei_cldev_enable(struct mei_cl_device *cldev);
int mei_cldev_disable(struct mei_cl_device *cldev);
bool mei_cldev_enabled(struct mei_cl_device *cldev);

#endif 
