

#ifndef LINUX_CEC_PIN_H
#define LINUX_CEC_PIN_H

#include <linux/types.h>
#include <linux/atomic.h>
#include <media/cec.h>

enum cec_pin_state {
	
	CEC_ST_OFF,
	
	CEC_ST_IDLE,

	

	
	CEC_ST_TX_WAIT,
	
	CEC_ST_TX_WAIT_FOR_HIGH,
	
	CEC_ST_TX_START_BIT_LOW,
	
	CEC_ST_TX_START_BIT_HIGH,
	
	CEC_ST_TX_DATA_BIT_0_LOW,
	
	CEC_ST_TX_DATA_BIT_0_HIGH,
	
	CEC_ST_TX_DATA_BIT_1_LOW,
	
	CEC_ST_TX_DATA_BIT_1_HIGH,
	
	CEC_ST_TX_DATA_BIT_1_HIGH_PRE_SAMPLE,
	
	CEC_ST_TX_DATA_BIT_1_HIGH_POST_SAMPLE,

	

	
	CEC_ST_RX_START_BIT_LOW,
	
	CEC_ST_RX_START_BIT_HIGH,
	
	CEC_ST_RX_DATA_SAMPLE,
	
	CEC_ST_RX_DATA_POST_SAMPLE,
	
	CEC_ST_RX_DATA_HIGH,
	
	CEC_ST_RX_ACK_LOW,
	
	CEC_ST_RX_ACK_LOW_POST,
	
	CEC_ST_RX_ACK_HIGH_POST,
	
	CEC_ST_RX_ACK_FINISH,

	
	CEC_ST_LOW_DRIVE,
	
	CEC_ST_RX_IRQ,

	
	CEC_PIN_STATES
};


struct cec_pin_ops {
	bool (*read)(struct cec_adapter *adap);
	void (*low)(struct cec_adapter *adap);
	void (*high)(struct cec_adapter *adap);
	bool (*enable_irq)(struct cec_adapter *adap);
	void (*disable_irq)(struct cec_adapter *adap);
	void (*free)(struct cec_adapter *adap);
	void (*status)(struct cec_adapter *adap, struct seq_file *file);
};

#define CEC_NUM_PIN_EVENTS 128

#define CEC_PIN_IRQ_UNCHANGED	0
#define CEC_PIN_IRQ_DISABLE	1
#define CEC_PIN_IRQ_ENABLE	2

struct cec_pin {
	struct cec_adapter		*adap;
	const struct cec_pin_ops	*ops;
	struct task_struct		*kthread;
	wait_queue_head_t		kthread_waitq;
	struct hrtimer			timer;
	ktime_t				ts;
	unsigned int			wait_usecs;
	u16				la_mask;
	bool				enabled;
	bool				monitor_all;
	bool				rx_eom;
	bool				enable_irq_failed;
	enum cec_pin_state		state;
	struct cec_msg			tx_msg;
	u32				tx_bit;
	bool				tx_nacked;
	u32				tx_signal_free_time;
	struct cec_msg			rx_msg;
	u32				rx_bit;

	struct cec_msg			work_rx_msg;
	u8				work_tx_status;
	ktime_t				work_tx_ts;
	atomic_t			work_irq_change;
	atomic_t			work_pin_events;
	unsigned int			work_pin_events_wr;
	unsigned int			work_pin_events_rd;
	ktime_t				work_pin_ts[CEC_NUM_PIN_EVENTS];
	bool				work_pin_is_high[CEC_NUM_PIN_EVENTS];
	ktime_t				timer_ts;
	u32				timer_cnt;
	u32				timer_100ms_overruns;
	u32				timer_300ms_overruns;
	u32				timer_max_overrun;
	u32				timer_sum_overrun;
};


void cec_pin_changed(struct cec_adapter *adap, bool value);


struct cec_adapter *cec_pin_allocate_adapter(const struct cec_pin_ops *pin_ops,
					void *priv, const char *name, u32 caps);

#endif
