#pragma once
#include <iostream>

struct TestState {
    int failures = 0;
};

#define CHECK(state, expr) \
    do { \
        if (!(expr)) { \
            std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << " " << #expr << "\n"; \
            (state).failures++; \
        } \
    } while (0)
