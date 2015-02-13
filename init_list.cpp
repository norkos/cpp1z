#include "gtest/gtest.h"
#include <iostream>

namespace {

#define X 1
#define Y 2

struct A{
    int result = 0;
    
    A(std::initializer_list<int>): result(X) {
        
    }
    
    A(std::initializer_list<double>): result(Y) {
        
    }
    
};
    
TEST(INIT_LIST, constructors) {
    //  given && when
    A a = {1, 2, 3};
    A b = {1.0, 2.0, 3.0};
    
    //  then
    EXPECT_EQ(X, a.result);
    EXPECT_EQ(Y, b.result);
}

struct B{
    int a;
    double b, c;
};

TEST(INIT_LIST, pod) {
    //  given
    int a = 2;
    double b = 3.0;
    double c = 3.5;
    
    //  when
    B result = {a, b, c};
    
    //  then
    EXPECT_EQ(a, result.a);
    EXPECT_DOUBLE_EQ(b, result.b);
    EXPECT_DOUBLE_EQ(c, result.c);
}


}