/*
 * feed.cpp
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

  REQUIRE(rc::check("pass through equivalent", [feed](int checkThis) {
    Result<int, DefaultFailure> result = feed(checkThis);
    RC_ASSERT(result.hasSuccess());
    RC_ASSERT(!result.hasFailure());

    RC_ASSERT(result.getSuccess() == checkThis);
  }));
}

TEST_CASE("feedSuccess<int, string> works as expected", "[feed]") {
  std::function feed = feedSuccess<int, std::string>;

  Result<int, std::string> result = feed(5);

  REQUIRE(result.hasSuccess());
  REQUIRE_FALSE(result.hasFailure());

  CHECK(result.getSuccess() == 5);
}

TEST_CASE("feedFailure<int, std::string> works as expected", "[feed]") {
  std::function feed = feedFailure<int, std::string>;

  Result<int, std::string> result = feed("Hello");

  REQUIRE(result.hasFailure());
  REQUIRE_FALSE(result.hasSuccess());

  CHECK(result.getFailure() == "Hello");
}

}  // namespace
