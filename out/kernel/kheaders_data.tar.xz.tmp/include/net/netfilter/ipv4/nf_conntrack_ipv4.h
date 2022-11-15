/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _NF_CONNTRACK_IPV4_H
#define _NF_CONNTRACK_IPV4_H


extern struct nf_conntrack_l3proto nf_conntrack_l3proto_ipv4;

extern struct nf_conntrack_l4proto nf_conntrack_l4proto_tcp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4;
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_icmp;
#ifdef CONFIG_NF_CT_PROTO_DCCP
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_dccp4;
#endif
#ifdef CONFIG_NF_CT_PROTO_SCTP
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_sctp4;
#endif
#ifdef CONFIG_NF_CT_PROTO_UDPLITE
extern struct nf_conntrack_l4proto nf_conntrack_l4proto_udplite4;
#endif

int nf_conntrack_ipv4_compat_init(void);
void nf_conntrack_ipv4_compat_fini(void);

#endif 
