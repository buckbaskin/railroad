/*
 * PartialFailureResultTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include "catch2/catch.hpp"

#include "railroad/railroad.h"

namespace {

using ::railroad::DefaultFailure;
using ::railroad::PartialFailureResult;

TEST_CASE("PartialFailureResult default constructor {}",
          "[PartialFailureResult]") {
  PartialFailureResult<DefaultFailure> presult{};

  REQUIRE_FALSE(static_cast<bool>(presult));
}

TEST_CASE("PartialFailureResult default constructor",
          "[PartialFailureResult]") {
  PartialFailureResult<DefaultFailure> presult;

  REQUIRE_FALSE(static_cast<bool>(presult));
}

TEST_CASE("PartialFailureResult default constructor int",
          "[PartialFailureResult]") {
  PartialFailureResult<int> presult;

  REQUIRE_FALSE(static_cast<bool>(presult));
}

}  // namespace
