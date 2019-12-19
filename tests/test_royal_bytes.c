#include "unit_test_helpers.h"
#include "royal_bytes.h"

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

static void test_ByteBuf_pack(void)
{
	Royal_ByteBuf buff;
	const char* mes = "foo";
	unsigned sizer = 554;
	unsigned outs = 0;
	Royal_ByteBuf_init(&buff, NULL);
	CHECK(Royal_ByteBuf_pack(&buff, "us", sizer, mes));
	CHECK(Royal_ByteBuf_read(&buff, 0, &outs, sizeof(outs)));
	CHECK(outs == 554);
	Royal_ByteBuf_deinit(&buff);
}

int main(int argc, char const *argv[])
{
	test_ByteBuf_init();
	test_ByteBuf_deinit();
	test_ByteBuf_append();
	test_ByteBuf_append2();
	test_ByteBuf_read();
	test_ByteBuf_pack();
	RETURN_FAILURES
}