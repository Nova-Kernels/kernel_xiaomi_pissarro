/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _SMC_H
#define _SMC_H

struct smc_hashinfo {
	rwlock_t lock;
	struct hlist_head ht;
};

int smc_hash_sk(struct sock *sk);
void smc_unhash_sk(struct sock *sk);
#endif	
