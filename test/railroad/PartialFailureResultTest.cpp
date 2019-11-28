/*
 * PartialFailureResultTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include "catch2/catch.hpp"

#include <iostream>

#include "railroad/railroad.h"

namespace {

using ::railroad::DefaultFailure;
using ::railroad::PartialFailureResult;

TEST_CASE("PartialFailureResult default constructor {}", "[Result]") {
  PartialFailureResult<DefaultFailure> presult{};

  std::cout << presult << std::endl;

  REQUIRE_FALSE(static_cast<bool>(presult));
}

TEST_CASE("PartialFailureResult default constructor", "[Result]") {
  PartialFailureResult<DefaultFailure> presult;

  std::cout << presult << std::endl;

  REQUIRE_FALSE(static_cast<bool>(presult));
}

}  // namespace