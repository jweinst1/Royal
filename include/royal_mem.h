#ifndef ROYAL_MEM_H
#define ROYAL_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#if defined(UINT_MAX) && UINT_MAX == 0xffffffff
#  define ROYAL_CCHECK_INT_32BIT
#else // defined(UINT_MAX) && UINT_MAX == 0xffffffff
#  if defined(UINT_MAX) && UINT_MAX == 0xffff 
#    define ROYAL_CCHECK_INT_16BIT
#  endif // defined(UINT_MAX) && UINT_MAX == 0xffff
#endif // !defined(UINT_MAX) && UINT_MAX == 0xffffffff

#ifdef _MSC_VER
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#  include <stdint.h>
#endif


#ifdef ROYAL_MEM_DEBUG
#  define _Royal_alloc(ptr, size) \
                     (ptr) = calloc(1, size); \
                     printf("Royal DEBUG: size %lu (Alloc) -> %p\n", (unsigned long)size, ptr)
#else // !ROYAL_MEM_DEBUG
#  define _Royal_alloc(ptr, size) (ptr) = calloc(1, size)
#endif

#ifdef ROYAL_MEM_DEBUG
#  define _Royal_malloc(ptr, size) \
                     (ptr) = malloc(size); \
                     printf("Royal DEBUG: size %lu (Malloc) -> %p\n", (unsigned long)size, ptr)
#else // !ROYAL_MEM_DEBUG
#  define _Royal_alloc(ptr, size) (ptr) = malloc(size)
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

#ifndef ROYAL_ASSERT_EXIT_CODE
#define ROYAL_ASSERT_EXIT_CODE 33
#endif            

#define _Royal_assert(exp) if(!(exp)) { \
                     fprintf(stderr, "FATAL: exp '%s' asserts at line: %u\n", \
                     #exp, \
                     	(unsigned)__LINE__);  \
                     exit(ROYAL_ASSERT_EXIT_CODE); \
                      }
                     
#endif // ROYAL_MEM_H