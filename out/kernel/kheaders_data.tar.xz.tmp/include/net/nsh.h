#ifndef __NET_NSH_H
#define __NET_NSH_H 1

#include <linux/skbuff.h>




struct nsh_md1_ctx {
	__be32 context[4];
};

struct nsh_md2_tlv {
	__be16 md_class;
	u8 type;
	u8 length;
	u8 md_value[];
};

struct nshhdr {
	__be16 ver_flags_ttl_len;
	u8 mdtype;
	u8 np;
	__be32 path_hdr;
	union {
	    struct nsh_md1_ctx md1;
	    struct nsh_md2_tlv md2;
	};
};


#define NSH_VER_MASK       0xc000
#define NSH_VER_SHIFT      14
#define NSH_FLAGS_MASK     0x3000
#define NSH_FLAGS_SHIFT    12
#define NSH_TTL_MASK       0x0fc0
#define NSH_TTL_SHIFT      6
#define NSH_LEN_MASK       0x003f
#define NSH_LEN_SHIFT      0

#define NSH_MDTYPE_MASK    0x0f
#define NSH_MDTYPE_SHIFT   0

#define NSH_SPI_MASK       0xffffff00
#define NSH_SPI_SHIFT      8
#define NSH_SI_MASK        0x000000ff
#define NSH_SI_SHIFT       0


#define NSH_M_TYPE1     0x01
#define NSH_M_TYPE2     0x02
#define NSH_M_EXP1      0xFE
#define NSH_M_EXP2      0xFF


#define NSH_BASE_HDR_LEN  8


#define NSH_M_TYPE1_LEN   24


#define NSH_HDR_MAX_LEN 256


#define NSH_CTX_HDRS_MAX_LEN 248

static inline struct nshhdr *nsh_hdr(struct sk_buff *skb)
{
	return (struct nshhdr *)skb_network_header(skb);
}

static inline u16 nsh_hdr_len(const struct nshhdr *nsh)
{
	return ((ntohs(nsh->ver_flags_ttl_len) & NSH_LEN_MASK)
		>> NSH_LEN_SHIFT) << 2;
}

static inline u8 nsh_get_ver(const struct nshhdr *nsh)
{
	return (ntohs(nsh->ver_flags_ttl_len) & NSH_VER_MASK)
		>> NSH_VER_SHIFT;
}

static inline u8 nsh_get_flags(const struct nshhdr *nsh)
{
	return (ntohs(nsh->ver_flags_ttl_len) & NSH_FLAGS_MASK)
		>> NSH_FLAGS_SHIFT;
}

static inline u8 nsh_get_ttl(const struct nshhdr *nsh)
{
	return (ntohs(nsh->ver_flags_ttl_len) & NSH_TTL_MASK)
		>> NSH_TTL_SHIFT;
}

static inline void __nsh_set_xflag(struct nshhdr *nsh, u16 xflag, u16 xmask)
{
	nsh->ver_flags_ttl_len
		= (nsh->ver_flags_ttl_len & ~htons(xmask)) | htons(xflag);
}

static inline void nsh_set_flags_and_ttl(struct nshhdr *nsh, u8 flags, u8 ttl)
{
	__nsh_set_xflag(nsh, ((flags << NSH_FLAGS_SHIFT) & NSH_FLAGS_MASK) |
			     ((ttl << NSH_TTL_SHIFT) & NSH_TTL_MASK),
			NSH_FLAGS_MASK | NSH_TTL_MASK);
}

static inline void nsh_set_flags_ttl_len(struct nshhdr *nsh, u8 flags,
					 u8 ttl, u8 len)
{
	len = len >> 2;
	__nsh_set_xflag(nsh, ((flags << NSH_FLAGS_SHIFT) & NSH_FLAGS_MASK) |
			     ((ttl << NSH_TTL_SHIFT) & NSH_TTL_MASK) |
			     ((len << NSH_LEN_SHIFT) & NSH_LEN_MASK),
			NSH_FLAGS_MASK | NSH_TTL_MASK | NSH_LEN_MASK);
}

#endif 
