

#ifndef _TLS_OFFLOAD_H
#define _TLS_OFFLOAD_H

#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <net/tcp.h>

#include <uapi/linux/tls.h>



#define TLS_MAX_PAYLOAD_SIZE		((size_t)1 << 14)

#define TLS_HEADER_SIZE			5
#define TLS_NONCE_OFFSET		TLS_HEADER_SIZE

#define TLS_CRYPTO_INFO_READY(info)	((info)->cipher_type)

#define TLS_RECORD_TYPE_DATA		0x17

#define TLS_AAD_SPACE_SIZE		13

struct tls_sw_context {
	struct crypto_aead *aead_send;

	
	char aad_space[TLS_AAD_SPACE_SIZE];

	unsigned int sg_plaintext_size;
	int sg_plaintext_num_elem;
	struct scatterlist sg_plaintext_data[MAX_SKB_FRAGS];

	unsigned int sg_encrypted_size;
	int sg_encrypted_num_elem;
	struct scatterlist sg_encrypted_data[MAX_SKB_FRAGS];

	
	struct scatterlist sg_aead_in[2];
	
	struct scatterlist sg_aead_out[2];
};

enum {
	TLS_PENDING_CLOSED_RECORD
};

union tls_crypto_context {
	struct tls_crypto_info info;
	struct tls12_crypto_info_aes_gcm_128 aes_gcm_128;
};

struct tls_context {
	union tls_crypto_context crypto_send;

	void *priv_ctx;

	u8 tx_conf:2;

	u16 prepend_size;
	u16 tag_size;
	u16 overhead_size;
	u16 iv_size;
	char *iv;
	u16 rec_seq_size;
	char *rec_seq;

	struct scatterlist *partially_sent_record;
	u16 partially_sent_offset;
	unsigned long flags;
	bool in_tcp_sendpages;

	u16 pending_open_record_frags;
	int (*push_pending_record)(struct sock *sk, int flags);

	void (*sk_write_space)(struct sock *sk);
	void (*sk_proto_close)(struct sock *sk, long timeout);

	int  (*setsockopt)(struct sock *sk, int level,
			   int optname, char __user *optval,
			   unsigned int optlen);
	int  (*getsockopt)(struct sock *sk, int level,
			   int optname, char __user *optval,
			   int __user *optlen);
};

int wait_on_pending_writer(struct sock *sk, long *timeo);
int tls_sk_query(struct sock *sk, int optname, char __user *optval,
		int __user *optlen);
int tls_sk_attach(struct sock *sk, int optname, char __user *optval,
		  unsigned int optlen);


int tls_set_sw_offload(struct sock *sk, struct tls_context *ctx);
int tls_sw_sendmsg(struct sock *sk, struct msghdr *msg, size_t size);
int tls_sw_sendpage(struct sock *sk, struct page *page,
		    int offset, size_t size, int flags);
void tls_sw_close(struct sock *sk, long timeout);
void tls_sw_free_tx_resources(struct sock *sk);

void tls_sk_destruct(struct sock *sk, struct tls_context *ctx);
void tls_icsk_clean_acked(struct sock *sk);

int tls_push_sg(struct sock *sk, struct tls_context *ctx,
		struct scatterlist *sg, u16 first_offset,
		int flags);
int tls_push_pending_closed_record(struct sock *sk, struct tls_context *ctx,
				   int flags, long *timeo);

static inline bool tls_is_pending_closed_record(struct tls_context *ctx)
{
	return test_bit(TLS_PENDING_CLOSED_RECORD, &ctx->flags);
}

static inline int tls_complete_pending_work(struct sock *sk,
					    struct tls_context *ctx,
					    int flags, long *timeo)
{
	int rc = 0;

	if (unlikely(sk->sk_write_pending))
		rc = wait_on_pending_writer(sk, timeo);

	if (!rc && tls_is_pending_closed_record(ctx))
		rc = tls_push_pending_closed_record(sk, ctx, flags, timeo);

	return rc;
}

static inline bool tls_is_partially_sent_record(struct tls_context *ctx)
{
	return !!ctx->partially_sent_record;
}

static inline bool tls_is_pending_open_record(struct tls_context *tls_ctx)
{
	return tls_ctx->pending_open_record_frags;
}

static inline void tls_err_abort(struct sock *sk)
{
	sk->sk_err = EBADMSG;
	sk->sk_error_report(sk);
}

static inline bool tls_bigint_increment(unsigned char *seq, int len)
{
	int i;

	for (i = len - 1; i >= 0; i--) {
		++seq[i];
		if (seq[i] != 0)
			break;
	}

	return (i == -1);
}

static inline void tls_advance_record_sn(struct sock *sk,
					 struct tls_context *ctx)
{
	if (tls_bigint_increment(ctx->rec_seq, ctx->rec_seq_size))
		tls_err_abort(sk);
	tls_bigint_increment(ctx->iv + TLS_CIPHER_AES_GCM_128_SALT_SIZE,
			     ctx->iv_size);
}

static inline void tls_fill_prepend(struct tls_context *ctx,
			     char *buf,
			     size_t plaintext_len,
			     unsigned char record_type)
{
	size_t pkt_len, iv_size = ctx->iv_size;

	pkt_len = plaintext_len + iv_size + ctx->tag_size;

	
	buf[0] = record_type;
	buf[1] = TLS_VERSION_MINOR(ctx->crypto_send.info.version);
	buf[2] = TLS_VERSION_MAJOR(ctx->crypto_send.info.version);
	
	buf[3] = pkt_len >> 8;
	buf[4] = pkt_len & 0xFF;
	memcpy(buf + TLS_NONCE_OFFSET,
	       ctx->iv + TLS_CIPHER_AES_GCM_128_SALT_SIZE, iv_size);
}

static inline struct tls_context *tls_get_ctx(const struct sock *sk)
{
	struct inet_connection_sock *icsk = inet_csk(sk);

	return icsk->icsk_ulp_data;
}

static inline struct tls_sw_context *tls_sw_ctx(
		const struct tls_context *tls_ctx)
{
	return (struct tls_sw_context *)tls_ctx->priv_ctx;
}

static inline struct tls_offload_context *tls_offload_ctx(
		const struct tls_context *tls_ctx)
{
	return (struct tls_offload_context *)tls_ctx->priv_ctx;
}

int tls_proccess_cmsg(struct sock *sk, struct msghdr *msg,
		      unsigned char *record_type);

#endif 
