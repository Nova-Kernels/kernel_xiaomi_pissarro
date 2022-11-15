

#ifndef _UVERBS_IOCTL_
#define _UVERBS_IOCTL_

#include <rdma/uverbs_types.h>
#include <linux/uaccess.h>
#include <rdma/rdma_user_ioctl.h>
#include <rdma/ib_user_ioctl_verbs.h>



enum uverbs_attr_type {
	UVERBS_ATTR_TYPE_NA,
	UVERBS_ATTR_TYPE_PTR_IN,
	UVERBS_ATTR_TYPE_PTR_OUT,
	UVERBS_ATTR_TYPE_IDR,
	UVERBS_ATTR_TYPE_FD,
};

enum uverbs_obj_access {
	UVERBS_ACCESS_READ,
	UVERBS_ACCESS_WRITE,
	UVERBS_ACCESS_NEW,
	UVERBS_ACCESS_DESTROY
};

enum {
	UVERBS_ATTR_SPEC_F_MANDATORY	= 1U << 0,
	
	UVERBS_ATTR_SPEC_F_MIN_SZ	= 1U << 1,
};

struct uverbs_attr_spec {
	enum uverbs_attr_type		type;
	union {
		u16				len;
		struct {
			
			u16			obj_type;
			u8			access;
		} obj;
	};
	
	u8				flags;
};

struct uverbs_attr_spec_hash {
	size_t				num_attrs;
	unsigned long			*mandatory_attrs_bitmask;
	struct uverbs_attr_spec		attrs[0];
};

struct uverbs_attr_bundle;
struct ib_uverbs_file;

enum {
	
	UVERBS_ACTION_FLAG_CREATE_ROOT = 1U << 0,
};

struct uverbs_method_spec {
	
	u32						flags;
	size_t						num_buckets;
	size_t						num_child_attrs;
	int (*handler)(struct ib_device *ib_dev, struct ib_uverbs_file *ufile,
		       struct uverbs_attr_bundle *ctx);
	struct uverbs_attr_spec_hash		*attr_buckets[0];
};

struct uverbs_method_spec_hash {
	size_t					num_methods;
	struct uverbs_method_spec		*methods[0];
};

struct uverbs_object_spec {
	const struct uverbs_obj_type		*type_attrs;
	size_t					num_buckets;
	struct uverbs_method_spec_hash		*method_buckets[0];
};

struct uverbs_object_spec_hash {
	size_t					num_objects;
	struct uverbs_object_spec		*objects[0];
};

struct uverbs_root_spec {
	size_t					num_buckets;
	struct uverbs_object_spec_hash		*object_buckets[0];
};



struct uverbs_attr_def {
	u16                           id;
	struct uverbs_attr_spec       attr;
};

struct uverbs_method_def {
	u16                                  id;
	
	u32				     flags;
	size_t				     num_attrs;
	const struct uverbs_attr_def * const (*attrs)[];
	int (*handler)(struct ib_device *ib_dev, struct ib_uverbs_file *ufile,
		       struct uverbs_attr_bundle *ctx);
};

struct uverbs_object_def {
	u16					 id;
	const struct uverbs_obj_type	        *type_attrs;
	size_t				         num_methods;
	const struct uverbs_method_def * const (*methods)[];
};

struct uverbs_object_tree_def {
	size_t					 num_objects;
	const struct uverbs_object_def * const (*objects)[];
};

#define UA_FLAGS(_flags)  .flags = _flags
#define __UVERBS_ATTR0(_id, _len, _type, ...)                           \
	((const struct uverbs_attr_def)				  \
	 {.id = _id, .attr = {.type = _type, {.len = _len}, .flags = 0, } })
#define __UVERBS_ATTR1(_id, _len, _type, _flags)                        \
	((const struct uverbs_attr_def)				  \
	 {.id = _id, .attr = {.type = _type, {.len = _len}, _flags, } })
#define __UVERBS_ATTR(_id, _len, _type, _flags, _n, ...)		\
	__UVERBS_ATTR##_n(_id, _len, _type, _flags)

