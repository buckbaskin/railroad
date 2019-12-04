/*
 * rbindfTest.cpp
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

using ::railroad::DefaultSuccess;
using ::railroad::rbindf;  // aka bindFailure
using ::railroad::Success;
using PFRi = ::railroad::PartialFailureResult<int>;

TEST_CASE("rbindf works on pure func", "[rbindf]") {
  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> rbindf<DefaultSuccess, DefaultSuccess>(adder) >>
         terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult =
        (rbindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(Success<DefaultSuccess, int>(DefaultSuccess{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("rbindf works on partial func", "[rbindf]") {
  std::function<PFRi(PFRi)> adder = [](PFRi val) {
    return PFRi{val.unpack() + 1};
  };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(PFRi{checkThis}).unpack();
    std::optional<int> bindResult =
        (feed >> rbindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult =
        (rbindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(Success<DefaultSuccess, int>(DefaultSuccess{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

TEST_CASE("rbindf works on lambda", "[rbindf]") {
  auto adder = [](int incThis) -> int { return incThis + 1; };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> rbindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(checkThis);

    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feed, adder, terminate](bool checkThis) {
    std::optional<int> bindResult =
        (rbindf<DefaultSuccess, DefaultSuccess, int, int>(adder) >>
         terminate)(Success<DefaultSuccess, int>(DefaultSuccess{checkThis}));
    REQUIRE_FALSE(static_cast<bool>(bindResult));
  }));
}

}  // namespace
