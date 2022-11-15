


#ifndef _DRM_MM_H_
#define _DRM_MM_H_


#include <linux/bug.h>
#include <linux/rbtree.h>
#include <linux/kernel.h>
#include <linux/mm_types.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#ifdef CONFIG_DRM_DEBUG_MM
#include <linux/stackdepot.h>
#endif
#include <drm/drm_print.h>

#ifdef CONFIG_DRM_DEBUG_MM
#define DRM_MM_BUG_ON(expr) BUG_ON(expr)
#else
#define DRM_MM_BUG_ON(expr) BUILD_BUG_ON_INVALID(expr)
#endif


enum drm_mm_insert_mode {
	
	DRM_MM_INSERT_BEST = 0,

	
	DRM_MM_INSERT_LOW,

	
	DRM_MM_INSERT_HIGH,

	
	DRM_MM_INSERT_EVICT,
};


struct drm_mm_node {
	
	unsigned long color;
	
	u64 start;
	
	u64 size;
	
	struct drm_mm *mm;
	struct list_head node_list;
	struct list_head hole_stack;
	struct rb_node rb;
	struct rb_node rb_hole_size;
	struct rb_node rb_hole_addr;
	u64 __subtree_last;
	u64 hole_size;
	bool allocated : 1;
	bool scanned_block : 1;
#ifdef CONFIG_DRM_DEBUG_MM
	depot_stack_handle_t stack;
#endif
};


struct drm_mm {
	
	void (*color_adjust)(const struct drm_mm_node *node,
			     unsigned long color,
			     u64 *start, u64 *end);

	
	
	struct list_head hole_stack;
	
	struct drm_mm_node head_node;
	
	struct rb_root_cached interval_tree;
	struct rb_root holes_size;
	struct rb_root holes_addr;

	unsigned long scan_active;
};


struct drm_mm_scan {
	
	struct drm_mm *mm;

	u64 size;
	u64 alignment;
	u64 remainder_mask;

	u64 range_start;
	u64 range_end;

	u64 hit_start;
	u64 hit_end;

	unsigned long color;
	enum drm_mm_insert_mode mode;
};


static inline bool drm_mm_node_allocated(const struct drm_mm_node *node)
{
	return node->allocated;
}


static inline bool drm_mm_initialized(const struct drm_mm *mm)
{
	return mm->hole_stack.next;
}


static inline bool drm_mm_hole_follows(const struct drm_mm_node *node)
{
	return node->hole_size;
}

static inline u64 __drm_mm_hole_node_start(const struct drm_mm_node *hole_node)
{
	return hole_node->start + hole_node->size;
}


static inline u64 drm_mm_hole_node_start(const struct drm_mm_node *hole_node)
{
	DRM_MM_BUG_ON(!drm_mm_hole_follows(hole_node));
	return __drm_mm_hole_node_start(hole_node);
}

static inline u64 __drm_mm_hole_node_end(const struct drm_mm_node *hole_node)
{
	return list_next_entry(hole_node, node_list)->start;
}


static inline u64 drm_mm_hole_node_end(const struct drm_mm_node *hole_node)
{
	return __drm_mm_hole_node_end(hole_node);
}


#define drm_mm_nodes(mm) (&(mm)->head_node.node_list)


#define drm_mm_for_each_node(entry, mm) \
	list_for_each_entry(entry, drm_mm_nodes(mm), node_list)


#define drm_mm_for_each_node_safe(entry, next, mm) \
	list_for_each_entry_safe(entry, next, drm_mm_nodes(mm), node_list)


#define drm_mm_for_each_hole(pos, mm, hole_start, hole_end) \
	for (pos = list_first_entry(&(mm)->hole_stack, \
				    typeof(*pos), hole_stack); \
	     &pos->hole_stack != &(mm)->hole_stack ? \
	     hole_start = drm_mm_hole_node_start(pos), \
	     hole_end = hole_start + pos->hole_size, \
	     1 : 0; \
	     pos = list_next_entry(pos, hole_stack))


int drm_mm_reserve_node(struct drm_mm *mm, struct drm_mm_node *node);
int drm_mm_insert_node_in_range(struct drm_mm *mm,
				struct drm_mm_node *node,
				u64 size,
				u64 alignment,
				unsigned long color,
				u64 start,
				u64 end,
				enum drm_mm_insert_mode mode);


static inline int
drm_mm_insert_node_generic(struct drm_mm *mm, struct drm_mm_node *node,
			   u64 size, u64 alignment,
			   unsigned long color,
			   enum drm_mm_insert_mode mode)
{
	return drm_mm_insert_node_in_range(mm, node,
					   size, alignment, color,
					   0, U64_MAX, mode);
}


static inline int drm_mm_insert_node(struct drm_mm *mm,
				     struct drm_mm_node *node,
				     u64 size)
{
	return drm_mm_insert_node_generic(mm, node, size, 0, 0, 0);
}

void drm_mm_remove_node(struct drm_mm_node *node);
void drm_mm_replace_node(struct drm_mm_node *old, struct drm_mm_node *new);
void drm_mm_init(struct drm_mm *mm, u64 start, u64 size);
void drm_mm_takedown(struct drm_mm *mm);


static inline bool drm_mm_clean(const struct drm_mm *mm)
{
	return list_empty(drm_mm_nodes(mm));
}

struct drm_mm_node *
__drm_mm_interval_first(const struct drm_mm *mm, u64 start, u64 last);


#define drm_mm_for_each_node_in_range(node__, mm__, start__, end__)	\
	for (node__ = __drm_mm_interval_first((mm__), (start__), (end__)-1); \
	     node__->start < (end__);					\
	     node__ = list_next_entry(node__, node_list))

void drm_mm_scan_init_with_range(struct drm_mm_scan *scan,
				 struct drm_mm *mm,
				 u64 size, u64 alignment, unsigned long color,
				 u64 start, u64 end,
				 enum drm_mm_insert_mode mode);


static inline void drm_mm_scan_init(struct drm_mm_scan *scan,
				    struct drm_mm *mm,
				    u64 size,
				    u64 alignment,
				    unsigned long color,
				    enum drm_mm_insert_mode mode)
{
	drm_mm_scan_init_with_range(scan, mm,
				    size, alignment, color,
				    0, U64_MAX, mode);
}

bool drm_mm_scan_add_block(struct drm_mm_scan *scan,
			   struct drm_mm_node *node);
bool drm_mm_scan_remove_block(struct drm_mm_scan *scan,
			      struct drm_mm_node *node);
struct drm_mm_node *drm_mm_scan_color_evict(struct drm_mm_scan *scan);

void drm_mm_print(const struct drm_mm *mm, struct drm_printer *p);

#endif
