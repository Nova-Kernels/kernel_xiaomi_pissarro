

#ifndef _ABI_BPMP_ABI_H_
#define _ABI_BPMP_ABI_H_

#ifdef LK
#include <stdint.h>
#endif

#ifndef __ABI_PACKED
#define __ABI_PACKED __attribute__((packed))
#endif

#ifdef NO_GCC_EXTENSIONS
#define EMPTY char empty;
#define EMPTY_ARRAY 1
#else
#define EMPTY
#define EMPTY_ARRAY 0
#endif

#ifndef __UNION_ANON
#define __UNION_ANON
#endif








struct mrq_request {
	
	uint32_t mrq;
	
	uint32_t flags;
} __ABI_PACKED;


struct mrq_response {
	
	int32_t err;
	
	uint32_t flags;
} __ABI_PACKED;


#define MSG_MIN_SZ	128

#define MSG_DATA_MIN_SZ	120



#define MRQ_PING		0
#define MRQ_QUERY_TAG		1
#define MRQ_MODULE_LOAD		4
#define MRQ_MODULE_UNLOAD	5
#define MRQ_TRACE_MODIFY	7
#define MRQ_WRITE_TRACE		8
#define MRQ_THREADED_PING	9
#define MRQ_MODULE_MAIL		11
#define MRQ_DEBUGFS		19
#define MRQ_RESET		20
#define MRQ_I2C			21
#define MRQ_CLK			22
#define MRQ_QUERY_ABI		23
#define MRQ_PG_READ_STATE	25
#define MRQ_PG_UPDATE_STATE	26
#define MRQ_THERMAL		27
#define MRQ_CPU_VHINT		28
#define MRQ_ABI_RATCHET		29
#define MRQ_EMC_DVFS_LATENCY	31
#define MRQ_TRACE_ITER		64
#define MRQ_RINGBUF_CONSOLE	65
#define MRQ_PG			66




#define MAX_CPU_MRQ_ID		66







struct mrq_ping_request {

	uint32_t challenge;
} __ABI_PACKED;


struct mrq_ping_response {
	
	uint32_t reply;
} __ABI_PACKED;




struct mrq_query_tag_request {
  
	uint32_t addr;
} __ABI_PACKED;




struct mrq_module_load_request {
	
	uint32_t phys_addr; 
	
	uint32_t size;
} __ABI_PACKED;


struct mrq_module_load_response {
	
	uint32_t base;
} __ABI_PACKED;




struct mrq_module_unload_request {
	
	uint32_t base;
} __ABI_PACKED;




struct mrq_trace_modify_request {
	
	uint32_t clr;
	
	uint32_t set;
} __ABI_PACKED;


struct mrq_trace_modify_response {
	
	uint32_t mask;
} __ABI_PACKED;




struct mrq_write_trace_request {
	
	uint32_t area;
	
	uint32_t size;
} __ABI_PACKED;


struct mrq_write_trace_response {
	
	uint32_t eof;
} __ABI_PACKED;


struct mrq_threaded_ping_request {
	uint32_t challenge;
} __ABI_PACKED;


struct mrq_threaded_ping_response {
	uint32_t reply;
} __ABI_PACKED;




struct mrq_module_mail_request {
	
	uint32_t base;
	
	uint8_t data[EMPTY_ARRAY];
} __ABI_PACKED;


struct mrq_module_mail_response {
	
	uint8_t data[EMPTY_ARRAY];
} __ABI_PACKED;





enum mrq_debugfs_commands {
	CMD_DEBUGFS_READ = 1,
	CMD_DEBUGFS_WRITE = 2,
	CMD_DEBUGFS_DUMPDIR = 3,
	CMD_DEBUGFS_MAX
};


struct cmd_debugfs_fileop_request {
	
	uint32_t fnameaddr;
	
	uint32_t fnamelen;
	
	uint32_t dataaddr;
	
	uint32_t datalen;
} __ABI_PACKED;


struct cmd_debugfs_dumpdir_request {
	
	uint32_t dataaddr;
	
	uint32_t datalen;
} __ABI_PACKED;


struct cmd_debugfs_fileop_response {
	
	uint32_t reserved;
	
	uint32_t nbytes;
} __ABI_PACKED;


