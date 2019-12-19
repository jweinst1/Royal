#ifndef ROYAL_BYTES_H
#define ROYAL_BYTES_H

#include <stdarg.h>
#include "royal_mem.h"

typedef struct {
	unsigned char* data;
	size_t len;
	size_t cap;
} Royal_ByteBuf;

void Royal_ByteBuf_init(Royal_ByteBuf* buf, size_t* capac);

void Royal_ByteBuf_grow(Royal_ByteBuf* buf, size_t new_size);

void* Royal_ByteBuf_append(Royal_ByteBuf* buf, size_t size, const void* obj);

int Royal_ByteBuf_read(Royal_ByteBuf* buf, size_t offset, void* dest, size_t dsize);

int Royal_ByteBuf_pack(Royal_ByteBuf* buf, const char* fmt, ...);

void Royal_ByteBuf_deinit(Royal_ByteBuf* buf);

#define ROYAL_BYTEBUF_CLR(buf) (buf->len = 0)

#define ROYAL_BYTEBUF_SPACE(buf) (buf->cap - buf->len)
// Convenience macros
#define ROYAL_BYTEBUF_APPEND(buf, obj) Royal_ByteBuf_append(buf, sizeof(obj), &(obj))
#define ROYAL_BYTEBUF_READ(buf, off, obj) (Royal_ByteBuf_read(buf, off, &(obj), sizeof(obj)))

#endif // ROYAL_BYTES_H