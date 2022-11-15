

#ifndef OPA_ADDR_H
#define OPA_ADDR_H

#include <rdma/opa_smi.h>

#define	OPA_SPECIAL_OUI		(0x00066AULL)
#define OPA_MAKE_ID(x)          (cpu_to_be64(OPA_SPECIAL_OUI << 40 | (x)))
#define OPA_TO_IB_UCAST_LID(x) (((x) >= be16_to_cpu(IB_MULTICAST_LID_BASE)) \
				? 0 : x)
#define OPA_GID_INDEX		0x1

#define OPA_MCAST_NR 0x4 
#define OPA_COLLECTIVE_NR 0x1 


static inline bool ib_is_opa_gid(const union ib_gid *gid)
{
	return ((be64_to_cpu(gid->global.interface_id) >> 40) ==
		OPA_SPECIAL_OUI);
}


static inline u32 opa_get_lid_from_gid(const union ib_gid *gid)
{
	return be64_to_cpu(gid->global.interface_id) & 0xFFFFFFFF;
}


static inline bool opa_is_extended_lid(u32 dlid, u32 slid)
{
	if ((be32_to_cpu(dlid) >=
	     be16_to_cpu(IB_MULTICAST_LID_BASE)) ||
	    (be32_to_cpu(slid) >=
	     be16_to_cpu(IB_MULTICAST_LID_BASE)))
		return true;
	else
		return false;
}


static inline u32 opa_get_mcast_base(u32 nr_top_bits)
{
	return (be32_to_cpu(OPA_LID_PERMISSIVE) << (32 - nr_top_bits));
}

#endif 
