

#ifndef DRM_SCDC_HELPER_H
#define DRM_SCDC_HELPER_H

#include <linux/i2c.h>
#include <linux/types.h>

#define SCDC_SINK_VERSION 0x01

#define SCDC_SOURCE_VERSION 0x02

#define SCDC_UPDATE_0 0x10
#define  SCDC_READ_REQUEST_TEST (1 << 2)
#define  SCDC_CED_UPDATE (1 << 1)
#define  SCDC_STATUS_UPDATE (1 << 0)

#define SCDC_UPDATE_1 0x11

#define SCDC_TMDS_CONFIG 0x20
#define  SCDC_TMDS_BIT_CLOCK_RATIO_BY_40 (1 << 1)
#define  SCDC_TMDS_BIT_CLOCK_RATIO_BY_10 (0 << 1)
#define  SCDC_SCRAMBLING_ENABLE (1 << 0)

#define SCDC_SCRAMBLER_STATUS 0x21
#define  SCDC_SCRAMBLING_STATUS (1 << 0)

#define SCDC_CONFIG_0 0x30
#define  SCDC_READ_REQUEST_ENABLE (1 << 0)

#define SCDC_STATUS_FLAGS_0 0x40
#define  SCDC_CH2_LOCK (1 < 3)
#define  SCDC_CH1_LOCK (1 < 2)
#define  SCDC_CH0_LOCK (1 < 1)
#define  SCDC_CH_LOCK_MASK (SCDC_CH2_LOCK | SCDC_CH1_LOCK | SCDC_CH0_LOCK)
#define  SCDC_CLOCK_DETECT (1 << 0)

#define SCDC_STATUS_FLAGS_1 0x41

#define SCDC_ERR_DET_0_L 0x50
#define SCDC_ERR_DET_0_H 0x51
#define SCDC_ERR_DET_1_L 0x52
#define SCDC_ERR_DET_1_H 0x53
#define SCDC_ERR_DET_2_L 0x54
#define SCDC_ERR_DET_2_H 0x55
#define  SCDC_CHANNEL_VALID (1 << 7)

#define SCDC_ERR_DET_CHECKSUM 0x56

#define SCDC_TEST_CONFIG_0 0xc0
#define  SCDC_TEST_READ_REQUEST (1 << 7)
#define  SCDC_TEST_READ_REQUEST_DELAY(x) ((x) & 0x7f)

#define SCDC_MANUFACTURER_IEEE_OUI 0xd0
#define SCDC_MANUFACTURER_IEEE_OUI_SIZE 3

#define SCDC_DEVICE_ID 0xd3
#define SCDC_DEVICE_ID_SIZE 8

#define SCDC_DEVICE_HARDWARE_REVISION 0xdb
#define  SCDC_GET_DEVICE_HARDWARE_REVISION_MAJOR(x) (((x) >> 4) & 0xf)
#define  SCDC_GET_DEVICE_HARDWARE_REVISION_MINOR(x) (((x) >> 0) & 0xf)

#define SCDC_DEVICE_SOFTWARE_MAJOR_REVISION 0xdc
#define SCDC_DEVICE_SOFTWARE_MINOR_REVISION 0xdd

#define SCDC_MANUFACTURER_SPECIFIC 0xde
#define SCDC_MANUFACTURER_SPECIFIC_SIZE 34

ssize_t drm_scdc_read(struct i2c_adapter *adapter, u8 offset, void *buffer,
		      size_t size);
ssize_t drm_scdc_write(struct i2c_adapter *adapter, u8 offset,
		       const void *buffer, size_t size);


static inline int drm_scdc_readb(struct i2c_adapter *adapter, u8 offset,
				 u8 *value)
{
	return drm_scdc_read(adapter, offset, value, sizeof(*value));
}


static inline int drm_scdc_writeb(struct i2c_adapter *adapter, u8 offset,
				  u8 value)
{
	return drm_scdc_write(adapter, offset, &value, sizeof(value));
}

bool drm_scdc_get_scrambling_status(struct i2c_adapter *adapter);

bool drm_scdc_set_scrambling(struct i2c_adapter *adapter, bool enable);
bool drm_scdc_set_high_tmds_clock_ratio(struct i2c_adapter *adapter, bool set);
#endif
