#include "royal_graphs.h"
#include <limits.h>

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

int Royal_Graph_trim(Royal_Graph* gr, size_t to_trim)
{
	if (to_trim > gr->len)
		return 0;
	gr->len -= to_trim;
	return 1;
}

Royal_char* Royal_Graph_append(Royal_Graph* gr, const char* e1, const char* v, const char* e2, int enforce)
{
	char* writable;
	if (enforce) {
		if (strlen(e1) > gr->field || strlen(v) > gr->field || strlen(e2) > gr->field)
			return NULL;
	}
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

int Royal_Graph_str(const Royal_Graph* gr, char** output, char split, char sepch)
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
				    *writer++ = split;
				    sep_state = ROYAL_GRAPH_SEP_VE;
				    break;
				case ROYAL_GRAPH_SEP_VE:
				    *writer++ = split;
				    sep_state = ROYAL_GRAPH_SEP_N;
				    break;
				case ROYAL_GRAPH_SEP_N:
				    *writer++ = sepch;
				    sep_state = ROYAL_GRAPH_SEP_VS;
				    break;
			}
			// skip the null barrier
			++reader;
			counter = gr->field;
			continue;
		}
		// Never write \0 chars into output string
		--counter;
		if(*reader)
			*writer++ = *reader++;
		else
			++reader;
	}
	*output = writable;
	return 1;
}

int Royal_Graph_copy(Royal_Graph* dst, const Royal_Graph* src, size_t up_to)
{
	if (dst->data != NULL)
		return 0;
	if (up_to && up_to > src->len)
		return -1;
	dst->len = up_to ? up_to : src->len;
	dst->cap = src->cap;
	dst->v_off = src->v_off;
	dst->grow = src->grow;
	dst->field = src->field;
	dst->e_off = src->e_off;
	dst->n_off = src->n_off;
	_Royal_alloc(dst->data, ROYAL_GRAPH_SIZE(src));
	memcpy(dst->data, src->data, up_to ? ROYAL_GRAPH_NSIZE(src, up_to) : ROYAL_GRAPH_SIZE(src));
	return 1;
}

int Royal_Graph_get(const Royal_Graph* gr,
	                size_t index,
	                const char** e1,
	                const char** v, 
	                const char** e2)
{
	if (index >= gr->len)
		return 0;
	Royal_char* conn = ROYAL_GRAPH_GET(gr, index);
	*e1 = conn;
	*v = conn + gr->v_off;
	*e2 = conn + gr->e_off;
	return 1;
}

long Royal_Graph_count(const Royal_Graph* gr,
	                            const char* e1, 
	                            const char* v,
	                            const char* e2)
{
	long total = 0;
	if(gr->data == NULL)
		return -1;
	Royal_char* reader = gr->data;
	const Royal_char* read_end = reader + ROYAL_GRAPH_LSIZE(gr);
	while (reader != read_end) {
		if(e1 != NULL && 0 != strcmp(e1, reader))
			goto NEXT_CONN;
		if(v != NULL && 0 != strcmp(v, reader + gr->v_off))
			goto NEXT_CONN;
		if(e2 != NULL && 0 != strcmp(e2, reader + gr->e_off))
			goto NEXT_CONN;
		++total;
NEXT_CONN:
       reader += gr->n_off;
	}
	return total;
}

int Royal_Graph_match(const Royal_Graph* gr,
	                        Royal_Graph* output,
	                            const char* e1, 
	                            const char* v,
	                            const char* e2)
{
	Royal_char* reader = gr->data;
	const Royal_char* read_end = reader + ROYAL_GRAPH_LSIZE(gr);
	size_t outcap = gr->cap;
	size_t outgrow = gr->grow;
	if (gr->data == NULL)
		return 0;
	if (output->data == NULL) {
		if (!Royal_Graph_init(output, gr->field, &outcap, &outcap))
			return 0;
	} else {
		(void) outcap;
		(void) outgrow;
	}
	while(reader != read_end) {
		if (e1 != NULL && 0 != strcmp(e1, reader))
			goto NEXT_CONN;
		if (v != NULL && 0 != strcmp(v, reader + gr->v_off))
			goto NEXT_CONN;
		if (e2 != NULL && 0 != strcmp(e2, reader + gr->e_off))
			goto NEXT_CONN;
		if (output->len == output->cap) {
			ROYAL_GRAPH_GROW(output);
		}
		memcpy(output->data + ROYAL_GRAPH_LSIZE(output), reader, output->n_off);
		++(output->len);
NEXT_CONN:
         reader += gr->n_off;
	}
	return 1;
}


void Royal_Graph_deinit(Royal_Graph* gr)
{
	_Royal_free(gr->data);
	gr->data = NULL;
	gr->cap = 0;
	gr-> len = 0;
}