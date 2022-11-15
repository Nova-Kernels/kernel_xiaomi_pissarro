

struct charlcd {
	const struct charlcd_ops *ops;
	const unsigned char *char_conv;	

	int ifwidth;			
	int height;
	int width;
	int bwidth;			
	int hwidth;			

	void *drvdata;			
};

struct charlcd_ops {
	
	void (*write_cmd)(struct charlcd *lcd, int cmd);
	void (*write_data)(struct charlcd *lcd, int data);

	
	void (*write_cmd_raw4)(struct charlcd *lcd, int cmd);	
	void (*clear_fast)(struct charlcd *lcd);
	void (*backlight)(struct charlcd *lcd, int on);
};

struct charlcd *charlcd_alloc(unsigned int drvdata_size);

int charlcd_register(struct charlcd *lcd);
int charlcd_unregister(struct charlcd *lcd);

void charlcd_poke(struct charlcd *lcd);