struct cmd_debugfs_dumpdir_response {
	
	uint32_t reserved;
	
	uint32_t nbytes;
} __ABI_PACKED;


struct mrq_debugfs_request {
	uint32_t cmd;
	union {
		struct cmd_debugfs_fileop_request fop;
		struct cmd_debugfs_dumpdir_request dumpdir;
	} __UNION_ANON;
} __ABI_PACKED;


struct mrq_debugfs_response {
	
	int32_t reserved;
	union {
		
		struct cmd_debugfs_fileop_response fop;
		
		struct cmd_debugfs_dumpdir_response dumpdir;
	} __UNION_ANON;
} __ABI_PACKED;


#define DEBUGFS_S_ISDIR	(1 << 9)
#define DEBUGFS_S_IRUSR	(1 << 8)
#define DEBUGFS_S_IWUSR	(1 << 7)






enum mrq_reset_commands {
	CMD_RESET_ASSERT = 1,
	CMD_RESET_DEASSERT = 2,
	CMD_RESET_MODULE = 3,
	CMD_RESET_GET_MAX_ID = 4,
	CMD_RESET_MAX, 
};


struct mrq_reset_request {
	
	uint32_t cmd;
	
	uint32_t reset_id;
} __ABI_PACKED;


struct cmd_reset_get_max_id_response {
	
	uint32_t max_id;
} __ABI_PACKED;


struct mrq_reset_response {
	union {
		struct cmd_reset_get_max_id_response reset_get_max_id;
	} __UNION_ANON;
} __ABI_PACKED;




#define TEGRA_I2C_IPC_MAX_IN_BUF_SIZE	(MSG_DATA_MIN_SZ - 12)
#define TEGRA_I2C_IPC_MAX_OUT_BUF_SIZE	(MSG_DATA_MIN_SZ - 4)



#define SERIALI2C_TEN           0x0010
#define SERIALI2C_RD            0x0001
#define SERIALI2C_STOP          0x8000
#define SERIALI2C_NOSTART       0x4000
#define SERIALI2C_REV_DIR_ADDR  0x2000
#define SERIALI2C_IGNORE_NAK    0x1000
#define SERIALI2C_NO_RD_ACK     0x0800
#define SERIALI2C_RECV_LEN      0x0400


enum {
	CMD_I2C_XFER = 1
};


struct serial_i2c_request {
	
	uint16_t addr;
	
	uint16_t flags;
	
	uint16_t len;
	
	uint8_t data[];
} __ABI_PACKED;


struct cmd_i2c_xfer_request {
	
	uint32_t bus_id;

	
	uint32_t data_size;

	
	uint8_t data_buf[TEGRA_I2C_IPC_MAX_IN_BUF_SIZE];
} __ABI_PACKED;


struct cmd_i2c_xfer_response {
	
	uint32_t data_size;
	
	uint8_t data_buf[TEGRA_I2C_IPC_MAX_OUT_BUF_SIZE];
} __ABI_PACKED;


struct mrq_i2c_request {
	
	uint32_t cmd;
	
	struct cmd_i2c_xfer_request xfer;
} __ABI_PACKED;


struct mrq_i2c_response {
	struct cmd_i2c_xfer_response xfer;
} __ABI_PACKED;




enum {
	CMD_CLK_GET_RATE = 1,
	CMD_CLK_SET_RATE = 2,
	CMD_CLK_ROUND_RATE = 3,
	CMD_CLK_GET_PARENT = 4,
	CMD_CLK_SET_PARENT = 5,
	CMD_CLK_IS_ENABLED = 6,
	CMD_CLK_ENABLE = 7,
	CMD_CLK_DISABLE = 8,
	CMD_CLK_GET_ALL_INFO = 14,
	CMD_CLK_GET_MAX_CLK_ID = 15,
	CMD_CLK_MAX,
};



#define BPMP_CLK_HAS_MUX	(1 << 0)
#define BPMP_CLK_HAS_SET_RATE	(1 << 1)
#define BPMP_CLK_IS_ROOT	(1 << 2)


#define MRQ_CLK_NAME_MAXLEN	40
#define MRQ_CLK_MAX_PARENTS	16


