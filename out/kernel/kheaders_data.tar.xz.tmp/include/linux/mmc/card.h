
#ifndef LINUX_MMC_CARD_H
#define LINUX_MMC_CARD_H

#include <linux/device.h>
#include <linux/mod_devicetable.h>

struct mmc_cid {
	unsigned int		manfid;
	char			prod_name[8];
	unsigned char		prv;
	unsigned int		serial;
	unsigned short		oemid;
	unsigned short		year;
	unsigned char		hwrev;
	unsigned char		fwrev;
	unsigned char		month;
};

struct mmc_csd {
	unsigned char		structure;
	unsigned char		mmca_vsn;
	unsigned short		cmdclass;
	unsigned short		taac_clks;
	unsigned int		taac_ns;
	unsigned int		c_size;
	unsigned int		r2w_factor;
	unsigned int		max_dtr;
	unsigned int		erase_size;		
	unsigned int		wp_grp_size;
	unsigned int		read_blkbits;
	unsigned int		write_blkbits;
	unsigned int		capacity;
	unsigned int		read_partial:1,
				read_misalign:1,
				write_partial:1,
				write_misalign:1,
				dsr_imp:1;
};

struct mmc_ext_csd {
	u8			rev;
	u8			erase_group_def;
	u8			sec_feature_support;
	u8			rel_sectors;
	u8			rel_param;
	u8			part_config;
	u8			cache_ctrl;
	u8			rst_n_function;
	u8			max_packed_writes;
	u8			max_packed_reads;
	u8			packed_event_en;
	unsigned int		part_time;		
	unsigned int		sa_timeout;		
	unsigned int		sleep_notification_time; 
	unsigned int		generic_cmd6_time;	
	unsigned int            power_off_longtime;     
	u8			power_off_notification;	
	unsigned int		hs_max_dtr;
	unsigned int		hs200_max_dtr;
#define MMC_HIGH_26_MAX_DTR	26000000
#define MMC_HIGH_52_MAX_DTR	52000000
#define MMC_HIGH_DDR_MAX_DTR	52000000
#define MMC_HS200_MAX_DTR	200000000
	unsigned int		sectors;
	unsigned int		hc_erase_size;		
	unsigned int		hc_erase_timeout;	
	unsigned int		sec_trim_mult;	
	unsigned int		sec_erase_mult;	
	unsigned int		trim_timeout;		
	bool			partition_setting_completed;	
	unsigned long long	enhanced_area_offset;	
	unsigned int		enhanced_area_size;	
	unsigned int		cache_size;		
	bool			hpi_en;			
	bool			hpi;			
	unsigned int		hpi_cmd;		
	bool			bkops;		
	bool			man_bkops_en;	
	bool			auto_bkops_en;	
	bool			auto_bkops;	
	unsigned int            data_sector_size;       
	unsigned int            data_tag_unit_size;     
	unsigned int		boot_ro_lock;		
	bool			boot_ro_lockable;
	bool			ffu_capable;	
	bool			cmdq_en;	
	bool			cmdq_support;	
	unsigned int		cmdq_depth;	
#define MMC_FIRMWARE_LEN 8
	u8			fwrev[MMC_FIRMWARE_LEN];  
	u8			raw_exception_status;	
	u8			raw_partition_support;	
	u8			raw_rpmb_size_mult;	
	u8			boot_wp_status;	        
	u8			raw_erased_mem_count;	
	u8			strobe_support;		
	u8			raw_ext_csd_structure;	
	u8			raw_card_type;		
	u8			raw_driver_strength;	
	u8			out_of_int_time;	
	u8			raw_pwr_cl_52_195;	
	u8			raw_pwr_cl_26_195;	
	u8			raw_pwr_cl_52_360;	
	u8			raw_pwr_cl_26_360;	
	u8			raw_s_a_timeout;	
	u8			raw_hc_erase_gap_size;	
	u8			raw_erase_timeout_mult;	
	u8			raw_hc_erase_grp_size;	
	u8			raw_sec_trim_mult;	
	u8			raw_sec_erase_mult;	
	u8			raw_sec_feature_support;
	u8			raw_trim_mult;		
	u8			raw_pwr_cl_200_195;	
	u8			raw_pwr_cl_200_360;	
	u8			raw_pwr_cl_ddr_52_195;	
	u8			raw_pwr_cl_ddr_52_360;	
	u8			raw_pwr_cl_ddr_200_360;	
	u8			raw_bkops_status;	
	u8			raw_sectors[4];		
	u8			pre_eol_info;		
	u8			device_life_time_est_typ_a;	
	u8			device_life_time_est_typ_b;	

