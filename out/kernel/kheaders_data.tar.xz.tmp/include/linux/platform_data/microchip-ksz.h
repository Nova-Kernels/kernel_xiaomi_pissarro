

#ifndef __MICROCHIP_KSZ_H
#define __MICROCHIP_KSZ_H

#include <linux/kernel.h>

struct ksz_platform_data {
	u32 chip_id;
	u16 enabled_ports;
};

#endif
