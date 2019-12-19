#include "royal_bytes.h"

#ifndef ROYAL_BYTEBUF_DEFAULT_CAP
#define ROYAL_BYTEBUF_DEFAULT_CAP 100
#endif

#ifndef ROYAL_BYTEBUF_GROW_PAD
#define ROYAL_BYTEBUF_GROW_PAD 2
#endif

union _packing_station {
	double _dbl;
	unsigned _size;
	unsigned char _uch;
	const char* _strn;
};

void Royal_ByteBuf_init(Royal_ByteBuf* buf, size_t* capac)
{
	size_t capacity = (capac == NULL ? ROYAL_BYTEBUF_DEFAULT_CAP : *capac) * sizeof(unsigned char);
	_Royal_alloc(buf->data, capacity);
	buf->cap = capacity;
	buf->len = 0;
}

void Royal_ByteBuf_grow(Royal_ByteBuf* buf, size_t new_size)
{
	_Royal_assert(new_size > buf->cap);
	buf->cap = new_size;
	_Royal_realloc(buf->data, buf->data, new_size);
}

void* Royal_ByteBuf_append(Royal_ByteBuf* buf, size_t size, const void* obj)
{
	void* ret;
	if(size > ROYAL_BYTEBUF_SPACE(buf)) {
		buf->cap += size + ROYAL_BYTEBUF_GROW_PAD * buf->cap;
		_Royal_realloc(buf->data, buf->data, buf->cap);
	}
	ret = buf->data + buf->len;
	memcpy(ret, obj, size);
	buf->len += size;
	return ret;
}

int Royal_ByteBuf_read(Royal_ByteBuf* buf, size_t offset, void* dest, size_t dsize)
{
	if (buf->len <= (offset + dsize)) // can't read past the length
		return 0;
	else {
		memcpy(dest, buf->data + offset, dsize);
		return 1;
	}
}

int Royal_ByteBuf_pack(Royal_ByteBuf* buf, const char* fmt, ...)
{
	va_list to_pack;
	unsigned str_size;
	union _packing_station packer;
	va_start(to_pack, fmt);
	while(*fmt) {
		switch(*fmt++) {
			case 'b':
			   packer._uch = va_arg(to_pack, unsigned char);
			   Royal_ByteBuf_append(buf, sizeof(unsigned char), &(packer._uch));
			   break;
			case 's':
			   packer._strn = va_arg(to_pack, const char*);
			   str_size = (unsigned)(strlen(packer._strn) + 1);
			   Royal_ByteBuf_append(buf, sizeof(unsigned), &str_size);
			   Royal_ByteBuf_append(buf, str_size, packer._strn);
			   break;
			case 'd':
			   packer._dbl = va_arg(to_pack, double);
			   Royal_ByteBuf_append(buf, sizeof(double), &(packer._dbl));
			   break;
			case 'u':
			   packer._size = va_arg(to_pack, unsigned);
			   Royal_ByteBuf_append(buf, sizeof(unsigned), &(packer._size));
			   break;
			default:
			   // unknown format character.
			   va_end(to_pack);
			   return 0;
		}
	}
	va_end(to_pack);
	return 1;
}

void Royal_ByteBuf_deinit(Royal_ByteBuf* buf)
{
	free(buf->data);
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}