

#ifndef _LINUX_STM32_LPTIMER_H_
#define _LINUX_STM32_LPTIMER_H_

#include <linux/clk.h>
#include <linux/regmap.h>

#define STM32_LPTIM_ISR		0x00	
#define STM32_LPTIM_ICR		0x04	
#define STM32_LPTIM_IER		0x08	
#define STM32_LPTIM_CFGR	0x0C	
#define STM32_LPTIM_CR		0x10	
#define STM32_LPTIM_CMP		0x14	
#define STM32_LPTIM_ARR		0x18	
#define STM32_LPTIM_CNT		0x1C	


#define STM32_LPTIM_CMPOK_ARROK		GENMASK(4, 3)
#define STM32_LPTIM_ARROK		BIT(4)
#define STM32_LPTIM_CMPOK		BIT(3)


#define STM32_LPTIM_CMPOKCF_ARROKCF	GENMASK(4, 3)


#define STM32_LPTIM_CNTSTRT	BIT(2)
#define STM32_LPTIM_ENABLE	BIT(0)


#define STM32_LPTIM_ENC		BIT(24)
#define STM32_LPTIM_COUNTMODE	BIT(23)
#define STM32_LPTIM_WAVPOL	BIT(21)
#define STM32_LPTIM_PRESC	GENMASK(11, 9)
#define STM32_LPTIM_CKPOL	GENMASK(2, 1)


#define STM32_LPTIM_MAX_ARR	0xFFFF


struct stm32_lptimer {
	struct clk *clk;
	struct regmap *regmap;
	bool has_encoder;
};

#endif
