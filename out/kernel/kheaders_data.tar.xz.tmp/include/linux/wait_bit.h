/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_WAIT_BIT_H
#define _LINUX_WAIT_BIT_H


#include <linux/wait.h>

struct wait_bit_key {
	void			*flags;
	int			bit_nr;
#define WAIT_ATOMIC_T_BIT_NR	-1
	unsigned long		timeout;
};

struct wait_bit_queue_entry {
	struct wait_bit_key	key;
	struct wait_queue_entry	wq_entry;
};

#define __WAIT_BIT_KEY_INITIALIZER(word, bit)					\
	{ .flags = word, .bit_nr = bit, }

#define __WAIT_ATOMIC_T_KEY_INITIALIZER(p)					\
	{ .flags = p, .bit_nr = WAIT_ATOMIC_T_BIT_NR, }

typedef int wait_bit_action_f(struct wait_bit_key *key, int mode);
void __wake_up_bit(struct wait_queue_head *wq_head, void *word, int bit);
int __wait_on_bit(struct wait_queue_head *wq_head, struct wait_bit_queue_entry *wbq_entry, wait_bit_action_f *action, unsigned int mode);
int __wait_on_bit_lock(struct wait_queue_head *wq_head, struct wait_bit_queue_entry *wbq_entry, wait_bit_action_f *action, unsigned int mode);
void wake_up_bit(void *word, int bit);
void wake_up_atomic_t(atomic_t *p);
int out_of_line_wait_on_bit(void *word, int, wait_bit_action_f *action, unsigned int mode);
int out_of_line_wait_on_bit_timeout(void *word, int, wait_bit_action_f *action, unsigned int mode, unsigned long timeout);
int out_of_line_wait_on_bit_lock(void *word, int, wait_bit_action_f *action, unsigned int mode);
int out_of_line_wait_on_atomic_t(atomic_t *p, int (*)(atomic_t *), unsigned int mode);
struct wait_queue_head *bit_waitqueue(void *word, int bit);
extern void __init wait_bit_init(void);

int wake_bit_function(struct wait_queue_entry *wq_entry, unsigned mode, int sync, void *key);

#define DEFINE_WAIT_BIT(name, word, bit)					\
	struct wait_bit_queue_entry name = {					\
		.key = __WAIT_BIT_KEY_INITIALIZER(word, bit),			\
		.wq_entry = {							\
			.private	= current,				\
			.func		= wake_bit_function,			\
			.entry		=					\
				LIST_HEAD_INIT((name).wq_entry.entry),		\
		},								\
	}

extern int bit_wait(struct wait_bit_key *key, int bit);
extern int bit_wait_io(struct wait_bit_key *key, int bit);
extern int bit_wait_timeout(struct wait_bit_key *key, int bit);
extern int bit_wait_io_timeout(struct wait_bit_key *key, int bit);


static inline int
wait_on_bit(unsigned long *word, int bit, unsigned mode)
{
	might_sleep();
	if (!test_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit(word, bit,
				       bit_wait,
				       mode);
}


static inline int
wait_on_bit_io(unsigned long *word, int bit, unsigned mode)
{
	might_sleep();
	if (!test_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit(word, bit,
				       bit_wait_io,
				       mode);
}


static inline int
wait_on_bit_timeout(unsigned long *word, int bit, unsigned mode,
		    unsigned long timeout)
{
	might_sleep();
	if (!test_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit_timeout(word, bit,
					       bit_wait_timeout,
					       mode, timeout);
}


static inline int
wait_on_bit_action(unsigned long *word, int bit, wait_bit_action_f *action,
		   unsigned mode)
{
	might_sleep();
	if (!test_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit(word, bit, action, mode);
}


static inline int
wait_on_bit_lock(unsigned long *word, int bit, unsigned mode)
{
	might_sleep();
	if (!test_and_set_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit_lock(word, bit, bit_wait, mode);
}


static inline int
wait_on_bit_lock_io(unsigned long *word, int bit, unsigned mode)
{
	might_sleep();
	if (!test_and_set_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit_lock(word, bit, bit_wait_io, mode);
}


static inline int
wait_on_bit_lock_action(unsigned long *word, int bit, wait_bit_action_f *action,
			unsigned mode)
{
	might_sleep();
	if (!test_and_set_bit(bit, word))
		return 0;
	return out_of_line_wait_on_bit_lock(word, bit, action, mode);
}


static inline
int wait_on_atomic_t(atomic_t *val, int (*action)(atomic_t *), unsigned mode)
{
	might_sleep();
	if (atomic_read(val) == 0)
		return 0;
	return out_of_line_wait_on_atomic_t(val, action, mode);
}


static inline void clear_and_wake_up_bit(int bit, void *word)
{
	clear_bit_unlock(bit, word);
	
	smp_mb__after_atomic();
	wake_up_bit(word, bit);
}

#endif 
