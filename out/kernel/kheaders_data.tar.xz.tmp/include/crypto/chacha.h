/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _CRYPTO_CHACHA_H
#define _CRYPTO_CHACHA_H

#include <crypto/skcipher.h>
#include <linux/types.h>
#include <linux/crypto.h>


#define CHACHA_IV_SIZE		16

#define CHACHA_KEY_SIZE		32
#define CHACHA_BLOCK_SIZE	64


#define XCHACHA_IV_SIZE		32

struct chacha_ctx {
	u32 key[8];
	int nrounds;
};

void chacha_block(u32 *state, u8 *stream, int nrounds);
static inline void chacha20_block(u32 *state, u8 *stream)
{
	chacha_block(state, stream, 20);
}
void hchacha_block(const u32 *in, u32 *out, int nrounds);

void crypto_chacha_init(u32 *state, struct chacha_ctx *ctx, u8 *iv);

int crypto_chacha20_setkey(struct crypto_skcipher *tfm, const u8 *key,
			   unsigned int keysize);
int crypto_chacha12_setkey(struct crypto_skcipher *tfm, const u8 *key,
			   unsigned int keysize);

int crypto_chacha_crypt(struct skcipher_request *req);
int crypto_xchacha_crypt(struct skcipher_request *req);

#endif 
