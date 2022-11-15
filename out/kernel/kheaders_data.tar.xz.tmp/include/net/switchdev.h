
#ifndef _LINUX_SWITCHDEV_H_
#define _LINUX_SWITCHDEV_H_

#include <linux/netdevice.h>
#include <linux/notifier.h>
#include <linux/list.h>
#include <net/ip_fib.h>

#define SWITCHDEV_F_NO_RECURSE		BIT(0)
#define SWITCHDEV_F_SKIP_EOPNOTSUPP	BIT(1)
#define SWITCHDEV_F_DEFER		BIT(2)

struct switchdev_trans_item {
	struct list_head list;
	void *data;
	void (*destructor)(const void *data);
};

struct switchdev_trans {
	struct list_head item_list;
	bool ph_prepare;
};

static inline bool switchdev_trans_ph_prepare(struct switchdev_trans *trans)
{
	return trans && trans->ph_prepare;
}

static inline bool switchdev_trans_ph_commit(struct switchdev_trans *trans)
{
	return trans && !trans->ph_prepare;
}

enum switchdev_attr_id {
	SWITCHDEV_ATTR_ID_UNDEFINED,
	SWITCHDEV_ATTR_ID_PORT_PARENT_ID,
	SWITCHDEV_ATTR_ID_PORT_STP_STATE,
	SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS,
	SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS_SUPPORT,
	SWITCHDEV_ATTR_ID_PORT_MROUTER,
	SWITCHDEV_ATTR_ID_BRIDGE_AGEING_TIME,
	SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING,
	SWITCHDEV_ATTR_ID_BRIDGE_MC_DISABLED,
};

struct switchdev_attr {
	struct net_device *orig_dev;
	enum switchdev_attr_id id;
	u32 flags;
	void *complete_priv;
	void (*complete)(struct net_device *dev, int err, void *priv);
	union {
		struct netdev_phys_item_id ppid;	
		u8 stp_state;				
		unsigned long brport_flags;		
		unsigned long brport_flags_support;	
		bool mrouter;				
		clock_t ageing_time;			
		bool vlan_filtering;			
		bool mc_disabled;			
	} u;
};

enum switchdev_obj_id {
	SWITCHDEV_OBJ_ID_UNDEFINED,
	SWITCHDEV_OBJ_ID_PORT_VLAN,
	SWITCHDEV_OBJ_ID_PORT_MDB,
};

struct switchdev_obj {
	struct net_device *orig_dev;
	enum switchdev_obj_id id;
	u32 flags;
	void *complete_priv;
	void (*complete)(struct net_device *dev, int err, void *priv);
};


struct switchdev_obj_port_vlan {
	struct switchdev_obj obj;
	u16 flags;
	u16 vid_begin;
	u16 vid_end;
};

#define SWITCHDEV_OBJ_PORT_VLAN(obj) \
	container_of(obj, struct switchdev_obj_port_vlan, obj)


struct switchdev_obj_port_mdb {
	struct switchdev_obj obj;
	unsigned char addr[ETH_ALEN];
	u16 vid;
};

#define SWITCHDEV_OBJ_PORT_MDB(obj) \
	container_of(obj, struct switchdev_obj_port_mdb, obj)

void switchdev_trans_item_enqueue(struct switchdev_trans *trans,
				  void *data, void (*destructor)(void const *),
				  struct switchdev_trans_item *tritem);
void *switchdev_trans_item_dequeue(struct switchdev_trans *trans);

typedef int switchdev_obj_dump_cb_t(struct switchdev_obj *obj);


struct switchdev_ops {
	int	(*switchdev_port_attr_get)(struct net_device *dev,
					   struct switchdev_attr *attr);
	int	(*switchdev_port_attr_set)(struct net_device *dev,
					   const struct switchdev_attr *attr,
					   struct switchdev_trans *trans);
	int	(*switchdev_port_obj_add)(struct net_device *dev,
					  const struct switchdev_obj *obj,
					  struct switchdev_trans *trans);
	int	(*switchdev_port_obj_del)(struct net_device *dev,
					  const struct switchdev_obj *obj);
};

enum switchdev_notifier_type {
	SWITCHDEV_FDB_ADD_TO_BRIDGE = 1,
	SWITCHDEV_FDB_DEL_TO_BRIDGE,
	SWITCHDEV_FDB_ADD_TO_DEVICE,
	SWITCHDEV_FDB_DEL_TO_DEVICE,
	SWITCHDEV_FDB_OFFLOADED,
};

struct switchdev_notifier_info {
	struct net_device *dev;
};

struct switchdev_notifier_fdb_info {
	struct switchdev_notifier_info info; 
	const unsigned char *addr;
	u16 vid;
};

static inline struct net_device *
switchdev_notifier_info_to_dev(const struct switchdev_notifier_info *info)
{
	return info->dev;
}

#ifdef CONFIG_NET_SWITCHDEV

void switchdev_deferred_process(void);
int switchdev_port_attr_get(struct net_device *dev,
			    struct switchdev_attr *attr);
int switchdev_port_attr_set(struct net_device *dev,
			    const struct switchdev_attr *attr);
int switchdev_port_obj_add(struct net_device *dev,
			   const struct switchdev_obj *obj);
int switchdev_port_obj_del(struct net_device *dev,
			   const struct switchdev_obj *obj);
int register_switchdev_notifier(struct notifier_block *nb);
int unregister_switchdev_notifier(struct notifier_block *nb);
int call_switchdev_notifiers(unsigned long val, struct net_device *dev,
			     struct switchdev_notifier_info *info);
void switchdev_port_fwd_mark_set(struct net_device *dev,
				 struct net_device *group_dev,
				 bool joining);

bool switchdev_port_same_parent_id(struct net_device *a,
				   struct net_device *b);

#define SWITCHDEV_SET_OPS(netdev, ops) ((netdev)->switchdev_ops = (ops))
#else

static inline void switchdev_deferred_process(void)
{
}

static inline int switchdev_port_attr_get(struct net_device *dev,
					  struct switchdev_attr *attr)
{
	return -EOPNOTSUPP;
}

static inline int switchdev_port_attr_set(struct net_device *dev,
					  const struct switchdev_attr *attr)
{
	return -EOPNOTSUPP;
}

static inline int switchdev_port_obj_add(struct net_device *dev,
					 const struct switchdev_obj *obj)
{
	return -EOPNOTSUPP;
}

static inline int switchdev_port_obj_del(struct net_device *dev,
					 const struct switchdev_obj *obj)
{
	return -EOPNOTSUPP;
}

static inline int register_switchdev_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int unregister_switchdev_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int call_switchdev_notifiers(unsigned long val,
					   struct net_device *dev,
					   struct switchdev_notifier_info *info)
{
	return NOTIFY_DONE;
}

static inline bool switchdev_port_same_parent_id(struct net_device *a,
						 struct net_device *b)
{
	return false;
}

#define SWITCHDEV_SET_OPS(netdev, ops) do {} while (0)

#endif

#endif 
