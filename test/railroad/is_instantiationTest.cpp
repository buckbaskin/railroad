/*
 * is_instantiationTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include <queue>
#include <vector>
#include "catch2/catch.hpp"

#include "railroad/PartialFailureResult.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/is_instantiation.h"

namespace {

using ::railroad::is_instantiation;
using ::railroad::result::PartialFailureResult;
using ::railroad::result::PartialSuccessResult;

TEST_CASE("is instantiation", "[is_instantiation]") {
  using InspectThis = std::vector<int>;

  constexpr bool expected = is_instantiation<std::vector, InspectThis>::value;
  REQUIRE(expected);

  using PartialSuccessResultI = PartialSuccessResult<int>;

  constexpr bool alsoExpected =
      is_instantiation<PartialSuccessResult, PartialSuccessResultI>::value;
  REQUIRE(alsoExpected);

  using PartialFailureResultI = PartialFailureResult<int>;

  constexpr bool alsoExpected2 =
      is_instantiation<PartialFailureResult, PartialFailureResultI>::value;
  REQUIRE(alsoExpected2);
}

TEST_CASE("is not instantiation", "[is_instantiation]") {
  using InspectThis = std::vector<int>;

  constexpr bool expected = is_instantiation<std::queue, InspectThis>::value;
  REQUIRE_FALSE(expected);

  using PartialSuccessResultI = PartialSuccessResult<int>;

  constexpr bool alsoExpected =
      is_instantiation<PartialFailureResult, PartialSuccessResultI>::value;
  REQUIRE_FALSE(alsoExpected);
}

}  // namespace
