/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __DRM_WRITEBACK_H__
#define __DRM_WRITEBACK_H__
#include <drm/drm_connector.h>
#include <drm/drm_encoder.h>
#include <linux/workqueue.h>

struct drm_writeback_connector {
	struct drm_connector base;

	
	struct drm_encoder encoder;

	
	struct drm_property_blob *pixel_formats_blob_ptr;

	
	spinlock_t job_lock;

	
	struct list_head job_queue;

	
	unsigned int fence_context;
	
	spinlock_t fence_lock;
	
	unsigned long fence_seqno;
	
	char timeline_name[32];
};

struct drm_writeback_job {
	
	struct work_struct cleanup_work;

	
	struct list_head list_entry;

	
	struct drm_framebuffer *fb;

	
	struct dma_fence *out_fence;
};

int drm_writeback_connector_init(struct drm_device *dev,
				 struct drm_writeback_connector *wb_connector,
				 const struct drm_connector_funcs *con_funcs,
				 const struct drm_encoder_helper_funcs *enc_helper_funcs,
				 const u32 *formats, int n_formats);

void drm_writeback_queue_job(struct drm_writeback_connector *wb_connector,
			     struct drm_writeback_job *job);

void drm_writeback_cleanup_job(struct drm_writeback_job *job);

void
drm_writeback_signal_completion(struct drm_writeback_connector *wb_connector,
				int status);

struct dma_fence *
drm_writeback_get_out_fence(struct drm_writeback_connector *wb_connector);
#endif
