#include "unit_test_helpers.h"
#include "royal_graphs.h"

static void test_royal_graph_init(void)
{
	Royal_Graph gr;
	size_t a = 5;
	size_t b = 2;
	CHECK(Royal_Graph_init(&gr, 10, &a, &b));
	CHECK(gr.len == 0);
	CHECK(gr.cap == a);
	CHECK(gr.grow == b);
	CHECK(gr.v_off == 11);
	CHECK(gr.e_off == 22);
	CHECK(gr.n_off == 33);
	Royal_Graph_deinit(&gr);
}

static const char* TEST_OUTGR1 = "a/is/c\na/not/b\nc/like/b\n";

static void test_royal_graph_str(void)
{
	Royal_Graph gr;
	char* outs;
	size_t capt = 10;
	CHECK(Royal_Graph_init(&gr, 6, &capt, NULL));
	CHECK(Royal_Graph_append(&gr, "a", "is", "c") != NULL);
	CHECK(Royal_Graph_append(&gr, "a", "not", "b") != NULL);
	CHECK(Royal_Graph_append(&gr, "c", "like", "b") != NULL);
	CHECK(Royal_Graph_str(&gr, &outs));
	CHECK(outs != NULL);
	CHECK(0 == strcmp(outs, TEST_OUTGR1));
	printf("content of outs: %s", outs);
	Royal_Graph_deinit(&gr);
	free(outs);
}

int main(int argc, char const *argv[])
{
	test_royal_graph_init();
	test_royal_graph_str();
	RETURN_FAILURES
}

#undef _CLEAN_TEST_OUTBUF
