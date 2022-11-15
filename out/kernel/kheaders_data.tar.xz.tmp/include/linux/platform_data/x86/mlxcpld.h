

#ifndef _LINUX_I2C_MLXCPLD_H
#define _LINUX_I2C_MLXCPLD_H




struct mlxcpld_mux_plat_data {
	int *adap_ids;
	int num_adaps;
	int sel_reg_addr;
};

#endif 
