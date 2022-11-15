// SPDX-License-Identifier: GPL-2.0

#ifndef __PMIF_H__
#define __PMIF_H__

#include <linux/clk.h>

enum {
	SPMI_MASTER_0 = 0,
	SPMI_MASTER_1,
	SPMI_MASTER_2,
	SPMI_MASTER_3
};

struct pmif {
	void __iomem		*base;
	const u32		*regs;
	void __iomem		*spmimst_base;
	const u32		*spmimst_regs;
	void __iomem		*infra_base;
	const u32               *infra_regs;
	void __iomem		*topckgen_base;
	const u32               *topckgen_regs;
	void __iomem		*toprgu_base;
	const u32               *toprgu_regs;
	const u32		*dbgregs;
	u32			dbgver;
	u32                     swinf_ch_start;
	u32                     ap_swinf_no;
	int                     write;
	int                     mstid;
	int                     pmifid;
	int			irq;
	int			grpid;
	raw_spinlock_t          lock;
	struct wakeup_source *pmifThread_lock;
	struct mutex pmif_mutex;
	struct spmi_controller  *spmic;
	struct clk *pmif_sys_ck;
	struct clk *pmif_tmr_ck;
	struct clk *pmif_clk_mux;
	struct clk *pmif_clk_osc_d10;
	struct clk *pmif_clk26m;
	struct clk *spmimst_clk_mux;
	struct clk *spmimst_clk26m;
	struct clk *spmimst_clk_osc_d10;
	int (*cmd)(struct spmi_controller *ctrl, unsigned int opcode);
	int (*read_cmd)(struct spmi_controller *ctrl, u8 opc, u8 sid,
			u16 addr, u8 *buf, size_t len);
	int (*write_cmd)(struct spmi_controller *ctrl, u8 opc, u8 sid,
			u16 addr, const u8 *buf, size_t len);
	void (*pmif_enable_clk_set)(struct pmif *arb);
	void (*pmif_force_normal_mode)(struct pmif *arb);
	void (*pmif_enable_swinf)(struct pmif *arb, unsigned int chan_no,
			unsigned int swinf_no);
	void (*pmif_enable_cmdIssue)(struct pmif *arb, bool en);
	void (*pmif_enable)(struct pmif *arb);
	int (*is_pmif_init_done)(struct pmif *arb);
	void (*pmif_enable_reset)(struct pmif *arb);
	int (*pmif_cali_clock)(struct pmif *arb);
	int (*spmi_config_master)(struct pmif *arb, unsigned int mstid,
			bool en);
};
#endif 
