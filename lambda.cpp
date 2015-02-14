#include "gtest/gtest.h"
#include <iostream>
#include <algorithm>
#include <boost/concept_check.hpp>

namespace {

TEST(LAMBDA, basic_assertion) {
    //  given
    int x = 2;
    auto f = [&r = x, x = x + 1]() -> int { r += 2; return x + 2; };
    
    //when
    int y = f();
    
    //  then
    ASSERT_EQ(4, x);
    ASSERT_EQ(5, y);
}

void f(std::vector<int>& v, int factor){
    auto f = [factor] (int& a) { a =  a + factor; };
    std::for_each(v.begin(), v.end(), f);
}

TEST(LAMBA, sorting){
    //  given
    int a = 3, b = 4, factor = 10;
    std::vector<int> v = {a, b};
    
    //  when
    f(v, factor);
    
    //  then
    ASSERT_EQ(a + factor, v[0]);
    ASSERT_EQ(b + factor, v[1]);
}

    
}