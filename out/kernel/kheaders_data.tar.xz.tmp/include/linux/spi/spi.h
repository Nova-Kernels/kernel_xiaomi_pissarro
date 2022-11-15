

#ifndef __LINUX_SPI_H
#define __LINUX_SPI_H

#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/scatterlist.h>

struct dma_chan;
struct property_entry;
struct spi_controller;
struct spi_transfer;
struct spi_flash_read_message;


extern struct bus_type spi_bus_type;


struct spi_statistics {
	spinlock_t		lock; 

	unsigned long		messages;
	unsigned long		transfers;
	unsigned long		errors;
	unsigned long		timedout;

	unsigned long		spi_sync;
	unsigned long		spi_sync_immediate;
	unsigned long		spi_async;

	unsigned long long	bytes;
	unsigned long long	bytes_rx;
	unsigned long long	bytes_tx;

#define SPI_STATISTICS_HISTO_SIZE 17
	unsigned long transfer_bytes_histo[SPI_STATISTICS_HISTO_SIZE];

	unsigned long transfers_split_maxsize;
};

void spi_statistics_add_transfer_stats(struct spi_statistics *stats,
				       struct spi_transfer *xfer,
				       struct spi_controller *ctlr);

#define SPI_STATISTICS_ADD_TO_FIELD(stats, field, count)	\
	do {							\
		unsigned long flags;				\
		spin_lock_irqsave(&(stats)->lock, flags);	\
		(stats)->field += count;			\
		spin_unlock_irqrestore(&(stats)->lock, flags);	\
	} while (0)

#define SPI_STATISTICS_INCREMENT_FIELD(stats, field)	\
	SPI_STATISTICS_ADD_TO_FIELD(stats, field, 1)


struct spi_device {
	struct device		dev;
	struct spi_controller	*controller;
	struct spi_controller	*master;	
	u32			max_speed_hz;
	u8			chip_select;
	u8			bits_per_word;
	u16			mode;
#define	SPI_CPHA	0x01			
#define	SPI_CPOL	0x02			
#define	SPI_MODE_0	(0|0)			
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			
#define	SPI_LSB_FIRST	0x08			
#define	SPI_3WIRE	0x10			
#define	SPI_LOOP	0x20			
#define	SPI_NO_CS	0x40			
#define	SPI_READY	0x80			
#define	SPI_TX_DUAL	0x100			
#define	SPI_TX_QUAD	0x200			
#define	SPI_RX_DUAL	0x400			
#define	SPI_RX_QUAD	0x800			
	int			irq;
	void			*controller_state;
	void			*controller_data;
	char			modalias[SPI_NAME_SIZE];
	int			cs_gpio;	

	
	struct spi_statistics	statistics;

	
};

static inline struct spi_device *to_spi_device(struct device *dev)
{
	return dev ? container_of(dev, struct spi_device, dev) : NULL;
}


static inline struct spi_device *spi_dev_get(struct spi_device *spi)
{
	return (spi && get_device(&spi->dev)) ? spi : NULL;
}

static inline void spi_dev_put(struct spi_device *spi)
{
	if (spi)
		put_device(&spi->dev);
}


static inline void *spi_get_ctldata(struct spi_device *spi)
{
	return spi->controller_state;
}

static inline void spi_set_ctldata(struct spi_device *spi, void *state)
{
	spi->controller_state = state;
}



static inline void spi_set_drvdata(struct spi_device *spi, void *data)
{
	dev_set_drvdata(&spi->dev, data);
}

static inline void *spi_get_drvdata(struct spi_device *spi)
{
	return dev_get_drvdata(&spi->dev);
}

struct spi_message;
struct spi_transfer;


struct spi_driver {
	const struct spi_device_id *id_table;
	int			(*probe)(struct spi_device *spi);
	int			(*remove)(struct spi_device *spi);
	void			(*shutdown)(struct spi_device *spi);
	struct device_driver	driver;
};

static inline struct spi_driver *to_spi_driver(struct device_driver *drv)
{
	return drv ? container_of(drv, struct spi_driver, driver) : NULL;
}

