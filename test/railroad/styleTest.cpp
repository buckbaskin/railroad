/*
 * operatorPrecedenceTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include "catch2/catch.hpp"
#include "rapidcheck.h"

#include "railroad/railroad.h"

namespace {

using ::railroad::DefaultFailure;
using ::railroad::DefaultSuccess;
using ::railroad::Failure;
using ::railroad::rbind;
using ::railroad::Result;
using ::railroad::Success;
using ::railroad::result::PartialFailureResult;
using ::railroad::result::PartialSuccessResult;

TEST_CASE(">>= works on full func simple", "[operatorPrecedence]") {
  std::function<Result<int, int>(Result<int, int>)> adder =
      [](Result<int, int> input) -> Result<int, int> {
    if (input.hasFailure()) {
      return input;
    } else {
      return Success<int, int>(input.getSuccess() + 1);
    }
  };

  REQUIRE(rc::check([adder](int checkThis) {
    Result<int, int> explicitChain =
        (adder >> rbind(adder))(Success<int, int>(checkThis));
    Result<int, int> implicitChain =
        (adder >>= adder)(Success<int, int>(checkThis));

    REQUIRE(explicitChain.hasSuccess());
    REQUIRE(implicitChain.hasSuccess());
    REQUIRE(explicitChain.getSuccess() == implicitChain.getSuccess());
  }));
}

TEST_CASE(">>= works on full func", "[operatorPrecedence]") {
  std::function<Result<int, int>(Result<int, int>)> adder =
      [](Result<int, int> input) -> Result<int, int> {
    if (input.hasFailure()) {
      return input;
    } else {
      return Success<int, int>(input.getSuccess() + 1);
    }
  };

  REQUIRE(rc::check([adder](int checkThis) {
    Result<int, int> explicitChain =
        (adder >> rbind(adder) >> rbind(adder))(Success<int, int>(checkThis));

    Result<int, int> preMixedChain =
        (adder >>= adder >> rbind(adder))(Success<int, int>(checkThis));

    Result<int, int> postMixedChain =
        (adder >> rbind(adder) >>= adder)(Success<int, int>(checkThis));

    Result<int, int> implicitChain =
        (adder >>= adder >>= adder)(Success<int, int>(checkThis));

    REQUIRE(explicitChain.hasSuccess());
    REQUIRE(preMixedChain.hasSuccess());
    REQUIRE(postMixedChain.hasSuccess());
    REQUIRE(implicitChain.hasSuccess());

    REQUIRE(implicitChain.getSuccess() == explicitChain.getSuccess());
    REQUIRE(implicitChain.getSuccess() == preMixedChain.getSuccess());
    REQUIRE(implicitChain.getSuccess() == postMixedChain.getSuccess());
  }));
}

}  // namespace
