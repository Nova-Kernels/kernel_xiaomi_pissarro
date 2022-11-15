


#ifndef _CRYPTO_GF128MUL_H
#define _CRYPTO_GF128MUL_H

#include <asm/byteorder.h>
#include <crypto/b128ops.h>
#include <linux/slab.h>





void gf128mul_lle(be128 *a, const be128 *b);

void gf128mul_bbe(be128 *a, const be128 *b);



static inline u64 gf128mul_mask_from_bit(u64 x, int which)
{
	
	return ((s64)(x << (63 - which)) >> 63);
}

static inline void gf128mul_x_lle(be128 *r, const be128 *x)
{
	u64 a = be64_to_cpu(x->a);
	u64 b = be64_to_cpu(x->b);

	
	u64 _tt = gf128mul_mask_from_bit(b, 0) & ((u64)0xe1 << 56);

	r->b = cpu_to_be64((b >> 1) | (a << 63));
	r->a = cpu_to_be64((a >> 1) ^ _tt);
}

static inline void gf128mul_x_bbe(be128 *r, const be128 *x)
{
	u64 a = be64_to_cpu(x->a);
	u64 b = be64_to_cpu(x->b);

	
	u64 _tt = gf128mul_mask_from_bit(a, 63) & 0x87;

	r->a = cpu_to_be64((a << 1) | (b >> 63));
	r->b = cpu_to_be64((b << 1) ^ _tt);
}


static inline void gf128mul_x_ble(le128 *r, const le128 *x)
{
	u64 a = le64_to_cpu(x->a);
	u64 b = le64_to_cpu(x->b);

	
	u64 _tt = gf128mul_mask_from_bit(a, 63) & 0x87;

	r->a = cpu_to_le64((a << 1) | (b >> 63));
	r->b = cpu_to_le64((b << 1) ^ _tt);
}



struct gf128mul_4k {
	be128 t[256];
};

struct gf128mul_4k *gf128mul_init_4k_lle(const be128 *g);
struct gf128mul_4k *gf128mul_init_4k_bbe(const be128 *g);
void gf128mul_4k_lle(be128 *a, const struct gf128mul_4k *t);
void gf128mul_4k_bbe(be128 *a, const struct gf128mul_4k *t);

static inline void gf128mul_free_4k(struct gf128mul_4k *t)
{
	kzfree(t);
}




struct gf128mul_64k {
	struct gf128mul_4k *t[16];
};


struct gf128mul_64k *gf128mul_init_64k_bbe(const be128 *g);
void gf128mul_free_64k(struct gf128mul_64k *t);
void gf128mul_64k_bbe(be128 *a, const struct gf128mul_64k *t);

#endif 
