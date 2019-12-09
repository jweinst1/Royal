#ifndef UNIT_TEST_HELPERS_H
#define UNIT_TEST_HELPERS_H

#include <stdio.h>

// Will be used once per test.
static unsigned _failures = 0;

#define CHECK(cond) if(!(cond) && ++_failures) \
                                fprintf(stderr, "FAILURE: exp '%s' at line %u\n", #cond, (unsigned)__LINE__)


#define RETURN_FAILURES return _failures == 0 ? 0 : 3;

#endif 