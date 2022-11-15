/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_UACCESS_H__
#define __LINUX_UACCESS_H__

#include <linux/sched.h>
#include <linux/thread_info.h>
#include <linux/kasan-checks.h>

#define VERIFY_READ 0
#define VERIFY_WRITE 1

#define uaccess_kernel() segment_eq(get_fs(), KERNEL_DS)

#include <asm/uaccess.h>



static __always_inline unsigned long
__copy_from_user_inatomic(void *to, const void __user *from, unsigned long n)
{
	kasan_check_write(to, n);
	check_object_size(to, n, false);
	return raw_copy_from_user(to, from, n);
}

static __always_inline unsigned long
__copy_from_user(void *to, const void __user *from, unsigned long n)
{
	might_fault();
	kasan_check_write(to, n);
	check_object_size(to, n, false);
	return raw_copy_from_user(to, from, n);
}


static __always_inline unsigned long
__copy_to_user_inatomic(void __user *to, const void *from, unsigned long n)
{
	kasan_check_read(from, n);
	check_object_size(from, n, true);
	return raw_copy_to_user(to, from, n);
}

static __always_inline unsigned long
__copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	kasan_check_read(from, n);
	check_object_size(from, n, true);
	return raw_copy_to_user(to, from, n);
}

#ifdef INLINE_COPY_FROM_USER
static inline unsigned long
_copy_from_user(void *to, const void __user *from, unsigned long n)
{
	unsigned long res = n;
	might_fault();
	if (likely(access_ok(VERIFY_READ, from, n))) {
		kasan_check_write(to, n);
		res = raw_copy_from_user(to, from, n);
	}
	if (unlikely(res))
		memset(to + (n - res), 0, res);
	return res;
}
#else
extern unsigned long
_copy_from_user(void *, const void __user *, unsigned long);
#endif

#ifdef INLINE_COPY_TO_USER
static inline unsigned long
_copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	if (access_ok(VERIFY_WRITE, to, n)) {
		kasan_check_read(from, n);
		n = raw_copy_to_user(to, from, n);
	}
	return n;
}
#else
extern unsigned long
_copy_to_user(void __user *, const void *, unsigned long);
#endif

static __always_inline unsigned long __must_check
copy_from_user(void *to, const void __user *from, unsigned long n)
{
	if (likely(check_copy_size(to, n, false)))
		n = _copy_from_user(to, from, n);
	return n;
}

static __always_inline unsigned long __must_check
copy_to_user(void __user *to, const void *from, unsigned long n)
{
	if (likely(check_copy_size(from, n, true)))
		n = _copy_to_user(to, from, n);
	return n;
}
#ifdef CONFIG_COMPAT
static __always_inline unsigned long __must_check
copy_in_user(void __user *to, const void __user *from, unsigned long n)
{
	might_fault();
	if (access_ok(VERIFY_WRITE, to, n) && access_ok(VERIFY_READ, from, n))
		n = raw_copy_in_user(to, from, n);
	return n;
}
#endif

static __always_inline void pagefault_disabled_inc(void)
{
	current->pagefault_disabled++;
}

static __always_inline void pagefault_disabled_dec(void)
{
	current->pagefault_disabled--;
}


static inline void pagefault_disable(void)
{
	pagefault_disabled_inc();
	
	barrier();
}

static inline void pagefault_enable(void)
{
	
	barrier();
	pagefault_disabled_dec();
}


#define pagefault_disabled() (current->pagefault_disabled != 0)


#define faulthandler_disabled() (pagefault_disabled() || in_atomic())

#ifndef ARCH_HAS_NOCACHE_UACCESS

static inline unsigned long __copy_from_user_inatomic_nocache(void *to,
				const void __user *from, unsigned long n)
{
	return __copy_from_user_inatomic(to, from, n);
}

#endif		


extern long probe_kernel_read(void *dst, const void *src, size_t size);
extern long __probe_kernel_read(void *dst, const void *src, size_t size);


extern long notrace probe_kernel_write(void *dst, const void *src, size_t size);
extern long notrace __probe_kernel_write(void *dst, const void *src, size_t size);

extern long strncpy_from_unsafe(char *dst, const void *unsafe_addr, long count);


#define probe_kernel_address(addr, retval)		\
	probe_kernel_read(&retval, addr, sizeof(retval))

#ifndef user_access_begin
#define user_access_begin(type, ptr, len) access_ok(type, ptr, len)
#define user_access_end() do { } while (0)
#define unsafe_get_user(x, ptr, err) do { if (unlikely(__get_user(x, ptr))) goto err; } while (0)
#define unsafe_put_user(x, ptr, err) do { if (unlikely(__put_user(x, ptr))) goto err; } while (0)
static inline unsigned long user_access_save(void) { return 0UL; }
static inline void user_access_restore(unsigned long flags) { }
#endif

#endif		
