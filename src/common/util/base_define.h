#pragma once

#include "muduo/base/Logging.h"

#define BELL_SUCESS 0;
#define BELL_FAIL 1;

#define CHECK_NULL(ptr, ret)               \
    {                                      \
        if (ptr == NULL) {                 \
            LOG_WARN << #ptr << "is NULL"; \
            return ret;                    \
        }                                  \
    }                                      \
    while (0)