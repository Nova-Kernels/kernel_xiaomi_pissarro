#ifndef __LINUX_ERSPAN_H
#define __LINUX_ERSPAN_H



#define ERSPAN_VERSION	0x1

#define VER_MASK	0xf000
#define VLAN_MASK	0x0fff
#define COS_MASK	0xe000
#define EN_MASK		0x1800
#define T_MASK		0x0400
#define ID_MASK		0x03ff
#define INDEX_MASK	0xfffff

enum erspan_encap_type {
	ERSPAN_ENCAP_NOVLAN = 0x0,	
	ERSPAN_ENCAP_ISL = 0x1,		
	ERSPAN_ENCAP_8021Q = 0x2,	
	ERSPAN_ENCAP_INFRAME = 0x3,	
};

struct erspan_metadata {
	__be32 index;   
};

struct erspanhdr {
	__be16 ver_vlan;
#define VER_OFFSET  12
	__be16 session_id;
#define COS_OFFSET  13
#define EN_OFFSET   11
#define T_OFFSET    10
	struct erspan_metadata md;
};

#endif
