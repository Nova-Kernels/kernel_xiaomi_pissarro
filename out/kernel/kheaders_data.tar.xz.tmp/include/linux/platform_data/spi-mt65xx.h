

#ifndef ____LINUX_PLATFORM_DATA_SPI_MTK_H
#define ____LINUX_PLATFORM_DATA_SPI_MTK_H


struct mtk_chip_config {
	u32 tx_mlsb;
	u32 rx_mlsb;
	u32 sample_sel;

	//patch the parameter as customized
	u32 cs_setuptime;
	u32 cs_holdtime;
	u32 cs_idletime;
	u32 deassert_mode;
	u32 tick_delay;
};
#endif
