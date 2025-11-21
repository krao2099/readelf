#include <stdio.h>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define debug_printf(fmt, ...) do { if (DEBUG_TEST) printf(fmt, __VA_ARGS__);} while (0);