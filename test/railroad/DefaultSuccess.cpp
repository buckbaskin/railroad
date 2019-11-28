/*
 * DefaultSuccess.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "catch2/catch.hpp"

#include "railroad/DefaultSuccess.h"

namespace {

using railroad::DefaultSuccess;

TEST_CASE("Construct Success(true)", "[DefaultSuccess]") {
  DefaultSuccess df(true);
  std::cout << df << std::endl;
  REQUIRE(static_cast<bool>(df));
}

TEST_CASE("Construct Success(false)", "[DefaultSuccess]") {
  DefaultSuccess df(false);
  std::cout << df << std::endl;
  REQUIRE_FALSE(static_cast<bool>(df));
}

}  // namespace