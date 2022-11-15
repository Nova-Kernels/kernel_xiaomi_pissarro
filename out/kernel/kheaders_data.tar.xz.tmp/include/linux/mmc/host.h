
#ifndef LINUX_MMC_HOST_H
#define LINUX_MMC_HOST_H

#include <linux/sched.h>
#include <linux/device.h>
#include <linux/fault-inject.h>

#include <linux/mmc/core.h>
#include <linux/mmc/card.h>
#include <linux/mmc/pm.h>
#include <linux/dma-direction.h>

struct mmc_ios {
	unsigned int	clock;			
	unsigned short	vdd;



	unsigned char	bus_mode;		

#define MMC_BUSMODE_OPENDRAIN	1
#define MMC_BUSMODE_PUSHPULL	2

	unsigned char	chip_select;		

#define MMC_CS_DONTCARE		0
#define MMC_CS_HIGH		1
#define MMC_CS_LOW		2

	unsigned char	power_mode;		

#define MMC_POWER_OFF		0
#define MMC_POWER_UP		1
#define MMC_POWER_ON		2
#define MMC_POWER_UNDEFINED	3

	unsigned char	bus_width;		

#define MMC_BUS_WIDTH_1		0
#define MMC_BUS_WIDTH_4		2
#define MMC_BUS_WIDTH_8		3

	unsigned char	timing;			

#define MMC_TIMING_LEGACY	0
#define MMC_TIMING_MMC_HS	1
#define MMC_TIMING_SD_HS	2
#define MMC_TIMING_UHS_SDR12	3
#define MMC_TIMING_UHS_SDR25	4
#define MMC_TIMING_UHS_SDR50	5
#define MMC_TIMING_UHS_SDR104	6
#define MMC_TIMING_UHS_DDR50	7
#define MMC_TIMING_MMC_DDR52	8
#define MMC_TIMING_MMC_HS200	9
#define MMC_TIMING_MMC_HS400	10

	unsigned char	signal_voltage;		

#define MMC_SIGNAL_VOLTAGE_330	0
#define MMC_SIGNAL_VOLTAGE_180	1
#define MMC_SIGNAL_VOLTAGE_120	2

	unsigned char	drv_type;		

#define MMC_SET_DRIVER_TYPE_B	0
#define MMC_SET_DRIVER_TYPE_A	1
#define MMC_SET_DRIVER_TYPE_C	2
#define MMC_SET_DRIVER_TYPE_D	3

	bool enhanced_strobe;			
};

#ifdef CONFIG_MTK_EMMC_HW_CQ
struct mmc_cmdq_host_ops {
	int (*init)(struct mmc_host *host);
	int (*enable)(struct mmc_host *host);
	void (*disable)(struct mmc_host *host, bool soft);
	int (*request)(struct mmc_host *host, struct mmc_request *mrq);
	void (*post_req)(struct mmc_host *host, int tag, int err);
	int (*halt)(struct mmc_host *host, bool halt);
	void (*reset)(struct mmc_host *host, bool soft);
	void (*dumpstate)(struct mmc_host *host, bool more);
};
#endif

struct mmc_host;

struct mmc_host_ops {
	
	void	(*post_req)(struct mmc_host *host, struct mmc_request *req,
			    int err);
	void	(*pre_req)(struct mmc_host *host, struct mmc_request *req);
	void	(*request)(struct mmc_host *host, struct mmc_request *req);

	

	
	void	(*set_ios)(struct mmc_host *host, struct mmc_ios *ios);

	
	int	(*get_ro)(struct mmc_host *host);

	
	int	(*get_cd)(struct mmc_host *host);

	void	(*enable_sdio_irq)(struct mmc_host *host, int enable);
	void	(*ack_sdio_irq)(struct mmc_host *host);

	
	void	(*init_card)(struct mmc_host *host, struct mmc_card *card);

	int	(*start_signal_voltage_switch)(struct mmc_host *host, struct mmc_ios *ios);

	
	int	(*card_busy)(struct mmc_host *host);

	
	int	(*execute_tuning)(struct mmc_host *host, u32 opcode);

	
	int	(*prepare_hs400_tuning)(struct mmc_host *host, struct mmc_ios *ios);
	