#define UVERBS_ATTR(_id, _len, _type, ...)				\
	__UVERBS_ATTR(_id, _len, _type, ##__VA_ARGS__, 1, 0)
#define UVERBS_ATTR_PTR_IN_SZ(_id, _len, ...)				\
	UVERBS_ATTR(_id, _len, UVERBS_ATTR_TYPE_PTR_IN, ##__VA_ARGS__)

#define UVERBS_ATTR_PTR_IN(_id, _type, ...)				\
	UVERBS_ATTR_PTR_IN_SZ(_id, sizeof(_type), ##__VA_ARGS__)
#define UVERBS_ATTR_PTR_OUT_SZ(_id, _len, ...)				\
	UVERBS_ATTR(_id, _len, UVERBS_ATTR_TYPE_PTR_OUT, ##__VA_ARGS__)
#define UVERBS_ATTR_PTR_OUT(_id, _type, ...)				\
	UVERBS_ATTR_PTR_OUT_SZ(_id, sizeof(_type), ##__VA_ARGS__)


#define ___UVERBS_ATTR_OBJ0(_id, _obj_class, _obj_type, _access, ...)\
	((const struct uverbs_attr_def)					\
	{.id = _id,							\
	 .attr = {.type = _obj_class,					\
		  {.obj = {.obj_type = _obj_type, .access = _access } },\
		  .flags = 0} })
#define ___UVERBS_ATTR_OBJ1(_id, _obj_class, _obj_type, _access, _flags)\
	((const struct uverbs_attr_def)					\
	{.id = _id,							\
	.attr = {.type = _obj_class,					\
		 {.obj = {.obj_type = _obj_type, .access = _access} },	\
		  _flags} })
#define ___UVERBS_ATTR_OBJ(_id, _obj_class, _obj_type, _access, _flags, \
			   _n, ...)					\
	___UVERBS_ATTR_OBJ##_n(_id, _obj_class, _obj_type, _access, _flags)
#define __UVERBS_ATTR_OBJ(_id, _obj_class, _obj_type, _access, ...)	\
	___UVERBS_ATTR_OBJ(_id, _obj_class, _obj_type, _access,		\
			   ##__VA_ARGS__, 1, 0)
#define UVERBS_ATTR_IDR(_id, _idr_type, _access, ...)			 \
	__UVERBS_ATTR_OBJ(_id, UVERBS_ATTR_TYPE_IDR, _idr_type, _access,\
			  ##__VA_ARGS__)
#define UVERBS_ATTR_FD(_id, _fd_type, _access, ...)			\
	__UVERBS_ATTR_OBJ(_id, UVERBS_ATTR_TYPE_FD, _fd_type,		\
			  (_access) + BUILD_BUG_ON_ZERO(		\
				(_access) != UVERBS_ACCESS_NEW &&	\
				(_access) != UVERBS_ACCESS_READ),	\
			  ##__VA_ARGS__)
#define DECLARE_UVERBS_ATTR_SPEC(_name, ...)				\
	const struct uverbs_attr_def _name = __VA_ARGS__

#define _UVERBS_METHOD_ATTRS_SZ(...)					\
	(sizeof((const struct uverbs_attr_def * const []){__VA_ARGS__}) /\
	 sizeof(const struct uverbs_attr_def *))
#define _UVERBS_METHOD(_id, _handler, _flags, ...)			\
	((const struct uverbs_method_def) {				\
	 .id = _id,							\
	 .flags = _flags,						\
	 .handler = _handler,						\
	 .num_attrs = _UVERBS_METHOD_ATTRS_SZ(__VA_ARGS__),		\
	 .attrs = &(const struct uverbs_attr_def * const []){__VA_ARGS__} })
#define DECLARE_UVERBS_METHOD(_name, _id, _handler, ...)		\
	const struct uverbs_method_def _name =				\
		_UVERBS_METHOD(_id, _handler, 0, ##__VA_ARGS__)
#define DECLARE_UVERBS_CTX_METHOD(_name, _id, _handler, _flags, ...)	\
	const struct uverbs_method_def _name =				\
		_UVERBS_METHOD(_id, _handler,				\
			       UVERBS_ACTION_FLAG_CREATE_ROOT,		\
			       ##__VA_ARGS__)
#define _UVERBS_OBJECT_METHODS_SZ(...)					\
	(sizeof((const struct uverbs_method_def * const []){__VA_ARGS__}) / \
	 sizeof(const struct uverbs_method_def *))
#define _UVERBS_OBJECT(_id, _type_attrs, ...)				\
	((const struct uverbs_object_def) {				\
	 .id = _id,							\
	 .type_attrs = _type_attrs,					\
	 .num_methods = _UVERBS_OBJECT_METHODS_SZ(__VA_ARGS__),		\
	 .methods = &(const struct uverbs_method_def * const []){__VA_ARGS__} })
#define DECLARE_UVERBS_OBJECT(_name, _id, _type_attrs, ...)		\
	const struct uverbs_object_def _name =				\
		_UVERBS_OBJECT(_id, _type_attrs, ##__VA_ARGS__)
#define _UVERBS_TREE_OBJECTS_SZ(...)					\
	(sizeof((const struct uverbs_object_def * const []){__VA_ARGS__}) / \
	 sizeof(const struct uverbs_object_def *))
#define _UVERBS_OBJECT_TREE(...)					\
	((const struct uverbs_object_tree_def) {			\
	 .num_objects = _UVERBS_TREE_OBJECTS_SZ(__VA_ARGS__),		\
	 .objects = &(const struct uverbs_object_def * const []){__VA_ARGS__} })
#define DECLARE_UVERBS_OBJECT_TREE(_name, ...)				\
	const struct uverbs_object_tree_def _name =			\
		_UVERBS_OBJECT_TREE(__VA_ARGS__)



struct uverbs_ptr_attr {
	union {
		u64		data;
		void	__user *ptr;
	};
	u16		len;
	
	u16		flags;
};

struct uverbs_obj_attr {
	
	const struct uverbs_obj_type	*type;
	struct ib_uobject		*uobject;
	
	int				id;
};

struct uverbs_attr {
	
	struct ib_uverbs_attr __user	*uattr;
	union {
		struct uverbs_ptr_attr	ptr_attr;
		struct uverbs_obj_attr	obj_attr;
	};
};

struct uverbs_attr_bundle_hash {
	
	unsigned long *valid_bitmap;
	size_t num_attrs;
	
	struct uverbs_attr *attrs;
};

struct uverbs_attr_bundle {
	size_t				num_buckets;
	struct uverbs_attr_bundle_hash  hash[];
};

static inline bool uverbs_attr_is_valid_in_hash(const struct uverbs_attr_bundle_hash *attrs_hash,
						unsigned int idx)
{
	return test_bit(idx, attrs_hash->valid_bitmap);
}

static inline bool uverbs_attr_is_valid(const struct uverbs_attr_bundle *attrs_bundle,
					unsigned int idx)
{
	u16 idx_bucket = idx >>	UVERBS_ID_NS_SHIFT;

	if (attrs_bundle->num_buckets <= idx_bucket)
		return false;

	return uverbs_attr_is_valid_in_hash(&attrs_bundle->hash[idx_bucket],
					    idx & ~UVERBS_ID_NS_MASK);
}

static inline const struct uverbs_attr *uverbs_attr_get(const struct uverbs_attr_bundle *attrs_bundle,
							u16 idx)
{
	u16 idx_bucket = idx >>	UVERBS_ID_NS_SHIFT;

	if (!uverbs_attr_is_valid(attrs_bundle, idx))
		return ERR_PTR(-ENOENT);

	return &attrs_bundle->hash[idx_bucket].attrs[idx & ~UVERBS_ID_NS_MASK];
}

static inline int uverbs_copy_to(const struct uverbs_attr_bundle *attrs_bundle,
				 size_t idx, const void *from)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);
	u16 flags;

	if (IS_ERR(attr))
		return PTR_ERR(attr);

	flags = attr->ptr_attr.flags | UVERBS_ATTR_F_VALID_OUTPUT;
	return (!copy_to_user(attr->ptr_attr.ptr, from, attr->ptr_attr.len) &&
		!put_user(flags, &attr->uattr->flags)) ? 0 : -EFAULT;
}

static inline int _uverbs_copy_from(void *to, size_t to_size,
				    const struct uverbs_attr_bundle *attrs_bundle,
				    size_t idx)
{
	const struct uverbs_attr *attr = uverbs_attr_get(attrs_bundle, idx);

	if (IS_ERR(attr))
		return PTR_ERR(attr);

	if (to_size <= sizeof(((struct ib_uverbs_attr *)0)->data))
		memcpy(to, &attr->ptr_attr.data, attr->ptr_attr.len);
	else if (copy_from_user(to, attr->ptr_attr.ptr, attr->ptr_attr.len))
		return -EFAULT;

	return 0;
}

#define uverbs_copy_from(to, attrs_bundle, idx)				      \
	_uverbs_copy_from(to, sizeof(*(to)), attrs_bundle, idx)




#if IS_ENABLED(CONFIG_INFINIBAND_USER_ACCESS)
struct uverbs_root_spec *uverbs_alloc_spec_tree(unsigned int num_trees,
						const struct uverbs_object_tree_def **trees);
void uverbs_free_spec_tree(struct uverbs_root_spec *root);
#else
static inline struct uverbs_root_spec *uverbs_alloc_spec_tree(unsigned int num_trees,
							      const struct uverbs_object_tree_def **trees)
{
	return NULL;
}

static inline void uverbs_free_spec_tree(struct uverbs_root_spec *root)
{
}
#endif

#endif
