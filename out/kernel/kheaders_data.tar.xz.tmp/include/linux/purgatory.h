/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PURGATORY_H
#define _LINUX_PURGATORY_H

#include <linux/types.h>
#include <crypto/sha.h>
#include <uapi/linux/kexec.h>

struct kexec_sha_region {
	unsigned long start;
	unsigned long len;
};


extern struct kexec_sha_region purgatory_sha_regions[KEXEC_SEGMENT_MAX];
extern u8 purgatory_sha256_digest[SHA256_DIGEST_SIZE];

#endif
