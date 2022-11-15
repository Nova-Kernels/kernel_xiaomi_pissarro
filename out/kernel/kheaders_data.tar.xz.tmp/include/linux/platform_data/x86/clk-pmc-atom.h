

#ifndef __PLATFORM_DATA_X86_CLK_PMC_ATOM_H
#define __PLATFORM_DATA_X86_CLK_PMC_ATOM_H


struct pmc_clk {
	const char *name;
	unsigned long freq;
	const char *parent_name;
};


struct pmc_clk_data {
	void __iomem *base;
	const struct pmc_clk *clks;
	bool critical;
};

#endif 
