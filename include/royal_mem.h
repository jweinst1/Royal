#ifndef ROYAL_MEM_H
#define ROYAL_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if defined(UINT_MAX) && UINT_MAX == 0xffffffff
#  define ROYAL_CCHECK_INT_32BIT
#else // defined(UINT_MAX) && UINT_MAX == 0xffffffff
#  if defined(UINT_MAX) && UINT_MAX == 0xffff 
#    define ROYAL_CCHECK_INT_16BIT
#  endif // defined(UINT_MAX) && UINT_MAX == 0xffff
#endif // !defined(UINT_MAX) && UINT_MAX == 0xffffffff


#ifdef ROYAL_MEM_DEBUG
#  define _Royal_alloc(ptr, size) \
                     (ptr) = calloc(1, size); \
                     printf("Royal DEBUG: size %lu (Alloc) -> %p\n", (unsigned long)size, ptr)
#else // !ROYAL_MEM_DEBUG
#  define _Royal_alloc(ptr, size) (ptr) = calloc(1, size)
#endif

#ifdef ROYAL_MEM_DEBUG
#  define _Royal_realloc(new_ptr, old_ptr, new_size) \
                     (new_ptr) = (realloc((old_ptr), (new_size)); \
                     printf("Royal DEBUG: size %lu (Realloc) %p -> %p\n", (unsigned long)new_size, old_ptr, new_ptr)
#else // !ROYAL_MEM_DEBUG
#  define _Royal_realloc(new_ptr, old_ptr, new_size) (new_ptr) = (realloc((old_ptr), (new_size)))
#endif                     



#ifdef ROYAL_MEM_DEBUG
#  define _Royal_free(ptr) \
                     printf("Royal DEBUG: (Free) -> %p\n", ptr); \
                     free(ptr)
#else // !ROYAL_MEM_DEBUG
#  define _Royal_free(ptr) free(ptr)
#endif
                     
#endif // ROYAL_MEM_H