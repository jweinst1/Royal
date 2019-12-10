#ifndef ROYAL_IDS_H
#define ROYAL_IDS_H

#include "royal_mem.h"

#ifndef ROYAL_ID_NAME_SIZE
#define ROYAL_ID_NAME_SIZE 25
#endif // ROYAL_ID_NAME_SIZE

/**
 * This id type acts as a way to map and identify objects
 * with just an array-like structure. Access is primarily done
 * through a unique integer, so a hashmap is not optimal here.
 */
typedef size_t Royal_Id;

typedef struct {
	Royal_Id id;
	char name[ROYAL_ID_NAME_SIZE + 1];
} Royal_IdInfo;

typedef struct {
	Royal_IdInfo* ids;
	Royal_Id len;
	Royal_Id cap;
} Royal_IdMap;

void Royal_IdMap_init(Royal_IdMap* map, Royal_Id* capac);

void Royal_IdMap_deinit(Royal_IdMap* map);

const Royal_Id* Royal_IdMap_find(Royal_IdMap* map, const char* id_name);

void Royal_IdMap_grow(Royal_IdMap* map, Royal_Id new_cap);

const Royal_Id* Royal_IdMap_add(Royal_IdMap* map, const char* id_name);

const char* Royal_IdMap_get(const Royal_IdMap* map, Royal_Id id);

typedef struct {
	void* data;
	Royal_Id len;
	Royal_Id cap;
	size_t type_size;
} Royal_IdBuf;

#define ROYAL_IDBUF_GET(buf, idx) ((void*)(((char*)buf->data) + idx * buf->item_size))

void Royal_IdBuf_init(Royal_IdBuf* buf, size_t type_size, Royal_Id* capacity);

#define ROYAL_IDBUF_INIT(buf, objtype) Royal_IdBuf_init(buf, sizeof(objtype), NULL)

void Royal_IdBuf_deinit(Royal_IdBuf* buf);

void Royal_IdBuf_grow(Royal_IdBuf* buf, size_t new_size);

void* Royal_IdBuf_get(Royal_IdBuf* buf, Royal_Id idx);

#define ROYAL_IDBUF_RESERVE(buf, nmemb) Royal_IdBuf_grow(buf, nmemb * buf->type_size)

void Royal_IdBuf_put(Royal_IdBuf* buf, Royal_Id idx, void* ptr);

#endif // ROYAL_IDS_H