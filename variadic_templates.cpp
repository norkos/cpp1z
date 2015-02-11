#include "gtest/gtest.h"
#include <iostream>
#include <memory>

namespace {

/** Adder http://eli.thegreenplace.net/2014/variadic-templates-in-c/ */

template<typename T>
T adder(T v) {
  return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args){
    return first + adder(args...);
}

TEST(VARIADIC_TEMPLATE, check_simple_adder) {
    //  given
    int a = 1;
    int b = 2;
    int c = 4;
    
    //  when
    auto result = adder(a,b,c);
    
    // then
    ASSERT_EQ(a+b+c, result);
}

/** Are there any pairs ? */

template<typename T>
bool are_pairs(T&A){
    return false;
}

bool are_pairs(){
    return true;
}

template<typename T, typename K, typename... Args>
bool are_pairs(T&a, K&b, Args... args){
    return are_pairs(args...);
}

TEST(VARIADIC_TEMPLATE, check_pars) {
    //  given
    int a = 1;
    int b = 2;
    std::string c ="c";
    std::string d ="d";
    
    //  when
    bool is_pair = are_pairs(a,b, c, d);
    
    // then
    ASSERT_TRUE(is_pair);
}

TEST(VARIADIC_TEMPLATE, check_not_pars) {
    //  given
    int a = 1;
    std::string c ="c";
    std::string d ="d";
    
    //  when
    bool is_pair = are_pairs(a, c, d);
    
    // then
    ASSERT_FALSE(is_pair);
}

/** Maybe Tuple ? */
template<class... Ts>
struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
    tuple(T t, Ts... ts) : tuple<Ts...>(ts...), head(t) {}
    T head;
};

template <std::size_t k, class T>
struct elem_type_holder{
};

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>>{
    typedef T type;
};

template <std::size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>>{
    typedef typename elem_type_holder<k-1, tuple<Ts...>>::type type;
};

template<std::size_t k, class... Ts>
std::enable_if_t<k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>
get(tuple<Ts...> &t){
    return t.head;
}
 
template<std::size_t k, class T, class... Ts>
std::enable_if_t<k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>
get(tuple<T, Ts...> &head){
    tuple<Ts...>& tail = head;
    return get<k-1>(tail);
}

TEST(VARIADIC_TEMPLATE, check_tuple) {
    //  given
    int a = 2;
    double b = 3.0;
    std::string c("hi");
    
    //  when
    tuple<int, double, std::string> result(a, b,c);
    
    //  then
    ASSERT_EQ(a, get<0>(result));
    EXPECT_DOUBLE_EQ(b, get<1>(result));
    ASSERT_EQ(c, get<2>(result));
}

///////////////////////////

//  SUM values
template <template <typename, typename...> class MyContainer,
    typename Value, typename... Alloc>
Value sum(const MyContainer<Value, Alloc...> & container){
    Value result = 0;
    for(const auto& v : container){
        result += v;
    }
    return result;
}

TEST(VARIADIC_TEMPLATE, check_sum_vector) {
    //  given
    int a = 2;
    int b = 4;
    
    //  when
    std::vector<int> my_vector{a,b};
    int vector_sum = sum(my_vector);
    
    //  then
    ASSERT_EQ(a+b, vector_sum);
}

TEST(VARIADIC_TEMPLATE, check_sum_set) {
    //  given
    int a = 2;
    int b = 4;
    
    //  when
    std::set<int> my_set{a,b};
    int set_sum = sum(my_set);
    
    //  then
    ASSERT_EQ(a+b, set_sum);
}

//////////////////////// perfect_forwarding

struct B{ 
    int x;
    B(int x):x(x){} 
};

struct A{ 
    int x;
    int y;
    A(B& b, B& c):x(b.x), y(c.x){} 
    A(B&& b, B&& c):x(b.x), y(c.x){} 
};


template <typename T, typename... Arg>
std::shared_ptr<T> factory(Arg&&... arg){
    return std::make_shared<T>(std::forward<Arg>(arg)...);
}

TEST(VARIADIC_TEMPLATE, perfect_forwarding) {
    //  given
    int a = 10;
    int b = 5;
    
    //  when
    std::shared_ptr<A> p = factory<A>(B(a), B(b)); /** fast forward */

    //  then
    EXPECT_EQ(a, p->x);
    EXPECT_EQ(b, p->y);
}

} // namespace