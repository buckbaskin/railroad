/*
 * ResultTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include "catch2/catch.hpp"

#include "railroad/railroad.h"

namespace {

using ::railroad::DefaultFailure;
using ::railroad::Failure;
using ::railroad::PartialFailureResult;
using ::railroad::PartialSuccessResult;
using ::railroad::Result;
using ::railroad::Success;

TEST_CASE(
    "Result::Success(Partial<int>, Partial<DefaultFailure>) works as expected",
    "[Result]") {
  Result<int, DefaultFailure> result = Success<int, DefaultFailure>(
      PartialSuccessResult<int>{5}, PartialFailureResult<DefaultFailure>{});

  std::cout << result << std::endl;

  REQUIRE(result.hasSuccess());
  REQUIRE_FALSE(result.hasFailure());

  CHECK(result.getSuccess() == 5);
}

}  // namespace
