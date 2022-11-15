

#ifndef __XEN_PUBLIC_ARCH_X86_HVM_START_INFO_H__
#define __XEN_PUBLIC_ARCH_X86_HVM_START_INFO_H__


#define XEN_HVM_START_MAGIC_VALUE 0x336ec578


struct hvm_start_info {
    uint32_t magic;             
                                
    uint32_t version;           
    uint32_t flags;             
    uint32_t nr_modules;        
    uint64_t modlist_paddr;     
                                
    uint64_t cmdline_paddr;     
    uint64_t rsdp_paddr;        
                                
};

struct hvm_modlist_entry {
    uint64_t paddr;             
    uint64_t size;              
    uint64_t cmdline_paddr;     
    uint64_t reserved;
};

#endif 
