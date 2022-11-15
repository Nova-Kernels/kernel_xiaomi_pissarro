

#ifndef __SPI_S3C64XX_H
#define __SPI_S3C64XX_H

#include <linux/dmaengine.h>

struct platform_device;


struct s3c64xx_spi_csinfo {
	u8 fb_delay;
	unsigned line;
};


struct s3c64xx_spi_info {
	int src_clk_nr;
	int num_cs;
	bool no_cs;
	int (*cfg_gpio)(void);
};


extern void s3c64xx_spi0_set_platdata(int (*cfg_gpio)(void), int src_clk_nr,
						int num_cs);
extern void s3c64xx_spi1_set_platdata(int (*cfg_gpio)(void), int src_clk_nr,
						int num_cs);
extern void s3c64xx_spi2_set_platdata(int (*cfg_gpio)(void), int src_clk_nr,
						int num_cs);


extern int s3c64xx_spi0_cfg_gpio(void);
extern int s3c64xx_spi1_cfg_gpio(void);
extern int s3c64xx_spi2_cfg_gpio(void);

extern struct s3c64xx_spi_info s3c64xx_spi0_pdata;
extern struct s3c64xx_spi_info s3c64xx_spi1_pdata;
extern struct s3c64xx_spi_info s3c64xx_spi2_pdata;
#endif 
