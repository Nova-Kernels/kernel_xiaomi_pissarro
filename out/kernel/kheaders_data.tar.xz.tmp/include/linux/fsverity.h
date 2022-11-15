/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_FSVERITY_H
#define _LINUX_FSVERITY_H

#include <linux/fs.h>
#include <uapi/linux/fsverity.h>


struct fsverity_operations {

	
	int (*begin_enable_verity)(struct file *filp);

	
	int (*end_enable_verity)(struct file *filp, const void *desc,
				 size_t desc_size, u64 merkle_tree_size);

	
	int (*get_verity_descriptor)(struct inode *inode, void *buf,
				     size_t bufsize);

	
	struct page *(*read_merkle_tree_page)(struct inode *inode,
					      pgoff_t index,
					      unsigned long num_ra_pages);

	
	int (*write_merkle_tree_block)(struct inode *inode, const void *buf,
				       u64 index, int log_blocksize);
};

#ifdef CONFIG_FS_VERITY

static inline struct fsverity_info *fsverity_get_info(const struct inode *inode)
{
	
	return READ_ONCE(inode->i_verity_info);
}



int fsverity_ioctl_enable(struct file *filp, const void __user *arg);



int fsverity_ioctl_measure(struct file *filp, void __user *arg);



int fsverity_file_open(struct inode *inode, struct file *filp);
int fsverity_prepare_setattr(struct dentry *dentry, struct iattr *attr);
void fsverity_cleanup_inode(struct inode *inode);



bool fsverity_verify_page(struct page *page);
void fsverity_verify_bio(struct bio *bio);
void fsverity_enqueue_verify_work(struct work_struct *work);

#else 

static inline struct fsverity_info *fsverity_get_info(const struct inode *inode)
{
	return NULL;
}



static inline int fsverity_ioctl_enable(struct file *filp,
					const void __user *arg)
{
	return -EOPNOTSUPP;
}



static inline int fsverity_ioctl_measure(struct file *filp, void __user *arg)
{
	return -EOPNOTSUPP;
}



static inline int fsverity_file_open(struct inode *inode, struct file *filp)
{
	return IS_VERITY(inode) ? -EOPNOTSUPP : 0;
}

static inline int fsverity_prepare_setattr(struct dentry *dentry,
					   struct iattr *attr)
{
	return IS_VERITY(d_inode(dentry)) ? -EOPNOTSUPP : 0;
}

static inline void fsverity_cleanup_inode(struct inode *inode)
{
}



static inline bool fsverity_verify_page(struct page *page)
{
	WARN_ON(1);
	return false;
}

static inline void fsverity_verify_bio(struct bio *bio)
{
	WARN_ON(1);
}

static inline void fsverity_enqueue_verify_work(struct work_struct *work)
{
	WARN_ON(1);
}

#endif	


static inline bool fsverity_active(const struct inode *inode)
{
	return fsverity_get_info(inode) != NULL;
}

#endif	
