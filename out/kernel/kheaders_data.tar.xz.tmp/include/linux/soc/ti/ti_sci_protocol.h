

#ifndef __TISCI_PROTOCOL_H
#define __TISCI_PROTOCOL_H


struct ti_sci_version_info {
	u8 abi_major;
	u8 abi_minor;
	u16 firmware_revision;
	char firmware_description[32];
};

struct ti_sci_handle;


struct ti_sci_core_ops {
	int (*reboot_device)(const struct ti_sci_handle *handle);
};


struct ti_sci_dev_ops {
	int (*get_device)(const struct ti_sci_handle *handle, u32 id);
	int (*idle_device)(const struct ti_sci_handle *handle, u32 id);
	int (*put_device)(const struct ti_sci_handle *handle, u32 id);
	int (*is_valid)(const struct ti_sci_handle *handle, u32 id);
	int (*get_context_loss_count)(const struct ti_sci_handle *handle,
				      u32 id, u32 *count);
	int (*is_idle)(const struct ti_sci_handle *handle, u32 id,
		       bool *requested_state);
	int (*is_stop)(const struct ti_sci_handle *handle, u32 id,
		       bool *req_state, bool *current_state);
	int (*is_on)(const struct ti_sci_handle *handle, u32 id,
		     bool *req_state, bool *current_state);
	int (*is_transitioning)(const struct ti_sci_handle *handle, u32 id,
				bool *current_state);
	int (*set_device_resets)(const struct ti_sci_handle *handle, u32 id,
				 u32 reset_state);
	int (*get_device_resets)(const struct ti_sci_handle *handle, u32 id,
				 u32 *reset_state);
};


struct ti_sci_clk_ops {
	int (*get_clock)(const struct ti_sci_handle *handle, u32 did, u8 cid,
			 bool needs_ssc, bool can_change_freq,
			 bool enable_input_term);
	int (*idle_clock)(const struct ti_sci_handle *handle, u32 did, u8 cid);
	int (*put_clock)(const struct ti_sci_handle *handle, u32 did, u8 cid);
	int (*is_auto)(const struct ti_sci_handle *handle, u32 did, u8 cid,
		       bool *req_state);
	int (*is_on)(const struct ti_sci_handle *handle, u32 did, u8 cid,
		     bool *req_state, bool *current_state);
	int (*is_off)(const struct ti_sci_handle *handle, u32 did, u8 cid,
		      bool *req_state, bool *current_state);
	int (*set_parent)(const struct ti_sci_handle *handle, u32 did, u8 cid,
			  u8 parent_id);
	int (*get_parent)(const struct ti_sci_handle *handle, u32 did, u8 cid,
			  u8 *parent_id);
	int (*get_num_parents)(const struct ti_sci_handle *handle, u32 did,
			       u8 cid, u8 *num_parents);
	int (*get_best_match_freq)(const struct ti_sci_handle *handle, u32 did,
				   u8 cid, u64 min_freq, u64 target_freq,
				   u64 max_freq, u64 *match_freq);
	int (*set_freq)(const struct ti_sci_handle *handle, u32 did, u8 cid,
			u64 min_freq, u64 target_freq, u64 max_freq);
	int (*get_freq)(const struct ti_sci_handle *handle, u32 did, u8 cid,
			u64 *current_freq);
};


struct ti_sci_ops {
	struct ti_sci_core_ops core_ops;
	struct ti_sci_dev_ops dev_ops;
	struct ti_sci_clk_ops clk_ops;
};


struct ti_sci_handle {
	struct ti_sci_version_info version;
	struct ti_sci_ops ops;
};

#if IS_ENABLED(CONFIG_TI_SCI_PROTOCOL)
const struct ti_sci_handle *ti_sci_get_handle(struct device *dev);
int ti_sci_put_handle(const struct ti_sci_handle *handle);
const struct ti_sci_handle *devm_ti_sci_get_handle(struct device *dev);

#else	

static inline const struct ti_sci_handle *ti_sci_get_handle(struct device *dev)
{
	return ERR_PTR(-EINVAL);
}

static inline int ti_sci_put_handle(const struct ti_sci_handle *handle)
{
	return -EINVAL;
}

static inline
const struct ti_sci_handle *devm_ti_sci_get_handle(struct device *dev)
{
	return ERR_PTR(-EINVAL);
}

#endif	

#endif	
