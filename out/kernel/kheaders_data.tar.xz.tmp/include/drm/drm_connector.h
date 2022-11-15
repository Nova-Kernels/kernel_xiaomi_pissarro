

#ifndef __DRM_CONNECTOR_H__
#define __DRM_CONNECTOR_H__

#include <linux/list.h>
#include <linux/ctype.h>
#include <linux/hdmi.h>
#include <drm/drm_mode_object.h>

#include <uapi/drm/drm_mode.h>

struct drm_connector_helper_funcs;
struct drm_modeset_acquire_ctx;
struct drm_device;
struct drm_crtc;
struct drm_encoder;
struct drm_property;
struct drm_property_blob;
struct drm_printer;
struct edid;

enum drm_connector_force {
	DRM_FORCE_UNSPECIFIED,
	DRM_FORCE_OFF,
	DRM_FORCE_ON,         
	DRM_FORCE_ON_DIGITAL, 
};


enum drm_connector_status {
	
	connector_status_connected = 1,
	
	connector_status_disconnected = 2,
	
	connector_status_unknown = 3,
};

enum subpixel_order {
	SubPixelUnknown = 0,
	SubPixelHorizontalRGB,
	SubPixelHorizontalBGR,
	SubPixelVerticalRGB,
	SubPixelVerticalBGR,
	SubPixelNone,

};


struct drm_scrambling {
	
	bool supported;
	
	bool low_rates;
};


struct drm_scdc {
	
	bool supported;
	
	bool read_request;
	
	struct drm_scrambling scrambling;
};



struct drm_hdmi_info {
	
	struct drm_scdc scdc;

	
	unsigned long y420_vdb_modes[BITS_TO_LONGS(128)];

	
	unsigned long y420_cmdb_modes[BITS_TO_LONGS(128)];

	
	u64 y420_cmdb_map;

	
	u8 y420_dc_modes;
};


enum drm_link_status {
	DRM_LINK_STATUS_GOOD = DRM_MODE_LINK_STATUS_GOOD,
	DRM_LINK_STATUS_BAD = DRM_MODE_LINK_STATUS_BAD,
};


struct drm_display_info {
	
	char name[DRM_DISPLAY_INFO_LEN];

	
        unsigned int width_mm;
	
	unsigned int height_mm;

	
	unsigned int pixel_clock;
	
	unsigned int bpc;

	
	enum subpixel_order subpixel_order;

#define DRM_COLOR_FORMAT_RGB444		(1<<0)
#define DRM_COLOR_FORMAT_YCRCB444	(1<<1)
#define DRM_COLOR_FORMAT_YCRCB422	(1<<2)
#define DRM_COLOR_FORMAT_YCRCB420	(1<<3)

	
	u32 color_formats;

	
	const u32 *bus_formats;
	
	unsigned int num_bus_formats;

#define DRM_BUS_FLAG_DE_LOW		(1<<0)
#define DRM_BUS_FLAG_DE_HIGH		(1<<1)

#define DRM_BUS_FLAG_PIXDATA_POSEDGE	(1<<2)

#define DRM_BUS_FLAG_PIXDATA_NEGEDGE	(1<<3)

#define DRM_BUS_FLAG_DATA_MSB_TO_LSB	(1<<4)

#define DRM_BUS_FLAG_DATA_LSB_TO_MSB	(1<<5)

	
	u32 bus_flags;

	
	int max_tmds_clock;

	
	bool dvi_dual;

	
	u8 edid_hdmi_dc_modes;

	
	u8 cea_rev;

	
	struct drm_hdmi_info hdmi;
};

int drm_display_info_set_bus_formats(struct drm_display_info *info,
				     const u32 *formats,
				     unsigned int num_formats);


struct drm_tv_connector_state {
	enum drm_mode_subconnector subconnector;
	struct {
		unsigned int left;
		unsigned int right;
		unsigned int top;
		unsigned int bottom;
	} margins;
	unsigned int mode;
	unsigned int brightness;
	unsigned int contrast;
	unsigned int flicker_reduction;
	unsigned int overscan;
	unsigned int saturation;
	unsigned int hue;
};


