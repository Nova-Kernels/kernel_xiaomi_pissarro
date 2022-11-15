

#ifndef _MEDIA_CEC_H
#define _MEDIA_CEC_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/cec-funcs.h>
#include <media/rc-core.h>
#include <media/cec-notifier.h>

#define CEC_CAP_DEFAULTS (CEC_CAP_LOG_ADDRS | CEC_CAP_TRANSMIT | \
			  CEC_CAP_PASSTHROUGH | CEC_CAP_RC)


struct cec_devnode {
	
	struct device dev;
	struct cdev cdev;

	
	int minor;
	bool registered;
	bool unregistered;
	struct list_head fhs;
	struct mutex lock;
};

struct cec_adapter;
struct cec_data;
struct cec_pin;

struct cec_data {
	struct list_head list;
	struct list_head xfer_list;
	struct cec_adapter *adap;
	struct cec_msg msg;
	struct cec_fh *fh;
	struct delayed_work work;
	struct completion c;
	u8 attempts;
	bool new_initiator;
	bool blocking;
	bool completed;
};

struct cec_msg_entry {
	struct list_head	list;
	struct cec_msg		msg;
};

struct cec_event_entry {
	struct list_head	list;
	struct cec_event	ev;
};

#define CEC_NUM_CORE_EVENTS 2
#define CEC_NUM_EVENTS CEC_EVENT_PIN_CEC_HIGH

struct cec_fh {
	struct list_head	list;
	struct list_head	xfer_list;
	struct cec_adapter	*adap;
	u8			mode_initiator;
	u8			mode_follower;

	
	wait_queue_head_t	wait;
	struct mutex		lock;
	struct list_head	events[CEC_NUM_EVENTS]; 
	u8			queued_events[CEC_NUM_EVENTS];
	unsigned int		total_queued_events;
	struct cec_event_entry	core_events[CEC_NUM_CORE_EVENTS];
	struct list_head	msgs; 
	unsigned int		queued_msgs;
};

#define CEC_SIGNAL_FREE_TIME_RETRY		3
#define CEC_SIGNAL_FREE_TIME_NEW_INITIATOR	5
#define CEC_SIGNAL_FREE_TIME_NEXT_XFER		7


#define CEC_FREE_TIME_TO_USEC(ft)		((ft) * 2400)

struct cec_adap_ops {
	
	int (*adap_enable)(struct cec_adapter *adap, bool enable);
	int (*adap_monitor_all_enable)(struct cec_adapter *adap, bool enable);
	int (*adap_log_addr)(struct cec_adapter *adap, u8 logical_addr);
	int (*adap_transmit)(struct cec_adapter *adap, u8 attempts,
			     u32 signal_free_time, struct cec_msg *msg);
	void (*adap_status)(struct cec_adapter *adap, struct seq_file *file);
	void (*adap_free)(struct cec_adapter *adap);

	
	int (*received)(struct cec_adapter *adap, struct cec_msg *msg);
};


#define CEC_MAX_MSG_RX_QUEUE_SZ		(18 * 3)


#define CEC_MAX_MSG_TX_QUEUE_SZ		(18 * 1)

struct cec_adapter {
	struct module *owner;
	char name[32];
	struct cec_devnode devnode;
	struct mutex lock;
	struct rc_dev *rc;

	struct list_head transmit_queue;
	unsigned int transmit_queue_sz;
	struct list_head wait_queue;
	struct cec_data *transmitting;

	struct task_struct *kthread_config;
	struct completion config_completion;

	struct task_struct *kthread;
	wait_queue_head_t kthread_waitq;
	wait_queue_head_t waitq;

	const struct cec_adap_ops *ops;
	void *priv;
	u32 capabilities;
	u8 available_log_addrs;

	u16 phys_addr;
	bool needs_hpd;
	bool is_configuring;
	bool is_configured;
	bool cec_pin_is_high;
	u32 monitor_all_cnt;
	u32 monitor_pin_cnt;
	u32 follower_cnt;
	struct cec_fh *cec_follower;
	struct cec_fh *cec_initiator;
	bool passthrough;
	struct cec_log_addrs log_addrs;

	u32 tx_timeouts;

#ifdef CONFIG_MEDIA_CEC_RC
	bool rc_repeating;
	int rc_last_scancode;
	u64 rc_last_keypress;
#endif
#ifdef CONFIG_CEC_NOTIFIER
	struct cec_notifier *notifier;
#endif
#ifdef CONFIG_CEC_PIN
	struct cec_pin *pin;
#endif

	struct dentry *cec_dir;
	struct dentry *status_file;

	u16 phys_addrs[15];
	u32 sequence;

	char device_name[32];
	char input_phys[32];
	char input_drv[32];
};

static inline void *cec_get_drvdata(const struct cec_adapter *adap)
{
	return adap->priv;
}

static inline bool cec_has_log_addr(const struct cec_adapter *adap, u8 log_addr)
{
	return adap->log_addrs.log_addr_mask & (1 << log_addr);
}

static inline bool cec_is_sink(const struct cec_adapter *adap)
{
	return adap->phys_addr == 0;
}