struct cmd_clk_get_rate_request {
	EMPTY
} __ABI_PACKED;

struct cmd_clk_get_rate_response {
	int64_t rate;
} __ABI_PACKED;

struct cmd_clk_set_rate_request {
	int32_t unused;
	int64_t rate;
} __ABI_PACKED;

struct cmd_clk_set_rate_response {
	int64_t rate;
} __ABI_PACKED;

struct cmd_clk_round_rate_request {
	int32_t unused;
	int64_t rate;
} __ABI_PACKED;

struct cmd_clk_round_rate_response {
	int64_t rate;
} __ABI_PACKED;


struct cmd_clk_get_parent_request {
	EMPTY
} __ABI_PACKED;

struct cmd_clk_get_parent_response {
	uint32_t parent_id;
} __ABI_PACKED;

struct cmd_clk_set_parent_request {
	uint32_t parent_id;
} __ABI_PACKED;

struct cmd_clk_set_parent_response {
	uint32_t parent_id;
} __ABI_PACKED;


struct cmd_clk_is_enabled_request {
	EMPTY
} __ABI_PACKED;

struct cmd_clk_is_enabled_response {
	int32_t state;
} __ABI_PACKED;


struct cmd_clk_enable_request {
	EMPTY
} __ABI_PACKED;


struct cmd_clk_enable_response {
	EMPTY
} __ABI_PACKED;


struct cmd_clk_disable_request {
	EMPTY
} __ABI_PACKED;


struct cmd_clk_disable_response {
	EMPTY
} __ABI_PACKED;


struct cmd_clk_get_all_info_request {
	EMPTY
} __ABI_PACKED;

struct cmd_clk_get_all_info_response {
	uint32_t flags;
	uint32_t parent;
	uint32_t parents[MRQ_CLK_MAX_PARENTS];
	uint8_t num_parents;
	uint8_t name[MRQ_CLK_NAME_MAXLEN];
} __ABI_PACKED;


struct cmd_clk_get_max_clk_id_request {
	EMPTY
} __ABI_PACKED;

struct cmd_clk_get_max_clk_id_response {
	uint32_t max_id;
} __ABI_PACKED;




struct mrq_clk_request {
	
	uint32_t cmd_and_id;

	union {
		
		struct cmd_clk_get_rate_request clk_get_rate;
		struct cmd_clk_set_rate_request clk_set_rate;
		struct cmd_clk_round_rate_request clk_round_rate;
		
		struct cmd_clk_get_parent_request clk_get_parent;
		struct cmd_clk_set_parent_request clk_set_parent;
		
		struct cmd_clk_enable_request clk_enable;
		
		struct cmd_clk_disable_request clk_disable;
		
		struct cmd_clk_is_enabled_request clk_is_enabled;
		
		struct cmd_clk_get_all_info_request clk_get_all_info;
		
		struct cmd_clk_get_max_clk_id_request clk_get_max_clk_id;
	} __UNION_ANON;
} __ABI_PACKED;



struct mrq_clk_response {
	union {
		struct cmd_clk_get_rate_response clk_get_rate;
		struct cmd_clk_set_rate_response clk_set_rate;
		struct cmd_clk_round_rate_response clk_round_rate;
		struct cmd_clk_get_parent_response clk_get_parent;
		struct cmd_clk_set_parent_response clk_set_parent;
		
		struct cmd_clk_enable_response clk_enable;
		
		struct cmd_clk_disable_response clk_disable;
		struct cmd_clk_is_enabled_response clk_is_enabled;
		struct cmd_clk_get_all_info_response clk_get_all_info;
		struct cmd_clk_get_max_clk_id_response clk_get_max_clk_id;
	} __UNION_ANON;
} __ABI_PACKED;




struct mrq_query_abi_request {
	
	uint32_t mrq;
} __ABI_PACKED;


struct mrq_query_abi_response {
	
	int32_t status;
} __ABI_PACKED;




struct mrq_pg_read_state_request {
	
	uint32_t partition_id;
} __ABI_PACKED;


struct mrq_pg_read_state_response {
	
	uint32_t sram_state;
	
	uint32_t logic_state;
} __ABI_PACKED;






struct mrq_pg_update_state_request {
	
	uint32_t partition_id;
	