extern int __spi_register_driver(struct module *owner, struct spi_driver *sdrv);


static inline void spi_unregister_driver(struct spi_driver *sdrv)
{
	if (sdrv)
		driver_unregister(&sdrv->driver);
}


#define spi_register_driver(driver) \
	__spi_register_driver(THIS_MODULE, driver)


#define module_spi_driver(__spi_driver) \
	module_driver(__spi_driver, spi_register_driver, \
			spi_unregister_driver)


struct spi_controller {
	struct device	dev;

	struct list_head list;

	
	s16			bus_num;

	
	u16			num_chipselect;

	
	u16			dma_alignment;

	
	u16			mode_bits;

	
	u32			bits_per_word_mask;
#define SPI_BPW_MASK(bits) BIT((bits) - 1)
#define SPI_BIT_MASK(bits) (((bits) == 32) ? ~0U : (BIT(bits) - 1))
#define SPI_BPW_RANGE_MASK(min, max) (SPI_BIT_MASK(max) - SPI_BIT_MASK(min - 1))

	
	u32			min_speed_hz;
	u32			max_speed_hz;

	
	u16			flags;
#define SPI_CONTROLLER_HALF_DUPLEX	BIT(0)	
#define SPI_CONTROLLER_NO_RX		BIT(1)	
#define SPI_CONTROLLER_NO_TX		BIT(2)	
#define SPI_CONTROLLER_MUST_RX		BIT(3)	
#define SPI_CONTROLLER_MUST_TX		BIT(4)	

#define SPI_MASTER_GPIO_SS		BIT(5)	

	
	bool			slave;

	
	size_t (*max_transfer_size)(struct spi_device *spi);
	size_t (*max_message_size)(struct spi_device *spi);

	
	struct mutex		io_mutex;

	
	spinlock_t		bus_lock_spinlock;
	struct mutex		bus_lock_mutex;

	
	bool			bus_lock_flag;

	
	int			(*setup)(struct spi_device *spi);

	
	int			(*transfer)(struct spi_device *spi,
						struct spi_message *mesg);

	
	void			(*cleanup)(struct spi_device *spi);

	
	bool			(*can_dma)(struct spi_controller *ctlr,
					   struct spi_device *spi,
					   struct spi_transfer *xfer);

	
	bool				queued;
	struct kthread_worker		kworker;
	struct task_struct		*kworker_task;
	struct kthread_work		pump_messages;
	spinlock_t			queue_lock;
	struct list_head		queue;
	struct spi_message		*cur_msg;
	bool				idling;
	bool				busy;
	bool				running;
	bool				rt;
	bool				auto_runtime_pm;
	bool                            cur_msg_prepared;
	bool				cur_msg_mapped;
	struct completion               xfer_completion;
	size_t				max_dma_len;

	int (*prepare_transfer_hardware)(struct spi_controller *ctlr);
	int (*transfer_one_message)(struct spi_controller *ctlr,
				    struct spi_message *mesg);
	int (*unprepare_transfer_hardware)(struct spi_controller *ctlr);
	int (*prepare_message)(struct spi_controller *ctlr,
			       struct spi_message *message);
	int (*unprepare_message)(struct spi_controller *ctlr,
				 struct spi_message *message);
	int (*slave_abort)(struct spi_controller *ctlr);
	int (*spi_flash_read)(struct  spi_device *spi,
			      struct spi_flash_read_message *msg);
	bool (*spi_flash_can_dma)(struct spi_device *spi,
				  struct spi_flash_read_message *msg);
	bool (*flash_read_supported)(struct spi_device *spi);

	
	void (*set_cs)(struct spi_device *spi, bool enable);
	int (*transfer_one)(struct spi_controller *ctlr, struct spi_device *spi,
			    struct spi_transfer *transfer);
	void (*handle_err)(struct spi_controller *ctlr,
			   struct spi_message *message);

	
	int			*cs_gpios;

	
	struct spi_statistics	statistics;

	
	struct dma_chan		*dma_tx;
	struct dma_chan		*dma_rx;

	
	void			*dummy_rx;
	void			*dummy_tx;

