

#ifndef _LINUX_STM32_GPTIMER_H_
#define _LINUX_STM32_GPTIMER_H_

#include <linux/clk.h>
#include <linux/regmap.h>

#define TIM_CR1		0x00	
#define TIM_CR2		0x04	
#define TIM_SMCR	0x08	
#define TIM_DIER	0x0C	
#define TIM_SR		0x10	
#define TIM_EGR		0x14	
#define TIM_CCMR1	0x18	
#define TIM_CCMR2	0x1C	
#define TIM_CCER	0x20	
#define TIM_CNT		0x24	
#define TIM_PSC		0x28	
#define TIM_ARR		0x2c	
#define TIM_CCR1	0x34	
#define TIM_CCR2	0x38	
#define TIM_CCR3	0x3C	
#define TIM_CCR4	0x40	
#define TIM_BDTR	0x44	

#define TIM_CR1_CEN	BIT(0)	
#define TIM_CR1_DIR	BIT(4)  
#define TIM_CR1_ARPE	BIT(7)	
#define TIM_CR2_MMS	(BIT(4) | BIT(5) | BIT(6)) 
#define TIM_CR2_MMS2	GENMASK(23, 20) 
#define TIM_SMCR_SMS	(BIT(0) | BIT(1) | BIT(2)) 
#define TIM_SMCR_TS	(BIT(4) | BIT(5) | BIT(6)) 
#define TIM_DIER_UIE	BIT(0)	
#define TIM_SR_UIF	BIT(0)	
#define TIM_EGR_UG	BIT(0)	
#define TIM_CCMR_PE	BIT(3)	
#define TIM_CCMR_M1	(BIT(6) | BIT(5))  
#define TIM_CCER_CC1E	BIT(0)	
#define TIM_CCER_CC1P	BIT(1)	
#define TIM_CCER_CC1NE	BIT(2)	
#define TIM_CCER_CC1NP	BIT(3)	
#define TIM_CCER_CC2E	BIT(4)	
#define TIM_CCER_CC3E	BIT(8)	
#define TIM_CCER_CC4E	BIT(12)	
#define TIM_CCER_CCXE	(BIT(0) | BIT(4) | BIT(8) | BIT(12))
#define TIM_BDTR_BKE	BIT(12) 
#define TIM_BDTR_BKP	BIT(13) 
#define TIM_BDTR_AOE	BIT(14)	
#define TIM_BDTR_MOE	BIT(15)	
#define TIM_BDTR_BKF	(BIT(16) | BIT(17) | BIT(18) | BIT(19))
#define TIM_BDTR_BK2F	(BIT(20) | BIT(21) | BIT(22) | BIT(23))
#define TIM_BDTR_BK2E	BIT(24) 
#define TIM_BDTR_BK2P	BIT(25) 

#define MAX_TIM_PSC		0xFFFF
#define TIM_CR2_MMS_SHIFT	4
#define TIM_CR2_MMS2_SHIFT	20
#define TIM_SMCR_TS_SHIFT	4
#define TIM_BDTR_BKF_MASK	0xF
#define TIM_BDTR_BKF_SHIFT	16
#define TIM_BDTR_BK2F_SHIFT	20

struct stm32_timers {
	struct clk *clk;
	struct regmap *regmap;
	u32 max_arr;
};
#endif
