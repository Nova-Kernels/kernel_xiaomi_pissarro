/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _NF_CONNTRACK_L4PROTO_H
#define _NF_CONNTRACK_L4PROTO_H
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netns/generic.h>

struct seq_file;

struct nf_conntrack_l4proto {
	
	u_int16_t l3proto;

	
	u_int8_t l4proto;

	
	bool allow_clash;

	
	bool (*pkt_to_tuple)(const struct sk_buff *skb, unsigned int dataoff,
			     struct net *net, struct nf_conntrack_tuple *tuple);

	
	bool (*invert_tuple)(struct nf_conntrack_tuple *inverse,
			     const struct nf_conntrack_tuple *orig);

	
	int (*packet)(struct nf_conn *ct,
		      const struct sk_buff *skb,
		      unsigned int dataoff,
		      enum ip_conntrack_info ctinfo,
		      u_int8_t pf,
		      unsigned int *timeouts);

	
	bool (*new)(struct nf_conn *ct, const struct sk_buff *skb,
		    unsigned int dataoff, unsigned int *timeouts);

	
	void (*destroy)(struct nf_conn *ct);

	int (*error)(struct net *net, struct nf_conn *tmpl, struct sk_buff *skb,
		     unsigned int dataoff,
		     u_int8_t pf, unsigned int hooknum);

	
	bool (*can_early_drop)(const struct nf_conn *ct);

	
	unsigned int *(*get_timeouts)(struct net *net);

	
	int (*to_nlattr)(struct sk_buff *skb, struct nlattr *nla,
			 struct nf_conn *ct);
	
	int (*nlattr_size)(void);

	
	int (*from_nlattr)(struct nlattr *tb[], struct nf_conn *ct);

	int (*tuple_to_nlattr)(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *t);
	
	int (*nlattr_tuple_size)(void);
	int (*nlattr_to_tuple)(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t);
	const struct nla_policy *nla_policy;

	size_t nla_size;

#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
	struct {
		int (*nlattr_to_obj)(struct nlattr *tb[],
				     struct net *net, void *data);
		int (*obj_to_nlattr)(struct sk_buff *skb, const void *data);

		u16 obj_size;
		u16 nlattr_max;
		const struct nla_policy *nla_policy;
	} ctnl_timeout;
#endif
#ifdef CONFIG_NF_CONNTRACK_PROCFS
	
	void (*print_conntrack)(struct seq_file *s, struct nf_conn *);
#endif
	unsigned int	*net_id;
	
	int (*init_net)(struct net *net, u_int16_t proto);

	
	struct nf_proto_net *(*get_net_proto)(struct net *net);

	
	struct module *me;
};


extern struct nf_conntrack_l4proto nf_conntrack_l4proto_generic;

#define MAX_NF_CT_PROTO 256

const struct nf_conntrack_l4proto *__nf_ct_l4proto_find(u_int16_t l3proto,
						  u_int8_t l4proto);

const struct nf_conntrack_l4proto *nf_ct_l4proto_find_get(u_int16_t l3proto,
						    u_int8_t l4proto);
void nf_ct_l4proto_put(const struct nf_conntrack_l4proto *p);


int nf_ct_l4proto_pernet_register_one(struct net *net,
				const struct nf_conntrack_l4proto *proto);
void nf_ct_l4proto_pernet_unregister_one(struct net *net,
				const struct nf_conntrack_l4proto *proto);
int nf_ct_l4proto_pernet_register(struct net *net,
				  struct nf_conntrack_l4proto *const proto[],
				  unsigned int num_proto);
void nf_ct_l4proto_pernet_unregister(struct net *net,
				struct nf_conntrack_l4proto *const proto[],
				unsigned int num_proto);


int nf_ct_l4proto_register_one(struct nf_conntrack_l4proto *proto);
void nf_ct_l4proto_unregister_one(const struct nf_conntrack_l4proto *proto);
int nf_ct_l4proto_register(struct nf_conntrack_l4proto *proto[],
			   unsigned int num_proto);
void nf_ct_l4proto_unregister(struct nf_conntrack_l4proto *proto[],
			      unsigned int num_proto);


int nf_ct_port_tuple_to_nlattr(struct sk_buff *skb,
			       const struct nf_conntrack_tuple *tuple);
int nf_ct_port_nlattr_to_tuple(struct nlattr *tb[],
			       struct nf_conntrack_tuple *t);
int nf_ct_port_nlattr_tuple_size(void);
extern const struct nla_policy nf_ct_port_nla_policy[];

#ifdef CONFIG_SYSCTL
#define LOG_INVALID(net, proto)				\
	((net)->ct.sysctl_log_invalid == (proto) ||	\
	 (net)->ct.sysctl_log_invalid == IPPROTO_RAW)
#else
static inline int LOG_INVALID(struct net *net, int proto) { return 0; }
#endif 

#endif 
