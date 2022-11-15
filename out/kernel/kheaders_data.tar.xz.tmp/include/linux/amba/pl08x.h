

#ifndef AMBA_PL08X_H
#define AMBA_PL08X_H


#include <linux/dmaengine.h>
#include <linux/interrupt.h>

struct pl08x_driver_data;
struct pl08x_phy_chan;
struct pl08x_txd;


enum {
	PL08X_AHB1 = (1 << 0),
	PL08X_AHB2 = (1 << 1)
};


struct pl08x_channel_data {
	const char *bus_id;
	int min_signal;
	int max_signal;
	u32 muxval;
	dma_addr_t addr;
	bool single;
	u8 periph_buses;
};

enum pl08x_burst_size {
	PL08X_BURST_SZ_1,
	PL08X_BURST_SZ_4,
	PL08X_BURST_SZ_8,
	PL08X_BURST_SZ_16,
	PL08X_BURST_SZ_32,
	PL08X_BURST_SZ_64,
	PL08X_BURST_SZ_128,
	PL08X_BURST_SZ_256,
};

enum pl08x_bus_width {
	PL08X_BUS_WIDTH_8_BITS,
	PL08X_BUS_WIDTH_16_BITS,
	PL08X_BUS_WIDTH_32_BITS,
};


struct pl08x_platform_data {
	struct pl08x_channel_data *slave_channels;
	unsigned int num_slave_channels;
	enum pl08x_burst_size memcpy_burst_size;
	enum pl08x_bus_width memcpy_bus_width;
	bool memcpy_prot_buff;
	bool memcpy_prot_cache;
	int (*get_xfer_signal)(const struct pl08x_channel_data *);
	void (*put_xfer_signal)(const struct pl08x_channel_data *, int);
	u8 lli_buses;
	u8 mem_buses;
	const struct dma_slave_map *slave_map;
	int slave_map_len;
};

#ifdef CONFIG_AMBA_PL08X
bool pl08x_filter_id(struct dma_chan *chan, void *chan_id);
#else
static inline bool pl08x_filter_id(struct dma_chan *chan, void *chan_id)
{
	return false;
}
#endif

#endif	
