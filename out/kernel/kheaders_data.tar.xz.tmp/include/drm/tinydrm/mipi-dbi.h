

#ifndef __LINUX_MIPI_DBI_H
#define __LINUX_MIPI_DBI_H

#include <drm/tinydrm/tinydrm.h>

struct spi_device;
struct gpio_desc;
struct regulator;


struct mipi_dbi {
	struct tinydrm_device tinydrm;
	struct spi_device *spi;
	bool enabled;
	struct mutex cmdlock;
	int (*command)(struct mipi_dbi *mipi, u8 cmd, u8 *param, size_t num);
	const u8 *read_commands;
	struct gpio_desc *dc;
	u16 *tx_buf;
	void *tx_buf9;
	size_t tx_buf9_len;
	bool swap_bytes;
	struct gpio_desc *reset;
	unsigned int rotation;
	struct backlight_device *backlight;
	struct regulator *regulator;
};

static inline struct mipi_dbi *
mipi_dbi_from_tinydrm(struct tinydrm_device *tdev)
{
	return container_of(tdev, struct mipi_dbi, tinydrm);
}

int mipi_dbi_spi_init(struct spi_device *spi, struct mipi_dbi *mipi,
		      struct gpio_desc *dc);
int mipi_dbi_init(struct device *dev, struct mipi_dbi *mipi,
		  const struct drm_simple_display_pipe_funcs *pipe_funcs,
		  struct drm_driver *driver,
		  const struct drm_display_mode *mode, unsigned int rotation);
void mipi_dbi_pipe_enable(struct drm_simple_display_pipe *pipe,
			  struct drm_crtc_state *crtc_state);
void mipi_dbi_pipe_disable(struct drm_simple_display_pipe *pipe);
void mipi_dbi_hw_reset(struct mipi_dbi *mipi);
bool mipi_dbi_display_is_on(struct mipi_dbi *mipi);

int mipi_dbi_command_read(struct mipi_dbi *mipi, u8 cmd, u8 *val);
int mipi_dbi_command_buf(struct mipi_dbi *mipi, u8 cmd, u8 *data, size_t len);


#define mipi_dbi_command(mipi, cmd, seq...) \
({ \
	u8 d[] = { seq }; \
	mipi_dbi_command_buf(mipi, cmd, d, ARRAY_SIZE(d)); \
})

#ifdef CONFIG_DEBUG_FS
int mipi_dbi_debugfs_init(struct drm_minor *minor);
#else
#define mipi_dbi_debugfs_init		NULL
#endif

#endif 
