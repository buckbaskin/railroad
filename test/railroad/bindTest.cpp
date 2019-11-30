/*
 * bindTest.cpp
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

using ::railroad::bindr;
using ::railroad::Failure;
using ::railroad::Result;
using ::railroad::Success;
using ::railroad::result::PartialFailureResult;
using ::railroad::result::PartialSuccessResult;
using ::std::string;

TEST_CASE("bind works on complete func", "[bind]") {
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
            (feedS >> bindr(full) >> terminateS)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));

  REQUIRE(
      rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
        string normalResult = full(Failure<int, int>(checkThis)).getFailure();
        std::optional<string> bindResult =
            (feedF >> bindr(full) >> terminateF)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));
}

TEST_CASE("bind works on partialSS func", "[bind]") {
  std::function<PartialSuccessResult<string>(PartialSuccessResult<int>)> half =
      [](PartialSuccessResult<int> psr) -> PartialSuccessResult<string> {
    std::stringstream buf;
    buf << "Success: " << psr.unpack();
    return PartialSuccessResult<string>(buf.str());
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS =
      ::railroad::helpers::terminateSuccess<string, int>;
  std::function terminateF =
      ::railroad::helpers::terminateFailure<string, int>;

  REQUIRE(
      rc::check([feedS, feedF, half, terminateS, terminateF](int checkThis) {
        string normalResult = half(PartialSuccessResult(checkThis)).unpack();
        std::optional<string> bindResult =
            (feedS >> bindr<string, int, int, int>(half) >> terminateS)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));

  REQUIRE(
      rc::check([feedS, feedF, half, terminateS, terminateF](int checkThis) {
        int normalResult = checkThis;
        std::optional<int> bindResult =
            (feedF >> bindr<string, int, int, int>(half) >> terminateF)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));
}

TEST_CASE("bind works on partialFF func", "[bind]") {
  std::function<PartialFailureResult<string>(PartialFailureResult<int>)> half =
      [](PartialFailureResult<int> psr) -> PartialFailureResult<string> {
    std::stringstream buf;
    buf << "Failure: " << psr.unpack();
    return PartialFailureResult<string>(buf.str());
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS =
      ::railroad::helpers::terminateSuccess<int, string>;
  std::function terminateF =
      ::railroad::helpers::terminateFailure<int, string>;

  REQUIRE(
      rc::check([feedS, feedF, half, terminateS, terminateF](int checkThis) {
        int normalResult = checkThis;
        std::optional<int> bindResult =
            (feedS >> bindr<int, int, string, int>(half) >> terminateS)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));

  REQUIRE(
      rc::check([feedS, feedF, half, terminateS, terminateF](int checkThis) {
        string normalResult = half(PartialFailureResult(checkThis)).unpack();
        std::optional<string> bindResult =
            (feedF >> bindr<int, int, string, int>(half) >> terminateF)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));
}

}  // namespace