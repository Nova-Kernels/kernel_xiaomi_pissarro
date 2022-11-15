

#ifndef __LINUX_MTD_SPI_NOR_H
#define __LINUX_MTD_SPI_NOR_H

#include <linux/bitops.h>
#include <linux/mtd/cfi.h>
#include <linux/mtd/mtd.h>


#define SNOR_MFR_ATMEL		CFI_MFR_ATMEL
#define SNOR_MFR_GIGADEVICE	0xc8
#define SNOR_MFR_INTEL		CFI_MFR_INTEL
#define SNOR_MFR_MICRON		CFI_MFR_ST 
#define SNOR_MFR_MACRONIX	CFI_MFR_MACRONIX
#define SNOR_MFR_SPANSION	CFI_MFR_AMD
#define SNOR_MFR_SST		CFI_MFR_SST
#define SNOR_MFR_WINBOND	0xef 




#define SPINOR_OP_WREN		0x06	
#define SPINOR_OP_RDSR		0x05	
#define SPINOR_OP_WRSR		0x01	
#define SPINOR_OP_RDSR2		0x3f	
#define SPINOR_OP_WRSR2		0x3e	
#define SPINOR_OP_READ		0x03	
#define SPINOR_OP_READ_FAST	0x0b	
#define SPINOR_OP_READ_1_1_2	0x3b	
#define SPINOR_OP_READ_1_2_2	0xbb	
#define SPINOR_OP_READ_1_1_4	0x6b	
#define SPINOR_OP_READ_1_4_4	0xeb	
#define SPINOR_OP_PP		0x02	
#define SPINOR_OP_PP_1_1_4	0x32	
#define SPINOR_OP_PP_1_4_4	0x38	
#define SPINOR_OP_BE_4K		0x20	
#define SPINOR_OP_BE_4K_PMC	0xd7	
#define SPINOR_OP_BE_32K	0x52	
#define SPINOR_OP_CHIP_ERASE	0xc7	
#define SPINOR_OP_SE		0xd8	
#define SPINOR_OP_RDID		0x9f	
#define SPINOR_OP_RDSFDP	0x5a	
#define SPINOR_OP_RDCR		0x35	
#define SPINOR_OP_RDFSR		0x70	


#define SPINOR_OP_READ_4B	0x13	
#define SPINOR_OP_READ_FAST_4B	0x0c	
#define SPINOR_OP_READ_1_1_2_4B	0x3c	
#define SPINOR_OP_READ_1_2_2_4B	0xbc	
#define SPINOR_OP_READ_1_1_4_4B	0x6c	
#define SPINOR_OP_READ_1_4_4_4B	0xec	
#define SPINOR_OP_PP_4B		0x12	
#define SPINOR_OP_PP_1_1_4_4B	0x34	
#define SPINOR_OP_PP_1_4_4_4B	0x3e	
#define SPINOR_OP_BE_4K_4B	0x21	
#define SPINOR_OP_BE_32K_4B	0x5c	
#define SPINOR_OP_SE_4B		0xdc	


#define SPINOR_OP_READ_1_1_1_DTR	0x0d
#define SPINOR_OP_READ_1_2_2_DTR	0xbd
#define SPINOR_OP_READ_1_4_4_DTR	0xed

#define SPINOR_OP_READ_1_1_1_DTR_4B	0x0e
#define SPINOR_OP_READ_1_2_2_DTR_4B	0xbe
#define SPINOR_OP_READ_1_4_4_DTR_4B	0xee


#define SPINOR_OP_BP		0x02	
#define SPINOR_OP_WRDI		0x04	
#define SPINOR_OP_AAI_WP	0xad	


#define SPINOR_OP_XSE		0x50	
#define SPINOR_OP_XPP		0x82	
#define SPINOR_OP_XRDSR		0xd7	

#define XSR_PAGESIZE		BIT(0)	
#define XSR_RDY			BIT(7)	



#define SPINOR_OP_EN4B		0xb7	
#define SPINOR_OP_EX4B		0xe9	


#define SPINOR_OP_BRWR		0x17	
#define SPINOR_OP_CLSR		0x30	


#define SPINOR_OP_RD_EVCR      0x65    
#define SPINOR_OP_WD_EVCR      0x61    


#define SR_WIP			BIT(0)	
#define SR_WEL			BIT(1)	

#define SR_BP0			BIT(2)	
#define SR_BP1			BIT(3)	
#define SR_BP2			BIT(4)	
#define SR_TB			BIT(5)	
#define SR_SRWD			BIT(7)	

#define SR_E_ERR		BIT(5)
#define SR_P_ERR		BIT(6)

#define SR_QUAD_EN_MX		BIT(6)	


#define EVCR_QUAD_EN_MICRON	BIT(7)	


