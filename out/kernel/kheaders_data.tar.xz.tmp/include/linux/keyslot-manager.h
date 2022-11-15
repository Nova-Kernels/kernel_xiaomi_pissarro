/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __LINUX_KEYSLOT_MANAGER_H
#define __LINUX_KEYSLOT_MANAGER_H

#include <linux/bio.h>


enum {
	
	BLK_CRYPTO_FEATURE_STANDARD_KEYS = BIT(0),

	
	BLK_CRYPTO_FEATURE_WRAPPED_KEYS = BIT(1),
};

#ifdef CONFIG_BLK_INLINE_ENCRYPTION

struct keyslot_manager;


struct keyslot_mgmt_ll_ops {
	int (*keyslot_program)(struct keyslot_manager *ksm,
			       const struct blk_crypto_key *key,
			       unsigned int slot);
	int (*keyslot_evict)(struct keyslot_manager *ksm,
			     const struct blk_crypto_key *key,
			     unsigned int slot);
	int (*derive_raw_secret)(struct keyslot_manager *ksm,
				 const u8 *wrapped_key,
				 unsigned int wrapped_key_size,
				 u8 *secret, unsigned int secret_size);
};

struct keyslot_manager *keyslot_manager_create(
	struct device *dev,
	unsigned int num_slots,
	const struct keyslot_mgmt_ll_ops *ksm_ops,
	unsigned int features,
	const unsigned int crypto_mode_supported[BLK_ENCRYPTION_MODE_MAX],
	void *ll_priv_data);

void keyslot_manager_set_max_dun_bytes(struct keyslot_manager *ksm,
				       unsigned int max_dun_bytes);

int keyslot_manager_get_slot_for_key(struct keyslot_manager *ksm,
				     const struct blk_crypto_key *key);

void keyslot_manager_get_slot(struct keyslot_manager *ksm, unsigned int slot);

void keyslot_manager_put_slot(struct keyslot_manager *ksm, unsigned int slot);

bool keyslot_manager_crypto_mode_supported(struct keyslot_manager *ksm,
					   enum blk_crypto_mode_num crypto_mode,
					   unsigned int dun_bytes,
					   unsigned int data_unit_size,
					   bool is_hw_wrapped_key);

int keyslot_manager_evict_key(struct keyslot_manager *ksm,
			      const struct blk_crypto_key *key);

void keyslot_manager_reprogram_all_keys(struct keyslot_manager *ksm);

void *keyslot_manager_private(struct keyslot_manager *ksm);

void keyslot_manager_destroy(struct keyslot_manager *ksm);

struct keyslot_manager *keyslot_manager_create_passthrough(
	struct device *dev,
	const struct keyslot_mgmt_ll_ops *ksm_ops,
	unsigned int features,
	const unsigned int crypto_mode_supported[BLK_ENCRYPTION_MODE_MAX],
	void *ll_priv_data);

void keyslot_manager_intersect_modes(struct keyslot_manager *parent,
				     const struct keyslot_manager *child);

int keyslot_manager_derive_raw_secret(struct keyslot_manager *ksm,
				      const u8 *wrapped_key,
				      unsigned int wrapped_key_size,
				      u8 *secret, unsigned int secret_size);

inline void ksm_flock(struct keyslot_manager *ksm, unsigned int flags);

#endif 

#endif 
