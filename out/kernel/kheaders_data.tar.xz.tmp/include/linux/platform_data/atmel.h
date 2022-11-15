

#ifndef __ATMEL_H__
#define __ATMEL_H__

 
struct at91_cf_data {
	int	irq_pin;		
	int	det_pin;		
	int	vcc_pin;		
	int	rst_pin;		
	u8	chipselect;		
	u8	flags;
#define AT91_CF_TRUE_IDE	0x01
#define AT91_IDE_SWAP_A0_A2	0x02
};


#ifdef CONFIG_ATMEL_PM
extern int at91_suspend_entering_slow_clock(void);
#else
static inline int at91_suspend_entering_slow_clock(void)
{
	return 0;
}
#endif

#endif 
