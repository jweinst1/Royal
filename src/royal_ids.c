#include "royal_ids.h"

#ifndef ROYAL_IDMAP_DEFAULT_CAPACITY
#define ROYAL_IDMAP_DEFAULT_CAPACITY 25
#endif

#ifndef ROYAL_IDMAP_GROW_FAC
#define ROYAL_IDMAP_GROW_FAC 2
#endif

#ifndef ROYAL_IDBUF_DEFAULT_CAPACITY
#define ROYAL_IDBUF_DEFAULT_CAPACITY 25
#endif

#ifndef ROYAL_IDBUF_PUT_GROWTH
#define ROYAL_IDBUF_PUT_GROWTH 10
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

const Royal_Id* Royal_IdMap_find(Royal_IdMap* map, const char* id_name)
{
	Royal_Id c;
	for(c = 0; c < map->len; c++) {
		if(0 == strcmp(map->ids[c].name, id_name))
			return &(map->ids[c].id);
	}
	return NULL;
}

void Royal_IdMap_grow(Royal_IdMap* map, Royal_Id new_cap)
{
	_Royal_realloc(map->ids, map->ids, new_cap);
	map->cap = new_cap;
}

const Royal_Id* Royal_IdMap_add(Royal_IdMap* map, const char* id_name)
{
	const Royal_Id* found;
	if(map->cap == map->len)
		Royal_IdMap_grow(map, map->cap * ROYAL_IDMAP_GROW_FAC);
	if((found = Royal_IdMap_find(map, id_name)) != NULL)
		return found;
	strncpy(map->ids[map->len].name, id_name, ROYAL_ID_NAME_SIZE);
	return &(map->ids[map->len++].id);
}

const char* Royal_IdMap_get(const Royal_IdMap* map, Royal_Id id)
{
	return id < map->len ? map->ids[id].name : NULL;
}

void Royal_IdBuf_init(Royal_IdBuf* buf, size_t type_size, Royal_Id* capacity)
{
	buf->type_size = type_size;
	buf->cap = capacity == NULL ? ROYAL_IDBUF_DEFAULT_CAPACITY : *capacity;
	_Royal_alloc(buf->data, (buf->cap * type_size));
}

void Royal_IdBuf_grow(Royal_IdBuf* buf, size_t new_size)
{
	buf->cap = new_size * buf->type_size;
	_Royal_realloc(buf->data, buf->data, new_size);
}

void Royal_IdBuf_deinit(Royal_IdBuf* buf)
{
	_Royal_free(buf->data);
	buf->data = NULL;
	buf->cap = 0;
	buf->type_size = 0;
}

void* Royal_IdBuf_get(Royal_IdBuf* buf, Royal_Id idx)
{
	Royal_Id index = idx * buf->type_size;
	if (index < buf->cap) {
		return ((char*)buf->data) + index;
	} else {
		return NULL;
	}
}

void Royal_IdBuf_put(Royal_IdBuf* buf, Royal_Id idx, void* ptr)
{
	if (idx >= buf->cap)
		// Padding growth if put is out of range.
		Royal_IdBuf_grow(buf, idx + ROYAL_IDBUF_PUT_GROWTH);
	memcpy((char*)buf->data + (idx * buf->type_size), ptr, buf->type_size);
}

