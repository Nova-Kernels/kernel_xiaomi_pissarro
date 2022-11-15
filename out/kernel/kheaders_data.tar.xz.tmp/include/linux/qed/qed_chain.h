

#ifndef _QED_CHAIN_H
#define _QED_CHAIN_H

#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/qed/common_hsi.h>

enum qed_chain_mode {
	
	QED_CHAIN_MODE_NEXT_PTR,

	
	QED_CHAIN_MODE_SINGLE,

	
	QED_CHAIN_MODE_PBL,
};

enum qed_chain_use_mode {
	QED_CHAIN_USE_TO_PRODUCE,		
	QED_CHAIN_USE_TO_CONSUME,		
	QED_CHAIN_USE_TO_CONSUME_PRODUCE,	
};

enum qed_chain_cnt_type {
	
	QED_CHAIN_CNT_TYPE_U16,

	
	QED_CHAIN_CNT_TYPE_U32,
};

struct qed_chain_next {
	struct regpair	next_phys;
	void		*next_virt;
};

struct qed_chain_pbl_u16 {
	u16 prod_page_idx;
	u16 cons_page_idx;
};

struct qed_chain_pbl_u32 {
	u32 prod_page_idx;
	u32 cons_page_idx;
};

struct qed_chain_ext_pbl {
	dma_addr_t p_pbl_phys;
	void *p_pbl_virt;
};

struct qed_chain_u16 {
	
	u16 prod_idx;
	u16 cons_idx;
};

struct qed_chain_u32 {
	
	u32 prod_idx;
	u32 cons_idx;
};

struct addr_tbl_entry {
	void *virt_addr;
	dma_addr_t dma_map;
};

struct qed_chain {
	
	
	void *p_prod_elem;
	void *p_cons_elem;

	
	struct {
		
		struct addr_tbl_entry *pp_addr_tbl;

		union {
			struct qed_chain_pbl_u16 u16;
			struct qed_chain_pbl_u32 u32;
		} c;
	} pbl;

	union {
		struct qed_chain_u16 chain16;
		struct qed_chain_u32 chain32;
	} u;

	
	u32 capacity;
	u32 page_cnt;

	enum qed_chain_mode mode;

	
	u16 elem_per_page;
	u16 elem_per_page_mask;
	u16 elem_size;
	u16 next_page_mask;
	u16 usable_per_page;
	u8 elem_unusable;

	u8 cnt_type;

	

	
	struct {
		dma_addr_t p_phys_table;
		void *p_virt_table;
	} pbl_sp;

	
	void *p_virt_addr;
	dma_addr_t p_phys_addr;

	
	u32 size;

	u8 intended_use;

	bool b_external_pbl;
};

#define QED_CHAIN_PBL_ENTRY_SIZE        (8)
#define QED_CHAIN_PAGE_SIZE             (0x1000)
#define ELEMS_PER_PAGE(elem_size)       (QED_CHAIN_PAGE_SIZE / (elem_size))

#define UNUSABLE_ELEMS_PER_PAGE(elem_size, mode)	 \
	(((mode) == QED_CHAIN_MODE_NEXT_PTR) ?		 \
	 (u8)(1 + ((sizeof(struct qed_chain_next) - 1) / \
		   (elem_size))) : 0)

#define USABLE_ELEMS_PER_PAGE(elem_size, mode) \
	((u32)(ELEMS_PER_PAGE(elem_size) -     \
	       UNUSABLE_ELEMS_PER_PAGE(elem_size, mode)))

#define QED_CHAIN_PAGE_CNT(elem_cnt, elem_size, mode) \
	DIV_ROUND_UP(elem_cnt, USABLE_ELEMS_PER_PAGE(elem_size, mode))

#define is_chain_u16(p) ((p)->cnt_type == QED_CHAIN_CNT_TYPE_U16)
#define is_chain_u32(p) ((p)->cnt_type == QED_CHAIN_CNT_TYPE_U32)


