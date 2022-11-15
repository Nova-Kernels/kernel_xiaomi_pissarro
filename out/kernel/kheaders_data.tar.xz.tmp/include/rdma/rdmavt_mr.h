#ifndef DEF_RDMAVT_INCMR_H
#define DEF_RDMAVT_INCMR_H




#include <linux/percpu-refcount.h>


struct rvt_seg {
	void *vaddr;
	size_t length;
};


#define RVT_SEGSZ     (PAGE_SIZE / sizeof(struct rvt_seg))

struct rvt_segarray {
	struct rvt_seg segs[RVT_SEGSZ];
};

struct rvt_mregion {
	struct ib_pd *pd;       
	u64 user_base;          
	u64 iova;               
	size_t length;
	u32 lkey;
	u32 offset;             
	int access_flags;
	u32 max_segs;           
	u32 mapsz;              
	atomic_t lkey_invalid;	
	u8  page_shift;         
	u8  lkey_published;     
	struct percpu_ref refcount;
	struct completion comp; 
	struct rvt_segarray *map[0];    
};

#define RVT_MAX_LKEY_TABLE_BITS 23

struct rvt_lkey_table {
	
	u32 max;                
	u32 shift;              
	struct rvt_mregion __rcu **table;
	
	
	spinlock_t lock ____cacheline_aligned_in_smp;
	u32 next;               
	u32 gen;                
};


struct rvt_sge {
	struct rvt_mregion *mr;
	void *vaddr;            
	u32 sge_length;         
	u32 length;             
	u16 m;                  
	u16 n;                  
};

struct rvt_sge_state {
	struct rvt_sge *sg_list;      
	struct rvt_sge sge;   
	u32 total_len;
	u8 num_sge;
};

static inline void rvt_put_mr(struct rvt_mregion *mr)
{
	percpu_ref_put(&mr->refcount);
}

static inline void rvt_get_mr(struct rvt_mregion *mr)
{
	percpu_ref_get(&mr->refcount);
}

static inline void rvt_put_ss(struct rvt_sge_state *ss)
{
	while (ss->num_sge) {
		rvt_put_mr(ss->sge.mr);
		if (--ss->num_sge)
			ss->sge = *ss->sg_list++;
	}
}

static inline u32 rvt_get_sge_length(struct rvt_sge *sge, u32 length)
{
	u32 len = sge->length;

	if (len > length)
		len = length;
	if (len > sge->sge_length)
		len = sge->sge_length;

	return len;
}

static inline void rvt_update_sge(struct rvt_sge_state *ss, u32 length,
				  bool release)
{
	struct rvt_sge *sge = &ss->sge;

	sge->vaddr += length;
	sge->length -= length;
	sge->sge_length -= length;
	if (sge->sge_length == 0) {
		if (release)
			rvt_put_mr(sge->mr);
		if (--ss->num_sge)
			*sge = *ss->sg_list++;
	} else if (sge->length == 0 && sge->mr->lkey) {
		if (++sge->n >= RVT_SEGSZ) {
			if (++sge->m >= sge->mr->mapsz)
				return;
			sge->n = 0;
		}
		sge->vaddr = sge->mr->map[sge->m]->segs[sge->n].vaddr;
		sge->length = sge->mr->map[sge->m]->segs[sge->n].length;
	}
}

static inline void rvt_skip_sge(struct rvt_sge_state *ss, u32 length,
				bool release)
{
	struct rvt_sge *sge = &ss->sge;

	while (length) {
		u32 len = rvt_get_sge_length(sge, length);

		WARN_ON_ONCE(len == 0);
		rvt_update_sge(ss, len, release);
		length -= len;
	}
}

bool rvt_ss_has_lkey(struct rvt_sge_state *ss, u32 lkey);
bool rvt_mr_has_lkey(struct rvt_mregion *mr, u32 lkey);

#endif          
