
#ifndef _UDP_H
#define _UDP_H

#include <linux/list.h>
#include <linux/bug.h>
#include <net/inet_sock.h>
#include <net/sock.h>
#include <net/snmp.h>
#include <net/ip.h>
#include <linux/ipv6.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/static_key.h>

struct udp_skb_cb {
	union {
		struct inet_skb_parm	h4;
#if IS_ENABLED(CONFIG_IPV6)
		struct inet6_skb_parm	h6;
#endif
	} header;
	__u16		cscov;
	__u8		partial_cov;
};
#define UDP_SKB_CB(__skb)	((struct udp_skb_cb *)((__skb)->cb))


struct udp_hslot {
	struct hlist_head	head;
	int			count;
	spinlock_t		lock;
} __attribute__((aligned(2 * sizeof(long))));


struct udp_table {
	struct udp_hslot	*hash;
	struct udp_hslot	*hash2;
	unsigned int		mask;
	unsigned int		log;
};
extern struct udp_table udp_table;
void udp_table_init(struct udp_table *, const char *);
static inline struct udp_hslot *udp_hashslot(struct udp_table *table,
					     struct net *net, unsigned int num)
{
	return &table->hash[udp_hashfn(net, num, table->mask)];
}

static inline struct udp_hslot *udp_hashslot2(struct udp_table *table,
					      unsigned int hash)
{
	return &table->hash2[hash & table->mask];
}

extern struct proto udp_prot;

extern atomic_long_t udp_memory_allocated;


extern long sysctl_udp_mem[3];
extern int sysctl_udp_rmem_min;
extern int sysctl_udp_wmem_min;

struct sk_buff;


static inline __sum16 __udp_lib_checksum_complete(struct sk_buff *skb)
{
	return (UDP_SKB_CB(skb)->cscov == skb->len ?
		__skb_checksum_complete(skb) :
		__skb_checksum_complete_head(skb, UDP_SKB_CB(skb)->cscov));
}

static inline int udp_lib_checksum_complete(struct sk_buff *skb)
{
	return !skb_csum_unnecessary(skb) &&
		__udp_lib_checksum_complete(skb);
}


static inline __wsum udp_csum_outgoing(struct sock *sk, struct sk_buff *skb)
{
	__wsum csum = csum_partial(skb_transport_header(skb),
				   sizeof(struct udphdr), 0);
	skb_queue_walk(&sk->sk_write_queue, skb) {
		csum = csum_add(csum, skb->csum);
	}
	return csum;
}

static inline __wsum udp_csum(struct sk_buff *skb)
{
	__wsum csum = csum_partial(skb_transport_header(skb),
				   sizeof(struct udphdr), skb->csum);

	for (skb = skb_shinfo(skb)->frag_list; skb; skb = skb->next) {
		csum = csum_add(csum, skb->csum);
	}
	return csum;
}

static inline __sum16 udp_v4_check(int len, __be32 saddr,
				   __be32 daddr, __wsum base)
{
	return csum_tcpudp_magic(saddr, daddr, len, IPPROTO_UDP, base);
}

void udp_set_csum(bool nocheck, struct sk_buff *skb,
		  __be32 saddr, __be32 daddr, int len);

static inline void udp_csum_pull_header(struct sk_buff *skb)
{
	if (!skb->csum_valid && skb->ip_summed == CHECKSUM_NONE)
		skb->csum = csum_partial(skb->data, sizeof(struct udphdr),
					 skb->csum);
	skb_pull_rcsum(skb, sizeof(struct udphdr));
	UDP_SKB_CB(skb)->cscov -= sizeof(struct udphdr);
}

typedef struct sock *(*udp_lookup_t)(struct sk_buff *skb, __be16 sport,
				     __be16 dport);

struct sk_buff *udp_gro_receive(struct list_head *head, struct sk_buff *skb,
				struct udphdr *uh, struct sock *sk);
int udp_gro_complete(struct sk_buff *skb, int nhoff, udp_lookup_t lookup);

struct sk_buff *__udp_gso_segment(struct sk_buff *gso_skb,
				  netdev_features_t features);

static inline struct udphdr *udp_gro_udphdr(struct sk_buff *skb)
{
	struct udphdr *uh;
	unsigned int hlen, off;

	off  = skb_gro_offset(skb);
	hlen = off + sizeof(*uh);
	uh   = skb_gro_header_fast(skb, off);
	if (skb_gro_header_hard(skb, hlen))
		uh = skb_gro_header_slow(skb, hlen, off);

	return uh;
}


static inline int udp_lib_hash(struct sock *sk)
{
	BUG();
	return 0;
}

void udp_lib_unhash(struct sock *sk);
void udp_lib_rehash(struct sock *sk, u16 new_hash);