struct drm_connector_state {
	struct drm_connector *connector;

	
	struct drm_crtc *crtc;

	struct drm_encoder *best_encoder;

	
	enum drm_link_status link_status;

	struct drm_atomic_state *state;

	struct drm_tv_connector_state tv;

	
	enum hdmi_picture_aspect picture_aspect_ratio;

	
	unsigned int scaling_mode;

	
	struct drm_writeback_job *writeback_job;
};


struct drm_connector_funcs {
	
	int (*dpms)(struct drm_connector *connector, int mode);

	
	void (*reset)(struct drm_connector *connector);

	
	enum drm_connector_status (*detect)(struct drm_connector *connector,
					    bool force);

	
	void (*force)(struct drm_connector *connector);

	
	int (*fill_modes)(struct drm_connector *connector, uint32_t max_width, uint32_t max_height);

	
	int (*set_property)(struct drm_connector *connector, struct drm_property *property,
			     uint64_t val);

	
	int (*late_register)(struct drm_connector *connector);

	
	void (*early_unregister)(struct drm_connector *connector);

	
	void (*destroy)(struct drm_connector *connector);

	
	struct drm_connector_state *(*atomic_duplicate_state)(struct drm_connector *connector);

	
	void (*atomic_destroy_state)(struct drm_connector *connector,
				     struct drm_connector_state *state);

	
	int (*atomic_set_property)(struct drm_connector *connector,
				   struct drm_connector_state *state,
				   struct drm_property *property,
				   uint64_t val);

	
	int (*atomic_get_property)(struct drm_connector *connector,
				   const struct drm_connector_state *state,
				   struct drm_property *property,
				   uint64_t *val);

	
	void (*atomic_print_state)(struct drm_printer *p,
				   const struct drm_connector_state *state);
};


struct drm_cmdline_mode {
	bool specified;
	bool refresh_specified;
	bool bpp_specified;
	int xres, yres;
	int bpp;
	int refresh;
	bool rb;
	bool interlace;
	bool cvt;
	bool margins;
	enum drm_connector_force force;
};


struct drm_connector {
	struct drm_device *dev;
	struct device *kdev;
	struct device_attribute *attr;
	struct list_head head;

	struct drm_mode_object base;

	char *name;

	
	struct mutex mutex;

	
	unsigned index;

	int connector_type;
	int connector_type_id;
	bool interlace_allowed;
	bool doublescan_allowed;
	bool stereo_allowed;

	
	bool ycbcr_420_allowed;

	
	bool registered;

	
	struct list_head modes;

	
	enum drm_connector_status status;

	
	struct list_head probed_modes;

	
	struct drm_display_info display_info;
	const struct drm_connector_funcs *funcs;

	struct drm_property_blob *edid_blob_ptr;
	struct drm_object_properties properties;

	struct drm_property *scaling_mode_property;

	
	struct drm_property_blob *path_blob_ptr;

	
	struct drm_property_blob *tile_blob_ptr;



#define DRM_CONNECTOR_POLL_HPD (1 << 0)

#define DRM_CONNECTOR_POLL_CONNECT (1 << 1)


#define DRM_CONNECTOR_POLL_DISCONNECT (1 << 2)

	
	uint8_t polled;

	
	int dpms;

	const struct drm_connector_helper_funcs *helper_private;

	
	struct drm_cmdline_mode cmdline_mode;
	enum drm_connector_force force;
	bool override_edid;

#define DRM_CONNECTOR_MAX_ENCODER 3
	uint32_t encoder_ids[DRM_CONNECTOR_MAX_ENCODER];
	struct drm_encoder *encoder; 

#define MAX_ELD_BYTES	128
	
	uint8_t eld[MAX_ELD_BYTES];
	bool latency_present[2];
	int video_latency[2];	
	int audio_latency[2];
	int null_edid_counter; 
	unsigned bad_edid_counter;

	
	bool edid_corrupt;

	struct dentry *debugfs_entry;

	
	struct drm_connector_state *state;

	
	bool has_tile;
	struct drm_tile_group *tile_group;
	bool tile_is_single_monitor;

