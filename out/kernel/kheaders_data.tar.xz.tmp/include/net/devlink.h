
#ifndef _NET_DEVLINK_H_
#define _NET_DEVLINK_H_

#include <linux/device.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/list.h>
#include <linux/netdevice.h>
#include <net/net_namespace.h>
#include <uapi/linux/devlink.h>

struct devlink_ops;

struct devlink {
	struct list_head list;
	struct list_head port_list;
	struct list_head sb_list;
	struct list_head dpipe_table_list;
	struct devlink_dpipe_headers *dpipe_headers;
	const struct devlink_ops *ops;
	struct device *dev;
	possible_net_t _net;
	char priv[0] __aligned(NETDEV_ALIGN);
};

struct devlink_port {
	struct list_head list;
	struct devlink *devlink;
	unsigned index;
	bool registered;
	enum devlink_port_type type;
	enum devlink_port_type desired_type;
	void *type_dev;
	bool split;
	u32 split_group;
};

struct devlink_sb_pool_info {
	enum devlink_sb_pool_type pool_type;
	u32 size;
	enum devlink_sb_threshold_type threshold_type;
};


struct devlink_dpipe_field {
	const char *name;
	unsigned int id;
	unsigned int bitwidth;
	enum devlink_dpipe_field_mapping_type mapping_type;
};


struct devlink_dpipe_header {
	const char *name;
	unsigned int id;
	struct devlink_dpipe_field *fields;
	unsigned int fields_count;
	bool global;
};


struct devlink_dpipe_match {
	enum devlink_dpipe_match_type type;
	unsigned int header_index;
	struct devlink_dpipe_header *header;
	unsigned int field_id;
};


struct devlink_dpipe_action {
	enum devlink_dpipe_action_type type;
	unsigned int header_index;
	struct devlink_dpipe_header *header;
	unsigned int field_id;
};


struct devlink_dpipe_value {
	union {
		struct devlink_dpipe_action *action;
		struct devlink_dpipe_match *match;
	};
	unsigned int mapping_value;
	bool mapping_valid;
	unsigned int value_size;
	void *value;
	void *mask;
};


struct devlink_dpipe_entry {
	u64 index;
	struct devlink_dpipe_value *match_values;
	unsigned int match_values_count;
	struct devlink_dpipe_value *action_values;
	unsigned int action_values_count;
	u64 counter;
	bool counter_valid;
};


struct devlink_dpipe_dump_ctx {
	struct genl_info *info;
	enum devlink_command cmd;
	struct sk_buff *skb;
	struct nlattr *nest;
	void *hdr;
};

struct devlink_dpipe_table_ops;


struct devlink_dpipe_table {
	void *priv;
	struct list_head list;
	const char *name;
	bool counters_enabled;
	bool counter_control_extern;
	struct devlink_dpipe_table_ops *table_ops;
	struct rcu_head rcu;
};


struct devlink_dpipe_table_ops {
	int (*actions_dump)(void *priv, struct sk_buff *skb);
	int (*matches_dump)(void *priv, struct sk_buff *skb);
	int (*entries_dump)(void *priv, bool counters_enabled,
			    struct devlink_dpipe_dump_ctx *dump_ctx);
	int (*counters_set_update)(void *priv, bool enable);
	u64 (*size_get)(void *priv);
};


struct devlink_dpipe_headers {
	struct devlink_dpipe_header **headers;
	unsigned int headers_count;
};

