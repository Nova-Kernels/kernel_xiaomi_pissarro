

#ifndef _UVERBS_STD_TYPES__
#define _UVERBS_STD_TYPES__

#include <rdma/uverbs_types.h>
#include <rdma/uverbs_ioctl.h>
#include <rdma/ib_user_ioctl_verbs.h>

#if IS_ENABLED(CONFIG_INFINIBAND_USER_ACCESS)
extern const struct uverbs_object_def uverbs_object_comp_channel;
extern const struct uverbs_object_def uverbs_object_cq;
extern const struct uverbs_object_def uverbs_object_qp;
extern const struct uverbs_object_def uverbs_object_rwq_ind_table;
extern const struct uverbs_object_def uverbs_object_wq;
extern const struct uverbs_object_def uverbs_object_srq;
extern const struct uverbs_object_def uverbs_object_ah;
extern const struct uverbs_object_def uverbs_object_flow;
extern const struct uverbs_object_def uverbs_object_mr;
extern const struct uverbs_object_def uverbs_object_mw;
extern const struct uverbs_object_def uverbs_object_pd;
extern const struct uverbs_object_def uverbs_object_xrcd;
extern const struct uverbs_object_def uverbs_object_device;

extern const struct uverbs_object_tree_def uverbs_default_objects;
static inline const struct uverbs_object_tree_def *uverbs_default_get_objects(void)
{
	return &uverbs_default_objects;
}
#else
static inline const struct uverbs_object_tree_def *uverbs_default_get_objects(void)
{
	return NULL;
}
#endif

static inline struct ib_uobject *__uobj_get(const struct uverbs_obj_type *type,
					    bool write,
					    struct ib_ucontext *ucontext,
					    int id)
{
	return rdma_lookup_get_uobject(type, ucontext, id, write);
}

#define uobj_get_type(_object) uverbs_object_##_object.type_attrs

#define uobj_get_read(_type, _id, _ucontext)				\
	 __uobj_get(_type, false, _ucontext, _id)

#define uobj_get_obj_read(_object, _id, _ucontext)			\
({									\
	struct ib_uobject *__uobj =					\
		__uobj_get(uverbs_object_##_object.type_attrs,		\
			   false, _ucontext, _id);			\
									\
	(struct ib_##_object *)(IS_ERR(__uobj) ? NULL : __uobj->object);\
})

#define uobj_get_write(_type, _id, _ucontext)				\
	 __uobj_get(_type, true, _ucontext, _id)

static inline void uobj_put_read(struct ib_uobject *uobj)
{
	rdma_lookup_put_uobject(uobj, false);
}

#define uobj_put_obj_read(_obj)					\
	uobj_put_read((_obj)->uobject)

static inline void uobj_put_write(struct ib_uobject *uobj)
{
	rdma_lookup_put_uobject(uobj, true);
}

static inline int __must_check uobj_remove_commit(struct ib_uobject *uobj)
{
	return rdma_remove_commit_uobject(uobj);
}

static inline void uobj_alloc_commit(struct ib_uobject *uobj)
{
	rdma_alloc_commit_uobject(uobj);
}

static inline void uobj_alloc_abort(struct ib_uobject *uobj)
{
	rdma_alloc_abort_uobject(uobj);
}

static inline struct ib_uobject *__uobj_alloc(const struct uverbs_obj_type *type,
					      struct ib_ucontext *ucontext)
{
	return rdma_alloc_begin_uobject(type, ucontext);
}

#define uobj_alloc(_type, ucontext)	\
	__uobj_alloc(_type, ucontext)

#endif

