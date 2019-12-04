#include "royal_ids.h"

#ifndef ROYAL_IDMAP_DEFAULT_CAPACITY
#define ROYAL_IDMAP_DEFAULT_CAPACITY 25
#endif

void Royal_IdMap_init(Royal_IdMap* map, Royal_Id* capac)
{
	Royal_Id capacity = capac == NULL ? ROYAL_IDMAP_DEFAULT_CAPACITY : *capac;
	_Royal_alloc(map->ids, capacity);
	map->len = 0;
	map->cap = capacity;
}

void Royal_IdMap_deinit(Royal_IdMap* map)
{
	_Royal_free(map->ids);
	map->ids = NULL;
	map->cap = 0;
	map->len = 0;
}