/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NET_GENERIC_NETLINK_H
#define __NET_GENERIC_NETLINK_H

#include <linux/genetlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h>

#define GENLMSG_DEFAULT_SIZE (NLMSG_DEFAULT_SIZE - GENL_HDRLEN)


struct genl_multicast_group {
	char			name[GENL_NAMSIZ];
};

struct genl_ops;
struct genl_info;


struct genl_family {
	int			id;		
	unsigned int		hdrsize;
	char			name[GENL_NAMSIZ];
	unsigned int		version;
	unsigned int		maxattr;
	bool			netnsok;
	bool			parallel_ops;
	int			(*pre_doit)(const struct genl_ops *ops,
					    struct sk_buff *skb,
					    struct genl_info *info);
	void			(*post_doit)(const struct genl_ops *ops,
					     struct sk_buff *skb,
					     struct genl_info *info);
	int			(*mcast_bind)(struct net *net, int group);
	void			(*mcast_unbind)(struct net *net, int group);
	struct nlattr **	attrbuf;	
	const struct genl_ops *	ops;
	const struct genl_multicast_group *mcgrps;
	unsigned int		n_ops;
	unsigned int		n_mcgrps;
	unsigned int		mcgrp_offset;	
	struct module		*module;
};

struct nlattr **genl_family_attrbuf(const struct genl_family *family);


struct genl_info {
	u32			snd_seq;
	u32			snd_portid;
	struct nlmsghdr *	nlhdr;
	struct genlmsghdr *	genlhdr;
	void *			userhdr;
	struct nlattr **	attrs;
	possible_net_t		_net;
	void *			user_ptr[2];
	struct netlink_ext_ack *extack;
};

static inline struct net *genl_info_net(struct genl_info *info)
{
	return read_pnet(&info->_net);
}

static inline void genl_info_net_set(struct genl_info *info, struct net *net)
{
	write_pnet(&info->_net, net);
}

#define GENL_SET_ERR_MSG(info, msg) NL_SET_ERR_MSG((info)->extack, msg)

static inline int genl_err_attr(struct genl_info *info, int err,
				struct nlattr *attr)
{
	info->extack->bad_attr = attr;

	return err;
}


struct genl_ops {
	const struct nla_policy	*policy;
	int		       (*doit)(struct sk_buff *skb,
				       struct genl_info *info);
	int		       (*start)(struct netlink_callback *cb);
	int		       (*dumpit)(struct sk_buff *skb,
					 struct netlink_callback *cb);
	int		       (*done)(struct netlink_callback *cb);
	u8			cmd;
	u8			internal_flags;
	u8			flags;
};

int genl_register_family(struct genl_family *family);
int genl_unregister_family(const struct genl_family *family);
void genl_notify(const struct genl_family *family, struct sk_buff *skb,
		 struct genl_info *info, u32 group, gfp_t flags);

void *genlmsg_put(struct sk_buff *skb, u32 portid, u32 seq,
		  const struct genl_family *family, int flags, u8 cmd);


static inline struct nlmsghdr *
genlmsg_nlhdr(void *user_hdr, const struct genl_family *family)
{
	return (struct nlmsghdr *)((char *)user_hdr -
				   family->hdrsize -
				   GENL_HDRLEN -
				   NLMSG_HDRLEN);
}


static inline int genlmsg_parse(const struct nlmsghdr *nlh,
				const struct genl_family *family,
				struct nlattr *tb[], int maxtype,
				const struct nla_policy *policy,
				struct netlink_ext_ack *extack)
{
	return nlmsg_parse(nlh, family->hdrsize + GENL_HDRLEN, tb, maxtype,
			   policy, extack);
}


static inline void genl_dump_check_consistent(struct netlink_callback *cb,
					      void *user_hdr,
					      const struct genl_family *family)
{
	nl_dump_check_consistent(cb, genlmsg_nlhdr(user_hdr, family));
}


static inline void *genlmsg_put_reply(struct sk_buff *skb,
				      struct genl_info *info,
				      const struct genl_family *family,
				      int flags, u8 cmd)
{
	return genlmsg_put(skb, info->snd_portid, info->snd_seq, family,
			   flags, cmd);
}


static inline void genlmsg_end(struct sk_buff *skb, void *hdr)
{
	nlmsg_end(skb, hdr - GENL_HDRLEN - NLMSG_HDRLEN);
}


static inline void genlmsg_cancel(struct sk_buff *skb, void *hdr)
{
	if (hdr)
		nlmsg_cancel(skb, hdr - GENL_HDRLEN - NLMSG_HDRLEN);
}


static inline int genlmsg_multicast_netns(const struct genl_family *family,
					  struct net *net, struct sk_buff *skb,
					  u32 portid, unsigned int group, gfp_t flags)
{
	if (WARN_ON_ONCE(group >= family->n_mcgrps))
		return -EINVAL;
	group = family->mcgrp_offset + group;
	return nlmsg_multicast(net->genl_sock, skb, portid, group, flags);
}


static inline int genlmsg_multicast(const struct genl_family *family,
				    struct sk_buff *skb, u32 portid,
				    unsigned int group, gfp_t flags)
{
	return genlmsg_multicast_netns(family, &init_net, skb,
				       portid, group, flags);
}


int genlmsg_multicast_allns(const struct genl_family *family,
			    struct sk_buff *skb, u32 portid,
			    unsigned int group, gfp_t flags);


static inline int genlmsg_unicast(struct net *net, struct sk_buff *skb, u32 portid)
{
	return nlmsg_unicast(net->genl_sock, skb, portid);
}


static inline int genlmsg_reply(struct sk_buff *skb, struct genl_info *info)
{
	return genlmsg_unicast(genl_info_net(info), skb, info->snd_portid);
}


static inline void *genlmsg_data(const struct genlmsghdr *gnlh)
{
	return ((unsigned char *) gnlh + GENL_HDRLEN);
}


static inline int genlmsg_len(const struct genlmsghdr *gnlh)
{
	struct nlmsghdr *nlh = (struct nlmsghdr *)((unsigned char *)gnlh -
							NLMSG_HDRLEN);
	return (nlh->nlmsg_len - GENL_HDRLEN - NLMSG_HDRLEN);
}


static inline int genlmsg_msg_size(int payload)
{
	return GENL_HDRLEN + payload;
}


static inline int genlmsg_total_size(int payload)
{
	return NLMSG_ALIGN(genlmsg_msg_size(payload));
}


static inline struct sk_buff *genlmsg_new(size_t payload, gfp_t flags)
{
	return nlmsg_new(genlmsg_total_size(payload), flags);
}


static inline int genl_set_err(const struct genl_family *family,
			       struct net *net, u32 portid,
			       u32 group, int code)
{
	if (WARN_ON_ONCE(group >= family->n_mcgrps))
		return -EINVAL;
	group = family->mcgrp_offset + group;
	return netlink_set_err(net->genl_sock, portid, group, code);
}

static inline int genl_has_listeners(const struct genl_family *family,
				     struct net *net, unsigned int group)
{
	if (WARN_ON_ONCE(group >= family->n_mcgrps))
		return -EINVAL;
	group = family->mcgrp_offset + group;
	return netlink_has_listeners(net->genl_sock, group);
}
#endif	