	void	(*hs400_enhanced_strobe)(struct mmc_host *host,
					 struct mmc_ios *ios);
	int	(*select_drive_strength)(struct mmc_card *card,
					 unsigned int max_dtr, int host_drv,
					 int card_drv, int *drv_type);
	void	(*hw_reset)(struct mmc_host *host);
	void	(*card_event)(struct mmc_host *host);
	void	(*remove_bad_sdcard)(struct mmc_host *host);

	
	int	(*multi_io_quirk)(struct mmc_card *card,
				  unsigned int direction, int blk_size);
};

struct mmc_cqe_ops {
	
	int	(*cqe_enable)(struct mmc_host *host, struct mmc_card *card);
	
	void	(*cqe_disable)(struct mmc_host *host);
	
	int	(*cqe_request)(struct mmc_host *host, struct mmc_request *mrq);
	
	void	(*cqe_post_req)(struct mmc_host *host, struct mmc_request *mrq);
	
	void	(*cqe_off)(struct mmc_host *host);
	
	int	(*cqe_wait_for_idle)(struct mmc_host *host);
	
	bool	(*cqe_timeout)(struct mmc_host *host, struct mmc_request *mrq,
			       bool *recovery_needed);
	
	void	(*cqe_recovery_start)(struct mmc_host *host);
	
	void	(*cqe_recovery_finish)(struct mmc_host *host);
};

#ifdef CONFIG_MTK_EMMC_HW_CQ
struct mmc_cmdq_req {
	unsigned int cmd_flags;
	u32 blk_addr;
	
	struct mmc_request	mrq;
	struct mmc_data		data;
	struct mmc_command	cmd;
#define DCMD		(1 << 0)
#define QBR		(1 << 1)
#define DIR		(1 << 2)
#define PRIO		(1 << 3)
#define REL_WR		(1 << 4)
#define DAT_TAG	(1 << 5)
#define FORCED_PRG	(1 << 6)
	unsigned int		cmdq_req_flags;

	unsigned int		resp_idx;
	unsigned int		resp_arg;
	unsigned int		dev_pend_tasks;
	bool			resp_err;
	bool			skip_err_handling;
	bool			skip_dump; 
	bool			skip_reset; 
	int			tag; 
	u8			ctx_id;
};
#endif

struct keyslot_mgmt_ll_ops;
struct mmc_crypto_variant_ops {
	void (*setup_rq_keyslot_manager)(struct mmc_host *host,
					 struct request_queue *q);
	void (*destroy_rq_keyslot_manager)(struct mmc_host *host,
					   struct request_queue *q);
	int (*init_crypto)(struct mmc_host *host,
			       const struct keyslot_mgmt_ll_ops *ksm_ops);
	void (*enable)(struct mmc_host *host);
	void (*disable)(struct mmc_host *host);
	int (*suspend)(struct mmc_host *host);
	int (*resume)(struct mmc_host *host);
	int (*debug)(struct mmc_host *host);
	void (*host_init_crypto)(struct mmc_host *host);
	int (*get_crypto_capabilities)(struct mmc_host *host);
	int (*prepare_mqr_crypto)(struct mmc_host *host,
		u64 data_unit_num, int ddir, int slot);
	void (*host_program_key)(struct mmc_host *host,
			u32 *key, u32 *tkey, u32 config);
	int (*complete_mqr_crypto)(struct mmc_host *host);
	void (*msdc_crypto_keyslot_evict)(struct mmc_host *host);
	void *priv;
};

struct mmc_async_req {
	
	struct mmc_request	*mrq;
#ifdef CONFIG_MTK_EMMC_CQ_SUPPORT
	struct mmc_request	*mrq_que;
#endif
	