	int (*fw_translate_cs)(struct spi_controller *ctlr, unsigned cs);
};

static inline void *spi_controller_get_devdata(struct spi_controller *ctlr)
{
	return dev_get_drvdata(&ctlr->dev);
}

static inline void spi_controller_set_devdata(struct spi_controller *ctlr,
					      void *data)
{
	dev_set_drvdata(&ctlr->dev, data);
}

static inline struct spi_controller *spi_controller_get(struct spi_controller *ctlr)
{
	if (!ctlr || !get_device(&ctlr->dev))
		return NULL;
	return ctlr;
}

static inline void spi_controller_put(struct spi_controller *ctlr)
{
	if (ctlr)
		put_device(&ctlr->dev);
}

static inline bool spi_controller_is_slave(struct spi_controller *ctlr)
{
	return IS_ENABLED(CONFIG_SPI_SLAVE) && ctlr->slave;
}


extern int spi_controller_suspend(struct spi_controller *ctlr);
extern int spi_controller_resume(struct spi_controller *ctlr);


extern struct spi_message *spi_get_next_queued_message(struct spi_controller *ctlr);
extern void spi_finalize_current_message(struct spi_controller *ctlr);
extern void spi_finalize_current_transfer(struct spi_controller *ctlr);


extern struct spi_controller *__spi_alloc_controller(struct device *host,
						unsigned int size, bool slave);

static inline struct spi_controller *spi_alloc_master(struct device *host,
						      unsigned int size)
{
	return __spi_alloc_controller(host, size, false);
}

static inline struct spi_controller *spi_alloc_slave(struct device *host,
						     unsigned int size)
{
	if (!IS_ENABLED(CONFIG_SPI_SLAVE))
		return NULL;

	return __spi_alloc_controller(host, size, true);
}

extern int spi_register_controller(struct spi_controller *ctlr);
extern int devm_spi_register_controller(struct device *dev,
					struct spi_controller *ctlr);
extern void spi_unregister_controller(struct spi_controller *ctlr);

extern struct spi_controller *spi_busnum_to_master(u16 busnum);



typedef void (*spi_res_release_t)(struct spi_controller *ctlr,
				  struct spi_message *msg,
				  void *res);


struct spi_res {
	struct list_head        entry;
	spi_res_release_t       release;
	unsigned long long      data[]; 
};

extern void *spi_res_alloc(struct spi_device *spi,
			   spi_res_release_t release,
			   size_t size, gfp_t gfp);
extern void spi_res_add(struct spi_message *message, void *res);
extern void spi_res_free(void *res);

extern void spi_res_release(struct spi_controller *ctlr,
			    struct spi_message *message);






struct spi_transfer {
	
	const void	*tx_buf;
	void		*rx_buf;
	unsigned	len;

	dma_addr_t	tx_dma;
	dma_addr_t	rx_dma;
	struct sg_table tx_sg;
	struct sg_table rx_sg;

	unsigned	cs_change:1;
	unsigned	tx_nbits:3;
	unsigned	rx_nbits:3;
#define	SPI_NBITS_SINGLE	0x01 
#define	SPI_NBITS_DUAL		0x02 
#define	SPI_NBITS_QUAD		0x04 
	u8		bits_per_word;
	u16		delay_usecs;
	u32		speed_hz;

	struct list_head transfer_list;
};


struct spi_message {
	struct list_head	transfers;

	struct spi_device	*spi;

	unsigned		is_dma_mapped:1;

	

	
	void			(*complete)(void *context);
	void			*context;
	unsigned		frame_length;
	unsigned		actual_length;
	int			status;

	
	struct list_head	queue;
	void			*state;

	
	struct list_head        resources;
};

static inline void spi_message_init_no_memset(struct spi_message *m)
{
	INIT_LIST_HEAD(&m->transfers);
	INIT_LIST_HEAD(&m->resources);
}

static inline void spi_message_init(struct spi_message *m)
{
	memset(m, 0, sizeof *m);
	spi_message_init_no_memset(m);
}

