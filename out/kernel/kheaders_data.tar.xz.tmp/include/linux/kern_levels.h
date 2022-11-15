/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __KERN_LEVELS_H__
#define __KERN_LEVELS_H__

#define KERN_SOH	"\001"		
#define KERN_SOH_ASCII	'\001'

#define KERN_EMERG	KERN_SOH "0"	
#define KERN_ALERT	KERN_SOH "1"	
#define KERN_CRIT	KERN_SOH "2"	
#define KERN_ERR	KERN_SOH "3"	
#define KERN_WARNING	KERN_SOH "4"	
#define KERN_NOTICE	KERN_SOH "5"	
#define KERN_INFO	KERN_SOH "6"	
#define KERN_DEBUG	KERN_SOH "7"	

#define KERN_DEFAULT	KERN_SOH "d"	


#define KERN_CONT	KERN_SOH "c"


#define LOGLEVEL_SCHED		-2	
#define LOGLEVEL_DEFAULT	-1	
#define LOGLEVEL_EMERG		0	
#define LOGLEVEL_ALERT		1	
#define LOGLEVEL_CRIT		2	
#define LOGLEVEL_ERR		3	
#define LOGLEVEL_WARNING	4	
#define LOGLEVEL_NOTICE		5	
#define LOGLEVEL_INFO		6	
#define LOGLEVEL_DEBUG		7	

#endif