	enum mmc_blk_status (*err_check)(struct mmc_card *, struct mmc_async_req *);
#ifdef CONFIG_MTK_EMMC_CQ_SUPPORT
#define MMC_QUEUE_BEFORE_ENQ	(0)	
#define MMC_QUEUE_ENQ		(1)	
#define MMC_QUEUE_BEFORE_QRDY	(2)	
	
#define MMC_QUEUE_BEFORE_TRAN	(3)
#define MMC_QUEUE_TRAN		(4)	
	
#define MMC_QUEUE_BUSY		(5)
	
#define MMC_QUEUE_BEFORE_POST	(7)
	unsigned long		state;
	unsigned int		prio;
#endif

};


struct mmc_slot {
	int cd_irq;
	bool cd_wake_enabled;
	void *handler_priv;
};

#ifdef CONFIG_MTK_EMMC_HW_CQ

struct mmc_cmdq_context_info {
	unsigned long	active_reqs; 
	unsigned long	data_active_reqs; 
	unsigned long	curr_state;
#define	CMDQ_STATE_ERR 0
#define	CMDQ_STATE_DCMD_ACTIVE 1
#define	CMDQ_STATE_HALT 2
#define	CMDQ_STATE_CQ_DISABLE 3
#define	CMDQ_STATE_REQ_TIMED_OUT 4
	wait_queue_head_t	queue_empty_wq;
	wait_queue_head_t	wait;
	int active_small_sector_read_reqs;
	struct rw_semaphore err_rwsem;
};
#endif


struct mmc_context_info {
	bool			is_done_rcv;
	bool			is_new_req;
	bool			is_waiting_last_req;
	wait_queue_head_t	wait;
};

#ifdef CONFIG_MTK_EMMC_CQ_SUPPORT
#define EMMC_MAX_QUEUE_DEPTH		(32)
#define EMMC_MIN_RT_CLASS_TAG_COUNT	(4)
#endif

struct regulator;
struct mmc_pwrseq;

struct mmc_supply {
	struct regulator *vmmc;		
	struct regulator *vqmmc;	
};


union swcqhci_crypto_capabilities {
	__le32 reg_val;
	struct {
		u8 num_crypto_cap;
		u8 config_count;
		u8 reserved;
		u8 config_array_ptr;
	};
};

enum swcqhci_crypto_key_size {
	SWCQHCI_CRYPTO_KEY_SIZE_128		= 0,
	SWCQHCI_CRYPTO_KEY_SIZE_192		= 1,
	SWCQHCI_CRYPTO_KEY_SIZE_256		= 2,
	SWCQHCI_CRYPTO_KEY_SIZE_512		= 3,
	SWCQHCI_CRYPTO_KEY_SIZE_INVALID	= 4,
};

enum swcqhci_crypto_alg {
	SWCQHCI_CRYPTO_ALG_AES_XTS				= 4,
	SWCQHCI_CRYPTO_ALG_BITLOCKER_AES_CBC	= 3,
	SWCQHCI_CRYPTO_ALG_AES_ECB				= 2,
	SWCQHCI_CRYPTO_ALG_ESSIV_AES_CBC		= 1,
	SWCQHCI_CRYPTO_ALG_INVALID				= 0,
};


union swcqhci_crypto_cap_entry {
	__le32 reg_val;
	struct {
		u8 algorithm_id;
		u8 sdus_mask; 
		u8 key_size;
		u8 reserved;
	};
};


#define MMC_CRYPTO_CONFIGURATION_ENABLE (1 << 7)
#define MMC_CRYPTO_KEY_MAX_SIZE 64


union swcqhci_crypto_cfg_entry {
	__le32 reg_val[32];
	struct {
		u8 crypto_key[MMC_CRYPTO_KEY_MAX_SIZE];
		
		u8 data_unit_size;
		u8 crypto_cap_idx;
		u8 reserved_1;
		u8 config_enable;
		u8 reserved_multi_host;
		u8 reserved_2;
		u8 vsb[2];
		u8 reserved_3[56];
	};
};

