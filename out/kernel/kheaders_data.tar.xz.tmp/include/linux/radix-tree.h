
#ifndef _LINUX_RADIX_TREE_H
#define _LINUX_RADIX_TREE_H

#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/preempt.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>
#include <linux/types.h>


#define RADIX_TREE_ENTRY_MASK		3UL
#define RADIX_TREE_INTERNAL_NODE	1UL


#define RADIX_TREE_EXCEPTIONAL_ENTRY	2
#define RADIX_TREE_EXCEPTIONAL_SHIFT	2

static inline bool radix_tree_is_internal_node(void *ptr)
{
	return ((unsigned long)ptr & RADIX_TREE_ENTRY_MASK) ==
				RADIX_TREE_INTERNAL_NODE;
}



#define RADIX_TREE_MAX_TAGS 3

#ifndef RADIX_TREE_MAP_SHIFT
#define RADIX_TREE_MAP_SHIFT	(CONFIG_BASE_SMALL ? 4 : 6)
#endif

#define RADIX_TREE_MAP_SIZE	(1UL << RADIX_TREE_MAP_SHIFT)
#define RADIX_TREE_MAP_MASK	(RADIX_TREE_MAP_SIZE-1)

#define RADIX_TREE_TAG_LONGS	\
	((RADIX_TREE_MAP_SIZE + BITS_PER_LONG - 1) / BITS_PER_LONG)

#define RADIX_TREE_INDEX_BITS  (8  * sizeof(unsigned long))
#define RADIX_TREE_MAX_PATH (DIV_ROUND_UP(RADIX_TREE_INDEX_BITS, \
					  RADIX_TREE_MAP_SHIFT))


struct radix_tree_node {
	unsigned char	shift;		
	unsigned char	offset;		
	unsigned char	count;		
	unsigned char	exceptional;	
	struct radix_tree_node *parent;		
	struct radix_tree_root *root;		
	union {
		struct list_head private_list;	
		struct rcu_head	rcu_head;	
	};
	void __rcu	*slots[RADIX_TREE_MAP_SIZE];
	unsigned long	tags[RADIX_TREE_MAX_TAGS][RADIX_TREE_TAG_LONGS];
};


#define ROOT_IS_IDR	((__force gfp_t)(1 << __GFP_BITS_SHIFT))
#define ROOT_TAG_SHIFT	(__GFP_BITS_SHIFT + 1)

struct radix_tree_root {
	gfp_t			gfp_mask;
	struct radix_tree_node	__rcu *rnode;
};

#define RADIX_TREE_INIT(mask)	{					\
	.gfp_mask = (mask),						\
	.rnode = NULL,							\
}

#define RADIX_TREE(name, mask) \
	struct radix_tree_root name = RADIX_TREE_INIT(mask)

#define INIT_RADIX_TREE(root, mask)					\
do {									\
	(root)->gfp_mask = (mask);					\
	(root)->rnode = NULL;						\
} while (0)

static inline bool radix_tree_empty(const struct radix_tree_root *root)
{
	return root->rnode == NULL;
}


struct radix_tree_iter {
	unsigned long	index;
	unsigned long	next_index;
	unsigned long	tags;
	struct radix_tree_node *node;
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	unsigned int	shift;
#endif
};

static inline unsigned int iter_shift(const struct radix_tree_iter *iter)
{
#ifdef CONFIG_RADIX_TREE_MULTIORDER
	return iter->shift;
#else
	return 0;
#endif
}




static inline void *radix_tree_deref_slot(void __rcu **slot)
{
	return rcu_dereference(*slot);
}


static inline void *radix_tree_deref_slot_protected(void __rcu **slot,
							spinlock_t *treelock)
{
	return rcu_dereference_protected(*slot, lockdep_is_held(treelock));
}


static inline int radix_tree_deref_retry(void *arg)
{
	return unlikely(radix_tree_is_internal_node(arg));
}


static inline int radix_tree_exceptional_entry(void *arg)
{
	
	return (unsigned long)arg & RADIX_TREE_EXCEPTIONAL_ENTRY;
}


static inline int radix_tree_exception(void *arg)
{
	return unlikely((unsigned long)arg & RADIX_TREE_ENTRY_MASK);
}

int __radix_tree_create(struct radix_tree_root *, unsigned long index,
			unsigned order, struct radix_tree_node **nodep,
			void __rcu ***slotp);
