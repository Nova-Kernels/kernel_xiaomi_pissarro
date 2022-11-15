

#ifndef __LINUX_I2C_HID_H
#define __LINUX_I2C_HID_H

#include <linux/types.h>

struct regulator;


struct i2c_hid_platform_data {
	u16 hid_descriptor_address;
	struct regulator *supply;
	int post_power_delay_ms;
};

#endif 
