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

typedef enum {
	ROYAL_GRAPH_SEP_VS,
	ROYAL_GRAPH_SEP_VE,
	ROYAL_GRAPH_SEP_N
} Royal_GraphSepMode;


int Royal_Graph_init(Royal_Graph* gr, unsigned int fsize, size_t* cap, size_t* grow_fac)
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
	if(gr->data == NULL)
		return 0;
	else
		return 1;
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

int Royal_Graph_add(Royal_Graph* gr, const char* input, int enforce_fsize)
{
	// Not implemented yet.
	return 1;
}

int Royal_Graph_str(const Royal_Graph* gr, char** output)
{
	char* writable;
	char* writer;
	const char* reader;
	const char* read_end;
	size_t write_size;
	unsigned int counter;
	Royal_GraphSepMode sep_state = ROYAL_GRAPH_SEP_VS;
	if (gr->data == NULL) {
		*output = NULL;
		return 0;
	}
	if (!(gr->len)) {
		*output = NULL;
		return -1;
	}
	write_size = ROYAL_GRAPH_LSIZE(gr);
	_Royal_alloc(writable, write_size);
	writer = writable;
	reader = gr->data;
	counter = gr->field;
	read_end = reader + write_size;
	while(reader != read_end) {
		if(!counter) {
			switch(sep_state) {
				case ROYAL_GRAPH_SEP_VS:
				    *writer++ = '/';
				    sep_state = ROYAL_GRAPH_SEP_VE;
				    break;
				case ROYAL_GRAPH_SEP_VE:
				    *writer++ = '/';
				    sep_state = ROYAL_GRAPH_SEP_N;
				    break;
				case ROYAL_GRAPH_SEP_N:
				    *writer++ = '\n';
				    sep_state = ROYAL_GRAPH_SEP_VS;
				    break;
			}
			// skip the null barrier
			++reader;
			counter = gr->field;
			continue;
		}
		--counter;
		if(*reader)
			*writer++ = *reader++;
		else
			++reader;
	}
	*output = writable;
	return 1;
}

void Royal_Graph_deinit(Royal_Graph* gr)
{
	_Royal_free(gr->data);
	gr->data = NULL;
	gr->cap = 0;
	gr-> len = 0;
}