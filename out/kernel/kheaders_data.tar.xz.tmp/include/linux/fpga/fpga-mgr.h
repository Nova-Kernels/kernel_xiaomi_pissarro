
#include <linux/mutex.h>
#include <linux/platform_device.h>

#ifndef _LINUX_FPGA_MGR_H
#define _LINUX_FPGA_MGR_H

struct fpga_manager;
struct sg_table;


enum fpga_mgr_states {
	
	FPGA_MGR_STATE_UNKNOWN,
	FPGA_MGR_STATE_POWER_OFF,
	FPGA_MGR_STATE_POWER_UP,
	FPGA_MGR_STATE_RESET,

	
	FPGA_MGR_STATE_FIRMWARE_REQ,
	FPGA_MGR_STATE_FIRMWARE_REQ_ERR,

	
	FPGA_MGR_STATE_WRITE_INIT,
	FPGA_MGR_STATE_WRITE_INIT_ERR,
	FPGA_MGR_STATE_WRITE,
	FPGA_MGR_STATE_WRITE_ERR,
	FPGA_MGR_STATE_WRITE_COMPLETE,
	FPGA_MGR_STATE_WRITE_COMPLETE_ERR,

	
	FPGA_MGR_STATE_OPERATING,
};


#define FPGA_MGR_PARTIAL_RECONFIG	BIT(0)
#define FPGA_MGR_EXTERNAL_CONFIG	BIT(1)
#define FPGA_MGR_ENCRYPTED_BITSTREAM	BIT(2)
#define FPGA_MGR_BITSTREAM_LSB_FIRST	BIT(3)
#define FPGA_MGR_COMPRESSED_BITSTREAM	BIT(4)


struct fpga_image_info {
	u32 flags;
	u32 enable_timeout_us;
	u32 disable_timeout_us;
	u32 config_complete_timeout_us;
};


struct fpga_manager_ops {
	size_t initial_header_size;
	enum fpga_mgr_states (*state)(struct fpga_manager *mgr);
	int (*write_init)(struct fpga_manager *mgr,
			  struct fpga_image_info *info,
			  const char *buf, size_t count);
	int (*write)(struct fpga_manager *mgr, const char *buf, size_t count);
	int (*write_sg)(struct fpga_manager *mgr, struct sg_table *sgt);
	int (*write_complete)(struct fpga_manager *mgr,
			      struct fpga_image_info *info);
	void (*fpga_remove)(struct fpga_manager *mgr);
};


struct fpga_manager {
	const char *name;
	struct device dev;
	struct mutex ref_mutex;
	enum fpga_mgr_states state;
	const struct fpga_manager_ops *mops;
	void *priv;
};

#define to_fpga_manager(d) container_of(d, struct fpga_manager, dev)

int fpga_mgr_buf_load(struct fpga_manager *mgr, struct fpga_image_info *info,
		      const char *buf, size_t count);
int fpga_mgr_buf_load_sg(struct fpga_manager *mgr, struct fpga_image_info *info,
			 struct sg_table *sgt);

int fpga_mgr_firmware_load(struct fpga_manager *mgr,
			   struct fpga_image_info *info,
			   const char *image_name);

struct fpga_manager *of_fpga_mgr_get(struct device_node *node);

struct fpga_manager *fpga_mgr_get(struct device *dev);

void fpga_mgr_put(struct fpga_manager *mgr);

int fpga_mgr_register(struct device *dev, const char *name,
		      const struct fpga_manager_ops *mops, void *priv);

void fpga_mgr_unregister(struct device *dev);

#endif 
