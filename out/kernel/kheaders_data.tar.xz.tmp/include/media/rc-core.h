

#ifndef _RC_CORE
#define _RC_CORE

#include <linux/spinlock.h>
#include <linux/kfifo.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <media/rc-map.h>

extern int rc_core_debug;
#define IR_dprintk(level, fmt, ...)				\
do {								\
	if (rc_core_debug >= level)				\
		printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__);	\
} while (0)


enum rc_driver_type {
	RC_DRIVER_SCANCODE = 0,
	RC_DRIVER_IR_RAW,
	RC_DRIVER_IR_RAW_TX,
};


struct rc_scancode_filter {
	u32 data;
	u32 mask;
};


enum rc_filter_type {
	RC_FILTER_NORMAL = 0,
	RC_FILTER_WAKEUP,

	RC_FILTER_MAX
};


struct rc_dev {
	struct device			dev;
	bool				managed_alloc;
	const struct attribute_group	*sysfs_groups[5];
	const char			*device_name;
	const char			*input_phys;
	struct input_id			input_id;
	const char			*driver_name;
	const char			*map_name;
	struct rc_map			rc_map;
	struct mutex			lock;
	unsigned int			minor;
	struct ir_raw_event_ctrl	*raw;
	struct input_dev		*input_dev;
	enum rc_driver_type		driver_type;
	bool				idle;
	bool				encode_wakeup;
	u64				allowed_protocols;
	u64				enabled_protocols;
	u64				allowed_wakeup_protocols;
	enum rc_proto			wakeup_protocol;
	struct rc_scancode_filter	scancode_filter;
	struct rc_scancode_filter	scancode_wakeup_filter;
	u32				scancode_mask;
	u32				users;
	void				*priv;
	spinlock_t			keylock;
	bool				keypressed;
	unsigned long			keyup_jiffies;
	struct timer_list		timer_keyup;
	u32				last_keycode;
	enum rc_proto			last_protocol;
	u32				last_scancode;
	u8				last_toggle;
	u32				timeout;
	u32				min_timeout;
	u32				max_timeout;
	u32				rx_resolution;
	u32				tx_resolution;
	int				(*change_protocol)(struct rc_dev *dev, u64 *rc_proto);
	int				(*open)(struct rc_dev *dev);
	void				(*close)(struct rc_dev *dev);
	int				(*s_tx_mask)(struct rc_dev *dev, u32 mask);
	int				(*s_tx_carrier)(struct rc_dev *dev, u32 carrier);
	int				(*s_tx_duty_cycle)(struct rc_dev *dev, u32 duty_cycle);
	int				(*s_rx_carrier_range)(struct rc_dev *dev, u32 min, u32 max);
	int				(*tx_ir)(struct rc_dev *dev, unsigned *txbuf, unsigned n);
	void				(*s_idle)(struct rc_dev *dev, bool enable);
	int				(*s_learning_mode)(struct rc_dev *dev, int enable);
	int				(*s_carrier_report) (struct rc_dev *dev, int enable);
	int				(*s_filter)(struct rc_dev *dev,
						    struct rc_scancode_filter *filter);
	int				(*s_wakeup_filter)(struct rc_dev *dev,
							   struct rc_scancode_filter *filter);
	int				(*s_timeout)(struct rc_dev *dev,
						     unsigned int timeout);
};

#define to_rc_dev(d) container_of(d, struct rc_dev, dev)




struct rc_dev *rc_allocate_device(enum rc_driver_type);


struct rc_dev *devm_rc_allocate_device(struct device *dev, enum rc_driver_type);


void rc_free_device(struct rc_dev *dev);


int rc_register_device(struct rc_dev *dev);


int devm_rc_register_device(struct device *parent, struct rc_dev *dev);


void rc_unregister_device(struct rc_dev *dev);


int rc_open(struct rc_dev *rdev);


void rc_close(struct rc_dev *rdev);

void rc_repeat(struct rc_dev *dev);
void rc_keydown(struct rc_dev *dev, enum rc_proto protocol, u32 scancode,
		u8 toggle);
void rc_keydown_notimeout(struct rc_dev *dev, enum rc_proto protocol,
			  u32 scancode, u8 toggle);
void rc_keyup(struct rc_dev *dev);
u32 rc_g_keycode_from_table(struct rc_dev *dev, u32 scancode);


struct ir_raw_event {
	union {
		u32             duration;
		u32             carrier;
	};
	u8                      duty_cycle;

	unsigned                pulse:1;
	unsigned                reset:1;
	unsigned                timeout:1;
	unsigned                carrier_report:1;
};

#define DEFINE_IR_RAW_EVENT(event) struct ir_raw_event event = {}

static inline void init_ir_raw_event(struct ir_raw_event *ev)
{
	memset(ev, 0, sizeof(*ev));
}

#define IR_DEFAULT_TIMEOUT	MS_TO_NS(125)
#define IR_MAX_DURATION         500000000	
#define US_TO_NS(usec)		((usec) * 1000)
#define MS_TO_US(msec)		((msec) * 1000)
#define MS_TO_NS(msec)		((msec) * 1000 * 1000)

void ir_raw_event_handle(struct rc_dev *dev);
int ir_raw_event_store(struct rc_dev *dev, struct ir_raw_event *ev);
int ir_raw_event_store_edge(struct rc_dev *dev, bool pulse);
int ir_raw_event_store_with_filter(struct rc_dev *dev,
				struct ir_raw_event *ev);
void ir_raw_event_set_idle(struct rc_dev *dev, bool idle);
int ir_raw_encode_scancode(enum rc_proto protocol, u32 scancode,
			   struct ir_raw_event *events, unsigned int max);

static inline void ir_raw_event_reset(struct rc_dev *dev)
{
	struct ir_raw_event ev = { .reset = true };

	ir_raw_event_store(dev, &ev);
	ir_raw_event_handle(dev);
}


static inline u32 ir_extract_bits(u32 data, u32 mask)
{
	u32 vbit = 1, value = 0;

	do {
		if (mask & 1) {
			if (data & 1)
				value |= vbit;
			vbit <<= 1;
		}
		data >>= 1;
	} while (mask >>= 1);

	return value;
}


static inline u32 ir_nec_bytes_to_scancode(u8 address, u8 not_address,
					   u8 command, u8 not_command,
					   enum rc_proto *protocol)
{
	u32 scancode;

	if ((command ^ not_command) != 0xff) {
		
		scancode = not_address << 24 |
			address     << 16 |
			not_command <<  8 |
			command;
		*protocol = RC_PROTO_NEC32;
	} else if ((address ^ not_address) != 0xff) {
		
		scancode = address     << 16 |
			   not_address <<  8 |
			   command;
		*protocol = RC_PROTO_NECX;
	} else {
		
		scancode = address << 8 | command;
		*protocol = RC_PROTO_NEC;
	}

	return scancode;
}

#endif 
