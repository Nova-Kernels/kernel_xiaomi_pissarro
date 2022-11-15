

#ifndef _LINUX_SYNC_FILE_H
#define _LINUX_SYNC_FILE_H

#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/dma-fence.h>
#include <linux/dma-fence-array.h>


struct sync_file {
	struct file		*file;
	
	char			user_name[32];
#ifdef CONFIG_DEBUG_FS
	struct list_head	sync_file_list;
#endif

	wait_queue_head_t	wq;
	unsigned long		flags;

	struct dma_fence	*fence;
	struct dma_fence_cb cb;
};

#define POLL_ENABLED 0

struct sync_file *sync_file_create(struct dma_fence *fence);
struct dma_fence *sync_file_get_fence(int fd);
char *sync_file_get_name(struct sync_file *sync_file, char *buf, int len);

#endif 
