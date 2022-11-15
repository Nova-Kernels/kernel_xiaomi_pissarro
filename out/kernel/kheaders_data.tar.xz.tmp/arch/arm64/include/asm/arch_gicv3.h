
#ifndef __ASM_ARCH_GICV3_H
#define __ASM_ARCH_GICV3_H

#include <asm/sysreg.h>

#ifndef __ASSEMBLY__

#include <linux/stringify.h>
#include <asm/barrier.h>
#include <asm/cacheflush.h>

#define read_gicreg(r)			read_sysreg_s(SYS_ ## r)
#define write_gicreg(v, r)		write_sysreg_s(v, SYS_ ## r)



static inline void gic_write_eoir(u32 irq)
{
	write_sysreg_s(irq, SYS_ICC_EOIR1_EL1);
	isb();
}

static inline void gic_write_dir(u32 irq)
{
	write_sysreg_s(irq, SYS_ICC_DIR_EL1);
	isb();
}

static inline u64 gic_read_iar_common(void)
{
	u64 irqstat;

	irqstat = read_sysreg_s(SYS_ICC_IAR1_EL1);
	dsb(sy);
	return irqstat;
}


static inline u64 gic_read_iar_cavium_thunderx(void)
{
	u64 irqstat;

	nops(8);
	irqstat = read_sysreg_s(SYS_ICC_IAR1_EL1);
	nops(4);
	mb();

	return irqstat;
}

static inline void gic_write_pmr(u32 val)
{
	write_sysreg_s(val, SYS_ICC_PMR_EL1);
}

static inline void gic_write_ctlr(u32 val)
{
	write_sysreg_s(val, SYS_ICC_CTLR_EL1);
	isb();
}

static inline void gic_write_grpen1(u32 val)
{
	write_sysreg_s(val, SYS_ICC_IGRPEN1_EL1);
	isb();
}

static inline void gic_write_sgi1r(u64 val)
{
	write_sysreg_s(val, SYS_ICC_SGI1R_EL1);
}

static inline u32 gic_read_sre(void)
{
	return read_sysreg_s(SYS_ICC_SRE_EL1);
}

static inline void gic_write_sre(u32 val)
{
	write_sysreg_s(val, SYS_ICC_SRE_EL1);
	isb();
}

static inline void gic_write_bpr1(u32 val)
{
	write_sysreg_s(val, SYS_ICC_BPR1_EL1);
}

#define gic_read_typer(c)		readq_relaxed(c)
#define gic_write_irouter(v, c)		writeq_relaxed(v, c)
#define gic_read_lpir(c)		readq_relaxed(c)
#define gic_write_lpir(v, c)		writeq_relaxed(v, c)

#define gic_flush_dcache_to_poc(a,l)	__flush_dcache_area((a), (l))

#define gits_read_baser(c)		readq_relaxed(c)
#define gits_write_baser(v, c)		writeq_relaxed(v, c)

#define gits_read_cbaser(c)		readq_relaxed(c)
#define gits_write_cbaser(v, c)		writeq_relaxed(v, c)

#define gits_write_cwriter(v, c)	writeq_relaxed(v, c)

#define gicr_read_propbaser(c)		readq_relaxed(c)
#define gicr_write_propbaser(v, c)	writeq_relaxed(v, c)

#define gicr_write_pendbaser(v, c)	writeq_relaxed(v, c)
#define gicr_read_pendbaser(c)		readq_relaxed(c)

#define gits_write_vpropbaser(v, c)	writeq_relaxed(v, c)

#define gits_write_vpendbaser(v, c)	writeq_relaxed(v, c)
#define gits_read_vpendbaser(c)		readq_relaxed(c)

#endif 
#endif 
