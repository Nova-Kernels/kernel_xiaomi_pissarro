

#ifndef __DT_BINDINGS_PINCTRL_BRCM_STINGRAY_H__
#define __DT_BINDINGS_PINCTRL_BRCM_STINGRAY_H__


#define MODE_NITRO				0
#define MODE_NAND				1
#define MODE_PNOR				2
#define MODE_GPIO				3


#define PAD_SLEW_RATE_ENA			(1 << 0)
#define PAD_SLEW_RATE_ENA_MASK			(1 << 0)

#define PAD_DRIVE_STRENGTH_2_MA			(0 << 1)
#define PAD_DRIVE_STRENGTH_4_MA			(1 << 1)
#define PAD_DRIVE_STRENGTH_6_MA			(2 << 1)
#define PAD_DRIVE_STRENGTH_8_MA			(3 << 1)
#define PAD_DRIVE_STRENGTH_10_MA		(4 << 1)
#define PAD_DRIVE_STRENGTH_12_MA		(5 << 1)
#define PAD_DRIVE_STRENGTH_14_MA		(6 << 1)
#define PAD_DRIVE_STRENGTH_16_MA		(7 << 1)
#define PAD_DRIVE_STRENGTH_MASK			(7 << 1)

#define PAD_PULL_UP_ENA				(1 << 4)
#define PAD_PULL_UP_ENA_MASK			(1 << 4)

#define PAD_PULL_DOWN_ENA			(1 << 5)
#define PAD_PULL_DOWN_ENA_MASK			(1 << 5)

#define PAD_INPUT_PATH_DIS			(1 << 6)
#define PAD_INPUT_PATH_DIS_MASK			(1 << 6)

#define PAD_HYSTERESIS_ENA			(1 << 7)
#define PAD_HYSTERESIS_ENA_MASK			(1 << 7)

#endif
