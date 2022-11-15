/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __LINUX_BLK_TYPES_H
#define __LINUX_BLK_TYPES_H

#include <linux/types.h>
#include <linux/bvec.h>

struct bio_set;
struct bio;
struct bio_integrity_payload;
struct page;
struct block_device;
struct io_context;
struct cgroup_subsys_state;
typedef void (bio_end_io_t) (struct bio *);
struct bio_crypt_ctx;


#if defined(CONFIG_ALPHA) && !defined(__alpha_bwx__)
typedef u32 __bitwise blk_status_t;
#else
typedef u8 __bitwise blk_status_t;
#endif
#define	BLK_STS_OK 0
#define BLK_STS_NOTSUPP		((__force blk_status_t)1)
#define BLK_STS_TIMEOUT		((__force blk_status_t)2)
#define BLK_STS_NOSPC		((__force blk_status_t)3)
#define BLK_STS_TRANSPORT	((__force blk_status_t)4)
#define BLK_STS_TARGET		((__force blk_status_t)5)
#define BLK_STS_NEXUS		((__force blk_status_t)6)
#define BLK_STS_MEDIUM		((__force blk_status_t)7)
#define BLK_STS_PROTECTION	((__force blk_status_t)8)
#define BLK_STS_RESOURCE	((__force blk_status_t)9)
#define BLK_STS_IOERR		((__force blk_status_t)10)


#define BLK_STS_DM_REQUEUE    ((__force blk_status_t)11)

#define BLK_STS_AGAIN		((__force blk_status_t)12)

struct blk_issue_stat {
	u64 stat;
};


struct bio {
	struct bio		*bi_next;	
	struct gendisk		*bi_disk;
	unsigned int		bi_opf;		
	unsigned short		bi_flags;	
	unsigned short		bi_ioprio;
	unsigned short		bi_write_hint;
	blk_status_t		bi_status;
	u8			bi_partno;

	
	unsigned int		bi_phys_segments;

	
	unsigned int		bi_seg_front_size;
	unsigned int		bi_seg_back_size;

	struct bvec_iter	bi_iter;

	atomic_t		__bi_remaining;
	bio_end_io_t		*bi_end_io;

	void			*bi_private;
#ifdef CONFIG_BLK_CGROUP
	
	struct io_context	*bi_ioc;
	struct cgroup_subsys_state *bi_css;
#ifdef CONFIG_BLK_DEV_THROTTLING_LOW
	void			*bi_cg_private;
	struct blk_issue_stat	bi_issue_stat;
#endif
#endif

#ifdef CONFIG_BLK_INLINE_ENCRYPTION
	struct bio_crypt_ctx	*bi_crypt_context;
#if IS_ENABLED(CONFIG_DM_DEFAULT_KEY)
	bool			bi_skip_dm_default_key;
#endif
#endif

	union {
#if defined(CONFIG_BLK_DEV_INTEGRITY)
		struct bio_integrity_payload *bi_integrity; 
#endif
	};

	unsigned short		bi_vcnt;	

#ifdef CONFIG_MTK_HW_FDE
		
		unsigned int		bi_hw_fde;
		unsigned int		bi_key_idx;
#endif

	

	unsigned short		bi_max_vecs;	

	atomic_t		__bi_cnt;	

	struct bio_vec		*bi_io_vec;	

	struct bio_set		*bi_pool;

	
	struct bio_vec		bi_inline_vecs[0];
};

#define BIO_RESET_BYTES		offsetof(struct bio, bi_max_vecs)


#define BIO_SEG_VALID	1	
#define BIO_CLONED	2	
#define BIO_BOUNCED	3	
#define BIO_USER_MAPPED 4	
#define BIO_NULL_MAPPED 5	
#define BIO_WORKINGSET	6	
#define BIO_QUIET	7	
#define BIO_CHAIN	8	
#define BIO_REFFED	9	
#define BIO_THROTTLED	10	
#define BIO_TRACE_COMPLETION 11	



#define BVEC_POOL_NR		6
#define BVEC_POOL_MAX		(BVEC_POOL_NR - 1)


#define BVEC_POOL_BITS		(3)
#define BVEC_POOL_OFFSET	(16 - BVEC_POOL_BITS)
#define BVEC_POOL_IDX(bio)	((bio)->bi_flags >> BVEC_POOL_OFFSET)
#if (1<< BVEC_POOL_BITS) < (BVEC_POOL_NR+1)
# error "BVEC_POOL_BITS is too small"
#endif


#define BIO_RESET_BITS	BVEC_POOL_OFFSET


#define REQ_OP_BITS	8
#define REQ_OP_MASK	((1 << REQ_OP_BITS) - 1)
#define REQ_FLAG_BITS	24

enum req_opf {
	
	REQ_OP_READ		= 0,
	
	REQ_OP_WRITE		= 1,
	
	REQ_OP_FLUSH		= 2,
	
	REQ_OP_DISCARD		= 3,
	
	REQ_OP_ZONE_REPORT	= 4,
	
