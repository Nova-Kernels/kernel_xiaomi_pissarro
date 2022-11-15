/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_LINUX_BINDER_CTL_H
#define _UAPI_LINUX_BINDER_CTL_H

#include <linux/android/binder.h>
#include <linux/types.h>
#include <linux/ioctl.h>

#define BINDERFS_MAX_NAME 255


struct binderfs_device {
	char name[BINDERFS_MAX_NAME + 1];
	__u8 major;
	__u8 minor;
};


#define BINDER_CTL_ADD _IOWR('b', 1, struct binderfs_device)

#endif 

