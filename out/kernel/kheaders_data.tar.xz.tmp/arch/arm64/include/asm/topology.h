/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_TOPOLOGY_H
#define __ASM_TOPOLOGY_H

#include <linux/cpumask.h>

struct cpu_topology {
	int thread_id;
	int core_id;
	int cluster_id;
	cpumask_t thread_sibling;
	cpumask_t core_sibling;
};

extern struct cpu_topology cpu_topology[NR_CPUS];

#define topology_physical_package_id(cpu)	(cpu_topology[cpu].cluster_id)
#define topology_core_id(cpu)		(cpu_topology[cpu].core_id)
#define topology_core_cpumask(cpu)	(&cpu_topology[cpu].core_sibling)
#define topology_sibling_cpumask(cpu)	(&cpu_topology[cpu].thread_sibling)

void init_cpu_topology(void);
void store_cpu_topology(unsigned int cpuid);
const struct cpumask *cpu_coregroup_mask(int cpu);

#ifdef CONFIG_NUMA

struct pci_bus;
int pcibus_to_node(struct pci_bus *bus);
#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))

#endif 

#include <linux/arch_topology.h>


#define arch_scale_freq_capacity topology_get_freq_scale


#define arch_scale_max_freq_capacity topology_get_max_freq_scale


#define arch_scale_cpu_capacity topology_get_cpu_scale


#define arch_update_cpu_topology topology_update_cpu_topology


#define arch_max_cpu_freq topology_get_max_cpu_freq


#define arch_max_freq_scale topology_get_max_freq_scale
#define arch_min_freq_scale topology_get_min_freq_scale


int arch_is_multi_cluster(void);
int arch_is_smp(void);
int arch_get_nr_clusters(void);
int arch_get_cluster_id(unsigned int cpu);
void arch_build_cpu_topology_domain(void);

#include <asm-generic/topology.h>

#endif 
