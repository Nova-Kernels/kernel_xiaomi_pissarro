/* SPDX-License-Identifier: GPL-2.0 */


#ifndef LINUX_LOCKD_BIND_H
#define LINUX_LOCKD_BIND_H

#include <linux/lockd/nlm.h>

#include <linux/lockd/xdr.h>
#ifdef CONFIG_LOCKD_V4
#include <linux/lockd/xdr4.h>
#endif


struct svc_rqst;
struct rpc_task;


struct nlmsvc_binding {
	__be32			(*fopen)(struct svc_rqst *,
						struct nfs_fh *,
						struct file **);
	void			(*fclose)(struct file *);
};

extern const struct nlmsvc_binding *nlmsvc_ops;


struct nlmclnt_initdata {
	const char		*hostname;
	const struct sockaddr	*address;
	size_t			addrlen;
	unsigned short		protocol;
	u32			nfs_version;
	int			noresvport;
	struct net		*net;
	const struct nlmclnt_operations	*nlmclnt_ops;
};



extern struct nlm_host *nlmclnt_init(const struct nlmclnt_initdata *nlm_init);
extern void	nlmclnt_done(struct nlm_host *host);


struct nlmclnt_operations {
	
	void (*nlmclnt_alloc_call)(void *);

	
	bool (*nlmclnt_unlock_prepare)(struct rpc_task*, void *);

	
	void (*nlmclnt_release_call)(void *);
};

extern int	nlmclnt_proc(struct nlm_host *host, int cmd, struct file_lock *fl, void *data);
extern int	lockd_up(struct net *net);
extern void	lockd_down(struct net *net);

#endif 
