/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */

#ifndef _UAPI__ULEDS_H_
#define _UAPI__ULEDS_H_

#define LED_MAX_NAME_SIZE	64

struct uleds_user_dev {
	char name[LED_MAX_NAME_SIZE];
	int max_brightness;
};

#endif 