static inline void udp_lib_close(struct sock *sk, long timeout)
{
	sk_common_release(sk);
}

int udp_lib_get_port(struct sock *sk, unsigned short snum,
		     unsigned int hash2_nulladdr);

u32 udp_flow_hashrnd(void);

static inline __be16 udp_flow_src_port(struct net *net, struct sk_buff *skb,
				       int min, int max, bool use_eth)
{
	u32 hash;

	if (min >= max) {
		
		inet_get_local_port_range(net, &min, &max);
	}

	hash = skb_get_hash(skb);
	if (unlikely(!hash)) {
		if (use_eth) {
			
			hash = jhash(skb->data, 2 * ETH_ALEN,
				     (__force u32) skb->protocol);
		} else {
			
			hash = udp_flow_hashrnd();
		}
	}

	
	hash ^= hash << 16;

	return htons((((u64) hash * (max - min)) >> 32) + min);
}

static inline int udp_rqueue_get(struct sock *sk)
{
	return sk_rmem_alloc_get(sk) - READ_ONCE(udp_sk(sk)->forward_deficit);
}


void udp_destruct_sock(struct sock *sk);
void skb_consume_udp(struct sock *sk, struct sk_buff *skb, int len);
int __udp_enqueue_schedule_skb(struct sock *sk, struct sk_buff *skb);
void udp_skb_destructor(struct sock *sk, struct sk_buff *skb);
struct sk_buff *__skb_recv_udp(struct sock *sk, unsigned int flags,
			       int noblock, int *peeked, int *off, int *err);
static inline struct sk_buff *skb_recv_udp(struct sock *sk, unsigned int flags,
					   int noblock, int *err)
{
	int peeked, off = 0;

	return __skb_recv_udp(sk, flags, noblock, &peeked, &off, err);
}

int udp_v4_early_demux(struct sk_buff *skb);
bool udp_sk_rx_dst_set(struct sock *sk, struct dst_entry *dst);
int udp_get_port(struct sock *sk, unsigned short snum,
		 int (*saddr_cmp)(const struct sock *,
				  const struct sock *));
void udp_err(struct sk_buff *, u32);
int udp_abort(struct sock *sk, int err);
int udp_sendmsg(struct sock *sk, struct msghdr *msg, size_t len);
int udp_push_pending_frames(struct sock *sk);
void udp_flush_pending_frames(struct sock *sk);
void udp4_hwcsum(struct sk_buff *skb, __be32 src, __be32 dst);
int udp_rcv(struct sk_buff *skb);
int udp_ioctl(struct sock *sk, int cmd, unsigned long arg);
int udp_init_sock(struct sock *sk);
int __udp_disconnect(struct sock *sk, int flags);
int udp_disconnect(struct sock *sk, int flags);
unsigned int udp_poll(struct file *file, struct socket *sock, poll_table *wait);
struct sk_buff *skb_udp_tunnel_segment(struct sk_buff *skb,
				       netdev_features_t features,
				       bool is_ipv6);
int udp_lib_getsockopt(struct sock *sk, int level, int optname,
		       char __user *optval, int __user *optlen);
int udp_lib_setsockopt(struct sock *sk, int level, int optname,
		       char __user *optval, unsigned int optlen,
		       int (*push_pending_frames)(struct sock *));
struct sock *udp4_lib_lookup(struct net *net, __be32 saddr, __be16 sport,
			     __be32 daddr, __be16 dport, int dif);
struct sock *__udp4_lib_lookup(struct net *net, __be32 saddr, __be16 sport,
			       __be32 daddr, __be16 dport, int dif, int sdif,
			       struct udp_table *tbl, struct sk_buff *skb);
struct sock *udp4_lib_lookup_skb(struct sk_buff *skb,
				 __be16 sport, __be16 dport);
struct sock *udp6_lib_lookup(struct net *net,
			     const struct in6_addr *saddr, __be16 sport,
			     const struct in6_addr *daddr, __be16 dport,
			     int dif);
struct sock *__udp6_lib_lookup(struct net *net,
			       const struct in6_addr *saddr, __be16 sport,
			       const struct in6_addr *daddr, __be16 dport,
			       int dif, int sdif, struct udp_table *tbl,
			       struct sk_buff *skb);
struct sock *udp6_lib_lookup_skb(struct sk_buff *skb,
				 __be16 sport, __be16 dport);


struct udp_dev_scratch {
	
	u32 _tsize_state;

#if BITS_PER_LONG == 64
	
	u16 len;
	bool is_linear;
	bool csum_unnecessary;
#endif
};

static inline struct udp_dev_scratch *udp_skb_scratch(struct sk_buff *skb)
{
	return (struct udp_dev_scratch *)&skb->dev_scratch;
}

