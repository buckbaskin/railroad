/*
 * rbindExceptionTest.cpp
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
  std::function<Result<string, ExceptionFailure>(Result<int, ExceptionFailure>)>
      full = [](Result<int, ExceptionFailure> r)
      -> Result<string, ExceptionFailure> {
    if (r.hasFailure()) {
      return Failure<string, ExceptionFailure>(r.getFailure());
    } else {
      throw std::out_of_range("Test out of range handling");
      std::stringstream buf;
      buf << "Success: " << r.getSuccess();
      return Success<string, ExceptionFailure>(buf.str());
    }
  };

  std::function feedS = ::railroad::helpers::feedSuccess<int, ExceptionFailure>;
  std::function feedF = ::railroad::helpers::feedFailure<int, ExceptionFailure>;
  std::function terminateS =
      ::railroad::helpers::terminateSuccess<string, ExceptionFailure>;
  std::function terminateF =
      ::railroad::helpers::terminateFailure<string, ExceptionFailure>;

  CHECK(rc::check([feedS, feedF, full, terminateS, terminateF](int checkThis) {
    Result<int, ExceptionFailure> r = Success<int, ExceptionFailure>(checkThis);

    try {
      Result<string, ExceptionFailure> fullResult =
          full(Success<int, ExceptionFailure>(checkThis));
    } catch (const std::out_of_range& /* e */) {
      CHECK(true);
    } catch (...) {
      std::cout << "Sent the wrong exception" << std::endl;
      REQUIRE(false);
    }

    std::optional<ExceptionFailure> bindResult;
    REQUIRE_NOTHROW(bindResult =
                        (feedS >>
                         rbind<string, int, ExceptionFailure, ExceptionFailure,
                               std::out_of_range>(full) >>
                         terminateF)(checkThis));

    REQUIRE(static_cast<bool>(bindResult));

    std::out_of_range verifyWithThis("Failure mode");
    CHECK(ExceptionFailure(verifyWithThis) == *(bindResult));
  }));

  {
    std::out_of_range checkThis("Failure mode");
    ExceptionFailure normalResult =
        full(Failure<int, ExceptionFailure>(ExceptionFailure{checkThis}))
            .getFailure();

    std::optional<ExceptionFailure> bindResult;
    REQUIRE_NOTHROW(bindResult =
                        (feedF >> rbind(full) >> terminateF)(checkThis));
    REQUIRE(static_cast<bool>(bindResult));
    CHECK(normalResult == *(bindResult));
  };
}

}  // namespace TwoTwo

// 1:1
namespace OneOne {}  // namespace OneOne

// 1:2
namespace OneTwo {}  // namespace OneTwo

// 2:1
namespace TwoOne {}  // namespace TwoOne

}  // namespace
