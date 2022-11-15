

#ifndef __LINUX_DMA_FENCE_ARRAY_H
#define __LINUX_DMA_FENCE_ARRAY_H

#include <linux/dma-fence.h>
#include <linux/irq_work.h>


struct dma_fence_array_cb {
	struct dma_fence_cb cb;
	struct dma_fence_array *array;
};


struct dma_fence_array {
	struct dma_fence base;

	spinlock_t lock;
	unsigned num_fences;
	atomic_t num_pending;
	struct dma_fence **fences;

	struct irq_work work;
};

extern const struct dma_fence_ops dma_fence_array_ops;


static inline bool dma_fence_is_array(struct dma_fence *fence)
{
	return fence->ops == &dma_fence_array_ops;
}


static inline struct dma_fence_array *
to_dma_fence_array(struct dma_fence *fence)
{
	if (fence->ops != &dma_fence_array_ops)
		return NULL;

	return container_of(fence, struct dma_fence_array, base);
}

struct dma_fence_array *dma_fence_array_create(int num_fences,
					       struct dma_fence **fences,
					       u64 context, unsigned seqno,
					       bool signal_on_any);

bool dma_fence_match_context(struct dma_fence *fence, u64 context);

#endif 
