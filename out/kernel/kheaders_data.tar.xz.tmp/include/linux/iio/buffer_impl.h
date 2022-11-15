/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _IIO_BUFFER_GENERIC_IMPL_H_
#define _IIO_BUFFER_GENERIC_IMPL_H_
#include <linux/sysfs.h>
#include <linux/kref.h>

#ifdef CONFIG_IIO_BUFFER

struct iio_dev;
struct iio_buffer;


#define INDIO_BUFFER_FLAG_FIXED_WATERMARK BIT(0)


struct iio_buffer_access_funcs {
	int (*store_to)(struct iio_buffer *buffer, const void *data);
	int (*read_first_n)(struct iio_buffer *buffer,
			    size_t n,
			    char __user *buf);
	size_t (*data_available)(struct iio_buffer *buffer);

	int (*request_update)(struct iio_buffer *buffer);

	int (*set_bytes_per_datum)(struct iio_buffer *buffer, size_t bpd);
	int (*set_length)(struct iio_buffer *buffer, unsigned int length);

	int (*enable)(struct iio_buffer *buffer, struct iio_dev *indio_dev);
	int (*disable)(struct iio_buffer *buffer, struct iio_dev *indio_dev);

	void (*release)(struct iio_buffer *buffer);

	unsigned int modes;
	unsigned int flags;
};


struct iio_buffer {
	
	unsigned int length;

	
	size_t bytes_per_datum;

	
	const struct iio_buffer_access_funcs *access;

	
	long *scan_mask;

	
	struct list_head demux_list;

	
	wait_queue_head_t pollq;

	
	unsigned int watermark;

	
	
	struct attribute_group *scan_el_attrs;

	
	bool scan_timestamp;

	
	struct list_head scan_el_dev_attr_list;

	
	struct attribute_group buffer_group;

	
	struct attribute_group scan_el_group;

	
	bool stufftoread;

	
	const struct attribute **attrs;

	
	void *demux_bounce;

	
	struct list_head buffer_list;

	
	struct kref ref;
};


int iio_update_buffers(struct iio_dev *indio_dev,
		       struct iio_buffer *insert_buffer,
		       struct iio_buffer *remove_buffer);


void iio_buffer_init(struct iio_buffer *buffer);

struct iio_buffer *iio_buffer_get(struct iio_buffer *buffer);
void iio_buffer_put(struct iio_buffer *buffer);

#else 

static inline void iio_buffer_get(struct iio_buffer *buffer) {}
static inline void iio_buffer_put(struct iio_buffer *buffer) {}

#endif 
#endif 
