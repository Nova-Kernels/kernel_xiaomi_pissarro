
#ifndef _BLK_BSG_
#define _BLK_BSG_

#include <linux/blkdev.h>
#include <scsi/scsi_request.h>

struct request;
struct device;
struct scatterlist;
struct request_queue;

struct bsg_buffer {
	unsigned int payload_len;
	int sg_cnt;
	struct scatterlist *sg_list;
};

struct bsg_job {
	struct scsi_request sreq;
	struct device *dev;
	struct request *req;

	struct kref kref;

	
	void *request;
	void *reply;

	unsigned int request_len;
	unsigned int reply_len;
	

	
	struct bsg_buffer request_payload;
	struct bsg_buffer reply_payload;

	void *dd_data;		
};

void bsg_job_done(struct bsg_job *job, int result,
		  unsigned int reply_payload_rcv_len);
struct request_queue *bsg_setup_queue(struct device *dev, const char *name,
		bsg_job_fn *job_fn, int dd_job_size,
		void (*release)(struct device *));
void bsg_job_put(struct bsg_job *job);
int __must_check bsg_job_get(struct bsg_job *job);

#endif