static inline void
spi_message_add_tail(struct spi_transfer *t, struct spi_message *m)
{
	list_add_tail(&t->transfer_list, &m->transfers);
}

static inline void
spi_transfer_del(struct spi_transfer *t)
{
	list_del(&t->transfer_list);
}


static inline void
spi_message_init_with_transfers(struct spi_message *m,
struct spi_transfer *xfers, unsigned int num_xfers)
{
	unsigned int i;

	spi_message_init(m);
	for (i = 0; i < num_xfers; ++i)
		spi_message_add_tail(&xfers[i], m);
}



static inline struct spi_message *spi_message_alloc(unsigned ntrans, gfp_t flags)
{
	struct spi_message *m;

	m = kzalloc(sizeof(struct spi_message)
			+ ntrans * sizeof(struct spi_transfer),
			flags);
	if (m) {
		unsigned i;
		struct spi_transfer *t = (struct spi_transfer *)(m + 1);

		spi_message_init_no_memset(m);
		for (i = 0; i < ntrans; i++, t++)
			spi_message_add_tail(t, m);
	}
	return m;
}

static inline void spi_message_free(struct spi_message *m)
{
	kfree(m);
}

extern int spi_setup(struct spi_device *spi);
extern int spi_async(struct spi_device *spi, struct spi_message *message);
extern int spi_async_locked(struct spi_device *spi,
			    struct spi_message *message);
extern int spi_slave_abort(struct spi_device *spi);

static inline size_t
spi_max_message_size(struct spi_device *spi)
{
	struct spi_controller *ctlr = spi->controller;

	if (!ctlr->max_message_size)
		return SIZE_MAX;
	return ctlr->max_message_size(spi);
}

static inline size_t
spi_max_transfer_size(struct spi_device *spi)
{
	struct spi_controller *ctlr = spi->controller;
	size_t tr_max = SIZE_MAX;
	size_t msg_max = spi_max_message_size(spi);

	if (ctlr->max_transfer_size)
		tr_max = ctlr->max_transfer_size(spi);

	
	return min(tr_max, msg_max);
}





struct spi_replaced_transfers;
typedef void (*spi_replaced_release_t)(struct spi_controller *ctlr,
				       struct spi_message *msg,
				       struct spi_replaced_transfers *res);

struct spi_replaced_transfers {
	spi_replaced_release_t release;
	void *extradata;
	struct list_head replaced_transfers;
	struct list_head *replaced_after;
	size_t inserted;
	struct spi_transfer inserted_transfers[];
};

extern struct spi_replaced_transfers *spi_replace_transfers(
	struct spi_message *msg,
	struct spi_transfer *xfer_first,
	size_t remove,
	size_t insert,
	spi_replaced_release_t release,
	size_t extradatasize,
	gfp_t gfp);





extern int spi_split_transfers_maxsize(struct spi_controller *ctlr,
				       struct spi_message *msg,
				       size_t maxsize,
				       gfp_t gfp);





extern int spi_sync(struct spi_device *spi, struct spi_message *message);
extern int spi_sync_locked(struct spi_device *spi, struct spi_message *message);
extern int spi_bus_lock(struct spi_controller *ctlr);
extern int spi_bus_unlock(struct spi_controller *ctlr);


static inline int
spi_sync_transfer(struct spi_device *spi, struct spi_transfer *xfers,
	unsigned int num_xfers)
{
	struct spi_message msg;

	spi_message_init_with_transfers(&msg, xfers, num_xfers);

	return spi_sync(spi, &msg);
}


static inline int
spi_write(struct spi_device *spi, const void *buf, size_t len)
{
	struct spi_transfer	t = {
			.tx_buf		= buf,
			.len		= len,
		};

	return spi_sync_transfer(spi, &t, 1);
}


static inline int
spi_read(struct spi_device *spi, void *buf, size_t len)
{
	struct spi_transfer	t = {
			.rx_buf		= buf,
			.len		= len,
		};

	return spi_sync_transfer(spi, &t, 1);
}