#define FSR_READY		BIT(7)


#define CR_QUAD_EN_SPAN		BIT(1)	


#define SR2_QUAD_EN_BIT7	BIT(7)


#define SNOR_PROTO_INST_MASK	GENMASK(23, 16)
#define SNOR_PROTO_INST_SHIFT	16
#define SNOR_PROTO_INST(_nbits)	\
	((((unsigned long)(_nbits)) << SNOR_PROTO_INST_SHIFT) & \
	 SNOR_PROTO_INST_MASK)

#define SNOR_PROTO_ADDR_MASK	GENMASK(15, 8)
#define SNOR_PROTO_ADDR_SHIFT	8
#define SNOR_PROTO_ADDR(_nbits)	\
	((((unsigned long)(_nbits)) << SNOR_PROTO_ADDR_SHIFT) & \
	 SNOR_PROTO_ADDR_MASK)

#define SNOR_PROTO_DATA_MASK	GENMASK(7, 0)
#define SNOR_PROTO_DATA_SHIFT	0
#define SNOR_PROTO_DATA(_nbits)	\
	((((unsigned long)(_nbits)) << SNOR_PROTO_DATA_SHIFT) & \
	 SNOR_PROTO_DATA_MASK)

#define SNOR_PROTO_IS_DTR	BIT(24)	

#define SNOR_PROTO_STR(_inst_nbits, _addr_nbits, _data_nbits)	\
	(SNOR_PROTO_INST(_inst_nbits) |				\
	 SNOR_PROTO_ADDR(_addr_nbits) |				\
	 SNOR_PROTO_DATA(_data_nbits))
#define SNOR_PROTO_DTR(_inst_nbits, _addr_nbits, _data_nbits)	\
	(SNOR_PROTO_IS_DTR |					\
	 SNOR_PROTO_STR(_inst_nbits, _addr_nbits, _data_nbits))

enum spi_nor_protocol {
	SNOR_PROTO_1_1_1 = SNOR_PROTO_STR(1, 1, 1),
	SNOR_PROTO_1_1_2 = SNOR_PROTO_STR(1, 1, 2),
	SNOR_PROTO_1_1_4 = SNOR_PROTO_STR(1, 1, 4),
	SNOR_PROTO_1_1_8 = SNOR_PROTO_STR(1, 1, 8),
	SNOR_PROTO_1_2_2 = SNOR_PROTO_STR(1, 2, 2),
	SNOR_PROTO_1_4_4 = SNOR_PROTO_STR(1, 4, 4),
	SNOR_PROTO_1_8_8 = SNOR_PROTO_STR(1, 8, 8),
	SNOR_PROTO_2_2_2 = SNOR_PROTO_STR(2, 2, 2),
	SNOR_PROTO_4_4_4 = SNOR_PROTO_STR(4, 4, 4),
	SNOR_PROTO_8_8_8 = SNOR_PROTO_STR(8, 8, 8),

	SNOR_PROTO_1_1_1_DTR = SNOR_PROTO_DTR(1, 1, 1),
	SNOR_PROTO_1_2_2_DTR = SNOR_PROTO_DTR(1, 2, 2),
	SNOR_PROTO_1_4_4_DTR = SNOR_PROTO_DTR(1, 4, 4),
	SNOR_PROTO_1_8_8_DTR = SNOR_PROTO_DTR(1, 8, 8),
};

static inline bool spi_nor_protocol_is_dtr(enum spi_nor_protocol proto)
{
	return !!(proto & SNOR_PROTO_IS_DTR);
}

static inline u8 spi_nor_get_protocol_inst_nbits(enum spi_nor_protocol proto)
{
	return ((unsigned long)(proto & SNOR_PROTO_INST_MASK)) >>
		SNOR_PROTO_INST_SHIFT;
}

static inline u8 spi_nor_get_protocol_addr_nbits(enum spi_nor_protocol proto)
{
	return ((unsigned long)(proto & SNOR_PROTO_ADDR_MASK)) >>
		SNOR_PROTO_ADDR_SHIFT;
}

static inline u8 spi_nor_get_protocol_data_nbits(enum spi_nor_protocol proto)
{
	return ((unsigned long)(proto & SNOR_PROTO_DATA_MASK)) >>
		SNOR_PROTO_DATA_SHIFT;
}

static inline u8 spi_nor_get_protocol_width(enum spi_nor_protocol proto)
{
	return spi_nor_get_protocol_data_nbits(proto);
}

#define SPI_NOR_MAX_CMD_SIZE	8
enum spi_nor_ops {
	SPI_NOR_OPS_READ = 0,
	SPI_NOR_OPS_WRITE,
	SPI_NOR_OPS_ERASE,
	SPI_NOR_OPS_LOCK,
	SPI_NOR_OPS_UNLOCK,
};

