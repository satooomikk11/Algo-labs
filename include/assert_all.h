#ifndef ASSERT_ALL_H
#define ASSERT_ALL_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define ASSERT_ALL(...)                                                      \
    do {                                                                     \
        bool _conds[] = {__VA_ARGS__};                                       \
        for (size_t _i = 0; _i < sizeof(_conds)/sizeof(_conds[0]); _i++) {   \
            assert(_conds[_i]);                                              \
        }                                                                    \
    } while(0)

#endif // ASSERT_ALL_H
