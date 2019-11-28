/*
 * PartialSuccessResultTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include "catch2/catch.hpp"

#include <iostream>

#include "railroad/railroad.h"

namespace {

using ::railroad::DefaultSuccess;
using ::railroad::PartialSuccessResult;

TEST_CASE("PartialSuccessResult default constructor {}", "[Result]") {
  PartialSuccessResult<DefaultSuccess> presult{};

  std::cout << presult << std::endl;

  REQUIRE_FALSE(static_cast<bool>(presult));
}

TEST_CASE("PartialSuccessResult default constructor", "[Result]") {
  PartialSuccessResult<DefaultSuccess> presult;

  std::cout << presult << std::endl;

  REQUIRE_FALSE(static_cast<bool>(presult));
}

}  // namespace