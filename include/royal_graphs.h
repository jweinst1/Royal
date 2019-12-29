#ifndef ROYAL_GRAPHS_H
#define ROYAL_GRAPHS_H

#include "royal_mem.h"
/**
 * The character type used in a royal graph.
 */
typedef char Royal_char;

typedef struct {
	Royal_char* data;
	unsigned int field;
	unsigned int v_off;
	unsigned int e_off;
	unsigned int n_off;
	size_t len;
	size_t cap;
	size_t grow;
} Royal_Graph;

#define ROYAL_GRAPH_CSIZE(gr) ((gr->field + 1) * 3)

#define ROYAL_GRAPH_SIZE(gr) (ROYAL_GRAPH_CSIZE(gr) * gr->cap)

#define ROYAL_GRAPH_GET(gr, at) (gr->data + (ROYAL_GRAPH_CSIZE(gr) * (at))) 

void Royal_Graph_init(Royal_Graph* gr, unsigned int fsize, size_t* cap, size_t* grow_fac);

void Royal_Graph_reserve(Royal_Graph* gr, size_t new_size);

Royal_char* Royal_Graph_append(Royal_Graph* gr, const char* e1, const char* v, const char* e2);

void Royal_Graph_deinit(Royal_Graph* gr);

#endif // ROYAL_GRAPHS_H