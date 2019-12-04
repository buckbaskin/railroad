/*
 * bindTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "catch2/catch.hpp"
#include "rapidcheck.h"

#include "railroad/railroad.h"

namespace {

using ::railroad::Failure;
using ::railroad::rbind;
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
            (feedS >> rbind(full) >> terminateS)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));

  REQUIRE(
      rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
        string normalResult = full(Failure<int, int>(checkThis)).getFailure();
        std::optional<string> bindResult =
            (feedF >> rbind(full) >> terminateF)(checkThis);
        REQUIRE(static_cast<bool>(bindResult));
        REQUIRE(normalResult == *(bindResult));
      }));
}

enum class ExceptionHandled { NO_EXCEPTION, OUT_OF_RANGE };
struct ExceptionFailure {
  ExceptionFailure() : ex_(ExceptionHandled::NO_EXCEPTION) {}
  ExceptionFailure(const std::out_of_range& /* e */)
      : ex_(ExceptionHandled::OUT_OF_RANGE) {}

  ExceptionHandled ex_;
};

bool operator==(const ExceptionFailure& a, const ExceptionFailure& b) {
  return a.ex_ == b.ex_;
}

std::ostream& operator<<(std::ostream& out, const ExceptionFailure& ef) {
  out << "ExceptionFailure(";
  if (ef.ex_ == ExceptionHandled::NO_EXCEPTION) {
    out << "NO_EXCEPTION";
  } else if (ef.ex_ == ExceptionHandled::OUT_OF_RANGE) {
    out << "OUT_OF_RANGE";
  } else {
    out << "Invalid";
  }
  return out;
}

TEST_CASE("bind works on complete func with except", "[bind]") {
  std::cout << "started func call" << std::endl;

  std::function<Result<string, ExceptionFailure>(Result<int, ExceptionFailure>)>
      full = [](Result<int, ExceptionFailure> r)
      -> Result<string, ExceptionFailure> {
    if (r.hasFailure()) {
      std::cout << "inside except-full Fmode" << std::endl;
      return Failure<string, ExceptionFailure>(r.getFailure());
    } else {
      std::cout << "inside except-full Smode" << std::endl;
      std::cout << r << std::endl;
      std::cout << "logged result" << std::endl;
      throw std::out_of_range("Test out of range handling");
      std::stringstream buf;
      buf << "Success: " << r.getSuccess();
      return Success<string, ExceptionFailure>(buf.str());
    }
  };

  std::cout << "defined full func" << std::endl;

  std::function feedS = ::railroad::helpers::feedSuccess<int, ExceptionFailure>;
  std::function feedF = ::railroad::helpers::feedFailure<int, ExceptionFailure>;
  std::function terminateS =
      ::railroad::helpers::terminateSuccess<string, ExceptionFailure>;
  std::function terminateF =
      ::railroad::helpers::terminateFailure<string, ExceptionFailure>;

  std::cout << "Defined feeds and terminaters" << std::endl;

  CHECK(rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
    std::cout << "start normal result " << checkThis << std::endl;

    Result<int, ExceptionFailure> r = Success<int, ExceptionFailure>(checkThis);

    std::cout << "Can I build it?" << std::endl;

    try {
      Result<string, ExceptionFailure> fullResult =
          full(Success<int, ExceptionFailure>(checkThis));
    } catch (const std::out_of_range& /* e */) {
      std::cout << "Caught exception in naked call" << std::endl;
    } catch (...) {
      std::cout << "Sent the wrong exception" << std::endl;
      REQUIRE(false);
    }

    std::optional<ExceptionFailure> bindResult =
        (feedS >> rbind<string, int, ExceptionFailure, ExceptionFailure,
                        std::out_of_range>(full) >>
         terminateF)(checkThis);

    REQUIRE(static_cast<bool>(bindResult));

    std::out_of_range verifyWithThis("Failure mode");
    CHECK(ExceptionFailure(verifyWithThis) == *(bindResult));
  }));

  std::cout << "Start second test" << std::endl;

  {
    std::out_of_range checkThis("Failure mode");
    ExceptionFailure normalResult =
        full(Failure<int, ExceptionFailure>(ExceptionFailure{checkThis}))
            .getFailure();
    std::optional<ExceptionFailure> bindResult =
        (feedF >> rbind(full) >> terminateF)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    CHECK(normalResult == *(bindResult));
  };
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
        (feedS >> rbind<string, int, int, int>(half) >> terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedS, feedF, half, terminateS,
                     terminateF](int checkThis) {
    int normalResult = checkThis;
    std::optional<int> bindResult =
        (feedF >> rbind<string, int, int, int>(half) >> terminateF)(checkThis);
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
        (feedS >> rbind<int, int, string, int>(half) >> terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedS, feedF, half, terminateS,
                     terminateF](int checkThis) {
    string normalResult = half(PartialFailureResult(checkThis)).unpack();
    std::optional<string> bindResult =
        (feedF >> rbind<int, int, string, int>(half) >> terminateF)(checkThis);
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
        (feedS >> rbind<int, string>(half) >> terminateF)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedF, half, terminateF](string checkThis) {
    string normalResult = checkThis;
    std::optional<string> bindResult =
        (feedF >> rbind<int, string>(half) >> terminateF)(checkThis);
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
        (feedF >> rbind<string, int>(half) >> terminateS)(checkThis);
    REQUIRE(static_cast<bool>(bindResult));
    REQUIRE(normalResult == *(bindResult));
  }));

  REQUIRE(rc::check([feedS, half, terminateS](string checkThis) {
    string normalResult = checkThis;
    std::optional<string> bindResult =
        (feedS >> rbind<string, int>(half) >> terminateS)(checkThis);
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
        (feedS >> rbind<string, int, string, string>(half))(checkThis);
    REQUIRE(normalResult.getSuccess() == bindResult.getSuccess());
    REQUIRE(normalResult.getFailure() == bindResult.getFailure());
  }));

  REQUIRE(rc::check([feedF, half](string checkThis) {
    Result<string, string> normalResult{"", checkThis};
    Result<string, string> bindResult =
        (feedF >> rbind<string, int, string, string>(half))(checkThis);
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
        (feedS >> rbind<string, string, string, int>(half))(checkThis);
    REQUIRE(normalResult.getSuccess() == bindResult.getSuccess());
    REQUIRE_FALSE(bindResult.hasFailure());
  }));

  REQUIRE(rc::check([feedF, half](int checkThis) {
    Result<string, string> normalResult = half(PartialFailureResult(checkThis));
    Result<string, string> bindResult =
        (feedF >> rbind<string, string, string, int>(half))(checkThis);
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
    string bindResult = (feedS >> rbind<string, int, string, int>(half) >>
                         terminateS)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));

  REQUIRE(rc::check([feedF, half, terminateS](int checkThis) {
    string normalResult = half(Failure<int, int>(checkThis)).unpack();
    string bindResult = (feedF >> rbind<string, int, string, int>(half) >>
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
    string bindResult = (feedS >> rbind<string, int, string, int>(half) >>
                         terminateF)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));

  REQUIRE(rc::check([feedF, half, terminateF](int checkThis) {
    string normalResult = half(Failure<int, int>(checkThis)).unpack();
    string bindResult = (feedF >> rbind<string, int, string, int>(half) >>
                         terminateF)(checkThis);
    REQUIRE(normalResult == bindResult);
  }));
}
}  // namespace TwoOne

}  // namespace
