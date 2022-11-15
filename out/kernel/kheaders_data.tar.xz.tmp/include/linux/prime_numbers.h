/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_PRIME_NUMBERS_H
#define __LINUX_PRIME_NUMBERS_H

#include <linux/types.h>

bool is_prime_number(unsigned long x);
unsigned long next_prime_number(unsigned long x);


#define for_each_prime_number(prime, max) \
	for_each_prime_number_from((prime), 2, (max))


#define for_each_prime_number_from(prime, from, max) \
	for (prime = (from); prime <= (max); prime = next_prime_number(prime))

#endif 
