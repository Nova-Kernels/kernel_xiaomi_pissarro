
#ifndef LINUX_MMC_CORE_H
#define LINUX_MMC_CORE_H

#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/types.h>

struct mmc_data;
struct mmc_request;

enum mmc_blk_status {
	MMC_BLK_SUCCESS = 0,
	MMC_BLK_PARTIAL,
	MMC_BLK_CMD_ERR,
	MMC_BLK_RETRY,
	MMC_BLK_ABORT,
	MMC_BLK_DATA_ERR,
	MMC_BLK_ECC_ERR,
	MMC_BLK_NOMEDIUM,
	MMC_BLK_NEW_REQUEST,
};

struct mmc_command {
	u32			opcode;
	u32			arg;
#define MMC_CMD23_ARG_REL_WR	(1 << 31)
#define MMC_CMD23_ARG_PACKED	((0 << 31) | (1 << 30))
#define MMC_CMD23_ARG_TAG_REQ	(1 << 29)
	u32			resp[4];
	unsigned int		flags;		
#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136	(1 << 1)		
#define MMC_RSP_CRC	(1 << 2)		
#define MMC_RSP_BUSY	(1 << 3)		
#define MMC_RSP_OPCODE	(1 << 4)		

#define MMC_CMD_MASK	(3 << 5)		
#define MMC_CMD_AC	(0 << 5)
#define MMC_CMD_ADTC	(1 << 5)
#define MMC_CMD_BC	(2 << 5)
#define MMC_CMD_BCR	(3 << 5)

#define MMC_RSP_SPI_S1	(1 << 7)		
#define MMC_RSP_SPI_S2	(1 << 8)		
#define MMC_RSP_SPI_B4	(1 << 9)		
#define MMC_RSP_SPI_BUSY (1 << 10)		


#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)


#define MMC_RSP_R1_NO_CRC	(MMC_RSP_PRESENT|MMC_RSP_OPCODE)

#define mmc_resp_type(cmd)	((cmd)->flags & (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE))


#define MMC_RSP_SPI_R1	(MMC_RSP_SPI_S1)
#define MMC_RSP_SPI_R1B	(MMC_RSP_SPI_S1|MMC_RSP_SPI_BUSY)
#define MMC_RSP_SPI_R2	(MMC_RSP_SPI_S1|MMC_RSP_SPI_S2)
#define MMC_RSP_SPI_R3	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_RSP_SPI_R4	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_RSP_SPI_R5	(MMC_RSP_SPI_S1|MMC_RSP_SPI_S2)
#define MMC_RSP_SPI_R7	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)

#define mmc_spi_resp_type(cmd)	((cmd)->flags & \
		(MMC_RSP_SPI_S1|MMC_RSP_SPI_BUSY|MMC_RSP_SPI_S2|MMC_RSP_SPI_B4))


#define mmc_cmd_type(cmd)	((cmd)->flags & MMC_CMD_MASK)

	unsigned int		retries;	
	int			error;		



	unsigned int		busy_timeout;	
	
	bool			sanitize_busy;

	struct mmc_data		*data;		
	struct mmc_request	*mrq;		
};

struct mmc_data {
	unsigned int		timeout_ns;	
	unsigned int		timeout_clks;	
	unsigned int		blksz;		
	unsigned int		blocks;		
	unsigned int		blk_addr;	
	int			error;		
	unsigned int		flags;

#define MMC_DATA_WRITE		BIT(8)
#define MMC_DATA_READ		BIT(9)

#define MMC_DATA_QBR		BIT(10)		
#define MMC_DATA_PRIO		BIT(11)		
#define MMC_DATA_REL_WR		BIT(12)		
#define MMC_DATA_DAT_TAG	BIT(13)		
#define MMC_DATA_FORCED_PRG	BIT(14)		

	unsigned int		bytes_xfered;

	struct mmc_command	*stop;		
	struct mmc_request	*mrq;		

	unsigned int		sg_len;		
	int			sg_count;	
	struct scatterlist	*sg;		
	s32			host_cookie;	
};

struct mmc_host;
struct mmc_request {
	struct mmc_command	*sbc;		
	struct mmc_command	*cmd;
	struct mmc_data		*data;
	struct mmc_command	*stop;

	struct completion	completion;
	struct completion	cmd_completion;
	void			(*done)(struct mmc_request *);
	
	void			(*recovery_notifier)(struct mmc_request *);
	struct mmc_host		*host;
#ifdef CONFIG_MTK_EMMC_CQ_SUPPORT
	struct mmc_async_req	*areq;
	int			flags;
	struct list_head	link;
	struct list_head	hlist;
#endif

	struct request		*req;
#if defined(CONFIG_MTK_HW_FDE) || defined(CONFIG_MMC_CRYPTO)
	bool		is_mmc_req; 
#endif

#ifdef CONFIG_MTK_EMMC_HW_CQ
	struct mmc_cmdq_req *cmdq_req;
#endif

	
	bool			cap_cmd_during_tfr;

	int			tag;
#ifdef CONFIG_MMC_CRYPTO
	int crypto_key_slot;
	u64 data_unit_num;
	const struct blk_crypto_key *crypto_key;
#endif
};

#ifdef CONFIG_MMC_CRYPTO
static inline bool mmc_request_crypto_enabled(const struct mmc_request *mrq)
{
	return mrq->crypto_key != NULL;
}
#else
static inline bool mmc_request_crypto_enabled(const struct mmc_request *mrq)
{
	return false;
}
#endif

struct mmc_card;

void mmc_wait_for_req(struct mmc_host *host, struct mmc_request *mrq);
int mmc_wait_for_cmd(struct mmc_host *host, struct mmc_command *cmd,
		int retries);

int mmc_hw_reset(struct mmc_host *host);
#ifdef CONFIG_MTK_EMMC_HW_CQ
int mmc_cmdq_hw_reset(struct mmc_host *host);
#endif
void mmc_set_data_timeout(struct mmc_data *data, const struct mmc_card *card);

#endif 
