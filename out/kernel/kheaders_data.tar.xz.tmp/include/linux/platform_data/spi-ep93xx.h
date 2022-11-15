/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_MACH_EP93XX_SPI_H
#define __ASM_MACH_EP93XX_SPI_H

struct spi_device;


struct ep93xx_spi_info {
	int	*chipselect;
	int	num_chipselect;
	bool	use_dma;
};

#endif 
