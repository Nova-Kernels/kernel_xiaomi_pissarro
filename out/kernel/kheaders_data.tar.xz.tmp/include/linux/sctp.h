
#ifndef __LINUX_SCTP_H__
#define __LINUX_SCTP_H__

#include <linux/in.h>		
#include <linux/in6.h>		
#include <linux/skbuff.h>

#include <uapi/linux/sctp.h>


struct sctphdr {
	__be16 source;
	__be16 dest;
	__be32 vtag;
	__le32 checksum;
};

static inline struct sctphdr *sctp_hdr(const struct sk_buff *skb)
{
	return (struct sctphdr *)skb_transport_header(skb);
}


struct sctp_chunkhdr {
	__u8 type;
	__u8 flags;
	__be16 length;
};



enum sctp_cid {
	SCTP_CID_DATA			= 0,
        SCTP_CID_INIT			= 1,
        SCTP_CID_INIT_ACK		= 2,
        SCTP_CID_SACK			= 3,
        SCTP_CID_HEARTBEAT		= 4,
        SCTP_CID_HEARTBEAT_ACK		= 5,
        SCTP_CID_ABORT			= 6,
        SCTP_CID_SHUTDOWN		= 7,
        SCTP_CID_SHUTDOWN_ACK		= 8,
        SCTP_CID_ERROR			= 9,
        SCTP_CID_COOKIE_ECHO		= 10,
        SCTP_CID_COOKIE_ACK	        = 11,
        SCTP_CID_ECN_ECNE		= 12,
        SCTP_CID_ECN_CWR		= 13,
        SCTP_CID_SHUTDOWN_COMPLETE	= 14,

	
	SCTP_CID_AUTH			= 0x0F,

	
	SCTP_CID_FWD_TSN		= 0xC0,

	
	SCTP_CID_ASCONF			= 0xC1,
	SCTP_CID_ASCONF_ACK		= 0x80,
	SCTP_CID_RECONF			= 0x82,
}; 



enum {
	SCTP_CID_ACTION_DISCARD     = 0x00,
	SCTP_CID_ACTION_DISCARD_ERR = 0x40,
	SCTP_CID_ACTION_SKIP        = 0x80,
	SCTP_CID_ACTION_SKIP_ERR    = 0xc0,
};

enum { SCTP_CID_ACTION_MASK = 0xc0, };


enum { SCTP_CHUNK_FLAG_T = 0x01 };



#define sctp_test_T_bit(c)    ((c)->chunk_hdr->flags & SCTP_CHUNK_FLAG_T)



struct sctp_paramhdr {
	__be16 type;
	__be16 length;
};

enum sctp_param {

	
	SCTP_PARAM_HEARTBEAT_INFO		= cpu_to_be16(1),
	
	SCTP_PARAM_IPV4_ADDRESS			= cpu_to_be16(5),
	SCTP_PARAM_IPV6_ADDRESS			= cpu_to_be16(6),
	SCTP_PARAM_STATE_COOKIE			= cpu_to_be16(7),
	SCTP_PARAM_UNRECOGNIZED_PARAMETERS	= cpu_to_be16(8),
	SCTP_PARAM_COOKIE_PRESERVATIVE		= cpu_to_be16(9),
	SCTP_PARAM_HOST_NAME_ADDRESS		= cpu_to_be16(11),
	SCTP_PARAM_SUPPORTED_ADDRESS_TYPES	= cpu_to_be16(12),
	SCTP_PARAM_ECN_CAPABLE			= cpu_to_be16(0x8000),

	
	SCTP_PARAM_RANDOM			= cpu_to_be16(0x8002),
	SCTP_PARAM_CHUNKS			= cpu_to_be16(0x8003),
	SCTP_PARAM_HMAC_ALGO			= cpu_to_be16(0x8004),

	
	SCTP_PARAM_SUPPORTED_EXT	= cpu_to_be16(0x8008),

	
	SCTP_PARAM_FWD_TSN_SUPPORT	= cpu_to_be16(0xc000),

	
	SCTP_PARAM_ADD_IP		= cpu_to_be16(0xc001),
	SCTP_PARAM_DEL_IP		= cpu_to_be16(0xc002),
	SCTP_PARAM_ERR_CAUSE		= cpu_to_be16(0xc003),
	SCTP_PARAM_SET_PRIMARY		= cpu_to_be16(0xc004),
	SCTP_PARAM_SUCCESS_REPORT	= cpu_to_be16(0xc005),
	SCTP_PARAM_ADAPTATION_LAYER_IND = cpu_to_be16(0xc006),

	
	SCTP_PARAM_RESET_OUT_REQUEST		= cpu_to_be16(0x000d),
	SCTP_PARAM_RESET_IN_REQUEST		= cpu_to_be16(0x000e),
	SCTP_PARAM_RESET_TSN_REQUEST		= cpu_to_be16(0x000f),
	SCTP_PARAM_RESET_RESPONSE		= cpu_to_be16(0x0010),
	SCTP_PARAM_RESET_ADD_OUT_STREAMS	= cpu_to_be16(0x0011),
	SCTP_PARAM_RESET_ADD_IN_STREAMS		= cpu_to_be16(0x0012),
}; 