	uint32_t sram_state;
	
	uint32_t logic_state;
	
	uint32_t clock_state;
} __ABI_PACKED;





enum mrq_pg_cmd {
	
	CMD_PG_QUERY_ABI = 0,

	
	CMD_PG_SET_STATE = 1,

	
	CMD_PG_GET_STATE = 2,

	
	CMD_PG_GET_NAME = 3,


	
	CMD_PG_GET_MAX_ID = 4,
};


#define MRQ_PG_NAME_MAXLEN	40


enum pg_states {
	PG_STATE_OFF = 0,
	PG_STATE_ON = 1,
	PG_STATE_RUNNING = 2,
};

struct cmd_pg_query_abi_request {
	uint32_t type; 
} __ABI_PACKED;

struct cmd_pg_set_state_request {
	uint32_t state; 
} __ABI_PACKED;

struct cmd_pg_get_state_response {
	uint32_t state; 
} __ABI_PACKED;

struct cmd_pg_get_name_response {
	uint8_t name[MRQ_PG_NAME_MAXLEN];
} __ABI_PACKED;

struct cmd_pg_get_max_id_response {
	uint32_t max_id;
} __ABI_PACKED;



struct mrq_pg_request {
	uint32_t cmd;
	uint32_t id;
	union {
		struct cmd_pg_query_abi_request query_abi;
		struct cmd_pg_set_state_request set_state;
	} __UNION_ANON;
} __ABI_PACKED;



struct mrq_pg_response {
	union {
		struct cmd_pg_get_state_response get_state;
		struct cmd_pg_get_name_response get_name;
		struct cmd_pg_get_max_id_response get_max_id;
	} __UNION_ANON;
} __ABI_PACKED;


enum mrq_thermal_host_to_bpmp_cmd {
	
	CMD_THERMAL_QUERY_ABI = 0,

	
	CMD_THERMAL_GET_TEMP = 1,

	
	CMD_THERMAL_SET_TRIP = 2,

	
	CMD_THERMAL_GET_NUM_ZONES = 3,

	
	CMD_THERMAL_HOST_TO_BPMP_NUM
};

enum mrq_thermal_bpmp_to_host_cmd {
	
	CMD_THERMAL_HOST_TRIP_REACHED = 100,

	
	CMD_THERMAL_BPMP_TO_HOST_NUM
};


struct cmd_thermal_query_abi_request {
	uint32_t type;
} __ABI_PACKED;


struct cmd_thermal_get_temp_request {
	uint32_t zone;
} __ABI_PACKED;


struct cmd_thermal_get_temp_response {
	int32_t temp;
} __ABI_PACKED;


struct cmd_thermal_set_trip_request {
	uint32_t zone;
	int32_t low;
	int32_t high;
	uint32_t enabled;
} __ABI_PACKED;


struct cmd_thermal_host_trip_reached_request {
	uint32_t zone;
} __ABI_PACKED;


struct cmd_thermal_get_num_zones_response {
	uint32_t num;
} __ABI_PACKED;


struct mrq_thermal_host_to_bpmp_request {
	uint32_t type;
	union {
		struct cmd_thermal_query_abi_request query_abi;
		struct cmd_thermal_get_temp_request get_temp;
		struct cmd_thermal_set_trip_request set_trip;
	} __UNION_ANON;
} __ABI_PACKED;


struct mrq_thermal_bpmp_to_host_request {
	uint32_t type;
	union {
		struct cmd_thermal_host_trip_reached_request host_trip_reached;
	} __UNION_ANON;
} __ABI_PACKED;


union mrq_thermal_bpmp_to_host_response {
	struct cmd_thermal_get_temp_response get_temp;
	struct cmd_thermal_get_num_zones_response get_num_zones;
} __ABI_PACKED;





struct mrq_cpu_vhint_request {
	
	uint32_t addr; 
	
	uint32_t cluster_id; 
} __ABI_PACKED;


struct cpu_vhint_data {
	uint32_t ref_clk_hz; 
	uint16_t pdiv; 
	uint16_t mdiv; 
	uint16_t ndiv_max; 
	
	uint16_t ndiv[80];
	
	uint16_t ndiv_min;
	
