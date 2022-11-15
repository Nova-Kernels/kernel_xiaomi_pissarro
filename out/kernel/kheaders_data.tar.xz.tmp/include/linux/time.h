/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIME_H
#define _LINUX_TIME_H

# include <linux/cache.h>
# include <linux/seqlock.h>
# include <linux/math64.h>
# include <linux/time64.h>

extern struct timezone sys_tz;

int get_timespec64(struct timespec64 *ts,
		const struct timespec __user *uts);
int put_timespec64(const struct timespec64 *ts,
		struct timespec __user *uts);
int get_itimerspec64(struct itimerspec64 *it,
			const struct itimerspec __user *uit);
int put_itimerspec64(const struct itimerspec64 *it,
			struct itimerspec __user *uit);

#define TIME_T_MAX	(time_t)((1UL << ((sizeof(time_t) << 3) - 1)) - 1)

static inline int timespec_equal(const struct timespec *a,
                                 const struct timespec *b)
{
	return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
}


static inline int timespec_compare(const struct timespec *lhs, const struct timespec *rhs)
{
	if (lhs->tv_sec < rhs->tv_sec)
		return -1;
	if (lhs->tv_sec > rhs->tv_sec)
		return 1;
	return lhs->tv_nsec - rhs->tv_nsec;
}

static inline int timeval_compare(const struct timeval *lhs, const struct timeval *rhs)
{
	if (lhs->tv_sec < rhs->tv_sec)
		return -1;
	if (lhs->tv_sec > rhs->tv_sec)
		return 1;
	return lhs->tv_usec - rhs->tv_usec;
}

extern time64_t mktime64(const unsigned int year, const unsigned int mon,
			const unsigned int day, const unsigned int hour,
			const unsigned int min, const unsigned int sec);


static inline unsigned long mktime(const unsigned int year,
			const unsigned int mon, const unsigned int day,
			const unsigned int hour, const unsigned int min,
			const unsigned int sec)
{
	return mktime64(year, mon, day, hour, min, sec);
}

extern void set_normalized_timespec(struct timespec *ts, time_t sec, s64 nsec);


extern struct timespec timespec_add_safe(const struct timespec lhs,
					 const struct timespec rhs);


static inline struct timespec timespec_add(struct timespec lhs,
						struct timespec rhs)
{
	struct timespec ts_delta;
	set_normalized_timespec(&ts_delta, lhs.tv_sec + rhs.tv_sec,
				lhs.tv_nsec + rhs.tv_nsec);
	return ts_delta;
}


static inline struct timespec timespec_sub(struct timespec lhs,
						struct timespec rhs)
{
	struct timespec ts_delta;
	set_normalized_timespec(&ts_delta, lhs.tv_sec - rhs.tv_sec,
				lhs.tv_nsec - rhs.tv_nsec);
	return ts_delta;
}


static inline bool timespec_valid(const struct timespec *ts)
{
	
	if (ts->tv_sec < 0)
		return false;
	
	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC)
		return false;
	return true;
}

static inline bool timespec_valid_strict(const struct timespec *ts)
{
	if (!timespec_valid(ts))
		return false;
	
	if ((unsigned long long)ts->tv_sec >= KTIME_SEC_MAX)
		return false;
	return true;
}

static inline bool timeval_valid(const struct timeval *tv)
{
	
	if (tv->tv_sec < 0)
		return false;

	
	if (tv->tv_usec < 0 || tv->tv_usec >= USEC_PER_SEC)
		return false;

	return true;
}

extern struct timespec timespec_trunc(struct timespec t, unsigned gran);


static inline bool timeval_inject_offset_valid(const struct timeval *tv)
{
	

	
	if (tv->tv_usec < 0 || tv->tv_usec >= USEC_PER_SEC)
		return false;
	return true;
}

static inline bool timespec_inject_offset_valid(const struct timespec *ts)
{
	

	
	if (ts->tv_nsec < 0 || ts->tv_nsec >= NSEC_PER_SEC)
		return false;
	return true;
}


#ifdef CONFIG_ARCH_USES_GETTIMEOFFSET
extern u32 (*arch_gettimeoffset)(void);
#endif

struct itimerval;
extern int do_setitimer(int which, struct itimerval *value,
			struct itimerval *ovalue);
extern int do_getitimer(int which, struct itimerval *value);

extern long do_utimes(int dfd, const char __user *filename, struct timespec64 *times, int flags);


struct tm {
	
	int tm_sec;
	
	int tm_min;
	
	int tm_hour;
	
	int tm_mday;
	
	int tm_mon;
	
	long tm_year;
	
	int tm_wday;
	
	int tm_yday;
};

void time64_to_tm(time64_t totalsecs, int offset, struct tm *result);


static inline void time_to_tm(time_t totalsecs, int offset, struct tm *result)
{
	time64_to_tm(totalsecs, offset, result);
}


static inline s64 timespec_to_ns(const struct timespec *ts)
{
	return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
}


static inline s64 timeval_to_ns(const struct timeval *tv)
{
	return ((s64) tv->tv_sec * NSEC_PER_SEC) +
		tv->tv_usec * NSEC_PER_USEC;
}


extern struct timespec ns_to_timespec(const s64 nsec);


extern struct timeval ns_to_timeval(const s64 nsec);


static __always_inline void timespec_add_ns(struct timespec *a, u64 ns)
{
	a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
	a->tv_nsec = ns;
}

static inline bool itimerspec64_valid(const struct itimerspec64 *its)
{
	if (!timespec64_valid(&(its->it_interval)) ||
		!timespec64_valid(&(its->it_value)))
		return false;

	return true;
}


#define time_after32(a, b)	((s32)((u32)(b) - (u32)(a)) < 0)
#define time_before32(b, a)	time_after32(a, b)


#define time_between32(t, l, h) ((u32)(h) - (u32)(l) >= (u32)(t) - (u32)(l))
#endif
