#ifndef ROYAL_GRAPHS_H
#define ROYAL_GRAPHS_H

#include "royal_mem.h"
/**
 * The character type used in a royal graph.
 */
typedef char RoyalChar;

typedef struct {
	RoyalChar* data;
	unsigned int field;
	unsigned int v_off;
	unsigned int e_off;
	size_t len;
	size_t cap;
} RoyalGraph;

#define ROYAL_GRAPH_CSIZE(gr) ((gr->field + 1) * 3)

#define ROYAL_GRAPH_SIZE(gr) (ROYAL_GRAPH_CSIZE(gr) * gr->cap)

void RoyalGraph_init(RoyalGraph* gr, unsigned int fsize, size_t* cap);

void RoyalGraph_reserve(RoyalGraph* gr, size_t new_size);

void RoyalGraph_append(RoyalGraph* gr, const char* e1, const char* v, const char* e2);

void RoyalGraph_deinit(RoyalGraph* gr);

#endif // ROYAL_GRAPHS_H