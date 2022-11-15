/* SPDX-License-Identifier: GPL-2.0 */


struct ds1wm_driver_data {
	int active_high;
	int clock_rate;
	
	unsigned int reset_recover_delay;

	
	bool is_hw_big_endian;

	
	unsigned int bus_shift;
};
