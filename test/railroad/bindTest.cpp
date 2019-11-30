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
using ::railroad::Failure;
using ::railroad::Result;
using ::railroad::Success;
using ::railroad::result::PartialFailureResult;
using ::railroad::result::PartialSuccessResult;
using ::std::string;

TEST_CASE("binds works on complete func", "[binds]") {
  std::function<Result<string, string>(Result<int, int>)> full =
      [](Result<int, int> r) -> Result<string, string> {
    if (r.hasFailure()) {
      std::stringstream buf;
      buf << "Failure: " << r.getFailure();
      return Failure<string, string>(buf.str());
    } else {
      std::stringstream buf;
      buf << "Success: " << r.getSuccess();
      return Success<string, string>(buf.str());
    }
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS =
      ::railroad::helpers::terminateSuccess<string, string>;
  std::function terminateF =
      ::railroad::helpers::terminateFailure<string, string>;

  REQUIRE(
      rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
        string normalResult = full(Success<int, int>(checkThis)).getSuccess();
        std::optional<string> bindResult =
            (feedS >> bind(full) >> terminateS)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));

  REQUIRE(
      rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
        string normalResult = full(Failure<int, int>(checkThis)).getFailure();
        std::optional<string> bindResult =
            (feedF >> bind(full) >> terminateF)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));
}

}  // namespace