static inline u16 qed_chain_get_prod_idx(struct qed_chain *p_chain)
{
	return p_chain->u.chain16.prod_idx;
}

static inline u16 qed_chain_get_cons_idx(struct qed_chain *p_chain)
{
	return p_chain->u.chain16.cons_idx;
}

static inline u32 qed_chain_get_cons_idx_u32(struct qed_chain *p_chain)
{
	return p_chain->u.chain32.cons_idx;
}

static inline u16 qed_chain_get_elem_left(struct qed_chain *p_chain)
{
	u16 used;

	used = (u16) (((u32)0x10000 +
		       (u32)p_chain->u.chain16.prod_idx) -
		      (u32)p_chain->u.chain16.cons_idx);
	if (p_chain->mode == QED_CHAIN_MODE_NEXT_PTR)
		used -= p_chain->u.chain16.prod_idx / p_chain->elem_per_page -
		    p_chain->u.chain16.cons_idx / p_chain->elem_per_page;

	return (u16)(p_chain->capacity - used);
}

static inline u32 qed_chain_get_elem_left_u32(struct qed_chain *p_chain)
{
	u32 used;

	used = (u32) (((u64)0x100000000ULL +
		       (u64)p_chain->u.chain32.prod_idx) -
		      (u64)p_chain->u.chain32.cons_idx);
	if (p_chain->mode == QED_CHAIN_MODE_NEXT_PTR)
		used -= p_chain->u.chain32.prod_idx / p_chain->elem_per_page -
		    p_chain->u.chain32.cons_idx / p_chain->elem_per_page;

	return p_chain->capacity - used;
}

static inline u16 qed_chain_get_usable_per_page(struct qed_chain *p_chain)
{
	return p_chain->usable_per_page;
}

static inline u8 qed_chain_get_unusable_per_page(struct qed_chain *p_chain)
{
	return p_chain->elem_unusable;
}

static inline u32 qed_chain_get_page_cnt(struct qed_chain *p_chain)
{
	return p_chain->page_cnt;
}

static inline dma_addr_t qed_chain_get_pbl_phys(struct qed_chain *p_chain)
{
	return p_chain->pbl_sp.p_phys_table;
}


static inline void
qed_chain_advance_page(struct qed_chain *p_chain,
		       void **p_next_elem, void *idx_to_inc, void *page_to_inc)
{
	struct qed_chain_next *p_next = NULL;
	u32 page_index = 0;

	switch (p_chain->mode) {
	case QED_CHAIN_MODE_NEXT_PTR:
		p_next = *p_next_elem;
		*p_next_elem = p_next->next_virt;
		if (is_chain_u16(p_chain))
			*(u16 *)idx_to_inc += p_chain->elem_unusable;
		else
			*(u32 *)idx_to_inc += p_chain->elem_unusable;
		break;
	case QED_CHAIN_MODE_SINGLE:
		*p_next_elem = p_chain->p_virt_addr;
		break;

	case QED_CHAIN_MODE_PBL:
		if (is_chain_u16(p_chain)) {
			if (++(*(u16 *)page_to_inc) == p_chain->page_cnt)
				*(u16 *)page_to_inc = 0;
			page_index = *(u16 *)page_to_inc;
		} else {
			if (++(*(u32 *)page_to_inc) == p_chain->page_cnt)
				*(u32 *)page_to_inc = 0;
			page_index = *(u32 *)page_to_inc;
		}
		*p_next_elem = p_chain->pbl.pp_addr_tbl[page_index].virt_addr;
	}
}

#define is_unusable_idx(p, idx)	\
	(((p)->u.chain16.idx & (p)->elem_per_page_mask) == (p)->usable_per_page)

#define is_unusable_idx_u32(p, idx) \
	(((p)->u.chain32.idx & (p)->elem_per_page_mask) == (p)->usable_per_page)