	unsigned int            feature_support;
#define MMC_DISCARD_FEATURE	BIT(0)                  
};

struct sd_scr {
	unsigned char		sda_vsn;
	unsigned char		sda_spec3;
	unsigned char		bus_widths;
#define SD_SCR_BUS_WIDTH_1	(1<<0)
#define SD_SCR_BUS_WIDTH_4	(1<<2)
	unsigned char		cmds;
#define SD_SCR_CMD20_SUPPORT   (1<<0)
#define SD_SCR_CMD23_SUPPORT   (1<<1)
};

struct sd_ssr {
	unsigned int		au;			
	unsigned int		erase_timeout;		
	unsigned int		erase_offset;		
};

struct sd_switch_caps {
	unsigned int		hs_max_dtr;
	unsigned int		uhs_max_dtr;
#define HIGH_SPEED_MAX_DTR	50000000
#define UHS_SDR104_MAX_DTR	208000000
#define UHS_SDR50_MAX_DTR	100000000
#define UHS_DDR50_MAX_DTR	50000000
#define UHS_SDR25_MAX_DTR	UHS_DDR50_MAX_DTR
#define UHS_SDR12_MAX_DTR	25000000
	unsigned int		sd3_bus_mode;
#define UHS_SDR12_BUS_SPEED	0
#define HIGH_SPEED_BUS_SPEED	1
#define UHS_SDR25_BUS_SPEED	1
#define UHS_SDR50_BUS_SPEED	2
#define UHS_SDR104_BUS_SPEED	3
#define UHS_DDR50_BUS_SPEED	4

#define SD_MODE_HIGH_SPEED	(1 << HIGH_SPEED_BUS_SPEED)
#define SD_MODE_UHS_SDR12	(1 << UHS_SDR12_BUS_SPEED)
#define SD_MODE_UHS_SDR25	(1 << UHS_SDR25_BUS_SPEED)
#define SD_MODE_UHS_SDR50	(1 << UHS_SDR50_BUS_SPEED)
#define SD_MODE_UHS_SDR104	(1 << UHS_SDR104_BUS_SPEED)
#define SD_MODE_UHS_DDR50	(1 << UHS_DDR50_BUS_SPEED)
	unsigned int		sd3_drv_type;
#define SD_DRIVER_TYPE_B	0x01
#define SD_DRIVER_TYPE_A	0x02
#define SD_DRIVER_TYPE_C	0x04
#define SD_DRIVER_TYPE_D	0x08
	unsigned int		sd3_curr_limit;
#define SD_SET_CURRENT_LIMIT_200	0
#define SD_SET_CURRENT_LIMIT_400	1
#define SD_SET_CURRENT_LIMIT_600	2
#define SD_SET_CURRENT_LIMIT_800	3
#define SD_SET_CURRENT_NO_CHANGE	(-1)

#define SD_MAX_CURRENT_200	(1 << SD_SET_CURRENT_LIMIT_200)
#define SD_MAX_CURRENT_400	(1 << SD_SET_CURRENT_LIMIT_400)
#define SD_MAX_CURRENT_600	(1 << SD_SET_CURRENT_LIMIT_600)
#define SD_MAX_CURRENT_800	(1 << SD_SET_CURRENT_LIMIT_800)
};

struct sdio_cccr {
	unsigned int		sdio_vsn;
	unsigned int		sd_vsn;
	unsigned int		multi_block:1,
				low_speed:1,
				wide_bus:1,
				high_power:1,
				high_speed:1,
				disable_cd:1,
				sai:1,
				eai:1;
};

struct sdio_cis {
	unsigned short		vendor;
	unsigned short		device;
	unsigned short		blksize;
	unsigned int		max_dtr;
};

struct mmc_host;
struct sdio_func;
struct sdio_func_tuple;
struct mmc_queue_req;

#define SDIO_MAX_FUNCS		7


#define MMC_NUM_BOOT_PARTITION	2
#define MMC_NUM_GP_PARTITION	4
#define MMC_NUM_PHY_PARTITION	7
#define MAX_MMC_PART_NAME_LEN	20


