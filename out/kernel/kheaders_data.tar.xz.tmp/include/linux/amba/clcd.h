
#include <linux/fb.h>
#include <linux/amba/clcd-regs.h>

enum {
	
	CLCD_CAP_RGB444		= (1 << 0),
	CLCD_CAP_RGB5551	= (1 << 1),
	CLCD_CAP_RGB565		= (1 << 2),
	CLCD_CAP_RGB888		= (1 << 3),
	CLCD_CAP_BGR444		= (1 << 4),
	CLCD_CAP_BGR5551	= (1 << 5),
	CLCD_CAP_BGR565		= (1 << 6),
	CLCD_CAP_BGR888		= (1 << 7),

	
	CLCD_CAP_444		= CLCD_CAP_RGB444 | CLCD_CAP_BGR444,
	CLCD_CAP_5551		= CLCD_CAP_RGB5551 | CLCD_CAP_BGR5551,
	CLCD_CAP_565		= CLCD_CAP_RGB565 | CLCD_CAP_BGR565,
	CLCD_CAP_888		= CLCD_CAP_RGB888 | CLCD_CAP_BGR888,

	
	CLCD_CAP_RGB		= CLCD_CAP_RGB444 | CLCD_CAP_RGB5551 |
				  CLCD_CAP_RGB565 | CLCD_CAP_RGB888,
	CLCD_CAP_BGR		= CLCD_CAP_BGR444 | CLCD_CAP_BGR5551 |
				  CLCD_CAP_BGR565 | CLCD_CAP_BGR888,

	CLCD_CAP_ALL		= CLCD_CAP_BGR | CLCD_CAP_RGB,
};

struct backlight_device;

struct clcd_panel {
	struct fb_videomode	mode;
	signed short		width;	
	signed short		height;	
	u32			tim2;
	u32			tim3;
	u32			cntl;
	u32			caps;
	unsigned int		bpp:8,
				fixedtimings:1,
				grayscale:1;
	unsigned int		connector;
	struct backlight_device	*backlight;
	
	bool			bgr_connection;
};

struct clcd_regs {
	u32			tim0;
	u32			tim1;
	u32			tim2;
	u32			tim3;
	u32			cntl;
	unsigned long		pixclock;
};

struct clcd_fb;


struct clcd_board {
	const char *name;

	
	u32	caps;

	
	int	(*check)(struct clcd_fb *fb, struct fb_var_screeninfo *var);

	
	void	(*decode)(struct clcd_fb *fb, struct clcd_regs *regs);

	
	void	(*disable)(struct clcd_fb *);

	
	void	(*enable)(struct clcd_fb *);

	
	int	(*setup)(struct clcd_fb *);

	
	int	(*mmap)(struct clcd_fb *, struct vm_area_struct *);

	
	void	(*remove)(struct clcd_fb *);
};

struct amba_device;
struct clk;


struct clcd_vendor_data {
	bool	clock_timregs;
	bool	packed_24_bit_pixels;
	bool	st_bitmux_control;
	int	(*init_board)(struct amba_device *adev,
			      struct clcd_board *board);
	int	(*init_panel)(struct clcd_fb *fb,
			      struct device_node *panel);
};


struct clcd_fb {
	struct fb_info		fb;
	struct amba_device	*dev;
	struct clk		*clk;
	struct clcd_vendor_data	*vendor;
	struct clcd_panel	*panel;
	struct clcd_board	*board;
	void			*board_data;
	void __iomem		*regs;
	u16			off_ienb;
	u16			off_cntl;
	u32			clcd_cntl;
	u32			cmap[16];
	bool			clk_enabled;
};

