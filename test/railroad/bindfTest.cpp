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

using ::railroad::Bindfc;  // aka bindFailure
using ::railroad::DefaultSuccess;
using PFRi = ::railroad::PartialFailureResult<int>;

TEST_CASE("bindf works on pure func", "[bindf]") {
  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedFailure<DefaultSuccess, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<DefaultSuccess, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    Bindfc bindfc;
    int normalResult = adder(checkThis);
    std::optional<int> bindResult =
        (feed >> bindfc(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));
}

/*
TEST_CASE("bindf works on partial func", "[bindf]") {
  std::function<PFRi(PFRi)> adder = [](PFRi val) {
    return PFRi{val.unpack() + 1};
  };

  std::function feed = ::railroad::helpers::feedFailure<std::string, int>;
  std::function terminate =
      ::railroad::helpers::terminateFailure<std::string, int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    int normalResult = adder(PFRi{checkThis}).unpack();
    std::optional<int> bindResult =
        (feed >> bindf(adder) >> terminate)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));
}
*/

}  // namespace
