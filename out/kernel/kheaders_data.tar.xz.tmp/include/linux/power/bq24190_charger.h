

#ifndef _BQ24190_CHARGER_H_
#define _BQ24190_CHARGER_H_

#include <linux/regulator/machine.h>

struct bq24190_platform_data {
	const struct regulator_init_data *regulator_init_data;
};

#endif