struct mmc_host {
	struct device		*parent;
	struct device		class_dev;
	int			index;
	const struct mmc_host_ops *ops;
	struct mmc_pwrseq	*pwrseq;
	unsigned int		f_min;
	unsigned int		f_max;
	unsigned int		f_init;
	u32			ocr_avail;
	u32			ocr_avail_sdio;	
	u32			ocr_avail_sd;	
	u32			ocr_avail_mmc;	
#ifdef CONFIG_PM_SLEEP
	struct notifier_block	pm_notify;
#endif
	u32			max_current_330;
	u32			max_current_300;
	u32			max_current_180;

#define MMC_VDD_165_195		0x00000080	
#define MMC_VDD_20_21		0x00000100	
#define MMC_VDD_21_22		0x00000200	
#define MMC_VDD_22_23		0x00000400	
#define MMC_VDD_23_24		0x00000800	
#define MMC_VDD_24_25		0x00001000	
#define MMC_VDD_25_26		0x00002000	
#define MMC_VDD_26_27		0x00004000	
#define MMC_VDD_27_28		0x00008000	
#define MMC_VDD_28_29		0x00010000	
#define MMC_VDD_29_30		0x00020000	
#define MMC_VDD_30_31		0x00040000	
#define MMC_VDD_31_32		0x00080000	
#define MMC_VDD_32_33		0x00100000	
#define MMC_VDD_33_34		0x00200000	
#define MMC_VDD_34_35		0x00400000	
#define MMC_VDD_35_36		0x00800000	

	u32			caps;		

#define MMC_CAP_4_BIT_DATA	(1 << 0)	
#define MMC_CAP_MMC_HIGHSPEED	(1 << 1)	
#define MMC_CAP_SD_HIGHSPEED	(1 << 2)	
#define MMC_CAP_SDIO_IRQ	(1 << 3)	
#define MMC_CAP_SPI		(1 << 4)	
#define MMC_CAP_NEEDS_POLL	(1 << 5)	
#define MMC_CAP_8_BIT_DATA	(1 << 6)	
#define MMC_CAP_AGGRESSIVE_PM	(1 << 7)	
#define MMC_CAP_NONREMOVABLE	(1 << 8)	
#define MMC_CAP_WAIT_WHILE_BUSY	(1 << 9)	
#define MMC_CAP_ERASE		(1 << 10)	
#define MMC_CAP_3_3V_DDR	(1 << 11)	
#define MMC_CAP_1_8V_DDR	(1 << 12)	
#define MMC_CAP_1_2V_DDR	(1 << 13)	
#define MMC_CAP_POWER_OFF_CARD	(1 << 14)	
#define MMC_CAP_BUS_WIDTH_TEST	(1 << 15)	
#define MMC_CAP_UHS_SDR12	(1 << 16)	
#define MMC_CAP_UHS_SDR25	(1 << 17)	
#define MMC_CAP_UHS_SDR50	(1 << 18)	
#define MMC_CAP_UHS_SDR104	(1 << 19)	
#define MMC_CAP_UHS_DDR50	(1 << 20)	

#define MMC_CAP_DRIVER_TYPE_A	(1 << 23)	
#define MMC_CAP_DRIVER_TYPE_C	(1 << 24)	
#define MMC_CAP_DRIVER_TYPE_D	(1 << 25)	
#define MMC_CAP_CD_WAKE		(1 << 28)	
#define MMC_CAP_CMD_DURING_TFR	(1 << 29)	
#define MMC_CAP_CMD23		(1 << 30)	
#define MMC_CAP_HW_RESET	(1 << 31)	

	u32			caps2;		

#define MMC_CAP2_BOOTPART_NOACC	(1 << 0)	
#define MMC_CAP2_FULL_PWR_CYCLE	(1 << 2)	
#define MMC_CAP2_HS200_1_8V_SDR	(1 << 5)        
#define MMC_CAP2_HS200_1_2V_SDR	(1 << 6)        
#define MMC_CAP2_HS200		(MMC_CAP2_HS200_1_8V_SDR | \
				 MMC_CAP2_HS200_1_2V_SDR)
#define MMC_CAP2_CD_ACTIVE_HIGH	(1 << 10)	
#define MMC_CAP2_RO_ACTIVE_HIGH	(1 << 11)	
#define MMC_CAP2_NO_PRESCAN_POWERUP (1 << 14)	
#define MMC_CAP2_HS400_1_8V	(1 << 15)	
#define MMC_CAP2_HS400_1_2V	(1 << 16)	
#define MMC_CAP2_HS400		(MMC_CAP2_HS400_1_8V | \
				 MMC_CAP2_HS400_1_2V)
