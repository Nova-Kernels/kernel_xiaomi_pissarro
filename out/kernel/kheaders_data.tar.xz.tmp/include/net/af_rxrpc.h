

#ifndef _NET_RXRPC_H
#define _NET_RXRPC_H

#include <linux/rxrpc.h>

struct key;
struct sock;
struct socket;
struct rxrpc_call;


enum rxrpc_call_completion {
	RXRPC_CALL_SUCCEEDED,		
	RXRPC_CALL_REMOTELY_ABORTED,	
	RXRPC_CALL_LOCALLY_ABORTED,	
	RXRPC_CALL_LOCAL_ERROR,		
	RXRPC_CALL_NETWORK_ERROR,	
	NR__RXRPC_CALL_COMPLETIONS
};

typedef void (*rxrpc_notify_rx_t)(struct sock *, struct rxrpc_call *,
				  unsigned long);
typedef void (*rxrpc_notify_end_tx_t)(struct sock *, struct rxrpc_call *,
				      unsigned long);
typedef void (*rxrpc_notify_new_call_t)(struct sock *, struct rxrpc_call *,
					unsigned long);
typedef void (*rxrpc_discard_new_call_t)(struct rxrpc_call *, unsigned long);
typedef void (*rxrpc_user_attach_call_t)(struct rxrpc_call *, unsigned long);

void rxrpc_kernel_new_call_notification(struct socket *,
					rxrpc_notify_new_call_t,
					rxrpc_discard_new_call_t);
struct rxrpc_call *rxrpc_kernel_begin_call(struct socket *,
					   struct sockaddr_rxrpc *,
					   struct key *,
					   unsigned long,
					   s64,
					   gfp_t,
					   rxrpc_notify_rx_t);
int rxrpc_kernel_send_data(struct socket *, struct rxrpc_call *,
			   struct msghdr *, size_t,
			   rxrpc_notify_end_tx_t);
int rxrpc_kernel_recv_data(struct socket *, struct rxrpc_call *,
			   void *, size_t, size_t *, bool, u32 *);
bool rxrpc_kernel_abort_call(struct socket *, struct rxrpc_call *,
			     u32, int, const char *);
void rxrpc_kernel_end_call(struct socket *, struct rxrpc_call *);
void rxrpc_kernel_get_peer(struct socket *, struct rxrpc_call *,
			   struct sockaddr_rxrpc *);
int rxrpc_kernel_charge_accept(struct socket *, rxrpc_notify_rx_t,
			       rxrpc_user_attach_call_t, unsigned long, gfp_t);
void rxrpc_kernel_set_tx_length(struct socket *, struct rxrpc_call *, s64);
int rxrpc_kernel_retry_call(struct socket *, struct rxrpc_call *,
			    struct sockaddr_rxrpc *, struct key *);
int rxrpc_kernel_check_call(struct socket *, struct rxrpc_call *,
			    enum rxrpc_call_completion *, u32 *);

#endif 
