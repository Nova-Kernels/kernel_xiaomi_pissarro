

#ifndef __TEGRA_IVC_H

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/types.h>

struct tegra_ivc_header;

struct tegra_ivc {
	struct device *peer;

	struct {
		struct tegra_ivc_header *channel;
		unsigned int position;
		dma_addr_t phys;
	} rx, tx;

	void (*notify)(struct tegra_ivc *ivc, void *data);
	void *notify_data;

	unsigned int num_frames;
	size_t frame_size;
};


void *tegra_ivc_read_get_next_frame(struct tegra_ivc *ivc);


int tegra_ivc_read_advance(struct tegra_ivc *ivc);


void *tegra_ivc_write_get_next_frame(struct tegra_ivc *ivc);


int tegra_ivc_write_advance(struct tegra_ivc *ivc);


int tegra_ivc_notified(struct tegra_ivc *ivc);


void tegra_ivc_reset(struct tegra_ivc *ivc);

size_t tegra_ivc_align(size_t size);
unsigned tegra_ivc_total_queue_size(unsigned queue_size);
int tegra_ivc_init(struct tegra_ivc *ivc, struct device *peer, void *rx,
		   dma_addr_t rx_phys, void *tx, dma_addr_t tx_phys,
		   unsigned int num_frames, size_t frame_size,
		   void (*notify)(struct tegra_ivc *ivc, void *data),
		   void *data);
void tegra_ivc_cleanup(struct tegra_ivc *ivc);

#endif 