#define MMC_CAP2_HSX00_1_2V	(MMC_CAP2_HS200_1_2V_SDR | MMC_CAP2_HS400_1_2V)
#define MMC_CAP2_SDIO_IRQ_NOTHREAD (1 << 17)
#define MMC_CAP2_NO_WRITE_PROTECT (1 << 18)	
#define MMC_CAP2_NO_SDIO	(1 << 19)	
#define MMC_CAP2_HS400_ES	(1 << 20)	
#define MMC_CAP2_NO_SD		(1 << 21)	
#define MMC_CAP2_NO_MMC		(1 << 22)	
#define MMC_CAP2_CQE		(1 << 23)	
#define MMC_CAP2_CQE_DCMD	(1 << 24)	
#define MMC_CAP2_NMCARD		(1 << 26)
#define MMC_CAP2_CRYPTO		(1 << 27)	

	mmc_pm_flag_t		pm_caps;	

	
	unsigned int		max_seg_size;	
	unsigned short		max_segs;	
	unsigned short		unused;
	unsigned int		max_req_size;	
	unsigned int		max_blk_size;	
	unsigned int		max_blk_count;	
	unsigned int		max_busy_timeout; 

	
	spinlock_t		lock;		

	struct mmc_ios		ios;		
	struct mmc_ios		cached_ios;

	
	unsigned int		use_spi_crc:1;
	unsigned int		claimed:1;	
	unsigned int		bus_dead:1;	
	unsigned int		can_retune:1;	
	unsigned int		doing_retune:1;	
	unsigned int		retune_now:1;	
	unsigned int		retune_paused:1; 

	int			rescan_disable;	
	int			rescan_entered;	

	int			need_retune;	
	int			hold_retune;	
	unsigned int		retune_period;	
	struct timer_list	retune_timer;	

	bool			trigger_card_event; 

	struct mmc_card		*card;		

	wait_queue_head_t	wq;
	struct task_struct	*claimer;	
	int			claim_cnt;	

	struct delayed_work	detect;
	int			detect_change;	
	struct mmc_slot		slot;

	const struct mmc_bus_ops *bus_ops;	
	unsigned int		bus_refs;	

	unsigned int		sdio_irqs;
	struct task_struct	*sdio_irq_thread;
	struct delayed_work	sdio_irq_work;
	bool			sdio_irq_pending;
	atomic_t		sdio_irq_thread_abort;

	mmc_pm_flag_t		pm_flags;	

	struct led_trigger	*led;		

#ifdef CONFIG_REGULATOR
	bool			regulator_enabled; 
#endif
	struct mmc_supply	supply;

	struct dentry		*debugfs_root;

	struct mmc_async_req	*areq;		
	struct mmc_context_info	context_info;	

	
	struct mmc_request	*ongoing_mrq;

#ifdef CONFIG_MTK_EMMC_CQ_SUPPORT
	struct mmc_async_req	*areq_que[EMMC_MAX_QUEUE_DEPTH];
	struct mmc_async_req	*areq_cur;
	atomic_t		areq_cnt;

	spinlock_t		cmd_que_lock;
	spinlock_t		dat_que_lock;
	spinlock_t		que_lock;
	struct list_head	cmd_que;
	struct list_head	dat_que;

	unsigned long		state;
	wait_queue_head_t	cmp_que;
	wait_queue_head_t	cmdq_que;
	struct mmc_request	*done_mrq;
	struct mmc_command	chk_cmd;
	struct mmc_request	chk_mrq;
	struct mmc_command	que_cmd;
	struct mmc_request	que_mrq;
	struct mmc_command	deq_cmd;
	struct mmc_request	deq_mrq;

	struct mmc_queue_req	*mqrq_cur;
	struct mmc_queue_req	*mqrq_prev;
	struct mmc_request	*prev_mrq;

	struct task_struct	*cmdq_thread;
	atomic_t		cq_rw;
	atomic_t		cq_w;
	unsigned int		wp_error;
	atomic_t		cq_wait_rdy;
	atomic_t		cq_rdy_cnt;
	unsigned long		task_id_index;
	int			cur_rw_task;
