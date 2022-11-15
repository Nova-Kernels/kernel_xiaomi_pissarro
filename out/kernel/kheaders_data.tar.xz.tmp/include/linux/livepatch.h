

#ifndef _LINUX_LIVEPATCH_H_
#define _LINUX_LIVEPATCH_H_

#include <linux/module.h>
#include <linux/ftrace.h>
#include <linux/completion.h>

#if IS_ENABLED(CONFIG_LIVEPATCH)

#include <asm/livepatch.h>


#define KLP_UNDEFINED	-1
#define KLP_UNPATCHED	 0
#define KLP_PATCHED	 1


struct klp_func {
	
	const char *old_name;
	void *new_func;
	
	unsigned long old_sympos;
	bool immediate;

	
	unsigned long old_addr;
	struct kobject kobj;
	struct list_head stack_node;
	unsigned long old_size, new_size;
	bool patched;
	bool transition;
};


struct klp_object {
	
	const char *name;
	struct klp_func *funcs;

	
	struct kobject kobj;
	struct module *mod;
	bool patched;
};


struct klp_patch {
	
	struct module *mod;
	struct klp_object *objs;
	bool immediate;

	
	struct list_head list;
	struct kobject kobj;
	bool enabled;
	struct completion finish;
};

#define klp_for_each_object(patch, obj) \
	for (obj = patch->objs; obj->funcs || obj->name; obj++)

#define klp_for_each_func(obj, func) \
	for (func = obj->funcs; \
	     func->old_name || func->new_func || func->old_sympos; \
	     func++)

int klp_register_patch(struct klp_patch *);
int klp_unregister_patch(struct klp_patch *);
int klp_enable_patch(struct klp_patch *);
int klp_disable_patch(struct klp_patch *);

void arch_klp_init_object_loaded(struct klp_patch *patch,
				 struct klp_object *obj);


int klp_module_coming(struct module *mod);
void klp_module_going(struct module *mod);

void klp_copy_process(struct task_struct *child);
void klp_update_patch_state(struct task_struct *task);

static inline bool klp_patch_pending(struct task_struct *task)
{
	return test_tsk_thread_flag(task, TIF_PATCH_PENDING);
}

static inline bool klp_have_reliable_stack(void)
{
	return IS_ENABLED(CONFIG_STACKTRACE) &&
	       IS_ENABLED(CONFIG_HAVE_RELIABLE_STACKTRACE);
}

#else 

static inline int klp_module_coming(struct module *mod) { return 0; }
static inline void klp_module_going(struct module *mod) {}
static inline bool klp_patch_pending(struct task_struct *task) { return false; }
static inline void klp_update_patch_state(struct task_struct *task) {}
static inline void klp_copy_process(struct task_struct *child) {}

#endif 

#endif 
