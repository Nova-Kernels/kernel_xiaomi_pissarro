

#ifndef _RODATA_TEST_H
#define _RODATA_TEST_H

#ifdef CONFIG_DEBUG_RODATA_TEST
void rodata_test(void);
#else
static inline void rodata_test(void) {}
#endif

#endif 
