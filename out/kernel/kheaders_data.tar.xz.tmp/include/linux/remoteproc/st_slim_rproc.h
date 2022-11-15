
#ifndef _ST_REMOTEPROC_SLIM_H
#define _ST_REMOTEPROC_SLIM_H

#define ST_SLIM_MEM_MAX 2
#define ST_SLIM_MAX_CLK 4

enum {
	ST_SLIM_DMEM,
	ST_SLIM_IMEM,
};


struct st_slim_mem {
	void __iomem *cpu_addr;
	phys_addr_t bus_addr;
	size_t size;
};


struct st_slim_rproc {
	struct rproc *rproc;
	struct st_slim_mem mem[ST_SLIM_MEM_MAX];
	void __iomem *slimcore;
	void __iomem *peri;

	
	struct clk *clks[ST_SLIM_MAX_CLK];
};

struct st_slim_rproc *st_slim_rproc_alloc(struct platform_device *pdev,
					char *fw_name);
void st_slim_rproc_put(struct st_slim_rproc *slim_rproc);

#endif
