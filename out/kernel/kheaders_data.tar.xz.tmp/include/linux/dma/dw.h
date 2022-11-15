
#ifndef _DMA_DW_H
#define _DMA_DW_H

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/dmaengine.h>

#include <linux/platform_data/dma-dw.h>

struct dw_dma;


struct dw_dma_chip {
	struct device	*dev;
	int		id;
	int		irq;
	void __iomem	*regs;
	struct clk	*clk;
	struct dw_dma	*dw;

	const struct dw_dma_platform_data	*pdata;
};


#if IS_ENABLED(CONFIG_DW_DMAC_CORE)
int dw_dma_probe(struct dw_dma_chip *chip);
int dw_dma_remove(struct dw_dma_chip *chip);
#else
static inline int dw_dma_probe(struct dw_dma_chip *chip) { return -ENODEV; }
static inline int dw_dma_remove(struct dw_dma_chip *chip) { return 0; }
#endif 

#endif 