#define is_unusable_next_idx(p, idx)				 \
	((((p)->u.chain16.idx + 1) & (p)->elem_per_page_mask) == \
	 (p)->usable_per_page)

#define is_unusable_next_idx_u32(p, idx)			 \
	((((p)->u.chain32.idx + 1) & (p)->elem_per_page_mask) == \
	 (p)->usable_per_page)

#define test_and_skip(p, idx)						   \
	do {						\
		if (is_chain_u16(p)) {					   \
			if (is_unusable_idx(p, idx))			   \
				(p)->u.chain16.idx += (p)->elem_unusable;  \
		} else {						   \
			if (is_unusable_idx_u32(p, idx))		   \
				(p)->u.chain32.idx += (p)->elem_unusable;  \
		}					\
	} while (0)


static inline void qed_chain_return_produced(struct qed_chain *p_chain)
{
	if (is_chain_u16(p_chain))
		p_chain->u.chain16.cons_idx++;
	else
		p_chain->u.chain32.cons_idx++;
	test_and_skip(p_chain, cons_idx);
}


static inline void *qed_chain_produce(struct qed_chain *p_chain)
{
	void *p_ret = NULL, *p_prod_idx, *p_prod_page_idx;

	if (is_chain_u16(p_chain)) {
		if ((p_chain->u.chain16.prod_idx &
		     p_chain->elem_per_page_mask) == p_chain->next_page_mask) {
			p_prod_idx = &p_chain->u.chain16.prod_idx;
			p_prod_page_idx = &p_chain->pbl.c.u16.prod_page_idx;
			qed_chain_advance_page(p_chain, &p_chain->p_prod_elem,
					       p_prod_idx, p_prod_page_idx);
		}
		p_chain->u.chain16.prod_idx++;
	} else {
		if ((p_chain->u.chain32.prod_idx &
		     p_chain->elem_per_page_mask) == p_chain->next_page_mask) {
			p_prod_idx = &p_chain->u.chain32.prod_idx;
			p_prod_page_idx = &p_chain->pbl.c.u32.prod_page_idx;
			qed_chain_advance_page(p_chain, &p_chain->p_prod_elem,
					       p_prod_idx, p_prod_page_idx);
		}
		p_chain->u.chain32.prod_idx++;
	}

	p_ret = p_chain->p_prod_elem;
	p_chain->p_prod_elem = (void *)(((u8 *)p_chain->p_prod_elem) +
					p_chain->elem_size);

	return p_ret;
}


static inline u32 qed_chain_get_capacity(struct qed_chain *p_chain)
{
	return p_chain->capacity;
}


static inline void qed_chain_recycle_consumed(struct qed_chain *p_chain)
{
	test_and_skip(p_chain, prod_idx);
	if (is_chain_u16(p_chain))
		p_chain->u.chain16.prod_idx++;
	else
		p_chain->u.chain32.prod_idx++;
}


static inline void *qed_chain_consume(struct qed_chain *p_chain)
{
	void *p_ret = NULL, *p_cons_idx, *p_cons_page_idx;

	if (is_chain_u16(p_chain)) {
		if ((p_chain->u.chain16.cons_idx &
		     p_chain->elem_per_page_mask) == p_chain->next_page_mask) {
			p_cons_idx = &p_chain->u.chain16.cons_idx;
			p_cons_page_idx = &p_chain->pbl.c.u16.cons_page_idx;
			qed_chain_advance_page(p_chain, &p_chain->p_cons_elem,
					       p_cons_idx, p_cons_page_idx);
		}
		p_chain->u.chain16.cons_idx++;
	} else {
		if ((p_chain->u.chain32.cons_idx &
		     p_chain->elem_per_page_mask) == p_chain->next_page_mask) {
			p_cons_idx = &p_chain->u.chain32.cons_idx;
			p_cons_page_idx = &p_chain->pbl.c.u32.cons_page_idx;
			qed_chain_advance_page(p_chain, &p_chain->p_cons_elem,
					       p_cons_idx, p_cons_page_idx);
		}
		p_chain->u.chain32.cons_idx++;
	}

	p_ret = p_chain->p_cons_elem;
	p_chain->p_cons_elem = (void *)(((u8 *)p_chain->p_cons_elem) +
					p_chain->elem_size);

	return p_ret;
}


