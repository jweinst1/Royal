#include "unit_test_helpers.h"
#include "royal_bytes.h"

/*
typedef struct {
	unsigned char* data;
	size_t len;
	size_t cap;
} Royal_ByteBuf;

void Royal_ByteBuf_init(Royal_ByteBuf* buf, size_t* capac);

void Royal_ByteBuf_grow(Royal_ByteBuf* buf, size_t new_size);

void Royal_ByteBuf_append(Royal_ByteBuf* buf, size_t size, void* obj);

int Royal_ByteBuf_read(Royal_ByteBuf* buf, size_t offset, void* dest, size_t dsize);

void Royal_ByteBuf_deinit(Royal_ByteBuf* buf);

#define ROYAL_BYTEBUF_CLR(buf) (buf->len = 0)

#define ROYAL_BYTEBUF_SPACE(buf) (buf->cap - buf->len)
// Convenience macros
#define ROYAL_BYTEBUF_APPEND(buf, obj) Royal_ByteBuf_append(buf, sizeof(obj), &(obj))
#define ROYAL_BYTEBUF_READ(buf, off, obj) (Royal_ByteBuf_read(buf, off, &(obj), sizeof(obj)))
*/

static void test_ByteBuf_init(void)
{
	Royal_ByteBuf buff;
	size_t cap = 30;
	Royal_ByteBuf_init(&buff, &cap);
	CHECK(buff.cap == 30);
	CHECK(buff.len == 0);
	CHECK(buff.data != NULL);
}

static void test_ByteBuf_deinit(void)
{
	Royal_ByteBuf buff;
	size_t cap = 30;
	Royal_ByteBuf_init(&buff, &cap);
	Royal_ByteBuf_deinit(&buff);
	CHECK(buff.data == NULL);
	CHECK(buff.cap == 0);
}

static void test_ByteBuf_append(void)
{
	Royal_ByteBuf buff;
	double* got;
	size_t cap = 30;
	double a = 4.55;
	Royal_ByteBuf_init(&buff, &cap);
	got = Royal_ByteBuf_append(&buff, sizeof(double), &a);
	CHECK(got != NULL);
	CHECK(*got == 4.55);
	CHECK(buff.len == sizeof(double));
	Royal_ByteBuf_deinit(&buff);
}

static void test_ByteBuf_append2(void)
{
	Royal_ByteBuf buff;
	double* gota;
	double* gotb;
	double* gotc;
	size_t cap = 20;
	double a = 4.55;
	double b = 3.55;
	double c = 60.0;
	Royal_ByteBuf_init(&buff, &cap);
	gota = ROYAL_BYTEBUF_APPEND(&buff, a);
	gotb = ROYAL_BYTEBUF_APPEND(&buff, b);
	CHECK(gota != NULL);
	CHECK(*gota == 4.55);
	CHECK(buff.len == (sizeof(double) * 2));
	gotc = ROYAL_BYTEBUF_APPEND(&buff, c);
	CHECK(gotc != NULL);
	CHECK(*gotc == 60.0);
	CHECK(*gotb == 3.55);
	CHECK(buff.cap > cap);
	Royal_ByteBuf_deinit(&buff);
}

static void test_ByteBuf_read(void)
{
	Royal_ByteBuf buff;
	long foo = 0;
	long doo = 55;
	Royal_ByteBuf_init(&buff, NULL);
	CHECK(NULL != ROYAL_BYTEBUF_APPEND(&buff, doo));
	CHECK(NULL != ROYAL_BYTEBUF_APPEND(&buff, doo));
	CHECK(Royal_ByteBuf_read(&buff, 0, &foo, sizeof(foo)));
	CHECK(foo == 55);
	Royal_ByteBuf_deinit(&buff);
}

int main(int argc, char const *argv[])
{
	test_ByteBuf_init();
	test_ByteBuf_deinit();
	test_ByteBuf_append();
	test_ByteBuf_append2();
	test_ByteBuf_read();
	RETURN_FAILURES
}