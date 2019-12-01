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

using ::railroad::bindr;
using ::railroad::DefaultFailure;
using ::railroad::Result;
using ::railroad::Success;
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
        (adder >> bindr(adder))(Success<int, int>(checkThis));
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
        (adder >> bindr(adder) >> bindr(adder))(Success<int, int>(checkThis));

    Result<int, int> preMixedChain =
        (adder >>= adder >> bindr(adder))(Success<int, int>(checkThis));

    Result<int, int> postMixedChain =
        (adder >> bindr(adder) >>= adder)(Success<int, int>(checkThis));

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

TEST_CASE(">>= works on partialS->S func", "[operatorPrecedence]") {
  std::function<PartialSuccessResult<int>(PartialSuccessResult<int>)> adder =
      [](PartialSuccessResult<int> input) -> PartialSuccessResult<int> {
    return PartialSuccessResult<int>(input.unpack() + 1);
  };

  REQUIRE(rc::check([adder](int checkThis) {
    Result<int, DefaultFailure> explicitChain =
        (bindr<int, int, DefaultFailure, DefaultFailure>(adder) >>
         bindr<int, int, DefaultFailure, DefaultFailure>(adder) >>
         bindr<int, int, DefaultFailure, DefaultFailure>(adder))(
            Success<int, DefaultFailure>(checkThis));

    Result<int, DefaultFailure> preMixedChain =
        (bindr<int, int, DefaultFailure, DefaultFailure>(adder) >>=
         adder >> bindr<int, int, DefaultFailure, DefaultFailure>(adder))(
            Success<int, DefaultFailure>(checkThis));

    Result<int, DefaultFailure> postMixedChain =
        (bindr<int, int, DefaultFailure, DefaultFailure>(adder) >>
             bindr<int, int, DefaultFailure, DefaultFailure>(adder) >>=
         adder)(Success<int, DefaultFailure>(checkThis));

    Result<int, DefaultFailure> implicitChain =
        (bindr<int, int, DefaultFailure, DefaultFailure>(adder) >>= adder >>=
         adder)(Success<int, DefaultFailure>(checkThis));

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