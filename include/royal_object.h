#ifndef ROYAL_OBJECT_H
#define ROYAL_OBJECT_H

#include "royal_ids.h"

typedef enum {
	ROYAL_OBJ_TYPE_M_CUSTOMER
} Royal_ObjectType;

#define ROYAL_OBJECT_HEAD Royal_ObjectType type

typedef struct {
	ROYAL_OBJECT_HEAD;
} Royal_Object;

#define ROYAL_LINKOBJECT_HEAD \
                     ROYAL_OBJECT_HEAD; \
                     Royal_Object* next
typedef struct {
	ROYAL_LINKOBJECT_HEAD;
} Royal_LinkObject;

#endif // ROYAL_OBJECT_H
