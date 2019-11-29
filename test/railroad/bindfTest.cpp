/*
 * bindfTest.cpp
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

using ::railroad::bindf;  // aka bindFailure
using ::railroad::DefaultSuccess;
using ::railroad::Success;
using PFRi = ::railroad::PartialFailureResult<int>;

TEST_CASE("bindf works on pure func", "[bindf]") {
  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> bindf<DefaultSuccess, DefaultSuccess>(adder) >>
         terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult =
        (bindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(Success<DefaultSuccess, int>(DefaultSuccess{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("bindf works on partial func", "[bindf]") {
  std::function<PFRi(PFRi)> adder = [](PFRi val) {
    return PFRi{val.unpack() + 1};
  };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(PFRi{checkThis}).unpack();
    std::optional<int> bindResult =
        (feed >> bindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult =
        (bindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(Success<DefaultSuccess, int>(DefaultSuccess{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("bindf works on lambda", "[bindf]") {
  auto adder = [](int incThis) -> int { return incThis + 1; };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> bindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(checkThis);

    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult =
        (bindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(Success<DefaultSuccess, int>(DefaultSuccess{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

}  // namespace
