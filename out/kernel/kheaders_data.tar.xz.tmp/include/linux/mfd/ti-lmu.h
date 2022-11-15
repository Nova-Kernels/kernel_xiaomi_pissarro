

#ifndef __MFD_TI_LMU_H__
#define __MFD_TI_LMU_H__

#include <linux/gpio.h>
#include <linux/notifier.h>
#include <linux/regmap.h>


#define LMU_EVENT_MONITOR_DONE		0x01

enum ti_lmu_id {
	LM3532,
	LM3631,
	LM3632,
	LM3633,
	LM3695,
	LM3697,
	LMU_MAX_ID,
};

enum ti_lmu_max_current {
	LMU_IMAX_5mA,
	LMU_IMAX_6mA,
	LMU_IMAX_7mA = 0x03,
	LMU_IMAX_8mA,
	LMU_IMAX_9mA,
	LMU_IMAX_10mA = 0x07,
	LMU_IMAX_11mA,
	LMU_IMAX_12mA,
	LMU_IMAX_13mA,
	LMU_IMAX_14mA,
	LMU_IMAX_15mA = 0x0D,
	LMU_IMAX_16mA,
	LMU_IMAX_17mA,
	LMU_IMAX_18mA,
	LMU_IMAX_19mA,
	LMU_IMAX_20mA = 0x13,
	LMU_IMAX_21mA,
	LMU_IMAX_22mA,
	LMU_IMAX_23mA = 0x17,
	LMU_IMAX_24mA,
	LMU_IMAX_25mA,
	LMU_IMAX_26mA,
	LMU_IMAX_27mA = 0x1C,
	LMU_IMAX_28mA,
	LMU_IMAX_29mA,
	LMU_IMAX_30mA,
};

enum lm363x_regulator_id {
	LM3631_BOOST,		
	LM3631_LDO_CONT,	
	LM3631_LDO_OREF,	
	LM3631_LDO_POS,		
	LM3631_LDO_NEG,		
	LM3632_BOOST,		
	LM3632_LDO_POS,		
	LM3632_LDO_NEG,		
};


struct ti_lmu {
	struct device *dev;
	struct regmap *regmap;
	int en_gpio;
	struct blocking_notifier_head notifier;
};
#endif