int __radix_tree_insert(struct radix_tree_root *, unsigned long index,
			unsigned order, void *);
static inline int radix_tree_insert(struct radix_tree_root *root,
			unsigned long index, void *entry)
{
	return __radix_tree_insert(root, index, 0, entry);
}
void *__radix_tree_lookup(const struct radix_tree_root *, unsigned long index,
			  struct radix_tree_node **nodep, void __rcu ***slotp);
void *radix_tree_lookup(const struct radix_tree_root *, unsigned long);
void __rcu **radix_tree_lookup_slot(const struct radix_tree_root *,
					unsigned long index);
typedef void (*radix_tree_update_node_t)(struct radix_tree_node *, void *);
void __radix_tree_replace(struct radix_tree_root *, struct radix_tree_node *,
			  void __rcu **slot, void *entry,
			  radix_tree_update_node_t update_node, void *private);
void radix_tree_iter_replace(struct radix_tree_root *,
		const struct radix_tree_iter *, void __rcu **slot, void *entry);
void radix_tree_replace_slot(struct radix_tree_root *,
			     void __rcu **slot, void *entry);
void __radix_tree_delete_node(struct radix_tree_root *,
			      struct radix_tree_node *,
			      radix_tree_update_node_t update_node,
			      void *private);
void radix_tree_iter_delete(struct radix_tree_root *,
			struct radix_tree_iter *iter, void __rcu **slot);
void *radix_tree_delete_item(struct radix_tree_root *, unsigned long, void *);
void *radix_tree_delete(struct radix_tree_root *, unsigned long);
void radix_tree_clear_tags(struct radix_tree_root *, struct radix_tree_node *,
			   void __rcu **slot);
unsigned int radix_tree_gang_lookup(const struct radix_tree_root *,
			void **results, unsigned long first_index,
			unsigned int max_items);
unsigned int radix_tree_gang_lookup_slot(const struct radix_tree_root *,
			void __rcu ***results, unsigned long *indices,
			unsigned long first_index, unsigned int max_items);
int radix_tree_preload(gfp_t gfp_mask);
int radix_tree_maybe_preload(gfp_t gfp_mask);
int radix_tree_maybe_preload_order(gfp_t gfp_mask, int order);
void radix_tree_init(void);
void *radix_tree_tag_set(struct radix_tree_root *,
			unsigned long index, unsigned int tag);
void *radix_tree_tag_clear(struct radix_tree_root *,
			unsigned long index, unsigned int tag);
int radix_tree_tag_get(const struct radix_tree_root *,
			unsigned long index, unsigned int tag);
void radix_tree_iter_tag_set(struct radix_tree_root *,
		const struct radix_tree_iter *iter, unsigned int tag);
void radix_tree_iter_tag_clear(struct radix_tree_root *,
		const struct radix_tree_iter *iter, unsigned int tag);
unsigned int radix_tree_gang_lookup_tag(const struct radix_tree_root *,
		void **results, unsigned long first_index,
		unsigned int max_items, unsigned int tag);
unsigned int radix_tree_gang_lookup_tag_slot(const struct radix_tree_root *,
		void __rcu ***results, unsigned long first_index,
		unsigned int max_items, unsigned int tag);
int radix_tree_tagged(const struct radix_tree_root *, unsigned int tag);

static inline void radix_tree_preload_end(void)
{
	preempt_enable();
}

int radix_tree_split_preload(unsigned old_order, unsigned new_order, gfp_t);
int radix_tree_split(struct radix_tree_root *, unsigned long index,
			unsigned new_order);
int radix_tree_join(struct radix_tree_root *, unsigned long index,
			unsigned new_order, void *);

void __rcu **idr_get_free_cmn(struct radix_tree_root *root,
			      struct radix_tree_iter *iter, gfp_t gfp,
			      unsigned long max);
static inline void __rcu **idr_get_free(struct radix_tree_root *root,
					struct radix_tree_iter *iter,
					gfp_t gfp,
					int end)
{
	return idr_get_free_cmn(root, iter, gfp, end > 0 ? end - 1 : INT_MAX);
}

static inline void __rcu **idr_get_free_ext(struct radix_tree_root *root,
					    struct radix_tree_iter *iter,
					    gfp_t gfp,
					    unsigned long end)
{
	return idr_get_free_cmn(root, iter, gfp, end - 1);
}

