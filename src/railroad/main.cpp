/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include <sstream>
#include <string>

#include "railroad/FailurePipe.h"
#include "railroad/Result.h"
#include "railroad/SuccessPipe.h"
#include "railroad/ValidateSuccess.h"
#include "railroad/abc.h"
#include "railroad/bind.h"
#include "railroad/terminate.h"

class Increment : public ::railroad::abc::Callable1<int, int> {
 public:
  int operator()(const int& input) const override { return input + 1; }
};

std::ostream& operator<<(std::ostream& out, const Increment& /* inc */) {
  out << "Increment__function(int) -> int + 1";
  return out;
}

class StringWriter
    : public ::railroad::abc::Callable1<std::string, std::string> {
 public:
  std::string operator()(const std::string& input) const override {
    if (input.size() > 0) {
      return "Hello World";
    }
    return input;
  }
};

class RangeCheck
    : public ::railroad::abc::Callable1<::railroad::Result<int, std::string>,
                                        int> {
  using OutputResult = ::railroad::Result<int, std::string>;

 public:
  OutputResult operator()(const int& input) const override {
    if (0 <= input && input <= 1) {
      return OutputResult::Success(input);
    } else {
      std::stringstream buf;
      buf << "Integer Input Out of Range: " << input << " not in [0, 1]";
      return OutputResult::Failure(buf.str());
    }
  }
};

int main(int /* argc */, char** /* argv */) {
  using ::railroad::Result;
  using InputResult = Result<int, std::string>;

  int rawInput = 0;

  Increment successOnlyFunction;
  ::railroad::bind::SuccessPipe railroadIncrementFunction{successOnlyFunction};

  RangeCheck validateAndSplit;
  ::railroad::bind::ValidateSuccess railroadFromSplit(validateAndSplit);

  // clang-format off
  int fancyComposedResult =
      (railroadIncrementFunction
        >> railroadFromSplit
        >> railroadIncrementFunction
        >> railroadIncrementFunction)(InputResult::Success(rawInput)).getSuccess().unpack();
  // clang-format on
  std::cout << "Got result " << fancyComposedResult
            << " via syntax composition." << std::endl;

  return 0;
}