static inline void qed_chain_reset(struct qed_chain *p_chain)
{
	u32 i;

	if (is_chain_u16(p_chain)) {
		p_chain->u.chain16.prod_idx = 0;
		p_chain->u.chain16.cons_idx = 0;
	} else {
		p_chain->u.chain32.prod_idx = 0;
		p_chain->u.chain32.cons_idx = 0;
	}
	p_chain->p_cons_elem = p_chain->p_virt_addr;
	p_chain->p_prod_elem = p_chain->p_virt_addr;

	if (p_chain->mode == QED_CHAIN_MODE_PBL) {
		
		u32 reset_val = p_chain->page_cnt - 1;

		if (is_chain_u16(p_chain)) {
			p_chain->pbl.c.u16.prod_page_idx = (u16)reset_val;
			p_chain->pbl.c.u16.cons_page_idx = (u16)reset_val;
		} else {
			p_chain->pbl.c.u32.prod_page_idx = reset_val;
			p_chain->pbl.c.u32.cons_page_idx = reset_val;
		}
	}

	switch (p_chain->intended_use) {
	case QED_CHAIN_USE_TO_CONSUME:
		
		for (i = 0; i < p_chain->capacity; i++)
			qed_chain_recycle_consumed(p_chain);
		break;

	case QED_CHAIN_USE_TO_CONSUME_PRODUCE:
	case QED_CHAIN_USE_TO_PRODUCE:
	default:
		
		break;
	}
}


static inline void qed_chain_init_params(struct qed_chain *p_chain,
					 u32 page_cnt,
					 u8 elem_size,
					 enum qed_chain_use_mode intended_use,
					 enum qed_chain_mode mode,
					 enum qed_chain_cnt_type cnt_type)
{
	
	p_chain->p_virt_addr = NULL;
	p_chain->p_phys_addr = 0;
	p_chain->elem_size	= elem_size;
	p_chain->intended_use = (u8)intended_use;
	p_chain->mode		= mode;
	p_chain->cnt_type = (u8)cnt_type;

	p_chain->elem_per_page = ELEMS_PER_PAGE(elem_size);
	p_chain->usable_per_page = USABLE_ELEMS_PER_PAGE(elem_size, mode);
	p_chain->elem_per_page_mask = p_chain->elem_per_page - 1;
	p_chain->elem_unusable = UNUSABLE_ELEMS_PER_PAGE(elem_size, mode);
	p_chain->next_page_mask = (p_chain->usable_per_page &
				   p_chain->elem_per_page_mask);

	p_chain->page_cnt = page_cnt;
	p_chain->capacity = p_chain->usable_per_page * page_cnt;
	p_chain->size = p_chain->elem_per_page * page_cnt;

	p_chain->pbl_sp.p_phys_table = 0;
	p_chain->pbl_sp.p_virt_table = NULL;
	p_chain->pbl.pp_addr_tbl = NULL;
}


static inline void qed_chain_init_mem(struct qed_chain *p_chain,
				      void *p_virt_addr, dma_addr_t p_phys_addr)
{
	p_chain->p_virt_addr = p_virt_addr;
	p_chain->p_phys_addr = p_phys_addr;
}


static inline void qed_chain_init_pbl_mem(struct qed_chain *p_chain,
					  void *p_virt_pbl,
					  dma_addr_t p_phys_pbl,
					  struct addr_tbl_entry *pp_addr_tbl)
{
	p_chain->pbl_sp.p_phys_table = p_phys_pbl;
	p_chain->pbl_sp.p_virt_table = p_virt_pbl;
	p_chain->pbl.pp_addr_tbl = pp_addr_tbl;
}