#define cec_phys_addr_exp(pa) \
	((pa) >> 12), ((pa) >> 8) & 0xf, ((pa) >> 4) & 0xf, (pa) & 0xf

struct edid;

#if IS_REACHABLE(CONFIG_CEC_CORE)
struct cec_adapter *cec_allocate_adapter(const struct cec_adap_ops *ops,
		void *priv, const char *name, u32 caps, u8 available_las);
int cec_register_adapter(struct cec_adapter *adap, struct device *parent);
void cec_unregister_adapter(struct cec_adapter *adap);
void cec_delete_adapter(struct cec_adapter *adap);

int cec_s_log_addrs(struct cec_adapter *adap, struct cec_log_addrs *log_addrs,
		    bool block);
void cec_s_phys_addr(struct cec_adapter *adap, u16 phys_addr,
		     bool block);
void cec_s_phys_addr_from_edid(struct cec_adapter *adap,
			       const struct edid *edid);
int cec_transmit_msg(struct cec_adapter *adap, struct cec_msg *msg,
		     bool block);


void cec_transmit_done_ts(struct cec_adapter *adap, u8 status,
			  u8 arb_lost_cnt, u8 nack_cnt, u8 low_drive_cnt,
			  u8 error_cnt, ktime_t ts);

static inline void cec_transmit_done(struct cec_adapter *adap, u8 status,
				     u8 arb_lost_cnt, u8 nack_cnt,
				     u8 low_drive_cnt, u8 error_cnt)
{
	cec_transmit_done_ts(adap, status, arb_lost_cnt, nack_cnt,
			     low_drive_cnt, error_cnt, ktime_get());
}

void cec_transmit_attempt_done_ts(struct cec_adapter *adap,
				  u8 status, ktime_t ts);

static inline void cec_transmit_attempt_done(struct cec_adapter *adap,
					     u8 status)
{
	cec_transmit_attempt_done_ts(adap, status, ktime_get());
}

void cec_received_msg_ts(struct cec_adapter *adap,
			 struct cec_msg *msg, ktime_t ts);

static inline void cec_received_msg(struct cec_adapter *adap,
				    struct cec_msg *msg)
{
	cec_received_msg_ts(adap, msg, ktime_get());
}


void cec_queue_pin_cec_event(struct cec_adapter *adap,
			     bool is_high, ktime_t ts);


u16 cec_get_edid_phys_addr(const u8 *edid, unsigned int size,
			   unsigned int *offset);


void cec_set_edid_phys_addr(u8 *edid, unsigned int size, u16 phys_addr);


u16 cec_phys_addr_for_input(u16 phys_addr, u8 input);


int cec_phys_addr_validate(u16 phys_addr, u16 *parent, u16 *port);

#else

static inline int cec_register_adapter(struct cec_adapter *adap,
				       struct device *parent)
{
	return 0;
}

static inline void cec_unregister_adapter(struct cec_adapter *adap)
{
}

static inline void cec_delete_adapter(struct cec_adapter *adap)
{
}

static inline void cec_s_phys_addr(struct cec_adapter *adap, u16 phys_addr,
				   bool block)
{
}

static inline void cec_s_phys_addr_from_edid(struct cec_adapter *adap,
					     const struct edid *edid)
{
}

static inline u16 cec_get_edid_phys_addr(const u8 *edid, unsigned int size,
					 unsigned int *offset)
{
	if (offset)
		*offset = 0;
	return CEC_PHYS_ADDR_INVALID;
}

static inline void cec_set_edid_phys_addr(u8 *edid, unsigned int size,
					  u16 phys_addr)
{
}

static inline u16 cec_phys_addr_for_input(u16 phys_addr, u8 input)
{
	return CEC_PHYS_ADDR_INVALID;
}

static inline int cec_phys_addr_validate(u16 phys_addr, u16 *parent, u16 *port)
{
	return 0;
}

#endif


static inline void cec_phys_addr_invalidate(struct cec_adapter *adap)
{
	cec_s_phys_addr(adap, CEC_PHYS_ADDR_INVALID, false);
}


static inline unsigned int cec_get_edid_spa_location(const u8 *edid,
						     unsigned int size)
{
	unsigned int blocks = size / 128;
	unsigned int block;
	u8 d;

	
	if (blocks < 2 || size % 128)
		return 0;

	
	if (edid[0x7e] + 1 < blocks)
		blocks = edid[0x7e] + 1;

	for (block = 1; block < blocks; block++) {
		unsigned int offset = block * 128;

		
		if (edid[offset] != 0x02 || edid[offset + 1] != 0x03)
			continue;

		
		d = edid[offset + 2] & 0x7f;
		
		if (d <= 4)
			continue;
		if (d > 4) {
			unsigned int i = offset + 4;
			unsigned int end = offset + d;

			
			do {
				u8 tag = edid[i] >> 5;
				u8 len = edid[i] & 0x1f;

				if (tag == 3 && len >= 5 && i + len <= end &&
				    edid[i + 1] == 0x03 &&
				    edid[i + 2] == 0x0c &&
				    edid[i + 3] == 0x00)
					return i + 4;
				i += len + 1;
			} while (i < end);
		}
	}
	return 0;
}

#endif 
