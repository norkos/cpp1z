#include "gtest/gtest.h"
#include <iostream>
#include <memory>

namespace {

struct B{ 
    int x;
    B(int x):x(x){} 
};

struct A{ 
    int x; 
    A(int x = 0):x(x){} 
    A(B& b):x(b.x){} 
    A(B&& b):x(b.x){} 
};

struct References{
    bool is_temp_object(const A&) { return false; }
    bool is_temp_object(A &&) { return true; }  
};

TEST(RVALUE_TEST, check_tmp_objs) {
    //  given
    References checker;
    
    //  when
    A a;
    
    //  then
    EXPECT_FALSE(checker.is_temp_object(a));
    EXPECT_TRUE(checker.is_temp_object(A()));
}

TEST(RVALUE_TEST, check_rvalue) {
    //  given
    int value = 10;
    
    //  when
    A&& a = A(value);
    
    //  then
    EXPECT_EQ(value, a.x);
}

TEST(RVALUE_TEST, check_move) {
    // given
    int value = 10;
    std::vector<std::unique_ptr<A>> v1, v2;
    
    //  when
    v1.push_back(std::make_unique<A>(value));
    v2 = std::move(v1);
    
    //  then
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(value, v2[0]->x);
}

////////////////////////

template <typename T, typename Arg>
std::shared_ptr<T> factory(Arg&& arg){
    return std::make_shared<T>(std::forward<Arg>(arg));
}

TEST(RVALUE_TEST, perfect_forwarding) {
    //  given
    int value = 10;
    B b(value);
    
    //  when
    std::shared_ptr<A> z = factory<A>(b);
    std::shared_ptr<A> p = factory<A>(B(10)); /** fast forward */
    std::shared_ptr<A> q = factory<A>(value);

    //  then
    EXPECT_EQ(value, z->x);
    EXPECT_EQ(value, p->x);
    EXPECT_EQ(value, q->x);
}

////////////////////////

// http://stackoverflow.com/questions/8610571/what-is-rvalue-reference-for-this
struct LocalContainter{
    
    LocalContainter(int lvalue = 0, int rvalue = 0):
        lvalue(lvalue), rvalue(rvalue){}
    
    int data() &{
        return lvalue;
    }
    
    int data() && {
        return rvalue;        
    }
    
    int lvalue;
    int rvalue;
};


TEST(RVALUE_TEST, lvalue_this) {
    //  given
    int l_value = 10;
    LocalContainter resources(l_value);
    
    //  when
    int result = resources.data();
    
    //  then
    ASSERT_EQ(l_value, result);
}

TEST(RVALUE_TEST, rvalue_this) {
    //  given
    int l_value = 5;
    int r_value = 10;
    
    //  when
    int result = 
        LocalContainter(l_value, r_value).data();
    
    //  then
    ASSERT_EQ(r_value, result);
}

}// namespace

