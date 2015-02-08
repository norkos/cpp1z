#include "gtest/gtest.h"
#include <boost/concept_check.hpp>

namespace non_static_data_member {

const int GLOBAL_INT = 5;

struct A{
    int x = GLOBAL_INT;
    int y;    
    
    A(int z):  y(z) {}
};

TEST(NON_STATIC_MEMBER, check_basic_init) {
    //  given
    int y = 3;
    
    // when
    A a = A(y);
    
    // then
    EXPECT_EQ(GLOBAL_INT, a.x);
    EXPECT_EQ(y, a.y);
}

////////////////////////

int f(bool is_ok){
    return is_ok ? GLOBAL_INT : 0;
}

struct B{
    bool cond = true;
    int x = f(cond);    
};

TEST(NON_STATIC_MEMBER, check_init_with_function) {
    //  given && when
    B b;
    
    // then
    EXPECT_EQ(GLOBAL_INT, b.x);
}

struct C{
    int x = f(cond);    
    bool cond = true;
};

TEST(NON_STATIC_MEMBER, check_init_order_still_important) {
    //  given && when
    C b;
    
    // then
    EXPECT_NE(GLOBAL_INT, b.x);
}

} // namespace
