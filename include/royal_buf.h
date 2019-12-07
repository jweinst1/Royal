#ifndef ROYAL_BUF_H
#define ROYAL_BUF_H

#include "royal_mem.h"


#define ROYAL_BUF_DECLARATIONS(type) \
                typedef struct { \
                	size_t len; \
                	size_t cap; \
                	type* data; \
                } Royal_#type#Buf; \
                                    \
    void Royal_#type#Buf_init(Royal_#type#Buf* buf)

#endif // ROYAL_BUF_H