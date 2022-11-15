/* SPDX-License-Identifier: GPL-2.0 */
#ifndef CEPH_CRUSH_MAPPER_H
#define CEPH_CRUSH_MAPPER_H



#include "crush.h"

extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
int crush_do_rule(const struct crush_map *map,
		  int ruleno, int x, int *result, int result_max,
		  const __u32 *weight, int weight_max,
		  void *cwin, const struct crush_choose_arg *choose_args);


static inline size_t crush_work_size(const struct crush_map *map,
				     int result_max)
{
	return map->working_size + result_max * 3 * sizeof(__u32);
}

void crush_init_workspace(const struct crush_map *map, void *v);

#endif
