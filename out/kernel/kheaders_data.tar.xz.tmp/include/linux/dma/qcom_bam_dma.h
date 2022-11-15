

#ifndef _QCOM_BAM_DMA_H
#define _QCOM_BAM_DMA_H

#include <asm/byteorder.h>


struct bam_cmd_element {
	__le32 cmd_and_addr;
	__le32 data;
	__le32 mask;
	__le32 reserved;
};


enum bam_command_type {
	BAM_WRITE_COMMAND = 0,
	BAM_READ_COMMAND,
};


static inline void
bam_prep_ce_le32(struct bam_cmd_element *bam_ce, u32 addr,
		 enum bam_command_type cmd, __le32 data)
{
	bam_ce->cmd_and_addr =
		cpu_to_le32((addr & 0xffffff) | ((cmd & 0xff) << 24));
	bam_ce->data = data;
	bam_ce->mask = cpu_to_le32(0xffffffff);
}


static inline void
bam_prep_ce(struct bam_cmd_element *bam_ce, u32 addr,
	    enum bam_command_type cmd, u32 data)
{
	bam_prep_ce_le32(bam_ce, addr, cmd, cpu_to_le32(data));
}
#endif
