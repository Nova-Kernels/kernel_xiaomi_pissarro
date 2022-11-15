/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __MTK_LPM_H__
#define __MTK_LPM_H__

#include <linux/cpu_pm.h>
#include <linux/cpuidle.h>
#include <asm/cpuidle.h>

#define MTK_CPUIDLE_PM_NAME	"mtk_cpuidle_pm"

struct mtk_cpuidle_op {
	
	int (*cpuidle_prepare)(struct cpuidle_driver *drv, int index);

	
	void (*cpuidle_resume)(struct cpuidle_driver *drv, int index);
};


int mtk_lpm_drv_cpuidle_ops_set(struct mtk_cpuidle_op *op);
void mtk_lpm_drv_cpuidle_ops_clr(void);

#endif