	REQ_OP_SECURE_ERASE	= 5,
	
	REQ_OP_ZONE_RESET	= 6,
	
	REQ_OP_WRITE_SAME	= 7,
	
	REQ_OP_WRITE_ZEROES	= 9,

	
	REQ_OP_SCSI_IN		= 32,
	REQ_OP_SCSI_OUT		= 33,
	
	REQ_OP_DRV_IN		= 34,
	REQ_OP_DRV_OUT		= 35,

	REQ_OP_LAST,
};

enum req_flag_bits {
	__REQ_FAILFAST_DEV =	
		REQ_OP_BITS,
	__REQ_FAILFAST_TRANSPORT, 
	__REQ_FAILFAST_DRIVER,	
	__REQ_SYNC,		
	__REQ_META,		
	__REQ_PRIO,		
	__REQ_NOMERGE,		
	__REQ_IDLE,		
	__REQ_INTEGRITY,	
	__REQ_FUA,		
	__REQ_PREFLUSH,		
	__REQ_RAHEAD,		
	__REQ_BACKGROUND,	

	
	__REQ_NOUNMAP,		

	__REQ_NOWAIT,           
#ifdef MTK_UFS_HQA
	__REQ_POWER_LOSS,	
#endif
	__REQ_NR_BITS,		
};

#define REQ_FAILFAST_DEV	(1ULL << __REQ_FAILFAST_DEV)
#define REQ_FAILFAST_TRANSPORT	(1ULL << __REQ_FAILFAST_TRANSPORT)
#define REQ_FAILFAST_DRIVER	(1ULL << __REQ_FAILFAST_DRIVER)
#define REQ_SYNC		(1ULL << __REQ_SYNC)
#define REQ_META		(1ULL << __REQ_META)
#define REQ_PRIO		(1ULL << __REQ_PRIO)
#define REQ_NOMERGE		(1ULL << __REQ_NOMERGE)
#define REQ_IDLE		(1ULL << __REQ_IDLE)
#define REQ_INTEGRITY		(1ULL << __REQ_INTEGRITY)
#define REQ_FUA			(1ULL << __REQ_FUA)
#define REQ_PREFLUSH		(1ULL << __REQ_PREFLUSH)
#define REQ_RAHEAD		(1ULL << __REQ_RAHEAD)
#define REQ_BACKGROUND		(1ULL << __REQ_BACKGROUND)

#define REQ_NOUNMAP		(1ULL << __REQ_NOUNMAP)
#define REQ_NOWAIT		(1ULL << __REQ_NOWAIT)

#ifdef MTK_UFS_HQA

#define REQ_POWER_LOSS		(1ULL << __REQ_POWER_LOSS)
#endif

#define REQ_FAILFAST_MASK \
	(REQ_FAILFAST_DEV | REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER)

#define REQ_NOMERGE_FLAGS \
	(REQ_NOMERGE | REQ_PREFLUSH | REQ_FUA)

#define bio_op(bio) \
	((bio)->bi_opf & REQ_OP_MASK)
#define req_op(req) \
	((req)->cmd_flags & REQ_OP_MASK)


static inline void bio_set_op_attrs(struct bio *bio, unsigned op,
		unsigned op_flags)
{
	bio->bi_opf = op | op_flags;
}

static inline bool op_is_write(unsigned int op)
{
	return (op & 1);
}


static inline bool op_is_flush(unsigned int op)
{
	return op & (REQ_FUA | REQ_PREFLUSH);
}


static inline bool op_is_sync(unsigned int op)
{
	return (op & REQ_OP_MASK) == REQ_OP_READ ||
		(op & (REQ_SYNC | REQ_FUA | REQ_PREFLUSH));
}

typedef unsigned int blk_qc_t;
#define BLK_QC_T_NONE		-1U
#define BLK_QC_T_SHIFT		16
#define BLK_QC_T_INTERNAL	(1U << 31)

static inline bool blk_qc_t_valid(blk_qc_t cookie)
{
	return cookie != BLK_QC_T_NONE;
}

static inline blk_qc_t blk_tag_to_qc_t(unsigned int tag, unsigned int queue_num,
				       bool internal)
{
	blk_qc_t ret = tag | (queue_num << BLK_QC_T_SHIFT);

	if (internal)
		ret |= BLK_QC_T_INTERNAL;

	return ret;
}

static inline unsigned int blk_qc_t_to_queue_num(blk_qc_t cookie)
{
	return (cookie & ~BLK_QC_T_INTERNAL) >> BLK_QC_T_SHIFT;
}

static inline unsigned int blk_qc_t_to_tag(blk_qc_t cookie)
{
	return cookie & ((1u << BLK_QC_T_SHIFT) - 1);
}

static inline bool blk_qc_t_is_internal(blk_qc_t cookie)
{
	return (cookie & BLK_QC_T_INTERNAL) != 0;
}

struct blk_rq_stat {
	s64 mean;
	u64 min;
	u64 max;
	s32 nr_samples;
	s32 nr_batch;
	u64 batch;
};

#endif 