	uint8_t num_h_tile, num_v_tile;
	uint8_t tile_h_loc, tile_v_loc;
	uint16_t tile_h_size, tile_v_size;

	uint brightness_clone;
	uint dc_status;
	uint8_t panel_id;
	int panel_event;
};

#define obj_to_connector(x) container_of(x, struct drm_connector, base)

int drm_connector_init(struct drm_device *dev,
		       struct drm_connector *connector,
		       const struct drm_connector_funcs *funcs,
		       int connector_type);
void drm_connector_attach_edid_property(struct drm_connector *connector);
int drm_connector_register(struct drm_connector *connector);
void drm_connector_unregister(struct drm_connector *connector);
int drm_mode_connector_attach_encoder(struct drm_connector *connector,
				      struct drm_encoder *encoder);

void drm_connector_cleanup(struct drm_connector *connector);
static inline unsigned drm_connector_index(struct drm_connector *connector)
{
	return connector->index;
}


static inline struct drm_connector *drm_connector_lookup(struct drm_device *dev,
		struct drm_file *file_priv,
		uint32_t id)
{
	struct drm_mode_object *mo;
	mo = drm_mode_object_find(dev,
			file_priv, id, DRM_MODE_OBJECT_CONNECTOR);
	return mo ? obj_to_connector(mo) : NULL;
}


static inline void drm_connector_get(struct drm_connector *connector)
{
	drm_mode_object_get(&connector->base);
}


static inline void drm_connector_put(struct drm_connector *connector)
{
	drm_mode_object_put(&connector->base);
}


static inline void drm_connector_reference(struct drm_connector *connector)
{
	drm_connector_get(connector);
}


static inline void drm_connector_unreference(struct drm_connector *connector)
{
	drm_connector_put(connector);
}

const char *drm_get_connector_status_name(enum drm_connector_status status);
const char *drm_get_subpixel_order_name(enum subpixel_order order);
const char *drm_get_dpms_name(int val);
const char *drm_get_dvi_i_subconnector_name(int val);
const char *drm_get_dvi_i_select_name(int val);
const char *drm_get_tv_subconnector_name(int val);
const char *drm_get_tv_select_name(int val);

int drm_mode_create_dvi_i_properties(struct drm_device *dev);
int drm_mode_create_tv_properties(struct drm_device *dev,
				  unsigned int num_modes,
				  const char * const modes[]);
int drm_mode_create_scaling_mode_property(struct drm_device *dev);
int drm_connector_attach_scaling_mode_property(struct drm_connector *connector,
					       u32 scaling_mode_mask);
int drm_mode_create_aspect_ratio_property(struct drm_device *dev);
int drm_mode_create_suggested_offset_properties(struct drm_device *dev);

int drm_mode_connector_set_path_property(struct drm_connector *connector,
					 const char *path);
int drm_mode_connector_set_tile_property(struct drm_connector *connector);
int drm_mode_connector_update_edid_property(struct drm_connector *connector,
					    const struct edid *edid);
void drm_mode_connector_set_link_status_property(struct drm_connector *connector,
						 uint64_t link_status);


struct drm_tile_group {
	struct kref refcount;
	struct drm_device *dev;
	int id;
	u8 group_data[8];
};

struct drm_tile_group *drm_mode_create_tile_group(struct drm_device *dev,
						  char topology[8]);
struct drm_tile_group *drm_mode_get_tile_group(struct drm_device *dev,
					       char topology[8]);
void drm_mode_put_tile_group(struct drm_device *dev,
			     struct drm_tile_group *tg);


struct drm_connector_list_iter {

	struct drm_device *dev;
	struct drm_connector *conn;
};

void drm_connector_list_iter_begin(struct drm_device *dev,
				   struct drm_connector_list_iter *iter);
struct drm_connector *
drm_connector_list_iter_next(struct drm_connector_list_iter *iter);
void drm_connector_list_iter_end(struct drm_connector_list_iter *iter);


#define drm_for_each_connector_iter(connector, iter) \
	while ((connector = drm_connector_list_iter_next(iter)))

#endif
