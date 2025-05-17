#pragma once

#include "Common.h"

#define GLCall(x)                                                                        \
    GLClearError();                                                                      \
    x;                                                                                   \
    ASSERT(GLLogCall())

void GLClearError();
bool GLLogCall();
