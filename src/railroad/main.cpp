/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include <sstream>
#include <string>

#include "railroad/GenericCompose.h"
#include "railroad/Result.h"
#include "railroad/abc.h"
#include "railroad/bind.h"

class Increment : public ::railroad::abc::Callable1<int, int> {
 public:
  int operator()(const int& input) const override { return input + 1; }
};

// class Increment : public ::railroad::abc::Callable1<int, int> {
//  public:
//   int operator()(const int& input) const override { return input + 1; }
// };

std::ostream& operator<<(std::ostream& out, const Increment& /* inc */) {
  out << "Increment__function(int) -> int + 1";
  return out;
}

class Stringify : public ::railroad::abc::Callable1<std::string, int> {
 public:
  std::string operator()(const int& input) const override {
    std::stringstream buf;
    buf << "String Version: " << input;
    return buf.str();
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

using namespace ::railroad::bind::generic;

int main(int /* argc */, char** /* argv */) {
  using InputResult = ::railroad::Result<int, ::railroad::DefaultFailure>;
  using OutputResult =
      ::railroad::Result<std::string, ::railroad::DefaultFailure>;

  int rawInput = 0;

  Increment intInc;
  Stringify intToStr;
  RangeCheck validateAndSplit;

  auto boundIntInc = ::railroad::binds(intInc);
  auto boundIntToStr = ::railroad::binds(intToStr);

  std::cout << "Completed Individual Binding" << std::endl;

  auto intermediate = (boundIntInc >> boundIntToStr);

  // clang-format off
  auto fancyComposedResult = (
    intermediate)(InputResult::Success(rawInput));
  // clang-format on

  std::cout << "Printing Result" << std::endl;
  std::cout << "Got result " << fancyComposedResult
            << " via syntax composition." << std::endl;

  return 0;
}
