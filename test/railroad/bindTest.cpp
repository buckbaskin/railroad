/*
 * bindTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include "catch2/catch.hpp"
#include "rapidcheck.h"

#include "railroad/railroad.h"

namespace {

using ::railroad::bindr;
using ::railroad::Failure;
using ::railroad::Result;
using ::railroad::Success;
using ::railroad::result::PartialFailureResult;
using ::railroad::result::PartialSuccessResult;
using ::std::string;

// 2:2
namespace TwoTwo {

TEST_CASE("bind works on complete func", "[bind]") {
  std::function<Result<string, string>(Result<int, int>)> full =
      [](Result<int, int> r) -> Result<string, string> {
    if (r.hasFailure()) {
      std::stringstream buf;
      buf << "Failure: " << r.getFailure();
      return Failure<string, string>(buf.str());
    } else {
      std::stringstream buf;
      buf << "Success: " << r.getSuccess();
      return Success<string, string>(buf.str());
    }
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS =
      ::railroad::helpers::terminateSuccess<string, string>;
  std::function terminateF =
      ::railroad::helpers::terminateFailure<string, string>;

  REQUIRE(
      rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
        string normalResult = full(Success<int, int>(checkThis)).getSuccess();
        std::optional<string> bindResult =
            (feedS >> bindr(full) >> terminateS)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));

  REQUIRE(
      rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
        string normalResult = full(Failure<int, int>(checkThis)).getFailure();
        std::optional<string> bindResult =
            (feedF >> bindr(full) >> terminateF)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));
}
}  // namespace TwoTwo

// 1:1
namespace OneOne {

TEST_CASE("bind works on partialSS func", "[bind]") {
  std::function<PartialSuccessResult<string>(PartialSuccessResult<int>)> half =
      [](PartialSuccessResult<int> psr) -> PartialSuccessResult<string> {
    std::stringstream buf;
    buf << "Success: " << psr.unpack();
    return PartialSuccessResult<string>(buf.str());
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS = ::railroad::helpers::terminateSuccess<string, int>;
  std::function terminateF = ::railroad::helpers::terminateFailure<string, int>;

  REQUIRE(rc::check([feedS, feedF, half, terminateS,
                     terminateF](int checkThis) {
    string normalResult = half(PartialSuccessResult(checkThis)).unpack();
    std::optional<string> bindResult =
        (feedS >> bindr<string, int, int, int>(half) >> terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedS, feedF, half, terminateS,
                     terminateF](int checkThis) {
    int normalResult = checkThis;
    std::optional<int> bindResult =
        (feedF >> bindr<string, int, int, int>(half) >> terminateF)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));
}

TEST_CASE("bind works on partialFF func", "[bind]") {
  std::function<PartialFailureResult<string>(PartialFailureResult<int>)> half =
      [](PartialFailureResult<int> psr) -> PartialFailureResult<string> {
    std::stringstream buf;
    buf << "Failure: " << psr.unpack();
    return PartialFailureResult<string>(buf.str());
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS = ::railroad::helpers::terminateSuccess<int, string>;
  std::function terminateF = ::railroad::helpers::terminateFailure<int, string>;

  REQUIRE(rc::check([feedS, feedF, half, terminateS,
                     terminateF](int checkThis) {
    int normalResult = checkThis;
    std::optional<int> bindResult =
        (feedS >> bindr<int, int, string, int>(half) >> terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedS, feedF, half, terminateS,
                     terminateF](int checkThis) {
    string normalResult = half(PartialFailureResult(checkThis)).unpack();
    std::optional<string> bindResult =
        (feedF >> bindr<int, int, string, int>(half) >> terminateF)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));
}

TEST_CASE("bind works on partialSF func", "[bind]") {
  std::function<PartialFailureResult<string>(PartialSuccessResult<int>)> half =
      [](PartialSuccessResult<int> psr) -> PartialFailureResult<string> {
    std::stringstream buf;
    buf << "Failure: " << psr.unpack();
    return PartialFailureResult<string>(buf.str());
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, string>;
  std::function feedF = ::railroad::helpers::feedFailure<int, string>;
  std::function terminateF = ::railroad::helpers::terminateFailure<int, string>;

  REQUIRE(rc::check([feedS, half, terminateF](int checkThis) {
    string normalResult = half(PartialSuccessResult(checkThis)).unpack();
    std::optional<string> bindResult =
        (feedS >> bindr<int, string>(half) >> terminateF)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedF, half, terminateF](string checkThis) {
    string normalResult = checkThis;
    std::optional<string> bindResult =
        (feedF >> bindr<int, string>(half) >> terminateF)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));
}

TEST_CASE("bind works on partialFS func", "[bind]") {
  std::function<PartialSuccessResult<string>(PartialFailureResult<int>)> half =
      [](PartialFailureResult<int> pfr) -> PartialSuccessResult<string> {
    std::stringstream buf;
    buf << "Success: " << pfr.unpack();
    return PartialSuccessResult<string>(buf.str());
  };

  std::function feedS = ::railroad::helpers::feedSuccess<string, int>;
  std::function feedF = ::railroad::helpers::feedFailure<string, int>;
  std::function terminateS = ::railroad::helpers::terminateSuccess<string, int>;

  REQUIRE(rc::check([feedF, half, terminateS](int checkThis) {
    string normalResult = half(PartialFailureResult(checkThis)).unpack();
    std::optional<string> bindResult =
        (feedF >> bindr<string, string, int, int>(half) >>
         terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedS, half, terminateS](string checkThis) {
    string normalResult = checkThis;
    std::optional<string> bindResult =
        (feedS >> bindr<string, string, int, int>(half) >>
         terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));
}
}  // namespace OneOne

// 1:2
namespace OneTwo {
TEST_CASE("bind works on partialSR func", "[bind]") {
  std::function<Result<string, string>(PartialSuccessResult<int>)> half =
      [](PartialSuccessResult<int> psr) {
        std::stringstream buff;
        buff << "Failure: " << psr.unpack();

        std::stringstream bufs;
        bufs << "Success: " << psr.unpack();

        return Result<string, string>(bufs.str(), buff.str());
      };

  std::function feedS = ::railroad::helpers::feedSuccess<int, string>;
  std::function feedF = ::railroad::helpers::feedFailure<int, string>;

  REQUIRE(rc::check([feedS, half](int checkThis) {
    Result<string, string> normalResult = half(PartialSuccessResult(checkThis));
    Result<string, string> bindResult =
        (feedS >> bindr<string, int, string, string>(half))(checkThis);
    REQUIRE(normalResult.getSuccess() == bindResult.getSuccess());
    REQUIRE(normalResult.getFailure() == bindResult.getFailure());
  }));

  REQUIRE(rc::check([feedF, half](string checkThis) {
    Result<string, string> normalResult{"", checkThis};
    Result<string, string> bindResult =
        (feedF >> bindr<string, int, string, string>(half))(checkThis);
    REQUIRE_FALSE(bindResult.hasSuccess());
    REQUIRE(normalResult.getFailure() == bindResult.getFailure());
  }));
}

TEST_CASE("bind works on partialFR func", "[bind]") {
  std::function<Result<string, string>(PartialFailureResult<int>)> half =
      [](PartialFailureResult<int> psr) {
        std::stringstream buff;
        buff << "Failure: " << psr.unpack();

        std::stringstream bufs;
        bufs << "Success: " << psr.unpack();

        return Result<string, string>(bufs.str(), buff.str());
      };

  std::function feedS = ::railroad::helpers::feedSuccess<string, int>;
  std::function feedF = ::railroad::helpers::feedFailure<string, int>;

  REQUIRE(rc::check([feedS, half](string checkThis) {
    Result<string, string> normalResult{checkThis, ""};
    Result<string, string> bindResult =
        (feedS >> bindr<string, string, string, int>(half))(checkThis);
    REQUIRE(normalResult.getSuccess() == bindResult.getSuccess());
    REQUIRE_FALSE(bindResult.hasFailure());
  }));

  REQUIRE(rc::check([feedF, half](int checkThis) {
    Result<string, string> normalResult = half(PartialFailureResult(checkThis));
    Result<string, string> bindResult =
        (feedF >> bindr<string, string, string, int>(half))(checkThis);
    REQUIRE(normalResult.getSuccess() == bindResult.getSuccess());
    REQUIRE(normalResult.getFailure() == bindResult.getFailure());
  }));
}

}  // namespace OneTwo

// 2:1
namespace TwoOne {
TEST_CASE("bind works on partialRS func", "[bind]") {
  std::function<PartialSuccessResult<string>(Result<int, int>)> half =
      [](Result<int, int> input) {
        if (input.hasFailure()) {
          return PartialSuccessResult<string>{string{"Failure :("}};
        }
        std::stringstream buf;
        buf << "Success: " << input.getSuccess();
        return PartialSuccessResult<string>{buf.str()};
      };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateS =
      ::railroad::unsafe::terminateSuccess<string, string>;

  REQUIRE(rc::check([feedS, half, terminateS](int checkThis) {
    string normalResult = half(Success<int, int>(checkThis)).unpack();
    string bindResult = (feedS >> bindr<string, int, string, int>(half) >>
                         terminateS)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));

  REQUIRE(rc::check([feedF, half, terminateS](int checkThis) {
    string normalResult = half(Failure<int, int>(checkThis)).unpack();
    string bindResult = (feedF >> bindr<string, int, string, int>(half) >>
                         terminateS)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));
}

TEST_CASE("bind works on partialRF func", "[bind]") {
  std::function<PartialFailureResult<string>(Result<int, int>)> half =
      [](Result<int, int> input) {
        if (input.hasFailure()) {
          return PartialFailureResult<string>{string{"Failure :("}};
        }
        std::stringstream buf;
        buf << "Success: " << input.getSuccess();
        return PartialFailureResult<string>{buf.str()};
      };

  std::function feedS = ::railroad::helpers::feedSuccess<int, int>;
  std::function feedF = ::railroad::helpers::feedFailure<int, int>;
  std::function terminateF =
      ::railroad::unsafe::terminateFailure<string, string>;

  REQUIRE(rc::check([feedS, half, terminateF](int checkThis) {
    string normalResult = half(Success<int, int>(checkThis)).unpack();
    string bindResult = (feedS >> bindr<string, int, string, int>(half) >>
                         terminateF)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));

  REQUIRE(rc::check([feedF, half, terminateF](int checkThis) {
    string normalResult = half(Failure<int, int>(checkThis)).unpack();
    string bindResult = (feedF >> bindr<string, int, string, int>(half) >>
                         terminateF)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));
}
}  // namespace TwoOne

}  // namespace
