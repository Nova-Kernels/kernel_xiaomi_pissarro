

#ifndef __ASM_ARCH_USB_H
#define __ASM_ARCH_USB_H

struct	da8xx_ohci_root_hub;

typedef void (*da8xx_ocic_handler_t)(struct da8xx_ohci_root_hub *hub,
				     unsigned port);


struct	da8xx_ohci_root_hub {
	
	int	(*set_power)(unsigned port, int on);
	
	int	(*get_power)(unsigned port);
	
	int	(*get_oci)(unsigned port);
	
	int	(*ocic_notify)(da8xx_ocic_handler_t handler);

	
	u8	potpgt;
};

void davinci_setup_usb(unsigned mA, unsigned potpgt_ms);

#endif	
