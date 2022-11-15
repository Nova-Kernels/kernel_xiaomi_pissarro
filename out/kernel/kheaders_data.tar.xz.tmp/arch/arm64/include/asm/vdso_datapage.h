
#ifndef __ASM_VDSO_DATAPAGE_H
#define __ASM_VDSO_DATAPAGE_H

#ifdef __KERNEL__

#ifndef __ASSEMBLY__

struct vdso_data {
	__u64 cs_cycle_last;	
	__u64 raw_time_sec;	
	__u64 raw_time_nsec;
	__u64 xtime_clock_sec;	
	__u64 xtime_clock_nsec;
	__u64 xtime_coarse_sec;	
	__u64 xtime_coarse_nsec;
	__u64 wtm_clock_sec;	
	__u64 wtm_clock_nsec;
	__u32 tb_seq_count;	
	
	__u32 cs_mono_mult;	
	__u32 cs_shift;		
	__u32 cs_raw_mult;	
	__u32 tz_minuteswest;	
	__u32 tz_dsttime;
	__u32 use_syscall;
	__u32 hrtimer_res;
};

#endif 

#endif 

#endif 
