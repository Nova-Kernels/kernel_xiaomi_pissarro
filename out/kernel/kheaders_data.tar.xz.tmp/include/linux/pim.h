/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_PIM_H
#define __LINUX_PIM_H

#include <linux/skbuff.h>
#include <asm/byteorder.h>


#define PIM_V1_VERSION		cpu_to_be32(0x10000000)
#define PIM_V1_REGISTER		1


#define PIM_VERSION		2


enum {
	PIM_TYPE_HELLO,
	PIM_TYPE_REGISTER,
	PIM_TYPE_REGISTER_STOP,
	PIM_TYPE_JOIN_PRUNE,
	PIM_TYPE_BOOTSTRAP,
	PIM_TYPE_ASSERT,
	PIM_TYPE_GRAFT,
	PIM_TYPE_GRAFT_ACK,
	PIM_TYPE_CANDIDATE_RP_ADV
};

#define PIM_NULL_REGISTER	cpu_to_be32(0x40000000)


struct pimhdr {
	__u8	type;
	__u8	reserved;
	__be16	csum;
};


struct pimreghdr {
	__u8	type;
	__u8	reserved;
	__be16	csum;
	__be32	flags;
};

int pim_rcv_v1(struct sk_buff *skb);

static inline bool ipmr_pimsm_enabled(void)
{
	return IS_BUILTIN(CONFIG_IP_PIMSM_V1) || IS_BUILTIN(CONFIG_IP_PIMSM_V2);
}

static inline struct pimhdr *pim_hdr(const struct sk_buff *skb)
{
	return (struct pimhdr *)skb_transport_header(skb);
}

static inline u8 pim_hdr_version(const struct pimhdr *pimhdr)
{
	return pimhdr->type >> 4;
}

static inline u8 pim_hdr_type(const struct pimhdr *pimhdr)
{
	return pimhdr->type & 0xf;
}


static inline bool pim_ipv4_all_pim_routers(__be32 addr)
{
	return addr == htonl(0xE000000D);
}
#endif
