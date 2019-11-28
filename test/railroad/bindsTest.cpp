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
using PSRi = ::railroad::PartialSuccessResult<int>;

TEST_CASE("binds works on pure func", "[binds]") {
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

TEST_CASE("binds works on partial func", "[binds]") {
  std::function<PSRi(PSRi)> adder = [](PSRi val) {
    return PSRi{val.unpack() + 1};
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  for (int i = -5; i <= 5; ++i) {
    int normalResult = adder(PSRi{i}).unpack();
    std::optional<int> bindResult = (feed >> binds(adder) >> terminate)(i);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }
}

}  // namespace