enum {
	SCTP_PARAM_ACTION_DISCARD     = cpu_to_be16(0x0000),
	SCTP_PARAM_ACTION_DISCARD_ERR = cpu_to_be16(0x4000),
	SCTP_PARAM_ACTION_SKIP        = cpu_to_be16(0x8000),
	SCTP_PARAM_ACTION_SKIP_ERR    = cpu_to_be16(0xc000),
};

enum { SCTP_PARAM_ACTION_MASK = cpu_to_be16(0xc000), };



struct sctp_datahdr {
	__be32 tsn;
	__be16 stream;
	__be16 ssn;
	__u32 ppid;
	__u8  payload[0];
};

struct sctp_data_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_datahdr data_hdr;
};


enum {
	SCTP_DATA_MIDDLE_FRAG	= 0x00,
	SCTP_DATA_LAST_FRAG	= 0x01,
	SCTP_DATA_FIRST_FRAG	= 0x02,
	SCTP_DATA_NOT_FRAG	= 0x03,
	SCTP_DATA_UNORDERED	= 0x04,
	SCTP_DATA_SACK_IMM	= 0x08,
};
enum { SCTP_DATA_FRAG_MASK = 0x03, };



struct sctp_inithdr {
	__be32 init_tag;
	__be32 a_rwnd;
	__be16 num_outbound_streams;
	__be16 num_inbound_streams;
	__be32 initial_tsn;
	__u8  params[0];
};

struct sctp_init_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_inithdr init_hdr;
};



struct sctp_ipv4addr_param {
	struct sctp_paramhdr param_hdr;
	struct in_addr addr;
};


struct sctp_ipv6addr_param {
	struct sctp_paramhdr param_hdr;
	struct in6_addr addr;
};


struct sctp_cookie_preserve_param {
	struct sctp_paramhdr param_hdr;
	__be32 lifespan_increment;
};


struct sctp_hostname_param {
	struct sctp_paramhdr param_hdr;
	uint8_t hostname[0];
};


struct sctp_supported_addrs_param {
	struct sctp_paramhdr param_hdr;
	__be16 types[0];
};


struct sctp_adaptation_ind_param {
	struct sctp_paramhdr param_hdr;
	__be32 adaptation_ind;
};


struct sctp_supported_ext_param {
	struct sctp_paramhdr param_hdr;
	__u8 chunks[0];
};


struct sctp_random_param {
	struct sctp_paramhdr param_hdr;
	__u8 random_val[0];
};


struct sctp_chunks_param {
	struct sctp_paramhdr param_hdr;
	__u8 chunks[0];
};


struct sctp_hmac_algo_param {
	struct sctp_paramhdr param_hdr;
	__be16 hmac_ids[0];
};


struct sctp_initack_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_inithdr init_hdr;
};


struct sctp_cookie_param {
	struct sctp_paramhdr p;
	__u8 body[0];
};


struct sctp_unrecognized_param {
	struct sctp_paramhdr param_hdr;
	struct sctp_paramhdr unrecognized;
};





struct sctp_gap_ack_block {
	__be16 start;
	__be16 end;
};

union sctp_sack_variable {
	struct sctp_gap_ack_block gab;
	__be32 dup;
};

struct sctp_sackhdr {
	__be32 cum_tsn_ack;
	__be32 a_rwnd;
	__be16 num_gap_ack_blocks;
	__be16 num_dup_tsns;
	union sctp_sack_variable variable[0];
};

struct sctp_sack_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_sackhdr sack_hdr;
};




struct sctp_heartbeathdr {
	struct sctp_paramhdr info;
};

struct sctp_heartbeat_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_heartbeathdr hb_hdr;
};



struct sctp_abort_chunk {
	struct sctp_chunkhdr uh;
};



struct sctp_shutdownhdr {
	__be32 cum_tsn_ack;
};

struct sctp_shutdown_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_shutdownhdr shutdown_hdr;
};



struct sctp_errhdr {
	__be16 cause;
	__be16 length;
	__u8  variable[0];
};

struct sctp_operr_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_errhdr err_hdr;
};


enum sctp_error {

