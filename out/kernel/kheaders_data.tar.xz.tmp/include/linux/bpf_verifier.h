
#ifndef _LINUX_BPF_VERIFIER_H
#define _LINUX_BPF_VERIFIER_H 1

#include <linux/bpf.h> 
#include <linux/filter.h> 
#include <linux/tnum.h>


#define BPF_MAX_VAR_OFF	(1 << 29)

#define BPF_MAX_VAR_SIZ	(1 << 29)


enum bpf_reg_liveness {
	REG_LIVE_NONE = 0, 
	REG_LIVE_READ, 
	REG_LIVE_WRITTEN, 
};

struct bpf_reg_state {
	enum bpf_reg_type type;
	union {
		
		u16 range;

		
		struct bpf_map *map_ptr;

		
		unsigned long raw;
	};
	
	s32 off;
	
	u32 id;
	
	
	struct tnum var_off;
	
	s64 smin_value; 
	s64 smax_value; 
	u64 umin_value; 
	u64 umax_value; 
	
	enum bpf_reg_liveness live;
};

enum bpf_stack_slot_type {
	STACK_INVALID,    
	STACK_SPILL,      
	STACK_MISC	  
};

#define BPF_REG_SIZE 8	

struct bpf_stack_state {
	struct bpf_reg_state spilled_ptr;
	u8 slot_type[BPF_REG_SIZE];
};


struct bpf_verifier_state {
	struct bpf_reg_state regs[MAX_BPF_REG];
	struct bpf_verifier_state *parent;
	int allocated_stack;
	struct bpf_stack_state *stack;
	bool speculative;
};


struct bpf_verifier_state_list {
	struct bpf_verifier_state state;
	struct bpf_verifier_state_list *next;
};


#define BPF_ALU_SANITIZE_SRC		1U
#define BPF_ALU_SANITIZE_DST		2U
#define BPF_ALU_NEG_VALUE		(1U << 2)
#define BPF_ALU_NON_POINTER		(1U << 3)
#define BPF_ALU_SANITIZE		(BPF_ALU_SANITIZE_SRC | \
					 BPF_ALU_SANITIZE_DST)

struct bpf_insn_aux_data {
	union {
		enum bpf_reg_type ptr_type;	
		struct bpf_map *map_ptr;	
		u32 alu_limit;			
	};
	int ctx_field_size; 
	int sanitize_stack_off; 
	bool seen; 
	u8 alu_state; 
};

#define MAX_USED_MAPS 64 

struct bpf_verifier_env;
struct bpf_ext_analyzer_ops {
	int (*insn_hook)(struct bpf_verifier_env *env,
			 int insn_idx, int prev_insn_idx);
};


struct bpf_verifier_env {
	u32 insn_idx;
	u32 prev_insn_idx;
	struct bpf_prog *prog;		
	struct bpf_verifier_stack_elem *head; 
	int stack_size;			
	bool strict_alignment;		
	struct bpf_verifier_state *cur_state; 
	struct bpf_verifier_state_list **explored_states; 
	const struct bpf_ext_analyzer_ops *analyzer_ops; 
	void *analyzer_priv; 
	struct bpf_map *used_maps[MAX_USED_MAPS]; 
	u32 used_map_cnt;		
	u32 id_gen;			
	bool allow_ptr_leaks;
	bool seen_direct_write;
	struct bpf_insn_aux_data *insn_aux_data; 
};

static inline struct bpf_reg_state *cur_regs(struct bpf_verifier_env *env)
{
	return env->cur_state->regs;
}

int bpf_analyzer(struct bpf_prog *prog, const struct bpf_ext_analyzer_ops *ops,
		 void *priv);

#endif 
