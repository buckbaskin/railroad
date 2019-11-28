/*
 * DefaultFailure.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "catch2/catch.hpp"

#include "railroad/DefaultFailure.h"

namespace {

using railroad::DefaultFailure;

TEST_CASE("Construct Failure(true)", "[DefaultFailure]") {
  DefaultFailure df(true);
  std::cout << df << std::endl;
  REQUIRE(static_cast<bool>(df));
}

TEST_CASE("Construct Failure(false)", "[DefaultFailure]") {
  DefaultFailure df(false);
  std::cout << df << std::endl;
  REQUIRE_FALSE(static_cast<bool>(df));
}

}  // namespace