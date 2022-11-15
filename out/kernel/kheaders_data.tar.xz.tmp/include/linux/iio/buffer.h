

#ifndef _IIO_BUFFER_GENERIC_H_
#define _IIO_BUFFER_GENERIC_H_
#include <linux/sysfs.h>
#include <linux/iio/iio.h>

struct iio_buffer;

void iio_buffer_set_attrs(struct iio_buffer *buffer,
			 const struct attribute **attrs);

int iio_push_to_buffers(struct iio_dev *indio_dev, const void *data);


static inline int iio_push_to_buffers_with_timestamp(struct iio_dev *indio_dev,
	void *data, int64_t timestamp)
{
	if (indio_dev->scan_timestamp) {
		size_t ts_offset = indio_dev->scan_bytes / sizeof(int64_t) - 1;
		((int64_t *)data)[ts_offset] = timestamp;
	}

	return iio_push_to_buffers(indio_dev, data);
}

bool iio_validate_scan_mask_onehot(struct iio_dev *indio_dev,
				   const unsigned long *mask);

void iio_device_attach_buffer(struct iio_dev *indio_dev,
			      struct iio_buffer *buffer);

#endif 
