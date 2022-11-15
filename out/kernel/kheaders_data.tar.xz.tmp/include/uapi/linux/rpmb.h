

#ifndef _UAPI_LINUX_RPMB_H_
#define _UAPI_LINUX_RPMB_H_

#include <linux/types.h>


struct rpmb_frame {
	__u8   stuff[196];
	__u8   key_mac[32];
	__u8   data[256];
	__u8   nonce[16];
	__be32 write_counter;
	__be16 addr;
	__be16 block_count;
	__be16 result;
	__be16 req_resp;
} __attribute__((packed));

#define RPMB_PROGRAM_KEY       0x1    
#define RPMB_GET_WRITE_COUNTER 0x2    
#define RPMB_WRITE_DATA        0x3    
#define RPMB_READ_DATA         0x4    
#define RPMB_RESULT_READ       0x5    

#define RPMB_REQ2RESP(_OP) ((_OP) << 8)
#define RPMB_RESP2REQ(_OP) ((_OP) >> 8)


#define hmac_data_len \
	(sizeof(struct rpmb_frame) - offsetof(struct rpmb_frame, data))


enum rpmb_op_result {
	RPMB_ERR_OK      = 0x0000,
	RPMB_ERR_GENERAL = 0x0001,
	RPMB_ERR_AUTH    = 0x0002,
	RPMB_ERR_COUNTER = 0x0003,
	RPMB_ERR_ADDRESS = 0x0004,
	RPMB_ERR_WRITE   = 0x0005,
	RPMB_ERR_READ    = 0x0006,
	RPMB_ERR_NO_KEY  = 0x0007,

	RPMB_ERR_COUNTER_EXPIRED = 0x0080
};

#define RPMB_F_WRITE     (1UL << 0)
#define RPMB_F_REL_WRITE (1UL << 1)


struct rpmb_ioc_cmd {
	__u32 flags;
	__u32 nframes;
	__aligned_u64 frames_ptr;
};

#define rpmb_ioc_cmd_set_frames(_cmd, _ptr) \
	(_cmd).frames_ptr = (__aligned_u64)(intptr_t)(_ptr)

#define rpmb_ioc_cmd_set(_cmd, _flags, _ptr, _n) do {        \
	struct rpmb_ioc_cmd *icmd = &(_cmd);                 \
	icmd->flags = (_flags);                              \
	icmd->nframes = (_n);                                \
	icmd->frames_ptr = (__aligned_u64)(intptr_t)(_ptr);  \
} while (0)


struct rpmb_ioc_req_cmd {
	__u64 req_type;
	struct rpmb_ioc_cmd icmd;
	struct rpmb_ioc_cmd ocmd;
};


struct rpmb_ioc_seq_cmd {
	__u64 num_of_cmds;
	struct rpmb_ioc_cmd cmds[0];
};

#define RPMB_IOC_REQ_CMD _IOWR(0xB5, 0, struct rpmb_ioc_req_cmd)
#define RPMB_IOC_SEQ_CMD _IOWR(0xB5, 1, struct rpmb_ioc_seq_cmd)

#endif 
