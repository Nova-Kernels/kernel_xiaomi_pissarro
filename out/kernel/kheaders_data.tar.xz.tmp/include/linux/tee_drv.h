

#ifndef __TEE_DRV_H
#define __TEE_DRV_H

#include <linux/types.h>
#include <linux/idr.h>
#include <linux/list.h>
#include <linux/tee.h>



#define TEE_SHM_MAPPED		0x1	
#define TEE_SHM_DMA_BUF		0x2	

struct device;
struct tee_device;
struct tee_shm;
struct tee_shm_pool;


struct tee_context {
	struct tee_device *teedev;
	struct list_head list_shm;
	void *data;
};

struct tee_param_memref {
	size_t shm_offs;
	size_t size;
	struct tee_shm *shm;
};

struct tee_param_value {
	u64 a;
	u64 b;
	u64 c;
};

struct tee_param {
	u64 attr;
	union {
		struct tee_param_memref memref;
		struct tee_param_value value;
	} u;
};


struct tee_driver_ops {
	void (*get_version)(struct tee_device *teedev,
			    struct tee_ioctl_version_data *vers);
	int (*open)(struct tee_context *ctx);
	void (*release)(struct tee_context *ctx);
	int (*open_session)(struct tee_context *ctx,
			    struct tee_ioctl_open_session_arg *arg,
			    struct tee_param *param);
	int (*close_session)(struct tee_context *ctx, u32 session);
	int (*invoke_func)(struct tee_context *ctx,
			   struct tee_ioctl_invoke_arg *arg,
			   struct tee_param *param);
	int (*cancel_req)(struct tee_context *ctx, u32 cancel_id, u32 session);
	int (*supp_recv)(struct tee_context *ctx, u32 *func, u32 *num_params,
			 struct tee_param *param);
	int (*supp_send)(struct tee_context *ctx, u32 ret, u32 num_params,
			 struct tee_param *param);
};


#define TEE_DESC_PRIVILEGED	0x1
struct tee_desc {
	const char *name;
	const struct tee_driver_ops *ops;
	struct module *owner;
	u32 flags;
};


struct tee_device *tee_device_alloc(const struct tee_desc *teedesc,
				    struct device *dev,
				    struct tee_shm_pool *pool,
				    void *driver_data);


int tee_device_register(struct tee_device *teedev);


void tee_device_unregister(struct tee_device *teedev);


struct tee_shm_pool_mem_info {
	unsigned long vaddr;
	phys_addr_t paddr;
	size_t size;
};


struct tee_shm_pool *
tee_shm_pool_alloc_res_mem(struct tee_shm_pool_mem_info *priv_info,
			   struct tee_shm_pool_mem_info *dmabuf_info);


void tee_shm_pool_free(struct tee_shm_pool *pool);


void *tee_get_drvdata(struct tee_device *teedev);


struct tee_shm *tee_shm_alloc(struct tee_context *ctx, size_t size, u32 flags);


void tee_shm_free(struct tee_shm *shm);


void tee_shm_put(struct tee_shm *shm);


int tee_shm_va2pa(struct tee_shm *shm, void *va, phys_addr_t *pa);


int tee_shm_pa2va(struct tee_shm *shm, phys_addr_t pa, void **va);


void *tee_shm_get_va(struct tee_shm *shm, size_t offs);


int tee_shm_get_pa(struct tee_shm *shm, size_t offs, phys_addr_t *pa);


int tee_shm_get_id(struct tee_shm *shm);


struct tee_shm *tee_shm_get_from_id(struct tee_context *ctx, int id);

#endif 
