/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_SERIAL_SCI_H
#define __LINUX_SERIAL_SCI_H

#include <linux/bitops.h>
#include <linux/serial_core.h>
#include <linux/sh_dma.h>




#define SCSCR_TIE	BIT(7)	
#define SCSCR_RIE	BIT(6)	
#define SCSCR_TE	BIT(5)	
#define SCSCR_RE	BIT(4)	
#define SCSCR_REIE	BIT(3)	
#define SCSCR_TOIE	BIT(2)	
#define SCSCR_CKE1	BIT(1)	
#define SCSCR_CKE0	BIT(0)	


enum {
	SCIx_PROBE_REGTYPE,

	SCIx_SCI_REGTYPE,
	SCIx_IRDA_REGTYPE,
	SCIx_SCIFA_REGTYPE,
	SCIx_SCIFB_REGTYPE,
	SCIx_SH2_SCIF_FIFODATA_REGTYPE,
	SCIx_SH3_SCIF_REGTYPE,
	SCIx_SH4_SCIF_REGTYPE,
	SCIx_SH4_SCIF_BRG_REGTYPE,
	SCIx_SH4_SCIF_NO_SCSPTR_REGTYPE,
	SCIx_SH4_SCIF_FIFODATA_REGTYPE,
	SCIx_SH7705_SCIF_REGTYPE,
	SCIx_HSCIF_REGTYPE,

	SCIx_NR_REGTYPES,
};

struct plat_sci_port_ops {
	void (*init_pins)(struct uart_port *, unsigned int cflag);
};


struct plat_sci_port {
	unsigned int	type;			
	upf_t		flags;			

	unsigned int	sampling_rate;
	unsigned int	scscr;			

	
	unsigned char	regtype;

	struct plat_sci_port_ops	*ops;
};

#endif 
