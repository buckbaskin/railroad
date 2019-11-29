/*
 * bindsTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include "catch2/catch.hpp"
#include "rapidcheck.h"

#include "railroad/railroad.h"

namespace {

using ::railroad::binds;  // aka bindSuccess
using ::railroad::DefaultFailure;
using ::railroad::Failure;
using PSRi = ::railroad::PartialSuccessResult<int>;

TEST_CASE("binds works on pure func", "[binds]") {
  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> binds(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult = (binds(adder) >> terminate)(
        Failure<int, DefaultFailure>(DefaultFailure{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("binds works on partial func", "[binds]") {
  std::function<PSRi(PSRi)> adder = [](PSRi val) {
    return PSRi{val.unpack() + 1};
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(PSRi{checkThis}).unpack();
    std::optional<int> bindResult =
        (feed >> binds(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult = (binds(adder) >> terminate)(
        Failure<int, DefaultFailure>(DefaultFailure{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("binds works on lambda", "[binds]") {
  auto adder = [](int i) -> int { return i + 1; };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> binds<int, int>(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult = (binds<int, int>(adder) >> terminate)(
        Failure<int, DefaultFailure>(DefaultFailure{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

}  // namespace
