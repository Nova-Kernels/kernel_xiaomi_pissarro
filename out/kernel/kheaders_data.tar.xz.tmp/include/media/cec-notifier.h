

#ifndef LINUX_CEC_NOTIFIER_H
#define LINUX_CEC_NOTIFIER_H

#include <linux/types.h>
#include <media/cec.h>

struct device;
struct edid;
struct cec_adapter;
struct cec_notifier;

#if IS_REACHABLE(CONFIG_CEC_CORE) && IS_ENABLED(CONFIG_CEC_NOTIFIER)


struct cec_notifier *cec_notifier_get(struct device *dev);


void cec_notifier_put(struct cec_notifier *n);


void cec_notifier_set_phys_addr(struct cec_notifier *n, u16 pa);


void cec_notifier_set_phys_addr_from_edid(struct cec_notifier *n,
					  const struct edid *edid);


void cec_notifier_register(struct cec_notifier *n,
			   struct cec_adapter *adap,
			   void (*callback)(struct cec_adapter *adap, u16 pa));


void cec_notifier_unregister(struct cec_notifier *n);


void cec_register_cec_notifier(struct cec_adapter *adap,
			       struct cec_notifier *notifier);

#else
static inline struct cec_notifier *cec_notifier_get(struct device *dev)
{
	
	return (struct cec_notifier *)0xdeadfeed;
}

static inline void cec_notifier_put(struct cec_notifier *n)
{
}

static inline void cec_notifier_set_phys_addr(struct cec_notifier *n, u16 pa)
{
}

static inline void cec_notifier_set_phys_addr_from_edid(struct cec_notifier *n,
							const struct edid *edid)
{
}

static inline void cec_notifier_register(struct cec_notifier *n,
			 struct cec_adapter *adap,
			 void (*callback)(struct cec_adapter *adap, u16 pa))
{
}

static inline void cec_notifier_unregister(struct cec_notifier *n)
{
}

static inline void cec_register_cec_notifier(struct cec_adapter *adap,
					     struct cec_notifier *notifier)
{
}
#endif


static inline void cec_notifier_phys_addr_invalidate(struct cec_notifier *n)
{
	cec_notifier_set_phys_addr(n, CEC_PHYS_ADDR_INVALID);
}

#endif
