/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_IOMAP_H
#define LINUX_IOMAP_H 1

#include <linux/types.h>

struct fiemap_extent_info;
struct inode;
struct iov_iter;
struct kiocb;
struct vm_area_struct;
struct vm_fault;


#define IOMAP_HOLE	0x01	
#define IOMAP_DELALLOC	0x02	
#define IOMAP_MAPPED	0x03	
#define IOMAP_UNWRITTEN	0x04	


#define IOMAP_F_NEW	0x01	


#define IOMAP_F_MERGED	0x10	
#define IOMAP_F_SHARED	0x20	


#define IOMAP_NULL_BLOCK -1LL	

struct iomap {
	sector_t		blkno;	
	loff_t			offset;	
	u64			length;	
	u16			type;	
	u16			flags;	
	struct block_device	*bdev;	
	struct dax_device	*dax_dev; 
};


#define IOMAP_WRITE		(1 << 0) 
#define IOMAP_ZERO		(1 << 1) 
#define IOMAP_REPORT		(1 << 2) 
#define IOMAP_FAULT		(1 << 3) 
#define IOMAP_DIRECT		(1 << 4) 
#define IOMAP_NOWAIT		(1 << 5) 

struct iomap_ops {
	
	int (*iomap_begin)(struct inode *inode, loff_t pos, loff_t length,
			unsigned flags, struct iomap *iomap);

	
	int (*iomap_end)(struct inode *inode, loff_t pos, loff_t length,
			ssize_t written, unsigned flags, struct iomap *iomap);
};

ssize_t iomap_file_buffered_write(struct kiocb *iocb, struct iov_iter *from,
		const struct iomap_ops *ops);
int iomap_file_dirty(struct inode *inode, loff_t pos, loff_t len,
		const struct iomap_ops *ops);
int iomap_zero_range(struct inode *inode, loff_t pos, loff_t len,
		bool *did_zero, const struct iomap_ops *ops);
int iomap_truncate_page(struct inode *inode, loff_t pos, bool *did_zero,
		const struct iomap_ops *ops);
int iomap_page_mkwrite(struct vm_fault *vmf, const struct iomap_ops *ops);
int iomap_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		loff_t start, loff_t len, const struct iomap_ops *ops);
loff_t iomap_seek_hole(struct inode *inode, loff_t offset,
		const struct iomap_ops *ops);
loff_t iomap_seek_data(struct inode *inode, loff_t offset,
		const struct iomap_ops *ops);


#define IOMAP_DIO_UNWRITTEN	(1 << 0)	
#define IOMAP_DIO_COW		(1 << 1)	
typedef int (iomap_dio_end_io_t)(struct kiocb *iocb, ssize_t ret,
		unsigned flags);
ssize_t iomap_dio_rw(struct kiocb *iocb, struct iov_iter *iter,
		const struct iomap_ops *ops, iomap_dio_end_io_t end_io);

#endif 
