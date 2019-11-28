/*
 * binds.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include "catch2/catch.hpp"

#include "railroad/railroad.h"

namespace {

using ::railroad::binds;  // aka bindSuccess

TEST_CASE("binds works as expected", "[binds]") {
  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  for (int i = -5; i <= 5; ++i) {
    int normalResult = adder(i);
    std::optional<int> bindResult = (feed >> binds(adder) >> terminate)(i);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }
}

}  // namespace
