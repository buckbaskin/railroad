/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include <string>

#include "railroad/Result.h"
#include "railroad/abc.h"
#include "railroad/bind.h"

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

int main(int /* argc */, char** /* argv */) {
  using ::railroad::Result;
  using InputResult = Result<int, std::string>;
  using OutputResult = Result<int, std::string>;

  int rawInput = 0;
  std::string strInput = "";

  Increment successOnlyFunction;
  int directOutput = successOnlyFunction(successOnlyFunction(rawInput));

  StringWriter failureOnlyFunction;
  std::string directFailText = failureOnlyFunction(strInput);

  InputResult input = InputResult(rawInput, strInput);

  ::railroad::bind::SuccessPipe railroadIncrementFunction{successOnlyFunction};
  ::railroad::bind::FailurePipe<int, int, std::string, std::string>
      railroadStringFunction{failureOnlyFunction};

  OutputResult output = railroadStringFunction(
      railroadIncrementFunction(railroadIncrementFunction(input)));

  int railroadOutput = output.getSuccess().unpack();
  std::string railroadFailText = output.getFailure().unpack();

  if (directOutput == railroadOutput) {
    std::cout << "Successfully bound " << successOnlyFunction << " to get "
              << railroadOutput << " from " << rawInput << std::endl;
  } else {
    std::cout << "Binding failed for " << successOnlyFunction << " to get "
              << railroadOutput << " from " << rawInput << std::endl;
  }

  std::cout << "Direct fail text: >" << directFailText << "< vs railroad: >"
            << railroadFailText << "<" << std::endl;

  return 0;
}
