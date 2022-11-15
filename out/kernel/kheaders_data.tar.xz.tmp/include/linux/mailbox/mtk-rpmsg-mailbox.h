

#ifndef __MTK_RPMSG_MAILBOX_H__
#define __MTK_RPMSG_MAILBOX_H__

#include <linux/mailbox_controller.h>
#include <linux/mailbox_client.h>

enum MTK_MBOX_STATUS {
	LAST_TX_DONE = 0,
	LAST_TX_BUSY = 1,
};

struct mtk_mbox_chan {
	struct mbox_chan *chan;
	struct mtk_mbox_device *mbdev;
	struct mtk_mbox_operations *ops;
	struct mtk_ipi_msg *ipimsg;
	unsigned int mbox;
	unsigned int send_slot;
	unsigned int send_slot_size;
	unsigned int send_pin_index;
	unsigned int send_pin_offset;
};


struct mtk_mbox_operations {
	int (*mtk_send_ipi)(struct mtk_mbox_chan *mchan, void *data);
	bool (*mtk_tx_done)(struct mtk_mbox_chan *chan);
	int (*mtk_startup)(struct mtk_mbox_chan *chan);
	void (*mtk_shutdown)(struct mtk_mbox_chan *chan);
};

int mtk_mbox_chan_create(struct mbox_controller *mboxctrl,
		struct mtk_mbox_device *mbdev, int num);

#endif 
