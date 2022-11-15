/* SPDX-License-Identifier: GPL-2.0 */

struct pcs_pdata {
	int irq;
	void (*rearm)(void);
};
