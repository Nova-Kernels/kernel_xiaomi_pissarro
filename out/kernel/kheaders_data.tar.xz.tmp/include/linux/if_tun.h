
#ifndef __IF_TUN_H
#define __IF_TUN_H

#include <uapi/linux/if_tun.h>

#if defined(CONFIG_TUN) || defined(CONFIG_TUN_MODULE)
struct socket *tun_get_socket(struct file *);
struct skb_array *tun_get_skb_array(struct file *file);
#else
#include <linux/err.h>
#include <linux/errno.h>
struct file;
struct socket;
static inline struct socket *tun_get_socket(struct file *f)
{
	return ERR_PTR(-EINVAL);
}
static inline struct skb_array *tun_get_skb_array(struct file *f)
{
	return ERR_PTR(-EINVAL);
}
#endif 
#endif 
