#include "gtest/gtest.h"

TEST(AUTO_TEST, auto_type_test) {
  const int i = 1;
  auto a = i;
  EXPECT_TRUE((std::is_same<int, decltype(a)>::value));
}

