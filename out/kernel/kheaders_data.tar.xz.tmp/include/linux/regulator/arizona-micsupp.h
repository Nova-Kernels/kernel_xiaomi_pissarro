

#ifndef ARIZONA_MICSUPP_H
#define ARIZONA_MICSUPP_H

struct regulator_init_data;

struct arizona_micsupp_pdata {
	
	const struct regulator_init_data *init_data;
};

#endif