enum {
	RADIX_TREE_ITER_TAG_MASK = 0x0f,	
	RADIX_TREE_ITER_TAGGED   = 0x10,	
	RADIX_TREE_ITER_CONTIG   = 0x20,	
};


static __always_inline void __rcu **
radix_tree_iter_init(struct radix_tree_iter *iter, unsigned long start)
{
	
	iter->index = 0;
	iter->next_index = start;
	return NULL;
}


void __rcu **radix_tree_next_chunk(const struct radix_tree_root *,
			     struct radix_tree_iter *iter, unsigned flags);


static inline void __rcu **
radix_tree_iter_lookup(const struct radix_tree_root *root,
			struct radix_tree_iter *iter, unsigned long index)
{
	radix_tree_iter_init(iter, index);
	return radix_tree_next_chunk(root, iter, RADIX_TREE_ITER_CONTIG);
}


static inline void __rcu **
radix_tree_iter_find(const struct radix_tree_root *root,
			struct radix_tree_iter *iter, unsigned long index)
{
	radix_tree_iter_init(iter, index);
	return radix_tree_next_chunk(root, iter, 0);
}


static inline __must_check
void __rcu **radix_tree_iter_retry(struct radix_tree_iter *iter)
{
	iter->next_index = iter->index;
	iter->tags = 0;
	return NULL;
}

static inline unsigned long
__radix_tree_iter_add(struct radix_tree_iter *iter, unsigned long slots)
{
	return iter->index + (slots << iter_shift(iter));
}


void __rcu **__must_check radix_tree_iter_resume(void __rcu **slot,
					struct radix_tree_iter *iter);


static __always_inline long
radix_tree_chunk_size(struct radix_tree_iter *iter)
{
	return (iter->next_index - iter->index) >> iter_shift(iter);
}

#ifdef CONFIG_RADIX_TREE_MULTIORDER
void __rcu **__radix_tree_next_slot(void __rcu **slot,
				struct radix_tree_iter *iter, unsigned flags);
#else

static inline void __rcu **__radix_tree_next_slot(void __rcu **slot,
				struct radix_tree_iter *iter, unsigned flags)
{
	return slot;
}
#endif


static __always_inline void __rcu **radix_tree_next_slot(void __rcu **slot,
				struct radix_tree_iter *iter, unsigned flags)
{
	if (flags & RADIX_TREE_ITER_TAGGED) {
		iter->tags >>= 1;
		if (unlikely(!iter->tags))
			return NULL;
		if (likely(iter->tags & 1ul)) {
			iter->index = __radix_tree_iter_add(iter, 1);
			slot++;
			goto found;
		}
		if (!(flags & RADIX_TREE_ITER_CONTIG)) {
			unsigned offset = __ffs(iter->tags);

			iter->tags >>= offset++;
			iter->index = __radix_tree_iter_add(iter, offset);
			slot += offset;
			goto found;
		}
	} else {
		long count = radix_tree_chunk_size(iter);

		while (--count > 0) {
			slot++;
			iter->index = __radix_tree_iter_add(iter, 1);

			if (likely(*slot))
				goto found;
			if (flags & RADIX_TREE_ITER_CONTIG) {
				
				iter->next_index = 0;
				break;
			}
		}
	}
	return NULL;

 found:
	if (unlikely(radix_tree_is_internal_node(rcu_dereference_raw(*slot))))
		return __radix_tree_next_slot(slot, iter, flags);
	return slot;
}


#define radix_tree_for_each_slot(slot, root, iter, start)		\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter, 0)) ;	\
	     slot = radix_tree_next_slot(slot, iter, 0))


#define radix_tree_for_each_contig(slot, root, iter, start)		\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter,		\
				RADIX_TREE_ITER_CONTIG)) ;		\
	     slot = radix_tree_next_slot(slot, iter,			\
				RADIX_TREE_ITER_CONTIG))


#define radix_tree_for_each_tagged(slot, root, iter, start, tag)	\
	for (slot = radix_tree_iter_init(iter, start) ;			\
	     slot || (slot = radix_tree_next_chunk(root, iter,		\
			      RADIX_TREE_ITER_TAGGED | tag)) ;		\
	     slot = radix_tree_next_slot(slot, iter,			\
				RADIX_TREE_ITER_TAGGED | tag))

#endif 
