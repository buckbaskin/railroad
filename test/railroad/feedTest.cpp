/*
 * feedTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <string>
#include "catch2/catch.hpp"
#include "rapidcheck.h"

#include "railroad/railroad.h"

namespace {

using ::railroad::DefaultFailure;
using ::railroad::DefaultSuccess;
using ::railroad::Result;
using ::railroad::helpers::feedFailure;
using ::railroad::helpers::feedSuccess;

TEST_CASE("feedSuccess<int> works as expected", "[feed]") {
  std::function feed = feedSuccess<int>;

  REQUIRE(rc::check([feed](int checkThis) {
    Result<int, DefaultFailure> result = feed(checkThis);
    REQUIRE(result.hasSuccess());
    REQUIRE_FALSE(result.hasFailure());

    REQUIRE(result.getSuccess() == checkThis);
  }));
}

TEST_CASE("feedSuccess<int, string> works as expected", "[feed]") {
  std::function feed = feedSuccess<int, std::string>;

  REQUIRE(rc::check([feed](int checkThis) {
    Result<int, std::string> result = feed(checkThis);
    REQUIRE(result.hasSuccess());
    REQUIRE_FALSE(result.hasFailure());

    REQUIRE(result.getSuccess() == checkThis);
  }));
}

TEST_CASE("feedFailure<int, std::string> works as expected", "[feed]") {
  std::function feed = feedFailure<int, std::string>;

  REQUIRE(rc::check([feed](const std::string& checkThis) {
    Result<int, std::string> result = feed(checkThis);
    REQUIRE(result.hasFailure());
    REQUIRE_FALSE(result.hasSuccess());

    REQUIRE(result.getFailure() == checkThis);
  }));
}

}  // namespace
