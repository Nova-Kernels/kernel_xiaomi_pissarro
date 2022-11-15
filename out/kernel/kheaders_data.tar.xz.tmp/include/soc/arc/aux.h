

#ifndef __SOC_ARC_AUX_H__
#define __SOC_ARC_AUX_H__

#ifdef CONFIG_ARC

#define read_aux_reg(r)		__builtin_arc_lr(r)


#define write_aux_reg(r, v)	__builtin_arc_sr((unsigned int)(v), r)

#else	

static inline int read_aux_reg(u32 r)
{
	return 0;
}


static inline void write_aux_reg(u32 r, u32 v)
{
	;
}

#endif

#define READ_BCR(reg, into)				\
{							\
	unsigned int tmp;				\
	tmp = read_aux_reg(reg);			\
	if (sizeof(tmp) == sizeof(into)) {		\
		into = *((typeof(into) *)&tmp);		\
	} else {					\
		extern void bogus_undefined(void);	\
		bogus_undefined();			\
	}						\
}

#define WRITE_AUX(reg, into)				\
{							\
	unsigned int tmp;				\
	if (sizeof(tmp) == sizeof(into)) {		\
		tmp = (*(unsigned int *)&(into));	\
		write_aux_reg(reg, tmp);		\
	} else  {					\
		extern void bogus_undefined(void);	\
		bogus_undefined();			\
	}						\
}


#endif
