

#ifndef __IDR_H__
#define __IDR_H__

#include <linux/radix-tree.h>
#include <linux/gfp.h>
#include <linux/percpu.h>

struct idr {
	struct radix_tree_root	idr_rt;
	unsigned int		idr_next;
};


#define IDR_FREE	0


#define IDR_RT_MARKER		((__force gfp_t)(3 << __GFP_BITS_SHIFT))

#define IDR_INIT							\
{									\
	.idr_rt = RADIX_TREE_INIT(IDR_RT_MARKER)			\
}
#define DEFINE_IDR(name)	struct idr name = IDR_INIT


static inline unsigned int idr_get_cursor(const struct idr *idr)
{
	return READ_ONCE(idr->idr_next);
}


static inline void idr_set_cursor(struct idr *idr, unsigned int val)
{
	WRITE_ONCE(idr->idr_next, val);
}



void idr_preload(gfp_t gfp_mask);

int idr_alloc_cmn(struct idr *idr, void *ptr, unsigned long *index,
		  unsigned long start, unsigned long end, gfp_t gfp,
		  bool ext);


static inline int idr_alloc(struct idr *idr, void *ptr,
			    int start, int end, gfp_t gfp)
{
	unsigned long id;
	int ret;

	if (WARN_ON_ONCE(start < 0))
		return -EINVAL;

	ret = idr_alloc_cmn(idr, ptr, &id, start, end, gfp, false);

	if (ret)
		return ret;

	return id;
}

static inline int idr_alloc_ext(struct idr *idr, void *ptr,
				unsigned long *index,
				unsigned long start,
				unsigned long end,
				gfp_t gfp)
{
	return idr_alloc_cmn(idr, ptr, index, start, end, gfp, true);
}

int idr_alloc_cyclic(struct idr *, void *entry, int start, int end, gfp_t);
int idr_for_each(const struct idr *,
		 int (*fn)(int id, void *p, void *data), void *data);
void *idr_get_next(struct idr *, int *nextid);
void *idr_get_next_ext(struct idr *idr, unsigned long *nextid);
void *idr_replace(struct idr *, void *, int id);
void *idr_replace_ext(struct idr *idr, void *ptr, unsigned long id);
void idr_destroy(struct idr *);

static inline void *idr_remove_ext(struct idr *idr, unsigned long id)
{
	return radix_tree_delete_item(&idr->idr_rt, id, NULL);
}

static inline void *idr_remove(struct idr *idr, int id)
{
	return idr_remove_ext(idr, id);
}

static inline void idr_init(struct idr *idr)
{
	INIT_RADIX_TREE(&idr->idr_rt, IDR_RT_MARKER);
	idr->idr_next = 0;
}

static inline bool idr_is_empty(const struct idr *idr)
{
	return radix_tree_empty(&idr->idr_rt) &&
		radix_tree_tagged(&idr->idr_rt, IDR_FREE);
}


static inline void idr_preload_end(void)
{
	preempt_enable();
}


static inline void *idr_find_ext(const struct idr *idr, unsigned long id)
{
	return radix_tree_lookup(&idr->idr_rt, id);
}

static inline void *idr_find(const struct idr *idr, int id)
{
	return idr_find_ext(idr, id);
}


#define idr_for_each_entry(idr, entry, id)			\
	for (id = 0; ((entry) = idr_get_next(idr, &(id))) != NULL; ++id)
#define idr_for_each_entry_ext(idr, entry, id)			\
	for (id = 0; ((entry) = idr_get_next_ext(idr, &(id))) != NULL; ++id)


#define idr_for_each_entry_continue(idr, entry, id)			\
	for ((entry) = idr_get_next((idr), &(id));			\
	     entry;							\
	     ++id, (entry) = idr_get_next((idr), &(id)))


#define IDA_CHUNK_SIZE		128	
#define IDA_BITMAP_LONGS	(IDA_CHUNK_SIZE / sizeof(long))
#define IDA_BITMAP_BITS 	(IDA_BITMAP_LONGS * sizeof(long) * 8)

struct ida_bitmap {
	unsigned long		bitmap[IDA_BITMAP_LONGS];
};

DECLARE_PER_CPU(struct ida_bitmap *, ida_bitmap);

struct ida {
	struct radix_tree_root	ida_rt;
};

#define IDA_INIT	{						\
	.ida_rt = RADIX_TREE_INIT(IDR_RT_MARKER | GFP_NOWAIT),		\
}
#define DEFINE_IDA(name)	struct ida name = IDA_INIT

int ida_pre_get(struct ida *ida, gfp_t gfp_mask);
int ida_get_new_above(struct ida *ida, int starting_id, int *p_id);
void ida_remove(struct ida *ida, int id);
void ida_destroy(struct ida *ida);

int ida_simple_get(struct ida *ida, unsigned int start, unsigned int end,
		   gfp_t gfp_mask);
void ida_simple_remove(struct ida *ida, unsigned int id);

static inline void ida_init(struct ida *ida)
{
	INIT_RADIX_TREE(&ida->ida_rt, IDR_RT_MARKER | GFP_NOWAIT);
}


static inline int ida_get_new(struct ida *ida, int *p_id)
{
	return ida_get_new_above(ida, 0, p_id);
}

static inline bool ida_is_empty(const struct ida *ida)
{
	return radix_tree_empty(&ida->ida_rt);
}
#endif 