extern int spi_write_then_read(struct spi_device *spi,
		const void *txbuf, unsigned n_tx,
		void *rxbuf, unsigned n_rx);


static inline ssize_t spi_w8r8(struct spi_device *spi, u8 cmd)
{
	ssize_t			status;
	u8			result;

	status = spi_write_then_read(spi, &cmd, 1, &result, 1);

	
	return (status < 0) ? status : result;
}


static inline ssize_t spi_w8r16(struct spi_device *spi, u8 cmd)
{
	ssize_t			status;
	u16			result;

	status = spi_write_then_read(spi, &cmd, 1, &result, 2);

	
	return (status < 0) ? status : result;
}


static inline ssize_t spi_w8r16be(struct spi_device *spi, u8 cmd)

{
	ssize_t status;
	__be16 result;

	status = spi_write_then_read(spi, &cmd, 1, &result, 2);
	if (status < 0)
		return status;

	return be16_to_cpu(result);
}


struct spi_flash_read_message {
	void *buf;
	loff_t from;
	size_t len;
	size_t retlen;
	u8 read_opcode;
	u8 addr_width;
	u8 dummy_bytes;
	u8 opcode_nbits;
	u8 addr_nbits;
	u8 data_nbits;
	struct sg_table rx_sg;
	bool cur_msg_mapped;
};


static inline bool spi_flash_read_supported(struct spi_device *spi)
{
	return spi->controller->spi_flash_read &&
	       (!spi->controller->flash_read_supported ||
	       spi->controller->flash_read_supported(spi));
}

int spi_flash_read(struct spi_device *spi,
		   struct spi_flash_read_message *msg);






struct spi_board_info {
	
	char		modalias[SPI_NAME_SIZE];
	const void	*platform_data;
	const struct property_entry *properties;
	void		*controller_data;
	int		irq;

	
	u32		max_speed_hz;


	
	u16		bus_num;
	u16		chip_select;

	
	u16		mode;

	
};

#ifdef	CONFIG_SPI
extern int
spi_register_board_info(struct spi_board_info const *info, unsigned n);
#else

static inline int
spi_register_board_info(struct spi_board_info const *info, unsigned n)
	{ return 0; }
#endif



extern struct spi_device *
spi_alloc_device(struct spi_controller *ctlr);

extern int
spi_add_device(struct spi_device *spi);

extern struct spi_device *
spi_new_device(struct spi_controller *, struct spi_board_info *);

extern void spi_unregister_device(struct spi_device *spi);

extern const struct spi_device_id *
spi_get_device_id(const struct spi_device *sdev);

static inline bool
spi_transfer_is_last(struct spi_controller *ctlr, struct spi_transfer *xfer)
{
	return list_is_last(&xfer->transfer_list, &ctlr->cur_msg->transfers);
}



#define spi_master			spi_controller

#define SPI_MASTER_HALF_DUPLEX		SPI_CONTROLLER_HALF_DUPLEX
#define SPI_MASTER_NO_RX		SPI_CONTROLLER_NO_RX
#define SPI_MASTER_NO_TX		SPI_CONTROLLER_NO_TX
#define SPI_MASTER_MUST_RX		SPI_CONTROLLER_MUST_RX
#define SPI_MASTER_MUST_TX		SPI_CONTROLLER_MUST_TX

#define spi_master_get_devdata(_ctlr)	spi_controller_get_devdata(_ctlr)
#define spi_master_set_devdata(_ctlr, _data)	\
	spi_controller_set_devdata(_ctlr, _data)
#define spi_master_get(_ctlr)		spi_controller_get(_ctlr)
#define spi_master_put(_ctlr)		spi_controller_put(_ctlr)
#define spi_master_suspend(_ctlr)	spi_controller_suspend(_ctlr)
#define spi_master_resume(_ctlr)	spi_controller_resume(_ctlr)

#define spi_register_master(_ctlr)	spi_register_controller(_ctlr)
#define devm_spi_register_master(_dev, _ctlr) \
	devm_spi_register_controller(_dev, _ctlr)
#define spi_unregister_master(_ctlr)	spi_unregister_controller(_ctlr)

#endif 
