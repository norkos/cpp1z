#include "gtest/gtest.h"
#include <iostream>

namespace {

template <typename T>
struct Check {
    static_assert(sizeof(int) <= sizeof(T), "not big enough");
};

TEST(STATIC_ASSERT, basic_assertion) {
    //Check<uint8_t> a;
}

TEST(STATIC_ASSERT, x64) {
    static_assert(sizeof(long) >= 8, "64-bit code generation not enabled/supported.");
}


}