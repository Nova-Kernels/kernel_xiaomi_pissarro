

#ifndef __LINUX_DMA_FENCE_H
#define __LINUX_DMA_FENCE_H

#include <linux/err.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <linux/kref.h>
#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/rcupdate.h>

struct dma_fence;
struct dma_fence_ops;
struct dma_fence_cb;


struct dma_fence {
	struct kref refcount;
	const struct dma_fence_ops *ops;
	struct rcu_head rcu;
	struct list_head cb_list;
	spinlock_t *lock;
	u64 context;
	unsigned seqno;
	unsigned long flags;
	ktime_t timestamp;
	int error;
};

enum dma_fence_flag_bits {
	DMA_FENCE_FLAG_SIGNALED_BIT,
	DMA_FENCE_FLAG_TIMESTAMP_BIT,
	DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT,
	DMA_FENCE_FLAG_USER_BITS, 
};

typedef void (*dma_fence_func_t)(struct dma_fence *fence,
				 struct dma_fence_cb *cb);


struct dma_fence_cb {
	struct list_head node;
	dma_fence_func_t func;
};



struct dma_fence_ops {
	const char * (*get_driver_name)(struct dma_fence *fence);
	const char * (*get_timeline_name)(struct dma_fence *fence);
	bool (*enable_signaling)(struct dma_fence *fence);
	void (*disable_signaling)(struct dma_fence *fence);
	bool (*signaled)(struct dma_fence *fence);
	signed long (*wait)(struct dma_fence *fence,
			    bool intr, signed long timeout);
	void (*release)(struct dma_fence *fence);

	int (*fill_driver_data)(struct dma_fence *fence, void *data, int size);
	void (*fence_value_str)(struct dma_fence *fence, char *str, int size);
	void (*timeline_value_str)(struct dma_fence *fence,
				   char *str, int size);
};

void dma_fence_init(struct dma_fence *fence, const struct dma_fence_ops *ops,
		    spinlock_t *lock, u64 context, unsigned seqno);

void dma_fence_release(struct kref *kref);
void dma_fence_free(struct dma_fence *fence);


static inline void dma_fence_put(struct dma_fence *fence)
{
	if (fence)
		kref_put(&fence->refcount, dma_fence_release);
}


static inline struct dma_fence *dma_fence_get(struct dma_fence *fence)
{
	if (fence)
		kref_get(&fence->refcount);
	return fence;
}


static inline struct dma_fence *dma_fence_get_rcu(struct dma_fence *fence)
{
	if (kref_get_unless_zero(&fence->refcount))
		return fence;
	else
		return NULL;
}


static inline struct dma_fence *
dma_fence_get_rcu_safe(struct dma_fence * __rcu *fencep)
{
	do {
		struct dma_fence *fence;

		fence = rcu_dereference(*fencep);
		if (!fence || !dma_fence_get_rcu(fence))
			return NULL;

		
		if (fence == rcu_access_pointer(*fencep))
			return rcu_pointer_handoff(fence);

		dma_fence_put(fence);
	} while (1);
}

int dma_fence_signal(struct dma_fence *fence);
int dma_fence_signal_locked(struct dma_fence *fence);
signed long dma_fence_default_wait(struct dma_fence *fence,
				   bool intr, signed long timeout);
int dma_fence_add_callback(struct dma_fence *fence,
			   struct dma_fence_cb *cb,
			   dma_fence_func_t func);
bool dma_fence_remove_callback(struct dma_fence *fence,
			       struct dma_fence_cb *cb);
void dma_fence_enable_sw_signaling(struct dma_fence *fence);


static inline bool
dma_fence_is_signaled_locked(struct dma_fence *fence)
{
	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return true;

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		dma_fence_signal_locked(fence);
		return true;
	}

	return false;
}


static inline bool
dma_fence_is_signaled(struct dma_fence *fence)
{
	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return true;

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		dma_fence_signal(fence);
		return true;
	}

	return false;
}


static inline bool __dma_fence_is_later(u32 f1, u32 f2)
{
	return (int)(f1 - f2) > 0;
}


static inline bool dma_fence_is_later(struct dma_fence *f1,
				      struct dma_fence *f2)
{
	if (WARN_ON(f1->context != f2->context))
		return false;

	return __dma_fence_is_later(f1->seqno, f2->seqno);
}


static inline struct dma_fence *dma_fence_later(struct dma_fence *f1,
						struct dma_fence *f2)
{
	if (WARN_ON(f1->context != f2->context))
		return NULL;

	
	if (dma_fence_is_later(f1, f2))
		return dma_fence_is_signaled(f1) ? NULL : f1;
	else
		return dma_fence_is_signaled(f2) ? NULL : f2;
}


static inline int dma_fence_get_status_locked(struct dma_fence *fence)
{
	if (dma_fence_is_signaled_locked(fence))
		return fence->error ?: 1;
	else
		return 0;
}

int dma_fence_get_status(struct dma_fence *fence);


static inline void dma_fence_set_error(struct dma_fence *fence,
				       int error)
{
	WARN_ON(test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags));
	WARN_ON(error >= 0 || error < -MAX_ERRNO);

	fence->error = error;
}

signed long dma_fence_wait_timeout(struct dma_fence *,
				   bool intr, signed long timeout);
signed long dma_fence_wait_any_timeout(struct dma_fence **fences,
				       uint32_t count,
				       bool intr, signed long timeout,
				       uint32_t *idx);


static inline signed long dma_fence_wait(struct dma_fence *fence, bool intr)
{
	signed long ret;

	
	ret = dma_fence_wait_timeout(fence, intr, MAX_SCHEDULE_TIMEOUT);

	return ret < 0 ? ret : 0;
}

u64 dma_fence_context_alloc(unsigned num);

#define DMA_FENCE_TRACE(f, fmt, args...) \
	do {								\
		struct dma_fence *__ff = (f);				\
		if (IS_ENABLED(CONFIG_DMA_FENCE_TRACE))			\
			pr_info("f %llu#%u: " fmt,			\
				__ff->context, __ff->seqno, ##args);	\
	} while (0)

#define DMA_FENCE_WARN(f, fmt, args...) \
	do {								\
		struct dma_fence *__ff = (f);				\
		pr_warn("f %llu#%u: " fmt, __ff->context, __ff->seqno,	\
			 ##args);					\
	} while (0)

#define DMA_FENCE_ERR(f, fmt, args...) \
	do {								\
		struct dma_fence *__ff = (f);				\
		pr_err("f %llu#%u: " fmt, __ff->context, __ff->seqno,	\
			##args);					\
	} while (0)

#endif 
