#include "unit_test_helpers.h"
#include "royal_ids.h"



struct _Test_Type {
	int a;
	char msg[3];
};

static void __Royal_IdBuf_debug(Royal_IdBuf* buf)
{
	printf("RoyalIdBuf DEBUG: @(%p) cap(%lu), len(%lu), type_size(%lu), data(%p)\n",
	                    buf, (unsigned long)buf->cap, (unsigned long)buf->len, (unsigned long)buf->type_size
	                    , buf->data);
}


static void test_Royal_IdBuf_init(void)
{
	Royal_IdBuf test_buf;
	ROYAL_IDBUF_INIT(&test_buf, struct _Test_Type);
	CHECK(test_buf.len == 0);
	CHECK(test_buf.type_size == sizeof(struct _Test_Type));
	CHECK(test_buf.data != NULL);
	CHECK(test_buf.cap != 0);
	Royal_IdBuf_deinit(&test_buf);
	CHECK(test_buf.data == NULL);
}

static void test_Royal_IdBuf_get(void)
{
	Royal_IdBuf test_buf;
	Royal_Id tidx = 0;
	Royal_Id bufcap = 10;
	struct _Test_Type* ptr = NULL;
	Royal_IdBuf_init(&test_buf, sizeof(struct _Test_Type), &bufcap);
	__Royal_IdBuf_debug(&test_buf);
	CHECK(test_buf.cap == bufcap);
	test_buf.len++; // manual increase to test only get
	ptr = Royal_IdBuf_get(&test_buf, tidx);
	CHECK(ptr != NULL);
	CHECK(ptr == ((struct _Test_Type*)(test_buf.data)) + tidx);
	printf("DEBUG: ptr(%p)\n", ptr);
	Royal_IdBuf_deinit(&test_buf);
}





int main(int argc, char const *argv[])
{
	test_Royal_IdBuf_init();
	test_Royal_IdBuf_get();
	RETURN_FAILURES
}