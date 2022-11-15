

#ifndef DRM_PRINT_H_
#define DRM_PRINT_H_

#include <linux/compiler.h>
#include <linux/printk.h>
#include <linux/seq_file.h>
#include <linux/device.h>




struct drm_printer {
	
	void (*printfn)(struct drm_printer *p, struct va_format *vaf);
	void *arg;
	const char *prefix;
};

void __drm_printfn_seq_file(struct drm_printer *p, struct va_format *vaf);
void __drm_printfn_info(struct drm_printer *p, struct va_format *vaf);
void __drm_printfn_debug(struct drm_printer *p, struct va_format *vaf);

__printf(2, 3)
void drm_printf(struct drm_printer *p, const char *f, ...);



static inline struct drm_printer drm_seq_file_printer(struct seq_file *f)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_seq_file,
		.arg = f,
	};
	return p;
}


static inline struct drm_printer drm_info_printer(struct device *dev)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_info,
		.arg = dev,
	};
	return p;
}


static inline struct drm_printer drm_debug_printer(const char *prefix)
{
	struct drm_printer p = {
		.printfn = __drm_printfn_debug,
		.prefix = prefix
	};
	return p;
}
#endif 
