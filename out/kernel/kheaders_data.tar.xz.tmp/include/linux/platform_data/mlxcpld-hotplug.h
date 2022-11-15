

#ifndef __LINUX_PLATFORM_DATA_MLXCPLD_HOTPLUG_H
#define __LINUX_PLATFORM_DATA_MLXCPLD_HOTPLUG_H


struct mlxcpld_hotplug_device {
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	struct i2c_board_info brdinfo;
	u16 bus;
};


struct mlxcpld_hotplug_platform_data {
	u16 top_aggr_offset;
	u8 top_aggr_mask;
	u8 top_aggr_psu_mask;
	u16 psu_reg_offset;
	u8 psu_mask;
	u8 psu_count;
	struct mlxcpld_hotplug_device *psu;
	u8 top_aggr_pwr_mask;
	u16 pwr_reg_offset;
	u8 pwr_mask;
	u8 pwr_count;
	struct mlxcpld_hotplug_device *pwr;
	u8 top_aggr_fan_mask;
	u16 fan_reg_offset;
	u8 fan_mask;
	u8 fan_count;
	struct mlxcpld_hotplug_device *fan;
};

#endif 
