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

static void test_royal_graph_append(void)
{
	Royal_Graph gr;
	size_t capt = 5;
	Royal_char* a1;
	Royal_char* a2;
	CHECK(Royal_Graph_init(&gr, 5, &capt, NULL));
	a1 = Royal_Graph_append(&gr, "foo", "->", "bar", 0);
	a2 = Royal_Graph_append(&gr, "doo", "->", "bar", 0);
	CHECK(a1 != NULL && a2 != NULL);
	CHECK(0 == strcmp(a1, "foo"));
	CHECK(0 == strcmp(a2, "doo"));
	Royal_Graph_deinit(&gr);
}

static const char* TEST_OUTGR1 = "a/is/c\na/not/b\nc/like/b\n";

static void test_royal_graph_str(void)
{
	Royal_Graph gr;
	char* outs;
	size_t capt = 10;
	CHECK(Royal_Graph_init(&gr, 6, &capt, NULL));
	CHECK(Royal_Graph_append(&gr, "a", "is", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "a", "not", "b", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "c", "like", "b", 0) != NULL);
	CHECK(Royal_Graph_str(&gr, &outs));
	CHECK(outs != NULL);
	CHECK(0 == strcmp(outs, TEST_OUTGR1));
	printf("content of outs: %s", outs);
	Royal_Graph_deinit(&gr);
	free(outs);
}

static void test_royal_graph_get(void)
{
	Royal_Graph gr;
	const char* e1 = NULL;
	const char* v = NULL;
	const char* e2 = NULL;
	CHECK(Royal_Graph_init(&gr, 5, NULL, NULL));
	CHECK(Royal_Graph_append(&gr, "a", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "a", "to", "b", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "b", "to", "c", 0) != NULL);
	CHECK(1 == Royal_Graph_get(&gr, 1, &e1, &v, &e2));
	CHECK(e1 != NULL && 0 == strcmp(e1, "a"));
	CHECK(v != NULL && 0 == strcmp(v, "to"));
	CHECK(e2 != NULL && 0 == strcmp(e2, "b"));
	Royal_Graph_deinit(&gr);
}

static void test_royal_graph_count(void)
{
	Royal_Graph gr;
	CHECK(Royal_Graph_init(&gr, 10, NULL, NULL));
	CHECK(Royal_Graph_append(&gr, "a", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "a", "to", "b", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "b", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "f", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "e", "to", "b", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "b", "not", "c", 0) != NULL);
	CHECK(5 == Royal_Graph_count(&gr, NULL, "to", NULL));
	Royal_Graph_deinit(&gr);
}

static void test_royal_graph_match(void)
{
	Royal_Graph gr;
	Royal_Graph matched;
	// must be NULL to allow init
	matched.data = NULL;
	CHECK(Royal_Graph_init(&gr, 10, NULL, NULL));
	CHECK(Royal_Graph_append(&gr, "a", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "a", "to", "b", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "b", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "f", "to", "c", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "e", "to", "b", 0) != NULL);
	CHECK(Royal_Graph_append(&gr, "b", "not", "c", 0) != NULL);
	CHECK(Royal_Graph_match(&gr, &matched, NULL, "to", NULL));
	CHECK(matched.data != NULL);
	CHECK(matched.len == 5);
	CHECK(matched.field == gr.field);
	// compare first field
	CHECK(0 == strcmp(matched.data, gr.data));
	Royal_Graph_deinit(&gr);
	Royal_Graph_deinit(&matched);
}

int main(int argc, char const *argv[])
{
	test_royal_graph_init();
	test_royal_graph_str();
	test_royal_graph_append();
	test_royal_graph_get();
	test_royal_graph_count();
	test_royal_graph_match();
	RETURN_FAILURES
}