struct devlink_ops {
	int (*port_type_set)(struct devlink_port *devlink_port,
			     enum devlink_port_type port_type);
	int (*port_split)(struct devlink *devlink, unsigned int port_index,
			  unsigned int count);
	int (*port_unsplit)(struct devlink *devlink, unsigned int port_index);
	int (*sb_pool_get)(struct devlink *devlink, unsigned int sb_index,
			   u16 pool_index,
			   struct devlink_sb_pool_info *pool_info);
	int (*sb_pool_set)(struct devlink *devlink, unsigned int sb_index,
			   u16 pool_index, u32 size,
			   enum devlink_sb_threshold_type threshold_type);
	int (*sb_port_pool_get)(struct devlink_port *devlink_port,
				unsigned int sb_index, u16 pool_index,
				u32 *p_threshold);
	int (*sb_port_pool_set)(struct devlink_port *devlink_port,
				unsigned int sb_index, u16 pool_index,
				u32 threshold);
	int (*sb_tc_pool_bind_get)(struct devlink_port *devlink_port,
				   unsigned int sb_index,
				   u16 tc_index,
				   enum devlink_sb_pool_type pool_type,
				   u16 *p_pool_index, u32 *p_threshold);
	int (*sb_tc_pool_bind_set)(struct devlink_port *devlink_port,
				   unsigned int sb_index,
				   u16 tc_index,
				   enum devlink_sb_pool_type pool_type,
				   u16 pool_index, u32 threshold);
	int (*sb_occ_snapshot)(struct devlink *devlink,
			       unsigned int sb_index);
	int (*sb_occ_max_clear)(struct devlink *devlink,
				unsigned int sb_index);
	int (*sb_occ_port_pool_get)(struct devlink_port *devlink_port,
				    unsigned int sb_index, u16 pool_index,
				    u32 *p_cur, u32 *p_max);
	int (*sb_occ_tc_port_bind_get)(struct devlink_port *devlink_port,
				       unsigned int sb_index,
				       u16 tc_index,
				       enum devlink_sb_pool_type pool_type,
				       u32 *p_cur, u32 *p_max);

	int (*eswitch_mode_get)(struct devlink *devlink, u16 *p_mode);
	int (*eswitch_mode_set)(struct devlink *devlink, u16 mode);
	int (*eswitch_inline_mode_get)(struct devlink *devlink, u8 *p_inline_mode);
	int (*eswitch_inline_mode_set)(struct devlink *devlink, u8 inline_mode);
	int (*eswitch_encap_mode_get)(struct devlink *devlink, u8 *p_encap_mode);
	int (*eswitch_encap_mode_set)(struct devlink *devlink, u8 encap_mode);
};

static inline void *devlink_priv(struct devlink *devlink)
{
	BUG_ON(!devlink);
	return &devlink->priv;
}

static inline struct devlink *priv_to_devlink(void *priv)
{
	BUG_ON(!priv);
	return container_of(priv, struct devlink, priv);
}

struct ib_device;

#if IS_ENABLED(CONFIG_NET_DEVLINK)

struct devlink *devlink_alloc(const struct devlink_ops *ops, size_t priv_size);
int devlink_register(struct devlink *devlink, struct device *dev);
void devlink_unregister(struct devlink *devlink);
void devlink_free(struct devlink *devlink);
int devlink_port_register(struct devlink *devlink,
			  struct devlink_port *devlink_port,
			  unsigned int port_index);
void devlink_port_unregister(struct devlink_port *devlink_port);
void devlink_port_type_eth_set(struct devlink_port *devlink_port,
			       struct net_device *netdev);
void devlink_port_type_ib_set(struct devlink_port *devlink_port,
			      struct ib_device *ibdev);
void devlink_port_type_clear(struct devlink_port *devlink_port);
void devlink_port_split_set(struct devlink_port *devlink_port,
			    u32 split_group);
int devlink_sb_register(struct devlink *devlink, unsigned int sb_index,
			u32 size, u16 ingress_pools_count,
			u16 egress_pools_count, u16 ingress_tc_count,
			u16 egress_tc_count);
void devlink_sb_unregister(struct devlink *devlink, unsigned int sb_index);
int devlink_dpipe_table_register(struct devlink *devlink,
				 const char *table_name,
				 struct devlink_dpipe_table_ops *table_ops,
				 void *priv, bool counter_control_extern);
void devlink_dpipe_table_unregister(struct devlink *devlink,
				    const char *table_name);
int devlink_dpipe_headers_register(struct devlink *devlink,
				   struct devlink_dpipe_headers *dpipe_headers);
void devlink_dpipe_headers_unregister(struct devlink *devlink);
bool devlink_dpipe_table_counter_enabled(struct devlink *devlink,
					 const char *table_name);
