

#ifndef _UVERBS_TYPES_
#define _UVERBS_TYPES_

#include <linux/kernel.h>
#include <rdma/ib_verbs.h>

struct uverbs_obj_type;

struct uverbs_obj_type_class {
	
	struct ib_uobject *(*alloc_begin)(const struct uverbs_obj_type *type,
					  struct ib_ucontext *ucontext);
	void (*alloc_commit)(struct ib_uobject *uobj);
	void (*alloc_abort)(struct ib_uobject *uobj);

	struct ib_uobject *(*lookup_get)(const struct uverbs_obj_type *type,
					 struct ib_ucontext *ucontext, int id,
					 bool exclusive);
	void (*lookup_put)(struct ib_uobject *uobj, bool exclusive);
	
	int __must_check (*remove_commit)(struct ib_uobject *uobj,
					  enum rdma_remove_reason why);
	u8    needs_kfree_rcu;
};

struct uverbs_obj_type {
	const struct uverbs_obj_type_class * const type_class;
	size_t	     obj_size;
	unsigned int destroy_order;
};



struct uverbs_obj_idr_type {
	
	struct uverbs_obj_type  type;

	
	int __must_check (*destroy_object)(struct ib_uobject *uobj,
					   enum rdma_remove_reason why);
};

struct ib_uobject *rdma_lookup_get_uobject(const struct uverbs_obj_type *type,
					   struct ib_ucontext *ucontext,
					   int id, bool exclusive);
void rdma_lookup_put_uobject(struct ib_uobject *uobj, bool exclusive);
struct ib_uobject *rdma_alloc_begin_uobject(const struct uverbs_obj_type *type,
					    struct ib_ucontext *ucontext);
void rdma_alloc_abort_uobject(struct ib_uobject *uobj);
int __must_check rdma_remove_commit_uobject(struct ib_uobject *uobj);
int rdma_alloc_commit_uobject(struct ib_uobject *uobj);
int rdma_explicit_destroy(struct ib_uobject *uobject);

struct uverbs_obj_fd_type {
	
	struct uverbs_obj_type  type;
	int (*context_closed)(struct ib_uobject_file *uobj_file,
			      enum rdma_remove_reason why);
	const struct file_operations	*fops;
	const char			*name;
	int				flags;
};

extern const struct uverbs_obj_type_class uverbs_idr_class;
extern const struct uverbs_obj_type_class uverbs_fd_class;

#define UVERBS_BUILD_BUG_ON(cond) (sizeof(char[1 - 2 * !!(cond)]) -	\
				   sizeof(char))
#define UVERBS_TYPE_ALLOC_FD(_order, _obj_size, _context_closed, _fops, _name, _flags)\
	((&((const struct uverbs_obj_fd_type)				\
	 {.type = {							\
		.destroy_order = _order,				\
		.type_class = &uverbs_fd_class,				\
		.obj_size = (_obj_size) +				\
			UVERBS_BUILD_BUG_ON((_obj_size) < sizeof(struct ib_uobject_file)), \
	 },								\
	 .context_closed = _context_closed,				\
	 .fops = _fops,							\
	 .name = _name,							\
	 .flags = _flags}))->type)
#define UVERBS_TYPE_ALLOC_IDR_SZ(_size, _order, _destroy_object)	\
	((&((const struct uverbs_obj_idr_type)				\
	 {.type = {							\
		.destroy_order = _order,				\
		.type_class = &uverbs_idr_class,			\
		.obj_size = (_size) +					\
			UVERBS_BUILD_BUG_ON((_size) <			\
					    sizeof(struct ib_uobject))	\
	 },								\
	 .destroy_object = _destroy_object,}))->type)
#define UVERBS_TYPE_ALLOC_IDR(_order, _destroy_object)			\
	 UVERBS_TYPE_ALLOC_IDR_SZ(sizeof(struct ib_uobject), _order,	\
				  _destroy_object)

#endif
