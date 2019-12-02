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

using ::std::string;

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

TEST_CASE(">>= works on 2:2 -> S:2 func", "[operatorPrecedence]") {
  std::function<Result<int, string>(Result<int, string>)> TwoTwo =
      [](Result<int, string> input) {
        if (input.hasFailure()) {
          return input;
        } else {
          return Success<int, string>(input.getSuccess() + 1);
        }
      };

  std::function<Result<int, string>(PartialSuccessResult<int>)> STwo =
      [](PartialSuccessResult<int> in) {
        return Success<int, string>(in.unpack() + 1);
      };

  REQUIRE(rc::check([TwoTwo, STwo](int checkThis) {
    Result<int, string> explicitChain =
        (TwoTwo >> rbind<int, int, string, string>(STwo))(
            Success<int, string>(checkThis));

    Result<int, string> implicitChain =
        (TwoTwo >>= STwo)(Success<int, string>(checkThis));

    REQUIRE(explicitChain.hasSuccess());
    REQUIRE(implicitChain.hasSuccess());

    REQUIRE(implicitChain.getSuccess() == explicitChain.getSuccess());
  }));
}

TEST_CASE(">>= works on 2:2 -> F:2 func", "[operatorPrecedence]") {
  std::function<Result<int, string>(Result<int, string>)> TwoTwo =
      [](Result<int, string> input) -> Result<int, string> {
    if (input.hasFailure()) {
      return input;
    } else {
      std::stringstream buf;
      buf << "Failure: " << (input.getSuccess() + 1);
      return Failure<int, string>(buf.str());
    }
  };

  std::function<Result<int, string>(PartialFailureResult<string>)> FTwo =
      [](PartialFailureResult<string> in) {
        return Failure<int, string>("F: " + in.unpack());
      };

  REQUIRE(rc::check([TwoTwo, FTwo](int checkThis) {
    Result<int, string> explicitChain =
        (TwoTwo >> rbind<int, int, string, string>(FTwo))(
            Success<int, string>(checkThis));

    Result<int, string> implicitChain =
        (TwoTwo >>= FTwo)(Success<int, string>(checkThis));

    REQUIRE(explicitChain.hasFailure());
    REQUIRE(implicitChain.hasFailure());

    REQUIRE(implicitChain.getFailure() == explicitChain.getFailure());
  }));
}

}  // namespace
