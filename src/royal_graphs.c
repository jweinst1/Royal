#include "royal_graphs.h"

#ifndef ROYAL_GRAPH_DEFAULT_CAP
#define ROYAL_GRAPH_DEFAULT_CAP 15
#endif

#ifndef ROYAL_GRAPH_DEFAULT_GROW
#define ROYAL_GRAPH_DEFAULT_GROW 2
#endif

#define ROYAL_GRAPH_GROW(gr) \
                    gr->cap *= gr->grow; \
                    _Royal_realloc(gr->data, gr->data, ROYAL_GRAPH_SIZE(gr))


void Royal_Graph_init(Royal_Graph* gr, unsigned int fsize, size_t* cap, size_t* grow_fac)
{
	size_t capacity = cap == NULL ? ROYAL_GRAPH_DEFAULT_CAP : *cap;
	size_t growth = grow_fac == NULL ? ROYAL_GRAPH_DEFAULT_GROW : *grow_fac;
	gr->len = 0;
	gr->cap = capacity;
	gr->grow = growth;
	gr->field = fsize;
	gr->v_off = fsize + 1;
	gr->e_off = (fsize * 2) + 2;
	gr->n_off = (fsize * 3) + 3;
	_Royal_alloc(gr->data, ROYAL_GRAPH_SIZE(gr));
}

void Royal_Graph_reserve(Royal_Graph* gr, size_t new_size)
{
	_Royal_assert(new_size > gr->cap);
	gr->cap = new_size;
	_Royal_realloc(gr->data, gr->data, ROYAL_GRAPH_SIZE(gr));
}

Royal_char* Royal_Graph_append(Royal_Graph* gr, const char* e1, const char* v, const char* e2)
{
	char* writable;
	if (gr->len == gr->cap) {
		ROYAL_GRAPH_GROW(gr);
	}
	writable = ROYAL_GRAPH_GET(gr, gr->len);
	gr->len++;
	strncpy(writable, e1, gr->field);
	strncpy(writable + gr->v_off, v, gr->field);
	strncpy(writable + gr->e_off, e2, gr->field);
	return writable;
}

void Royal_Graph_deinit(Royal_Graph* gr)
{
	_Royal_free(gr->data);
	gr->data = NULL;
	gr->cap = 0;
	gr-> len = 0;
}