#if BITS_PER_LONG == 64
static inline unsigned int udp_skb_len(struct sk_buff *skb)
{
	return udp_skb_scratch(skb)->len;
}

static inline bool udp_skb_csum_unnecessary(struct sk_buff *skb)
{
	return udp_skb_scratch(skb)->csum_unnecessary;
}

static inline bool udp_skb_is_linear(struct sk_buff *skb)
{
	return udp_skb_scratch(skb)->is_linear;
}

#else
static inline unsigned int udp_skb_len(struct sk_buff *skb)
{
	return skb->len;
}

static inline bool udp_skb_csum_unnecessary(struct sk_buff *skb)
{
	return skb_csum_unnecessary(skb);
}

static inline bool udp_skb_is_linear(struct sk_buff *skb)
{
	return !skb_is_nonlinear(skb);
}
#endif

static inline int copy_linear_skb(struct sk_buff *skb, int len, int off,
				  struct iov_iter *to)
{
	int n;

	n = copy_to_iter(skb->data + off, len, to);
	if (n == len)
		return 0;

	iov_iter_revert(to, n);
	return -EFAULT;
}


#define UDP_INC_STATS(net, field, is_udplite)		      do { \
	if (is_udplite) SNMP_INC_STATS((net)->mib.udplite_statistics, field);       \
	else		SNMP_INC_STATS((net)->mib.udp_statistics, field);  }  while(0)
#define __UDP_INC_STATS(net, field, is_udplite) 	      do { \
	if (is_udplite) __SNMP_INC_STATS((net)->mib.udplite_statistics, field);         \
	else		__SNMP_INC_STATS((net)->mib.udp_statistics, field);    }  while(0)

#define __UDP6_INC_STATS(net, field, is_udplite)	    do { \
	if (is_udplite) __SNMP_INC_STATS((net)->mib.udplite_stats_in6, field);\
	else		__SNMP_INC_STATS((net)->mib.udp_stats_in6, field);  \
} while(0)
#define UDP6_INC_STATS(net, field, __lite)		    do { \
	if (__lite) SNMP_INC_STATS((net)->mib.udplite_stats_in6, field);  \
	else	    SNMP_INC_STATS((net)->mib.udp_stats_in6, field);      \
} while(0)

#if IS_ENABLED(CONFIG_IPV6)
#define __UDPX_MIB(sk, ipv4)						\
({									\
	ipv4 ? (IS_UDPLITE(sk) ? sock_net(sk)->mib.udplite_statistics :	\
				 sock_net(sk)->mib.udp_statistics) :	\
		(IS_UDPLITE(sk) ? sock_net(sk)->mib.udplite_stats_in6 :	\
				 sock_net(sk)->mib.udp_stats_in6);	\
})
#else
#define __UDPX_MIB(sk, ipv4)						\
({									\
	IS_UDPLITE(sk) ? sock_net(sk)->mib.udplite_statistics :		\
			 sock_net(sk)->mib.udp_statistics;		\
})
#endif
#define __UDPX_INC_STATS(sk, field) \
	__SNMP_INC_STATS(__UDPX_MIB(sk, (sk)->sk_family == AF_INET), field)

int udp_seq_open(struct inode *inode, struct file *file);

struct udp_seq_afinfo {
	char				*name;
	sa_family_t			family;
	struct udp_table		*udp_table;
	const struct file_operations	*seq_fops;
	struct seq_operations		seq_ops;
};

struct udp_iter_state {
	struct seq_net_private  p;
	sa_family_t		family;
	int			bucket;
	struct udp_table	*udp_table;
};

#ifdef CONFIG_PROC_FS
int udp_proc_register(struct net *net, struct udp_seq_afinfo *afinfo);
void udp_proc_unregister(struct net *net, struct udp_seq_afinfo *afinfo);

int udp4_proc_init(void);
void udp4_proc_exit(void);
#endif

int udpv4_offload_init(void);

void udp_init(void);

static struct static_key udp_encap_needed __read_mostly;
void udp_encap_enable(void);
#if IS_ENABLED(CONFIG_IPV6)
void udpv6_encap_enable(void);
#endif

static inline struct sk_buff *udp_rcv_segment(struct sock *sk,
					      struct sk_buff *skb, bool ipv4)
{
	struct sk_buff *segs;

	
	segs = __skb_gso_segment(skb, NETIF_F_SG, false);
	if (unlikely(IS_ERR_OR_NULL(segs))) {
		int segs_nr = skb_shinfo(skb)->gso_segs;

		atomic_add(segs_nr, &sk->sk_drops);
		SNMP_ADD_STATS(__UDPX_MIB(sk, ipv4), UDP_MIB_INERRORS, segs_nr);
		kfree_skb(skb);
		return NULL;
	}

	consume_skb(skb);
	return segs;
}

#endif	
