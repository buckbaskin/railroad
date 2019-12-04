/*
 * rbindsTest.cpp
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

using ::railroad::DefaultFailure;
using ::railroad::Failure;
using ::railroad::rbinds;  // aka bindSuccess
using PSRi = ::railroad::PartialSuccessResult<int>;

TEST_CASE("rbinds works on pure func", "[rbinds]") {
  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> rbinds(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult = (rbinds(adder) >> terminate)(
        Failure<int, DefaultFailure>(DefaultFailure{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("rbinds works on partial func", "[rbinds]") {
  std::function<PSRi(PSRi)> adder = [](PSRi val) {
    return PSRi{val.unpack() + 1};
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(PSRi{checkThis}).unpack();
    std::optional<int> bindResult =
        (feed >> rbinds(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult = (rbinds(adder) >> terminate)(
        Failure<int, DefaultFailure>(DefaultFailure{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("rbinds works on lambda", "[rbinds]") {
  auto adder = [](int i) -> int { return i + 1; };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> rbinds<int, int>(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult = (rbinds<int, int>(adder) >> terminate)(
        Failure<int, DefaultFailure>(DefaultFailure{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

}  // namespace