#define CQ_TASK_IDLE 99

	atomic_t		is_data_dma;
	atomic_t		cq_tuning_now;
#ifdef CONFIG_MMC_FFU
	atomic_t		stop_queue;
#endif
	unsigned int		data_mrq_queued[32];
	unsigned int		cmdq_support_changed;
	int			align_size;
#endif

#ifdef CONFIG_FAIL_MMC_REQUEST
	struct fault_attr	fail_mmc_request;
#endif

	unsigned int		actual_clock;	

	unsigned int		slotno;	

	int			dsr_req;	
	u32			dsr;	

	
	const struct mmc_cqe_ops *cqe_ops;
	void			*cqe_private;
	int			cqe_qdepth;
	bool			cqe_enabled;
	bool			cqe_on;

#ifdef CONFIG_MTK_EMMC_HW_CQ
	const struct mmc_cmdq_host_ops *cmdq_ops;
	struct mmc_cmdq_context_info	cmdq_ctx;
	int num_cq_slots;
	int dcmd_cq_slot;
	bool			cmdq_thist_enabled;
	
	void *cmdq_private;
	struct mmc_request	*err_mrq;
#endif

#ifdef CONFIG_MMC_CRYPTO
	
	const struct mmc_crypto_variant_ops *crypto_vops;
	union swcqhci_crypto_capabilities crypto_capabilities;
	union swcqhci_crypto_cap_entry *crypto_cap_array;
	u8 crypto_cfg_register;
	union swcqhci_crypto_cfg_entry *crypto_cfgs;
	struct keyslot_manager *ksm;
#endif 


#ifdef CONFIG_MMC_EMBEDDED_SDIO
	struct {
		struct sdio_cis			*cis;
		struct sdio_cccr		*cccr;
		struct sdio_embedded_func	*funcs;
		int				num_funcs;
	} embedded_sdio_data;
#endif
	unsigned long		private[0] ____cacheline_aligned;
};

struct device_node;

struct mmc_host *mmc_alloc_host(int extra, struct device *);
int mmc_add_host(struct mmc_host *);
void mmc_remove_host(struct mmc_host *);
void mmc_free_host(struct mmc_host *);
int mmc_of_parse(struct mmc_host *host);
int mmc_of_parse_voltage(struct device_node *np, u32 *mask);

#ifdef CONFIG_MMC_EMBEDDED_SDIO
extern void mmc_set_embedded_sdio_data(struct mmc_host *host,
				       struct sdio_cis *cis,
				       struct sdio_cccr *cccr,
				       struct sdio_embedded_func *funcs,
				       int num_funcs);
#endif

static inline void *mmc_priv(struct mmc_host *host)
{
	return (void *)host->private;
}

#ifdef CONFIG_MTK_EMMC_HW_CQ
static inline void *mmc_cmdq_private(struct mmc_host *host)
{
	return host->cmdq_private;
}
#endif

#define mmc_host_is_spi(host)	((host)->caps & MMC_CAP_SPI)

#define mmc_dev(x)	((x)->parent)
#define mmc_classdev(x)	(&(x)->class_dev)
#define mmc_hostname(x)	(dev_name(&(x)->class_dev))

int mmc_power_save_host(struct mmc_host *host);
int mmc_power_restore_host(struct mmc_host *host);

void mmc_detect_change(struct mmc_host *, unsigned long delay);
void mmc_request_done(struct mmc_host *, struct mmc_request *);
void mmc_command_done(struct mmc_host *host, struct mmc_request *mrq);

#ifdef CONFIG_MTK_EMMC_CQ_SUPPORT
int mmc_blk_end_queued_req(struct mmc_host *host,
	struct mmc_async_req *areq_active, int index, int status);

extern int current_mmc_part_type;
extern int emmc_resetting_when_cmdq;
#endif

