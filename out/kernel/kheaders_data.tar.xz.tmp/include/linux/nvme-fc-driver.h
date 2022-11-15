

#ifndef _NVME_FC_DRIVER_H
#define _NVME_FC_DRIVER_H 1







#define FC_PORT_ROLE_NVME_INITIATOR	0x10
#define FC_PORT_ROLE_NVME_TARGET	0x20
#define FC_PORT_ROLE_NVME_DISCOVERY	0x40



struct nvme_fc_port_info {
	u64			node_name;
	u64			port_name;
	u32			port_role;
	u32			port_id;
};



struct nvmefc_ls_req {
	void			*rqstaddr;
	dma_addr_t		rqstdma;
	u32			rqstlen;
	void			*rspaddr;
	dma_addr_t		rspdma;
	u32			rsplen;
	u32			timeout;

	void			*private;

	void (*done)(struct nvmefc_ls_req *req, int status);

} __aligned(sizeof(u64));	


enum nvmefc_fcp_datadir {
	NVMEFC_FCP_NODATA,	
	NVMEFC_FCP_WRITE,
	NVMEFC_FCP_READ,
};


#define NVME_FC_MAX_SEGMENTS		256


struct nvmefc_fcp_req {
	void			*cmdaddr;
	void			*rspaddr;
	dma_addr_t		cmddma;
	dma_addr_t		rspdma;
	u16			cmdlen;
	u16			rsplen;

	u32			payload_length;
	struct sg_table		sg_table;
	struct scatterlist	*first_sgl;
	int			sg_cnt;
	enum nvmefc_fcp_datadir	io_dir;

	__le16			sqid;

	void (*done)(struct nvmefc_fcp_req *req);

	void			*private;

	u32			transferred_length;
	u16			rcv_rsplen;
	u32			status;
} __aligned(sizeof(u64));	



enum nvme_fc_obj_state {
	FC_OBJSTATE_UNKNOWN,
	FC_OBJSTATE_NOTPRESENT,
	FC_OBJSTATE_ONLINE,
	FC_OBJSTATE_OFFLINE,		
	FC_OBJSTATE_BLOCKED,
	FC_OBJSTATE_BYPASSED,
	FC_OBJSTATE_DIAGNOSTICS,
	FC_OBJSTATE_LINKDOWN,
	FC_OBJSTATE_ERROR,
	FC_OBJSTATE_LOOPBACK,
	FC_OBJSTATE_DELETED,
};



struct nvme_fc_local_port {
	
	u32 port_num;
	u32 port_role;
	u64 node_name;
	u64 port_name;

	void *private;

	
	u32 port_id;
	enum nvme_fc_obj_state port_state;
} __aligned(sizeof(u64));	



struct nvme_fc_remote_port {
	
	u32 port_num;
	u32 port_role;
	u64 node_name;
	u64 port_name;

	struct nvme_fc_local_port *localport;

	void *private;

	
	u32 port_id;
	enum nvme_fc_obj_state port_state;
} __aligned(sizeof(u64));	



struct nvme_fc_port_template {
	
	void	(*localport_delete)(struct nvme_fc_local_port *);
	void	(*remoteport_delete)(struct nvme_fc_remote_port *);
	int	(*create_queue)(struct nvme_fc_local_port *,
				unsigned int qidx, u16 qsize,
				void **handle);
	void	(*delete_queue)(struct nvme_fc_local_port *,
				unsigned int qidx, void *handle);
	void	(*poll_queue)(struct nvme_fc_local_port *, void *handle);
	int	(*ls_req)(struct nvme_fc_local_port *,
				struct nvme_fc_remote_port *,
				struct nvmefc_ls_req *);
	int	(*fcp_io)(struct nvme_fc_local_port *,
				struct nvme_fc_remote_port *,
				void *hw_queue_handle,
				struct nvmefc_fcp_req *);
	void	(*ls_abort)(struct nvme_fc_local_port *,
				struct nvme_fc_remote_port *,
				struct nvmefc_ls_req *);
	void	(*fcp_abort)(struct nvme_fc_local_port *,
				struct nvme_fc_remote_port *,
				void *hw_queue_handle,
				struct nvmefc_fcp_req *);

	u32	max_hw_queues;
	u16	max_sgl_segments;
	u16	max_dif_sgl_segments;
	u64	dma_boundary;

	
	u32	local_priv_sz;
	u32	remote_priv_sz;
	u32	lsrqst_priv_sz;
	u32	fcprqst_priv_sz;
};