struct mmc_part {
	unsigned int	size;	
	unsigned int	part_cfg;	
	char	name[MAX_MMC_PART_NAME_LEN];
	bool	force_ro;	
	unsigned int	area_type;
#define MMC_BLK_DATA_AREA_MAIN	(1<<0)
#define MMC_BLK_DATA_AREA_BOOT	(1<<1)
#define MMC_BLK_DATA_AREA_GP	(1<<2)
#define MMC_BLK_DATA_AREA_RPMB	(1<<3)
};

#define MMC_QUIRK_CMDQ_DELAY_BEFORE_DCMD 6 


struct mmc_card {
	struct mmc_host		*host;		
	struct device		dev;		
	u32			ocr;		
	unsigned int		rca;		
	unsigned int		type;		
#define MMC_TYPE_MMC		0		
#define MMC_TYPE_SD		1		
#define MMC_TYPE_SDIO		2		
#define MMC_TYPE_SD_COMBO	3		
	unsigned int		state;		
#ifdef CONFIG_MMC_FFU
#define MMC_STATE_FFUED		(1<<22)		
#endif
	unsigned int		quirks; 	
#define MMC_QUIRK_LENIENT_FN0	(1<<0)		
#define MMC_QUIRK_BLKSZ_FOR_BYTE_MODE (1<<1)	
						
#define MMC_QUIRK_NONSTD_SDIO	(1<<2)		
						
#define MMC_QUIRK_NONSTD_FUNC_IF (1<<4)		
#define MMC_QUIRK_DISABLE_CD	(1<<5)		
#define MMC_QUIRK_INAND_CMD38	(1<<6)		
#define MMC_QUIRK_BLK_NO_CMD23	(1<<7)		
#define MMC_QUIRK_BROKEN_BYTE_MODE_512 (1<<8)	
						
#define MMC_QUIRK_LONG_READ_TIME (1<<9)		
#define MMC_QUIRK_SEC_ERASE_TRIM_BROKEN (1<<10)	
#define MMC_QUIRK_BROKEN_IRQ_POLLING	(1<<11)	
#define MMC_QUIRK_TRIM_BROKEN	(1<<12)		
#define MMC_QUIRK_BROKEN_HPI	(1<<13)		
#define MMC_QUIRK_DISABLE_SNO   (1<<22)


#define MMC_QUIRK_CMDQ_EMPTY_BEFORE_DCMD (1 << 17)

	bool			reenable_cmdq;	

	unsigned int		erase_size;	
	unsigned int		erase_shift;	
	unsigned int		pref_erase;	
	unsigned int		eg_boundary;	
	u8			erased_byte;	
	unsigned int		wp_grp_size;    

	u32			raw_cid[4];	
	u32			raw_csd[4];	
	u32			raw_scr[2];	
	u32			raw_ssr[16];	
	struct mmc_cid		cid;		
	struct mmc_csd		csd;		
	struct mmc_ext_csd	ext_csd;	
	struct sd_scr		scr;		
	struct sd_ssr		ssr;		
	struct sd_switch_caps	sw_caps;	

	unsigned int		sdio_funcs;	
	struct sdio_cccr	cccr;		
	struct sdio_cis		cis;		
	struct sdio_func	*sdio_func[SDIO_MAX_FUNCS]; 
	struct sdio_func	*sdio_single_irq; 
	unsigned		num_info;	
	const char		**info;		
	struct sdio_func_tuple	*tuples;	

	unsigned int		sd_bus_speed;	
	unsigned int		mmc_avail_type;	
	unsigned int		drive_strength;	

	struct dentry		*debugfs_root;
	struct mmc_part	part[MMC_NUM_PHY_PARTITION]; 
	unsigned int    nr_parts;
#ifdef CONFIG_MTK_EMMC_HW_CQ
	unsigned int	part_curr;
	bool cqe_init;
#endif

	unsigned int		bouncesz;	
};

static inline bool mmc_large_sector(struct mmc_card *card)
{
	return card->ext_csd.data_sector_size == 4096;
}

bool mmc_card_is_blockaddr(struct mmc_card *card);

#define mmc_card_mmc(c)		((c)->type == MMC_TYPE_MMC)
#define mmc_card_sd(c)		((c)->type == MMC_TYPE_SD)
#define mmc_card_sdio(c)	((c)->type == MMC_TYPE_SDIO)

#endif 
