

#ifndef __UAPI_MTK_VCU_CONTROLS_H__
#define __UAPI_MTK_VCU_CONTROLS_H__

#define SHARE_BUF_SIZE 72
#define LOG_INFO_SIZE 1024
#define VCODEC_CMDQ_CMD_MAX           (2048)


struct mem_obj {
	u64 iova;
	u32 len;
	u64 pa;
	u64 va;
};


struct map_obj {
	u32 map_buf;
	u32 map_type;
	u64 reserved;
};


struct gce_cmds {
	u8  cmd[VCODEC_CMDQ_CMD_MAX];
	u64 addr[VCODEC_CMDQ_CMD_MAX];
	u64 data[VCODEC_CMDQ_CMD_MAX];
	u32 mask[VCODEC_CMDQ_CMD_MAX];
	u32 dma_offset[VCODEC_CMDQ_CMD_MAX];
	u32 dma_size[VCODEC_CMDQ_CMD_MAX];
	u32 cmd_cnt;
};


struct gce_cmdq_obj {
	u64	cmds_user_ptr;
	u64	gce_handle;
	u32	flush_order;
	u32	codec_type;
	u32	core_id;
	u32 secure;
};


struct gce_obj {
	u64	gce_handle;
	u32	flush_order;
	u32	codec_type;
};

enum gce_cmd_id {
	CMD_READ = 0,    
	CMD_WRITE,       
	CMD_POLL_REG,
	
	CMD_WAIT_EVENT,      
	CMD_MEM_MV,      
	CMD_POLL_ADDR,
	
	CMD_SEC_WRITE,   
	CMD_MAX
};

enum gce_event_id {
	VDEC_EVENT_0,    
	VDEC_EVENT_1,    
	VDEC_EVENT_2,    
	VDEC_EVENT_3,    
	VDEC_EVENT_4,
	
	VDEC_EVENT_5,
	
	VDEC_EVENT_6,    
	VDEC_EVENT_7,
	
	VDEC_EVENT_8,
	
	VDEC_EVENT_9,
	
	VDEC_EVENT_10,
	
	VDEC_EVENT_11,
	
	VDEC_EVENT_12,   
	VDEC_EVENT_13,   
	VDEC_EVENT_14,   
	VDEC_EVENT_15,   
	VDEC_LAT_EVENT_0,
	VDEC_LAT_EVENT_1,
	VDEC_LAT_EVENT_2,
	VDEC_LAT_EVENT_3,
	VDEC_LAT_EVENT_4,
	VDEC_LAT_EVENT_5,
	VDEC_LAT_EVENT_6,
	VDEC_LAT_EVENT_7,
	VDEC_LAT_EVENT_8,
	VDEC_LAT_EVENT_9,
	VDEC_LAT_EVENT_10,
	VDEC_LAT_EVENT_11,
	VDEC_LAT_EVENT_12,
	VDEC_LAT_EVENT_13,
	VDEC_LAT_EVENT_14,
	VDEC_LAT_EVENT_15,
	VDEC_EVENT_COUNT,
	VENC_EOF = VDEC_EVENT_COUNT,
	VENC_CMDQ_PAUSE_DONE,
	VENC_MB_DONE,
	VENC_128BYTE_CNT_DONE,
	VENC_EOF_C1,
	VENC_WP_2ND_DONE,
	VENC_WP_3ND_DONE,
	VENC_SPS_DONE,
	VENC_PPS_DONE
};


#define VCU_SET_OBJECT	_IOW('v', 0, struct share_obj)
#define VCU_MVA_ALLOCATION	_IOWR('v', 1, struct mem_obj)
#define VCU_MVA_FREE		_IOWR('v', 2, struct mem_obj)
#define VCU_CACHE_FLUSH_ALL	_IOWR('v', 3, struct mem_obj)
#define VCU_CACHE_FLUSH_BUFF	_IOWR('v', 4, struct mem_obj)
#define VCU_CACHE_INVALIDATE_BUFF	_IOWR('v', 5, struct mem_obj)
#define VCU_PA_ALLOCATION	_IOWR('v', 6, struct mem_obj)
#define VCU_PA_FREE		_IOWR('v', 7, struct mem_obj)
#define VCU_GCE_SET_CMD_FLUSH _IOW('v', 8, struct gce_cmdq_obj)
#define VCU_GCE_WAIT_CALLBACK _IOW('v', 9, struct gce_obj)
#define VCU_GET_OBJECT		_IOWR('v', 10, struct share_obj)
#define VCU_GET_LOG_OBJECT	_IOW('v', 11, struct log_test_nofuse)
#define VCU_SET_LOG_OBJECT	_IOW('v', 12, struct log_test)
#define VCU_SET_MMAP_TYPE	_IOW('v', 13, struct map_obj)

#define COMPAT_VCU_SET_OBJECT		_IOW('v', 0, struct share_obj)
#define COMPAT_VCU_MVA_ALLOCATION	_IOWR('v', 1, struct compat_mem_obj)
#define COMPAT_VCU_MVA_FREE		_IOWR('v', 2, struct compat_mem_obj)
#define COMPAT_VCU_CACHE_FLUSH_ALL	_IOWR('v', 3, struct compat_mem_obj)
#define COMPAT_VCU_CACHE_FLUSH_BUFF	_IOWR('v', 4, struct compat_mem_obj)
#define COMPAT_VCU_CACHE_INVALIDATE_BUFF _IOWR('v', 5, struct compat_mem_obj)
#define COMPAT_VCU_PA_ALLOCATION	_IOWR('v', 6, struct compat_mem_obj)
#define COMPAT_VCU_PA_FREE		_IOWR('v', 7, struct compat_mem_obj)
#define COMPAT_VCU_SET_MMAP_TYPE	_IOW('v', 13, struct map_obj)

#if IS_ENABLED(CONFIG_COMPAT)
struct compat_mem_obj {
	u64 iova;
	u32 len;
	compat_u64 pa;
	compat_u64 va;
};
#endif


struct share_obj {
	s32 id;
	u32 len;
	unsigned char share_buf[SHARE_BUF_SIZE];
};

struct log_test_nofuse {
	char log_info[LOG_INFO_SIZE];
};

#endif

