/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */


#ifndef _UAPI_LINUX_ASPEED_LPC_CTRL_H
#define _UAPI_LINUX_ASPEED_LPC_CTRL_H

#include <linux/ioctl.h>
#include <linux/types.h>


#define ASPEED_LPC_CTRL_WINDOW_FLASH	1
#define ASPEED_LPC_CTRL_WINDOW_MEMORY	2



struct aspeed_lpc_ctrl_mapping {
	__u8	window_type;
	__u8	window_id;
	__u16	flags;
	__u32	addr;
	__u32	offset;
	__u32	size;
};

#define __ASPEED_LPC_CTRL_IOCTL_MAGIC	0xb2

#define ASPEED_LPC_CTRL_IOCTL_GET_SIZE	_IOWR(__ASPEED_LPC_CTRL_IOCTL_MAGIC, \
		0x00, struct aspeed_lpc_ctrl_mapping)

#define ASPEED_LPC_CTRL_IOCTL_MAP	_IOW(__ASPEED_LPC_CTRL_IOCTL_MAGIC, \
		0x01, struct aspeed_lpc_ctrl_mapping)

#endif 