int nvme_fc_register_localport(struct nvme_fc_port_info *pinfo,
			struct nvme_fc_port_template *template,
			struct device *dev,
			struct nvme_fc_local_port **lport_p);

int nvme_fc_unregister_localport(struct nvme_fc_local_port *localport);

int nvme_fc_register_remoteport(struct nvme_fc_local_port *localport,
			struct nvme_fc_port_info *pinfo,
			struct nvme_fc_remote_port **rport_p);

int nvme_fc_unregister_remoteport(struct nvme_fc_remote_port *remoteport);






struct nvmet_fc_port_info {
	u64			node_name;
	u64			port_name;
	u32			port_id;
};



struct nvmefc_tgt_ls_req {
	void		*rspbuf;
	dma_addr_t	rspdma;
	u16		rsplen;

	void (*done)(struct nvmefc_tgt_ls_req *req);
	void *nvmet_fc_private;		
};


enum {
	NVMET_FCOP_READDATA	= 1,	
	NVMET_FCOP_WRITEDATA	= 2,	
	NVMET_FCOP_READDATA_RSP	= 3,	
	NVMET_FCOP_RSP		= 4,	
};


struct nvmefc_tgt_fcp_req {
	u8			op;
	u16			hwqid;
	u32			offset;
	u32			timeout;
	u32			transfer_length;
	struct fc_ba_rjt	ba_rjt;
	struct scatterlist	*sg;
	int			sg_cnt;
	void			*rspaddr;
	dma_addr_t		rspdma;
	u16			rsplen;

	void (*done)(struct nvmefc_tgt_fcp_req *);

	void *nvmet_fc_private;		

	u32			transferred_length;
	int			fcp_error;
};



enum {
	NVMET_FCTGTFEAT_READDATA_RSP = (1 << 0),
		
	NVMET_FCTGTFEAT_CMD_IN_ISR = (1 << 1),
		
	NVMET_FCTGTFEAT_OPDONE_IN_ISR = (1 << 2),
		
};



struct nvmet_fc_target_port {
	
	u32 port_num;
	u64 node_name;
	u64 port_name;

	void *private;

	
	u32 port_id;
	enum nvme_fc_obj_state port_state;
} __aligned(sizeof(u64));	



struct nvmet_fc_target_template {
	void (*targetport_delete)(struct nvmet_fc_target_port *tgtport);
	int (*xmt_ls_rsp)(struct nvmet_fc_target_port *tgtport,
				struct nvmefc_tgt_ls_req *tls_req);
	int (*fcp_op)(struct nvmet_fc_target_port *tgtport,
				struct nvmefc_tgt_fcp_req *fcpreq);
	void (*fcp_abort)(struct nvmet_fc_target_port *tgtport,
				struct nvmefc_tgt_fcp_req *fcpreq);
	void (*fcp_req_release)(struct nvmet_fc_target_port *tgtport,
				struct nvmefc_tgt_fcp_req *fcpreq);
	void (*defer_rcv)(struct nvmet_fc_target_port *tgtport,
				struct nvmefc_tgt_fcp_req *fcpreq);

	u32	max_hw_queues;
	u16	max_sgl_segments;
	u16	max_dif_sgl_segments;
	u64	dma_boundary;

	u32	target_features;

	u32	target_priv_sz;
};


int nvmet_fc_register_targetport(struct nvmet_fc_port_info *portinfo,
			struct nvmet_fc_target_template *template,
			struct device *dev,
			struct nvmet_fc_target_port **tgtport_p);

int nvmet_fc_unregister_targetport(struct nvmet_fc_target_port *tgtport);

int nvmet_fc_rcv_ls_req(struct nvmet_fc_target_port *tgtport,
			struct nvmefc_tgt_ls_req *lsreq,
			void *lsreqbuf, u32 lsreqbuf_len);

int nvmet_fc_rcv_fcp_req(struct nvmet_fc_target_port *tgtport,
			struct nvmefc_tgt_fcp_req *fcpreq,
			void *cmdiubuf, u32 cmdiubuf_len);

void nvmet_fc_rcv_fcp_abort(struct nvmet_fc_target_port *tgtport,
			struct nvmefc_tgt_fcp_req *fcpreq);

#endif 