	uint16_t vfloor;
	
	uint16_t vceil;
	
	uint16_t vindex_mult;
	
	uint16_t vindex_div;
	
	uint16_t reserved[328];
} __ABI_PACKED;






#define BPMP_ABI_RATCHET_VALUE 3


struct mrq_abi_ratchet_request {
	
	uint16_t ratchet;
};


struct mrq_abi_ratchet_response {
	
	uint16_t ratchet;
};





struct emc_dvfs_latency {
	
	uint32_t freq;
	
	uint32_t latency;
} __ABI_PACKED;

#define EMC_DVFS_LATENCY_MAX_SIZE	14

struct mrq_emc_dvfs_latency_response {
	
	uint32_t num_pairs;
	
	struct emc_dvfs_latency pairs[EMC_DVFS_LATENCY_MAX_SIZE];
} __ABI_PACKED;




enum {
	
	TRACE_ITER_INIT = 0,
	
	TRACE_ITER_CLEAN = 1
};


struct mrq_trace_iter_request {
	
	uint32_t cmd;
} __ABI_PACKED;






#define MRQ_RINGBUF_CONSOLE_MAX_WRITE_LEN 112


#define MRQ_RINGBUF_CONSOLE_MAX_READ_LEN 116

enum mrq_ringbuf_console_host_to_bpmp_cmd {
	
	CMD_RINGBUF_CONSOLE_QUERY_ABI = 0,
	
	CMD_RINGBUF_CONSOLE_READ = 1,
	
	CMD_RINGBUF_CONSOLE_WRITE = 2,
	
	CMD_RINGBUF_CONSOLE_GET_FIFO = 3,
};


struct cmd_ringbuf_console_query_abi_req {
	
	uint32_t cmd;
} __ABI_PACKED;


struct cmd_ringbuf_console_query_abi_resp {
	EMPTY
} __ABI_PACKED;


struct cmd_ringbuf_console_read_req {
	
	uint8_t len;
} __ABI_PACKED;


struct cmd_ringbuf_console_read_resp {
	
	uint8_t data[MRQ_RINGBUF_CONSOLE_MAX_READ_LEN];
	
	uint8_t len;
} __ABI_PACKED;


struct cmd_ringbuf_console_write_req {
	
	uint8_t data[MRQ_RINGBUF_CONSOLE_MAX_WRITE_LEN];
	
	uint8_t len;
} __ABI_PACKED;


struct cmd_ringbuf_console_write_resp {
	
	uint32_t space_avail;
	
	uint8_t len;
} __ABI_PACKED;


struct cmd_ringbuf_console_get_fifo_req {
	EMPTY
} __ABI_PACKED;


struct cmd_ringbuf_console_get_fifo_resp {
	
	uint64_t bpmp_tx_buf_addr;
	
	uint64_t bpmp_tx_head_addr;
	
	uint64_t bpmp_tx_tail_addr;
	
	uint32_t bpmp_tx_buf_len;
} __ABI_PACKED;


struct mrq_ringbuf_console_host_to_bpmp_request {
	
	uint32_t type;
	
	union {
		struct cmd_ringbuf_console_query_abi_req query_abi;
		struct cmd_ringbuf_console_read_req read;
		struct cmd_ringbuf_console_write_req write;
		struct cmd_ringbuf_console_get_fifo_req get_fifo;
	} __UNION_ANON;
} __ABI_PACKED;


union mrq_ringbuf_console_bpmp_to_host_response {
	struct cmd_ringbuf_console_query_abi_resp query_abi;
	struct cmd_ringbuf_console_read_resp read;
	struct cmd_ringbuf_console_write_resp write;
	struct cmd_ringbuf_console_get_fifo_resp get_fifo;
} __ABI_PACKED;














#define BPMP_ENOENT	2

#define BPMP_ENOHANDLER	3

#define BPMP_EIO	5

#define BPMP_EBADCMD	6

#define BPMP_ENOMEM	12

#define BPMP_EACCES	13

#define BPMP_EFAULT	14

#define BPMP_ENODEV	19

#define BPMP_EISDIR	21

#define BPMP_EINVAL	22

#define BPMP_ETIMEDOUT  23

#define BPMP_ERANGE	34


#endif
