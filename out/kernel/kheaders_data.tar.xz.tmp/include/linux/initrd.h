/* SPDX-License-Identifier: GPL-2.0 */

#define INITRD_MINOR 250 


extern int rd_doload;


extern int rd_prompt;


extern int rd_image_start;


extern unsigned long rd_size;


extern int initrd_below_start_ok;


extern unsigned long initrd_start, initrd_end;
extern void free_initrd_mem(unsigned long, unsigned long);

extern unsigned int real_root_dev;
