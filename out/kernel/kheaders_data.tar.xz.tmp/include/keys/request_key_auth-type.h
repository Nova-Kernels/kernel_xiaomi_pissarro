

#ifndef _KEYS_REQUEST_KEY_AUTH_TYPE_H
#define _KEYS_REQUEST_KEY_AUTH_TYPE_H

#include <linux/key.h>


struct request_key_auth {
	struct key		*target_key;
	struct key		*dest_keyring;
	const struct cred	*cred;
	void			*callout_info;
	size_t			callout_len;
	pid_t			pid;
	char			op[8];
} __randomize_layout;

static inline struct request_key_auth *get_request_key_auth(const struct key *key)
{
	return key->payload.data[0];
}


#endif 
