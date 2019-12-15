#include "unit_test_helpers.h"
#include "royal_ids.h"



struct _Test_Type {
	int a;
	char msg[3];
};

static void __Royal_IdBuf_debug(Royal_IdBuf* buf)
{
	printf("RoyalIdBuf DEBUG: @(%p) cap(%lu), type_size(%lu), data(%p)\n",
	                    buf, (unsigned long)buf->cap, (unsigned long)buf->type_size
	                    , buf->data);
}


static void test_Royal_IdBuf_init(void)
{
	Royal_IdBuf test_buf;
	ROYAL_IDBUF_INIT(&test_buf, struct _Test_Type);
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
	CHECK(test_buf.cap == bufcap);
	ptr = Royal_IdBuf_get(&test_buf, tidx);
	CHECK(ptr != NULL);
	CHECK(ptr == ((struct _Test_Type*)(test_buf.data)) + tidx);
	Royal_IdBuf_deinit(&test_buf);
}

static void test_Royal_IdBuf_put(void)
{
	Royal_IdBuf test_buf;
	Royal_Id bufcap = 10;
	struct _Test_Type memb;
	struct _Test_Type* ptr;
	Royal_IdBuf_init(&test_buf, sizeof(struct _Test_Type), &bufcap);
	memb.a = 77;
	memb.msg[0] = '3';
	Royal_IdBuf_put(&test_buf, 1, &memb);
	ptr = Royal_IdBuf_get(&test_buf, 1);
	CHECK(ptr->a == 77);
	CHECK(ptr->msg[0] == '3');
	Royal_IdBuf_deinit(&test_buf);
}

static void test_Royal_IdBuf_put2(void)
{
	Royal_IdBuf test_buf;
	Royal_Id bufcap = 10;
	struct _Test_Type memb;
	struct _Test_Type* ptr;
	Royal_IdBuf_init(&test_buf, sizeof(struct _Test_Type), &bufcap);
	memb.a = 77;
	memb.msg[0] = '3';
	Royal_IdBuf_put(&test_buf, 15, &memb);
	__Royal_IdBuf_debug(&test_buf);
	ptr = Royal_IdBuf_get(&test_buf, 15);
	CHECK(ptr != NULL);
	CHECK(ptr->a == 77);
	CHECK(ptr->msg[0] == '3');
	Royal_IdBuf_deinit(&test_buf);
}

struct test_slot_t {
	long a;
	long b;
};

static void test_Royal_IdBuf_ret(void)
{
	Royal_IdBuf test_buf;
	struct test_slot_t* ptr;
	struct test_slot_t* got;
	Royal_Id bufcap = 10;
	Royal_IdBuf_init(&test_buf, sizeof(struct test_slot_t), &bufcap);
	ptr = Royal_IdBuf_ret(&test_buf, 14);
	CHECK(test_buf.cap > bufcap);
	CHECK(ptr > (struct test_slot_t*)(test_buf.data));
	ptr->a = 88;
	ptr->b = 44;
	got = Royal_IdBuf_get(&test_buf, 14);
	CHECK(got->a == 88);
	CHECK(got->b == 44);
	Royal_IdBuf_deinit(&test_buf);
}



int main(int argc, char const *argv[])
{
	test_Royal_IdBuf_init();
	test_Royal_IdBuf_get();
	test_Royal_IdBuf_put();
	test_Royal_IdBuf_put2();
	test_Royal_IdBuf_ret();
	RETURN_FAILURES
}