static inline void clcdfb_decode(struct clcd_fb *fb, struct clcd_regs *regs)
{
	struct fb_var_screeninfo *var = &fb->fb.var;
	u32 val, cpl;

	
	val = ((var->xres / 16) - 1) << 2;
	val |= (var->hsync_len - 1) << 8;
	val |= (var->right_margin - 1) << 16;
	val |= (var->left_margin - 1) << 24;
	regs->tim0 = val;

	val = var->yres;
	if (fb->panel->cntl & CNTL_LCDDUAL)
		val /= 2;
	val -= 1;
	val |= (var->vsync_len - 1) << 10;
	val |= var->lower_margin << 16;
	val |= var->upper_margin << 24;
	regs->tim1 = val;

	val = fb->panel->tim2;
	val |= var->sync & FB_SYNC_HOR_HIGH_ACT  ? 0 : TIM2_IHS;
	val |= var->sync & FB_SYNC_VERT_HIGH_ACT ? 0 : TIM2_IVS;

	cpl = var->xres_virtual;
	if (fb->panel->cntl & CNTL_LCDTFT)	  
		;
	else if (!var->grayscale)		  
		cpl = cpl * 8 / 3;
	else if (fb->panel->cntl & CNTL_LCDMONO8) 
		cpl /= 8;
	else					  
		cpl /= 4;

	regs->tim2 = val | ((cpl - 1) << 16);

	regs->tim3 = fb->panel->tim3;

	val = fb->panel->cntl;
	if (var->grayscale)
		val |= CNTL_LCDBW;

	if (fb->panel->caps && fb->board->caps && var->bits_per_pixel >= 16) {
		
		if (var->red.offset == 0)
			val &= ~CNTL_BGR;
		else
			val |= CNTL_BGR;
		if (fb->panel->bgr_connection)
			val ^= CNTL_BGR;
	}

	switch (var->bits_per_pixel) {
	case 1:
		val |= CNTL_LCDBPP1;
		break;
	case 2:
		val |= CNTL_LCDBPP2;
		break;
	case 4:
		val |= CNTL_LCDBPP4;
		break;
	case 8:
		val |= CNTL_LCDBPP8;
		break;
	case 16:
		
		if (amba_part(fb->dev) == 0x110 ||
		    var->green.length == 5)
			val |= CNTL_LCDBPP16;
		else if (var->green.length == 6)
			val |= CNTL_LCDBPP16_565;
		else
			val |= CNTL_LCDBPP16_444;
		break;
	case 24:
		
		val |= CNTL_ST_LCDBPP24_PACKED;
		break;
	case 32:
		val |= CNTL_LCDBPP24;
		break;
	}

	regs->cntl = val;
	regs->pixclock = var->pixclock;
}

static inline int clcdfb_check(struct clcd_fb *fb, struct fb_var_screeninfo *var)
{
	var->xres_virtual = var->xres = (var->xres + 15) & ~15;
	var->yres_virtual = var->yres = (var->yres + 1) & ~1;

#define CHECK(e,l,h) (var->e < l || var->e > h)
	if (CHECK(right_margin, (5+1), 256) ||	
	    CHECK(left_margin, (5+1), 256) ||	
	    CHECK(hsync_len, (5+1), 256) ||
	    var->xres > 4096 ||
	    var->lower_margin > 255 ||		
	    var->upper_margin > 255 ||		
	    var->vsync_len > 32 ||
	    var->yres > 1024)
		return -EINVAL;
#undef CHECK

	
	

	
	if (var->grayscale != fb->fb.var.grayscale ||
	    (var->vmode & FB_VMODE_MASK) != FB_VMODE_NONINTERLACED)
		return -EINVAL;

#define CHECK(e) (var->e != fb->fb.var.e)
	if (fb->panel->fixedtimings &&
	    (CHECK(xres)		||
	     CHECK(yres)		||
	     CHECK(bits_per_pixel)	||
	     CHECK(pixclock)		||
	     CHECK(left_margin)		||
	     CHECK(right_margin)	||
	     CHECK(upper_margin)	||
	     CHECK(lower_margin)	||
	     CHECK(hsync_len)		||
	     CHECK(vsync_len)		||
	     CHECK(sync)))
		return -EINVAL;
#undef CHECK

	var->nonstd = 0;
	var->accel_flags = 0;

	return 0;
}
