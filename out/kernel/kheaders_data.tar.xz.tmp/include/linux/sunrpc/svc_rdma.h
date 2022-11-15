

#ifndef SVC_RDMA_H
#define SVC_RDMA_H
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/svcsock.h>
#include <linux/sunrpc/rpc_rdma.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#define SVCRDMA_DEBUG


enum {
	RPCRDMA_DEF_INLINE_THRESH = 4096,
	RPCRDMA_MAX_INLINE_THRESH = 65536
};


extern unsigned int svcrdma_ord;
extern unsigned int svcrdma_max_requests;
extern unsigned int svcrdma_max_bc_requests;
extern unsigned int svcrdma_max_req_size;

extern atomic_t rdma_stat_recv;
extern atomic_t rdma_stat_read;
extern atomic_t rdma_stat_write;
extern atomic_t rdma_stat_sq_starve;
extern atomic_t rdma_stat_rq_starve;
extern atomic_t rdma_stat_rq_poll;
extern atomic_t rdma_stat_rq_prod;
extern atomic_t rdma_stat_sq_poll;
extern atomic_t rdma_stat_sq_prod;


struct svc_rdma_op_ctxt {
	struct list_head list;
	struct xdr_buf arg;
	struct ib_cqe cqe;
	u32 byte_len;
	struct svcxprt_rdma *xprt;
	enum dma_data_direction direction;
	int count;
	unsigned int mapped_sges;
	int hdr_count;
	struct ib_send_wr send_wr;
	struct ib_sge sge[1 + RPCRDMA_MAX_INLINE_THRESH / PAGE_SIZE];
	struct page *pages[RPCSVC_MAXPAGES];
};

struct svcxprt_rdma {
	struct svc_xprt      sc_xprt;		
	struct rdma_cm_id    *sc_cm_id;		
	struct list_head     sc_accept_q;	
	int		     sc_ord;		
	int                  sc_max_sge;
	bool		     sc_snd_w_inv;	

	atomic_t             sc_sq_avail;	
	unsigned int	     sc_sq_depth;	
	unsigned int	     sc_rq_depth;	
	__be32		     sc_fc_credits;	
	u32		     sc_max_requests;	
	u32		     sc_max_bc_requests;
	int                  sc_max_req_size;	
	u8		     sc_port_num;

	struct ib_pd         *sc_pd;

	spinlock_t	     sc_ctxt_lock;
	struct list_head     sc_ctxts;
	int		     sc_ctxt_used;
	spinlock_t	     sc_rw_ctxt_lock;
	struct list_head     sc_rw_ctxts;

	struct list_head     sc_rq_dto_q;
	spinlock_t	     sc_rq_dto_lock;
	struct ib_qp         *sc_qp;
	struct ib_cq         *sc_rq_cq;
	struct ib_cq         *sc_sq_cq;

	spinlock_t	     sc_lock;		

	wait_queue_head_t    sc_send_wait;	
	unsigned long	     sc_flags;
	struct list_head     sc_read_complete_q;
	struct work_struct   sc_work;
};

#define RDMAXPRT_CONN_PENDING	3

#define RPCRDMA_LISTEN_BACKLOG  10

#define RPCRDMA_ORD             (64/4)
#define RPCRDMA_MAX_REQUESTS    32


#define RPCRDMA_MAX_BC_REQUESTS	2

#define RPCSVC_MAXPAYLOAD_RDMA	RPCSVC_MAXPAYLOAD


static inline void svc_rdma_count_mappings(struct svcxprt_rdma *rdma,
					   struct svc_rdma_op_ctxt *ctxt)
{
	ctxt->mapped_sges++;
}


extern int svc_rdma_handle_bc_reply(struct rpc_xprt *xprt,
				    __be32 *rdma_resp,
				    struct xdr_buf *rcvbuf);


extern int svc_rdma_recvfrom(struct svc_rqst *);


extern void svc_rdma_destroy_rw_ctxts(struct svcxprt_rdma *rdma);
extern int svc_rdma_recv_read_chunk(struct svcxprt_rdma *rdma,
				    struct svc_rqst *rqstp,
				    struct svc_rdma_op_ctxt *head, __be32 *p);
extern int svc_rdma_send_write_chunk(struct svcxprt_rdma *rdma,
				     __be32 *wr_ch, struct xdr_buf *xdr);
extern int svc_rdma_send_reply_chunk(struct svcxprt_rdma *rdma,
				     __be32 *rp_ch, bool writelist,
				     struct xdr_buf *xdr);


extern int svc_rdma_map_reply_hdr(struct svcxprt_rdma *rdma,
				  struct svc_rdma_op_ctxt *ctxt,
				  __be32 *rdma_resp, unsigned int len);
extern int svc_rdma_post_send_wr(struct svcxprt_rdma *rdma,
				 struct svc_rdma_op_ctxt *ctxt,
				 int num_sge, u32 inv_rkey);
extern int svc_rdma_sendto(struct svc_rqst *);


extern void svc_rdma_wc_send(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_reg(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_read(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_inv(struct ib_cq *, struct ib_wc *);
extern int svc_rdma_send(struct svcxprt_rdma *, struct ib_send_wr *);
extern int svc_rdma_post_recv(struct svcxprt_rdma *, gfp_t);
extern int svc_rdma_repost_recv(struct svcxprt_rdma *, gfp_t);
extern int svc_rdma_create_listen(struct svc_serv *, int, struct sockaddr *);
extern struct svc_rdma_op_ctxt *svc_rdma_get_context(struct svcxprt_rdma *);
extern void svc_rdma_put_context(struct svc_rdma_op_ctxt *, int);
extern void svc_rdma_unmap_dma(struct svc_rdma_op_ctxt *ctxt);
extern void svc_sq_reap(struct svcxprt_rdma *);
extern void svc_rq_reap(struct svcxprt_rdma *);
extern void svc_rdma_prep_reply_hdr(struct svc_rqst *);

extern struct svc_xprt_class svc_rdma_class;
#ifdef CONFIG_SUNRPC_BACKCHANNEL
extern struct svc_xprt_class svc_rdma_bc_class;
#endif


extern struct workqueue_struct *svc_rdma_wq;
extern int svc_rdma_init(void);
extern void svc_rdma_cleanup(void);

#endif
