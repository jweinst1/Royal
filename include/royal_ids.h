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

#endif // ROYAL_IDS_H