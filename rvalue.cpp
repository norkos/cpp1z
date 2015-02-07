#include "gtest/gtest.h"
#include <iostream>
#include <memory>


struct CheckReference{
    bool is_temp_object(const std::string&) { return false; }
    bool is_temp_object(std::string &&) { return true; }  
};

TEST(RVALUE_TEST, check_tmp_objs) {
    CheckReference checker;
    
    std::string name( "norbert" );
    EXPECT_FALSE(checker.is_temp_object(name));
    EXPECT_TRUE(checker.is_temp_object(std::string("check")));
}

struct A{ 
    int x; 
    A(int x=0):x(x){} 
};

TEST(RVALUE_TEST, check_rvalue) {
    int value = 10;
    A&& a = A(value);
    EXPECT_EQ(value, a.x);
}

TEST(RVALUE_TEST, check_move) {
    int value = 10;
    std::vector<std::unique_ptr<A>> v1, v2;
    v1.push_back(std::make_unique<A>(value));
    v2 = std::move(v1);
    
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(1u, v2.size());
    EXPECT_EQ(value, v2[0]->x);
}