static inline void
qed_chain_init_next_ptr_elem(struct qed_chain *p_chain,
			     void *p_virt_curr,
			     void *p_virt_next, dma_addr_t p_phys_next)
{
	struct qed_chain_next *p_next;
	u32 size;

	size = p_chain->elem_size * p_chain->usable_per_page;
	p_next = (struct qed_chain_next *)((u8 *)p_virt_curr + size);

	DMA_REGPAIR_LE(p_next->next_phys, p_phys_next);

	p_next->next_virt = p_virt_next;
}


static inline void *qed_chain_get_last_elem(struct qed_chain *p_chain)
{
	struct qed_chain_next *p_next = NULL;
	void *p_virt_addr = NULL;
	u32 size, last_page_idx;

	if (!p_chain->p_virt_addr)
		goto out;

	switch (p_chain->mode) {
	case QED_CHAIN_MODE_NEXT_PTR:
		size = p_chain->elem_size * p_chain->usable_per_page;
		p_virt_addr = p_chain->p_virt_addr;
		p_next = (struct qed_chain_next *)((u8 *)p_virt_addr + size);
		while (p_next->next_virt != p_chain->p_virt_addr) {
			p_virt_addr = p_next->next_virt;
			p_next = (struct qed_chain_next *)((u8 *)p_virt_addr +
							   size);
		}
		break;
	case QED_CHAIN_MODE_SINGLE:
		p_virt_addr = p_chain->p_virt_addr;
		break;
	case QED_CHAIN_MODE_PBL:
		last_page_idx = p_chain->page_cnt - 1;
		p_virt_addr = p_chain->pbl.pp_addr_tbl[last_page_idx].virt_addr;
		break;
	}
	
	size = p_chain->elem_size * (p_chain->usable_per_page - 1);
	p_virt_addr = (u8 *)p_virt_addr + size;
out:
	return p_virt_addr;
}


static inline void qed_chain_set_prod(struct qed_chain *p_chain,
				      u32 prod_idx, void *p_prod_elem)
{
	if (p_chain->mode == QED_CHAIN_MODE_PBL) {
		u32 cur_prod, page_mask, page_cnt, page_diff;

		cur_prod = is_chain_u16(p_chain) ? p_chain->u.chain16.prod_idx :
			   p_chain->u.chain32.prod_idx;

		
		page_mask = ~p_chain->elem_per_page_mask;

		
		page_diff = (((cur_prod - 1) & page_mask) -
			     ((prod_idx - 1) & page_mask)) /
			    p_chain->elem_per_page;

		page_cnt = qed_chain_get_page_cnt(p_chain);
		if (is_chain_u16(p_chain))
			p_chain->pbl.c.u16.prod_page_idx =
				(p_chain->pbl.c.u16.prod_page_idx -
				 page_diff + page_cnt) % page_cnt;
		else
			p_chain->pbl.c.u32.prod_page_idx =
				(p_chain->pbl.c.u32.prod_page_idx -
				 page_diff + page_cnt) % page_cnt;
	}

	if (is_chain_u16(p_chain))
		p_chain->u.chain16.prod_idx = (u16) prod_idx;
	else
		p_chain->u.chain32.prod_idx = prod_idx;
	p_chain->p_prod_elem = p_prod_elem;
}


static inline void qed_chain_pbl_zero_mem(struct qed_chain *p_chain)
{
	u32 i, page_cnt;

	if (p_chain->mode != QED_CHAIN_MODE_PBL)
		return;

	page_cnt = qed_chain_get_page_cnt(p_chain);

	for (i = 0; i < page_cnt; i++)
		memset(p_chain->pbl.pp_addr_tbl[i].virt_addr, 0,
		       QED_CHAIN_PAGE_SIZE);
}

#endif