int devlink_dpipe_entry_ctx_prepare(struct devlink_dpipe_dump_ctx *dump_ctx);
int devlink_dpipe_entry_ctx_append(struct devlink_dpipe_dump_ctx *dump_ctx,
				   struct devlink_dpipe_entry *entry);
int devlink_dpipe_entry_ctx_close(struct devlink_dpipe_dump_ctx *dump_ctx);
void devlink_dpipe_entry_clear(struct devlink_dpipe_entry *entry);
int devlink_dpipe_action_put(struct sk_buff *skb,
			     struct devlink_dpipe_action *action);
int devlink_dpipe_match_put(struct sk_buff *skb,
			    struct devlink_dpipe_match *match);
extern struct devlink_dpipe_header devlink_dpipe_header_ethernet;
extern struct devlink_dpipe_header devlink_dpipe_header_ipv4;
extern struct devlink_dpipe_header devlink_dpipe_header_ipv6;

#else

static inline struct devlink *devlink_alloc(const struct devlink_ops *ops,
					    size_t priv_size)
{
	return kzalloc(sizeof(struct devlink) + priv_size, GFP_KERNEL);
}

static inline int devlink_register(struct devlink *devlink, struct device *dev)
{
	return 0;
}

static inline void devlink_unregister(struct devlink *devlink)
{
}

static inline void devlink_free(struct devlink *devlink)
{
	kfree(devlink);
}

static inline int devlink_port_register(struct devlink *devlink,
					struct devlink_port *devlink_port,
					unsigned int port_index)
{
	return 0;
}

static inline void devlink_port_unregister(struct devlink_port *devlink_port)
{
}

static inline void devlink_port_type_eth_set(struct devlink_port *devlink_port,
					     struct net_device *netdev)
{
}

static inline void devlink_port_type_ib_set(struct devlink_port *devlink_port,
					    struct ib_device *ibdev)
{
}

static inline void devlink_port_type_clear(struct devlink_port *devlink_port)
{
}

static inline void devlink_port_split_set(struct devlink_port *devlink_port,
					  u32 split_group)
{
}

static inline int devlink_sb_register(struct devlink *devlink,
				      unsigned int sb_index, u32 size,
				      u16 ingress_pools_count,
				      u16 egress_pools_count,
				      u16 ingress_tc_count,
				      u16 egress_tc_count)
{
	return 0;
}

static inline void devlink_sb_unregister(struct devlink *devlink,
					 unsigned int sb_index)
{
}

static inline int
devlink_dpipe_table_register(struct devlink *devlink,
			     const char *table_name,
			     struct devlink_dpipe_table_ops *table_ops,
			     void *priv, bool counter_control_extern)
{
	return 0;
}

static inline void devlink_dpipe_table_unregister(struct devlink *devlink,
						  const char *table_name)
{
}

static inline int devlink_dpipe_headers_register(struct devlink *devlink,
						 struct devlink_dpipe_headers *
						 dpipe_headers)
{
	return 0;
}

static inline void devlink_dpipe_headers_unregister(struct devlink *devlink)
{
}

static inline bool devlink_dpipe_table_counter_enabled(struct devlink *devlink,
						       const char *table_name)
{
	return false;
}

static inline int
devlink_dpipe_entry_ctx_prepare(struct devlink_dpipe_dump_ctx *dump_ctx)
{
	return 0;
}

static inline int
devlink_dpipe_entry_ctx_append(struct devlink_dpipe_dump_ctx *dump_ctx,
			       struct devlink_dpipe_entry *entry)
{
	return 0;
}

static inline int
devlink_dpipe_entry_ctx_close(struct devlink_dpipe_dump_ctx *dump_ctx)
{
	return 0;
}

static inline void
devlink_dpipe_entry_clear(struct devlink_dpipe_entry *entry)
{
}

static inline int
devlink_dpipe_action_put(struct sk_buff *skb,
			 struct devlink_dpipe_action *action)
{
	return 0;
}

static inline int
devlink_dpipe_match_put(struct sk_buff *skb,
			struct devlink_dpipe_match *match)
{
	return 0;
}

#endif

#endif 
