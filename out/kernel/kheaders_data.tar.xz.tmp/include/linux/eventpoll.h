
#ifndef _LINUX_EVENTPOLL_H
#define _LINUX_EVENTPOLL_H

#include <uapi/linux/eventpoll.h>
#include <uapi/linux/kcmp.h>



struct file;


#ifdef CONFIG_EPOLL

#ifdef CONFIG_CHECKPOINT_RESTORE
struct file *get_epoll_tfile_raw_ptr(struct file *file, int tfd, unsigned long toff);
#endif


static inline void eventpoll_init_file(struct file *file)
{
	INIT_LIST_HEAD(&file->f_ep_links);
	INIT_LIST_HEAD(&file->f_tfile_llink);
}



void eventpoll_release_file(struct file *file);


static inline void eventpoll_release(struct file *file)
{

	
	if (likely(list_empty(&file->f_ep_links)))
		return;

	
	eventpoll_release_file(file);
}

#else

static inline void eventpoll_init_file(struct file *file) {}
static inline void eventpoll_release(struct file *file) {}

#endif

#endif 
