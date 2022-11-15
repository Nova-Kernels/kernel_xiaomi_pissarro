

#ifndef __XEN_PUBLIC_IO_DISPLIF_H__
#define __XEN_PUBLIC_IO_DISPLIF_H__

#include "ring.h"
#include "../grant_table.h"


#define XENDISPL_PROTOCOL_VERSION	"1"





#define XENDISPL_OP_DBUF_CREATE		0x10
#define XENDISPL_OP_DBUF_DESTROY	0x11
#define XENDISPL_OP_FB_ATTACH		0x12
#define XENDISPL_OP_FB_DETACH		0x13
#define XENDISPL_OP_SET_CONFIG		0x14
#define XENDISPL_OP_PG_FLIP		0x15


#define XENDISPL_EVT_PG_FLIP		0x00


#define XENDISPL_DRIVER_NAME		"vdispl"

#define XENDISPL_LIST_SEPARATOR		","
#define XENDISPL_RESOLUTION_SEPARATOR	"x"

#define XENDISPL_FIELD_BE_VERSIONS	"versions"
#define XENDISPL_FIELD_FE_VERSION	"version"
#define XENDISPL_FIELD_REQ_RING_REF	"req-ring-ref"
#define XENDISPL_FIELD_REQ_CHANNEL	"req-event-channel"
#define XENDISPL_FIELD_EVT_RING_REF	"evt-ring-ref"
#define XENDISPL_FIELD_EVT_CHANNEL	"evt-event-channel"
#define XENDISPL_FIELD_RESOLUTION	"resolution"
#define XENDISPL_FIELD_BE_ALLOC		"be-alloc"



#define XENDISPL_DBUF_FLG_REQ_ALLOC	(1 << 0)

struct xendispl_dbuf_create_req {
	uint64_t dbuf_cookie;
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
	uint32_t buffer_sz;
	uint32_t flags;
	grant_ref_t gref_directory;
};



struct xendispl_page_directory {
	grant_ref_t gref_dir_next_page;
	grant_ref_t gref[1]; 
};



struct xendispl_dbuf_destroy_req {
	uint64_t dbuf_cookie;
};



struct xendispl_fb_attach_req {
	uint64_t dbuf_cookie;
	uint64_t fb_cookie;
	uint32_t width;
	uint32_t height;
	uint32_t pixel_format;
};



struct xendispl_fb_detach_req {
	uint64_t fb_cookie;
};



struct xendispl_set_config_req {
	uint64_t fb_cookie;
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
};



struct xendispl_page_flip_req {
	uint64_t fb_cookie;
};



struct xendispl_pg_flip_evt {
	uint64_t fb_cookie;
};

struct xendispl_req {
	uint16_t id;
	uint8_t operation;
	uint8_t reserved[5];
	union {
		struct xendispl_dbuf_create_req dbuf_create;
		struct xendispl_dbuf_destroy_req dbuf_destroy;
		struct xendispl_fb_attach_req fb_attach;
		struct xendispl_fb_detach_req fb_detach;
		struct xendispl_set_config_req set_config;
		struct xendispl_page_flip_req pg_flip;
		uint8_t reserved[56];
	} op;
};

struct xendispl_resp {
	uint16_t id;
	uint8_t operation;
	uint8_t reserved;
	int32_t status;
	uint8_t reserved1[56];
};

struct xendispl_evt {
	uint16_t id;
	uint8_t type;
	uint8_t reserved[5];
	union {
		struct xendispl_pg_flip_evt pg_flip;
		uint8_t reserved[56];
	} op;
};

DEFINE_RING_TYPES(xen_displif, struct xendispl_req, struct xendispl_resp);



struct xendispl_event_page {
	uint32_t in_cons;
	uint32_t in_prod;
	uint8_t reserved[56];
};

#define XENDISPL_EVENT_PAGE_SIZE XEN_PAGE_SIZE
#define XENDISPL_IN_RING_OFFS (sizeof(struct xendispl_event_page))
#define XENDISPL_IN_RING_SIZE (XENDISPL_EVENT_PAGE_SIZE - XENDISPL_IN_RING_OFFS)
#define XENDISPL_IN_RING_LEN (XENDISPL_IN_RING_SIZE / sizeof(struct xendispl_evt))
#define XENDISPL_IN_RING(page) \
	((struct xendispl_evt *)((char *)(page) + XENDISPL_IN_RING_OFFS))
#define XENDISPL_IN_RING_REF(page, idx) \
	(XENDISPL_IN_RING((page))[(idx) % XENDISPL_IN_RING_LEN])

#endif 
