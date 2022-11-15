/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_EFI_BGRT_H
#define _LINUX_EFI_BGRT_H

#include <linux/acpi.h>

#ifdef CONFIG_ACPI_BGRT

void efi_bgrt_init(struct acpi_table_header *table);
int __init acpi_parse_bgrt(struct acpi_table_header *table);


extern size_t bgrt_image_size;
extern struct acpi_table_bgrt bgrt_tab;

#else 

static inline void efi_bgrt_init(struct acpi_table_header *table) {}
static inline int __init acpi_parse_bgrt(struct acpi_table_header *table)
{
	return 0;
}

#endif 

#endif 
