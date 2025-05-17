#pragma once

#define ASSERT(x)                                                                        \
    {                                                                                    \
        if(!(x)) {                                                                       \
            __builtin_trap();                                                            \
        }                                                                                \
    }
