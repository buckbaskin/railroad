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

using ::railroad::binds;
using ::railroad::Result;
using ::railroad::Success;
using ::railroad::result::PartialSuccessResult;

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
        (adder >> bindr(adder))(Success<int, int>(checkThis));
    Result<int, int> implicitChain =
        (adder >>= adder)(Success<int, int>(checkThis));

    REQUIRE(explicitChain.hasSuccess());
    REQUIRE(implicitChain.hasSuccess());
    REQUIRE(explicitChain.getSuccess() == implicitChain.getSuccess());
  }));
}

/*
TEST_CASE(">>= works on partialS->S func", "[operatorPrecedence]") {
  std::function<PartialSuccessResult<int>(PartialSuccessResult<int>)> adder =
      [](PartialSuccessResult<int> i) {
        return PartialSuccessResult<int>{i.unpack() + 1};
      };
  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  REQUIRE(rc::check([feed, adder, terminate](int checkThis) {
    Result<int, int> explicitChain = (feed >> bind(adder))(checkThis);
    Result<int, int> implicitChain = (feed >>= adder)(checkThis);

    REQUIRE(explicitChain.hasSuccess());
    REQUIRE(implicitChain.hasSuccess());
    REQUIRE(explicitChain.getSuccess() == implicitChain.getSuccess());
  }));
}
*/

}  // namespace