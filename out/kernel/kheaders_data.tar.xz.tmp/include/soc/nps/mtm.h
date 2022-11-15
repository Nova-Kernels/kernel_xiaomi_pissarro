

#ifndef SOC_NPS_MTM_H
#define SOC_NPS_MTM_H

#define CTOP_INST_HWSCHD_OFF_R3                 0x3B6F00BF
#define CTOP_INST_HWSCHD_RESTORE_R3             0x3E6F70C3

static inline void hw_schd_save(unsigned int *flags)
{
	__asm__ __volatile__(
	"       .word %1\n"
	"       st r3,[%0]\n"
	:
	: "r"(flags), "i"(CTOP_INST_HWSCHD_OFF_R3)
	: "r3", "memory");
}

static inline void hw_schd_restore(unsigned int flags)
{
	__asm__ __volatile__(
	"       mov r3, %0\n"
	"       .word %1\n"
	:
	: "r"(flags), "i"(CTOP_INST_HWSCHD_RESTORE_R3)
	: "r3");
}

#endif 
