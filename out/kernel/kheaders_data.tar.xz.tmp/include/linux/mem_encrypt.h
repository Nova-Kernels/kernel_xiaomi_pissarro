

#ifndef __MEM_ENCRYPT_H__
#define __MEM_ENCRYPT_H__

#ifndef __ASSEMBLY__

#ifdef CONFIG_ARCH_HAS_MEM_ENCRYPT

#include <asm/mem_encrypt.h>

#else	

#define sme_me_mask	0ULL

#endif	

static inline bool sme_active(void)
{
	return !!sme_me_mask;
}

static inline u64 sme_get_me_mask(void)
{
	return sme_me_mask;
}

#ifdef CONFIG_AMD_MEM_ENCRYPT

#define __sme_set(x)		((x) | sme_me_mask)
#define __sme_clr(x)		((x) & ~sme_me_mask)
#else
#define __sme_set(x)		(x)
#define __sme_clr(x)		(x)
#endif

#endif	

#endif	