	SCTP_ERROR_NO_ERROR	   = cpu_to_be16(0x00),
	SCTP_ERROR_INV_STRM	   = cpu_to_be16(0x01),
	SCTP_ERROR_MISS_PARAM 	   = cpu_to_be16(0x02),
	SCTP_ERROR_STALE_COOKIE	   = cpu_to_be16(0x03),
	SCTP_ERROR_NO_RESOURCE 	   = cpu_to_be16(0x04),
	SCTP_ERROR_DNS_FAILED      = cpu_to_be16(0x05),
	SCTP_ERROR_UNKNOWN_CHUNK   = cpu_to_be16(0x06),
	SCTP_ERROR_INV_PARAM       = cpu_to_be16(0x07),
	SCTP_ERROR_UNKNOWN_PARAM   = cpu_to_be16(0x08),
	SCTP_ERROR_NO_DATA         = cpu_to_be16(0x09),
	SCTP_ERROR_COOKIE_IN_SHUTDOWN = cpu_to_be16(0x0a),


	

	SCTP_ERROR_RESTART         = cpu_to_be16(0x0b),
	SCTP_ERROR_USER_ABORT      = cpu_to_be16(0x0c),
	SCTP_ERROR_PROTO_VIOLATION = cpu_to_be16(0x0d),

	
	SCTP_ERROR_DEL_LAST_IP	= cpu_to_be16(0x00A0),
	SCTP_ERROR_RSRC_LOW	= cpu_to_be16(0x00A1),
	SCTP_ERROR_DEL_SRC_IP	= cpu_to_be16(0x00A2),
	SCTP_ERROR_ASCONF_ACK   = cpu_to_be16(0x00A3),
	SCTP_ERROR_REQ_REFUSED	= cpu_to_be16(0x00A4),

	
	 SCTP_ERROR_UNSUP_HMAC	= cpu_to_be16(0x0105)
};




struct sctp_ecnehdr {
	__be32 lowest_tsn;
};

struct sctp_ecne_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_ecnehdr ence_hdr;
};


struct sctp_cwrhdr {
	__be32 lowest_tsn;
};


struct sctp_fwdtsn_skip {
	__be16 stream;
	__be16 ssn;
};

struct sctp_fwdtsn_hdr {
	__be32 new_cum_tsn;
	struct sctp_fwdtsn_skip skip[0];
};

struct sctp_fwdtsn_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_fwdtsn_hdr fwdtsn_hdr;
};



struct sctp_addip_param {
	struct sctp_paramhdr param_hdr;
	__be32 crr_id;
};

struct sctp_addiphdr {
	__be32	serial;
	__u8	params[0];
};

struct sctp_addip_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_addiphdr addip_hdr;
};


struct sctp_authhdr {
	__be16 shkey_id;
	__be16 hmac_id;
	__u8   hmac[0];
};

struct sctp_auth_chunk {
	struct sctp_chunkhdr chunk_hdr;
	struct sctp_authhdr auth_hdr;
};

struct sctp_infox {
	struct sctp_info *sctpinfo;
	struct sctp_association *asoc;
};

struct sctp_reconf_chunk {
	struct sctp_chunkhdr chunk_hdr;
	__u8 params[0];
};

struct sctp_strreset_outreq {
	struct sctp_paramhdr param_hdr;
	__be32 request_seq;
	__be32 response_seq;
	__be32 send_reset_at_tsn;
	__be16 list_of_streams[0];
};

struct sctp_strreset_inreq {
	struct sctp_paramhdr param_hdr;
	__be32 request_seq;
	__be16 list_of_streams[0];
};

struct sctp_strreset_tsnreq {
	struct sctp_paramhdr param_hdr;
	__be32 request_seq;
};

struct sctp_strreset_addstrm {
	struct sctp_paramhdr param_hdr;
	__be32 request_seq;
	__be16 number_of_streams;
	__be16 reserved;
};

enum {
	SCTP_STRRESET_NOTHING_TO_DO	= 0x00,
	SCTP_STRRESET_PERFORMED		= 0x01,
	SCTP_STRRESET_DENIED		= 0x02,
	SCTP_STRRESET_ERR_WRONG_SSN	= 0x03,
	SCTP_STRRESET_ERR_IN_PROGRESS	= 0x04,
	SCTP_STRRESET_ERR_BAD_SEQNO	= 0x05,
	SCTP_STRRESET_IN_PROGRESS	= 0x06,
};

struct sctp_strreset_resp {
	struct sctp_paramhdr param_hdr;
	__be32 response_seq;
	__be32 result;
};

struct sctp_strreset_resptsn {
	struct sctp_paramhdr param_hdr;
	__be32 response_seq;
	__be32 result;
	__be32 senders_next_tsn;
	__be32 receivers_next_tsn;
};

#endif 