static inline void mmc_signal_sdio_irq(struct mmc_host *host)
{
	host->ops->enable_sdio_irq(host, 0);
	host->sdio_irq_pending = true;
	if (host->sdio_irq_thread)
		wake_up_process(host->sdio_irq_thread);
}

void sdio_run_irqs(struct mmc_host *host);
void sdio_signal_irq(struct mmc_host *host);

#ifdef CONFIG_REGULATOR
int mmc_regulator_get_ocrmask(struct regulator *supply);
int mmc_regulator_set_ocr(struct mmc_host *mmc,
			struct regulator *supply,
			unsigned short vdd_bit);
int mmc_regulator_set_vqmmc(struct mmc_host *mmc, struct mmc_ios *ios);
#else
static inline int mmc_regulator_get_ocrmask(struct regulator *supply)
{
	return 0;
}

static inline int mmc_regulator_set_ocr(struct mmc_host *mmc,
				 struct regulator *supply,
				 unsigned short vdd_bit)
{
	return 0;
}

static inline int mmc_regulator_set_vqmmc(struct mmc_host *mmc,
					  struct mmc_ios *ios)
{
	return -EINVAL;
}
#endif

u32 mmc_vddrange_to_ocrmask(int vdd_min, int vdd_max);
int mmc_regulator_get_supply(struct mmc_host *mmc);

static inline int mmc_card_is_removable(struct mmc_host *host)
{
	return !(host->caps & MMC_CAP_NONREMOVABLE);
}

static inline int mmc_card_keep_power(struct mmc_host *host)
{
	return host->pm_flags & MMC_PM_KEEP_POWER;
}

static inline int mmc_card_wake_sdio_irq(struct mmc_host *host)
{
	return host->pm_flags & MMC_PM_WAKE_SDIO_IRQ;
}

#ifdef CONFIG_MTK_EMMC_HW_CQ
static inline void mmc_host_set_halt(struct mmc_host *host)
{
	set_bit(CMDQ_STATE_HALT, &host->cmdq_ctx.curr_state);
}

static inline void mmc_host_clr_halt(struct mmc_host *host)
{
	clear_bit(CMDQ_STATE_HALT, &host->cmdq_ctx.curr_state);
}

static inline int mmc_host_halt(struct mmc_host *host)
{
	return test_bit(CMDQ_STATE_HALT, &host->cmdq_ctx.curr_state);
}

static inline void mmc_host_set_cq_disable(struct mmc_host *host)
{
	set_bit(CMDQ_STATE_CQ_DISABLE, &host->cmdq_ctx.curr_state);
}

static inline void mmc_host_clr_cq_disable(struct mmc_host *host)
{
	clear_bit(CMDQ_STATE_CQ_DISABLE, &host->cmdq_ctx.curr_state);
}

static inline int mmc_host_cq_disable(struct mmc_host *host)
{
	return test_bit(CMDQ_STATE_CQ_DISABLE, &host->cmdq_ctx.curr_state);
}
#endif


static inline int mmc_card_hs(struct mmc_card *card)
{
	return card->host->ios.timing == MMC_TIMING_SD_HS ||
		card->host->ios.timing == MMC_TIMING_MMC_HS;
}


static inline int mmc_card_uhs(struct mmc_card *card)
{
	return card->host->ios.timing >= MMC_TIMING_UHS_SDR12 &&
		card->host->ios.timing <= MMC_TIMING_UHS_DDR50;
}

void mmc_retune_timer_stop(struct mmc_host *host);

static inline void mmc_retune_needed(struct mmc_host *host)
{
	if (host->can_retune)
		host->need_retune = 1;
}

static inline bool mmc_can_retune(struct mmc_host *host)
{
	return host->can_retune == 1;
}

static inline enum dma_data_direction mmc_get_dma_dir(struct mmc_data *data)
{
	return data->flags & MMC_DATA_WRITE ? DMA_TO_DEVICE : DMA_FROM_DEVICE;
}

int mmc_send_tuning(struct mmc_host *host, u32 opcode, int *cmd_error);
int mmc_abort_tuning(struct mmc_host *host, u32 opcode);

#endif 