enum spi_nor_option_flags {
	SNOR_F_USE_FSR		= BIT(0),
	SNOR_F_HAS_SR_TB	= BIT(1),
	SNOR_F_NO_OP_CHIP_ERASE	= BIT(2),
	SNOR_F_S3AN_ADDR_DEFAULT = BIT(3),
	SNOR_F_READY_XSR_RDY	= BIT(4),
	SNOR_F_USE_CLSR		= BIT(5),
};


struct spi_nor {
	struct mtd_info		mtd;
	struct mutex		lock;
	struct device		*dev;
	u32			page_size;
	u8			addr_width;
	u8			erase_opcode;
	u8			read_opcode;
	u8			read_dummy;
	u8			program_opcode;
	enum spi_nor_protocol	read_proto;
	enum spi_nor_protocol	write_proto;
	enum spi_nor_protocol	reg_proto;
	bool			sst_write_second;
	u32			flags;
	u8			cmd_buf[SPI_NOR_MAX_CMD_SIZE];

	int (*prepare)(struct spi_nor *nor, enum spi_nor_ops ops);
	void (*unprepare)(struct spi_nor *nor, enum spi_nor_ops ops);
	int (*read_reg)(struct spi_nor *nor, u8 opcode, u8 *buf, int len);
	int (*write_reg)(struct spi_nor *nor, u8 opcode, u8 *buf, int len);

	ssize_t (*read)(struct spi_nor *nor, loff_t from,
			size_t len, u_char *read_buf);
	ssize_t (*write)(struct spi_nor *nor, loff_t to,
			size_t len, const u_char *write_buf);
	int (*erase)(struct spi_nor *nor, loff_t offs);

	int (*flash_lock)(struct spi_nor *nor, loff_t ofs, uint64_t len);
	int (*flash_unlock)(struct spi_nor *nor, loff_t ofs, uint64_t len);
	int (*flash_is_locked)(struct spi_nor *nor, loff_t ofs, uint64_t len);

	void *priv;
};

static inline void spi_nor_set_flash_node(struct spi_nor *nor,
					  struct device_node *np)
{
	mtd_set_of_node(&nor->mtd, np);
}

static inline struct device_node *spi_nor_get_flash_node(struct spi_nor *nor)
{
	return mtd_get_of_node(&nor->mtd);
}


struct spi_nor_hwcaps {
	u32	mask;
};


#define SNOR_HWCAPS_READ_MASK		GENMASK(14, 0)
#define SNOR_HWCAPS_READ		BIT(0)
#define SNOR_HWCAPS_READ_FAST		BIT(1)
#define SNOR_HWCAPS_READ_1_1_1_DTR	BIT(2)

#define SNOR_HWCAPS_READ_DUAL		GENMASK(6, 3)
#define SNOR_HWCAPS_READ_1_1_2		BIT(3)
#define SNOR_HWCAPS_READ_1_2_2		BIT(4)
#define SNOR_HWCAPS_READ_2_2_2		BIT(5)
#define SNOR_HWCAPS_READ_1_2_2_DTR	BIT(6)

#define SNOR_HWCAPS_READ_QUAD		GENMASK(10, 7)
#define SNOR_HWCAPS_READ_1_1_4		BIT(7)
#define SNOR_HWCAPS_READ_1_4_4		BIT(8)
#define SNOR_HWCAPS_READ_4_4_4		BIT(9)
#define SNOR_HWCAPS_READ_1_4_4_DTR	BIT(10)

#define SNOR_HWCPAS_READ_OCTO		GENMASK(14, 11)
#define SNOR_HWCAPS_READ_1_1_8		BIT(11)
#define SNOR_HWCAPS_READ_1_8_8		BIT(12)
#define SNOR_HWCAPS_READ_8_8_8		BIT(13)
#define SNOR_HWCAPS_READ_1_8_8_DTR	BIT(14)


#define SNOR_HWCAPS_PP_MASK	GENMASK(22, 16)
#define SNOR_HWCAPS_PP		BIT(16)

#define SNOR_HWCAPS_PP_QUAD	GENMASK(19, 17)
#define SNOR_HWCAPS_PP_1_1_4	BIT(17)
#define SNOR_HWCAPS_PP_1_4_4	BIT(18)
#define SNOR_HWCAPS_PP_4_4_4	BIT(19)

#define SNOR_HWCAPS_PP_OCTO	GENMASK(22, 20)
#define SNOR_HWCAPS_PP_1_1_8	BIT(20)
#define SNOR_HWCAPS_PP_1_8_8	BIT(21)
#define SNOR_HWCAPS_PP_8_8_8	BIT(22)


int spi_nor_scan(struct spi_nor *nor, const char *name,
		 const struct spi_nor_hwcaps *hwcaps);

#endif
