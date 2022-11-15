
#ifndef _LINUX_PSTORE_H
#define _LINUX_PSTORE_H

#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/kmsg_dump.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/time.h>
#include <linux/types.h>

struct module;


enum pstore_type_id {
	PSTORE_TYPE_DMESG	= 0,
	PSTORE_TYPE_MCE		= 1,
	PSTORE_TYPE_CONSOLE	= 2,
	PSTORE_TYPE_FTRACE	= 3,
	
	PSTORE_TYPE_PPC_RTAS	= 4,
	PSTORE_TYPE_PPC_OF	= 5,
	PSTORE_TYPE_PPC_COMMON	= 6,
	PSTORE_TYPE_PMSG	= 7,
	PSTORE_TYPE_PPC_OPAL	= 8,
	PSTORE_TYPE_UNKNOWN	= 255
};

struct pstore_info;

struct pstore_record {
	struct pstore_info	*psi;
	enum pstore_type_id	type;
	u64			id;
	struct timespec		time;
	char			*buf;
	ssize_t			size;
	ssize_t			ecc_notice_size;

	int			count;
	enum kmsg_dump_reason	reason;
	unsigned int		part;
	bool			compressed;
};


struct pstore_info {
	struct module	*owner;
	char		*name;

	struct semaphore buf_lock;
	char		*buf;
	size_t		bufsize;

	struct mutex	read_mutex;

	int		flags;
	void		*data;

	int		(*open)(struct pstore_info *psi);
	int		(*close)(struct pstore_info *psi);
	ssize_t		(*read)(struct pstore_record *record);
	int		(*write)(struct pstore_record *record);
	int		(*write_user)(struct pstore_record *record,
				      const char __user *buf);
	int		(*erase)(struct pstore_record *record);
};


#define PSTORE_FLAGS_DMESG	(1 << 0)
#define PSTORE_FLAGS_CONSOLE	(1 << 1)
#define PSTORE_FLAGS_FTRACE	(1 << 2)
#define PSTORE_FLAGS_PMSG	(1 << 3)

extern int pstore_register(struct pstore_info *);
extern void pstore_unregister(struct pstore_info *);

struct pstore_ftrace_record {
	unsigned long ip;
	unsigned long parent_ip;
	u64 ts;
};



#if NR_CPUS <= 2 && defined(CONFIG_ARM_THUMB)
#define PSTORE_CPU_IN_IP 0x1
#elif NR_CPUS <= 4 && defined(CONFIG_ARM)
#define PSTORE_CPU_IN_IP 0x3
#endif

#define TS_CPU_SHIFT 8
#define TS_CPU_MASK (BIT(TS_CPU_SHIFT) - 1)


#ifdef PSTORE_CPU_IN_IP
static inline void
pstore_ftrace_encode_cpu(struct pstore_ftrace_record *rec, unsigned int cpu)
{
	rec->ip |= cpu;
}

static inline unsigned int
pstore_ftrace_decode_cpu(struct pstore_ftrace_record *rec)
{
	return rec->ip & PSTORE_CPU_IN_IP;
}

static inline u64
pstore_ftrace_read_timestamp(struct pstore_ftrace_record *rec)
{
	return rec->ts;
}

static inline void
pstore_ftrace_write_timestamp(struct pstore_ftrace_record *rec, u64 val)
{
	rec->ts = val;
}
#else
static inline void
pstore_ftrace_encode_cpu(struct pstore_ftrace_record *rec, unsigned int cpu)
{
	rec->ts &= ~(TS_CPU_MASK);
	rec->ts |= cpu;
}

static inline unsigned int
pstore_ftrace_decode_cpu(struct pstore_ftrace_record *rec)
{
	return rec->ts & TS_CPU_MASK;
}

static inline u64
pstore_ftrace_read_timestamp(struct pstore_ftrace_record *rec)
{
	return rec->ts >> TS_CPU_SHIFT;
}

static inline void
pstore_ftrace_write_timestamp(struct pstore_ftrace_record *rec, u64 val)
{
	rec->ts = (rec->ts & TS_CPU_MASK) | (val << TS_CPU_SHIFT);
}
#endif

#endif 
