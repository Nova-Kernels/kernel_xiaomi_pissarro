

#ifndef _RMI_H
#define _RMI_H
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/kfifo.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/types.h>

#define NAME_BUFFER_SIZE 256


struct rmi_2d_axis_alignment {
	bool swap_axes;
	bool flip_x;
	bool flip_y;
	u16 clip_x_low;
	u16 clip_y_low;
	u16 clip_x_high;
	u16 clip_y_high;
	u16 offset_x;
	u16 offset_y;
	u8 delta_x_threshold;
	u8 delta_y_threshold;
};


enum rmi_sensor_type {
	rmi_sensor_default = 0,
	rmi_sensor_touchscreen,
	rmi_sensor_touchpad
};

#define RMI_F11_DISABLE_ABS_REPORT      BIT(0)


struct rmi_2d_sensor_platform_data {
	struct rmi_2d_axis_alignment axis_align;
	enum rmi_sensor_type sensor_type;
	int x_mm;
	int y_mm;
	int disable_report_mask;
	u16 rezero_wait;
	bool topbuttonpad;
	bool kernel_tracking;
	int dmax;
	int dribble;
	int palm_detect;
};


struct rmi_f30_data {
	bool buttonpad;
	bool trackstick_buttons;
	bool disable;
};



enum rmi_reg_state {
	RMI_REG_STATE_DEFAULT = 0,
	RMI_REG_STATE_OFF = 1,
	RMI_REG_STATE_ON = 2
};


struct rmi_f01_power_management {
	enum rmi_reg_state nosleep;
	u8 wakeup_threshold;
	u8 doze_holdoff;
	u8 doze_interval;
};


struct rmi_device_platform_data_spi {
	u32 block_delay_us;
	u32 split_read_block_delay_us;
	u32 read_delay_us;
	u32 write_delay_us;
	u32 split_read_byte_delay_us;
	u32 pre_delay_us;
	u32 post_delay_us;
	u8 bits_per_word;
	u16 mode;

	void *cs_assert_data;
	int (*cs_assert)(const void *cs_assert_data, const bool assert);
};


struct rmi_device_platform_data {
	int reset_delay_ms;
	int irq;

	struct rmi_device_platform_data_spi spi_data;

	
	struct rmi_2d_sensor_platform_data sensor_pdata;
	struct rmi_f01_power_management power_management;
	struct rmi_f30_data f30_data;
};


struct rmi_function_descriptor {
	u16 query_base_addr;
	u16 command_base_addr;
	u16 control_base_addr;
	u16 data_base_addr;
	u8 interrupt_source_count;
	u8 function_number;
	u8 function_version;
};

struct rmi_device;


struct rmi_transport_dev {
	struct device *dev;
	struct rmi_device *rmi_dev;

	const char *proto_name;
	const struct rmi_transport_ops *ops;

	struct rmi_device_platform_data pdata;

	struct input_dev *input;
};


struct rmi_transport_ops {
	int (*write_block)(struct rmi_transport_dev *xport, u16 addr,
			   const void *buf, size_t len);
	int (*read_block)(struct rmi_transport_dev *xport, u16 addr,
			  void *buf, size_t len);
	int (*reset)(struct rmi_transport_dev *xport, u16 reset_addr);
};


struct rmi_driver {
	struct device_driver driver;

	int (*reset_handler)(struct rmi_device *rmi_dev);
	int (*clear_irq_bits)(struct rmi_device *rmi_dev, unsigned long *mask);
	int (*set_irq_bits)(struct rmi_device *rmi_dev, unsigned long *mask);
	int (*store_productid)(struct rmi_device *rmi_dev);
	int (*set_input_params)(struct rmi_device *rmi_dev,
			struct input_dev *input);
	void *data;
};


struct rmi_device {
	struct device dev;
	int number;

	struct rmi_driver *driver;
	struct rmi_transport_dev *xport;

};

struct rmi4_attn_data {
	unsigned long irq_status;
	size_t size;
	void *data;
};

struct rmi_driver_data {
	struct list_head function_list;

	struct rmi_device *rmi_dev;

	struct rmi_function *f01_container;
	struct rmi_function *f34_container;
	bool bootloader_mode;

	int num_of_irq_regs;
	int irq_count;
	void *irq_memory;
	unsigned long *irq_status;
	unsigned long *fn_irq_bits;
	unsigned long *current_irq_mask;
	unsigned long *new_irq_mask;
	struct mutex irq_mutex;
	struct input_dev *input;

	u8 pdt_props;

	u8 num_rx_electrodes;
	u8 num_tx_electrodes;

	bool enabled;
	struct mutex enabled_mutex;

	struct rmi4_attn_data attn_data;
	DECLARE_KFIFO(attn_fifo, struct rmi4_attn_data, 16);
};

int rmi_register_transport_device(struct rmi_transport_dev *xport);
void rmi_unregister_transport_device(struct rmi_transport_dev *xport);

void rmi_set_attn_data(struct rmi_device *rmi_dev, unsigned long irq_status,
		       void *data, size_t size);

int rmi_driver_suspend(struct rmi_device *rmi_dev, bool enable_wake);
int rmi_driver_resume(struct rmi_device *rmi_dev, bool clear_wake);
#endif
