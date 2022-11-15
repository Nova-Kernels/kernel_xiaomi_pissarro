
#ifndef _LINUX_UDP_H
#define _LINUX_UDP_H

#include <net/inet_sock.h>
#include <linux/skbuff.h>
#include <net/netns/hash.h>
#include <uapi/linux/udp.h>

static inline struct udphdr *udp_hdr(const struct sk_buff *skb)
{
	return (struct udphdr *)skb_transport_header(skb);
}

static inline struct udphdr *inner_udp_hdr(const struct sk_buff *skb)
{
	return (struct udphdr *)skb_inner_transport_header(skb);
}

#define UDP_HTABLE_SIZE_MIN		(CONFIG_BASE_SMALL ? 128 : 256)

static inline u32 udp_hashfn(const struct net *net, u32 num, u32 mask)
{
	return (num + net_hash_mix(net)) & mask;
}

struct udp_sock {
	
	struct inet_sock inet;
#define udp_port_hash		inet.sk.__sk_common.skc_u16hashes[0]
#define udp_portaddr_hash	inet.sk.__sk_common.skc_u16hashes[1]
#define udp_portaddr_node	inet.sk.__sk_common.skc_portaddr_node
	int		 pending;	
	unsigned int	 corkflag;	
	__u8		 encap_type;	
	unsigned char	 no_check6_tx:1,
			 no_check6_rx:1,
			 gro_enabled:1;	
	
	__u16		 len;		
	__u16		 gso_size;
	
	__u16		 pcslen;
	__u16		 pcrlen;

#define UDPLITE_BIT      0x1  		
#define UDPLITE_SEND_CC  0x2  		
#define UDPLITE_RECV_CC  0x4		
	__u8		 pcflag;        
	__u8		 unused[3];
	
	int (*encap_rcv)(struct sock *sk, struct sk_buff *skb);
	void (*encap_destroy)(struct sock *sk);

	
	struct sk_buff *	(*gro_receive)(struct sock *sk,
					       struct list_head *head,
					       struct sk_buff *skb);
	int			(*gro_complete)(struct sock *sk,
						struct sk_buff *skb,
						int nhoff);

	
	struct sk_buff_head	reader_queue ____cacheline_aligned_in_smp;

	
	int		forward_deficit;
};

#define UDP_MAX_SEGMENTS	(1 << 6UL)

static inline struct udp_sock *udp_sk(const struct sock *sk)
{
	return (struct udp_sock *)sk;
}

static inline void udp_set_no_check6_tx(struct sock *sk, bool val)
{
	udp_sk(sk)->no_check6_tx = val;
}

static inline void udp_set_no_check6_rx(struct sock *sk, bool val)
{
	udp_sk(sk)->no_check6_rx = val;
}

static inline bool udp_get_no_check6_tx(struct sock *sk)
{
	return udp_sk(sk)->no_check6_tx;
}

static inline bool udp_get_no_check6_rx(struct sock *sk)
{
	return udp_sk(sk)->no_check6_rx;
}

static inline void udp_cmsg_recv(struct msghdr *msg, struct sock *sk,
				 struct sk_buff *skb)
{
	int gso_size;

	if (skb_shinfo(skb)->gso_type & SKB_GSO_UDP_L4) {
		gso_size = skb_shinfo(skb)->gso_size;
		put_cmsg(msg, SOL_UDP, UDP_GRO, sizeof(gso_size), &gso_size);
	}
}

static inline bool udp_unexpected_gso(struct sock *sk, struct sk_buff *skb)
{
	return (!sk || !udp_sk(sk)->gro_receive) && skb_is_gso(skb) &&
	       skb_shinfo(skb)->gso_type & SKB_GSO_UDP_L4;
}

#define udp_portaddr_for_each_entry(__sk, list) \
	hlist_for_each_entry(__sk, list, __sk_common.skc_portaddr_node)

#define udp_portaddr_for_each_entry_rcu(__sk, list) \
	hlist_for_each_entry_rcu(__sk, list, __sk_common.skc_portaddr_node)

#define IS_UDPLITE(__sk) (__sk->sk_protocol == IPPROTO_UDPLITE)

#endif	
