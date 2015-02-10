#include "gtest/gtest.h"
#include <iostream>

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

TEST(RVALUE_TEST, check_simple_adder) {
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

TEST(RVALUE_TEST, check_pars) {
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

TEST(RVALUE_TEST, check_not_pars) {
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
template <class... Ts> struct tuple{};

template <class T, class... Ts>
struct tuple<T, Ts...>: tuple<Ts...>{
    tuple(T t, Ts... ts) :
        tuple<Ts...>(ts...), tail(t) 
        {}
    T tail;
};

template <std::size_t k,  class T> 
struct elem_type_holder {};

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
  typedef T type;
};

template <std::size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
  typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
};

template <std::size_t k, class... Ts>
std::enable_if_t<k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>
get(tuple<Ts...>& t){
    return t.tail;
}

template <std::size_t k, class T, class... Ts>
std::enable_if_t<k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>
get(tuple<T, Ts...>&t){
    tuple<Ts...>& base =t;
    return get<k-1>(base);
}

TEST(RVALUE_TEST, check_tuple) {
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

} // namespace