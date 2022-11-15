
#ifndef _V4L2_FWNODE_H
#define _V4L2_FWNODE_H

#include <linux/errno.h>
#include <linux/fwnode.h>
#include <linux/list.h>
#include <linux/types.h>

#include <media/v4l2-mediabus.h>

struct fwnode_handle;

#define V4L2_FWNODE_CSI2_MAX_DATA_LANES	4


struct v4l2_fwnode_bus_mipi_csi2 {
	unsigned int flags;
	unsigned char data_lanes[V4L2_FWNODE_CSI2_MAX_DATA_LANES];
	unsigned char clock_lane;
	unsigned short num_data_lanes;
	bool lane_polarities[1 + V4L2_FWNODE_CSI2_MAX_DATA_LANES];
};


struct v4l2_fwnode_bus_parallel {
	unsigned int flags;
	unsigned char bus_width;
	unsigned char data_shift;
};


struct v4l2_fwnode_bus_mipi_csi1 {
	bool clock_inv;
	bool strobe;
	bool lane_polarity[2];
	unsigned char data_lane;
	unsigned char clock_lane;
};


struct v4l2_fwnode_endpoint {
	struct fwnode_endpoint base;
	
	enum v4l2_mbus_type bus_type;
	union {
		struct v4l2_fwnode_bus_parallel parallel;
		struct v4l2_fwnode_bus_mipi_csi1 mipi_csi1;
		struct v4l2_fwnode_bus_mipi_csi2 mipi_csi2;
	} bus;
	u64 *link_frequencies;
	unsigned int nr_of_link_frequencies;
};


struct v4l2_fwnode_link {
	struct fwnode_handle *local_node;
	unsigned int local_port;
	struct fwnode_handle *remote_node;
	unsigned int remote_port;
};

int v4l2_fwnode_endpoint_parse(struct fwnode_handle *fwnode,
			       struct v4l2_fwnode_endpoint *vep);
struct v4l2_fwnode_endpoint *v4l2_fwnode_endpoint_alloc_parse(
	struct fwnode_handle *fwnode);
void v4l2_fwnode_endpoint_free(struct v4l2_fwnode_endpoint *vep);
int v4l2_fwnode_parse_link(struct fwnode_handle *fwnode,
			   struct v4l2_fwnode_link *link);
void v4l2_fwnode_put_link(struct v4l2_fwnode_link *link);

#endif 
